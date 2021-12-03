#include "rolegoal.hpp"
#include "servercommon/rolegoaldef.hpp"
#include "servercommon/errornum.h"
#include "servercommon/maildef.hpp"
#include "servercommon/string/gameworldstr.h"

#include "obj/character/role.h"
#include "effect/skilleffect/effectonce.hpp"
#include "protocal/msgother.h"

#include "config/logicconfigmanager.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"

#include "item/knapsack.h"
#include "item/itemextern.hpp"
#include "equipment/stonemananger.hpp"
#include "equipment/equipmentmanager.h"
#include "other/route/mailroute.hpp"
#include "other/convertshop/convertshop.hpp"
#include "other/runesystem/runesystem.hpp"
#include "other/xiannv/xiannvmanager.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/footprint/footprint.hpp"
#include "other/equipsuit/equipsuit.hpp"
#include "other/fb/rolepatafb.hpp"
#include "other/chengjiu/chengjiu.h"
#include "other/fb/roledailyfb.hpp"
#include "other/xunzhang/xunzhangmanager.hpp"
#include "other/zhibao/zhibaomanager.hpp"
#include "other/capability/capability.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "engineadapter.h"

RoleGoal::RoleGoal() : m_role(NULL), m_kill_monster_exp_add(0), m_hurt_monster_add(0),
	m_absord_blood_colddown_time(0),m_absord_blood_per(0),m_absord_blood_trigger_per(0), m_other_capability(0)
{

}

RoleGoal::~RoleGoal()
{

}

void RoleGoal::Init(Role *role, const RoleGoalParam &param)
{
	m_role = role;
	m_param = param;

	int open_day_index = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time());

	std::map<int, BattleFieldGoalCfg> &battlefield_goal_map = LOGIC_CONFIG->GetRoleGoalCfg().GetBattleFieldGoalCfgMap();
	std::map<int, BattleFieldGoalCfg>::iterator iter = battlefield_goal_map.begin();
	for( ;iter != battlefield_goal_map.end(); iter++)
	{
		BattleFieldGoalCfg &battlefield_goal = iter->second;

		if (role->GetLevel() >= battlefield_goal.level_limit)
		{
			if (open_day_index >= battlefield_goal.open_server_day)
			{
				if (0 < battlefield_goal.skill_type && battlefield_goal.skill_type < FIELD_GOAL_SKILL_TYPE_MAX)
				{
					if (0 == m_param.skill_level_list[battlefield_goal.skill_type])
					{
						m_param.skill_level_list[battlefield_goal.skill_type] = 1;
					}
				}

				if (0 != battlefield_goal.is_has_reward)
				{
					if (0 == (m_param.field_goal_fetch_flag & (1 << battlefield_goal.field_type)))
					{
						MailContentParam contentparam; contentparam.Reset();
						for (int item_count = 0; item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
						{
							if (battlefield_goal.item_reward[item_count].item_id > 0 && battlefield_goal.item_reward[item_count].num > 0)
							{
								contentparam.AddItem(battlefield_goal.item_reward[item_count].item_id, battlefield_goal.item_reward[item_count].num, battlefield_goal.item_reward[item_count].is_bind);
								LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ROLE_GOAL, role->GetUID(), role->GetName(), NULL, role->GetLevel(), battlefield_goal.item_reward[item_count].item_id, NULL, NULL, battlefield_goal.item_reward[item_count].num, battlefield_goal.item_reward[item_count].is_bind, 0, 0);
							}
							else
							{
								break;
							}
						}

						int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_role_goal_reward_content);
						if (length_1 > 0)
						{
							MailRoute::MailToUser(role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
							m_param.field_goal_fetch_flag |= (1 << battlefield_goal.field_type);
						}
					}
				}
			}
		}
	}

	this->CalcGoalExpAdd();
	this->CalcGoalHurtAdd();
	this->CalcGoalAbsord();

	this->CheckCond(PERSONAL_GOAL_COND_INVALID_TYPE, 0, 0);
}

void RoleGoal::GetInitParam(RoleGoalParam *param)
{
	*param = m_param;
}

