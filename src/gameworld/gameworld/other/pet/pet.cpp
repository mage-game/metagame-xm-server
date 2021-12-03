#include "pet.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/pet/petconfig.hpp"
#include "item/itemextern.hpp"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "other/event/eventhandler.hpp"
#include "protocal/msgpet.h"
#include "servercommon/noticenum.h"
#include "obj/character/attribute.hpp"
#include "protocal/msgpet.h"
#include "item/itempool.h"
#include "item/knapsack.h"
#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/namefilter.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "skill/skillpool.hpp"
#include "skill/skillbase.hpp"
#include "gamelog.h"
#include "other/capability/capability.hpp"
#include "other/sublock/sublock.hpp"
#include "scene/scene.h"

Pet::Pet() : m_role(NULL), m_ignore_fangyu_percent(0), m_ignore_fangyu_percent_level(0) 
{
}

Pet::~Pet()
{

}

void Pet::Init(Role *role, const PetParam &param)
{
	m_role = role;
	m_param = param;

	for (int index = PET_MAX_STORE_COUNT - 1; index >= 0; -- index)
	{
		PetStoreItem &store_item = m_param.backpack_item_list[index];
		if (ItemBase::INVALID_ITEM_ID == store_item.id || 0 == store_item.num)
		{
			m_empty_grid_index_list.push(index);
		}
	}
}

void Pet::GetInitParam(PetParam *param)
{
	*param = m_param;
}

void Pet::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		std::set<int> cur_own_pet_id_set;

		for (int pet_id = 0; pet_id < PET_MAX_COUNT_LIMIT; ++ pet_id)
		{
			if (m_param.own_pet_list[pet_id].level <= 0)
			{
				continue;
			}

			cur_own_pet_id_set.insert(pet_id);

			// 等级
			const PetLevelConfig *level_cfg = LOGIC_CONFIG->GetPetCfg().GetPetLevelCfg(pet_id, m_param.own_pet_list[pet_id].level);
			if (NULL != level_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg->shanbi;

			}

			// 阶
			const PetGradeConfig *grade_cfg = LOGIC_CONFIG->GetPetCfg().GetPetGradeCfg(pet_id, m_param.own_pet_list[pet_id].grade);
			if (NULL != grade_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += grade_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += grade_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += grade_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += grade_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += grade_cfg->shanbi;
			}

			// 亲密等级
			int qinmi_level = m_param.pet_qinmi_list[pet_id].level;
			if (qinmi_level > 0)
			{
				const PetQinmiLevelConfig::StarConfig * star_cfg = LOGIC_CONFIG->GetPetCfg().GetQinmiLevelStarCfg(qinmi_level, 1);
				if (NULL != star_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += star_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += star_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += star_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += star_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += star_cfg->shanbi;
				}
			}
		}

		{
			// 宠物羁绊属性
			int extra_attr_count = LOGIC_CONFIG->GetPetCfg().GetExtraAttrCount();
			for (int index = 0; index < extra_attr_count; ++ index)
			{
				const PetExtraAttrConfig *extra_attr_cfg = LOGIC_CONFIG->GetPetCfg().GetExtraAttrPetList(index);
				if (NULL != extra_attr_cfg)
				{
					bool cur_suit_active = true;
					for (int i = 0; i < PET_MAX_EXTRA_ATTR_PET_COUNT_LIMIT; ++ i)
					{
						int temp_pet_id = extra_attr_cfg->pet_id_list[i];

						if (INVALID_PET_ID == temp_pet_id) continue;

						if (cur_own_pet_id_set.find(temp_pet_id) == cur_own_pet_id_set.end())
						{
							cur_suit_active = false;
							break;
						}
					}

					if (cur_suit_active)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += extra_attr_cfg->maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += extra_attr_cfg->gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += extra_attr_cfg->fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += extra_attr_cfg->mingzhong;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += extra_attr_cfg->shanbi;
					}
				}
			}
		}

		{
			// 特殊形象
			for (int img_id = 1; img_id <= PET_SPECIAL_IMG_MAX_ID_LIMIT; ++ img_id)
			{
				if (0 != (m_param.special_img_active_flag & (1 << img_id)))
				{
					const PetSpecialConfig *cfg_item = LOGIC_CONFIG->GetPetCfg().GetSpecialImgCfg(img_id);
					if (NULL != cfg_item)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi;
					}

					const int img_level = m_param.special_img_level_list[img_id];
					const PetSpecialImageUplevelConfig::UplevelItemConfig *level_cfg = LOGIC_CONFIG->GetPetCfg().GetSpecialImgUplevelCfg(img_id, img_level);
					if (NULL != level_cfg)
					{
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg->gongji;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg->fangyu;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg->maxhp;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg->mingzhong;
						m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg->shanbi;
					}
				}
			}
		}

		{
			this->ReCalcIgnoreFangyuPercent();
			// 破防
			if (m_ignore_fangyu_percent > 0)
			{
//				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG] += m_ignore_fangyu_percent;

				// 下面特殊属性用乘法
				m_attrs_add.AddValue(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG, m_ignore_fangyu_percent);

			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_PET, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void Pet::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;

	m_param.food_market_free_times = LOGIC_CONFIG->GetPetCfg().GetOtherCfg().food_market_free_times_per_day;
	this->SendPetBackpackList();

	for (int index = 0; index < PET_MAX_COUNT_LIMIT; ++ index)
	{
		m_param.pet_qinmi_list[index].qinmi_val = 0;
	}
	this->SendPetQinmiAllInfo();
}


void Pet::ActivePet(int pet_id)
{
	if (pet_id < 0 || pet_id >= PET_MAX_COUNT_LIMIT)
	{
		return;
	}

	const PetEggConfig *egg_cfg = LOGIC_CONFIG->GetPetCfg().GetPetEggCfg(pet_id);
	if (NULL == egg_cfg || egg_cfg->pet_id < 0 || egg_cfg->pet_id >= PET_MAX_COUNT_LIMIT)
	{
		return;
	}

	if (m_param.own_pet_list[pet_id].level > 0)
	{
		m_role->NoticeNum(errornum::EN_PET_HAS_ACTIVE);
		return;
	}

	const PetInfoConfig *info_cfg = LOGIC_CONFIG->GetPetCfg().GetPetInfo(pet_id);
	if (NULL == info_cfg)
	{
		return;
	}

	// 消耗
	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!m_role->GetKnapsack()->ConsumeItem(egg_cfg->item_id, 1, "ActivePet"))
	{
		m_role->NoticeNum(errornum::EN_PET_EGG_NOT_ENOUGH);
		return;
	}

	// 激活宠物
	m_param.own_pet_list[pet_id].level = 1;
	F_STRNCPY(m_param.own_pet_list[pet_id].name, info_cfg->pet_name, sizeof(GameName));

	m_role->NoticeNum(noticenum::NT_PET_ACTIVE_SUCC);

	if (INVALID_PET_ID == m_param.cur_on_use_pet_id) // 首次激活宠物，默认出战
	{
		this->SelectPet(pet_id);
	}

	this->SendPetInfo();
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_PET);

	this->CheckIsActiveSuit(pet_id);

	// 日志
	gamelog::g_log_pet.printf(LL_INFO, "Pet::ActivePet user[%d %s] egg_item_id[%d] petid[%d] petname[%s] petlevel[%d]",
		m_role->GetUID(), m_role->GetName(), egg_cfg->item_id, egg_cfg->pet_id, m_param.own_pet_list[egg_cfg->pet_id].name, m_param.own_pet_list[egg_cfg->pet_id].level);
}

void Pet::SelectPet(int pet_id)
{
	if (pet_id < 0 || pet_id >= PET_MAX_COUNT_LIMIT)
	{
		return;
	}

	if (m_param.own_pet_list[pet_id].level <= 0)
	{
		m_role->NoticeNum(errornum::EN_PET_HAS_NOT_ACTIVE);
		return;
	}

	int old_use_pet_id = m_param.cur_on_use_pet_id;

	if (m_param.cur_on_use_pet_id == pet_id)		//取消出战
	{
		this->UseImage(INVALID_PET_SPECIAL_IMG);	//先取消幻化
		m_param.cur_on_use_pet_id = INVALID_PET_ID;
		m_role->NoticeNum(noticenum::NT_PET_CALL_BACK_SUCC);
	}
	else
	{
		m_param.cur_on_use_pet_id = pet_id;
		m_role->NoticeNum(noticenum::NT_PET_FIGHT_OUT_SUCC);
	}

	static Protocol::SCFightOutPetInfo foinfo;
	foinfo.cur_on_use_pet_id = m_param.cur_on_use_pet_id;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&foinfo, sizeof(foinfo));

	this->SendPetInfo();
	this->SendPetQinmiSingleInfo(pet_id);
	this->BroadcastPetInfo();

	this->ChangePetSkill(old_use_pet_id, m_param.cur_on_use_pet_id);
}

