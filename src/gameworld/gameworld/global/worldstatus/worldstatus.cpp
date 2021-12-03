#include "worldstatus.hpp"

#include "global/rmibackobjdef.h"
#include "internalcomm.h"
#include "engineadapter.h"
#include "global/camp/campmanager.hpp"
#include "global/rank/topranker.hpp"
#include "global/challengefield/challengefield.hpp"
#include "world.h"
#include "global/usercache/usercache.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "global/worldevent/worldeventmanager.hpp"
#include "global/combineserveractivity/combineserveractivitymanager.hpp"
#include "other/event/eventhandler.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"
#include "scene/speciallogic/worldspecial/campdefendmanager.hpp"
#include "other/famousman/famousmanconfig.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/serverconfig/localconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/questionconfig.hpp"
#include "servercommon/errornum.h"
#include "other/route/mailroute.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "globalconfig/namecreateconfig.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "other/shizhuang/shizhuang.h"
#include "protocal/msgshop.h"
#include "global/bossfamily/bossfamilymanager.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/itemlevelreward/itemlevelreward.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/vip/vip.hpp"
#include "other/cross/rolecross.h"
#include "config/agentadapterconfig.hpp"
#include "config/dynamicadjustconfig.hpp"
#include "global/crossrandactivity/crossramanagerlocal.hpp"
#include "gameworld/scene/activityshadow/activityshadow.hpp"

WorldStatus::WorldStatus()
	: m_data_state(WORLDSTATE_DATA_STATUS_INVALID), m_start_time(0), m_world_time(0), m_last_update_dayid(0), m_xianmengzhan_mengzhu_id(0),
	m_challengefield_king_userid(0), m_question_id(0), m_question_answer(-1), m_question_cur_begin(0), m_question_cur_end(0), m_question_next_begin(0),
	m_question_cur_right_num(0), m_chongzhi_count_next_refresh_time(0), m_can_refresh_chongzhi_count(0), m_first_charge_fake_num(0)
{
	memset(m_question_fast_uid, 0, sizeof(m_question_fast_uid));
}

WorldStatus::~WorldStatus()
{

}

void WorldStatus::Init(const WorldStatusParam &worldstatus_param)
{
	m_start_time = worldstatus_param.start_time;
	m_world_time = worldstatus_param.world_time;
	m_last_update_dayid = worldstatus_param.last_update_dayid;
	m_xianmengzhan_mengzhu_id = worldstatus_param.xianmengzhan_mengzhu_id;
	m_first_charge_fake_num = worldstatus_param.common_data.first_charge_fake_num;

	if (0 == m_start_time) m_start_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (0 == m_last_update_dayid) m_last_update_dayid = static_cast<unsigned int>(EngineAdapter::Instance().DayID());
	if (0 == m_common_data.last_reset_role_module_dayid) m_common_data.last_reset_role_module_dayid = static_cast<unsigned int>(EngineAdapter::Instance().DayID());
	if (0 == m_common_data.last_reset_role_modlue_timestamp) m_common_data.last_reset_role_modlue_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	m_activity_open_record = worldstatus_param.activity_open_records;
	m_common_data = worldstatus_param.common_data;
	m_cross_guildbattle_param = worldstatus_param.cross_guildbattle_param;

	ChallengeField::Instance().Init(worldstatus_param.challenge_field_data);
	
	WorldEventManager::Instance().Init(worldstatus_param.world_event_param);
	CombineServerActivityManager::Instance().Init(worldstatus_param.csa_activity_rank_param, worldstatus_param.csa_activity_other_param,
		worldstatus_param.csa_boss_rank_param);
	BossFamilyManager::Instance().Init(worldstatus_param.boss_refresh_param);

	CrossRAManagerLocal::Instance().Init(worldstatus_param.cross_rand_activity_param, worldstatus_param.cross_rand_activity_big_param);

	CrossRAManagerLocal::Instance().Init2(worldstatus_param.cross_rand_activity_big_param2);

	this->LoadWorldStatusSucc();
}

void WorldStatus::OnServerStart()
{
	this->LoadWorldStatus(0);
	this->CalcNextQuestionTime();

	bool is_open_fake = false;
	const char *plat = LocalConfig::Instance().GetPlatName();
	if (NULL != plat)
	{
		const std::string plat_name(plat);
		is_open_fake = LOGIC_CONFIG->GetAgentAdapterConfig().GetOpenFirstChargeFakeCfgByPlatName(plat_name);

		const FirstChargeFakeCfg *cfg = LOGIC_CONFIG->GetAgentAdapterConfig().GetOpenFirstChargeFakeCfg(plat_name);

		if (cfg != NULL)
		{
			if (is_open_fake)
			{
				int fix_num = 0;
				fix_num = static_cast<int>(LOGIC_CONFIG->GetRandActivityCfg().GetFirstChargeTuanFixNumOnInit()
					* (RandomNum(cfg->min_percent, cfg->max_percent) * 0.01));

				if (fix_num > 0)
				{
					if (m_common_data.today_chongzhi_role_count < fix_num)			//如果实际充值人数少于配置充值人数，修正
					{
						if (fix_num > this->GetTodayFirstChongzhiFakeCount())
						{
							m_first_charge_fake_num = fix_num;
						}
					}
				}

				int next_refresh_time = LOGIC_CONFIG->GetRandActivityCfg().GetFirstChargeTuanRefreshTime();
				if (next_refresh_time > 0)
				{
					m_chongzhi_count_next_refresh_time = next_refresh_time;
					m_can_refresh_chongzhi_count = true;
				}
			}
		}
	}
}

void WorldStatus::OnServerStop()
{
	this->SaveWorldStatus(); 
}

void WorldStatus::OnEnterScene(Role *role)
{
}

void WorldStatus::Update(unsigned long interval, time_t now_second)
{
	if (WORLDSTATE_DATA_STATUS_LOAD_FINISH != m_data_state) return;

	static time_t last_save_time = now_second;

	if (now_second >= last_save_time + WORLDSTATUS_SAVE_INTERVAL)
	{
		m_world_time += static_cast<int>(now_second - last_save_time);	

		this->SaveWorldStatus(); 

		last_save_time = now_second;
	}

	if (m_question_next_begin > 0 && static_cast<unsigned int>(now_second) > m_question_next_begin)
	{
		const WGQuestionCfg *question_cfg = LOGIC_CONFIG->GetQuestionConfig().GetWGQuestionCfg(QUESTION_TYPE_WORLD);
		if (NULL == question_cfg)
		{
			m_question_next_begin = 0;
			return;
		}

		m_question_id = LOGIC_CONFIG->GetQuestionConfig().RandQuestionID(&m_question_answer, QUESTION_TYPE_WORLD);
		m_question_cur_begin = static_cast<unsigned int>(now_second);
		m_question_cur_end = static_cast<unsigned int>(now_second) + question_cfg->answer_continue_time;
		this->CalcNextQuestionTime();

		this->BrocastWorldQuestion(NULL);
	}

	if (m_question_cur_end > 0 && static_cast<unsigned int>(now_second) > m_question_cur_end)
	{
		m_question_id = 0;
		m_question_answer = -1;
		m_question_cur_begin = 0;
		m_question_cur_end = 0;
		m_question_cur_right_num = 0;

		UserCacheNode *first_user_node = UserCacheManager::Instance().GetUserNode(m_question_fast_uid[0]);
		UserCacheNode *second_user_node = UserCacheManager::Instance().GetUserNode(m_question_fast_uid[1]);
		UserCacheNode *third_user_node = UserCacheManager::Instance().GetUserNode(m_question_fast_uid[2]);
		if (NULL != first_user_node && NULL != second_user_node && NULL != third_user_node)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_question_fast_world_reward,
				first_user_node->uid, first_user_node->user_name, (int)first_user_node->camp,
				second_user_node->uid, second_user_node->user_name, (int)second_user_node->camp,
				third_user_node->uid, third_user_node->user_name, (int)third_user_node->camp);
			if (length)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
			}
		}

		memset(m_question_fast_uid, 0, sizeof(m_question_fast_uid));
	}

	//修正实时充值人数
	{
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN))
		{
			bool is_open_fake = false;

			const char *plat = LocalConfig::Instance().GetPlatName();
			if (NULL != plat)
			{
				const std::string plat_name(plat);
				is_open_fake = LOGIC_CONFIG->GetAgentAdapterConfig().GetOpenFirstChargeFakeCfgByPlatName(plat_name);

				const FirstChargeFakeCfg *cfg = LOGIC_CONFIG->GetAgentAdapterConfig().GetOpenFirstChargeFakeCfg(plat_name);

				if (cfg != NULL)
				{
					if (is_open_fake)
					{
						if (m_chongzhi_count_next_refresh_time == 0)
						{
							int next_refresh_time = LOGIC_CONFIG->GetRandActivityCfg().GetFirstChargeTuanRefreshTime();
							if (next_refresh_time > 0)
							{
								m_chongzhi_count_next_refresh_time = next_refresh_time;
								m_can_refresh_chongzhi_count = true;
							}
						}

						if (now_second >= m_chongzhi_count_next_refresh_time)
						{
							if (m_can_refresh_chongzhi_count)
							{
								unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(EngineAdapter::Instance().Time()));

								int fix_num = 0;
								fix_num = static_cast<int>(LOGIC_CONFIG->GetRandActivityCfg().GetFirstChargeTuanFixChargeTotalNum(m_chongzhi_count_next_refresh_time - today_zero_time) 
									* (RandomNum(cfg->min_percent, cfg->max_percent) * 0.01));

								if (fix_num > 0)
								{
									if (m_common_data.today_chongzhi_role_count < fix_num)			//如果实际充值人数少于配置充值人数，修正
									{
										if (fix_num > this->GetTodayFirstChongzhiFakeCount())
										{
											m_first_charge_fake_num = fix_num;
										}

										std::set<Role*> role_list;
										World::GetInstWorld()->GetSceneManager()->GetAllRole(role_list);
										for (auto role : role_list)
										{
											role->GetRoleActivity()->SendRAOpenServerSingleInfo(RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN);
										}
									}
								}

								m_can_refresh_chongzhi_count = false;

								int next_refresh_time = 0;
								next_refresh_time = LOGIC_CONFIG->GetRandActivityCfg().GetFirstChargeTuanRefreshTime();

								if (next_refresh_time > 0)
								{
									m_chongzhi_count_next_refresh_time = next_refresh_time;
									m_can_refresh_chongzhi_count = true;
								}
							}
						}
					}
				}
			}
		}
	}
}

