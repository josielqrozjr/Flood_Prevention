from flask import Blueprint, Flask, render_template, request

actuators = Blueprint("actuators", __name__, template_folder = "template")

actuators_list = {
    'LED':'Ligado'
}

@actuators.route('/register_actuator')
def register_actuator():
    return render_template('register_actuator.html')

@actuators.route('/add_actuator', methods=['POST', 'GET'])
def add_actuator():
    global actuators_list
    if request.method == 'POST':
        actuator_name = request.form['actuator_name']
        actuator_value = request.form['actuator_value']
    else:
        actuator_name = request.args.get('actuator_name', None)
        actuator_value = request.args.get('actuator_value', None)
    actuators_list[actuator_name] = actuator_value
    return render_template('actuators.html', devices=actuators_list)

@actuators.route('/list_actuators')
def list_actuators():
    return render_template('actuators.html', devices=actuators_list)

@actuators.route('/remove_actuator')
def remove_actuator():
    global actuators_list
    return render_template("remove_actuator.html", devices=actuators_list)

@actuators.route('/del_actuator', methods=['GET', 'POST'])
def del_actuator():
    global actuators_list
    if request.method == 'POST':
        actuator_name = request.form['actuator_name']
    else:
        actuator_name = request.args.get('actuator_name', None)
    actuators_list.pop(actuator_name)
    return render_template("actuators.html", devices=actuators_list)