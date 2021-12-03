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

	int m_world_level;												// ����ȼ�
	int m_top_user_level;											// ��������ɫ��ߵȼ�
	int m_capablity_rank_mincap;									// ս�������ս��

	int m_gm_world_level;											// GM���õ�����ȼ�
	bool m_is_get_world_level;										// �Ƿ��Ѿ���ȡ������ȼ�
	int m_hidden_world_level;										// ���ط�����ȼ�
};

#endif // __WORLD_SHADOW_HPP__

