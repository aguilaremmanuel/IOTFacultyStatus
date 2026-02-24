/*
 * Smart Faculty Status Indicator System (SFSI)
 * ESP32 + TFT LCD Display Code
 *
 * This code fetches teacher status from the web server
 * and displays it on the TFT LCD screen with department filtering
 * and pagination support.
 *
 * Hardware:
 * - ESP32 Development Board
 * - TFT LCD 3.2" Touch Shield (ILI9341)
 *
 * Libraries Required:
 * - TFT_eSPI (for display)
 * - ArduinoJson (for parsing API response)
 * - WiFi (built-in)
 * - HTTPClient (built-in)
 */

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>

// ============== CONFIGURATION ==============
// WiFi Settings - CHANGE THESE!
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Server Settings
const char* SERVER_URL = "https://smartfacultystatus.pythonanywhere.com/api/status/";

// Refresh interval (in milliseconds)
const unsigned long REFRESH_INTERVAL = 5000;  // 5 seconds

// ============== DISPLAY SETUP ==============
TFT_eSPI tft = TFT_eSPI();

// Colors (RGB565 format)
#define COLOR_BG            0x1082  // Dark blue background
#define COLOR_WHITE         0xFFFF
#define COLOR_GREEN         0x07E0  // Available
#define COLOR_YELLOW        0xFFE0  // Busy
#define COLOR_RED           0xF800  // Not Present
#define COLOR_GRAY          0x7BEF
#define COLOR_HEADER        0x2945  // Dark header
#define COLOR_BTN_NORMAL    0x3186  // Normal button color
#define COLOR_BTN_SELECTED  0x04FF  // Highlighted/selected button (cyan)
#define COLOR_BTN_TEXT      0xFFFF  // Button text color
#define COLOR_BTN_DISABLED  0x2104  // Disabled button color

// ============== DEPARTMENT BUTTONS ==============
#define MAX_DEPARTMENTS 6
#define BTN_HEIGHT 25
#define BTN_MARGIN 5
#define BTN_Y_START 45

struct DepartmentButton {
    String name;
    String code;
    int x;
    int width;
    bool selected;
};

DepartmentButton deptButtons[MAX_DEPARTMENTS];
int numDepartments = 0;
int selectedDeptIndex = 0;  // 0 = "All"

// ============== PAGINATION ==============
#define ITEMS_PER_PAGE 3
#define PAGE_BTN_WIDTH 50
#define PAGE_BTN_HEIGHT 25
#define PAGE_NAV_Y (tft.height() - 55)  // Position above legend

int currentPage = 0;
int totalPages = 1;
int totalFilteredTeachers = 0;

// Page button positions
int prevBtnX = 20;
int nextBtnX = 0;  // Calculated in setup

// ============== VARIABLES ==============
unsigned long lastRefresh = 0;
bool wifiConnected = false;
String cachedPayload = "";  // Cache the JSON response for filtering

// Touch calibration values (adjust for your display)
#define TOUCH_MIN_X 300
#define TOUCH_MAX_X 3800
#define TOUCH_MIN_Y 300
#define TOUCH_MAX_Y 3800

void setup() {
    Serial.begin(115200);
    Serial.println("\n=== Smart Faculty Status Indicator ===");

    // Initialize TFT display
    tft.init();
    tft.setRotation(1);  // Landscape mode
    tft.fillScreen(COLOR_BG);

    // Calculate next button position
    nextBtnX = tft.width() - PAGE_BTN_WIDTH - 20;

    // Initialize touch
    uint16_t calData[5] = {275, 3620, 264, 3532, 1};
    tft.setTouch(calData);

    // Initialize default "All" button
    initializeDefaultButtons();

    // Show startup screen
    showStartupScreen();

    // Connect to WiFi
    connectToWiFi();
}

void loop() {
    // Check WiFi connection
    if (WiFi.status() != WL_CONNECTED) {
        wifiConnected = false;
        showError("WiFi Disconnected");
        connectToWiFi();
        return;
    }

    // Handle touch input
    handleTouch();

    // Refresh data at intervals
    if (millis() - lastRefresh >= REFRESH_INTERVAL) {
        fetchAndDisplayStatus();
        lastRefresh = millis();
    }
}

