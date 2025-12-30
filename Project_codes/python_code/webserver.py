from flask import Flask, render_template, request, jsonify

server = Flask(__name__)

# Global sensor data (latest values)
sensor_data = {
    "temperature": "--",
    "humidity": "--",
    "gas": "--",
}

@server.route("/")
def dashboard():
    return render_template("dashboard.html")

# API to update sensor data
@server.route("/update", methods=["POST"])
def update_data():
    sensor_data["temperature"] = request.form.get("temp")
    sensor_data["humidity"] = request.form.get("humidity")
    sensor_data["gas"] = request.form.get("gas")

    print(sensor_data)
    return "Data Updated", 200

# API to fetch live data
@server.route("/data")
def get_data():
    return jsonify(sensor_data)

if __name__ == "__main__":
    server.run(host="0.0.0.0", port=5000, debug=True)
