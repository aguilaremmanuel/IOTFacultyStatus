____________________________________________________________
DESIGN OF A SMART FACULTY STATUS INDICATOR SYSTEM PROTOTYPE FOR REAL-TIME FACULTY AVAILABILITY DISPLAY

INTRODUCTION 

Background of the Study

	Effective communication between students and teachers is essential for academic success, as it supports student engagement, motivation, and a clear understanding of academic expectations. Research shows that perceived teacher support and meaningful interaction are strongly related to higher levels of student engagement, which contribute to improved learning outcomes across different educational settings (Prananto et al., 2025). The use of information and communication technologies (ICT) in education has further improved student–teacher communication by allowing faster and more flexible interaction beyond the classroom. Tools such as email, online platforms, and learning management systems (LMS) are commonly used to share academic information, explain learning materials, and provide support outside regular class hours, thereby improving access to teachers and communication efficiency (Batista et al., 2021).
	Despite these advantages, many existing communication systems are mainly asynchronous and do not offer real-time information about teacher availability. This limitation can cause uncertainty and delays when students need immediate academic assistance. Studies on online and technology-supported learning environments emphasize that teacher presence and timely communication are essential for effective learning; however, the lack of real-time interaction remains a challenge (Najjar, 2025). In practice, students often depend on traditional approaches, such as visiting teachers during free periods, checking posted schedules, or sending messages that may not receive prompt responses. At the same time, research indicates that teachers generally have positive views toward ICT-based communication, especially when systems are easy to use and supported by adequate technological resources (Akram, 2022). These findings suggest the need for systems that clearly indicate teacher presence and availability.
	Although many studies highlight the importance of ICT in educational communication, most existing systems focus on online or platform-based interaction. This focus reveals a gap in localized, real-time communication solutions within physical school environments, particularly faculty rooms. Common tools such as LMS and messaging platforms do not provide visible and immediate indicators of teacher availability, which can lead to confusion for both students and teachers. While some studies have introduced LCD-based and web-based information systems (Raut et al., 2021; Suryawanshi Rutuja et al., 2025; Syafariani & Devi, 2019), these solutions mainly serve as announcement tools and do not address real-time faculty presence.
	To address this gap, the Smart Faculty Status Indicator System (SFSI) prototype is proposed to display real-time status information, such as Available, Busy, or Not Present, outside faculty offices. This system can help students use their time more effectively and support a more organized academic environment. Furthermore, limited research has focused on low-cost, prototype-based systems that combine web-based status updates with physical, wall-mounted displays to support non-intrusive student–teacher communication. As a result, a gap remains in the development of practical solutions that provide real-time visibility of faculty availability in on-site educational settings. In response to this need, the present study aims to design the Smart Faculty Status Indicator to improve real-time communication and strengthen student–teacher interaction within faculty room environments.


Statement of the Problem/Objective of the Study
	This study aims to design and test a Smart Faculty Status Indicator System (SFSI) prototype that provides real-time updates on faculty availability, helping students, teachers, and administrators manage schedules more efficiently, reduce interruptions, and improve overall coordination within the academic environment.
	Specifically, this study aims to answer the following research questions:
What is the response time between status updates on the website and the information displayed on the TFT LCD screen?
How accurately does the system reflect faculty status (Available, Busy, or Not Present) when updated through the web application?
What is the overall performance of the Smart Faculty Status Indicator prototype in displaying real-time faculty availability information?

Hypothesis of the Study
	To determine whether the Smart Faculty Status Indicator System accurately displays faculty availability, the following hypotheses are formulated:
Null Hypothesis (H₀): The web-based application does not accurately reflect the teacher’s real-time availability on the TFT LCD screen.
Alternative Hypothesis (H₁): The web-based application accurately reflects the teacher’s real-time availability on the TFT LCD screen.

Significance of the Study
	This study aims to design and test the Smart Faculty Status Indicator System (SFSI) prototype that displays real-time faculty availability. By improving the way faculty availability information is communicated, the system seeks to reduce unnecessary interruptions, improve time management, and enhance coordination within the academic environment. The findings of this study will provide valuable insights into the usefulness and performance of the proposed system and its potential application in educational institutions. 
	Specifically, this study aims to benefit the following groups:
Students. This study will help students by determining how effectively the Smart Faculty Status Indicator System prototype provides clear and timely information about faculty availability. The system may help students avoid unnecessary interruptions, reduce waiting time, and manage their schedules more efficiently.
Teachers. This study will benefit teachers by evaluating how the SFSI prototype allows them to control and update their availability status. The system may help minimize unwanted disturbances and support a more organized and productive working environment.
School Administrators. This study will assist school administrators by demonstrating how the SFSI prototype can function as a low-cost and efficient communication tool. The results may support better coordination, faculty management, and improvements in school operations.
Academic Institutions. This study will benefit academic institutions by presenting the potential of the Smart Faculty Status Indicator System as a communication and management tool. The findings may serve as a basis for future adoption, enhancement, or integration of similar systems.
Future Researchers. This study will serve as a reference for future researchers by providing data and insights related to the design, development, and testing of the Smart Faculty Status Indicator System prototype. The results may contribute to further studies, system improvements, or related technological innovations in educational settings.

