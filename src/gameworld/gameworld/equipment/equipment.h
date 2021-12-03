#ifndef __EQUIPMENT_H__
#define __EQUIPMENT_H__

#include "gamedef.h"
#include "item/itembase.h"
#include "servercommon/struct/itemlistparam.h"
#include "obj/character/attribute.hpp"
#include "servercommon/stonedef.hpp"
#include "servercommon/struct/itemlistparam.h"

#define EQUIP_ATTR_PER 0.0001f //װ����صļӳɰٷֱȣ������������ֱȣ�
// ------------------------------------------------------------------------------------------------------------
class EquipmentParam : public ItemParam	
{
public:
	EquipmentParam();

	virtual bool SetInStructData(ItemParamDataStruct *equip_data) const;

	void *	operator new(size_t c);
	void	operator delete(void *m);

	short quality;														// Ʒ��
	short strengthen_level;												// ǿ���ȼ�
	short shen_level;													// �����ȼ�
	short fuling_level;													// ����ȼ�
	
	short star_level;													// ���ǵȼ�
	short has_lucky;													// �Ƿ�����������
	short fumo_id;														// ��ħid
	short xianpin_equip;                                                // ��Ʒװ��,1100-1110������Ʒװ��

	unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];	// ��Ʒ��������

	int param1;
	int param2;
	int param3;

	unsigned char rand_attr_type_1;											// �����������1
	unsigned char rand_attr_type_2;											// �����������2
	unsigned char rand_attr_type_3;											// �����������3
	unsigned char reserve_type;

	unsigned short rand_attr_val_1;											// �������1
	unsigned short rand_attr_val_2;											// �������2
	unsigned short rand_attr_val_3;											// �������3
	unsigned short rand_attr_val_reserve;

	JingLingSkillInfo jing_ling_skill_list[JING_LING_SKILL_COUNT_MAX];
	unsigned short really;													// ��*װ��
	unsigned short xianpin_star_num;                                        // ��Ʒ������
};

class Equipment : public ItemBase
{
public:

	static const int BASE_ATTR_NUM = 12;							// ������������

	enum EQUIPMENT_TYPE												// װ������
	{
		E_TYPE_MIN = 100,

		E_TYPE_TOUKUI = E_TYPE_MIN,									// ͷ��
		E_TYPE_YIFU,												// �·�
		E_TYPE_KUZI,												// ����
		E_TYPE_XIEZI,												// Ь��
		E_TYPE_HUSHOU,												// ����
		E_TYPE_XIANGLIAN,											// ����
		E_TYPE_WUQI,												// ����
		E_TYPE_JIEZHI,												// ��ָ
		E_TYPE_YAODAI,												// ����
		E_TYPE_YUPEI,												// ����
		E_TYPE_SHOUZHUO,											// ����
		E_TYPE_MAX,

		E_TYPE_SPECIAL_MIN = 200,									// ����װ�����ͣ��羫��)��ʹ��equipment�ṹ�壬���ݹ�����equipmentmanager�����ڸ���ģ��
		E_TYPE_SPECIAL_JINGLING ,									// ����
		E_TYPE_SPECIAL_HUNJIE,										// ���
		E_TYPE_SPECIAL_IMP_GUARD,                                   // С��
		E_TYPE_SPECIAL_MINGHUN,										// ����
		E_TYPE_SPECIAL_LITTLEPET,									// С����
		E_TYPE_SPECIAL_MAX,

		E_TYPE_FIGHT_MOUNT = 300,									// ս������
		E_TYPE_MOUNT = 400,											// ����
		E_TYPE_WING = 500,											// ����
		E_TYPE_HALO = 600,											// �⻷
		E_TYPE_SHEN_GONG = 700,										// ��
		E_TYPE_SHEN_YI = 800,										// ����

		E_TYPE_ZHUANSHENG_MIN = 900,
		E_TYPE_ZHUANSHENG_1 = E_TYPE_ZHUANSHENG_MIN,				// ת��1
		E_TYPE_ZHUANSHENG_2,										// ת��2
		E_TYPE_ZHUANSHENG_3,										// ת��3
		E_TYPE_ZHUANSHENG_4,										// ת��4
		E_TYPE_ZHUANSHENG_5,										// ת��5
		E_TYPE_ZHUANSHENG_6,										// ת��6
		E_TYPE_ZHUANSHENG_7,										// ת��7
		E_TYPE_ZHUANSHENG_8,										// ת��8
		E_TYPE_ZHUANSHENG_MAX,