void Pet::ChangeCurPetName(const GameName new_name)
{
	if (NULL == new_name)
	{
		return;
	}

	GameName tmp_new_name; memset(tmp_new_name, 0, sizeof(tmp_new_name));

	memcpy(tmp_new_name, new_name, sizeof(tmp_new_name));
	tmp_new_name[sizeof(GameName) - 1] = 0;

	InvalidCharFilte(tmp_new_name);

	bool NAMEFILTER_OPEN_FLAG = ServerConfigPool::Instance().common_config.namefilter_open_flag;
	if (NAMEFILTER_OPEN_FLAG && NameFilter::Instance().HasSensitiveWord(sizeof(GameName), tmp_new_name)) 
	{
		m_role->NoticeNum(errornum::EN_RENAME_NAME_INVALID);
		return;
	} 

	if (INVALID_PET_ID == m_param.cur_on_use_pet_id || m_param.own_pet_list[m_param.cur_on_use_pet_id].level <= 0)
	{
		m_role->NoticeNum(errornum::EN_PET_HAS_NOT_ACTIVE);
		return;
	}

	GameName old_name;
	F_STRNCPY(old_name, m_param.own_pet_list[m_param.cur_on_use_pet_id].name, sizeof(GameName));

	// 获取可消耗的背包材料列表
	const PetOtherConfig &other_cfg = LOGIC_CONFIG->GetPetCfg().GetOtherCfg();
	short stuff_count = 0;

	//材料
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[stuff_count].item_id = other_cfg.rename_card_id;
	stuff_list[stuff_count].num = 1;
	stuff_list[stuff_count].buyable = false;
	++ stuff_count;

	// 检查消耗
	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
	{
		return;
	}

	// 消耗改名卡
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "Pet::ChangeCurPetName"))
	{
		return;
	}

	F_STRNCPY(m_param.own_pet_list[m_param.cur_on_use_pet_id].name, tmp_new_name, sizeof(GameName)); 

	this->BroadcastPetInfo();

	this->SendPetInfo();

	m_role->NoticeNum(noticenum::NT_PET_RENAME_SUCC);

	gamelog::g_log_pet.printf(LL_INFO, "Pet::ChangeCurPetName user[%d %s] petid[%d] oldname[%s] newname[%s]" ,
		m_role->GetUID(), m_role->GetName(), m_param.cur_on_use_pet_id, old_name, new_name);
}

void Pet::UplevelCurPet()
{
	if (INVALID_PET_ID == m_param.cur_on_use_pet_id || m_param.own_pet_list[m_param.cur_on_use_pet_id].level <= 0)
	{
		m_role->NoticeNum(errornum::EN_PET_HAS_NOT_ACTIVE);
		return;
	}

	if (m_param.own_pet_list[m_param.cur_on_use_pet_id].level >= PET_MAX_LEVEL_LIMIT)
	{
		m_role->NoticeNum(errornum::EN_PET_LEVEL_LIMIT);
		return;
	}

	if (m_role->GetSublock()->CheckHasLock())
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	// 消耗
	const PetLevelConfig *level_cfg = LOGIC_CONFIG->GetPetCfg().GetPetLevelCfg(m_param.cur_on_use_pet_id, m_param.own_pet_list[m_param.cur_on_use_pet_id].level);
	if (NULL == level_cfg)
	{
		return;
	}

	Money *money = knapsack->GetMoney();
	if (!money->UseAllCoin(level_cfg->uplevel_consume_coin, "UplevelCurPet")) 
	{
		m_role->NoticeNum(errornum::EN_COIN_NOT_ENOUGH);
		return;
	}

	// 生效
	++ m_param.own_pet_list[m_param.cur_on_use_pet_id].level;

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_PET);

	this->SendPetInfo();
	this->BroadcastPetInfo();

	// 日志
	gamelog::g_log_pet.printf(LL_INFO, "Pet::UplevelCurPet user[%d %s] petid[%d] consume_coin[%d] petlevel[%d] ",
		m_role->GetUID(), m_role->GetName(), m_param.cur_on_use_pet_id, level_cfg->uplevel_consume_coin, m_param.own_pet_list[m_param.cur_on_use_pet_id].level);
}

bool Pet::UpgradeCurPet(int is_auto_buy)
{
	int pet_id = m_param.cur_on_use_pet_id;
	if (INVALID_PET_ID == pet_id || m_param.own_pet_list[pet_id].level <= 0)
	{
		m_role->NoticeNum(errornum::EN_PET_HAS_NOT_ACTIVE);
		return false;
	}

	int max_grade = LOGIC_CONFIG->GetPetCfg().GetPetMaxGrade();
	if (m_param.own_pet_list[pet_id].grade >= PET_MAX_GRADE_LIMIT && m_param.own_pet_list[pet_id].grade >= max_grade)
	{
		m_role->NoticeNum(errornum::EN_PET_GRADE_LIMIT);
		return false;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	const PetGradeConfig *grade_cfg = LOGIC_CONFIG->GetPetCfg().GetPetGradeCfg(pet_id, m_param.own_pet_list[pet_id].grade);
	if (NULL == grade_cfg)
	{
		return false;
	}

	const PetGradeConfig *next_grade_cfg = LOGIC_CONFIG->GetPetCfg().GetPetGradeCfg(pet_id, m_param.own_pet_list[pet_id].grade + 1);
	if (NULL == next_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_PET_GRADE_LIMIT);
		return false;
	}

	const PetUpGradeRateConfig *upgrade_rate_cfg = LOGIC_CONFIG->GetPetCfg().GetSuccRate(m_param.own_pet_list[pet_id].grade, m_param.own_pet_list[pet_id].bless);
	if (NULL == upgrade_rate_cfg)
	{
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	{
		// 获取可消耗的背包材料列表

		short stuff_count = 0;

		//材料
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = grade_cfg->consume_stuff_id;
		stuff_list[stuff_count].num = grade_cfg->consume_stuff_num;
		stuff_list[stuff_count].buyable = (0 == is_auto_buy) ? false : true;

		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "Pet::UpgradeCurPet"))
	{
		return false;
	}

	{
		// 生效
		int rand_val = RandomNum(10000);
		if (rand_val < upgrade_rate_cfg->succ_rate)
		{
			//进阶成功
			m_param.own_pet_list[pet_id].bless = 0;
			++ m_param.own_pet_list[pet_id].grade;

			this->BroadcastPetInfo();

			m_role->NoticeNum(noticenum::NT_PET_UPGRADE_SUCC);
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_PET);

			//传闻
			if (next_grade_cfg->is_broadcast)
			{
				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pet_upgrade,
					UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), m_param.own_pet_list[pet_id].grade);

				if (len > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
		else
		{
			m_param.own_pet_list[pet_id].bless += upgrade_rate_cfg->GetRandomZhufuOnFail();

			if (m_param.own_pet_list[pet_id].bless > grade_cfg->max_bless_value)
			{
				m_param.own_pet_list[pet_id].bless = grade_cfg->max_bless_value;
			}
		}
	}

	this->SendPetInfo();

	//日志
	gamelog::g_log_pet.printf(LL_INFO, "Pet::UpgradeCurPet user[%d %s] is_auto_buy[%d] cur_grade[%d] cur_bless[%d] pet_id[%d]",
		m_role->GetUID(), m_role->GetName(), is_auto_buy, m_param.own_pet_list[pet_id].grade, m_param.own_pet_list[pet_id].bless, pet_id);

	return true;
}

