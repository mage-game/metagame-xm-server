#include "tianshuxunzhu.hpp"
#include "tianshuxunzhuconfig.hpp"
#include "gameworld/config/logicconfigmanager.hpp"
#include "gameworld/item/knapsack.h"
#include "gameworld/equipment/equipmentmanager.h"
#include "other/capability/capability.hpp"
#include "crossserver/config/sharedconfig.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/baizhanequip/baizhanequip.h"
#include "other/shenshou/shenshou.hpp"
#include "other/zhuanzhiequip/zhuanzhiequip.h"

void TianShuXunZhu::CheckGuBenTianShu(Role *role, long long param)
{
	if (NULL == role)
	{
		return;
	}

	CommonDataParam *common_data = role->GetCommonDataParam();

	common_data->tianshu_xunzhu_zhuanzhi_equip_fangyu = param;

	bool have_act = false;
	const TianShuXZGoalConfig * goal_config = LOGIC_CONFIG->GetTianshuXZConfig().GetTianShuXZGoalConfig(TIANSHUXZ_TYPE_EXP);
	if(goal_config != NULL)
	{
		for (int i = 0; i < goal_config->count && i < TIANSHU_XUNZHU_SIGNLE_MAX_NUM; ++i)
		{
			// 已激活
			if (0 != (common_data->tianshu_xunzhu_active_flag_list[TIANSHUXZ_TYPE_EXP] & (1 << i)))
			{
				continue;
			}

			if (param >= goal_config->golal_list[i].cond_param[0])
			{
				common_data->tianshu_xunzhu_active_flag_list[TIANSHUXZ_TYPE_EXP] |= (1 << i);
				have_act = true;
			}
		}
	}

	if (have_act)
	{
		TianShuXunZhu::SendTianshuXunzhuInfo(role);
	}
}

void TianShuXunZhu::CheckShiXueTianShu(Role *role, int param, int param2)
{
	if (NULL == role)
	{
		return;
	}

	const TianShuXZOtherCfg & other_cfg = LOGIC_CONFIG->GetTianshuXZConfig().GetOtherCfg();
	int open_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() +1 ;

	if (open_day > other_cfg.shixue_open_day_end)
	{
		return;
	}

	CommonDataParam *common_data = role->GetCommonDataParam();

	bool have_act = false;
	const TianShuXZGoalConfig * goal_config = LOGIC_CONFIG->GetTianshuXZConfig().GetTianShuXZGoalConfig(TIANSHUXZ_TYPE_SHIXUE);
	if (goal_config != NULL)
	{
		for (int i = 0; i < goal_config->count && i < TIANSHU_XUNZHU_SIGNLE_MAX_NUM; ++i)
		{
			// 已激活
			if (0 != (common_data->tianshu_xunzhu_active_flag_list[TIANSHUXZ_TYPE_SHIXUE] & (1 << i)))
			{
				continue;
			}

// 			if (goal_config->golal_list[i].cond_param[0] == CHENGZHANG_TIANSHU_TYPE_ZERO_BUY_RETURN)
// 			{
// 				if (2 == goal_config->golal_list[i].cond_param.size()&& param == goal_config->golal_list[i].cond_param[0] && param2 == goal_config->golal_list[i].cond_param[1])
// 				{
// 					common_data->tianshu_xunzhu_active_flag_list[TIANSHUXZ_TYPE_SHIXUE] |= (1 << i);
// 					have_act = true;
// 				}
// 			}
// 			else
			{
				if(param == goal_config->golal_list[i].cond_param[0])
				{
					common_data->tianshu_xunzhu_active_flag_list[TIANSHUXZ_TYPE_SHIXUE] |= (1 << i);
					have_act = true;
				}
			}
		}
	}

	if (have_act)
	{
		TianShuXunZhu::SendTianshuXunzhuInfo(role);
	}
}

