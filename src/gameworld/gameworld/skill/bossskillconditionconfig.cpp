#include "bossskillconditionconfig.hpp"

#include "servercommon/configcommon.h"

#include "skill/skillpool.hpp"

BossSkillConditionConfig::BossSkillConditionConfig()
	: m_bossskill_cond_id(0), m_skill_count(0), m_goback(true), m_hate_drive(false)
{
	memset(m_skill_list, 0, sizeof(m_skill_list));
}

BossSkillConditionConfig::~BossSkillConditionConfig()
{

}

bool BossSkillConditionConfig::LoadConfig(const char *path, std::string *err)
{
	TiXmlDocument m_document;
	char errinfo[1024];

	if (0 == path || !m_document.LoadFile(path))
	{
		sprintf(errinfo,"%s: Load BossSkillCondition Config Error, %s.", path, m_document.ErrorDesc());
		*err = errinfo;
		return false;
	}

	TiXmlElement *RootElement = m_document.RootElement();
	if (!RootElement)
	{
		sprintf(errinfo,"%s: xml node error in root.", path);
		*err = errinfo;
		return false;
	}
	
	if (!GetSubNodeValue(RootElement, "id", m_bossskill_cond_id) || 0 == m_bossskill_cond_id)
	{
		sprintf(errinfo,"%s: xml node error in [config->id].", path);
		*err = errinfo;
		return false;
	}

	int goback = 0;
	if (!GetSubNodeValue(RootElement, "goback", goback) || goback < 0)
	{
		sprintf(errinfo,"%s: xml node error in [config->goback].", path);
		*err = errinfo;
		return false;
	}
	m_goback = (0 != goback);

	int hate_drive = 0;
	if (!GetSubNodeValue(RootElement, "hate_drive", hate_drive) || hate_drive < 0)
	{
		sprintf(errinfo,"%s: xml node error in [config->hate_drive].", path);
		*err = errinfo;
		return false;
	}
	m_hate_drive = (0 != hate_drive);

	{
		TiXmlElement *cond_listElement = RootElement->FirstChildElement("cond_list"); 
		if (!cond_listElement)
		{
			sprintf(errinfo,"%s: xml node error in [config->cond_list].", path);
			*err = errinfo;
			return false;
		}

		TiXmlElement *condElement = cond_listElement->FirstChildElement("cond"); 
		while (condElement)
		{
			int cond_type = 0;
			if (!GetSubNodeValue(condElement, "cond_type", cond_type) || cond_type < BOSS_SKILL_COND_HPLOW_PER || cond_type >= BOSS_SKILL_COND_MAX)
			{
				sprintf(errinfo,"%s: xml node error in [config->cond_list->cond:%d invalid].", path, cond_type);
				*err = errinfo;
				return false;
			}

			BossSkillTypeCond &type_cond = m_typecond_list[cond_type];
			if (type_cond.count >= BOSS_SKILL_TYPE_COND_COUNT)
			{
				sprintf(errinfo,"%s: xml node error in [config->cond_list->cond:%d too much].", path, cond_type);
				*err = errinfo;
				return false;
			}
			
			BossSkillCondParam &cond_param = type_cond.lists[type_cond.count];

			{
				if (!GetSubNodeValue(condElement, "param0", cond_param.cond_param_list[0]))
				{
					sprintf(errinfo,"%s: xml node error in [config->cond_list->cond->param0:%d invalid].", path, cond_param.cond_param_list[0]);
					*err = errinfo;
					return false;
				}
				if (!GetSubNodeValue(condElement, "param1", cond_param.cond_param_list[1]))
				{
					sprintf(errinfo,"%s: xml node error in [config->cond_list->cond->param1:%d invalid].", path, cond_param.cond_param_list[1]);
					*err = errinfo;
					return false;
				}
				if (!GetSubNodeValue(condElement, "param2", cond_param.cond_param_list[2]))
				{
					sprintf(errinfo,"%s: xml node error in [config->cond_list->cond->param2:%d invalid].", path, cond_param.cond_param_list[2]);
					*err = errinfo;
					return false;
				}
				if (!GetSubNodeValue(condElement, "param3", cond_param.cond_param_list[3]))
				{
					sprintf(errinfo,"%s: xml node error in [config->cond_list->cond->param3:%d invalid].", path, cond_param.cond_param_list[3]);
					*err = errinfo;
					return false;
				}
				
				switch (cond_type)
				{
				case BOSS_SKILL_COND_HPLOW_PER:		// param 低血量百分比
					{
						if (type_cond.count > 0 && cond_param.cond_param_list[0] <= type_cond.lists[type_cond.count - 1].cond_param_list[0])
						{
							sprintf(errinfo,"%s: xml node error in [config->cond_list->cond->param0:%d, should appear before ].", path, cond_param.cond_param_list[0]);
							*err = errinfo;
							return false;
						}
					}
					break;

				case BOSS_SKILL_COND_HPLOW_RANGE:	// param 血量范围低 血量范围高
					{
						if (cond_param.cond_param_list[0] >= cond_param.cond_param_list[1])
						{
							sprintf(errinfo,"%s: xml node error in [config->cond_list->cond->param0:%d param1:%d, Invalid].", path, cond_param.cond_param_list[0], cond_param.cond_param_list[1]);
							*err = errinfo;
							return false;
						}
					}
					break;

				case BOSS_SKILL_COND_FIGHTTIMES:			// param 战斗时间毫秒
					{
						if (cond_param.cond_param_list[0] < 0)
						{
							sprintf(errinfo,"%s: xml node error in [config->cond_list->cond->param0:%d invalid time value].", path, cond_param.cond_param_list[0]);
							*err = errinfo;
							return false;
						}
					}
					break;

				case BOSS_SKILL_COND_ON_DIE:		// 
					break;

				case BOSS_SKILL_COND_BORNTIME:				// param 出生时间豪秒
					{
						if (cond_param.cond_param_list[0] < 0)
						{
							sprintf(errinfo,"%s: xml node error in [config->cond_list->cond->param0:%d invalid time value].", path, cond_param.cond_param_list[0]);
							*err = errinfo;
							return false;
						}
					}
					break;

				case BOSS_SKILL_COND_ANYWAY:
					break;
				}

				UNSTD_STATIC_CHECK(BOSS_SKILL_PARAM_COUNT == 4); // 上面直接用下标 改这个值 必须改上面代码 调用点的代码也得改

				{
					TiXmlElement *skilllist_Element = condElement->FirstChildElement("skill_list"); 
					if (!skilllist_Element)
					{
						sprintf(errinfo,"%s: xml node error in [config->cond_list->cond->skill_list].", path);
						*err = errinfo;
						return false;
					}

					TiXmlElement *skillElement = skilllist_Element->FirstChildElement("skill"); 
					if (!skillElement)
					{
						sprintf(errinfo,"%s: xml node error in [config->cond_list->cond->skill_list->skill].", path);
						*err = errinfo;
						return false;
					}
				
					while (skillElement)
					{
						if (cond_param.cond_param_skill_count >= BOSS_SKILL_PARAM_SKILL_COUNT)
						{
							sprintf(errinfo,"%s: xml node error in [config->cond_list->cond->skill_list->skill count too much].", path);
							*err = errinfo;
							return false;
						}

						UInt16 skill_id = 0;
						if (!GetSubNodeValue(skillElement, "skill_id", skill_id) || 0 == skill_id)
						{
							sprintf(errinfo,"%s: xml node error in [config->cond_list->cond->skill_list->skill->skill_id:%d invalid].", path, skill_id);
							*err = errinfo;
							return false;
						}
						if (NULL == SKILLPOOL->GetSkill(skill_id))
						{
							sprintf(errinfo,"%s: xml node error in [config->cond_list->cond->skill_list->skill->skill_id:%d not exist].", path, skill_id);
							*err = errinfo;
							return false;
						}

						int skill_index = 0;
						if (!this->AddBossSkillID(skill_id, &skill_index))
						{
							sprintf(errinfo,"%s: xml node error in [config->cond_list->cond->skill_list->skill too much].", path);
							*err = errinfo;
							return false;
						}

						cond_param.cond_param_skill_index_list[cond_param.cond_param_skill_count] = skill_index + 1; // 人为约束技能列表的顺序
						++ cond_param.cond_param_skill_count;

						skillElement = skillElement->NextSiblingElement("skill");
					}
					
				}
			}

			++ type_cond.count;

			condElement = condElement->NextSiblingElement("cond");
		}
	}

	return true;
}

bool BossSkillConditionConfig::AddBossSkillID(UInt16 skill_id, int *skill_index)
{
	int i = 0;
	for (i = 0; i < m_skill_count; i++)
	{
		if (skill_id == m_skill_list[i])
		{
			if (NULL != skill_index) *skill_index = i;
			return true;
		}
	}

	if (i >= m_skill_count && m_skill_count >= SkillListParam::MAX_SKILL_NUM - 1) return false; 
	
	if (NULL != skill_index) *skill_index = m_skill_count;

	m_skill_list[m_skill_count ++] = skill_id;

	return true;
}

