#ifndef _MSG_TIANSHENHUTI_H_
#define _MSG_TIANSHENHUTI_H_

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/tianshenhutidef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCTianshenhutiALlInfo												// ��ĩװ��װ����Ϣ  8628
	{
	public:
		SCTianshenhutiALlInfo();
		MessageHeader header;

		TianshenEquipID equip_list[TIANSHENHUTI_EQUIP_MAX_COUNT];			// ÿ����λ��Ӧװ��ID

		short used_free_times;												// �Ѿ�ʹ�õ���Ѵ���
		unsigned short backpack_num;										// ������װ����������Ч���鳤�ȣ�
		int roll_score;														// �齱����
		unsigned int next_free_roll_time;									// �´ο�����ѳ齱ʱ�� �����жϴ��� ���Ƿ������� ���ж�ʱ�� �Ƿ���ʾ����ʱ��
		short accumulate_roll_times;										// �ۼƳ齱����
		short reserved_sh;													// Ԥ��λ
		TianshenEquipID backpack_list[TIANSHENHUTI_BACKPACK_MAX_COUNT];		// ������ӵ�е�����װ���б�

	};

	const static int TIANSHENHUTI_BATCH_ROLL_TIMES = 5;

	class SCTianshenhutiRollResult											// ��ĩװ���齱���  8629
	{
	public:
		SCTianshenhutiRollResult();
		MessageHeader header;

		short reward_count;
		TianshenEquipID reward_list[TIANSHENHUTI_BATCH_ROLL_TIMES];

		UNSTD_STATIC_CHECK(5 == TIANSHENHUTI_BATCH_ROLL_TIMES);
	};

	enum CS_TIANSHENHUTI_REQ_TYPE
	{
		CS_TIANSHENHUTI_REQ_TYPE_PUT_ON = 0,								// ����װ��-�����±�
		CS_TIANSHENHUTI_REQ_TYPE_TAKE_OFF,									// ����װ��-װ����λ�±�
		CS_TIANSHENHUTI_REQ_TYPE_TRANSFORM,									// ת��װ��-2�������±�
		CS_TIANSHENHUTI_REQ_TYPE_COMBINE,									// �ϳ�װ��-3�������±�
		CS_TIANSHENHUTI_REQ_TYPE_ROLL,										// ������齱
		CS_TIANSHENHUTI_REQ_TYPE_COMBINE_ONE_KEY,							// һ���ϳ�װ�� p1 ѡ�еȼ�

		CS_TIANSHENHUTI_REQ_TYPE_MAX,
	};

	class SCTianshenhutiReqResult											// ��ĩװ����������� 8630
	{
	public:
		SCTianshenhutiReqResult();
		MessageHeader header;

		unsigned short req_type;											// CS_TIANSHENHUTI_REQ_TYPE
		unsigned short param_1;
		TianshenEquipID new_equip_id;
		unsigned short reserve_sh;
	};

	class CSTianshenhutiReq													// ��ĩװ��װ������  8631
	{
	public:
		CSTianshenhutiReq();
		MessageHeader header;

		unsigned short req_type;											// CS_TIANSHENHUTI_REQ_TYPE
		unsigned short param_1;
		unsigned short param_2;
		unsigned short param_3;

		unsigned short param_4;
		unsigned short reserve_sh;
	};

	class SCTianshenhutiScoreChange											//  8632 ��ĩװ�����ֱ䶯
	{
	public:
		SCTianshenhutiScoreChange();
		MessageHeader header;

		int roll_score;
	};

	class SCTianshenhutiCombineOneKeyResult									// ��ĩװ��һ���ϳɷ��� 8633
	{
	public:
		SCTianshenhutiCombineOneKeyResult();
		MessageHeader header;

		static const int MAX_COMBINE_NUM = 40;

		int combine_count;
		TianshenEquipID new_equip_id[MAX_COMBINE_NUM];
	};

}

#pragma pack(pop)

#endif

