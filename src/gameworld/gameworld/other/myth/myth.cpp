#include "myth.hpp"
#include "mythconfig.hpp"
#include "servercommon/errornum.h"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "servercommon/noticenum.h"
#include "item/itemextern.hpp"
#include "obj/character/attribute.hpp"
#include "other/capability/capability.hpp"
#include "item/money.h"
#include "other/event/eventhandler.hpp"
#include "item/knapsack.h"
#include <set>
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "protocal/msgmyth.h"
#include <algorithm>

Myth::Myth() : m_role(NULL)
{
}

Myth::~Myth()
{
}

void Myth::InitParam(Role *role, const MythParam &param)
{
	m_role = role;
	m_param = param;
}

void Myth::GetInitParam(MythParam *param)
{
	*param = m_param;
}

void Myth::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		//Attribute rand_attr_val[MYTH_ATTR_TYPE_INVALID_MAX] = { 0 };		//神魂随机属性列表

		//{
		//	// 先计算神魂随机属性总万分比
		//	for (int chpater_id = 1; chpater_id <= MAX_MYTH_CHAPTER_ID; chpater_id++)
		//	{
		//		int chpater_index = chpater_id - 1;
		//		const MythChpaterItem &chpater_item = m_param.chpater_list[chpater_index];

		//		for (int soul_god_index = 0; soul_god_index < MAX_MYTH_SOUL_SLOT; soul_god_index++)
		//		{
		//			if (chpater_item.soul_god_list[soul_god_index].Isvalid())
		//			{
		//				for (int attr_index = 0; attr_index < MAX_MYTH_SOUL_RAND_ATTR_COUNT; attr_index ++)
		//				{
		//					int type = chpater_item.soul_god_list[soul_god_index].attr_list[attr_index].attr_type;
		//					rand_attr_val[type] += chpater_item.soul_god_list[soul_god_index].attr_list[attr_index].attr_value;
		//				}
		//			}
		//		}
		//	}
		//}

		for (int chpater_id = 1; chpater_id <= MAX_MYTH_CHAPTER_ID; chpater_id++)
		{
			int chpater_index = chpater_id - 1;
			const MythChpaterItem &chpater_item = m_param.chpater_list[chpater_index];

			// 等级属性
			const MythChapterLevelConfig * level_cfg = LOGIC_CONFIG->GetMythConfig().GetChpaterLevelCfg(chpater_id, chpater_item.level);
			if (NULL != level_cfg)
			{
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, level_cfg->maxhp);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, level_cfg->gongji);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, level_cfg->fangyu);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, level_cfg->mingzhong);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, level_cfg->shanbi);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI, level_cfg->baoji);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN, level_cfg->jianren);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, level_cfg->pofang_per);
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG, level_cfg->mianshang_per);
			}
		}

		//{	// 神魂随机属性万分比加成
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] * (rand_attr_val[MYTH_ATTR_TYPE_PER_HP] / 10000.0));
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] * (rand_attr_val[MYTH_ATTR_TYPE_PER_GONGJI] / 10000.0));
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] * (rand_attr_val[MYTH_ATTR_TYPE_PER_FANGYU] / 10000.0));
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] * (rand_attr_val[MYTH_ATTR_TYPE_PER_SHANBI] / 10000.0));
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] * (rand_attr_val[MYTH_ATTR_TYPE_PER_BAOJI] / 10000.0));
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] * (rand_attr_val[MYTH_ATTR_TYPE_PER_JIANREN] / 10000.0));
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] * (rand_attr_val[MYTH_ATTR_TYPE_PER_MINGZHONG] / 10000.0));
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] * (rand_attr_val[MYTH_ATTR_TYPE_PER_GUSHANG] / 10000.0));
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += (Attribute)(m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] * (rand_attr_val[MYTH_ATTR_TYPE_PER_JIANSHANG] / 10000.0));

		//	// 神魂随机属性
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += rand_attr_val[MYTH_ATTR_TYPE_MAX_HP];
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += rand_attr_val[MYTH_ATTR_TYPE_GONGJI];
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += rand_attr_val[MYTH_ATTR_TYPE_FANGYU];
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += rand_attr_val[MYTH_ATTR_TYPE_SHANBI];
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += rand_attr_val[MYTH_ATTR_TYPE_BAOJI];
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += rand_attr_val[MYTH_ATTR_TYPE_JIANREN];
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += rand_attr_val[MYTH_ATTR_TYPE_MINGZHONG];
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG] += rand_attr_val[MYTH_ATTR_TYPE_GUSHANG];
		//	m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG] += rand_attr_val[MYTH_ATTR_TYPE_JIANSHANG];
		//}

		for (int chpater_id = 1; chpater_id <= MAX_MYTH_CHAPTER_ID; chpater_id++)
		{
			int chpater_index = chpater_id - 1;
			const MythChpaterItem &chpater_item = m_param.chpater_list[chpater_index];
			const MythResonanceItem &resonance_item = chpater_item.resonance_item;
			const int cur_resonance_level = resonance_item.resonance_level;

			// 共鸣战力-累加
			for (int level = cur_resonance_level; level >= 1; level--)
			{
				const MythResonanceLevelConfig * resonance_level_cfg = LOGIC_CONFIG->GetMythConfig().GetResonanceCfg(chpater_id, level);
				if (NULL != resonance_level_cfg)
				{
					int attr_count = 0;
					for (int solt_index = 0; solt_index < MAX_MYTH_SOUL_SLOT && attr_count < MAX_MYTH_SOUL_SLOT; solt_index++)
					{	
						// 检测当前等级槽位
						if (level == cur_resonance_level)
						{
							if (resonance_item.cur_level_resonance[solt_index] != resonance_level_cfg->position_soul_list[solt_index])
							{
								continue;
							}
						}					
						
						int attr_type = this->TranslateToRoleCharattr(resonance_level_cfg->resonance_attr_list[attr_count].attr_type);
						if (CharIntAttrs::IsRoleBaseAttr(attr_type))
						{
							m_attrs_add.AddValue(attr_type, resonance_level_cfg->resonance_attr_list[attr_count].attr_val);
						}
						else
						{
							attr_type = resonance_level_cfg->resonance_attr_list[attr_count].attr_type;
							this->AddResonanceAttrPer(m_attrs_add, attr_type, resonance_level_cfg->resonance_attr_list[attr_count].attr_val);
						}

						++attr_count;
					}

					// 需求又变了!	索引 0~3			
					// 需求：共鸣要加多一条属性。。有的是3条，有的是4条。。当有4条的时候，第三次锁定会激活3.4条属性。。
					if (MAX_MYTH_SOUL_SLOT == attr_count)
					{
						int attr_type = this->TranslateToRoleCharattr(resonance_level_cfg->resonance_attr_list[attr_count].attr_type);
						if (CharIntAttrs::IsRoleBaseAttr(attr_type))
						{
							m_attrs_add.AddValue(attr_type, resonance_level_cfg->resonance_attr_list[attr_count].attr_val);
						}
						else
						{
							attr_type = resonance_level_cfg->resonance_attr_list[attr_count].attr_type;
							this->AddResonanceAttrPer(m_attrs_add, attr_type, resonance_level_cfg->resonance_attr_list[attr_count].attr_val);
						}
					}
				}
			}

			//// 神魂基础属性
			//for (int soul_god_index = 0; soul_god_index < MAX_MYTH_SOUL_SLOT; soul_god_index++)
			//{
			//	if (chpater_item.soul_god_list[soul_god_index].Isvalid())
			//	{
			//		const MythSoulGodConfig *soul_cfg = LOGIC_CONFIG->GetMythConfig().GetSoulGodCfgByItem(chpater_item.soul_god_list[soul_god_index].item_id);
			//		if (NULL != soul_cfg)
			//		{
			//			m_attrs_add.AddAttrFromConfig(soul_cfg->attr);
			//		}
			//	}
			//}

			//// 领悟等级属性
			//const DigestionLevelConfig * digestion_level_cfg = LOGIC_CONFIG->GetMythConfig().GetDigestionCfg(chpater_id, chpater_item.digestion_level);
			//if (NULL != digestion_level_cfg)
			//{
			//	m_attrs_add.AddAttrFromConfig(digestion_level_cfg->attr);
			//}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_MYTH, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
	base_add.AddPercent(m_attrs_add.m_percent_attrs);
}