		E_TYPE_QINGYUAN_MIN = 1000,
		E_TYPE_QINGYUAN_1 = E_TYPE_QINGYUAN_MIN,					// ���1
		E_TYPE_QINGYUAN_2,											// ���2
		E_TYPE_QINGYUAN_3,											// ���3
		E_TYPE_QINGYUAN_4,											// ���4
		E_TYPE_QINGYUAN_MAX,

// 		E_TYPE_FEIXIAN_MIN = 1100,									// ���� 
// 		E_TYPE_FEIXIAN_WUQI = E_TYPE_FEIXIAN_MIN,					// ����
// 		E_TYPE_FEIXIAN_YIFU,										// ����
// 		E_TYPE_FEIXIAN_HUSHOU,										// ����
// 		E_TYPE_FEIXIAN_YAODAI,										// ����
// 		E_TYPE_FEIXIAN_HUTUI,										// ����
// 		E_TYPE_FEIXIAN_TOUKUI,										// ͷ��
// 		E_TYPE_FEIXIAN_XIANGLIAN,									// ����
// 		E_TYPE_FEIXIAN_SHOUZHUO,									// ����
// 		E_TYPE_FEIXIAN_JIEZHI,										// ��ָ
// 		E_TYPE_FEIXIAN_XIEZI,										// Ь��
// 		E_TYPE_FEIXIAN_YUPEI,										// ����
// 		E_TYPE_FEIXIAN_MAX,

		E_TYPE_ZHUANZHI_MIN = 1100,									// תְ 
		E_TYPE_ZHUANZHI_WUQI = E_TYPE_ZHUANZHI_MIN,					// ����
		E_TYPE_ZHUANZHI_YIFU,										// ����
		E_TYPE_ZHUANZHI_HUSHOU,										// ����
		E_TYPE_ZHUANZHI_YAODAI,										// ����
//		E_TYPE_ZHUANZHI_HUTUI,										// ����
		E_TYPE_ZHUANZHI_TOUKUI,										// ͷ��
		E_TYPE_ZHUANZHI_XIANGLIAN,									// ����
		E_TYPE_ZHUANZHI_SHOUZHUO,									// ����
		E_TYPE_ZHUANZHI_JIEZHI,										// ��ָ
		E_TYPE_ZHUANZHI_XIEZI,										// Ь��
		E_TYPE_ZHUANZHI_YUPEI,										// ����
		E_TYPE_ZHUANZHI_MAX,

		E_TYPE_LITTLE_PET_MIN = 1200,								// С����װ��
		E_TYPE_LITTLE_PET_1 = E_TYPE_LITTLE_PET_MIN,
		E_TYPE_LITTLE_PET_2,
		E_TYPE_LITTLE_PET_3,
		E_TYPE_LITTLE_PET_4,
		E_TYPE_LITTLE_PET_MAX,

		E_TYPE_CHINESE_ZODIAC_MIN = 1300,							//��Фװ��
		E_TYPE_CHINESE_ZODIAC_1 = E_TYPE_CHINESE_ZODIAC_MIN,
		E_TYPE_CHINESE_ZODIAC_2,
		E_TYPE_CHINESE_ZODIAC_3,
		E_TYPE_CHINESE_ZODIAC_4,
		E_TYPE_CHINESE_ZODIAC_5,
		E_TYPE_CHINESE_ZODIAC_MAX,

		E_TYPE_SHENSHOU_MIN = 1400,								   //����װ��
		E_TYPE_SHENSHOU_1 = E_TYPE_SHENSHOU_MIN,
		E_TYPE_SHENSHOU_2,
		E_TYPE_SHENSHOU_3,
		E_TYPE_SHENSHOU_4,
		E_TYPE_SHENSHOU_5,
		E_TYPE_SHENSHOU_MAX,

