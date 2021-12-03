#include "shizhuang.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/shizhuang/shizhuangconfig.h"
#include "config/activityconfig/gongchengzhanconfig.hpp"
#include "item/itemextern.hpp"
#include "servercommon/errornum.h"
#include "servercommon/serverconfig/cross1v1commonconfig.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "other/cross/rolecross.h"
#include "protocal/msgother.h"
#include "other/achieve/title/titleconfig.hpp"
#include "servercommon/serverconfig/crossguildbattleconfig.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"
#include "servercommon/string/crossstr.h"
#include "servercommon/string/gameworldstr.h"
#include "serverlogic.h"
#include "world.h"
#include "gstr.h"
#include "other/event/eventhandler.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/shuxingdanconfig.hpp"
#include "other/imgfuling/imgfuling.hpp"
#include "servercommon/imgfulingdef.hpp"
#include "other/upgradeskill/upgradeskillconfig.hpp"
#include "other/talent/talent.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "global/usercache/usercache.hpp"
#include "gameworld/config/sharedconfig/sharedconfig.h"
#include "gameworld/task/taskrecorder.h"
#include "other/jinjiesys/jinjiesysfunchelper.hpp"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
#include "other/dressingroom/dressingroomconfig.hpp"
#include "other/dressingroom/dressingroommgr.hpp"
#include "other/shenqi/shenqi.hpp"
#include "servercommon/serverconfig/crossrankrewardconfig.hpp"
#include "global/worldstatus3/worldstatus3.hpp"
#include "global/globalrole/qingyuanglobal/qingyuanglobal.hpp"
#include "other/route/mailroute.hpp"
#include "servercommon/activitydef.hpp"
#include "other/upgradesyscrit/upgradesyscrit.hpp"
#include "common/bintohex.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/xianzunka/xianzunka.hpp"
#include "other/tianshuxunzhu/tianshuxunzhuconfig.hpp"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"

Shizhuang::Shizhuang() : m_role(NULL), m_body_skill0_last_colddown_time(0), m_wuqi_skill0_last_colddown_time(0)
{
	memset(m_other_capability, 0, sizeof(m_other_capability));
}

Shizhuang::~Shizhuang()
{

}

void Shizhuang::InitParam(Role *role, const ShizhuangParam &param)
{
	m_role = role;
	m_param = param;
	/*for (int i = 0; i < SHIZHUANG_TYPE_MAX; i++)
	{
		if (m_param.item_list[i].img_active_flag == 0)
		{
			if (m_role->GetLevel() >= LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenRoleLevel(FUN_OPEN_TYPE_SHIZHUANG))
			{
				ActiveShiZhuang(i, false);
			}
		}
	}*/
}

void Shizhuang::GetInitParam(ShizhuangParam *param)
{
	*param = m_param;
}

void Shizhuang::OnWeekChange()
{
	bool need_notify = false;

	for (int shizhuan_type = 0; shizhuan_type < SHIZHUANG_TYPE_MAX; ++shizhuan_type)
	{
		if (this->IsActive(shizhuan_type, SHIZHUANG_CROSS_RANK_REWARD_INDEX))
		{
			BitmapFunction::ClearBit(m_param.item_list[shizhuan_type].special_img_active_flag, sizeof(m_param.item_list[shizhuan_type].special_img_active_flag), SHIZHUANG_CROSS_RANK_REWARD_INDEX);
			if (SHIZHUANG_CROSS_RANK_REWARD_INDEX == m_param.item_list[shizhuan_type].use_special_img)
			{
				m_param.item_list[shizhuan_type].use_special_img = 0;
			}

			need_notify = true;
		}
	}

	if (need_notify)
	{
		this->SendInfo();
		m_role->RefreshApperance(true);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG);
	}
}

void Shizhuang::OnLogin()
{
	this->CheckActive();
	this->CheckActiveByCondition();
}

void Shizhuang::Update(unsigned int now_second)
{
	for (int i = 0; i < SHIZHUANG_TYPE_MAX; ++i)
	{
		ShizhuangItem &item = m_param.item_list[i];
		const ShizhuangGradeConfig *grade_cfg;
		grade_cfg = (i == SHIZHUANG_TYPE_WUQI) ? LOGIC_CONFIG->GetShizhuangConfig().GetWeaponGradeConfig(item.grade) : LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangGradeConfig(item.grade);
			
		if (nullptr != grade_cfg && 1 == grade_cfg->is_clear_bless && now_second >= item.clear_bless_value_time)
		{
			item.grade_bless= 0;
			item.clear_bless_value_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + EngineAdapter::Instance().NextDayInterval(NEXTDAY_CLEAR_BLESS_VAL_TIME, 0, 0);
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG); //清除临时属性
			this->SendInfo();
		}

		for (int j = 1; j < SHIZHUANG_MAX_IMG_COUNT && j < SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT; ++j)
		{
			const ShizhuangSpecialImgCfg* img_cfg = (i == SHIZHUANG_TYPE_WUQI) ? LOGIC_CONFIG->GetShizhuangConfig().GetWeaponSpecialImgCfg(j) : LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangSpecialImgCfg(j);
			
			if (img_cfg == NULL) continue;
	
			if (item.valid_timestamp_list[j] > 0 && now_second >= item.valid_timestamp_list[j] && img_cfg->valid_time_s >0 )
			{
				this->UnActiveSpecialImg(i, j);
			}
			
		}
	}

	if(!CrossConfig::Instance().IsHiddenServer())
	{
		int cross_guildbattle_index = -1;
		Guild *guild = GuildManager::Instance().GetGuild(m_role->GetGuildID());
		if (NULL != guild)
		{
			CrossGuildBattleGuildItem old_cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
			WorldStatus::Instance().GetCrossGuildBattleInfo(old_cross_guildbattle_list);
			for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
			{
				if (old_cross_guildbattle_list[i].plat_type != m_role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type)
				{
					continue;
				}

				if (old_cross_guildbattle_list[i].guild_id <= 0)
				{
					continue;
				}

				if (m_role->GetGuildID() == old_cross_guildbattle_list[i].guild_id && m_role->GetUID() == guild->GetGuildMemberManager().GetTuanZhangUid())
				{
					cross_guildbattle_index = i;
					break;
				}
			}
		}
	
		const CrossGuildBattleOwnRewardCfg *rewrd_cfg = CrossGuildBattleConfig::Instance().GetOwnServerRewardCfg(cross_guildbattle_index);
		if (NULL != rewrd_cfg)
		{
			bool is_change_weapon = false;
			bool is_change_shizhuang = false;

			is_change_weapon = this->ActiveSpecialImg(rewrd_cfg->part_0, rewrd_cfg->index_0, false);
			is_change_shizhuang = this->ActiveSpecialImg(rewrd_cfg->part_1, rewrd_cfg->index_1, false);

			if (is_change_weapon)
			{
				EventHandler::Instance().OnGetSceneActivityReward(m_role, CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE, rewrd_cfg->index_0, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_WEAPON);
			}

			if (is_change_shizhuang)
			{
				EventHandler::Instance().OnGetSceneActivityReward(m_role, CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE, rewrd_cfg->index_1, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_SHIZHUANG);
			}
		}

		if (ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE) || cross_guildbattle_index == -1)
		{
			for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
			{
				const CrossGuildBattleOwnRewardCfg *rewrd_cfg = CrossGuildBattleConfig::Instance().GetOwnServerRewardCfg(i);
				if (NULL != rewrd_cfg)
				{
					this->UnActiveSpecialImg(rewrd_cfg->part_0, rewrd_cfg->index_0);
					this->UnActiveSpecialImg(rewrd_cfg->part_1, rewrd_cfg->index_1);
				}
			}
		}
	}
}

void Shizhuang::OnRoleUpLevel(int level)
{
	this->CheckActive();
	/*for (int i = 0; i < SHIZHUANG_TYPE_MAX;)
	{
		if (m_param.item_list[i].img_active_flag != 0)
		{
			i += 1;
			continue;
		}

		if (level >= LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenRoleLevel(FUN_OPEN_TYPE_SHIZHUANG))
		{
			ActiveShiZhuang(i);
			i += 1;
		}
	}*/
}

void Shizhuang::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		this->CheckActive();
	}
}

void Shizhuang::OnCompleteTask(TaskID task_id)
{
	this->CheckActive();
}

bool Shizhuang::ActiveShiZhuang(int shizhuang_type, bool is_recal)
{
	if (m_param.item_list[shizhuang_type].grade <= 0)
	{
		m_param.item_list[shizhuang_type].grade = 1;
		m_param.item_list[shizhuang_type].grade_bless = 0;
		const ShizhuangGradeConfig *grade_cfg = NULL;
		switch (shizhuang_type)
		{
		case SHIZHUANG_TYPE_WUQI:
		{
			grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangGradeConfig(1);
			JinjiesysFuncHelper::Instance().OnActiveJinjieSys(m_role, JINJIE_TYPE_SHENBING);
			break;
		}
		case SHIZHUANG_TYPE_BODY:
		{
			grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponGradeConfig(1);
			JinjiesysFuncHelper::Instance().OnActiveJinjieSys(m_role, JINJIE_TYPE_SHIZHUANG);
			break;
		}
		default:
			break;
		}
		if (NULL != grade_cfg)
		{
			BitmapFunction::SetBit(m_param.item_list[shizhuang_type].img_active_flag, sizeof(m_param.item_list[shizhuang_type].img_active_flag), grade_cfg->active_image_id);
			UseImg(shizhuang_type, grade_cfg->active_image_id);
		}

		if (is_recal)
		{
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG);
		}

	}

	return true;
}


