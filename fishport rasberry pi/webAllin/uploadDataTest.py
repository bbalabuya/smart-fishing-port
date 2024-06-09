from flask import Flask, render_template, request, jsonify, send_from_directory
import os

app = Flask(__name__)

# 전역 변수 선언
sensor_data = {'temperature': None, 'water_level': None, 'turbidity': None}
image_folder = 'images'
os.makedirs(image_folder, exist_ok=True)

@app.route('/')
def index():
    image_path = None
    if os.path.exists(os.path.join(image_folder, 'image.jpg')):
        image_path = 'image.jpg'
    return render_template('index.html', data=sensor_data, image=image_path)

@app.route('/data', methods=['POST'])
def receive_data():
    global sensor_data
    sensor_data = {
        'temperature': request.form['temperature'],
        'water_level': request.form['water_level'],
        'turbidity': request.form['turbidity']
    }

    # 이미지를 저장
    image = request.files.get('image')
    if image:
        image.save(os.path.join(image_folder, 'image.jpg'))
    return 'Data received successfully'

@app.route('/sensor_data', methods=['GET'])
def get_sensor_data():
    return jsonify(sensor_data)

@app.route('/images/<filename>')
def get_image(filename):
    return send_from_directory(image_folder, filename)

if __name__ == '__main__':
    app.run(debug=True, port=8000, host='0.0.0.0')

