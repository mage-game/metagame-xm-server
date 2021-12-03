#include "shenqi.hpp"
#include "gameworld/gameworld/config/logicconfigmanager.hpp"
#include "shenqiconfig.hpp"
#include "gameworld/gameworld/obj/character/role.h"
#include "servercommon/errornum.h"
#include "protocal/msgshenqi.hpp"
#include "gameworld/gamecommon.h"
#include "servercommon/string/gameworldstr.h"
#include "globalconfig/globalconfig.h"
#include "protocal/msgscene.h"
#include "servercommon/commondata.hpp"
#include "gameworld/obj/character/role.h"
#include "other/capability/capability.hpp"
#include "other/specialappearance/specialappearance.hpp"
#include "protocal/msgrole.h"
#include "servercommon/string/gameworldstr.h" 
#include "gameworld/world.h"
#include "item/itemextern.hpp"
#include "item/itempool.h"
#include "other/shizhuang/shizhuang.h"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/magicalprecious/magicalpreciouscfg.hpp"

Shenqi::Shenqi() : m_role(nullptr), m_cur_baojia_texiao_id(0)
{
	
}


void Shenqi::InitParam(const ShenqiParam &shenqi_param)
{
	m_param = shenqi_param;
	m_cur_baojia_texiao_id = CheckBaojiaTeXiaoId(m_param.baojia_cur_texiao_id);
}

void Shenqi::GetInitParam(ShenqiParam* shenqi_param)
{
	*shenqi_param = m_param;
	CommonDataParam *common_data = m_role->GetCommonDataParam();
	common_data->shenbing_use_image_id = m_param.shenbing_cur_image_id;
	common_data->baojia_use_image_id = m_param.baojia_cur_image_id;
}

void Shenqi::ReCalcAttr(CharIntAttrs &base_attr, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		CharIntAttrs shenbing_attrs_add; shenbing_attrs_add.Reset();
		CharIntAttrs baojia_attrs_add;	baojia_attrs_add.Reset();

		for (int i = 0; i < SHENQI_SUIT_NUM_MAX; ++i)
		{
			for (int j = SHENQI_PART_TYPE_0; j < SHENQI_PART_TYPE_MAX; ++j)
			{
				//神兵镶嵌属性
				const short shenbing_quality = m_param.shenbing_list[i].quality_list[j];

				const ShenqiInlayCfg* shenbing_inlay_cfg = LOGIC_CONFIG->GetShenqiCfg().GetShenBingInlayCfg(i, j);
				if (shenbing_inlay_cfg != nullptr)
				{
					if (shenbing_quality > SHENQI_QUALITY_TYPE_INVALID && shenbing_quality < SHENQI_QUALITY_TYPE_MAX)
					{
						shenbing_inlay_cfg->attibute_config_list[shenbing_quality].AddAttrs(shenbing_attrs_add);
					}
				}

				//宝甲镶嵌属性
				const short baojia_quality = m_param.baojia_list[i].quality_list[j];

				const ShenqiInlayCfg* baojia_inlay_cfg = LOGIC_CONFIG->GetShenqiCfg().GetBaojiaInlayCfg(i, j);
				if (baojia_inlay_cfg != nullptr)
				{
					if (baojia_quality > SHENQI_QUALITY_TYPE_INVALID && baojia_quality < SHENQI_QUALITY_TYPE_MAX)
					{
						baojia_inlay_cfg->attibute_config_list[baojia_quality].AddAttrs(baojia_attrs_add);
					}
				}
			}

			//神兵升级属性
			const ShenqiUplevelCfg* shenbing_level_cfg = LOGIC_CONFIG->GetShenqiCfg().GetShenBingUplevelCfg(m_param.shenbing_list[i].level);
			if (shenbing_level_cfg != nullptr)
			{
				shenbing_level_cfg->attibute_config.AddAttrs(shenbing_attrs_add);
			}

			//宝甲升级属性
			const ShenqiUplevelCfg* baojia_level_cfg = LOGIC_CONFIG->GetShenqiCfg().GetBaojieUplevelCfg(m_param.baojia_list[i].level);
			if (baojia_level_cfg != nullptr)
			{
				baojia_level_cfg->attibute_config.AddAttrs(baojia_attrs_add);
			}

			//神兵特效激活属性
			const ShenqiActiveTeXiaoCfg *shenbing_texiao_cfg = LOGIC_CONFIG->GetShenqiCfg().GetShenbingActiveTeXiaoCfg(i,SHEN_QI_QUALITY_RED);
			if (nullptr != shenbing_texiao_cfg && (m_param.shenbing_texiao_flag & (LL_ONE_BIT << shenbing_texiao_cfg->seq)))
			{
				shenbing_texiao_cfg->attibute_config.AddAttrs(shenbing_attrs_add);
				shenbing_texiao_cfg->extra_attibute.AddAttrs(shenbing_attrs_add);
			}
			else
			{
				const ShenqiActiveTeXiaoCfg *shenbing_texiao_cfg_o = LOGIC_CONFIG->GetShenqiCfg().GetShenbingActiveTeXiaoCfg(i, SHEN_QI_QUALITY_ORANGE);
				if (nullptr != shenbing_texiao_cfg_o && (m_param.shenbing_texiao_flag & (LL_ONE_BIT << shenbing_texiao_cfg_o->seq)))
				{
					shenbing_texiao_cfg_o->attibute_config.AddAttrs(shenbing_attrs_add);
					shenbing_texiao_cfg_o->extra_attibute.AddAttrs(shenbing_attrs_add);
				}
			}
			//宝甲特效激活属性
			const ShenqiActiveTeXiaoCfg *baojia_texiao_cfg = LOGIC_CONFIG->GetShenqiCfg().GetBaojiaActiveTeXiaoCfg(i, SHEN_QI_QUALITY_RED);
			if (nullptr != baojia_texiao_cfg && (m_param.baojia_texiao_flag & (LL_ONE_BIT << baojia_texiao_cfg->seq)))
			{
				baojia_texiao_cfg->attibute_config.AddAttrs(baojia_attrs_add);
				baojia_texiao_cfg->extra_attibute.AddAttrs(baojia_attrs_add);
			}
			else
			{
				const ShenqiActiveTeXiaoCfg *baojia_texiao_cfg_o = LOGIC_CONFIG->GetShenqiCfg().GetBaojiaActiveTeXiaoCfg(i, SHEN_QI_QUALITY_ORANGE);
				if (nullptr != baojia_texiao_cfg_o && (m_param.baojia_texiao_flag & (LL_ONE_BIT << baojia_texiao_cfg_o->seq)))
				{
					baojia_texiao_cfg_o->attibute_config.AddAttrs(baojia_attrs_add);
					baojia_texiao_cfg_o->extra_attibute.AddAttrs(baojia_attrs_add);
				}
			}
		}

		m_attrs_add.Add(shenbing_attrs_add.m_attrs);
		m_attrs_add.AddPercent(shenbing_attrs_add.m_percent_attrs);
		m_attrs_add.Add(baojia_attrs_add.m_attrs);
		m_attrs_add.AddPercent(baojia_attrs_add.m_percent_attrs);
	}

	base_attr.Add(m_attrs_add.m_attrs, true);
	base_attr.AddPercent(m_attrs_add.m_percent_attrs);

	m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SHENQI, m_attrs_add);

}