void TianShuXunZhu::CheckTuMoTianShu(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	CommonDataParam *common_data = role->GetCommonDataParam();
	EquipmentManager *equip_mgr = role->GetEquipmentManager();

	bool have_act = false;
	const TianShuXZGoalConfig * goal_config = LOGIC_CONFIG->GetTianshuXZConfig().GetTianShuXZGoalConfig(TIANSHUXZ_TYPE_TUMO);
	if (goal_config != NULL)
	{
		int level = 0; int equip_count = 0;
		for (int i = 0; i < goal_config->count && i < TIANSHU_XUNZHU_SIGNLE_MAX_NUM; ++i)
		{
			// 已激活
			if (0 != (common_data->tianshu_xunzhu_active_flag_list[TIANSHUXZ_TYPE_TUMO] & (1 << i)))
			{
				continue;
			}
			if(goal_config->golal_list[i].cond_param.size() == 2)
			{
				if (level != goal_config->golal_list[i].cond_param[0])
				{
					level = goal_config->golal_list[i].cond_param[0];
					equip_count = equip_mgr->GetEquipNumLevelBigger(level);
				}
				if (equip_count >= goal_config->golal_list[i].cond_param[1])
				{
					have_act = true;
					common_data->tianshu_xunzhu_active_flag_list[TIANSHUXZ_TYPE_TUMO] |= (1 << i);
				}
			}
		}
	}
	
	if (have_act)
	{
		TianShuXunZhu::SendTianshuXunzhuInfo(role);
	}
}

void TianShuXunZhu::CheckShouHuTianShu(Role *role, int equip_part, int grade)
{
	if (NULL == role)
	{
		return;
	}

	CommonDataParam *common_data = role->GetCommonDataParam();

	bool have_act = false;

	int count = role->GetZhuanzhiEquip()->GetForgeZhuanzhiNum();
	const TianShuXZGoalConfig * goal_config = LOGIC_CONFIG->GetTianshuXZConfig().GetTianShuXZGoalConfig(TIANSHUXZ_TYPE_SHOUHU);
	if (goal_config != NULL && count > 0)
	{
		for (int i = 0; i < goal_config->count && i < TIANSHU_XUNZHU_SIGNLE_MAX_NUM; ++i)
		{
			// 已激活
			if (0 != (common_data->tianshu_xunzhu_active_flag_list[TIANSHUXZ_TYPE_SHOUHU] & (1 << i)))
			{
				continue;
			}
// 			if (goal_config->golal_list[i].cond_param.size() == 2)
// 			{
// 				if (equip_part == goal_config->golal_list[i].cond_param[0] && grade >= goal_config->golal_list[i].cond_param[1])
// 				{
// 					common_data->tianshu_xunzhu_active_flag_list[TIANSHUXZ_TYPE_SHOUHU] |= (1 << i);
// 					have_act = true;
// 				}
// 			}

			if (count >= goal_config->golal_list[i].cond_param[0])
			{
				common_data->tianshu_xunzhu_active_flag_list[TIANSHUXZ_TYPE_SHOUHU] |= (1 << i);
				have_act = true;
			}
		}
	}

	if (have_act)
	{
		TianShuXunZhu::SendTianshuXunzhuInfo(role);
	}
}

void TianShuXunZhu::CheckBaiZhanTianshu(Role * role)
{
	if (NULL == role)
	{
		return;
	}

	CommonDataParam *common_data = role->GetCommonDataParam();
	//EquipmentManager *equip_mgr = role->GetEquipmentManager();

	bool have_act = false;
	const TianShuXZGoalConfig * goal_config = LOGIC_CONFIG->GetTianshuXZConfig().GetTianShuXZGoalConfig(TIANSHUXZ_TYPE_BAIZHAN);
	if (goal_config != NULL)
	{
		int equip_count = role->GetBaizhanEquip()->HaveEquipNum();
		for (int i = 0; i < goal_config->count && i < TIANSHU_XUNZHU_SIGNLE_MAX_NUM; ++i)
		{
			// 已激活
			if (0 != (common_data->tianshu_xunzhu_active_flag_list[TIANSHUXZ_TYPE_BAIZHAN] & (1 << i)))
			{
				continue;
			}
			if (equip_count >= goal_config->golal_list[i].cond_param[0])
			{
				have_act = true;
				common_data->tianshu_xunzhu_active_flag_list[TIANSHUXZ_TYPE_BAIZHAN] |= (1 << i);
			}
		}
	}

	if (have_act)
	{
		TianShuXunZhu::SendTianshuXunzhuInfo(role);
	}
}

