#include "roleyaojiang.hpp"

#include "config/logicconfigmanager.hpp"
#include "other/yaojiang/yaojiangconfig.hpp"
#include "obj/character/role.h"
#include "world.h"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "item/knapsack.h"

RoleYaojiang::RoleYaojiang() : m_role(NULL)
{

}

RoleYaojiang::~RoleYaojiang()
{

}

void RoleYaojiang::Init(Role *role)
{
	m_role = role;
}

void RoleYaojiang::OnYaojiangReq(int yaojiang_type)
{
	int index = -1;
	const YaojiangItemCfg *cfg = LOGIC_CONFIG->GetYaojiangConfig().RandYaojiangItemCfg(yaojiang_type, &index);
	if (NULL == cfg)
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->ConsumeItem(LOGIC_CONFIG->GetYaojiangConfig().GetCostItemId(yaojiang_type), 1, "Yaojiang"))
	{
		m_role->NoticeNum(errornum::EN_NO_YAOJIANG_ITEM);
		return;
	}

	if (!m_role->GetKnapsack()->Put(cfg->item, PUT_REASON_YAOJIANG))
	{
		return;
	}

	if (0 != cfg->is_notice)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_yaojiang_notice,
			UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(), cfg->item.item_id);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	Protocol::SCYaojiangInfo yji;
	yji.rand_index = index;
	yji.item_id = cfg->item.item_id;
	yji.num = cfg->item.num;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&yji, sizeof(yji));
}