void Shenqi::Update(unsigned long interval, time_t now_second)
{
	
}

void Shenqi::OnDie(Role *killer)
{
	/*
	 * 检查神兵套装是否达到配置等级、品质
	 * 触发神兵套装特效效果
	 * 记录击杀者当前套装id
	 */
	if (!killer)
	{
		return;
	}

	const short killer_cur_shenbing_texiao_id = killer->GetShenqi()->GetCurShenbingTeXiaoid();
	const auto &other_cfg = LOGIC_CONFIG->GetShenqiCfg().GetOtherCfg();
	//if (!killer->GetShenqi()->IsActiveQualityShenbingSuit(killer_cur_shenbing_suit_id, other_cfg.shenbing_suit_trigger_quality))
	//{
	//	return;
	//}

	//if (!killer->GetShenqi()->IsAchieveShenbingSuitLevel(killer_cur_shenbing_suit_id, other_cfg.shenbing_suit_trigger_level))
	//{
	//	return;
	//}
	//if (0 == (killer->GetShenqi()->m_param.shenbing_texiao_flag & (LL_ONE_BIT << killer_cur_shenbing_texiao_id)))
	//{
	//	return;
	//}

	// 百分比概率
	if (RandomNum(100) > other_cfg.shenbing_suit_trigger_per)
	{
		return;
	}

    m_param.killer_shenbing_texiao_id = killer_cur_shenbing_texiao_id;
}

void Shenqi::OnRealive()
{
	const auto &other_cfg = LOGIC_CONFIG->GetShenqiCfg().GetOtherCfg();
	unsigned int end_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.shenbing_suit_duration_time;
	// 如果击杀者使用神兵特效id不为0，改变角色形象
	if (m_param.killer_shenbing_texiao_id > 0)
	{
		m_role->GetSpecialAppearance()->SetShenqiAppearance(m_param.killer_shenbing_texiao_id, end_time);
		m_param.killer_shenbing_texiao_id = 0;
	}
}

void Shenqi::GetRoleAppearance(RoleAppearance *appearance)
{
	if (nullptr == appearance) return;

	appearance->shenbing_image_id = m_param.shenbing_cur_image_id;
	appearance->shenbing_texiao_id = m_param.shenbing_cur_texiao_id;
	appearance->baojia_image_id = m_param.baojia_cur_image_id;
	appearance->baojia_texiao_id = m_param.baojia_cur_texiao_id;
}

void Shenqi::OnShenqiOperaReq(Protocol::CSShenqiOperaReq* sqor)
{
	switch (sqor->opera_type)
	{
	case Protocol::SHENQI_OPERA_REQ_TYPE_INFO:
		{
			this->SendShenqiInfo();
		}
		break;

	case Protocol::SHENQI_OPERA_REQ_TYPE_DECOMPOSE:
		{
			this->Decompose(sqor->param_1, sqor->param_2);
		}
		break;

	case Protocol::SHENQI_OPERA_REQ_TYPE_SHENBING_INLAY:
		{
			this->ShenbingInlay(sqor->param_1, sqor->param_2, sqor->param_3);
		}
		break;

	case Protocol::SHENQI_OPERA_REQ_TYPE_SHENBING_UPLEVEL:
		{
			if (sqor->param_2 != 0)
			{
				if (sqor->param_1 < 0 || sqor->param_1 >= SHENQI_SUIT_NUM_MAX) return;

				bool is_continue = true;
				const int old_level = m_param.shenbing_list[sqor->param_1].level;

				for (int i = 0; i < sqor->param_3; ++i)
				{
					bool result = this->ShenbingUplevel(sqor->param_1);
					if (m_param.shenbing_list[sqor->param_1].level != old_level || !result)
					{
						m_role->SendOperateResult(Protocol::SCOperateResult::OP_SHENQI_SHENGBING_UPLEVEL, 0);
						is_continue = false;
						break;
					}
				}

				if (is_continue)
				{
					m_role->SendOperateResult(Protocol::SCOperateResult::OP_SHENQI_SHENGBING_UPLEVEL, 1);
				}
			}
			else
			{
				this->ShenbingUplevel(sqor->param_1);
			}
		}
		break;

	case Protocol::SHENQI_OPERA_REQ_TYPE_SHENBING_USE_IMAGE:
		{
			this->UseShenbingImage(sqor->param_1);
		}
		break;

	case Protocol::SHENQI_OPERA_REQ_TYPE_SHENBING_USE_TEXIAO:
		{
			this->UseShenbingTexiao(sqor->param_1);
		}
		break;

	case Protocol::SHENQI_OPERA_REQ_TYPE_BAOJIA_INLAY:
		{
			this->BaojiaInlay(sqor->param_1, sqor->param_2, sqor->param_3);
		}
		break;

	case Protocol::SHENQI_OPERA_REQ_TYPE_BAOJIA_UPLEVEL:
		{
			if (sqor->param_2 != 0)
			{
				if (sqor->param_1 < 0 || sqor->param_1 >= SHENQI_SUIT_NUM_MAX) return;

				bool is_continue = true;
				const int old_level = m_param.baojia_list[sqor->param_1].level;
					
				for (int i = 0; i < sqor->param_3; ++i)
				{
					bool result = this->BaojiaUplevel(sqor->param_1);
					if (m_param.baojia_list[sqor->param_1].level != old_level || !result)
					{
						m_role->SendOperateResult(Protocol::SCOperateResult::OP_SHENQI_BAOJIA_UPLEVEL, 0);
						is_continue = false;
						break;
					}
				}

				if (is_continue)
				{
					m_role->SendOperateResult(Protocol::SCOperateResult::OP_SHENQI_BAOJIA_UPLEVEL, 1);
				}
			}
			else
			{
				this->BaojiaUplevel(sqor->param_1);
			}
		}
		break;

	case Protocol::SHENQI_OPERA_REQ_TYPE_BAOJIA_USE_IMAGE:
		{
			this->UseBaojiaImage(sqor->param_1);
		}
		break;

	case Protocol::SHENQI_OPERA_REQ_TYPE_BAOJIA_USE_TEXIAO:
		{
			this->UseBaojiaTexiao(sqor->param_1);
		}
		break;
	case Protocol::SHENQI_OPERA_REQ_TYPE_SHENBING_TEXIAO_ACTIVE:
		{
			if(this->IsActiveShenbingTeXiao(sqor->param_1,SHEN_QI_QUALITY_ORANGE))
			{
				this->ActiveShenbingTeXiao(sqor->param_1, SHEN_QI_QUALITY_RED);
			}
			else
			{
				this->ActiveShenbingTeXiao(sqor->param_1, SHEN_QI_QUALITY_ORANGE);
			}
		}
		break;
	case Protocol::SHENQI_OPERA_REQ_TYPE_BAOJIA_TEXIAO_ACTIVE:
		{
			if (this->IsActiveBaojiaTeXiao(sqor->param_1, SHEN_QI_QUALITY_ORANGE))
			{
				this->ActiveBaojiaTeXiao(sqor->param_1, SHEN_QI_QUALITY_RED);
			}
			else
			{
				this->ActiveBaojiaTeXiao(sqor->param_1, SHEN_QI_QUALITY_ORANGE);
			}
		}
		break;
	case Protocol::SHENQI_OPERA_REQ_TYPE_BAOJIA_OPEN_TEXIAO:
		{
			m_param.baojia_texiao_open_flag = static_cast<char>(sqor->param_1);			
			this->SendImageInfo(Protocol::SHENQI_SC_INFO_TYPE_BAOJIA);
		}
		break;
	}
}

