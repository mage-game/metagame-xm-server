#ifndef __RAND_ACTIVITY_MANAGER_HPP__
#define __RAND_ACTIVITY_MANAGER_HPP__

#include "servercommon/activitydef.hpp"

#include "randactivity.hpp"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "global/rank/personrank.hpp"

class Role;
class RandActivityLuckRoll;
class RandActivityServerPanicBuy;
class RandActivityShouYouYuXiang;
class RandActivityCornucopia;
class RandActivityLevelLottery;
class RandActivityChongzhiNiuEgg;
class RandActivityZhenBaoGe;
class RandActivityMoneyTree;
class RandActivityMoneyTree2;
class RandActivityZhenBaoGe2;
class RandActivityPerfectLover;
class RandActivityMiJingXunBao3;
class RandActivityHuanLeZaDan;
class RandActivityHuanLeYaoJiang;
class RandActivityHuanLeYaoJiang2;
class RandActivityLottery1;
class RandActivityGiftHarvest;
class RandActivityLuckyCloudBuy;
class RandActivityCracyBuyToAll;

class RandActivityManager
{
	struct RASetTimeItem
	{
		RASetTimeItem() : rand_activty_type(0), begin_time(0), end_time(0), open_type(0), delay_to_time(0) {}

		int rand_activty_type;
		unsigned int begin_time;
		unsigned int end_time;
		int open_type;

		unsigned int delay_to_time; // 延时到何时设置时间
	};

public:
	static RandActivityManager & Instance();

	void Init(const RandActivityParam &param);
	void GetInitParam(RandActivityParam *param);

	void Update(unsigned long interval, time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void OnUserLogin(Role *user);
	void OnUserLogout(Role *user);

	bool IsRandActivityOpen(int rand_activity_type);
	int GetSevenDayBipinCurOpenType();

	bool OnSetActivityTime(int rand_activity_type, unsigned int begin_time, unsigned int end_time, int limit_day);
	void OnForceToNextState(int rand_activity_type);
	bool OnForceSetActivityTime(int rand_activity_type, unsigned int begin_time, unsigned int end_time, int open_type = RAND_ACTIVITY_OPEN_TYPE_NORMAL);

	void OnPersonRankSnapshot(int old_top_uid_list[PERSON_RANK_TYPE_MAX], int new_top_uid_list[PERSON_RANK_TYPE_MAX]);
	bool IsSyncRankData(int rand_activity_type);

	RandActivityCornucopia * GetRandActivityCornucopia(int activity_type);
	RandActivityLuckRoll * GetRandActivityLuckRoll();
	RandActivityServerPanicBuy * GetRandActivityServerPanicBuy();
	RandActivityShouYouYuXiang * GetRandActivityShouYouYuXiang();
	RandActivityLevelLottery * GetRandActivityLevelLottery();
	RandActivityChongzhiNiuEgg * GetRandActivityNiuEgg();
	RandActivityZhenBaoGe * GetRandActivityZhenBaoGe();
	RandActivityMoneyTree * GetRandActivityMoneyTree();
	RandActivityMoneyTree2 * GetRandActivityMoneyTree2();
	RandActivityZhenBaoGe2 * GetRandActivityZhenBaoGe2();
	RandActivityPerfectLover *GetRandActivityPerfectLover();
	RandActivityHuanLeYaoJiang2 * GetRandActivityHuanLeYaoJiang2();

	RandActivityMiJingXunBao3 * GetRandActivityMiJingXunBao3();
	RandActivityHuanLeZaDan * GetRandActivityHuanLeZaDan();
	RandActivityHuanLeYaoJiang * GetRandActivityHuanLeYaoJiang();
	RandActivityLottery1 * GetRandActivityLottery1();
	RandActivityGiftHarvest* GetRandActivityGiftHarvest();

	RandActivityLuckyCloudBuy* GetRandActivityLuckyCloudBuy();

	void GetRandActivityBipinRank3(int activity_type, PersonRankItem top_user_list[Protocol::TOP_COUNT]);

	RandActivity * ForceGetRandActivity(int activity_type);

	unsigned int GetRanadActivityOpenTime(int rand_activity_type);

	int GetRanadActivityDayIndex(int rand_activity_type);
	RandActivityCracyBuyToAll * GetRandActivityCracyBuy();
private:
	RandActivityManager();
	~RandActivityManager();

	void CalActivityOpenTime();

	RandActivity *m_rand_activity_list[RAND_ACTIVITY_TYPE_MAX];

	bool m_is_first;

	typedef std::map<long long, RASetTimeItem>::iterator RASetTimeItemIt;
	std::map<long long, RASetTimeItem> m_ra_delay_settime_map;				// OnSetActivityTime函数延时设置时间列表
	std::map<long long, RASetTimeItem> m_ra_delay_force_settime_map;		// OnForceSetActivityTime函数延时设置时间列表
};

#endif // __RAND_ACTIVITY_MANAGER_HPP__

