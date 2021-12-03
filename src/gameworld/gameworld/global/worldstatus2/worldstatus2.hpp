// 此模块用于管理跨服gs全局数据

#ifndef __WORLD_STATUS2_HPP__
#define __WORLD_STATUS2_HPP__

class Role;

#include "iclockmodule.h"
#include "inetworkmodule.h"
#include "servercommon/struct/global/worldstatus2param.hpp"
#include "servercommon/crossdef.hpp"

#include <deque>

enum WORLDSTATE2_DATA_STATUS
{			
	WORLDSTATE2_DATA_STATUS_INVALID = 0,						// 初始状态
	WORLDSTATE2_DATA_STATUS_LOADING,							// 数据加载中
	WORLDSTATE2_DATA_STATUS_LOAD_FINISH,						// 数据加载完成
	WORLDSTATE2_DATA_STATUS_MAX,
};

static const int WORLDSTATE2_SAVE_INTERVAL = 30;				// 半分钟回写一次

struct CrossBossDropRecordItem
{
	CrossBossDropRecordItem() : uuid(0), pickup_timestamp(0), scene_id(0), monster_id(0), item_id(0)
	{
		memset(name, 0, sizeof(name));
	}

	long long uuid;
	unsigned int pickup_timestamp;
	int scene_id;
	unsigned short monster_id;
	ItemID item_id;
	GameName name;
	ItemDataWrapper item_wrapper;
};

class WorldStatus2
{
	friend class RMIInitWorldStatus2BackObjectImpl;

public:
	static WorldStatus2 & Instance();

	void Init(const WorldStatus2Param &worldstatus2_param);

	void OnServerStart();
	void OnServerStop();

	void Update(unsigned long interval, time_t now_second);

	void OnEnterScene(Role *role);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnResetData(unsigned int old_dayid, unsigned int now_dayid);

	bool IsLoadFinish() const;


	WorldStatus2CommonDataParam & GetCommonData2() { return m_world_status2_param.common_data_2; }

	// 婚宴----------------------------------------------
	WeddingParam &GetWeddingParam() { return m_world_status2_param.wedding_param; }

	char GetWeedingYuYueSeqByUID(int uid);
	bool IsInWeddingGuests(int owner_uid, int guest_uid);
	bool AddWeddingGeust(int owner_uid, int guest_uid);
	bool RemoveWeddingGeust(int owner_uid, int guest_uid);
	bool AddWeddingCanInviteGuestNum(int owner_uid);
	int GetWeddingHasInviteGuestNum(int owner_uid);  // 婚宴名单人数(已经邀请了多少人)
	int AddTotalWeddingNum();
	void ClearWeddingGuests(int wedding_seq);

	// 表白墙----------------------------------------------
	void RecoredProfession(Role* role, PersonProfessItem& prfess_item); // 记录表白
	void SendProfessionInfo(Role* role); // 发送公共表白墙
	void OnRoleMuteClearProfession(int uid);
	// 跨服boss掉落记录----------------------------------------------
	std::deque<CrossBossDropRecordItem> &GetCrossBossDropRecord() { return m_drop_record_list; }

	// 猎鲲----------------------------------------------
	void SetCrossLieKunFBGuildId(int zone, GuildID liekun_guild_id);
	void GetCrossLieKunFBGuildId(GuildID liekun_guild_id_list[LIEKUN_ZONE_TYPE_COUNT]);
	void ClearCrossLieKunFBGuildId(bool is_to_open = false);
	GuildID GetCrossLieKunFBTopOneGuildId();

	// 夜战、乱斗活动-----------------------------------------------
	void SyncRoomActivityTitleInfo(Role *role,int reason);
	void ClearMessBattleActiviyTopRank();
	void SetMessBattleTopRankUid(int rank,int uid);

	void ClearNightFightActivityTopRank();
	void SetNightFightTopRankUid(int rank, int uid);

	void ClearCrossXiuluoTowerRankTitle();
	void SetCrossXiuluoTowerRankTitle(int rank, int uid);
	void SyncCrossActivityTitleInfo(Role* role, int reason);

	int GetTotalUpgradeQilinbiRecordList(int grade);	//获取麒麟臂进阶人数
	int GetTotalUpgradeToushiRecordList(int grade);		//获取头饰进阶人数
	int GetTotalUpgradeYaoshiRecordList(int grade);		//获取腰饰进阶人数
	int GetTotalUpgradeFightMountRecordList(int grade);	//获取战骑进阶人数
	int GetTotalUpgradeRecordList(int type, int grade);	//进阶系统进阶人数
	int GetTotalUpgradeFabaoRecordList(int grade);		//获取法宝进阶人数
	int GetTotalUpgradeShizhuangRecordList(int grade);	//获取时装进阶人数
	int GetTotalUpgradeShenbingRecordList(int grade);	//获取神兵进阶人数
	int GetTotalUpgradeFootprintRecordList(int grade);	//获取足迹进阶人数

	void IncTotalUpgradeQilinbiRecordList(int grade);	//麒麟臂进阶人数+1
	void IncTotalUpgradeToushiRecordList(int grade);	//头饰进阶人数+1
	void IncTotalUpgradeYaoshiRecordList(int grade);	//腰饰进阶人数+1
	void IncTotalUpgradeFightMountRecordList(int grade);//战骑进阶人数+1
	void IncTotalUpgradeRecordList(int type, int grade);//进阶系统进阶人数+1
	void IncTotalUpgradeFabaoRecordList(int grade);		//法宝进阶人数+1
	void IncTotalUpgradeShizhuangRecordList(int grade);	//时装进阶人数+1
	void IncTotalUpgradeShenbingRecordList(int grade);	//神兵进阶人数+1
	void IncTotalUpgradeFootprintRecordList(int grade);	//足迹进阶人数+1

	void GetImageCompetitionTypeAndId(int *type, int *id);
	void SetImageCompetitionTypeAndId(int type, int id);

	void SetLieKunTopRankGuildId(GuildID guild_id);

	// ----------------------------------------------------------------

	// 全服收购记录
	void AddWorldAcquisitionLog(const WorldAcquisitionLogItem &log);	// 添加一条
	void SendWorldAcquisitionLog(short notice_type, Role* role);		// 下发通知/广播

	// ----------------------------------------------------------------

private:
	WorldStatus2();
	~WorldStatus2();

	bool LoadWorldStatus2(long long id_from);
	void LoadWorldStatus2Succ();

	bool SaveWorldStatus2(); 

	int m_data_state; 

	WorldStatus2Param m_world_status2_param;

	GuildID m_liekun_guild_id_list[LIEKUN_ZONE_TYPE_COUNT];	// 不存档 猎鲲地带杀死区域boss的帮派id
	std::deque<CrossBossDropRecordItem> m_drop_record_list; // 不存档

	// ----------------------------------------------------------------
	void InitWorldAcquisitionLogList();			// 从Param到deque
	void SaveWorldAcquisitionLogList();			// 从deque到Param
	std::deque<WorldAcquisitionLogItem> m_world_acquisition_log_list; // 全服收购记录
};

#endif // __WORLD_STATUS2_HPP__
