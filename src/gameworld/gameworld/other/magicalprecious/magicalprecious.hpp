#ifndef __MAGICAL_PRECIOUS_H__
#define __MAGICAL_PRECIOUS_H__

#include "servercommon/magicalpreciousdef.hpp"

class Role;

class MagicalPrecious
{
public:

	MagicalPrecious();
	~MagicalPrecious();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const MagicalPreciousParam &param);
	void GetInitParam(MagicalPreciousParam *param);

	void Update(unsigned int now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void OnRoleLevelChange(int level);										

	void FetchMagicalPreciousReward(int reward_index, int chapter);						// 领取奖励
	void ConvertPreciousChapterReward(int chapter, int seq);							// 兑换章节奖励
	void FetchScore(int reward_index, int chapter);										// 领取积分

	bool IsFetchReward(int reward_index, int chapter);									// 判断是否已领取
	bool IsFinish();																	// 判断是否全部完成
	bool SetFetchRewardFlag(int reward_index, int chapter);								// 设置已领取标记

	void SendMagicalPreciousAllInfo();													// 发送全部信息
	void SendMagicalPreciousChapterInfo(int chapter);									// 发送章节信息
	void SendMagicalPreciousConditionChange(int condition_type);						// 任务状态改变

	void AddConditionTimes(int condition_type, int add_times);							// 增加任务次数
	void CheckAddKillBossTimes(int condition_type, int add_times, int level);			// 增加特定等级特定boss击杀次数

	void GMIncScore(int chapter, int score);
	void GMDecScore(int chapter, int score);
	void GMIncChapter(int chapter);
	void GMFinish(int chapter, int reward_index, char flag);

private:

	Role *m_role;
	MagicalPreciousParam m_param;
	bool m_is_send_all;
	static const int FINISH_CHAPTER_ID = -2;								// 全部完成的章节ID
};

#endif
