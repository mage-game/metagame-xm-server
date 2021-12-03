#include "rolefighting.hpp"

#include "obj/character/role.h"
#include "world.h"
#include "global/usercache/usercache.hpp"
#include "protocal/msgfighting.h"
#include "global/globalrole/globalrolemanager.hpp"
#include "servercommon/errornum.h"
#include "other/route/mailroute.hpp"
#include "gamelog.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "global/rank/rankmanager.hpp"
#include "fightingconfig.hpp"
#include "globalconfig/namecreateconfig.hpp"
#include "item/knapsack.h"
#include "config/logicconfigmanager.hpp"
#include "other/vip/vipconfig.hpp"
#include "other/vip/vip.hpp"
#include "other/shop/roleshop.hpp"
#include "other/capability/capability.hpp"
#include "task/newtaskmanager.h"
#include "config/joinlimitconfig.hpp"
#include "skill/skillpool.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/shengesystem/shengesystem.hpp"

RoleFighting::RoleFighting(GlobalRole *g_role) : IGlobalRoleItem(g_role), m_tmp_challenge_type(CHALLENGE_TYPE_NONE), m_tmp_challenge_param1(0), m_tmp_challenge_param2(0), m_next_add_challenge_timestamp(0), m_next_update_fake_fighting_times(0)
{
}

RoleFighting::~RoleFighting()
{
}

void RoleFighting::Init(const FightingParam &little_pet_attr)
{
	m_param = little_pet_attr;

	if (m_param.challenge_left_times < FIGHTING_CHALLENGE_DEFAULT_TIMES) // 低于默认次数则开始定时增加次数
	{
		m_next_add_challenge_timestamp = (unsigned int)EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().cf_restore_join_times_need_time_m * SECOND_PER_MIN;
	}
}

void  RoleFighting::GetAttr(FightingParam &little_pet_attr)
{
	little_pet_attr = m_param;
}

void RoleFighting::Update(unsigned int now_second)
{
	Role *role = this->GetRole();

	if (m_next_add_challenge_timestamp > 0 && now_second > m_next_add_challenge_timestamp)
	{
		if (m_param.challenge_left_times < FIGHTING_CHALLENGE_DEFAULT_TIMES)
		{
			++m_param.challenge_left_times;
			m_next_add_challenge_timestamp = now_second + LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().cf_restore_join_times_need_time_m * SECOND_PER_MIN;

			if (NULL != role)
			{
				this->ChallengeSendBaseInfo(role);
			}
			this->OnDataChange();
		}
		else
		{
			// 超过了默认次数，则停止定时加次数，使用次数那边会重新开始倒计时
			m_next_add_challenge_timestamp = 0;
		}
	}

	if (now_second > m_next_update_fake_fighting_times)
	{
		m_next_update_fake_fighting_times = now_second + MINING_SEALING_UPDATE_FAKE_INTERVAL_S;
		this->UpdateFakeFigting(now_second);
	}


	if (now_second > m_param.challenge_next_auto_reflush_time)
	{
		m_param.challenge_next_auto_reflush_time = now_second + LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().cf_auto_reflush_interval_s;

		if (NULL != role)
		{
			this->ChallengeCheckReflush(role, true); // 每隔一段时间自动刷新，跟手动刷新方式独立
		}
	}
}

void RoleFighting::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid)	return;

	m_param.mining_today_times = 0;
	m_param.mining_today_buy_times = 0;
	m_param.mining_today_rob_times = 0;

	m_param.sailing_today_times = 0;
	m_param.sailing_today_buy_times = 0;
	m_param.sailing_today_rob_times = 0;

	m_param.challenge_left_times = FIGHTING_CHALLENGE_DEFAULT_TIMES;
	m_param.challenge_vip_buy_times = 0;
	m_param.challenge_score = 0;

	Role *owner_role = this->GetRole();
	if (NULL != owner_role)
	{
		this->MiningSendBaseInfo(owner_role);
		this->SailingSendBaseInfo(owner_role);
		this->ChallengeSendBaseInfo(owner_role);
	}

	this->OnDataChange();
}

void RoleFighting::OnRoleLogin(Role *role)
{
	if (NULL != role)
	{
		// 登录登出都同步一下外观缓存，其他人挑战时用到
//		m_param.role_appearance_backup = role->GetAppearance();
		this->OnDataChange();
	}

	this->MiningSendBaseInfo(role);
	this->SailingSendBaseInfo(role);
	this->ChallengeSendBaseInfo(role);
	this->ChallengeCheckReflush(role);
}

void RoleFighting::OnRoleLogout(Role *role)
{
	if (NULL != role)
	{
//		m_param.role_appearance_backup = role->GetAppearance();
		this->OnDataChange();
	}
}

void RoleFighting::OnRoleUpLevel(Role *role, int new_level)
{
	this->ChallengeCheckReflush(role); 
}

void RoleFighting::OnHandleReq(short req_type, short param1, int param2)
{
	Role *owner_role = this->GetRole();
	if (NULL == owner_role) return;

	switch (req_type)
	{
		// 挖矿请求
	case Protocol::CSFightingReq::REQ_TYPE_M_MINING_INFO:
		{
			this->MiningSendListInfo(owner_role);
			m_param.mining_been_rob_need_notify = 0; // 主动请求了之后把标记还原
		}
		break;

	case Protocol::CSFightingReq::REQ_TYPE_M_BEEN_ROB_INFO:
		{
			this->MiningSendBeenRobInfo(owner_role);
		}
		break;

	case Protocol::CSFightingReq::REQ_TYPE_M_REFLUSH_MINING:
		{
			this->MiningReflush(owner_role);
		}
		break;

	case Protocol::CSFightingReq::REQ_TYPE_M_START_MINING:
		{
			this->MiningStart(owner_role);
		}
		break;

	case Protocol::CSFightingReq::REQ_TYPE_M_BUY_TIMES:
		{
			this->MiningBuyTimes(owner_role);
		}
		break;

	case Protocol::CSFightingReq::REQ_TYPE_M_ROB:
		{
			this->MiningRob(owner_role, param2);
		}
		break;

	case Protocol::CSFightingReq::REQ_TYPE_M_ROB_ROBOT:
		{
			this->MiningRobRobot(owner_role, param1);
		}
		break;
		

	case Protocol::CSFightingReq::REQ_TYPE_M_REVENGE:
		{
			this->MiningRevenge(owner_role, param1);
		}
		break;

	case Protocol::CSFightingReq::REQ_TYPE_M_FETCH_REWARD:
		{
			this->MiningFetchReward(owner_role);
		}
		break;

		// 航海请求
	case Protocol::CSFightingReq::REQ_TYPE_S_SAILING_INFO:
		{
			this->SailingSendListInfo(owner_role);
			m_param.sailing_been_rob_need_notify = 0; // 主动请求了之后把标记还原
		}
		break;
	case Protocol::CSFightingReq::REQ_TYPE_S_BEEN_ROB_INFO:
		{
			this->SailingSendBeenRobInfo(owner_role);
		}
		break;
	case Protocol::CSFightingReq::REQ_TYPE_S_REFLUSH_SAILING:
		{
			this->SailingReflush(owner_role);
		}
		break;
	case Protocol::CSFightingReq::REQ_TYPE_S_START_SAILING:
		{
			this->SailingStart(owner_role);
		}
		break;
	case Protocol::CSFightingReq::REQ_TYPE_S_BUY_TIMES:
		{
			this->SailingBuyTimes(owner_role);
		}
		break;

	case Protocol::CSFightingReq::REQ_TYPE_S_ROB:
		{
			this->SailingRob(owner_role, param2);
		}
		break;

	case Protocol::CSFightingReq::REQ_TYPE_S_ROB_ROBOT:
		{
			this->SailingRobRobot(owner_role, param1);
		}
		break;

	case Protocol::CSFightingReq::REQ_TYPE_S_REVENGE:
		{
			this->SailingRevenge(owner_role, param1);
		}
		break;
	case Protocol::CSFightingReq::REQ_TYPE_S_FETCH_REWARD:
		{
			this->SailingFetchReward(owner_role);
		}
		break;


		// 挑衅请求
	case Protocol::CSFightingReq::REQ_TYPE_C_INFO:
		{
			if (owner_role->GetLevel() >= LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().cf_stop_level)
			{
				break;
			}

			this->ChallengeSendList(owner_role);
		}
		break;

	case Protocol::CSFightingReq::REQ_TYPE_C_REFLUSH:
		{
			const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
			if (owner_role->GetLevel() >= other_cfg.cf_stop_level)
			{
				break;
			}

			if (!owner_role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.cf_reflush_need_bind_gold, "CSFightingReq::REQ_TYPE_C_REFLUSH"))
			{
				owner_role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
				break;
			}
			
			this->ChallengeReflush(owner_role);
		}
		break;

	case Protocol::CSFightingReq::REQ_TYPE_C_BUY_FIGHTING_TIMES:
		{
			if (owner_role->GetLevel() >= LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().cf_stop_level)
			{
				break;
			}

			this->ChallengeBuyFightingTimes(owner_role);
		}
		break;

	case Protocol::CSFightingReq::REQ_TYPE_C_FIGHTING:
		{
			if (owner_role->GetLevel() >= LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().cf_stop_level)
			{
				break;
			}

			this->ChallengeFightingReq(owner_role, param1);
		}
		break;

	default: break;
	}
}

void RoleFighting::OnFightingResult(bool is_win)
{
	Role *owner_role = this->GetRole();
	if (NULL == owner_role) return;

	switch (m_tmp_challenge_type)
	{
	case CHALLENGE_TYPE_MINING_ROB:
		{
			this->MiningOnRobResult(owner_role, is_win);
		}
		break;

	case CHALLENGE_TYPE_MINING_ROB_ROBOT:
		{
			this->MiningOnRobRobotResult(owner_role, is_win);
		}
		break;

	case CHALLENGE_TYPE_MINING_REVENGE:
		{
			this->MiningOnRevengeResult(owner_role, is_win);
		}
		break;

	case CHALLENGE_TYPE_SAILING_ROB:
		{
			this->SailingOnRobResult(owner_role, is_win);
		}
		break;

	case CHALLENGE_TYPE_SAILING_ROB_ROBOT:
		{
			this->SailingOnRobRobotResult(owner_role, is_win);
		}
		break;

	case CHALLENGE_TYPE_SAILING_REVENGE:
		{
			this->SailingOnRevengeResult(owner_role, is_win);
		}
		break;

	case CHALLENGE_TYPE_FIGHTING:
		{
			this->OnChallengeResult(owner_role, is_win);
		}

	default: break;
	}

	// 防止在各个result函数里中间返回，导致状态没有重置，所以把重置操作放外面
	m_tmp_challenge_type = CHALLENGE_TYPE_NONE;
}

