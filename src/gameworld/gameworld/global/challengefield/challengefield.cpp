#include <stdlib.h>
#include <time.h>
#include <vector>
#include "world.h"
#include "challengefield.hpp"
#include "challengeuser.hpp"

#include "rmibackobjdef.h"
#include "internalcomm.h"
#include "engineadapter.h"

#include "servercommon/errornum.h"
#include "servercommon/string/globalstr.h"
#include "servercommon/serverclmempool.h"
#include "servercommon/internalprotocal/ggprotocal.h"
#include "global/rmibackobjdef.h"
#include "obj/character/role.h"
#include "serverlogic.h"
#include "config/logicconfigmanager.hpp"
#include "global/challengefield/challengefieldconfig.hpp"
#include "servercommon/serverconfig/localconfig.hpp"
#include "config/joinlimitconfig.hpp"
#include "other/vip/vipconfig.hpp"
#include "config/sharedconfig/sharedconfig.h"

#include "servercommon/string/gameworldstr.h"
#include "gamelog.h"
#include "servercommon/struct/skilllistparam.h"
#include "scene/activityshadow/activityshadow.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "globalconfig/globalconfig.h"
#include "item/knapsack.h"
#include "other/vip/vip.hpp"
#include "other/shop/roleshop.hpp"
#include "other/capability/capability.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/sublock/sublock.hpp"
#include "other/event/eventhandler.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/route/mailroute.hpp"
#include "other/cross/rolecross.h"
#include "servercommon/string/gameworldstr.h"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/itemlevelreward/itemlevelreward.hpp"

#include <algorithm>
#include <vector>

ChallengeField & ChallengeField::Instance()
{
	static ChallengeField cf;
	return cf;
}

ChallengeField::ChallengeField()
	: m_data_state(CHALLENGE_FIELD_DATA_STATE_INVALID), m_is_first(true), m_rankuser_num(0), m_rankuser_list(NULL), m_dirty_mark(NULL), m_old_state(NULL), m_is_change(false), 
	m_last_save_time(0), m_last_check_give_lucky_reward_timestamp(0), m_last_check_give_rank_reward_timestamp(0)
{
	this->ResizeUserList(CHALLENGE_FIELD_MAX_USER_NUM);
}

ChallengeField::~ChallengeField()
{
	for (int i = 0; i < m_rankuser_num; i++)
	{
		if (NULL != m_rankuser_list[i])
		{
			delete m_rankuser_list[i];
		}
	}

	if (NULL != m_rankuser_list)
	{
		ServerMsgMem *tmp = (ServerMsgMem*)m_rankuser_list;
		delete []tmp;
		tmp = (ServerMsgMem*)m_dirty_mark;
		delete []tmp;
		tmp = (ServerMsgMem*)m_old_state;
		delete []tmp;
	}

	m_rankuser_num = 0;
}

void ChallengeField::ResizeUserList(int size)
{
	if (m_rankuser_num >= size) return;

	ChallengeUser **rankuser_list_new = (ChallengeUser **)(new ServerMsgMem[sizeof(ChallengeUser*) * size]);
	bool *dirty_mark_new = (bool *)(new ServerMsgMem[sizeof(bool) * size]);
	bool *old_state_new = (bool *)(new ServerMsgMem[sizeof(bool) * size]);

	memset(rankuser_list_new, 0, sizeof(ChallengeUser *) * size);
	memset(dirty_mark_new, 0, sizeof(bool) * size);
	memset(old_state_new, 0, sizeof(bool) * size);

	if (NULL != m_rankuser_list)
	{
		memcpy(rankuser_list_new, m_rankuser_list, sizeof(ChallengeUser*) * m_rankuser_num);
		memcpy(dirty_mark_new, m_dirty_mark, sizeof(bool) * m_rankuser_num);
		memcpy(old_state_new, m_old_state, sizeof(bool) * m_rankuser_num);

		ServerMsgMem *tmp = (ServerMsgMem*)m_rankuser_list;
		delete []tmp;
		tmp = (ServerMsgMem*)m_dirty_mark;
		delete []tmp;
		tmp = (ServerMsgMem*)m_old_state;
		delete []tmp;
	}

	m_rankuser_list = rankuser_list_new;
	m_dirty_mark = dirty_mark_new;
	m_old_state = old_state_new;

	m_rankuser_num = size;
}

bool ChallengeField::OnServerStart()
{
	return this->LoadChallengeUser(-1);
}

bool ChallengeField::LoadChallengeUser(int rank_from)
{
	if (this->IsLoadFinish()) return false;
	
	m_data_state = CHALLENGE_FIELD_DATA_STATE_LOADING;

	RMIInitChallengeUserObjectImpl *impl = new RMIInitChallengeUserObjectImpl();
	impl->challenge_field = this;

	RMIGlobalClient gc;
	gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());  
	return gc.InitChallengeUserAsyn(rank_from, impl);
}

void RMIInitChallengeUserObjectImpl::InitChallengeUserRet(int ret, const ChallengeUserListParam &challenge_user_p)
{
	if (0 != ret)
	{
		printf("ChallengeField::Init Failed ret:%d \n", ret);
		ServerLogic::GetInstServerLogic()->StopServer();
		return;
	}

	int next_rank_from = -1;

	for (int i = 0; i < challenge_user_p.count && i < CHALLENGE_FIELD_ONCE_OPER_USER_COUNT; i++)
	{
		if (!challenge_field->InitChallengeUser(challenge_user_p.user_list[i]))
		{
			printf("ChallengeField::InitChallengeUser Failed\n");
			ServerLogic::GetInstServerLogic()->StopServer();
			return;
		}

		if (challenge_user_p.user_list[i].rankpos > next_rank_from)
		{
			next_rank_from = challenge_user_p.user_list[i].rankpos;
		}
	}

	if (challenge_user_p.count > 0 )
	{
		challenge_field->LoadChallengeUser(next_rank_from);
	}
	else
	{
		challenge_field->LoadChallengeUserSucc();
	}
}

bool ChallengeField::InitChallengeUser(const ChallengeUserListParam::ChallengeUserAttr &user_attr)
{
	const int max_user_num = CrossConfig::Instance().IsHiddenServer() ? CROSS_CHALLENGE_FIELD_MAX_USER : CHALLENGE_FIELD_MAX_USER;
	int rankpos = user_attr.rankpos;
// 	if (CrossConfig::Instance().IsHiddenServer())
// 	{
// 		if (rankpos >= m_rankuser_num)
// 		{
// 			UNSTD_STATIC_CHECK(CHALLENGE_FIELD_INCREASE_STEP > 0);
// 
// 			int new_size = (rankpos / CHALLENGE_FIELD_INCREASE_STEP + 1) * CHALLENGE_FIELD_INCREASE_STEP;
// 			if (new_size > CHALLENGE_FIELD_MAX_USER) new_size = CHALLENGE_FIELD_MAX_USER;
// 			this->ResizeUserList(new_size);
// 		}
// 
// 		m_dirty_mark[rankpos] = true;
// 		m_old_state[rankpos] = true;
// 		m_is_change = true;
// 		return true;
// 	}

	if (rankpos < 0 || rankpos >= max_user_num)
	{
		gamelog::g_log_serious_error.printf(LL_CRITICAL, "[ChallengeField::Init][rankpos:%d role_id:%lld is invalid]", rankpos, user_attr.role_id);
		return false;
	}

	if (rankpos >= 0 && rankpos < m_rankuser_num && NULL != m_rankuser_list[rankpos])
	{	
		gamelog::g_log_serious_error.printf(LL_CRITICAL, "[ChallengeField::Init][ranekpos:%d repeat]", rankpos);
		return false;
	}

	UniqueUserID user_id = LongLongToUniqueUserID(user_attr.role_id);
	if (UidToInt(user_id.user_id)!= user_attr.challenge_p.role_id)
	{
		gamelog::g_log_serious_error.printf(LL_CRITICAL, "[ChallengeField::Init][rankpos:%d role_id:%d is not match with ChallengeParam.role_id:%d]", rankpos, UidToInt(user_id.user_id), user_attr.challenge_p.role_id);
		return false;
	}

	if (m_rankpos_map.end() == m_rankpos_map.find(LongLongToUniqueUserID(user_attr.role_id)))
	{
		ChallengeUser *challenge_user = new ChallengeUser(this);
		challenge_user->Init(user_attr);
		this->SetChallengeUser(rankpos, challenge_user, true);
	}
	else
	{
		// 容错处理，防止数据库里边一个role_id有2个以上排名
		this->SetChallengeUser(rankpos, NULL, false);
	}

	m_old_state[rankpos] = true;

	return true;
}

void ChallengeField::LoadChallengeUserSucc()
{
	m_data_state = CHALLENGE_FIELD_DATA_STATE_LOAD_FINISH;
// 	if (CrossConfig::Instance().IsHiddenServer())
// 	{
// 		if (m_challenge_field_data.clear_data_version != 2)
// 		{
// 			m_challenge_field_data.clear_data_version = 2;
// 			ResetCrossChallengeUserRank();
// 		}
// 	}

	m_last_save_time = EngineAdapter::Instance().Time();

	if (m_rankpos_map.empty())
	{
		const std::vector<ChallengeFieldRobotConfig> &robot_name_vector = LOGIC_CONFIG->GetChallengeFieldCfg().GetChallengeFieldRobotName();
		
		int robot_num = static_cast<int>(robot_name_vector.size());
		if (robot_num >= ChallengeFieldRobotConfig::CONFIG_ITEM_MAX_COUNT)
		{
			robot_num = ChallengeFieldRobotConfig::CONFIG_ITEM_MAX_COUNT;
		}

		for (int i = 0; i < robot_num; ++ i)
		{
			ChallengeFieldRobotConfig robot_cfg = robot_name_vector[i];

			UserID user_id(CHALLENGE_FIELD_ROBOT_SERVER_ID, i + 1);
			UniqueUserID unique_userid(CHALLENGE_FIELD_ROBOT_PLAT_TYPE, user_id);
			ChallengeParam param;
			RoleAppearance appearance;
			{
				param.plat_type = CHALLENGE_FIELD_ROBOT_PLAT_TYPE;
				param.role_id = UidToInt(user_id);
				SNPRINTF(param.role_name, sizeof(param.role_name), "%s", robot_cfg.robot_name);
				param.avatar = 0;
				//if (robot_cfg.is_sex_common)
				//{
				//	param.sex = static_cast<char>(RandomNum(SEX_MAX));
				//}
				//else
				//{
				//	param.sex = static_cast<char>(robot_cfg.robot_sex);
				//}

				if (PROF_TYPE_PROF_4 >= PROF_TYPE_PROF_1)
				{
					param.prof = PROF_TYPE_PROF_1 + RandomNum(PROF_TYPE_PROF_4 - PROF_TYPE_PROF_1 + 1);
				}

				if (param.prof == PROF_TYPE_PROF_1 || param.prof == PROF_TYPE_PROF_2)
				{
					param.sex = MALE;
				}
				else
				{
					param.sex = FEMALE;
				}

				param.camp = 0;
				param.move_speed = BIRTH_MOVESPEED_BASE;
				param.vip_level = 0;

				if (CrossConfig::Instance().IsHiddenServer())
				{
					if (!GenrateCrossChallengeUserParamInfo(param, appearance, i, robot_num))
					{
						continue;
					}
				}
				else
				{
					if (!GenrateChallengeUserParamInfo(param, appearance, i, robot_num))
					{
						continue;
					}
				}
			}

			{
				// 机器人数据插入到榜单
				int rankuser_index = -1;
				ChallengeUser *challenge_user = this->GetChallengeUser(unique_userid, &rankuser_index);

				if (NULL == challenge_user)
				{
					const int max_user_num = CrossConfig::Instance().IsHiddenServer() ? CROSS_CHALLENGE_FIELD_MAX_USER : CHALLENGE_FIELD_MAX_USER;
					if (static_cast<int>(m_rankpos_map.size()) < max_user_num)
					{
						int rankpos = (int)m_rankpos_map.size();
						challenge_user = this->CreateChallengeUser(unique_userid, param, appearance);
						this->SetChallengeUser(rankpos, challenge_user);
					}
				}
			}
		}
	}
}

void ChallengeField::OnServerStop()
{
	this->CheckSave(EngineAdapter::Instance().Time() + CHALLENGE_FIELD_SAVE_INTERVAL * 2);
}

void ChallengeField::Init(const ChallengeFieldDataParam &data_param)
{
	m_challenge_field_data = data_param;

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (0 == m_challenge_field_data.next_give_luckyreward_timestamp)
	{
		int next_give_rank_reward_interval = EngineAdapter::Instance().NextDayInterval(CHALLENGE_FIELD_GIVE_RANK_REWARD_HOUR, 0, 0);
		m_challenge_field_data.next_give_luckyreward_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time() + next_give_rank_reward_interval);
		
		this->GenrateLuckyRankPos();
	}

	if (0 == m_challenge_field_data.next_give_rankreward_timestamp)
	{
		int reward_percent = 0;
		unsigned int next_give_rankreward_timestamp = LOGIC_CONFIG->GetChallengeFieldCfg().CalNextRankRewadTime(&reward_percent);

		m_challenge_field_data.next_give_rankreward_timestamp = next_give_rankreward_timestamp;
		m_challenge_field_data.rankreward_percent = reward_percent;

		// 初始的奖励结算天数一定是有效的开服天数
		{
			m_challenge_field_data.reward_opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
			if (m_challenge_field_data.reward_opengame_day <= 0)
			{
				m_challenge_field_data.reward_opengame_day = 1;
			}
		}
	}
}

void ChallengeField::GetInitParam(ChallengeFieldDataParam *data_param)
{
	*data_param = m_challenge_field_data;
}

