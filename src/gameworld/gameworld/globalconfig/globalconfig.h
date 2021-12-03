#ifndef __GLOBALCONFIG_H__
#define __GLOBALCONFIG_H__

#include "roleglobalconfig.h"
#include "roleexpconfig.h"
#include "otherglobalconfig.h"
#include "keyconfig.h"
#include "equipconfig.h"
#include "dynamicconfig.hpp"
#include "config/activedegreeconfig.hpp"
#include "config/fbconfig.hpp"
#include "gamedef.h"

#define GLOBALCONFIG GlobalConfig::Instance()
#define GETPROFVALUE GlobalConfig::Instance()->GetDiffProfAttrValue

class GlobalConfig
{
public:
	static GlobalConfig * Instance();

	bool Init(const std::string &path, const std::string &configname, std::string *err);
	static bool Reload(const std::string &path, const std::string &configname, std::string *err);

	const Attribute GetRoleGlobalCooldown() const { return m_roleconfig.m_cooldown; }
	const Posi & GetRoleInTeamDistance() const { return m_roleconfig.m_in_team_distance; }
	const unsigned int GetRecoverHpInterval() const { return m_roleconfig.m_recover_hp_interval; }
	const int GetRecoverHpPer() const { return m_roleconfig.m_recover_hp_per; }
	const int GetRecoverHpFightPer() const { return m_roleconfig.m_recover_hp_fight_per; }
	const int GetRoleXinShouProtectedLevel() const { return m_roleconfig.m_xinshuo_protected_level; }
	const int GetRoleUpgradeLogLevel() const { return m_roleconfig.m_upgrade_log_level; }
	const int GetRolePeaceModeInterval() const { return m_roleconfig.m_peace_mode_interval; }
	const int GetRoleReAliveInterval() const { return m_roleconfig.m_realive_interval; }
	const int GetRoleInFightMs() const { return m_roleconfig.m_in_fight_ms; }


	const EquipConfig & GetEquipConfig() const { return m_equipconfig; }
	DynamicConfig & GetDynamicConfig() { return m_dynamicconfig; }
	globalconfig::RoleGlobalConfig & GetRoleCfg() { return m_roleconfig; }

	const unsigned int GetOtherConfig_ItemDrop_Interval_Owner() const;					// ��ȡ������Ʒ���˵�ʰȡʱ��
	const unsigned int GetOtherConfig_ItemDrop_Disappear_Time() const;					// ��ȡ������Ʒ����ʧʱ��
	const unsigned int GetOtherConfig_ItemDrop_Disappear_Time_Non_Static_Scene() const;	// ��Ұ���ȡ������Ʒ����ʧʱ��
	const unsigned int GetOtherConfig_ItemDrop_Length() const;							// ��ȡ��Ʒ��ʰȡ��������
	const unsigned int GetOtherConfig_Monster_Dead_Delay() const;						// ��ȡ������������ʧʱ��
	const unsigned int GetOtherConfig_Monster_Follow_Interval() const;					// ��ù������ʱ��
	const unsigned int GetOtherConfig_SortInOrder_Interval() const;						// ��ȡ������Ʒ�����ʱ����
	const unsigned int GetOtherConfig_Fight_Status_Leave_Interval_S() const;			// ��ȡ����ս��״̬��ʱ����
	const bool GetOtherConfig_IsCheckValid() const;										// �Ƿ�������ƶ�
	const unsigned int GetOtherConfig_MaxMoveSpeed() const;								// ��Ϸ�п��ܵ�����ٶ�
	const unsigned int GetOtherConfig_MoveCheck_Length() const;							// ��ȡ������ƶ��Ƿ�����
	const unsigned int GetOtherConfig_MaxTransportLength() const;						// ��ȡ���͵����Զ���������
	const int GetOtherConfig_KnapsackExtendNeedItemCount(int extend_index ,bool is_storage) const;		// ������չ �����
	const ItemID GetOtherConfig_KnapsackExtendBagItemID() const;						// ������չ ��ƷID
	const ItemID GetOtherConfig_KnapsackExtendStorageItemID() const;						// �ֿ���չ ��ƷID

