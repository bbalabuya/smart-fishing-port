import serial
import time
import json
import requests
from picamera2 import Picamera2, Preview

# 아두이노에서 정보를 받아옴
ser = serial.Serial('/dev/ttyACM0', 9600)

# Flask 서버 URL 설정 ( 웹 서버 준비 )
FLASK_SERVER_URL = 'http://localhost:8000/data'

# 카메라 설정 및 사진 찍기 (한 번만 수행)
picam = Picamera2()
config = picam.create_preview_configuration()
picam.configure(config)
picam.start_preview(Preview.QTGL)
picam.start()
time.sleep(3)

# 사진을 찍고 파일로 저장
image_path = "now.jpg"
picam.capture_file(image_path)
picam.stop_preview()
picam.close()

#계속 데이터를 전달받음
while True:
    if ser.in_waiting > 0:
        try:
            # 아두이노에서 받아온 데이터를 변수에 입력
            data = ser.readline().decode('utf-8').rstrip()
            sensor_data = json.loads(data)
        except json.JSONDecodeError as e:
            print(f"JSON Decode Error: {e}")
            print(f"Received data: {data}")
            continue

        #데이터 분류 및 저장
        temp = sensor_data.get('temp')
        water = sensor_data.get('water')
        turbidity = sensor_data.get('turbidity')

        #터미널에 출력
        print(f"Temperature: {temp} C, Water Level: {water}, Turbidity: {turbidity}")
        
        # 데이터를 폼 데이터로 전환
        payload = {
            'temperature': temp,
            'water_level': water,
            'turbidity': turbidity
        }
        
        # 파일을 읽어서 전송
        with open(image_path, 'rb') as image_file:
            files = {'image': ('now.jpg', image_file, 'image/jpeg')}
            
            try:
                response = requests.post(FLASK_SERVER_URL, data=payload, files=files)

                #정상출력시
                if response.status_code == 200:
                    print("Data and image sent successfully to the Flask server.")
                else:
                #서버 전송 실패시rial.Serial('/dev/ttyACM0', 9600)

# Flask 서버 URL 설정 ( 웹 서버 준비 )
FLASK_SERVER_URL = 'http://localhost:8000/data'

# 카메라 설정 및 사진 찍기 (한 번만 수행)
picam = Picamera2()
config = picam.create_preview_configuration()
picam.configure(config)
picam.start_preview(Preview.QTGL)
picam.start()
time.sleep(3)

# 사진을 찍고 파일로 저장
image_path = "now.jpg"
picam.capture_file(image_path)
picam.stop_preview()
picam.close()

#계속 데이터를 전달받음
while True:
    if ser.in_waiting > 0:
        try:
            # 아두이노에서 받아온 데이터를 변수에 입력
            data = ser.readline().decode('utf-8').rstrip()
            sensor_data = json.loads(data)
        except json.JSONDecodeError as e:
            print(f"JSON Decode Error: {e}")
            print(f"Received data: {data}")
            continue

        #데이터 분류 및 저장
        temp = sensor_data.get('temp')
        water = sensor_data.get('water')
        turbidity = sensor_data.get('turbidity')

        #터미널에 출력
        print(f"Temperature: {temp} C, Water Level: {water}, Turbidity: {turbidity}")
        
        # 데이터를 폼 데이터로 전환
        payload = {
            'temperature': temp,
            'water_level': water,
            'turbidity': turbidity
        }
        
        # 파일을 읽어서 전송
        with open(image_path, 'rb') as image_file:
            files = {'image': ('now.jpg', image_file, 'image/jpeg')}
            
            try:
                response = requests.post(FLASK_SERVER_URL, data=payload, files=files)

                #정상출력시
                if response.status_code == 200:
                    print("Data and image sent successfully to the Flask server.")
                else:
                #서버 전송 실패시
                    print(f"Failed to send data to the Flask server. Status code: {response.status_code}")
            except requests.exceptions.RequestException as e:
                print(f"An error occurred: {e}")
        
        time.sleep(1)
