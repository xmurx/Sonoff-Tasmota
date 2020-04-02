#!/usr/bin/env python3

"""A MQTT to InfluxDB Bridge

This script receives MQTT data and saves those to InfluxDB.

"""

import re
from typing import NamedTuple

import paho.mqtt.client as mqtt
from influxdb import InfluxDBClient

#!/usr/bin/env python3

import json

INFLUXDB_ADDRESS    = 'iotstack'
INFLUXDB_USER       = 'tankuser'
INFLUXDB_PASSWORD   = 'tank'
INFLUXDB_DATABASE   = 'tank'

MQTT_ADDRESS    = 'iotstack'
MQTT_USER       = 'mqttuser'
MQTT_PASSWORD   = 'mqttpassword'
MQTT_TOPIC      = 'tele/tank/SENSOR'
MQTT_CLIENT_ID  = 'MQTTInfluxDBBridge'

influxdb_client = InfluxDBClient(INFLUXDB_ADDRESS, 8086, INFLUXDB_USER, INFLUXDB_PASSWORD, None)

class SensorData(NamedTuple):
    temperatureName: str
    temperatureValue: float
    humidityName: str
    humidityValue: float
    airPressureName: str
    airPressureValue: float
    distanceName: str
    distanceValue: float
    tankLevelName: str
    tankLevelValue: float

def mqtt_on_connect(client, userdata, flags, rc):
    print('Connected to ' + MQTT_ADDRESS + ' result code ' + str(rc))
    client.subscribe(MQTT_TOPIC)

def mqtt_on_message(client, userdata, msg):
    data = _parse_mqtt_message(msg.topic, msg.payload.decode('utf-8'))
    if data is not None:
        _send_sensor_data_to_influxdb(data)

def _parse_mqtt_message(topic, payload):
    parsed_json = json.loads(payload)
    if parsed_json is not None:
        Tank = parsed_json['Tank']
        level = Tank['Level']
        print('Level: ' + str(level) + ' % free')
        BME280 = parsed_json['BME280']
        temp = BME280['Temperature']
        humi = BME280['Humidity']
        pres = BME280['Pressure']
        vl53 = parsed_json['VL53L0X']
        dist = vl53['Distance']
        return SensorData('TankTemp', float(temp), 'TankHumi', float(humi), 'TankPressure', float(pres), 'Distance', float(dist), 'TankLevel', float(level) )
    else:
        return None

def _send_sensor_data_to_influxdb(sensor):
    json_body = [
        {
            'measurement': 'tank',
            'tags': {
                'location': 'tank',
            },
            'fields': {
                sensor.tankLevelName: sensor.tankLevelValue,
                sensor.distanceName: sensor.distanceValue,
                sensor.temperatureName: sensor.temperatureValue,
                sensor.humidityName: sensor.humidityValue,
                sensor.airPressureName: sensor.airPressureValue
            }
        }
    ]
    influxdb_client.write_points(json_body)


def _init_influxdb_database():
    databases = influxdb_client.get_list_database()
    if len(list(filter(lambda x: x['name'] == INFLUXDB_DATABASE, databases))) == 0:
        influxdb_client.create_database(INFLUXDB_DATABASE)
    influxdb_client.switch_database(INFLUXDB_DATABASE)

def main():
    _init_influxdb_database()
    mqtt_client = mqtt.Client(MQTT_CLIENT_ID)
    mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    mqtt_client.on_connect = mqtt_on_connect
    mqtt_client.on_message = mqtt_on_message
    mqtt_client.connect(MQTT_ADDRESS, 1883)
    mqtt_client.loop_forever()

if __name__ == '__main__':
    print('MQTT to InfluxDB bridge')
    main()
