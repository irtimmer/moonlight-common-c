#pragma once

#include "Limelight.h"
#include "Platform.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <wlanapi.h>
#include <timeapi.h>
#define SetLastSocketError(x) WSASetLastError(x)
#define LastSocketError() WSAGetLastError()

#define SHUT_RDWR SD_BOTH

// errno.h will include incompatible definitions of these
// values compared to what Winsock uses, so we must undef
// them to ensure the correct value is used.

#ifdef EWOULDBLOCK
#undef EWOULDBLOCK
#endif
#define EWOULDBLOCK WSAEWOULDBLOCK

#ifdef EAGAIN
#undef EAGAIN
#endif
#define EAGAIN WSAEWOULDBLOCK

#ifdef EINPROGRESS
#undef EINPROGRESS
#endif
#define EINPROGRESS WSAEINPROGRESS

#ifdef EINTR
#undef EINTR
#endif
#define EINTR WSAEINTR

#ifdef ETIMEDOUT
#undef ETIMEDOUT
#endif
#define ETIMEDOUT WSAETIMEDOUT

typedef int SOCK_RET;
typedef int SOCKADDR_LEN;

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#if defined(__vita__)
#define POLLIN      0x0001
#define POLLPRI     0x0002
#define POLLOUT     0x0004
#define POLLERR     0x0008
#define POLLRDNORM  0x0040
#define POLLWRNORM  POLLOUT
#define POLLRDBAND  0x0080
#define POLLWRBAND  0x0100

struct pollfd {
    int fd;
    short events;
    short revents;
};
#else
#include <poll.h>
#endif

#define ioctlsocket ioctl
#define LastSocketError() errno
#define SetLastSocketError(x) errno = x
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

typedef int SOCKET;
typedef ssize_t SOCK_RET;
typedef socklen_t SOCKADDR_LEN;
#endif

#define LastSocketFail() ((LastSocketError() != 0) ? LastSocketError() : -1)

// IPv6 addresses have 2 extra characters for URL escaping
#define URLSAFESTRING_LEN (INET6_ADDRSTRLEN+2)
void addrToUrlSafeString(struct sockaddr_storage* addr, char* string);

SOCKET createSocket(int addressFamily, int socketType, int protocol, bool nonBlocking);
SOCKET connectTcpSocket(struct sockaddr_storage* dstaddr, SOCKADDR_LEN addrlen, unsigned short port, int timeoutSec);
int sendMtuSafe(SOCKET s, char* buffer, int size);
SOCKET bindUdpSocket(int addrfamily, int bufferSize);
int enableNoDelay(SOCKET s);
int setSocketNonBlocking(SOCKET s, bool enabled);
int recvUdpSocket(SOCKET s, char* buffer, int size, bool useSelect);
void shutdownTcpSocket(SOCKET s);
int setNonFatalRecvTimeoutMs(SOCKET s, int timeoutMs);
void setRecvTimeout(SOCKET s, int timeoutSec);
void closeSocket(SOCKET s);
bool isPrivateNetworkAddress(struct sockaddr_storage* address);
int pollSockets(struct pollfd* pollFds, int pollFdsCount, int timeoutMs);

#define TCP_PORT_MASK 0xFFFF
#define TCP_PORT_FLAG_ALWAYS_TEST 0x10000
int resolveHostName(const char* host, int family, int tcpTestPort, struct sockaddr_storage* addr, SOCKADDR_LEN* addrLen);

void enterLowLatencyMode(void);
void exitLowLatencyMode(void);

int initializePlatformSockets(void);
void cleanupPlatformSockets(void);
