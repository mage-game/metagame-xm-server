#ifndef EQUIPCONFIG_H
#define EQUIPCONFIG_H

#include "gamedef.h"
#include "servercommon/basedef.h"
#include <map>
#include <vector>

#include "equipment/equipmentfunction.h"
#include "servercommon/struct/itemlistparam.h"

#include "equipment/equipment.h"
#include "item/itemfunction.h"
#include "item/itembase.h"
#include "servercommon/maildef.hpp"

class TiXmlElement;

/*
װ������ equipforge.xml
*/

/////ǿ����������////////////////////////////////////////////////
struct StrengthBaseConfig										
{
	StrengthBaseConfig() : equip_index(0), strength_level(0), equip_level(0), need_order(0), stone_hole_num(0), coin(0), stuff_id(0), stuff_count(0),
						   base_succ_rate(0), is_downlevel_on_fail(0), protect_stuff_id(0), protect_stuff_count(0), lucky_val_success(0), lucky_val_fail(0), 
						  add_maxhp(0), add_gongji(0), add_fangyu(0), add_mingzhong(0), add_shanbi(0), add_baoji(0), add_jianren(0), is_broadcast(0)
	{
	}

	short equip_index;
	short strength_level;										// ǿ���ȼ�
	short equip_level;											// װ���ȼ�
	short need_order;											// ����Ľ����ȼ�
	short stone_hole_num;										// ��ʯ����
	int coin;													// ����ͭǮ
	ItemID stuff_id;											// ǿ������ID
	short stuff_count;											// ǿ����������
	int base_succ_rate;											// �����ɹ���
	int is_downlevel_on_fail;									// ʧ��ʱ�Ƿ����
	ItemID protect_stuff_id;									// ������ID
	short protect_stuff_count;									// ����������
	short lucky_val_success;									// ����ֵ����
	short lucky_val_fail;										// ����ֵ����

	Attribute add_maxhp;										// ǿ����Ѫ
	Attribute add_gongji;										// ǿ������
	Attribute add_fangyu;                                       // ǿ������
	Attribute add_mingzhong;
	Attribute add_shanbi;
	Attribute add_baoji;
	Attribute add_jianren;
	short is_broadcast;
};

//////ȫ����ǿ���ȼ��ӳ�����////////////////////////////////////
struct StrengthTotalLvlAddConfig
{
	StrengthTotalLvlAddConfig() : seq(0), total_strength_level(0), add_maxhp(0), add_gongji(0), add_fangyu(0),
								add_mingzhong(0), add_shanbi(0), add_baoji(0), add_jianren(0), is_broadcast(0)
	{
	}
	
	int seq;
	short total_strength_level;										// ǿ���ܵȼ�
	int add_maxhp;
	int add_gongji;
	int add_fangyu;
	int add_mingzhong;
	int add_shanbi;
	int add_baoji;
	int add_jianren;
	int is_broadcast;
};

//////��������/////////////////////////////////////////////////////
struct UpStarConfig
{
	UpStarConfig() : equip_index(0), star_level(0), equip_level(0), need_mojing(0), add_maxhp(0), add_gongji(0), add_fangyu(0), is_broadcast(0)
	{
	}

	int equip_index;
	int star_level;
	int equip_level;
	int need_mojing;
	Attribute add_maxhp;
	Attribute add_gongji;
	Attribute add_fangyu;
	int is_broadcast;
};

// ȫ������
struct UpStarTotalConfig
{
	UpStarTotalConfig() : seq(0), total_star(0), add_maxhp(0), add_gongji(0), add_fangyu(0), is_broadcast(0)
	{
	}
	
	int seq;
	int total_star;
	Attribute add_maxhp;
	Attribute add_gongji;
	Attribute add_fangyu;
	int is_broadcast;
};


//////��������/////////////////////////////////////////////////////

// ��Ʒ���Ե�������������װ���ģ�BIG_TYPE_ALL:�������������͵�װ����������������Ʒ����
enum BIG_TYPE
{
	BIG_TYPE_GONGJI = 0,					// ����
	BIG_TYPE_FANGYU,						// ����
	BIG_TYPE_ALL,							// ȫ��
};

