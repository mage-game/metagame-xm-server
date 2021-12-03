#include "gamelog.h"
#include "servercommon/servercommon.h"
#include "item/knapsack.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/shuxingdanconfig.hpp"
#include "other/chengjiu/chengjiuconfig.hpp"
#include "servercommon/noticenum.h"
#include "obj/character/attribute.hpp"
#include "config/logicconfigmanager.hpp"
#include <set>
#include "servercommon/string/gameworldstr.h"
#include "item/itempool.h"
#include "other/event/eventhandler.hpp"
#include "equipment/equipment.h"
#include "item/itembase.h"
#include "item/itemgriddata.h"
#include "scene/scene.h"
#include "chengjiu.h"
#include "protocal/msgchengjiu.h"
#include "globalconfig/globalconfig.h"
#include "world.h"
#include "other/capability/capability.hpp"

ChengJiu::ChengJiu() : m_role(NULL)
{
	
}

ChengJiu::~ChengJiu() 
{
 
}

void ChengJiu::Init(Role *role, const ChengJiuParam &chengjiu_param)
{
	m_role = role;
	m_chengjiu_param = chengjiu_param;

}

void ChengJiu::GetInitParam(ChengJiuParam *chengjiu_param)
{
	if (chengjiu_param != NULL)
	{
		*chengjiu_param = m_chengjiu_param;
	}
}

void ChengJiu::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		m_chengjiu_param.day_times = 0;
		this->SyncChengJiuInfo();
	}
}

void ChengJiu::OnLogin()
{
	this->RecalcChengJiuConditionOnLogin();
}

int ChengJiu::GetChengJiuTitleLevel(short *title_level)
{
	if (NULL != title_level)
	{
		*title_level = m_chengjiu_param.title_level;
	}

	return m_chengjiu_param.title_level;
}

void ChengJiu::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();
		
		{
			const TitleLevelCfg *level_cfg = LOGIC_CONFIG->GetChengJiuCfg().GetTitleLevelCfg(m_chengjiu_param.title_level);
			if (NULL != level_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, level_cfg->gongji);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, level_cfg->fangyu);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, level_cfg->maxhp);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, level_cfg->shanbi);
			}
		}

// 		{
// 			for(int i = 1; i <= m_chengjiu_param.fuwen_level + 1; ++ i)
// 			{
// 				const FuwenLevelCfg *fuwen_level_cfg = LOGIC_CONFIG->GetChengJiuCfg().GetFuwenLevelCfg(i);
// 				if (NULL != fuwen_level_cfg)
// 				{
// 					int times = i < m_chengjiu_param.fuwen_level + 1? fuwen_level_cfg->max_times : m_chengjiu_param.process_times;
// 
// 					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, fuwen_level_cfg->once_add_gongji * times);
// 					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, fuwen_level_cfg->once_add_fangyu * times);
// 					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, fuwen_level_cfg->once_add_maxhp * times);
// 					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, fuwen_level_cfg->once_add_shanbi * times);
// 
// 					if (times >= fuwen_level_cfg->max_times)
// 					{
// 						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI, fuwen_level_cfg->add_baoji_on_act);
// 						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO, fuwen_level_cfg->add_kangbao_on_act);
// 					}
// 				}
// 			}
// 		}

		// 属性丹
		for (int i = 0; i < ChengJiuParam::CHENGJIU_SHUXINGDAN_MAX_TYPE; ++ i)
		{
			const int num = m_chengjiu_param.shuxingdan_list[i];
			if (num > 0)
			{
				const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_CHENGJIU, i);

				if (NULL != cfg_item)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp * num;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji * num;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu * num;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong * num;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi * num;
				}
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_CHENGJIU, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void ChengJiu::UpTitleLevel()
{
	const TitleLevelCfg *level_cfg = LOGIC_CONFIG->GetChengJiuCfg().GetTitleLevelCfg(m_chengjiu_param.title_level + 1);
	if (NULL == level_cfg)
	{
		m_role->NoticeNum(errornum::EN_CHENGJIU_TITLE_MAX_LEVEL);
		return;
	}

	if (m_role->GetChengJiu() < level_cfg->chengjiu)
	{
		m_role->NoticeNum(errornum::EN_CHENGJIU_CHENGJIU_NOT_ENOUGH);
		return;
	}
	
	short old_title_level = m_chengjiu_param.title_level;

	int cost_chengjiu = -1 * abs(level_cfg->chengjiu);
	m_role->AddChengJiu(cost_chengjiu, "UpTitleLevel");
	++ m_chengjiu_param.title_level;

	EventHandler::Instance().OnUpChengJiuTitleLevel(m_role, old_title_level, m_chengjiu_param.title_level);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHENGJIU);
	this->SyncChengJiuInfo();
	this->BroadcastChengjiuTitleLevel();

	//传闻
	{
		if (NULL != level_cfg && level_cfg->is_broadcast > 0)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_link_chengjiu_title,
				UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), (int)m_chengjiu_param.title_level);
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	gamelog::g_log_chengjiu.printf(LL_INFO, "UpTitleLevel user %d %s, old_title_level %d, new_title_level %d, cost_chengjiu %d",
		m_role->GetUID(), m_role->GetName(), old_title_level, m_chengjiu_param.title_level, cost_chengjiu);
}

