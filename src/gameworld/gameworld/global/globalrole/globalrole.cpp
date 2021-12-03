#include "globalrole.hpp"

#include "world.h"
#include "globalrolemanager.hpp"
#include "servercommon/struct/structcommon.h"
#include "litttlepet/littlepet.hpp"
#include "lovetree/lovetree.hpp"
#include "lovecontract/lovecontract.hpp"
#include "qingyuanglobal/qingyuanglobal.hpp"

#include "jinglinghome/jinglinghome.hpp"
#include "fighting/rolefighting.hpp"
#include "global/usercache/usercache.hpp"
#include "servercommon/string/gameworldstr.h"
#include "config/logicconfigmanager.hpp"
#include "fighting/fightingconfig.hpp"
#include "global/rank/rankmanager.hpp"
#include "global/globalrole/commondata/commondataglobal.hpp"
#include "obj/character/role.h"
#include "other/vip/vip.hpp"
#include "other/capability/capability.hpp"
#include "other/route/mailroute.hpp"
#include "spousehome/spousehome.hpp"

IGlobalRoleItem::IGlobalRoleItem(GlobalRole *g_role) : m_global_role(g_role)
{
	if (NULL != g_role)
	{
		m_role_id = g_role->GetOwnerUid();
	}
}

void IGlobalRoleItem::OnDataChange()
{
	m_global_role->OnDataChange();
}

Role * IGlobalRoleItem::GetRole() const
{
	return World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role_id));
}

UserCacheNode *IGlobalRoleItem::GetUserCache() const
{
	return UserCacheManager::Instance().GetUserNode(m_role_id);
}

GlobalRole::GlobalRole(GlobalRoleManager *mgr, int uid) : m_global_role_mgr(mgr), m_role_id(uid)
{
	m_little_pet = new LittlePet(this);
	m_love_tree = new LoveTree(this);
	m_love_contract = new LoveContract(this);
	m_jing_ling_home = new JingLingHome(this);
	m_role_fighting = new RoleFighting(this);
	m_qingyuan_global = new QingyuanGlobal(this);
	m_common_data_global = new CommonDataGlobal(this);
	m_spouse_home_global = new SpouseHome(this);
}

GlobalRole::~GlobalRole()
{
	delete m_little_pet;
	delete m_love_tree;
	delete m_love_contract;
	delete m_jing_ling_home;
	delete m_role_fighting;
	delete m_qingyuan_global;
	delete m_common_data_global;
	delete m_spouse_home_global;
}

void GlobalRole::Init(const GlobalRoleDatalListParam::RoleDataAttr &global_role_attr)
{
	m_role_id = global_role_attr.role_id;
	m_role_appearance = global_role_attr.role_appearance;

	m_little_pet->Init(global_role_attr.little_pet_param);
	m_love_tree->Init(global_role_attr.love_tree_param);
	m_love_contract->Init(global_role_attr.love_contract_param);
	m_jing_ling_home->Init(global_role_attr.jing_ling_home_param);
	m_role_fighting->Init(global_role_attr.fighting_data);
	m_qingyuan_global->Init(global_role_attr.qingyuan_global_param);
	m_common_data_global->Init(global_role_attr.common_data_global_param);
	m_spouse_home_global->Init(global_role_attr.spouse_home_param);
}

void GlobalRole::GetAttr(GlobalRoleDatalListParam::RoleDataAttr &global_role_attr)
{
	global_role_attr.role_id = m_role_id;
	global_role_attr.role_appearance = m_role_appearance;

	m_little_pet->GetAttr(global_role_attr.little_pet_param);
	m_love_tree->GetAttr(global_role_attr.love_tree_param);
	m_love_contract->GetAttr(global_role_attr.love_contract_param);
	m_jing_ling_home->GetAttr(global_role_attr.jing_ling_home_param);
	m_role_fighting->GetAttr(global_role_attr.fighting_data);
	m_qingyuan_global->GetAttr(global_role_attr.qingyuan_global_param);
	m_common_data_global->GetAttr(global_role_attr.common_data_global_param);
	m_spouse_home_global->GetInit(global_role_attr.spouse_home_param);
}