void ChallengeField::Update(unsigned long interval, time_t now_second)
{
	if (!this->IsLoadFinish())
	{
		return;
	}

	if (CrossConfig::Instance().IsHiddenServer())
	{
		this->CheckSave(now_second);
		return;
	}

	if (m_is_first)
	{
		m_is_first = false;

		for (int i = 0; i < m_rankuser_num; ++ i)
		{
			if (NULL != m_rankuser_list[i])
			{
				if (CrossConfig::Instance().IsHiddenServer())
				{
					this->CrossRandOpponentHelper(m_rankuser_list[i]->GetUniqueUserID());
				}
				else
				{
					this->RandOpponentHelper(m_rankuser_list[i]->GetUniqueUserID());
				}
			}
		}
	}

	this->CheckSave(now_second);

	//this->CheckGiveLuckyReward(now_second);
	this->CheckGiveRankReward(now_second);
}

void ChallengeField::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid)
	{
		return;
	}

	if (CrossConfig::Instance().IsHiddenServer())
	{
		this->CheckGiveCrossRankReward();
	}
	else
	{
		for (RankPosMapIt it = m_rankpos_map.begin(); m_rankpos_map.end() != it; ++it)
		{
			int rank_pos = it->second;
			if (rank_pos >= 0 && rank_pos < m_rankuser_num && NULL != m_rankuser_list[rank_pos])
			{
				m_rankuser_list[rank_pos]->OnDayChange(old_dayid, now_dayid);
				this->SetDirty(rank_pos);
			}
		}
	}
}

void ChallengeField::SetDirty(int rankpos)
{
	if (rankpos >= 0 && rankpos < m_rankuser_num)
	{
		m_dirty_mark[rankpos] = true;
		m_is_change = true;
	}
}

bool ChallengeField::IsChallengeOpen()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return false;
	}
	// 开服持续X天，在最后一天的最后一次结算后活动结束
	int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	const int CHALLENGE_FIELD_OPEN_DUR_DAY = LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg().open_dur_day;

	if (opengame_day > CHALLENGE_FIELD_OPEN_DUR_DAY) return false;
	if (opengame_day < CHALLENGE_FIELD_OPEN_DUR_DAY) return true;
	
	// 结束当天
	const ChallengeFieldRankRewardTime *reward_time = LOGIC_CONFIG->GetChallengeFieldCfg().GetLastEndRankRewardTime();
	if (nullptr == reward_time)
	{
		return false;
	}

	// 结算时即为活动结束
	time_t now_time = EngineAdapter::Instance().Time();
	time_t close_time = GetZeroTime(now_time) + reward_time->hour * SECOND_PER_HOUR + reward_time->minute * SECOND_PER_MIN;

	return now_time < close_time;
}

int ChallengeField::GetEndRankRewardOpenday()
{
	int open_day = LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg().open_dur_day;

	return open_day;
}

void ChallengeField::GetChangeList(ChallengeUserListParam *p, int rank_begin, int *rank_end) const
{
	if (!m_is_change || rank_begin < 0 || rank_begin >= m_rankuser_num)
	{
		p->count = 0;
		return;
	}

	int count = 0;
	int i = 0;
	for (i = rank_begin; i < m_rankuser_num && count < CHALLENGE_FIELD_ONCE_OPER_USER_COUNT; ++i)
	{
		if (m_dirty_mark[i])
		{
			if (NULL == m_rankuser_list[i])
			{
				if (!m_old_state[i]) continue;

				p->user_list[count].change_state = structcommon::CS_DELETE;
				p->user_list[count].rankpos = i;
				p->user_list[count].role_id = 0;
			}
			else
			{
				p->user_list[count].change_state = m_old_state[i] ? structcommon::CS_UPDATE : structcommon::CS_INSERT;
				p->user_list[count].rankpos = i;
				p->user_list[count].role_id = UniqueUserIDToLongLong(m_rankuser_list[i]->GetUniqueUserID());

				m_rankuser_list[i]->GetAttr(&p->user_list[count]);
			}

			++ count;
		}
	}

	p->count = count;
	if (NULL != rank_end) *rank_end = i;
}

void ChallengeField::ClearDirtyMark(int rank_begin, int rank_end)
{
	if (m_is_change)
	{
		if (rank_begin >= 0 && rank_begin < m_rankuser_num && rank_end >= 0 && rank_end <= m_rankuser_num && rank_begin < rank_end)
		{
			for (int i = rank_begin; i < rank_end; ++i)
			{
				m_dirty_mark[i] = false;
				m_old_state[i] = (NULL != m_rankuser_list[i]);
			}

			if (rank_end >= m_rankuser_num) m_is_change = false;
		}
	}
}

void ChallengeField::CheckSave(time_t now_second)
{
	UNSTD_STATIC_CHECK(CHALLENGE_FIELD_ONCE_OPER_USER_COUNT > 0);

	if (!this->IsLoadFinish()) return;

	if (now_second >= m_last_save_time + CHALLENGE_FIELD_SAVE_INTERVAL)
	{
		if (m_is_change)
		{
			static ChallengeUserListParam p;
			int rank_begin = 0, rank_end = 0;

			int loop_count = 0;
			while (++ loop_count <= 128)
			{
				this->GetChangeList(&p, rank_begin, &rank_end);
				
				if (p.count > 0)
				{
					RMISaveChallengeUserObjectImpl *impl = new RMISaveChallengeUserObjectImpl();

					RMIGlobalClient gc;
					gc.__bind_session(InternalComm::Instance().GetDBHandler().GetGlobalDB());  
					if (gc.SaveChallengeUserAsyn(p, impl))
					{
						this->ClearDirtyMark(rank_begin, rank_end);
					}
					else
					{
						break;
					}

					rank_begin = rank_end;
				}
				else
				{
					m_is_change = false;
					break;
				}
			};
		}

		m_last_save_time = now_second;
	}
}

int ChallengeField::GetFreeRankPosSlot()
{
	for (int i = 0; i < m_rankuser_num; ++ i)
	{
		if (NULL == m_rankuser_list[i]) return i;
	}

	return -1;
}

ChallengeUser * ChallengeField::CreateChallengeUser(const UniqueUserID &user_id, const ChallengeParam &challenge_p, const RoleAppearance &challenge_appearance)
{
	static ChallengeUserListParam::ChallengeUserAttr attr;

	attr.rankpos = -1;
	attr.role_id = UniqueUserIDToLongLong(user_id);
	attr.challenge_p = challenge_p;
	attr.role_appearance = challenge_appearance;
	attr.challenge_p.role_id = UidToInt(user_id.user_id);

	ChallengeUser *challenge_user = new ChallengeUser(this);
	challenge_user->Init(attr);

	return challenge_user;
}

void ChallengeField::SetChallengeUser(int rankpos, ChallengeUser *challenge_user, bool is_init , bool need_reset_opponen)
{
	const int max_user_num = CrossConfig::Instance().IsHiddenServer() ? CROSS_CHALLENGE_FIELD_MAX_USER : CHALLENGE_FIELD_MAX_USER;
	if (rankpos >= 0 && rankpos < max_user_num)
	{
		if (rankpos >= m_rankuser_num)
		{
			UNSTD_STATIC_CHECK(CHALLENGE_FIELD_INCREASE_STEP > 0);

			int new_size = (rankpos / CHALLENGE_FIELD_INCREASE_STEP + 1) * CHALLENGE_FIELD_INCREASE_STEP;
			if (new_size > max_user_num) new_size = max_user_num;
			this->ResizeUserList(new_size);
		}

		m_rankuser_list[rankpos] = challenge_user;

		if (NULL != challenge_user)
		{
			m_rankpos_map[challenge_user->GetUniqueUserID()] = rankpos;
			challenge_user->SetRankPos(rankpos, is_init, need_reset_opponen);

			this->OnRankPosChange(challenge_user->GetUniqueUserID(), rankpos);
		}

		if (!is_init) this->SetDirty(rankpos);
	}
	else
	{
		if (NULL != challenge_user && CrossConfig::Instance().IsHiddenServer())
		{
			challenge_user->SetRankPos(rankpos, true, false);
			auto it = m_rankpos_map.find(challenge_user->GetUniqueUserID());
			if (it != m_rankpos_map.end())
			{
				m_rankpos_map.erase(it);
			}
		}
	}
}

bool ChallengeField::GenrateChallengeUserParamInfo(ChallengeParam &param, RoleAppearance &appearance, int rank_index, int robot_num)
{
	if (rank_index < 0) return false;
	if (robot_num <= 0) return false;

	if (rank_index >= robot_num)
	{
		rank_index = robot_num - 1;
	}
	
	int rank = rank_index + 1;

	float rate = 1 - (rank - 1) / (robot_num * 1.0f);
	param.level = 130 + static_cast<int>(70 * rate);
	if (param.level < 0) param.level = 1;

	//int x = robot_num - rank;

	//2019.02.28 文熙需求
	// 	if (x < 980)
	// 	{
	// 		param.capability = static_cast<int>(5000.0 + 145000.0 * (2.0 * x  * x + 1000.0 * x - 1002.0) / (2.0 * robot_num * robot_num + 1000.0 * robot_num - 1002));
	// 	}
	// 	else
	// 	{
	// 		param.capability = static_cast<int>(-8.413 * pow((robot_num - x), 3) + 969.9 * pow((robot_num - x), 2) - 29019 * (robot_num - x) + 415325);
	// 	}

	int num_1 = 500000 - 25000 * (rank - 1);
	int num_2 = 400000 - 20000 * (rank - 5);
	int num_3 = 300000 - 10000 * (rank - 10);
	int num_4 = 200000 - 1000 * (rank - 20);
	int num_5 = 180000 - 500 * (rank - 40);
	int num_6 = 100000 - 80 * rank;

	std::vector<int> num_list = { num_1, num_2, num_3, num_4, num_5, num_6 };

	auto biggest_it = std::max_element(std::begin(num_list), std::end(num_list));
	param.capability = *biggest_it / 2;

	param.max_hp = static_cast<Attribute>(param.capability * 1.724137931);
	param.gongji = static_cast<Attribute>(param.capability * 0.25862069);
	param.fangyu = static_cast<Attribute>(param.capability * 0.086206897);
	param.mingzhong = static_cast<Attribute>(param.capability * 0.017241379);
	param.shanbi = static_cast<Attribute>(param.capability * 0.017241379);
	param.baoji = static_cast<Attribute>(param.capability * 0.017241379);
	param.jianren = static_cast<Attribute>(param.capability * 0.017241379);
	param.fujiashanghai = 0;
	param.dikangshanghai = 0;
	param.jingzhun_per = 0;
	param.baoji_per = 0;
	param.kangbao_per = 0;
	param.pofang_per = 0;
	param.mianshang_per = 0;
	param.constant_zengshang = 0;
	param.constant_mianshang = 0;

	int skill_level = param.level / 10;
	if (skill_level <= 0) skill_level = 1;

	param.guild_post = 0;
	param.guild_id = INVALID_GUILD_ID;
	memset(param.guild_name, 0, sizeof(param.guild_name));
	param.avatar_timestamp = 0;

	{
		// 外观信息
		UNSTD_STATIC_CHECK(CAMP_TYPE_MAX > 1);
		UNSTD_STATIC_CHECK(PROF_TYPE_PROF_NO_LIMIT >= 5);

		static ItemID item_id_list[] = { 0, 8110, 8210, 8310, 8410 };

		appearance.wuqi_id = 0;
		if (param.prof >= 0 && param.prof < static_cast<char>(sizeof(item_id_list) / sizeof(item_id_list[0])))
		{
			appearance.wuqi_id = item_id_list[(int)param.prof];
		}

		appearance.wing_used_imageid = 1 + RandomNum(5);

		{
			// 个人技能信息
			param.skill_list[0].skill_id = param.prof * 100 + 21;
			param.skill_list[0].level = skill_level;

			param.skill_list[1].skill_id = param.prof * 100 + 31;
			param.skill_list[1].level = skill_level;

			param.skill_list[2].skill_id = param.prof * 100 + 41;
			param.skill_list[2].level = skill_level;

			param.skill_list[3].skill_id = param.prof * 100 + 11;
			param.skill_list[3].level = skill_level;

			param.skill_list[4].skill_id = param.prof * 100 + 1;
			param.skill_list[4].level = skill_level;
		}
	}

	return true;
}

ChallengeUser * ChallengeField::GetChallengeUser(const UniqueUserID &user_id, int *p_rankpos)
{
	RankPosMapIt it = m_rankpos_map.find(user_id);
	if (it != m_rankpos_map.end())
	{
		int rank_pos = it->second;
		if (rank_pos >= 0 && rank_pos < m_rankuser_num)
		{
			ChallengeUser *challenge_user = m_rankuser_list[rank_pos];
			if (NULL != challenge_user && challenge_user->GetUniqueUserID() == user_id && challenge_user->GetRankPos() == rank_pos)
			{
				if (NULL != p_rankpos) *p_rankpos = rank_pos;
				return challenge_user;
			}
		}
	}

	return NULL;
}

void ChallengeField::GMSetEmemySelf(Role *role, int percent)
{
	if (nullptr == role) return;

	ChallengeUser *challenge_user = this->GetChallengeUser(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (nullptr == challenge_user) return;

	int rankpos = (int)m_rankpos_map.size();

	UserID user_id(CHALLENGE_FIELD_ROBOT_SERVER_ID, rankpos);
	UniqueUserID unique_userid(CHALLENGE_FIELD_ROBOT_PLAT_TYPE, user_id);

	ChallengeParam param = challenge_user->GetChallengeParam();
	RoleAppearance role_appearance = challenge_user->GetChallengeAppearance();
	ChallengeParam copy_param = param;
	copy_param.role_id = UidToInt(user_id);
	copy_param.capability = static_cast<int>(role->GetCapability()->GetTotalCapability() * (percent / 100.0f));
	SNPRINTF(copy_param.role_name, sizeof(copy_param.role_name), "robot_%s", role->GetName());

	{
		// 机器人数据插入到榜单
		int rankuser_index = -1;
		ChallengeUser *challenge_user = this->GetChallengeUser(unique_userid, &rankuser_index);

		if (NULL == challenge_user)
		{
			const int max_user_num = CrossConfig::Instance().IsHiddenServer() ? CROSS_CHALLENGE_FIELD_MAX_USER : CHALLENGE_FIELD_MAX_USER;
			if (static_cast<int>(m_rankpos_map.size()) < max_user_num)
			{
				
				challenge_user = this->CreateChallengeUser(unique_userid, copy_param, role_appearance);
				this->SetChallengeUser(rankpos, challenge_user);
			}
		}
	}

	ChallengeFieldOpponentInfo opponent_userid_list[CHALLENGE_FIELD_OPPONENT_MAX];
	opponent_userid_list[0].user_id = unique_userid;
	opponent_userid_list[0].rank_pos = rankpos;

	challenge_user->ResetOpponentUserList(opponent_userid_list);
	challenge_user->SetOpponentUser(0);

	return;
}


bool ChallengeField::IsCanSyncCrossChallengeUserInfo()
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return false;
	}

	if (!this->IsLoadFinish())
	{
		return false;
	}

	if (!CrossConfig::Instance().CheckCrossServerHasCrossActivity(CROSS_ACTIVITY_TYPE_CROSS_CHALLENGEFIELD))
	{
		return false;
	}

	return true;
}