	const unsigned int GetGatherMaxDistance() const;									// �ɼ�����
	const unsigned int GetNpcMaxTouchDistance() const;									// NPC���������

	const unsigned int GetKeyConfig_ZoneMatrixX() const;
	const unsigned int GetKeyConfig_ZoneMatrixY() const;
	const unsigned int GetKeyConfig_RoleAoiX() const;
	const unsigned int GetKeyConfig_RoleAoiY() const;
	const unsigned int GetKeyConfig_RoleAoiLeaveMoreThenEnter() const;
	const unsigned int GetKeyConfig_RoleAoiSensitiveDistance() const;
	const unsigned int GetKeyConfig_PetAoiSensitiveDistance() const;
	const unsigned int GetKeyConfig_RoleSaveTime() const;

	bool IsGmOpen() const;

	long long GetRoleMaxExp(int level);
	RoleLevelCfg * GetRoleLvlCfg(int level);

	RoleZhuanShengCfg *GetRoleZhuanShengCfg(int role_level);

	globalconfig::OtherGlobalConfig &GetOtherGlobalCfg() { return m_other; }

	int GetDefaultSkillID(int prof, int default_skill_id_list[globalconfig::MAX_DEFAULT_SKILL_COUNT]);
	int GetFlyIncreaseSpeedPercent();

	int GetRandomCampItemID();
	int GetRandomCampGoldBind();
	int GetIgnoreLevelLimitChongzhi();

	int GetSpeakerLevel();
	int GetClearMentalityCDIntervalPerGold();

	int GetRestLimitLevel();
	int GetRestRecoveryInterval();
	int GetRestRecoveryHpPer();

	int GetRestRewardInterval();

	int GetShuangXiuAddPer();
	int GetShuangxiuIntimacyInterval();
	int GetShuangxiuIntimacyAdd();

	int GetNameColorEvil(int namecolor);
	int GetNameColorUpdateIntervalS();
	int GetNameColorMaxEvil();
	int GetNameColorUpdateDecEvil();
	int GetNameColorByEvil(int evil);
	bool IsRedNameColor(int evil);
	int GetReduceExpPercentByNameColor(int namecolor);
	int GetReduceHpPercentByNameColor(int namecolor);
	int GetReduceGongjiPercentByNameColor(int namecolor);

	ItemID GetRealiveItemId() const;
	int GetRealiveRecoverCommonScenePercent() const;
	int GetRealiveRecoverFbPercent() const;

	TaskID GetRoleBirthTaskId();

	int GetTeamTodayIntimacyLimit();
	int GetTeamIntimacyReward();
	int GetIntimacyRewardMonsterLevel();

	int GetFriendMaxNumber(int level);

	int GetHonourInitValue() { return m_other.honour_init_value; }
	int GetKillOtherCampHounorPer() { return m_other.kill_other_camp_get_honour_per; }
	int GetKillOtherCampMaxLevelDiff() { return m_other.kill_other_camp_max_level_diff; }
	int GetDoubleHonourKillCount() { return m_other.double_honour_kill_count; }

	int GetSceneIdByCampType(int camp_type);
	char GetCampTypeOfScene(int scene_id);
	bool IsCampScene(int scene_id);
	bool IsOtherCampScene(char my_camp, int scene_id);

	int GetCtrlEffectReduceCircleS();
	int GetCtrlEffectReducePercent(int ctrl_effect_times);

	bool GetFriendAttrList(int intimacy, CharIntAttrs &attr_add);
	bool CheckFriendAttrChange(int old_intimacy, int new_intimacy);

	int GetMaxNuqi() { return m_other.max_nuqi; }
	int GetNuqiAtkOther() { return m_other.nuqi_atk_other; }
	int GetNuqiAtked() { return m_other.nuqi_atked; }
	int GetMaxEnergy() { return m_other.max_energy; }
	int GetBuyMaxEnergy() { return m_other.buy_max_energy; }
	int GetGiveEnergyIntervalS() { return m_other.give_energy_interval_s; }
	int GetGiveEnergyPerTime() { return m_other.give_energy_per_time; }