void RoleGoal::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	bool is_need_calc = false;
	int open_day_index = GetDayIndex(m_role->GetServerStartTime(), EngineAdapter::Instance().Time());
	std::map<int, BattleFieldGoalCfg> &battlefield_goal_map = LOGIC_CONFIG->GetRoleGoalCfg().GetBattleFieldGoalCfgMap();
	std::map<int, BattleFieldGoalCfg>::iterator iter = battlefield_goal_map.begin();
	for( ;iter != battlefield_goal_map.end(); iter++)
	{
		BattleFieldGoalCfg &battlefield_goal = iter->second;

		if (m_role->GetLevel() >= battlefield_goal.level_limit)
		{
			if (open_day_index >= battlefield_goal.open_server_day)
			{
				if (0 < battlefield_goal.skill_type && battlefield_goal.skill_type < FIELD_GOAL_SKILL_TYPE_MAX)
				{
					if (0 == m_param.skill_level_list[battlefield_goal.skill_type])
					{
						m_param.skill_level_list[battlefield_goal.skill_type] = 1;
						is_need_calc = true;
					}
				}

				if (0 != battlefield_goal.is_has_reward)
				{
					if (0 == (m_param.field_goal_fetch_flag & (1 << battlefield_goal.field_type)))
					{
						MailContentParam contentparam; contentparam.Reset();
						for (int item_count = 0; item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
						{
							if (battlefield_goal.item_reward[item_count].item_id > 0 && battlefield_goal.item_reward[item_count].num > 0)
							{
								contentparam.AddItem(battlefield_goal.item_reward[item_count].item_id, battlefield_goal.item_reward[item_count].num, battlefield_goal.item_reward[item_count].is_bind);
								LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ROLE_GOAL, m_role->GetUID(), m_role->GetName(), NULL, m_role->GetLevel(), battlefield_goal.item_reward[item_count].item_id, NULL, NULL, battlefield_goal.item_reward[item_count].num, battlefield_goal.item_reward[item_count].is_bind, 0, 0);
							}
							else
							{
								break;
							}
						}

						int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_role_goal_reward_content);
						if (length_1 > 0)
						{
							MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
							m_param.field_goal_fetch_flag |= (1 << battlefield_goal.field_type);
						}
					}
				}
			}
		}
	}
	
	if (is_need_calc)
	{
		this->CalcGoalExpAdd();
		this->CalcGoalHurtAdd();
		this->CalcGoalAbsord();
	}
}

void RoleGoal::OnRoleLevelChange(int role_level)
{
	bool is_need_calc = false;
	int open_day_index = GetDayIndex(m_role->GetServerStartTime(), EngineAdapter::Instance().Time());
	std::map<int, BattleFieldGoalCfg> &battlefield_goal_map = LOGIC_CONFIG->GetRoleGoalCfg().GetBattleFieldGoalCfgMap();
	std::map<int, BattleFieldGoalCfg>::iterator iter = battlefield_goal_map.begin();
	for( ;iter != battlefield_goal_map.end(); iter++)
	{
		BattleFieldGoalCfg &battlefield_goal = iter->second;

		if (m_role->GetLevel() >= battlefield_goal.level_limit)
		{
			if (open_day_index >= battlefield_goal.open_server_day)
			{
				if (0 < battlefield_goal.skill_type && battlefield_goal.skill_type < FIELD_GOAL_SKILL_TYPE_MAX)
				{
					if (0 == m_param.skill_level_list[battlefield_goal.skill_type])
					{
						m_param.skill_level_list[battlefield_goal.skill_type] = 1;
						is_need_calc = true;
					}
				}

				if (0 != battlefield_goal.is_has_reward)
				{
					if (0 == (m_param.field_goal_fetch_flag & (1 << battlefield_goal.field_type)))
					{
						MailContentParam contentparam; contentparam.Reset();
						for (int item_count = 0; item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
						{
							if (battlefield_goal.item_reward[item_count].item_id > 0 && battlefield_goal.item_reward[item_count].num > 0)
							{
								contentparam.AddItem(battlefield_goal.item_reward[item_count].item_id, battlefield_goal.item_reward[item_count].num, battlefield_goal.item_reward[item_count].is_bind);
								LOG_QUICK12(gamelog::g_log_quick, LOG_TYPE_ROLE_GOAL, m_role->GetUID(), m_role->GetName(), NULL, m_role->GetLevel(), battlefield_goal.item_reward[item_count].item_id, NULL, NULL, battlefield_goal.item_reward[item_count].num, battlefield_goal.item_reward[item_count].is_bind, 0, 0);
							}
							else
							{
								break;
							}
						}

						int length_1 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_role_goal_reward_content);
						if (length_1 > 0)
						{
							MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
							m_param.field_goal_fetch_flag |= (1 << battlefield_goal.field_type);
						}
					}
				}
			}
		}
	}

	if (is_need_calc)
	{
		this->CalcGoalExpAdd();
		this->CalcGoalHurtAdd();
		this->CalcGoalAbsord();
	}
}

void RoleGoal::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_other_capability = 0;

		for (int i = FIELD_GOAL_INVALID_SKILL_TYPE + 1; i < FIELD_GOAL_MAX_SKILL_TYPE && i < FIELD_GOAL_SKILL_TYPE_MAX; i++)
		{
			const BattleFieldGoalSkillCfg::ConfigItem *cfg = LOGIC_CONFIG->GetRoleGoalCfg().GetBattleFieldGoalSkillCfg(i, m_param.skill_level_list[i]);
			if (NULL != cfg)
			{
				m_other_capability += cfg->capability;
			}
		}
	}

	m_role->GetCapability()->AddCap(CAPABILITY_TYPE_ROLE_GOAL, m_other_capability);
}

