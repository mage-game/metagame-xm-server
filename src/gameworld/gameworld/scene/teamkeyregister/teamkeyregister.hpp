#ifndef __TEAM_KEY_REGISTER_HPP__
#define __TEAM_KEY_REGISTER_HPP__

#include "servercommon/serverdef.h"

#include <map>

class TeamKeyRegister
{
public:
	static TeamKeyRegister & Instance();

	void RegisterTeamKey(const UserID &user_id, int scene_id, int scene_key, unsigned int create_time);

	int GetTeamSceneKey(const UserID &user_id);
private:
	TeamKeyRegister();
	~TeamKeyRegister();

	struct TeamKey
	{
		TeamKey() : scene_id(0), scene_key(0), create_time(0) {}
		TeamKey(int _scene_id, int _scene_key, int _create_time) : scene_id(_scene_id), scene_key(_scene_key), create_time(_create_time){}

		int scene_id;
		int scene_key;
		unsigned int create_time;
	};

	typedef std::map<UserID, TeamKey> UserTeamKeyMap;
	typedef std::map<UserID, TeamKey>::iterator UserTeamKeyMapIt;
	UserTeamKeyMap m_user_teamkey_map;
};

#endif // __TEAM_KEY_REGISTER_HPP__