void Pet::ChouPet(int type, short is_gold_first)
{
	if (type < PET_CHOU_TYPE_LOW || type >= PET_CHOU_TYPE_MAX)
	{
		return;
	}

	//检查消耗
	const PetOtherConfig &other_cfg = LOGIC_CONFIG->GetPetCfg().GetOtherCfg();

	int consume_gold = 0;
	ItemID consume_item_id = ItemBase::INVALID_ITEM_ID;

	int all_repeat_times = 1;
	bool is_free_chou = false;
	const int PET_CHOU_TYPE_HIGH_TIMES = 10;


	unsigned int nowtime = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (PET_CHOU_TYPE_LOW == type)
	{
		// 当前时间
		unsigned int cooldown_interval = other_cfg.free_chou_cold_time_hour * 60 * 60;

		if (nowtime - m_param.last_free_chou_timestamp > cooldown_interval)
		{	
			// 免费
			is_free_chou = true;
		}
		else
		{
			consume_gold = other_cfg.low_chou_consume_gold;
			consume_item_id = other_cfg.low_chou_consume_item_id;
		}
	}
	else if (PET_CHOU_TYPE_HIGH == type)
	{
		all_repeat_times = PET_CHOU_TYPE_HIGH_TIMES;
		consume_gold = other_cfg.high_chou_consume_gold;
		consume_item_id = other_cfg.high_chou_consume_item_id;
	}
	else
	{
		return;
	}

	if ((int)m_empty_grid_index_list.size() < all_repeat_times)
	{
		m_role->NoticeNum(errornum::EN_PET_BACKPACK_FULL);
		return;
	}

	// 保存抽奖参数
	PetRewardConfig reward_list[PET_CHOU_TYPE_HIGH_TIMES];

	for (int index = 0; index < all_repeat_times; ++ index)
	{
		const PetRewardConfig *reward_cfg = LOGIC_CONFIG->GetPetCfg().GetRandomRewardCfg(type);
		if (NULL == reward_cfg)
		{
			return;
		}

		PetRewardConfig &cur_reward = reward_list[index];
		cur_reward.seq = reward_cfg->seq;
		cur_reward.is_get_reward = reward_cfg->is_get_reward;

		cur_reward.reward_item.item_id = reward_cfg->reward_item.item_id;
		cur_reward.reward_item.num = reward_cfg->reward_item.num;
		cur_reward.reward_item.is_bind = reward_cfg->reward_item.is_bind;

		cur_reward.height_weight = reward_cfg->height_weight;
		cur_reward.low_weight = reward_cfg->low_weight;

		cur_reward.is_broadcast = reward_cfg->is_broadcast;
	}

	gamelog::g_log_pet.buff_printf("Pet::ChouPet user[%d %s] type[%d] is_gold_first[%d], consume_gold[%d], last_free_chou_timestamp[%d], item list[",
		m_role->GetUID(), m_role->GetName(), type, is_gold_first, consume_gold, m_param.last_free_chou_timestamp);

	bool is_result_bind = true; // 抽到的物品是否绑定

	if (is_free_chou)
	{
		m_param.last_free_chou_timestamp = nowtime;
	}
	else
	{
		//消耗
		if (!m_role->GetKnapsack()->ConsumeItem(consume_item_id, 1, "pet_ChouPet"))
		{
			if (is_gold_first)
			{
				if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(consume_gold))
				{
					m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}

				is_result_bind = false;

				m_role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "pet_ChouPet");
			}
			else
			{
				if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(consume_gold))
				{
					m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}

				if (m_role->GetKnapsack()->GetMoney()->GetGoldBind() <= 0) // 不消耗任何绑定元宝的情况下，奖励不绑定
				{
					is_result_bind = false;
				}

				m_role->GetKnapsack()->GetMoney()->UseAllGold(consume_gold, "pet_ChouPet");
			}
		}
	}

	//发放奖励
	static Protocol::SCPetChouResult chou_result;
	chou_result.reward_list_count = 0;

	for(int cur_repeat_time = 0; cur_repeat_time < all_repeat_times; ++ cur_repeat_time)
	{
		if (chou_result.reward_list_count < Protocol::SC_CHOU_PET_MAX_TIMES)
		{
			chou_result.all_reward_index_list[chou_result.reward_list_count ++] = reward_list[cur_repeat_time].seq;
		}

		if (0 == reward_list[cur_repeat_time].is_get_reward) //抽不到奖品
		{
			m_role->NoticeNum(errornum::EN_PET_NULL_REWARD);
		}
		else
		{
			bool has_put_into_store = false;
			for (int i = 0; i < PET_MAX_STORE_COUNT; ++ i)
			{
				bool cur_is_bind = (m_param.backpack_item_list[i].is_bind == 1);
				if (m_param.backpack_item_list[i].id != reward_list[cur_repeat_time].reward_item.item_id ||
					cur_is_bind != is_result_bind)
				{
					continue;
				}

				const ItemBase *item_base = ITEMPOOL->GetItem(m_param.backpack_item_list[i].id);
				if (NULL == item_base)
				{
					continue;
				}

				int item_total_num = m_param.backpack_item_list[i].num + reward_list[cur_repeat_time].reward_item.num;
				if (item_total_num <= item_base->GetPileLimit())
				{
					m_param.backpack_item_list[i].num = item_total_num;
					has_put_into_store = true;
					break;
				}
			}

			if (!has_put_into_store)
			{
				int index = m_empty_grid_index_list.top();
				if (index < 0 || index >= PET_MAX_STORE_COUNT)
				{
					continue;
				}
				m_param.backpack_item_list[index].id = reward_list[cur_repeat_time].reward_item.item_id;
				m_param.backpack_item_list[index].num += reward_list[cur_repeat_time].reward_item.num;
				m_param.backpack_item_list[index].is_bind = is_result_bind;

				m_empty_grid_index_list.pop();
			}
		}

		//传闻
		if (reward_list[cur_repeat_time].is_broadcast)
		{
			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pet_reward,
				UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), reward_list[cur_repeat_time].reward_item.item_id);

			if (len > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
			}
		}

		//日志
		gamelog::g_log_pet.buff_printf("(%d,%d,%d,%d),"
			,reward_list[cur_repeat_time].reward_item.item_id, reward_list[cur_repeat_time].reward_item.num, reward_list[cur_repeat_time].reward_item.is_bind, reward_list[cur_repeat_time].is_get_reward);
	}

	gamelog::g_log_pet.buff_printf("]");
	gamelog::g_log_pet.commit_buff(LL_INFO);

	ROLE_LOG_QUICK10(LOG_TYPE_PET, m_role, 0, type, NULL, NULL, is_gold_first, consume_gold, is_free_chou, is_result_bind);

	if (chou_result.reward_list_count > 0) // 有抽到奖励才发
	{
		this->SendPetBackpackList();
		int len = sizeof(chou_result) - (Protocol::SC_CHOU_PET_MAX_TIMES - chou_result.reward_list_count) * sizeof(chou_result.all_reward_index_list[0]); 
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&chou_result, len);
	}
}

void Pet::RecycleEgg(ItemID egg_id)
{
	if (NULL == ITEMPOOL->GetItem(egg_id))
	{
		return;
	}

	if (!m_role->GetKnapsack()->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	const PetEggConfig *egg_cfg = LOGIC_CONFIG->GetPetCfg().GetPetEggCfgByItemid(egg_id);
	if (NULL == egg_cfg)
	{
		return;
	}

	ItemConfigData item_data;
	item_data.item_id = egg_cfg->transform_item_id;
	item_data.num = egg_cfg->transform_item_num;
	item_data.is_bind = true;

	// 消耗
	if (!m_role->GetKnapsack()->ConsumeItem(egg_id, 1, "Pet::RecycleEgg"))
	{
		return;
	}

	// 给予
	m_role->GetKnapsack()->Put(item_data, PUT_REASON_PET_RECYCLE);

	gamelog::g_log_pet.printf(LL_INFO, "Pet::RecycleEgg user[%d %s] eggid[%d] item_id[%d] num[%d] is_bind[%d]",
		m_role->GetUID(), m_role->GetName(), egg_id, item_data.item_id, item_data.num, item_data.is_bind? 1 : 0);

}

void Pet::SendPetInfo()
{
	static Protocol::SCPetInfo petinfo;

	petinfo.cur_on_use_pet_id = m_param.cur_on_use_pet_id;
	petinfo.special_img_active_flag = m_param.special_img_active_flag;                    
	memcpy(petinfo.own_pet_list, m_param.own_pet_list, sizeof(petinfo.own_pet_list));
	memcpy(petinfo.special_img_level_list, m_param.special_img_level_list, sizeof(petinfo.special_img_level_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&petinfo, sizeof(petinfo));
}

void Pet::SendPetBackpackList()
{
	static Protocol::SCPetBackpackInfo petbpinfo;
	petbpinfo.last_free_chou_timestamp = m_param.last_free_chou_timestamp;
	memcpy(petbpinfo.backpack_item_list, m_param.backpack_item_list, sizeof(petbpinfo.backpack_item_list));

	if (!m_param.has_played_food_market)
	{
		m_param.has_played_food_market = 1;
		m_param.food_market_free_times = LOGIC_CONFIG->GetPetCfg().GetOtherCfg().food_market_free_times_per_day;
	}
	petbpinfo.food_market_free_times = m_param.food_market_free_times;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&petbpinfo, sizeof(petbpinfo));
}

void Pet::BroadcastPetInfo()
{
	if (NULL != m_role->GetScene())
	{
		static Protocol::SCPetViewChangeNotify petviewinfo;
		petviewinfo.obj_id = m_role->GetId();
		petviewinfo.pet_id = m_param.cur_on_use_pet_id;
		petviewinfo.use_img_id  = m_param.use_img_id;
		if (m_param.cur_on_use_pet_id < 0 || m_param.cur_on_use_pet_id >= PET_MAX_COUNT_LIMIT)
		{
			petviewinfo.pet_level = 0;
			petviewinfo.pet_grade = 0;
			memset(petviewinfo.pet_name, 0, sizeof(GameName));
		}
		else
		{
			petviewinfo.pet_level = m_param.own_pet_list[m_param.cur_on_use_pet_id].level;
			petviewinfo.pet_grade = m_param.own_pet_list[m_param.cur_on_use_pet_id].grade;
			F_STRNCPY(petviewinfo.pet_name, m_param.own_pet_list[m_param.cur_on_use_pet_id].name, sizeof(GameName));
		}

		m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void*)&petviewinfo, sizeof(petviewinfo));
	}
}

