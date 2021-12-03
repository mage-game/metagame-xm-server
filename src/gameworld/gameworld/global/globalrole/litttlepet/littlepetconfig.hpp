#ifndef __LITTLEPET_CONFIG_HPP__
#define __LITTLEPET_CONFIG_HPP__

#include "servercommon/littlepetdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

static const int LITTLEPET_MAX_QUALITY_TYPE = 5;								// С����Ʒ�����͸���
static const int LITTLEPET_MAX_NUM = 20;										// С���������������
static const int LITTLEPET_CHOU_CFG_MAX_COUNT = 50;								// С����齱�����������
static const int LITTLEPET_EXCHANGE_CFG_MAX_NUM = 50;							// С����һ������������
static const int LITTLEPET_FEED_LEVEL_MAX = 100;								// С����ι���ȼ������
static const int LITTLEPET_EQUIP_LEVEL_MAX = 100;								// С����װ���ȼ������

struct LittlePetOtherCfg
{
	LittlePetOtherCfg() : feed_need_gold(0),
		per_pet_interact_times(0), per_user_total_interact_times(0), per_interact_add_score(0), feed_clear_interval_h(0), fangsheng_grid_add_num(0),
		feed_value(0), one_chou_consume_gold(0), ten_chou_consume_gold(0), free_chou_interval_h(0), rename_consume(0),
		replacement_id(0), consume_replacement(0), ten_chou_use_itemid(0), ten_chou_use_item_num(0), can_use_item(true) {}

	int feed_need_gold;
	int per_pet_interact_times;													// ÿ�����ﻥ������
	int per_user_total_interact_times;											// ÿ����ҵ��ܻ�������
	int per_interact_add_score;													// ÿ�λ������Ӵ���
	int feed_clear_interval_h;													// ��ʳ����ռ��
	int fangsheng_grid_add_num;													// һ��ǿ���������ӵķ�����Ʒ����
	int feed_value;																// ÿ�����ӵı�ʳ��
	int one_chou_consume_gold;													// 1����������
	int ten_chou_consume_gold;													// 10����������
	int free_chou_interval_h;													// û�ѳ齱���
	int rename_consume;															// ����������������
	ItemConfigData first_reward_item_cfg;										// �״γ齱��Ʒ
	int choujiang_get_score;													// �齱��û���

	int replacement_id;															//һ��������
	int consume_replacement;													//һ������������
	int ten_chou_use_itemid;													//10��������
	int ten_chou_use_item_num;													//10������������
	bool can_use_item;															//�ܷ�ʹ����Ʒ����Ԫ���齱
};

struct LittlePetBirthValueCfg
{
	LittlePetBirthValueCfg() : value_type(0), value_max(0), value_min(0) {}

	int value_type;
	int value_max;
	int value_min;
};

struct LittlePetStrengthenValueCfg
{
	LittlePetStrengthenValueCfg() : value_type(0), weight(0), value_max(0), value_min(0), value_criticality(0) {}

	int value_type;
	int weight;
	int value_max;
	int value_min;
	int value_criticality;
};

struct LittlePetStrengthenCfg
{
	LittlePetStrengthenCfg() : point_type(-1), baodi_count(0), total_weight(0) {}

	int point_type;
	int baodi_count;
	int total_weight;

	LittlePetStrengthenValueCfg strengthen_value_cfg[LITTLEPET_VALUE_TYPE_MAX_NUM];
};

struct LittlePetQualityCfg
{
	LittlePetQualityCfg() : quality_type(0), grid_num(0), max_feed_degree(0), add_attr_percent(0), qianghua_need_item_id(0), qianghua_need_item_num(0), is_specail(0) {}

	int quality_type;									// Ʒ������
	int grid_num;										// ǿ��������
	int max_feed_degree;								// ��ʳ������
	int add_attr_percent;								// ��ʳ���Լӳ�
	ItemID qianghua_need_item_id;						// ǿ��������Ʒ
	int qianghua_need_item_num;							// ǿ��������Ʒ����
	int is_specail;										// �Ƿ��������

	LittlePetBirthValueCfg base_value_cfg[LITTLEPET_VALUE_TYPE_MAX_NUM];
	LittlePetStrengthenCfg strengthen_cfg[LITTLEPET_QIANGHUAPOINT_CURRENT_NUM];							// С����ǿ����������
};

struct LittlePetBaseCfg
{
	LittlePetBaseCfg() : little_pet_id(0), active_item_id(0), quality_type(0), using_img_id(0) { memset(pet_name, 0, sizeof(GameName)); }

	int little_pet_id;
	int active_item_id;
	int quality_type;
	int using_img_id;
	ItemConfigData return_item;
	GameName pet_name;

	int attr_list[LITTLEPET_VALUE_TYPE_MAX_NUM];
};

struct LittlePetChouJiangCfg
{
	struct ChouJiangCfg
	{
		ChouJiangCfg() : seq(0), weight(0), is_broadcast(0) {}
		int seq;
		int weight;
		int is_broadcast;
		ItemConfigData reward_item_cfg;
	};

	ChouJiangCfg chou_cfg_list[LITTLEPET_CHOU_CFG_MAX_COUNT];
};

struct LittlePetExchangeCfg
{
	LittlePetExchangeCfg() : seq(0), need_score(0) {}

	int seq;
	int need_score;
	ItemConfigData exchange_item_cfg;
};

