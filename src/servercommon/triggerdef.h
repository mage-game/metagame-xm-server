#ifndef __TRIGGER_DEF_H__
#define __TRIGGER_DEF_H__

#include "servercommon/servercommon.h"

static const int TRIGGER_COND_PARAM_MAX = 2;						// ����������������
static const int TRIGGER_ACTION_PARAM_MAX = 4;						// ����������������

enum TRIGGER_COND_TYPE
{
	TRIGGER_COND_TYPE_INVALID = 0,

	TRIGGER_COND_TYPE_ENTER_SCENE,									// ���볡��
	TRIGGER_COND_TYPE_ENTER_AREA,									// ��������
	TRIGGER_COND_TYPE_FB_PASS,										// ����ͨ��
	TRIGGER_COND_TYPE_MONSTER_DIE,									// ָ����������

	TRIGGER_COND_TYPE_MAX
};

enum TRIGGER_ACTION_TYPE
{
	TRIGGER_ACTION_TYPE_INVALID = 0,

	TRIGGER_ACTION_TYPE_CREATE_MONSTER,								// ˢ��
	TRIGGER_ACTION_TYPE_SURROUND_MONSTER,							// �������
	TRIGGER_ACTION_TYPE_SET_FB_FOLLOW_NPC,							// ���ø�������NPC
	TRIGGER_ACTION_TYPE_RESET_FB_FOLLOW_NPC,						// ȡ����������NPC
	TRIGGER_ACTION_TYPE_SPECIAL_DICI,								// �ش�
	TRIGGER_ACTION_TYPE_SPECIAL_BEILAO,								// ����
	TRIGGER_ACTION_TYPE_SPECIAL_BANMASUO,							// ������
	TRIGGER_ACTION_TYPE_PLAY_STORY,									// ���ž���
	TRIGGER_ACTION_TYPE_CREATE_MONSTER_APPOINT_POS,					// ����ˢ��
	TRIGGER_ACTION_TYPE_SPECIALLOGIC,								// �������������߼�
	TRIGGER_ACTION_TYPE_ICE_LANDMINE,								// ��˪����
	TRIGGER_ACTION_TYPE_FIRE_LANDMINE,								// �������

	TRIGGER_ACTION_TYPE_MAX
};

enum TRIGGER_SPECIALLOGIC_PARAM0									// �������������߼� trigger_action_param_list[0]
{
	TRIGGER_SPECIALLOGIC_PARAM0_YJQMS_SKILL = 0,					// ��������ɽ ����
	TRIGGER_SPECIALLOGIC_PARAM0_YJQMS_XIANGJIAOPI,					// ��������ɽ �㽶Ƥ
};

#endif	// __TRIGGER_DEF_H__

