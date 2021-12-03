#ifndef __ITEMLIST_PARAM_H__
#define __ITEMLIST_PARAM_H__

#include "servercommon/basedef.h"
#include "common/tlvprotocol.h"
#include "servercommon/servercommon.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/roleshopdef.hpp"

class ItemParam;

namespace ItemNamespace
{
	static const int DEFAULT_KNAPSACK_GRID_NUM = 100;								// 默认背包初始格子数量 如果初始配置改了这里也要改
	static const int DEFAULT_STORAGE_GRID_NUM = 20;									// 默认仓库初始格子数量 

	static const int MAX_KNAPSACK_GRID_NUM = 400;									// 最大背包格子数量
	static const int MAX_STORAGE_GRID_NUM = 400;									// 最大仓库格子数量
	static const int MAX_EQUIPMENT_GRID_NUM = 11;									// 最大装备格子数量
		
	static const int STORAGE_INDEX_BEGIN = MAX_KNAPSACK_GRID_NUM;					// 仓库起始位置即背包最大数
	static const int EQUIPMENT_INDEX_BEGIN = 1000;									// 装备起始格子

	static const int MAX_KNAPSACK_STORAGE_GRID_NUM = MAX_KNAPSACK_GRID_NUM + MAX_STORAGE_GRID_NUM;	// 背包仓库 数组大小

	static const int MAX_GRID_NUM = MAX_KNAPSACK_STORAGE_GRID_NUM + MAX_EQUIPMENT_GRID_NUM;			// 背包仓库再加上装备物品的总格子大小

	static const int MAX_BATCH_CONSUME_ITEM_NUM = 4;								// 批量消耗，单次最大物品种类
	static const int MAX_XIANPIN_NUM = 6;											// 卓越属性最大数量

	UNSTD_STATIC_CHECK(MAX_CHEST_XIANPIN_NUM == MAX_XIANPIN_NUM);

	static const int MAX_USE_NOREXITEM_RECORD_NUM = 100;
}

static const int JING_LING_SKILL_COUNT_MAX = 12;									// 精灵技能最大数量

#pragma pack(push, 4)
// 精灵技能信息
struct JingLingSkillInfo
{
	JingLingSkillInfo() : skill_id(0), can_move(0), is_open(0) {}

	void Reset() { skill_id = 0; can_move = 0;}

	short skill_id;
	char can_move;
	char is_open;      // 槽位开启
};

struct ItemParamDataStruct		// 本结构不可有虚函数 因为整个结构要以字节拷贝 要用到协议中 注意字节对齐
{
	static const int RAND_ATTR_NUM = 3;

	ItemParamDataStruct() { this->Reset(); }

	ItemParamDataStruct & operator = (const ItemParamDataStruct & rh)
	{
		if (&rh == this) return *this;

		quality = rh.quality;
		strengthen_level = rh.strengthen_level;
		shen_level = rh.shen_level;
		fuling_level = rh.fuling_level;

		star_level = rh.star_level;
		has_lucky = rh.has_lucky;
		fumo_id = rh.fumo_id;
		xianpin_equip = rh.xianpin_equip;
	
		memcpy(xianpin_type_list, rh.xianpin_type_list, sizeof(xianpin_type_list));

		param1 = rh.param1;
		param2 = rh.param2;
		param3 = rh.param3;

		rand_attr_type_1 = rh.rand_attr_type_1;
		rand_attr_val_1 = rh.rand_attr_val_1;
		rand_attr_type_2 = rh.rand_attr_type_2;
		rand_attr_val_2 = rh.rand_attr_val_2;
		rand_attr_type_3 = rh.rand_attr_type_3;
		rand_attr_val_3 = rh.rand_attr_val_3;

		memcpy(jing_ling_skill_list, rh.jing_ling_skill_list, sizeof(jing_ling_skill_list));
		
		really = rh.really;
		xianpin_star_num = rh.xianpin_star_num;

		return *this;
	}

