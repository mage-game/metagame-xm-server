#ifndef __REALLYEQUIP_CONFIG_HPP__
#define __REALLYEQUIP_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/basedef.h"
#include <map>

#include "servercommon/struct/itemlistparam.h"

#include "item/itembase.h"

#include "obj/character/attribute.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <vector>

// �ϳ�����
struct ComposeListConfig
{
	ComposeListConfig() : itemid(ItemBase::INVALID_ITEM_ID), coin(0), bind(0)
	{
	}
	const static unsigned int COIN_BIND = 1;

	ItemID itemid;
	unsigned int coin;
	unsigned short bind;	// 1 �� 0 �ǰ�
	unsigned short shuxing; // ��װ�����Լӳ� n%
};
// ��������
struct LevelupListConfig
{
	LevelupListConfig() : itemid(ItemBase::INVALID_ITEM_ID), stuff_id_1(ItemBase::INVALID_ITEM_ID), stuff_count_1(0), stuff_id_2(ItemBase::INVALID_ITEM_ID), stuff_count_2(0)
	{
	}

	ItemID itemid;
	ItemID dirid;
	ItemID stuff_id_1;
	unsigned int stuff_count_1;
	ItemID stuff_id_2;
	unsigned int stuff_count_2;
};

// ��������
struct FeiXianOtherConfig
{
	unsigned short total_score_min;      // ���������
	unsigned short total_score_max;      // ���������
	unsigned short star_num;             // ��Ӧ��������(�ȼ�)
};

// ����װ������
class ReallyequipComposeConfig : public ConfigBase
{
public:
	ReallyequipComposeConfig();
	~ReallyequipComposeConfig();

	bool Init(const std::string &configname, std::string *err);

	const ComposeListConfig * GetCompostListCfg(unsigned int itemid);
	const LevelupListConfig * GetLevelupListCfg(unsigned int itemid);
	const FeiXianOtherConfig* GetOtherCfg(unsigned short score);

private:
	int InitComposeListConfig(PugiXmlNode RootElement);	// �ϳ��б�����
	int InitLevelUpConfig(PugiXmlNode RootElement);		// �����б�����
	int InitOtherConfig(PugiXmlNode RootElement);         // �����б�����

	std::map<int, ComposeListConfig> m_comlist_map;
	std::map<int, LevelupListConfig> m_levelup_map;
	std::vector<FeiXianOtherConfig> m_other_vec;                   // �����������������
};

#endif




