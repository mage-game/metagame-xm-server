#include "upgrademanager.hpp"
#include "gamelog.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"

#include "protocal/msgrole.h"
#include "protocal/msgupgrade.hpp"

#include "scene/scene.h"
#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/upgradeconfig/upgradeconfigmanager.hpp"
#include "serverlogic.h"
#include "config/shuxingdanconfig.hpp"
#include "task/taskrecorder.h"
#include "other/capability/capability.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "other/dressingroom/dressingroomconfig.hpp"
#include "other/dressingroom/dressingroommgr.hpp"

#include<algorithm>

UpgradeManager::UpgradeManager()
	: m_role(nullptr)
{
	memset(m_upgrade_list, 0, sizeof(m_upgrade_list));

	for (int upgrade_type = 0; upgrade_type < UPGRADE_TYPE_MAX; upgrade_type++)
	{
		bool is_use_template = LOGIC_CONFIG->GetUpgradeCfgManager().IsUseUpgradeTemplate(upgrade_type);
		if (is_use_template)
		{
			m_upgrade_list[upgrade_type] = new Upgrade(this, upgrade_type);
		}
	}
}

UpgradeManager::~UpgradeManager()
{
	for (int upgrade_type = 0; upgrade_type < UPGRADE_TYPE_MAX; upgrade_type++)
	{
		if (m_upgrade_list[upgrade_type])
		{
			delete m_upgrade_list[upgrade_type];
			m_upgrade_list[upgrade_type] = nullptr;
		}
	}
}

void UpgradeManager::Init(Role *role, const UpgradeManagerParam &param)
{
	m_role = role;	

	for (int upgrade_type = 0; upgrade_type < UPGRADE_TYPE_MAX; upgrade_type ++)
	{
		if (nullptr != m_upgrade_list[upgrade_type])
		{
			m_upgrade_list[upgrade_type]->Init(role, param.data_list[upgrade_type]);
		}
	}

	// 进阶技能需获取进阶系统数据，所以初始化放后面
	m_upgrade_skill.Init(role, this);
}

void UpgradeManager::GetInitParam(UpgradeManagerParam *param)
{
	if (nullptr == param)
	{
		return;
	}

	for (int upgrade_type = 0; upgrade_type < UPGRADE_TYPE_MAX; upgrade_type++)
	{
		if (nullptr != m_upgrade_list[upgrade_type])
		{
			m_upgrade_list[upgrade_type]->GetInitParam(&param->data_list[upgrade_type]);
		}
	}
}

void UpgradeManager::OnInitFinish()
{
	m_upgrade_skill.OnInitFinish();
}

void UpgradeManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	for (int upgrade_type = 0; upgrade_type < UPGRADE_TYPE_MAX; upgrade_type++)
	{
		if (nullptr != m_upgrade_list[upgrade_type])
		{
			m_upgrade_list[upgrade_type]->OnDayChange(old_dayid, now_dayid);
		}
	}
}

void UpgradeManager::Update(time_t now_second)
{
	for (int upgrade_type = 0; upgrade_type < UPGRADE_TYPE_MAX; upgrade_type++)
	{
		if (nullptr != m_upgrade_list[upgrade_type])
		{
			m_upgrade_list[upgrade_type]->Update(now_second);
		}
	}
}

void UpgradeManager::ReCalcAttr(CharIntAttrs &base_add, int recalc_reason, bool recalc_all)
{
	int upgrade_type = UPGRADE_TYPE_INVALID;
	if (recalc_reason >= CharIntAttrs::RECALC_REASON_UPGRADE_SYS_BEGIN)
	{
		upgrade_type = recalc_reason - CharIntAttrs::RECALC_REASON_UPGRADE_SYS_BEGIN;
	}

	for (int type = 0; type < UPGRADE_TYPE_MAX; type++)
	{
		if (nullptr != m_upgrade_list[type])
		{
			m_upgrade_list[type]->ReCalcAttr(base_add, (type == upgrade_type) || recalc_all);
		}
	}
}