void GlobalRole::ReCalcAttr(CharIntAttrs &base_add, int recalc_reason, bool recalc_all, Role *role)
{
	if (NULL == role) return;

	// 小宠物
	m_little_pet->ReCalcAttr(base_add, CharIntAttrs::RECALC_REASON_LITTLE_PET == recalc_reason || recalc_all, role);

	// 相思树
	m_love_tree->ReCalcAttr(base_add, CharIntAttrs::RECALC_REASON_LOVE_TREE == recalc_reason || recalc_all, role);

	// 情缘
	m_qingyuan_global->ReCalcAttr(base_add, CharIntAttrs::RECALC_REASON_QINGYUAN == recalc_reason || recalc_all, role);

	// 夫妻家园
	m_spouse_home_global->ReCalcAttr(base_add, CharIntAttrs::RECALC_REASON_SPOUSE_HOME == recalc_reason || recalc_all, role);
}

void GlobalRole::Update(unsigned long interval, time_t now_second)
{
	m_little_pet->Update((unsigned int)now_second);
	m_love_tree->Update((unsigned int)now_second);
	m_role_fighting->Update((unsigned int)now_second);
	m_qingyuan_global->Update((unsigned int)now_second);
}

void GlobalRole::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	// 注意，这个OnDayChange并不像Role，如果切天时玩家不在线，等玩家上线后也不会再触发之前的OnDayChange

	m_little_pet->OnDayChange(old_dayid, now_dayid);
	m_love_tree->OnDayChange(old_dayid, now_dayid);
	m_love_contract->OnDayChange(old_dayid, now_dayid);
	m_jing_ling_home->OnDayChange(old_dayid, now_dayid);
	m_role_fighting->OnDayChange(old_dayid, now_dayid);
	m_qingyuan_global->OnDayChange(old_dayid, now_dayid);

	this->CheckFightingChallengeRankReward();
}

void GlobalRole::GetCrossData(RoleCrossGlobalData &cross_role_global_data, Role *role)
{
	if (NULL == role) return;

	static CharIntAttrs tmp_attr; tmp_attr.Reset();


	this->ReCalcAttr(tmp_attr, CharIntAttrs::RECALC_REASON_INVALID, false, role);

	for (int i = 0; i < CharIntAttrs::BASE_CHARINTATTR_TYPE_MAX - CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN; i++)
	{
		cross_role_global_data.attrs[i] += tmp_attr.m_attrs[i + CharIntAttrs::BASE_CHARINTATTR_TYPE_BEGIN];
	}
}

void GlobalRole::OnRoleLogin(Role *role)
{
	m_role_appearance = role->GetAppearance();

	m_common_data_global->OnRoleLogin(role);
	m_little_pet->OnRoleLogin(role);
	m_love_tree->OnRoleLogin(role);
	m_love_contract->OnRoleLogin(role);
	m_role_fighting->OnRoleLogin(role);
	m_qingyuan_global->OnRoleLogin(role);
	m_spouse_home_global->OnRoleLogin(role);
}

void GlobalRole::OnRoleLogout(Role *role)
{
	m_role_appearance = role->GetAppearance();

	m_role_fighting->OnRoleLogout(role);
	m_common_data_global->OnRoleLogout(role);
}

void GlobalRole::OnDataChange()
{
	GlobalRoleManager::Instance().OnGlobalRoleDataChange(this, structcommon::CS_UPDATE);
}