void Myth::AddResonanceAttrPer(CharIntAttrs &m_attrs_add, int attr_per_type, Attribute attr_per_val)
{
	int attr_type = CharIntAttrs::CHARINTATTR_TYPE_INVALID;

	switch (attr_per_type)
	{
	case MYTH_RESONANCE_ATTR_TYPE_PER_GONGJI:
		{
			attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI;
		}
		break;

	case MYTH_RESONANCE_ATTR_TYPE_PER_FANGYU:
		{
			attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU;
		}
		break;

	case MYTH_RESONANCE_ATTR_TYPE_PER_HP:
		{
			attr_type = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP;
		}
		break;

	default:
		break;
	}

	if (CharIntAttrs::IsRoleBaseAttr(attr_type))
	{
		m_attrs_add.AddPercentValue(attr_type, attr_per_val * ROLE_ATTR_PER_F);
	}
}


int Myth::TranslateToRoleCharattr(int resonace_type)
{
	switch (resonace_type)
	{
	case MYTH_RESONANCE_ATTR_TYPE_MAXHP:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP;
	}
	break;

	case MYTH_RESONANCE_ATTR_TYPE_GONGJI:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI;
	}
	break;

	case MYTH_RESONANCE_ATTR_TYPE_FANGYU:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU;
	}
	break;

	case MYTH_RESONANCE_ATTR_TYPE_MINGZHONG:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG;
	}
	break;

	case MYTH_RESONANCE_ATTR_TYPE_SHANBI:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI;
	}
	break;

	case MYTH_RESONANCE_ATTR_TYPE_BAOJI:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI;
	}
	break;

	case MYTH_RESONANCE_ATTR_TYPE_JIANREN:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN;
	}
	break;

	case MYTH_RESONANCE_ATTR_TYPE_CONSTANT_ZENGSHANG:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG;
	}
	break;

	case MYTH_RESONANCE_ATTR_TYPE_CONSTANT_MIANSHANG:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG;
	}
	break;

	case MYTH_RESONANCE_ATTR_TYPE_PER_BAOJI:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI;
	}
	break;

	case MYTH_RESONANCE_ATTR_TYPE_PER_GUSHANG:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG;
	}
	break;

	case MYTH_RESONANCE_ATTR_TYPE_PER_JIANSHANG:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG;
	}
	break;

	case MYTH_RESONANCE_ATTR_TYPE_SKILL_ZENGSHANG_PER:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER;
	}
	break;
	case MYTH_RESONANCE_ATTR_TYPE_SKILL_JIANSHANG_PER:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER;
	}
	break;
	case MYTH_RESONANCE_ATTR_TYPE_PER_KANGBAO:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO;
	}
	break;
	case MYTH_RESONANCE_ATTR_TYPE_MINGZHONG_PER:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG_PER;
	}
	break;
	case MYTH_RESONANCE_ATTR_TYPE_SHANBI_PER:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI_PER;
	}
	break;
	case MYTH_RESONANCE_ATTR_TYPE_GEDANG_PER:
	{
		return CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_PER;
	}
	break;

	default:
		return CharIntAttrs::CHARINTATTR_TYPE_INVALID;
	}

	return CharIntAttrs::CHARINTATTR_TYPE_INVALID;
}