bool RoleFighting::ConstructOpponentParam(ChallengeParam &param, RoleAppearance &appearance) const
{
	Role *owner_role = this->GetRole();
	if (NULL == owner_role)
	{
		return false;
	}

	param.plat_type = LocalConfig::Instance().GetPlatType();

	int opponent_uid = 0;
	int opponent_index = -1;
	int opponent_capability = 0;

	if (CHALLENGE_TYPE_MINING_ROB == m_tmp_challenge_type ||
		CHALLENGE_TYPE_MINING_REVENGE == m_tmp_challenge_type ||
		CHALLENGE_TYPE_SAILING_ROB == m_tmp_challenge_type ||
		CHALLENGE_TYPE_SAILING_REVENGE == m_tmp_challenge_type)
	{
		opponent_uid = m_tmp_challenge_param1;
	}
	else if (CHALLENGE_TYPE_MINING_ROB_ROBOT == m_tmp_challenge_type)
	{
		if (m_tmp_challenge_param1 >= 0 && m_tmp_challenge_param1 < FIGHTING_FAKE_MINING_SAILING_DATA_COUNT)
		{
			const FightingFakeMiningSailingData& fake_item = m_param.mining_fake_list[m_tmp_challenge_param1];
			opponent_index = fake_item.random_index;
			opponent_capability = fake_item.capability;
			this->ConstructAppearance(appearance);
		}
	}
	else if (CHALLENGE_TYPE_SAILING_ROB_ROBOT == m_tmp_challenge_type)
	{
		if (m_tmp_challenge_param1 >= 0 && m_tmp_challenge_param1 < FIGHTING_FAKE_MINING_SAILING_DATA_COUNT)
		{
			const FightingFakeMiningSailingData& fake_item = m_param.sailing_fake_list[m_tmp_challenge_param1];
			opponent_index = fake_item.random_index;
			opponent_capability = fake_item.capability;
			this->ConstructAppearance(appearance);
		}
	}
	else if (CHALLENGE_TYPE_FIGHTING == m_tmp_challenge_type)
	{
		if (m_tmp_challenge_param1 >= 0 && m_tmp_challenge_param1 < FIGHTING_CHALLENGE_OPPONENT_COUNT)
		{
			const FightingChellangeOpponentInfo &opponent_info = m_param.challenge_opponent_lsit[m_tmp_challenge_param1];
			RoleAppearance *user_appearance = GlobalRoleManager::Instance().GetRoleAppearance(opponent_info.random_uid);
			if (NULL != user_appearance)
			{
				appearance = *user_appearance;
			}
			else
			{
				this->ConstructAppearance(appearance);
			}

			if (opponent_info.random_uid > 0) // 挑衅分真人跟假人，真人则跟挖矿航海挑战一样获取信息
			{
				opponent_uid = opponent_info.random_uid;
			}
			else
			{
				opponent_index = opponent_info.random_name_index;
				opponent_capability = opponent_info.capability;
			}
		}
	}

	if (opponent_uid > 0) // 玩家属性构造
	{
		UserCacheNode* opponnet_cache = UserCacheManager::Instance().GetUserNode(opponent_uid);
		if (NULL == opponnet_cache)
		{
			return false;
		}

		param.role_id = opponent_uid;
		opponnet_cache->GetName(param.role_name);
		param.avatar = 0;
		param.sex = opponnet_cache->sex;
		param.prof = opponnet_cache->prof;
		param.camp = 0;// opponnet_cache->camp;
		param.level = opponnet_cache->level;
		param.vip_level = opponnet_cache->vip_level;
		param.capability = opponnet_cache->capability;
		param.avatar_timestamp = opponnet_cache->avatar_timestamp;
	}
	else if (opponent_index >= 0) // 机器人属性构造
	{
		param.role_id = owner_role->GetUID();
		LOGIC_CONFIG->GetNameCreateCfg().GetRandName(param.role_name, opponent_index, opponent_index % 2 <= 0);
		param.avatar = 0;
		param.sex = opponent_index % SEX_MAX;
		param.prof = opponent_index % (PROF_TYPE_PROF_4 - 1) + 1; // 随机三个职业（第四个职业在ug01里无效，代码却没注释。。）
		param.camp = 0;// opponent_index % (CAMP_TYPE_MAX - 1) + 1;

		param.level = opponent_index % 10 + owner_role->GetLevel();	// 比主角高10级以内
		if (param.level >= MAX_ROLE_LEVEL)
		{
			param.level = MAX_ROLE_LEVEL - 1; // 最大给个999级
		}

		param.vip_level = opponent_index % MAX_VIP_LEVEL;
		param.capability = opponent_capability;
		param.avatar_timestamp = owner_role->GetAvatarTimeStamp();
	}
	else
	{
		return false;
	}

	//		cache没有的信息就不赋值了
	//		param.guild_post = owner_role->GetGuildPost();
	//		param.guild_id = owner_role->GetGuildID();
	//		owner_role->GetGuildName(param.guild_name);
	//		param.footprint_effect_id = owner_role->GetFootPrint()->GetFootPrintEffectID();

	//param.appearance = owner_role->GetAppearance();

	SkillListParam skill_list_param;
	owner_role->GetSkillManager()->GetSkillList(&skill_list_param);
	int skill_count = 0;

	// 给机器人配备职业默认的几个技能
	for (int i = 0; i < skill_list_param.count && i < SkillListParam::MAX_SKILL_NUM && i < ROLE_PROF_SKILL_COUNT; ++i)
	{
		int skill_id = SkillPool::Instance()->GetSkillId(param.prof, i);
		if (skill_id > 0)
		{
			param.skill_list[skill_count].skill_id = skill_id;
			param.skill_list[skill_count].level = skill_list_param.skill_list[i].level; // 技能等级暂用玩家的
			++skill_count;
		}
	}
	

	double cap_fact = param.capability * 1.0 / owner_role->GetCapability()->GetTotalCapability();
	if (cap_fact <= 0) return false;

	param.max_hp = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP) * cap_fact);
	param.gongji = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) * cap_fact);
	param.fangyu = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * cap_fact);
	param.mingzhong = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG) * cap_fact);
	param.shanbi = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI) * cap_fact);
	param.baoji = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI) * cap_fact);
	param.jianren = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN) * cap_fact);
	param.move_speed = owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED); // 速度不用缩放
	param.fujiashanghai = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI) * cap_fact);
	param.dikangshanghai = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI) * cap_fact);

	param.jingzhun_per = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN) * cap_fact);
	param.baoji_per = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT) * cap_fact);
	param.kangbao_per = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO) * cap_fact);
	param.pofang_per = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG) * cap_fact);
	param.mianshang_per = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG) * cap_fact);
	param.constant_zengshang = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG) * cap_fact);
	param.constant_mianshang = (Attribute)(owner_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG) * cap_fact);

	return true;
}

void RoleFighting::OnGmReq(int type, Role *role)
{
	if (NULL == role) return;

	//   /gm rolefighting:type
	enum
	{
		TYPE_MIN_INVALID = 0,
		TYPE_MINING_RESET_TIMES,			// 挖矿 - 还原次数（挖矿次数，购买次数，抢夺次数全部置零）
		TYPE_SAILING_RESET_TIMES,			// 航海 - 还原次数（航行次数，购买次数，抢夺次数全部置零）
		TYPE_MAX_INVALID,
	};

	switch (type)
	{
	case TYPE_MINING_RESET_TIMES:
		{
			m_param.mining_today_times = 0;
			m_param.mining_today_buy_times = 0;
			m_param.mining_today_rob_times = 0;
			this->MiningSendBaseInfo(role);
		}
		break;

	case TYPE_SAILING_RESET_TIMES:
		{
			m_param.sailing_today_times = 0;
			m_param.sailing_today_buy_times = 0;
			m_param.sailing_today_rob_times = 0;
			this->MiningSendBaseInfo(role);
		}
		break;

	default: break;
	}
}

const Posi& RoleFighting::GetCurOpponentPos() const
{
	if (CHALLENGE_TYPE_MINING_ROB == m_tmp_challenge_type ||
		CHALLENGE_TYPE_MINING_ROB_ROBOT == m_tmp_challenge_type ||
		CHALLENGE_TYPE_MINING_REVENGE == m_tmp_challenge_type)
	{
		return LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().dm_opponent_pos;
	}
	else if (CHALLENGE_TYPE_SAILING_ROB == m_tmp_challenge_type ||
		CHALLENGE_TYPE_SAILING_ROB_ROBOT == m_tmp_challenge_type ||
		CHALLENGE_TYPE_SAILING_REVENGE == m_tmp_challenge_type)
	{
		return LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().sl_opponent_pos;
	}

	return LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().cf_opponent_pos;
}

void RoleFighting::OnSkipMining(Role *role, int quality)
{
	const FightingSkipTypeCfg &skip_cfg = LOGIC_CONFIG->GetFightingConfig().GetSkipCfg();
	if (role == nullptr || quality < 0 || quality > skip_cfg.mining_max_quality || quality >= FightingMiningRewardCfg::MAX_QUALITY_TYPE)
	{
		return;
	}

	if (role->GetLevel() < skip_cfg.mining_reward_cfg_list[quality].limit_level || role->GetLevel() < skip_cfg.mining_rob_limit_level)
	{
		return;
	}

	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();

	int mining_times = other_cfg.dm_day_times + m_param.mining_today_buy_times - m_param.mining_today_times;

	int rob_times = other_cfg.dm_rob_times - m_param.mining_today_rob_times;

	int consume = skip_cfg.mining_reward_cfg_list[quality].consume * mining_times + skip_cfg.mining_rob_consume * rob_times;
	if (consume <= 0)
	{
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseAllGold(consume, "Skip mining consume"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_param.mining_today_times += mining_times;
	for (int i = 0; i < mining_times; ++i)
	{
		m_param.mining_cur_quality = quality;
		this->MiningFetchReward(role, true);
	}

	for (int i = 0; i < rob_times; ++i)
	{
		int rand_quality = LOGIC_CONFIG->GetFightingConfig().MiningGetRandomRobotQuality();
		const FightingMiningRewardCfg *figting_cfg = LOGIC_CONFIG->GetFightingConfig().GetMiningCfg(rand_quality);
		if (NULL == figting_cfg)
		{
			continue;
		}

		// 给予抢夺奖励 
		double reward_fect = other_cfg.dm_rob_reward_rate / 100.0;
		if (reward_fect < 0)
		{
			reward_fect = 0;
		}

		int level_fact = role->GetLevel() + 50;
		long long add_exp = (long long)(level_fact * reward_fect * figting_cfg->reward_exp);
		ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < figting_cfg->reward_item_count; ++i)
		{
			reward_item_list[i] = figting_cfg->reward_item_list[i];
			//reward_item_list[i].num = int(reward_item_list[i].num * reward_fect);
			reward_item_list[i].num = figting_cfg->rob_get_item_count;
			if (reward_item_list[i].num > 0)
			{
				role->GetKnapsack()->Put(reward_item_list[i], PUT_REASON_FIGHTING_MININE_ROB);
			}
		}
		role->AddExp(add_exp, "RoleFighting::MiningOnRobRobotResult", Role::EXP_ADD_REASON_DEFAULT);

		++m_param.mining_today_rob_times;

		this->SendFightingResult(role, true, add_exp, reward_item_list, true);

		role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_MINING_ROB);

		this->MiningSendBaseInfo(role);
	}
}

void RoleFighting::OnSkipSailing(Role *role, int quality)
{
	const FightingSkipTypeCfg &skip_cfg = LOGIC_CONFIG->GetFightingConfig().GetSkipCfg();
	if (role == nullptr || quality < 0 || quality > skip_cfg.sailing_max_quality || quality > FightingSailingRewardCfg::MAX_QUALITY_TYPE)
	{
		return;
	}

	if (role->GetLevel() < skip_cfg.sailing_reward_cfg_list[quality].limit_level || role->GetLevel() < skip_cfg.sailing_rob_limit_level)
	{
		return;
	}

	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
	int total_times = other_cfg.sl_day_times + m_param.sailing_today_buy_times - m_param.sailing_today_times;

	int rob_times = other_cfg.sl_rob_times - m_param.sailing_today_rob_times;

	int consume = skip_cfg.sailing_reward_cfg_list[quality].consume * total_times + rob_times * skip_cfg.sailing_rob_consume;
	if (consume <= 0)
	{
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseAllGold(consume, "Skip sailing consume"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_param.sailing_today_times += total_times;
	for (int i = 0; i < total_times; ++i)
	{
		m_param.sailing_cur_quality = quality;
		this->SailingFetchReward(role, true);
	}

	for (int i = 0; i < rob_times; ++i)
	{
		int rand_quality = LOGIC_CONFIG->GetFightingConfig().SailingGetRandomRobotQuality();
		const FightingSailingRewardCfg *figting_cfg = LOGIC_CONFIG->GetFightingConfig().GetSailingCfg(rand_quality);
		if (NULL == figting_cfg)
		{
			continue;
		}

		// 给予抢夺奖励 
		double reward_fect = other_cfg.sl_rob_reward_rate / 100.0;
		if (reward_fect < 0)
		{
			reward_fect = 0;
		}

		int level_fact = role->GetLevel() + 50;
		long long add_exp = (long long)(level_fact * reward_fect * figting_cfg->reward_exp);
		ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < figting_cfg->reward_item_count; ++i)
		{
			reward_item_list[i] = figting_cfg->reward_item_list[i];
			//reward_item_list[i].num = int(reward_item_list[i].num * reward_fect);
			reward_item_list[i].num = figting_cfg->rob_get_item_count;
			if (reward_item_list[i].num > 0)
			{
				role->GetKnapsack()->Put(reward_item_list[i], PUT_REASON_FIGHTING_MININE_ROB);
			}
		}
		role->AddExp(add_exp, "RoleFighting::SailingOnRobRobotResult", Role::EXP_ADD_REASON_DEFAULT);

		++m_param.sailing_today_rob_times;

		this->SendFightingResult(role, true, add_exp, reward_item_list, true);
		this->SailingSendBaseInfo(role);

		role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_SAILING_ROB);
	}
}