void WorldStatus::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;

	m_activity_open_record.OnResetData();
	m_common_data.yesterday_chongzhi_role_count = m_common_data.today_chongzhi_role_count;
	m_common_data.today_chongzhi_role_count = 0;
	m_common_data.new_cornucopia_total_value = 0;

	if (m_common_data.total_join_darknight_role_count > 0)
	{
		m_common_data.total_join_darknight_average_level = m_common_data.total_join_darknight_role_level / m_common_data.total_join_darknight_role_count;
	}

	m_common_data.total_join_darknight_role_level = 0;
	m_common_data.total_join_darknight_role_count = 0;

	m_chongzhi_count_next_refresh_time = 0;
	m_first_charge_fake_num = 0;
}

bool WorldStatus::LoadWorldStatus(long long id_fram)
{
	if (WORLDSTATE_DATA_STATUS_LOAD_FINISH == m_data_state) return false;

	m_data_state = WORLDSTATE_DATA_STATUS_LOADING;

	RMIInitWorldStatusBackObjectImpl *impl = new RMIInitWorldStatusBackObjectImpl();

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	return gc.InitWorldStatusAsyn(id_fram, impl);	
}

void RMIInitWorldStatusBackObjectImpl::InitWorldStatusRet(int ret, const WorldStatusParam &worldstatus_param)
{
	if (ret < 0)
	{
		printf(" RMIInitWorldStatusBackObjectImpl::InitWorldStatusRet Failed \n");
		EngineAdapter::Instance().StopGame();
		return;
	}

	WorldStatus::Instance().Init(worldstatus_param);
}

void WorldStatus::LoadWorldStatusSucc()
{
	if (WORLDSTATE_DATA_STATUS_LOAD_FINISH != m_data_state)
	{
		m_data_state = WORLDSTATE_DATA_STATUS_LOAD_FINISH;
		CampManager::Instance().OnWorldStatusFirstStart();
		CampDefendMgr::Instance().OnWorldStatusFirstStart();
	}
}

bool WorldStatus::SaveWorldStatus()
{
	static WorldStatusParam worldstate_param; 

	worldstate_param.start_time = m_start_time;
	worldstate_param.world_time = m_world_time;
	worldstate_param.last_update_dayid = m_last_update_dayid;
	worldstate_param.activity_open_records = m_activity_open_record;
	worldstate_param.common_data = m_common_data;
	worldstate_param.cross_guildbattle_param = m_cross_guildbattle_param;
	worldstate_param.xianmengzhan_mengzhu_id = m_xianmengzhan_mengzhu_id;

	ChallengeField::Instance().GetInitParam(&worldstate_param.challenge_field_data);
	WorldEventManager::Instance().GetInitParam(&worldstate_param.world_event_param);
	CombineServerActivityManager::Instance().GetInitParam(&worldstate_param.csa_activity_rank_param, &worldstate_param.csa_activity_other_param,
		&worldstate_param.csa_boss_rank_param);
	BossFamilyManager::Instance().GetInitParam(&worldstate_param.boss_refresh_param);
	CrossRAManagerLocal::Instance().GetInitParam(&worldstate_param.cross_rand_activity_param, &worldstate_param.cross_rand_activity_big_param);
	CrossRAManagerLocal::Instance().GetInitParam2(&worldstate_param.cross_rand_activity_big_param2);

	RMISaveWorldStatusBackObjectImpl *impl = new RMISaveWorldStatusBackObjectImpl();

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());
	return gc.SaveWorldStatusAsyn(worldstate_param, impl);
}

WorldStatus & WorldStatus::Instance()
{
	static WorldStatus ws;
	return ws;
}

unsigned int WorldStatus::GetLastUpdateDayId()
{
	return m_last_update_dayid;
}

void WorldStatus::SetLastUpdateDayId( unsigned int dayid )
{
	m_last_update_dayid = dayid;
}

bool WorldStatus::IsLoadFinish() const
{
	return (WORLDSTATE_DATA_STATUS_LOAD_FINISH == m_data_state);
}

void WorldStatus::OnRecordActivityClose(int activity_type)
{
	switch (activity_type)
	{

	case ACTIVITY_TYPE_QUNXIANLUANDOU:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_QUNXIANLUANDOU);
	}
	break;
	
	case CROSS_ACTIVITY_TYPE_FISHING:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_FISHING);
	}
	break;

	case ACTIVITY_TYPE_GUILD_QUESTION:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_BANGPAIDATI);
	}
	break;

	case ACTIVITY_TYPE_SHUIJING:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_SHUIJING);
	}
	break;

	case ACTIVITY_TYPE_WORSHIP:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_WORSHIP);
	}
	break;
	
	case ACTIVITY_TYPE_NIGHT_FIGHT_FB:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_NIGHT_FIGHT_FB);
	}
	break;

	case ACTIVITY_TYPE_MESS_BATTLE_FB:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_MESS_BATTLE_FB);
	}
	break;

	case ACTIVITY_TYPE_GUILD_YUNBIAO:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_GUILD_YUNBIAO);
	}
	break;

	case ACTIVITY_TYPE_GUILD_FB:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_GUILD_FB);
	}
	break;

	case CROSS_ACTIVITY_TYPE_CROSS_1V1:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_CROSS_1V1);
	}
	break;

	case CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_CROSS_MULTIUSER_CHALLENGE);
	}
	break;

	case CROSS_ACTIVITY_TYPE_HOT_SPRING:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_HOT_SPRING);
	}
	break;

	case CROSS_ACTIVITY_TYPE_SHUIJING:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_CROSS_SHUIJING);
	}
	break;

	case CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_CROSS_GUILDBATTLE);
	}
	break;

	case CROSS_ACTIVITY_TYPE_DARK_NIGHT:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_DARK_NIGHT);
	}
	break;

	case CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_CROSS_NIGHT_FIGHT_FB);
	}
	break;

	case CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_CROSS_MESS_BATTLE_FB);
	}
	break;

	case CROSS_ACTIVITY_TYPE_LIEKUN_FB:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_LIEKUN_FB);
	}
	break;

	case CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER:
	{
		this->OnActivityFinish(ACTIVITY_FIND_TYPE_XIULUOTOWER);
	}
	break;

	default:
	break;
	}
}

void WorldStatus::OnActivityFinish(int activity_find_type)
{
	m_activity_open_record.record_list[0] |= (1 << activity_find_type);
}

void WorldStatus::SetChallengeFieldKingUserId(long long king_userid)
{
	long long old_king_userid = m_challengefield_king_userid;
	m_challengefield_king_userid = king_userid;

	if (old_king_userid != m_challengefield_king_userid)
	{
		Role *old_king = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(old_king_userid);
		if (NULL != old_king)
		{
			this->SynOtherTitleInfo(old_king, SYNC_REASON_ACTIVITY);
		}

		Role *new_king = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(m_challengefield_king_userid);
		if (NULL != new_king)
		{
			this->SynOtherTitleInfo(new_king, SYNC_REASON_ACTIVITY);
		}
	}
}

void WorldStatus::SetXianmengzhanMengzhu(int mengzhu_id)
{
	int old_mengzhu_id = m_xianmengzhan_mengzhu_id;
	m_xianmengzhan_mengzhu_id = mengzhu_id;

	if (old_mengzhu_id != m_xianmengzhan_mengzhu_id)
	{
		Role *old_mengzhu = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_mengzhu_id));
		if (NULL != old_mengzhu)
		{
			this->SynActivityTitleInfo(old_mengzhu, SYNC_REASON_ACTIVITY);
		}

		Role *new_mengzhu = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_xianmengzhan_mengzhu_id));
		if (NULL != new_mengzhu)
		{
			this->SynActivityTitleInfo(new_mengzhu, SYNC_REASON_ACTIVITY);
		}
	}

	EventHandler::Instance().OnChangeXianmengzhanWiner(old_mengzhu_id, mengzhu_id);
}