void Pet::TransportRewardToKnapsack(int index, int is_fetch_all)
{
	if (is_fetch_all)
	{
		while (!m_empty_grid_index_list.empty())
		{
			m_empty_grid_index_list.pop();
		}

		gamelog::g_log_pet.buff_printf("OnPetFetch_reward::[Knapsack]::user[%d, %s], item_list[",
			m_role->GetUID(), m_role->GetName());

		ItemConfigData item_data;
		for (int store_index = PET_MAX_STORE_COUNT - 1; store_index >= 0 ; -- store_index)
		{
			PetStoreItem &store_item = m_param.backpack_item_list[store_index];
			if (ItemBase::INVALID_ITEM_ID != store_item.id && store_item.num > 0)
			{
				if (!m_role->GetKnapsack()->HaveSpace())
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
					continue;
				}

				// 背包有空间，直接发到背包
				item_data.item_id = store_item.id;
				item_data.num = store_item.num;
				item_data.is_bind = store_item.is_bind ? 1 : 0;

				if (m_role->GetKnapsack()->Put(item_data, PUT_REASON_PET_REWARD))
				{
					store_item.id = ItemBase::INVALID_ITEM_ID;
					store_item.num = 0;
					store_item.is_bind = 1;

					gamelog::g_log_pet.buff_printf("(%s) ", ItemConfigDataLog(&item_data, NULL));
				}
			}
			m_empty_grid_index_list.push(store_index);
		}

		gamelog::g_log_pet.buff_printf("]");
		gamelog::g_log_pet.commit_buff(LL_INFO);
	}
	else	//单个领取
	{
		if (index < 0 || index >= PET_MAX_STORE_COUNT)
		{
			return;
		}

		PetStoreItem &store_item = m_param.backpack_item_list[index];
		if (ItemBase::INVALID_ITEM_ID == store_item.id || store_item.num <= 0)
		{
			return;
		}

		if (!m_role->GetKnapsack()->HaveSpace())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}

		// 背包有空间，直接发到背包
		ItemConfigData item_data; 
		item_data.item_id = store_item.id;
		item_data.num = store_item.num;
		item_data.is_bind = store_item.is_bind ? 1 : 0;

		if (m_role->GetKnapsack()->Put(item_data, PUT_REASON_PET_REWARD))
		{
			store_item.id = ItemBase::INVALID_ITEM_ID;
			store_item.num = 0;
			store_item.is_bind = 1;

			m_empty_grid_index_list.push(index);

			gamelog::g_log_pet.printf(LL_INFO, "OnPetFetch_reward::[Knapsack]::user[%d, %s], reward_item[%s]",
				m_role->GetUID(), m_role->GetName(), ItemConfigDataLog(&item_data, NULL));
		}
	}

	this->SendPetBackpackList();
}

void Pet::LearnSkill(int pet_skill_slot, int skill_id)
{
	int pet_id = m_param.cur_on_use_pet_id;
	if (INVALID_PET_ID == pet_id || m_param.own_pet_list[pet_id].level <= 0)
	{
		m_role->NoticeNum(errornum::EN_PET_HAS_NOT_ACTIVE);
		return;
	}

	const PetInfoConfig *cur_pet_info = LOGIC_CONFIG->GetPetCfg().GetPetInfo(pet_id);
	if (NULL == cur_pet_info)
	{
		return;
	}

	if (pet_skill_slot < PET_SKILL_SLOT_TYPE_ACTIVE || pet_skill_slot >= PET_SKILL_SLOT_TYPE_COUNT)
	{
		return;
	}

	int cur_pet_id = m_param.cur_on_use_pet_id;
	if (INVALID_PET_ID == cur_pet_id || m_param.own_pet_list[cur_pet_id].level <= 0)
	{
		m_role->NoticeNum(errornum::EN_PET_HAS_NOT_ACTIVE);
		return;
	}

	PetSkillItem &skill_item = m_param.own_pet_list[cur_pet_id].skill_list[pet_skill_slot];
	if (skill_item.skill_level > 0)
	{
		m_role->NoticeNum(errornum::EN_PET_SKILL_ALEADY_LEARN);
		return;
	}

	const PetSkillConfig *pet_skill = LOGIC_CONFIG->GetPetCfg().GetSkillWithId(skill_id);
	if (NULL == pet_skill)
	{
		return;
	}

	if (pet_skill->own_by_pet_id != INVALID_PET_ID && cur_pet_id != pet_skill->own_by_pet_id)
	{
		m_role->NoticeNum(errornum::EN_PET_CUR_PET_CANNOT_LEARN_THIS_SKILL);
		return;
	}

	const SkillBase *skill_base = SKILLPOOL->GetSkill(pet_skill->skill_id);
	if (NULL == skill_base)
	{
		return;
	}
	
	SkillType skill_type = skill_base->GetSkillType();

	// 检测技能类型跟槽类型匹配
	if (SkillBase::SKILL_TYPE_PASSIVE == skill_type && PET_SKILL_SLOT_TYPE_ACTIVE == pet_skill_slot)
	{
		m_role->NoticeNum(errornum::EN_PET_CUR_PET_THIS_SKILL_NOT_MATCH_SLOT);
		return;
	}
	
	if (SkillBase::SKILL_TYPE_ACTIVE == skill_type && (PET_SKILL_SLOT_TYPE_PASSIVE_1 == pet_skill_slot || PET_SKILL_SLOT_TYPE_PASSIVE_2 == pet_skill_slot))
	{
		m_role->NoticeNum(errornum::EN_PET_CUR_PET_THIS_SKILL_NOT_MATCH_SLOT);
		return;
	}

	int cur_active_skill_num = 0;
	// 检测技能数量限制
	if (SkillBase::SKILL_TYPE_PASSIVE == skill_type)
	{
		cur_active_skill_num = pet_skill_slot;
		if (cur_active_skill_num > cur_pet_info->passive_skill_num)
		{
			m_role->NoticeNum(errornum::EN_PET_CUR_PET_HAS_NO_THIS_SKILL_SLOT);
			return;
		}
	}
	else if (SkillBase::SKILL_TYPE_ACTIVE == skill_type)
	{
		cur_active_skill_num = pet_skill_slot + 1;
		if (cur_active_skill_num > cur_pet_info->active_skill_num)
		{
			m_role->NoticeNum(errornum::EN_PET_CUR_PET_HAS_NO_THIS_SKILL_SLOT);
			return;
		}
	}
		

	// 检测是否已装备了此技能
	for (int skill_slot_index = PET_SKILL_SLOT_TYPE_ACTIVE; skill_slot_index < PET_SKILL_SLOT_TYPE_COUNT; ++ skill_slot_index)
	{
		PetSkillItem &skill_item = m_param.own_pet_list[cur_pet_id].skill_list[skill_slot_index];
		if (skill_item.skill_level > 0 && pet_skill->skill_index == skill_item.skill_index)
		{
			m_role->NoticeNum(errornum::EN_PET_CUR_PET_THIS_SKILL_ARE_IN_USED);
			return;
		}
	}

	int target_skill_level = 1;
	int stuff_count = 0;

	//材料
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[stuff_count].item_id = pet_skill->skill_level_cfglist[target_skill_level].consume_item_id;
	stuff_list[stuff_count].num = pet_skill->skill_level_cfglist[target_skill_level].consume_item_num;
	stuff_list[stuff_count].buyable = false;
	++ stuff_count;

	// 检查消耗
	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
	{
		return;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "Pet::LearnSkill"))
	{
		return;
	}

	skill_item.skill_index = pet_skill->skill_index;
	skill_item.skill_level = target_skill_level;

	this->SendPetInfo();

	m_role->GetSkillManager()->RemoveSkill(pet_skill->skill_id);
	m_role->GetSkillManager()->AddPetSkill(pet_skill->skill_id, skill_item.skill_level);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SKILL);
	m_role->NoticeNum(noticenum::NT_PET_SKILL_LEARNT_SUCC);

	// 日志
	gamelog::g_log_pet.printf(LL_INFO, "Pet::UpgradeSkill user[%d %s] skill_slot[%d] skill_index[%d] skill_id[%d] new_level[%d]",
		m_role->GetUID(), m_role->GetName(), pet_skill_slot, pet_skill->skill_index, pet_skill->skill_id, skill_item.skill_level);
}