void Myth::OnChpaterUplevel(int chpater_id)
{
	if (chpater_id <= 0 || chpater_id > MAX_MYTH_CHAPTER_ID)
	{
		return;
	}
	
	// 判断消耗,背包锁住了，就不用往下判断了
	Knapsack *knapsack = m_role->GetKnapsack();
	if (nullptr == knapsack)
	{
		return;
	}

	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	const int chpater_index = chpater_id - 1;
	MythChpaterItem &chpater = m_param.chpater_list[chpater_index];
	MythResonanceItem &resonance_item = chpater.resonance_item;

	int cur_level = chpater.level;
	const MythChapterLevelConfig * level_cfg = LOGIC_CONFIG->GetMythConfig().GetChpaterLevelCfg(chpater_id, cur_level + 1);
	if (NULL == level_cfg)
	{
		return;
	}

	// 取消虚拟背包
	//if ((this->GetItemCount(level_cfg->stuff_id1.item_id) < level_cfg->stuff_id1.num))
	//{
	//	m_role->NoticeNum(errornum::EN_MYTH_STUFF_NOT_ENOUGH);
	//	return;
	//}

	//if (!this->ConsumeItem(level_cfg->stuff_id1.item_id, level_cfg->stuff_id1.num))
	//{
	//	return;
	//}

	if ((knapsack->Count(level_cfg->stuff_id1.item_id) < level_cfg->stuff_id1.num))
	{
		m_role->NoticeNum(errornum::EN_MYTH_STUFF_NOT_ENOUGH);
		return;
	}	
	
	if (!knapsack->ConsumeItem(level_cfg->stuff_id1.item_id, level_cfg->stuff_id1.num, "OnChpaterUplevel"))
	{
		// 消耗失败
		return;
	}
	
	chpater.UpLevelMythChpater();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MYTH);

	// 未激活共鸣
	if (!resonance_item.IsActivatedResonance())
	{
		const MythResonanceOpenLimitItem  * resonance_open_limit_cfg = LOGIC_CONFIG->GetMythConfig().GetResonanceOpenLimitCfg(chpater_id, 1);
		if (NULL != resonance_open_limit_cfg &&
			chpater.level >= resonance_open_limit_cfg->chpater_level)
		{
			resonance_item.ActivatedResonance();
		}
	}

	if (level_cfg->is_broadcast)
	{
		int length = 0;
		if (chpater.IsActivatingMythChpater())
		{
			length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_myth_activate_broadcast,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), chpater_id);
		}
		else
		{
			length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_myth_uplevel_broadcast,
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), chpater_id, chpater.level - 1);
		}
	
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL, 0, 0);
		}
	}

	this->SendChpaterSingleInfo(chpater_id);
}

bool Myth::IsChpaterResonanceFinsh(int chpater_id)
{
	if (chpater_id <= 0 || chpater_id > MAX_MYTH_CHAPTER_ID)
	{
		return false;
	}

	int chpater_index = chpater_id - 1;
	const MythChpaterItem &chpater = m_param.chpater_list[chpater_index];

	const MythResonanceLevelConfig * resonance_cfg = LOGIC_CONFIG->GetMythConfig().GetResonanceCfg(chpater_id, chpater.resonance_item.resonance_level);
	if (NULL == resonance_cfg)
	{
		return false; 
	}
	
	const MythResonanceItem &resonance_item = chpater.resonance_item;
	for (int i = 0; i < MAX_MYTH_SOUL_SLOT; i ++)
	{
		if (resonance_item.cur_level_resonance[i] != (char)resonance_cfg->position_soul_list[i])
		{
			return false;
		}
	}

	return true;
}