	bool IsEqual(const ItemParamDataStruct &param_data) const
	{
		if (quality != param_data.quality) return false;
		if (strengthen_level != param_data.strengthen_level) return false;
		if (shen_level != param_data.shen_level) return false;
		if (fuling_level != param_data.fuling_level) return false;
		if (star_level != param_data.star_level) return false;
		if (has_lucky != param_data.has_lucky) return false;
		if (fumo_id != param_data.fumo_id) return false;
		if (xianpin_equip != param_data.xianpin_equip) return false;
		if (param1 != param_data.param1) return false;
		if (param2 != param_data.param2) return false;
		if (param3 != param_data.param3) return false;
		for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; ++i)
		{
			if (xianpin_type_list[i] != param_data.xianpin_type_list[i])
			{
				return false;
			}
		}

		if (rand_attr_type_1 != param_data.rand_attr_type_1) return false;
		if (rand_attr_val_1 != param_data.rand_attr_val_1) return false;
		if (rand_attr_type_2 != param_data.rand_attr_type_2) return false;
		if (rand_attr_val_2 != param_data.rand_attr_val_2) return false;
		if (rand_attr_type_3 != param_data.rand_attr_type_3) return false;
		if (rand_attr_val_3 != param_data.rand_attr_val_3) return false;

		for (int i = 0; i < JING_LING_SKILL_COUNT_MAX; ++i)
		{
			if (jing_ling_skill_list[i].skill_id != param_data.jing_ling_skill_list[i].skill_id
				|| jing_ling_skill_list[i].can_move != param_data.jing_ling_skill_list[i].can_move
				|| jing_ling_skill_list[i].is_open != param_data.jing_ling_skill_list[i].is_open)
			{
				return false;
			}
		}
		if (really != param_data.really) return false;
		if (xianpin_star_num != param_data.xianpin_star_num) return false;

		return true;
	}

	void Reset()
	{
		quality = 0;
		strengthen_level = 0; 
		shen_level = 0; 
		fuling_level = 0;

		star_level = 0;
		has_lucky = 0;
		fumo_id = 0;
		xianpin_equip = 0;

		memset(xianpin_type_list, 0, sizeof(xianpin_type_list));

		param1 = param2 = param3 = 0;

		rand_attr_type_1 = 0;
		rand_attr_type_2 = 0;
		rand_attr_type_3 = 0;
		reserve_type = 0;
		rand_attr_val_1 = 0;
		rand_attr_val_2 = 0;
		rand_attr_val_3 = 0;
		rand_attr_val_reserve = 0;

		memset(jing_ling_skill_list, 0, sizeof(jing_ling_skill_list));
		really = 0;
		xianpin_star_num = 0;
	}

	short quality;													// 品质
	short strengthen_level;											// 强化等级
	short shen_level;												// 神铸等级
	short fuling_level;												// 附灵等级
	
	short star_level;
	short has_lucky;												// 是否有幸运属性
	short fumo_id;													// 附魔id
	short xianpin_equip;                                            // 仙品装备,1100-1109属于仙品装备

	unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM];		// 仙品属性类型

	int param1;
	int param2;
	int param3;

	unsigned char rand_attr_type_1;											// 随机属性类型1
	unsigned char rand_attr_type_2;											// 随机属性类型2
	unsigned char rand_attr_type_3;											// 随机属性类型3
	unsigned char reserve_type;

	unsigned short rand_attr_val_1;											// 随机属性1
	unsigned short rand_attr_val_2;											// 随机属性2
	unsigned short rand_attr_val_3;											// 随机属性3
	unsigned short rand_attr_val_reserve;

	JingLingSkillInfo jing_ling_skill_list[JING_LING_SKILL_COUNT_MAX];		// 精灵技能信息

	unsigned short really;													// 真*装备
	unsigned short xianpin_star_num;                                        // 仙品星星数量
};

class TiXmlElement;

struct ItemConfigData
{
	ItemConfigData() : item_id(0), is_bind(false), num(0) {}
	ItemConfigData(ItemID _item_id, bool _is_bind, int _num) : item_id(_item_id), is_bind(_is_bind), num(_num) {}
	ItemConfigData(const ItemConfigData & other) {item_id = other.item_id; is_bind = other.is_bind; num = other.num;}