enum QUALITY_COLOR_TYPE
{
	QUALITY_COLOR_TYPE_INVALID = 0,	

	QUALITY_COLOR_TYPE_BLUE,	            // ��ɫ					
	QUALITY_COLOR_TYPE_PURPLE,	            // ��ɫ
	QUALITY_COLOR_TYPE_ORANGE,              // ��ɫ
	QUALITY_COLOR_TYPE_RED,                 // ��ɫ

	QUALITY_COLOR_TYPE_MAX,
};


enum INHERIT_TYPE
{
	COMMON_INHERIT = 0,				// ��ͨ�̳�
	LUCKY_INHERIT,					// ���˼̳�
	INHERIT_MAX,
};

enum COST_TYPE
{
	COST_COIN = 0,					// ͭǮ
	COST_GOLD,						// Ԫ��
	COST_MAX,
};

struct InheritConfig									 
{
	InheritConfig() : coin(0), gold(0), xy_gold(0)
	{

	}

	int coin;											
	int gold;
	int xy_gold;
};

//////����Ʒ������////////////////////////////////////////////////
 struct UpQualityConfig
 {
 	UpQualityConfig() : equip_index(0), quality(0), equip_level(0), coin(0), 
 						stuff_id(0), stuff_count(0), attr_percent(0)
 	{
 	}
 
 	short equip_index;
 	short quality;												// Ʒ��
 	short equip_level;											// װ���ȼ�
 	int coin;													// ����ͭǮ
 	ItemID stuff_id;											// ���Ĳ���ID
 	short stuff_count;											// ���Ĳ�������
 	int attr_percent;											// ����������ֱ�
	short color;												// ��ɫ
	bool is_notice;												// �Ƿ�ȫ������
 };

//////����////////////////////////////////////////////////////
struct ShenOPConfig
{
	ShenOPConfig() : equip_index(0), shen_level(0), equip_level(0), coin(0), stuff_id(0), stuff_count(0), 
					 attr_percent(0), add_maxhp(0), add_gongji(0), add_fangyu(0), add_mingzhong(0), add_shanbi(0), add_baoji(0), add_jianren(0),
					 is_broadcast(0)
	{
	}

	int equip_index;
	int shen_level;
	int equip_level;
	int coin;
	ItemID stuff_id;
	short stuff_count;

	int attr_percent;
	Attribute add_maxhp;
	Attribute add_gongji;
	Attribute add_fangyu;
	Attribute add_mingzhong;
	Attribute add_shanbi;
	Attribute add_baoji;
	Attribute add_jianren;
	short is_broadcast;
};

struct ShenOPTotalConfig
{
	ShenOPTotalConfig() : shen_level(0), add_maxhp(0), add_gongji(0), add_fangyu(0), add_mingzhong(0), add_shanbi(0), add_baoji(0), add_jianren(0), is_broadcast(0)
	{
	}

	int shen_level;
	int add_maxhp;
	int add_gongji;
	int add_fangyu;
	int add_mingzhong;
	int add_shanbi;
	int add_baoji;
	int add_jianren;
	int is_broadcast;
};

//////��������////////////////////////////////////////////////////
//struct UpLevelConfig
//{
//	UpLevelConfig() : m_old_equip_id(0), m_new_equip_id(0), m_nv_wa_shi(0)
//	{
//	}
//
//	short m_old_equip_id;										// ��װ��ID
//	short m_new_equip_id;										// ��װ��ID
//	int m_nv_wa_shi;											// ����Ů�ʯ
//};

