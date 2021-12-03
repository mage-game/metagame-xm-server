#include "rolezhuanzhi.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "servercommon/errornum.h"
#include "other/event/eventhandler.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "servercommon/noticenum.h"
#include "scene/scene.h"
#include "scene/speciallogic/speciallogic.hpp"
#include "obj/character/monster.h"
#include "effect/skilleffect/effectonce.hpp"
#include "skill/skillpool.hpp"
#include "effect/skilleffect/effecthurtinc.hpp"
#include "task/newtaskmanager.h"
#include "item/knapsack.h"
#include "task/taskrecorder.h"
#include "skill/skillbase.hpp"
#include "friend/teaminfo.hpp"
#include "task/other/zhuanzhitask.hpp"
#include "protocal/msgrole.h"
#include "globalconfig/globalconfig.h"
#include "protocal/msgitem.h"
#include "other/itemlevelreward/itemlevelreward.hpp"

RoleZhuanZhi::RoleZhuanZhi() : m_role(NULL)
{

	m_zhuanzhi_info.Reset();
}

RoleZhuanZhi::~RoleZhuanZhi()
{

}

void RoleZhuanZhi::Init(Role *role, char zhuanzhi_tianming_level, char zhuanzhi_canglong_level, char zhuanzhi_shengxiao_level, char zhuanzhi_yuanqi_level, char zhuanzhi_shenqi_level, char zhuanzhi_xingxiu_level)
{
	m_role = role;
	m_zhuanzhi_info.zhuanzhi_level_tianming = zhuanzhi_tianming_level;
	m_zhuanzhi_info.zhuanzhi_level_canglong = zhuanzhi_canglong_level;
	m_zhuanzhi_info.zhuanzhi_level_shengxiao = zhuanzhi_shengxiao_level;
	m_zhuanzhi_info.zhuanzhi_level_yuanqi = zhuanzhi_yuanqi_level;
	m_zhuanzhi_info.zhuanzhi_level_shenqi = zhuanzhi_shenqi_level;
	m_zhuanzhi_info.zhuanzhi_level_xingxiu = zhuanzhi_xingxiu_level;

	m_skill.Init(m_role, this);
}


void RoleZhuanZhi::OnInitFinish()
{
	m_skill.OnInitFinish();
}

void RoleZhuanZhi::GetInitParam(char *zhuanzhi_tianming_level, char *zhuanzhi_canglong_level, char *zhuanzhi_shengxiao_level, char *zhuanzhi_yuanqi_level, char *zhuanzhi_shenqi_level, char *zhuanzhi_xingxiu_level)
{
	*zhuanzhi_tianming_level = m_zhuanzhi_info.zhuanzhi_level_tianming;
	*zhuanzhi_canglong_level = m_zhuanzhi_info.zhuanzhi_level_canglong;
	*zhuanzhi_shengxiao_level = m_zhuanzhi_info.zhuanzhi_level_shengxiao;
	*zhuanzhi_yuanqi_level = m_zhuanzhi_info.zhuanzhi_level_yuanqi;
	*zhuanzhi_shenqi_level = m_zhuanzhi_info.zhuanzhi_level_shenqi;
	*zhuanzhi_xingxiu_level = m_zhuanzhi_info.zhuanzhi_level_xingxiu;
}

