#include "globalconfig.h"
#include "servercommon/frienddef.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/campdef.hpp"
#include "servercommon/servercommon.h"
#include "item/itempool.h"
#include "gamecommon.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "scene/worldshadow/worldshadow.hpp"

using namespace globalconfig;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GlobalConfig *gs_global_config = NULL;

GlobalConfig * GlobalConfig::Instance()
{
	if (NULL == gs_global_config) 
	{
		gs_global_config = new GlobalConfig();
	}

	return gs_global_config;
}

bool GlobalConfig::Reload(const std::string& path, const std::string& configname, std::string* err)
{
	// 搞成static的函数就是防止在类的函数里面delete自己

	GlobalConfig *temp = new GlobalConfig();
	
	if (!temp->Init(path, configname, err))
	{
		delete temp; temp = NULL;
		return false;
	}

	if (NULL != gs_global_config) delete gs_global_config;

	gs_global_config = temp;

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool GlobalConfig::Init(const std::string &path, const std::string &configname, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, (path + configname), "GlobalConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	std::string configpath;

	if (!RootElement)
	{
		*err = configname + ": xml node error in root";
		return false;
	}

	{
		// 加载角色全局配置
		if (!PugiGetSubNodeValue(RootElement, "role", configpath))
		{
			*err = configname + ": xml node error in role";
			return false;
		}
		if (!m_roleconfig.Init(path+configpath, err))
		{
			return false;
		}
	}
	
	{
		// 加载角色经验配置
		if (!PugiGetSubNodeValue(RootElement, "roleexp", configpath))
		{
			*err = configname + ": xml node error in roleexp";
			return false;
		}
		if (!m_rolelevelconfig.Init(path+configpath, err))
		{
			return false;
		}
	}

	//{
	//	// 加载转生配置
	//	if (!PugiGetSubNodeValue(RootElement, "rolezhuansheng", configpath))
	//	{
	//		*err = configname + ": xml node error in rolezhuansheng";
	//		return false;
	//	}
	//	if (!m_zhuanshengconfig.Init(path+configpath, err))
	//	{
	//		return false;
	//	}
	//}

	
	{
		// 处理otherconfig
		if (!PugiGetSubNodeValue(RootElement, "other", configpath))
		{
			*err = configname + ": xml node error in other";
			return false;
		}
		if (!m_other.Init(path+configpath, err))
		{
			return false;
		}
	}

	{
		// 处理keyconfig
		if (!PugiGetSubNodeValue(RootElement, "key", configpath))
		{
			*err = configname + ": xml node error in key";
			return false;
		}
		if (!m_key.Init(path+configpath, err))
		{
			return false;
		}
	}
	
	{
		// 处理equipconfig
		if (!PugiGetSubNodeValue(RootElement, "equip", configpath))
		{
			*err = configname + ": xml node error in equip";
			return false;
		}
		if (!m_equipconfig.Init(path+configpath, err))
		{
			return false;
		}
	}

	{
		// 活动配置
		//m_dynamicconfig

		std::string dynamic_cfg_path;
		int len = static_cast<int>(strlen(path.c_str()) - strlen("gameworld/"));
		
		if (len > 0)
		{
			std::string real_path = path;
			real_path.resize(len);
			dynamic_cfg_path = real_path + "dynamic_cfg.xml";
		}
		else
		{
			dynamic_cfg_path = "../config/dynamic_cfg.xml";
		}

		if (!m_dynamicconfig.Init(dynamic_cfg_path, err))
		{
			return false;
		}
	}

	return true;
}

const unsigned int GlobalConfig::GetOtherConfig_ItemDrop_Interval_Owner() const
{
	return m_other.itemdrop_interval_owner;
}

const unsigned int GlobalConfig::GetOtherConfig_ItemDrop_Disappear_Time() const
{
	return m_other.itemdrop_interval_delete;
}

const unsigned int GlobalConfig::GetOtherConfig_ItemDrop_Disappear_Time_Non_Static_Scene() const
{
	return m_other.itemdrop_interval_delete_no_static_scene;
}

const unsigned int GlobalConfig::GetOtherConfig_ItemDrop_Length() const
{
	return m_other.itemdrop_length;
}

const unsigned int GlobalConfig::GetOtherConfig_Monster_Dead_Delay() const
{
	return m_other.monster_dead_delay;
}

const unsigned int GlobalConfig::GetOtherConfig_Monster_Follow_Interval() const
{
	return m_other.monster_follow_interval;
}

const unsigned int GlobalConfig::GetOtherConfig_SortInOrder_Interval() const
{
	return m_other.bag_sortinorder_interval;
}

const unsigned int GlobalConfig::GetOtherConfig_Fight_Status_Leave_Interval_S() const
{
	return m_other.fight_status_leave_interval_s;
}

const bool GlobalConfig::GetOtherConfig_IsCheckValid() const
{
	return m_other.is_check_valid;
}

const unsigned int GlobalConfig::GetOtherConfig_MaxMoveSpeed() const
{
	return m_other.max_move_speed;
}

const unsigned int GlobalConfig::GetOtherConfig_MoveCheck_Length() const
{
	return m_other.move_check_length;
}

const unsigned int GlobalConfig::GetOtherConfig_MaxTransportLength() const
{
	return m_other.max_transport_length;
}

const int GlobalConfig::GetOtherConfig_KnapsackExtendNeedItemCount(int extend_index, bool is_storage) const
{
	for (int i = m_other.knapsack_extend_cost_cfgcount - 1; i >= 0; -- i)
	{
		if (is_storage)
		{
			if (extend_index >= m_other.knapsack_extend_cost_cfglist[i].min_extend_index_storage)
			{
				return m_other.knapsack_extend_cost_cfglist[i].need_item_count_storage;
			}
		}
		else
		{
			if (extend_index >= m_other.knapsack_extend_cost_cfglist[i].min_extend_index)
			{
				return m_other.knapsack_extend_cost_cfglist[i].need_item_count;
			}
		}
	}

	return 0;
}

const ItemID GlobalConfig::GetOtherConfig_KnapsackExtendBagItemID() const
{
	return m_other.knapsack_extend_bag_item_id;
}

const ItemID GlobalConfig::GetOtherConfig_KnapsackExtendStorageItemID() const
{
	return m_other.knapsack_extend_storage_item_id;
}

const unsigned int GlobalConfig::GetGatherMaxDistance() const
{
	return m_other.max_gather_distance;
}

const unsigned int GlobalConfig::GetNpcMaxTouchDistance() const
{
	return m_other.max_npc_touch_distance;
}

const unsigned int GlobalConfig::GetKeyConfig_ZoneMatrixX() const
{
	return m_key.GetZoneMatrixX();
}
const unsigned int GlobalConfig::GetKeyConfig_ZoneMatrixY() const
{
	return m_key.GetZoneMatrixY();
}

const unsigned int GlobalConfig::GetKeyConfig_RoleAoiX() const
{
	return m_key.GetRoleAoiX();
}
const unsigned int GlobalConfig::GetKeyConfig_RoleAoiY() const
{
	return m_key.GetRoleAoiY();
}
const unsigned int GlobalConfig::GetKeyConfig_RoleAoiLeaveMoreThenEnter() const
{
	return m_key.GetRoleAoiLeaveMoreThenEnter();
}

const unsigned int GlobalConfig::GetKeyConfig_RoleAoiSensitiveDistance() const
{
	return m_key.GetRoleAoiSensitiveDistance();
}
const unsigned int GlobalConfig::GetKeyConfig_PetAoiSensitiveDistance() const
{
	return m_key.GetPetAoiSensitiveDistance();
}

const unsigned int GlobalConfig::GetKeyConfig_RoleSaveTime() const
{
	return m_key.GetRoleSaveTime();
}

bool GlobalConfig::IsGmOpen() const
{
	return 0 != m_other.gm_switch;
}

long long GlobalConfig::GetRoleMaxExp(int level)
{
	return m_rolelevelconfig.GetRoleMaxExp(level);
}

RoleLevelCfg* GlobalConfig::GetRoleLvlCfg(int level)
{
	return m_rolelevelconfig.GetLevelCfg(level);
}

RoleZhuanShengCfg *GlobalConfig::GetRoleZhuanShengCfg(int role_level)
{
	return m_zhuanshengconfig.GetZhuanShengCfg(role_level);
}

int GlobalConfig::GetDefaultSkillID(int prof, int default_skill_id_list[globalconfig::MAX_DEFAULT_SKILL_COUNT])
{
	return m_roleconfig.GetDefaultSkillID(prof, default_skill_id_list);
}

int GlobalConfig::GetFlyIncreaseSpeedPercent()
{
	return m_roleconfig.GetFlyIncreaseSpeedPercent();
}

int GlobalConfig::GetRandomCampItemID()
{
	return m_other.random_camp_itemid;
}

int GlobalConfig::GetRandomCampGoldBind()
{
	return m_other.random_camp_gold_bind;
}

int GlobalConfig::GetIgnoreLevelLimitChongzhi()
{
	return m_other.ignore_level_limit_chongzhi;
}

int GlobalConfig::GetSpeakerLevel()
{
	return m_other.speaker_level;
}

int GlobalConfig::GetClearMentalityCDIntervalPerGold()
{
	return m_other.clear_mentality_cd_interval_per_gold;
}

int GlobalConfig::GetRestLimitLevel()
{
	return m_other.rest_limit_level;
}

int GlobalConfig::GetRestRecoveryInterval()
{
	return m_other.rest_recovery_interval;
}

int GlobalConfig::GetRestRecoveryHpPer()
{
	return m_other.rest_recovery_hp_per;
}


int GlobalConfig::GetRestRewardInterval()
{
	return m_other.rest_reward_interval;
}

int GlobalConfig::GetShuangXiuAddPer()
{
	return m_other.shuangxiu_add_per;
}

int GlobalConfig::GetShuangxiuIntimacyInterval()
{
	return m_other.shuangxiu_intimacy_interval;
}

int GlobalConfig::GetShuangxiuIntimacyAdd()
{
	return m_other.shuangxiu_intimacy_add;
}

int GlobalConfig::GetNameColorEvil(int namecolor)
{
	if (namecolor < NAME_COLOR_WHITE || namecolor >= NAME_COLOR_MAX) return 0;
	return m_other.namecolor_evil_list[namecolor];
}

int GlobalConfig::GetNameColorUpdateIntervalS()
{
	return m_other.namecolor_update_interval_s;
}

int GlobalConfig::GetNameColorMaxEvil()
{
	return m_other.namecolor_max_evil;
}

int GlobalConfig::GetNameColorUpdateDecEvil()
{
	return m_other.namecolor_update_dec_evil;
}

int GlobalConfig::GetNameColorByEvil(int evil)
{
	int namecolor = NAME_COLOR_WHITE;

	for (int i = NAME_COLOR_WHITE; i < NAME_COLOR_MAX; i++)
	{
		if (evil >= m_other.namecolor_evil_list[i])
		{
			namecolor = i;
		}
	}

	return namecolor;
}

bool GlobalConfig::IsRedNameColor(int evil)
{
	return evil >= m_other.namecolor_evil_list[NAME_COLOR_RED_1];
}

int GlobalConfig::GetReduceExpPercentByNameColor(int namecolor)
{
	if (namecolor >= NAME_COLOR_WHITE + 1 && namecolor < NAME_COLOR_MAX)
	{
		return m_other.namecolor_reduce_exp_percent[namecolor];
	}

	return 0;
}

int GlobalConfig::GetReduceHpPercentByNameColor(int namecolor)
{
	if (namecolor >= NAME_COLOR_WHITE + 1 && namecolor < NAME_COLOR_MAX)
	{
		return m_other.namecolor_reduce_hp_per_percent[namecolor];
	}

	return 0;
}

int GlobalConfig::GetReduceGongjiPercentByNameColor(int namecolor)
{
	if (namecolor >= NAME_COLOR_WHITE + 1 && namecolor < NAME_COLOR_MAX)
	{
		return m_other.namecolor_reduce_gongji_per_percent[namecolor];
	}

	return 0;
}

ItemID GlobalConfig::GetRealiveItemId() const
{
	return m_other.realive_item_id;
}

int GlobalConfig::GetRealiveRecoverCommonScenePercent() const
{
	return m_other.realive_recover_common_scene_percent;
}

int GlobalConfig::GetRealiveRecoverFbPercent() const
{
	return m_other.realive_recover_fb_percent;
}

TaskID GlobalConfig::GetRoleBirthTaskId()
{
	return m_other.role_birth_task_id;
}

int GlobalConfig::GetTeamTodayIntimacyLimit()
{
	return m_other.team_today_intimacy_limit;
}

int GlobalConfig::GetTeamIntimacyReward()
{
	return m_other.team_intimacy_reward;
}

int GlobalConfig::GetIntimacyRewardMonsterLevel()
{
	return m_other.intimacy_reward_monster_level;
}

int GlobalConfig::GetFriendMaxNumber(int level)
{
	if (level <= 0 || level > MAX_ROLE_LEVEL) return 0;

	for (int i = 0; i < FRIENDNUM_FOR_LEVEL_COUNT; i++)
	{
		if (level < m_other.friendnum_for_level[i].level)
		{
			return m_other.friendnum_for_level[i].number;
		}
	}

	return MAX_FRIEND_NUM;
}

int GlobalConfig::GetSceneIdByCampType(int camp_type)
{
	switch (camp_type)
	{
	case CAMP_TYPE_FEIXING:
		return m_other.sceneid_of_camp1;

	case CAMP_TYPE_ZHURI:
		return m_other.sceneid_of_camp2;

	case CAMP_TYPE_ZHUIYUE:
		return m_other.sceneid_of_camp3;
	}

	return 0;
}

char GlobalConfig::GetCampTypeOfScene(int scene_id)
{
	if (scene_id == m_other.sceneid_of_camp1) return CAMP_TYPE_FEIXING;
	if (scene_id == m_other.sceneid_of_camp2) return CAMP_TYPE_ZHURI;
	if (scene_id == m_other.sceneid_of_camp3) return CAMP_TYPE_ZHUIYUE;

	return CAMP_TYPE_INVALID;
}

bool GlobalConfig::IsCampScene(int scene_id)
{
	return scene_id == m_other.sceneid_of_camp1 || scene_id == m_other.sceneid_of_camp2 || scene_id == m_other.sceneid_of_camp3;
}

bool GlobalConfig::IsOtherCampScene(char my_camp, int scene_id)
{
	char scene_camp = this->GetCampTypeOfScene(scene_id);

	if (CAMP_TYPE_INVALID == scene_camp) return false;

	if (CAMP_TYPE_INVALID == my_camp) return true;

	if (my_camp != scene_camp) return true;

	return false;
}

int GlobalConfig::GetCtrlEffectReduceCircleS()
{
	return m_other.ctrl_effect_reduce_circle_time;
}

int GlobalConfig::GetCtrlEffectReducePercent(int ctrl_effect_times)
{
	if (ctrl_effect_times < 1) return 100;
	if (ctrl_effect_times >= static_cast<int>(sizeof(m_other.ctrl_effect_reduce_percent_list) / sizeof(m_other.ctrl_effect_reduce_percent_list[0]))) return 0;
	
	return m_other.ctrl_effect_reduce_percent_list[ctrl_effect_times];
}

bool GlobalConfig::GetFriendAttrList(int intimacy, CharIntAttrs &attr_add)
{
	if (intimacy <= 0) return false;

	for (int i = m_other.friend_attr_count - 1; i >= 0; i--)
	{
		if (intimacy >= m_other.friend_attr_list[i].intimacy)
		{
			globalconfig::OtherGlobalConfig::FriendAttrItem *attr_item = &m_other.friend_attr_list[i];
			for (int j = 0; j < m_other.friend_attr_list[i].count && j < globalconfig::OtherGlobalConfig::MAX_FRIEND_ATTR_TYPE_NUM; j++)
			{
				if (CharIntAttrs::IsFightAttr(attr_item->attr_list[j].attr_type, true))
				{
					attr_add.m_attrs[attr_item->attr_list[j].attr_type] += attr_item->attr_list[j].attr_value;
				}
			}

			return true;
		}
	}

	return false;
}

bool GlobalConfig::CheckFriendAttrChange(int old_intimacy, int new_intimacy)
{
	int high_intimacy = new_intimacy >= old_intimacy ? new_intimacy : old_intimacy;
	int low_intimay = new_intimacy < old_intimacy ? new_intimacy : old_intimacy;

	for (int i = m_other.friend_attr_count - 1; i >= 0; i--)
	{
		if (high_intimacy >= m_other.friend_attr_list[i].intimacy)
		{
			if (low_intimay < m_other.friend_attr_list[i].intimacy)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}

char GlobalConfig::GetCampOfMonster(int monster_id)
{
	for (int i = 1; i < CAMP_TYPE_MAX; ++i)
	{
		if (monster_id == m_other.shenjiang_list[i])
		{
			return i;
		}
	}

	return CAMP_TYPE_INVALID;
}

// 不同职业的属性值
Attribute GlobalConfig::GetDiffProfAttrValue(char prof, int attr_type, Attribute value)
{
// 	float hp_per = 1.0f;
// 	float gongji_per = 1.0f;
// 	float fangyu_per = 1.0f;

// 	if(PROF_TYPE_PROF_1 == prof || PROF_TYPE_PROF_ZHUANZHI_1 == prof)
// 	{
// 		hp_per = 1.1103f;
// 		gongji_per = 0.89f;
// 		fangyu_per = 1.0f;
// 	}
// 	if (PROF_TYPE_PROF_2 == prof || PROF_TYPE_PROF_ZHUANZHI_2 == prof)
// 	{	
// 		hp_per = 0.9662f;
// 		gongji_per = 1.03f;
// 		fangyu_per = 1.0f;
// 	}
// 	if (PROF_TYPE_PROF_3 == prof || PROF_TYPE_PROF_ZHUANZHI_3 == prof)
// 	{
// 		hp_per = 1.0356f;
// 		gongji_per = 0.98f;
// 		fangyu_per = 1.0f;
// 	}
// 	if (PROF_TYPE_PROF_4 == prof || PROF_TYPE_PROF_ZHUANZHI_4 == prof)
// 	{
// 		hp_per = 0.8832f;
// 		gongji_per = 1.09f;
// 		fangyu_per = 1.0f;
// 	}
// 
// 	if (CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP == attr_type && value != 0)
// 	{
// 		return static_cast<int>(value * hp_per);
// 	}
// 	
// 	if (CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI == attr_type && value != 0)
// 	{
// 		return static_cast<int>(value * gongji_per);
// 	}
// 
// 	if (CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU == attr_type && value != 0)
// 	{
// 		return static_cast<int>(value * fangyu_per);
// 	}

	return value;
}

int GlobalConfig::GetPublicSaleOnceTax(int price)
{
	for (int i = OtherGlobalConfig::MAX_PUBLIC_SALE_TAX_NUM - 1; i >= 0; -- i)
	{
		OtherGlobalConfig::PublicsaleTaxCfg &tax_cfg = m_other.publicsale_once_tax_list[i];
		if (tax_cfg.gold != 0 && price >= tax_cfg.gold)
		{
			return tax_cfg.tax;
		}
	}

	return 0;
}

double GlobalConfig::GetMonsterRewardLevelFactor(int role_level, int delta_level, bool is_monster_high)
{
	std::vector<globalconfig::OtherGlobalConfig::LevelFactor> m_level_factor_vec;
	if (is_monster_high)
	{
		m_level_factor_vec = m_other.m_high_level_factor_vec;
	}
	else
	{
		m_level_factor_vec = m_other.m_low_level_factor_vec;
	}

	for (std::vector<globalconfig::OtherGlobalConfig::LevelFactor>::reverse_iterator r_iter = m_level_factor_vec.rbegin(); r_iter != m_level_factor_vec.rend(); r_iter++)
	{
		if (role_level >= (*r_iter).role_level)
		{
			for (std::vector<globalconfig::OtherGlobalConfig::MonsterDeltaLevel>::reverse_iterator r_it = (*r_iter).monster_delta_level_vec.rbegin(); r_it != (*r_iter).monster_delta_level_vec.rend(); r_it++)
			{
				if (delta_level >= (*r_it).delta_level)
				{
					return (*r_it).factor;
				}
			}
		}
	}

	return 1.0;
}

bool GlobalConfig::CanAutoExtendKnapsack(int count, unsigned int online_time)
{
	if (count >= static_cast<int>(m_other.knapsack_grid_auto_add_time_vec.size()))
	{
		return false;
	}

	if (online_time < m_other.knapsack_grid_auto_add_time_vec[count])
	{
		return false;
	}

	return true;
}