void RoleFighting::OnSkipChallenge(Role *role)
{
	if (role == nullptr)
	{
		return;
	}

	const FightingSkipTypeCfg &skip_cfg = LOGIC_CONFIG->GetFightingConfig().GetSkipCfg();

	if (role->GetLevel() < skip_cfg.challenge_skip_limit_level)
	{
		return;
	}

	if (role->GetLevel() >= LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().cf_stop_level)
	{
		return;
	}

	int total_times = m_param.challenge_left_times;
	int consume = skip_cfg.challenge_skip_consume * total_times;
	if (consume <= 0)
	{
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseAllGold(consume, "Skip challenge consume"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	for (int i = 0; i < total_times; ++i)
	{
		this->OnChallengeResult(role, true, true);
	}

	if (0 == m_next_add_challenge_timestamp)
	{
		m_next_add_challenge_timestamp = (unsigned int)EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().cf_restore_join_times_need_time_m * SECOND_PER_MIN;
		this->ChallengeSendBaseInfo(role);
	}
}

void RoleFighting::MiningFillInfo(Protocol::SCFightingMiningList& fml, int watcher_role_id) const
{
	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();

	if (m_param.mining_cur_been_rob_times >= other_cfg.dm_been_rob_times)
	{
		return;
	}

	if ((unsigned int)EngineAdapter::Instance().Time() > m_param.mining_end_time)
	{
		return;
	}

	// 推算出开始挖矿时间，用来判断在这个时间后，查看信息的玩家有无掠夺过此矿
	unsigned int mining_start_time = m_param.mining_end_time - LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().dm_cost_time_m * SECOND_PER_MIN;
	for (int i = 0; i < FIGHTING_MAX_ROB_HISTORY_COUNT; ++i)
	{
		const FightingRobHistoryItem &history_item = m_param.mining_been_rob_list[i];
		if (history_item.rober_uid == watcher_role_id && history_item.rob_time > mining_start_time)
		{
			return;
		}
	}

	if (m_param.mining_end_time > 0 && m_param.mining_cur_quality >= 0 && m_param.mining_cur_quality <= LOGIC_CONFIG->GetFightingConfig().GetMineMaxQuality())
	{
		UserCacheNode *user_node = this->GetUserCache();
		if (NULL != user_node)
		{
			Protocol::SCFightingMiningList::ItemInfo &item = fml.mine_list[fml.mine_count++];
			item.cur_quality = m_param.mining_cur_quality;
			item.owner_uid = m_role_id;
			item.random_index = -1;
			item.robot_index = -1;
			item.mining_end_time = m_param.mining_end_time;
			
			item.owner_sex = user_node->sex;
			item.owner_prof = user_node->prof;
			item.owner_capability = user_node->capability;
			item.owner_level = user_node->level;
			user_node->GetName(item.owner_name);
		}
	}
}

void RoleFighting::MiningSendListInfo(Role *role) const
{
	if (NULL == role) return;

	static Protocol::SCFightingMiningList fml;
	fml.mine_count = 0;

	const GlobalRoleManager::GlobalRoleMap &global_role_map = GLOBAL_ROLE_MGR.GetGlobalRoleMap();
	for (GlobalRoleManager::GlobalRoleMap::const_iterator con_it = global_role_map.begin(); con_it != global_role_map.end() && fml.mine_count < Protocol::SCFightingMiningList::MAX_NOTIFY_COUNT; ++con_it)
	{
		con_it->second->GetRoleFighting()->MiningFillInfo(fml, m_role_id);
	}

	if (fml.mine_count < 8) // 少于指定数量的人挖矿，则把机器人矿点暴露给玩家
	{
		for (int i = 0; i < FIGHTING_FAKE_MINING_SAILING_DATA_COUNT && fml.mine_count < Protocol::SCFightingMiningList::MAX_NOTIFY_COUNT; ++i)
		{
			const FightingFakeMiningSailingData &fake_mining = m_param.mining_fake_list[i];
			if (fake_mining.random_index < 0 || fake_mining.cur_been_rob_times > 0) continue;

			Protocol::SCFightingMiningList::ItemInfo &item = fml.mine_list[fml.mine_count++];
			item.cur_quality = fake_mining.cur_quality;
			item.owner_uid = 0;
			item.random_index = fake_mining.random_index;
			item.robot_index = i;
			item.mining_end_time = fake_mining.end_time;
			item.owner_sex = fake_mining.random_index % SEX_MAX;
			item.owner_prof = fake_mining.random_index % (PROF_TYPE_PROF_4 - 1) + 1; // 随机三个职业（第四个职业在ug01里无效，代码却没注释。。）
			item.owner_capability = fake_mining.capability;
			item.owner_level = role->GetLevel();
			//F_STRNCPY(item.owner_name, "SOS", sizeof(GameName)); // 机器人的名字不要用服务端下发的，用fake_mining.random_index去随机名字列表里找
		}
	}

	int send_len = sizeof(fml) - (Protocol::SCFightingMiningList::MAX_NOTIFY_COUNT - fml.mine_count) * sizeof(fml.mine_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&fml, send_len);
}

void RoleFighting::MiningSendBeenRobInfo(Role *role)
{
	if (NULL == role) return;

	static Protocol::SCFightingMiningBeenRobList fmbrl;
	for (int rob_index = 0; rob_index < FIGHTING_MAX_ROB_HISTORY_COUNT; ++rob_index)
	{
		FightingRobHistoryItem &rob_item = m_param.mining_been_rob_list[rob_index];
		UserCacheNode *rober_cache = UserCacheManager::Instance().GetUserNode(rob_item.rober_uid);
		if (NULL != rober_cache)
		{
			rober_cache->GetName(fmbrl.rob_list[rob_index].rober_name);
			fmbrl.rob_list[rob_index].rob_time = rob_item.rob_time;
			fmbrl.rob_list[rob_index].has_revenge = rob_item.has_revenge;
			fmbrl.rob_list[rob_index].real_index = rob_index;
			fmbrl.rob_list[rob_index].cur_quality = rob_item.quality;
			fmbrl.rob_list[rob_index].rober_sex = rober_cache->sex;
			fmbrl.rob_list[rob_index].rober_prof = rober_cache->prof;
			fmbrl.rob_list[rob_index].rober_capability = rober_cache->capability;
		}
		else
		{
			// 无效uid说明此条记录无效，设置一下无效值，客户端就不会显示
			fmbrl.rob_list[rob_index].rob_time = 0;
			fmbrl.rob_list[rob_index].real_index = -1;
		}
	}
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&fmbrl, sizeof(fmbrl));
}

void RoleFighting::MiningReflush(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	int total_times = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().dm_day_times + m_param.mining_today_buy_times;
	if (m_param.mining_today_times >= total_times)
	{
		role->NoticeNum(errornum::EN_FIGHTING_MINING_TIMES_LIMIT);
		return;
	}

	const FightingMiningRewardCfg *figting_cfg = LOGIC_CONFIG->GetFightingConfig().GetMiningCfg(m_param.mining_cur_quality);
	if (NULL == figting_cfg)
	{
		return;
	}

	const FightingMiningRewardCfg *next_figting_cfg = LOGIC_CONFIG->GetFightingConfig().GetMiningCfg(m_param.mining_cur_quality + 1);
	if (NULL == next_figting_cfg)
	{
		role->NoticeNum(errornum::EN_FIGHTING_MINING_QUALITY_FULL);
		return;
	}

	// 消耗
	if (!role->GetKnapsack()->GetMoney()->UseGold(figting_cfg->consume_gold, "RoleFighting::MiningReflush"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int old_quality = m_param.mining_cur_quality;
	// 给予
	if (RandomNum(100) < figting_cfg->upgrade_rate)
	{
		++m_param.mining_cur_quality;
		this->MiningSendBaseInfo(role);
		this->OnDataChange();
	}
	else
	{
		role->NoticeNum(errornum::EN_FIGHTING_MINING_REFLUSH_FAIL);
	}

	gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::MiningReflush user[%d %s] consume_gold[%d] quality[ %d -> %d ]", role->GetUID(), role->GetName(), figting_cfg->consume_gold, old_quality, m_param.mining_cur_quality);
}

void RoleFighting::MiningSendBaseInfo(Role *role) const
{
	if (NULL == role) return;

	static Protocol::SCFightingMiningBaseInfo fmbi;
	fmbi.mining_quality = m_param.mining_cur_quality;
	fmbi.mining_end_time = m_param.mining_end_time;
	fmbi.today_mining_times = m_param.mining_today_times;
	fmbi.today_buy_times = m_param.mining_today_buy_times;
	fmbi.today_rob_mine_times = m_param.mining_today_rob_times;
	fmbi.mining_been_rob_times = m_param.mining_cur_been_rob_times;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&fmbi, sizeof(fmbi));
}

void RoleFighting::MiningStart(Role* role)
{
	if (NULL == role) return;

	if (m_param.mining_end_time > 0)
	{
		role->NoticeNum(errornum::EN_FIGHTING_MINING_ALEADY);
		return;
	}

	int total_times = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().dm_day_times + m_param.mining_today_buy_times;
	if (m_param.mining_today_times >= total_times)
	{
		role->NoticeNum(errornum::EN_FIGHTING_MINING_TIMES_LIMIT);
		return;
	}

	// 消耗
	++m_param.mining_today_times;

	// 给予
	m_param.mining_cur_been_rob_times = 0;
	m_param.mining_end_time = (unsigned int)EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().dm_cost_time_m * SECOND_PER_MIN;

	this->MiningSendBaseInfo(role);
	this->MiningSendListInfo(role);
	this->OnDataChange();
	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_MINE_TIMES, 1);

	gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::MiningStart user[%d %s] mining_today_times[%d] end_time[%d]", role->GetUID(), role->GetName(), m_param.mining_today_times, m_param.mining_end_time);
}

void RoleFighting::MiningBuyTimes(Role* role)
{
	if (NULL == role) return;

	int vip_max_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_FIGHTING_MINING_BUY_TIMES);
	if (m_param.mining_today_buy_times >= vip_max_buy_times)
	{
		role->NoticeNum(errornum::EN_FIGHTING_VIP_BUY_LIMIT);
		return;
	}

	// 消耗
	if (!role->GetKnapsack()->GetMoney()->UseGold(LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().dm_buy_time_need_gold, "RoleFighting::MiningBuyTimes"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 给予
	++m_param.mining_today_buy_times;
	
	this->MiningSendBaseInfo(role);
	this->OnDataChange();

	gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::MiningBuyTimes user[%d %s] mining_today_buy_times[%d] consume_gold[%d]", role->GetUID(), role->GetName(), m_param.mining_today_buy_times, LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().dm_buy_time_need_gold);
}

void RoleFighting::MiningRob(Role* role, int uid)
{
	if (NULL == role || uid == m_role_id) return;

	if (!role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}

	// 防止重复点击挑战，重复消耗次数
	if (CHALLENGE_TYPE_NONE != m_tmp_challenge_type)
	{
		return;
	}

	RoleFighting *targert_fighting = GlobalRoleManager::Instance().GetRoleFighting(uid);
	if (NULL == targert_fighting) return;

	FightingParam &target_param = targert_fighting->m_param;

	if (target_param.mining_end_time <= 0)
	{
		return;
	}

	if ((unsigned int)EngineAdapter::Instance().Time() > target_param.mining_end_time)
	{
		role->NoticeNum(errornum::EN_FIGHTING_MINING_TIME_END);
		return;
	}

	// 推算出开始挖矿时间，用来判断在这个时间后，当前玩家有无掠夺过此矿
	unsigned int mining_start_time = target_param.mining_end_time - LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().dm_cost_time_m * SECOND_PER_MIN;
	for (int i = 0; i < FIGHTING_MAX_ROB_HISTORY_COUNT; ++i)
	{
		FightingRobHistoryItem &history_item = target_param.mining_been_rob_list[i];
		if (history_item.rober_uid == m_role_id && history_item.rob_time > mining_start_time)
		{
			role->NoticeNum(errornum::EN_FIGHTING_MINING_ALEADY_ROB);
			return;
		}
	}

	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
	if (target_param.mining_cur_been_rob_times >= other_cfg.dm_been_rob_times)
	{
		role->NoticeNum(errornum::EN_FIGHTING_MINING_BEEN_ROB_LIMIT);
		return;
	}

	if (m_param.mining_today_rob_times >= other_cfg.dm_rob_times)
	{
		role->NoticeNum(errornum::EN_FIGHTING_MINING_ROB_LIMIT);
		return;
	}

	// 消耗次数
	++m_param.mining_today_rob_times;

	// 埋好铺垫
	m_tmp_challenge_type = CHALLENGE_TYPE_MINING_ROB;
	m_tmp_challenge_param1 = uid;

	this->MiningSendBaseInfo(role);
	this->OnDataChange();

	// 送入决斗场
	World::GetInstWorld()->GetSceneManager()->GoTo(role, other_cfg.dm_scene_id, role->GetUID(), other_cfg.dm_sponsor_pos);
}

void RoleFighting::MiningOnRobResult(Role* role, bool is_win)
{
	RoleFighting *targert_fighting = GlobalRoleManager::Instance().GetRoleFighting(m_tmp_challenge_param1);
	if (NULL == targert_fighting) return;

	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
	FightingParam &target_param = targert_fighting->m_param;
	if (target_param.mining_cur_been_rob_times >= other_cfg.dm_been_rob_times)
	{
		return;
	}

	const FightingMiningRewardCfg *figting_cfg = LOGIC_CONFIG->GetFightingConfig().GetMiningCfg(target_param.mining_cur_quality);
	if (NULL == figting_cfg)
	{
		return;
	}

	long long add_exp = 0;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	if (is_win)
	{
		target_param.mining_been_rob_last_index = (target_param.mining_been_rob_last_index + 1) % FIGHTING_MAX_ROB_HISTORY_COUNT;
		FightingRobHistoryItem &rob_history_item = target_param.mining_been_rob_list[int(target_param.mining_been_rob_last_index)];

		// 增加抢夺记录
		rob_history_item.quality = target_param.mining_cur_quality;
		rob_history_item.rober_uid = m_role_id;
		rob_history_item.rob_time = (unsigned int)EngineAdapter::Instance().Time();
		rob_history_item.has_revenge = 0;
		++target_param.mining_cur_been_rob_times;

		// 给予抢夺奖励 
		double reward_fect = other_cfg.dm_rob_reward_rate / 100.0;
		if (reward_fect < 0)
		{
			reward_fect = 0;
		}

		UserCacheNode *node = UserCacheManager::Instance().GetUserNode(m_tmp_challenge_param1);
		if (NULL == node)
		{
			return;
		}

		int level_fact = role->GetLevel() + 50;

		add_exp = (long long)(level_fact * reward_fect * figting_cfg->reward_exp);
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < figting_cfg->reward_item_count; ++i)
		{
			reward_item_list[i] = figting_cfg->reward_item_list[i];
			//reward_item_list[i].num = int(reward_item_list[i].num * reward_fect);
			reward_item_list[i].num = figting_cfg->rob_get_item_count;
			if (reward_item_list[i].num > 0)
			{
				role->GetKnapsack()->Put(reward_item_list[i], PUT_REASON_FIGHTING_MININE_ROB);
			}
		}
		role->AddExp(add_exp, "RoleFighting::MiningOnRobResult", Role::EXP_ADD_REASON_DEFAULT);

		target_param.mining_been_rob_need_notify = 1;
		Role *target_role = targert_fighting->GetRole();
		if (NULL != target_role)
		{
			this->MiningSendBaseInfo(target_role);
		}

		targert_fighting->BeenRobCheckNotify(0);

		this->RobingNotify(0, target_param.mining_cur_quality, role, node);
	}

	this->SendFightingResult(role, is_win, add_exp, reward_item_list);

	m_tmp_challenge_param1 = 0;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_MINING_ROB);

	std::string item_list_str;
	ItemConfigDataListLog(item_list_str, figting_cfg->reward_item_count, reward_item_list);
	gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::MiningOnRobResult user[%d %s] is_win[%d] add_exp[%lld] item_list[%s]",
		role->GetUID(), role->GetName(), is_win ? 1 : 0, add_exp, item_list_str.c_str());
}