void WorldStatus::SetYiZhanDaoDiKingUserId(int user_id, int rank)
{
	int old_userid = 0;
	switch (rank)
	{
	case 0:
	{
		old_userid = m_common_data.yizhandaodi_win_user_id;
		m_common_data.yizhandaodi_win_user_id = user_id;
	}
	break;

	case 1:
	{
		old_userid = m_common_data.yizhandaodi_second_user_id;
		m_common_data.yizhandaodi_second_user_id = user_id;

	}
	break;

	case 2:
	{
		old_userid = m_common_data.yizhandaodi_third_user_id;
		m_common_data.yizhandaodi_third_user_id = user_id;
	}
	break;

	default:
		break;
	}

	if (old_userid != user_id)
	{
		Role *old_user = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_userid));
		if (NULL != old_user)
		{
			this->SynActivityTitleInfo(old_user, SYNC_REASON_ACTIVITY);
		}

		Role *new_user = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(user_id));
		if (NULL != new_user)
		{
			this->SynActivityTitleInfo(new_user, SYNC_REASON_ACTIVITY);
		}
	}
}

void WorldStatus::ClearYiZhanDaoUserTitle()
{
	int old_userid = 0;
	{
		old_userid = m_common_data.yizhandaodi_win_user_id;
		m_common_data.yizhandaodi_win_user_id = 0;

		Role *old_user = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_userid));
		if (NULL != old_user)
		{
			this->SynActivityTitleInfo(old_user, SYNC_REASON_ACTIVITY);
		}
	}

	{
		old_userid = m_common_data.yizhandaodi_second_user_id;
		m_common_data.yizhandaodi_second_user_id = 0;

		Role *old_user = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_userid));
		if (NULL != old_user)
		{
			this->SynActivityTitleInfo(old_user, SYNC_REASON_ACTIVITY);
		}
	}

	{
		old_userid = m_common_data.yizhandaodi_third_user_id;
		m_common_data.yizhandaodi_third_user_id = 0;

		Role *old_user = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_userid));
		if (NULL != old_user)
		{
			this->SynActivityTitleInfo(old_user, SYNC_REASON_ACTIVITY);
		}
	}
}
void WorldStatus::SynActivityTitleInfo(Role *role, int reason)
{
	if (NULL == role)
	{
		return;
	}

	ActivityTitleInfo activity_title_info;
	activity_title_info.xianmengzhan_mengzhu_id = m_xianmengzhan_mengzhu_id;
	activity_title_info.gongchengzhan_mengzhu_id = m_common_data.gcz_chengzhu_id;
	activity_title_info.guildbattle_mengzhu_guild_id = m_common_data.guildbattle_mengzhu_guild_id;
	
	activity_title_info.yizhandaodi_win_user_id = m_common_data.yizhandaodi_win_user_id;
	activity_title_info.yizhandaodi_second_user_id = m_common_data.yizhandaodi_second_user_id;
	activity_title_info.yizhandaodi_third_user_id = m_common_data.yizhandaodi_third_user_id;

	memcpy(activity_title_info.qunxian_luandou_user_id_list, m_common_data.qunxian_luandou_user_id_list, sizeof(activity_title_info.qunxian_luandou_user_id_list));

	role->GetTitleManager()->OnActivityTitleInfo(activity_title_info, reason);
}

void WorldStatus::SynOtherTitleInfo(Role *role, int reason)
{
	if (NULL == role)
	{
		return;
	}

	OtherTitleInfo other_title_info;
	other_title_info.challengefield_king_userid = m_challengefield_king_userid;

	UNSTD_STATIC_CHECK(sizeof(other_title_info.camp_toprank3_uid) == sizeof(m_common_data.camp_toprank3_uid))
	memcpy(other_title_info.camp_toprank3_uid, m_common_data.camp_toprank3_uid, sizeof(other_title_info.camp_toprank3_uid));

	ARRAYCPY(other_title_info.cross_fishing_top_rank_uid_list, m_common_data.cross_fishing_top_rank_uid_list);
	ARRAYCPY(other_title_info.cross_pasture_top_rank_uid_list, m_common_data.cross_pasture_top_rank_uid_list);	

	other_title_info.qingyuan_cap_top_male_uid = m_common_data.qingyuan_cap_top_male_uid;
	other_title_info.qingyuan_cap_top_female_uid = m_common_data.qingyuan_cap_top_female_uid;
	other_title_info.baby_cap_top_male_uid = m_common_data.baby_cap_top_male_uid;
	other_title_info.baby_cap_top_female_uid = m_common_data.baby_cap_top_female_uid;
	other_title_info.littlepet_cap_top_male_uid = m_common_data.little_pet_cap_rank_top_male_uid;
	other_title_info.littlepet_cap_top_female_uid = m_common_data.little_pet_cap_rank_top_female_uid;

	role->GetTitleManager()->OnOtherTitleInfo(other_title_info, reason);
}

void WorldStatus::SendYizhanDaoDiKingUserId(Role *role)
{
	static Protocol::SCYiZhanDaoDiLastFirstInfo ebi;
	ebi.uid = 0;
	memset(ebi.game_name, 0, sizeof(ebi.game_name));

	UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(m_common_data.yizhandaodi_win_user_id);
	if (NULL != user_node)
	{
		ebi.uid = user_node->uid;
		user_node->GetName(ebi.game_name);
	}

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&ebi, sizeof(ebi));
	}
	else
	{
		World::GetInstWorld()->SendToAllGateway((const char*)&ebi, sizeof(Protocol::SCYiZhanDaoDiLastFirstInfo));
	}
}

void WorldStatus::SetQunxianlundouSideFirstUser(int side_first_user_list[], int list_len)
{
	for (int i = 0; i < list_len; ++i)
	{
		int old_user_id = m_common_data.qunxian_luandou_user_id_list[i];
		m_common_data.qunxian_luandou_user_id_list[i] = side_first_user_list[i];

 		if (old_user_id != side_first_user_list[i])
 		{
 			Role *old_user = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_user_id));
 			if (NULL != old_user)
 			{
 				this->SynActivityTitleInfo(old_user, SYNC_REASON_ACTIVITY);
 			}
 
 			Role *new_user = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(side_first_user_list[i]));
 			if (NULL != new_user)
 			{
 				this->SynActivityTitleInfo(new_user, SYNC_REASON_ACTIVITY);
 			}
 		}
	}

	this->SendQuanxianLuandouSideFirstUser(nullptr);
}

void WorldStatus::SendQuanxianLuandouSideFirstUser(Role *role)
{
	static Protocol::SCQunxianLuandouFirstRankInfo pro;
	memset(pro.first_rank_num, 0, sizeof(pro.first_rank_num));
	memset(pro.first_rank_uid, 0, sizeof(pro.first_rank_uid));

	for (int i = 0; i < QUNXIANLUANDOU_SIDE_MAX; ++i)
	{
		int user_id = m_common_data.qunxian_luandou_user_id_list[i];
		UserCacheNode *user = UserCacheManager::Instance().GetUserNode(user_id);
		if (nullptr != user)
		{
			user->GetName(pro.first_rank_num[i]);
			pro.first_rank_uid[i] = user_id;
		}
	}

	if (nullptr != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, sizeof(pro));
	}
	else
	{
		World::GetInstWorld()->SendToAllGateway((const char *)&pro, sizeof(pro));
	}
}

void WorldStatus::OnXunbao(int xunbao_times)
{
	// m_common_data.xunbao_times += xunbao_times;
}

void WorldStatus::OnUpgradeVipLevel(int vip_level)
{
	/*
	if (vip_level > 0 && vip_level <= MAX_VIP_LEVEL)
	{
		++ m_common_data.vip_level_role_num[vip_level];
	}
	*/
}

void WorldStatus::OnBuyMonthCard()
{
	// ++ m_common_data.buy_month_card_times;
}

