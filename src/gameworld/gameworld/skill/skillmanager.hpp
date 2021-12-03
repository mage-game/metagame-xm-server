#ifndef __SKILLMANAGER_HPP__
#define __SKILLMANAGER_HPP__

#include "skilldef.hpp"

class Character;
class SkillBase;
class Role;

#include "gamedef.h"
#include "servercommon/struct/skilllistparam.h"
#include "obj/character/attribute.hpp"

static const int MAX_PROF = 4;
static const int SKILL_COUNT = 5;
static const int SKILL_ID_LIST[MAX_PROF][SKILL_COUNT] = 
{
	{111, 121, 131, 141, 101},
	{211, 221, 231, 241, 201},
	{311, 321, 331, 341, 301},
	{411, 421, 431, 441,401},
};
static const int PASSIVE_SKILL_COUNT = 7;
static const int PASSIVE_SKILL_ID_LIST[PASSIVE_SKILL_COUNT]=
{
	41,42,43,44,45,46,47
};

// 101、201、301、401为不同职业必杀技id
// 上面写死了主动技能id和必杀技id, 注意不要漏了GetRoleActiveSkillList里的修改


//增加技能释放距离判断. 客户端走到释放临界位置就会释放, 对方在释放前摇移动了就会频繁失败
static const int PERFORM_SKILL_ADD_CHECK_DISTANCE = 5;

class SkillManager
{
public:
	struct SkillItem 
	{
		SkillItem() : skill_id(0), level(0), last_perform_time(0), exp(0) {}
		
		bool Invalid() const { return 0 == skill_id; }
		void Reset() { skill_id = 0; level = 0; last_perform_time = 0; exp = 0; }
		
		UInt16 skill_id;
		int level;
		unsigned int last_perform_time;
		int exp;
	};

	SkillManager();
	~SkillManager();

	void Init(Character *character, const SkillListParam &skill_list, const SkillOtherParam &skill_other_param);
	void GetInitParam(SkillOtherParam *skill_other_param);

	void Update(time_t now_second);

	void InitDefaultSkill();

	int GetDefaultSkill() const { return m_default_skill_index; }
	bool NextIsDefaultSkill() { return m_next_skill_index == m_default_skill_index; }
	bool NextIsChongfengSkill();
	bool NextIsNuqiSkill();
	bool NextValidInSaveArea();
	bool NextStopActionwhenPerform();
	bool SetNextSkill(int next_skill_index, const Posi &pos, ObjID target);
	bool GetSkillByID(UInt16 skillid, SkillItem *skill_item = NULL);
	bool GetSkillByID(UInt16 skillid, int *skill_index = NULL);
	int GetSkillCostMp(int skill_index);
	int GetSkillTotalLevel();

	int GetNextSkillData() { return m_next_skill_data; }
	void SetNextSkillData(int next_skill_data) { m_next_skill_data = next_skill_data; }

	bool CheckNextSkillDistance(Character *target, Posi *pos = NULL);
	bool CheckNextSkillCoolDown();
	bool CheckSkillCoolDown(int skill_index);
	bool CheckSkillTargetEnemy(int skill_index);
	bool CheckSkillAttackCharacter(int skill_index);
	bool CheckNextSkillAttackCharacter();
	bool CanPerformRoleSkill(char prof, int skill_index);
	bool CheckPerformRoleHp(int skill_index);
	
	Character * GetNextSkillTarget();
	void PerformNextSkill(Character *target, int special_param = 0);

	void GetSkillList(SkillListParam *p);
	int GetRoleActiveSkillList(SkillListParam *p);
	void GetChangeSkillList(SkillListParam *p);
	void ClearDirtyMark();

	void SendSkillListInfo(bool is_init = false);
	void SendOtherSkillInfo();

	bool AddSkill(UInt16 skill_id, int skill_level, int *out_skill_index = NULL, bool is_init = false, bool is_notify = true);
	void AddSkillByTask(Role *role, int prof, int skill_num);
	bool RemoveSkill(UInt16 skill_id, bool is_notify = true);
	bool GmSetSkillLevel(UInt16 skill_id, int skill_level);
	bool AddJingLingSkill(UInt16 skill_id, int skill_level);
	bool AddMojieSkill(UInt16 skill_id, int skill_level, int skill_type);
	bool AddXiannvSkill(UInt16 skill_id, int skill_level);

	bool AddPetSkill(UInt16 skill_id, int skill_level);
	bool AddMagicCardSkill(UInt16 skill_id);
	bool AddEquipmentSkill(UInt16 skill_id, int skill_level);

	void ReduceSkillCD(UInt16 skill_id, int reduce_percent);
	
	int GetNextSkillIndex() { return m_next_skill_index; }
	const SkillItem * GetSkillItem(int skill_index);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

	void ClearAllCD();
	void StartCD(int skill_index);

	int GetHurtIncPercent();
	int GetHurtDecPercent();

	void OnChangeProf(char old_prof);
	void OnZhuanzhi();

	void OnAttack(Character *attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill);
	void OnHurtOther(Character *target, int skill_id, long long &injure, int fight_type);

	void OnRoleLevelChange(int role_level);

	static bool IsRoleSkill(int skill_id);
	static bool IsNormalAttackSkill(int skill_id);								// 是否是角色普攻
	static bool IsKillAttackSkill(int skill_id);								// 是否是角色必杀
	long long CalcSkillHurtByHpPer(Character *target, int skill_id, bool is_main_target);			//获取角色技能上限伤害
	long long CalcSkillBaodiHurtByHpPer(Character *target, int skill_id, bool is_main_target);		//获取角色技能保底伤害

	bool IsCommonSkill(int skill_id);

	static bool IsGreateSoldierSkill(int skill_id);

private:
	void SendSkillInfo(int skill_index);

	void PerformSkillHelper(int skill_index, Character *target, const Posi &skill_pos, int special_param);
	void DelayPerformSkill(int skill_index, Character *target, const Posi &skill_pos, int special_param, int delay_time_ms);
	void OnDelayTimer(unsigned long interval);
	void CancelTimer();

	Character *m_character;

	SkillItem m_skill_list[SkillListParam::MAX_SKILL_NUM];

	bool m_dirty_mark[SkillListParam::MAX_SKILL_NUM];
	bool m_old_state[SkillListParam::MAX_SKILL_NUM];
	bool m_is_change;

	int m_default_skill_index;

	// 存储预输入技能索引值（释放技能会存储一个预输入的技能）
	int m_next_skill_index;
	Posi m_next_skill_pos;
	ObjID m_next_skill_target;
	int m_next_skill_data;

	CharIntAttrs m_attrs_add;
	int m_capablity;

	SkillOtherParam m_skill_other_param;

	// 角色读条技能处理
	bool m_is_addtimer;
	bool m_is_delay_perform_skill;
	long m_delay_perform_time_ms;
	int m_delay_skill_index;
	ObjID m_delay_skill_target_id;
	Character *m_delay_skill_target;
	Posi m_delay_skill_pos;
	int m_delay_special_param;
};
#endif