void RoleFighting::MiningRobRobot(class Role* role, int index)
{
	if (NULL == role || index < 0 || index >= FIGHTING_FAKE_MINING_SAILING_DATA_COUNT) return;

	if (!role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}

	// 防止重复点击挑战，重复消耗次数
	if (CHALLENGE_TYPE_NONE != m_tmp_challenge_type)
	{
		return;
	}

	FightingFakeMiningSailingData &fake_item = m_param.mining_fake_list[index];

	if (fake_item.end_time <= 0)
	{
		return;
	}

	if (fake_item.cur_been_rob_times > 0)
	{
		role->NoticeNum(errornum::EN_FIGHTING_MINING_ALEADY_ROB);
		return;
	}

	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
	if (m_param.mining_today_rob_times >= other_cfg.dm_rob_times)
	{
		role->NoticeNum(errornum::EN_FIGHTING_MINING_ROB_LIMIT);
		return;
	}

	// 消耗次数
	++m_param.mining_today_rob_times;

	// 埋好铺垫
	m_tmp_challenge_type = CHALLENGE_TYPE_MINING_ROB_ROBOT;
	m_tmp_challenge_param1 = index;

	this->MiningSendBaseInfo(role);
	this->OnDataChange();

	// 送入决斗场
	World::GetInstWorld()->GetSceneManager()->GoTo(role, other_cfg.dm_scene_id, role->GetUID(), other_cfg.dm_sponsor_pos);
}

void RoleFighting::MiningOnRobRobotResult(Role* role, bool is_win)
{
	if (NULL == role || m_tmp_challenge_param1 < 0 || m_tmp_challenge_param1 >= FIGHTING_FAKE_MINING_SAILING_DATA_COUNT) return;

	FightingFakeMiningSailingData &fake_item = m_param.mining_fake_list[m_tmp_challenge_param1];

	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
	if (fake_item.cur_been_rob_times >= other_cfg.dm_been_rob_times)
	{
		return;
	}

	const FightingMiningRewardCfg *figting_cfg = LOGIC_CONFIG->GetFightingConfig().GetMiningCfg(fake_item.cur_quality);
	if (NULL == figting_cfg)
	{
		return;
	}

	// 消耗
	if (fake_item.cur_been_rob_times <= 0)
	{
		++fake_item.cur_been_rob_times;
	}

	long long add_exp = 0;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	if (is_win)
	{
		// 给予抢夺奖励 
		double reward_fect = other_cfg.dm_rob_reward_rate / 100.0;
		if (reward_fect < 0)
		{
			reward_fect = 0;
		}

		int level_fact = role->GetLevel() + 50;
		add_exp = (long long)(level_fact * reward_fect * figting_cfg->reward_exp);
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < figting_cfg->reward_item_count; ++i)
		{
			reward_item_list[i] = figting_cfg->reward_item_list[i];
			//reward_item_list[i].num = int(reward_item_list[i].num * reward_fect);
			reward_item_list[i].num = figting_cfg->rob_get_item_count;
			if (reward_item_list[i].num > 0)
			{
				role->GetKnapsack()->Put(reward_item_list[i], PUT_REASON_FIGHTING_MININE_ROB);
			}
		}
		role->AddExp(add_exp, "RoleFighting::MiningOnRobRobotResult", Role::EXP_ADD_REASON_DEFAULT);

		this->RobingNotify(0, fake_item.cur_quality, role, NULL, fake_item.random_index);
	}

	this->SendFightingResult(role, is_win, add_exp, reward_item_list);

	m_tmp_challenge_param1 = 0;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_MINING_ROB);

	std::string item_list_str;
	ItemConfigDataListLog(item_list_str, figting_cfg->reward_item_count, reward_item_list);
	gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::MiningOnRobRobotResult user[%d %s] robot_index[%d] is_win[%d] add_exp[%lld] item_list[%s]",
		role->GetUID(), role->GetName(), m_tmp_challenge_param1, is_win ? 1 : 0, add_exp, item_list_str.c_str());
}

void RoleFighting::MiningRevenge(Role* role, short been_rob_list_index)
{
	if (NULL == role || been_rob_list_index < 0 || been_rob_list_index >= FIGHTING_MAX_ROB_HISTORY_COUNT)
	{
		return;
	}

	if (!role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}

	// 防止重复点击挑战，重复消耗次数
	if (CHALLENGE_TYPE_NONE != m_tmp_challenge_type)
	{
		return;
	}

	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
	FightingRobHistoryItem &history_item = m_param.mining_been_rob_list[been_rob_list_index];
	if (history_item.has_revenge > 0) // 已经复仇过，不理
	{
		return;
	}

	if (m_param.mining_today_rob_times >= other_cfg.dm_rob_times)
	{
		role->NoticeNum(errornum::EN_FIGHTING_MINING_ROB_LIMIT);
		return;
	}

	// 埋好铺垫
	m_tmp_challenge_type = CHALLENGE_TYPE_MINING_REVENGE;
	m_tmp_challenge_param1 = history_item.rober_uid;
	m_tmp_challenge_param2 = been_rob_list_index;

	// 送入决斗场
	World::GetInstWorld()->GetSceneManager()->GoTo(role, other_cfg.dm_scene_id, role->GetUID(), other_cfg.dm_sponsor_pos);
}

void RoleFighting::MiningOnRevengeResult(Role* role, bool is_win)
{
	if (NULL == role || m_tmp_challenge_param2 < 0 || m_tmp_challenge_param2 >= FIGHTING_MAX_ROB_HISTORY_COUNT)
	{
		return;
	}

	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
	FightingRobHistoryItem *history_item = &m_param.mining_been_rob_list[m_tmp_challenge_param2];

	// 找到的复仇列表对应玩家名不是当前对战玩家，说明有状态冲突，需要重新遍历抢劫列表找到对应玩家ID
	if (history_item->rober_uid != m_tmp_challenge_param1)
	{
		history_item = NULL;
		for (int i = 0; i < FIGHTING_MAX_ROB_HISTORY_COUNT; ++i)
		{
			if (m_param.mining_been_rob_list[i].rober_uid == m_tmp_challenge_param1)
			{
				history_item = &m_param.mining_been_rob_list[i];
				break;
			}
		}

		if (NULL == history_item)
		{
			return;
		}
	}

	if (history_item->has_revenge > 0) // 已经复仇过，不理
	{
		return;
	}

	const FightingMiningRewardCfg *figting_cfg = LOGIC_CONFIG->GetFightingConfig().GetMiningCfg(m_param.mining_cur_quality);
	if (NULL == figting_cfg)
	{
		return;
	}

	// 给予复仇奖励 
	long long add_exp = 0;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	if (is_win)
	{
		// 复仇成功，消耗抢劫次数
		++m_param.mining_today_rob_times;
		this->MiningSendBaseInfo(role);

		history_item->has_revenge = 1;
		double reward_fect = 2 * other_cfg.dm_rob_reward_rate / 100.0;
		if (reward_fect < 0)
		{
			reward_fect = 0;
		}

		int level_fact = role->GetLevel() + 50;
		add_exp = (long long)(level_fact * reward_fect * figting_cfg->reward_exp);
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < figting_cfg->reward_item_count; ++i)
		{
			ItemConfigData &reward_item = reward_item_list[i];
			reward_item = figting_cfg->reward_item_list[i];
			//reward_item.num = int(reward_item_list[i].num * reward_fect);
			reward_item_list[i].num = figting_cfg->rob_get_item_count * 2;
			if (reward_item.num > 0)
			{
				role->GetKnapsack()->Put(reward_item, PUT_REASON_FIGHTING_MININE_REVENGE);
			}
		}
		role->AddExp(add_exp, "RoleFighting::MiningOnRobResult", Role::EXP_ADD_REASON_DEFAULT);
	}

	this->SendFightingResult(role, is_win, add_exp, reward_item_list);

	m_tmp_challenge_param1 = 0;
	m_tmp_challenge_param2 = 0;

	std::string item_list_str;
	ItemConfigDataListLog(item_list_str, figting_cfg->reward_item_count, reward_item_list);
	gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::MiningOnRevengeResult user[%d %s] is_win[%d] add_exp[%lld] item_list[%s]",
		role->GetUID(), role->GetName(), is_win ? 1 : 0, add_exp, item_list_str.c_str());
}

