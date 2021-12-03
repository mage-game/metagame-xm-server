#ifndef __CROSS_BOSS_CONFIG_HPP__
#define __CROSS_BOSS_CONFIG_HPP__

#include "servercommon/crossdef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <vector>
#include <map>

struct CrossBossOtherCfg
{
	CrossBossOtherCfg() : is_open(false), gather_time_s(0), gather_time_s1(0), buy_relive_times_need_gold(0), relive_time(0), monster_flush_time_s(0), treasure_crystal_gather_times(0),
		treasure_crystal_flush_time_s(0), treasure_crystal_flush_num(0), ordinary_crystal_gather_times(0), ordinary_crystal_flush_time_s(0),
		ordinary_crystal_gather_id(0), daily_boss_num(0), reward_exp_cd_s(0), gather_reduce_hp_precent(0), gather_reduce_hp_cd_s(0), relive_tire_max_level(0),
		relive_tire_time_s(0), relive_tire_clear_time_s(0)
	{
	}

	bool is_open;															// �Ƿ񿪷ű�����-�ܿ���

	int gather_time_s;														// �ɼ�ʱ��
	int gather_time_s1;														// �ɼ�ʱ��
	int buy_relive_times_need_gold;											// ����һ�θ����������Ԫ����
	int relive_time;														// ����ʱ��
	int monster_flush_time_s;												// С��ˢ��ʱ��
	int treasure_crystal_gather_times;										// ��ϧˮ���ɼ�����
	int treasure_crystal_flush_time_s;										// ��ϧˮ��ˢ��ʱ��
	int treasure_crystal_flush_num;											// ��ϧˮ��ˢ�¸���
	int ordinary_crystal_gather_times;										// ��ͨˮ���ɼ�����
	int ordinary_crystal_flush_time_s;										// ��ͨˮ��ˢ��ʱ��
	int ordinary_crystal_gather_id;											// ��ͨˮ���ɼ�id
	int daily_boss_num;														// bossƣ��
	int reward_exp_cd_s;													// �Ӿ���cd
	int gather_reduce_hp_precent;											// Ѫ�����ٰٷֱ�
	int gather_reduce_hp_cd_s;												// Ѫ������cd
	int relive_tire_max_level;                                              // ����ƣ������
	int relive_tire_time_s;                                                 // ����ʱ�䣨�룩
	int relive_tire_clear_time_s;                                           // ����ƣ��ʱ��
	ItemConfigData ordinary_crystal_reward_item;
};

struct CrossBossMonsterCfg
{
	struct MonsterCfg
	{
		MonsterCfg() : monster_index(0), pos_x(0), pos_y(0), monster_id(0)
		{
		}

		int monster_index;
		int pos_x;
		int pos_y;
		int monster_id;
	};

	int monster_count;
	MonsterCfg monster_list[CROSS_BOSS_MAX_MONSTER_NUM];
};

struct CrossBossBossCfg
{
	struct BossCfg
	{
		BossCfg() : boss_index(0), boss_id(0), flush_interval(0), flush_pos_x(0), flush_pos_y(0), min_delta_level(0), max_delta_level(0)
		{
		}

		int boss_index;
		int boss_id;
		int flush_interval;
		int flush_pos_x;
		int flush_pos_y;
		int min_delta_level;
		int max_delta_level;
	};

	int boss_count;
	BossCfg boss_list[MAX_CROSS_BOSS_PER_SCENE];
};

struct CrossBossLayerCfg
{
	CrossBossLayerCfg() : layer_index(0), level_limit(0), scene_id(0), treasure_crystal_gather_id(0),
		enter_pos(0, 0), is_flush_monster(0)
	{
	}

	int layer_index;
	int level_limit;
	int scene_id;
	int treasure_crystal_gather_id;
	ItemConfigData treasure_crystal_reward_item;
	Posi enter_pos;
	int is_flush_monster;
};

struct CrossBossOrdinaryCrystalPosCfg
{
	CrossBossOrdinaryCrystalPosCfg() : index(0) {}

	int index;
	Posi pos;
};

struct CrossBossTreasureCrystalPosCfg
{
	CrossBossTreasureCrystalPosCfg() : pos_num(0) {}