void Shizhuang::CheckShizhuangSkill()
{
	//const ShizhuangSkillCfg *skill_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangSkillCfg(0, m_param.item_list[SHIZHUANG_TYPE_BODY].skill_level_list[0]);
	//if (nullptr == skill_cfg)
	//{
	//	return;
	//}

	//unsigned long long now_sec_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();

	//if (now_sec_ms < m_body_skill0_last_colddown_time)
	//{
	//	return;
	//}

	//int rand_val = RandomNum(10000);
	//if (skill_cfg->param_a > rand_val)
	//{
	//	return;
	//}

	//EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), 1, skill_cfg->param_c, 1);
	//effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	//effect_buff->AddJianRenEffect(skill_cfg->param_b);
	//m_role->AddEffect(effect_buff);

	//m_body_skill0_last_colddown_time = now_sec_ms + skill_cfg->param_d;
}

// 受到攻击时，有param_b%概率抵消这次伤害的param_c%。冷却param_a毫秒
void Shizhuang::OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill)
{
	if (delta_hp >= 0)
	{
		return;
	}

	const ShizhuangSkillCfg *skill_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangSkillCfg(0, m_param.item_list[SHIZHUANG_TYPE_BODY].skill_level_list[0]);
	if (nullptr == skill_cfg)
	{
		return;
	}

	unsigned long long now_sec_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();

	if (now_sec_ms < m_body_skill0_last_colddown_time)
	{
		return;
	}

	int rand_val = RandomNum(10000);
	if (rand_val >= skill_cfg->param_b)
	{
		return;
	}

	Attribute param_1 = static_cast<Attribute>(delta_hp * (skill_cfg->param_c * ROLE_ATTR_PER));
	delta_hp -= param_1;

	m_body_skill0_last_colddown_time = now_sec_ms + skill_cfg->param_a;
}

// 攻击时有param_b概率使自身暴击几率提高param_c%，持续param_d毫秒。冷却param_a毫秒
void Shizhuang::OnHurtOther(Character *target, long long base_injure, long long &injure)
{
	if (nullptr == target)
	{
		return;
	}

	const ShizhuangSkillCfg *skill_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponSkillCfg(0, m_param.item_list[SHIZHUANG_TYPE_WUQI].skill_level_list[0]);
	if (nullptr == skill_cfg)
	{
		return;
	}

	unsigned long long now_sec_ms = ServerLogic::GetInstServerLogic()->GetLocalTimeMs();

	if (now_sec_ms < m_wuqi_skill0_last_colddown_time)
	{
		return;
	}

	int rand_val = RandomNum(10000);
	if (rand_val >= skill_cfg->param_b)
	{
		return;
	}

	// 提高暴击率
	EffectBuff *effect_buff = new EffectBuff(m_role->GetId(), 1, skill_cfg->param_d, skill_cfg->skill_level);
	effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
	effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI, skill_cfg->param_c, skill_cfg->param_c);
	effect_buff->SetClientEffectType(EffectBase::ECT_SKILL_ADD_BAOJI_PER);
	m_role->AddEffect(effect_buff);

	m_wuqi_skill0_last_colddown_time = now_sec_ms + skill_cfg->param_a;
}

void Shizhuang::SendInfo()
{
	static Protocol::SCShizhuangInfo szi;
	for (int i = 0; i < SHIZHUANG_TYPE_MAX; ++i)
	{
		szi.item_list[i].use_img = m_param.item_list[i].use_img;
		szi.item_list[i].use_special_img = m_param.item_list[i].use_special_img;
		szi.item_list[i].is_use_special_img = m_param.item_list[i].is_use_special_img;

		UNSTD_STATIC_CHECK(sizeof(szi.item_list[i].active_flag) == sizeof(m_param.item_list[i].img_active_flag));
		memcpy(szi.item_list[i].active_flag, m_param.item_list[i].img_active_flag, sizeof(szi.item_list[i].active_flag));

		UNSTD_STATIC_CHECK(sizeof(szi.item_list[i].special_img_active_flag) == sizeof(m_param.item_list[i].special_img_active_flag));
		memcpy(szi.item_list[i].special_img_active_flag, m_param.item_list[i].special_img_active_flag, sizeof(szi.item_list[i].special_img_active_flag));

		szi.item_list[i].grade = m_param.item_list[i].grade;
		szi.item_list[i].grade_bless = m_param.item_list[i].grade_bless;
		szi.item_list[i].shuxingdan_count = m_param.item_list[i].shuxingdan_count;
		szi.item_list[i].equip_skill_level = m_param.item_list[i].equip_skill_level;
		szi.item_list[i].clear_bless_value_time = m_param.item_list[i].clear_bless_value_time;
		memcpy(szi.item_list[i].special_img_grade_list, m_param.item_list[i].special_img_grade_list, sizeof(szi.item_list[i].special_img_grade_list));
		memcpy(szi.item_list[i].valid_timestamp_list, m_param.item_list[i].valid_timestamp_list, sizeof(szi.item_list[i].valid_timestamp_list));
		memcpy(szi.item_list[i].skill_level_list, m_param.item_list[i].skill_level_list, sizeof(szi.item_list[i].skill_level_list));
		memcpy(szi.item_list[i].equip_level_list, m_param.item_list[i].equip_level_list, sizeof(szi.item_list[i].equip_level_list));
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&szi, sizeof(szi));
}

void Shizhuang::GetRoleAppearance(RoleAppearance *appearance)
{
	if (NULL != appearance)
	{
		if (m_param.item_list[SHIZHUANG_TYPE_WUQI].is_use_special_img > 0)
		{
			appearance->shizhuang_wuqi = m_param.item_list[SHIZHUANG_TYPE_WUQI].use_special_img;
		}
		else
		{
			appearance->shizhuang_wuqi = m_param.item_list[SHIZHUANG_TYPE_WUQI].use_img;
		}
		appearance->shizhuang_wuqi_is_special = m_param.item_list[SHIZHUANG_TYPE_WUQI].is_use_special_img;

		if (m_param.item_list[SHIZHUANG_TYPE_BODY].is_use_special_img > 0)
		{
			appearance->shizhuang_body = m_param.item_list[SHIZHUANG_TYPE_BODY].use_special_img;
		}
		else
		{
			appearance->shizhuang_body = m_param.item_list[SHIZHUANG_TYPE_BODY].use_img;
		}
		appearance->shizhuang_body_is_special= m_param.item_list[SHIZHUANG_TYPE_BODY].is_use_special_img;
	}
}