void ChallengeField::OnSyncChallengeUserInitDataToGame(NetID net_id)
{
	if (!IsCanSyncCrossChallengeUserInfo())
	{
		return;
	}
	
	static crossgameprotocal::SyncCrossChallengefieldDataInfo cross_msg;
	cross_msg.sync_type = crossgameprotocal::SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_TO_INIT_GAME;
	cross_msg.sub_type = crossgameprotocal::SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_TO_INIT_GAME_BEGIN;
	cross_msg.count = 0;

	auto send_data_info = [=](const crossgameprotocal::SyncCrossChallengefieldDataInfo& data)
	{
		int send_len = sizeof(data) - (CHALLENGE_FIELD_MAX_USER_NUM - data.count) * sizeof(data.user_list[0]);
		if (net_id == 0)
		{
			InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_COMMON, 0, (const char*)& data, send_len);
		}
		else
		{
			InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_GLOBAL_NETID, net_id, (const char*)& data, send_len);
		}
	};

	//begin
	send_data_info(cross_msg);

	//info
	cross_msg.sub_type = crossgameprotocal::SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_TO_INIT_GAME_INFO;
	for (auto it : m_rankpos_map)
	{
		if (cross_msg.count >= CHALLENGE_FIELD_MAX_USER_NUM)
		{
			send_data_info(cross_msg);
			cross_msg.count = 0;
		}

		ChallengeUser *challenge_user = this->GetChallengeUserByPos(it.second);
		if (NULL == challenge_user)
		{
			continue;
		}

		ChallengeUserListParam::ChallengeUserAttr &user_data = cross_msg.user_list[cross_msg.count];
		challenge_user->GetAttr(&user_data);

		cross_msg.count++;
	}

	//end
	cross_msg.sub_type = crossgameprotocal::SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_TO_INIT_GAME_END;
	send_data_info(cross_msg);
}

void ChallengeField::OnSyncOrignalChallengefieldInfo(crossgameprotocal::SyncCrossChallengefieldDataInfo *sccbi)
{
	if (!IsCanSyncCrossChallengeUserInfo())
	{
		return;
	}

	if (nullptr == sccbi)
	{
		return;
	}

	for (unsigned int i = 0; i < (unsigned int)sccbi->count && i < static_array_size(sccbi->user_list); ++i)
	{
		auto it = m_rankpos_map.find(LongLongToUniqueUserID(sccbi->user_list[i].role_id));
		int rank_pos = -1;
		if (it == m_rankpos_map.end())
		{
			rank_pos = (int)m_rankpos_map.size();
			if (rank_pos >= 0 && rank_pos < m_rankuser_num && NULL != m_rankuser_list[rank_pos])
			{
				rank_pos = this->GetFreeRankPosSlot();
				if (rank_pos < 0 || rank_pos >= m_rankuser_num || NULL != m_rankuser_list[rank_pos])
				{
					continue;
				}
			}

			if (rank_pos >= CROSS_CHALLENGE_FIELD_MAX_USER)
			{
				continue;
			}

			ChallengeUser *challenge_user = new ChallengeUser(this);
			challenge_user->Init(sccbi->user_list[i]);
			this->SetChallengeUser(rank_pos, challenge_user, true);
			
		}
		else
		{
			rank_pos = it->second;
			if (rank_pos >= 0 && rank_pos < CROSS_CHALLENGE_FIELD_MAX_USER)
			{
				ChallengeUser *challenge_user = m_rankuser_list[rank_pos];
				if (NULL != challenge_user)
				{
					delete challenge_user;
				}

				ChallengeUser *tmp_challenge_user = new ChallengeUser(this);
				tmp_challenge_user->Init(sccbi->user_list[i]);

				this->SetChallengeUser(rank_pos, tmp_challenge_user, false, true);
			}
		}
	}

	this->OnUpdateChallengefieldInfoToGame();
}

void ChallengeField::OnSyncChallengefieldRankInfo(crossgameprotocal::SyncCrossChallengefieldDataInfo *sccbi)
{
	if (!IsCanSyncCrossChallengeUserInfo())
	{
		return;
	}

	if (nullptr == sccbi)
	{
		return;
	}

	// 获取双方实际排名
	int swap_rank_list[CHALLENGE_FIELD_SIDE_MAX] = { -1 };
	for (unsigned int i = 0; i < (unsigned int)sccbi->count && i < static_array_size(sccbi->user_list); ++i)
	{
		if (i >= CHALLENGE_FIELD_SIDE_MAX)
		{
			break;
		}

		auto it = m_rankpos_map.find(LongLongToUniqueUserID(sccbi->user_list[i].role_id));
		//int rank_pos = 0;
		if (it != m_rankpos_map.end())
		{
			swap_rank_list[i] = it->second;
		}
		else
		{
			swap_rank_list[i] = sccbi->user_list[i].rankpos;
		}
	}

	int insert_rank_num = 0;
	for (int i = 0; i < CHALLENGE_FIELD_SIDE_MAX; i ++)
	{
		if (swap_rank_list[i] < 0 || swap_rank_list[i] >= m_rankuser_num)
		{
			insert_rank_num++;
		}
	}

	if (insert_rank_num >= CHALLENGE_FIELD_SIDE_MAX)
	{
		return;
	}

	// 如果挑战者排名高则不替换排名
	if (swap_rank_list[CHALLENGE_FIELD_SIDE_SPONSOR] < swap_rank_list[CHALLENGE_FIELD_SIDE_OPPONENT])
	{
		return;
	}

	// 获取ChallengeUser指针
	ChallengeUser *sponsor_user = NULL;
	ChallengeUser *opponent_user = NULL;

	if (swap_rank_list[CHALLENGE_FIELD_SIDE_SPONSOR] >= CROSS_CHALLENGE_FIELD_MAX_USER)
	{
		sponsor_user = new ChallengeUser(this);
		sponsor_user->Init(sccbi->user_list[CHALLENGE_FIELD_SIDE_SPONSOR]);
	}
	else if(swap_rank_list[CHALLENGE_FIELD_SIDE_SPONSOR] >= 0 || swap_rank_list[CHALLENGE_FIELD_SIDE_SPONSOR] < m_rankuser_num)
	{
		sponsor_user = m_rankuser_list[swap_rank_list[CHALLENGE_FIELD_SIDE_SPONSOR]];
	}

	if (swap_rank_list[CHALLENGE_FIELD_SIDE_OPPONENT] >= CROSS_CHALLENGE_FIELD_MAX_USER)
	{
		opponent_user = new ChallengeUser(this);
		opponent_user->Init(sccbi->user_list[CHALLENGE_FIELD_SIDE_OPPONENT]);
	}
	else if (swap_rank_list[CHALLENGE_FIELD_SIDE_OPPONENT] >= 0 || swap_rank_list[CHALLENGE_FIELD_SIDE_OPPONENT] < m_rankuser_num)
	{
		opponent_user = m_rankuser_list[swap_rank_list[CHALLENGE_FIELD_SIDE_OPPONENT]];
	}

	const std::vector<ChallengeFieldRobotConfig> &robot_name_vector = LOGIC_CONFIG->GetChallengeFieldCfg().GetChallengeFieldRobotName();
	int robot_num = static_cast<int>(robot_name_vector.size());
	if (robot_num >= ChallengeFieldRobotConfig::CONFIG_ITEM_MAX_COUNT)
	{
		robot_num = ChallengeFieldRobotConfig::CONFIG_ITEM_MAX_COUNT;
	}

	if (NULL != sponsor_user && NULL != opponent_user)
	{
		// 发起者占领对方的排行名次
		if (sponsor_user->IsRobot() && (swap_rank_list[CHALLENGE_FIELD_SIDE_OPPONENT] < CROSS_CHALLENGE_FIELD_MAX_USER) )
		{
			// 如果对方是机器人，那么需要重新将机器人恢复为发起者名次对应的属性
			this->GenrateCrossChallengeUserParamInfo(sponsor_user->GetParam(), sponsor_user->GetAppearance(), swap_rank_list[CHALLENGE_FIELD_SIDE_OPPONENT], robot_num);
		}
		this->SetChallengeUser(swap_rank_list[CHALLENGE_FIELD_SIDE_OPPONENT], sponsor_user, false, true);

		// 对方名次下降到发起者的名次 
		if (opponent_user->IsRobot() && (swap_rank_list[CHALLENGE_FIELD_SIDE_SPONSOR] < CROSS_CHALLENGE_FIELD_MAX_USER))
		{
			// 如果对方是机器人，那么需要重新将机器人恢复为发起者名次对应的属性
			this->GenrateCrossChallengeUserParamInfo(opponent_user->GetParam(), opponent_user->GetAppearance(), swap_rank_list[CHALLENGE_FIELD_SIDE_SPONSOR], robot_num);
		}
		this->SetChallengeUser(swap_rank_list[CHALLENGE_FIELD_SIDE_SPONSOR], opponent_user, false, true);

		if (sponsor_user->GetRankPos() < 20)
		{
			int length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "cross_challengefield_rankpos_first_changed",
				UidToInt(sponsor_user->GetUserID()), sponsor_user->GetName(), (int)sponsor_user->GetCamp(), sponsor_user->GetRankPos() + 1,
				UidToInt(opponent_user->GetUserID()), opponent_user->GetName(), (int)opponent_user->GetCamp());
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
		}

// 		Role *opponent_role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(UniqueUserIDToLongLong(opponent_userid));
// 		if (NULL != opponent_role)
// 		{
// 			Protocol::SCCrossChallengeFieldBeDefeatNotice cfbdn;
// 			EngineAdapter::Instance().NetSend(opponent_role->GetNetId(), (const char *)&cfbdn, sizeof(cfbdn));
// 		}

		// 连胜
		{
			sponsor_user->OnCrossWin(true);
			opponent_user->OnCrossFail();
		}

		// 发送跨服邮件
		if (!opponent_user->IsRobot())
		{
			static MailContentParam contentparam; contentparam.Reset();
			
			int length = gstr::MailContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), "cross_challengefield_rank_drop", sponsor_user->GetName());
			if (length)
			{
				MailRoute::MailToCrossUser(opponent_user->GetUniqueUserID(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}

		{
			// 增加战报
			unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

			{
				ChallengeUserReport::ReportItem report_item;
				report_item.is_sponsor = 1;
				report_item.is_win = 1;
				report_item.challenge_time = now_time;
				report_item.target_uid = UniqueUserIDToLongLong(opponent_user->GetUniqueUserID());
				opponent_user->GetName(report_item.target_name);
				report_item.old_rankpos = opponent_user->GetRankPos();
				report_item.new_rankpos = sponsor_user->GetRankPos();

				sponsor_user->AddReport(report_item);
			}

			{
				ChallengeUserReport::ReportItem report_item;
				report_item.is_sponsor = 0;
				report_item.is_win = 0;
				report_item.challenge_time = now_time;
				report_item.target_uid = UniqueUserIDToLongLong(sponsor_user->GetUniqueUserID());
				sponsor_user->GetName(report_item.target_name);
				report_item.old_rankpos = sponsor_user->GetRankPos();
				report_item.new_rankpos = opponent_user->GetRankPos();

				opponent_user->AddReport(report_item);
			}
		}

		// 释放内存,放最后
		{
			if (sponsor_user->GetRankPos() >= CROSS_CHALLENGE_FIELD_MAX_USER)
			{
				delete sponsor_user;
				sponsor_user = NULL;
			}

			if (opponent_user->GetRankPos() >= CROSS_CHALLENGE_FIELD_MAX_USER)
			{
				delete opponent_user;
				opponent_user = NULL;
			}
		}

		this->OnUpdateChallengefieldInfoToGame();
	}
}

void ChallengeField::OnUpdateChallengefieldInfoToGame()
{
	if (!IsCanSyncCrossChallengeUserInfo())
	{
		return;
	}

	if (m_is_change)
	{
		static ChallengeUserListParam p;
		int rank_begin = 0, rank_end = 0;

		int loop_count = 0;
		while (++loop_count <= 128)
		{
			this->GetChangeList(&p, rank_begin, &rank_end);

			static crossgameprotocal::SyncCrossChallengefieldDataInfo cross_msg;
			cross_msg.sync_type = crossgameprotocal::SYNC_CROSS_CHALLENGEFILED_INFO_TYPE_UPDATE_TO_GAME;
			cross_msg.count = 0;

			for (int count = 0; count < p.count; count++)
			{
				if (cross_msg.count >= CHALLENGE_FIELD_MAX_USER_NUM)
				{
					break;
				}

				ChallengeUserListParam::ChallengeUserAttr &user_data = cross_msg.user_list[cross_msg.count];

				user_data = p.user_list[count];

				cross_msg.count++;
			}

			if (cross_msg.count > 0)
			{
				int send_len = sizeof(cross_msg) - (CHALLENGE_FIELD_MAX_USER_NUM - cross_msg.count) * sizeof(cross_msg.user_list[0]);
				InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_COMMON, 0, (const char *)&cross_msg, send_len);
			}

			if (p.count > 0)
			{
				if (cross_msg.count >= p.count)
				{
				}
				else
				{
					break;
				}

				rank_begin = rank_end;
			}
			else
			{
				break;
			}
		};
	}
}

