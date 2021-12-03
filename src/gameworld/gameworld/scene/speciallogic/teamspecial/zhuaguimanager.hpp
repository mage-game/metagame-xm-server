#ifndef _ZHUAGUI_MGR_HPP_
#define _ZHUAGUI_MGR_HPP_

#include "servercommon/servercommon.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "inetworkmodule.h"

#include <map>
#include <vector>

class Role;

class ZhuaGuiMgr
{
public:
	static ZhuaGuiMgr & Instance();

	void Update(unsigned long interval, time_t now_second);
	void OnRoleLogin(Role *role);

	void OnTouchNpc(Role *role, int scene_id, ObjID objid);	
	void SyncZhuaGuiInfo(Role *role);

private:
	ZhuaGuiMgr();
	~ZhuaGuiMgr();

	void OnRoleEnterZhuaguiFB(int scene_id, ObjID objid);
	void FlushNpcHelper(int scene_id);
	int GetZhuaGuiSceneKey() { return ++ m_zhuagui_scene_key_inc; }

	struct NpcObjReg
	{
		NpcObjReg() : scene_id(0), flush_pos(0, 0), scene_idx(-1), objid(INVALID_OBJ_ID)
		{
		}

		int scene_id;
		Posi flush_pos;

		SceneIndex scene_idx;
		ObjID objid;
	};

	typedef std::map<int, std::vector<NpcObjReg> > SceneNpcMap;
	typedef std::map<int, std::vector<NpcObjReg> >::iterator SceneNpcMapIt;
	SceneNpcMap m_scene_npc_map;

	bool m_first;

	int m_zhuagui_scene_key_inc;
};

#endif




