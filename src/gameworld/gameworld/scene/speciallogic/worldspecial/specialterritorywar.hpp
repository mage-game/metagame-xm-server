#ifndef __SPECIAL_TERRITORY_WAR_HPP__
#define __SPECIAL_TERRITORY_WAR_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "protocal/msgactivity.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/territorywarconfig.hpp"
#include "servercommon/activitydef.hpp"
#include "engineadapter.h"
#include <map>
#include <vector>

static const int CAR_SKILL_MAX_NUM = 2;							// 攻城车技能最大数量

class SpecialTerritoryWar : public SpecialLogic
{
public:

	//技能
	struct TerritoryWarSkillItem
	{
		TerritoryWarSkillItem() 
		{ 
			this->Reset();
		}

		void Reset()
		{
			skill_index = TW_SKILL_INVALID;
			last_perform_time = 0;
		}

		bool Isvalid()
		{
			return TW_SKILL_INVALID < skill_index && TW_SKILL_MAX > skill_index;
		}

		int skill_index;
		unsigned int last_perform_time;
	};

	//参与者
	struct JoinerInfo
	{
		JoinerInfo () 
		{
			this->Reset();
		}

		void Reset()
		{
			uid = 0;
			guild_id = 0;
			current_credit = 0;
			history_credit = 0;
			credit_reward_flag = 0;
			fetch_last_credit_reward_index = 0;
			kill_count = 0;
			assist_count = 0;
			death_count = 0;
			kill_building_count = 0;
			special_image_id = INVAILD_APPEARANCE;
			side = -1;
			attack_player_map.clear();
			ice_landmine_count = 0;
			fire_landmine_count = 0;
			lianzhan = 0;
		}

		void ResetSkillList()
		{
			for(int i = 0; i < CAR_SKILL_MAX_NUM; i++)
			{
				skill_list[i].Reset();
			}
		}

		void SetSkillList(int skll_id1, int skill_id2)
		{
			if (TW_SKILL_INVALID < skll_id1 && TW_SKILL_MAX > skll_id1)
			{
				skill_list[0].skill_index = skll_id1;
				skill_list[0].last_perform_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			}

			if (TW_SKILL_INVALID < skill_id2 && TW_SKILL_MAX > skill_id2)
			{
				skill_list[1].skill_index = skill_id2;
				skill_list[0].last_perform_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			}
		}

		bool IsValidSkill(int skill_id)
		{
			if (TW_SKILL_INVALID >= skill_id && TW_SKILL_MAX <= skill_id)
				return false;

			for(int i = 0; i < CAR_SKILL_MAX_NUM; i++)
			{
				if (skill_list[i].skill_index == skill_id)
					return true;
			}

			return false;
		}

		void SetSkillColdTime(int skill_id)
		{
			if (TW_SKILL_INVALID >= skill_id && TW_SKILL_MAX <= skill_id)
				return ;
			
			for(int i = 0; i < CAR_SKILL_MAX_NUM; i++)
			{
				if (skill_list[i].skill_index == skill_id)
				{
					const TerritroyWarSkillItemCfg * skill_cfg = LOGIC_CONFIG->GetTerritoryWarConfig().GetSkillItemCfg(skill_id);
					if (NULL == skill_cfg) return;

					skill_list[i].last_perform_time = static_cast<unsigned int>(EngineAdapter::Instance().Time() + skill_cfg->cd_s);
				}
			}
		}

		TerritoryWarSkillItem * GetSkillItem(int skill_id)
		{
			if (TW_SKILL_INVALID >= skill_id && TW_SKILL_MAX <= skill_id)
				return NULL;

			for(int i = 0; i < CAR_SKILL_MAX_NUM; i++)
			{
				if (skill_list[i].skill_index == skill_id)
					return &skill_list[i];
			}
			
			return NULL;
		}