void WorldStatus::OnSyncGCZInfo(int wing_guild, int chengzhu_id)
{
	if (INVALID_GUILD_ID == wing_guild)  //清除守城一方 
	{
		Role *old_chengzhu = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_common_data.gcz_chengzhu_id));

		m_common_data.gcz_series_win = 0;
		m_common_data.gcz_chengzhu_id = 0;
		m_common_data.gcz_win_guild = INVALID_GUILD_ID;

		this->SendGongChengZhangOwnerInfo();

		if (NULL != old_chengzhu)
		{
			this->SynActivityTitleInfo(old_chengzhu, SYNC_REASON_ACTIVITY);
			old_chengzhu->GetShizhuang()->CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_GCZ);
		}

		UserCacheNode *old_cz = UserCacheManager::Instance().GetUserNode(IntToUid(m_common_data.gcz_chengzhu_id));
		if (nullptr != old_cz)
		{
			Role *old_lover = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_cz->lover_id));
			if (nullptr != old_lover)
			{
				this->SynActivityTitleInfo(old_lover, SYNC_REASON_ACTIVITY);
				old_lover->GetShizhuang()->CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_GCZ);
			}
		}
		
		return;
	}

	if (0 == chengzhu_id)			//清除城主
	{
		Role *old_chengzhu = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_common_data.gcz_chengzhu_id));

		m_common_data.gcz_chengzhu_id = 0;

		this->SendGongChengZhangOwnerInfo();

		if (NULL != old_chengzhu)
		{
			this->SynActivityTitleInfo(old_chengzhu, SYNC_REASON_ACTIVITY);
			old_chengzhu->GetShizhuang()->CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_GCZ);
		}

		UserCacheNode *old_cz = UserCacheManager::Instance().GetUserNode(IntToUid(m_common_data.gcz_chengzhu_id));
		if (nullptr != old_cz)
		{
			Role *old_lover = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_cz->lover_id));
			if (nullptr != old_lover)
			{
				this->SynActivityTitleInfo(old_lover, SYNC_REASON_ACTIVITY);
				old_lover->GetShizhuang()->CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_GCZ);
			}
		}

		return;
	}

	// 替换守城一方和城主
	{
		int old_chengzhu_id = m_common_data.gcz_chengzhu_id;

		m_common_data.gcz_chengzhu_id = chengzhu_id;


		Guild *guild = GuildManager::Instance().GetGuild(wing_guild);
		if (NULL != guild)
		{
			if (wing_guild == m_common_data.gcz_win_guild)
			{
				++ m_common_data.gcz_series_win;
			}
			else
			{
				m_common_data.gcz_series_win = 0;
			}

			m_common_data.gcz_win_guild = wing_guild;
		}

		if (old_chengzhu_id != m_common_data.gcz_chengzhu_id)
		{
			Role *old_chengzhu = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_chengzhu_id));
			if (NULL != old_chengzhu)
			{
				this->SynActivityTitleInfo(old_chengzhu, SYNC_REASON_ACTIVITY);
				old_chengzhu->GetShizhuang()->CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_GCZ);
			}

			UserCacheNode *old_cz = UserCacheManager::Instance().GetUserNode(IntToUid(old_chengzhu_id));
			if (nullptr != old_cz)
			{
				Role *old_lover = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_cz->lover_id));
				if (nullptr != old_lover)
				{
					this->SynActivityTitleInfo(old_lover, SYNC_REASON_ACTIVITY);
					old_lover->GetShizhuang()->CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_GCZ);
				}
			}

			Role *new_mengzhu = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_common_data.gcz_chengzhu_id));
			if (NULL != new_mengzhu)
			{
				this->SynActivityTitleInfo(new_mengzhu, SYNC_REASON_ACTIVITY);
				new_mengzhu->GetShizhuang()->CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_GCZ);
			}

			UserCacheNode *new_cz = UserCacheManager::Instance().GetUserNode(IntToUid(m_common_data.gcz_chengzhu_id));
			if (nullptr != new_cz)
			{
				Role *new_lover = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(new_cz->lover_id));
				if (nullptr != new_lover)
				{
					this->SynActivityTitleInfo(new_lover, SYNC_REASON_ACTIVITY);
					new_lover->GetShizhuang()->CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_GCZ);
				}
			}

			this->SendGongChengZhangOwnerInfo();
		}

		EventHandler::Instance().OnChangeGongchengzhanWiner(old_chengzhu_id, chengzhu_id);
	}
}

void WorldStatus::OnSyncGBMenzhuInfo(GuildID guild_id)
{ 
	GuildID old_guild_id = m_common_data.guildbattle_mengzhu_guild_id;
	m_common_data.guildbattle_mengzhu_guild_id = guild_id;

	int old_guild_master_id = 0, new_guild_master_id = 0;
	if (old_guild_id != guild_id)
	{
		Guild *old_guild = GuildManager::Instance().GetGuild(old_guild_id);
		if (nullptr != old_guild)
		{
			old_guild_master_id = old_guild->GetGuildMemberManager().GetTuanZhangUid();
		}

		Guild *new_guild = GuildManager::Instance().GetGuild(guild_id);
		if (nullptr != new_guild)
		{
			new_guild_master_id = new_guild->GetGuildMemberManager().GetTuanZhangUid();
		}
	}

	CombineServerActivityManager::Instance().OnChangeXianmengzhanWiner(old_guild_master_id, new_guild_master_id);
}

void WorldStatus::OnSyncQingyuanCapTop(int male_uid, int female_uid)
{
	int last_top_male_uid = m_common_data.qingyuan_cap_top_male_uid;
	int last_top_female_uid = m_common_data.qingyuan_cap_top_female_uid;

	m_common_data.qingyuan_cap_top_male_uid = male_uid;
	m_common_data.qingyuan_cap_top_female_uid = female_uid;

	{
		// 昨日top同步称号
		Role *last_top_male_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(last_top_male_uid));
		if (nullptr != last_top_male_role)
		{
			this->SynOtherTitleInfo(last_top_male_role, SYNC_REASON_RANK);
		}

		Role *last_top_female_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(last_top_female_uid));
		if (nullptr != last_top_female_role)
		{
			this->SynOtherTitleInfo(last_top_female_role, SYNC_REASON_RANK);
		}
	}

	{
		// 今日top同步称号
		Role *male_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(male_uid));
		if (nullptr != male_role)
		{
			this->SynOtherTitleInfo(male_role, SYNC_REASON_RANK);
		}

		Role *female_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(female_uid));
		if (nullptr != female_role)
		{
			this->SynOtherTitleInfo(female_role, SYNC_REASON_RANK);
		}
	}
}

void WorldStatus::OnSyncBabyCapTop(int male_uid, int female_uid)
{
	int last_top_male_uid = m_common_data.baby_cap_top_male_uid;
	int last_top_female_uid = m_common_data.baby_cap_top_female_uid;

	m_common_data.baby_cap_top_male_uid = male_uid;
	m_common_data.baby_cap_top_female_uid = female_uid;

	{
		// 昨日top同步称号
		Role *last_top_male_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(last_top_male_uid));
		if (nullptr != last_top_male_role)
		{
			this->SynOtherTitleInfo(last_top_male_role, SYNC_REASON_RANK);
		}

		Role *last_top_female_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(last_top_female_uid));
		if (nullptr != last_top_female_role)
		{
			this->SynOtherTitleInfo(last_top_female_role, SYNC_REASON_RANK);
		}
	}

	{
		// 今日top同步称号
		Role *male_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(male_uid));
		if (nullptr != male_role)
		{
			this->SynOtherTitleInfo(male_role, SYNC_REASON_RANK);
		}

		Role *female_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(female_uid));
		if (nullptr != female_role)
		{
			this->SynOtherTitleInfo(female_role, SYNC_REASON_RANK);
		}
	}
}

void WorldStatus::OnSyncLittePetCapTop(int male_uid, int female_uid)
{
	int last_top_male_uid = m_common_data.little_pet_cap_rank_top_male_uid;
	int last_top_female_uid = m_common_data.little_pet_cap_rank_top_female_uid;

	m_common_data.little_pet_cap_rank_top_male_uid = male_uid;
	m_common_data.little_pet_cap_rank_top_female_uid = female_uid;

	{
		// 昨日top同步称号
		Role *last_top_male_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(last_top_male_uid));
		if (nullptr != last_top_male_role)
		{
			this->SynOtherTitleInfo(last_top_male_role, SYNC_REASON_RANK);
		}

		Role *last_top_female_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(last_top_female_uid));
		if (nullptr != last_top_female_role)
		{
			this->SynOtherTitleInfo(last_top_female_role, SYNC_REASON_RANK);
		}
	}

	{
		// 今日top同步称号
		Role *male_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(male_uid));
		if (nullptr != male_role)
		{
			this->SynOtherTitleInfo(male_role, SYNC_REASON_RANK);
		}

		Role *female_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(female_uid));
		if (nullptr != female_role)
		{
			this->SynOtherTitleInfo(female_role, SYNC_REASON_RANK);
		}
	}
}

char WorldStatus::GetGCZChengzhuProf()
{
	UserCacheNode *cache_user = UserCacheManager::Instance().GetUserNode(this->GetGCZChengzhuID());
	if (NULL != cache_user)
	{
		return cache_user->prof;
	}
	return PROF_TYPE_INVALID;
}

void WorldStatus::SendGongChengZhangOwnerInfo(Role *role)
{
	static Protocol::SCGongChengZhanOwnerInfo gczoi;
	gczoi.owner_id = 0;

	UserCacheNode *cache_user = UserCacheManager::Instance().GetUserNode(WorldStatus::Instance().GetGCZChengzhuID());
	if (NULL != cache_user)
	{
		gczoi.owner_id = cache_user->uid;
		F_STRNCPY(gczoi.owner_name, cache_user->user_name, sizeof(gczoi.owner_name));
		gczoi.vip_level = cache_user->vip_level;
		gczoi.sex = cache_user->sex;
		gczoi.prof = cache_user->prof;
		gczoi.guild_id = m_common_data.gcz_win_guild;
	}

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&gczoi, sizeof(gczoi));
	}
	else
	{
		World::GetInstWorld()->SendToAllGateway((const char*)&gczoi, sizeof(gczoi));
	}
}

int WorldStatus::GetCampExistsDiaoXiangNum(int camp)
{
	if (camp <= CAMP_TYPE_INVALID || camp >= CAMP_TYPE_MAX)
	{
		return 0;
	}

	int exist_num = 0;

	for (int i = 0; i < MAX_CAMP_RECORD_RANK3_NUM; ++ i)
	{
		if (MAX_CAMP_TOPRANK3 > 0 && i / MAX_CAMP_TOPRANK3 + 1 == camp && 
			DIAO_XIANG_STATUS_EXIST == m_common_data.camp_diaoxiang_status[i])
		{
			++ exist_num;
		}
	}

	return exist_num;
}

int WorldStatus::GetCampDiaoXiangStatus(int index)
{
	if (index >= 0 && index < MAX_CAMP_RECORD_RANK3_NUM)
	{
		return m_common_data.camp_diaoxiang_status[index];
	}

	return DIAO_XIANG_STATUS_DESTORY;
}

void WorldStatus::SetCampDiaoXiangStatus(int index, int result)
{
	if (index >= 0 && index < MAX_CAMP_RECORD_RANK3_NUM && 
		result >= DIAO_XIANG_STATUS_DESTORY && result <= DIAO_XIANG_STATUS_EXIST)
	{
		m_common_data.camp_diaoxiang_status[index] = result;
	}
}