// ============== BUTTON FUNCTIONS ==============
void initializeDefaultButtons() {
    // Start with just "All" button, others will be added from API
    deptButtons[0].name = "All";
    deptButtons[0].code = "ALL";
    deptButtons[0].selected = true;
    numDepartments = 1;
}

void updateDepartmentButtons(JsonArray& teachers) {
    // Reset buttons, keep "All" as first
    numDepartments = 1;
    deptButtons[0].name = "All";
    deptButtons[0].code = "ALL";

    // Extract unique departments from teachers
    for (JsonObject teacher : teachers) {
        const char* dept = teacher["department"];
        String deptStr = String(dept);

        // Check if department already exists
        bool exists = false;
        for (int i = 0; i < numDepartments; i++) {
            if (deptButtons[i].name == deptStr) {
                exists = true;
                break;
            }
        }

        // Add new department if not exists and we have room
        if (!exists && numDepartments < MAX_DEPARTMENTS) {
            deptButtons[numDepartments].name = deptStr;
            deptButtons[numDepartments].code = getDeptCode(deptStr);
            deptButtons[numDepartments].selected = false;
            numDepartments++;
        }
    }

    // Calculate button positions
    calculateButtonPositions();
}

String getDeptCode(String fullName) {
    // Convert full department name to short code
    if (fullName.indexOf("Computer Science") >= 0) return "BSCS";
    if (fullName.indexOf("Information Tech") >= 0) return "BSIT";
    if (fullName.indexOf("Electronics") >= 0) return "BSECE";
    if (fullName.indexOf("Computer Eng") >= 0) return "BSCpE";
    if (fullName.indexOf("Electrical") >= 0) return "BSEE";

    // Default: take first 4 characters
    if (fullName.length() > 4) {
        return fullName.substring(0, 4);
    }
    return fullName;
}

void calculateButtonPositions() {
    int totalWidth = tft.width() - (BTN_MARGIN * 2);
    int btnWidth = (totalWidth - (BTN_MARGIN * (numDepartments - 1))) / numDepartments;

    // Minimum button width
    if (btnWidth < 45) btnWidth = 45;

    int x = BTN_MARGIN;
    for (int i = 0; i < numDepartments; i++) {
        deptButtons[i].x = x;
        deptButtons[i].width = btnWidth;
        x += btnWidth + BTN_MARGIN;
    }
}

void drawDepartmentButtons() {
    for (int i = 0; i < numDepartments; i++) {
        uint16_t btnColor = (i == selectedDeptIndex) ? COLOR_BTN_SELECTED : COLOR_BTN_NORMAL;
        uint16_t textColor = COLOR_BTN_TEXT;

        // Draw button background with rounded corners
        tft.fillRoundRect(deptButtons[i].x, BTN_Y_START,
                          deptButtons[i].width, BTN_HEIGHT, 5, btnColor);

        // Draw button border for selected button
        if (i == selectedDeptIndex) {
            tft.drawRoundRect(deptButtons[i].x, BTN_Y_START,
                              deptButtons[i].width, BTN_HEIGHT, 5, COLOR_WHITE);
        }

        // Draw button text (centered)
        tft.setTextColor(textColor, btnColor);
        tft.setTextSize(1);

        String displayText = (i == 0) ? "All" : deptButtons[i].code;
        int textWidth = displayText.length() * 6;  // Approximate width
        int textX = deptButtons[i].x + (deptButtons[i].width - textWidth) / 2;
        int textY = BTN_Y_START + (BTN_HEIGHT - 8) / 2;

        tft.setCursor(textX, textY);
        tft.println(displayText);
    }
}