void Shizhuang::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		for (int type = 0; type < SHIZHUANG_TYPE_MAX; ++type)
		{
			auto& m_attrs_add = m_attrs_add_array[type];
			m_attrs_add.Reset();
			m_other_capability[type] = 0;
			CharIntAttrs other_per_add_attr;
			CharIntAttrs grade_attrs;
			double rate = 0.0;                              //属性加成万分比
			ShizhuangItem &item = m_param.item_list[type];
			{
				// 阶数加成
				const ShizhuangGradeConfig * curr_grade_config = NULL;
//				const ShizhuangGradeConfig * next_grade_config = NULL;
				switch (type)
				{
				case SHIZHUANG_TYPE_BODY:
				{
					curr_grade_config = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangGradeConfig(item.grade);
//					next_grade_config = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangGradeConfig(item.grade + 1);
					break;
				}
				case SHIZHUANG_TYPE_WUQI:
				{
					curr_grade_config = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponGradeConfig(item.grade);
//					next_grade_config = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponGradeConfig(item.grade + 1);
					break;
				}

				default:
					break;
				}

				if (NULL != curr_grade_config)
				{
					CharIntAttrs grade_attrs;
					grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += curr_grade_config->maxhp;
					grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += curr_grade_config->gongji;
					grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += curr_grade_config->fangyu;
					grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += curr_grade_config->mingzhong;
					grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += curr_grade_config->shanbi;
					grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += curr_grade_config->baoji;
					grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += curr_grade_config->jianren;
					grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += curr_grade_config->extra_zengshang;
					grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += curr_grade_config->extra_mianshang;
					// 特殊属性
					grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += curr_grade_config->per_jingzhun;
					grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += curr_grade_config->per_baoji;
// 					grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += curr_grade_config->per_mianshang;
// 					grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += curr_grade_config->per_pofang;
// 					grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += curr_grade_config->pvp_zengshang_per;
// 					grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += curr_grade_config->pve_zengshang_per;
// 					grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += curr_grade_config->pvp_jianshang_per;
// 					grade_attrs.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += curr_grade_config->pve_jianshang_pe	

					// 下面特殊属性用乘法
					grade_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, curr_grade_config->per_mianshang);
					grade_attrs.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, curr_grade_config->per_pofang);
					grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, curr_grade_config->pvp_zengshang_per);
					grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, curr_grade_config->pve_zengshang_per);
					grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, curr_grade_config->pvp_jianshang_per);
					grade_attrs.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, curr_grade_config->pve_jianshang_per);

					//仙尊卡
					int add_grade_attrs_per = 0;
					switch (type)
					{
					case SHIZHUANG_TYPE_BODY:
					{
						add_grade_attrs_per = m_role->GetXianZunKa()->GetAddShizhuangGradeAttrsPer();
						break;
					}
					case SHIZHUANG_TYPE_WUQI:
					{
						add_grade_attrs_per = m_role->GetXianZunKa()->GetAddShenbingGradeAttrsPer();
						break;
					}
					default:
						break;
					}
					for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX; ++i)
					{
						if (CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED == i) continue;

						grade_attrs.AddValue(i, static_cast<Attribute>(grade_attrs.m_attrs[i] * (ROLE_ATTR_PER * add_grade_attrs_per)));
					}

					// 大目标特殊形象万分比
					int special_img_per_add = 0;
					switch (type)
					{
					case SHIZHUANG_TYPE_BODY:
					{
						special_img_per_add = JinjieSysRewardMgr::Instance().GetJinjieSpecialImgExtraAddPer(m_role, JINJIE_TYPE_SHIZHUANG);
						break;
					}
					case SHIZHUANG_TYPE_WUQI:
					{
						special_img_per_add = JinjieSysRewardMgr::Instance().GetJinjieSpecialImgExtraAddPer(m_role, JINJIE_TYPE_SHENBING);
						break;
					}
					default:
						break;
					}
					if (special_img_per_add > 0)
					{
						other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (ROLE_ATTR_PER * special_img_per_add));
						other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (ROLE_ATTR_PER * special_img_per_add));
						other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (ROLE_ATTR_PER * special_img_per_add));
						other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (ROLE_ATTR_PER * special_img_per_add));
						other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (ROLE_ATTR_PER * special_img_per_add));
						other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (ROLE_ATTR_PER * special_img_per_add));
						other_per_add_attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (Attribute)(grade_attrs.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (ROLE_ATTR_PER * special_img_per_add));
					}
 					// 祝福值加成
 					//if (item.grade_bless > 0)
 					//{
 					//	if (NULL != next_grade_config  && curr_grade_config->bless_val_limit > 0)
 					//	{
 					//		double rate = (((double)item.grade_bless) / curr_grade_config->bless_val_limit) * (next_grade_config->bless_addition * ROLE_ATTR_PER);
 					//		//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)((next_grade_config->maxhp - curr_grade_config->maxhp) * rate);
 					//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)((next_grade_config->gongji - curr_grade_config->gongji) * rate);
 					//		//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)((next_grade_config->fangyu - curr_grade_config->fangyu) * rate);
 					//		//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (Attribute)((next_grade_config->mingzhong - curr_grade_config->mingzhong) * rate);
 					//		//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (Attribute)((next_grade_config->shanbi - curr_grade_config->shanbi) * rate);
 					//		//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (Attribute)((next_grade_config->baoji - curr_grade_config->baoji) * rate);
 					//		//m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (Attribute)((next_grade_config->jianren - curr_grade_config->jianren) * rate);
 					//	}
 					//}
					m_attrs_add.Add(grade_attrs.m_attrs);
					switch (type)
					{
					case SHIZHUANG_TYPE_BODY:
					{
						m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_SHIZHUANG, m_attrs_add, m_attrs_add);
						m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_SHIZHUANG_BASE, grade_attrs, m_attrs_add);
						break;
					}
					case  SHIZHUANG_TYPE_WUQI:
					{
						m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_SHENBING, m_attrs_add, m_attrs_add);
						m_role->GetTalent()->OtherModuleReCalcAttr(TALENT_OTHER_MODULE_SHENBING_BASE, grade_attrs, m_attrs_add);
						break;
					}
					default:
						break;
					}
				}
			}
			{
				// 时装和神兵装备
				int active_equip_grade = LOGIC_CONFIG->GetShizhuangConfig().GetACtiveEquipGrade();
				if (active_equip_grade <= item.grade)
				{
					for (int i = 0; i < ShizhuangItem::EQUIP_COUNT; ++i)
					{
						const ShizhuangEquipInfoCfg *equip_cfg = NULL;
						switch (type)
						{
						case SHIZHUANG_TYPE_BODY:
						{
							equip_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangEquipInfoCfg(i);
							break;
						}
						case  SHIZHUANG_TYPE_WUQI:
						{
							equip_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponEquipInfoCfg(i);
							break;
						}
						default:
							break;
						}
						if (NULL != equip_cfg)
						{
							int level = item.equip_level_list[i];
							if (level >= 0 && level < equip_cfg->max_level)
							{
								rate += equip_cfg->level_cfg[level].add_percent;

								m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += equip_cfg->level_cfg[level].maxhp;
								m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += equip_cfg->level_cfg[level].gongji;
								m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += equip_cfg->level_cfg[level].fangyu;
								m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += equip_cfg->level_cfg[level].mingzhong;
								m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += equip_cfg->level_cfg[level].shanbi;
								m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += equip_cfg->level_cfg[level].baoji;
								m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += equip_cfg->level_cfg[level].jianren;
							}
					    }
				   }
			  }
				rate *= ROLE_ATTR_PER;
				if (rate > 0)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * rate);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * rate);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * rate);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * rate);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * rate);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * rate);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * rate);
				}
			}

			//特殊形象加成
			for (int special_img_id = 1; special_img_id < SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT; ++special_img_id)
			{
				if (!this->IsActive(type, special_img_id)) continue;

				const ShizhuangSpecialImgUpgradeCfg *special_img_cfg = NULL;
				switch (type)
				{
				case SHIZHUANG_TYPE_BODY:
					special_img_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangSpecialImgUpgradeCfg(special_img_id, item.special_img_grade_list[special_img_id]);
					this->ShiZhuangHuanHuaSpecialCapAdd(special_img_id, item.special_img_grade_list[special_img_id], m_attrs_add);	//时装幻化特殊战力
					break;

				case SHIZHUANG_TYPE_WUQI:
					special_img_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponSpecialImgUpgradeCfg(special_img_id, item.special_img_grade_list[special_img_id]);
					this->ShiZhuangWuQiHuanHuaSpecialCapAdd(special_img_id, item.special_img_grade_list[special_img_id], m_attrs_add);  //神兵幻化特殊战力
					break;
				default:
					break;
				}
				if (NULL != special_img_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += special_img_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += special_img_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += special_img_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += special_img_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += special_img_cfg->shanbi;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += special_img_cfg->baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += special_img_cfg->jianren;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += special_img_cfg->extra_zengshang;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += special_img_cfg->extra_mianshang;
					// 特殊属性
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN] += special_img_cfg->per_jingzhun;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT] += special_img_cfg->per_baoji;
// 					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG] += special_img_cfg->per_mianshang;
// 					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += special_img_cfg->per_pofang;
// 					m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER] += special_img_cfg->pvp_zengshang_per;
// 					m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER] += special_img_cfg->pve_zengshang_per;
// 					m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER] += special_img_cfg->pvp_jianshang_per;
// 					m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER] += special_img_cfg->pve_jianshang_pe	

					// 下面特殊属性用乘法
					m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, special_img_cfg->per_mianshang);
					m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, special_img_cfg->per_pofang);
					m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER, special_img_cfg->pvp_zengshang_per);
					m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER, special_img_cfg->pve_zengshang_per);
					m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, special_img_cfg->pvp_jianshang_per);
					m_attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER, special_img_cfg->pve_jianshang_per);
				}	
			}
			// 属性丹
			if (item.shuxingdan_count > 0)
			{
				const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_SHIZHUANG, 0);
				if (NULL != cfg_item)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp * item.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji * item.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu * item.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong * item.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi * item.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg_item->baoji * item.shuxingdan_count;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg_item->jianren * item.shuxingdan_count;
				}
			}
			//技能属性加成
			for (int i = 0; i < ShizhuangItem::SKILL_COUNT; i++)
			{
				if (item.skill_level_list[i] > 0)
				{
					const ShizhuangSkillCfg *skill_cfg = NULL;
					switch (type)
					{
					case SHIZHUANG_TYPE_BODY:
						skill_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangSkillCfg(i, item.skill_level_list[i]);
						break;

					case SHIZHUANG_TYPE_WUQI:
						skill_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponSkillCfg(i, item.skill_level_list[i]);
						break;

					default:
						break;
					}

					if (NULL != skill_cfg)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += skill_cfg->maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += skill_cfg->gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += skill_cfg->fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += skill_cfg->mingzhong;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += skill_cfg->shanbi;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += skill_cfg->baoji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += skill_cfg->jianren;
						m_attrs_add.m_attrs[CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_SKILL_CAP_PER] += skill_cfg->skill_cap_per;
						m_other_capability[type] += skill_cfg->capability;
					}
				}
			}
			// 其他百分比属性加成，与上面的全属性加成加算
			{
				m_attrs_add.AddAttrs(&other_per_add_attr);
			}
			if (SHIZHUANG_TYPE_WUQI == type)
			{
				m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SHIZHUANG_WUQI, m_attrs_add);
				m_role->GetCapability()->AddCap(CAPABILITY_TYPE_SHIZHUANG_WUQI, m_other_capability[type]);
				m_param.item_list[type].capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHIZHUANG_WUQI);
			}
			else
			{
				m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SHIZHUANG, m_attrs_add);
				m_role->GetCapability()->AddCap(CAPABILITY_TYPE_SHIZHUANG, m_other_capability[type]);
				m_param.item_list[type].capability = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHIZHUANG);
			}
		}
	}
	for (auto& m_attrs_add : m_attrs_add_array)
	{
		base_add.Add(m_attrs_add.m_attrs, true);
	}
}