void Pet::UpgradeSkill(int pet_skill_slot)
{
	if (pet_skill_slot < PET_SKILL_SLOT_TYPE_ACTIVE || pet_skill_slot >= PET_SKILL_SLOT_TYPE_COUNT)
	{
		return;
	}

	int cur_pet_id = m_param.cur_on_use_pet_id;
	if (INVALID_PET_ID == cur_pet_id || m_param.own_pet_list[cur_pet_id].level <= 0)
	{
		m_role->NoticeNum(errornum::EN_PET_HAS_NOT_ACTIVE);
		return;
	}

	PetSkillItem &skill_item = m_param.own_pet_list[cur_pet_id].skill_list[pet_skill_slot];
	if (skill_item.skill_level <= 0)
	{
		m_role->NoticeNum(errornum::EN_PET_SKILL_HAS_NOT_LEARN);
		return;
	}

	const PetSkillConfig *pet_skill = LOGIC_CONFIG->GetPetCfg().GetSkillWithIndex(skill_item.skill_index);
	if (NULL == pet_skill)
	{
		return;
	}

	if (pet_skill->own_by_pet_id != INVALID_PET_ID && cur_pet_id != pet_skill->own_by_pet_id)
	{
		m_role->NoticeNum(errornum::EN_PET_CUR_PET_CANNOT_LEARN_THIS_SKILL);
		return;
	}

	if (skill_item.skill_level >= pet_skill->max_skill_level)
	{
		m_role->NoticeNum(errornum::EN_PET_CUR_PET_THIS_SKILL_HAS_MAX_LEVEL);
		return;
	}

	int target_skill_level = skill_item.skill_level + 1;

	//材料
	int stuff_count = 0;
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
	stuff_list[stuff_count].item_id = pet_skill->skill_level_cfglist[target_skill_level].consume_item_id;
	stuff_list[stuff_count].num = pet_skill->skill_level_cfglist[target_skill_level].consume_item_num;
	stuff_list[stuff_count].buyable = false;
	++ stuff_count;

	// 检查消耗
	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
	{
		return;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "Pet::UpgradeSkill"))
	{
		return;
	}

	int old_skill_level = skill_item.skill_level;

	skill_item.skill_level = target_skill_level;

	this->SendPetInfo();

	//m_role->GetSkillManager()->RemoveSkill(pet_skill->skill_id);
	m_role->GetSkillManager()->AddPetSkill(pet_skill->skill_id, skill_item.skill_level);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SKILL);
	m_role->NoticeNum(noticenum::NT_PET_SKILL_UPGRADE_SUCC);

	// 日志
	gamelog::g_log_pet.printf(LL_INFO, "Pet::UpgradeSkill user[%d %s] skill_slot[%d] skill_index[%d] skill_id[%d] old_level[%d] new_level[%d]",
		m_role->GetUID(), m_role->GetName(), pet_skill_slot, pet_skill->skill_index, pet_skill->skill_id, old_skill_level, skill_item.skill_level);
}

void Pet::ForgetSkill(int pet_skill_slot)
{
	if (pet_skill_slot < PET_SKILL_SLOT_TYPE_ACTIVE || pet_skill_slot >= PET_SKILL_SLOT_TYPE_COUNT)
	{
		return;
	}

	int cur_pet_id = m_param.cur_on_use_pet_id;
	if (INVALID_PET_ID == cur_pet_id || m_param.own_pet_list[cur_pet_id].level <= 0)
	{
		m_role->NoticeNum(errornum::EN_PET_HAS_NOT_ACTIVE);
		return;
	}

	PetSkillItem &skill_item = m_param.own_pet_list[cur_pet_id].skill_list[pet_skill_slot];
	if (skill_item.skill_level <= 0)
	{
		m_role->NoticeNum(errornum::EN_PET_SKILL_HAS_NOT_LEARN);
		return;
	}
	
	const PetSkillConfig *pet_skill = LOGIC_CONFIG->GetPetCfg().GetSkillWithIndex(skill_item.skill_index);
	if (NULL == pet_skill)
	{
		return;
	}

	m_role->GetSkillManager()->RemoveSkill(pet_skill->skill_id);

	int old_skill_level = skill_item.skill_level;

	skill_item.skill_index = 0;
	skill_item.skill_level = 0;
	this->SendPetInfo();

	m_role->NoticeNum(noticenum::NT_PET_SKILL_FORGET_SUCC);
	
	// 日志
	gamelog::g_log_pet.printf(LL_INFO, "Pet::ForgetSkill user[%d %s] skill_slot[%d] skill_index[%d] skill_id[%d] skill_level[%d]",
		m_role->GetUID(), m_role->GetName(), pet_skill_slot, pet_skill->skill_index, pet_skill->skill_id, old_skill_level);
}

void Pet::GetPetInfo(int *use_pet_id, short *pet_level, short *pet_grade, GameName pet_name, short *user_pet_special_img)
{
	if (NULL != use_pet_id) *use_pet_id = INVALID_PET_ID;
	if (NULL != pet_level) *pet_level = 0;
	if (NULL != pet_grade) *pet_grade = 0;
	if (NULL != user_pet_special_img) *user_pet_special_img = 0;
	if (NULL != pet_name) memset(pet_name, 0, sizeof(GameName));

	if (m_param.cur_on_use_pet_id < 0 || m_param.cur_on_use_pet_id >= PET_MAX_COUNT_LIMIT)
	{
		return;
	}

	if (NULL != use_pet_id)
	{
		*use_pet_id = m_param.cur_on_use_pet_id;
	}

	if (NULL != pet_level)
	{
		*pet_level = m_param.own_pet_list[m_param.cur_on_use_pet_id].level;
	}

	if (NULL != pet_grade)
	{
		*pet_grade = m_param.own_pet_list[m_param.cur_on_use_pet_id].grade;
	}

	if (NULL != pet_name)
	{
		F_STRNCPY(pet_name, m_param.own_pet_list[m_param.cur_on_use_pet_id].name, sizeof(GameName));
	}

	if (NULL != user_pet_special_img)
	{
		*user_pet_special_img = m_param.use_img_id;
	}
}


short Pet::GetCurPetGrade()
{
	if (m_param.cur_on_use_pet_id >= 0 && m_param.cur_on_use_pet_id < PET_MAX_COUNT_LIMIT)
	{
		return m_param.own_pet_list[m_param.cur_on_use_pet_id].grade;
	}

	return 0;
}

void Pet::CheckIsActiveSuit(int cur_active_pet_id)
{
	//检查 是否集齐了一套宠物 发送传闻

	std::set<int> cur_own_pet_id_set;
	for (int pet_id = 0; pet_id < PET_MAX_COUNT_LIMIT; ++ pet_id)
	{
		if (m_param.own_pet_list[pet_id].level > 0)
		{
			cur_own_pet_id_set.insert(pet_id);
		}
	}

	int extra_attr_count = LOGIC_CONFIG->GetPetCfg().GetExtraAttrCount();
	for (int index = 0; index < extra_attr_count; ++ index)
	{
		const PetExtraAttrConfig *extra_attr_cfg = LOGIC_CONFIG->GetPetCfg().GetExtraAttrPetList(index);
		if (NULL != extra_attr_cfg)
		{
			bool cur_suit_active = true; // 当前套装全部激活
			bool contain_cur_active = false; // 当前套装包含刚激活的宠物ID

			for (int i = 0; i < PET_MAX_EXTRA_ATTR_PET_COUNT_LIMIT; ++ i)
			{
				int temp_pet_id = extra_attr_cfg->pet_id_list[i];

				if (INVALID_PET_ID == temp_pet_id) continue;

				if (cur_own_pet_id_set.find(temp_pet_id) == cur_own_pet_id_set.end())
				{
					cur_suit_active = false;
					break;
				}

				if (temp_pet_id == cur_active_pet_id)
				{
					contain_cur_active = true;
				}
			}

			if (cur_suit_active && contain_cur_active)
			{
				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_pet_active_suit,
					UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), extra_attr_cfg->seq, extra_attr_cfg->seq);

				if (len > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
				}
			}
		}
	}
}