// ============== PAGINATION FUNCTIONS ==============
void drawPaginationControls() {
    // Clear pagination area
    tft.fillRect(0, PAGE_NAV_Y, tft.width(), PAGE_BTN_HEIGHT + 5, COLOR_BG);

    // Draw Previous button
    bool canGoPrev = (currentPage > 0);
    uint16_t prevColor = canGoPrev ? COLOR_BTN_NORMAL : COLOR_BTN_DISABLED;
    uint16_t prevTextColor = canGoPrev ? COLOR_WHITE : COLOR_GRAY;

    tft.fillRoundRect(prevBtnX, PAGE_NAV_Y, PAGE_BTN_WIDTH, PAGE_BTN_HEIGHT, 5, prevColor);
    if (canGoPrev) {
        tft.drawRoundRect(prevBtnX, PAGE_NAV_Y, PAGE_BTN_WIDTH, PAGE_BTN_HEIGHT, 5, COLOR_GRAY);
    }
    tft.setTextColor(prevTextColor, prevColor);
    tft.setTextSize(2);
    tft.setCursor(prevBtnX + 18, PAGE_NAV_Y + 5);
    tft.print("<");

    // Draw page indicator
    tft.setTextColor(COLOR_WHITE, COLOR_BG);
    tft.setTextSize(1);
    String pageText = "Page " + String(currentPage + 1) + " of " + String(totalPages);
    int pageTextWidth = pageText.length() * 6;
    int pageTextX = (tft.width() - pageTextWidth) / 2;
    tft.setCursor(pageTextX, PAGE_NAV_Y + 8);
    tft.print(pageText);

    // Draw Next button
    bool canGoNext = (currentPage < totalPages - 1);
    uint16_t nextColor = canGoNext ? COLOR_BTN_NORMAL : COLOR_BTN_DISABLED;
    uint16_t nextTextColor = canGoNext ? COLOR_WHITE : COLOR_GRAY;

    tft.fillRoundRect(nextBtnX, PAGE_NAV_Y, PAGE_BTN_WIDTH, PAGE_BTN_HEIGHT, 5, nextColor);
    if (canGoNext) {
        tft.drawRoundRect(nextBtnX, PAGE_NAV_Y, PAGE_BTN_WIDTH, PAGE_BTN_HEIGHT, 5, COLOR_GRAY);
    }
    tft.setTextColor(nextTextColor, nextColor);
    tft.setTextSize(2);
    tft.setCursor(nextBtnX + 18, PAGE_NAV_Y + 5);
    tft.print(">");
}

int countFilteredTeachers(JsonArray& teachers) {
    String filterDept = "";
    if (selectedDeptIndex > 0) {
        filterDept = deptButtons[selectedDeptIndex].name;
    }

    int count = 0;
    for (JsonObject teacher : teachers) {
        const char* department = teacher["department"];
        if (filterDept.length() == 0 || String(department) == filterDept) {
            count++;
        }
    }
    return count;
}

void calculatePagination(JsonArray& teachers) {
    totalFilteredTeachers = countFilteredTeachers(teachers);
    totalPages = (totalFilteredTeachers + ITEMS_PER_PAGE - 1) / ITEMS_PER_PAGE;
    if (totalPages < 1) totalPages = 1;

    // Reset to first page if current page is out of bounds
    if (currentPage >= totalPages) {
        currentPage = 0;
    }
}

// ============== TOUCH HANDLING ==============
void handleTouch() {
    uint16_t touchX, touchY;

    if (tft.getTouch(&touchX, &touchY)) {
        // Map touch coordinates to screen coordinates
        int screenX = touchX;
        int screenY = touchY;

        // Check if touch is in department button area
        if (screenY >= BTN_Y_START && screenY <= BTN_Y_START + BTN_HEIGHT) {
            for (int i = 0; i < numDepartments; i++) {
                if (screenX >= deptButtons[i].x &&
                    screenX <= deptButtons[i].x + deptButtons[i].width) {

                    // Department button pressed!
                    if (selectedDeptIndex != i) {
                        selectedDeptIndex = i;
                        currentPage = 0;  // Reset to first page on filter change

                        // Update selected state
                        for (int j = 0; j < numDepartments; j++) {
                            deptButtons[j].selected = (j == i);
                        }

                        // Redraw with filter
                        redrawWithFilter();

                        // Debounce delay
                        delay(200);
                    }
                    break;
                }
            }
        }

        // Check if touch is in pagination area
        if (screenY >= PAGE_NAV_Y && screenY <= PAGE_NAV_Y + PAGE_BTN_HEIGHT) {
            // Previous button
            if (screenX >= prevBtnX && screenX <= prevBtnX + PAGE_BTN_WIDTH) {
                if (currentPage > 0) {
                    currentPage--;
                    redrawWithFilter();
                    delay(200);  // Debounce
                }
            }
            // Next button
            else if (screenX >= nextBtnX && screenX <= nextBtnX + PAGE_BTN_WIDTH) {
                if (currentPage < totalPages - 1) {
                    currentPage++;
                    redrawWithFilter();
                    delay(200);  // Debounce
                }
            }
        }
    }
}