bool UpgradeManager::IsCanOpera(int upgrade_type, int opera_type)
{
	bool is_can_opera = true;
	int module_type = GetOperaModuleType(opera_type);
	if (module_type > UPGRADE_MODULE_TYPE_INVALID)
	{
		is_can_opera = LOGIC_CONFIG->GetUpgradeCfgManager().IsUpgradeModuleOpen(upgrade_type, module_type);
	}

	if (is_can_opera)
	{
		if (nullptr == m_upgrade_list[upgrade_type] || (m_upgrade_list[upgrade_type]->GetUpgradeType() != upgrade_type))
		{
			is_can_opera = false;
		}
	}

	return is_can_opera;
}

void UpgradeManager::OnRoleOpera(Protocol::CSUpgradeOperaReq *upr)
{
	if (nullptr == upr)
	{
		return;
	}

	// 判断能否操作
	if (!IsCanOpera(upr->upgrade_type, upr->opera_type))
	{
		return;
	}

	switch (upr->opera_type)
	{
	case Protocol::UPGRADE_OPERA_TYPE_INFO:
	{
		m_upgrade_list[upr->upgrade_type]->SendUpgradeInfo();
	}
	break;

	case Protocol::UPGRADE_OPERA_TYPE_USE_IMAGE:
	{
		if (1 == upr->param_1)
		{
			m_upgrade_list[upr->upgrade_type]->UseTemporaryImage(upr->param_2);
		}
		else
		{
			m_upgrade_list[upr->upgrade_type]->UseImage(upr->param_2);
		}
	}
	break;

	case Protocol::UPGRADE_OPERA_TYPE_FIGHT_OUT:
	{
		m_upgrade_list[upr->upgrade_type]->OnFightOut(1 == upr->param_1);
	}
	break;

	case Protocol::UPGRADE_OPERA_TYPE_UPGRADE:
	{
		bool ret = m_upgrade_list[upr->upgrade_type]->OnUpgrade(1 == upr->param_2, upr->param_1);
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_UPGRADE_SYS_UPGRADE + upr->upgrade_type, ret);
		/*for (int i = 0; i < upr->param_1; i ++)
		{
			int old_grade = this->GetGrade(upr->upgrade_type);
			bool ret = m_upgrade_list[upr->upgrade_type]->OnUpgrade(1 == upr->param_2);

			if (!ret || old_grade != this->GetGrade(upr->upgrade_type))
			{
				m_role->SendOperateResult(Protocol::SCOperateResult::OP_UPGRADE_SYS_UPGRADE + upr->upgrade_type, 0);
				return;
			}
		}

		if (upr->param_1 > 0)
		{
			m_role->SendOperateResult(Protocol::SCOperateResult::OP_UPGRADE_SYS_UPGRADE + upr->upgrade_type, 1);
		}*/
	}
	break;

	case Protocol::UPGRADE_OPERA_TYPE_IMAGE_UPGRADE:
	{
		m_upgrade_list[upr->upgrade_type]->CheckCanImgUpgrade(upr->param_1);
	}
	break;

	case Protocol::UPGRADE_OPERA_TYPE_SKILL_UPGRADE:
	{
		m_upgrade_list[upr->upgrade_type]->CheckCanUpLevelSkill(upr->param_1, 1 == upr->param_2);
	}
	break;

	case Protocol::UPGRADE_OPERA_TYPE_EQUIP_UPGRADE:
	{
		m_upgrade_list[upr->upgrade_type]->CheckCanUplevelEquip(upr->param_1);
	}
	break;

	}
}

