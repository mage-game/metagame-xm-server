#ifndef __WORLD_SHADOW_HPP__
#define __WORLD_SHADOW_HPP__

#include "servercommon/internalprotocal/gglobalprotocal.h"

const static int MONSTER_ADD_ATTR_EFFECTIVE_MIN_WORLD_LEVEL = 30;

class WorldShadow
{
public:
	static WorldShadow & Instance();

	void OnSyncWorldLevelInfo(gglobalprotocal::SyncWorldLevelInfo *wli);
	void SetHiddenWorldLevel(int world_level) { m_hidden_world_level = world_level; }
	int GetHiddenWorldLevel() { return m_hidden_world_level; }
	int GetWorldLevel();
	int GetTopUserLevel() { return m_top_user_level; }
	int GetCapabilityRankMincap() { return m_capablity_rank_mincap; }

	long long GetWorLeveldExtraExp(int role_level, long long add_exp);
	double GetWorLeveldExtraExpPer(int role_level);
	double GetWorLeveldExtraExpPerByConfig(int role_level, int exp_add_reason = Role::EXP_ADD_REASON_DEFAULT);

	void SetGMWorldLevel(int gm_world_level) { m_gm_world_level = gm_world_level; }
	bool IsLoadWordLevel() { return m_is_get_world_level; }

private:
	WorldShadow();
	~WorldShadow();

	int m_world_level;												// 世界等级
	int m_top_user_level;											// 服务器角色最高等级
	int m_capablity_rank_mincap;									// 战力榜最低战力

	int m_gm_world_level;											// GM设置的世界等级
	bool m_is_get_world_level;										// 是否已经获取了世界等级
	int m_hidden_world_level;										// 隐藏服世界等级
};

#endif // __WORLD_SHADOW_HPP__

