#include "skillmanager.hpp"
#include "skillbase.hpp"
#include "skillpool.hpp"
#include "servercommon/serverclmempool.h"
#include "servercommon/commondata.hpp"

#include "obj/character/character.h"
#include "obj/character/role.h"
#include "engineadapter.h"

#include "scene/scene.h"

#include "protocal/msgrole.h"

#include "globalconfig/globalconfig.h"

#include "gamelog.h"

#include "config/logicconfigmanager.hpp"
#include "other/upgradeskill/upgradeskillconfig.hpp"
#include "servercommon/struct/structcommon.h"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "other/event/eventhandler.hpp"
#include "scene/speciallogic/speciallogic.hpp"
#include "hightimermgr.h"
#include "other/mojie/mojieconfig.hpp"
#include "other/capability/capability.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "other/shenshou/shenshou.hpp"
#include "other/shop/roleshop.hpp"

#define INT32_ONE_BIT ((long long)0x00000001)

const static int ROLE_DELAY_SKILL_1 = 221;		// 延时释放技能
const static int ROLE_DELAY_SKILL_2 = 421;		// 延时释放技能

//精灵技能
const static int JINGLING_SKILL_START_ID = 151;
const static int JINGLING_SKILL_END_ID = 170;

const static int MOJIE_SKILL_START_ID = 70;
const static int MOJIE_SKILL_END_ID = 72;

const static int XIANNV_SKILL_START_ID = 80;
const static int XIANNV_SKILL_END_ID = 86;

SkillManager::SkillManager()
	: m_character(NULL), m_is_change(false), m_default_skill_index(0), m_next_skill_index(0), 
	m_next_skill_pos(0, 0), m_next_skill_target(INVALID_OBJ_ID), m_next_skill_data(0), m_capablity(0),
	m_is_addtimer(false), m_is_delay_perform_skill(false), m_delay_perform_time_ms(0), m_delay_skill_index(0),
	m_delay_skill_target_id(INVALID_OBJ_ID), m_delay_skill_target(NULL), m_delay_special_param(0)
{
	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));
	memset(m_old_state, 0, sizeof(m_old_state));
}

SkillManager::~SkillManager()
{
	this->CancelTimer();
}

void SkillManager::Init(Character *character, const SkillListParam &skill_list, const SkillOtherParam &skill_other_param)
{
	m_character = character;

	m_skill_other_param = skill_other_param;

	int count = 0;
	if (skill_list.count < SkillListParam::MAX_SKILL_NUM)
	{
		count = skill_list.count;
	}
	else
	{
		count = SkillListParam::MAX_SKILL_NUM;
	}

	for (int i = count - 1; i >= 0; --i)
	{
		int index = skill_list.skill_list[i].index;
		if (index < 0 || index >= SkillListParam::MAX_SKILL_NUM)
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[SkillManager::Init] [skill index is invalid:%d]", index);
			continue;
		}
		
		if (!m_skill_list[index].Invalid())
		{
			gamelog::g_log_serious_error.printf(LL_CRITICAL, "[SkillManager::Init] [skill index duplicate:%d]",  index);
			continue; 
		}

		m_skill_list[index].skill_id = skill_list.skill_list[i].skill_id;
		m_skill_list[index].level = skill_list.skill_list[i].level;
		m_skill_list[index].last_perform_time = static_cast<unsigned int>(skill_list.skill_list[i].last_perform_time);
		m_skill_list[index].exp = skill_list.skill_list[i].exp;

		m_old_state[index] = true;
	}

	this->InitDefaultSkill();
}

void SkillManager::GetInitParam(SkillOtherParam *skill_other_param)
{
	*skill_other_param = m_skill_other_param;
}

void SkillManager::Update(time_t now_second)
{
}

// 检查攻击距离够不够
bool SkillManager::CheckNextSkillDistance(Character *target, Posi *pos)
{
	if (m_next_skill_index < 0 || m_next_skill_index >= SkillListParam::MAX_SKILL_NUM || m_skill_list[m_next_skill_index].Invalid()) 
	{
		return false;
	}

	const SkillBase *skill = SKILLPOOL->GetSkill(m_skill_list[m_next_skill_index].skill_id);
	if (NULL == skill) return false;

	Posi target_pos;
	if (skill->IsAttackCharacter())
	{
		if (NULL == target) return false;

		target_pos = target->GetPos();
	}
	else												
	{
		/*
		这之前的逻辑，如果用的是打地面技能就假设已经设置了技能的next_skill_pos，但怪物AI判断默认技能能否打到的时候是没设置next_skill_pos的
		*/

		if (NULL != pos) 
		{
			target_pos = *pos;
		}
		else
		{
			target_pos = m_next_skill_pos;
		}		
	}

	int atk_dis = skill->GetAttackDis();

	if (Obj::OBJ_TYPE_ROLE == m_character->GetObjType())
	{
		atk_dis += PERFORM_SKILL_ADD_CHECK_DISTANCE;
	}

	return !(target_pos - m_character->GetPos()).IsLongerThan(atk_dis);
}

// 检查下一个技能的CD
bool SkillManager::CheckNextSkillCoolDown()
{
	return this->CheckSkillCoolDown(m_next_skill_index);
}

bool SkillManager::CheckSkillCoolDown(int skill_index)
{
	if (skill_index < 0 || skill_index >= SkillListParam::MAX_SKILL_NUM || m_skill_list[skill_index].Invalid()) 
	{
		return false;
	}

	const SkillBase *skill = SKILLPOOL->GetSkill(m_skill_list[skill_index].skill_id);
	if (NULL == skill) return false;
	
	int cooldown_suo_duan_time = 0;
	if (m_character != NULL && Obj::OBJ_TYPE_ROLE == m_character->GetObjType())
	{
		Role *role = (Role*)m_character;

		int reduce_cold_down = role->GetChineseZodiac()->GetSkillCoolDownShuoduan();
		if (reduce_cold_down > 6000)
		{
			reduce_cold_down = 6000;
		}

		// 星座秘籍缩短冷却时间
		cooldown_suo_duan_time = static_cast<unsigned int>(skill->GetCoolDown(m_skill_list[skill_index].level) * (reduce_cold_down * ROLE_ATTR_PER));
		if (cooldown_suo_duan_time < 0)
		{
			cooldown_suo_duan_time = 0;
		}
	}

	unsigned int now = (unsigned int)EngineAdapter::Instance().Time();
	return (now - m_skill_list[skill_index].last_perform_time) >= skill->GetCoolDown(m_skill_list[skill_index].level) - cooldown_suo_duan_time;
}

