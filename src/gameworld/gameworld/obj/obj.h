
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
		// ��������ʱ��IsCharacter��������ͬʱ�޸�
		OBJ_TYPE_INVALID,
		OBJ_TYPE_ROLE,					// ��ɫ
		OBJ_TYPE_MONSTER,				// ����
		OBJ_TYPE_FALLINGITEM,			// ������Ʒ
		OBJ_TYPE_GATHER,				// �ɼ���Ʒ
		OBJ_TYPE_SCENEAREA,				// ��������
		OBJ_TYPE_EFFECT,				// ����Ч����Ʒ
		OBJ_TYPE_TRIGGER,				// ������
		OBJ_TYPE_BIG_COIN,				// ��ͭ��
		OBJ_TYPE_BATTLEFIELD_SHENSHI,	// ս����ʯ
		OBJ_TYPE_MONSTER_HALO,			// ����⻷
		OBJ_TYPE_MARRY,					// ���ģ��
		OBJ_TYPE_ROLE_SHADOW,			// ��ɫӰ��
		OBJ_TYPE_WORLDEVENT_OBJ,		// �����¼�����
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
	const Posi& GetObjPosi() { return m_obj_posi; } // ��ȡ���������OBJ��λ��
	void		SetObjPosi(const Posi &pos) { m_obj_posi = pos; } // ���ò��������OBJ��λ��
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
	Posi		m_obj_posi;				// ���������obj(����ʱ)��λ��

	GameName	m_name;					// ����
	Scene		*m_scene;				// ���ڳ�����id
	UInt32		m_single_mgr_index;
	Posf		m_posf;					// Ŀǰ�ľ�ȷλ�ã������˶�����
	Posi		m_posi;					// Ŀǰ��λ�ã������߼�����
	Dir			m_dir;					// ����
	Scalar		m_dir_distance;			// �������
	short		m_height;				// �߶�

	UInt32		m_obser_handle;
	
	bool        m_valid;				// �Ƿ���Ч 

	short		m_fly_height;			// ���и߶�
};

#endif


