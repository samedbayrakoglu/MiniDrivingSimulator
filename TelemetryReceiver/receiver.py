import socket
import struct

HOST = "127.0.0.1"
PORT = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((HOST, PORT))

print(f"Listening on {HOST}:{PORT}...")

while True:
    data, _ = sock.recvfrom(1024)

    if len(data) != 32:
        print(f"Invalid packet size: {len(data)} bytes")
        continue

    values = struct.unpack("8f", data)

    speed, rpm, pos_x, pos_y, pos_z, steering, throttle, brake = values

    print(
        f"Speed: {speed:.2f} | "
        f"RPM: {rpm:.2f} | "
        f"Pos: ({pos_x:.2f}, {pos_y:.2f}, {pos_z:.2f}) | "
        f"Steering: {steering:.2f} | "
        f"Throttle: {throttle:.2f} | "
        f"Brake: {brake:.2f}"
    )