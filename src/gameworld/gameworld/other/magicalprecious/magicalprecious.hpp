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

	void FetchMagicalPreciousReward(int reward_index, int chapter);						// ��ȡ����
	void ConvertPreciousChapterReward(int chapter, int seq);							// �һ��½ڽ���
	void FetchScore(int reward_index, int chapter);										// ��ȡ����

	bool IsFetchReward(int reward_index, int chapter);									// �ж��Ƿ�����ȡ
	bool IsFinish();																	// �ж��Ƿ�ȫ�����
	bool SetFetchRewardFlag(int reward_index, int chapter);								// ��������ȡ���

	void SendMagicalPreciousAllInfo();													// ����ȫ����Ϣ
	void SendMagicalPreciousChapterInfo(int chapter);									// �����½���Ϣ
	void SendMagicalPreciousConditionChange(int condition_type);						// ����״̬�ı�

	void AddConditionTimes(int condition_type, int add_times);							// �����������
	void CheckAddKillBossTimes(int condition_type, int add_times, int level);			// �����ض��ȼ��ض�boss��ɱ����

	void GMIncScore(int chapter, int score);
	void GMDecScore(int chapter, int score);
	void GMIncChapter(int chapter);
	void GMFinish(int chapter, int reward_index, char flag);

private:

	Role *m_role;
	MagicalPreciousParam m_param;
	bool m_is_send_all;
	static const int FINISH_CHAPTER_ID = -2;								// ȫ����ɵ��½�ID
};

#endif