void Shizhuang::UseImg(int shizhuang_type, int img_id)
{
	if (shizhuang_type < 0 || shizhuang_type >= SHIZHUANG_TYPE_MAX || img_id < 0 || img_id >= SHIZHUANG_MAX_IMG_COUNT)
	{
		return;
	}

	//ShizhuangItem &shizhuang_item = m_param.item_list[shizhuang_type];

	if (0 != img_id && !this->IsActiveImage(shizhuang_type, img_id))
	{
		m_role->NoticeNum(errornum::EN_SHIZHUANG_NOT_ACTIVE);
		return;
	}

	m_param.item_list[shizhuang_type].use_img = img_id;
	m_param.item_list[shizhuang_type].is_use_special_img = 0;
	m_param.item_list[shizhuang_type].use_special_img = 0;

	{
		if (SHIZHUANG_TYPE_WUQI == shizhuang_type)
		{
			// warning 这里调用UseShenbingImage时必须传0，防止递归
			m_role->GetShenqi()->UseShenbingImage(0);
			m_role->RefreshApperance(true, APPEARANCE_WUQI_USE_TYPE_SHIZHUANG);
		}
		else
		{
			// warning 这里调用UseBaojiaImage时必须传0，防止递归
			m_role->GetShenqi()->UseBaojiaImage(0);
			m_role->RefreshApperance(true, APPEARANCE_WUQI_USE_TYPE_INVALID, APPEARANCE_BODY_USE_TYPE_SHIZHUANG);
		}
	}

	this->SendInfo();
}

void Shizhuang::UseMaxGradeImg(int shizhuang_type)
{
	if (shizhuang_type < 0 || shizhuang_type >= SHIZHUANG_TYPE_MAX)
	{
		return;
	}
	int grade = this->GetShiZhuangGrade(shizhuang_type);
	const ShizhuangGradeConfig * grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetGradeConfigByType(grade, shizhuang_type);
	if (nullptr == grade_cfg)
	{
		return;
	}
	this->UseImg(shizhuang_type, grade_cfg->active_image_id);
}

