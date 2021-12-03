#include "gameconfigcommon.h"
#include "gamecommon.h"
#include "monster/monsterpool.h"
#include "obj/character/character.h"

bool StrListConfig::ReadConfig(TiXmlElement * data_element, const char *node)
{
	std::string tmp_str;
	if (!GetSubNodeValue(data_element, node, tmp_str))
	{
		return false;
	}

	char buffer[1024] = { 0 };
	if (SNPRINTF(buffer, sizeof(buffer), "%s", tmp_str.c_str()) < 0)
	{
		return false;
	}

	char *token = strtok(buffer, "|");
	while (NULL != token)
	{
		str_list.emplace_back(token);
		token = strtok(NULL, "|");
	}

	return true;
}

bool StrListConfig::ReadConfig(pugi::xml_node data_element, const pugi::char_t* name)
{
	std::string tmp_str;

	if (!PugiGetSubNodeValue(data_element, name, tmp_str))
	{
		return false;
	}

	char buffer[1024] = { 0 };
	if (SNPRINTF(buffer, sizeof(buffer), "%s", tmp_str.c_str()) < 0)
	{
		return false;
	}

	char *token = strtok(buffer, "|");
	while (NULL != token)
	{
		str_list.emplace_back(token);
		token = strtok(NULL, "|");
	}

	return true;
}

bool StrListConfig::Check(int policy)
{
	if (CHECK_POLICY_NONE == policy) return true;

	for (std::vector<std::string>::iterator it = str_list.begin(); it != str_list.end(); ++ it)
	{
		const std::string &value = *it;
		switch (policy)
		{
		case CHECK_POLICY_GRANTER_0:
			{
				int n = 0;
				if ( !StringToBaseType(value, n) || n <= 0)
				{
					return false;
				}
			}
			break;

		case CHECK_POLICY_MONSTER:
			{
				int monster_id;
				if ( !StringToBaseType(value, monster_id) || NULL == MONSTERPOOL->GetMonsterParam(monster_id))
				{
					return false;
				}
			}
			break;
		}
	}

	return true;
}

int LuckyConfig::Random() const
{
	auto rand_item = gamecommon::RandListItem(luck_item_list);
	if (!rand_item) return 0;
	return rand_item->val;
}

int AttributesConfig::ReadConfig(PugiXmlNode  data_element)
{
	if (!PugiGetSubNodeValue(data_element, "maxhp", maxhp) || maxhp < 0) { return -1; }
	if (!PugiGetSubNodeValue(data_element, "gongji", gongji) || gongji < 0) { return -2; }
	if (!PugiGetSubNodeValue(data_element, "fangyu", fangyu) || fangyu < 0) { return -3; }
	if (!PugiGetSubNodeValue(data_element, "mingzhong", mingzhong) || mingzhong < 0) { return -4; }
	if (!PugiGetSubNodeValue(data_element, "shanbi", shanbi) || shanbi < 0) { return -5; }
	if (!PugiGetSubNodeValue(data_element, "baoji", baoji) || baoji < 0) { return -6; }
	if (!PugiGetSubNodeValue(data_element, "jianren", jianren) || jianren < 0) { return -7; }

	return 0;
}

void AttributesConfig::AddAttrs(CharIntAttrs &attrs, double factor) const
{
	attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(maxhp * factor);
	attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(gongji * factor);
	attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(fangyu * factor);
	attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(mingzhong * factor);
	attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(shanbi * factor);
	attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(baoji * factor);
	attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(jianren * factor);
}

void AttributesConfig::AddFightAttrs(CharIntAttrs &attrs, double factor /*= 1.0*/) const
{
	attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP] += static_cast<Attribute>(maxhp * factor);

	attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(maxhp * factor);
	attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(gongji * factor);
	attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(fangyu * factor);
	attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(mingzhong * factor);
	attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(shanbi * factor);
	attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(baoji * factor);
	attrs.m_attrs[CharIntAttrs::FIGHT_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(jianren * factor);

}

//////////////////////////////////////////////////////////////////////////
const RandItemConfig::RandItem * RandItemConfig::Rand() const
{
	if (this->Empty()) return nullptr;

	return gamecommon::RandListItem(rand_item_list);
}

