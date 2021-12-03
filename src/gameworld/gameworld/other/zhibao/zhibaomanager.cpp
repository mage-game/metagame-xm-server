#include "zhibaomanager.hpp"
#include "servercommon/configcommon.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/zhibao/zhibaoconfig.hpp"
#include "global/bossfamily/bossfamilyconfig.hpp"
#include "scene/scene.h"
#include "servercommon/namefilter.hpp"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/commondata.hpp"
#include "gameworld/world.h"
#include "protocal/msgother.h"
#include "other/event/eventhandler.hpp"
#include "gameworld/scene/scenemanager.h"
#include "other/welfare/welfare.hpp"
#include "serverlogic.h"
#include "effect/effectbase.hpp"
#include "protocal/msgequipment.h"
#include "other/capability/capability.hpp"
#include "item/knapsack.h"

ZhiBaoManager::ZhiBaoManager() : m_role(NULL), m_tianlei_hurt(0)
{
	memset(m_skill_level_list, 0, sizeof(m_skill_level_list));
}

ZhiBaoManager::~ZhiBaoManager()
{
}

void ZhiBaoManager::Init(Role *role, const ZhiBaoParam &zhibao_param)
{
	m_role = role;
	m_zhibao_param = zhibao_param;
	this->RefreshSkillLevel();
}

void ZhiBaoManager::GetInitParam(ZhiBaoParam *zhibao_param)
{
	*zhibao_param = m_zhibao_param;
}

void ZhiBaoManager::GetRoleAppearance(RoleAppearance *appearance)
{
	if (NULL != appearance)
	{
		appearance->zhibao_used_imageid = m_zhibao_param.use_image;
	}
}

