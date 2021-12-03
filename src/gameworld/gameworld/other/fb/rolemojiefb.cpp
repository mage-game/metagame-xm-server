#include "rolemojiefb.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "servercommon/errornum.h"
#include "protocal/msgfb.h"
#include "other/event/eventhandler.hpp"
#include "monster/monsterpool.h"
#include "gameworld/world.h"
#include "servercommon/string/gameworldstr.h"
#include "scene/scene.h"
#include "item/itempool.h"
#include "other/route/mailroute.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "global/usercache/usercache.hpp"
#include "other/fb/mojiefbconfig.hpp"
#include "equipment/equipmentmanager.h"

#include "effect/effectbase.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "task/taskrecorder.h"

RoleMojieFB::RoleMojieFB() : m_role(NULL)
{

}

RoleMojieFB::~RoleMojieFB()
{

}

void RoleMojieFB::Init(Role *role, const MojieFBParam &param)
{
	m_role = role;
	m_param = param;
}

void RoleMojieFB::GetInitParam(MojieFBParam *param)
{
	*param = m_param;
}

bool RoleMojieFB::CanEnter(int fb_id)
{
	if (fb_id < 0 || fb_id >= MOJIE_FB_HUMAN_PIAN_MAX_COUNT)
	{
		return false;
	}

	const HumanPianCfg *cfg = LOGIC_CONFIG->GetMoJieFBConfig().GetHumanPianCfg(fb_id);
	if (NULL == cfg)
	{
		return false;
	}

	if (!m_role->GetTaskRecorder()->IsFinished(cfg->pretaskid))
	{
		m_role->NoticeNum(errornum::EN_MOJIE_NOT_FINISH_PRETASK);
		return false;
	}

	if (m_param.mojie_human_pian_task_mark >= fb_id)
	{
		m_role->NoticeNum(errornum::EN_MOJIE_TASK_BEEN_FINISH);
		return false;
	}

	return true;
}

void RoleMojieFB::OnEnterFB(int fb_id)
{
	if (fb_id < 0 || fb_id >= MOJIE_FB_HUMAN_PIAN_MAX_COUNT)
	{
		return;
	}
}

void RoleMojieFB::SendInfo()
{
	static Protocol::SCMoJieFBAllInfo cmd;

	cmd.mojie_human_pian_task_mark = m_param.mojie_human_pian_task_mark;
	cmd.mojie_airen_pian_task_mark = m_param.mojie_airen_pian_task_mark;
	cmd.mojie_jingling_pian_task_mark = m_param.mojie_jingling_pian_task_mark;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

const MojieFBParam * RoleMojieFB::GetMojieFbParam()
{
	return &m_param;
}

void RoleMojieFB::OnPassRenLeiFb(int fb_id)
{
	if (fb_id < 0 || fb_id >= MOJIE_FB_HUMAN_PIAN_MAX_COUNT || m_param.mojie_human_pian_task_mark != fb_id)
	{
		return;
	}

	m_param.mojie_human_pian_task_mark ++;
}

void RoleMojieFB::OnChangeFangyu() 
{
	this->OnPassJingLingFb(MOJIEFB_FANGYU_TASK, (int)m_role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU));
}

void RoleMojieFB::OnPutonEquip(short equip_type, short quality, short color)
{
	this->OnPassJingLingFb(MOJIEFB_ZHUANGBEI_TASK, equip_type, quality, color);
	this->OnPassJingLingFb(MOJIEFB_SHENZHU_TASK, m_role->GetEquipmentManager()->GetmTotalShenZhuLevel());
}

void RoleMojieFB::OnKillMonster(int monster_id)
{
	this->OnPassJingLingFb(MOJIEFB_BOSS_TASK, monster_id);
}

void RoleMojieFB::OnPassJingLingFb(int chapter, int param1, int param2, int param3)
{
	//暂时不开启魔戒系统
// 	const JingLingPianCfg::TaskConfig * cfg = LOGIC_CONFIG->GetMoJieFBConfig().GetJingLingCfgToParam(chapter, param1, param2, param3);
// 	if(NULL != cfg)
// 	{
// 		if((m_param.mojie_jingling_pian_task_mark & (1 << cfg->id)) == 0)
// 		{
// 			static MailContentParam contentparam; 
// 			contentparam.Reset();
// 
// 			UNSTD_STATIC_CHECK(MAX_ATTACHMENT_ITEM_NUM >= 2);
// 			contentparam.item_list[0].item_id = cfg->reward1.item_id;
// 			contentparam.item_list[0].is_bind = cfg->reward1.is_bind;
// 			contentparam.item_list[0].num = cfg->reward1.num;
// 
// 			contentparam.item_list[1].item_id = cfg->reward2.item_id;
// 			contentparam.item_list[1].is_bind = cfg->reward2.is_bind;
// 			contentparam.item_list[1].num = cfg->reward2.num;
// 
// 			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_mojie_jinglingpian_reward_notice, chapter + 1, cfg->task + 1, cfg->task_desc.c_str());
// 			if (length > 0)
// 			{
// 				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
// 			}
// 			m_param.mojie_jingling_pian_task_mark |= (1 << cfg->id);
// 		}
// 	}
}

