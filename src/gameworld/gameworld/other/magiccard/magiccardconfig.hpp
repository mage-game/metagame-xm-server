#ifndef __MAGIC_CARD_CONFIG_H__
#define __MAGIC_CARD_CONFIG_H__

#include "obj/character/attribute.hpp"
#include "common/tinyxml/tinyxml.h"
#include "servercommon/magiccarddef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <set>
#include "item/itembase.h"

struct MagicCardOtherConfig
{
	MagicCardOtherConfig() : open_level(0), purple_chou_card_consume_gold(0), orange_chou_card_consume_gold(0), orange_chou_card_consume_item_id(0), day_free_times(0), red_chou_card_consume_gold(0), red_chou_card_consume_item_id(0) {}

	int open_level;												// ���ŵȼ�
	int purple_chou_card_consume_gold;							// �Ͽ��齱1������Ԫ��
	int orange_chou_card_consume_gold;							// �ȿ��齱1������Ԫ��
	int orange_chou_card_consume_item_id;						//�ȿ��齱1��������Ʒ
	int day_free_times;											// ÿ����Ѵ���
	int red_chou_card_consume_gold;								// �쿨�齱1������Ԫ��
	int red_chou_card_consume_item_id;							//�쿨�齱1��������Ʒ
};

struct MagicCardSuitConfig
{
	MagicCardSuitConfig() : color(0), skill_need_level(0), skill_id(0), attr_need_level(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0) {}

	int color;																// ħ����ɫ

	int skill_need_level;													// ȫ�״ﵽָ���ȼ������
	int skill_id;															// �����ID

	int attr_need_level;													// ȫ�״ﵽָ���ȼ�������װ����
	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
};

struct MagicCardInfoConfig
{
	struct StrengthAttr
	{
		StrengthAttr() : strength_level(0), up_level_need_exp(0), contain_exp(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), 
			shanbi(0) {}

		int strength_level;						// ǿ���ȼ�
		int up_level_need_exp;					// �������辭��
		int contain_exp;						// ������ת���ľ���


		Attribute maxhp;
		Attribute gongji;
		Attribute fangyu;
		Attribute mingzhong;
		Attribute shanbi;
	};

	MagicCardInfoConfig() : card_id(0), item_id(ItemBase::INVALID_ITEM_ID), color(0), slot_index(0), type(0), need_sprit_num(0), strength_max_level(0) {}

	const StrengthAttr * GetStrenthAttrCfg(int strength) const;

	int card_id;															// ħ��ID
	ItemID item_id;															// ��Ӧ����ƷID
	int color;																// ħ����ɫ
	int slot_index;															// ����λ��
	int type;																// ����
	int need_sprit_num;														// �һ���Ҫħ������

	int strength_max_level;
	StrengthAttr strength_attr_list[MAGIC_CARD_LIMIT_STRENGTH_LEVEL_MAX + 1]; // ǿ�������б��±����ǿ���ȼ�
};

struct MagicCardChouCardConfig
{
	MagicCardChouCardConfig() : reward_seq(0), reward_card_id(0), reward_count(0), weight(0), is_broadcast(false) {}

	int reward_seq;															// ��Ʒ���
	int reward_card_id;														// ��Ʒħ��ID
	int reward_count;														// ��Ʒ����
	int weight;																// Ȩ��

	bool is_broadcast;														// �Ƿ񷢴���
};

enum MAGIC_CARD_CHOU_CARD_TYPE
{
	MAGIC_CARD_CHOU_CARD_TYPE_PURPLE = 0,									// �Ͽ��齱
	MAGIC_CARD_CHOU_CARD_TYPE_ORAGEN,										// �ȿ��齱
	MAGIC_CARD_CHOU_CARD_TYPE_RED,											// �쿨�齱

	MAGIC_CARD_CHOU_CARD_TYPE_MAX_NUM,
};

enum MAGIC_CARD_CARD_TYPE
{
	MAGIC_CARD_CARD_TYPE_NORMAL_CARD = 0,									// ��ͨ���ƿ���
	MAGIC_CARD_CARD_TYPE_SPRITE,											// ħ��
	MAGIC_CARD_CARD_TYPE_EXP_CARD,											// ���鿨

	MAGIC_CARD_CARD_TYPE_MAX_NUM,
};


class MagicCardConfig : public ConfigBase
{
public:
	MagicCardConfig();
	~MagicCardConfig();

	bool Init(const std::string &configname, std::string *err);

	const MagicCardOtherConfig & GetOtherCfg() { return m_other_cfg; }
	const MagicCardSuitConfig * GetCardSuitCfg(int color);
	const MagicCardInfoConfig * GetCardCfg(int card_id);
	const MagicCardChouCardConfig * GetRandomRewardCfg(int type);
	int GetSpriteCardId(int color);
	int GetCardIdWithItemID(ItemID item_id);

protected:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitCardSuitCfg(PugiXmlNode RootElement);
	int InitAllCardInfoCfg(PugiXmlNode RootElement);
	int InitStrengthCfg(PugiXmlNode RootElement);
	int InitPurpleChouCardCfg(PugiXmlNode RootElement);
	int InitOrangeChouCardCfg(PugiXmlNode RootElement);
	int InitRedChouCardCfg(PugiXmlNode RootElement);
	MagicCardOtherConfig m_other_cfg;

	int m_card_suit_cfg_count;
	MagicCardSuitConfig m_card_suit_cfg_list[MAGIC_CARD_COLOR_TYPE_COLOR_COUNT];

	int m_all_card_count;
	MagicCardInfoConfig m_all_card_list[MAGIC_CARD_MAX_LIMIT_COUNT + 1];

	std::map<ItemID, int> m_item_id_to_card_id_map;

	int m_purple_choucard_cfg_count;
	MagicCardChouCardConfig m_purple_choucard_cfg_list[MAGIC_CARD_CHOU_CARD_LIMIT_REWARD_COUNT];

	int m_orange_choucard_cfg_count;
	MagicCardChouCardConfig m_orange_choucard_cfg_list[MAGIC_CARD_CHOU_CARD_LIMIT_REWARD_COUNT];

	int m_red_choucard_cfg_count;
	MagicCardChouCardConfig m_red_choucard_cfg_list[MAGIC_CARD_CHOU_CARD_LIMIT_REWARD_COUNT];

	int m_sprite_list_cout;
	int m_sprite_card_id_list[MAGIC_CARD_COLOR_TYPE_COLOR_COUNT];
};

#endif

