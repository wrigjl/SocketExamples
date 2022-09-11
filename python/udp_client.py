
"""Example UDP client program
   to use it, $ python udp_client.py [address] [portnumber]
   If portnumber is omitted, defaults to 9999. If address is
   omitted, the program connects to localhost"""

import argparse
from socket import socket, AF_INET, SOCK_DGRAM

def main():
    """udp client handler: creates socket and sends data"""

    parser = argparse.ArgumentParser(description="UDP client example")
    parser.add_argument('address', nargs='?',
                        help="the address for remote machine",
                        default='localhost')
    parser.add_argument('port', help="the port to use",
                        nargs='?', type=int,
                        default=9999)
    args = parser.parse_args()

    sock = socket(AF_INET, SOCK_DGRAM)

    message = input('Input lowercase sentence: ').encode()

    sock.sendto(message, (args.address, args.port))

    print(f"message sent: {message}")
    message, server = sock.recvfrom(1024)
    print(f"message received from {server}: {message}")
    sock.close()

if __name__ == "__main__":
    main()
