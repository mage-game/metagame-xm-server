#ifndef __MARRY_SHADOW_H__
#define __MARRY_SHADOW_H__

#include "servercommon/marrydef.hpp"

#include "scene/scene.h"

class Character;

class MarryShadow
{
public:
	MarryShadow();
	~MarryShadow();

	static MarryShadow & Instance();

	void OnSyncMarryInfo(const SyncMarryInfo &marry_info);

	void Update(unsigned long interval);

	void OnCharacterMoveStop(Character *cha);

	bool CanMarryXunyouOpera(const UserID &user_id);
	bool IsMarryUser(const UserID &user_id);
	bool GetMarryobjPos(int *pos_x, int *pos_y);
	bool GetMarrySceneId(int *scene_id);
	int GetMarryStatus() { return m_marry_info.marry_status; }
	unsigned int GetMarryStartTime() { return m_marry_info.marry_start_time; }

	Scene * GetMarryScene();

private:
	void OnChangeToStatus(int to_status);

	void OnMarryStatusToBaiTianDiZhunBei();
	void OnMarryStatusToXunYou();
	void OnMarryStatusToHunYanZhunBei();
	void OnMarryStatusToHunYan();
	void OnMarryStatusToFinish();

	SyncMarryInfo m_marry_info;

	int m_cur_xunyou_point_index;
	unsigned long m_marryobj_stop_time;

	ObjID m_marryobj_id_list[MARRY_MARRYOBJ_MAX_NUM];
};

#endif // __MARRY_SHADOW_H__

