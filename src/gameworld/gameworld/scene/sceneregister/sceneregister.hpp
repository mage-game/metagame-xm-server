#ifndef __SCENE_REGISTER_HPP__
#define __SCENE_REGISTER_HPP__

#include <map>
#include <set>

#include "servercommon/serverdef.h"
#include "protocal/msgscene.h"

class Role;
struct ConfigSceneDoor;

struct SceneLineInfo
{
	SceneLineInfo() : scene_id(0), max_line(0), line_role_count(0), open_line(1) {}

	int scene_id;
	int max_line;
	int line_role_count;
	int open_line;
};

class SceneRegister
{
public:
	static SceneRegister & Instance();

	bool LoadSceneLineConfig(const std::string &path, std::string *err);

	void GetAllObjMoveInfo(Role *role, Protocol::CSGetAllObjMoveInfoReq *gaomir);
	bool AddSceneLevelLimit(int scene_id, int level_limit);
	bool CheckSceneLevelLimit(Role *role, int scene_id);

	void AddScene(int scene_id);
	bool CheckSceneIsExist(int scene_id);

	bool IsThisGSScene(int total_server, int server_index, int scene_id);

	void AddWorldBossScene(int scene_id);
	bool IsWorldBossScene(int scene_id);

	void AddDoor(int scene_id, ConfigSceneDoor door);
	const ConfigSceneDoor *GetDoor(int scene_id, int door_id);

	void AddLine(int scene_id, int max_line, int line_role_count);
	SceneLineInfo *GetSceneLineInfo(int scene_id);

private:
	bool CanGetAllObjMoveInfo(int scene_type, int scene_id, const UserID &user_id);

	SceneRegister();
	~SceneRegister();

	typedef std::map<UserID, unsigned int> UserGetAllObjOperTimeMap;
	typedef std::map<UserID, unsigned int>::iterator UserGetAllObjOperTimeMapIt;
	UserGetAllObjOperTimeMap m_getallobj_opertime_map;											// 客户端拉取地图所有对象列表的时间戳

	std::map<int, int> m_scene_level_limit_map;													// 场景等级限制
	std::set<int> m_all_scene_set;																// 场景ID列表
	std::set<int> m_worldboss_scene_set;														// 世界boss场景ID列表
	std::map<int, ConfigSceneDoor> m_door_map;													// 传送门列表
	std::map<int, SceneLineInfo> m_line_map;													// 分线信息
};

#endif // __TEAM_KEY_REGISTER_HPP__

