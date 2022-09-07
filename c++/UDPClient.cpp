
#include <sys/types.h>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#endif
#include <string.h>
#include <string>
#include <iostream>

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

#ifndef _WIN32
typedef int SOCKET;
extern int closesocket(int fd);
#else
#pragma comment(lib, "Ws2_32.lib")
#endif

using std::cerr;
using std::endl;
using std::cin;
using std::cout;
using std::string;
using std::getline;

#define BUFLEN 1024

void socket_error(const char* prefix);

int
main(int argc, char* argv[]) {
#ifdef _WIN32
    WSADATA wsaData;
#endif
    SOCKET sock = INVALID_SOCKET;
    int res, rc = 0;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    struct sockaddr_storage saddr;
    string message;
    socklen_t socklen;
    char buf[BUFLEN];

#ifdef _WIN32
    // initialize the winsock API
    res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res) {
        std::cerr << "WSAStartup failed: " << res << std::endl;
        return 1;
    }
#endif

    if (argc != 3) {
        cerr << argv[0] << " hostname service" << endl;
        rc = 1;
        goto out;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    res = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (res != 0) {
        cerr << gai_strerror(res) << ": " << argv[1] << ":" << argv[2] << endl;
        rc = 1;
        goto out;
    }

    cout << "Input lowercase sentence: " << endl;
    getline(cin, message);

    // getaddrinfo returns a linked list of addresses/ports, we need to
    // try them all

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sock == INVALID_SOCKET) {
            socket_error("socket");
            continue;
        }

        res = sendto(sock, message.c_str(), (int)strlen(message.c_str()), 0,
            ptr->ai_addr, (int)ptr->ai_addrlen);
        if (res == -1) {
            socket_error("sendto");
            closesocket(sock);
            sock = INVALID_SOCKET;
            continue;
        }

        break;
    }

    if (sock == INVALID_SOCKET) {
        // If the socket is invalid here, we've exhausted all the results
        cerr << "exhausted all the addresses" << endl;
        rc = 1;
        goto out;
    }

    socklen = sizeof(saddr);
    memset(&saddr, 0, sizeof(saddr));
    res = recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr*)&saddr, &socklen);
    if (res == -1) {
        socket_error("recvfrom");
        rc = 1;
        goto out;
    }

    cout.write(buf, res);

out:
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }

    if (result != NULL) {
        freeaddrinfo(result);
        result = NULL;
    }

    return rc;
}

#ifndef _WIN32
int closesocket(int fd) {
    return close(fd);
}
#endif

void
socket_error(const char* prefix) {
    cerr << prefix << ": ";
#ifdef _WIN32
    cerr << "error code " << WSAGetLastError() << endl;
#else
    cerr << strerror(errno) << endl;
#endif
}
