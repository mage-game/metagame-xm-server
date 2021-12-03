#ifndef __SPECIAL_GONGCHENGZHAN_HPP__
#define __SPECIAL_GONGCHENGZHAN_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "config/logicconfigmanager.hpp"
#include "protocal/msgactivity.hpp"

enum GCZ_NOTICE_TYPE
{
	NOTICE_TYPE_PO_MENG = 0,	// ����
	NOTICE_TYPE_PO_QI,			// ����
};

enum GCZ_SITE_TYPE
{
	SITE_TYPE_GONG = 0,			// ����
	SITE_TYPE_SHOU = 1,			// �ط�
};

class SpecialGongChengZhan : public SpecialLogic
{
public:

	// ������
	struct JoinerItem
	{
		JoinerItem () 
		{
			this->Reset();
		}

		void Reset()
		{
			uid = 0;
			guild_id = 0;
			zhangong = 0;
			zhangong_reward_mark = 0;
			add_zhangong_time = 0;
			honor_reward = 0;
			gold_reward = 0;
			shengwang_reward = 0;
			attack_qi_hp = 0;
			kill_num = 0;
			lianzhan = 0;
			assist = 0;
			sos_times = 0;
			memset(role_name, 0, sizeof(role_name));
			guild_post = 0;
			hurt_roles = 0;
			kill_flags = 0;
			hurt_flags = 0;
		}

		int uid;
		GuildID guild_id;
		int zhangong;
		int zhangong_reward_mark;		// �Ѿ���ȡ��ս������
		unsigned int add_zhangong_time;
		int honor_reward;
		int gold_reward;
		int shengwang_reward;
		long long attack_qi_hp;
		int kill_num;
		int lianzhan;
		int assist;
		char sos_times = 0;
		GameName role_name;
		std::vector<ItemConfigData> reward_item_vec;
		std::map<UserID, time_t> hurt_map;													// �˺��ҵ� �û��б�
		char guild_post;
		long long hurt_roles;							//������˺�
		int kill_flags;									//��ɱ������
		long long hurt_flags;							//�������˺�
	};

	// ���빫��
	struct GuildItem
	{
		GuildItem() : guild_id(0), shouchen_time(0){}

		bool Invalid() { return 0 == guild_id; }

		int guild_id;
		unsigned int shouchen_time;
		std::vector<int> member_uid_vec;
		std::vector<JoinerItem*> member_joiner_vec;
	};

	// Ĥ����Ϣ
	struct WorshipItem
	{
		WorshipItem() :manual_worship_times(0), next_can_manual_worship_timestamp(0), manual_worship_reward_timestamp(0) {}

		Int32 manual_worship_times;					// Ĥ�ݴ���
		UInt32 next_can_manual_worship_timestamp;   // �´��ܹ�Ĥ�ݵ�ʱ���
		UInt32 manual_worship_reward_timestamp;     // Ĥ�ݸ��轱����ʱ���
	};

	SpecialGongChengZhan(Scene *scene);
	virtual ~SpecialGongChengZhan();

	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool CanDestroy() { return false; }
	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual void OnAttackCalcHurt(Character *cha, ObjID enemy_id, Attribute &delta_hp);
	virtual int GetSpecialParam(Role *role);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual void OnHurtOther(Character *cha, Character *target);

	bool GetRoleIsShouSite(Role *role);
	int GetRoleSite(Role *role);
	void OnActivityStatusChange(int status);
	bool IsPoCheng() { return m_is_po_cheng; }

	void OnGczChangePlace(Role *role, int place_type);
	bool OnSosCost(Protocol::CSSendGuildSosReq *req, Role *role);
	
	void SendGuildInfoStatistic(long long guild_id, int notify, Role *role = NULL);

	void SendResultInGuildChat(); // ����ʤ���������������

	bool IsWorshipDuring();
	void OnWorship(Role* role);
	void SendWorshipInfo(Role *role);
	void SyncWorshipStart(Role *role);
	void CheckWorshipReward(time_t now_second);

protected:
	void OnActivityStandy();
	void OnActivityStart();
	void OnActivityClose();
	
