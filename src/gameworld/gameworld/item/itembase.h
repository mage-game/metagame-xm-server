#ifndef __ITEMBASE_H__
#define __ITEMBASE_H__

#include <string>
#include <map>

#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"

struct ItemParamDataStruct;
class Role;
struct ItemDataWrapper;
struct ItemGridData;
struct EquipSpecialParamStruct;

class ItemParam
{
public:
	enum ITEMPARAM_TYPE
	{
		PARAM_TYPE_INVALID = 0,
		PARAM_TYPE_NORMAL_EQUIP,					// һ��װ��
	};

	ItemParam(short _param_type) : param_type(_param_type) {}
	virtual ~ItemParam() {}

	virtual bool SetInStructData(ItemParamDataStruct *param_data) const { return false; }
	virtual bool GetSpecialParam(EquipSpecialParamStruct *_special_param) const { return false; }

	short GetParamType() const { return param_type; }

protected:

	short param_type;
};

class ItemBase
{
public:
	static const UInt16 INVALID_ITEM_ID = 0;
	static const int MAX_EQUIP_LEVEL = 200;			// ���װ���ȼ�
	static const int QUALITY_PER_COLOR = 3;			//ÿ��Ʒ��֮��ĵȼ���

	enum ITEM_COLOR
	{
		I_COLOR_INVALID = 0,

		I_COLOR_GREEN = 1,								// ��
		I_COLOR_BLUE = 2,								// ��
		I_COLOR_PURPLE = 3,								// ��
		I_COLOR_ORANGE = 4,								// ��
		I_COLOR_RED = 5,								// ��
		I_COLOR_PINK = 6,								// ��

		I_COLOR_MAX,
	};

	enum ITEM_QUALITY
	{
		I_QUALITY_MIN = 0,
		I_QUALITY_GREEN = I_QUALITY_MIN,		// ��
		I_QUALITY_BLUE,							// ��
		I_QUALITY_PURPLE,						// ��
		I_QUALITY_ORANGE,						// ��
		I_QUALITY_RED,							// ��
		//I_QUALITY_GOLD,							// ��

		I_QUALITY_MAX,
	};

	enum ITEM_TYPE
	{
		I_TYPE_INVALID = -1,						// -1 ��Ч

		I_TYPE_EXPENSE = 0,							// 0 ������ ��ֱ��ʹ��
		I_TYPE_OTHER,								// 1 ����ʹ���� type value ��ò�Ҫֱ��ʹ��
		I_TYPE_GIFT,								// 2 �����

		I_TYPE_EQUIPMENT = 100,						// װ������
	};


	enum ITEM_SEARCH_TYPE							// û�õ� �����ڶ���
	{
		I_SEARCH_TYPE_WEAPON_1 = 1,					// 
		I_SEARCH_TYPE_WEAPON_2,						// 
		I_SEARCH_TYPE_WEAPON_3,						// 
		I_SEARCH_TYPE_WEAPON_4,						// 

		I_SEARCH_TYPE_TOUKUI = 20,					// ͷ��
		I_SEARCH_TYPE_YIFU,							// �·�
		I_SEARCH_TYPE_HUJIAN,						// ����
		I_SEARCH_TYPE_YAODAI,						// ����
		I_SEARCH_TYPE_HUTUI,						// ����
		I_SEARCH_TYPE_XIEZI,						// Ь��
		I_SEARCH_TYPE_HUWAN,						// ����
		I_SEARCH_TYPE_HUSHOU,						// ����
		
		I_SEARCH_TYPE_XIANGLIANG = 30,				// ����
		I_SEARCH_TYPE_HUFU,							// ����
		I_SEARCH_TYPE_JIEZHI,						// ��ָ
		I_SEARCH_TYPE_SHIZHUANG,					// ʱװ

		I_SEARCH_TYPE_QIANGHUASHI = 40,				// ǿ��ʯ
		I_SEARCH_TYPE_XILIANSHI,					// ϴ��ʯ
		I_SEARCH_TYPE_JINGLIAN,						// ��������
		I_SEARCH_TYPE_JINJIE,						// ���ײ���
		I_SEARCH_TYPE_SUIPIAN,						// ��Ƭ
		I_SEARCH_TYPE_QIANZHUIPINZHI,				// ǰ׺Ʒ��
		I_SEARCH_TYPE_XIANGQIANSHI,					// ��Ƕʯ

		I_SEARCH_TYPE_XUANTIESUIPIAN = 50,			// ǿ����
		I_SEARCH_TYPE_BAOHUFU,						// ������

		I_SEARCH_TYPE_NO_USE = 60,					// ��ʱ����
		I_SEARCH_TYPE_CHONGWUDAN,					// ���ﵰ
		I_SEARCH_TYPE_CHONGWUJINENG,				// ���＼��
		I_SEARCH_TYPE_CHONGWULIANGSHI,				// ������ʳ
		I_SEARCH_TYPE_CHONGWUCHENGZHANG,			// ����ɳ�
		I_SEARCH_TYPE_CHONGWUQIANNENG,				// ����Ǳ��

		I_SEARCH_GOLD = 70,							// Ԫ��
		I_SEARCH_TONGQIAN,							// ͭǮ

		I_SEARCH_HP = 80,							// ��Ѫ
		I_SEARCH_MP,								// ����
		I_SEARCH_HPPACKAGE,							// ��Ѫ��
		I_SEARCH_MPPACKAGE,							// ������

		I_SEARCH_MOUNT_CARD = 90,					// ���￨
		I_SEARCH_MOUNT_GROW,						// ����ɳ�

		I_SEARCH_TYPE_BANGPAI = 100,				// ����
		I_SEARCH_TYPE_TASK,							// ����
		I_SEARCH_TYPE_HUODONG,						// �
		I_SEARCH_TYPE_OTHER,						// ����
		I_SEARCH_TYPE_YUANSHEN,						// Ԫ��

		I_SEARCH_SKILLBOOK_PROF1 = 110,				// ְҵ1������
		I_SEARCH_SKILLBOOK_PROF2,					// ְҵ2������
		I_SEARCH_SKILLBOOK_PROF3,					// ְҵ3������
		I_SEARCH_SKILLBOOK_PROF4,					// ְҵ4������
	};

	enum NOT_DISCARD_REASON							// ���ɶ�������
	{
		NDR_DEFAULT,
		NDR_INLAY,									// ����Ƕ
	};

	ItemBase(short item_type);
	virtual ~ItemBase();

	short GetItemType() const { return m_item_type; }
	UInt16 GetItemId() const { return m_item_id; }
	const char * GetItemName() const { return m_item_name; }
	short GetSearchType() const { return m_search_type; }
	short GetColor() const { return m_color; }
	short GetLimitLevel() const { return m_limit_level; }
	short GetLimitProf() const { return m_limit_prof; }
	short GetLimitSex() const { return m_limit_sex; }
	short GetPileLimit() const { return m_pile_limit; }

	bool HasInvalidTime() const;
	bool IsCurDayValid() const { return m_is_curday_valid; }
	UInt32 GetTimeLength() const { return m_time_length; }
	UInt32 CalInvalidTime() const;

	bool IsRecord() const { return m_is_record; }
	bool IsBroadcast() const {return m_is_broadcast; }
	bool IsBind() const { return m_is_bind; }
	ItemID GetOtherSexItemId() const { return m_other_sex_itemid; }

	virtual bool Init(const char* path, std::string *err) = 0;
	virtual int Use(Role *role, const ItemGridData &item_data, short &num, short equip_index = -1) const = 0;

	virtual int GetSellPrice(ItemParam *param) const { return m_sell_price; }
	virtual int GetRecyclType(ItemParam *param) const { return m_recycl_type; }
	virtual int GetRecyclGet(ItemParam *param) const { return m_recycl_get; }
	virtual bool CanDiscard(ItemParam *param, int *reason) const;
	virtual bool CanSell(ItemParam *param) const { return m_can_sell; }

	virtual ItemParam * CreateParam() const { return 0; }
	virtual bool SetParamFromStruct(ItemParam *param, const ItemParamDataStruct *equip_data) const { return false; }
	virtual ItemParam * CreateParamFromStruct(const ItemParamDataStruct *equip_data) const { return 0; }

	static bool CanMerge(ItemID item_id);
	static bool CanTrade(const ItemGridData *item_data);

	virtual bool CheckIsSameItemExceptSex(const ItemBase *other_item) const;

protected:
	const short m_item_type;
	short m_search_type;							// ��������
	ItemID m_item_id;								// ��Ʒid
	short m_color;									// ��ɫ
	bool m_is_bind;									// ��ʼ�Ƿ��
	bool m_can_discard;								// �Ƿ�ɶ���
	bool m_can_sell;								// �ܷ����
	bool m_is_broadcast;							// �Ƿ�㲥
	int m_sell_price;								// ���ۼ۸�
	int m_recycl_type;								// ��������
	int m_recycl_get;								// ���ջ��
	bool m_is_record;								// �Ƿ��¼����־�У����䡢ʹ��
	UInt32 m_time_length;							// ��Чʱ���������Ʒ���㣩
	UInt32 m_invalid_time;							// ʧЧʱ�䣨�ض�ʱ��㣩
	bool m_is_curday_valid;							// �Ƿ����õ�����Ч
	short m_limit_prof;								// ְҵ����
	short m_limit_level;							// �ȼ�����
	short m_limit_sex;								// �Ա�����
	ItemID m_other_sex_itemid;						// �Ա�����ʱ��Ӧ����һ�Ա���Ʒid
	short m_pile_limit;								// �ѵ�����
	GameName m_item_name;		
};


#endif