//struct UpLevelConfig
//{
//	UpLevelConfig() : previous_itemid(0), old_itemid(0), new_itemid(0), coin(0), stuff_id(0), stuff_count(0),
//		base_succ_rate(0), is_downlevel_on_fail(0), protect_stuff_id(0), protect_add_rate(0)
//	{
//	}
//	ItemID previous_itemid;										//��һ��װ��ID,����ʧ�ܺ�ת������ID
//	ItemID old_itemid;											//ԭװ��ID
//	ItemID new_itemid;											//������װ��ID
//	int coin;													// ����ͭǮ
//	ItemID stuff_id;											// ǿ������ID
//	short stuff_count;											// ǿ����������
//	int base_succ_rate;											// �����ɹ���
//	int is_downlevel_on_fail;									// ʧ��ʱ�Ƿ����
//	ItemID protect_stuff_id;									// ������ID
//	int protect_add_rate;										//���������ӳɹ���
//};

//////��������////////////////////////////////////////////////////
struct UpLevelConfig
{
	//UpLevelConfig() : m_old_equip_id(0), m_new_equip_id(0), m_coin(0),
	//	m_stuff_id_1(0), m_stuff_count_1(0), m_stuff_id_2(0), m_stuff_count_2(0), m_stuff_id_3(0), m_stuff_count_3(0),
	//	m_protect_id(0), m_strength_0_prob(0), m_strength_1_prob(0), m_strength_2_prob(0)
	//{

	//}

	//short GetStrengthDropLevel() const
	//{
	//	int rand_val = RandomNum(100);
	//	if (rand_val < m_strength_0_prob) return 0;
	//	else if (rand_val < m_strength_1_prob) return 1;
	//	else return 2;
	//}

	UpLevelConfig() : m_old_equip_id(0), m_new_equip_id(0), 
		m_stuff_id_1(0), m_stuff_count_1(0), m_stuff_id_2(0), m_stuff_count_2(0), m_stuff_id_3(0), m_stuff_count_3(0)
	{

	}

	short m_old_equip_id;										// ��װ��ID
	short m_new_equip_id;										// ��װ��ID
	//int m_coin;													// ����ͭǮ
	ItemID m_stuff_id_1;										// ����1��ID
	short m_stuff_count_1;										// ����1������
	ItemID m_stuff_id_2;										// ����2��ID
	short m_stuff_count_2;										// ����2������
	ItemID m_stuff_id_3;										// ����3��ID
	short m_stuff_count_3;										// ����3������
	//ItemID m_protect_id;										// ������ID
	//short m_strength_0_prob;									// ǿ����0���ĸ���
	//short m_strength_1_prob;									// ǿ����1���ĸ���
	//short m_strength_2_prob;									// ǿ����2���ĸ���
};

// ��ʯ
struct StoneCfg
{
	StoneCfg() : item_id(0), stone_type(0), attr_type1(0), att_value1(0), attr_type2(0), att_value2(0), level(0)
	{
	}

	int item_id;
	int stone_type;
	short attr_type1;
	Attribute att_value1;
	short attr_type2;
	Attribute att_value2;
	int level;
	int level_need_num;
};

struct StoneExAdd
{
	StoneExAdd() : seq(0), total_level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), is_broadcast(0){}

	int seq;
	int total_level;													
	int maxhp;													
	int gongji;													
	int fangyu;												
	int mingzhong;													
	int shanbi;
	int baoji;
	int jianren;
	int is_broadcast;
};

struct EquipFulingConfig
{
	EquipFulingConfig() :fuling_level(0), equip_level_limit(0), equip_color_limit(0), wuqi_stuff_id(0), wuqi_stuff_count(0), 
		shipin_stuff_id(0), shipin_stuff_count(0),hujia_stuff_id(0), hujia_stuff_count(0), fangju_stuff_id(0), 
		fangju_stuff_count(0), coin(0), gongji_per(0), gongji(0), fangyu_per(0), fangyu(0), is_broadcast(0)
	{
	}

	int fuling_level;
	int equip_level_limit;
	int equip_color_limit;
	int wuqi_stuff_id;
	int wuqi_stuff_count;
	int shipin_stuff_id;
	int shipin_stuff_count;
	int hujia_stuff_id;
	int hujia_stuff_count;
	int fangju_stuff_id;
	int fangju_stuff_count;
	int coin;	

