#ifndef __SCENE_SKILL_BASE_HPP__
#define __SCENE_SKILL_BASE_HPP__

#include "iclockmodule.h"

#include "gamedef.h"
#include "servercommon/serverdef.h"

#include <string>

class Scene;
class Character;

class SceneSkillBase
{
public:
	enum SCENE_SKIL_ID
	{
		SCENE_SKILL_ID_INVALID = 10,							// �����൱����ID ֧���滻���� ��ɫ��ͨ���ܴ�1��ʼ �����10��ʼ �мǣ���
		SCENE_SKILL_ID_MAX
	};

	static bool IsValidSceneSkill(int scene_skill_id) 
	{
		if (scene_skill_id <= SCENE_SKILL_ID_INVALID || scene_skill_id >= SCENE_SKILL_ID_MAX) return false;

		return true;
	}
	
	SceneSkillBase();
	virtual ~SceneSkillBase();

	virtual bool LoadConfig(const char *dir, std::string *err) = 0;
	virtual char GetSceneSkillId() const = 0;
	virtual void Perform(Scene *scene, Character *chara, const Posi &target_pos, int special_param = 0) const = 0;

	int GetCoolDown() const { return m_cooldown; }
	int GetDistance() const { return m_distance; }
	int GetRange() const { return m_range; }

	void SendSceneSkillAttackMsg(Scene *scene, Character *chara, const Posi &target_pos) const;

protected:
	int m_cooldown;														// ��ȴʱ�� ��
	int m_distance;														// ʩ�ž�������
	int m_range;														// ���÷�Χ
};

#endif // __SCENE_SKILL_BASE_HPP__

