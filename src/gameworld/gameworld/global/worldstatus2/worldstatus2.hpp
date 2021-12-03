// ��ģ�����ڹ�����gsȫ������

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
	WORLDSTATE2_DATA_STATUS_INVALID = 0,						// ��ʼ״̬
	WORLDSTATE2_DATA_STATUS_LOADING,							// ���ݼ�����
	WORLDSTATE2_DATA_STATUS_LOAD_FINISH,						// ���ݼ������
	WORLDSTATE2_DATA_STATUS_MAX,
};

static const int WORLDSTATE2_SAVE_INTERVAL = 30;				// ����ӻ�дһ��

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

	// ����----------------------------------------------
	WeddingParam &GetWeddingParam() { return m_world_status2_param.wedding_param; }

	char GetWeedingYuYueSeqByUID(int uid);
	bool IsInWeddingGuests(int owner_uid, int guest_uid);
	bool AddWeddingGeust(int owner_uid, int guest_uid);
	bool RemoveWeddingGeust(int owner_uid, int guest_uid);
	bool AddWeddingCanInviteGuestNum(int owner_uid);
	int GetWeddingHasInviteGuestNum(int owner_uid);  // ������������(�Ѿ������˶�����)
	int AddTotalWeddingNum();
	void ClearWeddingGuests(int wedding_seq);

	// ���ǽ----------------------------------------------
	void RecoredProfession(Role* role, PersonProfessItem& prfess_item); // ��¼���
	void SendProfessionInfo(Role* role); // ���͹������ǽ
	void OnRoleMuteClearProfession(int uid);
	// ���boss�����¼----------------------------------------------
	std::deque<CrossBossDropRecordItem> &GetCrossBossDropRecord() { return m_drop_record_list; }

	// ����----------------------------------------------
	void SetCrossLieKunFBGuildId(int zone, GuildID liekun_guild_id);
	void GetCrossLieKunFBGuildId(GuildID liekun_guild_id_list[LIEKUN_ZONE_TYPE_COUNT]);
	void ClearCrossLieKunFBGuildId(bool is_to_open = false);
	GuildID GetCrossLieKunFBTopOneGuildId();

	// ҹս���Ҷ��-----------------------------------------------
	void SyncRoomActivityTitleInfo(Role *role,int reason);
	void ClearMessBattleActiviyTopRank();
	void SetMessBattleTopRankUid(int rank,int uid);

	void ClearNightFightActivityTopRank();
	void SetNightFightTopRankUid(int rank, int uid);

	void ClearCrossXiuluoTowerRankTitle();
	void SetCrossXiuluoTowerRankTitle(int rank, int uid);
	void SyncCrossActivityTitleInfo(Role* role, int reason);

	int GetTotalUpgradeQilinbiRecordList(int grade);	//��ȡ����۽�������
	int GetTotalUpgradeToushiRecordList(int grade);		//��ȡͷ�ν�������
	int GetTotalUpgradeYaoshiRecordList(int grade);		//��ȡ���ν�������
	int GetTotalUpgradeFightMountRecordList(int grade);	//��ȡս���������
	int GetTotalUpgradeRecordList(int type, int grade);	//����ϵͳ��������
	int GetTotalUpgradeFabaoRecordList(int grade);		//��ȡ������������
	int GetTotalUpgradeShizhuangRecordList(int grade);	//��ȡʱװ��������
	int GetTotalUpgradeShenbingRecordList(int grade);	//��ȡ�����������
	int GetTotalUpgradeFootprintRecordList(int grade);	//��ȡ�㼣��������

	void IncTotalUpgradeQilinbiRecordList(int grade);	//����۽�������+1
	void IncTotalUpgradeToushiRecordList(int grade);	//ͷ�ν�������+1
	void IncTotalUpgradeYaoshiRecordList(int grade);	//���ν�������+1
	void IncTotalUpgradeFightMountRecordList(int grade);//ս���������+1
	void IncTotalUpgradeRecordList(int type, int grade);//����ϵͳ��������+1
	void IncTotalUpgradeFabaoRecordList(int grade);		//������������+1
	void IncTotalUpgradeShizhuangRecordList(int grade);	//ʱװ��������+1
	void IncTotalUpgradeShenbingRecordList(int grade);	//�����������+1
	void IncTotalUpgradeFootprintRecordList(int grade);	//�㼣��������+1

	void GetImageCompetitionTypeAndId(int *type, int *id);
	void SetImageCompetitionTypeAndId(int type, int id);

	void SetLieKunTopRankGuildId(GuildID guild_id);

	// ----------------------------------------------------------------

	// ȫ���չ���¼
	void AddWorldAcquisitionLog(const WorldAcquisitionLogItem &log);	// ���һ��
	void SendWorldAcquisitionLog(short notice_type, Role* role);		// �·�֪ͨ/�㲥

	// ----------------------------------------------------------------

private:
	WorldStatus2();
	~WorldStatus2();

	bool LoadWorldStatus2(long long id_from);
	void LoadWorldStatus2Succ();

	bool SaveWorldStatus2(); 

	int m_data_state; 

	WorldStatus2Param m_world_status2_param;

	GuildID m_liekun_guild_id_list[LIEKUN_ZONE_TYPE_COUNT];	// ���浵 ����ش�ɱ������boss�İ���id
	std::deque<CrossBossDropRecordItem> m_drop_record_list; // ���浵

	// ----------------------------------------------------------------
	void InitWorldAcquisitionLogList();			// ��Param��deque
	void SaveWorldAcquisitionLogList();			// ��deque��Param
	std::deque<WorldAcquisitionLogItem> m_world_acquisition_log_list; // ȫ���չ���¼
};

#endif // __WORLD_STATUS2_HPP__