	int gongji_per;											
	int gongji;												
	int fangyu_per;											
	int fangyu;

	short is_broadcast;
};

struct EquipFumoConfig
{
	EquipFumoConfig() : id(0), equip_sub_type(0), time(0), gongji(0), max_hp(0), fangyu(0), mingzhong(0), shanbi(0) {}
	
	int id;
	int equip_sub_type;
	int time;
	int gongji;
	int max_hp;
	int fangyu;
	int mingzhong;
	int shanbi;
};

const static int EQUIP_CROSS_EQUIP_CONFIG_MAX_COUNT = 30;

struct EquipCrossEquipConfig
{
	EquipCrossEquipConfig() : item_id(0), gongji(0), fangyu(0), maxhp(0), mingzhong(0), shanbi(0), fujiashanghai(0), dikangshanghai(0) {}

	ItemID item_id;

	int gongji;													
	int fangyu;													
	int maxhp;													
	int mingzhong;												
	int shanbi;													
	int fujiashanghai;													
	int dikangshanghai;		
};

//////��Ʒ����////////////////////////////////////////////////////
struct EquipXianPinCountConfig
{
	EquipXianPinCountConfig(): xianpin_count(0) {}

	short xianpin_count;
};

struct EquipXianPinConfig
{
	EquipXianPinConfig() : xianpin_type(0), big_type(0), add_value(0), weight(0), shuxing_type(0), equip_color(0), color(0), capability(0), score(0) {}

	int xianpin_type;
	int big_type;
	Attribute add_value;
	int weight;
	int shuxing_type;
	int equip_color;
	int color;
	int capability;
	int score;
};

struct EquipXianPinStarCfg
{
	EquipXianPinStarCfg() : star_level(0), need_capability(0) {}

	int star_level;
	int need_capability;
};

struct EquipXianPinFixConfig
{
	static const int GETWAY_XIANPIN_MAX_COUNT = 4;

	EquipXianPinFixConfig() : get_way(0), equip_id(0), param_1(0), param_2(0), total_weight(0)
	{
		memset(xianpin_rate_list, 0, sizeof(xianpin_rate_list));
		memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
	}

	int get_way;
	ItemID equip_id;
	int param_1;
	int param_2;
	int total_weight;
	int xianpin_rate_list[GETWAY_XIANPIN_MAX_COUNT];
	short xianpin_type_list[GETWAY_XIANPIN_MAX_COUNT];
};

struct EquipXianPinDropConfig
{
	static const int DROP_XIANPIN_MAX_COUNT = 3;

	EquipXianPinDropConfig() : equip_id(0)
	{
		memset(drop_rate_list, 0, sizeof(drop_rate_list));
		memset(drop_xianpin_type_list, 0, sizeof(drop_xianpin_type_list));
	}

	ItemID equip_id;
	int drop_rate_list[DROP_XIANPIN_MAX_COUNT];
	int drop_xianpin_type_list[DROP_XIANPIN_MAX_COUNT];
};

enum TAOZHUANG_TYPE
{
	BAOSHI_TAOZHUANG = 0,
	STREGNGTHEN_TAOZHUANG =1,
	UPSTAR_TAOZHUANG =2
};

//////װ��ר������////////////////////////////////////////////////////
struct EquipSkillConfig
{
	EquipSkillConfig() : item_id(0), skill_id(0), skill_level(0) {}

	int item_id;
	int skill_id;
	int skill_level;
};

//////װ���ɳ�����////////////////////////////////////////////////////
struct EquipGrowUpConfig
{
	EquipGrowUpConfig() : equip_index(0), add_maxhp(0), add_gongji(0), add_fangyu(0), add_mingzhong(0), add_shanbi(0), add_baoji(0), add_jianren(0)
	{
	}

	int equip_index;
	int add_maxhp;
	int add_gongji;
	int add_fangyu;
	int add_mingzhong;
	int add_shanbi;
	int add_baoji;
	int add_jianren;
};