void RoleZhuanZhi::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
 	if (is_recalc)
 	{
 		m_attrs_add.Reset();
 
		int complete_task_count = m_role->GetZhuanzhiTask()->GetZhuanzhiTaskCompleteCount();
		const ZhuanzhiAttrCfg *zhuanzhi_attr_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiAttrCfg(complete_task_count);
 		if (NULL != zhuanzhi_attr_cfg)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += zhuanzhi_attr_cfg->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += zhuanzhi_attr_cfg->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += zhuanzhi_attr_cfg->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += zhuanzhi_attr_cfg->per_pojia;
		}

		const ZhuanzhiStruct *tianming_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiTianMingCfg(m_zhuanzhi_info.zhuanzhi_level_tianming);
		if (NULL != tianming_cfg)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += tianming_cfg->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += tianming_cfg->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += tianming_cfg->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += tianming_cfg->per_pojia;
		}

		const ZhuanzhiStruct *canglong_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiCangLongCfg(m_zhuanzhi_info.zhuanzhi_level_canglong);
		if (NULL != canglong_cfg)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += canglong_cfg->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += canglong_cfg->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += canglong_cfg->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += canglong_cfg->per_pojia;
		}

		const ZhuanzhiStruct *shengxiao_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiShengXiaoCfg(m_zhuanzhi_info.zhuanzhi_level_shengxiao);
		if (NULL != shengxiao_cfg)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += shengxiao_cfg->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += shengxiao_cfg->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += shengxiao_cfg->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += shengxiao_cfg->per_pojia;
		}

		const ZhuanzhiStruct *yuanqi_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiYuanQiCfg(m_zhuanzhi_info.zhuanzhi_level_yuanqi);
		if (NULL != yuanqi_cfg)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += yuanqi_cfg->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += yuanqi_cfg->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += yuanqi_cfg->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += yuanqi_cfg->per_pojia;
		}

		const ZhuanzhiStruct *shenqi_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiShenQiCfg(m_zhuanzhi_info.zhuanzhi_level_shenqi);
		if (NULL != shenqi_cfg)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += shenqi_cfg->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += shenqi_cfg->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += shenqi_cfg->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += shenqi_cfg->per_pojia;
		}

		const ZhuanzhiStruct *xingxiu_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiXingXiuCfg(m_zhuanzhi_info.zhuanzhi_level_xingxiu);
		if (NULL != xingxiu_cfg)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += xingxiu_cfg->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += xingxiu_cfg->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += xingxiu_cfg->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += xingxiu_cfg->per_pojia;
		}

		// 转职技能
 		int prof_level = m_role->GetProfLevel();
 		for (; prof_level >= 1; prof_level--)
 		{
 			const ZhuanZhiLevelSkillConfig::ConfigItem *zhuanzhi_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiSkillCfg(prof_level, m_role->GetBaseProf());
 			if (NULL != zhuanzhi_cfg)
 			{
				// 策划需求，转职技能pvp / pve属性不加战斗力
 				//m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += zhuanzhi_cfg->pvp_zengshang_per;
 				//m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += zhuanzhi_cfg->pvp_jianshang_per;
 				//m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += zhuanzhi_cfg->pve_zengshang_per;
 				//m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += zhuanzhi_cfg->pve_jianshang_per;
				
				// 2转技能 角色升级属性增加
				const RoleLevelCfg* lvl_cfg = GLOBALCONFIG->GetRoleLvlCfg(m_role->GetLevel());
				if (NULL != lvl_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(lvl_cfg->maxhp * zhuanzhi_cfg->role_level_attr_per * ROLE_ATTR_PER);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(lvl_cfg->gongji * zhuanzhi_cfg->role_level_attr_per * ROLE_ATTR_PER);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(lvl_cfg->fangyu * zhuanzhi_cfg->role_level_attr_per * ROLE_ATTR_PER);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (Attribute)(lvl_cfg->mingzhong * zhuanzhi_cfg->role_level_attr_per * ROLE_ATTR_PER);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (Attribute)(lvl_cfg->shanbi * zhuanzhi_cfg->role_level_attr_per * ROLE_ATTR_PER);
				}
 			}
 		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_ZHUANZHI, m_attrs_add);
	}
	
	base_add.Add(m_attrs_add.m_attrs, true);
}


void RoleZhuanZhi::Update(unsigned long interval, unsigned int now_second)
{
	m_skill.CheckRecover(now_second);
	m_skill.CheckKillOtherRole(false);
	m_skill.CheckBeenAttack(now_second);
}