void RoleGoal::FetchPersonalGoalReward()
{
	if (m_role->GetBaseProf() <= PROF_TYPE_INVALID || m_role->GetBaseProf() >= PROF_TYPE_PROF_NO_LIMIT)
	{
		return;
	}

	if (m_param.old_chapter < 0 || m_param.old_chapter >= PERSONAL_GOAL_CHAPTER_MAX)
	{
		m_role->NoticeNum(errornum::EN_ROLE_GOAL_HAS_FETCH_REWARD);
		return;
	}

	if (m_param.old_chapter >= m_param.cur_chapter)
	{
		m_role->NoticeNum(errornum::EN_ROLE_GOAL_HAS_FETCH_REWARD);
		return;
	}

	const PersonalGoalChapterCfg *chapter_cfg = LOGIC_CONFIG->GetRoleGoalCfg().GetPersonalGoalChapterCfg(m_param.old_chapter + 1);
	if (NULL == chapter_cfg)
	{
		m_role->NoticeNum(errornum::EN_ROLE_GOAL_HAS_FETCH_REWARD);
		return;
	}

	switch(m_role->GetBaseProf())
	{
	case PROF_TYPE_PROF_1:
		{
			m_role->GetKnapsack()->PutListOrMail(chapter_cfg->prof_reward_1, PUT_REASON_ROLE_GOAL_REWARD);
		}
		break;

	case PROF_TYPE_PROF_2:
		{
			m_role->GetKnapsack()->PutListOrMail(chapter_cfg->prof_reward_2, PUT_REASON_ROLE_GOAL_REWARD);
		}
		break;

	case PROF_TYPE_PROF_3:
		{
			m_role->GetKnapsack()->PutListOrMail(chapter_cfg->prof_reward_3, PUT_REASON_ROLE_GOAL_REWARD);
		}
		break;

	case PROF_TYPE_PROF_4:
		{
			m_role->GetKnapsack()->PutListOrMail(chapter_cfg->prof_reward_4, PUT_REASON_ROLE_GOAL_REWARD);
		}
		break;

	default:
		return;
	}

	m_param.old_chapter++;

	this->CheckCond(PERSONAL_GOAL_COND_INVALID_TYPE, 0, 0);
	this->SendInfoToRole();
}

void RoleGoal::OnUplevelSkill(int skill_type)
{
	if (skill_type <= 0 || skill_type >= FIELD_GOAL_SKILL_TYPE_MAX)
	{
		return;
	}

	const BattleFieldGoalSkillCfg::ConfigItem *cfg = LOGIC_CONFIG->GetRoleGoalCfg().GetBattleFieldGoalSkillCfg(skill_type, m_param.skill_level_list[skill_type]);
	if (NULL == cfg)
	{
		return;
	}

	if (NULL == LOGIC_CONFIG->GetRoleGoalCfg().GetBattleFieldGoalSkillCfg(skill_type, m_param.skill_level_list[skill_type] + 1))
	{
		m_role->NoticeNum(errornum::EN_ROLE_GOAL_SKILL_LEVEL_MAX);
		return;
	}

	g_consume_list.Reset();
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[0].buyable = false;

	if (PROF_TYPE_PROF_1 == m_role->GetBaseProf())
	{
		stuff_list[0].item_id = cfg->uplevel_stuff_prof1.item_id;
		stuff_list[0].num = cfg->uplevel_stuff_prof1.num;
	}
	else if (PROF_TYPE_PROF_2 == m_role->GetBaseProf())
	{
		stuff_list[0].item_id = cfg->uplevel_stuff_prof2.item_id;
		stuff_list[0].num = cfg->uplevel_stuff_prof2.num;
	}
	else if (PROF_TYPE_PROF_3 == m_role->GetBaseProf())
	{
		stuff_list[0].item_id = cfg->uplevel_stuff_prof3.item_id;
		stuff_list[0].num = cfg->uplevel_stuff_prof3.num;
	}
	else if (PROF_TYPE_PROF_4 == m_role->GetBaseProf())
	{
		stuff_list[0].item_id = cfg->uplevel_stuff_prof4.item_id;
		stuff_list[0].num = cfg->uplevel_stuff_prof4.num;
	}

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &g_consume_list, true))
	{
		return;
	}

	// ÏûºÄ
	if (!ItemExtern::ConsumeItem(m_role, g_consume_list, "RoleGoal::OnUplevelSkill"))
	{
		return;
	}

	m_param.skill_level_list[skill_type]++;

	this->CalcGoalExpAdd();
	this->CalcGoalHurtAdd();
	this->CalcGoalAbsord();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ROLE_GOAL);

	this->SendInfoToRole();
}