void ChengJiu::FetchReward(short reward_id)
{
	const ChengJiuRewardCfg *reward_cfg = LOGIC_CONFIG->GetChengJiuCfg().GetRewardCfg(reward_id);
	if (NULL == reward_cfg)
	{
		return;
	}

	if (reward_id <= 0 || reward_id >= CHENGJIU_REWARD_MAX_NUM)
	{
		return;
	}

	ChengJiuRewardItem &reward_item =  m_chengjiu_param.reward_list[reward_id];

	if (CJ_REWARD_STATUS_CAN_NOT_FETCH == reward_item.flag)
	{
		m_role->NoticeNum(errornum::EN_CHENGJIU_NOT_COMPLETE);
		return;
	}

	if (CJ_RWARD_STATUS_BE_FETCHED == reward_item.flag)
	{
		m_role->NoticeNum(errornum::EN_CHENGJIU_ALREADY_REWARD);
		return;
	}

	if (CJ_REWARD_STATUS_CAN_FETCH != reward_item.flag)
	{
		return;
	}

	reward_item.flag = CJ_RWARD_STATUS_BE_FETCHED;
	m_role->AddChengJiu(reward_cfg->chengjiu, "FetchChengjiuReward");
	m_role->GetKnapsack()->GetMoney()->AddGoldBind(reward_cfg->bind_gold, "FetchChengjiuReward");

	this->SyncChengJiuInfo();
}

