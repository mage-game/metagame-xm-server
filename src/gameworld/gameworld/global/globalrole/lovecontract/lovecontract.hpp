#ifndef __LOVCONTRACT_HPP__
#define __LOVCONTRACT_HPP__

#include "obj/character/attribute.hpp"
#include "global/globalrole/globalrole.hpp"
#include "servercommon/serverdef.h"
#include "protocal/msgother.h"

class Role;

class LoveContract : public IGlobalRoleItem
{
public:
	LoveContract(GlobalRole *g_role);
	~LoveContract();

	virtual void Init(const LoveContractData &love_contract_attr);
	virtual void GetAttr(LoveContractData &love_contract_attr);

	unsigned int GetContractStartTimestamp(){ return m_love_contract_param.love_contract_start_timestamp; }
	void BuyLoveContract();

	void BuyLoveContractReq(Role *role);
	void FetchLoveContractReward(Role *role, int day_num, LoveContractNotice notice);
	void NoticeLoverBuyLoveContract(Role *role);
	void SendLoveContractInfo(int info_type = Protocol::LOVE_CONTRACT_INFO_TYPE_NORMAL);

	void OnDivorce();
	void OnMarry();
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnRoleLogin(Role *role);

	void GetContractNotice(Protocol::ContractNoticePack (&contract_notice)[MAX_NOTICE_COUNT], int &notice_list_count);
	void ChekCanReceiveDayNum(Role *role);

	void OnSendNotice(LoveContractNotice notice);

private:

	void InsertNotice(LoveContractNotice notice);

	LoveContractData m_love_contract_param;
};

#endif // __LOVCONTRACT_HPP__