enum STONE_LIMIT_TYPE
{
	//STONE_LIMIT_TYPE_NULL = 0,			// ������
	STONE_LIMIT_TYPE_EQUIP_LIMIT_LEVEL = 0,	// װ�����Ƶȼ�
	STONE_LIMIT_TYPE_PATA,					// ����֮������
	STONE_LIMIT_TYPE_VIP,					// vip�ȼ�
	STONE_LIMIT_TYPE_STRENGTHEN,			// ǿ���ȼ�

	STONE_LIMIT_TYPE_MAX		  	
};

struct StoneGridOpenLimit
{
	StoneGridOpenLimit() : stone_index(-1), limit(0), param1(0){}

	int stone_index;
	int limit;
	int param1;
};

struct StoneGridOpenLimitByStrengthLevel
{
	StoneGridOpenLimitByStrengthLevel() : stone_index(-1), limit(0) ,equip_index(0), param1(0) {}

	int stone_index;
	int limit;
	int equip_index;
	int param1;
};

// װ���ϳ�
struct EquipmentCompoundCfg
{
	EquipmentCompoundCfg(): item_id(0), xianpin_count(0), gain_item(0), compound_xianpin(0), is_notice(0), discard_return_count(0)
	{}

	ItemID item_id;
	short xianpin_count;
	ItemConfigData stuff_item;
	ItemID gain_item;
	int compound_xianpin;
	short is_notice;
	int discard_return_count;
	ItemConfigData discard_return[MAX_ATTACHMENT_ITEM_NUM];
};

// ����װ��
struct EquipEternityCfg
{
	EquipEternityCfg() : equip_index(0), eternity_level(0), show_level(0), jingjie_level(0), stuff_id(0), stuff_count(0),
		stuff_2_id(0), stuff_2_num(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0),
		jianren(0), is_broadcast(false)
	{
		memset(name, 0, sizeof(name));
	}

	int equip_index;
	int eternity_level;
	int show_level;
	int jingjie_level;
	ItemID stuff_id;
	short stuff_count;
	ItemID stuff_2_id;
	short stuff_2_num;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	bool is_broadcast;
	GameName name;
};

// ������װ
struct EquipEternitySuitCfg
{
	EquipEternitySuitCfg() : suit_level(0), show_level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0),
		baoji(0), jianren(0), hxyj(0), hxyj_hurt_per(0), per_attr(0), is_broadcast(false) 
	{
		memset(name, 0, sizeof(name));
	}

	int suit_level;
	int show_level;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	Attribute hxyj;						// ����һ��
	Attribute hxyj_hurt_per;				// ����һ���˺���
	int per_attr;
	bool is_broadcast;
	GameName name;
};

// �ֽ���ͨװ��
struct DecomposeCfg
{
	DecomposeCfg() : type(0){}
	int type;
	ItemConfigData return_item;
};

struct EquipUpLevelSuitAttrCfg
{
	EquipUpLevelSuitAttrCfg() : need_count(0), maxhp(0), fangyu(0), mingzhong(0), shanbi(0),
		baoji(0), jianren(0), per_zhufuyiji(0)
	{}

	int need_count;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
	int per_zhufuyiji;
};

struct EquipUpLevelSuitCfg
{
	EquipUpLevelSuitCfg() : suit_order(0), suit_count(0) {}

	int suit_order;
	int suit_count;
	EquipUpLevelSuitAttrCfg attr[Equipment::E_INDEX_MAX];
};