void RoleFighting::MiningFetchReward(Role* role, bool is_skip)
{
	if (NULL == role) return;

	if (!is_skip)
	{
		unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();
		if (m_param.mining_end_time <= 0 || now_second < m_param.mining_end_time)
		{
			role->NoticeNum(errornum::EN_FIGHTING_MINING_FETCH_TIME_LIMIT);
			return;
		}
	}

	long long add_exp = 0;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	const FightingMiningRewardCfg* figting_cfg = LOGIC_CONFIG->GetFightingConfig().GetMiningCfg(m_param.mining_cur_quality);
	if (NULL != figting_cfg)
	{
		const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
		double reward_fect = 1 - (m_param.mining_cur_been_rob_times * other_cfg.dm_rob_reward_rate / 100.0);
		if (reward_fect < 0)
		{
			reward_fect = 0;
		}

		int level_fact = role->GetLevel() + 50;
		add_exp = (long long)(level_fact * reward_fect * figting_cfg->reward_exp);
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < figting_cfg->reward_item_count; ++i)
		{
			ItemConfigData &reward_item = reward_item_list[i];
			reward_item = figting_cfg->reward_item_list[i];
			//reward_item.num = int(reward_item_list[i].num * reward_fect);
			reward_item.num = int(reward_item_list[i].num - figting_cfg->rob_get_item_count * m_param.mining_cur_been_rob_times);
			if (reward_item.num > 0)
			{
				role->GetKnapsack()->Put(reward_item, PUT_REASON_FIGHTING_MININE_REVENGE);
			}
		}
		role->AddExp(add_exp, "RoleFighting::MiningOnRobResult", Role::EXP_ADD_REASON_DEFAULT);
	}

	m_param.mining_end_time = 0;
	m_param.mining_cur_quality = 0;
	m_param.mining_cur_been_rob_times = 0;

	this->MiningSendBaseInfo(role);
	this->MiningSendListInfo(role);
	this->OnDataChange();

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_MINING);

	std::string item_list_str;
	ItemConfigDataListLog(item_list_str, figting_cfg->reward_item_count, reward_item_list);
	gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::MiningFetchReward user[%d %s] add_exp[%lld] item_list[%s]",
		role->GetUID(), role->GetName(), add_exp, item_list_str.c_str());
}

void RoleFighting::ChallengeSendBaseInfo(Role *role) const
{
	if (NULL == role) return;

	static Protocol::SCFightingChallengeBaseInfo fcbi;
	fcbi.challenge_score = m_param.challenge_score;
	fcbi.challenge_limit_times = m_param.challenge_left_times;
	fcbi.vip_buy_times = m_param.challenge_vip_buy_times;
	fcbi.next_add_challenge_timestamp = m_next_add_challenge_timestamp;
	fcbi.next_auto_reflush_time = m_param.challenge_next_auto_reflush_time;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&fcbi, sizeof(fcbi));
}

void RoleFighting::ChallengeSendList(Role* role)
{
	if (NULL == role) return;

	static Protocol::SCFightingChallengeList fcl;
			
	for (int index = 0; index < FIGHTING_CHALLENGE_OPPONENT_COUNT; ++index)
	{
		FightingChellangeOpponentInfo &opponent_info = m_param.challenge_opponent_lsit[index];
		Protocol::SCFightingChallengeList::OpponentInfo &p_opponent_info = fcl.opponent_list[index];
		RoleAppearance *user_appearance = GlobalRoleManager::Instance().GetRoleAppearance(opponent_info.random_uid);
		if (NULL != user_appearance)
		{
			p_opponent_info.appearance = *user_appearance;
		}
		else
		{
			this->ConstructAppearance(p_opponent_info.appearance);
		}
		p_opponent_info.has_win = opponent_info.has_win;

		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(opponent_info.random_uid);
		RoleFighting *opponent_fighting = GlobalRoleManager::Instance().GetRoleFighting(opponent_info.random_uid);
		if (NULL != user_node && NULL != opponent_fighting) // 从本服玩家取信息
		{
			p_opponent_info.camp = user_node->camp;
			p_opponent_info.prof = user_node->prof;
			p_opponent_info.sex = user_node->sex;
			p_opponent_info.random_name_index = -1;
			user_node->GetName(p_opponent_info.name);
			p_opponent_info.capability = user_node->capability;
		}
		else
		{
			int opponent_index = opponent_info.random_name_index;
			p_opponent_info.camp = opponent_index % (CAMP_TYPE_MAX - 1) + 1;
			p_opponent_info.prof = opponent_index % (PROF_TYPE_PROF_4 - 1) + 1; // 随机三个职业（第四个职业在ug01里无效，代码却没注释。。）
			p_opponent_info.sex = opponent_index % SEX_MAX;
			p_opponent_info.random_name_index = opponent_index;
			memset(p_opponent_info.name, 0, sizeof(p_opponent_info.name));
			p_opponent_info.capability = opponent_info.capability;
		}
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&fcl, sizeof(fcl));
}

void RoleFighting::ChallengeCheckReflush(Role* role, bool is_auto_reflush)
{
	if (NULL == role) return;

	if (!LOGIC_CONFIG->GetJoinLimitConfig().CanJoinOther(JLMT_OTHER_FIGHTING, role))
	{
		return;
	}

	// 若所有数据都未初始化状态，或者都已经挑战胜利，则自动刷新一次
	bool all_challenger_invalid = true;
	bool all_challenger_been_defeat = true;
	for (int i = 0; i < FIGHTING_CHALLENGE_OPPONENT_COUNT; ++i)
	{
		if (0 != m_param.challenge_opponent_lsit[i].random_uid || -1 != m_param.challenge_opponent_lsit[i].random_name_index)
		{
			all_challenger_invalid = false;
		}

		if (m_param.challenge_opponent_lsit[i].has_win <= 0)
		{
			all_challenger_been_defeat = false;
		}
	}

	if (all_challenger_invalid || all_challenger_been_defeat || is_auto_reflush)
	{
		this->ChallengeReflush(role);
	}
}

void RoleFighting::ChallengeReflush(Role* role)
{
	int role_capability = role->GetCapability()->GetTotalCapability();

	// 每一个对手对应需要的战力条件（跟自己的战力对比百分比）
	static int cap_con_min_list[FIGHTING_CHALLENGE_OPPONENT_COUNT] = { 110, 100,  95,  95 };
	static int cap_con_max_list[FIGHTING_CHALLENGE_OPPONENT_COUNT] = { 120, 110, 105, 105 };

	UserID user_id_list[PERSON_RANK_MAX_SIZE];
	int rank_count = RankManager::Instance().GetPersonRank()->GetRankTopUser(PERSON_RANK_TYPE_CAPABILITY_ALL, PERSON_RANK_MAX_SIZE / 2, user_id_list);

	int rank_index = 0;
	std::vector<int> uid_list;
	for (int opponent_index = 0; opponent_index < FIGHTING_CHALLENGE_OPPONENT_COUNT; ++opponent_index)
	{
		uid_list.clear(); // 继续遍历排行榜，找当前位置符合条件的对手
		for (; rank_index < rank_count && rank_index < PERSON_RANK_MAX_SIZE; ++rank_index)
		{
			if (role->GetUID() == UidToInt(user_id_list[rank_index])) break; // 若遍历到自己的排名，则跳出循环——不找战力比自己低的玩家作为对手
			
			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(user_id_list[rank_index]);
			if (NULL == user_node) continue;

			int cap_percent = int(100 * (user_node->capability * 1.0 / role_capability));
			if (cap_percent < cap_con_min_list[opponent_index] || cap_percent > cap_con_max_list[opponent_index]) continue;

			uid_list.push_back(UidToInt(user_id_list[rank_index]));
		}

		if (uid_list.size() > 1) // 超过一个候选人，则在候选人中随机抽一个
		{
			int uid = uid_list[RandomNum(int(uid_list.size()))];

			UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(uid);
			if (NULL == user_node) continue;

			FightingChellangeOpponentInfo &opponent_info = m_param.challenge_opponent_lsit[opponent_index];
			opponent_info.random_uid = uid;
			opponent_info.random_name_index = -1;
			opponent_info.has_win = 0;
			opponent_info.capability = user_node->capability;
		}
		else // 否则生成一个机器人
		{
			FightingChellangeOpponentInfo &opponent_info = m_param.challenge_opponent_lsit[opponent_index];
			opponent_info.random_uid = 0;
			opponent_info.random_name_index = RandomNum(SHRT_MAX); // 随机一个值就行，客户端求余映射到随机名字
			int rand_cap_per = RandomNum(cap_con_max_list[opponent_index] - cap_con_min_list[opponent_index]) + cap_con_min_list[opponent_index];
			opponent_info.has_win = 0;
			opponent_info.capability = int(role_capability * (rand_cap_per / 100.0));
		}
	}

	this->ChallengeSendBaseInfo(role);
	this->ChallengeSendList(role);
}

void RoleFighting::ChallengeBuyFightingTimes(Role* role)
{
	if (NULL == role) return;

	int vip_max_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_FIGHTING_DAY_BUY_CHALLENGE_TIMES);
	if (m_param.challenge_vip_buy_times >= vip_max_buy_times)
	{
		role->NoticeNum(errornum::EN_FIGHTING_VIP_BUY_LIMIT);
		return;
	}

	// 消耗
	if (!role->GetKnapsack()->GetMoney()->UseGold(LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().cf_buy_time_need_gold, "RoleFighting::ChallengeBuyFightingTimes"))
	{
		role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
		return;
	}
	++m_param.challenge_vip_buy_times;

	// 给予
	++m_param.challenge_left_times;

	this->ChallengeSendBaseInfo(role);
	this->OnDataChange();

	gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::ChallengeBuyFightingTimes user[%d %s] left_times[%d] vip_buy_times[%d]",
		role->GetUID(), role->GetName(), m_param.challenge_left_times, m_param.challenge_vip_buy_times);
}

void RoleFighting::ChallengeFightingReq(Role* role, short opponent_index)
{
	if (NULL == role || opponent_index < 0 || opponent_index >= FIGHTING_CHALLENGE_OPPONENT_COUNT)
	{
		return;
	}

	if (!role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}

	// 防止重复点击挑战，重复消耗次数
	if (CHALLENGE_TYPE_NONE != m_tmp_challenge_type)
	{
		return;
	}

	if (m_param.challenge_left_times <= 0)
	{
		role->NoticeNum(errornum::EN_FIGHTING_CHALLENGE_LEFT_TIMES_LIMIT);
		return;
	}

	FightingChellangeOpponentInfo &opponent_info = m_param.challenge_opponent_lsit[opponent_index];
	if (opponent_info.has_win > 0)
	{
		role->NoticeNum(errornum::EN_FIGHTING_CHALLENGE_ALEADY_WIN);
		return;
	}

	if (opponent_info.random_uid <= 0 && opponent_info.random_name_index < 0) // 对手不存在（理论上不可能）
	{
		return;
	}

	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();

	if (FIGHTING_CHALLENGE_DEFAULT_TIMES == m_param.challenge_left_times) // 低于默认次数则开始定时增加次数
	{
		m_next_add_challenge_timestamp = (unsigned int)EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().cf_restore_join_times_need_time_m * SECOND_PER_MIN;
	}

	// 埋好铺垫
	m_tmp_challenge_type = CHALLENGE_TYPE_FIGHTING;
	m_tmp_challenge_param1 = opponent_index;

	// 决斗场里面有个挑衅，不管成功与否都算一次 -- 刘琪
	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_FIGHTING_CHALLENGE);

	// 送入决斗场
	World::GetInstWorld()->GetSceneManager()->GoTo(role, other_cfg.cf_scene_id, role->GetUID(), other_cfg.cf_sponsor_pos);
}

