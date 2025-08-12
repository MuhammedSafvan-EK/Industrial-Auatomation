# Distance-Based Object Counting in Industrial Automation
#### An IoT-powered industrial automation system that uses ultrasonic sensors and an ESP32-C3 microcontroller to accurately detect, count, and monitor objects in real time. The system integrates with a Django web application for cloud-based analytics, remote monitoring, and automated alerts.

### 📌 Features
Real-time Object Counting using ultrasonic distance measurement

Cloud Integration for data storage, analytics, and remote access

OLED Display & Buzzer Alerts for local feedback

Threshold-based Notifications to prevent operational bottlenecks

Web Dashboard to view live data, trends, and history

Scalable Architecture to support multiple sensors and locations

### 🏗 System Architecture
Hardware Components
Ultrasonic Sensor – Measures object distance

ESP32-C3 – Processes sensor data & handles cloud communication

OLED Display – Displays live count

Buzzer – Alerts on threshold breaches

Rotary Encoder – Adjusts system settings

Cloud Platform – Stores and analyzes count data

Software Stack
Backend: Django (Python)

Frontend: HTML, CSS, JavaScript

Database: SQLite3

Firmware: C++ (Arduino IDE for ESP32-C3)

IoT Communication: MQTT / HTTP

### ⚙️ How It Works
Object Detection – The ultrasonic sensor detects objects entering the measurement zone.

Data Processing – ESP32-C3 calculates counts and sends them to the cloud.

Local Feedback – OLED shows counts; buzzer alerts when limits are reached.

Cloud Sync – Data is transmitted to the Django web app for visualization.

Remote Monitoring – Users can view live and historical data via the web dashboard.

##🚀 Installation & Setup
1. Hardware Setup
Connect the ultrasonic sensor, OLED, buzzer, and rotary encoder to ESP32-C3.

Upload the provided firmware using Arduino IDE.

2. Backend Setup
bash
Copy
Edit
# Clone the repository
git clone https://github.com/yourusername/project-name.git
cd project-name

# Create virtual environment
python -m venv venv
source venv/bin/activate   # On Windows: venv\Scripts\activate

# Install dependencies
pip install -r requirements.txt

# Run migrations
python manage.py migrate

# Start the server
python manage.py runserver
3. Configure ESP32-C3
Update Wi-Fi credentials and server API endpoint in the firmware code.

Upload firmware to ESP32-C3.

#### 📊 Applications
Manufacturing – Count products on conveyor belts

Warehouse & Logistics – Monitor package movement

Retail – Automated inventory counting

Traffic Management – Vehicle counting

Event Management – Track attendees

#### 📈 Future Enhancements
AI-powered object classification

Multi-sensor integration

Energy-efficient ESP32 modes

Predictive analytics for operations

## 📜 License
This project is licensed under the MIT License.

# Device Image
<img width="465" height="398" alt="image" src="https://github.com/user-attachments/assets/08e6176c-c5d8-48c8-90b8-449ee7f42ada" />
<img width="891" height="650" alt="image" src="https://github.com/user-attachments/assets/ced1ecf4-678c-4a23-b540-b20ad318c142" />
<img width="896" height="501" alt="image" src="https://github.com/user-attachments/assets/d796e462-3ece-4701-bc42-e17125976bd7" />


