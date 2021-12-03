#ifndef __SHENSHOU_HPP__
#define __SHENSHOU_HPP__

#include "servercommon/shenshoudef.hpp"
#include "obj/character/attribute.hpp"
#include <vector>

class Role;
class Character;

class Shenshou
{
	static const int MAX_SKILL_ADDTION_TIMES = 10;	// 最大相同技能叠加次数（仅内存中使用，以后可以根据需要加大）

	struct SkillItem
	{
		int skill_type;
		int level;
		unsigned int last_perform_time;				// 上一次触发技能的时间
	};

	enum SKILL_PARAM
	{
		PARAM1 = 0, PARAM2, PARAM3, PARAM4, PARAM5, PARAM6, PARAM7,
		PARAM_COUNT,
	};

public: 
	Shenshou();
	~Shenshou();

	//void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const ShenshouParam &Shenshou_param); 
	void GetInitParam(ShenshouParam *shenshou_param) const;

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ReCalcSpecialRoleBigGoalAttr();                    // 激活人物大目标属性加成   只加神兽及装备，不算装备等级
	void OnRoleBeenAttack(ObjID attacker);
	void OnHurtOther(Character *target, long long injure);

	bool IsMianYiXuanYun();
	bool IsMianYiChenMo();

	long long RecalcExtraInjureById(Character *target, long long injure, int skill_id);
	void RecalcCD(int skill_id, int cold_down_times, int &reduce_cd) const;

	//bool TransformFromKnapsackItem(UInt16 item_base_id, int item_base_num, short item_id, short strength_level, bool is_rand_start, int start_num , const char *reason);
	int PutOnEquip(ItemDataWrapper &item_wrapper, int shou_id);
	bool TakeOffEquip(int shenshou_id,int slot_index,bool is_puton);
	void OnHandlerOpera(short opera_type, short param_1, short param_2, short param_3, int param_4);
	void OnGMReq(int type, int param1, int param2, int param3);
	void UpgradeEquip(short shenshou_id, short equip_index, short destroy_num, short destroy_backpack_index_list[SHENSHOU_MAX_BACKPACK_COUNT], bool use_double_shuliandu = false);

	int GetSkill10HurtPercent();
	int GetSkill12HurtPercent();
	void GetSkill12HurtNum(int &num);
	void GetSkill10DecSpeedInterval(int &Interval);

	void CheckFirstHuanlingRefresh();
	void AddHunalingScore(int add_score);
	void OnDayChange(int old_dayid, int now_dayid);
	void OnRoleLevelChange(int level);
	void OnCompleteTask(TaskID task_id);

	int GetActiveCount();  // 获取激活个数
	int GetEquipCount(int quality);  // 获取穿戴个数

	bool IsActiveShenshou(int shenshou_id) const;

protected:
	//bool AddItem(ShenshouBackpackItem &tmp_item, const char *reason, int num = 1, bool is_send = true, char is_full_backpack = 0);											// 增加物品
	void RecalcSkillAttrForSingleShou(const CharIntAttrs & char_int_attrs, CharIntAttrs & char_extra_attrs, int skill_id, const int param_list[]);			// 基于单个战神兽属性来增加属性
	void RecalcSkillAttrForAllZhuzhanShou(const CharIntAttrs & char_int_attrs, CharIntAttrs & char_extra_attrs, int skill_id, const int param_list[]);		// 基于全体助战神兽属性来增加属性
	static void RecalcSkillAttrForRoleBaseAttr(CharIntAttrs & role_attrs, int skill_id, const int param_list[]);			// 基于主角基础属性来增加属性
	void RecalcSkillAttrForRoleAttr(CharIntAttrs & role_attrs, int skill_id, const int param_list[]);  // 基于角色全身属性增加
	static void CalcRandomAttr(const CharIntAttrs & char_int_attrs, CharIntAttrs & char_extra_attrs, CharIntAttrs & role_attrs, int attr_type, int attr_value);			// 基于神兽基础属性来计算额外属性加成（用于装备随机属性）

	
	//bool EmptyBackpackNumMoreThan(int check_num) const;
	static int GetAttrType(int Shenshou_attr_type);
	//void PutOnEquip(int shou_id, int backpack_index, int slot_index);
	//void TakeOffEquip(int shou_id, int slot_index);
	void Zhuzhan(int shou_id);
	void AddZhuzhan();
	void Compose(int v_item_id, int index_1, int index_2, int index_3);
	int GetHurtDecPer();
	int GetHurtIncPercent();

	void HuanlingRefresh(bool is_first_refresh = false);
	void HuanlingDraw();

	bool CheckOpengameDay() const;
	void CheckFuncOpen();
#ifdef _DEBUG
	bool AutoTest();
#endif
	//void SendBackpackAdditionInfo(int backpack_index);
	//void SendBackpackInfo();
	void SendShenshouInfo(int shenshou_id = 0);
	void SendShenshouBaseInfo();
	void SendShenshouHuanlingInfo();
	void SendShenshouHuanlingDrawResult(int seq);
	void SpecialSkillDeal();
	void RecalcSkillList(int shou_id = 0, bool is_new_active = true);					// 根据当前所有神兽信息重算助战技能

private:
	Role *m_role;
	ShenshouParam m_param;
	CharIntAttrs m_attrs_add;															// 各属性加成数值
	
	//bool m_backpack_durty_list[SHENSHOU_MAX_BACKPACK_COUNT];

	std::map<int, std::vector<SkillItem> > m_shenshou_active_skill_list;				// 所有神兽当前激活的技能
	std::map<int, unsigned int> m_skill_last_perform_time_map;							// 公共CD的技能，释放时间
	std::map<int, int> m_stength_skill_map;

	int m_cur_zhuzhan_count;															// 当前助战神兽个数（动态维护，不存盘）
	int m_cd_reduce_percent;
};

#endif // __SHENSHOU_HPP__



