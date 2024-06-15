import io
import json
import threading
from flask import Flask, jsonify, render_template, Response
from picamera2 import Picamera2
from picamera2.encoders import JpegEncoder
from picamera2.outputs import FileOutput
import serial

app = Flask(__name__)
sensor_data = {
    "temperature": None,
    "water_level": None,
    "turbidity": None,
    "ph": None
}

@app.route('/sensor_data')
def get_sensor_data():
    return jsonify(sensor_data)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/stream.mjpg')
def video_feed():
    return Response(gen(),
                    mimetype='multipart/x-mixed-replace; boundary=FRAME')

class StreamingOutput(io.BufferedIOBase):
    def __init__(self):
        self.frame = None
        self.buffer = io.BytesIO()
        self.condition = threading.Condition()

    def write(self, buf):
        if buf.startswith(b'\xff\xd8'):
            self.buffer.truncate()
            with self.condition:
                self.frame = self.buffer.getvalue()
                self.condition.notify_all()
            self.buffer.seek(0)
        return self.buffer.write(buf)

def gen():
    while True:
        with output.condition:
            output.condition.wait()
            frame = output.frame
        yield (b'--FRAME\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')

def sensor_data_collection():
    global sensor_data
    ser = serial.Serial('/dev/ttyACM0', 9600)
    
    while True:
        if ser.in_waiting > 0:
            try:
                data = ser.readline().decode('utf-8').rstrip()
                print(f"Received data: {data}")  # 수신된 데이터 콘솔에 출력
                sensor_data = json.loads(data)
            except json.JSONDecodeError as e:
                print(f"JSON Decode Error: {e}")
                print(f"Received data: {data}")

if __name__ == '__main__':
    picam2 = Picamera2()
    video_config = picam2.create_video_configuration(main={"size": (640, 480)})
    picam2.configure(video_config)
    output = StreamingOutput()

    encoder = JpegEncoder()
    picam2.start_recording(encoder, FileOutput(output))

    threading.Thread(target=sensor_data_collection, daemon=True).start()

    app.run(host='0.0.0.0', port=8000, debug=True, use_reloader=False)

    picam2.stop_recording()


 