void RoleZhuanZhi::OnKillOtherRole(Role * target_role)
{
	if (nullptr == target_role)
	{
		return;
	}

	m_skill.CheckKillOtherRole(true);
}

//一键完成三转所有任务
void RoleZhuanZhi::OnOneKeyFinishLevel3TaskReq()
{
	if (m_role->GetProfLevel() != ONE_KEY_FINISH_TASK_PROF_LEVEL_LIMIT)
	{
		return;
	}

	//三转转职任务
	const ZhuanzhiTaskConfig::ConfigItem *cfg = TASKPOOL->GetZhuanzhiTaskConfigByProfLevel(m_role->GetProfLevel() + 1);
	if (NULL == cfg)
	{
		return;
	}

	int task_begin = cfg->first_task;

	int cur_task_id = cfg->first_task;
	if (m_role->GetTaskManager()->GetCurZhuanzhiTaskid(&cur_task_id))
	{
		task_begin = cur_task_id;
	}

	const RoleZhuanZhiOtherCfg &other_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetRoleZhuanZhiOtherCfg();
	int one_key_level3_task_cost_gold = 0;
	int have_item_num = 0;
	int still_need_num = 0;

	int have_nanmu_num = 0;
	int have_yaodan_num = 0;
	int yaodan_id = other_cfg.one_key_item_id_1;
	int nanmu_id = other_cfg.one_key_item_id_2;
	for (int i = task_begin; i <= cfg->end_task; i++)
	{
		const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(i);
		if (NULL == task_cfg)
		{
			continue;
		}

		if (task_cfg->condition != TASK_CON_COMMIT_ITEM)
		{
			continue;
		}
		have_item_num = m_role->GetKnapsack()->GetItemNum(task_cfg->c_param1);
		still_need_num = task_cfg->c_param2 - have_item_num > 0 ? task_cfg->c_param2 - have_item_num : 0;
		if (still_need_num > 0)
		{
			int single_price = other_cfg.one_key_item_price_2;   //
			if (task_cfg->c_param1 == yaodan_id)               // 
			{
				single_price = other_cfg.one_key_item_price_1;
				have_yaodan_num = have_item_num;
			}
			else if (task_cfg->c_param1 == nanmu_id)
			{
				have_nanmu_num = have_item_num;
			}
			int temp_need_gold = single_price * still_need_num;
			one_key_level3_task_cost_gold += temp_need_gold;
		}
	}



	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(one_key_level3_task_cost_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (yaodan_id > 0 && have_yaodan_num > 0 && !m_role->GetKnapsack()->ConsumeItem(yaodan_id, have_yaodan_num, "OnOneKeyFinishLevel3TaskReq"))
	{
		return;
	}

	if (nanmu_id > 0 && have_nanmu_num > 0 && !m_role->GetKnapsack()->ConsumeItem(nanmu_id, have_nanmu_num, "OnOneKeyFinishLevel3TaskReq"))
	{
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(one_key_level3_task_cost_gold, "OnOneKeyFinishLevel3TaskReq"))
	{
		return;
	}

	for (int task_id = task_begin; task_id <= cfg->end_task; task_id++)
	{
		m_role->GetTaskManager()->AcceptTask(task_id);
		m_role->GetTaskManager()->ForceCompleteTask(task_id);
		m_role->GetTaskManager()->CommitTask(task_id, true, false, true);
	}
}

void RoleZhuanZhi::OnRoleZhuanZhiReq(short opera_type, short param1, short param2, short param3)
{
	switch (opera_type)
	{
	case Protocol::CSRoleZhuanZhiReq::ZHUANZHI_OPERA_TYPE_TIANMING:
	{
		this->OnTianMingUpLevel(param1 != 0, param2 ==1);
	}
	break;

	case Protocol::CSRoleZhuanZhiReq::ZHUANZHI_OPERA_TYPE_CANGLONG:
	{
		this->OnCangLongUpLevel(param1 != 0, param2 == 1);
	}
	break;

	case Protocol::CSRoleZhuanZhiReq::ZHUANZHI_OPERA_TYPE_SHENGXIAO:
	{
		this->OnShengXiaoUpLevel(param1 != 0, param2 == 1);
	}
	break;

	case Protocol::CSRoleZhuanZhiReq::ZHUANZHI_OPERA_TYPE_YUANQI:
	{
		this->OnYuanQiUpLevel(param1 != 0, param2 == 1);
	}
	break;

	case Protocol::CSRoleZhuanZhiReq::ZHUANZHI_OPERA_TYPE_SHENQI:
	{
		this->OnShenQiUpLevel(param1 != 0, param2 == 1);
	}
	break;

	case Protocol::CSRoleZhuanZhiReq::ZHUANZHI_OPERA_TYPE_XINGXIU:
	{
		this->OnXingXiuUpLevel(param1 != 0, param2 == 1);
	}
	break;

	case Protocol::CSRoleZhuanZhiReq::ZHUANZHI_OPERA_TYPE_ONEKEY:
	{
		this->OnOneKeyFinishLevel3TaskReq();
	}
	break;

	case Protocol::CSRoleZhuanZhiReq::ZHUANZHI_OPERA_TYPE_PERFORM_SKILL:
	{
		m_skill.OnPerformSkillReq(param1, param2);
	}
	break;

	default:
		break;
	}
}

void RoleZhuanZhi::SendRoleZhuanZhiInfo()
{
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&m_zhuanzhi_info, sizeof(m_zhuanzhi_info));
}


