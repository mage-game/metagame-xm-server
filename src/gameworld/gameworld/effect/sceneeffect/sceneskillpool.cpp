#include "sceneskillpool.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/configcommon.h"

#include "effect/sceneeffect/sceneskill.hpp"

SceneSkillPool::SceneSkillPool()
{
	memset(m_scene_skill_list, 0, sizeof(m_scene_skill_list));
}

SceneSkillPool::~SceneSkillPool()
{
	this->Release();
}

#define INIT_SCENE_SKILL(SCENE_SKILL_CLASS)\
	SceneSkillBase *scene_skill = new SCENE_SKILL_CLASS();\
	if (!scene_skill->LoadConfig(scene_skill_dir, err)) \
	{\
		delete scene_skill;\
		Release();\
		return false;\
	}\
	if (m_scene_skill_list[(int)scene_skill->GetSceneSkillId()] != 0)\
	{\
		delete scene_skill;\
		Release();\
		return false;\
	}\
	m_scene_skill_list[(int)scene_skill->GetSceneSkillId()] = scene_skill;

bool SceneSkillPool::Init(const std::string &config_dir, std::string *err)
{
	//std::string scene_skill_dir_str = std::string(config_dir) + "sceneskill/";

	//const char *scene_skill_dir = scene_skill_dir_str.c_str();

	UNSTD_STATIC_CHECK(SceneSkillBase::SCENE_SKILL_ID_MAX == 11);

	return true;
}

const SceneSkillBase * SceneSkillPool::GetSceneSkill(int scene_skill_id) const
{
	if (scene_skill_id <= SceneSkillBase::SCENE_SKILL_ID_INVALID || scene_skill_id >= SceneSkillBase::SCENE_SKILL_ID_MAX) return NULL;

	return m_scene_skill_list[scene_skill_id];
}

void SceneSkillPool::Release()
{
	for (int i = 0; i < SceneSkillBase::SCENE_SKILL_ID_MAX; ++i)
	{
		if (NULL != m_scene_skill_list[i])
		{
			delete m_scene_skill_list[i];
		}
	}
	memset(m_scene_skill_list, 0, sizeof(m_scene_skill_list));
}

