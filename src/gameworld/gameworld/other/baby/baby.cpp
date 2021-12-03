#include "baby.hpp"
#include "friend/teaminfo.hpp"
#include "servercommon/errornum.h"
#include "babyconfig.hpp"
#include "global/team/team.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "protocal/msgbaby.h"
#include "world.h"
#include "gstr.h"
#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/namefilter.hpp"
#include "servercommon/noticenum.h"
#include "other/event/eventhandler.hpp"
#include "servercommon/string/crossstr.h"
#include "obj/character/role.h"
#include "gamelog.h"
#include "servercommon/string/gameworldstr.h"
#include "global/mail/mailmanager.hpp"
#include "global/usercache/babycache.hpp"
#include "global/usercache/usercache.hpp"
#include "item/itempool.h"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"
#include "other/sublock/sublock.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "global/globalrole/qingyuanglobal/qingyuanglobal.hpp"

Baby::Baby() : m_role(NULL), m_lover_request_create_baby_type(0)
{

}

Baby::~Baby()
{

}

void Baby::InitParam(Role *role, const BabyParam &param)
{
	m_role = role;
	m_param = param;

	// 初始化，身上拥有的宝宝全部跟cahce同步一遍
	for(int index = 0; index < BABY_MAX_COUNT; ++ index)
	{
		BabyInfo &info = m_param.baby_list[index];
		if (0 == info.unique_id)
		{
			continue;
		}

		m_dirty_baby_index_list.insert(index);
	}

	this->CheckSyncCache();
}

void Baby::GetInitParam(BabyParam *param)
{
	*param = m_param;
}

void Baby::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		for (int baby_index = 0; baby_index < BABY_MAX_COUNT; ++ baby_index)
		{
			if (m_param.baby_list[baby_index].baby_id < 0)
			{
				continue;
			}

			// 基础属性
			const BabyInfoCfg *base_info = LOGIC_CONFIG->GetBabyCfg().GetBabyInfo(m_param.baby_list[baby_index].baby_id);
			if (NULL != base_info)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += base_info->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += base_info->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += base_info->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += base_info->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += base_info->jianren;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += base_info->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += base_info->shanbi;
			}


			// 等级
			const BabyLevelCfg *level_cfg = LOGIC_CONFIG->GetBabyCfg().GetBabyLevelCfg(m_param.baby_list[baby_index].baby_id, m_param.baby_list[baby_index].level);
			if (NULL != level_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += level_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += level_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += level_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += level_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += level_cfg->jianren;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += level_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += level_cfg->shanbi;
			}

			// 阶

			const BabyGradeConfig *grade_cfg = LOGIC_CONFIG->GetBabyCfg().GetBabyGradeCfg(m_param.baby_list[baby_index].baby_id, m_param.baby_list[baby_index].grade);
			if (NULL != grade_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += grade_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += grade_cfg->gongji;

				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += grade_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += grade_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += grade_cfg->jianren;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += grade_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += grade_cfg->shanbi;
			}

			// 守护精灵
			for (int spirit_id = 0; spirit_id < BABY_SPIRIT_COUNT; ++ spirit_id)
			{
				const BabySpiritConfig *spirit_cfg = LOGIC_CONFIG->GetBabyCfg().GetBabySpiritCfg(spirit_id, m_param.baby_list[baby_index].baby_spirit_list[spirit_id].spirit_level);
				if (NULL != spirit_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += spirit_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += spirit_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += spirit_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += spirit_cfg->baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += spirit_cfg->jianren;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += spirit_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += spirit_cfg->shanbi;
				}
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_BABY, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void Baby::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid)
	{
		return;
	}
}

