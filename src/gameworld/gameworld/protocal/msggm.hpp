/******************************************************************************************
FileName: msggm.hpp
Author: dreamwang
Description: GMϵͳ���Э��
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/06/29/ 09:04:05     1.0           ����
*****************************************************************************************/

#ifndef __MSG_GM_HPP__
#define __MSG_GM_HPP__

#include "servercommon/userprotocal/msgheader.h"
#include "gamedef.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	// GM�������ͳ���
	static const int MAX_GM_COMMAND_TYPE_LEN = 64;

	// GM�����
	static const int MAX_GM_COMMAND_LEN = 1024;
	
	// GM�ظ����� 
	static const int MAX_GM_RESULT_LEN = 1024; 

	// GM���� 
	class CSGMCommand
	{
	public:
		CSGMCommand();
		MessageHeader	header;
		char type[MAX_GM_COMMAND_TYPE_LEN];
		char command[MAX_GM_COMMAND_LEN];
	};

	//GM����ظ�
	class SCGMCommand
	{
	public:
		SCGMCommand();
		MessageHeader	header;
		char type[MAX_GM_COMMAND_TYPE_LEN];
		char result[MAX_GM_COMMAND_LEN];
	};

	// ����
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

