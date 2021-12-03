
#ifndef OBJ_H
#define OBJ_H

#include <vector>
#include "gamedef.h"

class Scene;
class OfflineRegister;

class Obj
{
public:
	enum _ObjType
	{
		// 增加类型时在IsCharacter函数必须同时修改
		OBJ_TYPE_INVALID,
		OBJ_TYPE_ROLE,					// 角色
		OBJ_TYPE_MONSTER,				// 怪物
		OBJ_TYPE_FALLINGITEM,			// 掉落物品
		OBJ_TYPE_GATHER,				// 采集物品
		OBJ_TYPE_SCENEAREA,				// 特殊区域
		OBJ_TYPE_EFFECT,				// 技能效果物品
		OBJ_TYPE_TRIGGER,				// 触发器
		OBJ_TYPE_BIG_COIN,				// 大铜币
		OBJ_TYPE_BATTLEFIELD_SHENSHI,	// 战场神石
		OBJ_TYPE_MONSTER_HALO,			// 怪物光环
		OBJ_TYPE_MARRY,					// 结婚模型
		OBJ_TYPE_ROLE_SHADOW,			// 角色影子
		OBJ_TYPE_WORLDEVENT_OBJ,		// 世界事件对象
		OBJ_TYPE_COUNT,
	};

	Obj(ObjType type);
	Obj(ObjType type, ObjID obj_id, const GameName _name);
	virtual	~Obj();

	void		SetId(ObjID gid) { m_obj_id = gid; }
	ObjID		GetId() { return m_obj_id; }
	ObjType		GetObjType() { return m_obj_type; }
	UInt32		GetSingleMgrIndex() { return m_single_mgr_index; }
	void		SetSingleMgrIndex(UInt32 index) { m_single_mgr_index = index; }
	void		SetPos(const Posi &pos) { m_posi = pos; m_posf.Init((float)pos.x, (float)pos.y); }
	const Posi&	GetPos() const{ return m_posi; }
	const Posi& GetObjPosi() { return m_obj_posi; } // 获取产生掉落的OBJ的位置
	void		SetObjPosi(const Posi &pos) { m_obj_posi = pos; } // 设置产生掉落的OBJ的位置
	void		GetName(GameName name);
	const char* GetName() { return m_name; }
	void		SetName(GameName name);
	void		SetScene(Scene *scene) { m_scene = scene; }
	Scene*		GetScene() { return m_scene; }

	Dir			GetDir() { return m_dir; }
	Scalar		GetDirDistance() { return m_dir_distance; }
	UInt32		GetObserHandle() { return m_obser_handle; }

	virtual void OnEnterScene();
	virtual void OnLeaveScene();

	virtual void OnAOIEnter(ObjID obj_id){};
	virtual void OnAOILeave(ObjID obj_id){};

	virtual void Update(unsigned long interval, time_t now_second, unsigned int now_dayid) {}

	static bool IsCharactor(ObjType type);
	static bool IsFightCharactor(ObjType type);

	void		SetInvalid() { m_valid = false; }
	bool		IsValid() { return m_valid; }

protected:
	const ObjType	m_obj_type;
	ObjID		m_obj_id;
	Posi		m_obj_posi;				// 产生掉落的obj(死亡时)的位置

	GameName	m_name;					// 名称
	Scene		*m_scene;				// 所在场景的id
	UInt32		m_single_mgr_index;
	Posf		m_posf;					// 目前的精确位置，用于运动计算
	Posi		m_posi;					// 目前的位置，用于逻辑计算
	Dir			m_dir;					// 方向
	Scalar		m_dir_distance;			// 方向距离
	short		m_height;				// 高度

	UInt32		m_obser_handle;
	
	bool        m_valid;				// 是否有效 

	short		m_fly_height;			// 飞行高度
};

#endif


