#ifndef __ROLE_STATUS_MANAGER_HPP__
#define __ROLE_STATUS_MANAGER_HPP__

#include "servercommon/servercommon.h"

#include "obj/otherobj/gatherobj.h"

#include "gamedef.h"

class Obj;
class Role;

class RoleStatusManager
{
public:
	enum ROLE_ACTION_STATUS
	{
		ROLE_ACTION_STATUS_INVALID = 0,

		ROLE_ACTION_STATUS_IDLE,		// 静止
		ROLE_ACTION_STATUS_MOVE,		// 移动
		ROLE_ACTION_STATUS_FIGHT,		// 战斗
		ROLE_ACTION_STATUS_GATHER,		// 采集
		ROLE_ACTION_STATUS_REST,		// 打坐

		ROLE_ACTION_STATUS_MAX
	};

	RoleStatusManager();
	~RoleStatusManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role);

	void Update(unsigned int now_second);

	int GetRoleActionStatus();

	bool CanMountOn();								// 能否上坐骑
	bool CanFightMountOn();							// 能否上战斗坐骑
	bool CanEnterFB(bool check_acitivty = true, bool notify = true);	// 能否进入fb
	bool CanGather(GatherObj *gather);				// 能否采集
	bool CanRest(bool is_notify = true);			// 能否打坐
	bool CanPerformSkill();							// 能否使用技能
	bool CanFight(ObjID target_id);					// 能否攻击
	bool CanSupplyEffect(int supply_type);			// 补给能否起效
	bool CanUseItem(ItemID item_id);				// 能否使用物品
	bool CanGuildBackStation(bool notify = true);	// 能否回营
	bool CanTransmit(bool is_notify = true);		// 能否远程传送
	bool CanFlyByShoe(int target_scene_id);			// 能否使用小飞鞋
	bool CanTransportTo(int target_scene_id);		// 能否通过传送阵传送
	bool CanAcceptSpecialTask(TaskID task_id);		// 能否接特殊任务
	bool CanDating(bool is_notify = true);			// 能否约会
	bool CanStartCross();							// 能否跨服

	bool CanChangeAttackMode(int attack_mode);		// 能否改攻击模式
	bool CanGuildSos();								// 能否发军团求救
	int GetGuildSosType();							// 获取军团求救类型
	bool CanRecordLastKillMeUserInfo();				// 能否记录最后杀我的玩家信息

	bool CanUpdateGiveMP();							// 能否定时恢复MP

	bool IsInFB();									// 是否在副本中
	bool IsInBoss();								// 是否在BOSS场景中
	bool IsHaveTeam();								// 是否有队伍

	void OnEnterFB();								// 进入副本
	void OnGetFollowNPC();							// 获得跟随NPC
	void OnAddHusongTask();							// 接受护送任务
	void OnFly();									// 飞行

	void OnFight();									// 发起战斗或者被打
	bool IsInFightStatus();
	bool IsHpSupplyRewardExtra();					// 血量补给是否可奖励额外的量

private:
	bool CanAttackModeEnterHelper(int target_scene_id);		// 当前战斗模式能否进入
	bool CheckOtherSceneCanFly(int scene_id);

	Role *m_role;
	unsigned long m_last_fight_time;				// 最后战斗状态的时间
	unsigned int m_fight_end_time;					// 退出战斗状态的时间戳

};

#endif // __ROLE_STATUS_HPP__

