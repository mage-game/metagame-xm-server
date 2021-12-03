#ifndef __ROLE_STATUS_MANAGER_HPP__
#define __ROLE_STATUS_MANAGER_HPP__

#include "servercommon/servercommon.h"

#include "obj/otherobj/gatherobj.h"

#include "gamedef.h"

class Obj;
class Role;

class RoleStatusManager
{
public:
	enum ROLE_ACTION_STATUS
	{
		ROLE_ACTION_STATUS_INVALID = 0,

		ROLE_ACTION_STATUS_IDLE,		// ��ֹ
		ROLE_ACTION_STATUS_MOVE,		// �ƶ�
		ROLE_ACTION_STATUS_FIGHT,		// ս��
		ROLE_ACTION_STATUS_GATHER,		// �ɼ�
		ROLE_ACTION_STATUS_REST,		// ����

		ROLE_ACTION_STATUS_MAX
	};

	RoleStatusManager();
	~RoleStatusManager();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role);

	void Update(unsigned int now_second);

	int GetRoleActionStatus();

	bool CanMountOn();								// �ܷ�������
	bool CanFightMountOn();							// �ܷ���ս������
	bool CanEnterFB(bool check_acitivty = true, bool notify = true);	// �ܷ����fb
	bool CanGather(GatherObj *gather);				// �ܷ�ɼ�
	bool CanRest(bool is_notify = true);			// �ܷ����
	bool CanPerformSkill();							// �ܷ�ʹ�ü���
	bool CanFight(ObjID target_id);					// �ܷ񹥻�
	bool CanSupplyEffect(int supply_type);			// �����ܷ���Ч
	bool CanUseItem(ItemID item_id);				// �ܷ�ʹ����Ʒ
	bool CanGuildBackStation(bool notify = true);	// �ܷ��Ӫ
	bool CanTransmit(bool is_notify = true);		// �ܷ�Զ�̴���
	bool CanFlyByShoe(int target_scene_id);			// �ܷ�ʹ��С��Ь
	bool CanTransportTo(int target_scene_id);		// �ܷ�ͨ����������
	bool CanAcceptSpecialTask(TaskID task_id);		// �ܷ����������
	bool CanDating(bool is_notify = true);			// �ܷ�Լ��
	bool CanStartCross();							// �ܷ���

	bool CanChangeAttackMode(int attack_mode);		// �ܷ�Ĺ���ģʽ
	bool CanGuildSos();								// �ܷ񷢾������
	int GetGuildSosType();							// ��ȡ�����������
	bool CanRecordLastKillMeUserInfo();				// �ܷ��¼���ɱ�ҵ������Ϣ

	bool CanUpdateGiveMP();							// �ܷ�ʱ�ָ�MP

	bool IsInFB();									// �Ƿ��ڸ�����
	bool IsInBoss();								// �Ƿ���BOSS������
	bool IsHaveTeam();								// �Ƿ��ж���

	void OnEnterFB();								// ���븱��
	void OnGetFollowNPC();							// ��ø���NPC
	void OnAddHusongTask();							// ���ܻ�������
	void OnFly();									// ����

	void OnFight();									// ����ս�����߱���
	bool IsInFightStatus();
	bool IsHpSupplyRewardExtra();					// Ѫ�������Ƿ�ɽ����������

private:
	bool CanAttackModeEnterHelper(int target_scene_id);		// ��ǰս��ģʽ�ܷ����
	bool CheckOtherSceneCanFly(int scene_id);

	Role *m_role;
	unsigned long m_last_fight_time;				// ���ս��״̬��ʱ��
	unsigned int m_fight_end_time;					// �˳�ս��״̬��ʱ���

};

#endif // __ROLE_STATUS_HPP__