bool SkillManager::CheckSkillTargetEnemy(int skill_index)
{
	if (skill_index < 0 || skill_index >= SkillListParam::MAX_SKILL_NUM || m_skill_list[skill_index].Invalid()) 
	{
		return false;
	}

	const SkillBase *skill = SKILLPOOL->GetSkill(m_skill_list[skill_index].skill_id);
	if (NULL == skill) return false;

	return skill->IsTargetEnemy();
}

bool SkillManager::CheckSkillAttackCharacter(int skill_index)
{
	if (skill_index < 0 || skill_index >= SkillListParam::MAX_SKILL_NUM || m_skill_list[skill_index].Invalid()) 
	{
		return false;
	}

	const SkillBase *skill = SKILLPOOL->GetSkill(m_skill_list[skill_index].skill_id);
	if (NULL == skill) return false;

	return skill->IsAttackCharacter();
}

bool SkillManager::CheckNextSkillAttackCharacter()
{
	if (m_next_skill_index < 0 || m_next_skill_index >= SkillListParam::MAX_SKILL_NUM || m_skill_list[m_next_skill_index].Invalid()) 
	{
		return false;
	}

	const SkillBase *skill = SKILLPOOL->GetSkill(m_skill_list[m_next_skill_index].skill_id);
	if (NULL == skill) return false;

	return skill->IsAttackCharacter();
}

bool SkillManager::CanPerformRoleSkill(char prof, int skill_index)
{
	if (skill_index < 0 || skill_index >= SkillListParam::MAX_SKILL_NUM || m_skill_list[skill_index].Invalid()) 
	{
		return false;
	}

	return true;
}

bool SkillManager::NextIsChongfengSkill()
{
	if (m_next_skill_index < 0 || m_next_skill_index >= SkillListParam::MAX_SKILL_NUM || m_skill_list[m_next_skill_index].Invalid()) 
	{
		return false;
	}

	const SkillBase *skill = SKILLPOOL->GetSkill(m_skill_list[m_next_skill_index].skill_id);
	if (NULL == skill) return false;

	return (SkillBase::SKILL_TYPE_ACTIVE_CHONGFENG == skill->GetSkillType());
}

bool SkillManager::NextIsNuqiSkill()
{
	if (m_next_skill_index < 0 || m_next_skill_index >= SkillListParam::MAX_SKILL_NUM || m_skill_list[m_next_skill_index].Invalid()) 
	{
		return false;
	}

	const SkillBase *skill = SKILLPOOL->GetSkill(m_skill_list[m_next_skill_index].skill_id);
	if (NULL == skill) return false;

	return (SkillBase::SKILL_TYPE_NUQI == skill->GetSkillType());
}

bool SkillManager::NextValidInSaveArea()
{
	if (m_next_skill_index < 0 || m_next_skill_index >= SkillListParam::MAX_SKILL_NUM || m_skill_list[m_next_skill_index].Invalid()) 
	{
		return false;
	}

	const SkillBase *skill = SKILLPOOL->GetSkill(m_skill_list[m_next_skill_index].skill_id);
	if (NULL == skill) return false;

	return skill->ValidInSaveArea();
}

bool SkillManager::NextStopActionwhenPerform()
{
	if (m_next_skill_index < 0 || m_next_skill_index >= SkillListParam::MAX_SKILL_NUM || m_skill_list[m_next_skill_index].Invalid()) 
	{
		return false;
	}

	const SkillBase *skill = SKILLPOOL->GetSkill(m_skill_list[m_next_skill_index].skill_id);
	if (NULL == skill) return false;

	return skill->StopWhenPerform();
}

void SkillManager::PerformNextSkill(Character *target, int special_param)
{
	if (m_next_skill_index < 0 || m_next_skill_index >= SkillListParam::MAX_SKILL_NUM || m_skill_list[m_next_skill_index].Invalid()) 
	{
		return;
	}

	if (NULL == m_character->GetScene())
	{
		return;
	}

	UInt16 skill_id = m_skill_list[m_next_skill_index].skill_id;
	const SkillBase *skill = SKILLPOOL->GetSkill(skill_id);
	if (NULL == skill) return;
	if (skill->IsAttackCharacter() && NULL == target) return;

	int skill_index = m_next_skill_index;
	Posi skill_pos = m_next_skill_pos;
	m_next_skill_index = m_default_skill_index;
	m_next_skill_pos = Posi(0, 0);
	m_next_skill_target = INVALID_OBJ_ID;

// 	if (Obj::OBJ_TYPE_ROLE == m_character->GetObjType() && SkillBase::SKILL_TYPE_NUQI == skill->GetSkillType())
// 	{
// 		Role *role = (Role *)m_character;
// 		role->AddNuqi(-1 * GLOBALCONFIG->GetMaxNuqi());
// 	}

	// 神兽技能
	int reduce_cd = 0;
	if (Obj::OBJ_TYPE_ROLE == m_character->GetObjType() && !this->IsCommonSkill(skill->GetSkillId()))
	{
		Role *role = (Role *)m_character;
		role->GetShenshou()->RecalcCD(skill->GetSkillId(), (int)skill->GetCoolDown(m_skill_list[skill_index].level), reduce_cd);
		m_skill_list[skill_index].last_perform_time -= reduce_cd;
		this->SendSkillInfo(skill_index);
	}

	if ((SKILL_PHASE_READING == special_param && SkillBase::TIPS_WAY_READING == skill->GetTipsWay()) ||
		(SKILL_PHASE_PERFORM == special_param && SkillBase::TIPS_WAY_PERFORM == skill->GetTipsWay()))
	{
		skill->SendSkillTip(m_character);
	}

	if ((ROLE_DELAY_SKILL_1 == skill_id || ROLE_DELAY_SKILL_2 == skill_id) && skill->GetParamA(m_skill_list[m_next_skill_index].level) > 0)
	{
		this->DelayPerformSkill(skill_index, target, skill_pos, special_param, skill->GetParamA(m_skill_list[m_next_skill_index].level));
	}
	else
	{
		this->PerformSkillHelper(skill_index, target, skill_pos, special_param);
	}
}

