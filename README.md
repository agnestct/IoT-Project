This project presents an IoT-based smart elevator monitoring system that detects floor positions, movements, direction, and vibration in real time. The system includes a BMP581 pressure sensor to measure air pressure and estimate the current floor, and an ICM-20948 IMU sensor to measure acceleration, vibrations, and magnetic field strength for detecting elevator motion and door status. All data is processed locally on the IoT device (ESP32-S3) and transmitted to a gateway via BLE, then stored in Firebase for monitoring through a web dashboard.

The main objective is to provide a low-cost, scalable solution for monitoring elevator operation in smart buildings. The system applies lightweight statistical methods and simple regression models directly on the microcontroller to classify movement (idle or moving), track trips between floors, door movements, and identify abnormal pressure or vibration conditions.

Features:
- Real-time floor estimation using pressure data
- Movement detection using IMU Z-axis acceleration
- Magnetic field strength for detecting door status
- Vibration intensity calculation on-device
- Trip tracking (start floor → end floor)
- BLE communication to gateway
- Cloud storage (Firebase) and web dashboard visualization
- Basic anomaly detection (pressure and sudden movement)

System Architecture

Pressure + IMU -> IoT Device ->  BLE Gateway -> Firebase -> Web dashboard

This project demonstrates how embedded sensing, edge computation, and cloud integration can be combined to build an intelligent monitoring system for building infrastructure.

# IK1332-ElevatorMonitor
Web API: https://elevatormonitor-3e8cd.web.app/
=======
# IoT-Project
>>>>>>> 4c28cf2ac0f977207cb69c18e9ba5c171c14c7e0

# Elevator patterns web: https://iot-project-demo-94e17.web.app