void TianShuXunZhu::CheckShenShouTianshu(Role * role)
{
	if (NULL == role)
	{
		return;
	}

	CommonDataParam *common_data = role->GetCommonDataParam();
	//EquipmentManager *equip_mgr = role->GetEquipmentManager();

	bool have_act = false;
	const TianShuXZGoalConfig * goal_config = LOGIC_CONFIG->GetTianshuXZConfig().GetTianShuXZGoalConfig(TIANSHUXZ_TYPE_SHENSHOU);
	if (goal_config != NULL)
	{
		for (int i = 0; i < goal_config->count && i < TIANSHU_XUNZHU_SIGNLE_MAX_NUM; ++i)
		{
			// 已激活
			if (0 != (common_data->tianshu_xunzhu_active_flag_list[TIANSHUXZ_TYPE_SHENSHOU] & (1 << i)))
			{
				continue;
			}
			if (role->GetShenshou()->IsActiveShenshou(goal_config->golal_list[i].cond_param[0]))
			{
				have_act = true;
				common_data->tianshu_xunzhu_active_flag_list[TIANSHUXZ_TYPE_SHENSHOU] |= (1 << i);
			}
		}
	}

	if (have_act)
	{
		TianShuXunZhu::SendTianshuXunzhuInfo(role);
	}
}

void TianShuXunZhu::OnTianshuXunzhuFetchReward(Role *role, int type, int idx)
{
	if (NULL == role || type < 0 || type >= TIANSHU_TYPE_MAX || idx < 0 || idx >= TIANSHU_XUNZHU_SIGNLE_MAX_NUM)
	{
		return;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	const UInt32 active_flag = role->GetCommonDataParam()->tianshu_xunzhu_active_flag_list[type];
	UInt32 &fetch_flag = role->GetCommonDataParam()->tianshu_xunzhu_goal_fetch_flag_list[type];


	const TianShuXZGoalConfig *  goal_config = LOGIC_CONFIG->GetTianshuXZConfig().GetTianShuXZGoalConfig(type);
	if (NULL == goal_config) return;

	

	if (0 == (active_flag & (1 << idx)))
	{
		role->NoticeNum(errornum::EN_DANBI_CHONGZHI_REWARD_HAS_NOT_ACTIVE);
		return;
	}

	if (0 != (fetch_flag & (1 << idx)))
	{
		role->NoticeNum(errornum::EN_CSA_HAS_FETCH_REWARD);
		return;
	}

	// 消耗
	fetch_flag |= (1 << idx);

	// 给予
	if (idx < UINT_LAST_BIT)
	{
		role->GetKnapsack()->PutListOrMail(goal_config->golal_list[idx].goal_reward, PUT_REASON_TIANSHUXZ_REWARD);
	}
	else if (UINT_LAST_BIT == idx)
	{
		// 传闻
		int sendlen = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_tianshuxunzhu_fretch_reward,
			role->GetUID(), (const char*)role->GetName(), (int)role->GetCamp(), type);

		if (sendlen > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, sendlen, SYS_MSG_CENTER_ROLL);
		}

		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_TIANSHUXUNZHU);
	}


	// 该天书所有奖励领取完，激活终极奖励
	{
		bool is_act_final = true;
		for (int i = 0; i < goal_config->count && i < TIANSHU_XUNZHU_SIGNLE_MAX_NUM; ++i)
		{
			if (0 == (fetch_flag & (1 << i)))
			{
				is_act_final = false;
			}
		}

		if (is_act_final)
		{
			role->GetCommonDataParam()->tianshu_xunzhu_active_flag_list[type] |= (1 << UINT_LAST_BIT);
		}
	}


	// 发送
	TianShuXunZhu::SendTianshuXunzhuInfo(role);
}

void TianShuXunZhu::OnRoleLogin(Role *role)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		CheckTuMoTianShu(role);
		CheckBaiZhanTianshu(role);
		CheckShenShouTianshu(role);
		CheckShouHuTianShu(role,0,0);
		if (NULL != role && role->GetLastMarryTime() != 0)
		{
			CheckShiXueTianShu(role, CHENGZHANG_TIANSHU_TYPE_ONMARRY);
		}
	}

	SendTianshuXunzhuInfo(role);
}