void Myth::OnResonance(int chpater_id)
{
	if (chpater_id <= 0 || chpater_id > MAX_MYTH_CHAPTER_ID)
	{
		return;
	}
	
	// 判断消耗,背包锁住了，就不用往下判断了
	Knapsack *knapsack = m_role->GetKnapsack();
	if (nullptr == knapsack)
	{
		return;
	}

	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	const int chpater_index = chpater_id - 1;
	MythChpaterItem &chpater = m_param.chpater_list[chpater_index];
	MythResonanceItem &resonance_item = chpater.resonance_item;
	if (!resonance_item.IsActivatedResonance())
	{
		return;
	}

	if (this->IsChpaterResonanceFinsh(chpater_id))
	{
		m_role->NoticeNum(errornum::EN_MYTH_HAD_RESONANCED_ALREAD);					//当前等级已经共鸣成功了，不能重复该等级共鸣
		return;
	}

	const MythResonanceLevelConfig * resonance_cfg = LOGIC_CONFIG->GetMythConfig().GetResonanceCfg(chpater_id, resonance_item.resonance_level);
	if (NULL == resonance_cfg)
	{
		return;
	}

	{
		// 改需求了，需要个简单粗暴的
		// 需求：共鸣的要改成共鸣一次，必定锁定其中一个。不会出现1个以上锁定。。
		std::vector<int> soul_slot_index;		
		for (int i = 0; i < MAX_MYTH_SOUL_SLOT; i++)
		{
			// 未锁定
			if (resonance_item.cur_level_resonance[i] != (char)resonance_cfg->position_soul_list[i])
			{
				soul_slot_index.push_back(i);
			}
		}

		if (soul_slot_index.empty())
		{
			return;
		}
		std::random_shuffle(soul_slot_index.begin(), soul_slot_index.end());

		if ((knapsack->Count(resonance_cfg->stuff.item_id) < resonance_cfg->stuff.num))
		{
			m_role->NoticeNum(errornum::EN_MYTH_STUFF_NOT_ENOUGH);
			return;
		}

		if (!knapsack->ConsumeItem(resonance_cfg->stuff.item_id , resonance_cfg->stuff.num, "OnResonance"))
		{
			// 消耗失败
			return;
		}

		//	锁定
		int rand_soul_slot_index = *(soul_slot_index.begin());
		if (0 <= rand_soul_slot_index && rand_soul_slot_index < MAX_MYTH_SOUL_SLOT)
		{
			resonance_item.cur_level_resonance[rand_soul_slot_index] = (char)resonance_cfg->position_soul_list[rand_soul_slot_index];
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MYTH);
		}
	}

	if (this->IsChpaterResonanceFinsh(chpater_id))
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_myth_resonance,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), chpater_id, resonance_item.resonance_level);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL, 0, 0);
		}

		this->ResonanceUpLevel(chpater_id);
	}

	this->SendChpaterSingleInfo(chpater_id);
}

void Myth::ResonanceUpLevel(int chpater_id)
{
	if (chpater_id <= 0 || chpater_id > MAX_MYTH_CHAPTER_ID)
	{
		return;
	}

	const int chpater_index = chpater_id - 1;
	MythChpaterItem &chpater = m_param.chpater_list[chpater_index];
	MythResonanceItem &resonance_item = chpater.resonance_item;

	const MythResonanceOpenLimitItem  * resonance_open_limit_cfg = LOGIC_CONFIG->GetMythConfig().GetResonanceOpenLimitCfg(chpater_id, resonance_item.resonance_level + 1);
	if (nullptr == resonance_open_limit_cfg)
	{
		return;
	}

	// 篇章0级不能共鸣
	if (chpater.level <= 0 && chpater.level < resonance_open_limit_cfg->chpater_level)
	{
		return;
	}

	// 已到达最大等级
	const MythResonanceLevelConfig * next_level_resonance_cfg = LOGIC_CONFIG->GetMythConfig().GetResonanceCfg(chpater_id, resonance_item.resonance_level + 1);
	if (nullptr == next_level_resonance_cfg)
	{
		return;
	}

	resonance_item.ResonanceUpLevel();
}

void Myth::SendChpaterSingleInfo(int chpater_id)
{
	if (chpater_id <= 0 || chpater_id > MAX_MYTH_CHAPTER_ID)
	{
		return;
	}

	static Protocol::SCMythChpaterSingleInfo msg;

	msg.soul_essence = m_param.soul_essence;
	msg.chpater_id = chpater_id;
	msg.single_item = m_param.chpater_list[chpater_id - 1];

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&msg, sizeof(msg));
}

void Myth::SendChpaterAllInfo()
{
	static Protocol::SCMythChpaterInfo msg;

	msg.soul_essence = m_param.soul_essence;

	for (int i = 0; i < MAX_MYTH_CHAPTER_ID; i ++)
	{
		msg.chpater_list[i] = m_param.chpater_list[i];
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&msg, sizeof(msg));
}

bool Myth::AddSoulItem(MythSoulGodItem &tmp_item, const char *reason, bool is_send)
{
	return false;

	//if (NULL == reason)
	//{
	//	return false;
	//}

	//if (!this->EmptyBackpackNumMoreThan(1))
	//{
	//	m_role->NoticeNum(errornum::EN_MYTH_BACKPACK_NOT_ENOUGH);
	//	return false;
	//}

	//if (NULL == LOGIC_CONFIG->GetMythConfig().GetSoulGodCfgByItem(tmp_item.item_id))
	//{
	//	return false;
	//}

	//for (int b_index = 0; b_index < MAX_MYTH_KNAPSACK_GIRD_COUNT; ++b_index)
	//{
	//	if (m_param.knapasck_list[b_index].item_id <= 0) // 找到空格子，放进去
	//	{
	//		m_param.knapasck_list[b_index] = tmp_item;
	//		if (is_send)
	//		{
	//			this->SendBackpackAdditionInfo(b_index);
	//		}

	//		break;
	//	}
	//}

	//return true;
}