void SkillManager::InitDefaultSkill()
{
	if (Obj::OBJ_TYPE_ROLE == m_character->GetObjType() || Obj::OBJ_TYPE_ROLE_SHADOW == m_character->GetObjType())
	{
		int default_skill_id_list[globalconfig::MAX_DEFAULT_SKILL_COUNT] = {0};
		// 获取默认技能的数量
		int default_skill_id_count = GLOBALCONFIG->GetDefaultSkillID(m_character->SkillGetProf(), default_skill_id_list);
		if (default_skill_id_count > 0 && default_skill_id_count <= globalconfig::MAX_DEFAULT_SKILL_COUNT)
		{
			for (int i = 0; i < default_skill_id_count; i++)
			{
				int skill_index = 0;
				if (default_skill_id_list[i] > 0 && !this->GetSkillByID(default_skill_id_list[i], &skill_index))
				{	
					if (this->AddSkill(default_skill_id_list[i], 1, &skill_index, true))
					{
						// 默认技能赋值普通攻击三连击的第一击
						if (0 == i)
						{            
							m_default_skill_index = skill_index;
							m_next_skill_index = m_default_skill_index;
						}
					}
				}
			}
		}

		// 怒气技能学习
		int skill_index = 0;
		if (!this->GetSkillByID(NUQI_SKILL_ID, &skill_index))
		{
			this->AddSkill(NUQI_SKILL_ID, 0, &skill_index, true);
		}
	}
}

void SkillManager::SendSkillListInfo(bool is_init)
{
	if (Obj::OBJ_TYPE_ROLE != m_character->GetObjType()) return; 

	Role *role = (Role*)m_character;

	static Protocol::SCSkillListInfoAck sra;
	sra.default_skill_index = static_cast<short>(m_default_skill_index);
	sra.is_init = (is_init ? 1 : 0);
	sra.count = 0;

	for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; ++i)
	{
		if (m_skill_list[i].Invalid()) continue;

		sra.skill[sra.count].index = static_cast<short>(i);
		sra.skill[sra.count].skill_id = m_skill_list[i].skill_id;
		sra.skill[sra.count].level = m_skill_list[i].level;
		sra.skill[sra.count].exp = m_skill_list[i].exp;
		sra.skill[sra.count].last_perform = m_skill_list[i].last_perform_time;

		++ sra.count;
		if (sra.count >= Protocol::SC_MAX_SKILL_ITEM_NUM) break;
	}

	int sendlen = sizeof(sra) - (Protocol::SC_MAX_SKILL_ITEM_NUM - sra.count) * sizeof(sra.skill[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&sra, sendlen);
}

void SkillManager::SendOtherSkillInfo()
{
// 	if (Obj::OBJ_TYPE_ROLE != m_character->GetObjType()) return; 
// 	Role *role = (Role*)m_character;
// 
// 	Protocol::SCSkillOtherSkillInfo sosi;
// 	sosi.skill124_effect_baoji = m_skill_other_parma.skill124_effect_baoji;
// 	sosi.skill124_effect_star = m_skill_other_parma.skill124_effect_star;
// 	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&sosi, sizeof(sosi));
}

bool SkillManager::AddSkill(UInt16 skill_id, int skill_level, int *out_skill_index /* = NULL */, bool is_init /* = false */, bool is_notify /* = true */)
{
	if (skill_level <= 0 || skill_level > MAX_SKILL_LEVEL) return false;

	const SkillBase *skill_base = SKILLPOOL->GetSkill(skill_id);
	if (NULL == skill_base || skill_level > skill_base->GetMaxLevel()) return false;

	if (Obj::OBJ_TYPE_ROLE == m_character->GetObjType())
	{
		Role *role = (Role *)m_character;
		if (!role->IsProfSuitable(skill_base->GetProfLimit()))
		{
			return false;
		}
	}

	int skill_index = 0;
	if (this->GetSkillByID(skill_id, &skill_index))
	{
		if (skill_index < 0 || skill_index >= SkillListParam::MAX_SKILL_NUM) return false;

		m_skill_list[skill_index].level = skill_level;
		m_dirty_mark[skill_index] = true;
		m_is_change = true;

		if (NULL != out_skill_index) *out_skill_index = skill_index;
	}
	else 
	{
		int free_slot = -1;
		for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; i++)
		{
			if (m_skill_list[i].Invalid()) 
			{
				free_slot = i;
				break;
			}
		}
		if (free_slot < 0 || free_slot >= SkillListParam::MAX_SKILL_NUM) return false;

		m_skill_list[free_slot].skill_id = skill_id;
		m_skill_list[free_slot].level = skill_level;

		m_dirty_mark[free_slot] = true;
		m_is_change = true;

		skill_index = free_slot;
		if (NULL != out_skill_index) *out_skill_index = free_slot;
	}

	if (!is_init && is_notify) this->SendSkillListInfo();

	if (Obj::OBJ_TYPE_ROLE == m_character->GetObjType())
	{
		if (!is_init)
		{
			Role *role = (Role *)m_character;
			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SKILL);
		}
	}

	return true;
}

void SkillManager::AddSkillByTask(Role *role, int prof, int skill_num)
{
	if (NULL == role) return;

	if (prof > 0 && prof <= MAX_PROF && skill_num >= 0 && skill_num < SKILL_COUNT)
	{
		int prof_index = prof - 1;
		int skill_id = SKILL_ID_LIST[prof_index][skill_num];
		const SkillBase *skill_base = SKILLPOOL->GetSkill(skill_id);
		if (NULL != skill_base)
		{
			int skill_index = 0;
			if (!this->GetSkillByID(skill_id, &skill_index))
			{
				if (role->IsProfSuitable(skill_base->GetProfLimit())) 
				{
					this->AddSkill(skill_id, 1);
				}
			}
		}
	}
}