void RoleFighting::OnChallengeResult(Role* role, bool is_win, bool is_skip)
{
	if (NULL == role)
	{
		return;
	}

	if (role->GetLevel() >= LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().cf_stop_level)
	{
		return;
	}

	if (!is_skip && (m_tmp_challenge_param1 < 0 || m_tmp_challenge_param1 >= FIGHTING_CHALLENGE_OPPONENT_COUNT))
	{
		return;
	}

	FightingChellangeOpponentInfo &opponent_info = m_param.challenge_opponent_lsit[m_tmp_challenge_param1];
	if (opponent_info.has_win > 0 && !is_skip)
	{
		return;
	}

	if (is_win)
	{
		const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();

		// 消耗
		if (!is_skip)
		{
			opponent_info.has_win = 1;
		}
		--m_param.challenge_left_times;

		// 给予
		int level_fact = role->GetLevel() / 100 + 1;
		role->AddExp(level_fact * other_cfg.cf_win_add_exp, "RoleFighting::OnChallengeResult", Role::EXP_ADD_REASON_DEFAULT);
		role->GetRoleShop()->AddChestShopMojing(other_cfg.cf_win_add_mojing, "RoleFighting::OnChallengeResult");
		role->GetKnapsack()->PutListOrMail(other_cfg.cf_wint_item, PUT_REASON_FIGHTING_CHALLENGE);
		int old_score = m_param.challenge_score;
		m_param.challenge_score += other_cfg.cf_win_add_jifen;

		this->ChallengeSendBaseInfo(role);
		//this->ChallengeSendList(role);
		this->OnDataChange();
		this->ChallengeCheckReflush(role);
		role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_KILL_ROLE_IN_FIGHTING, 1);

		gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::OnChallengeResult user[%d %s] add_exp[%lld] win_add_mojing[%d] score[%d -> %d] item_1[%d] item_2[%d] item_3[%d]",
			role->GetUID(), role->GetName(), other_cfg.cf_win_add_exp, other_cfg.cf_win_add_mojing, old_score, m_param.challenge_score,
			other_cfg.cf_wint_item[0].item_id, other_cfg.cf_wint_item[1].item_id, other_cfg.cf_wint_item[2].item_id);
	}

	this->SendFightingResult(role, is_win, 0, NULL, is_skip);

	m_tmp_challenge_param1 = -1;
}

void RoleFighting::ConstructAppearance(RoleAppearance &appearance) const
{
	appearance.wuqi_id = 0;
	appearance.shizhuang_wuqi = 0;
	appearance.shizhuang_body = 0;
	appearance.mount_used_imageid = 0;
	appearance.wing_used_imageid = 1;
	appearance.halo_used_imageid = 2;
	appearance.shengong_used_imageid = 2;
	appearance.shenyi_used_imageid = 2;
	appearance.xiannvshouhu_imageid = 2;
	appearance.jingling_guanghuan_imageid = 2;
	appearance.jingling_fazhen_imageid = 2;
	appearance.fight_mount_used_imageid = 2;
	appearance.zhibao_used_imageid = 2;
	appearance.use_eternity_level = 2;
	appearance.footprint_used_imageid = 0;
	appearance.cloak_used_imageid = 0;
}

void RoleFighting::SendFightingResult(Role* role, bool is_win, long long add_exp, ItemConfigData *item_list, bool is_skip)
{
	if (NULL == role) return;

	static Protocol::SCFightingResultNotify frn;
	frn.fighting_type = m_tmp_challenge_type;
	frn.is_win = is_win ? 1 : 0;

	if (NULL != item_list)
	for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
	{
		frn.reward_item_list[i].item_id = item_list[i].item_id;
		frn.reward_item_list[i].num = item_list[i].num;
	}

	frn.reward_exp = add_exp;
	if (is_skip)
	{
		frn.is_skip = 1;
	}
	else
	{
		frn.is_skip = 0;
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&frn, sizeof(frn));
}

void RoleFighting::RobingNotify(int type, char quality, Role* rober_role, UserCacheNode *node_cache, int random_index) const
{
	static Protocol::SCFightingRobingNotify frn;
	frn.type = type;
	frn.quality = quality;
	rober_role->GetName(frn.rober_name);

	if (NULL != node_cache)
	{
		node_cache->GetName(frn.been_rob_name);
	}
	else if (random_index >= 0)
	{
		LOGIC_CONFIG->GetNameCreateCfg().GetRandName(frn.been_rob_name, random_index, random_index % 2 <= 0);
	}
	else
	{
		return; // 两个参数都无效，无法获取到被抢劫者名字，不理
	}

	World::GetInstWorld()->SendToAllGateway((char*)&frn, sizeof(frn));
}

void RoleFighting::BeenRobCheckNotify(int type) const
{
	Role *role = this->GetRole();
	if (NULL == role) return;

	if (0 == type) // 挖矿
	{
		if (m_param.mining_been_rob_need_notify <= 0) return;
	}
	else if (1 == type) // 航海
	{
		if (m_param.sailing_been_rob_need_notify <= 0) return;
	}
	else
	{
		return;
	}

	static Protocol::SCFightingBeenRobNotify fbrn;
	fbrn.type = type;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&fbrn, sizeof(fbrn));
}

void RoleFighting::UpdateFakeFigting(unsigned int now_second)
{
	UserCacheNode *u_cache = this->GetUserCache();
	if (NULL == u_cache) return;

	// 机器人的战力范围依赖角色
	int min_cap = int(u_cache->capability * 0.8);
	int max_cap = int(u_cache->capability * 1.2);

	FightingConfig &f_cfg = LOGIC_CONFIG->GetFightingConfig();
	for (int i = 0; i < FIGHTING_FAKE_MINING_SAILING_DATA_COUNT; ++i)
	{
		// 挖矿机器人
		FightingFakeMiningSailingData &fake_mining = m_param.mining_fake_list[i];
		unsigned int delay_tiime = RandomNum(SECOND_PER_MIN / 2);		// 故意延迟一段时间后才帮机器人收矿/换新的机器人挖矿
		if (now_second > fake_mining.end_time + delay_tiime)
		{
			fake_mining.random_index = RandomNum(SHRT_MAX);
			//fake_mining.cur_quality = RandomNum(f_cfg.GetMineMaxQuality() + 1);
			fake_mining.cur_quality = LOGIC_CONFIG->GetFightingConfig().MiningGetRandomRobotQuality();
			fake_mining.cur_been_rob_times = 0;
			fake_mining.capability = RandomNum(min_cap, max_cap);
			fake_mining.end_time = now_second + f_cfg.GetOtherCfg().dm_cost_time_m * SECOND_PER_MIN;
		}

		// 航海机器人。代码跟上面挖矿一样，改动时记得一起改动，除非策划要求不一样
		FightingFakeMiningSailingData &fake_sailing = m_param.sailing_fake_list[i];
		delay_tiime = RandomNum(SECOND_PER_MIN / 2);
		if (now_second > fake_sailing.end_time + delay_tiime)
		{
			fake_sailing.random_index = RandomNum(SHRT_MAX);
			//fake_sailing.cur_quality = RandomNum(f_cfg.GetSailingMaxQuality() + 1);
			fake_sailing.cur_quality = LOGIC_CONFIG->GetFightingConfig().SailingGetRandomRobotQuality();
			fake_sailing.cur_been_rob_times = 0;
			fake_sailing.capability = RandomNum(min_cap, max_cap);
			fake_sailing.end_time = now_second + f_cfg.GetOtherCfg().dm_cost_time_m * SECOND_PER_MIN;
		}
	}
}

void RoleFighting::SailingFillInfo(Protocol::SCFightingSailingList& fml, int watcher_role_id) const
{
	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();

	if (m_param.sailing_cur_been_rob_times >= other_cfg.sl_been_rob_times)
	{
		return;
	}

	if ((unsigned int)EngineAdapter::Instance().Time() > m_param.sailing_end_time)
	{
		return;
	}

	// 推算出开始挖矿时间，用来判断在这个时间后，查看信息的玩家有无掠夺过此矿
	unsigned int sailing_start_time = m_param.sailing_end_time - LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().sl_cost_time_m * SECOND_PER_MIN;
	for (int i = 0; i < FIGHTING_MAX_ROB_HISTORY_COUNT; ++i)
	{
		const FightingRobHistoryItem &history_item = m_param.sailing_been_rob_list[i];
		if (history_item.rober_uid == watcher_role_id && history_item.rob_time > sailing_start_time)
		{
			return;
		}
	}

	if (m_param.sailing_end_time > 0 && m_param.sailing_cur_quality >= 0 && m_param.sailing_cur_quality <= LOGIC_CONFIG->GetFightingConfig().GetSailingMaxQuality())
	{
		UserCacheNode *user_node = this->GetUserCache();
		if (NULL != user_node)
		{
			Protocol::SCFightingSailingList::ItemInfo &item = fml.sailing_list[fml.sailing_count++];
			item.cur_quality = m_param.sailing_cur_quality;
			item.owner_uid = m_role_id;
			item.random_index = -1;
			item.robot_index = -1;
			item.sailing_end_time = m_param.sailing_end_time;

			item.owner_sex = user_node->sex;
			item.owner_prof = user_node->prof;
			item.owner_capability = user_node->capability;
			item.owner_level = user_node->level;
			user_node->GetName(item.owner_name);
		}
	}
}

void RoleFighting::SailingSendListInfo(Role *role) const
{
	if (NULL == role) return;

	static Protocol::SCFightingSailingList fml;
	fml.sailing_count = 0;

	const GlobalRoleManager::GlobalRoleMap &global_role_map = GLOBAL_ROLE_MGR.GetGlobalRoleMap();
	for (GlobalRoleManager::GlobalRoleMap::const_iterator con_it = global_role_map.begin(); con_it != global_role_map.end() && fml.sailing_count < Protocol::SCFightingSailingList::MAX_NOTIFY_COUNT; ++con_it)
	{
		con_it->second->GetRoleFighting()->SailingFillInfo(fml, m_role_id);
	}

	if (fml.sailing_count < 8) // 少于指定数量的人航海，则把机器人矿点暴露给玩家
	{
		for (int i = 0; i < FIGHTING_FAKE_MINING_SAILING_DATA_COUNT && fml.sailing_count < Protocol::SCFightingSailingList::MAX_NOTIFY_COUNT; ++i)
		{
			const FightingFakeMiningSailingData &fake_sailing = m_param.sailing_fake_list[i];
			if (fake_sailing.random_index < 0 || fake_sailing.cur_been_rob_times > 0) continue;

			Protocol::SCFightingSailingList::ItemInfo &item = fml.sailing_list[fml.sailing_count++];
			item.cur_quality = fake_sailing.cur_quality;
			item.owner_uid = 0;
			item.random_index = fake_sailing.random_index;
			item.robot_index = i;
			item.sailing_end_time = fake_sailing.end_time;
			item.owner_sex = fake_sailing.random_index % SEX_MAX;
			item.owner_prof = fake_sailing.random_index % (PROF_TYPE_PROF_4 - 1) + 1; // 随机三个职业（第四个职业在ug01里无效，代码却没注释。。）
			item.owner_capability = fake_sailing.capability;
			item.owner_level = role->GetLevel();
			//F_STRNCPY(item.owner_name, "SOS", sizeof(GameName)); // 机器人的名字不要用服务端下发的，用fake_mining.random_index去随机名字列表里找
		}
	}

	int send_len = sizeof(fml) - (Protocol::SCFightingSailingList::MAX_NOTIFY_COUNT - fml.sailing_count) * sizeof(fml.sailing_list[0]);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&fml, send_len);
}