Definition of Terms
	This section presents the operational definitions of key terms used in the study. These terms are defined based on how they are applied and measured within the context of the Smart Faculty Status Indicator System (SFSI) to ensure clarity and consistency throughout the research.
Smart Faculty Status Indicator System (SFSI). A prototype system was developed to display real-time faculty availability through a web-based application and a TFT LCD screen, enabling students to view teachers’ status without direct interruption.
Web-Based Application. An online system accessed through a web browser that allows users to input, update, and retrieve information over the internet.
System Accuracy. The degree to which the faculty status displayed on the TFT LCD screen correctly corresponds to the status entered by the teacher through the web-based application reflects the quality and reliability of the system’s information output.
Prototype. An initial working model created to evaluate the functionality, performance, and feasibility of a system before full-scale implementation.
ESP32 Microcontroller. A low-cost microcontroller with built-in Wi-Fi capability used in this study to receive data from the web-based application and control the real-time display of faculty availability on the TFT LCD screen.

Scope and Delimitation 
	This study focuses on the design and prototype testing of the Smart Faculty Status Indicator (SFSI) System, intended to display real-time faculty availability. The variables considered include system functionality, accuracy, response time, reliability, and usability of the prototype. The study encompasses the design and development of a web-based application that enables faculty members to update their status as Available, Busy, or Not Present, as well as a hardware-based display utilizing a TFT LCD screen that displays the updated status in real-time. Prototype testing will determine whether status updates are correctly reflected on the display, measure the response time between input and output, and observe the system’s basic reliability during continuous operation. The population involved includes selected faculty members and students of Emilio Aguinaldo College Cavite, who will participate in usability testing. The study will be conducted during the second semester of Academic Year 2025 to 2026 at Emilio Aguinaldo College Cavite, and is limited to the design, development, and testing of a functional prototype and does not include full system deployment, large-scale implementation, and long-term operational evaluation.
Review/Survey of Related Literature
The literature and studies presented below will further explain additional aspects of this study.


ICT Availability, Functionality, and Teachers’ Digital Skills
The availability and functionality of information and communication technology (ICT) tools are important factors in supporting the use of digital systems in Philippine schools. Sumile and Lozano (2020)  investigated ICT tools and teachers’ digital literacy skills in high schools. Their study revealed high availability but moderate functionality of ICT tools, while teachers’ digital literacy skills were generally high. The study emphasized the importance of proper maintenance of ICT tools and continuous development of teachers’ digital skills to support effective technology use. This is relevant to the present study because the Smart Faculty Status Indicator System (SFSI) requires functional ICT infrastructure, such as stable devices and connectivity, to display accurate faculty availability. The study supports the need to consider existing ICT conditions when implementing educational technology systems.

Smart and Web-Based Attendance Monitoring Systems
Local studies demonstrate that automated and web-based systems can improve efficiency in academic monitoring. Alcantara et al. (2022) developed a smart attendance checking system using Bluetooth Low Energy (BLE) technology, mobile applications, and a web server. Their study highlighted that such systems can automate attendance processes and support monitoring in educational settings. Similarly, Amora et al. (2020) designed a web-based attendance monitoring system that utilized wireless technologies and a centralized portal to automate attendance recording and display information such as student records and announcements. Their study emphasized that web-based systems improve accessibility to academic information and support efficient monitoring. While these systems focus on attendance tracking, they do not specifically display real-time faculty availability. The Smart Faculty Status Indicator System (SFSI) applies these concepts by integrating automated and web-based approaches to indicate faculty presence in real time, allowing students to see availability without interrupting teachers.


IoT-Based Solutions in Education
According to Muzayanah et al. (2024), the Internet of Things (IoT) is a network of interconnected devices that enables the collection, exchange, and processing of information between users. One practical application of IoT in education is the computerized recording of attendance. Jayram et al. (2025) developed an IoT-based Smart Digital Notice Board that allows wireless and remote updating of information through web or mobile devices, enabling announcements to be displayed instantly on electronic screens within educational institutions. Similarly, Sheikh et al. (2024) created an IoT-based Smart Scheduler and Faculty Notification System designed to automate academic scheduling and deliver real-time notifications to faculty members regarding updates and changes. Both studies highlight the effectiveness of IoT in disseminating automated information and improving communication in academic environments.

	However, while these systems update information and notifications, they do not specifically address the real-time presence or availability of faculty members within school spaces. This indicates the need for IoT-based solutions that combine real-time status updates with a display system to support immediate and non-intrusive student–teacher interaction.




Theoretical Framework 
This study is anchored on two theories that explain both the acceptance and the effectiveness of the Smart Faculty Status Indicator System, namely the Technology Acceptance Model and the Information Systems Success Model.

Figure 1. Technology Acceptance Model (TAM)

