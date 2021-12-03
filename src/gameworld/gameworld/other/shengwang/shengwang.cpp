#include "gamelog.h"
#include "servercommon/servercommon.h"
#include "item/knapsack.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include "engineadapter.h"
#include "servercommon/noticenum.h"
#include "obj/character/attribute.hpp"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include "config/shuxingdanconfig.hpp"
#include <set>
#include "servercommon/string/gameworldstr.h"
#include "item/itempool.h"
#include "other/event/eventhandler.hpp"
#include "equipment/equipment.h"
#include "item/itembase.h"
#include "item/itemgriddata.h"
#include "scene/scene.h"
#include "shengwang.h"
#include "shengwangconfig.hpp"
#include "protocal/msgshengwang.h"
#include "globalconfig/globalconfig.h"

#include "world.h"
#include "gstr.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/servercommon.h"
#include "other/capability/capability.hpp"

ShengWang::ShengWang() : m_role(NULL)
{
	
}

ShengWang::~ShengWang() 
{
 
}

void ShengWang::Init(Role *role, const ShengWangParam &shengwang_param)
{
	m_role = role;
	m_shengwang_param = shengwang_param;
}

void ShengWang::GetInitParam(ShengWangParam *shengwang_param)
{
	*shengwang_param = m_shengwang_param;
}

void ShengWang::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		m_shengwang_param.day_times = 0;
		this->SyncShengWangInfo();
	}
}

void ShengWang::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();
		
		const XianJieLevelCfg *xianjie_level_cfg = LOGIC_CONFIG->GetShengWangCfg().GetXianJieLevelCfg(m_shengwang_param.xianjie_level);
		if (NULL != xianjie_level_cfg)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, xianjie_level_cfg->gongji);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, xianjie_level_cfg->fangyu);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, xianjie_level_cfg->maxhp);
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, xianjie_level_cfg->mingzhong);
		}

		for(int i = 1; i <= m_shengwang_param.xiandan_level + 1; ++ i)
		{
			const XianDanLevelCfg *xiandan_level_cfg = LOGIC_CONFIG->GetShengWangCfg().GetXianDanLevelCfg(i);
			if (NULL != xiandan_level_cfg)
			{
				int times = i < m_shengwang_param.xiandan_level + 1 ? xiandan_level_cfg->max_times : m_shengwang_param.process_times;

				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, xiandan_level_cfg->once_add_gongji * times);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, xiandan_level_cfg->once_add_fangyu * times);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, xiandan_level_cfg->once_add_maxhp * times);
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, xiandan_level_cfg->once_add_mingzhong * times);

				if (times >= xiandan_level_cfg->max_times)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT, xiandan_level_cfg->add_baoji_on_act);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO, xiandan_level_cfg->add_kangbao_on_act);
				}
			}
		}

		//  Ù–‘µ§
		for (int i = 0; i < ShengWangParam::SHENGWANG_SHUXINGDAN_MAX_TYPE; ++ i)
		{
			const int num = m_shengwang_param.shuxingdan_list[i];

			if (num > 0)
			{
				const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_SHENGWANG, i);

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
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SHENGWANG, m_attrs_add);
}

void ShengWang::GetXianJieLevel(short *xianjie_level)
{
	if (NULL != xianjie_level)
	{
		*xianjie_level = m_shengwang_param.xianjie_level;
	}
}

void ShengWang::UpXianjieLevel()
{
	const XianJieLevelCfg *level_cfg = LOGIC_CONFIG->GetShengWangCfg().GetXianJieLevelCfg(m_shengwang_param.xianjie_level + 1);

	if (NULL == level_cfg)
	{
		m_role->NoticeNum(errornum::EN_SHENGWANG_XIANJIE_MAX_LEVEL);
		return;
	}

	if (m_role->GetShengwang() < level_cfg->shengwang)
	{
		m_role->NoticeNum(errornum::EN_SHENGWANG_NOT_ENOUGH);
		return;
	}

	m_role->AddShengwang(-1 * abs(level_cfg->shengwang), "UpXianjieCost");
	int old_xianjie_level = m_shengwang_param.xianjie_level;
	++ m_shengwang_param.xianjie_level;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGWANG);
	this->SyncShengWangInfo();
	this->BroadcastXianJieLevel();

	EventHandler::Instance().OnUpXianJieLevel(m_role, old_xianjie_level, m_shengwang_param.xianjie_level);

	//¥´Œ≈
	{
		if (NULL != level_cfg && level_cfg->is_broadcast > 0)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_link_xianjie,
				UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), (int)m_shengwang_param.xianjie_level);
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	gamelog::g_log_shengwang.printf(LL_INFO, "UpXianjieLevel user %d %s, old_xianjie_level %d, new_xianjie_level %d, cost_shengwang %d",
		m_role->GetUID(), m_role->GetName(), (int)old_xianjie_level, (int)m_shengwang_param.xianjie_level, level_cfg->shengwang);
}