bool Baby::UpgradeBaby(short baby_index, int is_auto_buy)
{
	if (baby_index < 0 || baby_index >= BABY_MAX_COUNT)
	{
		return false;
	}

	if (m_param.baby_list[baby_index].baby_id < 0)
	{
		return false;
	}

	if (m_param.baby_list[baby_index].lover_uid != m_role->GetLoverUid())
	{
		m_role->NoticeNum(errornum::EN_BABY_IS_OVERDUE);
		return false;
	}

	int max_grade = LOGIC_CONFIG->GetBabyCfg().GetBabyMaxGrade();
	if (m_param.baby_list[baby_index].grade >= BABY_MAX_GRADE_LIMIT || m_param.baby_list[baby_index].grade >= max_grade)
	{
		m_role->NoticeNum(errornum::EN_BABY_GRADE_LIMIT);
		return false;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	const BabyGradeConfig *grade_cfg = LOGIC_CONFIG->GetBabyCfg().GetBabyGradeCfg(m_param.baby_list[baby_index].baby_id, m_param.baby_list[baby_index].grade);
	if (NULL == grade_cfg)
	{
		return false;
	}

	const BabyGradeConfig *next_grade_cfg = LOGIC_CONFIG->GetBabyCfg().GetBabyGradeCfg(m_param.baby_list[baby_index].baby_id, m_param.baby_list[baby_index].grade + 1);
	if (NULL == next_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_BABY_GRADE_LIMIT);
		return false;
	}

	const BabyUpGradeRateConfig *upgrade_rate_cfg = LOGIC_CONFIG->GetBabyCfg().GetSuccRate(m_param.baby_list[baby_index].grade, m_param.baby_list[baby_index].bless);
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
			m_role->NoticeNum(errornum::EN_BABY_UGRADE_STUFF_NOT_ENOUGHT);
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "Baby::UpgradeBaby"))
		{
			return false;
		}
	}

	{
		// 生效
		int rand_val = RandomNum(10000);
		if (rand_val < upgrade_rate_cfg->succ_rate)
		{
			//进阶成功
			m_param.baby_list[baby_index].bless = 0;
			++ m_param.baby_list[baby_index].grade;

			m_role->NoticeNum(noticenum::NT_BABY_UPGRADE_SUCC);
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_BABY);

			
			//传闻
			if (next_grade_cfg->is_broadcast)
			{
				int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_baby_upgrade,
					UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), m_param.baby_list[baby_index].grade);

				if (len > 0)
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE_3);
				}
			}
			{
				//发送邮件
				static MailContentParam contentparam; contentparam.Reset();
				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_baby_subject_mail);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_baby_upgrade_mail, m_param.baby_list[baby_index].grade);
				if (length1 > 0 && length2 > 0)
				{
					UserMailManager::Instance().MailToUser(MAIL_TYPE_SYSTEM, IntToUid(m_role->GetLoverUid()), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}
		else
		{
			m_param.baby_list[baby_index].bless += upgrade_rate_cfg->GetRandomZhufuOnFail();

			if (m_param.baby_list[baby_index].bless > grade_cfg->max_bless_value)
			{
				m_param.baby_list[baby_index].bless = grade_cfg->max_bless_value;
			}
		}
	}
	// syn
	unsigned int nowtime = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_param.baby_list[baby_index].last_modity_time = nowtime;
	m_dirty_baby_index_list.insert(baby_index);

	this->SendBabyInfo(baby_index);

	//日志
	gamelog::g_log_baby.printf(LL_INFO, "Baby::UpgradeBaby user[%d %s] is_auto_buy[%d] cur_grade[%d] cur_bless[%d] baby_index[%d] baby_id[%d] unique_id[%lld] time[%d]",
		m_role->GetUID(), m_role->GetName(), is_auto_buy, m_param.baby_list[baby_index].grade, m_param.baby_list[baby_index].bless, baby_index, m_param.baby_list[baby_index].baby_id, m_param.baby_list[baby_index].unique_id, nowtime);

	return true;
}

void Baby::UplevelBaby(short baby_index)
{
	if (baby_index < 0 || baby_index >= BABY_MAX_COUNT)
	{
		return;
	}

	if (m_param.baby_list[baby_index].baby_id < 0)
	{
		return;
	}

	if (m_param.baby_list[baby_index].lover_uid != m_role->GetLoverUid())
	{
		m_role->NoticeNum(errornum::EN_BABY_IS_OVERDUE);
		return;
	}

	if (m_param.baby_list[baby_index].level >= m_role->GetLevel() || m_param.baby_list[baby_index].level >= BABY_MAX_LEVEL_LIMIT)
	{
		m_role->NoticeNum(errornum::EN_BABY_LEVEL_LIMIT);
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

	const BabyLevelCfg *level_cfg = LOGIC_CONFIG->GetBabyCfg().GetBabyLevelCfg(m_param.baby_list[baby_index].baby_id, m_param.baby_list[baby_index].level);
	if (NULL == level_cfg)
	{
		return;
	}
	
	ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

	short stuff_count = 0;
	stuff_list[stuff_count].item_id = level_cfg->stuff_id_1;
	stuff_list[stuff_count].num = level_cfg->stuff_num_1;
	stuff_list[stuff_count].buyable = false;
	++ stuff_count;

	stuff_list[stuff_count].item_id = level_cfg->stuff_id_2;
	stuff_list[stuff_count].num = level_cfg->stuff_num_2;
	stuff_list[stuff_count].buyable = false;
	++ stuff_count;

	stuff_list[stuff_count].item_id = level_cfg->stuff_id_3;
	stuff_list[stuff_count].num = level_cfg->stuff_num_3;
	stuff_list[stuff_count].buyable = false;
	++ stuff_count;

	stuff_list[stuff_count].item_id = level_cfg->stuff_id_4;
	stuff_list[stuff_count].num = level_cfg->stuff_num_4;
	stuff_list[stuff_count].buyable = false;
	++ stuff_count;

	// 检查消耗
	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
	{
		return;
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "UplevelBaby"))
	{
		return;
	}

	// 生效
	++ m_param.baby_list[baby_index].level;

	{
		//发送邮件
		static MailContentParam contentparam; contentparam.Reset();
		int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_baby_subject_mail);
		int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_baby_uplevel_mail, m_param.baby_list[baby_index].level);
		if (length1 > 0 && length2 > 0)
		{
			UserMailManager::Instance().MailToUser(MAIL_TYPE_SYSTEM, IntToUid(m_role->GetLoverUid()), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_BABY);

	// sync
	unsigned int nowtime = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_param.baby_list[baby_index].last_modity_time = nowtime;
	m_dirty_baby_index_list.insert(baby_index);

	this->SendBabyInfo(baby_index);

	//log
	gamelog::g_log_baby.printf(LL_INFO, "Baby::UplevelBaby user[%d %s] cur_level[%d]  baby_index[%d] baby_id[%d] unique_id[%lld] time[%d]",
		m_role->GetUID(), m_role->GetName(), m_param.baby_list[baby_index].level, baby_index, m_param.baby_list[baby_index].baby_id, m_param.baby_list[baby_index].unique_id, nowtime);
}