void WorldStatus::ResetCampDiaoXiangStatus(int camp)
{
	for (int i = 0; i < MAX_CAMP_RECORD_RANK3_NUM; ++ i)
	{
		if (MAX_CAMP_TOPRANK3 > 0 && (i / MAX_CAMP_TOPRANK3) + 1 == camp)
		{
			m_common_data.camp_diaoxiang_status[i] = DIAO_XIANG_STATUS_EXIST;
		}
	}
}

int WorldStatus::GetFamousManID(int famous_type)
{
	if (famous_type <= FAMOUSN_MAN_INVAILD || famous_type >= FAMOUS_MAN_MAX_TYPE)
	{
		return 0;
	}

	switch(famous_type)
	{
	case FAMOUS_MAN_ROLE_LEVEL:
		{
			return m_common_data.famous_man_type_1_uid;
		}
		break;

	case FAMOUS_MAN_MOUNT_GRADE:
		{
			return m_common_data.famous_man_type_2_uid;
		}
		break;

	case FAMOUS_MAN_WING_GRADE:
		{
			return m_common_data.famous_man_type_3_uid;
		}
		break;

	case FAMOUS_MAN_EQUIP_GRADE:
		{
			return m_common_data.famous_man_type_4_uid;
		}
		break;

	case FAMOUS_MAN_TOWER_LEVEL:
		{
			return m_common_data.famous_man_type_5_uid;
		}
		break;

	case FAMOUS_MAN_ACTIVE_XIANNV:
		{
			return m_common_data.famous_man_type_6_uid;
		}
		break;
	}

	return 0;
}

void WorldStatus::SetFamousManID(int famous_type, int uid)
{
	if (famous_type <= FAMOUSN_MAN_INVAILD || famous_type >= FAMOUS_MAN_MAX_TYPE)
	{
		return;
	}

	switch(famous_type)
	{
	case FAMOUS_MAN_ROLE_LEVEL:
		{
			m_common_data.famous_man_type_1_uid = uid;
		}
		break;

	case FAMOUS_MAN_MOUNT_GRADE:
		{
			m_common_data.famous_man_type_2_uid = uid;
		}
		break;

	case FAMOUS_MAN_WING_GRADE:
		{
			m_common_data.famous_man_type_3_uid = uid;
		}
		break;

	case FAMOUS_MAN_EQUIP_GRADE:
		{
			m_common_data.famous_man_type_4_uid = uid;
		}
		break;

	case FAMOUS_MAN_TOWER_LEVEL:
		{
			m_common_data.famous_man_type_5_uid = uid;
		}
		break;

	case FAMOUS_MAN_ACTIVE_XIANNV:
		{
			m_common_data.famous_man_type_6_uid = uid;
		}
		break;
	}
}

// 假首冲传闻
void WorldStatus::OnFakeFirstChongzhiSystemMsg()
{
	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	time_t open_days = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), now_sec) + 1;
	if (open_days > 1)
	{
		return;
	}

	m_common_data.role_create_count++;

	if (0 == m_common_data.role_create_count % WORLDSTATUS_FAKE_CHONGZHI_MSG_ROLE_NUM && now_sec - m_common_data.last_send_fake_recharge_msg_time >= WORLDSTATUS_FAKE_CHONGZHI_MSG_MIN_INTERVAL)
	{
		m_common_data.last_send_fake_recharge_msg_time = now_sec;

		GameName rand_name;
		if(!LOGIC_CONFIG->GetNameCreateCfg().GetRandName(rand_name))
		{
			return;
		}

		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_chongzhi_first_chongzhi_reward_content, 
			0, rand_name, 0);
		if (length > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
		}	
	}
}

void WorldStatus::AddChestShopRecord(Role *role, ItemID item_id, int chest_shop_type)
{
	if (NULL == role || item_id <= 0) return;

	if (CHEST_SHOP_TYPE_EQUIP == chest_shop_type)
	{
		if (m_common_data.chestshop_index < 0 || m_common_data.chestshop_index >= MAX_CHEST_SHOP_RECORD_COUNT)
		{
			m_common_data.chestshop_index = 0;
		}

		WorldStatusCommonDataParam::ChestRecord &record = m_common_data.chestshop_record_list[m_common_data.chestshop_index];
		record.uid = role->GetUID();
		record.item_id = item_id;
		m_common_data.chestshop_index++;
	}

	else if (CHEST_SHOP_TYPE_EQUIP1 == chest_shop_type)
	{
		if (m_common_data.chestshop_index1 < 0 || m_common_data.chestshop_index1 >= MAX_CHEST_SHOP_RECORD_COUNT)
		{
			m_common_data.chestshop_index1 = 0;
		}

		WorldStatusCommonDataParam::ChestRecord &record = m_common_data.chestshop_record1_list[m_common_data.chestshop_index1];
		record.uid = role->GetUID();
		record.item_id = item_id;
		m_common_data.chestshop_index1++;
	}

	else if (CHEST_SHOP_TYPE_EQUIP2 == chest_shop_type)
	{
		if (m_common_data.chestshop_index2 < 0 || m_common_data.chestshop_index2 >= MAX_CHEST_SHOP_RECORD_COUNT)
		{
			m_common_data.chestshop_index2 = 0;
		}

		WorldStatusCommonDataParam::ChestRecord &record = m_common_data.chestshop_record2_list[m_common_data.chestshop_index2];
		record.uid = role->GetUID();
		record.item_id = item_id;
		m_common_data.chestshop_index2++;
	}

	else if (CHEST_SHOP_TYPE_EQUIP3 == chest_shop_type)
	{
		if (m_common_data.chestshop_index3 < 0 || m_common_data.chestshop_index3 >= MAX_CHEST_SHOP_RECORD_COUNT)
		{
			m_common_data.chestshop_index3 = 0;
		}

		WorldStatusCommonDataParam::ChestRecord &record = m_common_data.chestshop_record3_list[m_common_data.chestshop_index3];
		record.uid = role->GetUID();
		record.item_id = item_id;
		m_common_data.chestshop_index3++;
	}

	else if (CHEST_SHOP_TYPE_JINGLING == chest_shop_type)
	{
		if (m_common_data.chestshop_jl_index < 0 || m_common_data.chestshop_jl_index >= MAX_CHEST_SHOP_RECORD_COUNT)
		{
			m_common_data.chestshop_jl_index = 0;
		}

		WorldStatusCommonDataParam::ChestRecord &record = m_common_data.chestshop_jl_record_list[m_common_data.chestshop_jl_index];
		record.uid = role->GetUID();
		record.item_id = item_id;
		m_common_data.chestshop_jl_index++;
	}
	
}

void WorldStatus::SendChestShopRecord(Role *role, int chest_shop_type)
{
	if (NULL == role || chest_shop_type <= CHEST_SHOP_TYPE_INVALID || chest_shop_type >= CHEST_SHOP_TYPE_MAX) return;

	Protocol::SCChestShopRecordList cmd;
	cmd.count = 0;
	cmd.record_type = chest_shop_type;

	if (CHEST_SHOP_TYPE_EQUIP == chest_shop_type)
	{
		for (int i = 0; i < MAX_CHEST_SHOP_RECORD_COUNT && cmd.count < Protocol::SCChestShopRecordList::MAX_RECORD_COUNT; i++)
		{
			WorldStatusCommonDataParam::ChestRecord &record = m_common_data.chestshop_record_list[i];
			if (record.uid <= 0) break;

			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(record.uid);
			if (NULL == user_node)
			{
				continue;
			}

			cmd.record_list[cmd.count].item_id = record.item_id;
			user_node->GetName(cmd.record_list[cmd.count].role_name);
			cmd.count++;
		}
	}

	else if (CHEST_SHOP_TYPE_EQUIP1 == chest_shop_type)
	{
		for (int i = 0; i < MAX_CHEST_SHOP_RECORD_COUNT && cmd.count < Protocol::SCChestShopRecordList::MAX_RECORD_COUNT; i++)
		{
			WorldStatusCommonDataParam::ChestRecord &record = m_common_data.chestshop_record1_list[i];
			if (record.uid <= 0) break;

			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(record.uid);
			if (NULL == user_node)
			{
				continue;
			}

			cmd.record_list[cmd.count].item_id = record.item_id;
			user_node->GetName(cmd.record_list[cmd.count].role_name);
			cmd.count++;
		}
	}

	else if (CHEST_SHOP_TYPE_EQUIP2 == chest_shop_type)
	{
		for (int i = 0; i < MAX_CHEST_SHOP_RECORD_COUNT && cmd.count < Protocol::SCChestShopRecordList::MAX_RECORD_COUNT; i++)
		{
			WorldStatusCommonDataParam::ChestRecord &record = m_common_data.chestshop_record2_list[i];
			if (record.uid <= 0) break;

			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(record.uid);
			if (NULL == user_node)
			{
				continue;
			}

			cmd.record_list[cmd.count].item_id = record.item_id;
			user_node->GetName(cmd.record_list[cmd.count].role_name);
			cmd.count++;
		}
	}

	else if (CHEST_SHOP_TYPE_EQUIP3 == chest_shop_type)
	{
		for (int i = 0; i < MAX_CHEST_SHOP_RECORD_COUNT && cmd.count < Protocol::SCChestShopRecordList::MAX_RECORD_COUNT; i++)
		{
			WorldStatusCommonDataParam::ChestRecord &record = m_common_data.chestshop_record3_list[i];
			if (record.uid <= 0) break;

			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(record.uid);
			if (NULL == user_node)
			{
				continue;
			}

			cmd.record_list[cmd.count].item_id = record.item_id;
			user_node->GetName(cmd.record_list[cmd.count].role_name);
			cmd.count++;
		}
	}

	else if(CHEST_SHOP_TYPE_JINGLING == chest_shop_type)
	{
		for (int j = 0; j < MAX_CHEST_SHOP_RECORD_COUNT && cmd.count < Protocol::SCChestShopRecordList::MAX_RECORD_COUNT; j++)
		{
			WorldStatusCommonDataParam::ChestRecord &record = m_common_data.chestshop_jl_record_list[j];
			if (record.uid <= 0) break;

			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(record.uid);
			if (NULL == user_node)
			{
				continue;
			}

			cmd.record_list[cmd.count].item_id = record.item_id;
			user_node->GetName(cmd.record_list[cmd.count].role_name);
			cmd.count++;
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, 
		sizeof(Protocol::SCChestShopRecordList) - sizeof(Protocol::SCChestShopRecordList::ListItem) * (MAX_CHEST_SHOP_RECORD_COUNT - cmd.count));
}

void WorldStatus::GmSetNextQuestionTime()
{
	m_question_next_begin = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 1;
}

void WorldStatus::CalcNextQuestionTime()
{
	if (CrossConfig::Instance().IsHiddenServer()) return;

	const WGQuestionCfg *question_cfg = LOGIC_CONFIG->GetQuestionConfig().GetWGQuestionCfg(QUESTION_TYPE_WORLD);
	if (NULL == question_cfg) return;

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(static_cast<time_t>(now_sec)));
	unsigned today_sec = now_sec - today_zero_time;

	if (today_sec < static_cast<unsigned int>(question_cfg->begin_time))
	{
		m_question_next_begin = today_zero_time + question_cfg->begin_time;
	}
	else if (today_sec > static_cast<unsigned int>(question_cfg->end_time))
	{
		int next_day_refresh_interval = EngineAdapter::Instance().NextDayInterval(0, 0, 0) + question_cfg->begin_time;
		m_question_next_begin = now_sec + next_day_refresh_interval;
	}
	else
	{
		m_question_next_begin = now_sec + (question_cfg->question_interval - (today_sec - question_cfg->begin_time) % question_cfg->question_interval);
	}
}