void redrawWithFilter() {
    if (cachedPayload.length() > 0) {
        DynamicJsonDocument doc(4096);
        DeserializationError error = deserializeJson(doc, cachedPayload);
        if (!error) {
            displayTeacherStatus(doc);
        }
    }
}

// ============== WiFi FUNCTIONS ==============
void connectToWiFi() {
    tft.fillScreen(COLOR_BG);
    tft.setTextColor(COLOR_WHITE, COLOR_BG);
    tft.setTextSize(2);
    tft.setCursor(20, 100);
    tft.println("Connecting to WiFi...");
    tft.setCursor(20, 130);
    tft.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        tft.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        wifiConnected = true;
        Serial.println("\nWiFi Connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());

        tft.fillScreen(COLOR_BG);
        tft.setCursor(20, 100);
        tft.setTextColor(COLOR_GREEN, COLOR_BG);
        tft.println("WiFi Connected!");
        tft.setCursor(20, 130);
        tft.setTextColor(COLOR_WHITE, COLOR_BG);
        tft.print("IP: ");
        tft.println(WiFi.localIP());
        delay(2000);

        // Fetch initial data
        fetchAndDisplayStatus();
        lastRefresh = millis();
    } else {
        showError("WiFi Failed!");
        delay(5000);
    }
}

// ============== API FUNCTIONS ==============
void fetchAndDisplayStatus() {
    WiFiClientSecure client;
    HTTPClient http;

    // Skip certificate verification (for simplicity)
    client.setInsecure();

    Serial.println("Fetching: " + String(SERVER_URL));

    http.begin(client, SERVER_URL);
    http.setTimeout(10000);  // 10 second timeout

    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
        cachedPayload = http.getString();
        Serial.println("Response received");

        // Parse JSON
        DynamicJsonDocument doc(4096);
        DeserializationError error = deserializeJson(doc, cachedPayload);

        if (error) {
            Serial.print("JSON Parse Error: ");
            Serial.println(error.c_str());
            showError("JSON Error");
        } else {
            // Update department buttons from data
            JsonArray teachers = doc["teachers"];
            updateDepartmentButtons(teachers);

            displayTeacherStatus(doc);
        }
    } else {
        Serial.print("HTTP Error: ");
        Serial.println(httpCode);
        showError("Server Error: " + String(httpCode));
    }

    http.end();
}

// ============== DISPLAY FUNCTIONS ==============
void showStartupScreen() {
    tft.fillScreen(COLOR_BG);
    tft.setTextColor(COLOR_WHITE, COLOR_BG);
    tft.setTextSize(3);
    tft.setCursor(50, 80);
    tft.println("SFSI System");
    tft.setTextSize(2);
    tft.setCursor(30, 130);
    tft.println("Faculty Status Indicator");
    tft.setTextSize(1);
    tft.setCursor(60, 180);
    tft.setTextColor(COLOR_GRAY, COLOR_BG);
    tft.println("Emilio Aguinaldo College Cavite");
    delay(2000);
}

