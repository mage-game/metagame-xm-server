#ifndef __ROLE_ZHUAN_ZHI_SKILL_HPP__
#define __ROLE_ZHUAN_ZHI_SKILL_HPP__

#include "obj/character/attribute.hpp"
#include "obj/character/character.h"
#include "config/zhuanzhiconfig.hpp"
#include <map>

class Role;
class RoleZhuanZhi;

// 转职主动技能真正id
//static const int ZHUANZHI_LEVLE_2_HURT_SKILL_ID = 180;
//static const int ZHUANZHI_LEVLE_2_RECOVER_SKILL_ID = 181;

class RoleZhuanZhiSkill
{

	struct VirSkillItem
	{
		short vir_skill_id = 0;										// 虚拟技能id
		short vir_skill_type = 0;									// 虚拟技能类型
		short follow_perform_skill_id = 0;							// 跟随主动技能id
		short level = 0;											// 层数
		unsigned int last_perform_skill_timestamp = 0;				// 技能上次触发时间戳
	};

public:
	RoleZhuanZhiSkill();
	~RoleZhuanZhiSkill();

	void Init(Role * role, RoleZhuanZhi *rolezhuanzhi);
	void OnInitFinish();

	void RecalcActivedSkill(bool is_init = false);											// 重算一下已激活技能列表
	bool IsActiveSkill(int skill_id);									// 是否激活

	VirSkillItem *GetVirSkillItem(int vir_skill_type);

	void OnHurtOther(Character *target, int skill_id, long long injure, int fight_type);					// 攻击

	void OnRoleBeenAttack(ObjID attacker, UInt16 skill_id, long long delta_hp, bool from_skill);			// 被攻击
	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);					// 被攻击(重算伤害)

	// 攻击触发---------------------------------------------------------------------
	void CheckKillOtherRole(bool is_kill);							   // 杀人

	void OnHurtOtherPVPUpShanghaiAttr(Character *target, long long base_injure);				// 技能特殊属性增加对玩家伤害
	void OnHurtOtherPVEUpShanghaiAttr(Character *target, long long base_injure);				// 技能特殊属性增加对怪物伤害

	void OnHurtOtherPVEUpShanghai(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherPVPUpShanghai(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherPVPXuanyun(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherPVPUpShanghai2(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherPVPUpShanghai3(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherPVPUpShanghai4(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherPVMUpShanghai(Character * target, int vir_skill_id, int skill_id, long long injure);
	void OnHurtOtherHurtInterval(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherHurt1(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherHurt2(Character * target, int vir_skill_id, int skill_id, long long injure);
	void OnHurtOtherHurt4(Character * target, int vir_skill_id, int skill_id, long long injure);
	void OnHurtOtherHurt5(Character * target, int vir_skill_id, int skill_id, long long injure);
	void OnHurtOtherPVPChenmo(Character * target, int vir_skill_id, int skill_id);
	void OnHurtOtherPVERecover(Character * target, int vir_skill_id, int skill_id, long long injure);

	// 受击触发---------------------------------------------------------------------
	void OnBeendAttackPVPDownShanghaiAttr(ObjID attacker, Attribute &delta_hp);					// 技能特殊属性减少玩家对自己伤害
	void OnBeendAttackPVEDownShanghaiAttr(ObjID attacker, Attribute &delta_hp);					// 技能特殊属性减少怪物对自己伤害

	bool IsMianyiDeadlyAttack();									   // 是否免疫致命的一击
	void OnBeenAttackNormalHpStore(int vir_skill_id, int skill_id);
	void OnBeenAttackWudiHpStore(int vir_skill_id, int skill_id);
	void OnBeenAttackPVPRebound(ObjID attacker, int vir_skill_id, int skill_id, long long delta_hp);
	void OnBeenAttackPVPDownShanghai(ObjID attacker, int vir_skill_id, int skill_id);

	// 自动触发---------------------------------------------------------------------
	void CheckBeenAttack(unsigned int now);
	void CheckRecover(time_t now_second);								// 回复
	bool CheckMainyiXuanyun();											// 免疫眩晕
	double GetAddExpPer();												// 增加经验

	// 主动触发---------------------------------------------------------------------
	/*暂时无用*/
	void OnPerformSkillReq(int vir_skill_id, ObjID target_id);
	void OnPerformRecover(int vir_skill_id);
	void OnPerformHurt3(int vir_skill_id, ObjID target_id);

	// -----------------------------------------------------------------------------
	int GetAddRangeEnemyNum() { return m_normal_aoe_add_target_num; }	

	void BroadcastSkillTrigger(int vir_skill_id, ObjID target_obj_id, long long injure = 0, bool send_to_role = false);

private:
	Role *m_role = nullptr;
	RoleZhuanZhi *m_rolezhuanzhi = nullptr;

	int m_max_cfg_skill_count = 0;									// 当前配置的最大技能数
	int m_normal_aoe_add_target_num = 0;							// 目前AOE目标增加人数

	int m_special_attr_pvp_jianshang = 0;							// 技能特殊属性pvp减伤 
	int m_special_attr_pvp_zengshang = 0;							// 技能特殊属性pvp增伤
	int m_special_attr_pve_jianshang = 0;							// 技能特殊属性pve减伤
	int m_special_attr_pve_zengshang = 0;							// 技能特殊属性pve增伤

	std::map<int, VirSkillItem> m_cur_active_skill_map;				// 所有当前激活的技能
};

#endif