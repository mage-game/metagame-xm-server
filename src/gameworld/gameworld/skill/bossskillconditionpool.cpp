#include "bossskillconditionpool.hpp"
#include "bossskillconditionconfig.hpp"

#include "servercommon/configcommon.h"

BossSkillConditionPool::BossSkillConditionPool()
{
	memset(m_boss_skill_cond_list, 0, sizeof(m_boss_skill_cond_list));
}

BossSkillConditionPool::~BossSkillConditionPool()
{
	this->Release();
}

bool BossSkillConditionPool::Init(const std::string &dir, std::string *err)
{
	char errinfo[1024] = {0};

	std::string configname = "bossskillconditionmanager.xml";

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, dir + configname, "BossSkillConditionPool", *err);

	PugiXmlNode RootElement = document.document_element();
	if (!RootElement)
	{
		*err = configname + ": xml root node error.";
		return false;
	}

	PugiXmlNode currentElement = RootElement.child("path");
	while (!currentElement.empty())
	{
		std::string path;
		if (!PugiGetNodeValue(currentElement, path))
		{
			this->Release();
			return false;
		}

		BossSkillConditionConfig *bossskill_cond_cfg = new BossSkillConditionConfig();
		if (!bossskill_cond_cfg->LoadConfig((dir+path).c_str(),err))
		{
			delete bossskill_cond_cfg;
			this->Release();
			return false;
		}

		if (0 != m_boss_skill_cond_list[bossskill_cond_cfg->GetBossSkillCondID()])
		{
			sprintf(errinfo,"%s: Load BossSkillConditionPool Config Error, BossSkillCondition id [%d] is repeat.", (dir+configname).c_str(), bossskill_cond_cfg->GetBossSkillCondID());
			*err = errinfo;

			delete bossskill_cond_cfg;
			this->Release();
			return false;
		}

		m_boss_skill_cond_list[bossskill_cond_cfg->GetBossSkillCondID()] = bossskill_cond_cfg;

		currentElement = currentElement.next_sibling();
	}

	return true;
}

const BossSkillConditionConfig * BossSkillConditionPool::GetBossSkillConditionConfig(UInt16 bossskill_cond_id) const
{
	if (bossskill_cond_id < 0 || bossskill_cond_id >= MAX_BOSS_SKILL_CONDITION_ID)
	{
		return NULL;
	}

	return m_boss_skill_cond_list[bossskill_cond_id];
}

void BossSkillConditionPool::Release()
{
	for (int i = 0; i < MAX_BOSS_SKILL_CONDITION_ID; ++i)
	{
		if (NULL != m_boss_skill_cond_list[i])
		{
			delete m_boss_skill_cond_list[i];
		}
	}
	memset(m_boss_skill_cond_list, 0, sizeof(m_boss_skill_cond_list));
}