void Pet::ChangePetSkill(int old_use_pet_id, int cur_on_use_pet_id)
{
	if (old_use_pet_id < 0 || old_use_pet_id >= PET_MAX_COUNT_LIMIT || cur_on_use_pet_id < 0 || cur_on_use_pet_id >= PET_MAX_COUNT_LIMIT)
	{
		return;
	}

	PetParamItem &old_pet = m_param.own_pet_list[old_use_pet_id];
	PetParamItem &cur_pet = m_param.own_pet_list[cur_on_use_pet_id];

	for (int skill_slot_index = PET_SKILL_SLOT_TYPE_ACTIVE; skill_slot_index < PET_SKILL_SLOT_TYPE_COUNT; ++ skill_slot_index)
	{
		if (old_pet.level > 0) // 删除旧宠物技能
		{
			PetSkillItem &old_skill = old_pet.skill_list[skill_slot_index];
			if (old_skill.skill_level > 0 && old_skill.skill_index >= 0)
			{
				const PetSkillConfig *old_pet_skill_cfg = LOGIC_CONFIG->GetPetCfg().GetSkillWithIndex(old_skill.skill_index);
				if (NULL != old_pet_skill_cfg)
				{
					m_role->GetSkillManager()->RemoveSkill(old_pet_skill_cfg->skill_id);
				}
			}
		}
	}

	for (int skill_slot_index = PET_SKILL_SLOT_TYPE_ACTIVE; skill_slot_index < PET_SKILL_SLOT_TYPE_COUNT; ++ skill_slot_index)
	{
		if (cur_pet.level > 0) // 增加新宠物技能
		{
			PetSkillItem &cur_skill = cur_pet.skill_list[skill_slot_index];
			if (cur_skill.skill_level > 0 && cur_skill.skill_index >= 0)
			{
				const PetSkillConfig *new_pet_skill_cfg = LOGIC_CONFIG->GetPetCfg().GetSkillWithIndex(cur_skill.skill_index);
				if (NULL != new_pet_skill_cfg)
				{
					m_role->GetSkillManager()->AddPetSkill(new_pet_skill_cfg->skill_id, cur_skill.skill_level);
				}
			}
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SKILL);
}

bool Pet::ActiveSpecialImage(int imageid)
{
	if (imageid <= 0 || imageid > PET_SPECIAL_IMG_MAX_ID_LIMIT )
	{
		return false;
	}

	if (0 != (m_param.special_img_active_flag & (1 << imageid)))
	{
		m_role->NoticeNum(errornum::EN_PET_SPECIAL_IMG_ALREADY_ACTIVE);
		return false;
	}

	const PetSpecialConfig *cfg_item = LOGIC_CONFIG->GetPetCfg().GetSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}

	m_param.special_img_active_flag |= (1 << imageid);
	this->SendPetInfo();
	PetPrint("ActiveSpecialImage", imageid);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_PET);

	this->UseImage(imageid);

	return true;
}

bool Pet::UseImage(int imageid)
{
	if (imageid < 0 || imageid > PET_SPECIAL_IMG_MAX_ID_LIMIT )
	{
		return false;
	}

	if (0 != imageid)
	{
		if (0 == (m_param.special_img_active_flag & (1 << imageid)))
		{
			m_role->NoticeNum(errornum::EN_PET_SPECIAL_IMG_NOT_ACTIVE);
			return false;
		}
	}

	m_param.use_img_id = imageid;

	this->BroadcastPetInfo();
	PetPrint("UseImage", imageid);
	return true;
}

bool Pet::UplevelSpecialImage(const int imageid)
{
	if (imageid <= 0 || imageid > PET_SPECIAL_IMG_MAX_ID_LIMIT)
	{
		return false;
	}

	if (0 == (m_param.special_img_active_flag & (1 << imageid)))
	{
		m_role->NoticeNum(errornum::EN_PET_SPECIAL_IMG_NOT_ACTIVE);
		return false;
	}

	const int old_level = (int)m_param.special_img_level_list[imageid];
	const PetSpecialImageUplevelConfig::UplevelItemConfig *uplevel_cfg = LOGIC_CONFIG->GetPetCfg().GetSpecialImgUplevelCfg(imageid, old_level + 1);
	if (NULL == uplevel_cfg)
	{
		return false;
	}
	
	if (!m_role->GetKnapsack()->ConsumeItem(uplevel_cfg->item_id, uplevel_cfg->item_num, "UplevelPetSpecialImage"))
	{
		m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
		return false;
	}

	++ m_param.special_img_level_list[imageid];

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_PET);
	this->SendPetInfo();

	ItemConfigData item_to_log(uplevel_cfg->item_id, false, uplevel_cfg->item_num);

	gamelog::g_log_pet.printf(LL_INFO, "Pet::UplevelSpecialImage user[%d %s] old_level[%d] now_level[%d] consumeItem[%s]",
		m_role->GetUID(), m_role->GetName(), old_level, (int)m_param.special_img_level_list[imageid], ItemConfigDataLog(&item_to_log, NULL));

	return true;
}

bool Pet::PetQinmiPromote(int food_index, int use_pet_id)
{
	// 检查
	if (use_pet_id < 0 || use_pet_id >= PET_MAX_COUNT_LIMIT || m_param.own_pet_list[use_pet_id].level <= 0)
	{
		m_role->NoticeNum(errornum::EN_PET_HAS_NOT_ACTIVE);
		return false;
	}

	if (m_param.pet_qinmi_list[use_pet_id].level >= LOGIC_CONFIG->GetPetCfg().GetMaxQinmiLevel())
	{
		m_role->NoticeNum(errornum::EN_PET_QINMI_LEVEL_FULL);
		return false;
	}

	const PetFoodMarketConfig *food_cfg = LOGIC_CONFIG->GetPetCfg().GetFoodMarketCfg(LOGIC_CONFIG->GetPetCfg().GetPetFoodSeq(food_index));
	if (NULL == food_cfg)
	{
		return false;
	}

	// 模拟给予
	bool is_level_up = false;
	bool is_star_up = false;
	PetQinmiParamItem &qinmi_item = m_param.pet_qinmi_list[use_pet_id];
	int qinmi_val_tmp = qinmi_item.qinmi_val + food_cfg->add_qinmi_val;
	PetQinmiParamItem qinmi_item_tmp = qinmi_item;

	{
		const PetQinmiLevelConfig::StarConfig *star_cfg = LOGIC_CONFIG->GetPetCfg().GetQinmiLevelStarCfg(qinmi_item_tmp.level, qinmi_item_tmp.star);
		if (NULL == star_cfg)
		{
			return false;
		}

		int loops = 0;
		while (qinmi_val_tmp >= star_cfg->need_qinmi_val && loops ++ < 100)
		{
			qinmi_val_tmp -= star_cfg->need_qinmi_val;

			++ qinmi_item_tmp.star;
			is_star_up = true;

			if (qinmi_item_tmp.star > LOGIC_CONFIG->GetPetCfg().GetPetQinmiLevelMaxStar(qinmi_item_tmp.level))
			{
				++ qinmi_item_tmp.level;
				qinmi_item_tmp.star = 1;

				is_level_up = true;

				if (qinmi_item_tmp.level >= LOGIC_CONFIG->GetPetCfg().GetMaxQinmiLevel())
				{
					qinmi_val_tmp = 0;
					break;
				}
			}

			star_cfg = LOGIC_CONFIG->GetPetCfg().GetQinmiLevelStarCfg(qinmi_item_tmp.level, qinmi_item_tmp.star);
			if (NULL == star_cfg)
			{
				return false;
			}
		}
	}

	// 消耗
	if (!m_role->GetKnapsack()->ConsumeItem(food_cfg->reward_item.item_id, 1, "Pet::FeedCurPet"))
	{
		m_role->NoticeNum(errornum::EN_PET_QINMI_STUFF_ID_NOT_ENOUGH);
		return false;
	}

	// 给予
	qinmi_item = qinmi_item_tmp;
	qinmi_item.qinmi_val = qinmi_val_tmp;

	if (is_star_up)
	{
		m_role->NoticeNum(noticenum::NT_PET_QINMI_UPSTAR_SUCC);
	}

	// 重算属性和破防
	if (is_level_up)
	{
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_PET);
		this->ReCalcIgnoreFangyuPercent();
		m_role->NoticeNum(noticenum::NT_PET_QINMI_UPLEVEL_SUCC);
	}

	// 发送
	this->SendPetQinmiSingleInfo(use_pet_id);

	// 日志
	gamelog::g_log_pet.printf(LL_INFO, "Pet::PetQinmiPromote user[%d %s], use_pet_id[%d], food_index[%d], item_id[%d], qinmi_val[%d], star[%d], level[%d]", 
		m_role->GetUID(), m_role->GetName(), use_pet_id, food_index, food_cfg->reward_item.item_id, qinmi_item.qinmi_val, qinmi_item.star, qinmi_item.level);

	return true;
}

