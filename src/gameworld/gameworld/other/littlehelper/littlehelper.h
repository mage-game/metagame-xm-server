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

	bool GiveDropItemList(lhns::complete_type_t type);		//�������б�
	bool Consume(lhns::complete_type_t type);				//����
	bool ConsumeAndReward(lhns::complete_type_t type);		//�����뽱��

	void Opera(const Protocol::CSLittleHelperOpera::Req req);
	void OperaReq(const Protocol::CSLittleHelperOpera::Req req);
	void OperaRepeatReq(const Protocol::CSLittleHelperOpera::Req req_list[Protocol::LITTLE_HELPER_COMPLETE_TYPE_MAX], const int count);

	void EasyBoss(lhns::complete_type_t type);				//��boss
	void DifficultBoss(lhns::complete_type_t type);			//����boss
	void BabyBoss(lhns::complete_type_t type);				//����boss
	bool CalcBabyBossConsume(lhns::complete_type_t type);
	void SuitBoss(lhns::complete_type_t type);				//��װboss
	bool CalcSuitBossConsume(lhns::complete_type_t type);
	void DemonBoss(lhns::complete_type_t type);				//��ħboss
	void OnKillMonster(lhns::complete_type_t type);

	void EscortFairy(lhns::complete_type_t type);			//������Ů
	void CheckBuyEscortTimes(int type, int repeat_times);	//���Թ����ʹ���
	void AutoBuildTowerFB(short repeat_times, int flush_extra_num);	//������
	
	void SendRefreshInfo(short type);
	void CheckSendDropItemListInfo();

private:

	Role *m_role;

	std::map<ItemID, std::vector<Protocol::SCLittleHelperItemInfo::RewardItem> > m_drop_item_vec_map;
};

#endif // __LITTLE_HELPER_H__