void Baby::ChangeBabyName(short baby_index, const GameName new_name)
{
	if (NULL == new_name)
	{
		return;
	}

	if (baby_index < 0 || baby_index >= BABY_MAX_COUNT)
	{
		return;
	}

	if (m_param.baby_list[baby_index].baby_id < 0)
	{
		return;
	}

	if (m_param.baby_list[baby_index].lover_uid != m_role->GetLoverUid())
	{
		m_role->NoticeNum(errornum::EN_BABY_IS_OVERDUE);
		return;
	}

	GameName tmp_new_name; memset(tmp_new_name, 0, sizeof(tmp_new_name));

	memcpy(tmp_new_name, new_name, sizeof(tmp_new_name));
	tmp_new_name[sizeof(GameName) - 1] = 0;

	InvalidCharFilte(tmp_new_name);

	bool NAMEFILTER_OPEN_FLAG = ServerConfigPool::Instance().common_config.namefilter_open_flag;
	if (NAMEFILTER_OPEN_FLAG && (NameFilter::Instance().HasSensitiveWord(sizeof(GameName), tmp_new_name)
		|| NameFilter::Instance().HasNumberWord(sizeof(GameName), tmp_new_name)))
	{
		m_role->NoticeNum(errornum::EN_RENAME_NAME_INVALID);
		return;
	} 

	GameName old_name;
	F_STRNCPY(old_name, m_param.baby_list[baby_index].baby_name, sizeof(GameName));

	//消耗
	const BabyOtherCfg &other_cfg = LOGIC_CONFIG->GetBabyCfg().GetOtherCfg();
	if (m_param.baby_list[baby_index].rename_times < other_cfg.free_rename_times)
	{
		++ m_param.baby_list[baby_index].rename_times;
	}
	else
	{
		short stuff_count = 0;
		//材料
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = other_cfg.rename_card_id;
		stuff_list[stuff_count].num = 1;
		stuff_list[stuff_count].buyable = false;
		++ stuff_count;

		static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			m_role->NoticeNum(errornum::EN_BABY_RENAME_STUFF_NOT_ENOUGHT);
			return;
		}

		// 消耗改名卡
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "ChangeBabyName"))
		{
			return;
		}
	}

	F_STRNCPY(m_param.baby_list[baby_index].baby_name, tmp_new_name, sizeof(GameName));

	m_role->NoticeNum(noticenum::NT_BABY_RENAME_SUCC);

	// syn
	unsigned int nowtime = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_param.baby_list[baby_index].last_modity_time = nowtime;
	m_dirty_baby_index_list.insert(baby_index);

	this->SendBabyInfo(baby_index);
	
	//log
	gamelog::g_log_baby.printf(LL_INFO, "Baby::ChangeBabyName user[%d %s] baby_index[%d] baby_id[%d] oldname[%s] newname[%s] unique_id[%lld] time[%d]",
		m_role->GetUID(), m_role->GetName(), baby_index, m_param.baby_list[baby_index].baby_id, old_name, new_name, m_param.baby_list[baby_index].unique_id, nowtime);

}

