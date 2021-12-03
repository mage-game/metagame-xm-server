#ifndef __SPECIAL_XINGZUOYIJI_HPP__
#define __SPECIAL_XINGZUOYIJI_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include <map>
#include "config/activityconfig/xingzuoyijiconfig.hpp"

enum GATHER_TYPE
{
	COMMON_BOX = 0,				// 低级宝箱
	GREAT_BOX = 1				// 高级宝箱
};

class SpecialXingzuoYiji : public SpecialLogic
{
public:
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
			memset(gather_num_list, 0, sizeof(gather_num_list));
			memset(once_gather_num_list, 0, sizeof(once_gather_num_list));
		}

		int uid;
		short gather_num_list[XingzuoYiJiConfig::MAX_GRADE_BOX];
		short once_gather_num_list[XingzuoYiJiConfig::MAX_GRADE_BOX];
	};

	static const int MAX_GATHER_TIMES = 3;
	static const unsigned int REFRESH_COMMON_GATHER_DAY_INTERVAL = 60 * 60 * 9;

	SpecialXingzuoYiji(Scene *scene);
	virtual ~SpecialXingzuoYiji();

	void OnActivityStatusChange(int status);

	void SendtoRoleNotInScene(Role *role);
	void SkipGather(Role *role, int quality = XingzuoYiJiConfig::MEDIUM_BOX);

	virtual bool CanDestroy();

	virtual void OnRoleEnterScene(Role *role);

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual bool SpecialCanGather(Role *role, GatherObj *gather);
	virtual void OnStartGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);

private:
	void OnActivityStart();
	void OnActivityClose();
	void XingzuoYijiReset();

	JoinerItem *GetJoinerItem(int uid);
	void SyncInfoToAll();
	void SyncRoleInfo(Role *role);
	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);

	void RefreshBoss();
	void RefreshGather(int gather_type);
	void GetBoxItem(Role *role, int box_type);

	int GetGatherType(int box_type);

	void ResetJoinerOnceGather(int box_type);
	void CalNextRefreshGatherTime();

	std::map<int, JoinerItem> m_joiner_map;
	bool m_is_finish;

	unsigned int m_next_boss_refresh_time;

	bool m_is_refresh_gather;
	unsigned int m_temp_next_refresh_gather_time;

	int m_boss_dead_count;
	int m_now_gather_num;

	int m_now_boss_num;

	unsigned int m_last_notice_time;
	int m_notice_times;

	bool m_is_notice_end;

	unsigned int m_day_id;
	unsigned int m_next_refresh_gather_time;
	unsigned int m_last_broadcast_gather_time;
};

#endif // __SPECIAL_XINGZUOYIJI_HPP__