void ChengJiu::UpFuwenLevel()
{
// 	const FuwenLevelCfg *level_cfg = LOGIC_CONFIG->GetChengJiuCfg().GetFuwenLevelCfg(m_chengjiu_param.fuwen_level + 1);
// 
// 	if (NULL == level_cfg)
// 	{
// 		m_role->NoticeNum(errornum::EN_CHENGJIU_FUWEN_MAX_LEVEL);
// 		return;
// 	}
// 
// 	if (m_chengjiu_param.title_level < level_cfg->title_limit)
// 	{
// 		m_role->NoticeNum(errornum::EN_CHENGJIU_TITLE_LIMIT);
// 		return;
// 	}
// 
// 	if (m_role->GetChengJiu() < level_cfg->once_cost_chengjiu)
// 	{
// 		m_role->NoticeNum(errornum::EN_CHENGJIU_CHENGJIU_NOT_ENOUGH);
// 		return;
// 	}
// 
// 	int need_gold = 0;
// 	LOGIC_CONFIG->GetChengJiuCfg().GetFuwenUpLevelCostGold(m_chengjiu_param.day_times + 1, &need_gold);
// 	if (need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_gold))
// 	{
// 		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 		return;
// 	}
// 
// 	int cost_chengjiu = -1 * abs(level_cfg->once_cost_chengjiu);
// 	m_role->AddChengJiu(cost_chengjiu, "UpFuwenLevel");
// 
// 	if (need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "UpFuwenLevel"))
// 	{
// 		return;
// 	}
// 
// 	const short old_process_times = m_chengjiu_param.process_times;
// 	const short old_fuwen_level = m_chengjiu_param.fuwen_level;
// 
// 	++ m_chengjiu_param.day_times;
// 
// 	m_chengjiu_param.process_times = m_chengjiu_param.process_times + 1;
// 	if (m_chengjiu_param.process_times >= level_cfg->max_times)
// 	{
// 		if (m_chengjiu_param.fuwen_level < LOGIC_CONFIG->GetChengJiuCfg().GetMaxFuwenLevel())
// 		{
// 			m_chengjiu_param.process_times = 0;
// 			m_chengjiu_param.fuwen_level = m_chengjiu_param.fuwen_level + 1;
// 		}
// 	}
// 
// 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHENGJIU);
// 	this->SyncChengJiuInfo();
// 
// 	gamelog::g_log_chengjiu.printf(LL_INFO, "UpFuwenLevel user %d %s, old_process_times %d, old_fuwen_level %d"
// 		"new_process_times %d, new_fuwen_level %d, cost_chengjiu %d",
// 		m_role->GetUID(), m_role->GetName(),(int)old_process_times, (int)old_fuwen_level,
// 		(int)m_chengjiu_param.process_times, (int)m_chengjiu_param.fuwen_level, cost_chengjiu);
}

void ChengJiu::SyncChengJiuInfo()
{
	static Protocol::SCChengJiuInfo cmd;

	cmd.title_level = m_chengjiu_param.title_level;

	cmd.fuwen_level = m_chengjiu_param.fuwen_level;
	cmd.process_times = m_chengjiu_param.process_times;

	UNSTD_STATIC_CHECK(sizeof(cmd.shuxingdan_list) == sizeof(m_chengjiu_param.shuxingdan_list));
	memcpy(cmd.shuxingdan_list, m_chengjiu_param.shuxingdan_list, sizeof(cmd.shuxingdan_list));

	cmd.need_gold_on_uplevel_next_fuwen = 0;
	LOGIC_CONFIG->GetChengJiuCfg().GetFuwenUpLevelCostGold(m_chengjiu_param.day_times + 1, &cmd.need_gold_on_uplevel_next_fuwen);

	cmd.count = LOGIC_CONFIG->GetChengJiuCfg().GetRewardMaxNum();
	
	for (int i = 0; i < CHENGJIU_REWARD_MAX_NUM - 1 && i < cmd.count; ++ i)
	{
		int reward_id = i + 1;
		cmd.reward_list[i].reward_id = reward_id;
		cmd.reward_list[i].flag = m_chengjiu_param.reward_list[reward_id].flag;
		cmd.reward_list[i].process = m_chengjiu_param.reward_list[reward_id].process;
		
		// 登录天数最少为1天
		const ChengJiuRewardCfg *reward_cfg = LOGIC_CONFIG->GetChengJiuCfg().GetRewardCfg(reward_id);
		if (NULL != reward_cfg && CJ_CONDITION_LOGIN_LAST_DAY == reward_cfg->condition && 0 == cmd.reward_list[i].process)
		{
			cmd.reward_list[i].process = 1;
		}
	}
	
	unsigned int sendlen = sizeof(cmd) - sizeof(cmd.reward_list[0]) * (CHENGJIU_REWARD_MAX_NUM - cmd.count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sendlen);
}

void ChengJiu::BroadcastChengjiuTitleLevel()
{
	if (NULL != m_role->GetScene())
	{
		Protocol::SCChengJiuTitleViewChange cmd;
		cmd.obj_id = m_role->GetId();
		cmd.title_level = m_chengjiu_param.title_level;

		m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
	}
}

