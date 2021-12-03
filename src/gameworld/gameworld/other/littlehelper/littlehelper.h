#ifndef __LITTLE_HELPER_H__
#define __LITTLE_HELPER_H__

#include <map>
#include <vector>
#include <string>

#include "servercommon/rolelotterycountdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "config/littlehelperconfig.h"
#include "protocal/msgother.h"

class Role;

class LittleHelper
{
public:
	LittleHelper();
	~LittleHelper();

	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);

	void SetRole(Role *role) { m_role = role; }

	bool GiveDropItemList(lhns::complete_type_t type);		//发掉落列表
	bool Consume(lhns::complete_type_t type);				//消耗
	bool ConsumeAndReward(lhns::complete_type_t type);		//消耗与奖励

	void Opera(const Protocol::CSLittleHelperOpera::Req req);
	void OperaReq(const Protocol::CSLittleHelperOpera::Req req);
	void OperaRepeatReq(const Protocol::CSLittleHelperOpera::Req req_list[Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX], const int count);

	void EasyBoss(lhns::complete_type_t type);				//简单boss
	void DifficultBoss(lhns::complete_type_t type);			//困难boss
	void BabyBoss(lhns::complete_type_t type);				//宝宝boss
	bool CalcBabyBossConsume(lhns::complete_type_t type);
	void SuitBoss(lhns::complete_type_t type);				//套装boss
	bool CalcSuitBossConsume(lhns::complete_type_t type);
	void DemonBoss(lhns::complete_type_t type);				//神魔boss
	void OnKillMonster(lhns::complete_type_t type);

	void EscortFairy(lhns::complete_type_t type);			//护送仙女
	void CheckBuyEscortTimes(int type, int repeat_times);	//尝试购买护送次数
	void AutoBuildTowerFB(short repeat_times, int flush_extra_num);	//塔防本
	
	void SendRefreshInfo(short type);
	void CheckSendDropItemListInfo();

private:

	Role *m_role;

	std::map<ItemID, std::vector<Protocol::SCLittleHelperItemInfo::RewardItem> > m_drop_item_vec_map;
};

#endif // __LITTLE_HELPER_H__

