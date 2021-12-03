/******************************************************************************************
FileName: chatserverdef.hpp
Author: dreamwang
Description: 登录服务器相关定义 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/08/08/ 09:04:05     1.0           创建
*****************************************************************************************/
#ifndef __LOGINDEF_H__
#define __LOGINDEF_H__

typedef char	PlatName[64];
typedef char	LoginStr[32];
typedef char	RandConfigName[128];

static const int MAX_THREAD_NUM = 256;
static const int MAX_ROLE_ID_NUM = 3;

#define ROLE_PER_ACCOUNT 3

#endif
