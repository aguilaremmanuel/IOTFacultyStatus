# ESP32 Setup Guide for SFSI System

## Hardware Required
- ESP32 Development Board
- TFT LCD 3.2" Touch Shield (ILI9341 driver)
- Jumper wires
- USB cable for programming

## Software Required
1. **Arduino IDE** - Download from https://www.arduino.cc/en/software
2. **ESP32 Board Support**
3. **Required Libraries**

---

## Step 1: Install Arduino IDE

1. Download Arduino IDE from https://www.arduino.cc/en/software
2. Install and open Arduino IDE

---

## Step 2: Add ESP32 Board Support

1. Open Arduino IDE
2. Go to **File → Preferences**
3. In "Additional Board Manager URLs", add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Click OK
5. Go to **Tools → Board → Boards Manager**
6. Search for "ESP32"
7. Install "ESP32 by Espressif Systems"

---

## Step 3: Install Required Libraries

Go to **Sketch → Include Library → Manage Libraries**, then search and install:

| Library | Author | Purpose |
|---------|--------|---------|
| **TFT_eSPI** | Bodmer | TFT LCD display driver |
| **ArduinoJson** | Benoit Blanchon | Parse JSON from API |

---

## Step 4: Configure TFT_eSPI Library

This is important! You need to configure the library for your display.

1. Find the library folder:
   - Windows: `Documents/Arduino/libraries/TFT_eSPI/`
   - Mac: `~/Documents/Arduino/libraries/TFT_eSPI/`

2. Open `User_Setup.h` file

3. Make these changes:

```cpp
// Uncomment ONE driver (comment out others):
#define ILI9341_DRIVER

// Define the pins for ESP32:
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST   4
```

---

## Step 5: Configure the Code

Open `sfsi_display.ino` and change these values:

```cpp
// Your WiFi credentials
const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Your computer's IP address
const char* SERVER_IP = "192.168.1.100";  // Change this!
const int SERVER_PORT = 8080;
```

### How to Find Your Computer's IP Address:

**Windows:**
1. Open Command Prompt
2. Type `ipconfig`
3. Look for "IPv4 Address" under your WiFi adapter

**Mac:**
1. Open Terminal
2. Type `ifconfig | grep "inet "`
3. Or go to System Preferences → Network → WiFi → Advanced → TCP/IP

---

## Step 6: Wiring Diagram

Connect the TFT LCD to ESP32:

| TFT LCD Pin | ESP32 Pin |
|-------------|-----------|
| VCC | 3.3V |
| GND | GND |
| CS | GPIO 15 |
| RESET | GPIO 4 |
| DC | GPIO 2 |
| SDI (MOSI) | GPIO 23 |
| SCK | GPIO 18 |
| LED | 3.3V |
| SDO (MISO) | GPIO 19 |

---

## Step 7: Upload the Code

1. Connect ESP32 to computer via USB
2. In Arduino IDE, go to **Tools → Board** and select "ESP32 Dev Module"
3. Go to **Tools → Port** and select the correct COM port
4. Click the **Upload** button (→)

---

## Step 8: Run the Website

Make sure the Django website is running on your computer:

```bash
cd /Users/emmanaguilar/Documents/GitHub/IOT_WEBSITE
source venv/bin/activate
python manage.py runserver 0.0.0.0:8080
```

The `0.0.0.0` is important - it allows the ESP32 to connect to your computer.

---

## Troubleshooting

### ESP32 can't connect to WiFi
- Check WiFi name and password are correct
- Make sure ESP32 is within WiFi range
- Note: ESP32 only supports 2.4GHz WiFi, not 5GHz

### ESP32 can't reach the server
- Make sure both devices are on the same WiFi network
- Check the IP address is correct
- Make sure Django is running with `0.0.0.0:8080`
- Check your firewall isn't blocking port 8080

### Display shows nothing
- Check wiring connections
- Verify TFT_eSPI library is configured correctly
- Try adjusting the TFT_RST pin in code

### "JSON Error" on display
- Check the API URL is correct
- Test API in browser: `http://YOUR_IP:8080/api/status/`

---

## API Endpoints

| Endpoint | Description |
|----------|-------------|
| `/api/status/` | Get all teachers' status |
| `/api/status/1/` | Get specific teacher (by ID) |

Example response:
```json
{
  "teachers": [
    {
      "id": 1,
      "name": "John Smith",
      "department": "Computer Science",
      "status": "available",
      "status_display": "Available",
      "color": "#28a745"
    }
  ]
}
```
