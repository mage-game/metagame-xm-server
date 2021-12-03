/******************************************************************************************
FileName: speciallogic.hpp
Author: dreamwang
Description: 场景特殊逻辑接口规范 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/09/02/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __SPECIAL_LOGIC_HPP__
#define __SPECIAL_LOGIC_HPP__

#include <stdio.h>

#include "iclockmodule.h"

#include "servercommon/serverdef.h"
#include "speciallogicdef.hpp"
#include "servercommon/triggerdef.h"
#include "scene/scene.h"
#include "scene/map.h"
#include "scene/changescenedelayer.h"

class Scene;
class Obj;
class Character;
class Role;
class RoleShadow;
class BigCoinObj;
class Trigger;

class SceneSkillBase;
class SceneEffectBase;

class SpecialLogic
{
public:
	SpecialLogic(Scene *scene);
	virtual ~SpecialLogic();

	int GetSceneType() { return m_scene->GetSceneType(); }
	int GetSceneKey() { return m_scene->GetSceneKey(); }
	int GetSceneId()  { return m_scene->GetSceneID(); }
	Scene * GetScene() { return m_scene; }
	
	static SpecialLogic * CreateSpecialLogic(Scene *scene, int speciallogic_type);
	static void DestorySpecialLogic(SpecialLogic *speciallogic);

	virtual void Update(unsigned long interval, time_t now_second) { this->UpdateSceneEffectList(interval, now_second); }
	virtual void OnDayChange(unsigned int old_dayid, unsigned int now_dayid) {}

	virtual void OnAddObj(Obj *obj) {}
	virtual void OnRemoveObj(Obj *obj) {}

	virtual void OnAttackCalcHurt(Character *cha, ObjID enemy_id, Attribute &delta_hp);
	virtual void OnAttack(Character *cha, ObjID enemy_id, Attribute delta_hp);
	virtual void OnHurtOther(Character *cha, Character *target);
	virtual void OnStopAttack(Character *cha) {}
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);

	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout) {}			// 这个接口主要是在切换场景的时候 要保存角色数据之前统一回调的
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos) {}
	virtual void OnStartGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos) {}
	virtual void OnAddExp(Role *role, long long add_exp, int reason) {}
	virtual void OnSkillEffect(Character *deliver, int special_type, int param1, int param2) {}
	virtual void OnSpecialGuWu(Role *role, bool is_gold);

	virtual bool PerformSkill(Role *role, int skill_index, const Posi &pos, ObjID target_id) { return false; }

	virtual void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper) {}
	virtual void OnRolePickBigCoin(Role *role, BigCoinObj *bigcoin) {}
	virtual void OnTrigger(Role *role, Trigger *trigger, int trigger_action_param_list[TRIGGER_ACTION_PARAM_MAX], bool *remove) {}
	virtual int  OnAOITriggerEnter(Role *role){return -1;};
	virtual void OnAOIRoleEnter(Role *role, Role *target_role) { return; }

	virtual bool CanDestroy() { return true; }

	virtual bool SpecialCanTransport(Role *role) { return true; }
	virtual bool SpecialRelive(Role *role, int realive_type);
	virtual bool SpecialIsEnemy(Role *role, Obj *obj);
	virtual bool SpecialIsFriend(Role *role, Obj *obj, bool check_alive = true);
	virtual bool SpecialIsCostMp(Role *role) { return true; }
	virtual void SpecialCancelMonsterStatic() {}

	virtual bool SpecialCanGather(Role *role, GatherObj *gather) { return true; }
	virtual bool SpecialCanPick(Role *role, FallingItem *falling_item) { return true; }
	virtual bool SpecialCanSupplyEffect(Role *role, int supply_type) { return true; }
	virtual bool SpecialCanRecoverHp(Role *role){return true;}
	virtual bool SpecialCanUseItem(Role *role, ItemID item_id) { return true; }
	virtual bool SpecialCanFlyByShoe(Role *role) { return true; }
	virtual bool SpecialCanMountOn(Role *role) { return true; }
	virtual bool SpecialCanRest(Role *role) { return true; }
	virtual bool SpecialCanReAlive(Role *role, int realive_type) { return true; }
	virtual bool SpecialCanPerformSkill(Role *role) { return true; }
	virtual bool SpecialCanPerformCommonSkill(Role *role) { return true; }
	virtual bool SpecialStopGahterOnAttack(Role *role) { return true; }
	virtual bool SpecialStopGahter(Role *role) { return true; }		// 用于无敌采集（被攻击、眩晕等状态都不中断）
	virtual bool SpecialCanCampWeekBuffEffect(Role *role) { return true; }
	virtual bool SpecialCanGuWu() { return false; }  //是否可鼓舞，加buff
	virtual bool SpecialCanRcoverHpByMedicine() { return true; } //是否可以通过药回复血量
	virtual bool SpecialCanRebound(Role *role, Character *target) { return true; }

	virtual bool OnTimeout() { return false; }
	virtual void OnLeaveFB(Role *role) {}

	virtual void Reset() {}

	virtual int GetObjSide(ObjID id);
	virtual int GetSpecialParam(Role *role) { return 0; }
	virtual int GetRoleShadowSpecialParam(RoleShadow *roleshadow) { return 0; }
	virtual int GetSpecialExpAdd(Role *role, int exp_addon_reason) { return 0; }
	virtual long long SpecialRecalcMonsterExp(Role *role, Monster *monster, long long add_exp) { return add_exp; }
	virtual int SpecialRecalcMonsterYuanli(Role *role, Monster *monster, int add_yuanli) { return add_yuanli; }
	virtual long long SpecialRecalcHate(Role *role, int skill_id, long long hate) { return hate; }
	virtual long long SpecialRecalcInjure(Character *attacker, Character *injurer, long long injure) { return injure; }
	virtual bool SpecialCanPassiveSkillEffect(Character *deliver, ObjID target, int skill_id){return true;}

	void AddSceneEffect(SceneEffectBase *scene_effect);
	void DelSceneEffect(ObjID deliver_objid, int product_id, int product_method);

	void DelayKickOutRole(Role *role, int reason = CHANGE_SCENE_REASON_INVALID);
	void DelayKickOutAllRole(int reason = CHANGE_SCENE_REASON_INVALID);

	void AllRoleReLive();

	bool IsAllMonsterDead();
	void KillAllMonster();
	int GetPassTime();
	short GetKillAllMonster();
	short GetKillBoss();
	void CheckSceneMonsterStatus(unsigned long interval, time_t now_second);
	void NotifyBossHPPercent(unsigned short monster_id);	

protected:
	virtual void OnAllMonsterDead() {}

	void UpdateSceneEffectList(unsigned long interval, time_t now_second);
	void SendUserSceneSkillStatus(Role *role);

	void TriggerFBPass();

	bool CheckSkillRange(Role *role, ObjID target_objid, int skill_index, int skill_range) { return true; }
	void AutoPickFallingItem(Role *role);

	Scene *m_scene;
	unsigned int m_last_check_monster_status_time;								// 上次检查场景状态时间

	typedef std::vector<SceneEffectBase*> SceneEffectVector;
	typedef std::vector<SceneEffectBase*>::iterator SceneEffectVectorIt;
	SceneEffectVector m_scene_effect_vec;										// 场景effect
};	

#endif  // __SPECIAL_LOGIC_HPP__

