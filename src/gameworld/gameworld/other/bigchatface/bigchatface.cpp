#include "bigchatface.hpp"
#include "bigchatfaceconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "protocal/msgbigchatface.h"
#include "servercommon/string/gameworldstr.h"
#include "gameworld/world.h"
#include "other/event/eventhandler.hpp"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"

BigChatFace::BigChatFace() : m_role(NULL) 
{

}

BigChatFace::~BigChatFace()
{

}

void BigChatFace::Init(Role *role, const BigChatFaceParam &param) 
{ 
	m_role = role; 
	m_param = param; 
}

void BigChatFace::GetInitParam(BigChatFaceParam *param)	
{ 
	*param = m_param; 
}

void BigChatFace::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		const BigFaceLevelConfig *bigchatface_cfg = LOGIC_CONFIG->GetBigChatFaceCfg().GetBigFaceLevelCfg(m_param.big_face_level);
		if (NULL != bigchatface_cfg)
		{
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += bigchatface_cfg->maxhp;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += bigchatface_cfg->gongji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += bigchatface_cfg->fangyu;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += bigchatface_cfg->mingzhong;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += bigchatface_cfg->shanbi;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += bigchatface_cfg->baoji;
			m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += bigchatface_cfg->jianren;
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_BIG_CHATFACE, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

bool BigChatFace::OnBigFaceUpLevel()
{
	int max_level_cfg = LOGIC_CONFIG->GetBigChatFaceCfg().GetBigFaceMaxLevel();
	if (m_param.big_face_level < 0 || m_param.big_face_level >= max_level_cfg || m_param.big_face_level >= BIG_FACE_MAX_LEVEL)
	{
		m_role->NoticeNum(errornum::EN_BIG_CHATFACE_LEVEL_LIMIT);
		return false;
	}

	const BigFaceLevelConfig * cur_leve_cfg = LOGIC_CONFIG->GetBigChatFaceCfg().GetBigFaceLevelCfg(m_param.big_face_level);
	if (NULL == cur_leve_cfg)
	{
		m_role->NoticeNum(errornum::EN_BIG_CHATFACE_LEVEL_LIMIT);
		return false;
	}

	const BigFaceLevelConfig * next_leve_cfg = LOGIC_CONFIG->GetBigChatFaceCfg().GetBigFaceLevelCfg(m_param.big_face_level + 1);
	if (NULL == next_leve_cfg)
	{
		m_role->NoticeNum(errornum::EN_BIG_CHATFACE_LEVEL_LIMIT);
		return false;
	}

	int old_level = m_param.big_face_level;

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	{
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		int item_count = 0;

		if (cur_leve_cfg->common_item.item_id > 0)
		{
			stuff_list[item_count].item_id = cur_leve_cfg->common_item.item_id;
			stuff_list[item_count].num = cur_leve_cfg->common_item.num;
			stuff_list[item_count].buyable = false;
			item_count++;
		}

		if (PROF_TYPE_PROF_1 == m_role->GetBaseProf())
		{
			if (cur_leve_cfg->prof_one_item.item_id > 0)
			{
				stuff_list[item_count].item_id = cur_leve_cfg->prof_one_item.item_id;
				stuff_list[item_count].num = cur_leve_cfg->prof_one_item.num;
				stuff_list[item_count].buyable = false;	
				item_count++;
			}
		}
		else if (PROF_TYPE_PROF_2 == m_role->GetBaseProf())
		{
			if (cur_leve_cfg->prof_two_item.item_id > 0)
			{
				stuff_list[item_count].item_id = cur_leve_cfg->prof_two_item.item_id;
				stuff_list[item_count].num = cur_leve_cfg->prof_two_item.num;
				stuff_list[item_count].buyable = false;
				item_count++;
			}
		}
		else if (PROF_TYPE_PROF_3 == m_role->GetBaseProf())
		{
			if (cur_leve_cfg->prof_three_item.item_id > 0)
			{
				stuff_list[item_count].item_id = cur_leve_cfg->prof_three_item.item_id;
				stuff_list[item_count].num = cur_leve_cfg->prof_three_item.num;
				stuff_list[item_count].buyable = false;
				item_count++;
			}
		}
		else if (PROF_TYPE_PROF_4 == m_role->GetBaseProf())
		{
			if (cur_leve_cfg->prof_four_item.item_id > 0)
			{
				stuff_list[item_count].item_id = cur_leve_cfg->prof_four_item.item_id;
				stuff_list[item_count].num = cur_leve_cfg->prof_four_item.num;
				stuff_list[item_count].buyable = false;
				item_count++;
			}
		}
		else
		{
			return false;
		}

		if (item_count <= 0)
		{
			return false;
		}

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, item_count, stuff_list, &consume_list, true))
		{
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "BigChatFace::OnBigFaceUpLevel"))
		{
			return false;
		}
	}

	// 升级
	++ m_param.big_face_level;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_BIG_CHATFACE);
	this->SendBigChatFaceAllInfo();

	if (0 != next_leve_cfg->is_notice)
	{
		int length = 0;

		length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_bigchatface_activate_notice, 
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());

		if (length > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			m_param.reserve = 1;
		}
	}

	// 日志
	gamelog::g_log_bigchatface.printf(LL_INFO, "BigChatFace::OnBigFaceUpLevel user[%d %s] old_level[%d], cur_level[%d]",
		m_role->GetUID(), m_role->GetName(), old_level, m_param.big_face_level);

	return true;
}

void BigChatFace::SendBigChatFaceAllInfo()
{
	Protocol::SCBigChatFaceAllInfo bcfai;
	bcfai.big_face_level = m_param.big_face_level;
	bcfai.reserve_sh = 0;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&bcfai, sizeof(bcfai));
}

short BigChatFace::GetBigChatFaceStatus()
{
	if (m_param.big_face_level > 0)
	{
		return 1;
	}

	return 0;
}