void ChallengeField::CheckGiveCrossRankReward()
{
	if (!IsCanSyncCrossChallengeUserInfo())
	{
		return;
	}

	const tm *local_tm = EngineAdapter::Instance().LocalTime();
	if (NULL == local_tm)
	{
		return;
	}

	bool is_reward_day = LOGIC_CONFIG->GetChallengeFieldCfg().IsCrossRewardDay(local_tm->tm_wday);
	if (!is_reward_day)
	{
		return;
	}

	for (RankPosMapIt it = m_rankpos_map.begin(); m_rankpos_map.end() != it; ++it)
	{
		int rank_pos = it->second;
		if (rank_pos >= 0 && rank_pos < m_rankuser_num && NULL != m_rankuser_list[rank_pos])
		{
			if (m_rankuser_list[rank_pos]->IsRobot())
			{
				continue;
			}

			const CrossRankRewardCfg * rank_reward_cfg = LOGIC_CONFIG->GetChallengeFieldCfg().GetCrossRankReward(rank_pos + 1);
			if (rank_reward_cfg)
			{
				long long uuid = UniqueUserIDToLongLong(m_rankuser_list[rank_pos]->GetUniqueUserID());

				crossgameprotocal::GameCrossWorldCommonDataChange cross_msg;

				cross_msg.uuid = uuid;
				cross_msg.change_type = crossgameprotocal::WORLD_COMMONDATA_CHANGE_TYPE_CROSS_CHALLENGEFIELD;
				cross_msg.param_2 = rank_reward_cfg->img_id;
				cross_msg.param_3 = rank_reward_cfg->title_id;
				cross_msg.param_4 = rank_pos + 1;

				InternalComm::Instance().SendToCross((const char*)&cross_msg, sizeof(cross_msg));
			}

			const CrossRankOtherRewardCfg * rank_other_reward_cfg = LOGIC_CONFIG->GetChallengeFieldCfg().GetCrossRankOtherReward(rank_pos + 1);
			if (rank_other_reward_cfg)
			{
				static MailContentParam mail_param; mail_param.Reset();

				mail_param.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_CONVERTSHOP_HUNJING, rank_other_reward_cfg->reward_hunjing);

				int length = gstr::MailContent(mail_param.contenttxt, sizeof(mail_param.contenttxt), "cross_challengefield_rank_reward", rank_pos + 1 );
				if (length > 0)
				{
					MailRoute::MailToCrossUser(m_rankuser_list[rank_pos]->GetUniqueUserID(), SYSTEM_MAIL_REASON_INVALID, mail_param);
				}
			}
			
		}
	}

	this->ResetCrossChallengeUserRank();
}

void ChallengeField::CrossRandOpponentHelper(UniqueUserID user_id)
{
	if (INVALID_UNIQUE_USER_ID == user_id)
	{
		return;
	}

	const static int SECTION_RANK_POS_START_NUM = 20;

	ChallengeUser *challenge_user = this->GetChallengeUser(user_id);
	if (NULL != challenge_user)
	{
		ChallengeFieldOpponentInfo opponent_userid_list[CHALLENGE_FIELD_OPPONENT_MAX];

		int challenge_user_pos = challenge_user->GetRankPos();
		// 跨服竞技场匹配对手时，修正最大排名
		if (challenge_user_pos >= CROSS_CHALLENGE_FIELD_MAX_USER)
		{
			challenge_user_pos = CROSS_CHALLENGE_FIELD_MAX_USER;
		}

		if (challenge_user_pos < CHALLENGE_FIELD_OPPONENT_MAX)
		{
			int opponent_count = 0;
			for (int i = 0; i < m_rankuser_num && opponent_count < CHALLENGE_FIELD_OPPONENT_MAX; ++i)
			{
				if (NULL != m_rankuser_list[i] && m_rankuser_list[i]->GetUniqueUserID() != user_id)
				{
					opponent_userid_list[opponent_count].user_id = m_rankuser_list[i]->GetUniqueUserID();
					opponent_userid_list[opponent_count].rank_pos = i;
					++opponent_count;
				}

			}
		}
		else
		{
			// 先算出一个排名比玩家靠后1到5名的弱对手
// 			int weak_opponent_rank_pos = -1;
// 			{
// 				if (challenge_user->GetRankPos() < (int)m_rankpos_map.size() - 1)
// 				{
// 					const static int RANK_POS_RAND_MAX_INTERVAL = 5;
// 
// 					int rand_interval = (int)m_rankpos_map.size() - 1 - challenge_user->GetRankPos();
// 					if (rand_interval > RANK_POS_RAND_MAX_INTERVAL)
// 					{
// 						rand_interval = RANK_POS_RAND_MAX_INTERVAL;
// 					}
// 
// 					if (rand_interval > 0)
// 					{
// 						int rand_value = 1 + RandomNum(rand_interval);
// 
// 						weak_opponent_rank_pos = challenge_user->GetRankPos() + rand_value;
// 					}
// 				}
// 			}

			if (challenge_user_pos < SECTION_RANK_POS_START_NUM) // 如果玩家排名大于等于7名且小于20名
			{
				UNSTD_STATIC_CHECK(SECTION_RANK_POS_START_NUM > CHALLENGE_FIELD_OPPONENT_MAX + 1);

				int opponent_count = 0;
				for (int i = challenge_user_pos - 1; i >= 0 && opponent_count < CHALLENGE_FIELD_OPPONENT_MAX; --i)
				{
					if (NULL != m_rankuser_list[i])
					{
						opponent_userid_list[opponent_count].user_id = m_rankuser_list[i]->GetUniqueUserID();
						opponent_userid_list[opponent_count].rank_pos = i;
						++opponent_count;
					}
				}

				// 设置弱对手
// 				if (weak_opponent_rank_pos >= 0 && weak_opponent_rank_pos < m_rankuser_num)
// 				{
// 					ChallengeUser *weak_user = m_rankuser_list[weak_opponent_rank_pos];
// 					if (NULL != weak_user)
// 					{
// 						opponent_userid_list[CHALLENGE_FIELD_OPPONENT_MAX - 1].user_id = weak_user->GetUniqueUserID();
// 						opponent_userid_list[CHALLENGE_FIELD_OPPONENT_MAX - 1].rank_pos = weak_user->GetRankPos();
// 					}
// 				}
			}
			else													// 如果玩家排名大于等于20名
			{
				int opponent_count = 0;								// 已经确定的对手数量

																	// 设置弱对手
// 				bool set_weak_opponent_succ = false;
// 				{
// 					if (weak_opponent_rank_pos >= 0 && weak_opponent_rank_pos < m_rankuser_num)
// 					{
// 						ChallengeUser *weak_user = m_rankuser_list[weak_opponent_rank_pos];
// 						if (NULL != weak_user)
// 						{
// 							opponent_userid_list[0].user_id = weak_user->GetUniqueUserID();
// 							opponent_userid_list[0].rank_pos = weak_user->GetRankPos();
// 							opponent_count = 1;
// 
// 							set_weak_opponent_succ = true;
// 						}
// 					}
// 				}

				
				static int rank_pos_section_list[] = { 20, 50, 100, 200, 500, 1000, 2000 };
				int rank_pos_section_count = static_cast<int>(sizeof(rank_pos_section_list) / sizeof(rank_pos_section_list[0]));

				int suitable_section_idx = -1;
				{
					for (int i = rank_pos_section_count - 1; i >= 0; --i)
					{
						if (challenge_user_pos >= rank_pos_section_list[i])
						{
							suitable_section_idx = i;
							break;
						}
					}
					if (suitable_section_idx < 0 || suitable_section_idx >= rank_pos_section_count)
					{
						return;
					}
				}

				UNSTD_STATIC_CHECK((SECTION_RANK_POS_START_NUM - SECTION_RANK_POS_START_NUM * 0.7) >= CHALLENGE_FIELD_OPPONENT_MAX);
				UNSTD_STATIC_CHECK(CHALLENGE_FIELD_OPPONENT_MAX >= 2);

				int suitable_section_rank_pos = rank_pos_section_list[suitable_section_idx];
				int start_rank_pos = static_cast<int>(suitable_section_rank_pos * 0.7);

				int rank_pos_interval_count = CHALLENGE_FIELD_OPPONENT_MAX - 1;
				{
// 					if (!set_weak_opponent_succ)
// 					{
						rank_pos_interval_count = CHALLENGE_FIELD_OPPONENT_MAX;
//					}

					if (rank_pos_interval_count <= 0)
					{
						return;
					}
				}

				int rank_pos_interval = (challenge_user_pos - start_rank_pos) / rank_pos_interval_count;
				if (rank_pos_interval <= 0)
				{
					return;
				}

				for (int i = 0; i < rank_pos_interval_count && opponent_count < CHALLENGE_FIELD_OPPONENT_MAX; ++i)
				{
					int rand_value = RandomNum(rank_pos_interval);
					int result_rank_pos = start_rank_pos + (rank_pos_interval * i + rand_value);

					ChallengeUser *challenge_user = m_rankuser_list[result_rank_pos];
					if (NULL != challenge_user)
					{
						opponent_userid_list[opponent_count].user_id = challenge_user->GetUniqueUserID();
						opponent_userid_list[opponent_count].rank_pos = challenge_user->GetRankPos();
						++opponent_count;
					}
				}

				{
					// 如果随机没有随到强大的对手，就再计算出一个强大的对手
					bool has_powerful_opponent = false;
					{
						for (int i = 0; i < opponent_count; ++i)
						{
							if (opponent_userid_list[i].rank_pos < suitable_section_rank_pos)
							{
								has_powerful_opponent = true;
								break;
							}
						}
					}

					if (!has_powerful_opponent)
					{
						if (start_rank_pos - suitable_section_rank_pos > 0)
						{
							int rand_value = RandomNum(start_rank_pos - suitable_section_rank_pos);
							int result_pos = start_rank_pos + rand_value;

							ChallengeUser *opponent_user = m_rankuser_list[result_pos];
							if (NULL != opponent_user)
							{
								UNSTD_STATIC_CHECK(CHALLENGE_FIELD_OPPONENT_MAX >= 2);

// 								if (set_weak_opponent_succ)
// 								{
// 									opponent_userid_list[1].user_id = opponent_user->GetUniqueUserID();
// 									opponent_userid_list[1].rank_pos = opponent_user->GetRankPos();
// 								}
// 								else
 								{
									opponent_userid_list[0].user_id = opponent_user->GetUniqueUserID();
									opponent_userid_list[0].rank_pos = opponent_user->GetRankPos();
								}
							}
						}
					}
				}
			}
		}

		challenge_user->ResetOpponentUserList(opponent_userid_list);
	}
}

int ChallengeField::GetChallengeUserRankPos(Role *role)
{
	if (nullptr == role)
	{
		return -1;
	}

	ChallengeUser *challenge_user = this->GetChallengeUser(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == challenge_user)
	{
		return -1;
	}

	int rank_pos = challenge_user->GetRankPos();

	return rank_pos + 1;
}

ChallengeUser * ChallengeField::GetChallengeUserByPos(int pos)
{
	if (pos >= 0 && pos < m_rankuser_num)
	{
		if (NULL != m_rankuser_list[pos] && m_rankuser_list[pos]->GetRankPos() == pos)
		{
			return m_rankuser_list[pos];
		}
	}

	return NULL;
}

void ChallengeField::OnUserLevelChange(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	ChallengeUser *challenge_user = this->GetChallengeUser(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == challenge_user)
	{
		if (LOGIC_CONFIG->GetJoinLimitConfig().CanJoinOther(JLMT_OTHER_CHALLENGE_FIELD, role))
		{
			this->OnSyncChallengeParam(role);
		}
	}
}

void ChallengeField::OnUserLogin(Role *role)
{
	ChallengeUser *challenge_user = this->GetChallengeUser(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL != challenge_user)
	{
		challenge_user->SendChallengeReportInfo();
		challenge_user->SendChallengeUserInfo();
	}
}

void ChallengeField::OnUserLogout(Role *role)
{
	ChallengeUser * challenge_user = this->GetChallengeUser(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL != challenge_user)
	{
		challenge_user->SetLastOnlineTime();
	}
}

void ChallengeField::OnSyncChallengeParam(Role *role)
{
	if (!this->IsLoadFinish()) return;

	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (NULL == role)
	{
		return;
	}

	long long unique_uid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	int rankuser_index = -1;
	ChallengeParam challenge_param; 
	RoleAppearance appearance;
	this->ConstructChallengeParam(role,&challenge_param, appearance);
	ChallengeUser *challenge_user = this->GetChallengeUser(role->GetRoleCross()->GetCrossOriginalUniqueUserID(), &rankuser_index);

	if (NULL != challenge_user)
	{
		challenge_user->OnSyncChallengeParam(challenge_param, appearance);

		{
			// 如果角色上榜，同步榜单的角色信息
			for (int i = 0; i < CHALLENGE_FIELD_TOP_RANKER_ITEM_NUM; ++ i)
			{
				if (m_msg_rank_info.rank_list[i].user_id == unique_uid)
				{
					Protocol::SCChallengeFieldRankInfo::RankItem &rank_item = m_msg_rank_info.rank_list[i];

					rank_item.capability = challenge_param.capability;
					F_STRNCPY(rank_item.user_name, challenge_param.role_name, sizeof(GameName));
					rank_item.sex = challenge_param.sex;
				}
			}
		}
	}
	else 
	{
		if (static_cast<int>(m_rankpos_map.size()) < CHALLENGE_FIELD_MAX_USER)
		{
			int rankpos = (int)m_rankpos_map.size();
			if (rankpos >= 0 && rankpos < m_rankuser_num && NULL != m_rankuser_list[rankpos])
			{
				rankpos = this->GetFreeRankPosSlot();
				if (rankpos < 0 || rankpos >= m_rankuser_num || NULL != m_rankuser_list[rankpos])
				{
					return;
				}
			}

			challenge_user = this->CreateChallengeUser(role->GetRoleCross()->GetCrossOriginalUniqueUserID(), challenge_param, appearance);
			this->SetChallengeUser(rankpos, challenge_user);
		}
	}
}