void RoleGoal::RoleGoalAddRecordTimes(int cond_type, long long param1, int param2)
{
	if (PERSONAL_GOAL_COND_INVALID_TYPE >= cond_type || PERSONAL_GOAL_COND_MAX_TYPE <= cond_type || param1 <= 0)
	{
		return;
	}

	switch (cond_type)
	{
	case COND_TYPE_DAYLY_TASK:
		{
			m_param.role_goal_record.complete_total_num_1 += (int)param1;
		}
		break;

	case COND_TYPE_PHASE_FB:
		{
			m_param.role_goal_record.complete_total_num_3 += (int)param1;
		}
		break;

	case COND_TYPE_GUILD_TASK:
		{
			m_param.role_goal_record.complete_total_num_4 += (int)param1;
		}
		break;

	case COND_TYPE_CROSS_TEAM_FB:
		{
			m_param.role_goal_record.complete_total_num_5 += (int)param1;
		}
		break;

	case COND_TYPE_HUSONG_TASK:
		{
			m_param.role_goal_record.complete_total_num_6 += (int)param1;
		}
		break;

	case COND_TYPE_GUILD_DONATE:
		{
			m_param.role_goal_record.complete_total_num_10 += (int)param1;
		}
		break;

	case COND_TYPE_GUILD_STORAGE:
		{
			m_param.role_goal_record.complete_total_num_11 += (int)param1;
		}
		break;

	case COND_TYPE_GUILD_BOX_OPEN:
		{
			m_param.role_goal_record.complete_total_num_12 += (int)param1;
		}
		break;

	case COND_TYPE_KILL_BOSS:
		break;

	case COND_TYPE_RECYCL_EQUIP:
		break;

	case COND_TYPE_CONVERT_SHOP:
		{
			if (ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_MOJING == param2)
			{
				m_param.role_goal_record.complete_total_num_36_1 += (int)param1;
			}
			else if(ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_SHENGWANG == param2)
			{
				m_param.role_goal_record.complete_total_num_36_2 += (int)param1;
			}
			else if(ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_GONGXUN == param2)
			{
				m_param.role_goal_record.complete_total_num_36_3 += (int)param1;
			}
			else if(ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_WEIWANG == param2)
			{
				m_param.role_goal_record.complete_total_num_36_4 += (int)param1;
			}
			else if(ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT == param2)
			{
				m_param.role_goal_record.complete_total_num_36_5 += (int)param1;
			}
			else if(ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_JINGLING_CREDIT == param2)
			{
				m_param.role_goal_record.complete_total_num_36_6 += (int)param1;
			}
			else if(ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_HAPPY_TREE == param2)
			{
				m_param.role_goal_record.complete_total_num_36_7 += (int)param1;
			}
			else if(ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_CROSS_HONOR == param2)
			{
				m_param.role_goal_record.complete_total_num_36_8 += (int)param1;
			}
		}
		break;

	case COND_TYPE_EXP_FB:
		break;

	case COND_TYPE_PATA_FB:
		break;

	case COND_TYPE_ROLE_LEVEL:
		break;

	case COND_TYPE_TUO_DAN:
		{
			m_param.role_goal_record.complete_total_num_13 += (int)param1;
		}
		break;

	case COND_TYPE_ZHI_BAO_LEVEL:
		break;

	case COND_TYPE_XUN_ZHANG_LEVEL:
		break;

	case COND_TYPE_TOTAL_STRENGTHEN:
		break;

	case COND_TYPE_STRENGTHEN:
		break;

	case COND_TYPE_TOTAL_UPSTAR:
		break;

	case COND_TYPE_UPSTAR:
		break;

	case COND_TYPE_TOTAL_STONE:
		break;

	case COND_TYPE_STONE:
		break;

	case COND_TYPE_EQUIP_SUIT:
		break;

	case COND_TYPE_UPGRADE_MOUNT:
		break;

	case COND_TYPE_UPGRADE_WING:
		break;

	case COND_TYPE_UPGRADE_HALO:
		break;

	case COND_TYPE_UPGRADE_FIGHT_MOUNT:
		break;

	case COND_TYPE_UPGRADE_SHENGONG:
		break;

	case COND_TYPE_UPGRADE_SHENYI:
		break;

	case COND_TYPE_ACTIVE_XIANNV:
		break;

	case COND_TYPE_CHENG_JIU_LEVEL:
		break;

	case COND_TYPE_UPGRADE_FOOTPRINT:
		break;

	case COND_TYPE_GUILD_BOX_DIG:
		{
			m_param.role_goal_record.complete_total_num_32 += (int)param1;
		}
		break;

	case COND_TYPE_KILL_BOSS_TYPE:
		{
			if (CON_WORLD_BOSS == param2)
			{
				m_param.role_goal_record.complete_total_num_33_1 += (int)param1;
			}
			else if(COND_FAMILY_BOSS == param2)
			{
				m_param.role_goal_record.complete_total_num_33_2 += (int)param1;
			}
			else if(COND_MIKU_BOSS == param2)
			{
				m_param.role_goal_record.complete_total_num_33_3 += (int)param1;
			}
			else if(COND_DABAO_BOSS == param2)
			{
				m_param.role_goal_record.complete_total_num_33_4 += (int)param1;
			}
			else if(COND_ACTIVE_BOSS == param2)
			{
				m_param.role_goal_record.complete_total_num_33_5 += (int)param1;
			}
		}
		break;

	case COND_TYPE_CAUSE_HARM_ON_BOSS:
		{
			if (CON_WORLD_BOSS == param2)
			{
				if (INT_MAX - m_param.role_goal_record.complete_total_num_39_1 > param1)
				{
					m_param.role_goal_record.complete_total_num_39_1 += (int)param1;
				}
				else
				{
					m_param.role_goal_record.complete_total_num_39_1 = INT_MAX;
				}
			}
			else if(COND_FAMILY_BOSS == param2)
			{
				if (INT_MAX - m_param.role_goal_record.complete_total_num_39_2 > param1)
				{
					m_param.role_goal_record.complete_total_num_39_2 += (int)param1;
				}
				else
				{
					m_param.role_goal_record.complete_total_num_39_2 = INT_MAX;
				}	
			}
			else if(COND_MIKU_BOSS == param2)
			{
				if (INT_MAX - m_param.role_goal_record.complete_total_num_39_3 > param1)
				{
					m_param.role_goal_record.complete_total_num_39_3 += (int)param1;
				}
				else
				{
					m_param.role_goal_record.complete_total_num_39_3 = INT_MAX;
				}
			}
			else if(COND_DABAO_BOSS == param2)
			{
				if (INT_MAX - m_param.role_goal_record.complete_total_num_39_4 > param1)
				{
					m_param.role_goal_record.complete_total_num_39_4 += (int)param1;
				}
				else
				{
					m_param.role_goal_record.complete_total_num_39_4 = INT_MAX;
				}
			}
			else if(COND_ACTIVE_BOSS == param2)
			{
				if (INT_MAX - m_param.role_goal_record.complete_total_num_39_5 > param1)
				{
					m_param.role_goal_record.complete_total_num_39_5 += (int)param1;
				}
				else
				{
					m_param.role_goal_record.complete_total_num_39_5 = INT_MAX;
				}
			}
		}
		break;

	case COND_TYPE_FB_GUIDE_YUBIAO_TYPE:
		{
			m_param.role_goal_record.complete_total_num_34 += (int)param1;
		}
		break;

	case COND_TYPE_FB_GUIDE_SHUIJING_TYPE:
		{
			m_param.role_goal_record.complete_total_num_35 += (int)param1;
		}
		break;

	case COND_TYPE_FB_GUIDE_BOSS_TYPE:
		{
			m_param.role_goal_record.complete_total_num_38 += (int)param1;
		}
		break;

	case COND_TYPE_RUNE_TOWER:
		break;
	}
}