bool RoleMojieFB::IsUseSkill(int skill_type)
{
	for(int i = 0; i <=  m_param.mojie_human_pian_task_mark; i ++)
	{
		const HumanPianCfg *cfg = LOGIC_CONFIG->GetMoJieFBConfig().GetHumanPianCfg(i);
		if(NULL != cfg)
		{
			if(skill_type == cfg->mojie_id)
			{
				return true;
			}
		}
		
	}

	for(int i = 0; i <= m_param.mojie_airen_pian_task_mark; i ++)
	{
		const AiRenPianCfg *cfg = LOGIC_CONFIG->GetMoJieFBConfig().GetAiRenCfg(i);
		if(NULL != cfg)
		{
			if(skill_type == cfg->mojie_id)
			{
				return true;
			}
		}

	}

	for(int i = 0; i < LOGIC_CONFIG->GetMoJieFBConfig().getJingLingChapterCfgCount(); i ++)// 章节
	{
		const JingLingPianCfg *jing_ling_pian_cfg = LOGIC_CONFIG->GetMoJieFBConfig().GetJingLingCfg(i);

		if(NULL != jing_ling_pian_cfg)
		{
			for(int j = 0; j < jing_ling_pian_cfg->task_list_count; j ++)// 章节的任务
			{
				const JingLingPianCfg::TaskConfig task = jing_ling_pian_cfg->task_list[j];
			
				if((m_param.mojie_jingling_pian_task_mark & (1 << task.id)) != 0)
				{
					break;
				}

				if(j == jing_ling_pian_cfg->task_list_count && skill_type == task.mojie_id)
				{
					return true;
				}
			}
			
		}
	}

	return false;
}

void RoleMojieFB::OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag)
{
	if (NULL == target)
	{
		return;
	}

	if (0 != target_index)
	{
		return;
	}

	if(m_role->GetFBMojie()->IsUseSkill(MOJIEFB_SKILL_HUIFU))
	{
		// 魔戒被动技能：加血
		const MoJieSkillCfg *cfg = LOGIC_CONFIG->GetMoJieFBConfig().GetMoJieCfg(MOJIEFB_SKILL_HUIFU);
		if (RandomNum(100) < cfg->param_a * ROLE_ATTR_PER * 100)
		{
			Attribute exp_per = cfg->param_a;
			m_role->ChangeHp(&exp_per, true);
		}
	}
	
	if(m_role->GetFBMojie()->IsUseSkill(MOJIEFB_SKILL_CHIDUN))
	{
		// 魔戒被动技能：减速
		const MoJieSkillCfg *cfg = LOGIC_CONFIG->GetMoJieFBConfig().GetMoJieCfg(MOJIEFB_SKILL_CHIDUN);
		if (RandomNum(100) < cfg->param_a * ROLE_ATTR_PER * 100)
		{	
			int dec_movespeed = static_cast<int>(target->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * (cfg->param_b * SKILL_ATTR_PER));
			EffectMoveSpeed *effect_speed = new EffectMoveSpeed(target->GetId(), skill_id, cfg->param_c, EBT_CHIHUAN, 1);
			effect_speed->SetMoveSpeed(-dec_movespeed, -1 * (int)(cfg->param_b * SKILL_ATTR_PER * 100));
			effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
			effect_speed->SetSave(true);
			target->AddEffect(effect_speed);
		}
	}

}

double RoleMojieFB::GetJingLingPianExpPer(int exp_per)
{
	if(m_role->GetFBMojie()->IsUseSkill(exp_per))
	{
		const MoJieSkillCfg *cfg = LOGIC_CONFIG->GetMoJieFBConfig().GetMoJieCfg(MOJIEFB_SKILL_SHEHUN);
		
		if(NULL != cfg)
		{
			double exp_per = cfg->param_a / 100;
			return exp_per;
		}
	}
	return -1;
}