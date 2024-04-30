from flask import Blueprint, render_template, request, url_for

sensors = Blueprint("sensors", __name__, template_folder="templates")

sensors_list = {
    'DHT11':'25º',
    'HC-ST04':'100cm'
}

@sensors.route('/register_sensor')
def register_sensor():
    return render_template('register_sensor.html')

@sensors.route('/add_sensor', methods=['GET', 'POST'])
def add_sensor():
    global sensors_list
    if request.method == 'POST':
        sensor_name = request.form['sensor_name']
        sensor_value = request.form['sensor_value']
    
    else:
        sensor_name = request.args.get('sensor_name', None)
        sensor_value = request.args.get('sensor_value', None)
    sensors_list[sensor_name] = sensor_value
    return render_template('sensors.html', devices=sensors_list)

@sensors.route('/list_sensors')
def list_sensors():
    global sensors_list
    return render_template('sensors.html', devices=sensors_list)

@sensors.route('/remove_sensor')
def remove_sensor():
    global sensors_list
    return render_template("remove_sensor.html", devices=sensors_list)

@sensors.route('/del_sensor', methods=['GET', 'POST'])
def del_sensor():
    global sensors_list
    if request.method == 'POST':
        sensor_name = request.form['sensor_name']
    else:
        sensor_name = request.args.get('sensor_name', None)
    sensors_list.pop(sensor_name)
    return render_template("sensors.html", devices=sensors_list)