void RoleGoal::CheckCond(int cond_type, long long param1, int param2)
{
	if (m_param.cur_chapter < 0 || m_param.cur_chapter >= PERSONAL_GOAL_CHAPTER_MAX)
	{
		return;
	}

	const PersonalGoalChapterCfg *chapter_cfg = LOGIC_CONFIG->GetRoleGoalCfg().GetPersonalGoalChapterCfg(m_param.cur_chapter + 1);
	if (NULL == chapter_cfg)
	{
		return;
	}

	int data_list[PERSONAL_GOAL_COND_MAX] = {0};
	memcpy(data_list, m_param.data_list, sizeof(data_list));

	int complete_count = 0;
	for (int i = 0; i < PERSONAL_GOAL_COND_MAX; i++)
	{
		const PersonalGoalChapterCfg::CondCfg &cfg = chapter_cfg->cond_list[i];

		if (m_param.data_list[i] >= cfg.cond_param1)
		{
			m_param.data_list[i] = cfg.cond_param1;
			complete_count++;
			continue;
		}

		if (cfg.cond_type != cond_type && PERSONAL_GOAL_COND_INVALID_TYPE != cond_type)
		{
			continue;
		}

		switch (cfg.cond_type)
		{
		case COND_TYPE_DAYLY_TASK:
			{
				m_param.data_list[i] = m_param.role_goal_record.complete_total_num_1;
			}
			break;

		case COND_TYPE_PHASE_FB:
			{
				m_param.data_list[i] = m_param.role_goal_record.complete_total_num_3;
			}
			break;

		case COND_TYPE_GUILD_TASK:
			{
				m_param.data_list[i] = m_param.role_goal_record.complete_total_num_4;
			}
			break;

		case COND_TYPE_CROSS_TEAM_FB:
			{
				m_param.data_list[i] = m_param.role_goal_record.complete_total_num_5;
			}
			break;

		case COND_TYPE_HUSONG_TASK:
			{
				m_param.data_list[i] = m_param.role_goal_record.complete_total_num_6;
			}
			break;

		case COND_TYPE_GUILD_DONATE:
			{
				m_param.data_list[i] = m_param.role_goal_record.complete_total_num_10;
			}
			break;

		case COND_TYPE_GUILD_STORAGE:
			{
				m_param.data_list[i] = m_param.role_goal_record.complete_total_num_11;
			}
			break;

		case COND_TYPE_GUILD_BOX_OPEN:
			{
				m_param.data_list[i] = m_param.role_goal_record.complete_total_num_12;
			}
			break;

		case COND_TYPE_KILL_BOSS:
			{
				if (cfg.cond_param2 == param2)
				{
					m_param.data_list[i] += (int)param1;
				}
			}
			break;

		case COND_TYPE_RECYCL_EQUIP:
			if (cfg.cond_type == cond_type && cfg.cond_param2 == param2)
			{
				m_param.data_list[i] += (int)param1;
			}
			break;

		case COND_TYPE_CONVERT_SHOP:
			{
				if (cfg.cond_param2 == param2)
				{
					if (ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_MOJING == cfg.cond_param2)
					{
						m_param.data_list[i] = m_param.role_goal_record.complete_total_num_36_1;
					}
					else if(ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_SHENGWANG == cfg.cond_param2)
					{
						m_param.data_list[i] = m_param.role_goal_record.complete_total_num_36_2;
					}
					else if(ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_GONGXUN == cfg.cond_param2)
					{
						m_param.data_list[i] = m_param.role_goal_record.complete_total_num_36_3;
					}
					else if(ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_WEIWANG == cfg.cond_param2)
					{
						m_param.data_list[i] = m_param.role_goal_record.complete_total_num_36_4;
					}
					else if(ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_TREASURE_HUNT_CREDIT == cfg.cond_param2)
					{
						m_param.data_list[i] = m_param.role_goal_record.complete_total_num_36_5;
					}
					else if(ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_JINGLING_CREDIT == cfg.cond_param2)
					{
						m_param.data_list[i] = m_param.role_goal_record.complete_total_num_36_6;
					}
					else if(ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_HAPPY_TREE == cfg.cond_param2)
					{
						m_param.data_list[i] = m_param.role_goal_record.complete_total_num_36_7;
					}
					else if(ConvertShop::SCORE_TO_ITEM_PRICE_TYPE_CROSS_HONOR == cfg.cond_param2)
					{
						m_param.data_list[i] = m_param.role_goal_record.complete_total_num_36_8;
					}
				}
			}
			break;

		case COND_TYPE_EXP_FB:
			{
				m_param.data_list[i] = m_role->GetRoleDailyFb()->GetExpFbHistoryEnterTimes();
			}
			break;

		case COND_TYPE_PATA_FB:
			{
				m_param.data_list[i] = m_role->GetRolePataFB()->GetPassLevel();
			}
			break;

		case COND_TYPE_ROLE_LEVEL:
			{
				m_param.data_list[i] = m_role->GetLevel();
			}
			break;

		case COND_TYPE_TUO_DAN:
			if (m_role->IsMarry())
			{
				m_param.data_list[i] = cfg.cond_param1;
			}
			else if (cfg.cond_type == cond_type)
			{
				m_param.data_list[i] = m_param.role_goal_record.complete_total_num_13;
			}
			break;

		case COND_TYPE_ZHI_BAO_LEVEL:
			m_param.data_list[i] = m_role->GetZhiBaoManager()->GetZhiBaoLevel();
			break;

		case COND_TYPE_XUN_ZHANG_LEVEL:
			m_param.data_list[i] = m_role->GetXunZhangManager()->GetXunZhangTotalLevel();
			break;

		case COND_TYPE_TOTAL_STRENGTHEN:
			m_param.data_list[i] = m_role->GetEquipmentManager()->GetTotalStrengthen();
			break;

		case COND_TYPE_STRENGTHEN:
			m_param.data_list[i] = m_role->GetEquipmentManager()->GetMaxStrengthen();
			break;

		case COND_TYPE_TOTAL_UPSTAR:
			m_param.data_list[i] = m_role->GetEquipmentManager()->GetTotalStarLevel();
			break;

		case COND_TYPE_UPSTAR:
			m_param.data_list[i] = m_role->GetEquipmentManager()->GetMaxStarLevel();
			break;

		case COND_TYPE_TOTAL_STONE:
			m_param.data_list[i] = m_role->GetStoneManager()->GetTotalStoneLevel();
			break;

		case COND_TYPE_STONE:
			m_param.data_list[i] = m_role->GetStoneManager()->GetMaxStoneLevel();
			break;

		case COND_TYPE_EQUIP_SUIT:
			m_param.data_list[i] = m_role->GetEquipSuit()->GetSuitPartCount(cfg.cond_param2);
			break;

		case COND_TYPE_UPGRADE_MOUNT:
			m_param.data_list[i] = m_role->GetMountManager()->GetMountParam()->grade;
			break;

		case COND_TYPE_UPGRADE_WING:
			m_param.data_list[i] = m_role->GetWing()->GetWingParam()->grade;
			break;

		case COND_TYPE_UPGRADE_HALO:
			m_param.data_list[i] = m_role->GetHalo()->GetHaloParam()->grade;
			break;

		case COND_TYPE_UPGRADE_FOOTPRINT:
			m_param.data_list[i] = m_role->GetFootPrint()->GetFootprintParam()->grade;
			break;

		case COND_TYPE_UPGRADE_FIGHT_MOUNT:
			m_param.data_list[i] = m_role->GetFightMountManager()->GetFightMountParam()->grade;
			break;

		case COND_TYPE_UPGRADE_SHENGONG:
			m_param.data_list[i] = m_role->GetShengong()->GetShengongParam()->grade;
			break;

		case COND_TYPE_UPGRADE_SHENYI:
			m_param.data_list[i] = m_role->GetShenyi()->GetShenyiParam()->grade;
			break;

		case COND_TYPE_ACTIVE_XIANNV:
			m_param.data_list[i] = m_role->GetXiannvManager()->GetActiveXiannvCount();
			break;

		case COND_TYPE_CHENG_JIU_LEVEL:
			m_param.data_list[i] = m_role->GetChengJiuMgr()->GetChengJiuTitleLevel();
			break;

		case COND_TYPE_GUILD_BOX_DIG:
			{
				m_param.data_list[i] = m_param.role_goal_record.complete_total_num_32;
			}
			break;

		case COND_TYPE_KILL_BOSS_TYPE:
			{
				if (CON_WORLD_BOSS == cfg.cond_param2)
				{
					m_param.data_list[i] = m_param.role_goal_record.complete_total_num_33_1;
				}
				else if(COND_FAMILY_BOSS == cfg.cond_param2)
				{
					m_param.data_list[i] = m_param.role_goal_record.complete_total_num_33_2;
				}
				else if(COND_MIKU_BOSS == cfg.cond_param2)
				{
					m_param.data_list[i] = m_param.role_goal_record.complete_total_num_33_3;
				}
				else if(COND_DABAO_BOSS == cfg.cond_param2)
				{
					m_param.data_list[i] = m_param.role_goal_record.complete_total_num_33_4;
				}
				else if(COND_ACTIVE_BOSS == cfg.cond_param2)
				{
					m_param.data_list[i] = m_param.role_goal_record.complete_total_num_33_5;
				}
			}
			break;

		case COND_TYPE_CAUSE_HARM_ON_BOSS:
			{
				if (CON_WORLD_BOSS == cfg.cond_param2)
				{
					m_param.data_list[i] = m_param.role_goal_record.complete_total_num_39_1;
				}
				else if(COND_FAMILY_BOSS == cfg.cond_param2)
				{
					m_param.data_list[i] = m_param.role_goal_record.complete_total_num_39_2;
				}
				else if(COND_MIKU_BOSS == cfg.cond_param2)
				{
					m_param.data_list[i] = m_param.role_goal_record.complete_total_num_39_3;
				}
				else if(COND_DABAO_BOSS == cfg.cond_param2)
				{
					m_param.data_list[i] = m_param.role_goal_record.complete_total_num_39_4;
				}
				else if(COND_ACTIVE_BOSS == cfg.cond_param2)
				{
					m_param.data_list[i] = m_param.role_goal_record.complete_total_num_39_5;
				}
			}
			break;

		case COND_TYPE_FB_GUIDE_YUBIAO_TYPE:
			{
				m_param.data_list[i] = m_param.role_goal_record.complete_total_num_34;
			}
			break;

		case COND_TYPE_FB_GUIDE_SHUIJING_TYPE:
			{
				m_param.data_list[i] = m_param.role_goal_record.complete_total_num_35;
			}
			break;

		case COND_TYPE_FB_GUIDE_BOSS_TYPE:
			{
				m_param.data_list[i] = m_param.role_goal_record.complete_total_num_38;
			}
			break;

		case COND_TYPE_RUNE_TOWER:
			m_param.data_list[i] = m_role->GetRuneSystem()->GetRuneTowerPassLayer();
			break;

		case COND_TYPE_CLIENT_NOTICE:
			{
				m_param.data_list[i] = (int)param1;
			}
			break;
		}

		if (m_param.data_list[i] >= cfg.cond_param1)
		{
			m_param.data_list[i] = cfg.cond_param1;
			complete_count++;
		}
	}

	if (complete_count >= PERSONAL_GOAL_COND_MAX)
	{
		m_param.cur_chapter++;
		memset(m_param.data_list, 0, sizeof(m_param.data_list));

		this->CheckCond(PERSONAL_GOAL_COND_INVALID_TYPE, 0, 0);

		this->SendInfoToRole();
	}
	else
	{
		for (int i = 0; i < PERSONAL_GOAL_COND_MAX; i++)
		{
			if (data_list[i] != m_param.data_list[i])
			{
				this->SendInfoToRole();
				break;
			}
		}
	}
}