void Baby::RequestCreateBaby(int qifu_type)
{
	if (qifu_type <= QIFU_TREE_MIN || qifu_type >= QIFU_TREE_MAX)
	{
		return;
	}

	//TeamInfo *team_info = m_role->GetTeamInfo();
	//if (NULL == team_info)
	//{
	//	return;
	//}

	//if (!team_info->InTeam())
	//{
	//	m_role->NoticeNum(errornum::EN_BABY_HAVE_TO_IN_SAME_TEAM);
	//	return;
	//}

	//Team *team = team_info->GetMyTeam();
	//if (NULL == team)
	//{
	//	return;
	//}

	///*if (!team->IsLeader(m_role->GetUserId()))
	//{
	//	m_role->NoticeNum(errornum::EN_BABY_RESERVE_MUST_BE_TEAM_LEADER);
	//	return;
	//}*/

	//if (BABY_BORN_MEMBER_COUNT != team->GetMemberNum())
	//{
	//	m_role->NoticeNum(errornum::EN_BABY_TEAM_MEMBER_NUM_INVALID);
	//	return;
	//}

	if (!m_role->IsMarry())
	{
		m_role->NoticeNum(errornum::EN_MARRY_NOT_MARRIED);
		return;
	}

	int lover_id = m_role->GetLoverUid();
	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(lover_id));
	if (NULL == target_role)
	{
		m_role->NoticeNum(errornum::EN_BABY_LOVER_NOT_ONLINE);
		return;
	}

	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_BABY, &limit_unit);

	if (is_limit && m_role->GetLevel() < limit_unit.level)
	{
		m_role->NoticeNum(errornum::EN_BABY_LEVEL_NOT_ENOUGH);
		return;
	}

	if (is_limit && target_role->GetLevel() < limit_unit.level)
	{
		m_role->NoticeNum(errornum::EN_BABY_LEVEL_NOT_ENOUGH_OTHER);
		return;
	}

	//if (NULL == team->GetTeamMember(IntToUid(lover_id)))
	//{
	//	m_role->NoticeNum(errornum::EN_BABY_CAN_NOT_LITTLE_THREE);
	//	return;
	//}

	if (!target_role->IsMarry())
	{
		return;
	}

	if (this->CheckBabyListIsFull())
	{
		m_role->NoticeNum(errornum::EN_BABY_COUNT_FULL);
		return;
	}

	if (target_role->GetBaby()->CheckBabyListIsFull())
	{
		m_role->NoticeNum(errornum::EN_BABY_OTHER_COUNT_FULL);
		return;
	}

	const BabyQifuTreeConfig *qifu_cfg = LOGIC_CONFIG->GetBabyCfg().GetQifuCfg(qifu_type);
	if (NULL == qifu_cfg)
	{
		return;
	}

	int consume_gold = qifu_cfg->qifu_consume_gold;
	int consume_bind_gold = qifu_cfg->qifu_consume_bind_gold;
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DISCOUNT_GET_BABY))
	{
		consume_gold = qifu_cfg->activity_qifu_consume_gold;
		consume_bind_gold = qifu_cfg->activity_qifu_consume_bind_gold;
	}

	if (consume_bind_gold > 0 && !m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(consume_bind_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (consume_gold > 0 && !m_role->GetKnapsack()->GetMoney()->GoldMoreThan(consume_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!this->CheckBabyNeedOtherBabyGrade())
	{
		m_role->NoticeNum(errornum::EN_BABY_NOT_ENOUGH_LEVEL);
		return;
	}

	if (!target_role->GetBaby()->CheckBabyNeedOtherBabyGrade())
	{
		m_role->NoticeNum(errornum::EN_BABY_NOT_ENOUGH_LEVEL);
		return;
	}

	// 发送确认消息
	target_role->GetBaby()->LoverRequestCreateBaby(qifu_type);
	m_role->NoticeNum(errornum::EN_BABY_BIRTH_REQ_SED_SUCCESS);

	gamelog::g_log_baby.printf(LL_INFO, "Baby::RequestCreateBaby user[%d %s] type[%d] lover_id[%d]",
		m_role->GetUID(), m_role->GetName(), qifu_type, lover_id);
}

void Baby::ReplyCreateBabyRequest(int type, bool is_accept)
{
	if (m_lover_request_create_baby_type <= 0)
	{
		return;
	}

	if (type != m_lover_request_create_baby_type)
	{
		m_lover_request_create_baby_type = 0;
		return;
	}

	m_lover_request_create_baby_type = 0;

	if (!m_role->IsMarry())
	{
		m_role->NoticeNum(errornum::EN_MARRY_NOT_MARRIED);
		return;
	}

	//TeamInfo *team_info = m_role->GetTeamInfo();
	//if (NULL == team_info)
	//{
	//	return;
	//}

	//if (!team_info->InTeam())
	//{
	//	m_role->NoticeNum(errornum::EN_BABY_HAVE_TO_IN_SAME_TEAM);
	//	return;
	//}

	//Team *team = team_info->GetMyTeam();
	//if (NULL == team)
	//{
	//	return;
	//}

	int lover_id = m_role->GetLoverUid();
	//Team::TeamMember *member = team->GetTeamMember(IntToUid(lover_id));
	//if (NULL == member)
	//{
	//	m_role->NoticeNum(errornum::EN_BABY_CAN_NOT_LITTLE_THREE);
	//	return;
	//}

	//if (BABY_BORN_MEMBER_COUNT != team->GetMemberNum())
	//{
	//	m_role->NoticeNum(errornum::EN_BABY_TEAM_MEMBER_NUM_INVALID);
	//	return;
	//}

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(lover_id));
	if (NULL == target_role)
	{
		if (is_accept) m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	if (!is_accept)
	{
		target_role->NoticeNum(errornum::EN_BABY_UN_ACCEPT);
		return;
	}
	
	if (!this->CheckBabyNeedOtherBabyGrade())
	{
		m_role->NoticeNum(errornum::EN_BABY_NOT_ENOUGH_LEVEL);
		return;
	}

	if (!target_role->GetBaby()->CheckBabyNeedOtherBabyGrade())
	{
		m_role->NoticeNum(errornum::EN_BABY_NOT_ENOUGH_LEVEL);
		return;
	}

	const BabyQifuTreeConfig *qifu_cfg = LOGIC_CONFIG->GetBabyCfg().GetQifuCfg(type);
	if (NULL == qifu_cfg)
	{
		return;
	}

	const BabyInfoCfg *baby_info_cfg = LOGIC_CONFIG->GetBabyCfg().GetBabyInfo(qifu_cfg->qifu_reward_baby_id);
	if (NULL == baby_info_cfg)
	{
		return;
	}

	int unused_baby_index = this->GetUnusedBabyIndex();
	int unused_baby_index_lover = target_role->GetBaby()->GetUnusedBabyIndex();

	if (-1 == unused_baby_index || -1 == unused_baby_index_lover)
	{
		return;
	}

	if (is_accept)
	{
		//消耗
		int consume_gold = qifu_cfg->qifu_consume_gold;
		int consume_bind_gold = qifu_cfg->qifu_consume_bind_gold;
		if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DISCOUNT_GET_BABY))
		{
			consume_gold = qifu_cfg->activity_qifu_consume_gold;
			consume_bind_gold = qifu_cfg->activity_qifu_consume_bind_gold;
		}

		if (consume_bind_gold > 0 && !target_role->GetKnapsack()->GetMoney()->UseAllGold(consume_bind_gold, "Baby::ReplyCreateBabyRequest"))
		{
			target_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		if (consume_gold > 0 && !target_role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "Baby::ReplyCreateBabyRequest"))
		{
			target_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		// 给予
		long long unique_id = BabyCacheManager::Instance().GetNewUniqueId(target_role->GetUserId().db_index);

		this->GiveBaby(qifu_cfg->qifu_reward_baby_id, baby_info_cfg->baby_name, unique_id, unused_baby_index);
		target_role->GetBaby()->GiveBaby(qifu_cfg->qifu_reward_baby_id, baby_info_cfg->baby_name, unique_id, unused_baby_index_lover);
		//	
		if (QIFU_TREE_BEST == type)
		{
			// 传闻
			int len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_baby_qifu_succ,
			UidToInt(target_role->GetUserId()), target_role->GetName(), target_role->GetCamp(),	UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp());

			if (len > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, len, SYS_MSG_CENTER_NOTICE);
			}
		}

		m_dirty_baby_index_list.insert(unused_baby_index);

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_BABY);
		m_role->NoticeNum(noticenum::NT_BABY_BORN_SUCC);
		target_role->NoticeNum(noticenum::NT_BABY_BORN_SUCC);

		//江湖传闻
		GuildID guild_id = m_role->GetGuildID();
		if (INVALID_GUILD_ID != guild_id)
		{
			EventHandler::Instance().OnAddGuildRumorEvent(guild_id, m_role->GetName(), target_role->GetName(), GUILD_EVENT_TYPE_RUMOR_BABY, qifu_cfg->qifu_reward_baby_id);
		}

		// log
		gamelog::g_log_baby.printf(LL_INFO, "Baby::ReplyCreateBabyRequest user[%d %s] lover_uid[%d %s] unused_baby_index[%d] lover_unused_baby_index[%d] baby_id[%d] unique_id[%lld]",
				m_role->GetUID(), m_role->GetName(), target_role->GetUID(), target_role->GetName(), unused_baby_index, unused_baby_index_lover, m_param.baby_list[unused_baby_index].baby_id, unique_id);
	}
}