	ItemID item_id;
	bool is_bind;
	int num;

	bool ReadConfig(const PugiXmlNode & element);
	bool ReadConfigNoCheck(const PugiXmlNode & element);

	// 返回读取数量，负数代表错误
	static int ReadConfigList(const PugiXmlNode & element, const std::string &element_name, ItemConfigData *item_list, int max_count);
	static int ReadConfigListNoCheck(const PugiXmlNode & element, const std::string &element_name, ItemConfigData *item_list, int max_count);

	int ReadConfigByName(const PugiXmlNode & data_element, const std::string &element_name);
	int ReadConfigByNameNoCheck(const PugiXmlNode & data_element, const std::string &element_name);

	//增加const char *版本，尽量避免string构造
	int ReadConfigByName(const PugiXmlNode & data_element, const char *element_name);
	int ReadConfigByNameNoCheck(const PugiXmlNode & data_element, const char *element_name);

	bool ReadConfig(TiXmlElement *element);
	bool ReadConfigNoCheck(TiXmlElement *element);

	// 返回读取数量，负数代表错误
	static int ReadConfigList(TiXmlElement *element, const std::string &element_name, ItemConfigData *item_list, int max_count);
	static int ReadConfigListNoCheck(TiXmlElement *element, const std::string &element_name, ItemConfigData *item_list, int max_count);
};

struct ItemDataWrapper
{
	ItemDataWrapper() : item_id(0), num(0), is_bind(0), has_param(0), invalid_time(0), gold_price(0) {}
	ItemDataWrapper(ItemID _item_id, short _num, short _is_bind) : item_id(_item_id), num(_num), is_bind(_is_bind), has_param(0), invalid_time(0), gold_price(0) {}
	ItemDataWrapper(ItemID _item_id, short _num, short _is_bind, ItemParam *item_param, UInt32 _invalid_time, int _gold_price);

	static void RevealConfigItemId(int config_item_id, ItemID *item_id, short *is_bind)
	{
		if (NULL == item_id || NULL == is_bind) return;

		*is_bind = short((config_item_id / 100000) != 0);
		*item_id = config_item_id % 100000;
	}

	bool IsEqual(const ItemDataWrapper &item_wrapper, bool ignore_num = false) const
	{
		if (item_id != item_wrapper.item_id || invalid_time != item_wrapper.invalid_time ||
			is_bind != item_wrapper.is_bind || has_param != item_wrapper.has_param) return false;
		if (!ignore_num && item_wrapper.num != num) return false; 

		if (!has_param) return true;
		
		return param_data.IsEqual(item_wrapper.param_data);
	}

	bool Invalid() const { return 0 == item_id || num <= 0; }

	bool Serialize(TLVSerializer &outstream) const
	{
		bool ret = outstream.Push(item_id) && outstream.Push(num) && outstream.Push(is_bind)
			&& outstream.Push(has_param) && outstream.Push(invalid_time) && outstream.Push(gold_price);

		if (!ret) return false;

		if (0 != has_param)
		{
			TLVSerializer itemparam_data;
			itemparam_data.Reset((void*)&(param_data), sizeof(ItemParamDataStruct));
			itemparam_data.MoveCurPos(sizeof(ItemParamDataStruct));

			ret = outstream.Push(itemparam_data);

			if (!ret) return false;
		}

		return true;
	}

	bool Unserialize(TLVUnserializer &instream)
	{
		bool ret = instream.Pop(&item_id) && instream.Pop(&num) && instream.Pop(&is_bind)
			&& instream.Pop(&has_param) && instream.Pop(&invalid_time) && instream.Pop(&gold_price);

		if (!ret) return false;

		if (0 != has_param)
		{
			static TLVUnserializer itemparam_data;
			if (!instream.Pop(&itemparam_data)) return false;

			if (itemparam_data.Size() <= sizeof(ItemParamDataStruct))
			{
				memcpy(&param_data, itemparam_data.Ptr(), itemparam_data.Size());
			}
		}

		return true;
	}

