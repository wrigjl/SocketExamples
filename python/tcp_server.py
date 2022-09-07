
"""Example TCP server program
   to use it, $ python udp_server.py [portnumber] [address]
   If portnumber is omitted, defaults to 9999. If address is
   omitted, the program binds to all addresses on this host"""

import argparse
from socket import socket, AF_INET, SOCK_STREAM, SOL_SOCKET, SO_REUSEADDR

def main():
    """tcp server handler: creates socket and handles requests"""

    parser = argparse.ArgumentParser(description="TCPServer Example")
    parser.add_argument('port', help="the port to use",
                        nargs='?',
                        default=9999)
    parser.add_argument('address', nargs='?',
                        help="the address for this machine",
                        default='')
    args = parser.parse_args()

    sock = socket(AF_INET, SOCK_STREAM)
    sock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    sock.bind((args.address, args.port))
    sock.listen(5)

    print("server ready, waiting on connections")
    while True:
        clientsock, clientaddr = sock.accept()
        print(f"connection from {clientaddr}")
        message = clientsock.recv(1024)
        print(f"string from {clientaddr}: {message}")
        message = message.decode().upper().encode()
        print(f"Sending to {clientaddr}: {message}")
        clientsock.send(message)
        clientsock.close()

if __name__ == "__main__":
    main()