	int GetWorldLevelOpenLevel() { return m_other.world_level_open_level; }
	double GetWorldLevelExpAddPercentPerLevel() { return m_other.world_level_exp_add_percent_perlevel; }
	int GetWorldLevelMaxExpAddPercent() { return m_other.world_level_max_exp_add_percent; }

	int GetWelfareDailyFindDays() { return m_other.welfare_daily_find_days; }

	int GetRoleTaskTransmitTaskId() { return 0; }
	int GetJumpLimitLevel() { return m_other.jump_limit_level; }
	int GetJumpLimitTimes() { return m_other.jump_limit_times; }
	int GetJumpRecoverTime() { return m_other.jump_recover_time; }
	int GetResetJumpPosMinLevel() { return m_other.reset_jump_pos_min_level; }

	float GetFbExpLevelFactor(int role_level);
	float GetFbPetLevelFactor(int role_level);

	char GetCampOfMonster(int monster_id);

	int GetResetNameItemId() { return m_other.reset_name_item_id; }
	int GetSpeedupHelloMax() { return m_other.speedup_hello_count_max; }
	int GetSpeedupHelloTimeoutS() { return m_other.speedup_hello_timeout_s; }
	int GetSpeedupHelloCountForbitTimeS() { return m_other.speedup_hello_count_forbit_time_s; }
	int GetSpeedupHelloTimeoutForbitTimeS() { return m_other.speedup_hello_timeout_forbit_time_s; }
	int GetSpeedupHelloAllPackPercentLimit() { return m_other.speedup_hello_all_pack_percent_limit; }
	bool IsOpenSpeedupHelloCheck() { return m_other.speedup_hello_open_check; }

	int GetServerMaxRoleNum() { return m_other.server_max_role_num; }

	int GetFriendBlessRewardMaxCount() { return m_other.friend_bless_reward_max_count; }
	int GetFriendBlessRewardTili() { return m_other.friend_bless_reward_tili; }
	int GetFriendBlessMaxCount() { return m_other.friend_bless_max_count; }
	int GetFirstChangeAvatarReward() { return m_other.first_change_avatar_reward_gold; }

	bool GetWorldTalkIsLimit() { return 0 != m_other.world_talk_is_limit; }
	int GetWorldTalkEffectMsgMinLen() { return m_other.world_talk_effect_msg_min_len; }
	int GetWorldTalkTorelateSameMaxCount() { return m_other.world_talk_torelate_max_count; }
	int GetWorldTalkAccumulateCountResetInterval() { return m_other.world_talk_accumulate_count_reset_interval; }

	Attribute GetDiffProfAttrValue(char prof, int attr_type, Attribute value);

	int GetGuWuCostGold() { return m_other.fb_guwu_cost_gold; }
	int GetGuWuCostCoin() { return m_other.fb_guwu_cost_coin; }
	int GetGuWuTime() { return m_other.fb_guwu_time; }
	int GetGuWuGongPer() { return m_other.fb_guwu_gongji_per; }

	int GetTradeLoginTimeLimit() { return m_other.trade_logintime_limit; }
	unsigned int GetPublicSaleSaleOutTime() { return m_other.publicsale_solt_out_time; }
	int GetPublicSaleMaxPrice() { return m_other.publicsale_max_price; }
	int GetPublicSaleOnceTax(int price);

	double GetMonsterRewardLevelFactor(int role_level, int delta_level, bool is_monster_high);	//����ȼ���Ұ�����ľ�������

	bool CanAutoExtendKnapsack(int count, unsigned int online_time);			// �ܷ��Զ���չ����

	bool IsAllowGmPublicSale() { return m_other.is_allow_gm_public_sale_buy > 0; }  // �Ƿ��������潻��

public:
	GlobalConfig() {}
	~GlobalConfig() {}

	globalconfig::RoleGlobalConfig m_roleconfig;
	RoleLevelConfig m_rolelevelconfig;
	RoleZhuanShengConfig m_zhuanshengconfig;
	EquipConfig m_equipconfig;
	globalconfig::OtherGlobalConfig m_other;
	globalconfig::KeyConfig m_key;
	DynamicConfig m_dynamicconfig;

};

#endif
