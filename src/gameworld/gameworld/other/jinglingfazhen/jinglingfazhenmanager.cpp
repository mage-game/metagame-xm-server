#include "jinglingfazhenmanager.hpp"

#include "gamelog.h"
#include "engineadapter.h"

#include "obj/character/role.h"

#include "servercommon/errornum.h"
#include "protocal/msgshenyi.hpp"

#include "scene/scene.h"
#include "jinglingfazhenconfig.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "servercommon/noticenum.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/vipdef.hpp"
#include "other/jinglingfazhen/jinglingfazhenfunction.hpp"
#include "globalconfig/globalconfig.h"
#include "other/camp/campconfig.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "serverlogic.h"
#include "task/taskrecorder.h"
#include "other/capability/capability.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "servercommon/serverconfig/crossconfig.hpp"

JinglingFazhenManager::JinglingFazhenManager()
: m_role(NULL)
{

}

JinglingFazhenManager::~JinglingFazhenManager()
{

}

void JinglingFazhenManager::Init(Role *role, const JinglingFazhenParam &xiannvshouhu_param)
{
	m_role = role;
	m_jinglingfazhen_param = xiannvshouhu_param;
}

void JinglingFazhenManager::GetOtherInitParam(JinglingFazhenParam *param)
{
	*param = m_jinglingfazhen_param;
}

void JinglingFazhenManager::OnLogin()
{
	this->CheckActive();
}

void JinglingFazhenManager::GetRoleAppearance(RoleAppearance *appearance)
{
	if (NULL != appearance)
	{
		appearance->jingling_fazhen_imageid = m_jinglingfazhen_param.used_imageid;
	}
}

void JinglingFazhenManager::SendJinglingFazhenInfo()
{
	Protocol::SCJinglingFazhenInfo mi;
	mi.reserve = 0;
	mi.grade = m_jinglingfazhen_param.grade;
	mi.grade_bless_val = m_jinglingfazhen_param.grade_bless_val;
	mi.used_imageid = m_jinglingfazhen_param.used_imageid;
	mi.reserve_2 = 0;
	mi.active_image_flag = m_jinglingfazhen_param.active_image_flag;
	mi.active_special_image_flag = m_jinglingfazhen_param.active_special_image_flag;
	memcpy(mi.special_img_grade_list, m_jinglingfazhen_param.special_img_grade_list, sizeof(mi.special_img_grade_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&mi, sizeof(mi));
}

int JinglingFazhenManager::GetJinglingFazhenAppeID()
{
	return m_jinglingfazhen_param.used_imageid;
}

bool JinglingFazhenManager::ActiveImage(int imageid, bool recalc)
{
	if (imageid <= 0 || imageid >= JinglingFazhenParam::MAX_IMAGE_ID) return false;

	m_jinglingfazhen_param.ActiveImage(imageid);

	if (recalc)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING_FAZHEN);
	}

	return true;
}

bool JinglingFazhenManager::UseImage(int imageid)
{

	if (imageid < SPECIAL_IMG_START_ID)
	{
		if (!m_jinglingfazhen_param.IsImageActive(imageid))
		{
			return false;
		}
	}
	else
	{
		if (!m_jinglingfazhen_param.IsSpecialImageActive(imageid - SPECIAL_IMG_START_ID))
		{
			return false;
		}
	}
	
	m_jinglingfazhen_param.used_imageid = imageid;

	m_role->RefreshApperance(true);

	this->SendJinglingFazhenInfo();

	return true;
}

bool JinglingFazhenManager::UnUseImage(int imageid)
{
	if (imageid != m_jinglingfazhen_param.used_imageid)
	{
		return false;
	}

	m_jinglingfazhen_param.used_imageid = 0;

	m_role->RefreshApperance(true);

	this->SendJinglingFazhenInfo();

	return true;
}

void JinglingFazhenManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (!m_jinglingfazhen_param.IsJinglingFazhenActived())
	{
		return;
	}

	if (is_recalc)
	{
		m_attrs_add.Reset();

		{
			// 阶数加成
			const JinglingFazhenGradeConfig * grade_config = LOGIC_CONFIG->GetJinglingFazhenCfg().GetGradeConfig(m_jinglingfazhen_param.grade);
			if (NULL != grade_config) 
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += grade_config->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += grade_config->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += grade_config->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += grade_config->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += grade_config->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += grade_config->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += grade_config->jianren;
			}
		}

		{
			// 特殊形象
			for (int i = 0; i <= JinglingFazhenParam::MAX_SPECIAL_IMAGE_ID; ++ i)
			{
				if (0 != (m_jinglingfazhen_param.active_special_image_flag & (1 << i)))
				{
					// 特殊形象进阶
					const int grade = m_jinglingfazhen_param.special_img_grade_list[i];

					const JinglingFazhenSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetJinglingFazhenCfg().GetSpecialImgUpgradeCfg(i, grade);
					if (NULL != upgrade_cfg)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += upgrade_cfg->maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += upgrade_cfg->gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += upgrade_cfg->fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += upgrade_cfg->mingzhong ;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += upgrade_cfg->shanbi;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += upgrade_cfg->baoji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += upgrade_cfg->jianren;
					}
				}
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_JINGLING_FAZHEN, m_attrs_add);

		//策划需求，精灵法阵的战力算入精灵战力中
		m_role->GetCapability()->SetCap(CAPABILITY_TYPE_JINGLING, m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING) + m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING_FAZHEN));
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

JinglingFazhenParam * JinglingFazhenManager::GetJinglingFazhenParam()
{
	return &m_jinglingfazhen_param;
}

void JinglingFazhenManager::PrintJinglingFazhen(const char *reason, int param1, int param2, int param3, int param4, int param5)
{
	gamelog::g_log_jingling_fazhen.printf(LL_INFO, "JinglingFazhen role(%d,%s) %s(%d,%d,%d,%d,%d) "
		"grade:%d,%d, used_imageid:%d  image_flag:%d",
		m_role->GetUID(), m_role->GetName(),
		reason, param1, param2, param3, param4, param5,
		(int)m_jinglingfazhen_param.grade, (int)m_jinglingfazhen_param.grade_bless_val,
		(int)m_jinglingfazhen_param.used_imageid,
		(int)m_jinglingfazhen_param.active_image_flag);
}

void JinglingFazhenManager::OnCompleteTask(TaskID task_id)
{
	this->CheckActive();
//	if (LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenTaskID(FUN_OPEN_TYPE_JINGLINGGUANGHUAN) == task_id)
//	{
//		JinglingFazhenFunction::ActiveJinglingFazhen(m_role);
//	}
}

void JinglingFazhenManager::OnRoleUpLevel(int level)
{
	this->CheckActive();
//	if (m_jinglingfazhen_param.IsJinglingFazhenActived()) return;
//
//	if (level >= LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenRoleLevel(FUN_OPEN_TYPE_JINGLINGFAZHEN))
//	{
//		JinglingFazhenFunction::ActiveJinglingFazhen(m_role);
//	}
}

bool JinglingFazhenManager::ActiveSpecialImage(int imageid, bool is_notify)
{
	if (0 != (m_jinglingfazhen_param.active_special_image_flag & (1 << imageid)))
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_SPECIAL_IMG_ALREADY_ACTIVE);
		}

		return false;
	}

	const JinglingFazhenSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetJinglingFazhenCfg().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	m_jinglingfazhen_param.active_special_image_flag |= (1 << imageid);
	++m_jinglingfazhen_param.special_img_grade_list[imageid];
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING_FAZHEN);
	this->UseImage(SPECIAL_IMG_START_ID + imageid);

	return true;
}

bool JinglingFazhenManager::UnActiveSpecialImage(int imageid)
{
	if (0 == (m_jinglingfazhen_param.active_special_image_flag & (1 << imageid)))
	{
		return false;
	}

	const JinglingFazhenSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetJinglingFazhenCfg().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	int temp_flag = ~(1 << imageid);
	m_jinglingfazhen_param.active_special_image_flag &= temp_flag;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);

	if (this->UnUseImage(SPECIAL_IMG_START_ID + imageid))
	{
		this->UseImage(1); //默认再给一个
	}

	return true;
}

void JinglingFazhenManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->CheckActive();
	}
}

void JinglingFazhenManager::CheckActive()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (!m_jinglingfazhen_param.IsJinglingFazhenActived())
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_JINGLINGFAZHEN);
		if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetLevel() >= fun_open_cfg->role_level || m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
			{
				JinglingFazhenFunction::ActiveJinglingFazhen(m_role);
			}
		}
	}
}