#pragma once

struct GSNetID;
class StaticByteArray;

namespace Protocol {

	// ������������msgcode���ƶ������ģ�������߲鿴���ǵ�Ҫ��MT_AUTO_PROTOCOL_RESERVE_MIN,MT_AUTO_PROTOCOL_RESERVE_MAXһ��
	static const int AUTO_PROTOCOL_RESERVE_MIN = 11000;				// �Զ����ɵ�Э��ռ��Э��Σ���ʽЭ�鲻Ҫռ��
	static const int AUTO_PROTOCOL_RESERVE_MAX = 13999;				// �Զ����ɵ�Э��ռ��Э��Σ���ʽЭ�鲻Ҫռ��

	static const int AUTO_PTOROCOL_MAX_MSG_COUNT = AUTO_PROTOCOL_RESERVE_MAX - AUTO_PROTOCOL_RESERVE_MIN + 1;

	/*
	 * Э�����л����࣬����һ�㶼���Զ����ɵĴ���
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
 * Э�����л����������
 */
class MsgSerializeManager
{
public:
	static MsgSerializeManager & GetInstance();
	~MsgSerializeManager();

	bool NetSend(const GSNetID &gsnetid, const Protocol::MsgSerializeBase &msg);		// �������л�����ָ������
	bool NetMulSend(GSNetID *netidlist, int num, const Protocol::MsgSerializeBase &msg);
	Protocol::MsgSerializeBase * GetSerializeObj(unsigned short msg_type);				// ��ȡ����Ӧ�����л�������������ǹ��������Ψһ�����ⲿ��Ҫ�ͷŸö����ڴ�
	void RegisterSerialize(unsigned short msg_type, Protocol::MsgSerializeBase *msg);	// ע��һ������ָ��Э��ţ�һ�����Զ����ɵĴ���㶨����Ҫ��������

protected:
	MsgSerializeManager();

	Protocol::MsgSerializeBase *m_auto_protocol_map[Protocol::AUTO_PTOROCOL_MAX_MSG_COUNT];
};

#define SerializeMgr MsgSerializeManager::GetInstance()