void RoleZhuanZhi::GMFinishZhuanzhiTask()
{
	const ZhuanzhiTaskConfig::ConfigItem *cfg = TASKPOOL->GetZhuanzhiTaskConfigByProfLevel(m_role->GetProfLevel() + 1);
	if (NULL == cfg)
	{
		return;
	}

	int task_begin = cfg->first_task;

	int cur_task_id = cfg->first_task;
	if (m_role->GetTaskManager()->GetCurZhuanzhiTaskid(&cur_task_id))
	{
		task_begin = cur_task_id;
	}

	for (int task_id = task_begin; task_id <= cfg->end_task; task_id++)
	{
		m_role->GetTaskManager()->AcceptTask(task_id);
		m_role->GetTaskManager()->ForceCompleteTask(task_id);
		m_role->GetTaskManager()->CommitTask(task_id, true, false, true);
	}
}

bool RoleZhuanZhi::UpLevel(const ZhuanzhiStruct *next_level_cfg, bool is_use_exp, const char *reason, bool is_auto_buy)
{
	if (next_level_cfg == nullptr || reason == nullptr)
	{
		return false;
	}

	if (INVALID_TASK_ID != next_level_cfg->pretaskid && !m_role->GetTaskRecorder()->IsFinished(next_level_cfg->pretaskid))
	{
		m_role->NoticeNum(errornum::EN_TASK_ACCEPT_PRE_TASK);
		return false;
	}

	if (is_use_exp)
	{
		//if (m_role->GetExp() >= next_level_cfg->need_exp)
		//{
		//	long long reduce_exp = 0;
		//	m_role->ReduceExp(next_level_cfg->need_exp, reason, &reduce_exp);
		//	return true;
		//}

		long long need_exp = static_cast<long long>(next_level_cfg->exp_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(m_role->GetLevel(), 0)));

		if (m_role->GetExp() >= need_exp)
		{
			long long reduce_exp = 0;
			m_role->ReduceExp(need_exp, reason, &reduce_exp);

			//log

			return true;
		}
	}

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();

	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[0].item_id = next_level_cfg->need_stuff_id;
	stuff_list[0].num = next_level_cfg->need_stuff_num;
	stuff_list[0].buyable = is_auto_buy;

	if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list))
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return false;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "RoleZhuanZhi::UpLevel"))
	{
		return false;
	}

	return true;
}