void Shenqi::Decompose(int item_id, int num)
{
	const ShenqiDecomposeCfg* decompose_cfg = LOGIC_CONFIG->GetShenqiCfg().GetDecomposeCfg(item_id);
	if (nullptr == decompose_cfg)
	{
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (m_role->GetKnapsack()->Count(item_id) < num)
	{
		m_role->NoticeNum(errornum::EN_ITEM_NUM_NOT_ENOUGH);
		return;
	}

	int need_grid = 0;
	int get_item_count = 0;
	ItemConfigData get_item_list[MAX_ATTACHMENT_ITEM_NUM];

	for (int i = 0; i < decompose_cfg->get_item_count && i < MAX_ATTACHMENT_ITEM_NUM; ++i)
	{
		const ItemBase* item_base = ITEMPOOL->GetItem(decompose_cfg->get_item_list[i].item_id);
		if (nullptr == item_base) continue;

		get_item_list[i].item_id = decompose_cfg->get_item_list[i].item_id;
		get_item_list[i].num = decompose_cfg->get_item_list[i].num * num;
		get_item_list[i].is_bind = decompose_cfg->get_item_list[i].is_bind;
		++get_item_count;
		
		need_grid += get_item_list[i].num / item_base->GetPileLimit();
		if (get_item_list[i].num % item_base->GetPileLimit() != 0)
		{
			++need_grid;
		}
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(need_grid))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!m_role->GetKnapsack()->ConsumeItem(decompose_cfg->item_id, num, "Shenqi::Decompose"))
	{
		return;
	}

	for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
	{
		const ItemBase* item_base = ITEMPOOL->GetItem(get_item_list[i].item_id);
		if (nullptr == item_base) continue;

		int total_num = get_item_list[i].num;
		int loop_times = total_num / item_base->GetPileLimit();
		if (total_num % item_base->GetPileLimit() != 0)
		{
			++loop_times;
		}

		for (int j = 0; j < loop_times && j < 100; ++j)
		{
			int put_num = total_num > item_base->GetPileLimit() ? item_base->GetPileLimit() : total_num;
			m_role->GetKnapsack()->Put(ItemConfigData(get_item_list[i].item_id, get_item_list[i].is_bind, put_num), PUT_REASON_SHENQI_DECOMPOSE);
	
			total_num -= put_num;
			if (total_num <= 0)
			{
				break;
			}
		}
	}

	Protocol::SCShenqiDecomposeResult sqdr;

	sqdr.item_count = 0;
	for (; sqdr.item_count < get_item_count && sqdr.item_count < MAX_ATTACHMENT_ITEM_NUM; ++sqdr.item_count)
	{
		sqdr.item_list[sqdr.item_count].item_id = get_item_list[sqdr.item_count].item_id;
		sqdr.item_list[sqdr.item_count].num = get_item_list[sqdr.item_count].num;
		sqdr.item_list[sqdr.item_count].is_bind = get_item_list[sqdr.item_count].is_bind ? 1 : 0;
		sqdr.item_list[sqdr.item_count].reserve_ch = 0;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&sqdr, sizeof(sqdr));

	std::string log_str;
	ItemConfigDataListLog(log_str, get_item_count, get_item_list);
	gamelog::g_log_shenqi.printf(LL_INFO, "Shenqi::Decompose user[%d %s %d] source_item[item_id:%d num:%d] got_item[%s]", 
		m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), item_id, 1, log_str.c_str());
}