void Baby::RequestChaosheng()
{
	if (!m_role->IsMarry())
	{
		m_role->NoticeNum(errornum::EN_MARRY_NOT_MARRIED);
		return;
	}

	int lover_id = m_role->GetLoverUid();
	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(lover_id));
	if (NULL == target_role)
	{
		m_role->NoticeNum(errornum::EN_BABY_LOVER_NOT_ONLINE);
		return;
	}

	//if (!this->CheckBabyListIsFull())
	//{
	//	m_role->NoticeNum(errornum::EN_BABY_COUNT_NOT_FULL);
	//	return;
	//}

	if (m_param.chaosheng_count >= BABY_MAX_CHAOSHENG_COUNT)
	{
		m_role->NoticeNum(errornum::EN_BABY_CHAOSHENG_LIMIT);
		return;
	}

	int need_gold = LOGIC_CONFIG->GetBabyCfg().GetBabyChaoshengGold(m_param.chaosheng_count + 1);
	if (need_gold <= 0)
	{
		return;
	}

	// 检查
	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 消耗
	if (!m_role->GetKnapsack()->GetMoney()->UseGold(need_gold, "RequestChaosheng"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	// 增加超生名额
	this->AddChaoshengCount();
	target_role->GetBaby()->AddChaoshengCount();

	this->SendAllBabyInfo();
	target_role->GetBaby()->SendAllBabyInfo();
	
	m_role->NoticeNum(noticenum::NT_BABY_CHAOSHENG_SUCC);

	// 日志
	gamelog::g_log_baby.printf(LL_INFO, "Baby::RequestChaosheng user[%d %s] lover_uid[%d %s] user_chaosheng_count[%d] lover_chaosheng_count[%d] need_gold[%d]",
		m_role->GetUID(), m_role->GetName(), target_role->GetUID(), target_role->GetName(), m_param.chaosheng_count, target_role->GetBaby()->GetChaoshengCount(), need_gold);
}

bool Baby::TrainBabySpirit(int baby_index, int spirit_id, bool is_auto_buy)
{
	if (baby_index < 0 || baby_index >= BABY_MAX_COUNT || spirit_id < 0 || spirit_id >= BABY_SPIRIT_COUNT)
	{
		return false;
	}

	if (m_param.baby_list[baby_index].baby_id < 0)
	{
		return false;
	}

	if (m_param.baby_list[baby_index].lover_uid != m_role->GetLoverUid())
	{
		m_role->NoticeNum(errornum::EN_BABY_IS_OVERDUE);
		return false;
	}
	
	BabySpiritInfo &baby_spirit_info = m_param.baby_list[baby_index].baby_spirit_list[spirit_id];

	if (baby_spirit_info.spirit_level >= BABY_MAX_SPIRIT_LEVEL_LIMIT)
	{
		m_role->NoticeNum(errornum::EN_BABY_SPIRIT_LEVEL_FULL);
		return false;
	}

	const BabySpiritConfig* spirit_cfg = LOGIC_CONFIG->GetBabyCfg().GetBabySpiritCfg(spirit_id, baby_spirit_info.spirit_level + 1);
	if (NULL == spirit_cfg || NULL == ITEMPOOL->GetItem(spirit_cfg->consume_item) || spirit_cfg->train_val <= 0)
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
		stuff_list[stuff_count].item_id = spirit_cfg->consume_item;
		stuff_list[stuff_count].num = 1;
		stuff_list[stuff_count].buyable = is_auto_buy;

		++stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			m_role->NoticeNum(errornum::EN_BABY_UGRADE_STUFF_NOT_ENOUGHT);
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(m_role, consume_list, "Baby::TrainBabySpirit"))
		{
			return false;
		}
	}

	// 给予
	baby_spirit_info.spirit_train += 1;
	if (baby_spirit_info.spirit_train >= spirit_cfg->train_val)
	{
		// 升级成功
		++ baby_spirit_info.spirit_level;
		baby_spirit_info.spirit_train = 0;

		if (spirit_id == 0)            //文曲星
		{
			m_role->NoticeNum(noticenum::NT_BABY_SPIRIT_WEN_UPGRADE_SUCC);
		}
		else if (spirit_id == 1)       // 武曲星
		{
			m_role->NoticeNum(noticenum::NT_BABY_SPIRIT_WU_UPGRADE_SUCC);
		}
		else
		{
			m_role->NoticeNum(noticenum::NT_BABY_SPIRIT_UPGRADE_SUCC);
		}
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_BABY);
	}

	this->SendBabySpiritInfo(baby_index);

	// syn
	unsigned int nowtime = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_param.baby_list[baby_index].last_modity_time = nowtime;
	m_dirty_baby_index_list.insert(baby_index);

	// 日志
	gamelog::g_log_baby.printf(LL_INFO, "Baby::TrainBabySpirit user[%d %s] baby_index[%d] baby_id[%d] unique_id[%lld] spirit_id[%d] \
		spirit_level[%d] train_val[%d] consume_item_id[%d] is_auto_upgrade[%d] train_times[%d]", 
		m_role->GetUID(), m_role->GetName(), baby_index, m_param.baby_list[baby_index].baby_id, m_param.baby_list[baby_index].unique_id, spirit_id, 
		baby_spirit_info.spirit_level, baby_spirit_info.spirit_train, spirit_cfg->consume_item, (is_auto_buy ? 1 : 0), 1);

	return true;
}

