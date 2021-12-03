#ifndef __CONVERT_SHOP_HPP__
#define __CONVERT_SHOP_HPP__

#include <string>

#include "protocal/msgshop.h"
#include "item/itembase.h"
#include <map>
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class Role;

class ConvertShop : public ConfigBase
{
public:
	ConvertShop();
	~ConvertShop();

	enum SCORE_TO_ITEM_TYPE										
	{
		SCORE_TO_ITEM_TYPE_INVALID = 0, 

		SCORE_TO_ITEM_TYPE_HUNYIN,											// ��ӡ�һ�
		SCORE_TO_ITEM_TYPE_NORMAL_ITEM,										// ���߶һ�
		SCORE_TO_ITEM_TYPE_TREASURE_HUNT,									// Ѱ���һ�		
		SCORE_TO_ITEM_TYPE_EQUIP,											// װ���һ�
		SCORE_TO_ITEM_TYPE_JINGLING,										// ����һ�
		SCORE_TO_ITEM_TYPE_HAPPYTREE,										// ���ֹ����һ�
		SCORE_TO_ITEM_TYPE_CS_EQUIP1,										// װ��Ѱ���̵�һ�1		 6��Ʒ ����
		SCORE_TO_ITEM_TYPE_MYSTERIOUS_SHOP,									// �����̵�һ�
		SCORE_TO_ITEM_TYPE_TREASURE,										// �䱦�һ�
		SCORE_TO_ITEM_TYPE_ZHUANZHI_STONE_SCORE,							// ��ʯ����һ�
		SCORE_TO_ITEM_TYPE_HUNJING,											// �꾧�һ�
		SCORE_TO_ITEM_TYPE_GUILDSHOP,										// �����̵�
		SCORE_TO_ITEM_TYPE_JUST_USE_ITEM,									// ����һ�������Ʒ����Ʒ

		SCORE_TO_ITEM_TYPE_MAX,
	};

	enum SCORE_TO_ITEM_PRICE_TYPE											// �һ����ļ۸�����
	{
		SCORE_TO_ITEM_PRICE_TYPE_INVALID = 0,

		SCORE_TO_ITEM_PRICE_TYPE_MOJING = 1,								// ħ��-�һ���
		SCORE_TO_ITEM_PRICE_TYPE_SHENGWANG,									// ����-�һ���
		SCORE_TO_ITEM_PRICE_TYPE_GONGXUN,									// ��ѫ-�һ���
		SCORE_TO_ITEM_PRICE_TYPE_WEIWANG,									// ����-�һ���
		SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT,						// Ѱ������-�һ���
		SCORE_TO_ITEM_PRICE_TYPE_JINGLING_CREDIT,							// ����Ѱ������-�һ���
		SCORE_TO_ITEM_PRICE_TYPE_HAPPY_TREE,								// ���ֹ����ɳ�-�һ���
		SCORE_TO_ITEM_PRICE_TYPE_CROSS_HONOR,								// �������
		SCORE_TO_ITEM_PRICE_TYPE_GUANGHUI,									// ���-�һ���
		SCORE_TO_ITEM_PRICE_TYPE_MYSTERIOUS_SHOP,							// �����̵�
		SCORE_TO_ITEM_PRICE_TYPE_BLUE_LINGZHI,								// ����ֵ
		SCORE_TO_ITEM_PRICE_TYPE_PURPLE_LINGZHI,							// ����ֵ
		SCORE_TO_ITEM_PRICE_TYPE_ORANGE_LINGZHI,							// ����ֵ
		SCORE_TO_ITEM_PRICE_TYPE_PRECIOUS_BOSS_SCORE,						// �ز�boss����
		SCORE_TO_ITEM_PRICE_TYPE_SHENZHOUWEAPON_SCORE,						// ������
		SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT1,						// Ѱ������1-�һ���
		SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT2,						// Ѱ������2-�һ���
		SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT3,						// Ѱ������3-�һ���
		SCORE_TO_ITEM_PRICE_TYPE_ZHUANZHI_STONE_SCORE,						// תְ��ʯ����
		SCORE_TO_ITEM_PRICE_TYPE_HUNJING,									// �꾧-�һ���
		SCORE_TO_ITEM_PRICE_TYPE_GONGXIAN,									// ���˹���
		SCORE_TO_ITEM_PRICE_TYPE_JUST_USE_ITEM,								// ����������Ʒ����Ʒ

		SCORE_TO_ITEM_PRICE_TYPE_MAX,
	};

	enum CONVERT_REQUIRE_TYPE													// �һ���Ʒ��Ҫ�������������
	{
		CONVERT_REQUIRE_TYPE_INVALID = 0,									
		CONVERT_REQUIRE_TYPE_SUIT_LEVEL,											// ��������Ҫ����ȼ�����

		CONVERT_REQUIRE_TYPE_MAX
	};

	bool Init(const std::string &configname, std::string *err);
	int InitMultipleCost(PugiXmlNode RootElement);
	int InitConverShop(PugiXmlNode RootElement);

	void ConvertScoreToItem(Role *role, Protocol::CSScoreToItemConvert &stic);
	void CheckLimitTimeConvertItem(Role *role, int level);

private:
	bool CheckConvertRequire(Role *role, int convert_type, int conver_index, int convert_num);
	bool CheckConvertReqValid(Role *role, Protocol::CSScoreToItemConvert &stic);
	
	int GetPriceMultipleCost(int multiplecost_id, int times);
	
	struct ConvertShopItem
	{
		ConvertShopItem() : item_id(ItemBase::INVALID_ITEM_ID), is_bind(0), price_type(0), price(0), need_stuff_id(0), need_stuff_count(0), 
			require_type(0), require_value_min(0), require_value_max(0), limit_convert_count(0), multiple_cost_id(0), lifetime_convert_count(0), is_world_notice(0),
			is_guild_notice(0), from_open_game_day(0), to_open_game_day(0), limit_conver_time(0) {}

		ItemID item_id;
		short is_bind;
		int price_type;
		int price;
		int need_stuff_id;
		int need_stuff_count;

		int require_type;
		int require_value_min;
		int require_value_max;
		
		int limit_convert_count;
		int multiple_cost_id;
		int lifetime_convert_count;

		short is_world_notice;
		short is_guild_notice;
		int from_open_game_day;				//�ӿ����ڼ��쿪ʼ
		int to_open_game_day;				//�����������ڼ���
		int limit_conver_time;				// ��ʱ�һ�ʱ��(�ӵȼ�����ʱ��)
	};

	struct ConvertShopItemList
	{
		ConvertShopItemList() : count(0) {}

		int count;
		ConvertShopItem item_list[Protocol::MAX_CONVERT_SCORE_TO_ITEM_NUM];
	};
	ConvertShopItemList m_convert_shop_item_list[SCORE_TO_ITEM_TYPE_MAX];

	struct MultipleCostItem
	{
		MultipleCostItem() : multiple_cost_id(0), times_min(0), times_max(0), price_multile(0) {}

		int multiple_cost_id;
		int times_min;
		int times_max;
		int price_multile;
	};

	static const int MAX_CONVERT_MULTIPLE_NUM = 10;
	struct MultipleCostItemList
	{
		MultipleCostItemList() : multiple_cost_id(0) {}

		int multiple_cost_id;
		MultipleCostItem item_list[MAX_CONVERT_MULTIPLE_NUM];
	};
	std::map<int, MultipleCostItemList> m_multiplecost_map;
};

#endif // __CONVERT_SHOP_HPP__