		E_TYPE_GREATESOLDIER_MIN = 1500,							//��ħװ��(��������ϵͳ)
		E_TYPE_GREATESOLDIER_1 = E_TYPE_GREATESOLDIER_MIN,
		E_TYPE_GREATESOLDIER_2,
		E_TYPE_GREATESOLDIER_3,
		E_TYPE_GREATESOLDIER_4,
		E_TYPE_GREATESOLDIER_MAX,

		E_TYPE_BAIZHAN_MIN = 1600,									// ��ս 
		E_TYPE_BAIZHAN_WUQI = E_TYPE_BAIZHAN_MIN,					// ����
		E_TYPE_BAIZHAN_YIFU,										// �·�
		E_TYPE_BAIZHAN_HUSHOU,										// ����
		E_TYPE_BAIZHAN_YAODAI,										// ����
		E_TYPE_BAIZHAN_TOUKUI,										// ͷ��
		E_TYPE_BAIZHAN_XIANGLIAN,									// ����
		E_TYPE_BAIZHAN_SHOUZHUO,									// ����
		E_TYPE_BAIZHAN_JIEZHI,										// ��ָ
		E_TYPE_BAIZHAN_XIEZI,										// Ь��
		E_TYPE_BAIZHAN_YUPEI,										// ����
		E_TYPE_BAIZHAN_MAX,

		E_TYPE_CHUANSHI_MIN = 1700,
		E_TYPE_CHUANSHI_WUQI,										// �������� 
		E_TYPE_CHUANSHI_TOUKUI,										// ����ս�� 
		E_TYPE_CHUANSHI_YIFU,										// ����ս�� 
		E_TYPE_CHUANSHI_HUSHOU,										// ����ս��  
		E_TYPE_CHUANSHI_YAODAI,										// ��������
		E_TYPE_CHUANSHI_HUTUI,										// ��������
		E_TYPE_CHUANSHI_XIEZI,										// ����սѥ
		E_TYPE_CHUANSHI_ZHUOZI,										// ��������
		E_TYPE_CHUANSHI_XIANGLIAN,									// ��������
		E_TYPE_CHUANSHI_JIEZHI,										// ����ս��
		E_TYPE_CHUANSHI_MAX,

	};

	const static int EQUIPMENT_TYPE_COUNT = E_TYPE_MAX - E_TYPE_MIN;// װ����������

	enum EQUIPMENT_INDEX											// װ����λ
	{
		E_INDEX_TOUKUI = 0,											// ͷ��
		E_INDEX_YIFU,												// �·�
		E_INDEX_KUZI,												// ����
		E_INDEX_XIEZI,												// Ь��
		E_INDEX_HUSHOU,												// ����
		E_INDEX_XIANGLIAN,											// ����
		E_INDEX_WUQI,												// ����
		E_INDEX_JIEZHI,												// ��ָ
		E_INDEX_YAODAI,												// ����
		E_INDEX_YUPEI,												// ����
		E_INDEX_SHOUZHUO,											// ����
		E_INDEX_MAX,
	};
	UNSTD_STATIC_CHECK(MAX_STONE_EQUIP_PART <= E_INDEX_MAX);
	UNSTD_STATIC_CHECK(MAX_EQUIP_PART == E_INDEX_MAX);

	enum EQUIPMENT_XIANPIN_ATTRIBUTE								// װ����Ʒ��������
	{
		E_XIANPIN_INVALID = 0,

		E_XIANPIN_ADD_BASE_MAXHP_PER = 1,								// +X%��Ѫ����
		E_XIANPIN_ADD_BASE_GONGJI_PER = 2,								// +X%������
		E_XIANPIN_ADD_BASE_FANGYU_PER = 3,								// +X%������
		E_XIANPIN_ADD_BASE_MINGZHAONG_PER = 4,							// +X%����
		E_XIANPIN_ADD_BASE_SHANBI_PER = 5,								// +X%����
		E_XIANPIN_ADD_BASE_BAOJI_PER = 6,								// +X%����
		E_XIANPIN_ADD_BASE_JIANREN_PER = 7,								// +X%����

