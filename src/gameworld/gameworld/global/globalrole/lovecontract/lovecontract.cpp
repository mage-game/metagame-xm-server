#include "lovecontract.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/qingyuan/qingyuanconfig.hpp"
#include "servercommon/errornum.h"
#include "obj/character/role.h"
#include "item/itempool.h"
#include "scene/scene.h"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/usercache/usercache.hpp"
#include "world.h"
#include "servercommon/noticenum.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "item/knapsack.h"

LoveContract::LoveContract(GlobalRole *g_role) : IGlobalRoleItem(g_role)
{
}

LoveContract::~LoveContract()
{
}

void LoveContract::Init(const LoveContractData &love_contract_attr)
{
	m_love_contract_param = love_contract_attr;
}

void LoveContract::GetAttr(LoveContractData &love_contract_attr)
{
	love_contract_attr = m_love_contract_param;
}

void LoveContract::OnDivorce()
{
	m_love_contract_param.Reset();
	this->OnDataChange();
	this->SendLoveContractInfo();
}

void LoveContract::OnMarry()
{
	this->SendLoveContractInfo();
}

void LoveContract::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;
	Role *role = this->GetRole();
	if (NULL == role) return;

	this->ChekCanReceiveDayNum(role);
	this->SendLoveContractInfo();
}

void LoveContract::OnRoleLogin(Role *role)
{
	this->ChekCanReceiveDayNum(role);
}

void LoveContract::ChekCanReceiveDayNum(Role *role)
{
	if (NULL == role) return;

	if (EngineAdapter::Instance().DayID() != role->GetLastOnlineDayID())
	{
		if (m_love_contract_param.love_contract_start_timestamp != 0)
		{
			m_love_contract_param.can_receive_day_num ++;
			this->OnDataChange();
		}
	}
}

void LoveContract::OnSendNotice(LoveContractNotice notice)
{
	this->InsertNotice(notice);

	this->SendLoveContractInfo();
}

