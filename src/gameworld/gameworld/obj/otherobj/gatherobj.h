#ifndef GATHEROBJ_H
#define GATHEROBJ_H

#include "obj/obj.h"
#include "servercommon/struct/itemlistparam.h"

#include <set>

class Role;
class GatherGenerater;
struct LingMaiItem;

// 特殊采集物类型，可用于发协议时带的带数有不同的函义
enum SPECIAL_GATHER_TYPE
{
	SPECIAL_GATHER_TYPE_JINGHUA = 1,		// 精华采集
	SPECIAL_GATHER_TYPE_GUILDBATTLE = 2,	// 公会争霸采集物
	SPECIAL_GATHER_TYPE_BIGCOIN = 3,		// 天赐铜币大铜币
	SPECIAL_GATHER_TYPE_RESERVE_2 = 4,		// 
	SPECIAL_GATHER_TYPE_GUILD_BONFIRE = 5,	// 仙盟篝火
	SPEICAL_GATHER_TYPE_CROSS_FISHING = 6,	// 跨服钓鱼鱼篓
	SPEICAL_GATHER_TYPE_CROSS_GUILD_BATTLE_BOSS= 7, //跨服帮派除魔
	SPECIAL_GATHER_TYPE_HUSONG_SHUIJING = 8,// 跨服护送水晶
};

class GatherObj : public Obj
{
public:
	GatherObj();
	virtual ~GatherObj();

	void Init(GatherGenerater *gather_generater, int gather_id, int gather_time, int evil_add, bool disappear_after_gather, 
		int disappear_s = 0, int disappear_gather_max = 0);
	
	int GatherId() { return m_gather_id; }
	int GatherTime(const UserID &user_id) { return m_gather_time; }
	bool HasGather() { return m_has_gather; }

	void Gather(Role *gathar_role);
	bool CanGather(Role *gather_role);
													
	virtual void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);

	bool HasGenerater() { return NULL != m_gather_generater; }
	GatherGenerater *GetGatherGenerater() { return m_gather_generater; }
	void SetGenerater(GatherGenerater *g) { m_gather_generater = g; }

	void *			operator new(size_t c);
	void			operator delete(void *m);

	void InitBonfire(GuildID guild_id, GuildName guild_name);
	bool CanAddMuCai(Role * role);
	void AddMuCai(Role *role);
	
	GuildID GetBelongGuildID() { return m_belong_guildid; }
	void GetBelongGuildName(GuildName guild_name) { F_STRNCPY(guild_name, m_belong_guildname, sizeof(GuildName)); }
	bool IsBonfire();
	bool IsMillionaireBox();
	bool IsShenzhouGather();

	void SetSpecialGatherType(int special_gather_type) { m_special_gather_type = special_gather_type; }
	int GetSpecialGatherType() { return m_special_gather_type; }

	void OnGatherChange();

	long long GetData() { return m_data; }
	void SetData(int data) { m_data = data; }

	void InitCrossFishingGather(Int64 unique_user_id, const GameName name);
	void GetBelongName(GameName own_name) { F_STRNCPY(own_name, m_own_name, sizeof(GameName)); }

	void SetGatherIngObjId(ObjID obj_id) { m_gather_ing_obj_id = obj_id; }
	ObjID GetGatherIngObjId() { return m_gather_ing_obj_id; }
	const int GetLeftGatherTimes() { return m_disappear_gather_max - m_cur_gather_count; }
	void SetGatherTimesMax(int gather_time_max) { m_disappear_gather_max = gather_time_max;}

	void InitCrossGuildBattleBoss(GuildName guild_name);
private:
	void RemoveOnGather(Role *gathar_role);
	bool CanGatherBonfire(Role *gathar_role, bool notice = true);
	void OnGatharBonfire(Role *gathar_role);
	void UpdateBonfire(unsigned long interval, time_t now_second, unsigned int now_dayid);
	
	int m_gather_id;															// 采集ID
	int m_gather_time;															// 采集时间
	GatherGenerater *m_gather_generater;										// 采集管理器 可以为NULL
	int m_evil_add;																// 罪恶值
	
	bool m_disappear_after_gather;												// 采集后是否消失
	bool m_has_gather;															// 是否已经被采集

	unsigned int m_disappear_timestamp;											// 消失时间戳
	int m_disappear_gather_max;													// 最大采集次数
	int m_cur_gather_count;														// 当前采集次数
	long long m_data;

	unsigned int m_bonfire_next_reward_time;									// 篝火下次时间奖励
	GuildID m_belong_guildid;													// 所属仙盟ID
	GuildName m_belong_guildname;												// 所属仙盟名字

	int m_special_gather_type;													// 特殊采集物类型

	GameName m_own_name;														// 采集物归属者名字

	ObjID m_gather_ing_obj_id;													// 是否有人采集中
};

#endif

