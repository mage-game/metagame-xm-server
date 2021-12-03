#include "roleskillcondition.hpp"

#include "skill/skillpool.hpp"
#include "servercommon/configcommon.h"

RoleSkillCondition::RoleSkillCondition()
{
	
}

RoleSkillCondition::~RoleSkillCondition()
{
	
}

bool RoleSkillCondition::Init(const std::string &path, std::string *err)
{
	PugiXmlDocument m_document;
	PUGI_XML_LOAD_AND_CHECK(m_document, path, "RoleSkillCondition", *err);

	PugiXmlNode RootElement = m_document.document_element();
	if (!RootElement)
	{
		*err = path + ": xml root node error.";
		return false;
	}

	PugiXmlNode CondElement = RootElement.child("cond");
	if (CondElement.empty())
	{
		*err = path + ": xml root->cond node error.";
		return false;
	}

	PugiXmlNode dataElement = CondElement.child("data");

	while (!dataElement.empty())
	{
		UInt16 skill_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "skill_id", skill_id) || 0 == skill_id || NULL == SKILLPOOL->GetSkill(skill_id))
		{
			*err = path + ": xml node error in [config->cond->data->skill]";
			return false;
		}

		CondInfo &info = m_roleskill_cond_map[skill_id];

		if (!PugiGetSubNodeValue(dataElement, "type", info.type) || info.type < 0 || info.type >= ROLE_SHADOW_SKILL_COND_MAX)
		{
			*err = path + ": xml node error in [config->cond->data->type]";
			return false;
		}

		if (!PugiGetSubNodeValue(dataElement, "global_cd", info.global_cd) || info.global_cd < 0)
		{
			*err = path + ": xml node error in [config->cond->data->global_cd]";
			return false;
		}

		dataElement = dataElement.next_sibling();
	}

	return true;
}

int RoleSkillCondition::GetSkillCondType(unsigned short skill_id)
{
	SkillCondMarkMapIt it = m_roleskill_cond_map.find(skill_id);
	if (it != m_roleskill_cond_map.end())
	{
		return (it->second).type;
	}

	return ROLE_SHADOW_SKILL_COND_2;
}

int RoleSkillCondition::GetGlobalCD(unsigned short skill_id)
{
	SkillCondMarkMapIt it = m_roleskill_cond_map.find(skill_id);
	if (it != m_roleskill_cond_map.end())
	{
		return (it->second).global_cd;
	}

	return 500;
}

