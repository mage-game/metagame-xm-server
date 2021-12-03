#ifndef __SCENE_SKILL_POOL_HPP__
#define __SCENE_SKILL_POOL_HPP__

#include "effect/sceneeffect/sceneskillbase.hpp"
#include "servercommon/basedef.h"

#include <limits.h>
#include <string>

class SceneSkillBase;

class SceneSkillPool
{
public:
	SceneSkillPool();
	~SceneSkillPool();

	bool Init(const std::string &config_dir, std::string *err);

	const SceneSkillBase * GetSceneSkill(int scene_skill_id) const;

private:
	void Release();

	SceneSkillBase *m_scene_skill_list[SceneSkillBase::SCENE_SKILL_ID_MAX];
};

#endif //__SCENE_SKILL_POOL_HPP__
