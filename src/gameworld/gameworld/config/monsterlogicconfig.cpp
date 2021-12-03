#include "monsterlogicconfig.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"

#include "monster/monsterpool.h"

MonsterSpecialLogicConfig::MonsterSpecialLogicConfig()
{
}

MonsterSpecialLogicConfig::~MonsterSpecialLogicConfig()
{

}

bool MonsterSpecialLogicConfig::Init(const std::string &configname, std::string *err)
{
	//char errinfo[1024] = { 0 };

	PugiXmlDocument m_document;  // ¼ÓÔØÅäÖÃ
	PUGI_XML_LOAD_AND_CHECK(m_document, configname, "MonsterSpecialLogicConfig", *err);

	PugiXmlNode root_element = m_document.document_element();
	if (!root_element)
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	PugiXmlNode special_monster_element = root_element.child("special_monster");
	while (!special_monster_element.empty())
	{
		unsigned short monster_id = 0;
		if (!PugiGetSubNodeValue(special_monster_element, "monster_id", monster_id) || !MONSTERPOOL->IsMonsterExist(monster_id))
		{
			*err = configname + ": xml node error in [monster_id]";
			return false;
		}

		PugiXmlNode logic_item_element = special_monster_element.child("logic_item");
		if (logic_item_element.empty())
		{
			*err = configname + ": xml node error in [logic_item]";
			return false;
		}

		MonsterSpecialParam param;

		param.count = 0;

		while (!logic_item_element.empty())
		{
			if (param.count < 0 || param.count >= SPECIAL_LOGIC_ITEM_COUNT)
			{
				*err = configname + ": xml error param.count";
				return false;
			}

			MonsterSpecialLogicItem &logic_item = param.special_logic_item_list[param.count];

			if (!PugiGetSubNodeValue(logic_item_element, "special_logic_type", logic_item.special_type) || logic_item.special_type <= MONSTER_SPECIAL_LOGIC_TYPE_INVALID || logic_item.special_type >= MONSTER_SPECIAL_LOGIC_TYPE_MAX)
			{
				*err = configname + ": xml node error in [special_logic_type]";
				return false;
			}

			if (!PugiGetSubNodeValue(logic_item_element, "param1", logic_item.param1))
			{
				*err = configname + ": xml node error in [param1]";
				return false;
			}

			if (!PugiGetSubNodeValue(logic_item_element, "param2", logic_item.param2))
			{
				*err = configname + ": xml node error in [param2]";
				return false;
			}

			if (!PugiGetSubNodeValue(logic_item_element, "param3", logic_item.param3))
			{
				*err = configname + ": xml node error in [param3]";
				return false;
			}

			if (!PugiGetSubNodeValue(logic_item_element, "param4", logic_item.param4))
			{
				*err = configname + ": xml node error in [param4]";
				return false;
			}

			if (!this->CheckSpecialLogicItemConfig(logic_item, *err))
			{
				*err = configname + *err;
				return false;
			}

			param.count++;

			logic_item_element = logic_item_element.next_sibling("logic_item");
		}

		if (param.count <= 0)
		{
			*err = configname + ": xml error param.count 2";
			return false;
		}

		if (m_monster_special_param_map.find(monster_id) != m_monster_special_param_map.end())
		{
			*err = configname + ": xml error monster_id repeat";
			return false;
		}

		m_monster_special_param_map[monster_id] = param;

		special_monster_element = special_monster_element.next_sibling("special_monster");
	}

	return true;
}

MonsterSpecialParam *MonsterSpecialLogicConfig::GetMonsterSpecialParam(unsigned short monster_id)
{
	MonsterSpecialParamMapIt it = m_monster_special_param_map.find(monster_id);
	if (it != m_monster_special_param_map.end())
	{
		return &it->second;
	}

	return NULL;
}

bool MonsterSpecialLogicConfig::CheckSpecialLogicItemConfig(MonsterSpecialLogicItem &logic_item, std::string &err)
{
	switch (logic_item.special_type)
	{
	case MONSTER_SPECIAL_LOGIC_TYPE_TWINS:
		{
			if (logic_item.param1 < 0)
			{
				err = "[param1] is key, should >= 0";
				return false;
			}
			if (logic_item.param2 <= 0)
			{
				err = "[param2] is relive_time_ms, should > 0";
				return false;
			}
		}
		break;

	case MONSTER_SPECIAL_LOGIC_TYPE_RELATIVE:
		{
			if (logic_item.param1 < 0)
			{
				err = "[param1] is key, should >= 0";
				return false;
			}
			if (0 == logic_item.param2 && 0 == logic_item.param3)
			{
				err = "[param1 and param2] is percent and fix, can't all to 0";
				return false;
			}
		}
		break;

	default:
		err = "invalid special_type";
		return false;
	}

	return true;
}