// 有些在上线时为了绝对正确,成就需要重算
void ChengJiu::RecalcChengJiuConditionOnLogin()
{
	unsigned int skill_total_level = static_cast<unsigned int>(m_role->GetSkillManager()->GetSkillTotalLevel());
	this->OnChengJiuCondition(CJ_CONDITION_SKILL_TOTAL_LEVEL, skill_total_level, false);

	unsigned int all_gold_bind = static_cast<unsigned int>(m_role->GetKnapsack()->GetMoney()->GetGoldBind());
	this->OnChengJiuCondition(CJ_CONDITION_MONEY_GOLD_BIND, all_gold_bind, false);
}

void ChengJiu::OnChengJiuCondition(short condition, unsigned int param1, bool is_notify)
{
	if (param1 < 0 || condition <= CJ_CONDITION_INVALID || condition >= CJ_CONDITION_MAX)
	{
		return;
	}

	if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinOther(JLMT_OTHER_CHENGJIU, m_role))
	{
		return;
	}

	static Protocol::SCChengJiuRewardChange cmd;
	cmd.count = 0;

	std::map<short, ChengJiuRewardCfg> *reward_map = LOGIC_CONFIG->GetChengJiuCfg().GetRewardMap();
	
	std::map<short, ChengJiuRewardCfg>::const_iterator iter;

	for (iter = reward_map->begin(); iter != reward_map->end(); ++ iter)
	{
		if (condition == iter->second.condition && iter->second.reward_id >= 1 && iter->second.reward_id < CHENGJIU_REWARD_MAX_NUM)
		{
			bool is_change = false;

			ChengJiuRewardItem &reward_item = m_chengjiu_param.reward_list[iter->second.reward_id];
			if (reward_item.flag != CJ_REWARD_STATUS_CAN_NOT_FETCH)
			{
				continue;
			}

			switch (condition)
			{
			//累计个数或次数，每次累加1，数据库记录进度
			case CJ_CONDITION_KILL_MONSTER:
			case CJ_CONDITION_ADD_FRIEND:
			case CJ_CONDITION_JOIN_TEAM:
			case CJ_CONDITION_COMPOSE:
			case CJ_CONDITION_JOIN_GUILD:
			case CJ_CONDITION_EQUIP_STRENGTH:
			case CJ_CONDITION_EQUIP_UPSTAR:
			case CJ_CONDITION_EQUIP_SHENZHU:
			case CJ_CONDITION_EQUIP_UPLEVEL:
			case CJ_CONDITION_KILL_BOSS:
			case CJ_CONDITION_YSZC_KILL_ROLE:
			case CJ_CONDITION_XLT_KILL_ROLE:
			case CJ_CONDITION_GCZ_WIN_TIMES:
			case CJ_CONDITION_1V1_WIN_TIMES:
			case CJ_CONDITION_HUSONG_TIMES:
			case CJ_CONDITION_GUILD_FINISH_TASK:
			case CJ_CONDITION_GUILD_DONATION_EQU:
			case CJ_CONDITION_LTZ_SKILL_TA:
			case CJ_CONDITION_SKILL_ROLE:
			case CJ_CONDITION_SKILL_RED_NAME_ROLE:
			case CJ_CONDITION_SJHJ_SHUI_JIN_COUNT:
			case CJ_CONDITION_SKILL_GUILD_BOSS:
			case CJ_CONDITION_LIEQU_JINGLING_TOTAL:
			case CJ_CONDITION_EQU_JIEZHI:
				{
					is_change = true;
					reward_item.process += 1;
					if (reward_item.process >= iter->second.param1)
					{
						reward_item.flag = CJ_REWARD_STATUS_CAN_FETCH;
						Protocol::SCChengJiuRewardInfo cjri;
						cjri.reward_id = iter->second.reward_id;
						EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cjri, sizeof(cjri));
					}
				}
				break;

			//达到某个传过来的参数
			case CJ_CONDITION_ROLE_LEVEL:
			case CJ_CONDITION_LOGIN_LAST_DAY:
			case CJ_CONDITION_MONEY_GOLD_BIND:
			case CJ_CONDITION_PUTON_EQUIP_STRENGTH:
			case CJ_CONDITION_PUTON_EQUIP_STAR:
			case CJ_CONDITION_CHENGJIU_TITLE:
			case CJ_CONDITION_MOUNT_JIE:
			case CJ_CONDITION_SKILL_TOTAL_LEVEL:
			case CJ_CONDITION_XIANJIE_LEVEL:
			case CJ_CONDITION_WING_JIE_CHANGE:
			case CJ_CONDITION_FABAO_LEVEL:
			case CJ_CONDITION_EXP_FB_PASS_LEVEL:
			case CJ_CONDITION_PATA_PASS_LEVEL:
			case CJ_CONDITION_GUANG_HUAN_LEVEL:
			case CJ_CONDITION_MO_QI_LEVEL:
			case CJ_CONDITION_SHEN_GONG_LEVEL:
			case CJ_CONDITION_SHEN_WING_LEVEL:
			case CJ_CONDITION_XUN_ZHANG_TOTLE_LEVEL:
			case CJ_CONDITION_ZHUANGBEI_JINGLING_LEVEL:
			case CJ_CONDITION_NVSHEN_LEVEL:
			case CJ_CONDITION_EQU_1_STAR_AND_COLOR:
			case CJ_CONDITION_EQU_2_STAR_AND_COLOR:
			case CJ_CONDITION_EQU_3_STAR_AND_COLOR:
			case CJ_CONDITION_FOOTPRINT_LEVEL:
				{
					is_change = true;
					reward_item.process = param1;
					if (param1 >= iter->second.param1)
					{
						reward_item.flag = CJ_REWARD_STATUS_CAN_FETCH;
						Protocol::SCChengJiuRewardInfo cjri;
						cjri.reward_id = iter->second.reward_id;
						EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cjri, sizeof(cjri));
					}
				}
				break;
			
			//等于传过来的固定值
			case CJ_CONDITION_LIEQU_JINGLING:
			case CJ_CONDITION_ACTIVITE_NVSHEN:
			case CJ_CONDITION_ACTIVITE_MOJIE:
				{
					if (param1 == iter->second.param1)
					{
						is_change = true;
						reward_item.process = param1;
						reward_item.flag = CJ_REWARD_STATUS_CAN_FETCH;
						Protocol::SCChengJiuRewardInfo cjri;
						cjri.reward_id = iter->second.reward_id;
						EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cjri, sizeof(cjri));
					}
				}
				break;

			}

			if (is_change && cmd.count < CHENGJIU_REWARD_MAX_CHANGE_NUM_INBATCH)
			{
				cmd.reward_list[cmd.count].reward_id = iter->second.reward_id;
				cmd.reward_list[cmd.count].process = reward_item.process;
				cmd.reward_list[cmd.count].flag = reward_item.flag;
				++ cmd.count;
			}
		}
	}
	
	if (is_notify)
	{
		unsigned int sendlen = sizeof(cmd) - sizeof(cmd.reward_list[0]) * (CHENGJIU_REWARD_MAX_CHANGE_NUM_INBATCH - cmd.count);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sendlen);
	}
}

bool ChengJiu::OnUseShuxingdan(int slot_idx, int use_count)
{
	if (slot_idx < 0 || slot_idx >= ChengJiuParam::CHENGJIU_SHUXINGDAN_MAX_TYPE || use_count <= 0)
	{
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_CHENGJIU, slot_idx);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_chengjiu_param.shuxingdan_list[slot_idx] + use_count > cfg_item->use_limit_num)
	{
		m_role->NoticeNum(errornum::EN_SHUXINGDAN_USE_LIMIT_NUM);
		return false;
	}

	m_chengjiu_param.shuxingdan_list[slot_idx] += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CHENGJIU);
	this->SyncChengJiuInfo();

	return true;
}