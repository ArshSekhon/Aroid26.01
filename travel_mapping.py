import requests
import json

travel_map_points = []


def get_orientation_and_odometry_point():
    url = "http://192.168.0.99/sensorRead"
    payload = "sensorType=READ%3BORIENTATION%3B&undefined="
    headers = {'Content-Type': "application/x-www-form-urlencoded",'cache-control': "no-cache"}
    response = requests.request("POST", url, data=payload, headers=headers)
    wheel_dist = response.json()["WheelDist"]
    max_dist = max(zip(wheel_dist.values(), wheel_dist.keys()))[0]
    angle_of_heading = response.json()["MPU"]["Heading"]

    mapping_data_point = {
        "Heading": angle_of_heading,
        "Distance_So_Far": max_dist
    }
    travel_map_points.append(mapping_data_point)
    reset_odometer()

def save_travel_history():
    with open('travel_history.txt', 'wt') as fout:
        json.dump(travel_map_points, fout)


def reset_odometer():
    url = "http://192.168.0.99/serialInput"
    payload = "input=RESET_ODO%3B&undefined="
    headers = {'Content-Type': "application/x-www-form-urlencoded",'cache-control': "no-cache"}
    response = requests.request("POST", url, data=payload, headers=headers)



