#ifndef _ACTIVITY_MSG_MANAGER_H_
#define _ACTIVITY_MSG_MANAGER_H_
#include <map>
#include "servercommon/serverdef.h"
#include "servercommon/crossdef.hpp"
#include "gameworld/gamedef.h"

class Scene;
class Role;
class Monster;
static const int ACTIVITY_RANK_TOP_MSG_MAX = 3;

struct RoleActivityMsg
{
	RoleActivityMsg():user_key(-1),scene_type(0),lian_zhan_count(0),dead_count(0){}
	long long user_key;
	int scene_type;
	int lian_zhan_count;
	int dead_count;
};

class ActivityMsgManager
{
public:
	long long ParamToLongLong(int param_0, int param_1)
	{
		union
		{
			long long ll_value;
			struct
			{
				int param_0;
				int param_1;
			};
		} union_param;

		union_param.param_0 = param_0;
		union_param.param_1 = param_1;

		return union_param.ll_value;
	}

	struct RankTopItem
	{
		RankTopItem() :uid(0), camp(0)
		{
			memset(user_name, 0, sizeof(GameName));
		}
		long long uid;
		GameName user_name;
		int camp;
	};

	static ActivityMsgManager & Instance();
	void Init();
	void OnRoleLogout(Role * role);
	void OnRoleDie(Scene * scene,Role* role, ObjID killer);
	void OnMonsterDie(Scene *scene, Monster* monster, ObjID killer);
	void MonsterOnAttack(Scene * scene, Monster* monster, ObjID attacker, Attribute delta_hp);
	void RankTopMsg(Scene *scene, RankTopItem * top_list, int count);
	bool IsNeedRankTopMsg(Scene *scene);
	void RankFirstMsg(Scene *scene, RankTopItem & first);

private:
	long long GetRoleKey(Role *role);
	UniqueUserID GetGuildUniqueUserId(Role *role);
	RoleActivityMsg * GetRoleActivityMsg(long long user_key);
	void SendActivityMsg(Scene * scene, char* str_buff ,int len, int notice_type, GuildID guild_id = 0);

	std::map<long long, RoleActivityMsg> m_msg_role_map;
	std::map<long long, unsigned int> m_monster_attack_msg_time;
	std::map<long long, unsigned int> m_rank_top_time;
	std::map<int, unsigned int> m_guilder_role_die;
};
#endif