void RoleGoal::CheckBattleFieldCond(int field_type, int param1)
{
	if (field_type <= ACTIVITY_TYPE_INVALID || field_type >= ACTIVITY_TYPE_MAX || param1 <= 0)
	{
		return;
	}

	const BattleFieldGoalCfg *cfg = LOGIC_CONFIG->GetRoleGoalCfg().GetBattleFieldGoalCfg(field_type);
	if (NULL == cfg)
	{
		return;
	}

	if (0 != (m_param.field_goal_can_fetch_flag & (1 << field_type)))
	{
		return;
	}

	if (0 != (m_param.field_goal_fetch_flag & (1 << field_type)))
	{
		return;
	}

	if (param1 >= cfg->complete_score)
	{
		m_param.field_goal_can_fetch_flag |= (1 << field_type);

		if (0 < cfg->skill_type && cfg->skill_type < FIELD_GOAL_SKILL_TYPE_MAX)
		{
			if (0 == m_param.skill_level_list[cfg->skill_type])
			{
				m_param.skill_level_list[cfg->skill_type] = 1;
			}
		}

		this->SendInfoToRole();

		this->CalcGoalExpAdd();
		this->CalcGoalHurtAdd();
		this->CalcGoalAbsord();
	}
}

void RoleGoal::FetchBattleFieldGoalReward(int field_type)
{
	if (field_type <= ACTIVITY_TYPE_INVALID || field_type >= ACTIVITY_TYPE_MAX)
	{
		return;
	}

	if (0 != (m_param.field_goal_fetch_flag & (1 << field_type)))
	{
		m_role->NoticeNum(errornum::EN_RAND_ACTIVITY_HAS_FETCH_REWARD);
		return;
	}
	
	if (0 == (m_param.field_goal_can_fetch_flag & (1 << field_type)))
	{
		m_role->NoticeNum(errornum::EN_ROLE_GOAL_HAS_FETCH_REWARD);
		return;
	}

	const BattleFieldGoalCfg *cfg = LOGIC_CONFIG->GetRoleGoalCfg().GetBattleFieldGoalCfg(field_type);
	if (NULL == cfg)
	{
		m_role->NoticeNum(errornum::EN_ROLE_GOAL_HAS_FETCH_REWARD);
		return;
	}

	m_param.field_goal_fetch_flag |= (1 << field_type);
	this->SendInfoToRole();

	if (1 == cfg->is_has_reward)
	{
		m_role->GetKnapsack()->PutListOrMail(cfg->item_reward, PUT_REASON_ROLE_GOAL_REWARD);
	}
}