bool SkillManager::RemoveSkill(UInt16 skill_id, bool is_notify /* = true */)
{
	int skill_index = 0; 
	//int skill_level = 0;
	if (this->GetSkillByID(skill_id, &skill_index)) 
	{
		//skill_level = m_skill_list[skill_index].level;

		m_skill_list[skill_index].Reset();

		m_dirty_mark[skill_index] = true;
		m_is_change = true;

		const SkillBase *skill = SKILLPOOL->GetSkill(skill_id);

		if (NULL != skill)
		{
			if (Obj::OBJ_TYPE_ROLE == m_character->GetObjType())
			{
				Role *role = (Role *)m_character;
				role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SKILL);
			}
		}		
		if (is_notify) this->SendSkillListInfo();
		return true;
	}

	return false;
}

bool SkillManager::GmSetSkillLevel(UInt16 skill_id, int skill_level)
{
	int skill_index = 0; 
	if (this->GetSkillByID(skill_id, &skill_index)) 
	{
		m_skill_list[skill_index].level = skill_level;

		m_dirty_mark[skill_index] = true;
		m_is_change = true;

		const SkillBase *skill = SKILLPOOL->GetSkill(skill_id);
		if (NULL != skill)
		{
			if (Obj::OBJ_TYPE_ROLE == m_character->GetObjType())
			{
				Role *role = (Role *)m_character;
				role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SKILL);
			}
		}

		return true;
	}

	return false;
}

//只允许增加一种精灵技能
bool SkillManager::AddJingLingSkill(UInt16 skill_id, int skill_level)
{
	bool has_same_skill = false;

	for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; ++ i)
	{
		if (m_skill_list[i].skill_id >= JINGLING_SKILL_START_ID && m_skill_list[i].skill_id <= JINGLING_SKILL_END_ID)
		{
			if (m_skill_list[i].skill_id == skill_id)
			{
				has_same_skill = true;
				continue;
			}
			this->RemoveSkill(m_skill_list[i].skill_id);
		}
	}

	if (has_same_skill)
	{
		return true;
	}

	if (skill_id >= JINGLING_SKILL_START_ID && skill_id <= JINGLING_SKILL_END_ID)
	{
		int out_skill_index = 0;
		if (this->AddSkill(skill_id, skill_level, &out_skill_index) && out_skill_index > 0 && out_skill_index < SkillListParam::MAX_SKILL_NUM)
		{
			m_skill_list[out_skill_index].last_perform_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			m_dirty_mark[out_skill_index] = true;
			m_is_change = true;
			
			this->SendSkillListInfo();

			return true;
		}
	}
	
	return false;
}

//只允许增加一种魔戒技能
bool SkillManager::AddMojieSkill(UInt16 skill_id, int skill_level, int skill_type)
{
	if (skill_id < MOJIE_SKILL_START_ID || skill_id > MOJIE_SKILL_END_ID || 0 >= skill_level || skill_level >= MAX_SKILL_LEVEL)
	{
		return false;
	}

	unsigned int last_perform_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; ++ i)
	{
		if (m_skill_list[i].skill_id >= MOJIE_SKILL_START_ID && m_skill_list[i].skill_id <= MOJIE_SKILL_END_ID)
		{
			if (m_skill_list[i].skill_id == skill_id)
			{
				return true;
			}
			else
			{

				const MojieUplevelCfg *skill_level_cfg = LOGIC_CONFIG->GetMoJieConfig().GetMojieLevelCfg(skill_type, skill_level);
				if (NULL == skill_level_cfg)
				{
					return false;
				}

				if (0 != skill_level_cfg->use_global_cold_time)
				{
					last_perform_time = m_skill_list[i].last_perform_time; // 魔戒技能共用冷却时间，所以要记录下冷缺时间
				}
				
				this->RemoveSkill(m_skill_list[i].skill_id);
			}	
		}
	}

	int out_skill_index = 0;
	if (this->AddSkill(skill_id, skill_level, &out_skill_index) && out_skill_index > 0 && out_skill_index < SkillListParam::MAX_SKILL_NUM)
	{
		m_skill_list[out_skill_index].last_perform_time = last_perform_time;
		m_dirty_mark[out_skill_index] = true;
		m_is_change = true;

		this->SendSkillListInfo();

		return true;
	}

	return false;
}

//只允许增加一种仙女技能
bool SkillManager::AddXiannvSkill(UInt16 skill_id, int skill_level)
{
	if (skill_id < XIANNV_SKILL_START_ID || skill_id > XIANNV_SKILL_END_ID || skill_level <= 0 || skill_level > MAX_SKILL_LEVEL)
	{
		return false;
	}

	unsigned int last_perform_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; ++ i)
	{
		if (m_skill_list[i].skill_id >= XIANNV_SKILL_START_ID && m_skill_list[i].skill_id <= XIANNV_SKILL_END_ID)
		{
			if (m_skill_list[i].skill_id == skill_id)
			{
				return true;
			}
			else
			{
				last_perform_time = m_skill_list[i].last_perform_time; // 仙女技能共用冷却时间，所以要记录下冷缺时间
			
				this->RemoveSkill(m_skill_list[i].skill_id);
			}	
		}
	}

	int out_skill_index = 0;
	if (this->AddSkill(skill_id, skill_level, &out_skill_index) && out_skill_index > 0 && out_skill_index < SkillListParam::MAX_SKILL_NUM)
	{
		m_skill_list[out_skill_index].last_perform_time = last_perform_time;
		m_dirty_mark[out_skill_index] = true;
		m_is_change = true;

		this->SendSkillListInfo();

		return true;
	}

	return false;
}