void Shenqi::ShenbingInlay(int id, int part_type, int quality)
{
	if (id <= 0 || id >= SHENQI_SUIT_NUM_MAX || part_type < SHENQI_PART_TYPE_0 || part_type >= SHENQI_PART_TYPE_MAX)
	{
		return;
	}

	const ShenqiInlayCfg* inlay_cfg = LOGIC_CONFIG->GetShenqiCfg().GetShenBingInlayCfg(id, part_type);
	if (nullptr == inlay_cfg)
	{
		return;
	}

	if (quality <= SHENQI_QUALITY_TYPE_INVALID || quality >= SHENQI_QUALITY_TYPE_MAX)
	{
		return;
	}

	if (quality < m_param.shenbing_list[id].quality_list[part_type])
	{
		m_role->NoticeNum(errornum::EN_SHENQI_MAX_QUALITY_LIMIT);
		return;
	}

	//检查返回物品id
	int give_back_stuff_id = 0;
	int old_quality = m_param.shenbing_list[id].quality_list[part_type];

	if (old_quality > SHENQI_QUALITY_TYPE_INVALID && old_quality < SHENQI_QUALITY_TYPE_MAX)
	{
		give_back_stuff_id = inlay_cfg->inlay_stuff_list[old_quality];

		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}
	}

	int need_stuff_id = inlay_cfg->inlay_stuff_list[quality];

	if (!m_role->GetKnapsack()->ConsumeItem(need_stuff_id, 1, "Shenqi::ShenbingInlay"))
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return;
	}

	//返回物品
	if (give_back_stuff_id != 0)
	{
		ItemConfigData give_back_item;
		give_back_item.item_id = give_back_stuff_id;
		give_back_item.num = 1;
		give_back_item.is_bind = 1;

		m_role->GetKnapsack()->Put(give_back_item, PUT_REASON_SHENQI_INLAY_GIVE_BACK);
	}

	m_param.shenbing_list[id].quality_list[part_type] = quality;

	//检查激活形象
	const ShenqiImageCfg *image_cfg = LOGIC_CONFIG->GetShenqiCfg().GetShenbingImageCfg(id);
	if (image_cfg != nullptr)
	{
		if (0 == (m_param.shenbing_image_flag & (LL_ONE_BIT << image_cfg->active_image_id)))
		{
			char min_quality = CHAR_MAX;

			for (int i = SHENQI_PART_TYPE_0; i < SHENQI_PART_TYPE_MAX; ++i)
			{
				if (m_param.shenbing_list[id].quality_list[i] < min_quality)
				{
					min_quality = m_param.shenbing_list[id].quality_list[i];
				}
			}

			if (min_quality >= image_cfg->need_quality)
			{
				m_param.shenbing_image_flag |= LL_ONE_BIT << image_cfg->active_image_id;

				this->SendImageInfo(Protocol::SHENQI_SC_INFO_TYPE_SHENBING);

				int send_length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenqi_special_img_active,
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), Protocol::SHENQI_SC_INFO_TYPE_SHENBING, id);
				if (send_length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, send_length, SYS_MSG_CENTER_NOTICE_3);
				}

				/// 神器对宝石加成调用重算
				//m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_STONE);
			}
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENQI);

	this->SendSingleShenqiInfo(Protocol::SHENQI_SC_INFO_TYPE_SHENBING, id);
	m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_SHENQI_INLAY_COUNT);	//嘉年华
	ItemConfigData item_log(give_back_stuff_id, 1, 1);
	gamelog::g_log_shenqi.printf(LL_INFO, "Shenqi::ShenbingInlay user[%d %s %d] consume_item[item_id:%d num:%d] give_back[%s], active_image_flag[%lld]",
		m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), need_stuff_id, 1, ItemConfigDataLog(&item_log, nullptr), m_param.shenbing_image_flag);
}

bool Shenqi::ShenbingUplevel(int id)
{
	if (id <= 0 || id >= SHENQI_SUIT_NUM_MAX)
	{
		return false;
	}

	//if (0 == (m_param.shenbing_image_flag & (LL_ONE_BIT << id)))
	//{
	//	m_role->NoticeNum(errornum::EN_SHENQI_SHENBING_NOT_ACTIVE);
	//	return false;
	//}

	if (!CanShenbingUplevel(id))
	{
		m_role->NoticeNum(errornum::EN_SHENQI_NOT_SUIT);
		return false;
	}

	const ShenqiUplevelCfg* uplevel_cfg = LOGIC_CONFIG->GetShenqiCfg().GetShenBingUplevelCfg(m_param.shenbing_list[id].level + 1);
	if (nullptr == uplevel_cfg)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_FUNC_CONFIG_ERROR);
		return false;
	}

	const ShenqiOtherCfg& other_cfg = LOGIC_CONFIG->GetShenqiCfg().GetOtherCfg();

	{
		if (m_role->GetKnapsack()->IsLock())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
			return false;
		}


		//检查消耗
		ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

		int consume_count = 0;
		ItemExtern::ItemConsumeConfig item_consume_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

		item_consume_list[consume_count].item_id = other_cfg.shenbing_uplevel_stuff_id;
		item_consume_list[consume_count].num = 1;
		item_consume_list[consume_count].buyable = false;
		++consume_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, consume_count, item_consume_list, &consume_list))
		{
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "Shenqi::ShenbingUplevel"))
		{
			return false;
		}
	}

	const int addexp = LOGIC_CONFIG->GetShenqiCfg().GetUplevelRandAddexp();
	
	const int old_level = m_param.shenbing_list[id].level;
	const int old_exp = m_param.shenbing_list[id].exp;
	m_param.shenbing_list[id].exp += addexp;
	if (m_param.shenbing_list[id].exp >= uplevel_cfg->need_exp)
	{
		++m_param.shenbing_list[id].level;
		m_param.shenbing_list[id].exp -= uplevel_cfg->need_exp;

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENQI);

		// 神器对宝石加成调用重算
		//const ShenqiAddStonePerCfg *stone_add_per_cfg = GLOBALCONFIG->GetEquipConfig().GetShenqiAddStonePerCfgByAddType(StoneParam::ADD_TYPE_QILING);
		//if (stone_add_per_cfg)
		//{
		//	if (stone_add_per_cfg->condition == m_param.shenbing_list[id].level)
		//	{
		//		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_STONE);
		//	}
		//}
	}

	//if (uplevel_cfg->active_texiao_id > 0)
	//{
	//	m_param.shenbing_texiao_flag |= (LL_ONE_BIT << uplevel_cfg->active_texiao_id);
	//	this->SendImageInfo(Protocol::SHENQI_SC_INFO_TYPE_SHENBING);
	//}

	this->SendSingleShenqiInfo(Protocol::SHENQI_SC_INFO_TYPE_SHENBING, id);

	gamelog::g_log_shenqi.printf(LL_INFO, "Shenqi::ShenbingUplevel user[%d %s %d] consume_item[item_id:%d num:%d] old_status[level:%d exp:%d]\
		new_status[level:%d exp:%d]", m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), other_cfg.shenbing_uplevel_stuff_id, 1,
		old_level, old_exp, m_param.shenbing_list[id].level, m_param.shenbing_list[id].exp);

	return true;
}