void Shizhuang::UseSpecialImg(int shizhuang_type, int special_img_id)
{
	if (shizhuang_type < 0 || shizhuang_type >= SHIZHUANG_TYPE_MAX || special_img_id < 0 || special_img_id >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
	{
		return;
	}

	//ShizhuangItem &shizhuang_item = m_param.item_list[shizhuang_type];

	if (0 != special_img_id && !this->IsActive(shizhuang_type, special_img_id))
	{
		m_role->NoticeNum(errornum::EN_SHIZHUANG_NOT_ACTIVE);
		return;
	}


	m_param.item_list[shizhuang_type].use_img = 0;
	m_param.item_list[shizhuang_type].use_special_img = special_img_id;
	m_param.item_list[shizhuang_type].is_use_special_img = 1;

	{
		if (SHIZHUANG_TYPE_WUQI == shizhuang_type)
		{
			m_role->GetShenqi()->UseShenbingImage(0);
			m_role->RefreshApperance(true, APPEARANCE_WUQI_USE_TYPE_SHIZHUANG);
		}
		else
		{
			m_role->GetShenqi()->UseBaojiaImage(0);
			m_role->RefreshApperance(true, APPEARANCE_WUQI_USE_TYPE_INVALID, APPEARANCE_BODY_USE_TYPE_SHIZHUANG);
		}
	}
	//m_role->RefreshApperance(true);
	this->SendInfo();
}

void Shizhuang::UnUseSpecialImg(int shizhuang_type, int index)
{
	if (shizhuang_type < 0 || shizhuang_type >= SHIZHUANG_TYPE_MAX || index < 0 || index >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
	{
		return;
	}
	
	this->UseMaxGradeImg(shizhuang_type);
	/*m_param.item_list[shizhuang_type].use_special_img = 0;
	m_param.item_list[shizhuang_type].is_use_special_img = 0;
	m_role->RefreshApperance(true);
	this->SendInfo();*/
}

bool Shizhuang::UpOneGrade(int typ, int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num)
{
	if (typ < 0 || typ >= SHIZHUANG_TYPE_MAX) 
		return false;

	ShizhuangItem &item = m_param.item_list[typ];

	if (item.grade < valid_min_grade)
	{
		if (typ == SHIZHUANG_TYPE_BODY)
		{
			m_role->NoticeNum(errornum::EN_SHIZHUANG_BODY_UP_ONE_GRADE_LIMIT);
		}
		else
		{
			m_role->NoticeNum(errornum::EN_SHIZHUANG_WUQI_UP_ONE_GRADE_LIMIT);
		}
		return false;
	}
	else if (item.grade > valid_max_grade)
	{
		const ItemBase *item_base = ITEMPOOL->GetItem(return_item_id);
		if (item_base != nullptr)
		{
			for (int loop_time = 0; return_item_num > 0 && loop_time < 1000; ++loop_time)
			{
				int put_num = return_item_num > item_base->GetPileLimit() ? item_base->GetPileLimit() : return_item_num;
				return_item_num -= put_num;

				ItemConfigData return_item(return_item_id, true, put_num);
				m_role->GetKnapsack()->PutOrMail(return_item, PUT_REASON_SHIZHUANG_UPGRADE_ONE_RETURN);
			}

			return true;
		}
		return false;
	}

	short old_grade = item.grade;
	short old_bless_val = item.grade_bless;

	const ShizhuangGradeConfig * curr_grade_cfg = NULL;
	const ShizhuangGradeConfig * target_grade_cfg = NULL;
	const ShizhuangUpGradeProbConfig * upgrade_prob_cfg = NULL;
	switch (typ)
	{
	case SHIZHUANG_TYPE_BODY:
	{
		curr_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangGradeConfig(old_grade);
		if (NULL == curr_grade_cfg)
		{
			return false;
		}
		target_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangGradeConfig(old_grade + 1);
		if (NULL == target_grade_cfg)
		{
			m_role->NoticeNum(errornum::EN_SHIZHUANG_GRADE_LIMIT);
			return false;
		}
		upgrade_prob_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangUpGradeProbConfig(old_grade, old_bless_val);
		break;
	}

	case SHIZHUANG_TYPE_WUQI:
	{
		curr_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponGradeConfig(old_grade);
		if (NULL == curr_grade_cfg)
		{
			return false;
		}
		target_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponGradeConfig(old_grade + 1);
		if (NULL == target_grade_cfg)
		{
			m_role->NoticeNum(errornum::EN_SHIZHUANG_GRADE_LIMIT);
			return false;
		}
		upgrade_prob_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponUpGradeProbConfig(old_grade, old_bless_val);
		break;
	}

	default:
		break;
	}

	if (NULL == upgrade_prob_cfg)
	{
		m_role->NoticeNum(errornum::EN_SHIZHUANG_GRADE_LIMIT);
		return false;
	}


	item.grade += 1;
	item.grade_bless = 0;

	switch (typ)
	{
	case SHIZHUANG_TYPE_BODY:
		EventHandler::Instance().OnShiZhuangUpgrade(m_role, old_grade, item.grade);
		break;
	case SHIZHUANG_TYPE_WUQI:
		EventHandler::Instance().OnShenBingUpgrade(m_role, old_grade, item.grade);
		break;
	default:
		break;
	}

	unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();
	const UpgradeRushReturnCfg* upgrade_rush_cfg = LOGIC_CONFIG->GetUpgradeRushConfig().GetUpgradeRushReturnCfg(UPGRADE_RUSH_TYPE_SHIZHUANG, item.grade);      //返还进阶丹
	if (NULL != upgrade_rush_cfg)
	{
		if ((now_time - item.last_upgrade_succ_time) <= (unsigned int)(upgrade_rush_cfg->limit_time * SECOND_PER_HOUR))
		{
			m_role->GetKnapsack()->PutOrMail(upgrade_rush_cfg->reward_item, PUT_REASON_SHIZHUANG_UP);
		}
	}
	item.last_upgrade_succ_time = now_time;

	const ShizhuangSkillCfg *skill_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangSkillCfg(0, item.skill_level_list[0] + 1);
	if (NULL != skill_cfg && item.grade >= skill_cfg->shizhuang_grade)
	{
		item.skill_level_list[0]++;
	}

	BitmapFunction::SetBit(m_param.item_list[typ].img_active_flag, sizeof(m_param.item_list[typ].img_active_flag), target_grade_cfg->active_image_id);
	this->UseImg(typ, target_grade_cfg->active_image_id);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG);

	if (target_grade_cfg->is_broadcast)
	{
		int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shizhuang_upgrade,
			UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade);
		if (sendlen > 0)
		{
			if (target_grade_cfg->client_grade >= 5)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
			}
			else
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	this->SendInfo();

	gamelog::g_log_other.printf(LL_INFO, "Shizhuang::OnUpGrade user[%d, %s], shizhuang_type[%d], grade_bless[%d, %d] -> [%d, %d]",
		m_role->GetUID(), m_role->GetName(), typ, old_grade, old_bless_val, item.grade, item.grade_bless);
	return true;
}
bool Shizhuang::UpOneGradeShiZhuang(int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num)
{
	return UpOneGrade(SHIZHUANG_TYPE_BODY, valid_min_grade, valid_max_grade, return_item_id, return_item_num);
}
bool Shizhuang::UpOneGradeShenBing(int valid_min_grade, int valid_max_grade, int return_item_id, int return_item_num)
{
	return UpOneGrade(SHIZHUANG_TYPE_WUQI, valid_min_grade, valid_max_grade, return_item_id, return_item_num);
}


bool Shizhuang::OnUpGrade(int shizhuang_type, bool is_auto_buy, int repeat_times)
{
	if (shizhuang_type < 0 || shizhuang_type >= SHIZHUANG_TYPE_MAX) return false;

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	ShizhuangItem &item = m_param.item_list[shizhuang_type];

	short old_grade = item.grade;
	short oldest_bless_val = item.grade_bless;
	bool succeeded = false;

	const ShizhuangGradeConfig * curr_grade_cfg = NULL;
	const ShizhuangGradeConfig * target_grade_cfg = NULL;
	const ShizhuangUpGradeProbConfig * upgrade_prob_cfg = NULL;
	for (int i = 0; i < repeat_times; ++i)
	{
		short old_bless_val = item.grade_bless;
		switch (shizhuang_type)
		{
		case SHIZHUANG_TYPE_BODY:
		{
			curr_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangGradeConfig(old_grade);
			if (NULL == curr_grade_cfg)
			{
				return false;
			}
			target_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangGradeConfig(old_grade + 1);
			if (NULL == target_grade_cfg)
			{
				m_role->NoticeNum(errornum::EN_SHIZHUANG_GRADE_LIMIT);
				return false;
			}
			upgrade_prob_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangUpGradeProbConfig(old_grade, old_bless_val);
			break;
		}
		case SHIZHUANG_TYPE_WUQI:
		{
			curr_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponGradeConfig(old_grade);
			if (NULL == curr_grade_cfg)
			{
				return false;
			}
			target_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponGradeConfig(old_grade + 1);
			if (NULL == target_grade_cfg)
			{
				m_role->NoticeNum(errornum::EN_SHIZHUANG_GRADE_LIMIT);
				return false;
			}
			upgrade_prob_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponUpGradeProbConfig(old_grade, old_bless_val);
			break;
		}

		default:
			break;
		}

		if (NULL == upgrade_prob_cfg)
		{
			m_role->NoticeNum(errornum::EN_SHIZHUANG_GRADE_LIMIT);
			//return false;
			succeeded = true;
			break;
		}

		static ItemExtern::ItemConsumeList consume_list;
		consume_list.Reset();

		{
			// 进阶符
			int stuff_count = 0;
			ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

			short upgrade_stuff_num = m_role->GetKnapsack()->Count(curr_grade_cfg->upgrade_stuff2_id);
			// 如果有限时进阶丹, 则优先使用
			if (upgrade_stuff_num > 0)
			{
				stuff_list[stuff_count].item_id = curr_grade_cfg->upgrade_stuff2_id;
				stuff_list[stuff_count].num = upgrade_stuff_num >= curr_grade_cfg->upgrade_stuff_count ? curr_grade_cfg->upgrade_stuff_count : upgrade_stuff_num;
				stuff_list[stuff_count].buyable = is_auto_buy;

				++stuff_count;
			}

			// 如果限时进阶丹用完了，还不够一次批量使用，就使用普通进阶丹
			if (upgrade_stuff_num < curr_grade_cfg->upgrade_stuff_count && stuff_count < ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM)
			{
				stuff_list[stuff_count].item_id = curr_grade_cfg->upgrade_stuff_id;
				stuff_list[stuff_count].num = curr_grade_cfg->upgrade_stuff_count - upgrade_stuff_num;
				stuff_list[stuff_count].buyable = is_auto_buy;

				++stuff_count;
			}

			if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, false))
			{
				//return false;
				succeeded = true;
				break;
			}
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "Shizhuang::OnUpGrade"))
		{
			succeeded = true;
			break;
		}

		{
			// 生效

			int rand_val = RandomNum(10000);
			if (rand_val < upgrade_prob_cfg->succ_rate || item.grade_bless >= curr_grade_cfg->bless_val_limit)
			{
				succeeded = true;
			}

			if (false == succeeded)
			{
				int upgrade_sys_crit_type = shizhuang_type == SHIZHUANG_TYPE_BODY ? UPGRADE_SYS_CRIT_TYPE_SHIZHUANG : UPGRADE_SYS_CRIT_TYPE_SHENBING;
				int crit_value = UpgradeSysCrit::Instance().CalculateCritValue(m_role, upgrade_sys_crit_type);
				short new_bless_val = old_bless_val + (upgrade_prob_cfg->RandAddBlessVal() * crit_value);

				if (new_bless_val > curr_grade_cfg->bless_val_limit) new_bless_val = curr_grade_cfg->bless_val_limit;

				item.grade_bless = new_bless_val;

			}
			else
			{
				item.grade += 1;
				item.grade_bless = 0;

				switch (shizhuang_type)
				{
				case SHIZHUANG_TYPE_BODY:
				{
					EventHandler::Instance().OnShiZhuangUpgrade(m_role, old_grade, item.grade);
				}
				break;
				case SHIZHUANG_TYPE_WUQI:
				{
					EventHandler::Instance().OnShenBingUpgrade(m_role, old_grade, item.grade);
				}
				break;
				default:
					break;
				}

				unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();
				const UpgradeRushReturnCfg* upgrade_rush_cfg = LOGIC_CONFIG->GetUpgradeRushConfig().GetUpgradeRushReturnCfg(UPGRADE_RUSH_TYPE_SHIZHUANG, item.grade);      //返还进阶丹
				if (NULL != upgrade_rush_cfg)
				{
					if ((now_time - item.last_upgrade_succ_time) <= (unsigned int)(upgrade_rush_cfg->limit_time * SECOND_PER_HOUR))
					{
						m_role->GetKnapsack()->PutOrMail(upgrade_rush_cfg->reward_item, PUT_REASON_SHIZHUANG_UP);
					}
				}
				item.last_upgrade_succ_time = now_time;

				const ShizhuangSkillCfg *skill_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangSkillCfg(0, item.skill_level_list[0] + 1);
				if (NULL != skill_cfg && item.grade >= skill_cfg->shizhuang_grade)
				{
					item.skill_level_list[0]++;
				}

				BitmapFunction::SetBit(m_param.item_list[shizhuang_type].img_active_flag, sizeof(m_param.item_list[shizhuang_type].img_active_flag), target_grade_cfg->active_image_id);
				this->UseImg(shizhuang_type, target_grade_cfg->active_image_id);

				m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG);

				if (target_grade_cfg->is_broadcast)
				{
					int sendlen = 0;
					switch (shizhuang_type)
					{
					case SHIZHUANG_TYPE_BODY:
					{
						sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shizhuang_upgrade,
							UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade);
					}
					break;
					case SHIZHUANG_TYPE_WUQI:
					{
						sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenbing_upgrade,
							UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), target_grade_cfg->client_grade, target_grade_cfg->grade);
					}
					break;
					default:
						break;
					}

					if (sendlen > 0)
					{
						if (target_grade_cfg->client_grade >= 5)
						{
							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
						}
						else
						{
							World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_NOTICE_3);
						}
					}
				}
				break;
			}
		}
	}
	this->SendInfo();

	gamelog::g_log_other.printf(LL_INFO, "Shizhuang::OnUpGrade user[%d, %s], shizhuang_type[%d], grade_bless[%d, %d] -> [%d, %d]",
		m_role->GetUID(), m_role->GetName(), shizhuang_type, old_grade, oldest_bless_val, item.grade, item.grade_bless);

	return !succeeded;
}

bool Shizhuang::UpgradeSpecialImg(int shizhuang_type, int special_img_id)
{
	if (shizhuang_type < 0 || shizhuang_type >= SHIZHUANG_TYPE_MAX) return false;
	if (special_img_id <= 0 || special_img_id > SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT) return false;

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	ShizhuangItem &item = m_param.item_list[shizhuang_type];
	if (!this->IsActive(shizhuang_type, special_img_id))
	{
		m_role->NoticeNum(errornum::EN_SHIZHUANG_NOT_ACTIVE);
		return false;
	}

	short old_grade = item.special_img_grade_list[special_img_id];
	const ShizhuangSpecialImgUpgradeCfg * curr_grade_cfg = NULL;
	const ShizhuangSpecialImgUpgradeCfg * next_grade_cfg = NULL;

	switch (shizhuang_type)
	{
	case SHIZHUANG_TYPE_BODY:
	{
		curr_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangSpecialImgUpgradeCfg(special_img_id, old_grade);

		if (NULL == curr_grade_cfg)
		{
			return false;
		}

		next_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangSpecialImgUpgradeCfg(special_img_id, old_grade + 1);
		break;
	}
	case SHIZHUANG_TYPE_WUQI:
	{
		curr_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponSpecialImgUpgradeCfg(special_img_id, old_grade);

		if (NULL == curr_grade_cfg)
		{
			return false;
		}

		next_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponSpecialImgUpgradeCfg(special_img_id, old_grade + 1);
		break;
	}
	default:
		break;
	}

	if (NULL == next_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_SHIZHUANG_IMG_GRADE_LIMIT);
		return false;
	}

	// 消耗
	if (!m_role->GetKnapsack()->ConsumeItem(curr_grade_cfg->stuff_id, curr_grade_cfg->stuff_num, ""))
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return false;
	}
	// 给予
	++item.special_img_grade_list[special_img_id];
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG);
	this->SendInfo();

	gamelog::g_log_other.printf(LL_INFO, "Shizhuang::UpgradeSpecialImg user[%d, %s], shizhuang_type[%d], special_img_id[%d] grade[%d -> %d]",
		m_role->GetUID(), m_role->GetName(), shizhuang_type, special_img_id, old_grade, item.special_img_grade_list[special_img_id]);

	return true;
}

