#include "sceneskillbase.hpp"

#include "protocal/msgscene.h"

#include "scene/scene.h"

SceneSkillBase::SceneSkillBase() 
	: m_cooldown(0), m_distance(0), m_range(0)
{
	
}	
	
SceneSkillBase::~SceneSkillBase()
{
	
}

void SceneSkillBase::SendSceneSkillAttackMsg(Scene *scene, Character *chara, const Posi &target_pos) const
{
	static Protocol::SCPerformSceneSkill psk;
	psk.obj_id = chara->GetId();
	psk.scene_skill_id = static_cast<UInt16>(this->GetSceneSkillId());
	psk.target_x = static_cast<short>(target_pos.x);
	psk.target_y = static_cast<short>(target_pos.y);
	if (NULL != scene) scene->GetZoneMatrix()->NotifyAreaMsg(chara->GetObserHandle(), &psk, sizeof(psk));
}