void ZhiBaoManager::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		const ZhiBaoLevelCfg *level_cfg = LOGIC_CONFIG->GetZhiBaoConfig().GetZhiBaoLevelCfg(m_zhibao_param.level);
		if (NULL != level_cfg && m_zhibao_param.level > 0)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += level_cfg->baoji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += level_cfg->jianren;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg->mingzhong;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg->shanbi;
		}

		for (int huanhua_type = 0; huanhua_type < MAX_ZHIBAO_HUANHUA_TPYE_COUNT; huanhua_type++)
		{
			if (m_zhibao_param.grade[huanhua_type] > 0)
			{
				const ZhiBaoHuanhuaGradeCfg::GradeCfg *grade_cfg = LOGIC_CONFIG->GetZhiBaoConfig().GetHuanhuaGradeCfg(huanhua_type, m_zhibao_param.grade[huanhua_type]);
				if (NULL != grade_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += grade_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += grade_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += grade_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += grade_cfg->baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += grade_cfg->jianren;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += grade_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += grade_cfg->shanbi;
				}
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_ZHIBAO, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

int ZhiBaoManager::GetMountAddPer()
{
	const ZhiBaoLevelCfg *level_cfg = LOGIC_CONFIG->GetZhiBaoConfig().GetZhiBaoLevelCfg(m_zhibao_param.level);
	if (NULL != level_cfg)
	{
		return level_cfg->mount_attr_add;
	}

	return 0;
}

int ZhiBaoManager::GetWingAddPer()
{
	const ZhiBaoLevelCfg *level_cfg = LOGIC_CONFIG->GetZhiBaoConfig().GetZhiBaoLevelCfg(m_zhibao_param.level);
	if (NULL != level_cfg)
	{
		return level_cfg->wing_attr_add;
	}

	return 0;
}

void ZhiBaoManager::OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag)
{
	return;		// 策划要求屏蔽技能

	if (NULL == target)
	{
		return;
	}

	if (Obj::OBJ_TYPE_MONSTER == target->GetObjType())
	{
		CommonDataParam *common_p = m_role->GetCommonDataParam();
		if (common_p == NULL)
		{
			return;
		}

		const BossFamilyOtherCfg &bossfamily_othercfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetBossFamilyOtherCfg();

		const WearyCfg *miku_boss_weary_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetWearyCfg(0);//疲劳值类型weary_tpye，0是精英boss、1是活跃boss
		if (miku_boss_weary_cfg == NULL)
		{
			return;
		}

		const WearyCfg *active_boss_weary_cfg = LOGIC_CONFIG->GetBossFaimlyConfig().GetWearyCfg(1);//疲劳值类型weary_tpye，0是精英boss、1是活跃boss
		if (active_boss_weary_cfg == NULL)
		{
			return;
		}

		if (m_role->GetScene()->IsBossDongKuScene(m_role->GetScene()->GetSceneID()) && common_p->miku_boss_weary >= miku_boss_weary_cfg->weary_limit)
		{
			return;
		}

		if (m_role->GetScene()->IsDabaoBossScene(m_role->GetScene()->GetSceneID()) && common_p->dabao_angry_value >= bossfamily_othercfg.max_value)
		{
			return;
		}

		if (m_role->GetScene()->IsActiveBossScene(m_role->GetScene()->GetSceneID()) && common_p->active_boss_weary >= active_boss_weary_cfg->weary_limit)
		{
			return;
		}
	}

	UNSTD_STATIC_CHECK(MAX_ZHIBAO_SKILL_COUNT >= 4);
	if (m_skill_level_list[0] <= 0)
	{
		return;
	}

	short zhibao_skill_id = -1;
	short is_baoji = 0;
	if (0 == target_index)
	{
		m_tianlei_hurt = 0;

		// 天雷
		const ZhiBaoSkillCfg *skill0_cfg = LOGIC_CONFIG->GetZhiBaoConfig().GetZhiBaoSkillCfg(0, m_skill_level_list[0]);
		if (NULL != skill0_cfg)
		{
			unsigned long long now_time_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();
			if (now_time_ms < m_zhibao_param.last_skill_time)
			{
				return;
			}

			if (RandomNum(100) >= (skill0_cfg->param_b * ROLE_ATTR_PER) * 100)
			{
				return;
			}

			zhibao_skill_id = 0;
			m_zhibao_param.last_skill_time = now_time_ms + skill0_cfg->param_a;

			passive_flag |= (1 << EffectBase::PASSIVE_FLAG_ZHIBAO_TIANLEI);
			m_tianlei_hurt = skill0_cfg->param_c * m_zhibao_param.level;

			// 怒雷
			const ZhiBaoSkillCfg *skill2_cfg = LOGIC_CONFIG->GetZhiBaoConfig().GetZhiBaoSkillCfg(2, m_skill_level_list[2]);
			if (NULL != skill2_cfg)
			{
				if (RandomNum(100) < (skill2_cfg->param_a * ROLE_ATTR_PER) * 100)
				{
					is_baoji = 1;
					passive_flag |= (1 << EffectBase::PASSIVE_FLAG_ZHIBAO_NULEI);
					m_tianlei_hurt = (int)(m_tianlei_hurt * (1.0 + skill2_cfg->param_b * ROLE_ATTR_PER));
				}
			}

			// 雷暴
			const ZhiBaoSkillCfg *skill3_cfg = LOGIC_CONFIG->GetZhiBaoConfig().GetZhiBaoSkillCfg(3, m_skill_level_list[3]);
			if (NULL != skill3_cfg)
			{
				m_tianlei_hurt = (int)(m_tianlei_hurt * (1.0 + skill3_cfg->param_a * ROLE_ATTR_PER));
			}

			injure += m_tianlei_hurt;
		}
	}
	else
	{
		if (m_tianlei_hurt > 0)
		{
			// 雷阵
			const ZhiBaoSkillCfg *skill1_cfg = LOGIC_CONFIG->GetZhiBaoConfig().GetZhiBaoSkillCfg(1, m_skill_level_list[1]);
			if (NULL != skill1_cfg && target_index <= skill1_cfg->param_a)
			{
				zhibao_skill_id = 1;
				passive_flag |= (1 << EffectBase::PASSIVE_FLAG_ZHIBAO_TIANLEI);
				m_tianlei_hurt = (int)(m_tianlei_hurt * (1.0 + skill1_cfg->param_b * ROLE_ATTR_PER));
				injure += m_tianlei_hurt;
			}
		}
	}

	if (zhibao_skill_id >=0 && zhibao_skill_id <= 1)
	{
		static Protocol::SCZhiBaoAttack zba;
		zba.attacker_id = m_role->GetId();
		zba.target_id = target->GetId();
		zba.skill_idx = zhibao_skill_id;
		zba.is_baoji = is_baoji;
		zba.hurt = m_tianlei_hurt;

		m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(target->GetObserHandle(), (void*)&zba, sizeof(zba));
	}
}

void ZhiBaoManager::SendAllZhiBaoInfo()
{
	static Protocol::SCAllZhiBaoInfo pro;
	pro.level = m_zhibao_param.level;
	pro.use_image = m_zhibao_param.use_image;
	pro.exp = m_zhibao_param.exp;
	pro.reserve = 0;
	pro.grade_max = MAX_ZHIBAO_HUANHUA_TPYE_COUNT;
	
	memcpy(pro.grade, m_zhibao_param.grade, sizeof(pro.grade));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void ZhiBaoManager::AddZhiBaoExp(int add_exp)
{
	if (add_exp <= 0)
	{
		return;
	}

	m_zhibao_param.exp += add_exp;

	m_role->GetWelfare()->AddHappytreeGrowthvalue(GET_HAPPYTREE_VALUE_TYPE_ZHIBAO_EXP, add_exp);

	this->SendAllZhiBaoInfo();
	return;
}

bool ZhiBaoManager::UplevelZhiBao()
{
	const ZhiBaoLevelCfg *cur_level_cfg = LOGIC_CONFIG->GetZhiBaoConfig().GetZhiBaoLevelCfg(m_zhibao_param.level);
	if (NULL == cur_level_cfg)
	{
		m_role->NoticeNum(errornum::EN_ZHIBAO_LEVEL_TOO_LARGE);
		return false;
	}

	const ZhiBaoLevelCfg *next_level_cfg = LOGIC_CONFIG->GetZhiBaoConfig().GetZhiBaoLevelCfg(m_zhibao_param.level + 1);
	if (NULL == next_level_cfg)
	{
		m_role->NoticeNum(errornum::EN_ZHIBAO_LEVEL_TOO_LARGE);
		return false;
	}

	if (m_zhibao_param.exp < cur_level_cfg->upleve_exp)
	{
		return false;
	}

	m_zhibao_param.exp -= cur_level_cfg->upleve_exp;
	++ m_zhibao_param.level;

	this->RefreshSkillLevel();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHIBAO);

	if (cur_level_cfg->image_id < next_level_cfg->image_id)
	{
		this->UseImage(next_level_cfg->image_id);
	}

	this->SendAllZhiBaoInfo();
	
	EventHandler::Instance().OnUplevelZhiBao(m_role, m_zhibao_param.level);

	return true;
}

//至宝形象
void ZhiBaoManager::UseImage(int image)
{
	if (image < 0 || image > MAX_ZHIBAO_IMAGE_ID)
	{
		return;
	}

	const ZhiBaoLevelCfg *level_cfg = LOGIC_CONFIG->GetZhiBaoConfig().GetZhiBaoLevelCfg(m_zhibao_param.level);
	if (NULL == level_cfg || m_zhibao_param.level <= 0)
	{
		return;
	}

	if (level_cfg->image_id < image)
	{
		return;
	}

	m_zhibao_param.use_image = image;

	m_role->RefreshApperance(true);

	this->SendAllZhiBaoInfo();
}

// 幻化形象激活或升阶
void ZhiBaoManager::ActiveOrUpgradeHuanhua(int type)
{
	if (type < 0 || type >= MAX_ZHIBAO_HUANHUA_TPYE_COUNT)
	{
		return;
	}

	Knapsack * knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	ZhiBaoHuanhuaGradeCfg::GradeCfg *grade_cfg = LOGIC_CONFIG->GetZhiBaoConfig().GetHuanhuaGradeCfg(type, m_zhibao_param.grade[type]);
	if (NULL == grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_ZHIBAO_HUANHUA_GARDE_MAX);
		return;
	}

	ZhiBaoHuanhuaGradeCfg::GradeCfg *next_grade_cfg = LOGIC_CONFIG->GetZhiBaoConfig().GetHuanhuaGradeCfg(type, m_zhibao_param.grade[type] + 1);
	if (NULL == next_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_ZHIBAO_HUANHUA_GARDE_MAX);
		return;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	{
		// 获取可消耗的背包材料列表
		short stuff_count = 0;

		// 材料1
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id =grade_cfg->stuff_id;
		stuff_list[stuff_count].num = grade_cfg->stuff_count;
		stuff_list[stuff_count].buyable = false;
		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list,true))
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}
	}

	if (!ItemExtern::ConsumeItem(m_role, consume_list, "zhibaohuanhua"))
	{
		return;
	}

	m_zhibao_param.grade[type] += 1;

	if (1 == m_zhibao_param.grade[type])
	{
		m_zhibao_param.ActiveHuanHua(type);
		m_role->NoticeNum(noticenum::NT_ZHIBAO_HUANHUA_JIHUO_SUCC);
	}
	else
	{
		m_role->NoticeNum(noticenum::NT_ZHIBAO_HUANHUA_GARDE_UPLEVEL_SUCC);
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHIBAO);

	this->SendAllZhiBaoInfo();
}