void GlobalRole::CheckFightingChallengeRankReward() const
{
	UserID user_id_list[PERSON_RANK_MAX_SIZE];
	int rank_count = RankManager::Instance().GetPersonRank()->GetRankTopUser(PERSON_RANK_TYPE_FIGHTING_CHALLENGE_SCORE, PERSON_RANK_MAX_SIZE, user_id_list);
	
	static MailContentParam contentparam;
	for (int i = 0; i < rank_count && i < PERSON_RANK_MAX_SIZE; ++i)
	{
		int rank = i + 1;
		const FightingRankRewardCfg * rank_cfg = LOGIC_CONFIG->GetFightingConfig().GetChallengeRankReward(rank);
		if (NULL == rank_cfg) break; // 找不到配置，后面的也肯定都不需要奖励的了
		
		contentparam.Reset();
		gamelog::g_log_fighting.buff_printf("CheckFightingChallengeRankReward rank %d user_id %d reward_list[", rank, UidToInt(user_id_list[i]));
		
		int item_count = 0;
		for (int j = 0; j < rank_cfg->reward_item_count && j < FightingRankRewardCfg::CFG_COUNT && item_count < MAX_ATTACHMENT_ITEM_NUM; ++j)
		{
			const ItemConfigData &item_cfg = rank_cfg->reward_item_list[j];

			const ItemBase *reward_item_base = ITEMPOOL->GetItem(item_cfg.item_id);
			if (NULL != reward_item_base)
			{
				gamelog::g_log_fighting.buff_printf("%s, ", ItemConfigDataLog(&item_cfg, NULL));

				contentparam.item_list[item_count].item_id = item_cfg.item_id;
				contentparam.item_list[item_count].num = item_cfg.num;
				contentparam.item_list[item_count].is_bind = (item_cfg.is_bind) ? 1 : 0;
				contentparam.item_list[item_count].invalid_time = reward_item_base->CalInvalidTime();
				++item_count;
			}
		}

		int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_fighting_challenge_rank_rward_content, rank);
		if (length > 0)
		{
			MailRoute::MailToUser(user_id_list[i], SYSTEM_MAIL_REASON_INVALID, contentparam);

			gamelog::g_log_fighting.buff_print("]");
			gamelog::g_log_fighting.commit_buff(LL_INFO);
		}
		else
		{
			gamelog::g_log_fighting.clear_buff();
		}
	}

	RankManager::Instance().ClearPersonRankType(PERSON_RANK_TYPE_FIGHTING_CHALLENGE_SCORE);
}

void GlobalRole::UpdateToRank(Role *m_role) const
{
	if (NULL == m_role) return;

	static SyncPersonRankInfo rank_info;
	rank_info.ResetItems();

	UserID user_id = m_role->GetUserId();
	rank_info.db_index = user_id.db_index;
	rank_info.role_id = user_id.role_id;
	rank_info.exp = m_role->GetExp();
	rank_info.level = m_role->GetLevel();
	m_role->GetName(rank_info.user_name);
	m_role->GetGuildName(rank_info.guild_name);
	rank_info.reserve_ch = 0;
	rank_info.sex = m_role->GetSex();
	rank_info.prof = m_role->GetProf();
	rank_info.camp = m_role->GetCamp();
	rank_info.vip_level = m_role->GetVip()->GetVipLevel();
	rank_info.jingjie_level = m_role->GetJingJieLevel();
	rank_info.reserved2 = 0;

	UNSTD_STATIC_CHECK(sizeof(GameName) >= sizeof(GuildName));

	int item_count = 0;
	if (item_count < SYNC_PERSON_RANK_INFO_SIZE)															// 综合战力榜
	{
		rank_info.items[item_count].rank_type = PERSON_RANK_TYPE_FIGHTING_CHALLENGE_SCORE;
		rank_info.items[item_count].rank_value = m_role_fighting->GetChallengeScore();
		m_role->GetGuildName(rank_info.items[item_count].flexible_name);
		rank_info.items[item_count].record_index = 0;
		rank_info.items[item_count].flexible_int = m_role->GetCapability()->GetTotalCapability();

		++item_count;
	}

	rank_info.item_count = item_count;
	if (rank_info.item_count > 0)
	{
		RankManager::Instance().OnSyncPersonRankInfo(IntToUid(m_role_id), &rank_info);
	}
}

RoleAppearance & GlobalRole::GetRoleAppearance()
{
	return m_role_appearance;
}
