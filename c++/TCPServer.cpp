
#include <sys/types.h>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <basetsd.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#endif
#include <string.h>
#include <string>
#include <iostream>
#include <cctype>

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

#ifndef _WIN32
typedef int SOCKET;
extern int closesocket(int fd);
#else
#pragma comment(lib, "Ws2_32.lib")
typedef SSIZE_T ssize_t;
#endif

using std::cerr;
using std::endl;
using std::cin;
using std::cout;
using std::string;

#define BUFLEN 1024

void socket_error(const char* prefix);
bool send_all(SOCKET sockfd, const void *buf, size_t len);

int
main(int argc, char* argv[]) {
#ifdef _WIN32
    WSADATA wsaData;
#endif

    SOCKET sock = INVALID_SOCKET;
    int res, rc = 0;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    struct sockaddr_storage saddr;
    size_t remaining = BUFLEN;
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
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    res = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (res != 0) {
        cerr << gai_strerror(res) << ": " << argv[1] << ":" << argv[2] << endl;
        rc = 1;
        goto out;
    }

    // getaddrinfo returns a linked list of addresses/ports, we need to
    // try them all

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sock == INVALID_SOCKET) {
            socket_error("socket");
            continue;
        }

        if (bind(sock, ptr->ai_addr, (int)ptr->ai_addrlen) == -1) {
            socket_error("bind");
            closesocket(sock);
            sock = INVALID_SOCKET;
            continue;
        }

	if (listen(sock, 5) == -1) {
	    socket_error("listen");
	    closesocket(sock);
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

    for (;;) {
        // Now listen for incoming connections

	socklen = sizeof(saddr);
	SOCKET client = accept(sock, (struct sockaddr *)&saddr, &socklen);
	if (client == INVALID_SOCKET) {
	    socket_error("accept");
	    rc = 1;
	    goto out;
	}

	char *bufp = buf;
	remaining = BUFLEN - 1;
	memset(buf, 0, BUFLEN);

	while (remaining != 0) {
	    ssize_t r = recv(client, bufp, (int)remaining, 0);
	    if (r == 0) {
		cerr << "EOF from client" << endl;
		closesocket(client);
		goto out;
	    }

	    if (r < 0) {
		socket_error("recv");
		rc = 1;
		closesocket(client);
		goto out;
	    }

	    // We read a line of data, stop
	    char *nl = strchr(buf, '\n');
	    if (nl != NULL) {
		*nl = '\0';
		remaining = 0;
	    } else {
		remaining -= r;
		bufp += r;
	    }
	}

	for (int i = 0; buf[i]; i++) {
	    if (islower(buf[i]))
		buf[i] = toupper(buf[i]);
	}

	if (send_all(client, buf, strlen(buf))) {
	    socket_error("send");
	    closesocket(client);
	    rc = 1;
	    goto out;
	}

	closesocket(client);
    }

out:
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    if (result != NULL) {
        freeaddrinfo(result);
        result = NULL;
    }

#ifdef _WIN32
    WSACleanup();
#endif
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

//
// send -may- return having not sent all of the data, so I use
// a wrapper that doesn't return until there's an error OR all
// of the data has been sent.
//
bool
send_all(SOCKET sockfd, const void *buf, size_t len) {
    const char *cbuf = (const char *)buf;
    ssize_t remaining = len, r;
    while (remaining != 0) {
	r = send(sockfd, cbuf, (int)len, 0);
	if (r == -1)
	    return true;

        remaining -= r;
	len -= r;
	cbuf += r;
    }
    return false;
}
