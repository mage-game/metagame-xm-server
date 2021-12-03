
#pragma once

#ifndef PISOCKET_H
#define PISOCKET_H

//---------------------------------------------------------------------
// Unix 头部定义
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
// Windows 头部定义
//---------------------------------------------------------------------
#elif defined(_WIN32)

#ifdef FD_SETSIZE
#error "please include this file before 'WinSock2.h' or 'Windows.h'"
#else
#define FD_SETSIZE 1024
#endif

#include <WinSock2.h>

//#define EWOULDBLOCK			WSAEWOULDBLOCK // 整个项目没用到，注释掉取消警告
#define PI_SD_BOTH			SD_BOTH
#define PI_SD_READ			SD_RECEIVE
#define PI_SD_WRITE			SD_SEND

#pragma comment(lib, "WS2_32")

#endif



namespace PISocket
{
	// 开始网络
	int Startup();

	// 结束网络
	int Cleanup();

	// 初始化套接字
	SOCKET Socket(int family, int type, int protocol);

	// 关闭套接字
	int Close(SOCKET sock);

	// 连接目标地址
	int Connect(SOCKET sock, const struct sockaddr *addr);

	// 停止套接字
	int Shutdown(SOCKET sock, int mode);

	// 绑定端口
	int Bind(SOCKET sock, const struct sockaddr *addr);

	// 监听消息
	int Listen(SOCKET sock, int count);

	// 接收连接
	SOCKET Accept(SOCKET sock, struct sockaddr *addr);

	// 获取错误信息
	int Errno(void);

	// 发送消息
	int Send(SOCKET sock, const void *buf, int size, int mode);

	// 接收消息
	int Recv(SOCKET sock, void *buf, int size, int mode);

	// 非连接套接字发送消息
	int SendTo(SOCKET sock, const void *buf, int size, int mode, const struct sockaddr *addr);

	// 非连接套接字接收消息
	int RecvFrom(SOCKET sock, void *buf, int size, int mode, struct sockaddr *addr);

	// 调用ioctlsocket，设置输出输入参数
	int Ioctl(SOCKET sock, long cmd, unsigned long *argp);

	// 设置套接字参数
	int SetSockopt(SOCKET sock, int level, int optname, const char *optval, int optlen);

	// 读取套接字参数
	int GetSockopt(SOCKET sock, int level, int optname, char *optval, int *optlen);

	// 取得套接字地址
	int SockName(SOCKET sock, struct sockaddr_in *addr);

	// 取得套接字所连接地址
	int PeerName(SOCKET sock, struct sockaddr_in *addr);

	// 将错误码转换成对应字符串
	char *GetErrStr(int errnum, char *msg, int size);

}






#endif