		E_XIANPIN_ADD_BASE_MAXHP_ROLE_LEVEL = 8,						// +��ɫ�ȼ�*X��Ѫ����
		E_XIANPIN_ADD_BASE_GONGJI_ROLE_LEVEL = 9,						// +��ɫ�ȼ�*X������
		E_XIANPIN_ADD_BASE_FANGYU_ROLE_LEVEL = 10,						// +��ɫ�ȼ�*X������
		E_XIANPIN_ADD_BASE_MINGZHONG_ROLE_LEVEL = 11,					// +��ɫ�ȼ�*X����
		E_XIANPIN_ADD_BASE_SHANBI_ROLE_LEVEL = 12,						// +��ɫ�ȼ�*X����
		E_XIANPIN_ADD_BASE_BAOJI_ROLE_LEVEL = 13,						// +��ɫ�ȼ�*X����
		E_XIANPIN_ADD_BASE_JIANREN_ROLE_LEVEL = 14,						// +��ɫ�ȼ�*X����

		E_XIANPIN_ADD_KILL_MONSTER_EXP_REWARD_PER = 15,					// ɱ�־��飺+X%
		E_XIANPIN_ADD_HURT_TO_ROLE_PER = 16,							// ��Ҷ������������˺���+X%
		E_XIANPIN_ADD_HURT_TO_MONSTER_PER = 17,							// ��ҶԹ�������˺���+X%
		E_XIANPIN_DEC_HURT_FROM_ROLE_PER = 18,							// ������Ҷ��Լ�����˺���-X%
		E_XIANPIN_DEC_HURT_FROM_MONSTER_PER = 19,						// ������������˺���-X%

		E_XIANPIN_ADD_ALL_EQUIP_BASE_FANGYU_PER= 20,					// +X%ȫ��װ������
		E_XIANPIN_ADD_ALL_EQUIP_BASE_MAXHP_PER = 21,					// +X%ȫ��װ������
		E_XIANPIN_ADD_ALL_EQUIP_BASE_GONGJI_PER = 22,					// +X%ȫ��װ������

		E_XIANPIN_ADD_GONGJI_OF_ROLE_LEVEL = 23,						// ���ӣ��ȼ� * X���㹥��
		E_XIANPIN_ADD_FANGYU_OF_ROLE_LEVEL = 24,						// ���ӣ��ȼ� * X�������
		E_XIANPIN_ADD_MAXHP_OF_ROLE_LEVEL = 25,							// ���ӣ��ȼ� * X������Ѫ
		E_XIANPIN_ADD_MONSTER_DROP_COIN_PER = 26,						// Ұ�ֵ���������  X %  ���Ը�������ڵĹ�����Ч��
		E_XIANPIN_ADD_DAMAGE_TO_MONSTER_OF_ROLE_LEVEL = 27,				// �Թ��﹥��������ɣ��ȼ� * X�����˺�
		E_XIANPIN_DEC_DAMAGE_FROM_MONSTER_OF_ROLE_LEVEL = 28,			// ���������˺�������٣��ȼ� * X�����˺�
		E_XIANPIN_DEC_DAMAGE_FROM_ROLE_PER = 29,						// ���������˺����� X % �������ɵ������˺� * ��1 - X %�����������˺���
		E_XIANPIN_ADD_DAMAGE_TO_ROLE_PER = 30,							// ���������˺����� X % �������ɵ������˺� * ��1 - X %�����������˺���
		E_XIANPIN_ADD_MONSTER_DROP_EXP_PER = 31,						// ��ɱҰ�־����������  X %

		E_XIANPIN_MAX,
	};

	enum EQUIPMENT_FIX_GETWAY
	{
		EQUIPMENT_FIX_GETWAY_MIN = 0,