void RoleZhuanZhi::OnTianMingUpLevel(bool is_use_exp, bool is_auto_buy)
{
	if (m_role->GetProfLevel() < TIANMING_LIGHT_UP_PROF_LEVEL_LIMIT)
	{
		return;
	}

	if (m_zhuanzhi_info.zhuanzhi_level_tianming >= MAX_ZHUANZHI_TIANMING_LEVEL)
	{
		return;
	}

	const ZhuanzhiStruct *next_level_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiTianMingCfg(m_zhuanzhi_info.zhuanzhi_level_tianming + 1);
	if (NULL == next_level_cfg)
	{
		return;
	}

	if (!this->UpLevel(next_level_cfg, is_use_exp, "OnTianMingUpLevel", is_auto_buy))
	{
		return;
	}
	
	m_zhuanzhi_info.zhuanzhi_level_tianming++;

	this->SendRoleZhuanZhiInfo();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI);

	EventHandler::Instance().OnLightUpZhuanZhi(m_role, TASK_SATISFY_STATUS_TYPE_LIGHT_UP_TIANMING, m_zhuanzhi_info.zhuanzhi_level_tianming);

	m_role->NoticeNum(noticenum::NT_LIGHT_TIANMING_SUCC);
}

void RoleZhuanZhi::OnCangLongUpLevel(bool is_use_exp, bool is_auto_buy)
{
	if (m_role->GetProfLevel() < CANGLONG_LIGHT_UP_PROF_LEVEL_LIMIT)
	{
		return;
	}

	if (m_zhuanzhi_info.zhuanzhi_level_canglong >= MAX_ZHUANZHI_CANGLONG_LEVEL)
	{
		return;
	}

	const ZhuanzhiStruct *next_level_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiCangLongCfg(m_zhuanzhi_info.zhuanzhi_level_canglong + 1);
	if (NULL == next_level_cfg)
	{
		return;
	}

	if (!this->UpLevel(next_level_cfg, is_use_exp, "OnCangLongUpLevel", is_auto_buy))
	{
		return;
	}

	m_zhuanzhi_info.zhuanzhi_level_canglong++;

	this->SendRoleZhuanZhiInfo();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI);

	EventHandler::Instance().OnLightUpZhuanZhi(m_role, TASK_SATISFY_STATUS_TYPE_LIGHT_UP_CANGLONG, m_zhuanzhi_info.zhuanzhi_level_canglong);

	m_role->NoticeNum(noticenum::NT_LIGHT_CANGLONG_SUCC);
}

void RoleZhuanZhi::OnShengXiaoUpLevel(bool is_use_exp, bool is_auto_buy)
{
	if (m_role->GetProfLevel() < SHENGXIAO_LIGHT_UP_PROF_LEVEL_LIMIT)
	{
		return;
	}

	if (m_zhuanzhi_info.zhuanzhi_level_shengxiao >= MAX_ZHUANZHI_SHENGXIAO_LEVEL)
	{
		return;
	}

	const ZhuanzhiStruct *next_level_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiShengXiaoCfg(m_zhuanzhi_info.zhuanzhi_level_shengxiao + 1);
	if (NULL == next_level_cfg)
	{
		return;
	}

	if (!this->UpLevel(next_level_cfg, is_use_exp, "OnShengXiaoUpLevel", is_auto_buy))
	{
		return;
	}

	m_zhuanzhi_info.zhuanzhi_level_shengxiao++;

	this->SendRoleZhuanZhiInfo();

	EventHandler::Instance().OnLightUpZhuanZhi(m_role, TASK_SATISFY_STATUS_TYPE_LIGHT_UP_SHENGXIAO, m_zhuanzhi_info.zhuanzhi_level_shengxiao);
	m_role->NoticeNum(noticenum::NT_LIGHT_SHENGXIAO_SUCC);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI);
}

