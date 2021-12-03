
#pragma once

#ifndef PISOCKET_H
#define PISOCKET_H

//---------------------------------------------------------------------
// Unix ͷ������
//---------------------------------------------------------------------
#if defined(__unix)
#include <netinet/in.h>
#include <asm/ioctls.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>

typedef int SOCKET;
static const int SOCKET_ERROR = -1;
static const int INVALID_SOCKET = -1;
#define PI_SD_BOTH			SHUT_RDWR
#define PI_SD_READ			SHUT_RD
#define PI_SD_WRITE			SHUT_WR


//---------------------------------------------------------------------
// Windows ͷ������
//---------------------------------------------------------------------
#elif defined(_WIN32)

#ifdef FD_SETSIZE
#error "please include this file before 'WinSock2.h' or 'Windows.h'"
#else
#define FD_SETSIZE 1024
#endif

#include <WinSock2.h>

//#define EWOULDBLOCK			WSAEWOULDBLOCK // ������Ŀû�õ���ע�͵�ȡ������
#define PI_SD_BOTH			SD_BOTH
#define PI_SD_READ			SD_RECEIVE
#define PI_SD_WRITE			SD_SEND

#pragma comment(lib, "WS2_32")

#endif



namespace PISocket
{
	// ��ʼ����
	int Startup();

	// ��������
	int Cleanup();

	// ��ʼ���׽���
	SOCKET Socket(int family, int type, int protocol);

	// �ر��׽���
	int Close(SOCKET sock);

	// ����Ŀ���ַ
	int Connect(SOCKET sock, const struct sockaddr *addr);

	// ֹͣ�׽���
	int Shutdown(SOCKET sock, int mode);

	// �󶨶˿�
	int Bind(SOCKET sock, const struct sockaddr *addr);

	// ������Ϣ
	int Listen(SOCKET sock, int count);

	// ��������
	SOCKET Accept(SOCKET sock, struct sockaddr *addr);

	// ��ȡ������Ϣ
	int Errno(void);

	// ������Ϣ
	int Send(SOCKET sock, const void *buf, int size, int mode);

	// ������Ϣ
	int Recv(SOCKET sock, void *buf, int size, int mode);

	// �������׽��ַ�����Ϣ
	int SendTo(SOCKET sock, const void *buf, int size, int mode, const struct sockaddr *addr);

	// �������׽��ֽ�����Ϣ
	int RecvFrom(SOCKET sock, void *buf, int size, int mode, struct sockaddr *addr);

	// ����ioctlsocket����������������
	int Ioctl(SOCKET sock, long cmd, unsigned long *argp);

	// �����׽��ֲ���
	int SetSockopt(SOCKET sock, int level, int optname, const char *optval, int optlen);

	// ��ȡ�׽��ֲ���
	int GetSockopt(SOCKET sock, int level, int optname, char *optval, int *optlen);

	// ȡ���׽��ֵ�ַ
	int SockName(SOCKET sock, struct sockaddr_in *addr);

	// ȡ���׽��������ӵ�ַ
	int PeerName(SOCKET sock, struct sockaddr_in *addr);

	// ��������ת���ɶ�Ӧ�ַ���
	char *GetErrStr(int errnum, char *msg, int size);

}






#endif
