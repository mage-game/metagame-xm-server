#ifndef MSG_WUSHUANG_H
#define MSG_WUSHUANG_H

#include "gamedef.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/wushuangdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum WUSHUANG_OPERA_TYPE
	{
		WUSHUANG_OPERA_TYPE_INVALID = 0,
		WUSHUANG_OPERA_TYPE_DAZAO,
		WUSHUANG_OPERA_TYPE_UP_QUALITY,
		WUSHUANG_OPERA_TYPE_JINGLIAN,
		WUSHUANG_OPERA_TYPE_FUMO,
		WUSHUANG_OPERA_TYPE_TAKEOFF_FUMO,
		WUSHUANG_OPERA_TYPE_FUHUN,
		WUSHUANG_OPERA_TYPE_TAKEOFF_FUHUN,
		WUSHUANG_OPERA_TYPE_FUHUN_ADD_EXP,

		WUSHUANG_OPERA_TYPE_MAX,
	};

	class CSWushuangEquipOperaReq									// ��˫װ����������
	{
	public:
		CSWushuangEquipOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param_1;
		int param_2;
		int param_3;
	};

	enum HUNSHOU_OPERA_TYPE
	{
		HUNSHOU_OPERA_TYPE_INVALID,
		HUNSHOU_OPERA_TYPE_CHOUHUN,
		HUNSHOU_OPERA_TYPE_SUPER_CHOUHUN,
		HUNSHOU_OPERA_TYPE_BATCH_HUNSHOU,
		HUNSHOU_OPERA_TYPE_SPECIAL_BATCH_HUNSHOU,
		HUNSHOU_OPERA_TYPE_PUT_BAG,
		HUNSHOU_OPERA_TYPE_CONVERT_TO_EXP,
		HUNSHOU_OPERA_TYPE_MERGE,
		HUNSHOU_OPERA_TYPE_SINGLE_CONVERT_TO_EXP,

		HUNSHOU_OPERA_TYPE_MAX,
	};

	class CSHunshouOperaReq
	{
	public:
		CSHunshouOperaReq();
		MessageHeader header;

		short opera_type;
		short reserve_sh;
		int param_1;
		int param_2;
	};

	enum WUSHUANG_BAG_NOTIFY_REASON
	{
		WUSHUANG_BAG_NOTIFY_REASON_INVALID = 0,
		WUSHUANG_BAG_NOTIFY_REASON_BAG_MERGE,

		WUSHUANG_BAG_NOTIFY_REASON_MAX,
	};

	struct WushuangHunShou
	{
		short hunshou_id;
		short level;
		long long curr_exp;
	};

	class SCWushuangBagInfo
	{
	public:
		SCWushuangBagInfo();
		MessageHeader header;

		int notify_reason;
		long long hunshou_exp;
		char liehun_color;
		char reserve_ch;
		short reserve_sh;

		short liehun_pool[WUSHUANG_LIEHUN_POOL_MAX_COUNT];
		WushuangHunShou grid_list[WUSHUANG_HUNSHOU_BAG_GRID_MAX_COUNT];
	};

	enum WUSHUANG_EQUIP_NOTIFY_REASON
	{
		WUSHUANG_EQUIP_NOTIFY_REASON_DEFAULT = 0,
		WUSHUANG_EQUIP_NOTIFY_REASON_QUALITY_CHANGE,
		WUSHUANG_EQUIP_NOTIFY_REASON_DAZAO_CHANGE,

		WUSHUANG_EQUIP_NOTIFY_REASON_MAX,
	};

	struct WushuangEquip
	{
		UNSTD_STATIC_CHECK(3 == WUSHUANG_FUMO_SLOT_COUNT);

		short dazao_level;
		short quality_level;
		long long dazao_exp;
		int jinglian_attrs[WUSHUANG_JINGLIAN_ATTR_COUNT];
		ItemID fumo_items[WUSHUANG_FUMO_SLOT_COUNT];
		short reserve_sh;
		WushuangHunShou hunshou_list[WUSHUANG_FUHUN_SLOT_COUNT];
	};

	class SCWushuangEquipInfo										// ��˫װ����Ϣ
	{
	public:
		SCWushuangEquipInfo();
		MessageHeader header;

		int notify_reason;

		WushuangEquip equip_list[WUSHUANG_EQUIP_TYPE_MAX_COUNT];
	};

	enum WUSHUANG_FB_KICKOUT_REASON
	{
		WUSHUANG_FB_KICKOUT_REASON_PASSED = 0,
		WUSHUANG_FB_KICKOUT_REASON_TIME_UP = 1,

		WUSHUANG_FB_KICKOUT_REASON_MAX,
	};

	class SCWushuangFBKickOutNotify									// ��˫�����߳�֪ͨ
	{
	public:
		SCWushuangFBKickOutNotify();
		MessageHeader header;

		char notify_reason;
	};

	enum WUSHUANG_FB_REQUEST
	{
		WUSHUANG_FB_REQUEST_INFO = 0,
		WUSHUANG_FB_REQUEST_FETCH_REWARD = 1,

		WUSHUANG_FB_REQUEST_MAX,
	};

	class CSWushuangFBInfo											// ��������
	{
	public:
		CSWushuangFBInfo();
		MessageHeader header;

		char request_type;
	};

	class SCWushuangFBInfo											// ��˫������Ϣ����
	{
	public:
		SCWushuangFBInfo();
		MessageHeader header;

		short pass_level;											// ��ǰ��ͨ�ز���
		char has_fetch_day_reward;									// ���콱����ȡ״̬��0 δ��ȡ��1 ����ȡ��
		char reserve_ch;											
	};

	enum HUNSHOU_OPERA_STATUS_NOTIFY_REASON
	{
		HUNSHOU_OPERA_STATUS_NOTIFY_REASON_DEFAULT = 0, 
		HUNSHOU_OPERA_STATUS_NOTIFY_REASON_CHOUHUN,					// ���
		HUNSHOU_OPERA_STATUS_NOTIFY_REASON_SINGLE_CONVERT_TOEXP,	// ת������ 
		HUNSHOU_OPERA_STATUS_NOTIFY_REASON_PUT_TO_BAG,				// �Ի�ػ��޽�����
	};

	class SCHushouOperaStatus
	{
	public:
		SCHushouOperaStatus();
		MessageHeader header;

		char opera_status;
		char from_place;
		short from_index;
		char to_place;
		char reserve_ch;
		short to_index;
		short hunshou_id;
		short reserve_sh;
	};

	enum ZHUANZHI_FB_NOTIFY_REASON
	{
		ZHUANZHI_FB_NOTIFY_REASON_SUCCESS = 0,						// ͨ�سɹ�
		ZHUANZHI_FB_NOTIFY_REASON_TIME_OUT,							// ͨ�س�ʱʧ��
	};

	class SCZhuanzhiFBNotify
	{
	public:
		SCZhuanzhiFBNotify();
		MessageHeader header;

		char notify_reason;
		char zhuanzhi_fb_type;										// תְ�������ͣ�0 ���ˣ� 1 ���
	};
}

#pragma pack(pop)

#endif