bool SkillManager::AddPetSkill(UInt16 skill_id, int skill_level)
{
	const SkillBase *skill_base = SKILLPOOL->GetSkill(skill_id);
	if (NULL == skill_base) return false;

	int skill_index = 0;
	if (this->GetSkillByID(skill_id, &skill_index))
	{
		if (skill_index < 0 || skill_index >= SkillListParam::MAX_SKILL_NUM) return false;

		m_skill_list[skill_index].level = skill_level;
		m_skill_list[skill_index].last_perform_time = (unsigned int)EngineAdapter::Instance().Time();
		m_dirty_mark[skill_index] = true;
		m_is_change = true;
	}
	else 
	{
		int free_slot = -1;
		for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; i++)
		{
			if (m_skill_list[i].Invalid()) 
			{
				free_slot = i;
				break;
			}
		}
		if (free_slot < 0 || free_slot >= SkillListParam::MAX_SKILL_NUM) return false;

		m_skill_list[free_slot].skill_id = skill_id;
		m_skill_list[free_slot].level = skill_level;
		m_skill_list[free_slot].last_perform_time = (unsigned int)EngineAdapter::Instance().Time();

		m_dirty_mark[free_slot] = true;
		m_is_change = true;

		skill_index = free_slot;
	}

	this->SendSkillListInfo();

	return true;
}


bool SkillManager::AddMagicCardSkill(UInt16 skill_id)
{
	return false;
	//int skill_level = 1; // 技能只有1级

	//const SkillBase *skill_base = SKILLPOOL->GetSkill(skill_id);
	//if (NULL == skill_base) return false;

	//int skill_index = 0;
	//if (this->GetSkillByID(skill_id, &skill_index))
	//{
	//	if (skill_index < 0 || skill_index >= SkillListParam::MAX_SKILL_NUM) return false;

	//	m_skill_list[skill_index].level = skill_level;
	//	m_skill_list[skill_index].last_perform_time = (unsigned int)EngineAdapter::Instance().Time();
	//	m_dirty_mark[skill_index] = true;
	//	m_is_change = true;
	//}
	//else 
	//{
	//	int free_slot = -1;
	//	for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; i++)
	//	{
	//		if (m_skill_list[i].Invalid()) 
	//		{
	//			free_slot = i;
	//			break;
	//		}
	//	}
	//	if (free_slot < 0 || free_slot >= SkillListParam::MAX_SKILL_NUM) return false;

	//	m_skill_list[free_slot].skill_id = skill_id;
	//	m_skill_list[free_slot].level = skill_level;
	//	m_skill_list[free_slot].last_perform_time = (unsigned int)EngineAdapter::Instance().Time();

	//	m_dirty_mark[free_slot] = true;
	//	m_is_change = true;

	//	skill_index = free_slot;
	//}

	//this->SendSkillListInfo();

	//return true;
}

bool SkillManager::AddEquipmentSkill(UInt16 skill_id, int skill_level)
{
	const SkillBase *skill_base = SKILLPOOL->GetSkill(skill_id);
	if (NULL == skill_base) return false;

	int skill_index = 0;
	if (this->GetSkillByID(skill_id, &skill_index))
	{
		if (skill_index < 0 || skill_index >= SkillListParam::MAX_SKILL_NUM) return false;

		m_skill_list[skill_index].level = skill_level;
		m_skill_list[skill_index].last_perform_time = (unsigned int)EngineAdapter::Instance().Time();
		m_dirty_mark[skill_index] = true;
		m_is_change = true;
	}
	else 
	{
		int free_slot = -1;
		for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; i++)
		{
			if (m_skill_list[i].Invalid()) 
			{
				free_slot = i;
				break;
			}
		}
		if (free_slot < 0 || free_slot >= SkillListParam::MAX_SKILL_NUM) return false;

		m_skill_list[free_slot].skill_id = skill_id;
		m_skill_list[free_slot].level = skill_level;
		m_skill_list[free_slot].last_perform_time = (unsigned int)EngineAdapter::Instance().Time();

		m_dirty_mark[free_slot] = true;
		m_is_change = true;

		skill_index = free_slot;
	}

	this->SendSkillListInfo();

	return true;
}

void SkillManager::ReduceSkillCD(UInt16 skill_id, int reduce_percent)
{
	int skill_index = 0; 
	if (this->GetSkillByID(skill_id, &skill_index)) 
	{
		const SkillBase *skill = SKILLPOOL->GetSkill(skill_id);
		if (NULL == skill) return;

		unsigned int cd_reduce = static_cast<unsigned int>(skill->GetCoolDown(m_skill_list[skill_index].level) * reduce_percent / 100.0);
		if (cd_reduce < m_skill_list[skill_index].last_perform_time) 
		{
			m_skill_list[skill_index].last_perform_time -= cd_reduce;

			m_dirty_mark[skill_index] = true;
			m_is_change = true;

			this->SendSkillInfo(skill_index);
		}
	}
}

const SkillManager::SkillItem * SkillManager::GetSkillItem(int skill_index)
{
	if (skill_index < 0 || skill_index >= SkillListParam::MAX_SKILL_NUM) return NULL;
	return &m_skill_list[skill_index];
}

bool SkillManager::SetNextSkill(int next_skill_index, const Posi &pos, ObjID target)
{
	if (next_skill_index < 0 || next_skill_index >= SkillListParam::MAX_SKILL_NUM || m_skill_list[next_skill_index].Invalid()) 
	{
		return false;
	}

	m_next_skill_index = next_skill_index;
	m_next_skill_pos = pos;
	m_next_skill_target = target;

	return true;
}

bool SkillManager::GetSkillByID(UInt16 skillid, SkillItem *skill_item)
{
	if (0 == skillid) return false;

	for (int index = 0; index < SkillListParam::MAX_SKILL_NUM; ++index)
	{
		if (skillid == m_skill_list[index].skill_id)
		{
			if (NULL != skill_item) *skill_item = m_skill_list[index];

			return true;
		}
	}

	return false;
}

bool SkillManager::GetSkillByID(UInt16 skillid, int *skill_index)
{
	if (0 == skillid) return false;

	for (int index = 0; index < SkillListParam::MAX_SKILL_NUM; ++index)
	{
		if (skillid == m_skill_list[index].skill_id)
		{
			if (NULL != skill_index) *skill_index = index;

			return true;
		}
	}

	return false;
}

int SkillManager::GetSkillCostMp(int skill_index)
{
	const SkillItem *skill_item = this->GetSkillItem(skill_index);
	if (NULL == skill_item)
	{
		return 99999999;
	}

	const SkillBase *skill_base = SKILLPOOL->GetSkill(skill_item->skill_id);
	if (NULL == skill_base)
	{
		return 99999999;
	}

	return skill_base->GetCostMp();
}

