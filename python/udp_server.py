
"""Example UDP server program
   to use it, $ python udp_server.py [portnumber] [address]
   If portnumber is omitted, defaults to 9999. If address is
   omitted, the program binds to all addresses on this host"""

import argparse
from socket import socket, AF_INET, SOCK_DGRAM

def main():
    """udp server handler: creates socket and handles requests"""

    parser = argparse.ArgumentParser(description="UDPServer Example")
    parser.add_argument('port', help="the port to use",
                        nargs='?',
                        default=9999)
    parser.add_argument('address', nargs='?',
                        help="the address for this machine",
                        default='')
    args = parser.parse_args()

    sock = socket(AF_INET, SOCK_DGRAM)
    sock.bind((args.address, args.port))
    print("server ready, waiting on packets")
    while True:
        message, client = sock.recvfrom(1024)
        print(f"Packet from {client}: {message}")
        message = message.decode().upper().encode()
        print(f"Sending to {client}: {message}")
        sock.sendto(message, client)

if __name__ == "__main__":
    main()
