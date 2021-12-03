#ifndef __TRIGGER_H__
#define __TRIGGER_H__

#include "obj/obj.h"
#include "servercommon/triggerdef.h"

#include "config/sceneconfig.h"

class SceneManagerConfig;
class Role;

class Trigger : public Obj
{
public:
	Trigger();
	virtual ~Trigger();

	enum PRODUCT_ID_TRIGGER
	{
		PRODUCT_ID_TRIGGER_SPECIAL_DICI = 1200,		// �ش�
		PRODUCT_ID_TRIGGER_SPECIAL_BEILAO,			// ����
		PRODUCT_ID_TRIGGER_SPECIAL_BANMASUO,		// ������
		PRODUCT_ID_TRIGGER_SPECIAL_ICE_LANDMINE,	// ��˪����
		PRODUCT_ID_TRIGGER_SPECIAL_FIRE_LANDMINE,	// �������
	};
	
	static bool CheckTriggerConfig(const ConfigSceneTrigger &trigger_cfg, SceneManagerConfig *scene_manager_config, int mapid, std::string *err);

	void Init(UInt16 id, int range, int trigger_cond_type, const int trigger_cond_param_list[TRIGGER_COND_PARAM_MAX], int trigger_action_type, const int trigger_action_param_list[TRIGGER_ACTION_PARAM_MAX], int trigger_max_count);
	void InitVisible(bool is_hiden, bool notify_triggered);
	void InitAffiliation(int affiliation){m_affiliation = affiliation;}

	virtual void OnEnterScene();
	virtual void OnLeaveScene();
	virtual void OnAOIEnter(ObjID obj_id);
	virtual void OnAOILeave(ObjID obj_id);
	virtual void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);
	
	void OnTrigger(int trigger_cond_type, Role *role = NULL, int trigger_cond_param0 = 0, int trigger_cond_param1 = 0);

	bool HasHiddenMonster();

	bool IsHiden() { return m_is_hidden; }

	int GetActionType() { return m_trigger_action_type; }
	int GetActionParam(int param_idx);
	int GetAffiliation(){return m_affiliation;}

	void Reset();
	void ChangeTriggerCount(int count){ m_trigger_count = count;}

	void *operator new(size_t c);
	void operator delete(void *m);

protected:
	void TriggerAction(Role *role);

	void StartEffect(int interval);
	void StopEffect();
	bool Effect();
	void AttachEffect(Role *role);
	
	UInt16	m_trigger_id;											// ������ID
	int		m_range;												// ��Χ

	int		m_trigger_cond_type;									// ������������
	int		m_trigger_cond_param_list[TRIGGER_COND_PARAM_MAX];		// ������������

	int		m_trigger_action_type;									// ������������
	int		m_trigger_action_param_list[TRIGGER_ACTION_PARAM_MAX];	// ������������

	int		m_trigger_max_count;									// �ɴ�������

	UInt32	m_aoi_handle;											// AOI handle
	int		m_trigger_count;										// �Ѵ�������

	bool	m_is_in_effect;											// �Ƿ��������� ���ڳ������õĵش̵�
	int		m_effect_interval;										// ���ü��
	int		m_effect_time;											// ����ʱ��

	bool	m_is_hidden;											// �Ƿ�����
	bool	m_is_notify_triggered;									// ����ʱ�Ƿ�֪ͨ�ͻ���
	
	int		m_affiliation;											// ����
};

#endif	// __TRIGGER_H__

