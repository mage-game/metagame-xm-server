#ifndef __YIZHANDAODI_CONFIG_HPP__
#define __YIZHANDAODI_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/activitydef.hpp"
#include <vector>
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct YiZhanDaoDiDeadConfig
{
	const static int MAX_DEAD_TIMES = 32;

	struct ConfigItem
	{
		ConfigItem() : dead_times(0), add_maxhp_percent(0), has_item_reward(0) {}
		
		int dead_times;
		int add_maxhp_percent;
		int has_item_reward;
		ItemConfigData reward_item;
	};

	YiZhanDaoDiDeadConfig() : max_dead_times(0) {}

	int max_dead_times;
	ConfigItem cfg_list[MAX_DEAD_TIMES];
};

enum YIZHAN_GATHER_BUFF_TYPE
{
	YIZHAN_BUFF_ADD_SPEED = 0,			// ����
	YIZHAN_BUFF_DEC_SPEED,				// ����
	YIZHAN_BUFF_ADD_GONGJI,				// �ӹ�
	YIZHAN_BUFF_DEC_GONGJI,				// ����
	YIZHAN_BUFF_ADD_FANGYU,				// �ӷ�
	YIZHAN_BUFF_DEC_FANGYU,				// ����
	YIZHAN_BUFF_RECOVER,				// �ظ�
	YIZHAN_BUFF_WUDI,					// �޵�
	YIZHAN_BUFF_BIANSHEN,				// ����
	YIZHAN_BUFF_DEADLY_ATTACK,			// ����һ��

	YIZHAN_BUFF_MAX_TYPE,
};

struct YiZhanDaoDiGatherBuffCfg
{
	YiZhanDaoDiGatherBuffCfg(): buff_seq(0), buff_pro(0), buff_type(-1), is_notice(0)
	{
		memset(param, 0, sizeof(param));
	}

	short buff_seq;
	int	buff_pro;
	short buff_type;
	int param[4];
	short is_notice;
};

class YiZhanDaoDiConfig : public ConfigBase
{
public:
	YiZhanDaoDiConfig();
	~YiZhanDaoDiConfig();

	bool Init(const std::string &path, std::string *err);

	struct JiShaReward
	{
		JiShaReward() : reward_item_count(0) {}

		int reward_item_count;
		ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
	};

	struct JiShaTitle
	{
		JiShaTitle() : jisha_count(0), title_id(0) {}

		int jisha_count;
		int title_id;
	};
	
	struct KillNumReward
	{
		KillNumReward():kill_count(0), reward_count(0)
		{
			memset(reward_item, 0, sizeof(reward_item));
		}
		int kill_count;
		int reward_count;
		ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
	};

	struct JishaScoreReward
	{
		JishaScoreReward() : jisha_score_min(0), jisha_score_max(0), jisha_shengwang(0) {}

		int jisha_score_min;
		int jisha_score_max;
		int jisha_shengwang;
	};

	int GetOpenLevel() { return m_open_level; }
	int GetEnterPos(Posi *enter_pos);

	int GetDeadMaxCount() { return m_dead_max_count; }
	bool RandomRealivePos(Posi *pos);
	int GetJiShaTitle(int jisha_count);
	int GetLuckyInterval() { return m_lucky_interval; }
	const ItemConfigData & GetLuckyItem() { return m_lucky_item; }
	const ItemConfigData * GetJishaRankRewardItem(int rank_idx);

	const int GetJishaRankRewardItemCount(int rank_idx);

	int GetKillHonor() { return m_kill_honor; }

	void GetGongjiGuWuInfo(int *need_gold, int *max_gpngji_per, int *add_gongji_per);
	void GetMaxHPGuWuInfo(int *need_gold, int *max_hp_per, int *add_maxhp_per);
	int GetJishaShengwangReward(int jiasha_score);

	int GetMaxBeKillTimes() { return m_dead_cfg.max_dead_times; }
	const YiZhanDaoDiDeadConfig::ConfigItem * GetDeadCfg(int dead_times);

	int GetGatherID() { return m_gather_id;}
	int GetGatherNum() { return m_gather_num;}
	int GetGatherFirstInterval() { return m_first_refresh_interval;}
	int GetGatherInterval() { return m_refresh_interval;}
	int GetGatherTime() { return m_gather_time;}
	int GetGatherExitTime() { return m_gather_exit_time;}
	std::vector<Posi> & GetGatherPosiVec(){return m_gather_pos_list;}

	int GetGatherBuffToatalPro() { return m_gather_buff_total_pro;}
	std::map<int, YiZhanDaoDiGatherBuffCfg> & GetGatherBuffMap() { return m_gather_buff_map;}

	int GetBroactRoleNumInterval() {return m_broact_role_num_interval;}
	int GetBroactTitleID(){return m_broact_title_id;}

	int GetRankTitle(int rank);

	const KillNumReward* GetKillNumReward(int kill_count, int & index);

private:
	int InitKillNumReward(PugiXmlNode RootElement);

	int m_open_level;															// ���ŵȼ�
	int m_sceneid;																// һվ���� ����ID

	int m_realive_pos_count;													// һվ���� ��������
	Posi m_realive_poslist[YIZHANDAODI_REALIVE_POS_MAX];						// һվ���� ���������

	int m_dead_max_count;														// ������������һ�λ��

	int m_lucky_interval;														// ���˽������
	ItemConfigData m_lucky_item;												// ������Ʒ
	
	int m_reward_count;															// ��ɱ��������
	JiShaReward m_jisha_reward_list[YIZHANDAODI_RANK_NUM + 1];					// ��ɱ���� ǰ��100���� ���һ��ͳһ (100+1)

	int m_jisha_title_count;													// ��ɱ�ƺ�����
	JiShaTitle m_jisha_title_list[YIZHANDAODI_JISHA_TITLE_MAX];					// ��ɱ�ƺ��б�
	
	int m_jisha_score_reward_count;
	JishaScoreReward m_jisha_score_reward_list[YIZHANDAODI_JISHA_SCORE_REWARD_MAX];// ��ɱ���ֶһ������б�

	int m_guwu_gongji_gold;														// ���蹥������Ԫ��
	int m_guwu_gongji_max_per;													// ���蹥��������
	int m_guwu_gongji_percent;													// ����ӳɹ����ٷֱ�

	int m_guwu_maxhp_gold;														// ������Ѫ����Ԫ��
	int m_guwu_maxhp_max_per;													// ������Ѫ������
	int m_guwu_maxhp_percent;													// ����ӳ���Ѫ�ٷֱ�

	int m_kill_honor;															// ��ɱ��������

	YiZhanDaoDiDeadConfig m_dead_cfg;											// ��ɱ����

	int m_gather_id;															// �ɼ���ID
	int m_gather_num;															// �ɼ���ˢ������
	int m_first_refresh_interval;												// �״�ˢ�¼��
	int m_refresh_interval;														// ˢ�¼��
	int m_gather_time;															// �ɼ�ʱ��
	int m_gather_exit_time;														// �ɼ������ʱ��
	std::vector<Posi> m_gather_pos_list;										// �ɼ��������б�

	int m_gather_buff_total_pro;
	std::map<int, YiZhanDaoDiGatherBuffCfg> m_gather_buff_map;

	int m_broact_role_num_interval;
	int m_broact_title_id;

	int m_rank_user_reward_title_id_list[YIZHANDAODI_REWARD_TITLE_RANK_COUNT];

	int m_reward_kill_list_count;
	KillNumReward m_reward_kill_num[YIZHANDAODI_JISHA_REWARD_NUM];

};

#endif // __YIZHANDAODI_CONFIG_HPP__

