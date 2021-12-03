#include "randactivitymagicshop.hpp"
#include "config/sharedconfig/sharedconfig.h"
#include "global/randactivity/randactivitymanager.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

RandActivityMagicShop::RandActivityMagicShop(RandActivityManager *rand_activity_manager)
: RandActivity(rand_activity_manager, RAND_ACTIVITY_TYPE_MAGIC_SHOP)
{
 
}

RandActivityMagicShop::~RandActivityMagicShop()
{

}

void RandActivityMagicShop::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	//unsigned int now_second = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	//// 开启活动开服天数集合, 确保每个开启间隔大于活动开启持续时间, 是否应该判断活动应该关闭才设置呢
	//static std::set<int> open_act_opengame_day_set = { 5, 8, 12, 15, 19, 22, 26 };

	//if (!CrossConfig::Instance().IsHiddenServer())
	//{
	//	int open_server_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), now_second) + 1;

	//	if (open_act_opengame_day_set.find(open_server_day) != open_act_opengame_day_set.end()) // 开服第x天，强制开启活动
	//	{
	//		// 活动结束时间在切天前
	//		unsigned int fixed_interval = now_second - static_cast<unsigned int>(GetZeroTime(now_second)) + 3;
	//		m_rand_activity_manager->OnForceSetActivityTime(this->GetActivityType(), now_second, now_second + SECOND_PER_DAY * 3 - fixed_interval);
	//	}
	//}
}