void ShengWang::UpXiandanLevel()
{
	const XianDanLevelCfg *level_cfg = LOGIC_CONFIG->GetShengWangCfg().GetXianDanLevelCfg(m_shengwang_param.xiandan_level + 1);

	if (NULL == level_cfg)
	{
		m_role->NoticeNum(errornum::EN_SHENGWANG_XIANDAN_MAX_LEVEL);
		return;
	}

	if (m_shengwang_param.xianjie_level < level_cfg->xianjie_limit)
	{
		m_role->NoticeNum(errornum::EN_SHENGWANG_XIANJIE_LIMIT);
		return;
	}

	if (m_role->GetShengwang() < level_cfg->once_cost_shengwang)
	{
		m_role->NoticeNum(errornum::EN_SHENGWANG_NOT_ENOUGH);
		return;
	}

	int need_gold = 0;
	LOGIC_CONFIG->GetShengWangCfg().GetXianDanUpLevelCostGold(m_shengwang_param.day_times + 1, &need_gold);
	if (need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_role->AddShengwang(-1 * abs(level_cfg->once_cost_shengwang), "UpXianDanLevel");

	if (need_gold > 0 && !m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "UpXianDanLevel"))
	{
		return;
	}

	const short old_process_times = m_shengwang_param.process_times;
	const short old_xiandan_level = m_shengwang_param.xiandan_level;

	++ m_shengwang_param.day_times;

	m_shengwang_param.process_times = m_shengwang_param.process_times + 1;
	if (m_shengwang_param.process_times >= level_cfg->max_times)
	{
		if (m_shengwang_param.xiandan_level < LOGIC_CONFIG->GetShengWangCfg().GetMaxXianDanLevel())
		{
			m_shengwang_param.process_times = 0;
			m_shengwang_param.xiandan_level = m_shengwang_param.xiandan_level + 1;
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGWANG);
	this->SyncShengWangInfo();

	gamelog::g_log_shengwang.printf(LL_INFO, "UpXiandanLevel user %d %s, old_process_times %d, old_xiandan_level %d, new_process_times %d, new_xiandan_level %d",
		m_role->GetUID(), m_role->GetName(), (int)old_process_times, (int)old_xiandan_level,
		(int)m_shengwang_param.process_times, (int)m_shengwang_param.xiandan_level);
}

void ShengWang::SyncShengWangInfo()
{
	static Protocol::SCShengWangInfo cmd;

	UNSTD_STATIC_CHECK(sizeof(cmd.shuxingdan_list) == sizeof(m_shengwang_param.shuxingdan_list));
	memcpy(cmd.shuxingdan_list, m_shengwang_param.shuxingdan_list, sizeof(cmd.shuxingdan_list));

	cmd.xianjie_level = m_shengwang_param.xianjie_level;
	cmd.xiandan_level = m_shengwang_param.xiandan_level;
	cmd.process_times = m_shengwang_param.process_times;

	cmd.need_gold_on_uplevel_next_xiandan = 0;
	LOGIC_CONFIG->GetShengWangCfg().GetXianDanUpLevelCostGold(m_shengwang_param.day_times + 1, &cmd.need_gold_on_uplevel_next_xiandan);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sizeof(cmd));
}

void ShengWang::BroadcastXianJieLevel()
{
	if (NULL != m_role->GetScene())
	{
		Protocol::SCXianJieViewChange cmd;
		cmd.obj_id = m_role->GetId();
		cmd.xianjie_level = m_shengwang_param.xianjie_level;

		m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void*)&cmd, sizeof(cmd));
	}
}

bool ShengWang::OnUseShuxingdan(int slot_idx, short &use_count)
{
	if (slot_idx < 0 || slot_idx >= SHUXINGDAN_MAX_TYPE || use_count <= 0)
	{
		return false;
	}

	if (slot_idx < 0 || slot_idx >= ShengWangParam::SHENGWANG_SHUXINGDAN_MAX_TYPE || use_count <= 0) 
	{
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_SHENGWANG, slot_idx);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (m_shengwang_param.shuxingdan_list[slot_idx] + use_count > cfg_item->use_limit_num)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", slot_idx);
		return false;
	}

	m_shengwang_param.shuxingdan_list[slot_idx] += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGWANG);
	this->SyncShengWangInfo();

	return true;
}