	void CreateBoss();
	void GongchengzhanInit();
	void AddZhangong(JoinerItem *joiner, int zhanggong);
	void ResetPos(Role *role);
	void ResetPosAll();
	bool IsPosInArea(const Posi *pos, const Posi *area_zuo_xia_pos, const Posi *area_you_shang_pos);

	void CalcGuildRank();
	void CalcPersonalRank();
	void CalcGuildPersonalRank();
	void CalcRoleZhangongWhenPoCheng(GuildID guild_id);

	void RewardZhangongForAPeriodOfTime(time_t now_second);
	void RefreshZhangong(time_t now_second);
	
	void SyncGlobalInfoToAll();
	int GetGuildRankPosByGuildId(GuildID guild_id);
	void SyncRoleInfoToAll();
	void SyncRoleInfo(Role *role);
	void SyncRewardInfo();
	void AddRewardInfo(JoinerItem *joiner_item, ItemConfigData reward_item);

	void AddRewardListInfo(JoinerItem * joiner_item, ItemConfigData * reward_item, int reward_count);

	void UpdateChenZhuInfo(Role *role);
	void UpdateChenZhuInfo(GuildID guild_id);
	void MailRewardOnActClose();

	void RewardOfZhangong(JoinerItem *joiner);

	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);
	void SendLuckyInfoToUser(Role *role = NULL);
	void GaveLuckyItemToUser();

	GuildItem *GetGuildItem(GuildID guild_id);
	JoinerItem *GetJoinerItem(int uid);

	bool IsShouRole(GuildItem *guild_item);

	void NotifySideSystemMsg(int notice_type);

	void CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist = false);
	void OnFlagBeAttacked(Monster *monster);

	void SendGuildInfoStatisticAll();
	void SyncGuildMvpInfo(GuildID guild_id, Role* role = NULL);

	void SyncFlagInfo(Role * role = NULL);

	bool m_is_finish;								// �����
	bool m_is_po_cheng;								// �Ƴ�
	int m_po_cheng_times;							// �ƳǴ���
	bool m_is_po_qiang;								// ��ǽ����һ�ι��ǲ���ǽ
	time_t m_last_zhangong_reward_time;		        // ÿ�μӻ30��ͽ���һ��ս��
	time_t m_refresh_zhangong_time;					// ˢ�¸���ս��������ʱ��
	unsigned int m_last_shoucheng_time;				// �ϴμ���سǵ�ʱ��
	int m_shou_guild_id;							// �سǷ�����id

	unsigned int m_last_check_shoucheng_time;		// �س�ʱ��
	unsigned int m_last_first_guild_broadcast_time;
	unsigned int m_last_best_killer_broadcast_time;

	std::map<int, GuildItem *> m_guild_map;
	std::map<int, JoinerItem *> m_joiner_map;
	std::vector<GuildItem *> m_guild_rank_vec;
	std::vector<JoinerItem *> m_personal_rank_vec; 

	std::map<int, long long> m_hurt_flag_map;

	Protocol::SCGczLuckyRewardInfo m_luck;

	bool m_flag_has_be_hit_notice_flag;				// �����Ƿ񱻻���
	int m_flag_low_hp_broadcast_flag;				// ���ӵ�Ѫ���㲥���
	char m_cheng_zhu_prof;
	unsigned int m_kick_out_all_role_time;			//�ӳ��߳�ʱ��

	std::map<UniqueUserID, WorshipItem> m_worship_user_map;  // Ĥ�ݽ�ɫ�б�<unique_uid, info>
	time_t m_worship_start_time;							// Ĥ�ݿ�ʼʱ��

	bool m_need_sync_flag_info;
	unsigned int m_last_sync_flag_info_time;
	long long m_flag_max_hp;					//�������Ѫ����
	long long m_flag_cur_hp;					//���ĵ�ǰѪ��
};

#endif // __SPECIAL_GONGCHENGZHAN_HPP__