Technology Acceptance Model (TAM)
	The Technology Acceptance Model proposed by Davis (1989) explains how users come to accept and use a technological system. The model states that system adoption is influenced by perceived usefulness and perceived ease of use. Perceived usefulness refers to the extent to which users believe that the system improves task performance, while perceived ease of use refers to how simple and effortless the system is to operate (Davis, 1989). In this study, the model explains how faculty members and students are more likely to use the Smart Faculty Status Indicator System when they find it helpful in displaying real-time faculty availability and easy to understand and operate for academic coordination.




























Figure 2. Information Systems Success Model

Information Systems Success Model
The Information Systems Success Model developed by DeLone and McLean (2003) explains how the success of an information system can be evaluated through system quality, information quality, use, user satisfaction, and net benefits. System quality refers to the reliability and real-time performance of the system, while information quality focuses on the accuracy and timeliness of the information provided (DeLone & McLean, 2003). Net benefits refer to the positive outcomes gained from using the system. In this study, the model is used to assess the effectiveness of the Smart Faculty Status Indicator System in improving time efficiency, reducing unnecessary interruptions, and enhancing communication between faculty members and students.

























Conceptual Framework
Figure 3. Conceptual Framework of the Smart Faculty Status Indicator System (SFSI)
	Figure 3 presents the conceptual framework of the study using the Input–Process–Output (IPO) model. The input includes faculty status information entered through the web-based application, system hardware components such as the microcontroller and TFT LCD screen, and the network connection required for data transmission. The process involves receiving status updates from the web application, transmitting the data through the network, processing the information within the system, and synchronizing the updates between the web platform and the display unit. The output consists of the real-time display of faculty availability statuses—Available, Busy, or Not Present—on the TFT LCD screen, providing timely and accurate information to students, teachers, and administrators to support better coordination and communication within the academic environment.

Project Description 
	This project involves the design and testing of a Smart Faculty Status Indicator (SFSI) prototype that provides real-time information on faculty availability within an academic environment. The system allows faculty members to update their current status—Available, Busy, or Not Present—through a web-based application, which is then displayed on a TFT LCD screen for easy viewing by students. The project aims to assess how quickly and accurately the system reflects status changes, as well as its overall performance in delivering reliable real-time updates. By improving access to faculty availability information, the SFSI prototype seeks to reduce unnecessary interruptions, support better time management, and enhance coordination among students, faculty, and administrators.



Project Procedure 
The implementation of the Smart Faculty Status Indicator System (SFSI) follows a systematic approach consisting of material preparation, display assembly, website development, integration, and testing: 

1. Material Collection and Preparation
Gather all hardware components: ESP32 Development Board, TFT LCD Touch Shield 3.2 inch, breadboard, jumper wires, foam board, illustration board, steel wall brackets, and transparent acrylic for screen protection.
Prepare software tools: install Visual Studio Code with PlatformIO for programming the ESP32.
Develop a simple website with HTML for real-time teacher status updates and student access.
2. Display Board Assembly
Cut the foam board to the desired size and attach it to the illustration board for stability.
Mount the ESP32 microcontroller with the TFT LCD Touch Shield onto the board using brackets or tape.
Place a piece of transparent acrylic over the TFT screen to protect it and give it a neat appearance.
Connect the ESP32 to a power source and configure Wi-Fi settings to enable communication with the website.
3. Website Development
Design and develop a secure website for teacher status updates and student viewing.
Implement teacher login functionality so only teachers can update their own status.
Add a database to store teacher status (Available, Busy, Not Present).
Include a search feature for students to find a specific teacher’s availability.
4. Integration of Display and Website
Program the ESP32 to fetch teacher status data from the website in real time.
Configure the TFT LCD display to show the latest status for each teacher.
Apply color-coded indicators on the screen for easy understanding:
Green – Available
Yellow – Busy
Red – Not Present
5. Testing and Evaluation
Test the website for proper teacher login and status update functionality.
Verify that status changes on the website are reflected immediately on the TFT LCD display outside the faculty area.
Test student access to the website, including the search function and accurate display of teacher status.
Evaluate the system’s reliability, responsiveness, and usability to ensure smooth operation.
Through this procedure, the project ensures a reliable, real-time, and secure method for teachers to communicate their availability while allowing students to check teacher status efficiently, improving coordination and minimizing unnecessary interruptions.

Project Testing
	The Smart Faculty Status Indicator System (SFSI) will be tested to evaluate its accuracy, performance, and reliability. The testing will focus on determining whether the system can correctly display faculty availability in real time through the web-based application and the TFT LCD screen.
Functional testing will be conducted to verify that teachers are able to update their status on the website and that the correct status (Available, Busy, or Not Present) is displayed on the TFT LCD. Multiple status changes will be performed to confirm proper system operation.
Accuracy and response time testing will be carried out by comparing the status set on the website with the status shown on the display. The time delay between updating the status and its appearance on the screen will be observed to determine whether real-time updates are achieved.
Basic reliability and usability testing will also be performed. The system will be operated continuously for a period of time to check for errors or interruptions. Teachers and students will be asked to use the system to evaluate its ease of use and clarity. The results will be used to assess the system’s readiness for real-world implementation and to identify areas for improvement.
