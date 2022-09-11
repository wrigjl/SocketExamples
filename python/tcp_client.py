
"""Example TCP client program
   to use it, $ python tcp_client.py [address] [portnumber]
   If portnumber is omitted, defaults to 9999. If address is
   omitted, the program connects to localhost"""

import argparse
from socket import socket, AF_INET, SOCK_STREAM

def main():
    """tcp server handler: creates socket and sends/receives data"""

    parser = argparse.ArgumentParser(description="TCP client example")
    parser.add_argument('address', nargs='?',
                        help="the address for this machine",
                        default='localhost')
    parser.add_argument('port', help="the port to use",
                        nargs='?', type=int,
                        default=9999)
    args = parser.parse_args()

    sock = socket(AF_INET, SOCK_STREAM)

    sock.connect((args.address, args.port))

    message = input('Input lowercase sentence: ').encode()

    sock.send(message)
    print(f"message sent: {message}")
    message = sock.recv(1024)
    print(f"message received: {message}")


if __name__ == "__main__":
    main()