void Pet::PetQinmiAutoPromote(int cur_pet_id)
{
	// 检查
	if (cur_pet_id < 0 || cur_pet_id >= PET_MAX_COUNT_LIMIT || m_param.own_pet_list[cur_pet_id].level <= 0)
	{
		m_role->NoticeNum(errornum::EN_PET_HAS_NOT_ACTIVE);
		return;
	}

	if (m_param.pet_qinmi_list[cur_pet_id].level >= LOGIC_CONFIG->GetPetCfg().GetMaxQinmiLevel())
	{
		m_role->NoticeNum(errornum::EN_PET_QINMI_LEVEL_FULL);
		return;
	}

	PetQinmiParamItem &qinmi_item = m_param.pet_qinmi_list[cur_pet_id];
	const PetFoodMarketConfig *food_cfg = NULL;
	const PetQinmiLevelConfig::StarConfig *star_cfg = NULL;

	// 获取消耗列表，以及检查是否可以升星
	int need_food_count_list[PET_FOOD_MARKET_MAX_FOOD_COUNT]; memset(need_food_count_list, 0, sizeof(need_food_count_list));
	bool is_up_star = false;

	{
		// 获取拥有的食物数量列表
		int has_food_count_list[PET_FOOD_MARKET_MAX_FOOD_COUNT]; memset(has_food_count_list, 0, sizeof(has_food_count_list));
		bool has_food = false;

		for (int index = 0; index < PET_FOOD_MARKET_MAX_FOOD_COUNT; ++ index)
		{
			food_cfg = LOGIC_CONFIG->GetPetCfg().GetFoodMarketCfg(LOGIC_CONFIG->GetPetCfg().GetPetFoodSeq(index));
			if (NULL == food_cfg)
			{
				return;
			}

			int has_food_count = m_role->GetKnapsack()->Count(food_cfg->reward_item.item_id);
			if (has_food_count > 0)
			{
				has_food = true;
			}
			has_food_count_list[index] = has_food_count;
		}

		if (!has_food)
		{
			m_role->NoticeNum(errornum::EN_PET_QINMI_STUFF_ID_NOT_ENOUGH);
			return;
		}

		// 获取实际消耗的食物数量列表
		star_cfg = LOGIC_CONFIG->GetPetCfg().GetQinmiLevelStarCfg(qinmi_item.level, qinmi_item.star);
		if (NULL == star_cfg)
		{
			return;
		}

		int need_qinmi_val = star_cfg->need_qinmi_val - qinmi_item.qinmi_val;
		for (int index = 0; index < PET_FOOD_MARKET_MAX_FOOD_COUNT && need_qinmi_val > 0; ++ index)
		{
			food_cfg = LOGIC_CONFIG->GetPetCfg().GetFoodMarketCfg(LOGIC_CONFIG->GetPetCfg().GetPetFoodSeq(index));
			if (NULL == food_cfg || food_cfg->add_qinmi_val <= 0)	// 配置那里已经判断add_qinmi_val不可能等于0，以防万一
			{
				return;
			}

			int need_food_count = need_qinmi_val / food_cfg->add_qinmi_val;
			if (0 != need_qinmi_val % food_cfg->add_qinmi_val)
			{
				++ need_food_count;
			}

			int has_food_count = has_food_count_list[index];
			if (has_food_count >= need_food_count)
			{
				need_food_count_list[index] = need_food_count;
				is_up_star = true;
				break;
			}
			else
			{
				need_qinmi_val -= has_food_count * food_cfg->add_qinmi_val;
				need_food_count_list[index] = has_food_count;
			}
		}
	}

	int qinmi_val_tmp = qinmi_item.qinmi_val;
	for (int index = 0; index < PET_FOOD_MARKET_MAX_FOOD_COUNT; ++ index)
	{
		food_cfg = LOGIC_CONFIG->GetPetCfg().GetFoodMarketCfg(LOGIC_CONFIG->GetPetCfg().GetPetFoodSeq(index));
		if (NULL == food_cfg)
		{
			return;
		}

		qinmi_val_tmp +=  food_cfg->add_qinmi_val * need_food_count_list[index];
	}
	PetQinmiParamItem qinmi_item_tmp = qinmi_item;
	bool is_level_up = false;

	if (is_up_star)
	{
		// 模拟给予
		{
			star_cfg = LOGIC_CONFIG->GetPetCfg().GetQinmiLevelStarCfg(qinmi_item_tmp.level, qinmi_item_tmp.star);
			if (NULL == star_cfg)
			{
				return;
			}

			int loops = 0;
			while (qinmi_val_tmp >= star_cfg->need_qinmi_val && loops ++ < 100)
			{
				qinmi_val_tmp -= star_cfg->need_qinmi_val;

				++ qinmi_item_tmp.star;

				if (qinmi_item_tmp.star > LOGIC_CONFIG->GetPetCfg().GetPetQinmiLevelMaxStar(qinmi_item_tmp.level))
				{
					++ qinmi_item_tmp.level;
					qinmi_item_tmp.star = 1;

					is_level_up = true;

					if (qinmi_item_tmp.level >= LOGIC_CONFIG->GetPetCfg().GetMaxQinmiLevel())
					{
						qinmi_val_tmp = 0;
						break;
					}
				}

				star_cfg = LOGIC_CONFIG->GetPetCfg().GetQinmiLevelStarCfg(qinmi_item_tmp.level, qinmi_item_tmp.star);
				if (NULL == star_cfg)
				{
					return;
				}
			}
		}
	}

	// 消耗
	for (int index = 0; index < PET_FOOD_MARKET_MAX_FOOD_COUNT; ++ index)
	{
		food_cfg = LOGIC_CONFIG->GetPetCfg().GetFoodMarketCfg(LOGIC_CONFIG->GetPetCfg().GetPetFoodSeq(index));
		if (NULL == food_cfg)
		{
			return;
		}

		if (!m_role->GetKnapsack()->ConsumeItem(food_cfg->reward_item.item_id, need_food_count_list[index], "Pet::PetQinmiAutoPromote"))
		{
			continue;
		}
	}

	// 给予
	if (is_up_star)
	{
		qinmi_item = qinmi_item_tmp;
		m_role->NoticeNum(noticenum::NT_PET_QINMI_UPSTAR_SUCC);

		// 重算属性和破防
		if (is_level_up)
		{
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_PET);
			this->ReCalcIgnoreFangyuPercent();
			m_role->NoticeNum(noticenum::NT_PET_QINMI_UPLEVEL_SUCC);
		}
	}
	qinmi_item.qinmi_val = qinmi_val_tmp;

	// 发送
	this->SendPetQinmiSingleInfo(cur_pet_id);

	// 日志
	gamelog::g_log_pet.buff_printf("Pet::PetQinmiAutoPromote user[%d %s], cur_pet_id[%d], is_up_star[%d], qinmi_val[%d], star[%d], level[%d], consume_item: [", 
		m_role->GetUID(), m_role->GetName(), cur_pet_id, is_up_star, qinmi_item.qinmi_val, qinmi_item.star, qinmi_item.level);

	for (int index = 0; index < PET_FOOD_MARKET_MAX_FOOD_COUNT; ++ index)
	{
		food_cfg = LOGIC_CONFIG->GetPetCfg().GetFoodMarketCfg(LOGIC_CONFIG->GetPetCfg().GetPetFoodSeq(index));
		if (NULL == food_cfg)
		{
			continue;
		}

		gamelog::g_log_pet.buff_printf("(food_index[%d], item_id[%d], count[%d])", index, food_cfg->reward_item.item_id, need_food_count_list[index]);
	}

	gamelog::g_log_pet.buff_printf("]");
	gamelog::g_log_pet.commit_buff(LL_INFO);
}