bool Shizhuang::ActiveSpecialImg(int shizhuang_type, int special_img_id, bool is_notify)
{
	if (shizhuang_type < 0 || shizhuang_type >= SHIZHUANG_TYPE_MAX || special_img_id <= 0 || special_img_id >= SHIZHUANG_MAX_IMG_COUNT || special_img_id >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
	{
		return false;
	}

	ShizhuangItem &shizhuang_item = m_param.item_list[shizhuang_type];

	if (this->IsActive(shizhuang_type, special_img_id))
	{
		if (is_notify)
		{
			if (shizhuang_type == SHIZHUANG_TYPE_BODY)
			{
				m_role->NoticeNum(errornum::EN_SHIZHUANG_HAS_ACTIVE);
			}
			else if (shizhuang_type == SHIZHUANG_TYPE_WUQI)
			{
				m_role->NoticeNum(errornum::EN_WUQI_HAS_ACTIVE);
			}
		}
		return false;
	}

	const ShizhuangSpecialImgCfg * special_img_cfg = NULL;
	if (SHIZHUANG_TYPE_WUQI == shizhuang_type) special_img_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponSpecialImgCfg(special_img_id);
	else if (SHIZHUANG_TYPE_BODY == shizhuang_type) special_img_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangSpecialImgCfg(special_img_id);
	if (NULL == special_img_cfg)
	{
		return false;
	}

	if (special_img_cfg->valid_time_s > 0)
	{
		shizhuang_item.valid_timestamp_list[special_img_id] = (unsigned int)EngineAdapter::Instance().Time() + special_img_cfg->valid_time_s;
	}

	char old_active_flag_hex[SHIZHUANG_MAX_ACTIVE_SPECIAL_IMAGE_BYTE * 2 + 1] = { 0 };
	BinToHex((const char *)shizhuang_item.special_img_active_flag, sizeof(shizhuang_item.special_img_active_flag), old_active_flag_hex);

	BitmapFunction::SetBit(shizhuang_item.special_img_active_flag, sizeof(shizhuang_item.special_img_active_flag), special_img_id);

	char active_flag_hex[SHIZHUANG_MAX_ACTIVE_SPECIAL_IMAGE_BYTE * 2 + 1] = { 0 };
	BinToHex((const char *)shizhuang_item.special_img_active_flag, sizeof(shizhuang_item.special_img_active_flag), active_flag_hex);

	if (special_img_cfg->valid_time_s == 0)
	{
		if (SHIZHUANG_TYPE_BODY == shizhuang_type)
		{
			m_role->GetImgFuling()->OnPutOnImg(JINGJIE_TYPE_SHIZHUANG, special_img_id);
			JinjiesysFuncHelper::Instance().OnActiveSpecialImg(m_role, JINJIE_TYPE_SHIZHUANG, special_img_id);
		}
		if (SHIZHUANG_TYPE_WUQI == shizhuang_type)
		{
			m_role->GetImgFuling()->OnPutOnImg(JINGJIE_TYPE_SHIZHUANG_WEAPON, special_img_id);
			JinjiesysFuncHelper::Instance().OnActiveSpecialImg(m_role, JINJIE_TYPE_SHENBING, special_img_id);
		}
	}

	if (0 == shizhuang_item.special_img_grade_list[special_img_id])
	{
		shizhuang_item.special_img_grade_list[special_img_id] = 1;
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG);
	this->UseSpecialImg(shizhuang_type, special_img_id);
	if (SHIZHUANG_TYPE_BODY == shizhuang_type)
	{
		m_role->GetTitleManager()->CheckAllTitleCondition(TITLE_TYPE_SHIZHUANG);
		m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_SHIZHUANG_PART_1,special_img_id,true);

		//检查是否有表白属性加成
		QingyuanGlobal * qingyuuan_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetUID());
		if (NULL != qingyuuan_global)
		{
			qingyuuan_global->CheckSetAddProfessSharePer(special_img_id);
		}
	}
	if (SHIZHUANG_TYPE_WUQI == shizhuang_type)
	{
		m_role->GetTitleManager()->CheckAllTitleCondition(TITLE_TYPE_WEAPON);
		m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_SHIZHUANG_PART_0, special_img_id, true);
		TianShuXunZhu::CheckShiXueTianShu(m_role, CHENGZHANG_TIANSHU_TYPE_ACTIVE_WUQI_SHIZHUANG);
	}

	gamelog::g_log_other.printf(LL_INFO, "Shizhuang::UseSpecialImg user[%d, %s], level[%d], shizhuang_type[%d], special_img_id[%d], old_flag[%s], new_flag[%s]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), shizhuang_type, special_img_id, old_active_flag_hex, active_flag_hex);

	return true;
}

bool Shizhuang::UnActiveSpecialImg(int shizhuang_type, int special_img_id, bool is_need_mail_notice)
{
	if (shizhuang_type < 0 || shizhuang_type >= SHIZHUANG_TYPE_MAX || special_img_id <= 0 || special_img_id >= SHIZHUANG_MAX_IMG_COUNT || special_img_id >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
	{
		return false;
	}

	ShizhuangItem &shizhuang_item = m_param.item_list[shizhuang_type];

	if (!this->IsActive(shizhuang_type, special_img_id))
	{
		return false;
	}

	BitmapFunction::ClearBit(m_param.item_list[shizhuang_type].special_img_active_flag, sizeof(m_param.item_list[shizhuang_type].special_img_active_flag), special_img_id);
	m_param.item_list[shizhuang_type].valid_timestamp_list[special_img_id] = 0;
	if (1 == shizhuang_item.special_img_grade_list[special_img_id])
	{
		shizhuang_item.special_img_grade_list[special_img_id] = 0;
	}
	this->UnUseSpecialImg(shizhuang_type, special_img_id);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG);

	if (SHIZHUANG_TYPE_WUQI == shizhuang_type)
	{
		m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_SHIZHUANG_PART_0, special_img_id, false);
	}
	else if (SHIZHUANG_TYPE_BODY == shizhuang_type)
	{
		m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_SHIZHUANG_PART_1, special_img_id, false);
	}

	const ShizhuangSpecialImgCfg * special_img_cfg = NULL;
	if (SHIZHUANG_TYPE_WUQI == shizhuang_type) special_img_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponSpecialImgCfg(special_img_id);
	else if (SHIZHUANG_TYPE_BODY == shizhuang_type) special_img_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangSpecialImgCfg(special_img_id);
	if (NULL != special_img_cfg && is_need_mail_notice)
	{
		static MailContentParam mail_param; mail_param.Reset();
		int length = SNPRINTF(mail_param.contenttxt, sizeof(mail_param.contenttxt), gamestring::g_time_limit_cap_img_mail_context,
			special_img_cfg->active_item_id);

		if (length > 0)
		{
			MailRoute::MailToRole(m_role, SYSTEM_MAIL_REASON_INVALID, mail_param);
		}
	}

	gamelog::g_log_other.printf(LL_INFO, "Shizhuang::UnActiveSpecialImg user[%d, %s], level[%d], shizhuang_type[%d], special_img_id[%d]",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(), shizhuang_type, special_img_id);

	return true;
}