void RoleFighting::SailingSendBeenRobInfo(Role *role)
{
	if (NULL == role) return;

	static Protocol::SCFightingSailingBeenRobList fmbrl;
	for (int rob_index = 0; rob_index < FIGHTING_MAX_ROB_HISTORY_COUNT; ++rob_index)
	{
		FightingRobHistoryItem &rob_item = m_param.sailing_been_rob_list[rob_index];
		UserCacheNode *rober_cache = UserCacheManager::Instance().GetUserNode(rob_item.rober_uid);
		if (NULL != rober_cache)
		{
			rober_cache->GetName(fmbrl.rob_list[rob_index].rober_name);
			fmbrl.rob_list[rob_index].rob_time = rob_item.rob_time;
			fmbrl.rob_list[rob_index].has_revenge = rob_item.has_revenge;
			fmbrl.rob_list[rob_index].real_index = rob_index;
			fmbrl.rob_list[rob_index].cur_quality = rob_item.quality;
			fmbrl.rob_list[rob_index].rober_sex = rober_cache->sex;
			fmbrl.rob_list[rob_index].rober_prof = rober_cache->prof;
			fmbrl.rob_list[rob_index].rober_capability = rober_cache->capability;
		}
		else
		{
			// 无效uid说明此条记录无效，设置一下无效值，客户端就不会显示
			fmbrl.rob_list[rob_index].rob_time = 0;
			fmbrl.rob_list[rob_index].real_index = -1;
		}
	}
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&fmbrl, sizeof(fmbrl));
}

void RoleFighting::SailingReflush(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	int total_times = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().sl_day_times + m_param.sailing_today_buy_times;
	if (m_param.sailing_today_times >= total_times)
	{
		role->NoticeNum(errornum::EN_FIGHTING_SAILING_TIMES_LIMIT);
		return;
	}

	const FightingSailingRewardCfg *figting_cfg = LOGIC_CONFIG->GetFightingConfig().GetSailingCfg(m_param.sailing_cur_quality);
	if (NULL == figting_cfg)
	{
		return;
	}

	const FightingSailingRewardCfg *next_figting_cfg = LOGIC_CONFIG->GetFightingConfig().GetSailingCfg(m_param.sailing_cur_quality + 1);
	if (NULL == next_figting_cfg)
	{
		role->NoticeNum(errornum::EN_FIGHTING_SAILING_QUALITY_FULL);
		return;
	}

	// 消耗
	if (!role->GetKnapsack()->GetMoney()->UseGold(figting_cfg->consume_gold, "RoleFighting::SailingReflush"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	int old_quality = m_param.sailing_cur_quality;
	// 给予
	if (RandomNum(100) < figting_cfg->upgrade_rate)
	{
		++m_param.sailing_cur_quality;
		this->SailingSendBaseInfo(role);
		this->OnDataChange();
	}
	else
	{
		role->NoticeNum(errornum::EN_FIGHTING_SAILING_REFLUSH_FAIL);
	}

	gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::SailingReflush user[%d %s] consume_gold[%d] quality[ %d -> %d ]", role->GetUID(), role->GetName(), figting_cfg->consume_gold, old_quality, m_param.sailing_cur_quality);
}

void RoleFighting::SailingSendBaseInfo(Role *role) const
{
	if (NULL == role) return;

	static Protocol::SCFightingSailingBaseInfo fmbi;
	fmbi.sailing_quality= m_param.sailing_cur_quality;
	fmbi.sailing_end_time = m_param.sailing_end_time;
	fmbi.today_sailing_times = m_param.sailing_today_times;
	fmbi.today_buy_times = m_param.sailing_today_buy_times;
	fmbi.today_rob_times = m_param.sailing_today_rob_times;
	fmbi.sailing_been_rob_times = m_param.sailing_cur_been_rob_times;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&fmbi, sizeof(fmbi));
}

void RoleFighting::SailingStart(Role* role)
{
	if (NULL == role) return;

	if (m_param.sailing_end_time > 0)
	{
		role->NoticeNum(errornum::EN_FIGHTING_SAILING_ALEADY);
		return;
	}

	int total_times = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().sl_day_times + m_param.sailing_today_buy_times;
	if (m_param.sailing_today_times >= total_times)
	{
		role->NoticeNum(errornum::EN_FIGHTING_SAILING_TIMES_LIMIT);
		return;
	}
	
	// 消耗
	++m_param.sailing_today_times;

	// 给予
	m_param.sailing_cur_been_rob_times = 0;
	m_param.sailing_end_time = (unsigned int)EngineAdapter::Instance().Time() + LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().sl_cost_time_m * SECOND_PER_MIN;

	this->SailingSendBaseInfo(role);
	this->SailingSendListInfo(role);
	this->OnDataChange();
	role->GetMagicalPreciousManager()->AddConditionTimes(CONDITION_TYPE_SAILING_NUM, 1);
	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_SAILING);

	gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::SailingStart user[%d %s] sailing_today_times[%d] end_time[%d]", role->GetUID(), role->GetName(), m_param.sailing_today_times, m_param.sailing_end_time);
}

void RoleFighting::SailingBuyTimes(Role* role)
{
	if (NULL == role) return;

	int vip_max_buy_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_FIGHTING_SAILING_BUY_TIMES);
	if (m_param.sailing_today_buy_times >= vip_max_buy_times)
	{
		role->NoticeNum(errornum::EN_FIGHTING_VIP_BUY_LIMIT);
		return;
	}
	
	// 消耗
	if (!role->GetKnapsack()->GetMoney()->UseGold(LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().sl_buy_time_need_gold, "RoleFighting::SailingBuyTimes"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 给予
	++m_param.sailing_today_buy_times;

	this->SailingSendBaseInfo(role);
	this->OnDataChange();

	gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::SailingBuyTimes user[%d %s] sailing_today_buy_times[%d] consume_gold[%d]", role->GetUID(), role->GetName(), m_param.sailing_today_buy_times, LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().sl_buy_time_need_gold);
}

void RoleFighting::SailingRob(Role* role, int uid)
{
	if (NULL == role || uid == m_role_id) return;

	if (!role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}

	// 防止重复点击挑战，重复消耗次数
	if (CHALLENGE_TYPE_NONE != m_tmp_challenge_type)
	{
		return;
	}

	RoleFighting *targert_fighting = GlobalRoleManager::Instance().GetRoleFighting(uid);
	if (NULL == targert_fighting) return;

	FightingParam &target_param = targert_fighting->m_param;

	if (target_param.sailing_end_time <= 0)
	{
		return;
	}

	if ((unsigned int)EngineAdapter::Instance().Time() > target_param.sailing_end_time)
	{
		role->NoticeNum(errornum::EN_FIGHTING_SAILING_TIME_END);
		return;
	}

	// 推算出开始挖矿时间，用来判断在这个时间后，当前玩家有无掠夺过此矿
	unsigned int sailing_start_time = target_param.sailing_end_time - LOGIC_CONFIG->GetFightingConfig().GetOtherCfg().sl_cost_time_m * SECOND_PER_MIN;
	for (int i = 0; i < FIGHTING_MAX_ROB_HISTORY_COUNT; ++i)
	{
		FightingRobHistoryItem &history_item = target_param.sailing_been_rob_list[i];
		if (history_item.rober_uid == m_role_id && history_item.rob_time > sailing_start_time)
		{
			role->NoticeNum(errornum::EN_FIGHTING_SAILING_ALEADY_ROB);
			return;
		}
	}

	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
	if (target_param.sailing_cur_been_rob_times >= other_cfg.sl_been_rob_times)
	{
		role->NoticeNum(errornum::EN_FIGHTING_SAILING_BEEN_ROB_LIMIT);
		return;
	}

	if (m_param.sailing_today_rob_times >= other_cfg.sl_rob_times)
	{
		role->NoticeNum(errornum::EN_FIGHTING_SAILING_ROB_LIMIT);
		return;
	}

	// 消耗次数
	++m_param.sailing_today_rob_times;

	// 埋好铺垫
	m_tmp_challenge_type = CHALLENGE_TYPE_SAILING_ROB;
	m_tmp_challenge_param1 = uid;

	this->SailingSendBaseInfo(role);
	this->OnDataChange();

	// 送入决斗场
	World::GetInstWorld()->GetSceneManager()->GoTo(role, other_cfg.sl_scene_id, role->GetUID(), other_cfg.sl_sponsor_pos);
}

void RoleFighting::SailingOnRobResult(Role* role, bool is_win)
{
	RoleFighting *targert_fighting = GlobalRoleManager::Instance().GetRoleFighting(m_tmp_challenge_param1);
	if (NULL == targert_fighting) return;

	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
	FightingParam &target_param = targert_fighting->m_param;
	if (target_param.sailing_cur_been_rob_times >= other_cfg.sl_been_rob_times)
	{
		return;
	}

	const FightingSailingRewardCfg *figting_cfg = LOGIC_CONFIG->GetFightingConfig().GetSailingCfg(target_param.sailing_cur_quality);
	if (NULL == figting_cfg)
	{
		return;
	}

	long long add_exp = 0;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	if (is_win)
	{
		target_param.sailing_been_rob_last_index = (target_param.sailing_been_rob_last_index + 1) % FIGHTING_MAX_ROB_HISTORY_COUNT;
		FightingRobHistoryItem &rob_history_item = target_param.sailing_been_rob_list[int(target_param.sailing_been_rob_last_index)];

		// 增加抢夺记录
		rob_history_item.quality = target_param.sailing_cur_quality;
		rob_history_item.rober_uid = m_role_id;
		rob_history_item.rob_time = (unsigned int)EngineAdapter::Instance().Time();
		rob_history_item.has_revenge = 0;
		++target_param.sailing_cur_been_rob_times;

		// 给予抢夺奖励 
		double reward_fect = other_cfg.sl_rob_reward_rate / 100.0;
		if (reward_fect < 0)
		{
			reward_fect = 0;
		}

		UserCacheNode *node = UserCacheManager::Instance().GetUserNode(m_tmp_challenge_param1);
		if (NULL == node)
		{
			return;
		}

		int level_fact = role->GetLevel() + 50;

		add_exp = (long long)(level_fact * reward_fect * figting_cfg->reward_exp);
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < figting_cfg->reward_item_count; ++i)
		{
			reward_item_list[i] = figting_cfg->reward_item_list[i];
			//reward_item_list[i].num = int(reward_item_list[i].num * reward_fect);
			reward_item_list[i].num = figting_cfg->rob_get_item_count;
			if (reward_item_list[i].num > 0)
			{
				role->GetKnapsack()->Put(reward_item_list[i], PUT_REASON_FIGHTING_MININE_ROB);
			}
		}
		role->AddExp(add_exp, "RoleFighting::SailingOnRobResult", Role::EXP_ADD_REASON_DEFAULT);

		if (role->GetLevel() >= other_cfg.sl_jinghua_reward_open_level)
		{
			int reward_shenge_jinghua = static_cast<int>(reward_fect * figting_cfg->shenge_jinghua);
			if (reward_shenge_jinghua > 0)
			{
				role->GetShengeSystem()->AddMarrowScore(reward_shenge_jinghua, "SailingRobAddMarrowScore");
			}
		}

		target_param.sailing_been_rob_need_notify = 1;
		Role *target_role = targert_fighting->GetRole();
		if (NULL != target_role)
		{
			this->MiningSendBaseInfo(target_role);
		}
		targert_fighting->BeenRobCheckNotify(1);

		this->RobingNotify(1, target_param.mining_cur_quality, role, node);
	}

	this->SendFightingResult(role, is_win, add_exp, reward_item_list);

	m_tmp_challenge_param1 = 0;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_SAILING_ROB);

	std::string item_list_str;
	ItemConfigDataListLog(item_list_str, figting_cfg->reward_item_count, reward_item_list);
	gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::SailingOnRobResult user[%d %s] is_win[%d] add_exp[%lld] item_list[%s]",
		role->GetUID(), role->GetName(), is_win ? 1 : 0, add_exp, item_list_str.c_str());
}

