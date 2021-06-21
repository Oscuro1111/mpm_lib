#ifndef __MPM__TCP__SOCK
#define __MPM__TCP__SOCK

extern int SetupTCPServerSocket(const char *service);
extern int AcceptTCPConnection(int servSock);

#endif