void Shenqi::BaojiaInlay(int id, int part_type, int quality)
{
	if (id <= 0 || id >= SHENQI_SUIT_NUM_MAX || part_type < SHENQI_PART_TYPE_0 || part_type >= SHENQI_PART_TYPE_MAX)
	{
		return;
	}

	const ShenqiInlayCfg* inlay_cfg = LOGIC_CONFIG->GetShenqiCfg().GetBaojiaInlayCfg(id, part_type);
	if (nullptr == inlay_cfg)
	{
		return;
	}

	if (quality <= SHENQI_QUALITY_TYPE_INVALID || quality >= SHENQI_QUALITY_TYPE_MAX)
	{
		return;
	}

	if (quality < m_param.baojia_list[id].quality_list[part_type])
	{
		m_role->NoticeNum(errornum::EN_SHENQI_MAX_QUALITY_LIMIT);
		return;
	}

	int give_back_stuff_id = 0;
	int old_quality = m_param.baojia_list[id].quality_list[part_type];

	if (old_quality > SHENQI_QUALITY_TYPE_INVALID && old_quality < SHENQI_QUALITY_TYPE_MAX)
	{
		give_back_stuff_id = inlay_cfg->inlay_stuff_list[old_quality];

		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}
	}

	int need_stuff_id = inlay_cfg->inlay_stuff_list[quality];

	if (!m_role->GetKnapsack()->ConsumeItem(need_stuff_id, 1, "Shenqi::BaojiaInlay"))
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return;
	}

	//返回物品
	if (give_back_stuff_id != 0)
	{
		ItemConfigData give_back_item;
		give_back_item.item_id = give_back_stuff_id;
		give_back_item.num = 1;
		give_back_item.is_bind = 1;

		m_role->GetKnapsack()->Put(give_back_item, PUT_REASON_SHENQI_INLAY_GIVE_BACK);
	}

	m_param.baojia_list[id].quality_list[part_type] = quality;

	//检查激活形象
	const ShenqiImageCfg *image_cfg = LOGIC_CONFIG->GetShenqiCfg().GetBaojiaImageCfg(id);
	if (image_cfg != nullptr)
	{
		if (0 == (m_param.baojia_image_flag & (LL_ONE_BIT << image_cfg->active_image_id)))
		{
			char min_quality = CHAR_MAX;

			for (int i = SHENQI_PART_TYPE_0; i < SHENQI_PART_TYPE_MAX; ++i)
			{
				if (m_param.baojia_list[id].quality_list[i] < min_quality)
				{
					min_quality = m_param.baojia_list[id].quality_list[i];
				}
			}

			if (min_quality >= image_cfg->need_quality)
			{
				m_param.baojia_image_flag |= LL_ONE_BIT << image_cfg->active_image_id;

				this->SendImageInfo(Protocol::SHENQI_SC_INFO_TYPE_BAOJIA);

				int send_length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_shenqi_special_img_active,
					m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), Protocol::SHENQI_SC_INFO_TYPE_BAOJIA, id);
				if (send_length > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, send_length, SYS_MSG_CENTER_NOTICE_3);
				}

				// 神器对宝石加成调用重算
				//m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_STONE);
			}
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENQI);
	//m_cur_baojia_special_texiao_id = GetBaojiaSpecialImageId(m_param.baojia_cur_texiao_id);

	this->SendSingleShenqiInfo(Protocol::SHENQI_SC_INFO_TYPE_BAOJIA, id);
	m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_SHENQI_INLAY_COUNT);	//嘉年华
	ItemConfigData item_log(give_back_stuff_id, 1, 1);
	gamelog::g_log_shenqi.printf(LL_INFO, "Shenqi::BaojiaInlay user[%d %s %d] consume_item[item_id:%d num:%d] give_back[%s], active_image_flag[%lld]",
		m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), need_stuff_id, 1, ItemConfigDataLog(&item_log, nullptr), m_param.baojia_image_flag);
}

bool Shenqi::BaojiaUplevel(int id)
{
	if (id <= 0 || id >= SHENQI_SUIT_NUM_MAX)
	{
		return false;
	}

	//if (0 == (m_param.baojia_image_flag & (LL_ONE_BIT << id)))
	//{
	//	m_role->NoticeNum(errornum::EN_SHENQI_BAOJIA_NOT_ACTIVE);
	//	return false;
	//}

	if (!CanBaojiaUplevel(id))
	{
		m_role->NoticeNum(errornum::EN_SHENQI_NOT_SUIT);
		return false;
	}

	const ShenqiUplevelCfg* uplevel_cfg = LOGIC_CONFIG->GetShenqiCfg().GetBaojieUplevelCfg(m_param.baojia_list[id].level + 1);
	if (nullptr == uplevel_cfg)
	{
		m_role->NoticeNum(errornum::EN_EQUIP_LEVEL_MAX);
		return false;
	}

	const ShenqiOtherCfg& other_cfg = LOGIC_CONFIG->GetShenqiCfg().GetOtherCfg();

	{
		if (m_role->GetKnapsack()->IsLock())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
			return false;
		}

		//检查消耗
		ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

		int consume_count = 0;
		ItemExtern::ItemConsumeConfig item_consume_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

		item_consume_list[consume_count].item_id = other_cfg.baojia_uplevel_stuff_id;
		item_consume_list[consume_count].num = 1;
		item_consume_list[consume_count].buyable = false;
		++consume_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, consume_count, item_consume_list, &consume_list))
		{
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "Shenqi::BaojiaUplevel"))
		{
			return false;
		}
	}

	const int addexp = LOGIC_CONFIG->GetShenqiCfg().GetUplevelRandAddexp();

	const int old_level = m_param.baojia_list[id].level;
	const int old_exp = m_param.baojia_list[id].exp;
	m_param.baojia_list[id].exp += addexp;
	if (m_param.baojia_list[id].exp >= uplevel_cfg->need_exp)
	{
		++m_param.baojia_list[id].level;
		m_param.baojia_list[id].exp -= uplevel_cfg->need_exp;

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENQI);

		// 神器对宝石加成调用重算
		//const ShenqiAddStonePerCfg *stone_add_per_cfg = GLOBALCONFIG->GetEquipConfig().GetShenqiAddStonePerCfgByAddType(StoneParam::ADD_TYPE_QILING);
		//if (stone_add_per_cfg)
		//{
		//	if (stone_add_per_cfg->condition == m_param.shenbing_list[id].level)
		//	{
		//		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_STONE);
		//	}
		//}
	}

	//if (uplevel_cfg->active_texiao_id > 0)
	//{
	//	m_param.baojia_texiao_flag |= (LL_ONE_BIT << uplevel_cfg->active_texiao_id);
	//	this->SendImageInfo(Protocol::SHENQI_SC_INFO_TYPE_SHENBING);
	//}

	//m_cur_baojia_special_texiao_id = GetBaojiaSpecialImageId(m_param.baojia_cur_image_id);

	this->SendSingleShenqiInfo(Protocol::SHENQI_SC_INFO_TYPE_BAOJIA, id);

	gamelog::g_log_shenqi.printf(LL_INFO, "Shenqi::BaojiaUplevel user[%d %s %d] consume_item[item_id:%d num:%d] old_status[level:%d exp:%d] new_status[level:%d exp:%d], active_texiao_flag[%lld]",
		m_role->GetUID(), m_role->GetName(), m_role->GetCamp(), other_cfg.baojia_uplevel_stuff_id, 1, old_level, old_exp, m_param.baojia_list[id].level,
		m_param.baojia_list[id].exp, m_param.baojia_texiao_flag);

	return true;
}

