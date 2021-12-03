#include "jinglingguanghuanmanager.hpp"

#include "gamelog.h"
#include "engineadapter.h"

#include "obj/character/role.h"

#include "servercommon/errornum.h"
#include "protocal/msgjinglingguanghuan.hpp"

#include "scene/scene.h"
#include "jinglingguanghuanconfig.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "servercommon/noticenum.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/vipdef.hpp"
#include "other/jinglingguanghuan/jinglingguanghuanfunction.hpp"
#include "globalconfig/globalconfig.h"
#include "other/camp/campconfig.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "serverlogic.h"
#include "task/taskrecorder.h"
#include "other/capability/capability.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "servercommon/serverconfig/crossconfig.hpp"

JinglingGuanghuanManager::JinglingGuanghuanManager()
: m_role(NULL)
{

}

JinglingGuanghuanManager::~JinglingGuanghuanManager()
{

}

void JinglingGuanghuanManager::Init(Role *role, const JinglingGuanghuanParam &jinglingguanghuan_param)
{
	m_role = role;
	m_jinglingguanghuan_param = jinglingguanghuan_param;
}

void JinglingGuanghuanManager::GetOtherInitParam(JinglingGuanghuanParam *param)
{
	*param = m_jinglingguanghuan_param;
}

void JinglingGuanghuanManager::GetRoleAppearance(RoleAppearance *appearance)
{
	if (NULL != appearance)
	{
		appearance->jingling_guanghuan_imageid = m_jinglingguanghuan_param.used_imageid;
	}
}

void JinglingGuanghuanManager::SendJinglingGuanghuanInfo()
{
	Protocol::SCJinglingGuanghuanInfo mi;
	mi.reserve = 0;
	mi.grade = m_jinglingguanghuan_param.grade;
	mi.grade_bless_val = m_jinglingguanghuan_param.grade_bless_val;
	mi.used_imageid = m_jinglingguanghuan_param.used_imageid;
	mi.reserve_2 = 0;
	mi.active_image_flag = m_jinglingguanghuan_param.active_image_flag;
	mi.active_special_image_flag = m_jinglingguanghuan_param.active_special_image_flag;
	memcpy(mi.special_img_grade_list, m_jinglingguanghuan_param.special_img_grade_list, sizeof(mi.special_img_grade_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&mi, sizeof(mi));
}

int JinglingGuanghuanManager::GetJinglingGuanghuanAppeID()
{
	return m_jinglingguanghuan_param.used_imageid;
}

bool JinglingGuanghuanManager::ActiveImage(int imageid, bool recalc)
{
	if (imageid <= 0 || imageid >= JinglingGuanghuanParam::MAX_IMAGE_ID) return false;

	m_jinglingguanghuan_param.ActiveImage(imageid);

	if (recalc)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING_GUANGHUAN);
	}

	return true;
}

bool JinglingGuanghuanManager::UseImage(int imageid)
{
	if (imageid < SPECIAL_IMG_START_ID)
	{
		if (!m_jinglingguanghuan_param.IsImageActive(imageid))
		{
			return false;
		}
	}
	else
	{
		if (!m_jinglingguanghuan_param.IsSpecialImageActive(imageid - SPECIAL_IMG_START_ID))
		{
			return false;
		}
	}

	m_jinglingguanghuan_param.used_imageid = imageid;

	m_role->RefreshApperance(true);

	this->SendJinglingGuanghuanInfo();

	return true;
}

bool JinglingGuanghuanManager::UnUseImage(int imageid)
{
	if (imageid != m_jinglingguanghuan_param.used_imageid)
	{
		return false;
	}

	m_jinglingguanghuan_param.used_imageid = 0;

	m_role->RefreshApperance(true);

	this->SendJinglingGuanghuanInfo();

	return true;
}

void JinglingGuanghuanManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (!m_jinglingguanghuan_param.IsJinglingGuanghuanActived())
	{
		return;
	}

	if (is_recalc)
	{
		m_attrs_add.Reset();

		{
			// 阶数加成
			const JinglingGuanghuanGradeConfig * grade_config = LOGIC_CONFIG->GetJinglingGuanghuanCfg().GetGradeConfig(m_jinglingguanghuan_param.grade);
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
			for (int i = 0; i <= JinglingGuanghuanParam::MAX_SPECIAL_IMAGE_ID; ++ i)
			{
				if (0 != (m_jinglingguanghuan_param.active_special_image_flag & (1 << i)))
				{
					// 特殊形象进阶
					const int grade = m_jinglingguanghuan_param.special_img_grade_list[i];

					const JinglingGuanghuanSpecialImgUpgradeCfg *upgrade_cfg = LOGIC_CONFIG->GetJinglingGuanghuanCfg().GetSpecialImgUpgradeCfg(i, grade);
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


		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_JINGLING_GUANGHUAN, m_attrs_add);

		//策划需求，精灵光环的战力算入精灵战力中
		//m_role->GetCapability()->SetCap(CAPABILITY_TYPE_JINGLING, m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING) + m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING_GUANGHUAN));
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

JinglingGuanghuanParam * JinglingGuanghuanManager::GetJinglingGuanghuanParam()
{
	return &m_jinglingguanghuan_param;
}

void JinglingGuanghuanManager::PrintJinglingGuanghuan(const char *reason, int param1, int param2, int param3, int param4, int param5)
{
	gamelog::g_log_jingling_guanghuan.printf(LL_INFO, "JinglingGuanghuan role(%d,%s) %s(%d,%d,%d,%d,%d) "
		"grade:%d,%d, used_imageid:%d  image_flag:%d",
		m_role->GetUID(), m_role->GetName(),
		reason, param1, param2, param3, param4, param5,
		(int)m_jinglingguanghuan_param.grade, (int)m_jinglingguanghuan_param.grade_bless_val,
		(int)m_jinglingguanghuan_param.used_imageid,
		(int)m_jinglingguanghuan_param.active_image_flag);
}

void JinglingGuanghuanManager::OnCompleteTask(TaskID task_id)
{
	//this->CheckActive();
}

void JinglingGuanghuanManager::OnRoleUpLevel(int level)
{
	//this->CheckActive();
}

void JinglingGuanghuanManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	/*if (old_dayid != now_dayid)
	{
		this->CheckActive();
	}*/
}

void JinglingGuanghuanManager::CheckActive()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	if (!m_jinglingguanghuan_param.IsJinglingGuanghuanActived())
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;

		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_JINGLINGGUANGHUAN);
		if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
			{
				JinglingGuanghuanFunction::ActiveJinglingGuanghuan(m_role);
			}
		}
	}
}

bool JinglingGuanghuanManager::ActiveSpecialImage(int imageid, bool is_notify)
{
	if (0 != (m_jinglingguanghuan_param.active_special_image_flag & (1 << imageid)))
	{
		if (is_notify)
		{
			m_role->NoticeNum(errornum::EN_SPECIAL_IMG_ALREADY_ACTIVE);
		}

		return false;
	}

	const JinglingGuanghuanSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetJinglingGuanghuanCfg().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	m_jinglingguanghuan_param.active_special_image_flag |= (1 << imageid);
	++m_jinglingguanghuan_param.special_img_grade_list[imageid];
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING_GUANGHUAN);
	this->UseImage(SPECIAL_IMG_START_ID + imageid);
	return true;
}

bool JinglingGuanghuanManager::UnActiveSpecialImage(int imageid)
{
	if (0 == (m_jinglingguanghuan_param.active_special_image_flag & (1 << imageid)))
	{
		return false;
	}

	const JinglingGuanghuanSpecialImgCfg *cfg_item = LOGIC_CONFIG->GetJinglingGuanghuanCfg().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	int temp_flag = ~(1 << imageid);
	m_jinglingguanghuan_param.active_special_image_flag &= temp_flag;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_JINGLING_GUANGHUAN);

	if (this->UnUseImage(SPECIAL_IMG_START_ID + imageid))
	{
		this->UseImage(1); //默认再给一个
	}

	return true;
}