void ZhiBaoManager::UseHuanhuaType(int type)
{
	if (type < 0 || type >= MAX_ZHIBAO_HUANHUA_TPYE_COUNT)
	{
		return;
	}

	if (m_zhibao_param.grade[type] <= 0)
	{
		return;
	}

	m_zhibao_param.use_image = type + ZHIBAO_SPECIAL_IMG_START_ID;

	m_role->RefreshApperance(true);

	this->SendAllZhiBaoInfo();	
}

bool ZhiBaoManager::ActiveHuanHua(int huanhua_id)
{
	const ZhiBaoHuanhuaCfg::ConfigItem* huanhua_cfg = LOGIC_CONFIG->GetZhiBaoConfig().GetHuanHuaCfg(huanhua_id);
	if (NULL == huanhua_cfg)
	{
		return false;
	}

	if (m_zhibao_param.IsHuanHuaActive(huanhua_id))
	{
		m_role->NoticeNum(errornum::EN_ZHIBAO_ALREADY_ACTIVE);
		return false;
	}

	m_zhibao_param.ActiveHuanHua(huanhua_id);
	m_zhibao_param.grade[huanhua_id] = 1;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_ZHIBAO);
	this->SendAllZhiBaoInfo();
	m_role->NoticeNum(noticenum::NT_SHENWU_HUANHUA_SUCCESS);
	return true;
}

void ZhiBaoManager::RefreshSkillLevel()
{
	memset(m_skill_level_list, 0, sizeof(m_skill_level_list));

	for (int i = 0; i < MAX_ZHIBAO_SKILL_COUNT; i++)
	{
		for (int k = 1; k <= MAX_ZHIBAO_SKILL_LEVEL; k++)
		{
			const ZhiBaoSkillCfg *cfg = LOGIC_CONFIG->GetZhiBaoConfig().GetZhiBaoSkillCfg(i, k);
			if (NULL != cfg)
			{
				if (m_zhibao_param.level >= cfg->zhibao_level)
				{
					m_skill_level_list[i] = k;
				}
			}
		}
	}
}