void RoleGoal::OnAbsordBlood(long long injure)
{
	if (injure <= 0) return;

	if (m_absord_blood_colddown_time <= 0 || m_absord_blood_per <= 0 || m_absord_blood_trigger_per <= 0)
	{
		return;
	}

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_sec < m_param.absorb_blood_next_time) return;

	int rand_value = RandomNum(10000);
	if (rand_value <  m_absord_blood_trigger_per)
	{
		m_param.absorb_blood_next_time = now_sec + m_absord_blood_colddown_time;

		long long add_hp = (long long)(injure * (m_absord_blood_per * ROLE_ATTR_PER));
		if (add_hp > 0)
		{
			m_role->ChangeHp(&add_hp, true);
		}
	}
}

bool RoleGoal::IsFinishGoal(int cond_type)
{
	const PersonalGoalChapterCfg *chapter_cfg = LOGIC_CONFIG->GetRoleGoalCfg().GetPersonalGoalChapterCfg(m_param.cur_chapter + 1);
	if (NULL == chapter_cfg)
	{
		return false;
	}
	
	int chapter = LOGIC_CONFIG->GetRoleGoalCfg().GetChapterCfgByPersonalGoal(cond_type);
	if (-1 == chapter) return false;

	if (m_param.cur_chapter >= chapter)
	{
		return true;
	}

	for (int i = 0; i < PERSONAL_GOAL_COND_MAX; i++)
	{
		const PersonalGoalChapterCfg::CondCfg &cfg = chapter_cfg->cond_list[i];

		if (m_param.data_list[i] >= cfg.cond_param1 && cfg.cond_type == cond_type)
		{
			return true;
		}
	}

	return false;
}

