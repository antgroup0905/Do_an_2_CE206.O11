from flask import Flask, jsonify, render_template
import paho.mqtt.client as mqtt

app = Flask(__name__)

# Kết nối tới MQTT broker
broker_address = "broker.hivemq.com"
client = mqtt.Client()
client.connect(broker_address)

# Khởi tạo temperature và humidity
pHvalue = 0


# Trang chủ của ứng dụng web
@app.route("/")
def index():
    return render_template("index.html")


# API trả về dữ liệu nhiệt độ và độ ẩm mới nhất
@app.route("/api/data")
def get_data():
    # Gửi yêu cầu lấy dữ liệu mới nhất tới MQTT broker
    client.publish("antgroup0905/get_data", "get_data")
    # Trả về dữ liệu dưới dạng JSON
    return jsonify({"pHvalue": pHvalue})


# Hàm xử lý khi nhận được dữ liệu từ MQTT broker
def on_message(client, userdata, message):
    global pHvalue
    # Lấy dữ liệu từ message payload
    data = message.payload.decode()
    pHvalue = data
    pHvalue = float(pHvalue)


# Thiết lập callback function cho MQTT client
client.on_message = on_message
client.subscribe("testingESP32")

if __name__ == "__main__":
    # Start MQTT client
    client.loop_start()
    # Start Flask app
    app.run(debug=True)