int SkillManager::GetSkillTotalLevel()
{
	int total_level = 0;
	for (int index = 0; index < SkillListParam::MAX_SKILL_NUM; ++index)
	{
		if (!m_skill_list[index].Invalid())
		{
			total_level += m_skill_list[index].level;
		}
	}

	return total_level;
}

Character * SkillManager::GetNextSkillTarget()
{
	if (INVALID_OBJ_ID == m_next_skill_target) return NULL;

	if (m_next_skill_index < 0 || m_next_skill_index >= SkillListParam::MAX_SKILL_NUM || m_skill_list[m_next_skill_index].Invalid())
	{
		return NULL;
	}

	const SkillBase *skill = SKILLPOOL->GetSkill(m_skill_list[m_next_skill_index].skill_id);
	if (NULL == skill) return NULL;

	Obj *obj = m_character->GetScene()->GetObj(m_next_skill_target);
	if (NULL == obj || !Obj::IsCharactor(obj->GetObjType())) return NULL;

	Character *target = (Character*)obj;
	if (skill->CheckTarget(m_character, target))
	{
		return target;
	}

	return NULL;
}

void SkillManager::GetSkillList(SkillListParam *p)
{
	p->count = 0;

	for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; ++i)
	{
		if (m_skill_list[i].Invalid()) continue;
	
		p->skill_list[p->count].change_state = structcommon::CS_NONE;
		p->skill_list[p->count].index = i;

		p->skill_list[p->count].skill_id = m_skill_list[i].skill_id;
		p->skill_list[p->count].level = m_skill_list[i].level;
		p->skill_list[p->count].last_perform_time = m_skill_list[i].last_perform_time;
		p->skill_list[p->count].exp = m_skill_list[i].exp;

		++ p->count;
	}
}
int SkillManager::GetRoleActiveSkillList(SkillListParam *p)
{
	p->count = 0;
	int low_lev = MAX_SKILL_LEVEL;
	for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; ++i)
	{
		if (m_skill_list[i].Invalid()) continue;
		UInt16 skill_id = m_skill_list[i].skill_id;
		UInt16 prof = skill_id / 100;
		UInt16 skill_num = skill_id % 100 / 10;
		if ((skill_id % 10 != 1 || prof < 1 || prof > 4 || skill_num < 1 || skill_num > 4) && 
			skill_id != 101 && skill_id != 201 && skill_id != 301 && skill_id != 401)  
		{
			continue;
		}	
		
		p->skill_list[p->count].index = i;
		p->skill_list[p->count].skill_id = m_skill_list[i].skill_id;
		p->skill_list[p->count].level = m_skill_list[i].level;
		if (m_skill_list[i].level < low_lev) {
			low_lev = m_skill_list[i].level;
		}
		p->skill_list[p->count].exp = 0;
		++p->count;
	}
	return low_lev;
}

void SkillManager::GetChangeSkillList(SkillListParam *p)
{
	if (!m_is_change)
	{
		p->count = 0;
		return;
	}

	int count = 0;
	for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; ++i)
	{
		if (m_dirty_mark[i])
		{
			if (m_skill_list[i].Invalid())
			{
				if (!m_old_state[i]) continue;

				p->skill_list[count].change_state = structcommon::CS_DELETE;
				p->skill_list[count].index = i;
			}
			else
			{
				p->skill_list[count].change_state = m_old_state[i] ? structcommon::CS_UPDATE : structcommon::CS_INSERT;
				p->skill_list[count].index = i;

				p->skill_list[count].skill_id = m_skill_list[i].skill_id;
				p->skill_list[count].level = m_skill_list[i].level;
				p->skill_list[count].last_perform_time = m_skill_list[i].last_perform_time;
				p->skill_list[count].exp = m_skill_list[i].exp;
				
			}

			++ count;
		}
	}

	p->count = count;
}

void SkillManager::ClearDirtyMark()
{
	if (!m_is_change) return;

	memset(m_dirty_mark, 0, sizeof(m_dirty_mark));

	for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; ++i)
	{
		m_old_state[i] = !m_skill_list[i].Invalid();
	}

	m_is_change = false;
}

void SkillManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (Obj::OBJ_TYPE_ROLE == m_character->GetObjType())
	{
		Role *role = (Role *)m_character;

		if (is_recalc)
		{
			m_attrs_add.Reset();
			m_capablity = 0;

			for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; ++i)
			{
				if (m_skill_list[i].Invalid()) continue;

				const SkillBase *skillBase = SKILLPOOL->GetSkill(m_skill_list[i].skill_id);
				if (NULL == skillBase) continue;

				if (SkillBase::SKILL_TYPE_PASSIVE == skillBase->GetSkillType())
				{
					skillBase->ReCalcAttr(m_character, m_skill_list[i].level, m_attrs_add);
				}

				m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_SKILL_CAP_PER] += skillBase->GetSkillCapPer(m_skill_list[i].level);
				m_capablity += skillBase->GetCapablity(m_skill_list[i].level);
			}
		}

		role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SKILL, m_attrs_add);
		role->GetCapability()->AddCap(CAPABILITY_TYPE_SKILL, m_capablity);

		base_add.Add(m_attrs_add.m_attrs, true);
	}	
}

void SkillManager::ClearAllCD()
{
	for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; ++i)
	{
		m_skill_list[i].last_perform_time = 0;
	}
}

int SkillManager::GetHurtIncPercent()
{
	return 0;
}

int SkillManager::GetHurtDecPercent()
{
	return 0;
}

void SkillManager::StartCD(int skill_index)
{
	if (skill_index < 0 || skill_index >= SkillListParam::MAX_SKILL_NUM) return;

	if (Obj::OBJ_TYPE_ROLE == m_character->GetObjType())
	{
		const SkillBase *skill_base = SKILLPOOL->GetSkill(m_skill_list[skill_index].skill_id);
		if (NULL == skill_base) return;
		
		if (skill_base->IsJumpSkill())
		{
			m_skill_list[skill_index].last_perform_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			m_dirty_mark[skill_index] = true;
			m_is_change = true;

			this->SendSkillInfo(skill_index);
		}
	}
}