void WorldStatus::BrocastWorldQuestion(Role *role)
{
	if (m_question_id <= 0) return;

	static Protocol::SCQuestionBroadcast question_info;
	question_info.question_type = QUESTION_TYPE_WORLD;
	question_info.curr_question_id = m_question_id;

	question_info.curr_question_begin_time = m_question_cur_begin;
	question_info.curr_question_end_time = m_question_cur_end;
	question_info.next_question_begin_time = m_question_next_begin;

	LOGIC_CONFIG->GetQuestionConfig().GetQuestionStr(m_question_id, question_info.curr_question_str, QUESTION_TYPE_WORLD);
	LOGIC_CONFIG->GetQuestionConfig().GetAnswer0DescStr(m_question_id, question_info.curr_answer0_desc_str, QUESTION_TYPE_WORLD);
	LOGIC_CONFIG->GetQuestionConfig().GetAnswer1DescStr(m_question_id, question_info.curr_answer1_desc_str, QUESTION_TYPE_WORLD);
	LOGIC_CONFIG->GetQuestionConfig().GetAnswer2DescStr(m_question_id, question_info.curr_answer2_desc_str, QUESTION_TYPE_WORLD);
	LOGIC_CONFIG->GetQuestionConfig().GetAnswer3DescStr(m_question_id, question_info.curr_answer3_desc_str, QUESTION_TYPE_WORLD);
	
	if (NULL != role)
	{
		if (role->GetCommonDataParam()->world_question_last_answer_time < m_question_cur_begin)
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&question_info, sizeof(Protocol::SCQuestionBroadcast));
		}
	}
	else
	{
		World::GetInstWorld()->SendToAllGateway((const char*)&question_info, sizeof(Protocol::SCQuestionBroadcast));

		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_question_system_message_world,
			question_info.curr_question_str);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_ONLY_WORLD_QUESTION);
		}
	}
}

void WorldStatus::OnAnswerWorldQuestion(Role *role, int choose)
{
	if (NULL == role) return;

	const WGQuestionCfg *question_cfg = LOGIC_CONFIG->GetQuestionConfig().GetWGQuestionCfg(QUESTION_TYPE_WORLD);
	if (NULL == question_cfg) return;

	if (role->GetLevel() < question_cfg->open_level)
	{
		role->NoticeNum(errornum::EN_QUESTION_LEVEL_LIMIT);
		return;
	}

	unsigned int now_sec = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (m_question_id <= 0 || now_sec < m_question_cur_begin || now_sec > m_question_cur_end)
	{
		role->NoticeNum(errornum::EN_QUESTION_TIME_LIMIT);
		return;
	}

	CommonDataParam *common_data = role->GetCommonDataParam();
	if (NULL == common_data) return;

	if (common_data->world_question_last_answer_time >= m_question_cur_begin)
	{
		role->NoticeNum(errornum::EN_QUESTION_HAS_ANSWER_LIMIT);
		return;
	}

	common_data->world_question_last_answer_time = now_sec;
	common_data->world_question_right_num_change_time = now_sec;

	static Protocol::SCQuestionAnswerResult result;
	result.answer_type = QUESTION_TYPE_WORLD;
	result.right_result = choose == m_question_answer ? 1 : 0;
	result.result = m_question_answer;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&result, sizeof(result));

	if (1 == result.right_result)
	{
		common_data->world_question_right_num++;
		//role->AddExp(static_cast<long long>((role->GetLevel() + 50) *question_cfg->right_exp_reward), "WorldQuestiong");

		//策划需求，修改增加经验规则（2018-1-3）
		const QuestionOtherCfg *other_cfg = LOGIC_CONFIG->GetQuestionConfig().GetOtherCfg();
		if (NULL != other_cfg)
		{
			long long add_exp = static_cast<long long>(other_cfg->right_world_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));
			role->AddExp(add_exp, "WorldQuestion", Role::EXP_ADD_REASON_DEFAULT);
		}
	}
	else
	{
		//role->AddExp(static_cast<long long>((role->GetLevel() + 50) *question_cfg->wrong_exp_reward), "WorldQuestiong");

		//策划需求，修改增加经验规则（2018-1-3）
		const QuestionOtherCfg *other_cfg = LOGIC_CONFIG->GetQuestionConfig().GetOtherCfg();
		if (NULL != other_cfg)
		{
			long long add_exp = static_cast<long long>(other_cfg->wrong_world_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));
			role->AddExp(add_exp, "WorldQuestion", Role::EXP_ADD_REASON_DEFAULT);
		}
	}

	if (m_question_cur_right_num < question_cfg->fast_reward_num && 1 == result.right_result)
	{
		if (m_question_cur_right_num >= 0 && m_question_cur_right_num < 3)
		{
			m_question_fast_uid[m_question_cur_right_num] = role->GetUID();
		}

		m_question_cur_right_num++;

		static MailContentParam contentparam;
		contentparam.Reset();
		int item_count = 0;

		for (; item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
		{
			if (question_cfg->fast_reward[item_count].item_id <= 0) break;

			contentparam.item_list[item_count].item_id = question_cfg->fast_reward[item_count].item_id;
			contentparam.item_list[item_count].num = question_cfg->fast_reward[item_count].num;
			contentparam.item_list[item_count].is_bind = (question_cfg->fast_reward[item_count].is_bind) ? 1 : 0;
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_question_fast_world_reward_mail, m_question_cur_right_num);
		if (length > 0 && item_count > 0)
		{
			MailRoute::MailToUser(role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_question.printf(LL_INFO, "WorldQusestionReward::user[%d, %s], reward_item_1[%d],num_1[%d],reward_item_2[%d],num_2[%d],reward_item_3[%d],num_3[%d]",
				role->GetUID(), role->GetName(), contentparam.item_list[0].item_id, contentparam.item_list[0].num,
				contentparam.item_list[1].item_id, contentparam.item_list[1].num, contentparam.item_list[2].item_id, contentparam.item_list[2].num);
		}
	}
}

void WorldStatus::GetCrossGuildBattleInfo(CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM])
{
	CrossGuildBattleParam &param = m_cross_guildbattle_param;
	UNSTD_STATIC_CHECK(sizeof(cross_guildbattle_list[0]) * CROSS_GUILDBATTLE_MAX_SCENE_NUM == sizeof(param.cross_guildbattle_list));
	memcpy(cross_guildbattle_list, param.cross_guildbattle_list, sizeof(param.cross_guildbattle_list));
}

void WorldStatus::GetCrossGuildBattleTuanzhangInfo(CrossGuildBattleTuanzhangInfo cross_guildbattle_tuanzhang_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM])
{
	CrossGuildBattleParam &param = m_cross_guildbattle_param;
	UNSTD_STATIC_CHECK(sizeof(cross_guildbattle_tuanzhang_info_list[0]) * CROSS_GUILDBATTLE_MAX_SCENE_NUM == sizeof(param.info_list));
	memcpy(cross_guildbattle_tuanzhang_info_list, param.info_list, sizeof(param.info_list));
}
void WorldStatus::GetCrossGuildBattleApperanceInfo(CrossGuildBattleApperanceInfo cross_guildbattle_apperance_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM])
{
	CrossGuildBattleParam &param = m_cross_guildbattle_param;
	UNSTD_STATIC_CHECK(sizeof(cross_guildbattle_apperance_info_list[0]) * CROSS_GUILDBATTLE_MAX_SCENE_NUM == sizeof(param.apperance_info_list));
	memcpy(cross_guildbattle_apperance_info_list, param.apperance_info_list, sizeof(param.apperance_info_list));
}
int WorldStatus::GetCrossGuildBattleAverageLevel()
{
	return m_cross_guildbattle_param.average_level;
}