bool Myth::AddItem(ItemID item_id, int num)
{
	return false;

	//if (item_id <= 0)
	//{
	//	return false;
	//}

	//std::vector<int> empty_index_list;

	//int put_num = num;
	//for (int b_index = 0; b_index < MAX_MYTH_KNAPSACK_GIRD_COUNT; ++b_index)
	//{
	//	if (!m_param.knapasck_list[b_index].Isvalid())
	//	{
	//		empty_index_list.push_back(b_index);
	//	}

	//	if (item_id == m_param.knapasck_list[b_index].item_id)
	//	{
	//		int add_num = MAX_MYTH_KNAPSACK_GIRD_STACK_COUNT - m_param.knapasck_list[b_index].num;

	//		add_num = add_num > put_num ? put_num : add_num;
	//		m_param.knapasck_list[b_index].num += add_num;
	//		put_num -= add_num;

	//		this->SendBackpackAdditionInfo(b_index);
	//		if (put_num <= 0) return true;
	//	}
	//}

	//std::vector<int>::iterator it = empty_index_list.begin();
	//for (; it != empty_index_list.end(); it++)
	//{
	//	MythSoulGodItem &temp = m_param.knapasck_list[*it];
	//	temp.item_id = item_id;

	//	int add_num = MAX_MYTH_KNAPSACK_GIRD_STACK_COUNT - temp.num;
	//	add_num = add_num > put_num ? put_num : add_num;
	//	temp.num += add_num;
	//	put_num -= add_num;

	//	this->SendBackpackAdditionInfo(*it);
	//	if (put_num <= 0) return true;
	//}

	//return true;
}

bool Myth::EmptyBackpackNumMoreThan(int check_num)
{
	return false;

	//if (check_num <= 0)
	//{
	//	return true;
	//}

	//for (int index = 0; index < MAX_MYTH_KNAPSACK_GIRD_COUNT; ++index)
	//{
	//	if (m_param.knapasck_list[index].item_id <= 0)
	//	{
	//		if (--check_num <= 0)
	//		{
	//			return true;
	//		}
	//	}
	//}

	//return false;
}

bool Myth::BatchUseItemCheck(ItemID item_id, int check_num)
{
	return false;

	//if (check_num <= 0)
	//{
	//	return true;
	//}

	//std::vector<int> empty_index_list;

	//for (int b_index = 0; b_index < MAX_MYTH_KNAPSACK_GIRD_COUNT; ++b_index)
	//{
	//	if (!m_param.knapasck_list[b_index].Isvalid())
	//	{
	//		empty_index_list.push_back(b_index);
	//	}

	//	if (item_id == m_param.knapasck_list[b_index].item_id)
	//	{
	//		int add_num = MAX_MYTH_KNAPSACK_GIRD_STACK_COUNT - m_param.knapasck_list[b_index].num;

	//		add_num = add_num > check_num ? check_num : add_num;
	//		check_num -= add_num;

	//		if (check_num <= 0) return true;
	//	}
	//}

	//std::vector<int>::iterator it = empty_index_list.begin();
	//for (; it != empty_index_list.end(); it++)
	//{
	//	MythSoulGodItem &temp = m_param.knapasck_list[*it];

	//	int add_num = MAX_MYTH_KNAPSACK_GIRD_STACK_COUNT - temp.num;
	//	add_num = add_num > check_num ? check_num : add_num;
	//	check_num -= add_num;

	//	if (check_num <= 0) return true;
	//}

	//return false;
}

void Myth::InlaySoul(int chpater_id, int backpack_index, int slot_index)
{
//	if (chpater_id <= 0 || chpater_id > MAX_MYTH_CHAPTER_ID || slot_index < 0 || slot_index >= MAX_MYTH_SOUL_SLOT || backpack_index < 0 || backpack_index >= MAX_MYTH_KNAPSACK_GIRD_COUNT)
//	{
//		return;
//	}
//
//	int chpater_index = chpater_id - 1;
//	MythSoulGodItem &backpack_item = m_param.knapasck_list[backpack_index];
//	const MythSoulGodConfig *item_cfg = LOGIC_CONFIG->GetMythConfig().GetSoulGodCfgByItem(backpack_item.item_id);
//	if (NULL == item_cfg)
//	{
//		return;
//	}
//
//	if (item_cfg->soul_type != slot_index + 1)		// 每种魂器要对应格子
//	{
//		return;
//	}
//
//	MythSoulGodItem tmp_item = m_param.knapasck_list[backpack_index];
//	m_param.knapasck_list[backpack_index] = m_param.chpater_list[chpater_index].soul_god_list[slot_index];
//	m_param.chpater_list[chpater_index].soul_god_list[slot_index] = tmp_item;
//
//	this->SendBackpackAdditionInfo(backpack_index);
//	this->SendChpaterSingleInfo(chpater_id);
//
//	m_role->NoticeNum(noticenum::NT_MYTH_GOD_SOUL_INLAY_SUCC);
//	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MYTH);
}