void ChallengeField::OnChallengeResult(UniqueUserID sponsor_userid, UniqueUserID opponent_userid, bool is_sponsor_win)
{
	if (INVALID_UNIQUE_USER_ID == sponsor_userid || INVALID_UNIQUE_USER_ID == opponent_userid)
	{
		return;
	}

	int sponsor_rankpos = -1;
	ChallengeUser *sponsor_user = this->GetChallengeUser(sponsor_userid, &sponsor_rankpos);
	if (NULL == sponsor_user) 
	{
		return;
	}

	int opponent_rankpos = -1;
	ChallengeUser *opponent_user = this->GetChallengeUser(opponent_userid, &opponent_rankpos);
	if (NULL == opponent_user) 
	{
		return;
	}

	if (is_sponsor_win)
	{
		if (sponsor_rankpos > opponent_rankpos &&
			sponsor_rankpos >= 0 && sponsor_rankpos < m_rankuser_num &&
			opponent_rankpos >= 0 && opponent_rankpos < m_rankuser_num)
		{
			// 发起者占领对方的排行名次
			this->SetChallengeUser(opponent_rankpos, sponsor_user, false, true);

			if (opponent_user->IsRobot())
			{
				// 如果对方是机器人，那么需要重新将机器人恢复为发起者名次对应的属性
				const std::vector<ChallengeFieldRobotConfig> &robot_name_vector = LOGIC_CONFIG->GetChallengeFieldCfg().GetChallengeFieldRobotName();

				int robot_num = static_cast<int>(robot_name_vector.size());
				if (robot_num >= ChallengeFieldRobotConfig::CONFIG_ITEM_MAX_COUNT)
				{
					robot_num = ChallengeFieldRobotConfig::CONFIG_ITEM_MAX_COUNT;
				}

				this->GenrateChallengeUserParamInfo(opponent_user->GetParam(), opponent_user->GetAppearance(), sponsor_rankpos, robot_num);
			}
			// 对方名次下降到发起者的名次
			this->SetChallengeUser(sponsor_rankpos, opponent_user, false, true);

			if (sponsor_user->GetRankPos() < 20)
			{
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_challenge_field_rank_pos_first_changed,
					UidToInt(sponsor_user->GetUserID()), sponsor_user->GetName(), (int)sponsor_user->GetCamp(), sponsor_user->GetRankPos() + 1,
					UidToInt(opponent_user->GetUserID()), opponent_user->GetName(), (int)opponent_user->GetCamp());
				if (length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
				}
			}

			Role *opponent_role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(UniqueUserIDToLongLong(opponent_userid));
			if (NULL != opponent_role)
			{
				Protocol::SCChallengeFieldBeDefeatNotice cfbdn;
				EngineAdapter::Instance().NetSend(opponent_role->GetNetId(), (const char *)&cfbdn, sizeof(cfbdn));

				static MailContentParam contentparam; contentparam.Reset();
				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_challenge_field_rank_drop_subject);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_challenge_field_rank_drop_content, sponsor_user->GetName());
				if (length1 > 0 && length2 > 0)
				{
					MailRoute::MailToUser(opponent_userid.user_id, SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}
	}

	{
		// 加积分
		int win_add_jifen = LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg().win_add_jifen;
		int lose_add_jifen = LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg().lose_add_jifen;

		if (is_sponsor_win)
		{
			sponsor_user->AddJifen(win_add_jifen);
			sponsor_user->OnWin(true);
			opponent_user->OnFail();
		}
		else
		{
			sponsor_user->AddJifen(lose_add_jifen);
			sponsor_user->OnFail();
			opponent_user->OnWin();
		}
	}

	{
		Role *sponsor_role = World::GetInstWorld()->GetSceneManager()->GetRoleByUUID(UniqueUserIDToLongLong(sponsor_userid));
		if (NULL != sponsor_role)
		{
			//int win_add_exp = LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg().win_add_exp;
			//int lose_add_exp = LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg().lose_add_exp;

			//策划需求，修改增加经验规则（2018-1-3）
			long long win_add_exp = static_cast<long long>(LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg().win_exp_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(sponsor_role->GetLevel(), 0)));
			long long lose_add_exp = static_cast<long long>(LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg().lose_exp_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(sponsor_role->GetLevel(), 0)));

			int win_add_guanghui = LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg().win_add_guanghui;
			int lose_add_guanghui = LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg().lose_add_guanghui;

			if (is_sponsor_win)
			{
				//sponsor_role->AddExp(sponsor_role->RecalcExp(win_add_exp), "ChallengeField");
				sponsor_role->AddExp(win_add_exp, "ChallengeField", Role::EXP_ADD_REASON_DEFAULT);
				sponsor_role->GetRoleShop()->AddChestShopGuanghui(win_add_guanghui, "1V1ChangeFieldWin");
			}
			else
			{
				//sponsor_role->AddExp((sponsor_role->RecalcExp(lose_add_exp)) , "ChallengeField");
				sponsor_role->AddExp(lose_add_exp, "ChallengeField", Role::EXP_ADD_REASON_DEFAULT);
				sponsor_role->GetRoleShop()->AddChestShopGuanghui(lose_add_guanghui, "1V1ChangeFieldFail");
			}
			sponsor_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_CHALLENGE_RANK);//嘉年华
		}
	}

	{
		// 增加战报
		unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

		{
			ChallengeUserReport::ReportItem report_item;
			report_item.is_sponsor = 1;
			report_item.is_win = (is_sponsor_win ? 1 : 0);
			report_item.challenge_time = now_time;
			report_item.target_uid = UniqueUserIDToLongLong(opponent_userid);
			opponent_user->GetName(report_item.target_name);
			report_item.old_rankpos = sponsor_rankpos;
			report_item.new_rankpos = sponsor_user->GetRankPos();

			sponsor_user->AddReport(report_item);
		}

		{
			ChallengeUserReport::ReportItem report_item;
			report_item.is_sponsor = 0;
			report_item.is_win = (is_sponsor_win ? 0 : 1);
			report_item.challenge_time = now_time;
			report_item.target_uid = UniqueUserIDToLongLong(sponsor_userid);
			sponsor_user->GetName(report_item.target_name);
			report_item.old_rankpos = opponent_rankpos;
			report_item.new_rankpos = opponent_user->GetRankPos();

			opponent_user->AddReport(report_item);
		}
	}
}

void ChallengeField::OnGetChallengeUserInfo(Role *role)
{
	ChallengeUser *challenge_user = this->GetChallengeUser(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL != challenge_user)
	{
		challenge_user->SendChallengeUserInfo();
		challenge_user->SendChallengeReportInfo();
	}
}

void ChallengeField::OnResetOpponentReq(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (!this->IsChallengeOpen())
	{
		//role->NoticeNum(errornum::EN_CHALLENGE_FIELD_IS_CLOSED);
		return;
	}

	this->RandOpponentHelper(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	this->OnGetOpponentDetailInfo(role, false);
}

void ChallengeField::OnFightReq(Role *role, int opponent_idx, bool ignore_rank_pos, int rank_pos, bool buy_join_times)
{
	if (NULL == role)
	{
		return;
	}

	if (!this->IsChallengeOpen())
	{
		role->NoticeNum(errornum::EN_CHALLENGE_FIELD_IS_CLOSED);
		return;
	}

	UniqueUserID unique_user_id = role->GetRoleCross()->GetCrossOriginalUniqueUserID();
	ChallengeUser *challenge_user = this->GetChallengeUser(unique_user_id);
	if (NULL == challenge_user)
	{
		return;
	}

	if (!challenge_user->SetOpponentUser(opponent_idx))
	{
		role->NoticeNum(errornum::EN_CHALLENGE_FIELD_OPPONENT_NOT_EXIST);
		return;
	}

	UniqueUserID opponent_userid = challenge_user->GetCurrOpponentUserID();
	ChallengeUser *opponent_user = this->GetChallengeUser(opponent_userid);

	if (NULL == opponent_user)
	{
		role->NoticeNum(errornum::EN_CHALLENGE_FIELD_OPPONENT_NOT_EXIST);
		return;
	}

	if (!ignore_rank_pos)
	{
		if (rank_pos != opponent_user->GetRankPos())
		{
			Protocol::SCChallengeFieldOpponentRankPosChange cforpc;
			cforpc.user_id = UniqueUserIDToLongLong(opponent_userid);
			cforpc.rank_pos = opponent_user->GetRankPos();
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cforpc, sizeof(cforpc));

			return;
		}
	}

	if (!role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}

	//if (role->GetGreateSoldier()->IsBianshenStatus())
	//{
	//	role->NoticeNum(errornum::EN_BIANSHEN_LIMIT);
	//	return;
	//}

	const ChallengeFieldOtherConfig &other_cfg = LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg();
	int free_jion_times = LOGIC_CONFIG->GetChallengeFieldCfg().GetRoleFreeJoinTimesByCreateTime(role->GetCreateRoleTimestamp());

	if (challenge_user->GetJoinTimes() >= free_jion_times + challenge_user->GetBuyJoinTimes())
	{
		if (!buy_join_times)
		{
			role->NoticeNum(errornum::EN_CHALLENGE_FIELD_JOIN_TIMES_LIMIT);
			return;
		}
		else
		{
			if (role->GetSublock()->CheckHasLock())
			{
				return;
			}

			if (role->GetKnapsack()->IsLock())
			{
				role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
				return;
			}

			int vip_buytimes = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_FB_1V1_BUY_TIMES);
			if (challenge_user->GetBuyJoinTimes() >= vip_buytimes)
			{
				role->NoticeNum(errornum::EN_1V1_DAILYTIMES_BUY_TIMES_MAX);
				return;
			}

			long long use_gold = other_cfg.buy_join_times_cost, use_gold_bind = 0;
			if (other_cfg.buy_join_times_cost > 0 && !role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.buy_join_times_cost, "BuyChallengeFieldJoinTimes"))
			{
				role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}

			challenge_user->AddBuyJoinTimes(1);

			gamelog::g_log_challengefield.printf(LL_INFO, "BuyJoinTimes:: user[%d %s], use_gold[%lld], use_gold_bind[%lld], buy_join_times[%d]", 
				role->GetUID(), role->GetName(), use_gold, use_gold_bind, challenge_user->GetBuyJoinTimes());
		}
	}

	int target_scene_id = other_cfg.scene_id;
	Posi target_scene_pos = other_cfg.sponsor_pos;
	int scene_key = role->GetUID();

	if (challenge_user->GetRankPos() < opponent_user->GetRankPos() && role->GetCapability()->GetTotalCapability() >= opponent_user->GetCapability())
	{
		Protocol::SCChallengeFieldWin pro;
		pro.old_rank_pos = challenge_user->GetRankPos();

		challenge_user->OnJoinChallengeField();
		this->OnChallengeResult(role->GetRoleCross()->GetCrossOriginalUniqueUserID(), opponent_user->GetUniqueUserID(), true);

		pro.new_rank_pos = challenge_user->GetRankPos();
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, sizeof(pro));

		EventHandler::Instance().OnEnterChallengeField(role);
	}
	else
	{
		bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role, target_scene_id, scene_key, target_scene_pos);
		if (!ret)
		{
			role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
		}
		else
		{
			challenge_user->OnJoinChallengeField();
		}
	}
}

void ChallengeField::OnBuyJoinTimes(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (!this->IsChallengeOpen())
	{
		role->NoticeNum(errornum::EN_CHALLENGE_FIELD_IS_CLOSED);
		return;
	}

	ChallengeUser *challenge_user = this->GetChallengeUser(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == challenge_user)
	{
		return;
	}

	if (role->GetSublock()->CheckHasLock())
	{
		return;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}
	
	int vip_buytimes = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_FB_1V1_BUY_TIMES);
	if (challenge_user->GetBuyJoinTimes() >= vip_buytimes)
	{
		role->NoticeNum(errornum::EN_1V1_DAILYTIMES_BUY_TIMES_MAX);
		return;
	}

	const ChallengeFieldOtherConfig &other_cfg = LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg();
	long long use_gold = other_cfg.buy_join_times_cost, use_gold_bind = 0;
	if (other_cfg.buy_join_times_cost > 0 && !role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.buy_join_times_cost, "BuyChallengeFieldJoinTimes"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	challenge_user->AddBuyJoinTimes(1);

	gamelog::g_log_challengefield.printf(LL_INFO, "BuyJoinTimes:: user[%d %s], use_gold[%lld], use_gold_bind[%lld], buy_join_times[%d]", 
		role->GetUID(), role->GetName(), use_gold, use_gold_bind, challenge_user->GetBuyJoinTimes());
}

void ChallengeField::OnFetchJifenReq(Role *role, int seq)
{
	ChallengeUser *challenge_user = this->GetChallengeUser(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == challenge_user)
	{
		return;
	}

	challenge_user->OnFetchJifenReward(seq);
}

void ChallengeField::OnGetRankInfo(Role *role)
{
	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&m_msg_rank_info, sizeof(m_msg_rank_info));
	}
}

void ChallengeField::OnGetLuckyInfo(Role *role)
{
// 	if (NULL != role)
// 	{
// 		static Protocol::SCChallengeFieldLuckyPosInfo m_msg_lucky_pos_info;
// 
// 		for (int i = 0; i < CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT; ++ i)
// 		{
// 			m_msg_lucky_pos_info.lucky_rankpos_list[i] = m_challenge_field_data.luck_rank_pos_list[i];
// 
// 			m_msg_lucky_pos_info.lucky_info_list[i].rank_pos = m_challenge_field_data.lucky_user_list[i].rank_pos;
// 			m_msg_lucky_pos_info.lucky_info_list[i].user_id = m_challenge_field_data.lucky_user_list[i].user_id;
// 			F_STRNCPY(m_msg_lucky_pos_info.lucky_info_list[i].user_name, 
// 				m_challenge_field_data.lucky_user_list[i].user_name, sizeof(m_msg_lucky_pos_info.lucky_info_list[i].user_name));
// 		}
// 
// 		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&m_msg_lucky_pos_info, sizeof(m_msg_lucky_pos_info));
// 	}
}