void WorldStatus::SetCrossGuildBattleAverageLevel(int level)
{
	m_cross_guildbattle_param.average_level = level;
}

int WorldStatus::GetCrossGuildBattleRewardCalctime()
{
	return m_cross_guildbattle_param.reward_calc_time;
}

void WorldStatus::ResetCrossGuildBattleInfo()
{
	CrossGuildBattleParam &param = m_cross_guildbattle_param;

	memset(param.cross_guildbattle_list, 0, sizeof(param.cross_guildbattle_list));
	param.reward_calc_time = 0;
	memset(param.info_list, 0, sizeof(param.info_list));
}

void WorldStatus::SetCrossGuildBattleInfo(CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM])
{
	CrossGuildBattleParam &param = m_cross_guildbattle_param;
	param.reward_calc_time = static_cast<int>(EngineAdapter::Instance().Time());

	UNSTD_STATIC_CHECK(sizeof(cross_guildbattle_list[0]) * CROSS_GUILDBATTLE_MAX_SCENE_NUM == sizeof(param.cross_guildbattle_list));
	memcpy(param.cross_guildbattle_list, cross_guildbattle_list, sizeof(param.cross_guildbattle_list));
}

void WorldStatus::SetCrossGuildBattleTuanzhangInfo(CrossGuildBattleTuanzhangInfo cross_guildbattle_tuanzhang_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM])
{
	CrossGuildBattleParam &param = m_cross_guildbattle_param;

	UNSTD_STATIC_CHECK(sizeof(cross_guildbattle_tuanzhang_info_list[0]) * CROSS_GUILDBATTLE_MAX_SCENE_NUM == sizeof(param.info_list));
	memcpy(param.info_list, cross_guildbattle_tuanzhang_info_list, sizeof(param.info_list));
}

void WorldStatus::SetCrossGuildBattleApperanceInfo(CrossGuildBattleApperanceInfo cross_guildbattle_apperance_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM])
{
	CrossGuildBattleParam &param = m_cross_guildbattle_param;

	UNSTD_STATIC_CHECK(sizeof(cross_guildbattle_apperance_info_list[0]) * CROSS_GUILDBATTLE_MAX_SCENE_NUM == sizeof(param.apperance_info_list));
	memcpy(param.apperance_info_list, cross_guildbattle_apperance_info_list, sizeof(param.apperance_info_list));
}

void WorldStatus::OnRoleJoinDarkNight(int role_level)
{
	m_common_data.total_join_darknight_role_level += role_level;
	m_common_data.total_join_darknight_role_count++;
}


void WorldStatus::SetCrossFishingTopRankUid(Int32 rank, Int32 uid)
{
	if (rank <= 0 || rank > CROSS_FISHING_TOP_RANK_COUNT_MAX) return;

	Int32 old_uid = m_common_data.cross_fishing_top_rank_uid_list[rank - 1];
	m_common_data.cross_fishing_top_rank_uid_list[rank - 1] = uid;

	Role *old_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_uid));
	if (old_role != nullptr)
	{
		this->SynOtherTitleInfo(old_role, SYNC_REASON_ACTIVITY);
	}

	Role *new_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (new_role != nullptr)
	{
		this->SynOtherTitleInfo(new_role, SYNC_REASON_ACTIVITY);
	}
}

void WorldStatus::SetCrossPastureTopRankUid(Int32 rank, Int32 uid)
{
	if (rank <= 0 || rank > CROSS_FISHING_TOP_RANK_COUNT_MAX) return;

	Int32 old_uid = m_common_data.cross_pasture_top_rank_uid_list[rank - 1];
	m_common_data.cross_pasture_top_rank_uid_list[rank - 1] = uid;

	Role *old_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_uid));
	if (old_role != nullptr)
	{
		this->SynOtherTitleInfo(old_role, SYNC_REASON_ACTIVITY);
	}

	Role *new_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (new_role != nullptr)
	{
		this->SynOtherTitleInfo(new_role, SYNC_REASON_ACTIVITY);
	}
	
}

int WorldStatus::GetTotalUpgradeMountRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_RECORD_COUNT)
	{
		return 0;
	}
	return m_common_data.total_upgrade_mount_record_list[grade];
}

int WorldStatus::GetTotalUpgradeHaloRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_RECORD_COUNT)
	{
		return 0;
	}
	return m_common_data.total_upgrade_holo_record_list[grade];
}

int WorldStatus::GetTotalUpgradeWingRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_RECORD_COUNT)
	{
		return 0;
	}
	return m_common_data.total_upgrade_wing_record_list[grade];
}

int WorldStatus::GetTotalUpgradeShengongRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_RECORD_COUNT)
	{
		return 0;
	}
	return m_common_data.total_upgrade_shengong_record_list[grade];
}

int WorldStatus::GetTotalUpgradeShenyiRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_RECORD_COUNT)
	{
		return 0;
	}
	return m_common_data.total_upgrade_shenyi_record_list[grade];
}

void WorldStatus::IncTotalUpgradeMountRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_RECORD_COUNT)
	{
		return;
	}
	++m_common_data.total_upgrade_mount_record_list[grade];
}

void WorldStatus::IncTotalUpgradeHaloRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_RECORD_COUNT)
	{
		return;
	}
	++m_common_data.total_upgrade_holo_record_list[grade];
}

void WorldStatus::IncTotalUpgradeWingRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_RECORD_COUNT)
	{
		return;
	}
	++m_common_data.total_upgrade_wing_record_list[grade];
}

void WorldStatus::IncTotalUpgradeShengongRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_RECORD_COUNT)
	{
		return;
	}
	++m_common_data.total_upgrade_shengong_record_list[grade];
}

void WorldStatus::IncTotalUpgradeShenyiRecordList(int grade)
{
	if (grade < 0 || grade >= MAX_UPGRADE_RECORD_COUNT)
	{
		return;
	}
	++m_common_data.total_upgrade_shenyi_record_list[grade];
}

const int WorldStatus::GetChatLimitConditionType(const int chat_type)
{
	if (chat_type < 0 || chat_type >= OPENLEVEL_LIMIT_TYPE_MAX) return 0;

	if ((m_common_data.set_chat_limit_cond_type_flag & (1 << chat_type)) != 0)
	{
		return ((m_common_data.chat_limit_condition_type_flag >> chat_type) & 1);
	}
	else
	{
		const int chat_limit_condition_type_flag = LOGIC_CONFIG->GetAgentAdapterConfig().GetChatLimitConditionTypeFlag();

		return ((chat_limit_condition_type_flag >> chat_type) & 1);
	}

	return 0;
}

const int WorldStatus::GetChatLimitConditionTypeFlag()
{
	int temp_flag = LOGIC_CONFIG->GetAgentAdapterConfig().GetChatLimitConditionTypeFlag();

	for (int i = 0; i < OPENLEVEL_LIMIT_TYPE_MAX; ++i)
	{
		if ((m_common_data.set_chat_limit_cond_type_flag & (1 << i)) != 0)
		{
			int set_bit = m_common_data.chat_limit_condition_type_flag & (1 << i);

			if (set_bit > 0)
			{
				temp_flag |= (1 << i);
			}
			else
			{
				temp_flag &= ~(1 << i);
			}
		}
	}

	return temp_flag;
}

void WorldStatus::CmdSetChatLimitConditionType(const int chat_type, const int condition_type)
{
	if (chat_type < 0 || chat_type >= OPENLEVEL_LIMIT_TYPE_MAX) return;

	if (condition_type > 0)
	{
		m_common_data.chat_limit_condition_type_flag |= (1 << chat_type);
	}
	else
	{
		m_common_data.chat_limit_condition_type_flag &= ~(1 << chat_type);
	}

	m_common_data.set_chat_limit_cond_type_flag |= (1 << chat_type);
}

const int WorldStatus::GetChannelLevelLimit(short channel_type)
{
	if (channel_type < OPENLEVEL_LIMIT_TYPE_WORLD || channel_type >= OPENLEVEL_LIMIT_TYPE_MAX) return 0;

	return m_common_data.channel_level_limit_list[channel_type];
}

bool WorldStatus::GetChannelLevelLimit(int *level_list, int list_len)
{
	if (nullptr == level_list || list_len <= 0) return false;

	for (int i = 0; i < OPENLEVEL_LIMIT_TYPE_MAX && i < list_len; ++i)
	{
		if (m_common_data.channel_level_limit_list[i] > 0)
		{
			level_list[i] = m_common_data.channel_level_limit_list[i];
		}
	}

	return true;
}

bool WorldStatus::CmdSetChannelLevelLimit(short channel_type, short level)
{
	if (channel_type >= OPENLEVEL_LIMIT_TYPE_WORLD && channel_type < OPENLEVEL_LIMIT_TYPE_MAX)
	{
		m_common_data.channel_level_limit_list[channel_type] = static_cast<unsigned short>(level);
	}
	else
	{
		for (int i = 0; i < OPENLEVEL_LIMIT_TYPE_MAX; ++i)
		{
			m_common_data.channel_level_limit_list[i] = static_cast<unsigned short>(level);
		}
	}

	return true;
}

