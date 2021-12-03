#ifndef __SCENE_EFFECT_BASE_HPP__
#define __SCENE_EFFECT_BASE_HPP__

#include "iclockmodule.h"

#include "servercommon/serverdef.h"

#include "gamedef.h"

class Scene;
class Obj;

class SceneEffectBase
{
public:
	enum SCENE_EFFECT_TYPE
	{
		SCENE_EFFECT_TYPE_INVALID = 0,

		SCENE_EFFECT_TYPE_SPECIAL_FAZHEN,					// 法阵
		SCENE_EFFECT_TYPE_HURT,								// 纯伤害
		SCENE_EFFECT_TYPE_TOWERDEFEND_FAZHEN,				// 塔防法阵
		SCENE_EFFECT_TYPE_SKILL_121,						// 技能121法阵
		SCENE_EFFECT_TYPE_SKILL_421,						// 技能421法阵
		SCENE_EFFECT_TYPE_CLIENT,							// 客户端技能特效，只供显示
		
		SCENE_EFFECT_TYPE_MAX
	};
	
	SceneEffectBase(Scene *scene, ObjID deliver_objid, UInt16 product_id, char product_method);
	virtual ~SceneEffectBase();

	virtual bool Effect(unsigned long interval, time_t now_second) { return false; }
	virtual char GetSceneEffectType() const { return SCENE_EFFECT_TYPE_INVALID; }
	virtual void DelEffectObj() { }
	
	ObjID GetDeliverObjid() { return m_deliver_objid; }
	UInt16 GetProductid() { return m_product_id; }
	char GetProductMethod() { return m_product_method; }

protected:
	virtual bool IsFriend(Obj *obj, bool check_alive = true) { return false; }
	virtual bool IsEnemy(Obj *obj) { return false; }

	int GetFriendByArea(const Posi &pos, int range, ObjID *target_list, int max_num, bool check_alive = true);
	int GetTargetByArea(const Posi &pos, int range, ObjID *target_list, int max_num);

	Scene *m_scene;												// 场景
	ObjID m_deliver_objid;										// 施放者id
	UInt16 m_product_id;										// 产生此效果的技能id 场景buff类型
	char m_product_method;										// EffectBase::PRODUCT_METHOD
};

#endif // __SCENE_EFFECT_BASE_HPP__

