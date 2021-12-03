#pragma once

struct GSNetID;
class StaticByteArray;

namespace Protocol {

	// 这两个变量从msgcode里移动过来的，方便这边查看，记得要跟MT_AUTO_PROTOCOL_RESERVE_MIN,MT_AUTO_PROTOCOL_RESERVE_MAX一致
	static const int AUTO_PROTOCOL_RESERVE_MIN = 11000;				// 自动生成的协议占用协议段，旧式协议不要占用
	static const int AUTO_PROTOCOL_RESERVE_MAX = 13999;				// 自动生成的协议占用协议段，旧式协议不要占用

	static const int AUTO_PTOROCOL_MAX_MSG_COUNT = AUTO_PROTOCOL_RESERVE_MAX - AUTO_PROTOCOL_RESERVE_MIN + 1;

	/*
	 * 协议序列化基类，子类一般都是自动生成的代码
	 */
	class MsgSerializeBase
	{
	public:
		MsgSerializeBase() {}
		virtual ~MsgSerializeBase() {}

		virtual int GetMsgType() const = 0;
		virtual void Serialize(StaticByteArray &collector) const = 0;
		virtual void Unserialize(StaticByteArray &collector) = 0;
	};

#define DEFINE_PROTOCOL_DESCRIBE(PROTO_CLASS_NAME) \
	class __##PROTO_CLASS_NAME##Describe__\
	{\
	public:\
		__##PROTO_CLASS_NAME##Describe__() { MsgSerializeManager::GetInstance().RegisterSerialize(PROTO_CLASS_NAME::MSG_TYPE, new PROTO_CLASS_NAME()); }\
		virtual ~__##PROTO_CLASS_NAME##Describe__() {}\
	};

#define CREATE_PROTOCOL_DESCRIBE(PROTO_CLASS_NAME) \
	__##PROTO_CLASS_NAME##Describe__ for_describe_register_to___##PROTO_CLASS_NAME##describe__;

}

/*
 * 协议序列化对象管理器
 */
class MsgSerializeManager
{
public:
	static MsgSerializeManager & GetInstance();
	~MsgSerializeManager();

	bool NetSend(const GSNetID &gsnetid, const Protocol::MsgSerializeBase &msg);		// 发送序列化对象到指定网络
	bool NetMulSend(GSNetID *netidlist, int num, const Protocol::MsgSerializeBase &msg);
	Protocol::MsgSerializeBase * GetSerializeObj(unsigned short msg_type);				// 获取到对应的序列化对象，这个对象是管理器里的唯一对象，外部不要释放该对象内存
	void RegisterSerialize(unsigned short msg_type, Protocol::MsgSerializeBase *msg);	// 注册一个对象到指定协议号，一般由自动生成的代码搞定，不要主动调用

protected:
	MsgSerializeManager();

	Protocol::MsgSerializeBase *m_auto_protocol_map[Protocol::AUTO_PTOROCOL_MAX_MSG_COUNT];
};

#define SerializeMgr MsgSerializeManager::GetInstance()