void TianShuXunZhu::ReCalcAttr(Role * role, CharIntAttrs &base_attr, bool is_recalc)
{
	CharIntAttrs attrs_add;
	attrs_add.Reset();

	//CommonDataParam * common_data = role->GetCommonDataParam();

	int hurt_boss = GetSkillValue(role, TIANSHUXZ_TYPE_TUMO);
	if (hurt_boss > 0)
	{
		attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER,hurt_boss);
	}
	
	int skill_zengshang = GetSkillValue(role, TIANSHUXZ_TYPE_SHIXUE);
	if (skill_zengshang >0)
	{
		attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER, skill_zengshang);
	}

	int pvp_jianshang = GetSkillValue(role, TIANSHUXZ_TYPE_SHOUHU);
	if (pvp_jianshang >0 )
	{
		attrs_add.AddValue(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER, pvp_jianshang);
	}
	
	int gedang_dikang_per = GetSkillValue(role, TIANSHUXZ_TYPE_BAIZHAN);
	if (gedang_dikang_per > 0)
	{
		attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_PER, gedang_dikang_per);
	}

	int huixinyiji = GetSkillValue(role, TIANSHUXZ_TYPE_SHENSHOU);
	if (huixinyiji > 0)
	{
		attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI, huixinyiji);
	}

	base_attr.Add(attrs_add.m_attrs, true);
	base_attr.AddPercent(attrs_add.m_percent_attrs);

	role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_TIANSHUXUNZHU, attrs_add);
}

void TianShuXunZhu::SendTianshuXunzhuInfo(Role *role)
{
	if (NULL == role)
	{
		return;
	}
	UNSTD_STATIC_CHECK(Protocol::SCTianShuXZInfo::TIANSHU_XUNZHU_MAX_TYPE_COUNT == TIANSHU_XUNZHU_MAX_TYPE_COUNT)

	Protocol::SCTianShuXZInfo msg;
	memset(msg.fetch_flag_list, 0, sizeof(msg.fetch_flag_list));
	memset(msg.act_flag_list, 0, sizeof(msg.act_flag_list));

	CommonDataParam * common_data = role->GetCommonDataParam();
	msg.zhuanzhi_equip_fangyu = common_data->tianshu_xunzhu_zhuanzhi_equip_fangyu;
	msg.equip_level_50 = role->GetEquipmentManager()->GetEquipNumLevelBigger(TIANSHUXUNZHU_EQUIP_LEVEL_1);
	msg.equip_level_100 = role->GetEquipmentManager()->GetEquipNumLevelBigger(TIANSHUXUNZHU_EQUIP_LEVEL_2);
	msg.baizhan_equip_num = role->GetBaizhanEquip()->HaveEquipNum();
	msg.reserve_sh = 0;

	for (int i = 0; i < TIANSHU_TYPE_MAX && i < TIANSHU_XUNZHU_MAX_TYPE_COUNT; ++i)
	{
		msg.fetch_flag_list[i] =common_data->tianshu_xunzhu_goal_fetch_flag_list[i];
		msg.act_flag_list[i] = common_data->tianshu_xunzhu_active_flag_list[i];
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&msg, sizeof(msg));
}

int TianShuXunZhu::GetExtraExp(Role * role)
{
	return GetSkillValue(role, TIANSHUXZ_TYPE_EXP);
}

int TianShuXunZhu::OnHurtBoss(Role * role)
{
	return GetSkillValue(role, TIANSHUXZ_TYPE_TUMO);
}

int TianShuXunZhu::GetSkillZengShang(Role * role)
{
	return GetSkillValue(role,TIANSHUXZ_TYPE_SHIXUE);
}

int TianShuXunZhu::GetPvpJianSHang(Role * role)
{
	return GetSkillValue(role, TIANSHUXZ_TYPE_SHOUHU);
}

int TianShuXunZhu::GetGeDangDikang(Role* role)
{
	return GetSkillValue(role, TIANSHUXZ_TYPE_BAIZHAN);
}

int TianShuXunZhu::GetHuiXinYiJi(Role * role)
{
	return GetSkillValue(role, TIANSHUXZ_TYPE_SHENSHOU);
}

int TianShuXunZhu::GetSkillValue(Role * role, int tianshu_type)
{
	if (role == NULL || tianshu_type < 0 || tianshu_type >= TIANSHU_TYPE_MAX)
	{
		return 0;
	}

	CommonDataParam * common_data = role->GetCommonDataParam();
	if (0 == (common_data->tianshu_xunzhu_goal_fetch_flag_list[tianshu_type] & (1 << UINT_LAST_BIT)))
	{
		return 0;
	}

	const TianshuXZSkillCfg * skill_cfg = LOGIC_CONFIG->GetTianshuXZConfig().GetSKillCfg(tianshu_type);
	if (NULL != skill_cfg)
	{
		return skill_cfg->effect_per_val;
	}

	return 0;
}