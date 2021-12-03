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
		PRODUCT_ID_TRIGGER_SPECIAL_DICI = 1200,		// 地刺
		PRODUCT_ID_TRIGGER_SPECIAL_BEILAO,			// 焙烙
		PRODUCT_ID_TRIGGER_SPECIAL_BANMASUO,		// 绊马索
		PRODUCT_ID_TRIGGER_SPECIAL_ICE_LANDMINE,	// 冰霜地雷
		PRODUCT_ID_TRIGGER_SPECIAL_FIRE_LANDMINE,	// 火焰地雷
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
	
	UInt16	m_trigger_id;											// 触发器ID
	int		m_range;												// 范围

	int		m_trigger_cond_type;									// 触发条件类型
	int		m_trigger_cond_param_list[TRIGGER_COND_PARAM_MAX];		// 触发条件参数

	int		m_trigger_action_type;									// 触发动作类型
	int		m_trigger_action_param_list[TRIGGER_ACTION_PARAM_MAX];	// 触发动作参数

	int		m_trigger_max_count;									// 可触发次数

	UInt32	m_aoi_handle;											// AOI handle
	int		m_trigger_count;										// 已触发次数

	bool	m_is_in_effect;											// 是否在作用中 用于持续作用的地刺等
	int		m_effect_interval;										// 作用间隔
	int		m_effect_time;											// 作用时间

	bool	m_is_hidden;											// 是否隐藏
	bool	m_is_notify_triggered;									// 触发时是否通知客户端
	
	int		m_affiliation;											// 归属
};

#endif	// __TRIGGER_H__

