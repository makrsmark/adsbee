from pymavlink import mavutil
import serial
import os
import time # for timestamping

SERIAL_PORT = "COM3"
BAUD_RATE = 115200
REFRESH_RATE_HZ = 1

mavlink_connection = mavutil.mavlink_connection(SERIAL_PORT, baud=BAUD_RATE)

while (True):
    next_update_timestamp_s = time.time() + 1 / REFRESH_RATE_HZ
    os.system('cls') # Clear the screen.
    print("ICAO Addr |Latitude  |Longitude |Alt Type  |Alt (m)   |Hdg (deg) |Hvel (m/s)|Vvel (m/s)|Callsign  |Type      |TSLC (sec)|Flags     |Squawk")
    print("----------|----------|----------|----------|----------|----------|----------|----------|----------|----------|----------|----------|----------")
    msg = mavlink_connection.recv_match(type='ADSB_VEHICLE', blocking=False)
    while (msg):
        print(f"{msg.ICAO_address:10x}|{msg.lat / 1e7:+10.4f}|{msg.lon / 1e7:+10.4f}|{msg.altitude_type:10x}|"\
                f"{msg.altitude / 1e3:10.2f}|{msg.heading / 100:10.4f}|{msg.hor_velocity / 100:10}|"\
                f"{msg.ver_velocity / 100:10}|{msg.callsign:10}|{msg.emitter_type:10}|{msg.tslc:10}|{msg.flags:10b}|"\
                f"{msg.squawk:10o}")
        msg = mavlink_connection.recv_match(type='ADSB_VEHICLE', blocking=False)
    time.sleep(next_update_timestamp_s - time.time())