void Baby::OnRemoveBabyReq(int baby_index)
{
	if (baby_index < 0 || baby_index >= BABY_MAX_COUNT)
	{
		return;
	}

	if (m_param.baby_list[baby_index].baby_id < 0)
	{
		return;
	}

	// 2019.03.01 子豪需求
	if (m_param.baby_list[baby_index].baby_id == TOP_TALENT_BABY_ID && m_role->IsMarry())
	{
		gstr::SendError(m_role, "remove_baby_limit");
		return;
	}

	if (m_param.baby_list[baby_index].lover_uid == m_role->GetLoverUid())
	{
		UserID lover_user_id = IntToUid(m_role->GetLoverUid());
		Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(lover_user_id);
		if (nullptr == lover_role)
		{
			m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
			return;
		}

		static Protocol::SCBabyBackInfo rcb;
		rcb.info_type = Protocol::BABY_INFO_TYPE_REMOVE_BABY_REQ;
		rcb.param_1 = baby_index;
		rcb.param_2 = m_param.baby_list[baby_index].unique_id;
		EngineAdapter::Instance().NetSend(lover_role->GetNetId(), (const char*)&rcb, sizeof(rcb));

		gstr::SendNotice(m_role, "wait_confirm");
	}
	else
	{
		this->RemoveBaby(baby_index);
	}
}