void LoveContract::BuyLoveContractReq(Role *role)
{
	if (NULL == role) return;

	LoveContract * lover_love_contract = GLOBAL_ROLE_MGR.GetLoveContract(role->GetLoverUid());
	if (NULL == lover_love_contract || 0 != lover_love_contract->GetContractStartTimestamp()) return;

	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
	if (!role->GetKnapsack()->GetMoney()->UseGold(other_cfg.lovecontract_price, "LoveContract"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	lover_love_contract->m_love_contract_param.Reset();

	lover_love_contract->BuyLoveContract();

	this->SendLoveContractInfo();

	lover_love_contract->SendLoveContractInfo();

	lover_love_contract->OnDataChange(); 
	role->NoticeNum(noticenum::NT_BUY_LOVE_CONTRACT_SUCCESS);
}

void LoveContract::BuyLoveContract()
{ 
	m_love_contract_param.love_contract_start_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_love_contract_param.can_receive_day_num = 0;
}

void LoveContract::FetchLoveContractReward(Role *role, int day_num, LoveContractNotice notice)
{
	if (NULL == role) return;

	if (day_num < 0 || day_num >= LOVE_CONTRACT_DAY_COUNT) return;

	if (0 == m_love_contract_param.love_contract_start_timestamp) return;

	if (day_num > m_love_contract_param.can_receive_day_num) return;

	const QingyuanLoveContractReward *reward_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetLoveContractReward(day_num);
	if (NULL == reward_cfg) return;

	if (0 != (m_love_contract_param.love_contract_reward_falg & (1 << day_num))) return;

//	role->GetKnapsack()->PutListOrMail(reward_cfg->reward_item_list, PUT_REASON_ITEM_LOVE_CONTRACT_REWARD);

	role->GetKnapsack()->GetMoney()->AddGoldBind(reward_cfg->reward_gold_bind, "LoveContract::FetchLoveContractReward");

	m_love_contract_param.love_contract_reward_falg |= (1 << day_num);

	this->InsertNotice(notice);

	bool is_contract_finish = true;
	for (int i = 0; i < LOVE_CONTRACT_DAY_COUNT; i++)
	{
		if (0 == (m_love_contract_param.love_contract_reward_falg & (1 << i)))
		{
			is_contract_finish = false;
			break;
		}
	}

	// ÁìÍê½±Àø
	if (is_contract_finish)
	{
		m_love_contract_param.love_contract_reward_falg = 0;
		m_love_contract_param.love_contract_start_timestamp = 0;
		m_love_contract_param.can_receive_day_num = -1;

		const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
		static MailContentParam contentparam; contentparam.Reset();
		if (FEMALE == this->GetRole()->GetSex())
		{
			contentparam.item_list[0].item_id = other_cfg.lovecontract_title_girl.item_id;
			contentparam.item_list[0].num = other_cfg.lovecontract_title_girl.num;
			contentparam.item_list[0].is_bind = (other_cfg.lovecontract_title_girl.is_bind) ? 1 : 0;
		}
		else
		{
			contentparam.item_list[0].item_id = other_cfg.lovecontract_title_boy.item_id;
			contentparam.item_list[0].num = other_cfg.lovecontract_title_boy.num;
			contentparam.item_list[0].is_bind = (other_cfg.lovecontract_title_boy.is_bind) ? 1 : 0;
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_lovecontract_finish);
		if (length > 0)
		{
			MailRoute::MailToUser(role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}

	LoveContract * lover_love_contract = GLOBAL_ROLE_MGR.GetLoveContract(role->GetLoverUid());
	if (NULL != lover_love_contract)
	{
		lover_love_contract->SendLoveContractInfo();
	}

	this->SendLoveContractInfo();
	this->OnDataChange();
}

void LoveContract::NoticeLoverBuyLoveContract(Role *role)
{
	if (NULL == role) return;

	LoveContract * love_contract = GLOBAL_ROLE_MGR.GetLoveContract(role->GetUID());
	if (NULL == love_contract) return;

	if (0 != love_contract->GetContractStartTimestamp())
	{
		return;
	}

	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role->GetLoverUid()));
	if (NULL == lover_role)
	{
		role->NoticeNum(errornum::EN_BABY_LOVER_NOT_ONLINE);
		return;
	}

	LoveContract * lover_love_contract = GLOBAL_ROLE_MGR.GetLoveContract(role->GetLoverUid());
	if (NULL == lover_love_contract) return;

	lover_love_contract->SendLoveContractInfo(Protocol::LOVE_CONTRACT_INFO_TYPE_NOTICE_BUY_CONTRACT);
}

void LoveContract::SendLoveContractInfo(int info_type)
{	
	Role *role = this->GetRole();
	if (NULL == role) return;

	static Protocol::SCQingyuanLoveContractInfo pro;
	pro.info_type = info_type;
	pro.self_notice_list_count = 0;
	pro.lover_notice_list_count = 0;
	pro.lover_love_contract_timestamp = 0;
	for (int i = 0; i < MAX_NOTICE_COUNT; i++)
	{
		pro.self_notice_list[i].time = 0;
		memset(pro.self_notice_list[i].contract_notice, 0, sizeof(LoveContractNotice));

		pro.lover_notice_list[i].time = 0;
		memset(pro.lover_notice_list[i].contract_notice, 0, sizeof(LoveContractNotice));
	}
	
	pro.can_receive_day_num = m_love_contract_param.can_receive_day_num;
	pro.self_love_contract_reward_flag = m_love_contract_param.love_contract_reward_falg;
	
	this->GetContractNotice(pro.self_notice_list, pro.self_notice_list_count);

	LoveContract * lover_love_contract = GLOBAL_ROLE_MGR.GetLoveContract(role->GetLoverUid());
	if (NULL != lover_love_contract)
	{
		pro.lover_love_contract_timestamp = lover_love_contract->GetContractStartTimestamp();
		lover_love_contract->GetContractNotice(pro.lover_notice_list, pro.lover_notice_list_count);
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void LoveContract::GetContractNotice(Protocol::ContractNoticePack (&contract_notice)[MAX_NOTICE_COUNT], int &notice_list_count)
{
	for (int i = 0; i < MAX_NOTICE_COUNT; i++)
	{
		if (0 != m_love_contract_param.love_contract_notice_list[i].notice_timestamp)
		{
			contract_notice[i].time = m_love_contract_param.love_contract_notice_list[i].notice_timestamp;
			F_STRNCPY(contract_notice[i].contract_notice, m_love_contract_param.love_contract_notice_list[i].contract_notice, sizeof(LoveContractNotice));

			notice_list_count ++;
		}
	}
}

void LoveContract::InsertNotice(LoveContractNotice notice)
{
	int insert_succ = false;
	for (int i = 0; i < MAX_NOTICE_COUNT; i++)
	{
		for (int j = 0; j < MAX_NOTICE_COUNT; j++)
		{
			if (m_love_contract_param.love_contract_notice_list[i].notice_timestamp > m_love_contract_param.love_contract_notice_list[j].notice_timestamp)
			{
				break;
			}

			if (MAX_NOTICE_COUNT - 1 == j)
			{
				insert_succ = true;
				F_STRNCPY(m_love_contract_param.love_contract_notice_list[i].contract_notice, notice, sizeof(LoveContractNotice))
				m_love_contract_param.love_contract_notice_list[i].notice_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				break;
			}
		}

		if (insert_succ)
		{
			break;
		}
	}
}