void RoleGoal::SendInfoToRole()
{
	Protocol::SCRoleGoalInfo pro;
	pro.cur_chapter = m_param.cur_chapter;
	pro.old_chapter = m_param.old_chapter;
	memcpy(pro.data_list, m_param.data_list, sizeof(pro.data_list));
	pro.field_goal_can_fetch_flag = m_param.field_goal_can_fetch_flag;
	pro.field_goal_fetch_flag = m_param.field_goal_fetch_flag;
	memcpy(pro.skill_level_list, m_param.skill_level_list, sizeof(pro.skill_level_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&pro, sizeof(pro));
}

void RoleGoal::CalcGoalExpAdd()
{
	m_kill_monster_exp_add = 0;

	UNSTD_STATIC_CHECK(FIELD_GOAL_KILL_MONSTER_EXP_ADD > 0 && FIELD_GOAL_KILL_MONSTER_EXP_ADD < FIELD_GOAL_SKILL_TYPE_MAX);
	int skill_level = m_param.skill_level_list[FIELD_GOAL_KILL_MONSTER_EXP_ADD];

	const BattleFieldGoalSkillCfg::ConfigItem *cfg = LOGIC_CONFIG->GetRoleGoalCfg().GetBattleFieldGoalSkillCfg(FIELD_GOAL_KILL_MONSTER_EXP_ADD, skill_level);
	if (NULL == cfg)
	{
		return;
	}

	m_kill_monster_exp_add = cfg->param_1;
}

void RoleGoal::CalcGoalHurtAdd()
{
	m_hurt_monster_add = 0;

	UNSTD_STATIC_CHECK(FIELD_GOAL_HURT_MONSTER_ADD > 0 && FIELD_GOAL_HURT_MONSTER_ADD < FIELD_GOAL_SKILL_TYPE_MAX);
	int skill_level = m_param.skill_level_list[FIELD_GOAL_HURT_MONSTER_ADD];

	const BattleFieldGoalSkillCfg::ConfigItem *cfg = LOGIC_CONFIG->GetRoleGoalCfg().GetBattleFieldGoalSkillCfg(FIELD_GOAL_HURT_MONSTER_ADD, skill_level);
	if (NULL == cfg)
	{
		return;
	}

	m_hurt_monster_add = cfg->param_1;
}

void RoleGoal::CalcGoalAbsord()
{
	m_absord_blood_trigger_per = 0;
	m_absord_blood_per = 0;
	m_absord_blood_colddown_time = 0;

	UNSTD_STATIC_CHECK(FIELD_GOAL_ABSORB_BLOOD > 0 && FIELD_GOAL_ABSORB_BLOOD < FIELD_GOAL_SKILL_TYPE_MAX);
	int skill_level = m_param.skill_level_list[FIELD_GOAL_ABSORB_BLOOD];

	const BattleFieldGoalSkillCfg::ConfigItem *cfg = LOGIC_CONFIG->GetRoleGoalCfg().GetBattleFieldGoalSkillCfg(FIELD_GOAL_ABSORB_BLOOD, skill_level);
	if (NULL == cfg)
	{
		return;
	}

	m_absord_blood_trigger_per = cfg->param_1;
	m_absord_blood_per = cfg->param_2;
	m_absord_blood_colddown_time = cfg->param_3;
}