void Baby::OnRemoveBabyRet(int req_baby_index, long long baby_unique_id, bool is_agree)
{
	if (req_baby_index < 0 || req_baby_index >= BABY_MAX_COUNT)
	{
		return;
	}

	int my_baby_index = GetBabyIndex(baby_unique_id);
	if (my_baby_index < 0 || my_baby_index >= BABY_MAX_COUNT)
	{
		return;
	}

	UserID lover_user_id = IntToUid(m_role->GetLoverUid());
	Role *req_role = World::GetInstWorld()->GetSceneManager()->GetRole(lover_user_id);
	if (nullptr == req_role)
	{
		m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	if (is_agree)
	{
		req_role->GetBaby()->RemoveBaby(req_baby_index);
		this->RemoveBaby(my_baby_index);
	}
	else
	{
		gstr::SendError(req_role, "remove_baby_disagree");
	}
}

void Baby::RemoveBaby(int baby_index)
{
	if (baby_index < 0 || baby_index >= BABY_MAX_COUNT)
	{
		return;
	}

	if (m_param.baby_list[baby_index].baby_id < 0)
	{
		return;
	}

// 	if (m_param.baby_list[baby_index].lover_uid == m_role->GetLoverUid())
// 	{
// 		m_role->NoticeNum(errornum::EN_BABY_CAN_NOT_REMOVE_BABY_WHICH_AND_YOUR_LOVER);
// 		return;
// 	}

	BabyInfo &info = m_param.baby_list[baby_index];

	// 日志
	gamelog::g_log_baby.buff_printf("Baby::RemoveBaby user[%d %s] baby_index[%d] baby_id[%d] unique_id[%lld] lover_uid[%d] grade[%d] level[%d] bless[%d] name[%s] sprites[", 
		m_role->GetUID(), m_role->GetName(), baby_index, info.baby_id, info.unique_id, info.lover_uid, info.grade, info.level, info.bless, info.baby_name);

	for (int sp_index = 0; sp_index < BABY_SPIRIT_COUNT; ++ sp_index)
	{
		gamelog::g_log_baby.buff_printf("(level %d, train %d), ", (int)info.baby_spirit_list[sp_index].spirit_level, (int)info.baby_spirit_list[sp_index].spirit_train);
	}

	gamelog::g_log_baby.commit_buff(LL_INFO);

	info.Reset(); // 输出完日志再清理宝宝数据

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_BABY);
	this->SendBabyInfo(baby_index);
}

bool Baby::CheckBabyNeedOtherBabyGrade()
{
	const BabyOtherCfg &other_cfg = LOGIC_CONFIG->GetBabyCfg().GetOtherCfg();

	for (int i = 0; i < BABY_MAX_COUNT; ++ i)
	{
		if (m_param.baby_list[i].baby_id >= 0)
		{
			if (m_param.baby_list[i].lover_uid == m_role->GetLoverUid() && m_param.baby_list[i].grade < other_cfg.born_need_grade)
			{
				return false;
			}
		}
	}

	return true;
}

bool Baby::CheckBabyListIsFull()
{
	int baby_count = BABY_MAX_COUNT - BABY_MAX_CHAOSHENG_COUNT + m_param.chaosheng_count;
	if (baby_count <= 0)
	{
		return false;
	}

	for (int i = 0;i < baby_count; ++ i)
	{
		if (m_param.baby_list[i].baby_id < 0)
		{
			return false;
		}
	}
	return true;
}

int Baby::GetUnusedBabyIndex()
{
	for (int i = 0; i < BABY_MAX_COUNT; ++ i)
	{
		if (m_param.baby_list[i].baby_id < 0)
		{
			return i;
		}
	}

	return -1;
}

void Baby::AddChaoshengCount() 
{
	if (m_param.chaosheng_count < BABY_MAX_CHAOSHENG_COUNT)
	{
		++ m_param.chaosheng_count; 
	}
}

void Baby::SendBabyInfo(short baby_index)
{
	if (baby_index < 0 || baby_index >= BABY_MAX_COUNT)
	{
		return;
	}

	BabyInfo &babyinfo = m_param.baby_list[baby_index];

	static Protocol::SCBabyInfo bi;
	bi.baby_send.baby_index = baby_index;
	bi.baby_send.baby_id = babyinfo.baby_id;
	bi.baby_send.bless = babyinfo.bless;

	UserCacheNode *lover = UserCacheManager::Instance().GetUserNode(babyinfo.lover_uid);
	if (nullptr != lover)
	{
		lover->GetName(bi.baby_send.lover_name);
	}
	else
	{
		memset(bi.baby_send.lover_name, 0, sizeof(bi.baby_send.lover_name));
	}

	bi.baby_send.grade = babyinfo.grade;
	bi.baby_send.level = babyinfo.level;
	bi.baby_send.rename_times = babyinfo.rename_times;

	memcpy(bi.baby_send.baby_spirit_list, babyinfo.baby_spirit_list, sizeof(bi.baby_send.baby_spirit_list));

	F_STRNCPY(bi.baby_send.baby_name, babyinfo.baby_name, sizeof(GameName));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&bi, sizeof(bi));
}

void Baby::SendAllBabyInfo()
{
	Protocol::SCBabyAllInfo bai;
	for (int baby_index = 0; baby_index < BABY_MAX_COUNT; ++ baby_index)
	{
		BabyInfo &babyinfo = m_param.baby_list[baby_index];
		Protocol::BabySendInfo &scinfo = bai.baby_list[baby_index];

		scinfo.baby_index = baby_index;
		scinfo.baby_id = babyinfo.baby_id;
		scinfo.rename_times = babyinfo.rename_times;

		scinfo.grade = babyinfo.grade;
		scinfo.level = babyinfo.level;
		scinfo.bless = babyinfo.bless;

		memcpy(scinfo.baby_spirit_list, babyinfo.baby_spirit_list, sizeof(scinfo.baby_spirit_list));

		UserCacheNode *lover = UserCacheManager::Instance().GetUserNode(babyinfo.lover_uid);
		if (nullptr != lover)
		{
			lover->GetName(scinfo.lover_name);
		}
		else
		{
			memset(scinfo.lover_name, 0, sizeof(scinfo.lover_name));
		}

		F_STRNCPY(scinfo.baby_name, babyinfo.baby_name, sizeof(GameName));
	}

	bai.baby_chaosheng_count = m_param.chaosheng_count;

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&bai, sizeof(bai));
}

