#include "xiannvshouhumanager.hpp"

#include "gamelog.h"
#include "engineadapter.h"

#include "obj/character/role.h"

#include "servercommon/errornum.h"
#include "protocal/msgshenyi.hpp"

#include "scene/scene.h"
#include "xiannvshouhuconfig.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "servercommon/vipdef.hpp"
#include "other/xiannvshouhu/xiannvshouhufunction.hpp"
#include "globalconfig/globalconfig.h"
#include "other/camp/campconfig.hpp"
#include "other/shenyi/shenyiconfig.hpp"
#include "serverlogic.h"
#include "task/taskrecorder.h"
#include "other/capability/capability.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "servercommon/serverconfig/crossconfig.hpp"

XiannvShouhuManager::XiannvShouhuManager()
: m_role(NULL)
{

}

XiannvShouhuManager::~XiannvShouhuManager()
{

}

void XiannvShouhuManager::Init(Role *role, const XiannvShouhuParam &xiannvshouhu_param)
{
	m_role = role;
	m_xiannvshouhu_param = xiannvshouhu_param;
}

void XiannvShouhuManager::GetOtherInitParam(XiannvShouhuParam *param)
{
	*param = m_xiannvshouhu_param;
}

void XiannvShouhuManager::GetRoleAppearance(RoleAppearance *appearance)
{
	if (NULL != appearance)
	{
		appearance->xiannvshouhu_imageid = m_xiannvshouhu_param.used_imageid;
	}
}

void XiannvShouhuManager::SendXiannvShouhuInfo()
{
	Protocol::SCXiannvShouhuInfo mi;
	mi.star_level = m_xiannvshouhu_param.star_level;
	mi.grade = m_xiannvshouhu_param.grade;
	mi.grade_bless_val = m_xiannvshouhu_param.grade_bless_val;
	mi.used_imageid = m_xiannvshouhu_param.used_imageid;
	mi.active_image_flag = m_xiannvshouhu_param.active_image_flag;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&mi, sizeof(mi));
}

int XiannvShouhuManager::GetXiannvShouhuAppeID()
{
	return m_xiannvshouhu_param.used_imageid;
}

bool XiannvShouhuManager::ActiveImage(int imageid, bool recalc)
{
	if (imageid <= 0 || imageid >= XiannvShouhuParam::MAX_IMAGE_ID) return false;

	m_xiannvshouhu_param.ActiveImage(imageid);

	if (recalc)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_XIANNV_SHOUHU);
	}

	return true;
}

bool XiannvShouhuManager::UseImage(int imageid)
{

	if (!m_xiannvshouhu_param.IsImageActive(imageid))
	{
		m_role->NoticeNum(errornum::EN_SHENYI_IMG_NOT_ACTIVE);
		return false;
	}

	m_xiannvshouhu_param.used_imageid = imageid;

	m_role->RefreshApperance(true);

	this->SendXiannvShouhuInfo();

	return true;
}

bool XiannvShouhuManager::UnUseImage(int imageid)
{
	if (imageid != m_xiannvshouhu_param.used_imageid)
	{
		return false;
	}

	m_xiannvshouhu_param.used_imageid = 0;

	m_role->RefreshApperance(true);

	this->SendXiannvShouhuInfo();

	return true;
}

void XiannvShouhuManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (!m_xiannvshouhu_param.IsXiannvShouhuActived())
	{
		return;
	}

	if (is_recalc)
	{
		m_attrs_add.Reset();

		{
			// ÐÇ¼¶¼Ó³É  
			const ShenyiUpStarExpCfg * star_level_config = LOGIC_CONFIG->GetShenyiConfig().GetUpStarExpCfg(m_xiannvshouhu_param.star_level);
			if (NULL != star_level_config) 
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += star_level_config->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += star_level_config->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += star_level_config->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += star_level_config->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += star_level_config->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += star_level_config->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += star_level_config->jianren;
			}
		}
	}

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_XIANNV_SHOUHU, m_attrs_add);

	base_add.Add(m_attrs_add.m_attrs, true);
}

XiannvShouhuParam * XiannvShouhuManager::GetXiannvShouhuParam()
{
	return &m_xiannvshouhu_param;
}

void XiannvShouhuManager::PrintXiannvShouhu(const char *reason, int param1, int param2, int param3, int param4, int param5)
{
	gamelog::g_log_xiannv_shouhu.printf(LL_INFO, "XiannvShouhu role(%d,%s) %s(%d,%d,%d,%d,%d) "
		"grade:%d,%d, used_imageid:%d  image_flag:%d",
		m_role->GetUID(), m_role->GetName(),
		reason, param1, param2, param3, param4, param5,
		(int)m_xiannvshouhu_param.grade, (int)m_xiannvshouhu_param.grade_bless_val,
		(int)m_xiannvshouhu_param.used_imageid,
		(int)m_xiannvshouhu_param.active_image_flag);
}

void XiannvShouhuManager::OnCompleteTask(TaskID task_id)
{
	this->CheckActive();
 //	if (LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenTaskID(FUN_OPEN_TYPE_XIANNVSHOUHU) == task_id)
 //	{
 //		XiannvShouhuFunction::ActiveXiannvShouhu(m_role);
 //	}
}

void XiannvShouhuManager::OnRoleUpLevel(int level)
{
	this->CheckActive();
//	if (m_xiannvshouhu_param.IsXiannvShouhuActived()) return;
//
//	if (level >= LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenRoleLevel(FUN_OPEN_TYPE_XIANNVSHOUHU))
//	{
//		XiannvShouhuFunction::ActiveXiannvShouhu(m_role);
//	}
}

void XiannvShouhuManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->CheckActive();
	}
}

void XiannvShouhuManager::CheckActive()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (!m_xiannvshouhu_param.IsXiannvShouhuActived())
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_XIANNVSHOUHU);
		if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetLevel() >= fun_open_cfg->role_level || m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
			{
				XiannvShouhuFunction::ActiveXiannvShouhu(m_role);
			}
		}
	}
}