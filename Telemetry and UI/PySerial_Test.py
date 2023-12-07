import time
import serial

port='COM5'
baudrate=38400

ser = serial.Serial(
    port,
    baudrate,
    timeout=1
)

ser.isOpen()

i = 0
while True:

    try:
        received_data = ser.readline().decode('utf-8')
    except UnicodeDecodeError as e:
        print(f"Invalid Unicode Character. Ignore!")
    else:
        if(received_data.__contains__('GPRMC')):
            parsed = received_data.split(',')

            if parsed[3] != '' or parsed[5] != '':
                lat_deg = float((parsed[3])[0:2])
                long_deg = float((parsed[5])[0:3])

                lat_min = float((parsed[3])[2:parsed[3].__len__()])
                long_min = float((parsed[5])[3:parsed[5].__len__()])

                lat = lat_deg + lat_min / 60
                long = long_deg + long_min / 60

                latitude = str(lat) + ' ' + parsed[4]
                longitude = str(long) + ' ' + parsed[6]
                print(f"Latitude: {latitude}")
                print(f"Longitude: {longitude}")
            else:
                print("Waiting for Signal...")