// ��������Э��

#ifndef MSG_SHENGXINGZHULI_H
#define MSG_SHENGXINGZHULI_H

#include "servercommon/userprotocal/msgheader.h"
// #include "config/gameconfigcommon/gameconfigcommon.h"

#pragma pack(push) 
#pragma pack(4)


namespace Protocol
{
	class CSGetShengxingzhuliInfoReq	// 8700 �ͻ�����������������
	{
	public:
		CSGetShengxingzhuliInfoReq();
		MessageHeader header;
	};

	class SCGetShengxingzhuliInfoAck	// 8701 ��������������Ļظ�
	{
	public:
		SCGetShengxingzhuliInfoAck();
		MessageHeader header;

		int today_reward_fetch_stage;		// �Ƿ��Ѿ���ȡ���շݽ���,Ϊ1��ʾ����ȡ
		int chognzhi_today;				// ����봵Ľ�
		int func_level;					// ��Ӧϵͳ�ĵȼ�
		int func_type;					// ��Ӧ��ϵͳ
		int is_max_level;				// ��Ӧϵͳ�Ƿ���߼���0��ʾ������߼���1��ʾ����߼�
		int today_reward_stage;						// ��ǰ��λ
	};

	class CSGetShengxingzhuliRewardReq	// 8710 �ͻ���������ȡ���������Ľ���
	{
	public:
		CSGetShengxingzhuliRewardReq();
		MessageHeader header;
	};

	class SCGetShengxingzhuliRewardAck	// 8711 ����˻ظ���ȡ���������Ľ���
	{
	public:
		SCGetShengxingzhuliRewardAck();
		MessageHeader header;

		int is_succ;					// �Ƿ�ɹ���ȡ��Ϊ1��ʾ����ȡ
	};
}

#pragma pack(pop)
#endif