int UpgradeManager::GetOperaModuleType(int opera_type)
{
	int module_type = UPGRADE_MODULE_TYPE_INVALID;

	struct UpgradeOperaTypeItem
	{
		int opera_type;
		int module_type;
	};
	static const std::vector<UpgradeOperaTypeItem> module_items = {
		{ Protocol::UPGRADE_OPERA_TYPE_USE_IMAGE, UPGRADE_MODULE_TYPE_IMAGE },
		{ Protocol::UPGRADE_OPERA_TYPE_FIGHT_OUT, UPGRADE_MODULE_TYPE_IMAGE },
		{ Protocol::UPGRADE_OPERA_TYPE_UPGRADE, UPGRADE_MODULE_TYPE_UPGRADE },
		{ Protocol::UPGRADE_OPERA_TYPE_IMAGE_UPGRADE, UPGRADE_MODULE_TYPE_IMAGE },
		{ Protocol::UPGRADE_OPERA_TYPE_SKILL_UPGRADE, UPGRADE_MODULE_TYPE_SKILL },
		{ Protocol::UPGRADE_OPERA_TYPE_EQUIP_UPGRADE, UPGRADE_MODULE_TYPE_EQUIP },
	};

	auto it = std::find_if(module_items.cbegin(), module_items.cend(), [opera_type](const UpgradeOperaTypeItem &cmp_item) {
		if (opera_type == cmp_item.opera_type)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != module_items.end())
	{
		module_type = it->module_type;
	}

	return module_type;
}

bool UpgradeManager::IsValidUpgradeType(int upgrade_type)
{
	if (upgrade_type <= UPGRADE_TYPE_INVALID || upgrade_type >= UPGRADE_TYPE_MAX)
	{
		return false;
	}

	if (nullptr == m_upgrade_list[upgrade_type])
	{
		return false;
	}

	return true;
}

void UpgradeManager::OnLogin()
{
	for (int upgrade_type = 0; upgrade_type < UPGRADE_TYPE_MAX; upgrade_type++)
	{
		if (nullptr != m_upgrade_list[upgrade_type])
		{
			m_upgrade_list[upgrade_type]->OnLogin();
		}
	}
}

void UpgradeManager::OnCompleteTask(TaskID task_id)
{
	for (int upgrade_type = 0; upgrade_type < UPGRADE_TYPE_MAX; upgrade_type++)
	{
		if (nullptr != m_upgrade_list[upgrade_type])
		{
			m_upgrade_list[upgrade_type]->OnCompleteTask(task_id);
		}
	}
}

void UpgradeManager::OnRoleUpLevel(int level)
{
	for (int upgrade_type = 0; upgrade_type < UPGRADE_TYPE_MAX; upgrade_type++)
	{
		if (nullptr != m_upgrade_list[upgrade_type])
		{
			m_upgrade_list[upgrade_type]->OnRoleUpLevel(level);
		}
	}
}

bool UpgradeManager::OnUseShuxingdan(int upgrade_type, int slot_idx, short &use_count)
{
	if (!IsValidUpgradeType(upgrade_type))
	{
		return false;
	}

	bool is_succ = m_upgrade_list[upgrade_type]->OnUseShuxingdan(slot_idx, use_count);

	return is_succ;
}

bool UpgradeManager::UpOneGrade(int upgrade_type, int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num)
{
	if (!IsValidUpgradeType(upgrade_type))
	{
		return false;
	}

	return m_upgrade_list[upgrade_type]->CheckCanUpOneGrade(valid_min_grade, valid_max_grade, return_item_id, return_item_num);
}

Upgrade * UpgradeManager::GetUpgrade(int upgrade_type)
{
	if (!IsValidUpgradeType(upgrade_type))
	{
		return nullptr;
	}

	return m_upgrade_list[upgrade_type];
}

void UpgradeManager::GetBaseAttr(int upgrade_type, CharIntAttrs &tmp_attr)
{
	if (!IsValidUpgradeType(upgrade_type))
	{
		return;
	}

	m_upgrade_list[upgrade_type]->GetBaseAttr(tmp_attr);
}

void UpgradeManager::GetRoleAppearance(RoleAppearance *appearance)
{
	if (nullptr == appearance)
	{
		return;
	}

	appearance->upgrade_used_imageid_0 = GetUseImgID(UPGRADE_TYPE_0);
	appearance->upgrade_used_imageid_1 = GetUseImgID(UPGRADE_TYPE_1);
	appearance->upgrade_used_imageid_2 = GetUseImgID(UPGRADE_TYPE_2);
	appearance->upgrade_used_imageid_3 = GetUseImgID(UPGRADE_TYPE_3);
	appearance->upgrade_used_imageid_4 = GetUseImgID(UPGRADE_TYPE_4);
	appearance->upgrade_used_imageid_5 = GetUseImgID(UPGRADE_TYPE_5);
	appearance->upgrade_used_imageid_6 = GetUseImgID(UPGRADE_TYPE_6);
	appearance->upgrade_used_imageid_7 = GetUseImgID(UPGRADE_TYPE_7);
	appearance->upgrade_used_imageid_8 = GetUseImgID(UPGRADE_TYPE_8);
}

bool UpgradeManager::IsImageActive(int upgrade_type, int image_id)
{
	if (!IsValidUpgradeType(upgrade_type))
	{
		return false;
	}

	return m_upgrade_list[upgrade_type]->IsImageActive(image_id);
}

bool UpgradeManager::ActiveImage(int upgrade_type, int image_id)
{
	if (!IsValidUpgradeType(upgrade_type))
	{
		return false;
	}

	if (!m_upgrade_list[upgrade_type]->ActiveImage(image_id))
	{
		return false;
	}

	switch (upgrade_type)
	{

	case UPGRADE_TYPE_0:
	{
		m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_LINGZHU, image_id, true);
	}
	break;

	case UPGRADE_TYPE_1:
	{
		m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_XIANBAO, image_id, true);
	}
	break;

	case UPGRADE_TYPE_2:
	{
		m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_LINGTONG, image_id, true);
	}
	break;

	case UPGRADE_TYPE_3:
	{
		m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_LINGGONG, image_id, true);
	}
	break;

	case UPGRADE_TYPE_4:
	{
		m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_LINGQI, image_id, true);
	}
	break;

	case UPGRADE_TYPE_5:
	{
		m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_WEIYAN, image_id, true);
	}
	break;

	case UPGRADE_TYPE_6:
	{
		m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_SHOUHUAN, image_id, true);
	}
	break;

	case UPGRADE_TYPE_7:
	{
		m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_TAIL, image_id, true);
	}
	break;

	case UPGRADE_TYPE_8:
	{
		m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_FLYPET, image_id, true);
	}
	break;

	default:
		break;
	}

	return true;
}

