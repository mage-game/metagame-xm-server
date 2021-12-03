#ifndef __SPECIAL_CROSS_TEAM_FB_HPP__
#define __SPECIAL_CROSS_TEAM_FB_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "config/fbconfig.hpp"
#include "servercommon/crossdef.hpp"

class SpecialCrossTeamFB : public SpecialLogic
{
public:
	SpecialCrossTeamFB(Scene *scene);
	~SpecialCrossTeamFB();

	virtual void Update(unsigned long interval, time_t now_second);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);	
	virtual bool OnTimeout();
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual bool SpecialCanMountOn(Role *role){return false;}

	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);

	virtual int GetSpecialParam(Role *role) { return m_user_count; }
	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);

	void OnFetchFinishReward();

protected:
	Role * GetRole(ObjID obj_id);

	virtual void OnAllMonsterDead();

	void OnFBFinish(bool is_pass);											// 副本结束
	void FBAutoGetReward();
	void SendFBInfo();
	void ReCalculateBuff();

	void SyncFBSceneLogicInfo(Role *role, long long dps);							// 副本信息
	void SyncFBRoomInfo(int option_type);

	struct FBUserInfo
	{
		FBUserInfo()
		{
			this->Reset();
		}

		void Reset()
		{
			obj_id = INVALID_OBJ_ID;
			user_id = INVALID_USER_ID;
			memset(user_name, 0, sizeof(user_name));
			DPS = 0;
		}

		ObjID obj_id;
		UserID user_id;
		GameName user_name;
		long long DPS;
		std::vector<ItemDataWrapper> pick_item_vec;
	};

	bool m_is_finish;														// 是否结束
	bool m_has_fetch_finish_reward;											// 是否领取通关奖励

	int m_user_count;
	FBUserInfo m_user_info[CROSS_TEAM_FB_ROLE_MAX];

	int m_normal_monster_kill_num;											// 击杀普通怪总数
	int m_boss_monster_kill_num;											// 击杀boss总数

	time_t m_auto_get_reward_timestamp;										// 自动领取报酬时间
	int m_fb_state;															// 副本状态

	time_t m_next_send_fb_info_timestamp;									// 定时刷新副本信息时间
	time_t m_next_update_time;
};

#endif // __SPECIAL_CROSS_TEAM_FB_HPP__