void Pet::FoodMarketChouOnce(bool is_gold_first)
{
	// 检查
	if ((int)m_empty_grid_index_list.size() <= 0)
	{
		m_role->NoticeNum(errornum::EN_PET_BACKPACK_FULL);
		return;
	}

	bool is_bind = true;
	bool is_use_gold = false;
	if (m_param.food_market_free_times <= 0)
	{
		is_use_gold = true;
		is_bind = false;
	}

	// 获取一个随机奖励
	const PetFoodMarketConfig *reward_cfg = LOGIC_CONFIG->GetPetCfg().GetFoodMarketOnceRandomRewardCfg();
	if (NULL == reward_cfg)
	{
		return;
	}

	// 消耗
	int need_gold = LOGIC_CONFIG->GetPetCfg().GetOtherCfg().food_market_once_need_gold;

	if (is_use_gold)
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "Pet::FoodMarketChouOnce")) 
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}
	else
	{
		-- m_param.food_market_free_times;
	}

	// 给予
	ItemConfigData reward_item = reward_cfg->reward_item;
	reward_item.is_bind = is_bind;
	this->Put(reward_item);

	// 发送
	this->SendPetBackpackList();

	static Protocol::SCPetFoodMarcketChouResult fmcr;
	fmcr.reward_seq_list[0] = reward_cfg->seq;
	fmcr.reward_list_count = 1;
	int send_len = sizeof(fmcr) - (Protocol::SC_CHOU_PET_FOOD_MARKET_MAX_TIMES - fmcr.reward_list_count) * sizeof(fmcr.reward_seq_list[0]);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&fmcr, send_len);

	// 日志
	gamelog::g_log_pet.printf(LL_INFO, "Pet::FoodMarketChouOnce user[%d %s], food_market_free_times[%d], need_gold[%d], reward_item[%s]", 
		m_role->GetUID(), m_role->GetName(), m_param.food_market_free_times, need_gold, ItemConfigDataLog(&reward_item, NULL));
}

void Pet::FoodMarketChouTimes(int times, bool is_gold_first)
{
	// 检查
	if ((int)m_empty_grid_index_list.size() <= times)
	{
		m_role->NoticeNum(errornum::EN_PET_BACKPACK_FULL);
		return;
	}

	// 获取奖励
	int reward_seq_list[Protocol::SC_CHOU_PET_FOOD_MARKET_MAX_TIMES]; memset(reward_seq_list, 0, sizeof(reward_seq_list));
	if (!LOGIC_CONFIG->GetPetCfg().GetFoodMarketRandomRewards(reward_seq_list, times))
	{
		return;
	}

	// 消耗
	int need_gold = LOGIC_CONFIG->GetPetCfg().GetOtherCfg().food_market_ten_times_need_gold;

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "Pet::FoodMarketChouTimes")) 
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	static Protocol::SCPetFoodMarcketChouResult fmcr;
	fmcr.reward_list_count = 0;

	gamelog::g_log_pet.buff_printf("Pet::FoodMarketChouTimes user[%d %s], tims[%d], need_gold[%d], reward[", 
		m_role->GetUID(), m_role->GetName(), times, need_gold);

	// 给予
	for (int i = 0; i < times && i < Protocol::SC_CHOU_PET_FOOD_MARKET_MAX_TIMES; ++ i)
	{
		const PetFoodMarketConfig *reward_cfg = LOGIC_CONFIG->GetPetCfg().GetFoodMarketCfg(reward_seq_list[i]);
		if (NULL == reward_cfg)
		{
			continue;
		}
		ItemConfigData reward_item = reward_cfg->reward_item;
		reward_item.is_bind = false;

		this->Put(reward_item);

		fmcr.reward_seq_list[fmcr.reward_list_count ++] = reward_cfg->seq;

		gamelog::g_log_pet.buff_printf("(%s)", ItemConfigDataLog(&reward_item, NULL));
	}

	// 发送
	this->SendPetBackpackList();

	if (fmcr.reward_list_count > 0)
	{
		int send_len = sizeof(fmcr) - (Protocol::SC_CHOU_PET_FOOD_MARKET_MAX_TIMES - fmcr.reward_list_count) * sizeof(fmcr.reward_seq_list[0]);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&fmcr, send_len);
	}

	// 日志
	gamelog::g_log_pet.buff_printf("]");
	gamelog::g_log_pet.commit_buff(LL_INFO);
}

void Pet::Put(const ItemConfigData &item_info)
{
	bool has_put_into_store = false;
	for (int i = 0; i < PET_MAX_STORE_COUNT; ++ i)
	{
		bool cur_is_bind = (m_param.backpack_item_list[i].is_bind == 1);
		if (m_param.backpack_item_list[i].id != item_info.item_id ||
			cur_is_bind != item_info.is_bind)
		{
			continue;
		}

		const ItemBase *item_base = ITEMPOOL->GetItem(m_param.backpack_item_list[i].id);
		if (NULL == item_base)
		{
			continue;
		}

		int item_total_num = m_param.backpack_item_list[i].num + item_info.num;
		if (item_total_num <= item_base->GetPileLimit())
		{
			m_param.backpack_item_list[i].num = item_total_num;
			has_put_into_store = true;
			break;
		}
	}

	if (!has_put_into_store)
	{
		int index = m_empty_grid_index_list.top();
		if (index < 0 || index >= PET_MAX_STORE_COUNT)
		{
			//m_role->NoticeNum(errornum::EN_PET_BACKPACK_FULL);
			return;
		}
		m_param.backpack_item_list[index].id = item_info.item_id;
		m_param.backpack_item_list[index].num += item_info.num;
		m_param.backpack_item_list[index].is_bind = item_info.is_bind;

		m_empty_grid_index_list.pop();
	}
}

void Pet::SendPetQinmiSingleInfo(int pet_id)
{
	if (pet_id < 0 || pet_id >= PET_MAX_COUNT_LIMIT)
	{
		return;
	}

	static Protocol::SCPetQinmiSingleInfo qsi;
	qsi.pet_qinmi_item = m_param.pet_qinmi_list[pet_id];
	qsi.cur_pet_id = pet_id;
	qsi.ignore_fangyu_percent_level = m_ignore_fangyu_percent_level;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&qsi, sizeof(qsi));
}

void Pet::SendPetQinmiAllInfo()
{
	this->ReCalcIgnoreFangyuPercent();

	static Protocol::SCPetQinmiAllInfo qai;
	memcpy(qai.pet_qinmi_list, m_param.pet_qinmi_list, sizeof(qai.pet_qinmi_list));
	qai.ignore_fangyu_percent_level = m_ignore_fangyu_percent_level;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&qai, sizeof(qai));
}

void Pet::ReCalcIgnoreFangyuPercent()
{
	int pet_count = LOGIC_CONFIG->GetPetCfg().GetPetCount();
	int min_qinmi_level = LOGIC_CONFIG->GetPetCfg().GetMaxQinmiLevel();

	for (int index = 0; index < pet_count && index < PET_MAX_COUNT_LIMIT; ++ index)
	{
		int qinmi_level = m_param.pet_qinmi_list[index].level;
		if (qinmi_level < min_qinmi_level)
		{
			min_qinmi_level = qinmi_level;
		}
	}

	const PetQinmiLevelConfig::StarConfig * star_cfg = LOGIC_CONFIG->GetPetCfg().GetQinmiLevelStarCfg(min_qinmi_level, 1);
	if (star_cfg != NULL)
	{
		m_ignore_fangyu_percent = star_cfg->ignore_fangyu_percent;
	}
	else
	{
		m_ignore_fangyu_percent = 0;
	}

	m_ignore_fangyu_percent_level = min_qinmi_level;
}

int Pet::GetCurPetQinmiLevel()
{
	if (m_param.cur_on_use_pet_id >= 0 && m_param.cur_on_use_pet_id < PET_MAX_COUNT_LIMIT)
	{
		return m_param.pet_qinmi_list[m_param.cur_on_use_pet_id].level;
	}

	return 0;
}

void Pet::PetPrint(const char *reason, int param1, int param2, int param3)
{
	gamelog::g_log_pet.printf(LL_INFO, "Pet role(%d,%s), role_level[%d], %s(%d,%d,%d) "
		"cur_on_use_pet_id:%d, grade_bless:%d, grade:%d, level:%d, special_img_active_flag:%d, use_img_id:%d",
		m_role->GetUID(), m_role->GetName(), m_role->GetLevel(),
		reason, param1, param2, param3, 

		m_param.cur_on_use_pet_id,
		m_param.own_pet_list[m_param.cur_on_use_pet_id].bless,
		m_param.own_pet_list[m_param.cur_on_use_pet_id].grade, 
		m_param.own_pet_list[m_param.cur_on_use_pet_id].level,

		m_param.special_img_active_flag,
		m_param.use_img_id);
}