void Myth::TakeOffSoul(int chpater_id, int slot_index)
{
//	if (chpater_id <= 0 || chpater_id > MAX_MYTH_CHAPTER_ID || slot_index < 0 || slot_index >= MAX_MYTH_SOUL_SLOT)
//	{
//		return;
//	}
//
//	int chpater_index = chpater_id - 1;
//	MythSoulGodItem &tmp_item = m_param.chpater_list[chpater_index].soul_god_list[slot_index];
//	if (!tmp_item.Isvalid())
//	{
//		return;
//	}
//
//	if (!this->AddSoulItem(tmp_item, __FUNCTION__))
//	{
//		m_role->NoticeNum(errornum::EN_MYTH_BACKPACK_NOT_ENOUGH);
//		return;
//	}
//
//	tmp_item.Reset();
//
//	m_role->NoticeNum(noticenum::NT_MYTH_GOD_SOUL_TAKE_OFF_SUCC);
//	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MYTH);
//
//	this->SendBackpackInfo();
//	this->SendChpaterSingleInfo(chpater_id);
}

bool Myth::OnDigestion(int chpater_id)
{
	return false;

	//if (chpater_id <= 0 || chpater_id > MAX_MYTH_CHAPTER_ID)
	//{
	//	return false;
	//}

	//int chpater_index = chpater_id - 1;
	//MythChpaterItem &chpater = m_param.chpater_list[chpater_index];

	//const DigestionLevelConfig * digestion_cfg = LOGIC_CONFIG->GetMythConfig().GetDigestionCfg(chpater_id, chpater.digestion_level);
	//if (NULL == digestion_cfg)
	//{
	//	m_role->NoticeNum(errornum::EN_MYTH_DIGEATION_LEVEL_LIMIT);
	//	return false;
	//}

	//const DigestionLevelConfig * next_digestion_cfg = LOGIC_CONFIG->GetMythConfig().GetDigestionCfg(chpater_id, chpater.digestion_level + 1);
	//if (NULL == next_digestion_cfg)
	//{
	//	m_role->NoticeNum(errornum::EN_MYTH_DIGEATION_LEVEL_LIMIT);
	//	return false;
	//}

	//if (m_param.soul_essence < digestion_cfg->single_essence)
	//{
	//	m_role->NoticeNum(errornum::EN_MYTH_SOUL_ESSENCE_NOT_ENOUGH);
	//	return false;
	//}

	//m_param.soul_essence -= digestion_cfg->single_essence;

	//int multiple = LOGIC_CONFIG->GetMythConfig().GetBaoJiMultiple();
	//chpater.digestion_level_val += digestion_cfg->single_essence * multiple;

	//bool is_level_up = false;
	//if (chpater.digestion_level_val >= digestion_cfg->need_essence)
	//{
	//	chpater.digestion_level_val = 0;
	//	chpater.digestion_level++;

	//	is_level_up = true;

	//	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MYTH);
	//}

	//this->SendChpaterSingleInfo(chpater_id);
	//return !is_level_up;
}

void Myth::OnDecompose(int knapsack_index)
{
	//if (knapsack_index < 0 || knapsack_index >= MAX_MYTH_KNAPSACK_GIRD_COUNT)
	//{
	//	return;
	//}

	//MythSoulGodItem &knapsack_item = m_param.knapasck_list[knapsack_index];
	//if (!knapsack_item.Isvalid())
	//{
	//	return;
	//}

	//int soul_essence = LOGIC_CONFIG->GetMythConfig().GetDecomposeSoulEssence(knapsack_item.item_id);
	//if (soul_essence <= 0)
	//{
	//	return;
	//}

	//if (m_param.soul_essence + soul_essence * knapsack_item.num > INT_MAX)
	//{
	//	m_param.soul_essence = INT_MAX;
	//}
	//else
	//{
	//	m_param.soul_essence += soul_essence * knapsack_item.num;
	//}

	//knapsack_item.Reset();

	//this->SendBackpackAdditionInfo(knapsack_index);
	//this->SendChpaterSingleInfo(1);
}

