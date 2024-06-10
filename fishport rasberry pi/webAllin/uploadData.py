#여기는 서버여는 파일
from flask import Flask, render_template, request, jsonify, send_from_directory
import os

app = Flask(__name__)

# 전역 변수 선언 및 이미지 폴더 생성
sensor_data = {'temperature': None, 'water_level': None, 'turbidity': None}
image_folder = 'images'
#라즈베리파이 제어
os.makedirs(image_folder, exist_ok=True)

#URL에 대한 요청처리
@app.route('/')
def index():
    image_path = None
    if os.path.exists(os.path.join(image_folder, 'image.jpg')):              #사진이 존재할 경우
        image_path = 'image.jpg'                                             #사진 데이터 저장
    return render_template('index.html', data=sensor_data, image=image_path) # 사진 데이터 전달

#데이터를 받아옴
@app.route('/data', methods=['POST'])
def receive_data():
    global sensor_data
    sensor_data = {                #getData 파일에서 정보 넘겨받음
        'temperature': request.form['temperature'],
        'water_level': request.form['water_level'],
        'turbidity': request.form['turbidity']
    }

    # 이미지를 저장
    image = request.files.get('image')
    if image:
        image.save(os.path.join(image_folder, 'image.jpg'))
    return 'Data received successfully'

#json파일로 변환
@app.route('/sensor_data', methods=['GET'])
def get_sensor_data():
    return jsonify(sensor_data)

#이미지 저장
@app.route('/images/<filename>')
def get_image(filename):
    return send_from_directory(image_folder, filename)

#웹 서버 시작
if __name__ == '__main__':
    app.run(debug=True, port=8000, host='0.0.0.0')