void displayTeacherStatus(DynamicJsonDocument& doc) {
    tft.fillScreen(COLOR_BG);

    // Draw header
    tft.fillRect(0, 0, tft.width(), 40, COLOR_HEADER);
    tft.setTextColor(COLOR_WHITE, COLOR_HEADER);
    tft.setTextSize(2);
    tft.setCursor(10, 12);
    tft.println("Faculty Status");

    // Draw timestamp
    tft.setTextSize(1);
    tft.setCursor(tft.width() - 80, 15);
    tft.println("Live");

    // Draw department filter buttons
    drawDepartmentButtons();

    // Get selected department filter
    String filterDept = "";
    if (selectedDeptIndex > 0) {
        filterDept = deptButtons[selectedDeptIndex].name;
    }

    // Get teachers array and calculate pagination
    JsonArray teachers = doc["teachers"];
    calculatePagination(teachers);

    // Draw teacher list with pagination
    int y = BTN_Y_START + BTN_HEIGHT + 10;  // Start below buttons
    int cardHeight = 42;
    int displayCount = 0;
    int skipCount = currentPage * ITEMS_PER_PAGE;
    int skipped = 0;

    for (JsonObject teacher : teachers) {
        if (displayCount >= ITEMS_PER_PAGE) break;

        const char* name = teacher["name"];
        const char* status = teacher["status"];
        const char* statusDisplay = teacher["status_display"];
        const char* department = teacher["department"];

        // Apply department filter
        if (filterDept.length() > 0 && String(department) != filterDept) {
            continue;  // Skip if doesn't match filter
        }

        // Skip items from previous pages
        if (skipped < skipCount) {
            skipped++;
            continue;
        }

        // Draw card background
        uint16_t cardColor = 0x2104;  // Dark gray
        tft.fillRoundRect(5, y, tft.width() - 10, cardHeight - 5, 5, cardColor);

        // Draw status indicator circle
        uint16_t statusColor = getStatusColor(status);
        tft.fillCircle(25, y + 18, 10, statusColor);

        // Draw teacher name
        tft.setTextColor(COLOR_WHITE, cardColor);
        tft.setTextSize(2);
        tft.setCursor(45, y + 3);

        // Truncate name if too long
        String nameStr = String(name);
        if (nameStr.length() > 14) {
            nameStr = nameStr.substring(0, 12) + "..";
        }
        tft.println(nameStr);

        // Draw department
        tft.setTextSize(1);
        tft.setTextColor(COLOR_GRAY, cardColor);
        tft.setCursor(45, y + 25);
        tft.println(department);

        // Draw status text
        tft.setTextColor(statusColor, cardColor);
        tft.setCursor(tft.width() - 85, y + 13);
        tft.println(statusDisplay);

        y += cardHeight;
        displayCount++;
    }

    // Show message if no teachers match filter
    if (displayCount == 0) {
        tft.setTextColor(COLOR_GRAY, COLOR_BG);
        tft.setTextSize(2);
        tft.setCursor(40, 120);
        tft.println("No faculty found");
    }

    // Draw pagination controls
    drawPaginationControls();

    // Draw legend at bottom
    drawLegend();
}

uint16_t getStatusColor(const char* status) {
    if (strcmp(status, "available") == 0) return COLOR_GREEN;
    if (strcmp(status, "busy") == 0) return COLOR_YELLOW;
    if (strcmp(status, "not_present") == 0) return COLOR_RED;
    return COLOR_GRAY;
}

void drawLegend() {
    int y = tft.height() - 20;
    tft.setTextSize(1);
    tft.setTextColor(COLOR_WHITE, COLOR_BG);

    // Available
    tft.fillCircle(30, y, 5, COLOR_GREEN);
    tft.setCursor(40, y - 4);
    tft.println("Avail");

    // Busy
    tft.fillCircle(95, y, 5, COLOR_YELLOW);
    tft.setCursor(105, y - 4);
    tft.println("Busy");

    // Not Present
    tft.fillCircle(160, y, 5, COLOR_RED);
    tft.setCursor(170, y - 4);
    tft.println("Not Present");
}

void showError(String message) {
    tft.fillScreen(COLOR_BG);
    tft.setTextColor(COLOR_RED, COLOR_BG);
    tft.setTextSize(2);
    tft.setCursor(20, 100);
    tft.println("ERROR:");
    tft.setCursor(20, 130);
    tft.setTextColor(COLOR_WHITE, COLOR_BG);
    tft.println(message);
}
