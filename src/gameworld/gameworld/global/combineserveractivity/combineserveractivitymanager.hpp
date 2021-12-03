#ifndef __OPEN_COMBINE_SERVER_ACTIVITY_MANAGER_HPP__
#define __OPEN_COMBINE_SERVER_ACTIVITY_MANAGER_HPP__

#include "servercommon/activitydef.hpp"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "internalcomm.h"

class Role;

class CombineServerActivityManager
{
public:
	static CombineServerActivityManager & Instance();

	void Init(const CombineServerActivityRankParam &cbs_rank_param, const CombineServerActivityOtherParam &cbs_other_param, const CombineServerActivityBossRankParam &csa_boss_rank_param);
	void GetInitParam(CombineServerActivityRankParam *cbs_rank_param, CombineServerActivityOtherParam *cbs_other_param, CombineServerActivityBossRankParam *csa_boss_rank_param);

	void Update(unsigned long interval, time_t now_second);

	bool SubActivityOpen(int sub_type);
	bool SubActivityFinish(int sub_type);

	void OnSendActivityInfo(Role *user);
	void OnCombineServerActivitySyncValue(int rank_type, int role_id, long long rank_value, long long rank_reserve);
	void SyncInfoToGameWorld();

	int GetRankQianggouBuyNum(int item_idx);
	void OnBuyRankQianggouItem(Role *role, int item_idx);

	void OnChangeXianmengzhanWiner(int old_winer, int new_winer);								
	void OnChangeGongchengzhanWiner(int old_winer, int new_winer);	

	int GetServerPanicBuyNum(int item_idx);
	void OnServerPanicBuy(Role *role, int item_idx);

	void OnCSABossCheckRank(int rank_type, int id, int rank_value);
	void OnCSABossDismissGuild(int guild_id);
	void OnCSABossClearRank();
	void OnCSABossRankReward();
	void OnSendCSABossRankInfo(Role *role);

	int GetCSAGongchengzhanChengzhuLoverUID() { return m_csa_other_param.cas_gcz_winner_lover_id; }
	int GetCSAGongchengzhanChengzhuUID() { return m_csa_other_param.csa_gcz_winner_roleid; }
	int GetCSAGuildBattleWinnerUID() { return m_csa_other_param.csa_xmz_winner_roleid; }

	void ResetData();

	const CombineServerActivityRankParam GetCombineServerActivityRankParam() { return m_csa_rank_param; }

	enum BOSS_RANK_TYPE
	{
		BOSS_RANK_TYPE_PERSONAL = 0,
		BOSS_RANK_TYPE_GUILD = 1,
	};

private:
	CombineServerActivityManager();
	~CombineServerActivityManager();

	void UpdateRankUser(int rank_type, int id, long long rank_value, long long rank_reserve);
	void CheckRankReward(int rank_type);

	void ResetRankList(int rank_type);

	template<class T> 
	bool UpdateBossRank(T rank_list[COMBINE_SERVER_BOSS_RANK_NUM], int rank_list_len, int id, int rank_value);

	CombineServerActivityRankParam m_csa_rank_param;
	CombineServerActivityOtherParam m_csa_other_param;

	CombineServerActivityBossRankParam m_csa_boss_rank_param;
};

template<class T>
bool CombineServerActivityManager::UpdateBossRank(T rank_list[COMBINE_SERVER_BOSS_RANK_NUM], int rank_list_len, int id, int rank_value)
{
	if (COMBINE_SERVER_BOSS_RANK_NUM != rank_list_len)
	{
		return false;
	}

	if (rank_value > rank_list[rank_list_len - 1].rank_value)
	{
		int rank = rank_list_len - 1;
		for (; rank >= 0; --rank)						
		{
			if (rank_list[rank].id == id)						//�����һλ��ʼ,���ұ����Ƿ������а�,�����������
			{
				rank_list[rank].rank_value = rank_value;
				break;
			}
		}

		if (0 == rank)											//�����ǵ�һ��,ֱ�ӷ���
		{
			return true;
		}

		if (-1 == rank)
		{
			rank = rank_list_len - 1;							//���������а�,���滻���һλ
			rank_list[rank_list_len - 1].id = id;
			rank_list[rank_list_len - 1].rank_value = rank_value;
		}

		for (; rank > 0; --rank)								//������λ�ÿ�ʼ�Ƚ�����
		{
			if (rank_list[rank].rank_value > rank_list[rank - 1].rank_value)
			{
				T temp_rank = rank_list[rank - 1];
				rank_list[rank - 1] = rank_list[rank];
				rank_list[rank] = temp_rank;
			}
			else
			{
				break;
			}
		}

		return true;
	}

	return false;
}

#endif // __OPEN_COMBINE_SERVER_ACTIVITY_MANAGER_HPP__