void Shenqi::UseShenbingImage(const int image_id)
{
	if (image_id < 0 || image_id >= SHENQI_SUIT_NUM_MAX) return;

	if (image_id != 0)
	{
		if (0 == (m_param.shenbing_image_flag & (LL_ONE_BIT << image_id)))
		{
			m_role->NoticeNum(errornum::EN_JINGLING_IMG_NOT_ACTIVE);
			return;
		}
	}

	if (image_id != 0)
	{
		m_role->GetShizhuang()->UnUseSpecialImg(SHIZHUANG_TYPE_WUQI,0);
	}

	m_param.shenbing_cur_image_id = image_id;
	//m_cur_baojia_special_texiao_id = GetBaojiaSpecialImageId(m_param.baojia_cur_texiao_id);

	this->SendImageInfo(Protocol::SHENQI_SC_INFO_TYPE_SHENBING);
	
	m_role->RefreshApperance(true, APPEARANCE_WUQI_USE_TYPE_SHENQI);

	{
		//const char *notice_key = image_id != 0 ? "shenbing_use" : "shenbing_unuse";
		//gstr::SendNotice(m_role, notice_key);
	}
}

void Shenqi::UseShenbingTexiao(const int texiao_id)
{
	if (texiao_id < 0 || texiao_id >= SHENQI_SUIT_NUM_MAX) return;

	if (texiao_id != 0)
	{
		if (0 == (m_param.shenbing_texiao_flag & (LL_ONE_BIT << texiao_id)))
		{
			m_role->NoticeNum(errornum::EN_JINGLING_IMG_NOT_ACTIVE);
			return;
		}
	}

	m_param.shenbing_cur_texiao_id = texiao_id;

	this->SendImageInfo(Protocol::SHENQI_SC_INFO_TYPE_SHENBING);

	m_role->RefreshApperance(true);
}

bool Shenqi::IsShenbingImageActive(const int image_id)
{
	if (image_id < 0 || image_id >= SHENQI_SUIT_NUM_MAX) return false;

	if (0 != (m_param.shenbing_image_flag & (LL_ONE_BIT << image_id)))
	{
		return true;
	}

	return false;
}

bool Shenqi::IsBaojiaImageActive(const int image_id)
{
	if (image_id < 0 || image_id >= SHENQI_SUIT_NUM_MAX) return false;

	if (0 != (m_param.baojia_image_flag & (LL_ONE_BIT << image_id)))
	{
		return true;
	}

	return false;
}

bool Shenqi::IsActiveQualityShenbingSuit(int id, int quality)
{
	if (id < 0 || id >= SHENQI_SUIT_NUM_MAX) return false;
	if (quality <= SHENQI_QUALITY_TYPE_INVALID || quality >= SHENQI_QUALITY_TYPE_MAX) return false;
	for (int part = SHENQI_PART_TYPE_0; part < SHENQI_PART_TYPE_MAX; ++part)
	{
		if (m_param.shenbing_list[id].quality_list[part] < quality)
		{
			return false;
		}
	}
	return true;
}

bool Shenqi::IsActiveQualityBaojiaSuit(int id, int quality)
{
	if (id < 0 || id >= SHENQI_SUIT_NUM_MAX) return false;
	if (quality <= SHENQI_QUALITY_TYPE_INVALID || quality >= SHENQI_QUALITY_TYPE_MAX) return false;
	for (int part = SHENQI_PART_TYPE_0; part < SHENQI_PART_TYPE_MAX; ++part)
	{
		if (m_param.baojia_list[id].quality_list[part] < quality)
		{
			return false;
		}
	}
	return true;
}

int Shenqi::GetActiveQualityShenbingSuitCount(int quality)
{
	if (quality <= SHENQI_QUALITY_TYPE_INVALID || quality >= SHENQI_QUALITY_TYPE_MAX)
	{
		return 0;
	}
	int count = 0;
	for (int i = 0; i < SHENQI_SUIT_NUM_MAX; ++i)
	{
		for (int part = SHENQI_PART_TYPE_0; part < SHENQI_PART_TYPE_MAX; ++part)
		{
			if (m_param.shenbing_list[i].quality_list[part] >= quality)
			{
				++count;
			}
		}
	}
	return count;
}

int Shenqi::GetActiveQualityBaojiaSuitCount(int quality)
{
	if (quality <= SHENQI_QUALITY_TYPE_INVALID || quality >= SHENQI_QUALITY_TYPE_MAX)
	{
		return 0;
	}
	int count = 0;
	for (int i = 0; i < SHENQI_SUIT_NUM_MAX; ++i)
	{
		for (int part = SHENQI_PART_TYPE_0; part < SHENQI_PART_TYPE_MAX; ++part)
		{
			if (m_param.baojia_list[i].quality_list[part] >= quality)
			{
				++count;
			}
		}
	}
	return count;
}

int Shenqi::GetActiveQualityAllSuitCount(int quality)
{
	return this->GetActiveQualityBaojiaSuitCount(quality) + this->GetActiveQualityShenbingSuitCount(quality);
}

bool Shenqi::IsAchieveShenbingSuitLevel(int id, int level)
{
	if (id < 0 || id >= SHENQI_SUIT_NUM_MAX) return false;
	if (m_param.shenbing_list[id].level != level)
	{
		return false;
	}

	return true;
}

bool Shenqi::IsAchieveBaojiaSuitLevel(int id, int level)
{
	if (id < 0 || id >= SHENQI_SUIT_NUM_MAX) return false;
	if (m_param.baojia_list[id].level != level)
	{
		return false;
	}

	return true;
}

short Shenqi::CheckBaojiaTeXiaoId(short cur_baojia_id)
{
	/*
	 * 条件1：套装激活
	 * 条件2：套装达到某个品质
	 * 条件3：套装达到某个等级
	 */
	const auto& other_cfg = LOGIC_CONFIG->GetShenqiCfg().GetOtherCfg();
	if (!CanBaojiaUplevel(cur_baojia_id))
	{
		return 0;
	}
	if(!IsActiveQualityBaojiaSuit(cur_baojia_id, other_cfg.baojia_suit_trigger_quality))
	{
		return 0;
	}
	//if (!IsAchieveBaojiaSuitLevel(cur_baojia_id, other_cfg.baojia_suit_trigger_level))
	//{
	//	return 0;
	//}
	return cur_baojia_id;
}