void Shizhuang::CheckActiveByCondition(int check_active_type)
{
	if (NULL == m_role || m_role->IsInCross() || CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}


	// 检查城主时装
	if ((SHIZHUANG_CHECK_ACTIVE_TYPE_ALL == check_active_type || SHIZHUANG_CHECK_ACTIVE_TYPE_GCZ == check_active_type))
	{
		int yf_part_type = 0, yf_part_index = 0, wq_part_type = 0, wq_part_index = 0;
		bool succ = LOGIC_CONFIG->GetGongChengZhanConfig().GetChengZhuShizhuang(&yf_part_type, &yf_part_index, &wq_part_type, &wq_part_index);

		if (succ)
		{
			if (WorldStatus::Instance().GetGCZChengzhuID() == m_role->GetUID())
			{
				bool is_change_img = false;

				is_change_img = this->ActiveSpecialImg(yf_part_type, yf_part_index, false);

				if (is_change_img)
				{
					EventHandler::Instance().OnGetSceneActivityReward(m_role, ACTIVITY_TYPE_GONGCHENGZHAN, yf_part_index, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_SHIZHUANG);
				}
			}
			else
			{
				//检查城主伴侣时装
				succ = LOGIC_CONFIG->GetGongChengZhanConfig().GetChengZhuShizhuang(&yf_part_type, &yf_part_index, &wq_part_type, &wq_part_index, true);
				int chengzhu_id = WorldStatus::Instance().GetGCZChengzhuID();
				UserCacheNode *user_cache = UserCacheManager::Instance().GetUserNode(chengzhu_id);
				//不满足城主伴侣条件，取消城主形象
				if (!succ || nullptr == user_cache || m_role->GetLoverUid() != user_cache->uid)
				{
					this->UnActiveSpecialImg(yf_part_type, yf_part_index);
				}
				else
				{
					if (this->ActiveSpecialImg(yf_part_type, yf_part_index, false))
					{
						EventHandler::Instance().OnGetSceneActivityReward(m_role, ACTIVITY_TYPE_GONGCHENGZHAN, yf_part_index, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_SHIZHUANG);
					}
				}
			}
		}
	}

	std::set<ItemID> active_item_set;
	std::set<ItemID> unactive_item_set;

	// 跨服增加战力榜
	if (SHIZHUANG_CHECK_ACTIVE_TYPE_ALL == check_active_type || SHIZHUANG_CHECK_ACTIVE_TYPE_CROSS_ADD_CAP == check_active_type)
	{
		int uid_list[CROSS_RANK_REWARD_COUNT] = { 0 };
		WorldStatus3::Instance().GetCrossAddCapRankFirst(uid_list);
		for (int i = 0; i < CROSS_RANK_REWARD_COUNT; ++i)
		{
			const CrossRankReward *reward_cfg = CrossRankRewardConfig::Instance().GetReward(CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY, i + 1);
			if (nullptr != reward_cfg)
			{
				if (m_role->GetUID() == uid_list[i])
				{
					active_item_set.insert(reward_cfg->img_item_id);
				}
				else
				{
					unactive_item_set.insert(reward_cfg->img_item_id);
				}
			}
		}
	}

	// 跨服增加魅力榜
	if (SHIZHUANG_CHECK_ACTIVE_TYPE_ALL == check_active_type || SHIZHUANG_CHECK_ACTIVE_TYPE_CROSS_ADD_CHARM == check_active_type)
	{
		int uid_list[CROSS_RANK_REWARD_COUNT] = { 0 };
		WorldStatus3::Instance().GetCrossAddCharmRankFirst(uid_list);
		for (int i = 0; i < CROSS_RANK_REWARD_COUNT; ++i)
		{
			const CrossRankReward *reward_cfg = CrossRankRewardConfig::Instance().GetReward(CROSS_PERSON_RANK_TYPE_ADD_CHARM, i + 1);
			if (nullptr != reward_cfg)
			{
				if (m_role->GetUID() == uid_list[i])
				{
					active_item_set.insert(reward_cfg->img_item_id);
				}
				else
				{
					unactive_item_set.insert(reward_cfg->img_item_id);
				}
			}
		}
	}

	// 结算激活与不激活
	{
		int part_type = -1, img_id = -1;
		// 激活
		std::set<ItemID>::iterator iter = active_item_set.begin();
		for (; iter != active_item_set.end(); ++iter)
		{
			if (CrossRankRewardConfig::Instance().GetRewardImageTypeById(*iter, part_type, img_id))
			{
				int reward_type = 0;
				if (part_type == SHIZHUANG_TYPE_WUQI)
				{
					reward_type = Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_WEAPON;
				}
				else if (part_type == SHIZHUANG_TYPE_BODY)
				{
					reward_type = Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_SHIZHUANG;
				}

				if (this->ActiveSpecialImg(part_type, img_id, false))
				{
					switch (check_active_type)
					{
					case SHIZHUANG_CHECK_ACTIVE_TYPE_CROSS_ADD_CAP:
					{
						EventHandler::Instance().OnGetSceneActivityReward(m_role, Protocol::SCSceneActivityRewardInfo::REWARD_SOURCE_ID_CROSS_ADD_CAP, img_id, reward_type);
					}
					break;

					case SHIZHUANG_CHECK_ACTIVE_TYPE_CROSS_ADD_CHARM:
					{
						EventHandler::Instance().OnGetSceneActivityReward(m_role, Protocol::SCSceneActivityRewardInfo::REWARD_SOURCE_ID_CROSS_ADD_CHARM, img_id, reward_type);
					}
					break;

					default:
						break;
					}
				}
			}
		}

		// 取消激活
		for (iter = unactive_item_set.begin(); iter != unactive_item_set.end(); ++iter)
		{
			// 如果本次已经激活了，不应该取消掉
			if (active_item_set.find(*iter) == active_item_set.end())
			{
				if (CrossRankRewardConfig::Instance().GetRewardImageTypeById(*iter, part_type, img_id))
				{
					this->UnActiveSpecialImg(part_type, img_id, SHIZHUANG_CHECK_ACTIVE_TYPE_ALL != check_active_type);
				}
			}
		}
	}
}

bool Shizhuang::ActiveBodySpecialImage(int index, bool is_notify)
{
	return this->ActiveSpecialImg(SHIZHUANG_TYPE_BODY, index, is_notify);
}

bool Shizhuang::ActiveWuqiSpecialImage(int index, bool is_notify)
{
	return this->ActiveSpecialImg(SHIZHUANG_TYPE_WUQI, index, is_notify);
}

int Shizhuang::GetHasActiveShizhuang(int shizhuang_type)
{
	if (shizhuang_type < SHIZHUANG_TYPE_WUQI || shizhuang_type >= SHIZHUANG_TYPE_MAX)
	{
		return 0;
	}

	int count = 0;

	//ShizhuangItem &shizhuang_item = m_param.item_list[shizhuang_type];

	for (int i = 0; i < SHIZHUANG_MAX_IMG_COUNT; ++i)
	{
		if (this->IsActive(shizhuang_type, i))
		{
			++count;
		}
	}

	return count;
}

bool Shizhuang::OnUseShuxingdan(int slot_idx, short &use_count)
{
	if (slot_idx < 0 || slot_idx >= SHUXINGDAN_MAX_TYPE || use_count <= 0)
	{
		return false;
	}

	ShizhuangItem& shizhuang_param = m_param.item_list[slot_idx];
	const ShizhuangGradeConfig *grade_cfg = NULL;
	int type = 0;
	if (slot_idx == SHIZHUANG_TYPE_BODY)
	{
		grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangGradeConfig(shizhuang_param.grade);
		type = SHUXINGDAN_TYPE_SHIZHUANG;
	}
	else if (slot_idx == SHIZHUANG_TYPE_WUQI)
	{
		grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponGradeConfig(shizhuang_param.grade);
		type = SHUXINGDAN_TYPE_SHIZHUANG_SHENBING;
	}
	if (NULL == grade_cfg)
	{
		m_role->NoticeNum(slot_idx > SHIZHUANG_TYPE_WUQI ? errornum::EN_SHIZHUANF_SHUXINGDAN_USE_LIMIT_NUM : errornum::EN_SHIZHUANF_SHENBING_SHUXINGDAN_USE_LIMIT_NUM);
		return false;
	}

	const ShuxingdanItemConfig *cfg_item = LOGIC_CONFIG->GetShuxingdanCfg().GetShuxingdanItem(SHUXINGDAN_TYPE_SHIZHUANG, 0);
	if (NULL == cfg_item)
	{
		return false;
	}

	if (shizhuang_param.grade < cfg_item->order_limit)
	{
		gstr::SendError(m_role, "shuxingdan_use_order_limit_1", type, cfg_item->order_limit - 1, 0); // 客户端阶数-1
		return false;
	}

	if (shizhuang_param.shuxingdan_count + use_count > cfg_item->use_limit_num)
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", 0);
		return false;
	}

	int special_img_add_count = 0;
	for (int i = 0; i < SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT; i++)
	{
		if (!this->IsActive(slot_idx, i))
			continue;
		const ShizhuangSpecialImgUpgradeCfg *special_img_grade_cfg = NULL;
		if (slot_idx == SHIZHUANG_TYPE_BODY)
		{
			special_img_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangSpecialImgUpgradeCfg(i, shizhuang_param.special_img_grade_list[i]);
		}
		else if (slot_idx == SHIZHUANG_TYPE_WUQI)
		{
			special_img_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponSpecialImgUpgradeCfg(i, shizhuang_param.special_img_grade_list[i]);
		}
		if (NULL != special_img_grade_cfg)
		{
			special_img_add_count += special_img_grade_cfg->shuxingdan_count;
		}
	}

	// 策划需求，修正真正使用数量
	int can_use_count = grade_cfg->shuxingdan_limit + special_img_add_count - shizhuang_param.shuxingdan_count;
	if (can_use_count > 0)
	{
		if (can_use_count < use_count)
		{
			use_count = can_use_count;
		}
	}
	else
	{
		gstr::SendError(m_role, "shuxingdan_use_limit_num", 0);
		return false;
	}

	shizhuang_param.shuxingdan_count += use_count;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG);
	this->SendInfo();

	return true;
}