//////////////////////////////////////////////////////////////////////////
class EquipConfig
{
public:
	static const int MAX_EQUIPMENT_SHEN_LEVEL = 30;											// װ���������ȼ�
	static const int TOTAL_SHEN_OP_CFG_MAX_COUNT = 50;										// ȫ�����������������
	static const int MAX_STAR_LEVEL = 1000;													// װ���������ȼ�
	static const int TOTAL_STAR_CFG_MAX_COUNT = 60;											// ȫ�����������������
	static const int MAX_STRENGTH_LEVEL = 501;												// ���ǿ���ȼ�
	static const int TOTAL_STRENGTH_LEVEL_CFG_MAX_COUNT = 50;								// ȫ��ǿ���ȼ������������
	static const int TOTAL_STONE_LEVEL_CFG_MAX_COUNT = 64;									// ȫ��ʯ�ȼ������������
	static const int MAX_XIANPING_CFG_NUM = 512;											// ���������
	static const int MAX_XIANPING_STAR_LEVEL = 3;											// ��Ʒ����Ǽ�
	static const int MAX_FULIN_MAX_LEVEL = 10;
	static const int MAX_STONE_LEVEL = 10;
	static const int MAX_ETERNITY_LEVEL = 20;												// �������ȼ�
	static const int ETERNITY_SUIT_COUNT = 11;												// ������װ����
	static const int UPLEVEL_SUIT_COUNT = 16;												// ������װ����

	static int inherit_need_coin;	//������Ҫͭ��
	static int inherit_need_gold;	//������ҪԪ��

	EquipConfig();
	~EquipConfig();

	bool Init(const std::string &configname, std::string *err);

	// ǿ�����
	const StrengthBaseConfig * GetStrengthBaseCfg(int equip_type, short strength_level) const;
	const int GetStrengthMaxValue(int equip_type, short jieshu) const;	// ��ǰ�����ܴﵽ�����ǿ��
	const StrengthTotalLvlAddConfig * GetStrengthTotallvlAddCfg(short strength_level) const;
	
	// �����������
	const UpStarConfig * GetUpStarCfg(int equip_type, int star_level) const;
	const UpStarTotalConfig * GetUpStarTotalCfg(int star_level, bool is_system_msg) const;
	
	// �������
	const InheritConfig * GetInheritCfg() const {return &m_inherit_cfg; };

	// ����Ʒ�����
	const UpQualityConfig * GetUpQualityCfg(int equip_type, int quality) const;

	// �������
	const ShenOPConfig * GetShenOPCfg(int equip_type, int shen_level) const;
	const ShenOPTotalConfig * GetShenOPTotalCfg(int shen_level, bool is_system_msg = false) const;

	// �������
	const UpLevelConfig * GetUpLevelCfg(ItemID old_equip_id) const;
	//const UpLevelConfig * GetUpLevelCfg(int equip_index, short up_level) const;

	// ��ʯ���
	const StoneCfg * GetStoneCfg(int item_id) const;
	const StoneExAdd * GetStoneExAdd(int stone_level) const;
	const StoneGridOpenLimit * GetStoneOpenLimit(int index) const;
	const StoneGridOpenLimitByStrengthLevel * GetStoneOpenLimitByEquipIndex(int stone_index, int equip_index) const;

	// װ������
	const EquipFulingConfig *GetFulingCfg(int fuling_level) const;

	// ���װ��
	const EquipCrossEquipConfig * GetCrossEquipCfg(ItemID item_id) const;
	
	// ��Ʒ�������---------------------------------------------------------------------------------
	const EquipXianPinCountConfig * GetXianPinCountCfg(int color) const;		
	const EquipXianPinConfig * GetXianPinEquipCfg(int xianpin_type) const;

	const bool GetRandomXianpin(ItemGridData *item_grid_data, int getway = 0, int getway_param1 = 0, int fix_best_attr_num = 0) const;
	const bool GetFixedXianPinType(ItemGridData *item_grid_data, int get_way, int param_1) const;

	const EquipXianPinDropConfig * GetXianPinDropCfg(int equip_id) const; // ������
	const bool GetDropXianPinType(ItemGridData *item_grid_data) const;	  // ������

	int GetXianPinStarLevel(int xianpin_capability) const;		// ������

	//---------------------------------------------------------------------------------

	// ��ħ����
	const EquipFumoConfig *GetFumoEquipCfg(int id) const;

	// ��ͨװ����������
	const bool GetLucky(ItemGridData *item_grid_data) const;

	// ��ȡװ������
	const EquipSkillConfig * GetSkillCfg(ItemID item_id) const;

