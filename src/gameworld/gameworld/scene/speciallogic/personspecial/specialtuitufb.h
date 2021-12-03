#pragma once
#ifndef __SPECIAL_TUITU_FB_H__
#define __SPECIAL_TUITU_FB_H__
#include "gameworld/gameworld/scene/speciallogic/speciallogic.hpp"
#include <vector>

class SpecialTuitu : public SpecialLogic
{
public:
	SpecialTuitu(Scene *scene);
	virtual ~SpecialTuitu();

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnAddObj(Obj *obj);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRoleLeaveScene(Role *role, bool is_logout);
	virtual bool OnTimeout();
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual void Reset();

	void OnReqNextLevel();

protected:
	virtual void OnAllMonsterDead();

private:
	void OnFinish(int star);
	void SyncFBSceneLogicInfo(Role *role);

	bool m_is_finish;				//�Ƿ����
	bool m_is_pass;					//�Ƿ�ͨ��
	int m_pass_star;				//ͨ������

	int m_owner_uid;				//��������
	int m_fb_type;					//��ͼ��������
	int m_chapter;					//�½�
	int m_level;					//�ؿ�
	unsigned int m_start_timestamp;	//��ʼʱ���

	int m_total_boss_num;
	int m_cur_boss_num;
	int m_total_monster_num;
	int m_cur_monster_num;

	unsigned int m_die_timestamp;	//����ʱ���

	std::vector<ItemConfigData> fall_item_vec;		//������Ʒ�б�����չʾ
};

#endif