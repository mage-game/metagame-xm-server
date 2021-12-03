#include "crossserverconfig.h"
#include "servercommon/serverconfig/crosscommonconfig.hpp"
#include "servercommon/serverconfig/crossxiuluotowercommonconfig.hpp"
#include "servercommon/serverconfig/cross1v1commonconfig.hpp"
#include "servercommon/serverconfig/crossmultiuserchallengeconfig.hpp"
#include "servercommon/serverconfig/crosstuanzhanconfig.hpp"
#include "servercommon/serverconfig/crosspastureconfig.hpp"
#include "servercommon/serverconfig/crossbossconfig.hpp"
#include "servercommon/serverconfig/crosshotspringconfig.hpp"
#include "servercommon/serverconfig/crossshuijingconfig.hpp"
#include "servercommon/serverconfig/crossguildbattleconfig.hpp"
#include "servercommon/serverconfig/crossdarknightconfig.hpp"
#include "servercommon/serverconfig/cross3v3config.hpp"
#include "servercommon/serverconfig/crossfishingconfig.hpp"
#include "servercommon/serverconfig/doublesidecrossactivity/crossnightfightconfig.hpp"
#include "servercommon/serverconfig/doublesidecrossactivity/crossmessbattleconfig.hpp"
#include "servercommon/serverconfig/crossliekunfbconfig.hpp"
#include "servercommon/serverconfig/crossrankrewardconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/serverconfig/crossvipbossconfig.hpp"
#include "servercommon/serverconfig/crossmizangbossconfig.hpp"
#include "servercommon/serverconfig/crossyoumingbossconfig.hpp"
#include "servercommon/serverconfig/cloudpurchaseconfig.hpp"
#include "servercommon/serverconfig/crossrandactivityopenconfig.hpp"
#include "servercommon/serverconfig/crossbianjingzhidiconfig.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "servercommon/serverconfig/husongshuijingconfig.hpp"

CrossServerConfig::CrossServerConfig()
{

}

CrossServerConfig::~CrossServerConfig()
{

}

CrossServerConfig & CrossServerConfig::Instance()
{
	static CrossServerConfig inst;
	return inst;
}

bool CrossServerConfig::Init(const std::string &path, std::string *err)
{
	std::string configpath;

	// 跨服里也需要读cross.xml配置，为了跨服进程里判断当前跨服类型相关操作（比如有些指定跨服类型才开启的跨服活动）
	configpath = "../cross.xml";
	if (!CrossConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_common.xml";
	if (!CrossCommonConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_xiuluotower.xml";
	if (!CrossXiuluoTowerCommonConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_1v1.xml";
	if (!Cross1V1CommonConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

// 	configpath = "cross_mutiuserchallenge.xml";
// 	if (!CrossMultiUserChallengeConfig::Instance().Init(path + configpath, err))
// 	{
// 		return false;
// 	}

	configpath = "kuafu_tvt.xml";
	if (!Cross3v3Config::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_tuanzhan.xml";
	if (!CrossTuanzhanConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_pasture.xml";
	if (!CrossPastureConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_boss.xml";
	if (!CrossBossConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_hotspring.xml";
	if (!CrossHotSpringConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_shuijing.xml";
	if (!CrossShuiJingConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_guildbattle.xml";
	if (!CrossGuildBattleConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}
	configpath = "cross_dark_night.xml";
	if (!CrossDarkNightConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}
		
	configpath = "cross_fishing.xml";
	if (!CrossFishingConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_night_fight.xml";
	if (!CrossNightFightConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_mess_batlle.xml";
	if (!CrossMessBattleConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_liekun.xml";
	if (!CrossLieKunFbConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "crossrank_reward.xml";
	if (!CrossRankRewardConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_vip_boss.xml";
	if (!CrossVipBossConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_mizang_boss.xml";
	if (!CrossMizangBossConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_youming_boss.xml";
	if (!CrossYoumingBossConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cloud_purchase.xml";
	if (!CloudPurchaseConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_bianjingzhidi.xml";
	if (!CrossBianJingZhiDiConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	configpath = "cross_husong_shuijing.xml";
	if (!HusongShuijingConfig::Instance().Init(path + configpath, err))
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////

	char rand_open_cfg_path[256] = { 0 };
	SNPRINTF(rand_open_cfg_path, sizeof(rand_open_cfg_path), "cross_randactivity_open_cfg_%d.xml", LocalConfig::Instance().GetRandActZhuanfuType());
	if (!CrossRandActivityOpenConfig::Instance().Init(path + rand_open_cfg_path, err))
	{
		return false;
	}

	return true;
}