// С����ι������
struct LittlePetFeedCfg
{
	LittlePetFeedCfg() : quality_type(0), feed_level(0), max_hp(0), gongji(0), fangyu(0), base_attr_add_per(0)
	{
		memset(stuff_id_list, 0, sizeof(stuff_id_list));
		memset(stuff_num_list, 0, sizeof(stuff_num_list));
		memset(stuff_bind_list, 0, sizeof(stuff_bind_list));
	}

	int quality_type;
	int feed_level;
	Attribute max_hp;
	Attribute gongji;
	Attribute fangyu;
	int base_attr_add_per;
	int stuff_id_list[LITTLEPET_MAX_QUALITY_TYPE];
	int stuff_num_list[LITTLEPET_MAX_QUALITY_TYPE];
	int stuff_bind_list[LITTLEPET_MAX_QUALITY_TYPE];
};

// С����װ����������
struct LittlePetEquipUplevelCfg
{
	LittlePetEquipUplevelCfg() : equip_index(0), level(0), stuff_id(0), stuff_num(0), maxhp(0), gongji(0), fangyu(0), base_attrs_add_per(0) {}

	int equip_index;
	int level;
	int stuff_id;
	int stuff_num;
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	int base_attrs_add_per;
};
// struct LittlePetEquipUplevelCfg
// {
// 	struct UplevelCfg  // С����װ�������ȼ�����
// 	{
// 		UplevelCfg() : equip_index(0), quality(0), level(0), stuff_id(0), stuff_num(0), maxhp(0), gongji(0), fangyu(0), base_attrs_add_per(0) {}
// 
// 		int equip_index;
// 		int quality;
// 		int level;
// 		int stuff_id;
// 		int stuff_num;
// 		int maxhp;
// 		int gongji;
// 		int fangyu;
// 		int base_attrs_add_per;
// 	};
// 
// 	LittlePetEquipUplevelCfg()
// 	{
// 		memset(uplevel_cfg, 0, sizeof(uplevel_cfg));
// 	}
// 
// 	UplevelCfg uplevel_cfg[LITTLEPET_MAX_QUALITY_TYPE][LITTLEPET_EQUIP_LEVEL_MAX + 1];
// };

class LittlePetCfg : public ConfigBase
{
public:
	LittlePetCfg();
	~LittlePetCfg();

	bool Init(std::string configname, std::string *err);

	const LittlePetOtherCfg & GetLittlePetOtherCfg() { return m_other_cfg; }						// ��������
	int GetQualityTypeByPetId(int id);																// ��ȡС�����Ʒ������
	const LittlePetBaseCfg * GetBaseCfgById(int Pet_id);
	const LittlePetBaseCfg * GetBaseCfgByItemID(ItemID item_id);
	const LittlePetBaseCfg * GetBaseCfgByImgId(int img_id);
	const LittlePetQualityCfg * GetLittlePetQualityCfg(int quality_type);
	const LittlePetStrengthenCfg * GetLittlePetStrengthenCfg(int quality_type, int point_type);
	int GetLittlePetChouCfgCount() { return  m_chou_cfg_count; }
	int GetLittlePetChouTotalWeight() { return  m_chou_total_weight; }
	const LittlePetChouJiangCfg & GetLittlePetChouCfg();
	int GetLittlePetExchangeCfgCount() { return m_exchange_cfg_count; }
	const LittlePetExchangeCfg * GetLittlePetExchangeCfg(int seq);

	const LittlePetEquipUplevelCfg * GetEquipmentUplevelCfg(int equip_index, int level);
	const LittlePetFeedCfg * GetFeedCfg(int quality_type, int feed_level);
	bool CheckSeq(int seq);
private:

	int InitOtherCfg(PugiXmlNode RootElement);
	int InitQualityCfg(PugiXmlNode RootElement);
	int InitLittlePetBaseCfg(PugiXmlNode RootElement);
	int InitStrengthenCfg(PugiXmlNode RootElement);
	int InitChouCfg(PugiXmlNode RootElement);
	int InitExchangeCfg(PugiXmlNode RootElement);
	int InitFeedCfg(PugiXmlNode RootElement);
	int InitEquipUplevelCfg(PugiXmlNode RootElement);

	LittlePetOtherCfg m_other_cfg;																		// ��������

	int m_base_cfg_count;																				// С������������
	LittlePetBaseCfg m_base_cfg[LITTLEPET_MAX_NUM];														// ���С������������

	LittlePetQualityCfg m_quality_cfg[LITTLEPET_MAX_QUALITY_TYPE + 1];									// С����Ʒ������

	int m_chou_cfg_count;																				// С����齱��������
	int m_chou_total_weight;																			// �齱��Ȩ��
	LittlePetChouJiangCfg m_chou_cfg;																	// С����齱����

	int m_exchange_cfg_count;
	LittlePetExchangeCfg m_exchang_cfg[LITTLEPET_EXCHANGE_CFG_MAX_NUM];									// С����һ�����

	LittlePetFeedCfg m_feed_cfg_list[LITTLEPET_MAX_QUALITY_TYPE][LITTLEPET_FEED_LEVEL_MAX + 1];			// ����ι������

	LittlePetEquipUplevelCfg m_equip_uplevel_cfg_list[LITTLEPET_EQUIP_INDEX_MAX_NUM][LITTLEPET_EQUIP_LEVEL_MAX + 1]; // װ�����������б�
};


#endif // __LITTLEPET_CONFIG_HPP__