	void Reset()
	{
		item_id = 0; num = 0; is_bind = 0; has_param = 0; invalid_time = 0; gold_price = 0; param_data.Reset(); 
	}

	void Reset(const ItemDataWrapper &data_wrapper)
	{
		item_id = data_wrapper.item_id;
		num = data_wrapper.num;
		is_bind = data_wrapper.is_bind;
		has_param = data_wrapper.has_param;
		invalid_time = data_wrapper.invalid_time;
		gold_price = data_wrapper.gold_price;
		param_data = data_wrapper.param_data;
	}

	bool CanMerge(const ItemDataWrapper &data_wrapper)
	{
		if (&data_wrapper == this)
		{
			return false;
		}
		if (this->Invalid() || data_wrapper.Invalid())
		{
			return true;
		}
		if (item_id != data_wrapper.item_id || 
			num + data_wrapper.num < num ||
			is_bind != data_wrapper.is_bind || 
			has_param || data_wrapper.has_param || 
			invalid_time != data_wrapper.invalid_time || 
			gold_price != data_wrapper.gold_price || 
			!param_data.IsEqual(data_wrapper.param_data))
		{
			return false;
		}
		return true;
	}

	bool Merge(const ItemDataWrapper &data_wrapper)
	{
		if (!this->CanMerge(data_wrapper))
		{
			return false;
		}
		if (this->Invalid() && !data_wrapper.Invalid())
		{
			item_id = data_wrapper.item_id;
			num += data_wrapper.num;
			is_bind = data_wrapper.is_bind;
			has_param = data_wrapper.has_param;
			invalid_time = data_wrapper.invalid_time;
			gold_price = data_wrapper.gold_price;
			param_data = data_wrapper.param_data;
		}
		else
		{
			num += data_wrapper.num;
		}
		return true;
	}

	ItemID item_id;
	short num;
	short is_bind;
	short has_param;
	UInt32 invalid_time;
	int gold_price;
	ItemParamDataStruct param_data;
};

struct EquipSuitInfo
{
	EquipSuitInfo() : suit_id(0), count_flag(0) {}

	UInt16 suit_id;
	short count_flag;
};

struct EquipSuitInfoList
{
	static const int MAX_SUIT_COUNT_ON_BODY = 7;

	EquipSuitInfoList() { this->Reset(); }

	void Reset() { count = 0; reserve = 0; memset(suit_list, 0, sizeof(suit_list)); }

	short count;
	short reserve;
	EquipSuitInfo suit_list[MAX_SUIT_COUNT_ON_BODY];
};

struct NorextItemUseDayTimes
{
	NorextItemUseDayTimes() : item_id(0), times(0) {}
	ItemID item_id;
	short  times;
};

#pragma pack(pop)

typedef char ItemParamDataStructHex[sizeof(ItemParamDataStruct) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ItemParamDataStructHex) < 1024)

class ItemListParam 
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct ItemListData 
	{
		char change_state_item;	
		short index;

		ItemDataWrapper item_wrapper;
	};

	int count;
	ItemListData item_list[ItemNamespace::MAX_GRID_NUM];
};

static const int MAX_EQUIP_PART = 11;
struct EquipmentGridItem
{
	EquipmentGridItem() : strengthen_level(0), shenzhu_level(0), star_level(0), eternity_level(0), equip_level(0), star_exp(0) {}

	short strengthen_level;
	short shenzhu_level;
	short star_level;
	short eternity_level;
	short equip_level;
	int star_exp;
};

struct EquipmentGridParam
{
	EquipmentGridParam()
	{
		this->Reset();
	}

	void Reset() 
	{
		use_eternity_level = 0;
		cur_grade_suit_level = 0;
		memset(equi_grid_item_list, 0, sizeof(equi_grid_item_list));
	}

	short use_eternity_level;
	short cur_grade_suit_level;

	EquipmentGridItem equi_grid_item_list[MAX_EQUIP_PART];
};

typedef char EquipmentGridParamHex[sizeof(EquipmentGridParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(EquipmentGridParamHex) < 512)

#endif