		int uid;
		GuildID guild_id;
		int current_credit;					// 当前拥有积分
		int history_credit;					// 历史获得积分
		int credit_reward_flag;				// 积分奖励标志位
		int fetch_last_credit_reward_index; 
		short kill_count;					// 击杀玩家数
		short assist_count;					// 助攻击杀玩家数
		short death_count;					// 玩家死亡数
		char kill_building_count;			// 破塔数量
		char special_image_id;				// 购买攻城车形象
		short side;							// 排名单数的公会为1(红)，排名为双数的公会为0(蓝)
		short ice_landmine_count;			// 冰霜地雷数量
		short fire_landmine_count;			// 火焰地雷数量
		short lianzhan;						// 连杀数
		TerritoryWarSkillItem skill_list[CAR_SKILL_MAX_NUM];  // 攻城车技能列表

		std::map<int, unsigned int>attack_player_map;
		std::map<UserID, time_t> hurt_map;													// 伤害我的 用户列表
	};

	enum BIANSHEN_APPEARANCE
	{
		INVAILD_APPEARANCE = 0,
		CAR_FIGHT_APEARANCE,
		CAR_FANG_APEARANCE,
		CAR_CURE_APEARANCE,

		MAX_APPEARANCE,
	};

	// 建筑物
	struct BuildingInfo
	{
		BuildingInfo()
		{
			this->Reset();
		}

		void Reset()
		{
			building_id = 0;
			obj_id = INVALID_OBJ_ID;
			posi.x = 0;
			posi.y = 0;
			kill_guild_credit = 0;
			kill_personal_credit = 0;
			kill_assist_credit = 0;
			building_index = 0;
			assist_vaild_time = 0;

			tower_attack_man_per = 0;	
			tower_attack_fight_car_per = 0;
			tower_attack_fang_car_per = 0;
			tower_attack_cure_car_per = 0;

			side = 2;

			preposition_building_1 = 0;
			preposition_building_1 = 0;

			building_state = 0;
			max_hp = 0;
			current_hp = 0;
			attack_player_map.clear();
		}
	
		int building_id;
		ObjID obj_id;
		Posi posi;
		short kill_guild_credit;			// 击杀可获得公会积分
		short kill_personal_credit;			// 击杀可获得个人积分
		short kill_assist_credit;			// 击杀可获助攻积分
		int building_index;					// 建筑索引
		unsigned int assist_vaild_time;		// 助攻有效时间

		int tower_attack_man_per;			// 塔打人固定伤害万分比
		int tower_attack_fight_car_per;		// 塔打车（攻）固定伤害万分比
		int tower_attack_fang_car_per;		// 塔打车（防）固定伤害万分比
		int tower_attack_cure_car_per;		// 塔打车（治）固定伤害万分比
		
		short side;							// 1为红方塔，0为蓝方塔

		int preposition_building_1;			// 前置怪物1
		int preposition_building_2;			// 前置怪物2

		char building_state;						

		Attribute max_hp;
		Attribute current_hp;

		std::map<int, unsigned int>attack_player_map;		// 记录攻击过玩家的名单
	};

	struct GuajiMonsterInfo
	{
		GuajiMonsterInfo():monster_id(0), kill_credit_reward(0){}

		int monster_id;
		short kill_credit_reward;
	};

	enum ENBUILDINGSTATUS
	{
		ENBUILDINGSTATUS_NONEXIST = 0,		// 不存在
		ENBUILDINGSTATUS_EXIST,				// 存在
	};

	enum ENSIDESTATUS
	{
		ENSIDESTATUS_BLUE= 0,				// 蓝方					
		ENSIDESTATUS_RED,					// 红
	};

	SpecialTerritoryWar(Scene *scene);
	virtual ~SpecialTerritoryWar();

	virtual bool CanDestroy();
	virtual void Update(unsigned long interval, time_t now_second);

	void OnActivityStatusChange(int status);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);

	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual void OnHurtOther(Character *cha, Character *target);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);

	virtual bool PerformSkill(Role *role, int skill_index, const Posi &pos, ObjID target_id);

	virtual bool SpecialCanMountOn(Role *role) { return false; }
	virtual long long SpecialRecalcInjure(Character *attacker, Character *injurer, long long injure);
	virtual bool SpecialCanPassiveSkillEffect(Character *deliver, ObjID target, int skill_id);

	virtual void OnTrigger(Role *role, Trigger *trigger, int trigger_action_param_list[TRIGGER_ACTION_PARAM_MAX], bool *remove);

	void OnReliveShopBuyReq(Role *role, short goods_id);
	void OnFightShopBuyReq(Role *role, short type, short goods_id);

	JoinerInfo *GetJoinerInfo(int uid);
	bool IsBuilding(int building_id);
	short GetGuajiMonsterCredit(int monster_id);

	int GetSpecialParam(Role *role);
	void NotifySystemMsg(char *str_buff, int str_len, int notice_type);
	void NotifySideSystemMsg(char *str_buff, int str_len, int notice_type, short side);
	void NotifyImage(Role *role);
	
	void OnUseLandMine(Role *role, int type, Posi place_posi);

	bool IsActivityFinish(){return m_is_finish;}