void SkillManager::OnChangeProf(char old_prof)
{
	if (Obj::OBJ_TYPE_ROLE != m_character->GetObjType())
	{
		return;
	}

	Role *role = (Role *)m_character;

	int prof = role->GetProf() % PROF_TYPE_PROF_ZHUANZHI_BASE;
	if (prof <= PROF_TYPE_INVALID || prof >= PROF_TYPE_MAX)
	{
		return;
	}

	for (int i = 0; i < SkillListParam::MAX_SKILL_NUM; ++ i)
	{
		if (m_skill_list[i].skill_id > 110 && m_skill_list[i].skill_id < 150)
		{
			m_skill_list[i].skill_id = 100 + prof * 10 + m_skill_list[i].skill_id % 10;
			m_dirty_mark[i] = true;
			m_is_change = true;
		}
	}

	int old_default_skill_id_list[globalconfig::MAX_DEFAULT_SKILL_COUNT] = {0};
	int old_default_skill_id_count = GLOBALCONFIG->GetDefaultSkillID(old_prof, old_default_skill_id_list);
	int default_skill_id_list[globalconfig::MAX_DEFAULT_SKILL_COUNT] = {0};
	int default_skill_id_count = GLOBALCONFIG->GetDefaultSkillID(prof, default_skill_id_list);

	if (old_default_skill_id_count > 0 && old_default_skill_id_count <= globalconfig::MAX_DEFAULT_SKILL_COUNT)
	{
		for (int i = 0; i < default_skill_id_count; i++)
		{
			int skill_index = 0;
			if (old_default_skill_id_list[i] > 0 && this->GetSkillByID(old_default_skill_id_list[i], &skill_index))
			{
				if (default_skill_id_list[i] > 0)
				{
					m_skill_list[skill_index].skill_id = default_skill_id_list[i];
				}
				else
				{
					m_skill_list[skill_index].Reset();
				}
				m_dirty_mark[skill_index] = true;
				m_is_change = true;
			}
		}
	}

	this->InitDefaultSkill();

	this->SendSkillListInfo(false);
}

void SkillManager::OnZhuanzhi()
{
	if (Obj::OBJ_TYPE_ROLE != m_character->GetObjType())
	{
		return;
	}
}

// 受击
void SkillManager::OnAttack(Character *attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill)
{
	if (NULL == attacker) 
	{
		return;
	}
}

// 攻击别人
void SkillManager::OnHurtOther(Character *target, int skill_id, long long &injure, int fight_type)
{
	if (NULL == target) 
	{
		return;
	}
}

// 等级到自动学技能
void SkillManager::OnRoleLevelChange(int role_level)
{
	if (Obj::OBJ_TYPE_ROLE != m_character->GetObjType())
	{
		return;
	}

	Role *role = (Role *)m_character;
	int prof_index = role->GetProf() - 1;
	if (prof_index < 0 || prof_index >= MAX_PROF)
	{
		return;
	}

	for (int i = 0; i < SKILL_COUNT; i++)
	{
		int skill_id = SKILL_ID_LIST[prof_index][i];
		int skill_index = 0;
		if (this->GetSkillByID(skill_id, &skill_index))
		{
			continue;
		}

		const SkillBase *skill_base = SKILLPOOL->GetSkill(skill_id);
		if (NULL == skill_base)
		{
			continue;
		}

		if (!role->IsProfSuitable(skill_base->GetProfLimit())) 
		{
			continue;
		}

		if (role->GetLevel() < skill_base->GetLearnLevelLimit(1))
		{
			continue;
		}

		this->AddSkill(skill_id, 1);
	}
}

bool SkillManager::IsRoleSkill(int skill_id)
{
	for (int i = 0; i < MAX_PROF; ++i)
	{
		for (int j = 0; j < SKILL_COUNT; ++j)
		{
			if (skill_id == SKILL_ID_LIST[i][j])
			{
				return true;
			}
		}
	}

	return false;
}

bool SkillManager::IsNormalAttackSkill(int skill_id)
{
	for (int i = 0; i < MAX_PROF; ++i)
	{
		if (skill_id == SKILL_ID_LIST[i][0])
		{
			return true;
		}
	}

	return false;
}

bool SkillManager::IsKillAttackSkill(int skill_id)
{
	for (int i = 0; i < MAX_PROF; ++i)
	{
		if (skill_id == SKILL_ID_LIST[i][SKILL_COUNT - 1])
		{
			return true;
		}
	}

	return false;
}

long long SkillManager::CalcSkillHurtByHpPer(Character *target, int skill_id, bool is_main_target)
{
	if (nullptr == target || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return 0;
	}

	Role *target_role = static_cast<Role *>(target);

	SkillItem skill_item;
	this->GetSkillByID(static_cast<UInt16>(skill_id), &skill_item);

	const SkillBase *skill = SKILLPOOL->GetSkill(skill_id);
	if (nullptr != skill)
	{
		if (is_main_target || skill->IsEffectToOtherTarget(skill_item.level))
		{
			int per = skill->GetHurtByHpPer(skill_item.level);
			if (per > 0)
			{
				Attribute target_max_hp = target_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				return static_cast<Attribute>(per * SKILL_ATTR_PER * target_max_hp);
			}
		}
	}

	return 0;
}

long long SkillManager::CalcSkillBaodiHurtByHpPer(Character *target, int skill_id, bool is_main_target)
{
	if (nullptr == target || Obj::OBJ_TYPE_ROLE != target->GetObjType())
	{
		return 0;
	}

	Role *target_role = static_cast<Role *>(target);

	SkillItem skill_item;
	this->GetSkillByID(static_cast<UInt16>(skill_id), &skill_item);

	const SkillBase *skill = SKILLPOOL->GetSkill(skill_id);
	if (nullptr != skill)
	{
		if (is_main_target || skill->IsEffectToOtherTarget(skill_item.level))
		{
			int baodi_per = skill->GetBaodiHurtByHpPer(skill_item.level);
			if (baodi_per > 0)
			{
				Attribute target_max_hp = target_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
				return static_cast<Attribute>(baodi_per * SKILL_ATTR_PER * target_max_hp);
			}
		}
	}

	return 0;
}

