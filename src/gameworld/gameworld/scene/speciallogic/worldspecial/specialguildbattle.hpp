// #ifndef __SPECIAL_GUILD_BATTLE_HPP__
// #define __SPECIAL_GUILD_BATTLE_HPP__
// 
// #include "scene/speciallogic/speciallogic.hpp"
// #include "protocal/msgactivity.hpp"
// #include "config/logicconfigmanager.hpp"
// #include "servercommon/activitydef.hpp"
// #include <map>
// #include <vector>
// 
// class SpecialGuildBattle : public SpecialLogic
// {
// public:
// 	// 参与仙盟
// 	struct GuildItem
// 	{
// 		GuildItem() : guild_id(0), guild_credit(0), credit_change_time(0){}
// 
// 		bool Invalid() { return 0 == guild_id; }
// 
// 		int guild_id;
// 		int guild_credit;
// 		unsigned int credit_change_time;
// 	};
// 
// 	//参与者
// 	struct JoinerItem
// 	{
// 		JoinerItem () 
// 		{
// 			this->Reset();
// 		}
// 
// 		void Reset()
// 		{
// 			uid = 0;
// 			guild_id = 0;
// 			kill_role_num = 0;
// 			kill_credit_reward_flag = 0;
// 			history_get_person_credit = 0;
// 			history_get_guild_credit = 0;
// 			husong_goods_id = 0;
// 			personal_credit_reward_flag = 0;
// 			lianzhan = 0;
// 			assist = 0;
// 			fetch_last_personal_reward_index = 0;
// 			is_addhudun = false;
// 		}
// 
// 		bool IsFetchCredit(int reward_index) 
// 		{
// 			return 0 != (kill_credit_reward_flag & (1 << reward_index)); 
// 		}
// 
// 		void SetFetchCreditFlag(int reward_index) 
// 		{
// 			kill_credit_reward_flag |= (1 << reward_index); 
// 		}
// 
// 		bool IsFetchPersonalCreditReward(int reward_index)
// 		{
// 			return 0 != (personal_credit_reward_flag & (1 << reward_index)); 
// 		}
// 
// 		void SetFetchPersonalRewardFlag(int reward_index) 
// 		{
// 			personal_credit_reward_flag |= (1 << reward_index); 
// 		}
// 
// 		int uid;
// 		GuildID guild_id;
// 		int kill_role_num;
// 		int kill_credit_reward_flag;
// 		int history_get_person_credit;
// 		int history_get_guild_credit;
// 		int husong_goods_id;
// 		int personal_credit_reward_flag;
// 		int lianzhan;
// 		int assist;
// 		short fetch_last_personal_reward_index;
// 		bool is_addhudun;
// 		std::map<UserID, time_t> hurt_map;												// 伤害我的 用户列表	
// 	};
// 
// 	struct RankItem
// 	{
// 		RankItem() : guild_id(0), credit(0), score_change_time(0)
// 		{
// 		}
// 
// 		RankItem(GuildID _guild_id, int _credit, unsigned int _score_change_time):guild_id(_guild_id), 
// 			credit(_credit), score_change_time(_score_change_time)
// 		{
// 		}
// 
// 		GuildID guild_id;
// 		int credit;
// 		unsigned int score_change_time;
// 	};
// 
// 	enum HUSONG_APPEARANCE
// 	{
// 		INVAILD_APPEARANCE = 0,
// 		GLOD_BOX_APPEARANCE,
// 		SLIVER_BOX_APPEARANCE,
// 		COMMON_BOX_APPEARANCE,
// 		TOKEN_APPEARANCE,
// 
// 		MAX_APPEARANCE,
// 	};
// 
// 	SpecialGuildBattle(Scene *scene);
// 	virtual ~SpecialGuildBattle();
// 
// 	void OnActivityStatusChange(int status);
// 
// 	virtual bool CanDestroy();
// 	virtual void Update(unsigned long interval, time_t now_second);
// 
// 	void CheckBornBoss(time_t now_second);
// 	void CheckRareMonster(time_t now_second);
// 	void CheckBornGather(time_t now_second);
// 
// 	virtual bool SpecialCanMountOn(Role *role) { return false; }
// 
// 	virtual void OnRoleEnterScene(Role *role);
// 	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
// 
// 	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
// 
// 	virtual bool SpecialRelive(Role *role, int realive_type);
// 	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
// 
// 	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
// 	virtual void OnHurtOther(Character *cha, Character *target);
// 
// 	GuildItem *GetGuildItem(GuildID guild_id);
// 	JoinerItem *GetJoinerItem(int uid);
// 
// 	void OnAddHuDun(Role *role);
// 	void AddHuDun(Role *role);
// 	void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);
// 	virtual int GetSpecialParam(Role *role);
// 	void CalcSubmitGoodsScore(Role *role);
// 
// protected:
// 	void AddObj(int gather_id ,Posi pos);
// 
// 	bool RandDropGoods(Posi posi,int gather_id);
// 	bool RandCreateRareMonster(int monster_id);
// 	void RemoveAllGoods();
// 
// 	void OnActivityStandby();
// 	void OnActivityStart();
// 	void OnActivityClose();
// 
// 	void CalcKillOtherScore(Role *role);
// 	void CalcGuildRank();
// 	int FindGuildRank(GuildID guild_id);
// 
// 	void CalcRewardOnActClose();
// 
// 	void SyncGlobalInfoToAll();
// 	void SyncGlobalInfo(Role *role);
// 	void SyncRoleInfo(Role *role);
// 	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);
// 	void NotifyImage(Role *role);
// 
// 	void AddMoveSpeedEffect(Role *role,int gather_id);
// 	void ClearSpeedEffect(Role *role);
// 
// 	void CheckPersonalScoreReward(Role *role);
// 
// 	void SendLuckyInfoToUser(Role *role = NULL);
// 	void GaveLuckyItemToUser();
// 
// 	int GetBestKiller();
// 	void CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist = false);
// 
// 	bool m_is_finish;
// 	bool m_boss_is_alive;
// 	
// 	unsigned int m_next_calc_time;
// 	int m_cur_rare_monster_num;
// 	unsigned int m_husong_end_time;
// 	short m_glod_box_total_num;
// 	short m_sliver_box_total_num;
// 
// 	bool m_credit_is_change;
// 	std::map<int, GuildItem *> m_guild_map;
// 	std::map<int, JoinerItem *> m_joiner_map;
// 	std::vector<RankItem> m_guild_rank_vec;
// 	std::set<ObjID> m_gather_list;
// 
// 	int m_glod_box_id;
// 	int m_sliver_box_id;
// 	int m_common_box_id;
// 	int m_token_id;
// 	int m_refresh_rare_monster_max_num;
// 
// 	unsigned int m_boss_refresh_time;
// 	unsigned int m_rare_monster_refresh_time;
// 
// 	unsigned int m_next_notice_boss_refresh_time;
// 	unsigned int m_next_notice_rare_monster_refresh_time;
// 
// 	unsigned int m_last_first_guild_broadcast_time;
// 	unsigned int m_last_best_killer_broadcast_time;
// 
// 	Protocol::SCGBLuckyRewardInfo m_luck;
// };
// 
// #endif // __SPECIAL_GUILD_BATTLE_HPP__