bool UpgradeManager::OnImgUpgrade(int upgrade_type, int image_id)
{
	if (!IsValidUpgradeType(upgrade_type))
	{
		return false;
	}

	return m_upgrade_list[upgrade_type]->CheckCanImgUpgrade(image_id);
}

short UpgradeManager::GetUseImgID(int upgrade_type)
{
	if (!IsValidUpgradeType(upgrade_type))
	{
		return 0;
	}

	return m_upgrade_list[upgrade_type]->GetUpgradeAppeID();
}

bool UpgradeManager::IsSpecialImg(int upgrade_type, int img_id)
{
	if (!IsValidUpgradeType(upgrade_type))
	{
		return false;
	}

	bool is_special = m_upgrade_list[upgrade_type]->IsSpecialImg(img_id);

	return is_special;
}

int UpgradeManager::GetGrade(int upgrade_type)
{
	if (!IsValidUpgradeType(upgrade_type))
	{
		return 0;
	}

	int grade = m_upgrade_list[upgrade_type]->GetGrade();

	return grade;
}

int UpgradeManager::GetSkill0Level(int upgrade_type)
{
	if (!IsValidUpgradeType(upgrade_type))
	{
		return 0;
	}

	int skill_level = m_upgrade_list[upgrade_type]->GetSkill0Level();

	return skill_level;
}

int UpgradeManager::GetSpecialImageLevelById(int upgrade_type, int imageid)
{
	if (!IsValidUpgradeType(upgrade_type))
	{
		return 0;
	}

	if (!IsSpecialImg(upgrade_type, imageid))
	{
		return 0;
	}

	if (!IsImageActive(upgrade_type, imageid))
	{
		return 0;
	}

	return m_upgrade_list[upgrade_type]->GetImageLevel(imageid);
}