void Myth::Compose(int seq, int index_1, int index_2, int index_3)
{
	//if (index_1 < 0 || index_1 >= MAX_MYTH_KNAPSACK_GIRD_COUNT || index_3 < 0 || index_3 >= MAX_MYTH_KNAPSACK_GIRD_COUNT || index_2 < 0 || index_2 >= MAX_MYTH_KNAPSACK_GIRD_COUNT)
	//{
	//	return;
	//}

	//MythSoulGodItem &tmp_equip_item1 = m_param.knapasck_list[index_1];
	//MythSoulGodItem &tmp_equip_item2 = m_param.knapasck_list[index_2];
	//MythSoulGodItem &tmp_equip_item3 = m_param.knapasck_list[index_3];

	//const MythSoulGodConfig *tmp_item_cfg1 = LOGIC_CONFIG->GetMythConfig().GetSoulGodCfgByItem(tmp_equip_item1.item_id);
	//if (NULL == tmp_item_cfg1) return;

	//const MythSoulGodConfig *tmp_item_cfg2 = LOGIC_CONFIG->GetMythConfig().GetSoulGodCfgByItem(tmp_equip_item2.item_id);
	//if (NULL == tmp_item_cfg2) return;

	//const MythSoulGodConfig *tmp_item_cfg3 = LOGIC_CONFIG->GetMythConfig().GetSoulGodCfgByItem(tmp_equip_item3.item_id);
	//if (NULL == tmp_item_cfg3) return;

	//const MythComposeCfg *compose_cfg = LOGIC_CONFIG->GetMythConfig().GetComposeCfg(seq);
	//if (NULL == compose_cfg) return;

	//if (tmp_equip_item3.item_id != tmp_equip_item2.item_id || tmp_equip_item1.item_id != tmp_equip_item2.item_id) return;

	//const MythSoulGodConfig *target_equip = LOGIC_CONFIG->GetMythConfig().GetSoulGodCfgByItem(compose_cfg->after_item_id);
	//if (NULL == target_equip) return;

	//if (!LOGIC_CONFIG->GetMythConfig().CheckAttrStarCount(tmp_equip_item1, compose_cfg->need_start_num, tmp_item_cfg1->quality)) return;

	//if (!LOGIC_CONFIG->GetMythConfig().CheckAttrStarCount(tmp_equip_item2, compose_cfg->need_start_num, tmp_item_cfg2->quality)) return;

	//if (!LOGIC_CONFIG->GetMythConfig().CheckAttrStarCount(tmp_equip_item3, compose_cfg->need_start_num, tmp_item_cfg3->quality)) return;

	//MythSoulGodItem target_equip_item;
	//target_equip_item.Reset();
	//target_equip_item.item_id = compose_cfg->after_item_id;
	//target_equip_item.num = 1;

	//// 生成属性
	//const SoulAttrConfig *quality_cfg = LOGIC_CONFIG->GetMythConfig().GetSoulAttrCfg(target_equip->quality);
	//if (NULL == quality_cfg) return;

	//const RandStarConfig *star_cfg = LOGIC_CONFIG->GetMythConfig().GetStarCfg(target_equip->quality);
	//if (NULL == star_cfg) return;

	//int star_attr_num = compose_cfg->give_start_num;
	//int attr_num = 0;
	//std::set<int> star_attr_type_set;
	//std::set<int> normal_attr_type_set;
	//for (int i = 0; i < star_cfg->rand_attr_num && attr_num < MAX_MYTH_SOUL_RAND_ATTR_COUNT; ++i)
	//{
	//	const SoulRandAttrConfig *attr_cfg;
	//	if (star_attr_num > 0)
	//	{
	//		--star_attr_num;

	//		attr_cfg = LOGIC_CONFIG->GetMythConfig().GetRandomStarAttr(target_equip->quality, star_attr_type_set);
	//		if (NULL != attr_cfg)
	//		{
	//			star_attr_type_set.insert(attr_cfg->attr_type);
	//		}
	//	}
	//	else
	//	{
	//		attr_cfg = LOGIC_CONFIG->GetMythConfig().GetRandomNormalAttr(target_equip->quality, normal_attr_type_set);
	//		if (NULL != attr_cfg) normal_attr_type_set.insert(attr_cfg->attr_type);
	//	}

	//	if (NULL == attr_cfg) continue;

	//	target_equip_item.attr_list[attr_num].attr_type = attr_cfg->attr_type;
	//	target_equip_item.attr_list[attr_num].attr_value = attr_cfg->attr_val;
	//	++attr_num;
	//}

	//// 消耗物品
	//if (compose_cfg->is_need_item && !this->ConsumeItem(compose_cfg->consumeitem_id, compose_cfg->item_num))
	//{
	//	m_role->NoticeNum(errornum::EN_ITEM_NOT_ENOUGH);
	//	return;
	//}

	//tmp_equip_item1.Reset();
	//tmp_equip_item2.Reset();
	//tmp_equip_item3.Reset();

	//this->AddSoulItem(target_equip_item, __FUNCTION__);
	//this->SendBackpackInfo();
	//m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENSHOU);
	//m_role->NoticeNum(noticenum::NT_EQUIP_COMPOSE_SUCC);

	//if (compose_cfg->is_broadcast)
	//{
	//	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_myth_compose_broadcast,
	//		m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), compose_cfg->give_start_num);
	//	if (length > 0)
	//	{
	//		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL, 0, 0);
	//	}
	//}
}