protected:

	void OnActivityStandby();
	void OnActivityStart();
	void OnActivityClose();

	void CalcRewardOnActClose();

	void InitBudingMap();
	void InitGuajiMonsterMap();
	void CreateAllBuilding();
	bool CreateOneBuilding(BuildingInfo &building_info);

	void DoPerformSkill(Role *role, const TerritroyWarSkillItemCfg *skill_cfg, ObjID target_id);

	void SyncGlobalInfoToAll();
	void SyncGlobalInfo(Role *role);
	void SyncRoleInfo(Role *role);

	void AllAddCredit();
	void PersonalAddCredit(Role *role, int add_person_credti);
	void PersonalAddCredit(int uid, int add_person_credti);
	void AddGuildCredit(Role *role, int add_guild_credit);

	void CheckCreditReward(Role *role);
	void SetCreditRewardFlag(JoinerInfo *joiner, int reward_index) {joiner->credit_reward_flag |= (1 << reward_index);}
	bool IsGetCreditReward(JoinerInfo *joiner, int reward_index) { return 0 != (joiner->credit_reward_flag & (1 << reward_index));}

	void BuyTowerWudi(Role *role);
	void BuyTransport(Role *role);
	void BuyTransportToCenter(Role *role);

	void BuyCar(Role *role,short goods_id);
	void BuyMedicine(Role *role,short goods_id);
	void BuyLandMine(Role *role,short goods_id);

	void AddEdgeMedicineEffect(Role *role);
	void ClearEdgeMedicineEffect(Role *role);

	void SendLuckyInfoToUser(Role *role = NULL);
	void GaveLuckyItemToUser();

	void BroadcastZhanChang(time_t now_second);
	int GetBestKiller();

	void CheckLianZhanNotice(Role *kill_role, Role *dead_role, bool is_assist = false);

	bool m_is_finish;
	
	int m_red_total_credit;		// 红方公会总积分
	int m_blue_total_credit;	// 蓝方公会总积分

	unsigned int m_read_next_can_buy_tower_wudi;	// 记录下一可以购买塔无敌的时间，公会成员共享
	unsigned int m_blue_next_can_buy_tower_wudi;

	int red_building_survive_flag;		// 红方建筑存活状态，1表示已死亡，0表示存活，1-12位表示
	int blue_building_survive_flag;		// 蓝方建筑存活状态，1表示已死亡，0表示存活，1-12位表示

	GuildID m_center_relive_state;			// 中央复活点归属
	char m_winner_side;						// 胜利一方，-1表示没有胜利方，1红方胜利，0蓝方胜利
	unsigned int m_add_credit_interval;		// 集体加积分时间间隔
	unsigned int m_next_add_credit_time;	// 下一集体加积分时间

	int center_relive_point_id;
	int red_fortress_id;
	int blue_fortress_id;

	unsigned int m_building_be_attack_notice_interval;
	unsigned int m_red_next_notice_building_be_attack_time;
	unsigned int m_blue_next_notice_building_be_attack_time;
	unsigned int m_center_point_next_notice_building_be_attack_time;

	unsigned int m_last_first_guild_broadcast_time;
	unsigned int m_last_best_killer_broadcast_time;

	std::map<int, JoinerInfo *> m_joiner_map;
	std::map<int, BuildingInfo> m_building_map;
	std::map<int, GuajiMonsterInfo> m_guaji_monster_map;

	Protocol::SCTwLuckyRewardInfo m_luck;
};

#endif // __SPECIAL_TERRITORY_WAR_HPP__