	int pos_num;
	Posi pos_list[CROSS_BOSS_MAX_TREASURE_CRYSTAL_POS_NUM];
};

struct CrossBossFlushConfig
{
	CrossBossFlushConfig() : seq(0), time(0) {}

	int seq;
	int time;
};

struct CrossBossEnterTimeConfig
{
	CrossBossEnterTimeConfig() : seq(0), begin_time(0), end_time(0) {}
	int seq;
	int begin_time;
	int end_time;
};

class CrossBossConfig
{
public:
	CrossBossConfig();
	~CrossBossConfig();

	static CrossBossConfig & Instance();
	bool Init(const std::string &path, std::string *err);

	const CrossBossOtherCfg & GetOtherCfg() { return m_other_cfg; }

	int GetSceneIdList(int scene_id_list[CROSS_BOSS_SCENE_MAX]);
	int GetSceneIdFirst();  // ��ȡ��һ����Ч��sceneId

	int GetLayerIndexBySceneId(int scene_id) const;
	int GetLevelLimitBySceneLevel(int scene_level) const;
	int GetCrossBossMonsterCount(int layer) const;
	int GetCrossBossBossCount(int layer) const;
	long long GetCrossBossRewardExp(int level);
	int GetTreasureGatherIdBySceneId(int scene_id) const;
	int GetOrdinaryCrystalCount() const { return m_cross_boss_ordinary_crystal_num; }
	int GetIndexByBossId(int layer, int boss_id) const;

	const CrossBossTreasureCrystalPosCfg *GetTreasureCrystalPointListByLayer(int layer) const;

	const CrossBossMonsterCfg::MonsterCfg *GetCrossBossMonsterCfgByLayer(int layer, int index) const;
	const CrossBossBossCfg::BossCfg *GetCrossBossBossCfgByLayer(int layer, int index) const;
	const CrossBossBossCfg::BossCfg *GetCrossBossBossCfgByMonsterId(int scene_id, unsigned short monster_id) const;
	const CrossBossOrdinaryCrystalPosCfg *GetCrossBossOrdinaryCrystalPosCfg(int index) const;
	const CrossBossLayerCfg *GetCrossBossLayerCfg(int layer) const;
	int GetCrossBossNextFlushTimeInterval(unsigned int now_second);	// ���㵱ǰʱ�����´�ˢ�ļ��
	bool CanEnterCrossBoss(unsigned int now_second);                // ���㵱ǰʱ���ܷ������boss

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitRewardExpCfg(PugiXmlNode RootElement);
	int InitMonsterCfg(PugiXmlNode RootElement);
	int InitBossCfg(PugiXmlNode RootElement);
	int InitLayerCfg(PugiXmlNode RootElement);
	int InitOrdinaryCrystalPosCfg(PugiXmlNode RootElement);
	int InitTreasureCrystalPosCfg(PugiXmlNode RootElement);
	int InitCrossBossFlushCfg(PugiXmlNode RootElement);
	int InitCrossBossEnterTimeCfg(PugiXmlNode RootElement);

	CrossBossOtherCfg m_other_cfg;

	std::map<int, long long > m_reward_exp_map;

	CrossBossMonsterCfg m_cross_boss_monster_list[CROSS_BOSS_SCENE_MAX];
	CrossBossBossCfg m_cross_boss_boss_list[CROSS_BOSS_SCENE_MAX];

	int m_cross_boss_scene_count;
	CrossBossLayerCfg m_cross_boss_layer_list[CROSS_BOSS_SCENE_MAX];

	int m_cross_boss_ordinary_crystal_num;
	CrossBossOrdinaryCrystalPosCfg m_ordinary_crystal_pos_list[CROSS_BOSS_MAX_ORDINARY_CRYSTAL];

	CrossBossTreasureCrystalPosCfg m_treasure_crystal_pos_list[CROSS_BOSS_SCENE_MAX];

	std::vector<CrossBossFlushConfig> m_cross_boss_flush_cfg_vec;

	std::vector<CrossBossEnterTimeConfig> m_cross_boss_enter_time_vec;
};
#endif // __CROSS_BOSS_CONFIG_HPP__