void ChallengeField::OnFetchGuangHui(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	ChallengeUser *challenge_user = this->GetChallengeUser(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == challenge_user)
	{
		return;
	}

	if (0 == challenge_user->GetRewardDay())
	{
		role->NoticeNum(errornum::EN_CHALLENGE_FIELD_FETCH_EMPTY);
		return;
	}

	int count = 0;
	const ItemConfigData *reward_item_list = challenge_user->GetRewardItem(&count);
	if (NULL == reward_item_list)
	{
		return;
	}

	if (count > 0)
	{
		if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(count))
		{
			role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}

		role->GetKnapsack()->PutList(count, reward_item_list, PUT_REASON_CHALLENGE_FIELD);
		challenge_user->ClearRewardItem();
	}

	int own_guanghui = challenge_user->GetGuangHui();
	if (own_guanghui > 0)
	{
		challenge_user->AddGuangHui(-own_guanghui, "FetchGuangHui");
		role->GetRoleShop()->AddChestShopGuanghui(own_guanghui, "ChallengeFieldGuangHui");
	}

	challenge_user->AddRewradDay(-challenge_user->GetRewardDay());
	challenge_user->SendChallengeUserInfo();
}

void ChallengeField::OnGetOpponentDetailInfo(Role *role, bool is_get_top)
{
	if (NULL == role)
	{
		return;
	}

	ChallengeUser *challenge_user = this->GetChallengeUser(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == challenge_user)
	{
		return;
	}

	if (!is_get_top)
	{
		challenge_user->OnGetOpponentDetailInfo();
	}
	else
	{
		Protocol::SCChallengeFieldOpponentInfo cfoi;
		cfoi.opponent_count = 0;

		int TOP_USER_NUM = 5;
		for (int i = 0; i < TOP_USER_NUM && cfoi.opponent_count < CHALLENGE_FIELD_OPPONENT_MAX; ++ i)
		{
			ChallengeUser *top_user = this->GetChallengeUserByPos(i);
			if (NULL == top_user)
			{
				continue;
			}

			Protocol::SCChallengeFieldOpponentInfo::OpponentInfo &opponent_info = cfoi.opponent_info_list[cfoi.opponent_count];
			const ChallengeParam &opponent_param = top_user->GetChallengeParam();
			const RoleAppearance &opponent_appearance = top_user->GetChallengeAppearance();

			opponent_info.user_id = UniqueUserIDToLongLong(top_user->GetUniqueUserID());
			top_user->GetName(opponent_info.name);
			opponent_info.prof = opponent_param.prof;
			opponent_info.sex = opponent_param.sex;
			opponent_info.camp = opponent_param.camp;
			opponent_info.capability = opponent_param.capability;

			opponent_info.appearance = opponent_appearance;

			if (!top_user->IsRobot())
			{
				Role *opponent_role = World::GetInstWorld()->GetSceneManager()->GetRole(top_user->GetUserID());
				if (NULL != opponent_role)
				{
					opponent_info.best_rank_break_level = opponent_role->GetCommonDataParam()->challenge_field_best_rank_break_level;
				}
				else
				{
					opponent_info.best_rank_break_level = 0;
				}
			}
			else
			{
				opponent_info.best_rank_break_level = i;		//机器人特殊处理
			}

			++ cfoi.opponent_count;
		}

		int send_len = sizeof(cfoi) - (CHALLENGE_FIELD_OPPONENT_MAX - cfoi.opponent_count) * sizeof(cfoi.opponent_info_list[0]);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cfoi, send_len);
	}
}

void ChallengeField::OnChallengeFieldBuyBuff(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (!this->IsChallengeOpen())
	{
		role->NoticeNum(errornum::EN_CHALLENGE_FIELD_IS_CLOSED);
		return;
	}

	ChallengeUser *challenge_user = this->GetChallengeUser(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL == challenge_user)
	{
		return;
	}

	if (role->GetSublock()->CheckHasLock())
	{
		return;
	}

	if (role->GetKnapsack()->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	const ChallengeFieldOtherConfig &other_cfg = LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg();
	if (challenge_user->GetBuyBuffTimes() >= other_cfg.buy_buff_times_max)
	{
		role->NoticeNum(errornum::EN_CHALLENGE_FIELD_BUFF_TIMES_LIMIT);
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseGold(other_cfg.buy_buff_gold, "ChallengeFieldBuyBuff"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	challenge_user->AddBuyBuffTimes(1);

	gamelog::g_log_challengefield.printf(LL_INFO, "BuyBuff:: user[%d %s], use_gold[%d], buy_buff_times[%d]",
		role->GetUID(), role->GetName(), other_cfg.buy_buff_gold, challenge_user->GetBuyBuffTimes());
}

void ChallengeField::ConstructChallengeParam(Role *role, ChallengeParam * param, RoleAppearance &appearance)
{
	
	if (NULL != role)
	{
		param->plat_type = role->GetRoleCross()->GetCrossOriginalUniqueUserID().plat_type;
		param->role_id = role->GetUID();
		role->GetName(param->role_name);
		param->avatar = role->GetAvatar();
		param->sex = role->GetSex();
		param->prof = role->GetProf();
		param->camp = role->GetCamp();
		param->level = role->GetLevel();
		param->capability = role->GetCapability()->GetTotalCapability();

		param->max_hp = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP);
		param->gongji = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI);
		param->fangyu = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU);
		param->mingzhong = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG);
		param->shanbi = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI);
		param->baoji = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI);
		param->jianren = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN);
		param->move_speed = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED);
		param->fujiashanghai = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI);
		param->dikangshanghai = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI);
			 
		param->jingzhun_per = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN);
		param->baoji_per = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT);
		param->kangbao_per = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO);
		param->pofang_per = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG);
		param->mianshang_per = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG);
		param->constant_zengshang = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG);
		param->constant_mianshang = role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG);
			 
		param->vip_level = static_cast<char>(role->GetVip()->GetVipLevel());
			 
		param->guild_post = role->GetGuildPost();
		param->guild_id = role->GetGuildID();
		role->GetGuildName(param->guild_name);

		appearance = role->GetAppearance();
		param->avatar_timestamp = role->GetAvatarTimeStamp();

		{
			// 技能信息
			SkillListParam skill_list_param;
			role->GetSkillManager()->GetSkillList(&skill_list_param);

			for (int i = 0; i < skill_list_param.count && i < SkillListParam::MAX_SKILL_NUM; ++ i)
			{
				param->skill_list[i].skill_id = skill_list_param.skill_list[i].skill_id;
				param->skill_list[i].level = skill_list_param.skill_list[i].level;
			}
		}
	}

}

void ChallengeField::RandOpponentHelper(UniqueUserID user_id)
{
	if (INVALID_UNIQUE_USER_ID == user_id)
	{
		return;
	}

	const static int SECTION_RANK_POS_START_NUM = 20;

	ChallengeUser *challenge_user = this->GetChallengeUser(user_id);
	if (NULL != challenge_user)
	{
		ChallengeFieldOpponentInfo opponent_userid_list[CHALLENGE_FIELD_OPPONENT_MAX];

		int challenge_user_pos = challenge_user->GetRankPos();

		if (challenge_user_pos < CHALLENGE_FIELD_OPPONENT_MAX)
		{
			int opponent_count = 0;
			for (int i = 0; i < m_rankuser_num && opponent_count < CHALLENGE_FIELD_OPPONENT_MAX; ++ i)
			{
				if (NULL != m_rankuser_list[i] && m_rankuser_list[i]->GetUniqueUserID() != user_id)
				{
					opponent_userid_list[opponent_count].user_id = m_rankuser_list[i]->GetUniqueUserID();
					opponent_userid_list[opponent_count].rank_pos = i;
					++ opponent_count;
				}
				
			}
		}
		else
		{
			// 先算出一个排名比玩家靠后1到5名的弱对手
			int weak_opponent_rank_pos = -1;
			{
				if (challenge_user->GetRankPos() < (int)m_rankpos_map.size() - 1)
				{
					const static int RANK_POS_RAND_MAX_INTERVAL = 5;

					int rand_interval = (int)m_rankpos_map.size() - 1 - challenge_user->GetRankPos();
					if (rand_interval > RANK_POS_RAND_MAX_INTERVAL)
					{
						rand_interval = RANK_POS_RAND_MAX_INTERVAL;
					}

					if (rand_interval > 0)
					{
						int rand_value = 1 + RandomNum(rand_interval);

						weak_opponent_rank_pos = challenge_user->GetRankPos() + rand_value;
					}
				}
			}

			if (challenge_user_pos < SECTION_RANK_POS_START_NUM) // 如果玩家排名大于等于7名且小于20名
			{
				UNSTD_STATIC_CHECK(SECTION_RANK_POS_START_NUM > CHALLENGE_FIELD_OPPONENT_MAX + 1);

				int opponent_count = 0;
				for (int i = challenge_user_pos - 1; i >= 0 && opponent_count < CHALLENGE_FIELD_OPPONENT_MAX; -- i)
				{
					if (NULL != m_rankuser_list[i])
					{
						opponent_userid_list[opponent_count].user_id = m_rankuser_list[i]->GetUniqueUserID();
						opponent_userid_list[opponent_count].rank_pos = i;
						++ opponent_count;
					}
				}

				// 设置弱对手
				if (weak_opponent_rank_pos >= 0 && weak_opponent_rank_pos < m_rankuser_num)
				{
					ChallengeUser *weak_user = m_rankuser_list[weak_opponent_rank_pos]; 
					if (NULL != weak_user)
					{
						opponent_userid_list[CHALLENGE_FIELD_OPPONENT_MAX - 1].user_id = weak_user->GetUniqueUserID();
						opponent_userid_list[CHALLENGE_FIELD_OPPONENT_MAX - 1].rank_pos = weak_user->GetRankPos();
					}
				}
			}
			else													// 如果玩家排名大于等于20名
			{
				int opponent_count = 0;								// 已经确定的对手数量

				// 设置弱对手
				bool set_weak_opponent_succ = false;
				{
					if (weak_opponent_rank_pos >= 0 && weak_opponent_rank_pos < m_rankuser_num)
					{
						ChallengeUser *weak_user = m_rankuser_list[weak_opponent_rank_pos]; 
						if (NULL != weak_user)
						{
							opponent_userid_list[0].user_id = weak_user->GetUniqueUserID();
							opponent_userid_list[0].rank_pos = weak_user->GetRankPos();
							opponent_count = 1;

							set_weak_opponent_succ = true;
						}
					}
				}

				// 2019.02.28 文均需求
				static int rank_pos_section_list[] = {20, 50, 100, 200, 500, 1000, 2000};
				int rank_pos_section_count = static_cast<int>(sizeof(rank_pos_section_list) / sizeof(rank_pos_section_list[0]));

				int suitable_section_idx = -1;
				{
					for (int i = rank_pos_section_count - 1; i >= 0; -- i)
					{
						if (challenge_user_pos >= rank_pos_section_list[i])
						{
							suitable_section_idx = i;
							break;
						}
					}
					if (suitable_section_idx < 0 || suitable_section_idx >= rank_pos_section_count)
					{
						return;
					}
				}

				UNSTD_STATIC_CHECK((SECTION_RANK_POS_START_NUM - SECTION_RANK_POS_START_NUM * 0.7) >= CHALLENGE_FIELD_OPPONENT_MAX);
				UNSTD_STATIC_CHECK(CHALLENGE_FIELD_OPPONENT_MAX >= 2);

				int suitable_section_rank_pos = rank_pos_section_list[suitable_section_idx];	
				int start_rank_pos = static_cast<int>(suitable_section_rank_pos * 0.7);	

				int rank_pos_interval_count = CHALLENGE_FIELD_OPPONENT_MAX - 1;
				{
					if (!set_weak_opponent_succ)
					{
						rank_pos_interval_count = CHALLENGE_FIELD_OPPONENT_MAX;
					}

					if (rank_pos_interval_count <= 0)
					{
						return;
					}
				}

				int rank_pos_interval = (challenge_user_pos - start_rank_pos) / rank_pos_interval_count; 
				if (rank_pos_interval <= 0)
				{
					return;
				}

				for (int i = 0; i < rank_pos_interval_count && opponent_count < CHALLENGE_FIELD_OPPONENT_MAX; ++ i)
				{
					int rand_value = RandomNum(rank_pos_interval);
					int result_rank_pos = start_rank_pos + (rank_pos_interval * i + rand_value);

					ChallengeUser *challenge_user = m_rankuser_list[result_rank_pos];
					if (NULL != challenge_user)
					{
						opponent_userid_list[opponent_count].user_id = challenge_user->GetUniqueUserID();
						opponent_userid_list[opponent_count].rank_pos = challenge_user->GetRankPos();
						++ opponent_count;
					}
				}

				{
					// 如果随机没有随到强大的对手，就再计算出一个强大的对手
					bool has_powerful_opponent = false;
					{
						for (int i = 0; i < opponent_count; ++ i)
						{
							if (opponent_userid_list[i].rank_pos < suitable_section_rank_pos)
							{
								has_powerful_opponent = true;
								break;
							}
						}
					}

					if (!has_powerful_opponent)
					{
						if (start_rank_pos - suitable_section_rank_pos > 0)
						{
							int rand_value = RandomNum(start_rank_pos - suitable_section_rank_pos);
							int result_pos = start_rank_pos + rand_value;

							ChallengeUser *opponent_user = m_rankuser_list[result_pos]; 
							if (NULL != opponent_user)
							{
								UNSTD_STATIC_CHECK(CHALLENGE_FIELD_OPPONENT_MAX >= 2);

								if (set_weak_opponent_succ)
								{
									opponent_userid_list[1].user_id = opponent_user->GetUniqueUserID();
									opponent_userid_list[1].rank_pos = opponent_user->GetRankPos();
								}
								else
								{
									opponent_userid_list[0].user_id = opponent_user->GetUniqueUserID();
									opponent_userid_list[0].rank_pos = opponent_user->GetRankPos();
								}
							}
						}
					}
				}
			}
		}

		challenge_user->ResetOpponentUserList(opponent_userid_list);
	}
}