	// ��ȡװ���ɳ�
	const EquipGrowUpConfig * GetEquipGrowUpConfig(int equip_type) const;
	
	// װ���ϳ�
	const EquipmentCompoundCfg * GetEquipCompoundConfig(ItemID item_id, short xianpin_count) const;

	const EquipEternityCfg * GetEquipEternityCfg(int equip_index, int eternity_level) const;
	const EquipEternitySuitCfg * GetEquipEternitySuitCfg(int eternity_suit_level) const;

	// ����װ�����ֻ�����Ʒ������
	unsigned short GetXianpinStarNumByScore(unsigned short score) const;

	//��ͨװ���ֽ�
	const DecomposeCfg * GetEquipDecomposeCfg(int type) const;

	const EquipUpLevelSuitAttrCfg * GetEquipUplevelSuitCfg(int order, int count) const;

private:
	union ParamToLongLong
	{
		struct
		{
			short m_param1;
			short m_param2;										
			short m_param3;									
			short m_param4;										
		};

		long long val;
	};
	long long ConvertParamToLongLong(short param1 = 0, short param2 = 0, short param3 = 0, short param4 = 0) const;

	// ǿ�����
	int InitStrengthBaseCfg(TiXmlElement *RootElement);
	int InitStrengthTotallvlCfg(TiXmlElement *RootElement);
	StrengthBaseConfig m_strength_base_cfg_list[Equipment::E_INDEX_MAX][MAX_STRENGTH_LEVEL + 1];

	int m_strength_totallvl_cfg_count;
	StrengthTotalLvlAddConfig m_strength_totallvl_cfg_list[TOTAL_STRENGTH_LEVEL_CFG_MAX_COUNT];
	
	//�������
	int InitUpStarCfg(TiXmlElement *RootElement);
	int InitUpStarTotalCfg(TiXmlElement *RootElement);
	UpStarConfig m_up_star_list[Equipment::E_INDEX_MAX][MAX_STAR_LEVEL + 1];

	int m_up_star_total_cfg_count;
	UpStarTotalConfig m_up_star_total_cfg_list[TOTAL_STAR_CFG_MAX_COUNT];

	// �������
	InheritConfig m_inherit_cfg;

	// Ʒ�����
	int InitUpQualityCfg(TiXmlElement *RootElement);
	std::map<int, UpQualityConfig> m_up_quality_map;
	UpQualityConfig m_up_quality_list[Equipment::EQUIPMENT_TYPE_COUNT][ItemBase::I_QUALITY_MAX * ItemBase::QUALITY_PER_COLOR];

	// �������
	int InitShenOpCfg(TiXmlElement *RootElement);
	int InitShenOpTotalCfg(TiXmlElement *RootElement);
	std::map<int, ShenOPConfig> m_shen_op_map;
	ShenOPConfig m_shen_op_list[Equipment::E_INDEX_MAX][MAX_EQUIPMENT_SHEN_LEVEL + 1];

	int m_shen_op_total_cfg_count;
	ShenOPTotalConfig m_shen_op_total_cfg_list[TOTAL_SHEN_OP_CFG_MAX_COUNT];

	// �������
	int InitUpLevelCfg(TiXmlElement *RootElement);
	std::map<ItemID, UpLevelConfig> m_uplevel_map;
	//UpLevelConfig m_up_level_cfg_list[Equipment::E_INDEX_MAX][MAX_STRENGTH_LEVEL + 1];

	// �������
	int InitInheritCfg(TiXmlElement *RootElement);

	// ��ʯ���
	int InitStoneBaseCfg(TiXmlElement *RootElement);
	int InitStoneExAddCfg(TiXmlElement *RootElement);
	std::map<int, StoneCfg> m_stone_map;

	int m_stone_totallvl_cfg_count;
	StoneExAdd m_stone_ex_add_list[TOTAL_STONE_LEVEL_CFG_MAX_COUNT];

	// ��ʯ���ӿ�������
	int InitStoneGridOpenLimit(TiXmlElement *RootElement);
	std::vector<StoneGridOpenLimit> m_stone_grid_open_limit_vct;

