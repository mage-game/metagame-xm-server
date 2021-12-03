#ifndef __FISH_POOL_CONFIG_HPP__
#define __FISH_POOL_CONFIG_HPP__

#include "servercommon/fishpooldef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include <string>
#include <vector>

struct FishPoolOtherConfig
{
	FishPoolOtherConfig() : base_bullet_num(0), buy_bullet_price(0), give_bullet_per_buy(0), 
		day_buy_bullet_limit_times(0), guard_fish_last_time_s(0), today_free_fang_fish_times(0), today_buy_fang_fish_times_limit(0){}

	int base_bullet_num;							// �ӵ���ʼ����
	int buy_bullet_price;							// ���ӵ���Ǯ
	int give_bullet_per_buy;						// ÿ����������ӵ�
	int day_buy_bullet_limit_times;					// ÿ�칺���ӵ���������
	int guard_fish_last_time_s;						// ���������ʱ�� S
	int today_free_fang_fish_times;					// ÿ����ѷ������
	int today_buy_fang_fish_times_limit;
};

// ��Ʒ��
struct FishQualityTypeCfg
{
	FishQualityTypeCfg() : quality(0), count(0), need_time(0), gold(0), up_quality_gold(0), up_quality_pro(0), be_steal_pro(0), exp(0), rune_score(0), steal_limit(0) {}

	int quality;					
	int count;						
	int need_time;					
	int gold;
	int up_quality_gold;
	int up_quality_pro;	
	int be_steal_pro;
	long long exp;
	int rune_score;					
	int steal_limit;				
	ItemConfigData reward_item;
};

// �ӵ�����
struct BuyBulletCfg
{
	BuyBulletCfg() : bullet_count(0), gold(0) {}

	int bullet_count;
	int gold;
};

// �����������
struct BuyFangFishTimesCfg
{
	BuyFangFishTimesCfg() : buy_fang_fish_count(0), gold(0) {}

	int buy_fang_fish_count;
	int gold;
};

// ����Ʒ��Ȩ��
struct FakeFishQualityWeightCfg
{
	FakeFishQualityWeightCfg() : quality(0), weight(0) {}

	int quality;
	int weight;
};

//����ɨ������
struct FishSkipCfg
{
	FishSkipCfg() : skip_fish_quality(0), limit_level(0), consume(0)
	{}

	int skip_fish_quality;
	int limit_level;
	int consume;
};

//���ɨ������
struct FishPoolSkipCfg
{
	FishPoolSkipCfg() : skip_steal_level_limit(0), skip_steal_consume(0)
	{}

	int skip_steal_level_limit;
	int skip_steal_consume;

	FishSkipCfg fish_skip_cfg[FISH_QUALITY_COUNT];
};

class FishPoolConfig : public ConfigBase
{
public:
	FishPoolConfig();
	~FishPoolConfig();

	bool Init(std::string path, std::string *err);

	const FishPoolOtherConfig & GetOtherCfg() { return m_other_cfg; }
	const FishPoolSkipCfg & GetSkipCfg() { return m_fish_pool_skip_cfg; }
	bool CanSteal(int fish_type, int raise_last_time_s);

	FishQualityTypeCfg * GetFishQualityCfg(int quality);
	int GetBuyBulletGold(int times);
	int GetBuyFangFishGold(int times);
	int GetFakeFishQuality();

private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitFishQualityTypeCfg(PugiXmlNode RootElement);
	int InitBuyBulletCfg(PugiXmlNode RootElement);
	int InitBuyFangFishTimesCfg(PugiXmlNode RootElement);
	int InitFakeFishQualityWeightCfg(PugiXmlNode RootElement);
	int InitFishPoolSkipCfg(PugiXmlNode RootElement);

	FishPoolOtherConfig m_other_cfg;

	FishQualityTypeCfg m_quality_typ_cfg[FISH_QUALITY_COUNT];
	std::vector<BuyBulletCfg> m_buy_buttle_cfg;
	std::vector<BuyFangFishTimesCfg> m_buy_fang_fish_times_cfg;
	std::vector<FakeFishQualityWeightCfg> m_fake_fish_quality_cfg;
	FishPoolSkipCfg m_fish_pool_skip_cfg;
	int m_fake_fish_quality_total_weight;
};

#endif


