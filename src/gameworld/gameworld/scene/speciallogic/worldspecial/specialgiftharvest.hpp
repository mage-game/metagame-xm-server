#ifndef _SPECIAL_GIFT_HARVEST_HPP_
#define _SPECIAL_GIFT_HARVEST_HPP_

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include <set>
#include "servercommon/crossdef.hpp"
#include "servercommon/serverdef.h"

class Role;

// 礼物收割 - 技能信息
struct GiftHarvestSkillItem
{
	GiftHarvestSkillItem() : skill_id(0), next_can_perform_time(0) {}

	bool Invalid() const { return 0 == skill_id; }
	void Reset() { skill_id = 0; next_can_perform_time = 0; }

	UInt16 skill_id;
	unsigned int next_can_perform_time;
};

// 礼物收割 - 玩家信息
struct GiftHarvestPlayerInfo
{
	GiftHarvestPlayerInfo() : uid(INVALID_USER_ID), cur_get_score(0), cur_get_score_times(0), sex(0), prof(0), avatar_timestamp(0)
	{
		memset(name, 0, sizeof(name));
	}

	UserID uid;
	GameName name;															// 角色名
	int cur_get_score;														// 当轮 获得的积分
	short cur_get_score_times;												// 当轮 获取积分的次数
	char sex;																// 性别
	char prof;																// 职业
	long long avatar_timestamp;												// 头像

	GiftHarvestSkillItem skill_list[GIFT_HARVEST_SKILL_CFG_MAX_COUNT];		// 角色在当前场景里的额外技能信息
};

// 礼物收割 - 怪物信息
struct GiftHarvestMonster
{
	GiftHarvestMonster() : monster_id(0), total_get_score(0), max_num(0), cur_num(0) {}

	int monster_id;															// 怪物id
	int total_get_score;													// 拉进区域获得积分
	int max_num;															// 最大数量
	int cur_num;															// 当前数量
};

class SpecialGiftHarvest : public SpecialLogic
{
	static const int PLAYER_FIRST_ENTER_ADD_SCORE = 500;					// 玩家第一次进入赠送积分

public:
	SpecialGiftHarvest(Scene *scene);
	virtual ~SpecialGiftHarvest();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnAddObj(Obj *obj);
	virtual void OnRemoveObj(Obj *obj);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialCanPerformSkill(Role *role) { return m_activity_open; }
	virtual bool SpecialCanSupplyEffect(Role *role, int supply_type) { return m_activity_open; }
	virtual int SpecialRecalcInjure(Character *attacker, Character *injurer, int injure);
	virtual bool PerformSkill(Role *role, int skill_index, const Posi &pos, ObjID target_id);
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool CanDestroy() { return false; }

	void OnActivityStatusChange(int activity_status, int round_staus);

	void OnActivityOpen();	// 活动开启
	void OnActivityClose();	// 活动结束

	void DrawJoinReward();	// 派发参与奖励
	void ReadyRound();		// 准备本轮
	void StartRound();		// 开始本轮
	void StopRound();		// 结束本轮

	bool CanEnter();		// 能否进入

	void SendPlayerInfoToPlayer(Role *role);	// 发送信息
	void SendPlayerInfoToPlayer(ObjID obj_id);	// 发送信息

	GiftHarvestPlayerInfo * GetPlayerInfo(ObjID obj_id);
	GiftHarvestPlayerInfo * GetPlayerInfo(UserID uid);

protected:

	Role * _GetRole(ObjID obj_id);

	void _FixMonsterNum(int monster_id);					// 稳定怪物数量
	void _ResetAllData();									// 重置所有数据（活动开关时）
	void _ResetRoundData();									// 重置本轮数据（开始下一轮时）
	void _SyncSkillInfo(Role *role, GiftHarvestSkillItem *skill_info);	// 发送技能信息

	bool m_activity_open;													// 活动开启标志
	bool m_cur_round_open;													// 轮次开启标志
	unsigned int m_last_check_rank_time;									// 上一次更新排行榜的时间戳
	unsigned int m_last_check_monster_time;									// 上一次区域检测的时间戳

	typedef std::map<UserID, GiftHarvestPlayerInfo> UserInfoMap;
	typedef std::map<UserID, GiftHarvestPlayerInfo>::iterator UserInfoMapIt;
	UserInfoMap m_user_info_cache;											// 玩家信息缓存，包括离线玩家

	typedef std::map<int, GiftHarvestMonster> MonsterInfoMap;
	typedef std::map<int, GiftHarvestMonster>::iterator MonsterInfoMapIt;
	MonsterInfoMap m_monster_info_map;										// 怪物信息
};


#endif // _SPECIAL_GIFT_HARVEST_HPP_

