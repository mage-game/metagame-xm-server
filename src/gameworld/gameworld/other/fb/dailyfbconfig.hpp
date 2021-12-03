#ifndef __DAILYFB_CONFIG_HPP__
#define __DAILYFB_CONFIG_HPP__

#include "servercommon/vipdef.hpp"
#include "gamedef.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/maildef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include <map>
#include <vector>

static const int MAX_RESET_COUNT_CFG = 10;

struct DailyFbItem
{
	DailyFbItem() : dailyfb_type(0), role_level(0), complete_task_id(0), day_times(0), cost_gold(0), scene_id(0), birth_pos(0, 0), next_wave_refresh_interval(0) {}

	int dailyfb_type;
	int role_level;
	int	complete_task_id;
	int day_times;
	int cost_gold;
	int scene_id;
	Posi birth_pos;
	unsigned int next_wave_refresh_interval;
};

// 经验副本其他配置
struct ExpFbOtherCfg
{
	ExpFbOtherCfg(): time_limit(0), interval_time(0), day_times(0), buff_add_gongji_per(0), max_buff_time(0), buff_cost(0), start_refresh_monster_count(0), monster_count(0) ,extra_exp_per(0), one_bo_time(0), two_bo_time(0), first_time_limit(0), baodi_exp1(0), baodi_exp2(0), baodi_exp3(0), refresh_cd1(0), refresh_cd2(0), refresh_cd3(0), 
		total_wave(0), fir_wave_refresh_count(0), sec_wave_refresh_count(0), other_wave_refresh_count(0), exp_factor1(0), exp_factor2(0), exp_factor3(0) {}

	int time_limit;
	unsigned int interval_time;
	int day_times;
	int buff_add_gongji_per;
	int max_buff_time;
	int buff_cost;
	int start_refresh_monster_count;
	int monster_count;
	int extra_exp_per;
	ItemConfigData item_stuff;
	int one_bo_time;
	int two_bo_time;
	int first_time_limit;
	long long baodi_exp1;
	long long baodi_exp2;
	long long baodi_exp3;
	int refresh_cd1;
	int refresh_cd2;
	int refresh_cd3;
	int total_wave;
	int fir_wave_refresh_count;				// 第一波刷新次数
	int sec_wave_refresh_count;				// 第二波刷新次数
	int other_wave_refresh_count;			// 以后刷新次数
	double exp_factor1;						// 第一波经验系数
	double exp_factor2;						// 第二波经验系数
	double exp_factor3;						// 第三波经验系数
};

// 经验本
struct ExpFbWaveItem
{
	ExpFbWaveItem() : level(0), monster_id(0), monster_id2(0), monster_id3(0) {}

	short level;
	int monster_id;
	int monster_id2;
	int monster_id3;
};

// 经验本vip可购买次数
struct ExpFbVipPayCfg
{
	ExpFbVipPayCfg() : vip_level(0), pay_time(0){}

	int vip_level;
	int pay_time;
};

// 经验本购买配置
struct ExpFbResetCfg
{
	ExpFbResetCfg() : reset_time(0), need_gold(0){}

	int reset_time;
	int need_gold;
};

// 经验本小怪坐标
struct ExpFbWaveMonsterPos
{
	ExpFbWaveMonsterPos() : pos(0, 0) {}
	Posi pos;
};

// 铜币本
struct CoinFbWaveItem
{
	CoinFbWaveItem() : wave(0), monster_id(0), monster_count(0), next_wave_time_gap(0) {}

	short wave;
	int monster_id;
	int monster_count;
	int next_wave_time_gap;
};

class DailyFBConfig : public ConfigBase
{
public:
	DailyFBConfig();
	~DailyFBConfig();

	bool Init(const std::string &path, std::string *err);

	const DailyFbItem *GetDailyFbItemCfg(int dailyfb_type);
	int GetDailyFbTypeBySceneId(int scene_id);
	int GetMaxDailyFbType() { return m_max_dailyfb_type; }

	const ExpFbWaveItem *GetExpFbWaveItemCfg(int level);
	std::vector<ExpFbWaveMonsterPos> &GetExpMonsterPos() { return m_exp_monster_pos; }

	const CoinFbWaveItem *GetCoinFbWaveItemCfg(short wave);
	std::vector<Posi> &GetCoinPath() { return m_coin_path; }
	int GetCoinMaxWave() { return m_coin_max_wave; }

	void GetDailyFindReward(int dailyfb_type, int role_level, int *get_exp, int *get_coin);
	const int GetExpFbResetGold(int reset_time);
	const int GetVipPayTime(int vip_level);
	int GetExpRefreshCdByMemberNum(int member_num);
	long long GetExpBaodiByEnterTimes(int enter_times);

	const ExpFbOtherCfg & GetExpOtherCfg(){ return m_expfb_other_cfg; }
	double GetDailyFbFindFactor();

private:
	int InitDailyFbCfg(PugiXmlNode RootElement);

	int InitExpOtherCfg(PugiXmlNode RootElement);
	int InitExpWaveCfg(PugiXmlNode RootElement);
	int InitExpMonsterPosCfg(PugiXmlNode RootElement);
	int InitCoinWaveCfg(PugiXmlNode RootElement);
	int InitCoinPathCfg(PugiXmlNode RootElement);
	int InitExpResetCfg(PugiXmlNode RootElement);
	int InitExpFbVipPayCfg(PugiXmlNode RootElement);

	int m_max_dailyfb_type;		//日常副本类型最大值
	std::map<int, DailyFbItem> m_dailyfb_map;

	std::vector<ExpFbWaveItem> m_exp_wave_vector;			//经验本波数
	std::vector<ExpFbWaveMonsterPos> m_exp_monster_pos;		//经验本怪物出生
	ExpFbResetCfg m_expfb_reset_cfg[MAX_RESET_COUNT_CFG];	//经验本购买配置
	ExpFbVipPayCfg m_exp_vip_pay_cfg[MAX_VIP_LEVEL + 1];	//经验本vip对应可以购买的次数
	ExpFbOtherCfg m_expfb_other_cfg;						//经验本其他配置

	std::vector<CoinFbWaveItem> m_coin_wave_vector;			//铜币本波数
	std::vector<Posi> m_coin_path;							//铜币本怪物行走路径
	int m_coin_max_wave;									//铜币本最大波数		
};

#endif // __DAILYFB_CONFIG_HPP__

