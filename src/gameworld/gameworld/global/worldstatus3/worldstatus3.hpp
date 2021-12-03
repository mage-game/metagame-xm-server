// ��ģ�����ڹ�����gsȫ������

#ifndef __WORLD_STATUS3_HPP__
#define __WORLD_STATUS3_HPP__

class Role;

#include "iclockmodule.h"
#include "inetworkmodule.h"
#include "servercommon/struct/global/worldstatus3param.hpp"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/activitydef.hpp"

enum WORLDSTATE3_DATA_STATUS
{
	WORLDSTATE3_DATA_STATUS_INVALID = 0,						// ��ʼ״̬
	WORLDSTATE3_DATA_STATUS_LOADING,							// ���ݼ�����
	WORLDSTATE3_DATA_STATUS_LOAD_FINISH,						// ���ݼ������
	WORLDSTATE3_DATA_STATUS_MAX,
};

static const int WORLDSTATE3_SAVE_INTERVAL = 30;				// ����ӻ�дһ��

class WorldStatus3
{
	friend class RMIInitWorldStatus3BackObjectImpl;

public:
	static WorldStatus3 & Instance();

	void Init(const WorldStatus3Param &worldstatus3_param);

	void OnServerStart();
	void OnServerStop();

	void Update(unsigned long interval, time_t now_second);

	void OnEnterScene(Role *role);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnResetData(unsigned int old_dayid, unsigned int now_dayid);

	bool IsLoadFinish() const;

	void SetCrossAddCapRankFirst(int uid_list[], int list_count = CROSS_RANK_REWARD_COUNT);
	void GetCrossAddCapRankFirst(int uid_list[], int list_count = CROSS_RANK_REWARD_COUNT);

	void SetCrossAddCharmRankFirst(int uid_list[], int list_count = CROSS_RANK_REWARD_COUNT);
	void GetCrossAddCharmRankFirst(int uid_list[], int list_count = CROSS_RANK_REWARD_COUNT);

	void CheckClearOldCoupleRankData();		// ��ʼ��ʱ������������
	void SetCrossQingyuanCapRankFirstMale(long long uuid_list[], int list_count = CROSS_RANK_REWARD_COUNT);
	void GetCrossQingyuanCapRankFirstMale(long long uuid_list[], int list_count = CROSS_RANK_REWARD_COUNT);

	void SetCrossQingyuanCapRankFirstFemale(long long uuid_list[], int list_count = CROSS_RANK_REWARD_COUNT);
	void GetCrossQingyuanCapRankFirstFemale(long long uuid_list[], int list_count = CROSS_RANK_REWARD_COUNT);

	void SetCrossGuildKillRankFirst(int guild_list[], int list_count = CROSS_RANK_REWARD_COUNT);
	void GetCrossGuildKillRankFirst(int guild_list[], int list_count = CROSS_RANK_REWARD_COUNT);

	int GetWorldLotteryCount(int lottery_type);
	bool ResetWorldLotteryCount(int lottery_type);
	bool AddWorldLotteryCount(int lottery_type, UserLotteryCount count);

	RandActivityLuckyCloudBuyParam & GetRandActivityLuckyCloudBuyParam();

private:
	WorldStatus3();
	~WorldStatus3();

	bool LoadWorldStatus3(long long id_from);
	void LoadWorldStatus3Succ();

	bool SaveWorldStatus3();
	int m_data_state;

	WorldStatus3Param m_param;
};

#endif // __WORLD_STATUS3_HPP__