void RoleFighting::SailingRobRobot(class Role* role, int index)
{
	if (NULL == role || index < 0 || index >= FIGHTING_FAKE_MINING_SAILING_DATA_COUNT) return;

	if (!role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}

	// 防止重复点击挑战，重复消耗次数
	if (CHALLENGE_TYPE_NONE != m_tmp_challenge_type)
	{
		return;
	}

	FightingFakeMiningSailingData &fake_item = m_param.sailing_fake_list[index];

	if (fake_item.end_time <= 0)
	{
		return;
	}

	if (fake_item.cur_been_rob_times > 0)
	{
		role->NoticeNum(errornum::EN_FIGHTING_SAILING_ALEADY_ROB);
		return;
	}

	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
	if (m_param.sailing_today_rob_times >= other_cfg.sl_rob_times)
	{
		role->NoticeNum(errornum::EN_FIGHTING_SAILING_ROB_LIMIT);
		return;
	}

	// 消耗次数
	++m_param.sailing_today_rob_times;

	// 埋好铺垫
	m_tmp_challenge_type = CHALLENGE_TYPE_SAILING_ROB_ROBOT;
	m_tmp_challenge_param1 = index;

	this->SailingSendBaseInfo(role);
	this->OnDataChange();

	// 送入决斗场
	World::GetInstWorld()->GetSceneManager()->GoTo(role, other_cfg.sl_scene_id, role->GetUID(), other_cfg.sl_sponsor_pos);
}

void RoleFighting::SailingOnRobRobotResult(Role* role, bool is_win)
{
	if (NULL == role || m_tmp_challenge_param1 < 0 || m_tmp_challenge_param1 >= FIGHTING_FAKE_MINING_SAILING_DATA_COUNT) return;

	FightingFakeMiningSailingData &fake_item = m_param.sailing_fake_list[m_tmp_challenge_param1];

	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
	if (fake_item.cur_been_rob_times >= other_cfg.sl_been_rob_times)
	{
		return;
	}

	const FightingSailingRewardCfg *figting_cfg = LOGIC_CONFIG->GetFightingConfig().GetSailingCfg(fake_item.cur_quality);
	if (NULL == figting_cfg)
	{
		return;
	}

	// 消耗
	if (fake_item.cur_been_rob_times <= 0)
	{
		++fake_item.cur_been_rob_times;
	}

	long long add_exp = 0;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	if (is_win)
	{
		// 给予抢夺奖励 
		double reward_fect = other_cfg.sl_rob_reward_rate / 100.0;
		if (reward_fect < 0)
		{
			reward_fect = 0;
		}

		int level_fact = role->GetLevel() + 50;
		add_exp = (long long)(level_fact * reward_fect * figting_cfg->reward_exp);
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < figting_cfg->reward_item_count; ++i)
		{
			reward_item_list[i] = figting_cfg->reward_item_list[i];
			//reward_item_list[i].num = int(reward_item_list[i].num * reward_fect);
			reward_item_list[i].num = figting_cfg->rob_get_item_count;
			if (reward_item_list[i].num > 0)
			{
				role->GetKnapsack()->Put(reward_item_list[i], PUT_REASON_FIGHTING_MININE_ROB);
			}
		}
		role->AddExp(add_exp, "RoleFighting::SailingOnRobRobotResult", Role::EXP_ADD_REASON_DEFAULT);

		if (role->GetLevel() >= other_cfg.sl_jinghua_reward_open_level)
		{
			int reward_shenge_jinghua = static_cast<int>(reward_fect * figting_cfg->shenge_jinghua);
			if (reward_shenge_jinghua > 0)
			{
				role->GetShengeSystem()->AddMarrowScore(reward_shenge_jinghua, "SailingRobRobotAddMarrowScore");
			}
		}

		this->RobingNotify(1, fake_item.cur_quality, role, NULL, fake_item.random_index);
	}

	this->SendFightingResult(role, is_win, add_exp, reward_item_list);

	m_tmp_challenge_param1 = 0;

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_SAILING_ROB);

	std::string item_list_str;
	ItemConfigDataListLog(item_list_str, figting_cfg->reward_item_count, reward_item_list);
	gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::SailingOnRobRobotResult user[%d %s] robot_index[%d] is_win[%d] add_exp[%lld] item_list[%s]",
		role->GetUID(), role->GetName(), m_tmp_challenge_param1, is_win ? 1 : 0, add_exp, item_list_str.c_str());
}

void RoleFighting::SailingRevenge(Role* role, short been_rob_list_index)
{
	if (NULL == role || been_rob_list_index < 0 || been_rob_list_index >= FIGHTING_MAX_ROB_HISTORY_COUNT)
	{
		return;
	}

	if (!role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}
	
	// 防止重复点击挑战，重复消耗次数
	if (CHALLENGE_TYPE_NONE != m_tmp_challenge_type)
	{
		return;
	}

	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
	FightingRobHistoryItem &history_item = m_param.sailing_been_rob_list[been_rob_list_index];
	if (history_item.has_revenge > 0) // 已经复仇过，不理
	{
		return;
	}

	if (m_param.sailing_today_rob_times >= other_cfg.sl_rob_times)
	{
		role->NoticeNum(errornum::EN_FIGHTING_SAILING_ROB_LIMIT);
		return;
	}

	// 埋好铺垫
	m_tmp_challenge_type = CHALLENGE_TYPE_SAILING_REVENGE;
	m_tmp_challenge_param1 = history_item.rober_uid;
	m_tmp_challenge_param2 = been_rob_list_index;

	// 送入决斗场
	World::GetInstWorld()->GetSceneManager()->GoTo(role, other_cfg.sl_scene_id, role->GetUID(), other_cfg.sl_sponsor_pos);
}

void RoleFighting::SailingOnRevengeResult(Role* role, bool is_win)
{
	if (NULL == role || m_tmp_challenge_param2 < 0 || m_tmp_challenge_param2 >= FIGHTING_MAX_ROB_HISTORY_COUNT)
	{
		return;
	}

	const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
	FightingRobHistoryItem *history_item = &m_param.sailing_been_rob_list[m_tmp_challenge_param2];

	// 找到的复仇列表对应玩家名不是当前对战玩家，说明有状态冲突，需要重新遍历抢劫列表找到对应玩家ID
	if (history_item->rober_uid != m_tmp_challenge_param1)
	{
		history_item = NULL;
		for (int i = 0; i < FIGHTING_MAX_ROB_HISTORY_COUNT; ++i)
		{
			if (m_param.sailing_been_rob_list[i].rober_uid == m_tmp_challenge_param1)
			{
				history_item = &m_param.sailing_been_rob_list[i];
				break;
			}
		}

		if (NULL == history_item)
		{
			return;
		}
	}

	if (history_item->has_revenge > 0) // 已经复仇过，不理
	{
		return;
	}

	const FightingSailingRewardCfg *figting_cfg = LOGIC_CONFIG->GetFightingConfig().GetSailingCfg(m_param.sailing_cur_quality);
	if (NULL == figting_cfg)
	{
		return;
	}

	// 给予复仇奖励 
	long long add_exp = 0;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	if (is_win)
	{
		// 复仇成功，消耗抢劫次数
		++m_param.sailing_today_rob_times;
		this->SailingSendBaseInfo(role);

		history_item->has_revenge = 1;
		double reward_fect = 2 * other_cfg.sl_rob_reward_rate / 100.0;
		if (reward_fect < 0)
		{
			reward_fect = 0;
		}

		int level_fact = role->GetLevel() + 50;
		add_exp = (long long)(level_fact * reward_fect * figting_cfg->reward_exp);
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < figting_cfg->reward_item_count; ++i)
		{
			ItemConfigData &reward_item = reward_item_list[i];
			reward_item = figting_cfg->reward_item_list[i];
			//reward_item.num = int(reward_item_list[i].num * reward_fect);
			reward_item.num = figting_cfg->rob_get_item_count * 2;
			if (reward_item.num > 0)
			{
				role->GetKnapsack()->Put(reward_item, PUT_REASON_FIGHTING_SAILINE_REVENGE);
			}
		}
		role->AddExp(add_exp, "RoleFighting::SailingOnRobResult", Role::EXP_ADD_REASON_DEFAULT);


		if (role->GetLevel() >= other_cfg.sl_jinghua_reward_open_level)
		{
			int reward_shenge_jinghua = static_cast<int>(reward_fect * figting_cfg->shenge_jinghua);
			if (reward_shenge_jinghua > 0)
			{
				role->GetShengeSystem()->AddMarrowScore(reward_shenge_jinghua, "SailingRevengeAddMarrowScore");
			}
		}
	}

	this->SendFightingResult(role, is_win, add_exp, reward_item_list);

	m_tmp_challenge_param1 = 0;
	m_tmp_challenge_param2 = 0;

	std::string item_list_str;
	ItemConfigDataListLog(item_list_str, figting_cfg->reward_item_count, reward_item_list);
	gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::SailingOnRevengeResult user[%d %s] is_win[%d] add_exp[%lld] item_list[%s]",
		role->GetUID(), role->GetName(), is_win ? 1 : 0, add_exp, item_list_str.c_str());
}

void RoleFighting::SailingFetchReward(Role* role, bool is_skip)
{
	if (NULL == role) return;

	if (!is_skip)
	{
		unsigned int now_second = (unsigned int)EngineAdapter::Instance().Time();

		if (m_param.sailing_end_time <= 0 || now_second < m_param.sailing_end_time)
		{
			role->NoticeNum(errornum::EN_FIGHTING_SAILING_FETCH_TIME_LIMIT);
			return;
		}
	}

	long long add_exp = 0;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	const FightingSailingRewardCfg* figting_cfg = LOGIC_CONFIG->GetFightingConfig().GetSailingCfg(m_param.sailing_cur_quality);
	if (NULL != figting_cfg)
	{
		const FightingOtherConfig &other_cfg = LOGIC_CONFIG->GetFightingConfig().GetOtherCfg();
		double reward_fect = 1 - (m_param.sailing_cur_been_rob_times * other_cfg.sl_rob_reward_rate / 100.0);
		if (reward_fect < 0)
		{
			reward_fect = 0;
		}

		int level_fact = role->GetLevel() + 50;
		add_exp = (long long)(level_fact * reward_fect * figting_cfg->reward_exp);
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM && i < figting_cfg->reward_item_count; ++i)
		{
			ItemConfigData &reward_item = reward_item_list[i];
			reward_item = figting_cfg->reward_item_list[i];
			//reward_item.num = int(reward_item_list[i].num * reward_fect);
			reward_item.num = int(reward_item_list[i].num - figting_cfg->rob_get_item_count * m_param.sailing_cur_been_rob_times);
			if (reward_item.num > 0)
			{
				role->GetKnapsack()->Put(reward_item, PUT_REASON_FIGHTING_SAILINE_REVENGE);
			}
		}
		role->AddExp(add_exp, "RoleFighting::SailingOnRobResult", Role::EXP_ADD_REASON_DEFAULT);

		if (role->GetLevel() >= other_cfg.sl_jinghua_reward_open_level && figting_cfg->shenge_jinghua > 0)
		{
			role->GetShengeSystem()->AddMarrowScore(figting_cfg->shenge_jinghua, "SailingAddMarrowScore");
		}
	}

	m_param.sailing_end_time = 0;
	m_param.sailing_cur_quality = 0;
	m_param.sailing_cur_been_rob_times = 0;

	this->SailingSendBaseInfo(role);
	this->SailingSendListInfo(role);
	this->OnDataChange();

	std::string item_list_str;
	ItemConfigDataListLog(item_list_str, figting_cfg->reward_item_count, reward_item_list);
	gamelog::g_log_fighting.printf(LL_INFO, "RoleFighting::SailingFetchReward user[%d %s] add_exp[%lld] item_list[%s]",
		role->GetUID(), role->GetName(), add_exp, item_list_str.c_str());
}
