#ifndef __SPECIAL_QING_YUAN_SHENG_DI_HPP__
#define __SPECIAL_QING_YUAN_SHENG_DI_HPP__

#include "scene/speciallogic/speciallogic.hpp"
#include "config/qingyuanshengdicconfig.h"

class Role;

class SpecialQingYuanShengDi : public SpecialLogic
{
public:
	SpecialQingYuanShengDi(Scene *scene);
	virtual ~SpecialQingYuanShengDi();

	virtual bool CanDestroy() { return false; }

	virtual void Update(unsigned long interval, time_t now_second);
	virtual void OnCharacterDie(Character *dead, ObjID killer_id);
	virtual void OnRoleEnterScene(Role *role);
	virtual void OnRemoveObj(Obj *obj);
	virtual void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos);

protected:
	void InitRefresh();
	void RefreshMonster(unsigned int now_second);
	void RefreshGather(unsigned int now_second);
	bool GetPos(int pos_type, Posi *pos);
	void AddPos(int pos_type, const Posi &pos);
	void SyncMonsterInfo(Role *role = NULL);

	struct MonsterInfo
	{
		MonsterInfo() : obj_id(INVALID_OBJ_ID), pos(0, 0), next_refresh_time(0), is_boss(false) {}

		ObjID obj_id;
		Posi pos;
		unsigned int next_refresh_time;
		bool is_boss;

		QYSDLayerCfg::MonsterItem cfg;
	};

	struct GatherInfo
	{
		GatherInfo() : obj_id(INVALID_OBJ_ID), pos(0, 0), next_refresh_time(0) {}

		ObjID obj_id;
		Posi pos;
		unsigned int next_refresh_time;

		QYSDLayerCfg::GatherItem cfg;
	};

	bool m_is_init;
	unsigned int m_last_update_time;

	std::map<ObjID, MonsterInfo> m_scene_monster_map;
	std::vector<MonsterInfo> m_refresh_monster_vec;

	std::map<ObjID, GatherInfo> m_scene_gather_map;
	std::vector<GatherInfo> m_refresh_gather_vec;

	std::map<int, std::vector<Posi> > m_pos_map;
};

#endif