	// ��ʯ���ӿ�������(ͨ��ǿ���ȼ�����)
	int InitStoneGridOpenLimitByStrengthLevel(TiXmlElement *RootElement);
	std::vector<StoneGridOpenLimitByStrengthLevel> m_stone_grid_open_limit_by_strength_level_vct;

	// �������
	int InitFulingCfg(TiXmlElement *RootElement);
	EquipFulingConfig m_fuling_list[MAX_FULIN_MAX_LEVEL + 1];

	// ��ħ���
	int InitFumoCfg(TiXmlElement *RootElement);
	std::map<int, EquipFumoConfig> m_fumo_map;

	// ���װ��	
	typedef std::map<ItemID, EquipCrossEquipConfig> CrossEquipMap;
	typedef std::map<ItemID, EquipCrossEquipConfig>::const_iterator CrossEquipMapIt;

	int InitCrossEquip(TiXmlElement *RootElement);
	CrossEquipMap m_cross_equip_map;

	// ��Ʒ�������
	int InitXianPinCfg(TiXmlElement *RootElement);
	int InitXianPinCountCfg(TiXmlElement *RootElement);
	EquipXianPinCountConfig m_xianpin_count_cfg[ItemBase::I_COLOR_MAX];
	EquipXianPinConfig m_xianpin_cfg_list[MAX_XIANPING_CFG_NUM];
	EquipXianPinStarCfg m_xianpinstar_cfg_list[MAX_XIANPING_STAR_LEVEL];
	int m_xianpin_cfg_total_num;
	int m_gongji_xianpin_total_weight[ItemBase::I_COLOR_MAX];
	int m_fangyu_xianpin_taotal_weight[ItemBase::I_COLOR_MAX];

	// �̶���Ʒ
	int InitXianPinFixCfg(TiXmlElement *RootElement);
	typedef std::map<long long, EquipXianPinFixConfig> EquipXianPinFixConfigMap;
	typedef std::map<long long, EquipXianPinFixConfig>::const_iterator EquipXianPinFixConfigMapIt;
	EquipXianPinFixConfigMap m_xianpin_fix_cfg_map;

	// ������Ʒ����
	int InitXianPinDropCfg(TiXmlElement *RootElement);
	typedef std::map<int, EquipXianPinDropConfig> EquipXianPinDropConfigMap;
	typedef std::map<int, EquipXianPinDropConfig>::const_iterator EquipXianPinDropConfigMapIt;
	EquipXianPinDropConfigMap m_xianpin_drop_cfg_map;

	// ����ר������
	int InitEquipSkillCfg(TiXmlElement *RootElement);
	std::map<ItemID, EquipSkillConfig> m_equip_skill_map;

	// װ���ɳ�����
	int InitEquipGrowUpConfig(TiXmlElement *RootElement);
	EquipGrowUpConfig m_equip_grow_up_list[Equipment::EQUIPMENT_TYPE_COUNT];
	
	// װ���ϳ�
	int InitEquipmentCompoundCfg(TiXmlElement *RootElement);
	std::map<int, EquipmentCompoundCfg> m_equipment_compound_cfg;

	int InitEternityCfg(TiXmlElement *RootElement);
	int InitEternitySuitCfg(TiXmlElement *RootElement);
	EquipEternityCfg m_eternity_cfg_list[Equipment::E_INDEX_MAX][MAX_ETERNITY_LEVEL + 1];
	EquipEternitySuitCfg m_eternity_suit_cfg_list[MAX_ETERNITY_LEVEL + 1];

	//��ͨװ���ֽ�
	int InitDecomposeCfg(TiXmlElement *RootElement);
	std::map<int, DecomposeCfg> m_decompose_map;

	//��ͨװ���ȼ���װ
	int InitEquipUpLevelSuitCfg(TiXmlElement *RootElement);
	std::map<int, EquipUpLevelSuitCfg> m_equip_uplevel_suit_map;
};

#endif