void Baby::SendBabySpiritInfo(int baby_index)
{
	if (baby_index < 0 || baby_index >= BABY_MAX_COUNT)
	{
		return;
	}

	Protocol::SCBabySpiritInfo bsi;

	bsi.baby_index = baby_index;

	memcpy(bsi.baby_spirit_list, m_param.baby_list[baby_index].baby_spirit_list, sizeof(bsi.baby_spirit_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&bsi, sizeof(bsi));
}

short Baby::GetBabyGrade(short baby_index)
{
	if (baby_index < 0 || baby_index >= BABY_MAX_COUNT)
	{
		return 0;
	}

	return m_param.baby_list[baby_index].grade;
}

short Baby::GetSpiritLevel(short baby_index, short spirit_id)
{
	if (baby_index < 0 || baby_index >= BABY_MAX_COUNT || spirit_id < 0 || spirit_id >= BABY_SPIRIT_COUNT)
	{
		return 0;
	}

	return m_param.baby_list[baby_index].baby_spirit_list[spirit_id].spirit_level;
}

void Baby::LoverRequestCreateBaby(int type)
{
	m_lover_request_create_baby_type = type;

	static Protocol::SCBabyBackInfo rcb;
	rcb.info_type = Protocol::BABY_INFO_TYPE_REQUESET_CREATE_BABY;
	rcb.param_1 = type;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&rcb, sizeof(rcb));
}

void Baby::CheckSyncCache()
{
	for (std::set<int>::iterator it = m_dirty_baby_index_list.begin(); it != m_dirty_baby_index_list.end(); ++ it)
	{
		int baby_index = *it;
		if (baby_index < 0 || baby_index >= BABY_MAX_COUNT)
		{
			continue;
		}

		BabyInfo &info = m_param.baby_list[baby_index];
		if (info.baby_id < 0)
		{
			continue;
		}

		if (BabyCacheManager::Instance().SyncBabyCache(info))
		{
			Role *lover = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(info.lover_uid));
			if (NULL != lover)
			{
				lover->GetBaby()->SyncBabyCacheInfo(info.unique_id);
			}
		}
	}

	m_dirty_baby_index_list.clear();
}

void Baby::SyncBabyCacheInfo(long long unique_id)
{
	int index = -1;
	for (int i = 0; i < BABY_MAX_COUNT; ++ i)
	{
		if (m_param.baby_list[i].unique_id == unique_id)
		{
			index = i;
			break;
		}
	}

	if (-1 == index)
	{
		return;
	}

	BabyCacheManager::Instance().SyncBabyCache(m_param.baby_list[index]);
	
	this->SendBabyInfo(index);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_BABY);
}
	
int Baby::GetBabyIndex(long long baby_unique_id)
{
	int index = -1;
	for (int i = 0; i < BABY_MAX_COUNT; ++i)
	{
		if (m_param.baby_list[i].unique_id == baby_unique_id)
		{
			index = i;
			break;
		}
	}

	return index;
}

void Baby::GiveBaby(short baby_id, const GameName name, long long unique_id, int index)
{
	m_param.baby_list[index].unique_id = unique_id;
	m_param.baby_list[index].baby_id = baby_id;
	/*m_param.baby_list[index].level = 1;*/
	F_STRNCPY(m_param.baby_list[index].baby_name, name, sizeof(GameName));
	m_param.baby_list[index].lover_uid = m_role->GetLoverUid();

	unsigned int nowtime = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_param.baby_list[index].last_modity_time = nowtime;

	this->SendAllBabyInfo();		// 方便客户端更新 发所有信息
}

void Baby::OnBabyWalk(int is_idle_status)
{
	Protocol::SCBabyWalk msg;
	msg.obj_id = m_role->GetId();
	msg.is_special_baby = 0;

	if (0 == is_idle_status)  // 不是闲置状态
	{
		msg.baby_index = -1;
		memset(msg.baby_name, 0, sizeof(GameName));
	}
	else
	{
		// 优先龙凤宝宝
		QingyuanGlobal * qingyuuan_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetUID());
		if (qingyuuan_global)
		{
			int type = -1;
			if (qingyuuan_global->GetRandActiveSpecialBaby(m_role, type))
			{
				msg.is_special_baby = 1;
				msg.baby_index = type;
				memset(msg.baby_name, 0, sizeof(GameName));
			}
		}

		if (0 == msg.is_special_baby)
		{
			std::vector<int> valid_baby_idx_vec;
			for (int idx = 0; idx < BABY_MAX_COUNT; ++idx)
			{
				if (m_param.baby_list[idx].baby_id != INVALID_BABY_ID)
				{
					valid_baby_idx_vec.push_back(idx);
				}
			}

			if (valid_baby_idx_vec.empty())
			{
				return;
			}

			int rand_vec_idx = RandomNum(valid_baby_idx_vec.size());
			int rand_idx = valid_baby_idx_vec[rand_vec_idx];
			if (rand_idx >= 0 && rand_idx < BABY_MAX_COUNT)
			{
				msg.baby_index = rand_idx;
				memcpy(msg.baby_name, m_param.baby_list[rand_idx].baby_name, sizeof(m_param.baby_list[rand_idx].baby_name));
			}
		}
	}

	m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void*)&msg, sizeof(msg));

}

