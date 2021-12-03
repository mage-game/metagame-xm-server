#ifndef __MSG_BABY_H__
#define __MSG_BABY_H__

#include "servercommon/babydef.hpp"
#include "servercommon/userprotocal/msgheader.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum BABY_REQ_TYPE
	{
		BABY_REQ_TYPE_INFO = 0,									// ���󵥸�������Ϣ  ����1 �����б�����index
		BABY_REQ_TYPE_ALL_INFO = 1,								// �������б�����Ϣ
		BABY_REQ_TYPE_UPLEVEL = 2,								// ��������	����1 �����б�����index
		BABY_REQ_TYPE_QIFU = 3,									// ������ ����1 ������
		BABY_REQ_TYPE_QIFU_RET = 4,								// ����Ӧ���� ����1 �����ͣ�����2 �Ƿ����
		BABY_REQ_TYPE_CHAOSHENG = 5,							// ��������
		BABY_REQ_TYPE_SPIRIT_INFO = 6,							// ���󵥸��������ػ��������Ϣ
		BABY_REQ_TYPE_TRAIN_SPIRIT = 7,							// ������������
		BABY_REQ_TYPE_REMOVE_BABY = 8,							// ������������ p1:baby_index
		BABY_REQ_TYPE_REMOVE_BABY_RET = 9,						// ��Ӧ�Ƿ��������� p1:req_baby_index p2:1(ͬ��)��0����ͬ�⣩p4:unique_baby_id
		BABY_REQ_TYPE_WALK = 10,								// �ﱦ�� param1 ����Ƿ�idle 0���� 1��
	};

	struct BabySendInfo
	{
		short baby_index;

		short baby_id;
		short rename_times;
		short grade;
		short level;
		short bless;

		GameName baby_name;

		GameName lover_name;

		BabySpiritInfo baby_spirit_list[BABY_SPIRIT_COUNT];
	};

	class CSBabyOperaReq										// ��������  2417
	{
	public:
		CSBabyOperaReq();
		MessageHeader header;

		short opera_type;
		short param_0;
		int param_1;
		int param_2;
		int param_3;

		long long param_4;
	};

	class CSBabyUpgradeReq										// ������������  2418
	{
	public:
		CSBabyUpgradeReq();
		MessageHeader header;

		short baby_index;
		short repeat_times;
		short auto_buy;
		short is_auto_upgrade;
	};

	class CSBabyRenameReq										// ������������ 2419
	{
	public:
		CSBabyRenameReq();
		MessageHeader header;

		short baby_index;
		short reserve_sh;

		GameName newname;
	};

	enum BABY_INFO_TYPE
	{
		BABY_INFO_TYPE_REQUESET_CREATE_BABY = 0,					// ���� ���������� p1:type
		BABY_INFO_TYPE_REMOVE_BABY_REQ,								// ������������ p1:req_baby_index, p2:unique_baby_id
	};

	class SCBabyBackInfo										// ���� ���ͷ�����Ϣ  2414
	{
	public:
		SCBabyBackInfo();
		MessageHeader header;

		short info_type;
		short param_1;
		long long param_2;
	};

	class SCBabyInfo											//������ ������Ϣ  2413
	{
	public:
		SCBabyInfo();
		MessageHeader header;

		BabySendInfo baby_send;
	};

	class SCBabyAllInfo											//������ ���б�����Ϣ  2415
	{
	public:
		SCBabyAllInfo();
		MessageHeader header;

		BabySendInfo baby_list[BABY_MAX_COUNT];

		int baby_chaosheng_count;
	};

	class SCBabySpiritInfo										// ���� �ػ�������Ϣ 2416
	{
	public:
		SCBabySpiritInfo();
		MessageHeader header;

		int baby_index;

		BabySpiritInfo baby_spirit_list[BABY_SPIRIT_COUNT];
	};

	class SCBabyWalk				// 8061 �ﱦ��
	{
	public:
		SCBabyWalk();
		MessageHeader header;

		ObjID obj_id;
		short is_special_baby;  // �Ƿ����ﱦ�� 1�� 0��
		int baby_index;
		GameName baby_name;
	};
}

#pragma pack(pop)

#endif // __MSG_BABY_H__


