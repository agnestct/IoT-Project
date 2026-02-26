# IK1332 IoT Elevator Monitoring

Group 4 – Real-time Elevator Monitoring with IoT

# Elevator patterns web: https://iot-project-demo-94e17.web.app

🚀 Project Overview
A low-cost IoT system for monitoring elevators in real time. Tracks floors, elevator movements, and abnormal behavior using:
- Pressure Sensor (BMP581) – estimates floor levels
- IMU Sensor (ICM-20948) – detects motion anomalies
- ESP32-S3 – processes data locally and transmits via BLE & Wi-Fi
- Firebase + React WebUI – real-time monitoring and visualization

Key Features:
- Real-time floor estimation and movement tracking
- BLE queuing ensures no data loss
- Anomaly detection with IMU acceleration
- Historical and live data visualization

🏗 Project Structure
```text
.
├── Client/               # BLE client for data acquisition
│   ├── Client.ino
│   └── src/
├── data_collection/      # Data collection scripts
│   ├── imu_data_collection/
│   └── project/
├── Notify/               # BLE notification / gateway firmware
│   ├── Notify.ino
│   └── src/
├── train/                # MATLAB training scripts for linear regression & data analysis
├── WebUI/                # React web dashboard
│   └── src/
└── README.md
```

Notes:
- Client/ handles sensor reading and BLE client communication
- Notify/ acts as BLE gateway to Firebase
- data_collection/ is used for gathering sensor data for model training
- train/ contains scripts for model training, visualization, and pressure calibration
- WebUI/ shows the dashboard with floor, movement, pressure, and alarms

⚙️ Quick Start
1️⃣ Firmware
- Flash Notify.ino to ESP32 inside elevator (connect BMP581 + ICM-20948)
- Flash Client.ino to ESP32 gateway (handles BLE → Firebase)
- Configure Wi-Fi credentials in EduroamWiFi.h

2️⃣ Web Dashboard
- Go to WebUI/src/
- Run npm install
- Run npm start to open dashboard

3️⃣ Data Collection & Training (Optional)
- Train linear regression model using MATLAB scripts in train/

📊 Demo & Results
- Floor prediction accurate for all tested runs
- BLE reconnection mechanism ensures no data loss
- React WebUI visualizes current floor, movement, pressure, and anomalies

🔮 Future Work
- Add functionality to detect elevator door open/close status using magnetic field data
- Predictive maintenance alerts
- Multi-elevator monitoring and cloud scalability