bool Myth::TransformFromKnapsackItem(ItemID item_id, int item_base_num, const char *reason)
{
	return false;

	//if (0 == item_id || item_base_num <= 0)
	//{
	//	return false;
	//}

	//bool is_succ = true;
	//bool is_send = false;
	//const MythSoulGodConfig *item_cfg = LOGIC_CONFIG->GetMythConfig().GetSoulGodCfgByItem(item_id);
	//if (NULL != item_cfg)
	//{
	//	// 非堆叠类检测
	//	if (!this->EmptyBackpackNumMoreThan(item_base_num))
	//	{
	//		m_role->NoticeNum(errornum::EN_MYTH_BACKPACK_NOT_ENOUGH);
	//		return false;
	//	}		

	//	for (int index = 0; index < item_base_num; index++)
	//	{
	//		if (index == item_base_num - 1) is_send = true;

	//		const RandStarConfig *star_cfg = LOGIC_CONFIG->GetMythConfig().GetStarCfg(item_cfg->quality);
	//		if (NULL == star_cfg)
	//		{
	//			return false;
	//		}

	//		static MythSoulGodItem tmp_item; tmp_item.Reset();
	//		tmp_item.item_id = item_id;
	//		tmp_item.num = 1;
	//		tmp_item.quality = (unsigned char)item_cfg->quality;

	//		int star_attr_num = 0;
	//		int attr_num = 0;
	//		LOGIC_CONFIG->GetMythConfig().GetRandStarCount(item_cfg->quality, attr_num, star_attr_num);
	//		if (attr_num <= 0) return false;

	//		std::set<int> star_attr_type_set;
	//		std::set<int> normal_attr_type_set;
	//		for (int i = 0; i < star_cfg->rand_attr_num && i < MAX_MYTH_SOUL_RAND_ATTR_COUNT; ++i)
	//		{
	//			const SoulRandAttrConfig *attr_cfg;
	//			if (star_attr_num > 0)
	//			{
	//				--star_attr_num;

	//				attr_cfg = LOGIC_CONFIG->GetMythConfig().GetRandomStarAttr(item_cfg->quality, star_attr_type_set);
	//				if (NULL != attr_cfg)
	//				{
	//					star_attr_type_set.insert(attr_cfg->attr_type);
	//				}
	//			}
	//			else
	//			{
	//				attr_cfg = LOGIC_CONFIG->GetMythConfig().GetRandomNormalAttr(item_cfg->quality, normal_attr_type_set);
	//				if (NULL != attr_cfg) normal_attr_type_set.insert(attr_cfg->attr_type);
	//			}

	//			if (NULL == attr_cfg) continue;

	//			tmp_item.attr_list[i].attr_type = attr_cfg->attr_type;
	//			tmp_item.attr_list[i].attr_value = attr_cfg->attr_val;
	//		}

	//		is_succ = is_succ && this->AddSoulItem(tmp_item, reason, is_send);
	//	}
	//}
	//else
	//{
	//	// 堆叠类检测
	//	if (!this->BatchUseItemCheck(item_id, item_base_num))
	//	{
	//		m_role->NoticeNum(errornum::EN_MYTH_BACKPACK_NOT_ENOUGH);
	//		return false;
	//	}

	//	is_succ = this->AddItem(item_id, item_base_num);
	//}

	//return is_succ;
}

void Myth::SendBackpackInfo()
{
	//Protocol::SCMythKnapaskInfo msg;
	//msg.is_all = 1;
	//msg.count = 0;

	//for (int i = 0; i < MAX_MYTH_KNAPSACK_GIRD_COUNT; i++)
	//{
	//	if (m_param.knapasck_list[i].Isvalid())
	//	{
	//		msg.knapsack_list[msg.count].index = i;
	//		msg.knapsack_list[msg.count].item = m_param.knapasck_list[i];

	//		msg.count++;
	//	}
	//}

	//int sendlen = sizeof(msg) - (MAX_MYTH_KNAPSACK_GIRD_COUNT - msg.count) * sizeof(msg.knapsack_list[0]);
	//if (msg.count > 0 && sendlen > 0)
	//{
	//	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&msg, sendlen);
	//}
}

void Myth::SendBackpackAdditionInfo(int knapsack_index)
{
	//if (knapsack_index < 0 || knapsack_index >= MAX_MYTH_KNAPSACK_GIRD_COUNT)
	//{
	//	return;
	//}

	//Protocol::SCMythKnapaskInfo msg;
	//msg.is_all = 0;
	//msg.count = 0;

	//msg.knapsack_list[msg.count].index = knapsack_index;
	//msg.knapsack_list[msg.count].item = m_param.knapasck_list[knapsack_index];
	//msg.count++;

	//int sendlen = sizeof(msg) - (MAX_MYTH_KNAPSACK_GIRD_COUNT - msg.count) * sizeof(msg.knapsack_list[0]);
	//EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&msg, sendlen);
}

bool Myth::ConsumeItem(ItemID item_id, int num)
{
	return false;

	//if (num > this->GetItemCount(item_id))
	//{
	//	return false;
	//}

	//int consume_count = num;
	//int real_consume = 0;
	//for (int index = 0; index < MAX_MYTH_KNAPSACK_GIRD_COUNT; ++index)
	//{
	//	if (item_id == m_param.knapasck_list[index].item_id)
	//	{
	//		real_consume = m_param.knapasck_list[index].num > consume_count ? consume_count : m_param.knapasck_list[index].num;

	//		consume_count -= real_consume;
	//		m_param.knapasck_list[index].num -= real_consume;

	//		if (m_param.knapasck_list[index].num <= 0)
	//		{
	//			m_param.knapasck_list[index].Reset();
	//		}
	//	}

	//	if (consume_count <= 0)
	//	{
	//		break;
	//	}
	//}

	//this->SendBackpackInfo();
	//return true;
}

int Myth::GetItemCount(ItemID item_id)
{
	return 0;

	//int count = 0;
	//for (int index = 0; index < MAX_MYTH_KNAPSACK_GIRD_COUNT; ++index)
	//{
	//	if (item_id == m_param.knapasck_list[index].item_id)
	//	{
	//		count += m_param.knapasck_list[index].num;
	//	}
	//}

	//return count;
}

void Myth::GmAddEssence(int num)
{
	//if (num <= 0) return;
	//if (m_param.soul_essence + num > INT_MAX)
	//{
	//	m_param.soul_essence = INT_MAX;
	//}
	//else
	//{
	//	m_param.soul_essence += num;
	//}

	//this->SendChpaterSingleInfo(1);
}