void RoleZhuanZhi::OnYuanQiUpLevel(bool is_use_exp, bool is_auto_buy)
{
	if (m_role->GetProfLevel() < YUANQI_LIGHT_UP_PROF_LEVEL_LIMIT)
	{
		return;
	}

	if (m_zhuanzhi_info.zhuanzhi_level_yuanqi >= MAX_ZHUANZHI_YUANQI_LEVEL)
	{
		return;
	}

	const ZhuanzhiStruct *next_level_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiYuanQiCfg(m_zhuanzhi_info.zhuanzhi_level_yuanqi + 1);
	if (NULL == next_level_cfg)
	{
		return;
	}

	if (!this->UpLevel(next_level_cfg, is_use_exp, "OnYuanQiUpLevel", is_auto_buy))
	{
		return;
	}

	m_zhuanzhi_info.zhuanzhi_level_yuanqi++;

	this->SendRoleZhuanZhiInfo();

	EventHandler::Instance().OnLightUpZhuanZhi(m_role, TASK_SATISFY_STATUS_TYPE_LIGHT_UP_YUANQI, m_zhuanzhi_info.zhuanzhi_level_yuanqi);
	m_role->NoticeNum(noticenum::NT_LIGHT_YUANQI_SUCC);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI);
}

void RoleZhuanZhi::OnShenQiUpLevel(bool is_use_exp, bool is_auto_buy)
{
	if (m_role->GetProfLevel() < SHENQI_LIGHT_UP_PROF_LEVEL_LIMIT)
	{
		return;
	}

	if (m_zhuanzhi_info.zhuanzhi_level_shenqi >= MAX_ZHUANZHI_SHENQI_LEVEL)
	{
		return;
	}

	const ZhuanzhiStruct *next_level_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiShenQiCfg(m_zhuanzhi_info.zhuanzhi_level_shenqi + 1);
	if (NULL == next_level_cfg)
	{
		return;
	}

	if (!this->UpLevel(next_level_cfg, is_use_exp, "OnShenQiUpLevel", is_auto_buy))
	{
		return;
	}

	m_zhuanzhi_info.zhuanzhi_level_shenqi++;

	this->SendRoleZhuanZhiInfo();

	EventHandler::Instance().OnLightUpZhuanZhi(m_role, TASK_SATISFY_STATUS_TYPE_LIGHT_UP_SHENQI, m_zhuanzhi_info.zhuanzhi_level_shenqi);
	m_role->NoticeNum(noticenum::NT_LIGHT_SHENQI_SUCC);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI);
}

void RoleZhuanZhi::OnXingXiuUpLevel(bool is_use_exp, bool is_auto_buy)
{
	if (m_role->GetProfLevel() < XINGXIU_LIGHT_UP_PROF_LEVEL_LIMIT)
	{
		return;
	}

	if (m_zhuanzhi_info.zhuanzhi_level_xingxiu >= MAX_ZHUANZHI_XINGXIU_LEVEL)
	{
		return;
	}

	const ZhuanzhiStruct *next_level_cfg = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiXingXiuCfg(m_zhuanzhi_info.zhuanzhi_level_xingxiu + 1);
	if (NULL == next_level_cfg)
	{
		return;
	}

	if (!this->UpLevel(next_level_cfg, is_use_exp, "OnXingXiuUpLevel", is_auto_buy))
	{
		return;
	}

	m_zhuanzhi_info.zhuanzhi_level_xingxiu++;

	this->SendRoleZhuanZhiInfo();

	EventHandler::Instance().OnLightUpZhuanZhi(m_role, TASK_SATISFY_STATUS_TYPE_LIGHT_UP_XINGXIU, m_zhuanzhi_info.zhuanzhi_level_xingxiu);
	m_role->NoticeNum(noticenum::NT_LIGHT_XINGXIU_SUCC);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHUANZHI);
}
