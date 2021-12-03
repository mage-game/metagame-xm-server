#ifndef __MARRIAGE_SEEKING_MANAGER_H__
#define __MARRIAGE_SEEKING_MANAGER_H__

class Role;

#include "servercommon/serverdef.h"
#include "servercommon/servercommon.h"

enum MARRIAGE_SEEKING_TYPE
{
	MARRIAGE_SEEKING_TYPE_INVALID = 0,

	MARRIAGE_SEEKING_TYPE_INFO,
	MARRIAGE_SEEKING_TYPE_INSERT,

	MARRIAGE_SEEKING_TYPE_MAX,
};

const static int MARRIAGE_SEEKING_MAX_COUNT = 20;			// 征婚宣言最大个数

struct MarriageSeeking										
{
	MarriageSeeking()
		: uid(0), timestamp(0)
	{
		memset(seeking_notice, 0, sizeof(seeking_notice));
	}

	int uid;
	MarriageSeekingNotice seeking_notice;
	unsigned int timestamp;
};

struct MarriageSeekingList
{
	MarriageSeekingList()
		: m_curour(0), m_seeking_count(0)
	{

	}

	void InsertSeeking(Role *role, MarriageSeekingNotice seeking_notice);
	void RemoveSeeking(int uid);

	int m_curour;
	int m_seeking_count;
	MarriageSeeking m_seeking_list[MARRIAGE_SEEKING_MAX_COUNT];
};

class MarriageSeekingManager
{
public:
	static MarriageSeekingManager & Instance();
	
	void MarriageSeekingOperateType(Role *role, int type, MarriageSeekingNotice marriage_seeking_notice);
	void InsertMarriageSeekingNotice(Role *role, MarriageSeekingNotice marriage_seeking_notice);
	void SendMarriageSeekingNoticeList(Role *role);
	void OnChangeSex(Role *role);
	void OnMarry(Role *role);

private:
	MarriageSeekingManager();
	~MarriageSeekingManager();

	MarriageSeekingList m_girls_seeking_list;
	MarriageSeekingList m_boys_seeking_list;
};

#endif


