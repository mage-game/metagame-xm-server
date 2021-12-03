#include "rolevipfb.hpp"
#include "engineadapter.h"
#include "protocal/msgother.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/fb/vipfbconfig.hpp"
#include "scene/scene.h"
#include "servercommon/errornum.h"
#include "protocal/msgfb.h"
#include "global/worldstatus/worldstatus.hpp"
#include "global/usercache/usercache.hpp"
#include "item/knapsack.h"
#include "other/vip/vip.hpp"

RoleVipFB::RoleVipFB() : m_role(NULL)
{
}

RoleVipFB::~RoleVipFB()
{
}

void RoleVipFB::Init(Role *role, const VipFBParam &param)
{
	m_role = role;
	m_param = param;
}

void RoleVipFB::GetInitParam(VipFBParam *param)
{
	*param = m_param;
}

void RoleVipFB::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	for (int i = 0; i < FB_VIP_NUM_MAX; i++)
	{
		m_param.today_times_list[i] = 0;
	}

	this->SendAllInfo();
}

bool RoleVipFB::CanEnter(int level)
{
	if (level < 0 || level >= FB_VIP_NUM_MAX)
	{
		return false;
	}

	const VipLevelConfig *level_cfg = LOGIC_CONFIG->GetVipFbCfg().GetLevelCfgByLevel(level);
	if (NULL == level_cfg)
	{
		return false;
	}

	if (level_cfg->enter_level > m_role->GetVip()->GetVipLevel())
	{
		return false;
	}

	if (this->IsPassLevel(level))
	{
		return false;
	}

	if (m_param.today_times_list[level] >= level_cfg->free_times)
	{
		return false;
	}

	return true;
}

void RoleVipFB::OnEnterFB(int level)
{
	if (level < 0 || level >= FB_VIP_NUM_MAX)
	{
		return;
	}

	this->SendAllInfo();
}

void RoleVipFB::OnFinish(int level)
{
	if (level < 0 || level >= FB_VIP_NUM_MAX)
	{
		return;
	}

	const VipLevelConfig *level_cfg = LOGIC_CONFIG->GetVipFbCfg().GetLevelCfgByLevel(level);
	if (NULL != level_cfg)
	{
		m_role->GetKnapsack()->PutListOrMail(level_cfg->reward_item, PUT_REASON_VIP_PASS_REWARD);
		m_param.is_pass_flag |= (1 << level);
		m_param.today_times_list[level]++;
	}

	this->SendAllInfo();

	gamelog::g_log_fb.printf(LL_INFO, "RolePataFB::OnFinish user[%d, %s] role_level[%d], pass_level[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), level);
}

void RoleVipFB::AutoFBReq(int level)
{
	Scene *scene = m_role->GetScene();
	if (NULL == scene)
	{
		return;
	}

	if (Scene::SCENE_TYPE_VIP_FB == scene->GetSceneType())
	{
		m_role->NoticeNum(errornum::EN_FB_OPERATE_LIMIT);
		return;
	}

	if (level < 0 || level >= FB_VIP_NUM_MAX)
	{
		return;
	}

	const VipLevelConfig *level_cfg = LOGIC_CONFIG->GetVipFbCfg().GetLevelCfgByLevel(level);
	if (NULL == level_cfg)
	{
		return;
	}

	if (!this->IsPassLevel(level))
	{
		m_role->NoticeNum(errornum::EN_PHASE_FB_NO_PASS);
		return;
	}

	if (m_param.today_times_list[level] >= level_cfg->free_times)
	{
		return;
	}

	m_role->GetKnapsack()->PutListOrMail(level_cfg->reward_item, PUT_REASON_VIP_PASS_REWARD);
	m_param.today_times_list[level]++;
	this->SendAllInfo();

	gamelog::g_log_fb.printf(LL_INFO, "RolePataFB::OnAutoFB user[%d, %s] role_level[%d], pass_level[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), level);
}

bool RoleVipFB::IsPassLevel(int level)
{
	if (level < 0 || level >= FB_VIP_NUM_MAX)
	{
		return false;
	}

	return 0 != (m_param.is_pass_flag & (1 << level));;
}

void RoleVipFB::SendAllInfo()
{
	static Protocol::SCVipFbAllInfo cmd;
	cmd.is_pass_flag = m_param.is_pass_flag;
	memcpy(cmd.today_times_list, m_param.today_times_list, sizeof(cmd.today_times_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&cmd, sizeof(cmd));
}
