/******************************************************************************************
FileName: msggm.hpp
Author: dreamwang
Description: GM系统相关协议
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/06/29/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __MSG_GM_HPP__
#define __MSG_GM_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "gamedef.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	// GM命令类型长度
	static const int MAX_GM_COMMAND_TYPE_LEN = 64;

	// GM命令长度
	static const int MAX_GM_COMMAND_LEN = 1024;
	
	// GM回复长度 
	static const int MAX_GM_RESULT_LEN = 1024; 

	// GM命令 
	class CSGMCommand
	{
	public:
		CSGMCommand();
		MessageHeader	header;
		char type[MAX_GM_COMMAND_TYPE_LEN];
		char command[MAX_GM_COMMAND_LEN];
	};

	//GM命令回复
	class SCGMCommand
	{
	public:
		SCGMCommand();
		MessageHeader	header;
		char type[MAX_GM_COMMAND_TYPE_LEN];
		char result[MAX_GM_COMMAND_LEN];
	};

	// 测速
	class CSTestSpeed
	{
	public:
		CSTestSpeed();
		MessageHeader	header;

		int seq;
	};

	class SCTestSpeed
	{
	public:
		SCTestSpeed();
		MessageHeader	header;

		int seq;
	};
}

#pragma pack(pop)

#endif // __MSG_GM_HPP__

