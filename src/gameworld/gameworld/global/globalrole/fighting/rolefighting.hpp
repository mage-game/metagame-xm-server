#ifndef __RoleFighting_HPP__
#define __RoleFighting_HPP__

#include "global/globalrole/globalrole.hpp"
#include "servercommon/fightingdef.hpp"
#include "servercommon/internalprotocal/gglobalprotocal.h"

struct ChallengeParam;

namespace Protocol {
	class SCFightingSailingList;
	class SCFightingMiningList;
}

class RoleFighting : public IGlobalRoleItem
{
	static const int MINING_SEALING_UPDATE_FAKE_INTERVAL_S = 10;	// 机器人更新间隔

	enum ChallengeType
	{
		CHALLENGE_TYPE_NONE = 0,
		CHALLENGE_TYPE_MINING_ROB,						// 挖矿抢劫玩家，	param1 对手UID
		CHALLENGE_TYPE_MINING_ROB_ROBOT,				// 挖矿抢劫机器人，	param1 机器人index
		CHALLENGE_TYPE_MINING_REVENGE,					// 挖矿复仇，		param1 对手UID，param2 对应抢劫列表index
		CHALLENGE_TYPE_SAILING_ROB,						// 航海抢劫玩家，	param1 对手UID
		CHALLENGE_TYPE_SAILING_ROB_ROBOT,				// 航海抢劫机器人，	param1 机器人index
		CHALLENGE_TYPE_SAILING_REVENGE,					// 航海复仇，		param1 对手UID，param2 对应抢劫列表index
		CHALLENGE_TYPE_FIGHTING,						// 挑衅对战，		param1 对手下标
	};

public:
	RoleFighting(GlobalRole *g_role);
	virtual ~RoleFighting();

	virtual void Init(const FightingParam &little_pet_attr);
	virtual void GetAttr(FightingParam &little_pet_attr);

	void Update(unsigned int now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnRoleLogin(Role *role);
	void OnRoleLogout(Role *role);
	void OnRoleUpLevel(Role *role, int new_level);
	void OnHandleReq(short req_type, short param1, int param2);
	void OnFightingResult(bool is_win);
	bool ConstructOpponentParam(ChallengeParam &param, RoleAppearance &appearance) const;								// 构造当前对手的参数
//	const RoleAppearance &GetAppearance() const { return m_param.role_appearance_backup; }
	long long GetChallengeScore() const { return static_cast<long long>(m_param.challenge_score); }
	void OnGmReq(int type, Role *role);
	const Posi & GetCurOpponentPos() const;

	void OnSkipMining(Role *role, int quality);
	void OnSkipSailing(Role *role, int quality);
	void OnSkipChallenge(Role *role);

protected:
	void MiningFillInfo(Protocol::SCFightingMiningList& fml, int watcher_role_id) const;	// 把当前数据插入到协议内（不做越界检查，交给调用者保证）
	void MiningSendListInfo(Role *role) const;
	void MiningSendBeenRobInfo(Role *role);
	void MiningReflush(Role *role);
	void MiningSendBaseInfo(Role *role) const;
	void MiningStart(Role* role);
	void MiningBuyTimes(Role* role);
	void MiningRob(Role* role, int uid);
	void MiningOnRobResult(Role* role, bool is_win);
	void MiningRobRobot(Role* role, int index);
	void MiningOnRobRobotResult(Role* role, bool is_win);
	void MiningRevenge(Role* role, short been_rob_list_index);
	void MiningOnRevengeResult(Role* role, bool is_win);	
	void MiningFetchReward(Role* role, bool is_skip = false);

	void SailingFillInfo(Protocol::SCFightingSailingList& fml, int watcher_role_id) const;	// 把当前数据插入到协议内（不做越界检查，交给调用者保证）
	void SailingSendListInfo(Role *role) const;
	void SailingSendBeenRobInfo(Role *role);
	void SailingReflush(Role *role);
	void SailingSendBaseInfo(Role *role) const;
	void SailingStart(Role* role);
	void SailingBuyTimes(Role* role);
	void SailingRob(Role* role, int uid);
	void SailingOnRobResult(Role* role, bool is_win);
	void SailingRobRobot(Role* role, int index);
	void SailingOnRobRobotResult(Role* role, bool is_win);
	void SailingRevenge(Role* role, short been_rob_list_index);
	void SailingOnRevengeResult(Role* role, bool is_win);
	void SailingFetchReward(Role* role, bool is_skip = false);

	void ChallengeSendBaseInfo(Role *role) const;
	void ChallengeSendList(Role* role);
	void ChallengeCheckReflush(Role* role, bool is_auto_reflush = false);
	void ChallengeReflush(Role* role);
	void ChallengeBuyFightingTimes(Role* role);
	void ChallengeFightingReq(Role* role, short opponent_index);
	void OnChallengeResult(Role* role, bool is_win, bool is_skip = false);
	void ConstructAppearance(RoleAppearance &appearance) const;
	
	void SendFightingResult(Role* role, bool is_win, long long add_exp = 0, ItemConfigData *item_list = NULL, bool is_skip = false);
	void RobingNotify(int type, char quality, Role* rober_role, UserCacheNode *node_cache, int random_index = -1) const;
	void BeenRobCheckNotify(int type) const;												// 检查是否要通知玩家被抢劫了
	void UpdateFakeFigting(unsigned int now_second);

private:
	FightingParam m_param;

	int m_tmp_challenge_type;																// 当前挑战类型（临时存储，用于跟special那边交互）参考ChallengeType
	int m_tmp_challenge_param1;																// 战斗缓存参数，参考 ChallengeType
	int m_tmp_challenge_param2;

	unsigned int m_next_add_challenge_timestamp;												// 下一次增加挑战次数的时间戳（定时增加）
	unsigned int m_next_update_fake_fighting_times;											// 下一次更新机器人挖矿/航海信息的时间
};


#endif // __RoleFighting_HPP__
