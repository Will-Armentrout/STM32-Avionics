Peripheral Communication Testing Folder
Each file in here is labeled with the sensor it is testing.
All sensors were tested individually before being combined into one circuit.
To read sensor output all sensors data was output on a COM port on UART2 and read with PUTTY on COM5 and a Baud rate of 38400.

Peripherals Tested:
- Temperature Sensor: MCP9808 (On Adafruit 1782 Breakout Board)
	Communication: I2C
	Address: 
- IMU/GYRO: MPU-6050 (On HiLetGo GY-521 Breakout Board)
	Communication: I2C
	Address:
- Magnetometer: HMC5883L (On HiLetGo GY-271 Breakout Board)
	Communication: I2C
	Address:
- Pressure Sensor: BMP 280 (On HiLetGo GY-BM 280 Breakout Board)
	Communication: I2C
	Address:
- GPS: NEO-6M (On HiLetGo GY-NEO-6MV2 Breakout Board)
	Communication: I2C
	Address:
- Micro Servo: Miuzei MG90S 9G Micro Servo Motor
	Communication: PWM