#include "serialize_base.hpp"

#include "engineadapter.h"
#include "serverlogic.h"
#include "static_byte_array.hpp"

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/userprotocal/msgcode.h"
#include "gamelog.h"

MsgSerializeManager & MsgSerializeManager::GetInstance()
{
	static MsgSerializeManager instance;
	return instance;
}

MsgSerializeManager::MsgSerializeManager()
{
	memset(m_auto_protocol_map, 0, sizeof(m_auto_protocol_map));
}

MsgSerializeManager::~MsgSerializeManager()
{
	for (int i = 0; i < Protocol::AUTO_PTOROCOL_MAX_MSG_COUNT; ++i)
	{
		if (NULL != m_auto_protocol_map[i])
		{
			delete m_auto_protocol_map[i];
		}
	}
}

bool MsgSerializeManager::NetSend(const GSNetID &gsnetid, const Protocol::MsgSerializeBase &msg)
{
	static Protocol::MessageHeader header;
	header.msg_type = msg.GetMsgType();

	static StaticByteArray s_bytearray;
	s_bytearray.Clear();
	s_bytearray.WriteWriteObject(reinterpret_cast<char *>(&header), sizeof(header)); // 写入头部

	msg.Serialize(s_bytearray); // 写入序列化内容

	unsigned int all_byte_len = s_bytearray.BytesAvailable();

	return EngineAdapter::Instance().NetSend(gsnetid, s_bytearray.ReadByteData(all_byte_len), all_byte_len);
}

bool MsgSerializeManager::NetMulSend(GSNetID *netidlist, int num, const Protocol::MsgSerializeBase &msg)
{
	static Protocol::MessageHeader header;
	header.msg_type = msg.GetMsgType();

	static StaticByteArray s_bytearray;
	s_bytearray.Clear();
	s_bytearray.WriteWriteObject(reinterpret_cast<char *>(&header), sizeof(header)); // 写入头部

	msg.Serialize(s_bytearray); // 写入序列化内容
	unsigned int all_byte_len = s_bytearray.BytesAvailable();

	return EngineAdapter::Instance().NetMulSend(netidlist, num,s_bytearray.ReadByteData(all_byte_len), all_byte_len);
}

Protocol::MsgSerializeBase * MsgSerializeManager::GetSerializeObj(unsigned short msg_type)
{
	int index = msg_type - Protocol::MT_AUTO_PROTOCOL_RESERVE_MIN;
	if (index < 0 || index >= Protocol::AUTO_PTOROCOL_MAX_MSG_COUNT)
		return NULL;

	return m_auto_protocol_map[index];
}

void MsgSerializeManager::RegisterSerialize(unsigned short msg_type, Protocol::MsgSerializeBase *msg)
{
	int index = msg_type - Protocol::MT_AUTO_PROTOCOL_RESERVE_MIN;
	if (index < 0 || index >= Protocol::AUTO_PTOROCOL_MAX_MSG_COUNT)
	{
		gamelog::g_log_new_protocol.printf(LL_INFO, "[Error] MsgSerializeManager::RegisterSerialize msg_type(%d) is invalid", msg_type);

		return;
	}

	if (NULL != msg && msg_type != msg->GetMsgType())
	{
		return;
	}

	if (NULL != m_auto_protocol_map[index])
	{
		// 有协议号重复，不给启动服务器，让服务端开发期就可以发现错误
		gamelog::g_log_new_protocol.printf(LL_INFO, "[Error] MsgSerializeManager::RegisterSerialize msg_type(%d) repeat", msg_type);
		
		//printf("[Error] MsgSerializeManager::RegisterSerialize msg_type(%d) repeat\n", msg_type);
		ServerLogic::GetInstServerLogic()->StopServer();
		return;
	}

	m_auto_protocol_map[index] = msg;
}