		EQUIPMENT_FIX_GETWAY_DEFAULT_GIFT = 1,						// ������л��
		EQUIPMENT_FIX_GETWAY_CHEST_SHOP = 2,						// ��Ѱ�����һ��л��
		EQUIPMENT_FIX_GETWAY_MONSTER_ID_DROP = 3,					// �ӵ����л��(���ݹ���id)
		EQUIPMENT_FIX_GETWAY_SCENE_ID_DROP = 4,						// �ӵ����л��(���ݳ���id)
		EQUIPMENT_FIX_GETWAY_COLOR_DROP = 5,						// �ӵ����л��(����װ����ɫ)
		EQUIPMENT_FIX_GETWAY_COMPOSE = 6,							// װ���һ�
		EQUIPMENT_FIX_GETWAY_COMPOUND = 7,							// װ���ϳ�
		EQUIPMENT_FIX_GETWAY_ZHUANZHI_EQUIP_COMPOSE = 8,			// תְװ���ϳ�

		EQUIPMENT_FIX_GETWAY_MAX,
	};

	enum EQUIPMENT_STRENGTHEN_IS_DOWNLEVEL_ON_FAIL
	{
		E_STRENGTHEN_NOT_DOWNLEVEL_ON_FAIL = 0,						// ǿ��ʧ�ܲ�����
		E_STRENGTHEN_DOWNLEVEL_ON_FAIL								// ǿ��ʧ�ܵ���
	};

	UNSTD_STATIC_CHECK(E_INDEX_MAX == ItemNamespace::MAX_EQUIPMENT_GRID_NUM);
	
	Equipment(short equip_type);
	virtual ~Equipment() {}

	short GetEquipType() const { return m_equipment_type; }
	//short GetQuality() const { return m_quality; } 

	bool CanStrengthen() const { return m_can_strengthen; }
	bool CanFlush() const { return m_can_flush; }
	UInt16 GetSuitId() const { return m_suit_id; }
	short GetHoleNum() const { return m_hole_num; }
	char IsShen() const { return m_is_shen; }
	short GetActShizhuangType() const { return m_act_shizhung_type; }
	int GetOrder() const { return m_order; }
	int GetSubType() const { return m_sub_type; }
	int GetGuildStorageScore() const { return m_guild_storage_score; }
	int GetGodAttr()const { return god_attr; }

	virtual bool Init(const char* path, std::string *err);
	virtual int Use(Role *role, const ItemGridData &item_data, short &num, short equip_index = -1) const;

	virtual int GetSellPrice(ItemParam *param) const;
	virtual bool CanDiscard(ItemParam *param, int *reason) const;

	virtual EquipmentParam * CreateParam() const;
	virtual bool SetParamFromStruct(ItemParam *param, const ItemParamDataStruct *equip_data) const;
	virtual EquipmentParam * CreateParamFromStruct(const ItemParamDataStruct *equip_data) const;

	void GetBaseAttr(CharIntAttrs &int_attrs) const;
	void GetAttr(const EquipmentParam *param, CharIntAttrs &int_attrs, int role_level) const;

	int GetAttrIndexByAttrType(short attr_type) const;

	static bool IsWuQiIndex(short equip_index);
	static bool IsFangJuIndex(short equip_index);
	static bool IsShouShiIndex(short equip_index);
	static bool IsCommonEquipIndex(short equip_index);
	static bool IsMatchIndex(short equip_type, short equip_index);
	static bool IsValidType(short equip_type);
	static bool IsFangJuType(short equip_type);
	static bool IsShipinType(short equip_type);
	static bool IsWuqiType(short equip_type);
	static bool IsHuShouType(short equip_type);
	static bool IsGongjiType(short equip_type);

	virtual bool CheckIsSameItemExceptSex(const ItemBase *other_item) const;

protected:

	const short	m_equipment_type;									// װ������EQUIPMENT_TYPE ���� �·���
		
	//short m_quality;												// Ʒ��
	bool m_can_strengthen;											// �ɷ�ǿ��
	bool m_can_flush;												// �ɷ�ϴ��
	char m_is_shen;													// �Ƿ���װ
	short m_hole_num;												// ������
	UInt16 m_suit_id;												// ��װid
	short m_act_shizhung_type;										// ����ʱװ����
	int m_order;													// ����
	int m_sub_type;													// ������
	int m_guild_storage_score;										// ���˲ֿ�ְ��

	short m_base_attr_type_list[BASE_ATTR_NUM];						// ������������
	int m_base_attr_value_list[BASE_ATTR_NUM];						// ��������ֵ
	short god_attr;                                                 // ��������
};

#endif
