#ifndef __SPECIAL_SHUIJING_HPP__
#define __SPECIAL_SHUIJING_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include <map>
#include <vector>

class SpecialShuiJing : public SpecialLogic
{
public:
	SpecialShuiJing(Scene *scene);
	virtual ~SpecialShuiJing();

	//参与者
	struct JoinerItem
	{
		JoinerItem () 
		{
			this->Reset();
		}

		void Reset()
		{
			uid = 0;
			lianzhan = 0;
			assist = 0;
			gather_shuijing_total_num = 0;
			gather_big_shuijing_total_num = 0;
			gather_diamond_big_shuijing_num = 0;
			gather_best_shuijing_count = 0;
			fetch_task_reward_flag = 0;
			is_brocast_gather_buff = false;
		}

		bool IsFetchTaskReward(int task_id)
		{
			return 0 != (fetch_task_reward_flag & (1 << task_id)); 
		}

		void SetFetchTaskRewardFlag(int task_id) 
		{
			fetch_task_reward_flag |= (1 << task_id); 
		}

		int uid;
		int lianzhan;
		int assist;
		int gather_shuijing_total_num;				// 采集任意水晶次数
		int gather_big_shuijing_total_num;			// 采集任意大水晶次数
		int gather_diamond_big_shuijing_num;		// 采集钻石大水晶次数
		int gather_best_shuijing_count;				// 采集至尊水晶次数
		unsigned int fetch_task_reward_flag;
		bool is_brocast_gather_buff;
		std::map<UserID, time_t> hurt_map;												// 伤害我的 用户列表	
	};

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnAddObj(Obj *obj);
	virtual void OnRemoveObj(Obj *obj);
	virtual bool CanDestroy() { return false; }
	virtual bool SpecialCanGather(Role *role, GatherObj *gather);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialStopGahterOnAttack(Role *role);

	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual void OnHurtOther(Character *cha, Character *target);

	void OnBuyGatherBuff(Role *role);
	void OnActivityStatusChange(int status);

	void SendPlayerInfoToPlayer(Role *role = nullptr);

	JoinerItem *GetJoinerItem(int uid);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	void FetchShuijingTaskReward(Role *role ,int task_id);
	void SendJoinerTaskInfo(Role *role);
	void CalcShuijingGatherTimes(Role *role, int gather_id);

	virtual int GetSpecialParam(Role *role);
	void NotifyGatherBuffInfo(Role *role, short is_has_buff);

protected:
	void OnActivityOpen();
	void OnActivityClose();

	Role * GetRole(ObjID obj_id);

	void UpdateBigGatherNum();
	void GatherPauseBigShuiJing();

	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);

	void CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist = false);

	bool m_is_init;
	int m_big_shuijing_num;													// 至尊水晶数量
	std::vector<int> m_big_shuijing_next_flush_time;
	
	std::map<int, JoinerItem *> m_joiner_map;

	unsigned int m_next_notice_gather_buff_info_time;
	bool m_big_shuijing_notify;
};

#endif