const short WorldStatus::GetChannelVipLevelLimit(short channel_type)
{
	if (channel_type < OPENLEVEL_LIMIT_TYPE_WORLD || channel_type >= OPENLEVEL_LIMIT_TYPE_MAX) return 0;

	return m_common_data.channel_vip_level_limit_list[channel_type];
}

bool WorldStatus::GetChannelVipLevelLimit(short *vip_level_list, int list_len)
{
	if (nullptr == vip_level_list) return false;

	for (int i = 0; i < OPENLEVEL_LIMIT_TYPE_MAX && i < list_len; ++i)
	{
		if (m_common_data.channel_vip_level_limit_list[i] > 0)
		{
			vip_level_list[i] = m_common_data.channel_vip_level_limit_list[i];
		}
	}

	return true;
}

bool WorldStatus::CmdSetChannelVipLevelLimit(short channel_type, short vip_level)
{
	if (channel_type >= OPENLEVEL_LIMIT_TYPE_WORLD && channel_type < OPENLEVEL_LIMIT_TYPE_MAX)
	{
		m_common_data.channel_vip_level_limit_list[channel_type] = static_cast<char>(vip_level);
	}
	else
	{
		for (int i = 0; i < OPENLEVEL_LIMIT_TYPE_MAX; ++i)
		{
			m_common_data.channel_vip_level_limit_list[i] = static_cast<char>(vip_level);
		}
	}

	return true;
}

bool WorldStatus::CheckSatisfyLevel(Role *role, int channel_type)
{
	if (nullptr == role || channel_type < OPENLEVEL_LIMIT_TYPE_WORLD || channel_type >= OPENLEVEL_LIMIT_TYPE_MAX) return false;

	bool is_satisfy = false;
	bool is_satisfy_level = false;
	bool is_satisfy_vip_level = false;

	// 角色等级
	int channel_open_level = 0;
	const ChannelChatLimitCfg *channel_chat_limit_cfg = LOGIC_CONFIG->GetAgentAdapterConfig().GetChannelChatLimitCfg(channel_type);
	if (nullptr != channel_chat_limit_cfg)
	{
		channel_open_level = channel_chat_limit_cfg->level_limit;
	}

	if (this->GetChannelLevelLimit(channel_type) > 0)
	{
		channel_open_level = this->GetChannelLevelLimit(channel_type);
	}

	if (/*role->IsIgnoreChatOrMailLevelLimit() ||*/ role->GetLevel() >= channel_open_level)
	{
		is_satisfy_level = true;
	}

	// vip等级
	int vip_level_limit = 0;
	if (nullptr != channel_chat_limit_cfg)
	{
		vip_level_limit = channel_chat_limit_cfg->vip_level_limit;
	}

	if (this->GetChannelVipLevelLimit(channel_type) > 0)
	{
		vip_level_limit = this->GetChannelVipLevelLimit(channel_type);
	}

	int vip_level = role->GetVip()->IsTimeLimitVip() ? 0 : role->GetVip()->GetVipLevel();
	if (vip_level >= vip_level_limit)
	{
		is_satisfy_vip_level = true;
	}

	const int chat_limit_condition_type = this->GetChatLimitConditionType(channel_type);
	if (CHAT_LIMIT_CONDITION_TYPE_OR == chat_limit_condition_type)
	{
		if (is_satisfy_level || is_satisfy_vip_level)
		{
			is_satisfy = true;
		}
	}
	else if (CHAT_LIMIT_CONDITION_TYPE_AND == chat_limit_condition_type)
	{
		if (is_satisfy_level && is_satisfy_vip_level)
		{
			is_satisfy = true;
		}
	}

	if (!is_satisfy)
	{
		return false;
	}

	return true;
}

void WorldStatus::CmdSetServerRoleNum(const int server_role_num)
{
   const DynamicAdjustOtherCfg &other_cfg = LOGIC_CONFIG->GetDynamicAdjustConfig().GetAdjustOtherCfg();

	int min_server_role_num = other_cfg.min_server_role_num;
	int max_server_role_num = other_cfg.max_server_role_num;
	int real_server_role_num = server_role_num;

	if (server_role_num < min_server_role_num)
	{
		real_server_role_num = min_server_role_num;
	}

	if (server_role_num > max_server_role_num)
	{
		real_server_role_num = max_server_role_num;
	}

	m_common_data.adjust_server_role_num = real_server_role_num;
}

int WorldStatus::GetRefreshTimeInterval(const int refresh_time_interval)
{
	if (refresh_time_interval <= 0)
	{
		return refresh_time_interval;
	}

	const DynamicAdjustOtherCfg &other_cfg = LOGIC_CONFIG->GetDynamicAdjustConfig().GetAdjustOtherCfg();
	int server_role_num = other_cfg.default_server_role_num;

	PlatName local_plat_name; memset(local_plat_name, 0, sizeof(local_plat_name));
	LocalConfig::Instance().GetPlatName(local_plat_name);
	const AgentDefaultServerRoleNumCfg *agent_default_cfg = LOGIC_CONFIG->GetDynamicAdjustConfig().GetAgentDefalutNumCfg(local_plat_name);
	if (nullptr != agent_default_cfg)
	{
		server_role_num = agent_default_cfg->default_server_role_num;
	}

	if (m_common_data.adjust_server_role_num > 0)
	{
		server_role_num = m_common_data.adjust_server_role_num;
	}

	float refresh_rate = LOGIC_CONFIG->GetDynamicAdjustConfig().GetRefreshAdjustTimeRate(server_role_num);

	int real_refresh_time_interval = static_cast<int>(refresh_time_interval * refresh_rate);

	return real_refresh_time_interval;
}

void WorldStatus::AddGuildGongziByMonster(Role *role, int monster_id)
{
	if (nullptr == role)
	{
		return;
	}

	const GuildGongziGiveCfg *cfg = LOGIC_CONFIG->GetGuildConfig().GetGongziGiveCfg(monster_id);
	if (nullptr == cfg)
	{
		return;
	}

	if (CrossConfig::Instance().IsHiddenServer())
	{
		long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());

		crossgameprotocal::GameCrossWorldCommonDataChange cross_msg;

		cross_msg.uuid = uuid;
		cross_msg.change_type = crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_GUILD_GONGZI;
		cross_msg.sub_type = cfg->gongzi_give_type;
		cross_msg.param_2 = role->GetGuildID();
		cross_msg.param_3 = cfg->give_gongzi;
		cross_msg.param_4 = monster_id;

		InternalComm::Instance().SendToCross((const char*)&cross_msg, sizeof(cross_msg));

		if (cfg->gongzi_give_type == GUILD_GONGZI_GIVE_TYPE_NORMAL)
		{
			role->SendAddGuildGongziInfo(cfg->give_gongzi);
		}
	}
	else
	{
		std::string reason = "WorldStatus::AddGuildGongziByMonster: monster_id:" + std::to_string(monster_id);
		GuildManager::Instance().OnAddGongzi(role->GetGuildID(), role->GetUID(), cfg->gongzi_give_type, cfg->give_gongzi, reason.c_str());
	}
}

void WorldStatus::AddGuildKillBossNum(UniqueUserID unique_uid, int scene_type)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (INVALID_UNIQUE_USER_ID == unique_uid)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(unique_uid);

	if (Scene::SCENE_TYPE_CROSS_LIEKUN_FB == scene_type ||
		Scene::SCENE_TYPE_CROSS_GUILDBATTLE == scene_type ||
		Scene::SCENE_TYPE_CROSS_BIANJINGZHIDI == scene_type )
	{
		crossgameprotocal::GameCrossWorldCommonDataChange cross_msg;

		cross_msg.uuid = uuid;
		cross_msg.change_type = crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_GUILD_KILL_BOSS;
		cross_msg.param_2 = UidToInt(unique_uid.user_id);
		cross_msg.param_3 = 1;

		InternalComm::Instance().SendToCross((const char*)&cross_msg, sizeof(cross_msg));
	}
}

void WorldStatus::AddTodayFirstChongzhiCount(int num)
{
	m_common_data.today_chongzhi_role_count += num;

	std::set<Role*> role_list;
	World::GetInstWorld()->GetSceneManager()->GetAllRole(role_list);
	for (auto role : role_list)
	{
		role->GetRoleActivity()->SendRAOpenServerSingleInfo(RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN);
	}
}

void WorldStatus::SyncKillCrossBossRankGuild(int mengzhu, GuildID guild_id)
{
	int old_mengzhu = m_common_data.kill_cross_boss_rank_mengzhu;
	m_common_data.kill_cross_boss_rank_mengzhu = mengzhu;

	Role * old_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(old_mengzhu));
	if (NULL != old_role)
	{
		old_role->SyncKillCrossBossRankSkill();
	}
	
	Role * role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(mengzhu));
	if (NULL != role)
	{
		role->SyncKillCrossBossRankSkill();
	}

	m_common_data.kill_corss_boss_rank_guild = guild_id;
}

int WorldStatus::GetKillCrossBossRankMengzhu()
{
	return m_common_data.kill_cross_boss_rank_mengzhu;
}

GuildID WorldStatus::GetKillCrossBossRankGuild()
{
	return m_common_data.kill_corss_boss_rank_guild;
}
