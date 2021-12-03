#ifndef __GUILD_KEY_REGISTER_HPP__
#define __GUILD_KEY_REGISTER_HPP__

#include "servercommon/serverdef.h"

#include <map>

class GuildKeyRegister
{
public:
	static GuildKeyRegister & Instance();

	void RegisterGuildKey(const UserID &user_id, int scene_id, int scene_key, unsigned int create_time);

	int GetGuildSceneKey(const UserID &user_id);

	void RemoveGuildKey(int scene_key);

private:
	GuildKeyRegister();
	~GuildKeyRegister();

	struct GuildKey
	{
		GuildKey() : scene_id(0), scene_key(0), create_time(0) {}
		GuildKey(int _scene_id, int _scene_key, int _create_time) : scene_id(_scene_id), scene_key(_scene_key), create_time(_create_time) {}

		int scene_id;
		int scene_key;
		unsigned int create_time;
	};

	typedef std::map<UserID, GuildKey> UserGuildKeyMap;
	typedef std::map<UserID, GuildKey>::iterator UserGuildKeyMapIt;
	UserGuildKeyMap m_user_guildkey_map;
};

#endif // __GUILD_KEY_REGISTER_HPP__