void ChallengeField::OnRankPosChange(UniqueUserID user_id, int rank_pos)
{
	if (0 == rank_pos)
	{
		WorldStatus::Instance().SetChallengeFieldKingUserId(UniqueUserIDToLongLong(user_id)); // 设置最强挑战者
	}

	if (rank_pos >= 0 && rank_pos < CHALLENGE_FIELD_TOP_RANKER_ITEM_NUM)
	{
		ChallengeUser *challenge_user = this->GetChallengeUser(user_id);
		if (NULL != challenge_user)
		{
			Protocol::SCChallengeFieldRankInfo::RankItem &rank_item = m_msg_rank_info.rank_list[rank_pos];
			rank_item.user_id = UniqueUserIDToLongLong(user_id);
			rank_item.capability = challenge_user->GetCapability();
			challenge_user->GetName(rank_item.user_name);
			rank_item.sex = challenge_user->GetSex();
			rank_item.is_robot = challenge_user->IsRobot() ? 1 : 0;
			rank_item.prof = challenge_user->GetProf();
			rank_item.role_level = challenge_user->GetLevel();
			rank_item.appearance = challenge_user->GetChallengeAppearance();
		}
	}
}

void ChallengeField::CheckGiveLuckyReward(time_t now_second)
{
// 	if (now_second < m_last_check_give_lucky_reward_timestamp + 2)
// 	{
// 		return;
// 	}
// 
// 	m_last_check_give_lucky_reward_timestamp = now_second;
// 
// 	if (0 == m_challenge_field_data.is_luckyreward_gived && static_cast<unsigned int>(now_second) >= m_challenge_field_data.next_give_luckyreward_timestamp)
// 	{
// 		m_challenge_field_data.is_luckyreward_gived = 1;
// 
// 		// 幸运奖励
// 		{
// 			int reward_factor = 1;
// 			if (ActivityShadow::Instance().IsSpecialActivityOpen(SPECIAL_ACTIVITY_TYPE_CLOSEDBETA)) // 封测期间奖励翻倍
// 			{
// 				reward_factor *= CLOSE_BETA_CHALLENGE_FIELD_REWARD_TIMES;
// 			}
// 
// 			if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DOUFA_KUANGHUAN))
// 			{
// 				reward_factor *= RAND_ACTIVITY_DOUFA_KUANGHUAN_REWARD_TIMES;
// 			}
// 
// 			for (int i = 0; i < CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT; ++ i)
// 			{
// 				ChallengeFieldDataParam::LuckyUserItem &lucky_user_item = m_challenge_field_data.lucky_user_list[i];
// 				lucky_user_item.Reset();
// 
// 				int rank_pos = m_challenge_field_data.luck_rank_pos_list[i];
// 
// 				lucky_user_item.rank_pos = rank_pos;
// 
// 				if (rank_pos < 0 && rank_pos >= m_rankuser_num)
// 				{
// 					continue;
// 				}
// 
// 				ChallengeUser *lucky_user = m_rankuser_list[rank_pos];
// 				if (NULL == lucky_user)
// 				{
// 					continue;
// 				}
// 
// 				if (lucky_user->GetRankPos() != rank_pos)
// 				{
// 					continue;
// 				}
// 
// 				const ChallengeFieldLuckRewardConfig::ConfigItem *reward_cfg = LOGIC_CONFIG->GetChallengeFieldCfg().GetLuckyRewardCfg(i);
// 				if (NULL == reward_cfg)
// 				{
// 					continue;
// 				}
// 
// 				// 发邮件
// 				static MailContentParam contentparam; contentparam.Reset();
// 				int reward_shengwang = reward_cfg->reward_shengwang * reward_factor;
// 
// 				contentparam.virtual_item_list.virtual_item[MAIL_VIRTUAL_ITEM_SHENGWANG] = reward_shengwang;
// 
// 				int l1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_challenge_field_lucky_reward_subject);
// 				int l2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_challenge_field_lucky_reward_content);
// 
// 				if (l1 > 0 && l2 > 0)
// 				{
// 					MailRoute::MailToUser(lucky_user->GetUserID(), SYSTEM_MAIL_REASON_INVALID,contentparam);
// 
// 					gamelog::g_log_challengefield.printf(LL_INFO, "CheckGiveLuckyReward::index[%d], user[%d, %s] pos[%d] add_shengwang[%d]", 
// 						i, UidToInt(lucky_user->GetUserID()), lucky_user->GetName(), rank_pos, reward_shengwang);
// 				}
// 
// 				{
// 					lucky_user_item.rank_pos = rank_pos;
// 					lucky_user_item.user_id = UidToInt(lucky_user->GetUserID());
// 					lucky_user->GetName(lucky_user_item.user_name);
// 				}
// 			}
// 		}
// 	}
// 
// 	if (0 != m_challenge_field_data.is_luckyreward_gived)
// 	{
// 		int next_give_rank_reward_interval = EngineAdapter::Instance().NextDayInterval(CHALLENGE_FIELD_GIVE_RANK_REWARD_HOUR, 0, 0);
// 		if (next_give_rank_reward_interval < 12 * 60 * 60) // 容错，这样允许把发奖时间往后调整（12小时以内）,且不会一天发2次奖励
// 		{
// 			next_give_rank_reward_interval += 24 * 60 * 60;
// 		}
// 
// 		m_challenge_field_data.next_give_luckyreward_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time() + next_give_rank_reward_interval);
// 		m_challenge_field_data.is_luckyreward_gived = 0;
// 		this->GenrateLuckyRankPos();
// 	}
}

void ChallengeField::CheckGiveRankReward(time_t now_second)
{
	if (now_second < m_last_check_give_rank_reward_timestamp + 2)
	{
		return;
	}
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	m_last_check_give_rank_reward_timestamp = now_second;

	// 当前开服天数
	int cur_opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
	// 结算第几天的奖励
	const int reward_day = m_challenge_field_data.reward_opengame_day;

	if (0 == m_challenge_field_data.is_rankreward_gived && 
		static_cast<unsigned int>(now_second) >= m_challenge_field_data.next_give_rankreward_timestamp)
	{
		m_challenge_field_data.is_rankreward_gived = 1;

		// 排行奖励
		{
			int rank_reward_cfgcount = LOGIC_CONFIG->GetChallengeFieldCfg().GetRankRewardCfgCount(reward_day);

			// 结算清0
			memset(m_challenge_field_data.reward_rank_uid_list, 0, sizeof(m_challenge_field_data.reward_rank_uid_list));
			for (int i = 0; i < rank_reward_cfgcount; ++ i)
			{
				const ChallengeFieldRankRewardConfig::ConfigItem *reward_cfg = LOGIC_CONFIG->GetChallengeFieldCfg().GetRankRewardCfg(reward_day, i);
				if (NULL == reward_cfg)
				{
					continue;
				}

				for (int rank_pos = reward_cfg->min_rank_pos; rank_pos <= reward_cfg->max_rank_pos && 
					rank_pos < (int)m_rankpos_map.size() && rank_pos < m_rankuser_num; ++ rank_pos)
				{
					ChallengeUser *challenge_user = m_rankuser_list[rank_pos];
					if (NULL == challenge_user || challenge_user->IsRobot())
					{
						continue;
					}

					if (challenge_user->GetRankPos() != rank_pos)
					{
						continue;
					}

					/************************************************************************/
					/* modify 2019-03-10
					 * 竞技场开放5天
					 * 奖励称号和羽翼通过邮件发放，不再单独存库 
					 * 开服1-4天只结算魂玉，第5天结算时奖励称号和羽翼。去掉了奖励累计次数
					*/
					/************************************************************************/

					//{
					//	// 在发放奖励之前 因为有个规则 三个奖励没领取 就会 continue
//						UserID rank_user_id = challenge_user->GetUserID();
					
					//	// 记录结算排名玩家uid，发送称号和羽翼 避免重复发放
//						if ((CHALLENGE_FIELD_RECORD_RANK_POS_MAX_COUNT > rank_pos) && (0 <= rank_pos) && 
//							(0 == m_challenge_field_data.reward_rank_uid_list[rank_pos]))
//						{
//							m_challenge_field_data.reward_rank_uid_list[rank_pos] = UidToInt(rank_user_id);
					
//							Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(rank_user_id);
//							if (NULL != role)
//							{
					//			role->GetTitleManager()->OnChallengeFieldRankInfo(rank_pos, SYNC_REASON_RANK);
					//			role->GetWing()->OnChallengeFieldRankInfo(rank_pos);
//							}
//						}
					//
					//}
					//
					//if (challenge_user->GetRewardDay() >= LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg().rank_reward_count)
					//{
					//	continue;
					//}

					// 奖励魂玉
					challenge_user->AddRewradDay(1);

					if (reward_cfg->reward_guanghui > 0)
					{
						int reward_guanghui = static_cast<int>(reward_cfg->reward_guanghui * 1.0 * m_challenge_field_data.rankreward_percent / 100);
						challenge_user->AddGuangHui(reward_guanghui, "RankReward");
					}

					//for (int k = 0; k < MAX_ATTACHMENT_ITEM_NUM; k++)
					//{
					//	challenge_user->AddRewardItem(reward_cfg->reward_item_list[k].item_id, reward_cfg->reward_item_list[k].num, reward_cfg->reward_item_list[k].is_bind);
					//}

					// 奖励物品，称号，羽翼，邮件发送
					std::string item_list_str;
					ItemConfigDataListLog(item_list_str, reward_cfg->reward_count, reward_cfg->reward_item_list);
					if (reward_cfg->reward_count > 0)
					{
						static MailContentParam contentparam; contentparam.Reset();
						for (int item_count = 0; item_count < reward_cfg->reward_count && item_count < MAX_ATTACHMENT_ITEM_NUM; item_count++)
						{
							contentparam.AddItem(reward_cfg->reward_item_list[item_count]);
						}

						int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_challenge_field_rank_reward_subject);
						int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_challenge_field_rank_close_reward_content, rank_pos + 1);
						if (length1 > 0 && length2 > 0)
						{
							MailRoute::MailToUser(challenge_user->GetUserID(), SYSTEM_MAIL_REASON_INVALID, contentparam);
						}
					}

					// 下发给客户端展示
					{
						UserID rank_user_id = challenge_user->GetUserID();
						Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(rank_user_id);
						if (NULL != role)
						{
							EventHandler::Instance().OnGetSceneActivityReward(role, Protocol::SCSceneActivityRewardInfo::REWARD_SOURCE_ID_CHALLENGEFIELD, reward_cfg->title_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_TITLE, rank_pos + 1);
							EventHandler::Instance().OnGetSceneActivityReward(role, Protocol::SCSceneActivityRewardInfo::REWARD_SOURCE_ID_CHALLENGEFIELD, reward_cfg->wing_id, Protocol::SCSceneActivityRewardInfo::REWARD_TYPE_WING, rank_pos + 1);
						}
					}

					gamelog::g_log_challengefield.printf(LL_INFO, "CheckGiveRankReward::user[%d,%s], reward_day[%d], rankpos[%d], reward_guanghui[%d], reward_item[%s]",
						UidToInt(challenge_user->GetUserID()), challenge_user->GetName(), reward_day, rank_pos, reward_cfg->reward_guanghui, item_list_str.c_str());
				}
			}
		}

		{
			// 发传闻
			if (!m_rankpos_map.empty())
			{
				const int max_count = 3;  // 播报前三名
				int rank_count = 0;
				int all_name_len[max_count] = {0, 0, 0};
				char all_name_str[max_count][512] = {0, 0, 0};

				const int CHALLENGE_FIELD_OPEN_DUR_DAY = LOGIC_CONFIG->GetChallengeFieldCfg().GetOtherCfg().open_dur_day;
				bool is_close_reward = (CHALLENGE_FIELD_OPEN_DUR_DAY == reward_day);  // 是否结算最后一天的奖励

				for (int i = 0; i < (int)m_rankpos_map.size() && rank_count < max_count; ++ i)
				{
					ChallengeUser *challenge_user = m_rankuser_list[i];
					if (NULL == challenge_user)
					{
						continue;
					}

					if (challenge_user->IsRobot())
					{
						continue;
					}

					char member_name_str[64] = {0};

					int namelen = SNPRINTF(member_name_str, sizeof(member_name_str), "{r;%d;%s;%d} ", UidToInt(challenge_user->GetUserID()), challenge_user->GetName(), (int)challenge_user->GetCamp());
					if (namelen > 0 && all_name_len[rank_count] + namelen < (int)sizeof(all_name_str[rank_count]))
					{
						STRNCPY(all_name_str[rank_count] + all_name_len[rank_count], member_name_str, sizeof(all_name_str[rank_count]) - all_name_len[rank_count]);
						all_name_len[rank_count] += namelen;
					}

					++ rank_count;

					if (is_close_reward)  // 结束奖励称号和羽翼传闻
					{
						int name_index = rank_count - 1;
						int len = 0;
						if (name_index >= 0 || name_index < max_count)
						{
							switch (i)
							{
							case 0:
							{
								len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_challenge_field_close_rank_pos_notice1, all_name_str[name_index]);
							}
							break;
							case 1:
							{
								len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_challenge_field_close_rank_pos_notice2, all_name_str[name_index]);
							}
							break;
							case 2:
							{
								len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_challenge_field_close_rank_pos_notice3, all_name_str[name_index]);
							}
							break;
							}

							if (len > 0)
							{
								World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL);
							}
						}
					}
				}
				
				if (!is_close_reward && rank_count > 0)  // 每日结算魂玉传闻
				{
					int len = 0;
					switch (rank_count)
					{
					case 3:
					{
						len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_challenge_field_rank_pos_notice1, all_name_str[0], all_name_str[1], all_name_str[2]);
					}
					break;
					case 2:
					{
						len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_challenge_field_rank_pos_notice2, all_name_str[0], all_name_str[1]);
					}
					break;
					case 1:
					{
						len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_challenge_field_rank_pos_notice3, all_name_str[0]);
					}
					break;
					}

					if (len > 0)
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_ROLL);
					}
				}
			}
		}
	}

	if (0 != m_challenge_field_data.is_rankreward_gived)
	{
		if (this->IsChallengeOpen())
		{
			// 计算下一次发奖时间
			m_challenge_field_data.is_rankreward_gived = 0;

			int reward_percent = 0;
			m_challenge_field_data.next_give_rankreward_timestamp = LOGIC_CONFIG->GetChallengeFieldCfg().CalNextRankRewadTime(&reward_percent);
			m_challenge_field_data.rankreward_percent = reward_percent;

			// 下一次奖励结算的开服天数
			time_t next_reward_time = static_cast<time_t>(m_challenge_field_data.next_give_rankreward_timestamp);
			m_challenge_field_data.reward_opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), next_reward_time) + 1;

			// log
			gamelog::g_log_challengefield.printf(LL_INFO, "CheckGiveRankReward Calc next reward time. cur_day[%d], next_reward_day[%d], next_time[%u]",
				cur_opengame_day, m_challenge_field_data.reward_opengame_day, m_challenge_field_data.next_give_rankreward_timestamp);
		}
		else
		{
			m_challenge_field_data.reward_opengame_day = 0;
		}
	}
}

