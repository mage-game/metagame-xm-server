#ifndef __MSG_MAGIC_EQUIPMENT_H__
#define __MSG_MAGIC_EQUIPMENT_H__

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/magiccarddef.hpp"
#include "servercommon/magicequipmentdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum MAGIC_EQUIPMENT_REQ_TYPE
	{
		MAGIC_EQUIPMENT_REQ_TYPE_UPGRADE = 0,								// ���ɽ��ף�param1 ħ�����ͣ�param2 ��������
		MAGIC_EQUIPMENT_REQ_TYPE_STRENGTHEN,								// ����ǿ����param1 ħ�����ͣ�param2 �Ƿ��Զ�ǿ���� param3 �Ƿ��Զ�����
		MAGIC_EQUIPMENT_REQ_TYPE_EMBED,										// ��Ƕħʯ��param1 ħ�����ͣ�param2 ��Ƕ��λ��param3 ħʯ�±꣨������ģ�
		MAGIC_EQUIPMENT_REQ_TYPE_TAKE_OFF_STONE,							// ж��ħʯ��param1 ħ�����ͣ�param2 ��Ƕ��λ

		MAGIC_EQUIPMENT_REQ_TYPE_MAX,
	};

	class CSMagicEquipmentOperaReq											// ħ����������   5690
	{
	public:
		CSMagicEquipmentOperaReq();
		MessageHeader header;

		short opera_type;
		short param_1;
		short param_2;
		short param_3;
	};

	class SCMagicEquipmentAllInfo											// ħ��������Ϣ  6141
	{
	public:
		struct SubItem
		{
			short quality_progress;											// ���ɽ���
			short quality_level;											// Ʒ�ʵȼ�

			int strengthen_bless;											// ǿ��ף��ֵ
			short strengthen_level;											// ǿ���ȼ�
			short stone_list[MAGIC_EQUIP_STONE_SLOT_COUNT];					// ��Ƕ��ħʯ�б�
			short reserve_sh;
		};

		SCMagicEquipmentAllInfo();
		MessageHeader header;

		SubItem equip_list[MAGIC_EQUIP_MAX_COUNT];
	};

	enum MAGIC_EQUIPMENT_CHANGE_NOTIFY_TYPE
	{
		MAGIC_EQUIPMENT_CHANGE_NOTIFY_TYPE_QUALITY_LEVEL = 0,				// Ʒ�ʵȼ��ı䣺param1 ħ�����ͣ�param2 ħ��Ʒ�ʵȼ��� param3 ���ɽ���
		MAGIC_EQUIPMENT_CHANGE_NOTIFY_TYPE_STRENGTHEN_LEVEL,				// ����ȼ��ı䣺param1 ħ�����ͣ�param2 ħ������ȼ��� param3 ����ֵ��ף��ֵ��
		MAGIC_EQUIPMENT_CHANGE_NOTIFY_TYPE_EMBED,							// ��Ƕħʯ��param1 ħ�����ͣ�param2 ħʯ��λ�� param3 ħʯ�±꣨������ģ�
		MAGIC_EQUIPMENT_CHANGE_NOTIFY_TYPE_TAKE_OFF,						// ����ħʯ��param1 ħ�����ͣ�param2 ħʯ��λ
		
		MAGIC_EQUIPMENT_CHANGE_NOTIFY_TYPE_MAX,
	};

	class SCMagicEquipmentChangeNotify										// ħ����Ϣ��������  6142
	{
	public:
		SCMagicEquipmentChangeNotify();
		MessageHeader header;

		short notify_type;
		short reserve_sh;
		int param_1;
		int param_2;
		int param_3;
	};
}

#pragma pack(pop)

#endif