const int Shenqi::GetShenbingLevel(int index)
{
	if (index >= 0 && index < SHENQI_SUIT_NUM_MAX)
	{
		return m_param.shenbing_list[index].level;
	}

	return 0;
}

const int Shenqi::GetBaojiaLevel(int index)
{
	if (index >= 0 && index < SHENQI_SUIT_NUM_MAX)
	{
		return m_param.baojia_list[index].level;
	}

	return 0;
}

const char Shenqi::GetShenbingCurImageId()
{
	return m_param.shenbing_cur_image_id;
}

const char Shenqi::GetBaojiaCurImageId()
{
	return m_param.shenbing_cur_image_id;
}


void Shenqi::SetKillerTeXiaoeId(short texiao_id)
{
	m_param.killer_shenbing_texiao_id = texiao_id;
}

void Shenqi::UseBaojiaImage(const int image_id)
{
	if (image_id < 0 || image_id >= SHENQI_SUIT_NUM_MAX) return;

	if (image_id != 0)
	{
		if (0 == (m_param.baojia_image_flag & (LL_ONE_BIT << image_id)))
		{
			m_role->NoticeNum(errornum::EN_JINGLING_IMG_NOT_ACTIVE);
			return;
		}
	}

	if (image_id != 0)
	{
		m_role->GetShizhuang()->UnUseSpecialImg(SHIZHUANG_TYPE_BODY,0);
	}

	m_param.baojia_cur_image_id = image_id;
	//m_cur_baojia_special_texiao_id = GetBaojiaSpecialImageId(m_param.baojia_cur_image_id);

	this->SendImageInfo(Protocol::SHENQI_SC_INFO_TYPE_BAOJIA);
	
	m_role->RefreshApperance(true, APPEARANCE_WUQI_USE_TYPE_INVALID, APPEARANCE_BODY_USE_TYPE_SHENQI);
}

void Shenqi::UseBaojiaTexiao(const int texiao_id)
{
	if (texiao_id < 0 || texiao_id >= SHENQI_SUIT_NUM_MAX) return;

	if (texiao_id != 0)
	{
		if (0 == (m_param.baojia_texiao_flag & (LL_ONE_BIT << texiao_id)))
		{
			m_role->NoticeNum(errornum::EN_JINGLING_IMG_NOT_ACTIVE);
			return;
		}
	}

	m_param.baojia_cur_texiao_id = texiao_id;
	m_cur_baojia_texiao_id = texiao_id;

	this->SendImageInfo(Protocol::SHENQI_SC_INFO_TYPE_BAOJIA);
	m_role->RefreshApperance(true);
}

void Shenqi::SendShenqiInfo()
{
	static Protocol::SCShenqiAllInfo sqai;

	sqai.shenbing_image_flag = m_param.shenbing_image_flag;
	sqai.shenbing_texiao_flag = m_param.shenbing_texiao_flag;
	sqai.baojia_image_flag = m_param.baojia_image_flag;
	sqai.baojia_texiao_flag = m_param.baojia_texiao_flag;
	sqai.shenbing_cur_image_id = m_param.shenbing_cur_image_id;
	sqai.shenbing_cur_texiao_id = m_param.shenbing_cur_texiao_id;
	sqai.baojia_cur_image_id = m_param.baojia_cur_image_id;
	sqai.baojia_cur_texiao_id = m_param.baojia_cur_texiao_id;

	UNSTD_STATIC_CHECK(sizeof(sqai.shenbing_list) == sizeof(m_param.shenbing_list));
	memcpy(sqai.shenbing_list, m_param.shenbing_list, sizeof(sqai.shenbing_list));

	UNSTD_STATIC_CHECK(sizeof(sqai.baojia_list) == sizeof(m_param.baojia_list));
	memcpy(sqai.baojia_list, m_param.baojia_list, sizeof(sqai.baojia_list));
	sqai.baojia_texiao_open_flag = m_param.baojia_texiao_open_flag;
	sqai.reserve_ch = 0;
	sqai.reserve_sh = 0;

	//SEND_TO_ROLE(m_role, sqai);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&(sqai), sizeof(sqai));
}

void Shenqi::SendSingleShenqiInfo(const short type, const short index)
{
	if (index <= 0 || index >= SHENQI_SUIT_NUM_MAX) return;

	static Protocol::SCShenqiSingleInfo sqsi;

	sqsi.info_type = type;
	sqsi.item_index = index;

	UNSTD_STATIC_CHECK(sizeof(Protocol::SCShenqiSingleInfo::shenqi_item) == sizeof(ShenqiItemParam));
	if (Protocol::SHENQI_SC_INFO_TYPE_SHENBING == type)
	{
		memcpy(&sqsi.shenqi_item, &m_param.shenbing_list[index], sizeof(sqsi.shenqi_item));
	}
	else if (Protocol::SHENQI_SC_INFO_TYPE_BAOJIA == type)
	{
		memcpy(&sqsi.shenqi_item, &m_param.baojia_list[index], sizeof(sqsi.shenqi_item));
	}
	else
	{
		return;
	}

	//SEND_TO_ROLE(m_role, sqsi);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&(sqsi), sizeof(sqsi));
}

void Shenqi::SendImageInfo(const short type)
{
	static Protocol::SCShenqiImageInfo sqii;

	sqii.info_type = type;
	if (Protocol::SHENQI_SC_INFO_TYPE_SHENBING == type)
	{
		sqii.cur_use_imgage_id = m_param.shenbing_cur_image_id;
		sqii.cur_use_texiao_id = m_param.shenbing_cur_texiao_id;
		sqii.image_active_flag = m_param.shenbing_image_flag;
		sqii.texiao_active_flag = m_param.shenbing_texiao_flag;
		sqii.texiao_open_flag = 0;
		sqii.reserve_ch = 0;
		sqii.reserve_sh = 0;
	}
	else if (Protocol::SHENQI_SC_INFO_TYPE_BAOJIA == type)
	{
		sqii.cur_use_imgage_id = m_param.baojia_cur_image_id;
		sqii.cur_use_texiao_id = m_param.baojia_cur_texiao_id;
		sqii.image_active_flag = m_param.baojia_image_flag;
		sqii.texiao_active_flag = m_param.baojia_texiao_flag;
		sqii.texiao_open_flag = m_param.baojia_texiao_open_flag;
		sqii.reserve_ch = 0;
		sqii.reserve_sh = 0;
	}
	else
	{
		return;
	}
	
	//SEND_TO_ROLE(m_role, sqii);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&(sqii), sizeof(sqii));
}