bool Shizhuang::UpSkillLevel(Role *role, short shizhuang_type, int skill_idx, bool is_auto_buy)
{
	if (NULL == role || skill_idx < 0 || skill_idx >= MAX_SKILL_COUNT || shizhuang_type < 0 || shizhuang_type >= SHIZHUANG_TYPE_MAX)
	{
		return false;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (m_param.item_list[shizhuang_type].grade <= 0)
	{
		role->NoticeNum(errornum::EN_SHIZHUANG_NOT_ACTIVE);
		return false;
	}

	Shizhuang *shizhuang = role->GetShizhuang();
	if (NULL == shizhuang)
	{
		return false;
	}

	ShizhuangParam *shizhuangparam = shizhuang->GetShiZhuangParam();
	if (NULL == shizhuangparam)
	{
		return false;
	}

	const ShizhuangSkillCfg *target_skill_cfg = NULL;
	switch (shizhuang_type)
	{
	case SHIZHUANG_TYPE_BODY:
	{
		target_skill_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangSkillCfg(skill_idx, shizhuangparam->item_list[shizhuang_type].skill_level_list[skill_idx] + 1);
		break;
	}
	case SHIZHUANG_TYPE_WUQI:
	{
		target_skill_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponSkillCfg(skill_idx, shizhuangparam->item_list[shizhuang_type].skill_level_list[skill_idx] + 1);
		break;
	}
	default:
		break;
	}

	if (NULL == target_skill_cfg)
	{
		return false;
	}

	if (0 == target_skill_cfg->skill_type)
	{
		return false;
	}

	if (target_skill_cfg->shizhuang_grade > shizhuangparam->item_list[shizhuang_type].grade)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list;
	consume_list.Reset();

	{
		// 升级材料
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = target_skill_cfg->uplevel_stuff_id;
		stuff_list[0].num = target_skill_cfg->uplevel_stuff_num;
		stuff_list[0].buyable = is_auto_buy;

		if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
		{
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(role, consume_list, "Shizhuang::UpSkillLevel"))
	{
		return false;
	}

	shizhuangparam->item_list[shizhuang_type].skill_level_list[skill_idx]++;

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG);
	role->GetShizhuang()->SendInfo();
	return true;
}

int Shizhuang::RefreshMinEquipLevel(short shizhuang_type)
{
	int min = m_param.item_list[shizhuang_type].equip_level_list[0];

	for (int i = 1; i < EQUIP_COUNT; ++i)
	{
		min = m_param.item_list[shizhuang_type].equip_level_list[i] < min ? m_param.item_list[shizhuang_type].equip_level_list[i] : min;
	}

	return min;
}

bool Shizhuang::UplevelEquip(Role *role, short shizhuang_type, int equip_idx)
{
	if (NULL == role || equip_idx < 0 || equip_idx >= EQUIP_COUNT || shizhuang_type < 0 || shizhuang_type >= SHIZHUANG_TYPE_MAX)
	{
		return false;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	Shizhuang *shizhuang = role->GetShizhuang();
	if (NULL == shizhuang)
	{
		return false;
	}

	ShizhuangParam *shizhuangparam = shizhuang->GetShiZhuangParam();
	if (NULL == shizhuangparam)
	{
		return false;
	}

	if (m_param.item_list[shizhuang_type].grade <= 0)
	{
		role->NoticeNum(errornum::EN_SHIZHUANG_NOT_ACTIVE);
		return false;
	}

	const ShizhuangEquipInfoCfg *equip_cfg = NULL;
	const ShizhuangGradeConfig * cur_grade_cfg = NULL;
	switch (shizhuang_type)
	{
	case SHIZHUANG_TYPE_BODY:
	{
		equip_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangEquipInfoCfg(equip_idx);
		cur_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangGradeConfig(shizhuangparam->item_list[shizhuang_type].grade);
		break;
	}

	case SHIZHUANG_TYPE_WUQI:
	{
		equip_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponEquipInfoCfg(equip_idx);
		cur_grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetWeaponGradeConfig(shizhuangparam->item_list[shizhuang_type].grade);
		break;
	}

	default:
		break;
	}

	if (NULL == equip_cfg)
	{
		return false;
	}

	short level = shizhuangparam->item_list[shizhuang_type].equip_level_list[equip_idx];
	//if (level >= equip_cfg->max_level - 1)
	//{
	//	role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
	//	return false;
	//}

	if (level < 0)
	{
		return false;
	}

	//策划需求，装备升级级数由时装阶数决定
	if (cur_grade_cfg == NULL)
	{
		return false;
	}

	if (level >= cur_grade_cfg->equip_level_toplimit)
	{
		role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
		return false;
	}

	if (shizhuangparam->item_list[shizhuang_type].grade < LOGIC_CONFIG->GetShizhuangConfig().GetACtiveEquipGrade())
	{
		return false;
	}
	// 消耗
	if (!role->GetKnapsack()->ConsumeItem(equip_cfg->level_cfg[level].item.item_id, equip_cfg->level_cfg[level].item.num, "ShizhuangEquipUplevel"))
	{
		return false;
	}

	// 结算

	shizhuangparam->item_list[shizhuang_type].equip_level_list[equip_idx]++;

	bool is_active_upgrade_skill = false;
	int min_equip_level = this->RefreshMinEquipLevel(shizhuang_type);
	if (LOGIC_CONFIG->GetShizhuangConfig().GetActiveEquipSkillLevel() <= min_equip_level)
	{
		short equip_skill_level = LOGIC_CONFIG->GetShizhuangConfig().GetEquipSkillLevelCfg(min_equip_level);
		if (equip_skill_level > 0)
		{
			shizhuangparam->item_list[shizhuang_type].equip_skill_level = equip_skill_level;
			is_active_upgrade_skill = true;
		}
	}

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG);
	this->SendInfo();

	if (is_active_upgrade_skill)
	{
		CommonDataParam *common_data = role->GetCommonDataParam();
		if (NULL != common_data)
		{
			if (0 == (common_data->upgrade_skill_skill_active_flag & (1 << UpGradeSkillConfig::SKILL_TYPE_SHIZHUANG)))
			{
				common_data->upgrade_skill_skill_active_flag |= (1 << UpGradeSkillConfig::SKILL_TYPE_SHIZHUANG);
				if (-1 == common_data->upgrade_skill_next_skill_index)
				{
					common_data->upgrade_skill_next_skill_index = UpGradeSkillConfig::SKILL_TYPE_SHIZHUANG;
				}

				role->SendGradeSkillInfo(role);
			}
		}
	}
	return true;
}


void Shizhuang::CheckActive()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}
	//检查时装是否激活
	if (m_param.item_list[SHIZHUANG_TYPE_BODY].grade <= 0)
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHIZHUANG);
		if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
			{
				ActiveShiZhuang(SHIZHUANG_TYPE_BODY, true);
			}
		}
	}
	//检查神兵是否激活
	if (m_param.item_list[SHIZHUANG_TYPE_WUQI].grade <= 0)
	{
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
		const FunOpenUnit *fun_open_cfg = LOGIC_CONFIG->GetJoinLimitConfig().GetFunOpenCfg(FUN_OPEN_TYPE_SHENBING);
		if (nullptr != fun_open_cfg && opengame_day >= fun_open_cfg->opengame_day)
		{
			if (m_role->GetLevel() >= fun_open_cfg->role_level && m_role->GetTaskRecorder()->IsFinished(fun_open_cfg->task_id))
			{
				ActiveShiZhuang(SHIZHUANG_TYPE_WUQI, true);
			}
		}
	}
}

bool Shizhuang::IsActive(int part, int index)
{
	if (part < 0 || part >= SHIZHUANG_TYPE_MAX || index <= 0 || index >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
	{
		return false;
	}

	ShizhuangItem &shizhuang_item = m_param.item_list[part];
	return BitmapFunction::IsBitSet(shizhuang_item.special_img_active_flag, sizeof(shizhuang_item.special_img_active_flag), index);
}

bool Shizhuang::IsActiveImage(int part, int index)
{
	if (part < 0 || part >= SHIZHUANG_TYPE_MAX || index <= 0 || index >= SHIZHUANG_MAX_IMG_COUNT)
	{
		return false;
	}

	ShizhuangItem &shizhuang_item = m_param.item_list[part];
	return BitmapFunction::IsBitSet(shizhuang_item.img_active_flag, sizeof(shizhuang_item.img_active_flag), index);
}

// 时装幻化特殊战力加成
void Shizhuang::ShiZhuangHuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add)
{
	if (!LOGIC_CONFIG->GetShizhuangConfig().IsAddSpecialCap())
	{
		return;
	}

	auto cfg_item = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangHuanHuaSpecialCapAddCfg(huanhua_id);
	if (nullptr == cfg_item)
	{
		return;
	}

	if (huanhua_level >= cfg_item->huanhua_level)
	{
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp;
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji;
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu;
	}
}

// 神兵幻化特殊战力加成
void Shizhuang::ShiZhuangWuQiHuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add)
{
	if (!LOGIC_CONFIG->GetShizhuangConfig().IsAddSpecialCap())
	{
		return;
	}

	auto cfg_item = LOGIC_CONFIG->GetShizhuangConfig().GetShizhuangWuqiHuanHuaSpecialCapAddCfg(huanhua_id);
	if (nullptr == cfg_item)
	{
		return;
	}

	if (huanhua_level >= cfg_item->huanhua_level)
	{
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp;
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji;
		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu;
	}
}

int Shizhuang::GetShiZhuangGrade(int shizhuang_type)
{
	if (shizhuang_type < 0 || shizhuang_type >= SHIZHUANG_TYPE_MAX) return 0;

	int grade = m_param.item_list[shizhuang_type].grade;

	return grade;
}

int Shizhuang::GetSpecialImageLevelById(int shizhuang_type, int imageid)
{
	if (shizhuang_type < 0 || shizhuang_type >= SHIZHUANG_TYPE_MAX) return 0;

	if (imageid < 0 || imageid >= SHIZHUANG_MAX_SPECIAL_IMAGE_COUNT)
	{
		return 0;
	}

	if (!this->IsActive(shizhuang_type, imageid))
	{
		return 0;
	}

	return m_param.item_list[shizhuang_type].special_img_grade_list[imageid];
}