void SkillManager::SendSkillInfo(int skill_index)
{
	if (skill_index < 0 || skill_index >= SkillListParam::MAX_SKILL_NUM || m_skill_list[skill_index].Invalid()) 
	{
		return;
	}

	if (Obj::OBJ_TYPE_ROLE != m_character->GetObjType()) return; 
	Role *role = (Role*)m_character;

	static Protocol::SCSkillInfoAck sra;
	sra.skill_info.index = static_cast<short>(skill_index);
	sra.skill_info.skill_id = m_skill_list[skill_index].skill_id;
	sra.skill_info.level = m_skill_list[skill_index].level;
	sra.skill_info.exp = m_skill_list[skill_index].exp;
	sra.skill_info.last_perform = m_skill_list[skill_index].last_perform_time;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&sra, sizeof(sra));
}

bool SkillManager::IsCommonSkill(int skill_id)
{
	return Obj::OBJ_TYPE_ROLE == m_character->GetObjType() && skill_id > COMMONSKILL_ID_INVALID && skill_id < COMMONSKILL_ID_MAX;
}

bool SkillManager::IsGreateSoldierSkill(int skill_id)
{
	return GREATE_SOLDIER_SKILL_1_ID == skill_id || GREATE_SOLDIER_SKILL_2_ID == skill_id || GREATE_SOLDIER_SKILL_3_ID == skill_id;
}

void SkillManager::PerformSkillHelper(int skill_index, Character *target, const Posi &skill_pos, int special_param)
{
	m_is_delay_perform_skill = false;

	SkillItem &skill_item = m_skill_list[skill_index];

	const SkillBase *skill = SKILLPOOL->GetSkill(skill_item.skill_id);
	if (NULL == skill) return;
	if (skill->IsAttackCharacter() && NULL == target) return;

	if (NULL == target) target = m_character;	// 群攻如果写错代码 误使用了target 也不应该挂 仅仅是逻辑出错

	skill->Perform(m_character, skill_item.level, target, skill_pos, special_param);

	m_character->IncAttackSeq();

	skill_item.last_perform_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	m_dirty_mark[skill_index] = true;
	m_is_change = true;

	if (Obj::OBJ_TYPE_ROLE == m_character->GetObjType())
	{
		Role *role = (Role *)m_character;
		if (role->IsAlive())
		{
			// 如果技能是会触发进入战斗状态的
			if (SkillBase::FIGHT_STATUS_POLICY_PERFORM == skill->GetFightStatusPolicy())
			{
				if (/*Obj::OBJ_TYPE_ROLE == target->GetObjType() && */target->GetId() != m_character->GetId())
				{
					role->OnStartFight();
				}
			}
		}

		CommonDataParam *common_data = role->GetCommonDataParam();
		if (NULL != common_data)
		{
			if (common_data->upgrade_skill_next_skill_index >= UpGradeSkillConfig::SKILL_TYPE_MOUNT && common_data->upgrade_skill_next_skill_index < UpGradeSkillConfig::SKILL_TYPE_MAX)
			{
				common_data->upgrade_skill_attack_count++;
				role->SendGradeSkillInfo(role);
			}
		}

		if (role->GetScene()->GetSpecialLogic()->SpecialIsCostMp(role))
		{
			long long cost_mp = -1 * skill->GetCostMp();
			m_character->ChangeMp(&cost_mp, true);
		}

		// 技能经验增加
// 		skill_item.exp++;
// 
// 		m_dirty_mark[skill_index] = true;
// 		m_is_change = true;
// 
// 		int uplevel_exp = skill->GetLearnZhenqi(skill_item.level);
// 		if (skill_item.exp >= uplevel_exp 
// 			&& role->GetLevel() >= skill->GetLearnLevelLimit(skill_item.level)
// 			&& uplevel_exp > 0 
// 			&& skill_item.level < skill->GetMaxLevel())
// 		{
// 			skill_item.exp -= uplevel_exp;
// 			skill_item.level++;
// 			
// 			this->SendSkillInfo(skill_index);
// 
// 			int total_skill_level = role->GetSkillManager()->GetSkillTotalLevel();
// 			EventHandler::Instance().OnUpSkillLevel(role, skill_item.skill_id, skill_item.level - 1, skill_item.level, total_skill_level);
// 		}
		SendSkillInfo(skill_index);
	}
}

void SkillManager::DelayPerformSkill(int skill_index, Character *target, const Posi &skill_pos, int special_param, int delay_time_ms)
{
	if (!m_is_addtimer)
	{
		m_is_addtimer = true;
		HighTimerMgr::Instance().Register(this, &SkillManager::OnDelayTimer);
	}

	m_is_delay_perform_skill = true;
	m_delay_perform_time_ms = delay_time_ms;
	m_delay_skill_index = skill_index;
	if (NULL != target)
	{
		m_delay_skill_target_id = target->GetId();
	}
	else
	{
		m_delay_skill_target_id = INVALID_OBJ_ID;
	}
	m_delay_skill_target = target;
	m_delay_skill_pos = skill_pos;
	m_delay_special_param = special_param;
}

void SkillManager::OnDelayTimer(unsigned long interval)
{
	m_delay_perform_time_ms -= interval;
	if (m_is_delay_perform_skill && m_delay_perform_time_ms <= 0)
	{
		m_is_delay_perform_skill = false;

		if (m_character->IsFool() || m_character->IsChenMo()) return;
		if (NULL == m_character->GetScene()) return;

		if (INVALID_OBJ_ID != m_delay_skill_target_id && m_character->GetScene()->GetObj(m_delay_skill_target_id) != m_delay_skill_target)
		{
			return;
		}
		if (m_delay_skill_target)
		{
			if (!m_delay_skill_target->IsAlive()) return;
			if (!m_character->IsEnemy(m_delay_skill_target)) return;
		}

		this->PerformSkillHelper(m_delay_skill_index, m_delay_skill_target, m_delay_skill_pos, m_delay_special_param);
	}

	if (m_delay_perform_time_ms <= -30000)
	{
		this->CancelTimer();
	}
}

void SkillManager::CancelTimer()
{
	if (m_is_addtimer)
	{
		m_is_addtimer = false;
		HighTimerMgr::Instance().UnRegister(this);
	}
}