void ChallengeField::GenrateLuckyRankPos()
{
	{
		// 产生幸运排名, rank_pos_section数组的内容表示排名分段，这些排名必须是递增的
		UNSTD_STATIC_CHECK(10 == CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT);

		static int rank_pos_section[CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT + 1] = {10, 20, 30, 50, 70, 100, 150, 200, 250, 300, 400};

		for (int i = 0; i < CHALLENGE_FIELD_LUCK_RANK_POS_MAX_COUNT; ++ i)
		{
			int rank_pos_interval = rank_pos_section[i + 1] - rank_pos_section[i];
			if (rank_pos_interval > 0)
			{
				int rand_value = RandomNum(rank_pos_interval);
				int result_pos = rank_pos_section[i] + rand_value;
				m_challenge_field_data.luck_rank_pos_list[i] = result_pos;

				gamelog::g_log_challengefield.printf(LL_INFO, "GenrateLuckyRankPos::index[%d], pos[%d]", i, result_pos);
			}
		}
	}
}

bool ChallengeField::GenrateCrossChallengeUserParamInfo(ChallengeParam &param, RoleAppearance &appearance, int rank_index, int robot_num)
{
	if (rank_index < 0) return false;
	if (robot_num <= 0) return false;

	if (rank_index >= robot_num)
	{
		rank_index = robot_num - 1;
	}

	int rank = rank_index + 1;

	float rate = 1 - (rank - 1) / (robot_num * 1.0f);
	param.level = 200 + static_cast<int>(70 * rate);
	if (param.level < 0) param.level = 1;

	//2019.04.19

	int num_1 = 1500000 - 75000 * (rank - 1);
	int num_2 = 1200000 - 60000 * (rank - 5);
	int num_3 = 900000 - 30000 * (rank - 10);
	int num_4 = 600000 - 5000 * (rank - 20);
	int num_5 = 500000 - 5000 * (rank - 40);
	int num_6 = 310000 - 100 * rank;

	std::vector<int> num_list = { num_1, num_2, num_3, num_4, num_5, num_6 };

	auto biggest_it = std::max_element(std::begin(num_list), std::end(num_list));
	param.capability = *biggest_it;

	param.max_hp = static_cast<Attribute>(param.capability * 1.724137931);
	param.gongji = static_cast<Attribute>(param.capability * 0.25862069);
	param.fangyu = static_cast<Attribute>(param.capability * 0.086206897);
	param.mingzhong = static_cast<Attribute>(param.capability * 0.017241379);
	param.shanbi = static_cast<Attribute>(param.capability * 0.017241379);
	param.baoji = static_cast<Attribute>(param.capability * 0.017241379);
	param.jianren = static_cast<Attribute>(param.capability * 0.017241379);
	param.fujiashanghai = 0;
	param.dikangshanghai = 0;
	param.jingzhun_per = 0;
	param.baoji_per = 0;
	param.kangbao_per = 0;
	param.pofang_per = 0;
	param.mianshang_per = 0;
	param.constant_zengshang = 0;
	param.constant_mianshang = 0;

	int skill_level = param.level / 10;
	if (skill_level <= 0) skill_level = 1;

	param.guild_post = 0;
	param.guild_id = INVALID_GUILD_ID;
	memset(param.guild_name, 0, sizeof(param.guild_name));
	param.avatar_timestamp = 0;

	{
		// 外观信息
		UNSTD_STATIC_CHECK(CAMP_TYPE_MAX > 1);
		UNSTD_STATIC_CHECK(PROF_TYPE_PROF_NO_LIMIT >= 5);

		static ItemID item_id_list[] = { 0, 8110, 8210, 8310, 8410 };

		appearance.wuqi_id = 0;
		if (param.prof >= 0 && param.prof < static_cast<char>(sizeof(item_id_list) / sizeof(item_id_list[0])))
		{
			appearance.wuqi_id = item_id_list[(int)param.prof];
		}

		appearance.wing_used_imageid = 1 + RandomNum(5);

		{
			// 个人技能信息
			param.skill_list[0].skill_id = param.prof * 100 + 21;
			param.skill_list[0].level = skill_level;

			param.skill_list[1].skill_id = param.prof * 100 + 31;
			param.skill_list[1].level = skill_level;

			param.skill_list[2].skill_id = param.prof * 100 + 41;
			param.skill_list[2].level = skill_level;

			param.skill_list[3].skill_id = param.prof * 100 + 11;
			param.skill_list[3].level = skill_level;

			param.skill_list[4].skill_id = param.prof * 100 + 1;
			param.skill_list[4].level = skill_level;
		}
	}

	return true;
}

void ChallengeField::ResetCrossChallengeUserRank()
{
	const std::vector<ChallengeFieldRobotConfig> &robot_name_vector = LOGIC_CONFIG->GetChallengeFieldCfg().GetChallengeFieldRobotName();

	unsigned int robot_num = robot_name_vector.size();
	if (robot_num >= ChallengeFieldRobotConfig::CONFIG_ITEM_MAX_COUNT)
	{
		robot_num = ChallengeFieldRobotConfig::CONFIG_ITEM_MAX_COUNT;
	}

	// 清空
	{
		for (auto it : m_rankpos_map)
		{
			int rank_pos = it.second;
			if (rank_pos >= 0 && rank_pos < m_rankuser_num)
			{
				if (NULL != m_rankuser_list[rank_pos])
				{
					delete m_rankuser_list[rank_pos];
// 					m_dirty_mark[rank_pos] = true;
// 					m_old_state[rank_pos] = true;
					m_rankuser_list[rank_pos] = NULL;
				}
			}
		}

		m_rankpos_map.clear();
	}

// 	m_is_change = true;
// 	this->CheckSave(EngineAdapter::Instance().Time() + CHALLENGE_FIELD_SAVE_INTERVAL * 2);

	for (unsigned int rank_pos = 0; rank_pos < robot_num; rank_pos ++)
	{
		ChallengeFieldRobotConfig robot_cfg = robot_name_vector[rank_pos];

		UserID user_id(CHALLENGE_FIELD_ROBOT_SERVER_ID, rank_pos + 1);
		UniqueUserID unique_userid(CHALLENGE_FIELD_ROBOT_PLAT_TYPE, user_id);
		ChallengeParam param;
		RoleAppearance appearance;

		param.plat_type = CHALLENGE_FIELD_ROBOT_PLAT_TYPE;
		param.role_id = UidToInt(user_id);
		SNPRINTF(param.role_name, sizeof(param.role_name), "%s", robot_cfg.robot_name);
		param.avatar = 0;

		if (PROF_TYPE_PROF_4 >= PROF_TYPE_PROF_1)
		{
			param.prof = PROF_TYPE_PROF_1 + RandomNum(PROF_TYPE_PROF_4 - PROF_TYPE_PROF_1 + 1);
		}

		if (param.prof == PROF_TYPE_PROF_1 || param.prof == PROF_TYPE_PROF_2)
		{
			param.sex = MALE;
		}
		else
		{
			param.sex = FEMALE;
		}

		param.camp = 0;
		param.move_speed = BIRTH_MOVESPEED_BASE;
		param.vip_level = 0;

		this->GenrateCrossChallengeUserParamInfo(param, appearance, rank_pos, robot_num);

		ChallengeUser*challenge_user = this->CreateChallengeUser(unique_userid, param, appearance);

		this->SetChallengeUser(rank_pos, challenge_user);
	}

	this->OnSyncChallengeUserInitDataToGame(0);
}

void ChallengeField::SwapCrossChallengeUserRank()
{
	const std::vector<ChallengeFieldRobotConfig> &robot_name_vector = LOGIC_CONFIG->GetChallengeFieldCfg().GetChallengeFieldRobotName();

	unsigned int robot_num = robot_name_vector.size();
	if (robot_num >= ChallengeFieldRobotConfig::CONFIG_ITEM_MAX_COUNT)
	{
		robot_num = ChallengeFieldRobotConfig::CONFIG_ITEM_MAX_COUNT;
	}

	unsigned int total_user_num = m_rankpos_map.size();
	unsigned int player_num = total_user_num - robot_num;

	// 获取全部玩家排名
	std::set<int> player_rank_set;
	for (auto it : m_rankpos_map)
	{
		int rank_pos = it.second;
		if (rank_pos >= 0 && rank_pos < m_rankuser_num)
		{
			ChallengeUser *challenge_user = m_rankuser_list[rank_pos];
			if (NULL != challenge_user && !challenge_user->IsRobot())
			{
				player_rank_set.insert(rank_pos);
				if (player_rank_set.size() >= player_num)
				{
					break;
				}
			}
		}
	}

	// 获取玩家对应机器人的替换排名
	std::map<int, int> swap_rank_map;
	int tmp_rank_pos = (int)m_rankpos_map.size() - 1;
	for (; tmp_rank_pos >= 0; tmp_rank_pos --)
	{
		if (tmp_rank_pos >= 0 && tmp_rank_pos < m_rankuser_num)
		{
			if (NULL == m_rankuser_list[tmp_rank_pos] || !m_rankuser_list[tmp_rank_pos]->IsRobot())
			{
				continue;
			}

			if (player_rank_set.empty())
			{
				break;
			}

			for (auto it : player_rank_set)
			{
				if (it < tmp_rank_pos && it >= 0 && it < m_rankuser_num)
				{
					if (NULL != m_rankuser_list[it])
					{
						swap_rank_map[it] = tmp_rank_pos;

						player_rank_set.erase(it);
						break;
					}
				}
			}
		}
	}

	// 替换
	for (auto map_it : swap_rank_map)
	{
		if (map_it.first >= 0 && map_it.first < m_rankuser_num &&
			map_it.second >= 0 && map_it.second < m_rankuser_num)
		{
			if (nullptr != m_rankuser_list[map_it.first] && nullptr != m_rankuser_list[map_it.second])
			{
				SwapChallengeUserRank(m_rankuser_list[map_it.first], m_rankuser_list[map_it.second]);
			}
		}
	}

	this->OnUpdateChallengefieldInfoToGame();
}

void ChallengeField::SwapChallengeUserRank(ChallengeUser *challenge_user_1, ChallengeUser *challenge_user_2)
{
	if (nullptr == challenge_user_1 || nullptr == challenge_user_2)
	{
		return;
	}

	int rank_pos_1 = challenge_user_1->GetRankPos();
	int rank_pos_2 = challenge_user_2->GetRankPos();

	const std::vector<ChallengeFieldRobotConfig> &robot_name_vector = LOGIC_CONFIG->GetChallengeFieldCfg().GetChallengeFieldRobotName();

	int robot_num = static_cast<int>(robot_name_vector.size());
	if (robot_num >= ChallengeFieldRobotConfig::CONFIG_ITEM_MAX_COUNT)
	{
		robot_num = ChallengeFieldRobotConfig::CONFIG_ITEM_MAX_COUNT;
	}

	if (challenge_user_1->IsRobot() && (rank_pos_2 < CROSS_CHALLENGE_FIELD_MAX_USER) )
	{
		// 如果是机器人，那么需要重新将机器人恢复为名次对应的属性
		if (CrossConfig::Instance().IsHiddenServer())
		{
			this->GenrateCrossChallengeUserParamInfo(challenge_user_1->GetParam(), challenge_user_1->GetAppearance(), rank_pos_2, robot_num);
		}
		else
		{
			this->GenrateChallengeUserParamInfo(challenge_user_1->GetParam(), challenge_user_1->GetAppearance(), rank_pos_2, robot_num);
		}
	}

	this->SetChallengeUser(rank_pos_2, challenge_user_1, false, true);

	if (challenge_user_2->IsRobot() && (rank_pos_1 < CROSS_CHALLENGE_FIELD_MAX_USER))
	{
		// 如果是机器人，那么需要重新将机器人恢复为名次对应的属性
		if (CrossConfig::Instance().IsHiddenServer())
		{
			this->GenrateCrossChallengeUserParamInfo(challenge_user_2->GetParam(), challenge_user_2->GetAppearance(), rank_pos_1, robot_num);
		}
		else
		{
			this->GenrateChallengeUserParamInfo(challenge_user_2->GetParam(), challenge_user_2->GetAppearance(), rank_pos_1, robot_num);
		}
	}
	// 替换到对方名次
	this->SetChallengeUser(rank_pos_1, challenge_user_2, false, true);
}

int ChallengeField::GetRoleRewardRankPos(int uid)
{
	for (int rank_pos = 0; rank_pos < CHALLENGE_FIELD_RECORD_RANK_POS_MAX_COUNT; ++rank_pos)
	{
		if (uid == m_challenge_field_data.reward_rank_uid_list[rank_pos] && (0 != m_challenge_field_data.reward_rank_uid_list[rank_pos]))
		{
			return rank_pos;
		}
	}

	return 9999;
}