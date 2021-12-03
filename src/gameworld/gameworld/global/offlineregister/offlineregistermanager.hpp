#ifndef __OFFLINE_REGISTER_MANAGER_HPP__
#define __OFFLINE_REGISTER_MANAGER_HPP__

#include <map>
#include <vector>

#include "servercommon/serverdef.h"

#include "servercommon/servercommon.h"
#include "servercommon/struct/global/offlineregisterparam.hpp"
#include "servercommon/activitydef.hpp"
#include "servercommon/internalprotocal/gglobalprotocal.h"

class OfflineRegister;
class Role;

static const int MAX_SAVE_OFFLINE_REGISTER_INTERVAL = 20;		// 回档时间
static const int MAX_REMOVE_OFFLINE_REGISTER_INTERVAL = 30;		// Remove时间

enum OFFLINE_REGISTER_DATA_STATE
{				
	OFFLINE_REGISTER_STATE_INVALID = 0,							// 初始状态
	OFFLINE_REGISTER_STATE_LOADING,								// 数据加载中 
	OFFLINE_REGISTER_STATE_LOAD_FINISH,							// 数据加载完成 
	OFFLINE_REGISTER_DATA_STATE_MAX
};

class OfflineRegisterManager
{
	friend class RMIInitOfflineRegisterBackObjectImpl;

public:
	static OfflineRegisterManager & Instance();

	void OnServerStart();
	void OnServerStop();

	bool IsLoadFinish();

	void Update(unsigned long interval, time_t now_second);

	void OnEvaluateRole(const UserID &target_user_id, int add_charm);

	void OnGMEvaluateRole(const UserID &target_user_id, short rank_type, bool is_admire, int times);

	void OnDivorceReq(const UserID &target_user_id, unsigned int last_marry_time);
	void OnUserFirstEnterScene(Role *user);

	void OnChongzhiCmd(const UserID &target_user_id, int chongzhi_num);

	void OnRedPaperConsumeGold(const UserID &target_user_id, int gold_num);
	void OnAddRedPaperLuckyValue(const UserID &target_user_id, int lucky_value);

	void OnSpaceBeCai(const UserID &target_user_id);
	void OnSpaceBeGift(const UserID &target_user_id);

	void OnQingyuanBlessDaysChange(const UserID &target_user_id, int bless_days);
	void OnGetQingyuanBlessDays(const UserID &target_user_id, int *bless_days);

	GuildBoxInfo *GetUserGuildBoxInfo(const UserID &user_id, bool is_force = false);
	void OnGuildBoxInfoChange(const UserID &user_id);
	void GuildBoxResetAll();

	bool OnSetAuthorityType(int uid, int authority_type);

	bool OnSetConsumeItem(int uid, unsigned short item_id, unsigned short item_num);
	bool OnSetConsumeGold(int uid, int num);
	bool OnSetConsumeBindGold(int uid, int num);

	// 表白
//	void OnProfessFrom(const UserID &target_user_id, Role* profess_role, PersonProfessItem& item);
	bool OnSetForbidTalk(int uid, unsigned int forbid_talk_end_timestamp);

private:
	OfflineRegisterManager();
	~OfflineRegisterManager();

	OfflineRegister * GetOfflineRegister(const UserID &user_id);
	OfflineRegister * CreateOfflineRegister(const UserID &user_id);

	bool LoadOfflineRegister(long long offline_reg_id_from);
	void LoadOfflineRegisterSucc();

	bool OnOfflineRegisterChange(OfflineRegister *offline_reg, int change_state);
	bool AddOfflineRegister(const UserID &user_id, const OfflineRegisterParam::OfflineRegisterAttr &offline_reg_attr);
	bool CheckSaveOfflineRegister(time_t now_second);

	OfflineRegisterParam m_change_param; 
	int m_data_state;
	time_t m_last_save_time;
	time_t m_last_remove_time;

	typedef std::map<UserID, OfflineRegister*> OfflineRegisterMap; 
	typedef std::map<UserID, OfflineRegister*>::iterator OfflineRegisterMapIt;
	OfflineRegisterMap m_offline_register_map;
};

#endif // __OFFLINE_REGISTER_MANAGER_HPP__

