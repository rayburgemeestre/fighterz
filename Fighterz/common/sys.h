#ifndef _WIN32
/* *NIX ~!~~~~~ */
#define ERRNO errno
#define P_EWOULDBLOCK EWOULDBLOCK
#define P_EAGAIN EAGAIN
#define CLOSE_SOCK(fd) close(fd)
#else
/* Windoze :[ */
#define ERRNO WSAGetLastError()
#define P_EWOULDBLOCK WSAEWOULDBLOCK
#define P_EAGAIN WSAEWOULDBLOCK
#define CLOSE_SOCK(fd) closesocket(fd)
/* MWOEHahahAHA m$ suckzZZZzz */
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#endif