bool Shenqi::CanShenbingUplevel(int id)
{
	if (id <= 0 || id >= SHENQI_SUIT_NUM_MAX)
	{
		return false;
	}

	for (int part = SHENQI_PART_TYPE_0; part < SHENQI_PART_TYPE_MAX; part++)
	{
		if (m_param.shenbing_list[id].quality_list[part] == SHENQI_QUALITY_TYPE_INVALID)
		{
			return false;
		}
	}
	
	return true;
}

bool Shenqi::CanBaojiaUplevel(int id)
{
	if (id <= 0 || id >= SHENQI_SUIT_NUM_MAX)
	{
		return false;
	}

	for (int part = SHENQI_PART_TYPE_0; part < SHENQI_PART_TYPE_MAX; part++)
	{
		if (m_param.baojia_list[id].quality_list[part] == SHENQI_QUALITY_TYPE_INVALID)
		{
			return false;
		}
	}

	return true;
}

bool Shenqi::ActiveShenbingTeXiao(int shenqi_id, int quality)
{
	if (shenqi_id < 0 || shenqi_id >= SHENQI_SUIT_NUM_MAX)
	{
		return false;
	}

	const ShenqiActiveTeXiaoCfg *texiao_cfg = LOGIC_CONFIG->GetShenqiCfg().GetShenbingActiveTeXiaoCfg(shenqi_id,quality);
	if (nullptr == texiao_cfg)
	{
		return false;
	}

	//const ShenqiOtherCfg &other_cfg = LOGIC_CONFIG->GetShenqiCfg().GetOtherCfg();

	if (!IsActiveQualityShenbingSuit(shenqi_id, texiao_cfg->trigger_quality))
	{
		m_role->NoticeNum(errornum::EN_SHENQI_NOT_ACHIEVE_MAX_QUALITY);
		return false;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (0 != (m_param.shenbing_texiao_flag & (LL_ONE_BIT << texiao_cfg->seq)))
	{
		return false;
	}

	if (!m_role->GetKnapsack()->ConsumeItem(texiao_cfg->active_texiao_stuff_id, texiao_cfg->active_texiao_stuff_count, "ActiveShenbingTeXiao"))
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return false;
	}

	if (texiao_cfg->shenqi_id > 0)
	{
		m_param.shenbing_texiao_flag |= (LL_ONE_BIT << texiao_cfg->seq);
		//m_param.shenbing_cur_texiao_id = texiao_cfg->shenqi_id;
		//this->SendImageInfo(Protocol::SHENQI_SC_INFO_TYPE_SHENBING);
		this->UseShenbingTexiao(texiao_cfg->shenqi_id);
	}

	this->SendSingleShenqiInfo(Protocol::SHENQI_SC_INFO_TYPE_SHENBING, shenqi_id);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENQI);

	gamelog::g_log_shenqi.printf(LL_INFO, "Shenqi::ShenbingTeXiao user[%d %s] consume_item[item_id:%d num:%d] active_texiao_flag[%lld]", 
		m_role->GetUID(), m_role->GetName(), texiao_cfg->active_texiao_stuff_id, texiao_cfg->active_texiao_stuff_count, m_param.shenbing_texiao_flag);

	return true;
}

bool Shenqi::ActiveBaojiaTeXiao(int shenqi_id, int quality)
{
	if (shenqi_id < 0 || shenqi_id >= SHENQI_SUIT_NUM_MAX)
	{
		return false;
	}

	const ShenqiActiveTeXiaoCfg *texiao_cfg = LOGIC_CONFIG->GetShenqiCfg().GetBaojiaActiveTeXiaoCfg(shenqi_id,quality);
	if (nullptr == texiao_cfg)
	{
		return false;
	}
	//const ShenqiOtherCfg &other_cfg = LOGIC_CONFIG->GetShenqiCfg().GetOtherCfg();

	if (!IsActiveQualityBaojiaSuit(shenqi_id, texiao_cfg->trigger_quality))
	{
		m_role->NoticeNum(errornum::EN_SHENQI_NOT_ACHIEVE_MAX_QUALITY);
		return false;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	if (0 != (m_param.baojia_texiao_flag & (LL_ONE_BIT << texiao_cfg->seq)))
	{
		return false;
	}

	if (!m_role->GetKnapsack()->ConsumeItem(texiao_cfg->active_texiao_stuff_id, texiao_cfg->active_texiao_stuff_count, "ActiveBaojiaTeXiao"))
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return false;
	}

	if (texiao_cfg->shenqi_id > 0)
	{
		m_param.baojia_texiao_flag |= (LL_ONE_BIT << texiao_cfg->seq);
		//m_param.baojia_cur_texiao_id = texiao_cfg->shenqi_id;
		//m_cur_baojia_texiao_id = texiao_cfg->shenqi_id;
		//this->SendImageInfo(Protocol::SHENQI_SC_INFO_TYPE_BAOJIA);
		this->UseBaojiaTexiao(texiao_cfg->shenqi_id);
	}

	this->SendSingleShenqiInfo(Protocol::SHENQI_SC_INFO_TYPE_BAOJIA, shenqi_id);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENQI);

	gamelog::g_log_shenqi.printf(LL_INFO, "Shenqi::ShenBaojiaTeXiao user[%d %s] consume_item[item_id:%d num:%d] active_texiao_flag[%lld]", 
		m_role->GetUID(), m_role->GetName(), texiao_cfg->active_texiao_stuff_id, texiao_cfg->active_texiao_stuff_count, m_param.baojia_texiao_flag);

	return true;
}

bool Shenqi::IsActiveShenbingTeXiao(int id, int quality)
{
	if (id < 0 || id >= SHENQI_SUIT_NUM_MAX)
	{
		return false;
	}

	const ShenqiActiveTeXiaoCfg *texiao_cfg = LOGIC_CONFIG->GetShenqiCfg().GetShenbingActiveTeXiaoCfg(id, quality);
	if (nullptr == texiao_cfg)
	{
		return false;
	}

	if (0 != (m_param.shenbing_texiao_flag & (LL_ONE_BIT << texiao_cfg->seq)))
	{
		return true;
	}

	return false;
}

bool Shenqi::IsActiveBaojiaTeXiao(int id, int quality)
{
	if (id < 0 || id >= SHENQI_SUIT_NUM_MAX)
	{
		return false;
	}

	const ShenqiActiveTeXiaoCfg *texiao_cfg = LOGIC_CONFIG->GetShenqiCfg().GetBaojiaActiveTeXiaoCfg(id, quality);
	if (nullptr == texiao_cfg)
	{
		return false;
	}

	if (0 != (m_param.baojia_texiao_flag & (LL_ONE_BIT << texiao_cfg->seq)))
	{
		return true;
	}

	return false;
}
