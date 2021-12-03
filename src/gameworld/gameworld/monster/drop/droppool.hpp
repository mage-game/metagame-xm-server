#ifndef __DROP_POOL_HPP__
#define __DROP_POOL_HPP__

#include <string>

#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"

#include "gamedef.h"
#include "dropconfig.hpp"

class Scene;
class Role;

#define DROPPOOL DropPool::Instance()

class DropPool
{
public:
	static DropPool * Instance();

	bool Init(const std::string &config_dir, const std::string &configname, std::string *err);
	static bool Reload(const std::string &config_dir, const std::string &configname, std::string *err);

	DropConfig * GetDropConfig(UInt16 drop_id);
	
	void DropHelper(UInt16 dropid, Scene *scene, const Posi &pos, const UserID &user_id, char prof, UInt16 monster_id, bool is_log, bool is_from_body, int *dorp_index);
	void DropToHelper(Scene *scene, UInt16 item_id, int item_num, bool item_is_bind, int coin_or_gold, const Posi &pos, const UserID &owner_user_id, 
					UInt16 monster_id, bool is_log, short is_broadcast, bool is_from_body, int *dorp_index);

	void DropTest(UInt16 dropid, Scene *scene, const Posi &pos, const UserID &user_id, UInt16 monster_id, bool is_log);

	int GetDropItemList(UInt16 dropid, ItemConfigData item_list[DropConfig::DROP_ITEM_PROB_MAX]);

private:
	bool Drop(UInt16 dropid, Scene *scene, const Posi &pos, const UserID &user_id, char prof, UInt16 monster_id, bool is_log, bool is_from_body, int *dorp_index);

	DropPool();
	~DropPool();

	void Release();

	static const int MAX_DROP_ID = (int)USHRT_MAX + 1;
	DropConfig *m_drop_list[MAX_DROP_ID];
};

#endif // __DROP_POOL_HPP__

