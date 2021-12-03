#include "qingyuan.hpp"
#include "obj/character/role.h"
#include "other/qingyuan/qingyuanconfig.hpp"
#include "config/qingyuanshengdicconfig.h"
#include "config/logicconfigmanager.hpp"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "gameworld/gameworld/gamecommon.h"
#include "global/team/team.hpp"
#include "item/itempool.h"
#include "other/route/mailroute.hpp"
#include "world.h"
#include "gstr.h"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "servercommon/noticenum.h"
#include "global/offlineregister/offlineregistermanager.hpp"
#include "global/usercache/usercache.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "other/event/eventhandler.hpp"
#include "scene/speciallogic/worldspecial/specialhunyan.hpp"
#include "engineadapter.h"
#include "globalconfig/globalconfig.h"
#include "npc/transmit/fbmanager.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "protocal/msgchatmsg.h"
#include "chat/chatmanager.h"
#include "item/knapsack.h"
#include "item/other/otheritem.hpp"
#include "friend/friendmanager.h"
#include "friend/teaminfo.hpp"
#include "other/capability/capability.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "global/globalrole/qingyuanglobal/qingyuanglobal.hpp"
#include "other/activedegree/activedegree.hpp"
#include "gameworld/global/randactivity/impl/randactivityperfectlover.hpp"
#include "gameworld/global/randactivity/randactivitymanager.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "global/globalrole/commondata/commondataglobal.hpp"
#include "gameworld/protocal/msgrole.h"
#include "other/magicalprecious/magicalpreciouscfg.hpp"
#include "other/magicalprecious/magicalprecious.hpp"

#include <queue>

Qingyuan::Qingyuan() : m_role(NULL), m_is_in_couple_halo_effect(false), m_next_check_couple_halo_effect_time(0),
m_marry_type(MARRY_INVALID), m_lover_id(0), m_shici_type(SHICI_INVALID), m_marry_req_id(0), m_is_sure(false), m_yuyue_hunyan_type(0), m_is_trans_to_xunyou_scene(false),
m_next_sync_aoi_timestamp(0), m_last_profess_time(0)
{
	memset(m_halo_level_list, 0, sizeof(m_halo_level_list));
}

Qingyuan::~Qingyuan()
{

}

void Qingyuan::Init(Role *role, const QingyuanParam &qy_param, int marry_type, int lover_id, int shici_type, int marry_req_id, char is_sure)
{
	m_role = role;
	m_param = qy_param;

	// 求婚缓存信息
	{
		m_marry_type = marry_type;
		m_lover_id = lover_id;
		m_shici_type = shici_type;
		m_marry_req_id = marry_req_id;
		m_is_sure = (is_sure == 1) ? true : false;
	}

	// 同步伴侣的购买副本次数
	if (m_role->IsMarry()) {
		UserCacheNode *lover_target_cache = UserCacheManager::Instance().GetUserNode(m_role->GetLoverUid());
		if (NULL != lover_target_cache)
		{
			if (m_role->GetCommonDataParam()->hunyan_buy_fb_join_times < lover_target_cache->hunyan_buy_fb_join_times)
			{
				m_role->GetCommonDataParam()->hunyan_buy_fb_join_times = lover_target_cache->hunyan_buy_fb_join_times;
			}
		}
	}

	if (m_param.equiped_couple_halo_type <= -1 || m_param.equiped_couple_halo_type >= LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanCoupleHaloMaxType() || m_param.equiped_couple_halo_type >= QINGYUAN_COUPLE_HALO_MAX_TYPE)
	{
		m_param.equiped_couple_halo_type = -1;
	}
	else
	{
		int couple_halo_type = m_param.equiped_couple_halo_type;
		const QingyuanCoupleHaloLevelConfig *cur_level_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanCoupleHaloLevelCfg(m_param.equiped_couple_halo_type, m_param.couple_halo_level_list[couple_halo_type]);
		if (NULL == cur_level_cfg || 1 != cur_level_cfg->is_active_image)
		{
			m_param.equiped_couple_halo_type = -1;
		}
	}

	if (0 == m_param.task_info_count)
	{
		this->ResetQingYuanShengDi(true);
	}
}

void Qingyuan::Init(const PersonProfessParam &profess_param)
{
	m_profess_to.clear();
	m_profess_from.clear();

	for (int i = 0; i < PERSON_PROFESS_MAX_NUM; i++)
	{
		// 我表白的
		if (profess_param.profess_to_items[i].profess_time > 0)
			m_profess_to.push_back(profess_param.profess_to_items[i]);

		// 表白我的
		if (profess_param.profess_from_items[i].profess_time > 0)
			m_profess_from.push_back(profess_param.profess_from_items[i]);
	}
}

void Qingyuan::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid != now_dayid)
	{
		m_param.is_fetch_bless_reward = 0;
		m_param.today_yanhui_get_diamonds = 0;
		m_param.today_yanhui_get_hunjie_stuffs = 0;
		m_param.today_yanhui_get_other_stuffs = 0;
		m_param.today_yanhui_get_exps = 0;
		m_param.today_hunyan_gather_day_max_stuff_num = 0;
		m_param.join_fb_times = 0;
		m_param.today_tuodan_times = 0;
		//m_param.today_hunyan_open_times = 0;
		m_param.today_hunyan_join_times = 0;
		m_param.today_saxianhua_times = 0;
		m_param.today_hunjie_uplevel_times = 0;
		m_param.today_hunyan_be_zhufu_max_stuff_num = 0;
		m_param.today_hunyan_zhufu_max_stuff_num = 0;

		m_param.wedding_yuyue_seq = -1;
		m_param.is_hunyan_fb_already_open = 0;
		
		if (m_role->IsMarry())
		{
			// 结婚后才每天重置次数
			m_param.is_have_hunyan_times = 1;
			m_param.cur_marry_stauts = QINGYUAN_MARRY_STATUS_WEDDING_YUYUE;
		}

		// 光环
		{
		}

		{
			// 巡游
			m_param.today_xunyou_sa_hongbao_count = 0;
			m_param.today_xunyou_sa_hongbao_buy_count = 0;
			m_param.today_xunyou_give_flower_count = 0;
			m_param.today_xunyou_give_flower_buy_count = 0;
			m_param.today_xunyou_gather_hongbao_count = 0;
			m_is_trans_to_xunyou_scene = false;
		}

		// 同步伴侣的购买副本次数
		UserCacheNode *lover_target_cache = UserCacheManager::Instance().GetUserNode(m_role->GetLoverUid());
		if (NULL != lover_target_cache)
		{
			if(lover_target_cache->last_buy_hunyan_fb_times_dayid != now_dayid)  // 伴侣最后购买婚宴副本次数的dayid不是今天的，需要清零
			{
				m_role->GetCommonDataParam()->hunyan_buy_fb_join_times = 0;
			}
			else
			{
				// 同步伴侣的购买副本次数
				UserCacheNode *lover_target_cache = UserCacheManager::Instance().GetUserNode(m_role->GetLoverUid());
				if (NULL != lover_target_cache)
				{
					m_role->GetCommonDataParam()->hunyan_buy_fb_join_times = lover_target_cache->hunyan_buy_fb_join_times;
				}
			}
		}
		else
		{
			m_role->GetCommonDataParam()->hunyan_buy_fb_join_times = 0;
		}

		this->SendBaseFBInfo();
	}

	this->ResetQingYuanShengDi();
}

void Qingyuan::SendQingYuanShengDiInfo()
{
	Protocol::SCQingYuanShengDiTaskInfo pro;
	pro.is_fetched_task_other_reward = (char)m_param.is_fetched_task_other_reward;
	pro.lover_is_all_task_complete = 0;

	QingyuanGlobal *qy_blobal = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetLoverUid());
	if (NULL != qy_blobal)
	{
		pro.lover_is_all_task_complete = (char)qy_blobal->GetIsAllTaskComplete();
	}

	pro.task_count = m_param.task_info_count;
	memcpy(pro.task_info_list, m_param.task_info_list, sizeof(pro.task_info_list));
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void Qingyuan::ResetQingYuanShengDi(bool is_init)
{
	m_param.is_fetched_task_other_reward = 0;
	m_param.task_info_count = 0;
	memset(m_param.task_info_list, 0, sizeof(m_param.task_info_list));

	int task_id_list[QING_YUAN_SHENG_DI_TASK_MAX_COUNT] = { 0 };

	int marry_level = 0;
	QingyuanGlobal *qy_blobal = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetUID());
	if (NULL != qy_blobal)
	{
		qy_blobal->SetIsAllTaskComplete(0);
		marry_level = qy_blobal->GetMarryLevel();
	}

	m_param.task_info_count = LOGIC_CONFIG->GetQinYuanShengDiCfg().GetQYSDTaskList(marry_level, task_id_list, QING_YUAN_SHENG_DI_TASK_MAX_COUNT);
	if (m_param.task_info_count > QING_YUAN_SHENG_DI_TASK_MAX_COUNT)
	{
		m_param.task_info_count = QING_YUAN_SHENG_DI_TASK_MAX_COUNT;
	}

	for (int i = 0; i < m_param.task_info_count; i++)
	{
		m_param.task_info_list[i].task_id = (unsigned short)task_id_list[i];
	}

	if (!is_init)
	{
		this->SendQingYuanShengDiInfo();
	}
}

void Qingyuan::OnQingYuanShengDiFecthTaskReward(int task_index)
{
	if (task_index < 0 || task_index >= QING_YUAN_SHENG_DI_TASK_MAX_COUNT)
	{
		return;
	}

	QingYuanShengDiTask &info = m_param.task_info_list[task_index];

	if (0 == info.task_id || 0 != info.is_fetched_reward)
	{
		m_role->NoticeNum(errornum::EN_QING_YUAN_SHENG_DI_TASK_FETCHED);
		return;
	}

	const QYSDTaskCfg *cfg = LOGIC_CONFIG->GetQinYuanShengDiCfg().GetQYSDTaskCfg(info.task_id);
	if (NULL == cfg || info.param < cfg->param1)
	{
		m_role->NoticeNum(errornum::EN_QING_YUAN_SHENG_DI_TASK_NOT_COMPLETE);
		return;
	}

	int item_count = 0;
	const ItemConfigData *item_list = NULL;
	if (MALE == m_role->GetSex())
	{
		item_count = cfg->male_item_count;
		item_list = cfg->male_reward_item_list;
	}
	else
	{
		item_count = cfg->female_item_count;
		item_list = cfg->female_reward_item_list;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	info.is_fetched_reward = 1;

	if (cfg->reward_exp > 0)
	{
		m_role->AddExp(cfg->reward_exp, "", Role::EXP_ADD_REASON_DEFAULT);
	}

	if (item_count > 0)
	{
		m_role->GetKnapsack()->PutList(item_count, item_list, PUT_REASON_QING_YUAN_SHENG_DI);
	}

	this->SendQingYuanShengDiInfo();
}

void Qingyuan::OnQingYuanShengDiFecthOtherReward()
{
	if (0 != m_param.is_fetched_task_other_reward)
	{
		m_role->NoticeNum(errornum::EN_QING_YUAN_SHENG_DI_TASK_FETCHED);
		return;
	}

	for (int i = 0; i < m_param.task_info_count && i < QING_YUAN_SHENG_DI_TASK_MAX_COUNT; i++)
	{
		QingYuanShengDiTask &info = m_param.task_info_list[i];
		const QYSDTaskCfg *cfg = LOGIC_CONFIG->GetQinYuanShengDiCfg().GetQYSDTaskCfg(info.task_id);
		if (NULL == cfg || info.param < cfg->param1)
		{
			m_role->NoticeNum(errornum::EN_QING_YUAN_SHENG_DI_TASK_NOT_COMPLETE);
			return;
		}
	}

	QingyuanGlobal *qy_blobal = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetLoverUid());
	if (NULL == qy_blobal || 0 == qy_blobal->GetIsAllTaskComplete())
	{
		m_role->NoticeNum(errornum::EN_QING_YUAN_SHENG_DI_TASK_NOT_COMPLETE);
		return;
	}

	int item_count = 0;
	const ItemConfigData *item_list = NULL;

	const QYSDOtherCfg *other_cfg = LOGIC_CONFIG->GetQinYuanShengDiCfg().GetQYSDOtherCfg();
	if (MALE == m_role->GetSex())
	{
		item_count = other_cfg->male_item_count;
		item_list = other_cfg->male_reward_item_list;
	}
	else
	{
		item_count = other_cfg->female_item_count;
		item_list = other_cfg->female_reward_item_list;
	}

	if (item_count > 0 && NULL != item_list)
	{
		if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(item_count))
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
			return;
		}

		m_param.is_fetched_task_other_reward = 1;

		m_role->GetKnapsack()->PutList(item_count, item_list, PUT_REASON_QING_YUAN_SHENG_DI);

		this->SendQingYuanShengDiInfo();
	}
}

void Qingyuan::OnQingYuanShengDiKillMonster(int task_type, int monster_id)
{
	bool is_change = false;
	int complete_count = 0;

	for (int i = 0; i < m_param.task_info_count; i++)
	{
		QingYuanShengDiTask &info = m_param.task_info_list[i];
		const QYSDTaskCfg *cfg = LOGIC_CONFIG->GetQinYuanShengDiCfg().GetQYSDTaskCfg(info.task_id);
		if (NULL == cfg || info.param >= cfg->param1)
		{
			complete_count++;
			continue;
		}

		if (cfg->task_type == task_type || (QYSD_TASK_TYPE_MONSTER_ID == cfg->task_type && cfg->param2 == monster_id))
		{
			info.param++;
			is_change = true;

			if (info.param >= cfg->param1)
			{
				complete_count++;
				m_role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_QINGYUAN_SHENGDI);
			}
				
		}
	}

	if (is_change)
	{
		if (complete_count >= m_param.task_info_count)
		{
			QingyuanGlobal *qy_blobal = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetUID());
			if (NULL != qy_blobal)
			{
				qy_blobal->SetIsAllTaskComplete(1);
			}

			Role *lover = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role->GetLoverUid()));
			if (NULL != lover)
			{
				lover->GetQingyuan()->SendQingYuanShengDiInfo();
			}
		}
		this->SendQingYuanShengDiInfo();
	}
}

void Qingyuan::OnQingYuanShengDiGether(int gather_id)
{
	bool is_change = false;
	int complete_count = 0;

	for (int i = 0; i < m_param.task_info_count; i++)
	{
		QingYuanShengDiTask &info = m_param.task_info_list[i];
		const QYSDTaskCfg *cfg = LOGIC_CONFIG->GetQinYuanShengDiCfg().GetQYSDTaskCfg(info.task_id);
		if (NULL == cfg || info.param >= cfg->param1)
		{
			complete_count++;
			continue;
		}

		if (QYSD_TASK_TYPE_GATHER == cfg->task_type || (QYSD_TASK_TYPE_GATHER_ID == cfg->task_type && cfg->param2 == gather_id))
		{
			info.param++;
			is_change = true;

			if (info.param >= cfg->param1) 
			{
				complete_count++;
				m_role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_QINGYUAN_SHENGDI);
			}
			
		}
	}

	if (is_change)
	{
		if (complete_count >= m_param.task_info_count)
		{
			QingyuanGlobal *qy_blobal = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetUID());
			if (NULL != qy_blobal)
			{
				qy_blobal->SetIsAllTaskComplete(1);
			}

			Role *lover = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role->GetLoverUid()));
			if (NULL != lover)
			{
				lover->GetQingyuan()->SendQingYuanShengDiInfo();
			}
		}
		this->SendQingYuanShengDiInfo();
	}
}

void Qingyuan::OnQingYuanShengDiSkipTask()
{
	const QYSDOtherCfg *other_cfg = LOGIC_CONFIG->GetQinYuanShengDiCfg().GetQYSDOtherCfg();
	if (NULL != other_cfg)
	{
		if (m_role->GetLevel() < other_cfg->skip_task_limit_level)
		{
			return;
		}

		int complete_count = 0;
		for (int i = 0; i < m_param.task_info_count && i < QING_YUAN_SHENG_DI_TASK_MAX_COUNT; i++)
		{
			QingYuanShengDiTask &info = m_param.task_info_list[i];
			const QYSDTaskCfg *cfg = LOGIC_CONFIG->GetQinYuanShengDiCfg().GetQYSDTaskCfg(info.task_id);
			if (NULL != cfg && info.param >= cfg->param1)
			{
				++complete_count;
			}
		}

		if (complete_count >= m_param.task_info_count || complete_count >= QING_YUAN_SHENG_DI_TASK_MAX_COUNT)
		{
			return;
		}

		if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg->skip_task_consume * (m_param.task_info_count - complete_count), "Skip QingYuanShengDiTask"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		for (int i = 0; i < m_param.task_info_count && i < QING_YUAN_SHENG_DI_TASK_MAX_COUNT; i++)
		{
			QingYuanShengDiTask &info = m_param.task_info_list[i];
			const QYSDTaskCfg *cfg = LOGIC_CONFIG->GetQinYuanShengDiCfg().GetQYSDTaskCfg(info.task_id);
			if (NULL != cfg)
			{
				info.param = cfg->param1;
				this->OnQingYuanShengDiFecthTaskReward(i);
				m_role->GetActiveDegree()->AddActiveDegreeHelper(ACTIVEDEGREE_TYPE_QINGYUAN_SHENGDI);
			}
		}

	}
}

void Qingyuan::AddBlessDays()
{
	if (!m_role->IsMarry())
	{
		m_role->NoticeNum(errornum::EN_CLOSE_BETA_HAS_NOT_MARRY);
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	int my_bless_days = m_role->GetQingyuan()->GetBlessDays();
	int lover_bless_days = 0;
	UserID lover_uid = IntToUid(m_role->GetLoverUid());

	OfflineRegisterManager::Instance().OnGetQingyuanBlessDays(lover_uid, &lover_bless_days);

	int differ_bless_days = 0;
	//int max_bless_days = 0;
	bool is_lover_min = false;

	if(my_bless_days <= lover_bless_days)
	{
		differ_bless_days = lover_bless_days - my_bless_days;
		//max_bless_days = lover_bless_days;
	}
	else
	{
		differ_bless_days = my_bless_days - lover_bless_days;
		is_lover_min = true;
		//max_bless_days = my_bless_days;
	}

	const int bless_price_gold = LOGIC_CONFIG->GetQingyuanCfg().getQingyuanBlessPriceGold();
	int per_day_price = (int)ceil( (float)(bless_price_gold / (2 * QINGYUAN_ADD_BLESS_DAYS_PER)) );

	int need_gold = 0;
	if (0 == my_bless_days && 0 == lover_bless_days)
	{
		my_bless_days = QINGYUAN_ADD_BLESS_DAYS_PER;
		lover_bless_days = QINGYUAN_ADD_BLESS_DAYS_PER;
		need_gold = bless_price_gold;
	}
	else if (my_bless_days == lover_bless_days)
	{
		m_role->NoticeNum(errornum::EN_MARRY_BLESS_DAYS_EQUAL);
		return;
	}
	else
	{
		if (differ_bless_days >= QINGYUAN_ADD_BLESS_DAYS_PER)
		{
			need_gold = per_day_price * QINGYUAN_ADD_BLESS_DAYS_PER;
			if (is_lover_min)
			{
				lover_bless_days += QINGYUAN_ADD_BLESS_DAYS_PER;
			}
			else
			{
				my_bless_days += QINGYUAN_ADD_BLESS_DAYS_PER;
			}
		}
		else
		{
			need_gold = per_day_price * differ_bless_days;
			if (is_lover_min)
			{
				lover_bless_days += differ_bless_days;
			}
			else
			{
				my_bless_days += differ_bless_days;
			}
		}
	}

	if (!knapsack->GetMoney()->AllGoldMoreThan(need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (knapsack->GetMoney()->UseGold(need_gold, "honeymoonbless"))
	{
		Role *lover_target_role = World::GetInstWorld()->GetSceneManager()->GetRole(lover_uid);
		if (NULL == lover_target_role)
		{
			OfflineRegisterManager::Instance().OnQingyuanBlessDaysChange(lover_uid, lover_bless_days);
		}
		else
		{
			lover_target_role->GetQingyuan()->SetBlessDays(lover_bless_days);
		}

		m_role->GetQingyuan()->SetBlessDays(my_bless_days);
	}

	this->SendBlessInfo();
	m_role->NoticeNum(noticenum::NT_MARRY_HONEYMOON_ADD_DAYS_SUCC);

	// 日志
	GameName lover_name;
	m_role->GetLoverName(lover_name);
	gamelog::g_log_marry.printf(LL_INFO, "AddBlessDays::user[%d, %s], lover user[%d, %s], Gold[%d] Per_day_price_gold[%d],AddBlessDays_Succ",
		m_role->GetUID(), m_role->GetName(),m_role->GetLoverUid(), lover_name, need_gold, per_day_price);
}

void Qingyuan::GetInitParam(QingyuanParam *qy_param)
{
	*qy_param = m_param;
}

void Qingyuan::GetInitParam(PersonProfessParam *profess_param)
{
	profess_param->Reset();
	// 更新我表白的
	int i = 0;
	for (auto& item : m_profess_to)
	{
		if (i >= PERSON_PROFESS_MAX_NUM)
		{
			break;
		}
		profess_param->profess_to_items[i] = item;
		i++;
	}

	// 更新表白我的
	i = 0;
	for (auto& item : m_profess_from)
	{
		if (i >= PERSON_PROFESS_MAX_NUM)
		{
			break;
		}
		profess_param->profess_from_items[i] = item;
		i++;
	}
}

void Qingyuan::GetRoleInitParam(int *marry_type, int *lover_id, int *shici_type, int *marry_req_id, char *is_sure)
{
	// 求婚缓存信息
	{
		*marry_type = m_marry_type;
		*lover_id = m_lover_id;
		*shici_type = m_shici_type;
		*marry_req_id = m_marry_req_id;
		*is_sure = (m_is_sure ? 1 : 0);
	}
}

void Qingyuan::Update(unsigned int now_second)
{
	// 光环
	{
		this->CheckCoupleHaloEffect(now_second);
		this->CheckTejiaHalo(now_second);
	}

	// 巡游
	{
		this->CheckTransToXunyouPos();
		this->CheckSyncAOIInXunyou(now_second);
	}
}

void Qingyuan::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		{
			const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(m_role->GetCommonDataParam()->hunjie_item_id);
			if (NULL != equip && ItemBase::I_TYPE_EQUIPMENT == equip->GetItemType() && Equipment::E_TYPE_SPECIAL_HUNJIE == equip->GetEquipType()) //是戒指武器
			{
				const EquipUpLevelConfig::EquipStarItemConfig *item_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetEquipStarCfg(m_role->GetCommonDataParam()->hunjie_item_id, m_role->GetCommonDataParam()->hunjie_star_level); //星级
				if (NULL != item_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, item_cfg->gongji);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU, item_cfg->fangyu);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, item_cfg->maxhp);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG, item_cfg->mingzhong);
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += GETPROFVALUE(m_role->GetProf(), CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI, item_cfg->shanbi);
				}
			}
		}

		{
			// 夫妻光环
			int couple_halo_max_type_count = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanCoupleHaloMaxType();
			for (int halo_type = 0; halo_type < couple_halo_max_type_count && halo_type < QINGYUAN_COUPLE_HALO_MAX_TYPE; ++halo_type)
			{
				const QingyuanCoupleHaloLevelConfig *couple_halo_level_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanCoupleHaloLevelCfg(halo_type, m_param.couple_halo_level_list[halo_type]);
				if (NULL != couple_halo_level_cfg)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += couple_halo_level_cfg->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += couple_halo_level_cfg->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += couple_halo_level_cfg->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += couple_halo_level_cfg->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += couple_halo_level_cfg->shanbi;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += couple_halo_level_cfg->baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += couple_halo_level_cfg->jianren;
				}
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_QINGYUAN, m_attrs_add);
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void Qingyuan::OnRoleLogin(unsigned int last_save_time)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	// 已经预约了婚宴时间
	if (0 == m_param.is_have_hunyan_times && m_param.hunyan_type > YUYUE_HUNYAN_INVALUE)
	{
		char seq = WorldStatus2::Instance().GetWeedingYuYueSeqByUID(m_role->GetUID());
		if (seq < 0 || seq >= WEDDING_YUYUE_TIME_MAX)
		{
			m_param.cur_marry_stauts = QINGYUAN_MARRY_STATUS_INVAILD;
			m_param.is_hunyan_fb_already_open = 0;
			m_param.wedding_yuyue_seq = -1;
			return;
		}

		unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		unsigned int zero_time = static_cast<unsigned int>(GetZeroTime(now));

		const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();

		const WeddingYuYueTimeCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingYuYueTimeBySeq(seq);
		if (NULL != cfg_item)
		{
			UInt32 hunyan_begin_time = HHMMToNow(zero_time, cfg_item->begin_time);
			UInt32 hunyan_end_time = hunyan_begin_time + hunyan_cfg.xunyou_interval_s + hunyan_cfg.start_s;

			if (now <= hunyan_begin_time)
			{
				m_param.cur_marry_stauts = QINGYUAN_MARRY_STATUS_INVITE_GUESTS;
			}
			else if (now <= hunyan_end_time)
			{
				m_param.cur_marry_stauts = QINGYUAN_MARRY_STATUS_WEDDING;
			}
			else
			{
				m_param.cur_marry_stauts = QINGYUAN_MARRY_STATUS_INVAILD;
				m_param.is_hunyan_fb_already_open = 0;
				m_param.wedding_yuyue_seq = -1;
			}
		}

		m_is_trans_to_xunyou_scene = false;
	}

	// 夫妻光环
	this->CheckTejiaHaloActivity(true, last_save_time);
}

void Qingyuan::OnEnterScene(int scene_id)
{

}

void Qingyuan::OnMarryOperate(int ope_type, int param1, int param2)
{
	switch (ope_type)
	{
		case Protocol::MARRY_REQ_TYPE_PROPOSE:
		{
			this->MarryReq(param1, param2);
		}
		break;

		case Protocol::MARRY_CHOSE_SHICI_REQ:
		{
			bool ret = this->ChoseShici(param1);
			if (ret)
			{
				this->SendMarryOperaRet(m_role, Protocol::MARRY_CHOSE_SHICI, m_shici_type);
			}
		}
		break;

		case Protocol::MARRY_PRESS_FINGER_REQ:
		{
			this->OnPressFinger();
		}
		break;

		default:
		break;
	}
}

void Qingyuan::MarryReq(int hunli_type, int target_uid)//求婚
{
	if (0 == target_uid || hunli_type < 0 || hunli_type >= QINGYUAN_HUNLI_MAX_TYPE)
	{
		return;
	}

	// 检查元宝
	const QingyuanHunliTypeConfig *hunli_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanHunliTypeConfig(hunli_type);
	if (NULL == hunli_cfg)
	{
		m_role->NoticeNum(errornum::EN_CONFIG_ERROR);
		return;
	}

	int real_consume_bind_gold = hunli_cfg->need_bind_gold;
	int real_consume_gold = hunli_cfg->need_gold;

	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_WEST_WEDDING))
	{
		if (hunli_cfg->hunli_style == QingyuanHunliTypeConfig::HUNLI_TYPE_STYLE_WEST)
		{
			real_consume_gold = hunli_cfg->activity_price;
		}
	}

	if (real_consume_bind_gold > 0)
	{
		if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(real_consume_bind_gold))
		{
			m_role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
			return;
		}
	}

	if (real_consume_gold > 0)
	{
		if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(real_consume_gold))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}
	}

	Role *marry_target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(target_uid));
	if (NULL == marry_target_role)
	{
		m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
	if (m_role->GetFriendManager()->GetFriendIntimacy(marry_target_role->GetUserId()) < other_cfg.marry_limit_intimacy)
	{
		//m_role->NoticeNum(errornum::EN_MARRY_INTIMACY_NOT_ENOUGH);
		//return;
	}

	// 欧式婚礼必须是求过结婚的
	if (QingyuanHunliTypeConfig::HUNLI_TYPE_STYLE_WEST == hunli_cfg->hunli_style)
	{
		if (!m_role->IsMarry())
		{
			gstr::SendError(m_role, "qingyuan_marry_never_proposed"); // 还有没有求过婚
			return;
		}
	}

	// 如果未结婚
	if (!m_role->IsMarry())
	{
		if (m_role->GetSex() == marry_target_role->GetSex())
		{
			m_role->NoticeNum(errornum::EN_MARRY_MUST_DIFFERENT_SEX_IN_TEAM);
			return;
		}

		if (m_role->IsMarry() || marry_target_role->IsMarry())
		{
			m_role->NoticeNum(errornum::EN_MARRY_HAS_BEEN_MARRY);
			return;
		}

		if (m_role->GetLevel() < other_cfg.marry_limit_level || marry_target_role->GetLevel() < other_cfg.marry_limit_level)//等级限制
		{
			m_role->NoticeNum(errornum::EN_MARRY_LEVEL_NOT_ENOUGH);
			return;
		}

		if (GetDayIndex(m_role->GetLastDivorceTime(), EngineAdapter::Instance().Time()) < other_cfg.remarry_limit_days)//时间限制
		{
			m_role->NoticeNum(errornum::EN_QINGYUAN_REMARRY_DAYS_LIMIT);
			return;
		}

		if (GetDayIndex(marry_target_role->GetLastDivorceTime(), EngineAdapter::Instance().Time()) < other_cfg.remarry_limit_days) //求婚者再婚间隔
		{
			m_role->NoticeNum(errornum::EN_QINGYUAN_ANOTHER_REMARRY_LIMIT_DAYS);
			return;
		}
	}
	// 重复求婚
	else
	{
		if (m_role->GetLoverUid() != target_uid || m_role->GetLoverUid() != marry_target_role->GetUID())
		{
			gstr::SendError(m_role, "qingyuan_marry_only_lover");  // 必须是伴侣
			return;
		}

		// 欧式婚礼流程
		if (QingyuanHunliTypeConfig::HUNLI_TYPE_STYLE_WEST == hunli_cfg->hunli_style)
		{
			if (m_role->GetLevel() < other_cfg.marry_limit_level || marry_target_role->GetLevel() < other_cfg.marry_limit_level)//等级限制
			{
				m_role->NoticeNum(errornum::EN_MARRY_LEVEL_NOT_ENOUGH);
				return;
			}

			{
				// 检查
				//if (marry_target_role->GetKnapsack()->IsLock())
				//{
				//	marry_target_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
				//	return;
				//}

				if (m_role->GetKnapsack()->IsLock())
				{
					m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
					return;
				}

				// 扣除元宝
				{
					//if (!m_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(real_consume_bind_gold + real_consume_gold))
					//{
					//	m_role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
					//	return;
					//}
					//
					//if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(real_consume_gold))
					//{
					//	m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					//	return;
					//}

					if (real_consume_bind_gold > 0)
					{
						if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(real_consume_bind_gold, "MarryReqWestStyleUseAllGold"))
						{
							m_role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
							return;
						}
					}

					if (real_consume_gold > 0)
					{
						if (!m_role->GetKnapsack()->GetMoney()->UseGold(real_consume_gold, "MarryReqWestStyleUseGold"))
						{
							m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
							return;
						}
					}
				}
			}

			// 欧式婚礼直接给双方礼包，不在走契约流程
			this->GiveMarryReward(hunli_type);
			marry_target_role->GetQingyuan()->GiveMarryReward(hunli_type);
			
			// 欧式求婚成功提示，然后直接返回求婚流程
			gstr::SendNotice(m_role, "qingyuan_marry_propose_succ");
			marry_target_role->GetQingyuan()->SetMarrReqUid(m_role->GetUID());
			return;
		}
	}

	m_role->NoticeNum(noticenum::NT_MARRY_REQ_SUCC);
	marry_target_role->GetQingyuan()->SetMarrReqUid(m_role->GetUID());

	static Protocol::SCMarryReqRoute mrr;
	mrr.hunli_type = hunli_type;
	mrr.req_uid = UidToInt(m_role->GetUserId());
	m_role->GetName(mrr.req_name);
	EngineAdapter::Instance().NetSend(marry_target_role->GetNetId(), (const char*)&mrr, sizeof(mrr));
}

void Qingyuan::MarryRet(int hunli_type, int req_uid, bool is_accept)
{
	if (hunli_type < 0 || hunli_type >= QINGYUAN_HUNLI_MAX_TYPE) return;

	const QingyuanHunliTypeConfig *hunli_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanHunliTypeConfig(hunli_type);
	if (NULL == hunli_cfg)
	{
		return;
	}

	Role *marry_target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(req_uid));
	if (NULL == marry_target_role)
	{
		this->ResetAllData();
		if (is_accept) m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	if (req_uid != m_param.marry_req_uid)
	{
		return;
	}

	if (m_role->GetSex() == marry_target_role->GetSex())
	{
		m_role->NoticeNum(errornum::EN_MARRY_MUST_DIFFERENT_SEX_IN_TEAM);
		return;
	}

	// 如果未结婚
	if (!m_role->IsMarry())
	{
		if (m_role->IsMarry() || marry_target_role->IsMarry())
		{
			m_role->NoticeNum(errornum::EN_MARRY_HAS_BEEN_MARRY);
			return;
		}

		const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
		if (m_role->GetLevel() < other_cfg.marry_limit_level || marry_target_role->GetLevel() < other_cfg.marry_limit_level)//检查等级
		{
			m_role->NoticeNum(errornum::EN_MARRY_LEVEL_NOT_ENOUGH);
			return;
		}

		if (m_role->GetFriendManager()->GetFriendIntimacy(marry_target_role->GetUserId()) < other_cfg.marry_limit_intimacy)//结婚的亲密度限制
		{
			//m_role->NoticeNum(errornum::EN_MARRY_INTIMACY_NOT_ENOUGH);
			//return;
		}

		if (GetDayIndex(m_role->GetLastDivorceTime(), EngineAdapter::Instance().Time()) < other_cfg.remarry_limit_days)//自己再婚间隔
		{
			m_role->NoticeNum(errornum::EN_QINGYUAN_REMARRY_DAYS_LIMIT);
			return;
		}

		if (GetDayIndex(marry_target_role->GetLastDivorceTime(), EngineAdapter::Instance().Time()) < other_cfg.remarry_limit_days)//求婚者再婚间隔
		{
			m_role->NoticeNum(errornum::EN_QINGYUAN_ANOTHER_REMARRY_LIMIT_DAYS);
			return;
		}
	}
	//重复被求婚
	else
	{
		if (m_role->GetLoverUid() != req_uid || m_role->GetLoverUid() != marry_target_role->GetUID())
		{
			return;
		}
	}

	if (is_accept)
	{
		//暂时记录下对象的id,发起者id，结婚类型
		m_lover_id = req_uid;
		m_marry_req_id = req_uid;
		m_marry_type = hunli_type;
		//设置对方数据
		marry_target_role->GetQingyuan()->SetLoverId(m_role->GetUID());
		marry_target_role->GetQingyuan()->SetMarryReqId(req_uid);
		marry_target_role->GetQingyuan()->SetMarryType(hunli_type);

		Protocol::SCIsAcceptMarry am;
		am.is_accept = 1;
		EngineAdapter::Instance().NetSend(marry_target_role->GetNetId(), (const char*)&am, sizeof(am));

		{
			// 日志
			gamelog::g_log_marry.printf(LL_INFO, "MarryRet::user[%d, %s], target_user[%d, %s], marry_succ",
				m_role->GetUID(), m_role->GetName(), marry_target_role->GetUID(), marry_target_role->GetName());
		}
	}
	else
	{
		//拒绝就清空两个人的所有数据
		this->ResetAllData();
		marry_target_role->GetQingyuan()->ResetAllData();

		Protocol::SCIsAcceptMarry am;
		am.is_accept = 0;
		EngineAdapter::Instance().NetSend(marry_target_role->GetNetId(), (const char*)&am, sizeof(am));
	}
}

void Qingyuan::MarryNotice(Role * marry_target)
{	
	if (NULL == marry_target) return;

	Role * role_male = NULL;
	Role * role_female = NULL;

	if (MALE == marry_target->GetSex())
	{
		role_male = marry_target;
		role_female = m_role;
	}
	else
	{
		role_male = m_role;
		role_female = marry_target;
	}

	if (NULL == role_male || NULL == role_female)
	{
		return;
	}

	static Protocol::SCMarryNotic pro;
	pro.uid1 = role_male->GetUID();
	role_male->GetName(pro.name1);
	pro.avator_timestamp1 = role_male->GetAvatarTimeStamp();
	pro.prof1 = role_male->GetProf();

	pro.uid2 = role_female->GetUID();
	role_female->GetName(pro.name2);
	pro.avator_timestamp2 = role_female->GetAvatarTimeStamp();
	pro.prof2 = role_female->GetProf();

	pro.server_marry_times = WorldStatus::Instance().GetCommonData().marry_all_times;
	
	World::GetInstWorld()->SendToAllGateway((const char*)&pro, sizeof(pro));
}

void Qingyuan::SendMarryZhuhe(int uid, int type)
{
	if (WorldStatus::Instance().GetCommonData().marry_zhufu_count <= 0)
	{
		return;
	}

	WorldStatus::Instance().GetCommonData().marry_zhufu_count --;

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	Role * target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (NULL == target_role) return;

	Role * target_role_lover = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(target_role->GetLoverUid()));
	if (NULL == target_role_lover) return;

	static char buff[1024]; memset(buff, 0, sizeof(buff));

	Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)buff;
	cc->channel_type = chatdef::CHANNEL_TYPE_WORLD;
	
	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
	if (Protocol::MARRY_ZHUHE_TYPE0 == type)
	{
		m_role->GetKnapsack()->PutOrMail(other_cfg.marry_zhufu_fetch_item, PUT_REASON_MARRY_ZHUFU_REWARD);
		int rand_str = RandomNum(7);
		if (0 == rand_str)
		{
			cc->msg_length = SNPRINTF(buff + sizeof(Protocol::CSChannelChat), sizeof(buff) - sizeof(Protocol::CSChannelChat), gamestring::g_marry_zhufu1);
		}
		else if(1 == rand_str)
		{
			cc->msg_length = SNPRINTF(buff + sizeof(Protocol::CSChannelChat), sizeof(buff) - sizeof(Protocol::CSChannelChat), gamestring::g_marry_zhufu2);
		}
		else if(2 == rand_str)
		{
			cc->msg_length = SNPRINTF(buff + sizeof(Protocol::CSChannelChat), sizeof(buff) - sizeof(Protocol::CSChannelChat), gamestring::g_marry_zhufu3);
		}
		else if (3 == rand_str)
		{
			cc->msg_length = SNPRINTF(buff + sizeof(Protocol::CSChannelChat), sizeof(buff) - sizeof(Protocol::CSChannelChat), gamestring::g_marry_zhufu4);
		}
		else if (4 == rand_str)
		{
			cc->msg_length = SNPRINTF(buff + sizeof(Protocol::CSChannelChat), sizeof(buff) - sizeof(Protocol::CSChannelChat), gamestring::g_marry_zhufu5);
		}
		else if (5 == rand_str)
		{
			cc->msg_length = SNPRINTF(buff + sizeof(Protocol::CSChannelChat), sizeof(buff) - sizeof(Protocol::CSChannelChat), gamestring::g_marry_zhufu6);
		}
		else if (6 == rand_str)
		{
			cc->msg_length = SNPRINTF(buff + sizeof(Protocol::CSChannelChat), sizeof(buff) - sizeof(Protocol::CSChannelChat), gamestring::g_marry_zhufu7);
		}
	}
	else if(Protocol::MARRY_ZHUHE_TYPE1 == type)
	{
		if (!m_role->GetKnapsack()->GetMoney()->UseAllGold(other_cfg.marry_songhua_consume_glod, "SendMarryZhuhe_Songhua"))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		m_role->GetKnapsack()->Put(other_cfg.marry_songhua_fetch_item, PUT_REASON_MARRY_SONGHUA_REWARD);

		target_role->GetFriendManager()->AddAllCharm(other_cfg.marry_songhua_fetch_meili);
		target_role_lover->GetFriendManager()->AddAllCharm(other_cfg.marry_songhua_fetch_meili);
		
		cc->msg_length = SNPRINTF(buff + sizeof(Protocol::CSChannelChat), sizeof(buff) - sizeof(Protocol::CSChannelChat), gamestring::g_marry_songhua);
	}

	if (0 == cc->msg_length)
	{
		return;
	}

	ChatManager::Instance().OnChannelChat(m_role, cc, cc->msg_length + sizeof(Protocol::CSChannelChat), chatdef::ORIGIN_TYPE_NORMAL_CHAT);
	
	Protocol::SCMarryZhuheShou pro_shou;
	pro_shou.uid = m_role->GetUID();
	m_role->GetName(pro_shou.name);
	pro_shou.type = type;

	EngineAdapter::Instance().NetSend(target_role->GetNetId(), (const char*)&pro_shou, sizeof(pro_shou));
	EngineAdapter::Instance().NetSend(target_role_lover->GetNetId(), (const char*)&pro_shou, sizeof(pro_shou));
}

void Qingyuan::GiveMarryReward(int hunli_type)
{
//	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
	const QingyuanHunliTypeConfig * hunli_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanHunliTypeConfig(hunli_type);
	if (NULL == hunli_cfg) return;

	static MailContentParam contentparam; contentparam.Reset();
//	int index = 0;

	// 称号
	//m_role->GetTitleManager()->AddTitle(hunli_cfg->title_id, true, true, true, true);

	// 策划需求，每次都发婚戒
// 	if (0 == this->GetMarryCount())
// 	{
// 		const ItemBase *item_base = ITEMPOOL->GetItem(other_cfg.hunjie_item.item_id);
// 		if (NULL == item_base) return;
// 		contentparam.item_list[index].item_id = other_cfg.hunjie_item.item_id;
// 		contentparam.item_list[index].num = other_cfg.hunjie_item.num;
// 		contentparam.item_list[index].is_bind = (other_cfg.hunjie_item.is_bind) ? 1 : 0;
// 		contentparam.item_list[index].invalid_time = item_base->CalInvalidTime();
// 		contentparam.item_list[index].has_param = 1;
// 		contentparam.item_list[index].param_data.star_level = 1; //默认1
// 		
// 		index++;
//	}

	// 每种婚礼奖励一次
	int old_has_marry_huili_flag = m_param.has_marry_huili_type_flag;

	bool has_marry_hunli_type = IS_BIT_SET(m_param.has_marry_huili_type_flag, hunli_type);
	if (!has_marry_hunli_type)
	{
		// 奖励物品
		gamecommon::SendMultiMailToRole(m_role->GetUserId(), hunli_cfg->reward_item_list, hunli_cfg->reward_count, NULL, true, gamestring::g_qingyuan_marry_reward_content);

		SET_BIT(m_param.has_marry_huili_type_flag, hunli_type);
	}
	
	this->AddMarryCount();

	this->SendWeddingInfo(Protocol::QINGYUAN_INFO_TYPE_ROLE_INFO);

	gamelog::g_log_qingyuan.printf(LL_INFO, "Qingyuan::GiveMarryReward User[%d %s] old_has_marry_huili_flag[%d] now_has_marry_hunli_flag[%d]",
		m_role->GetUID(), m_role->GetName(), old_has_marry_huili_flag, m_param.has_marry_huili_type_flag);
}

void Qingyuan::DivorceReq(int is_forced_divorce)
{
	if (!m_role->IsMarry())
	{
		m_role->NoticeNum(errornum::EN_MARRY_NOT_MARRIED);
		return;
	}

	if (FBManager::Instance().IsHunyanRunning(m_role))
	{
		m_role->NoticeNum(errornum::EN_QINGYUAN_DIVORCE_LIMIT);
		return;
	}

	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();

	unsigned int last_marry_time = m_role->GetLastMarryTime();
	int day_index = GetDayIndex(last_marry_time, EngineAdapter::Instance().Time());
	if (day_index < other_cfg.divorce_limit_days)
	{
		m_role->NoticeNum(errornum::EN_QINGYUAN_DIVORCE_DAYS_LIMIT);
		return;
	}

	if (is_forced_divorce)
	{
		// 强制离婚
		if (m_role->GetKnapsack()->IsLock())
		{
			m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
			return;
		}

		if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg.divorce_coin_cost))
		{
			m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
			return;
		}

		if (m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg.divorce_coin_cost, "Divorce"))
		{
			//发离婚的邮件和传闻
			{
				GameName lover_name = {0};
				m_role->GetLoverName(lover_name);
				{
					//邮件发给自己
					static MailContentParam contentparam; contentparam.Reset();

					int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_qingyuan_divorce_mail_subject);
					int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_qingyuan_divorce_mail_content, lover_name);

					if (length1 > 0 && length2 > 0)
					{
						MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
					}
				}

				{
					// 邮件发给伴侣
					static MailContentParam contentparam; contentparam.Reset();

					int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_qingyuan_divorce_mail_subject);
					int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_qingyuan_divorce_mail_content, m_role->GetName());

					if (length1 > 0 && length2 > 0)
					{
						MailRoute::MailToUser(IntToUid(m_role->GetLoverUid()), SYSTEM_MAIL_REASON_INVALID, contentparam);
					}
				}

				{
					// 传闻
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_qingyuan_divorce_notice, m_role->GetName(), lover_name);
					if (length > 0) 
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
					}
				}

				//日志
				gamelog::g_log_marry.printf(LL_INFO, "DivorceReq::user[%d, %s] lover_user[%d, %s] divorce_succ", m_role->GetUID(), m_role->GetName(), m_role->GetLoverUid(), lover_name);
			}

			//设置离婚
			{
				Protocol::SCMarryInformation di;
				memset(di.lover_name, 0 ,sizeof(di.lover_name));
				di.lover_uid = 0;	
				EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&di, sizeof(di));

				unsigned int last_divorce_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				UserID target_userid = IntToUid(m_role->GetLoverUid());
				Role *target = World::GetInstWorld()->GetSceneManager()->GetRole(target_userid);
				if (NULL != target)
				{
					EngineAdapter::Instance().NetSend(target->GetNetId(), (const char*)&di, sizeof(di));
					target->GetQingyuan()->OnDivorce(last_divorce_time);
				}
				else
				{
					OfflineRegisterManager::Instance().OnDivorceReq(target_userid, last_divorce_time);
				}

				this->OnDivorce(last_divorce_time);
			}
		}

		return;
	}

	int lover_uid = m_role->GetLoverUid();
	
	Role *divorce_target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(lover_uid));
	if (NULL == divorce_target_role)
	{
		m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	m_role->NoticeNum(noticenum::NT_DIVORCE_REQ_SUCC);

	static Protocol::SCDivorceReqRoute drr;
	drr.req_uid = UidToInt(m_role->GetUserId());
	m_role->GetName(drr.req_name);

	EngineAdapter::Instance().NetSend(divorce_target_role->GetNetId(), (const char*)&drr, sizeof(drr));

	return;
}

void Qingyuan::DivorceRet(int req_uid, bool is_accept)
{
	int lover_uid = m_role->GetLoverUid();

	if (0 == req_uid || lover_uid != req_uid)
	{
		return;
	}

	Role *divorce_target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(lover_uid));
	if (NULL == divorce_target_role)
	{
		if (is_accept) m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	if (is_accept)
	{
		//发离婚的邮件和传闻
		{
			GameName lover_name = {0};
			m_role->GetLoverName(lover_name);
			{
				// 邮件发给自己
				static MailContentParam contentparam; contentparam.Reset();

				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_qingyuan_divorce_mail_subject);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_qingyuan_divorce_mail_content, lover_name);

				if (length1 > 0 && length2 > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}

			{
				// 邮件发给伴侣
				static MailContentParam contentparam; contentparam.Reset();

				int length1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_qingyuan_divorce_mail_subject);
				int length2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_qingyuan_divorce_mail_content, m_role->GetName());

				if (length1 > 0 && length2 > 0)
				{
					MailRoute::MailToUser(IntToUid(m_role->GetLoverUid()), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}

			{
				// 传闻
				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_qingyuan_divorce_notice, m_role->GetName(), lover_name);
				if (length > 0) 
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
				}
			}

			//日志
			gamelog::g_log_marry.printf(LL_INFO, "DivorceReq::user[%d, %s] lover_user[%d, %s] divorce_succ", m_role->GetUID(), m_role->GetName(), m_role->GetLoverUid(), lover_name);
		}

		// 设置离婚
		{
			unsigned int last_divorce_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			UserID target_userid = IntToUid(m_role->GetLoverUid());
			Role *target = World::GetInstWorld()->GetSceneManager()->GetRole(target_userid);
			if (NULL != target)
			{
				target->GetQingyuan()->OnDivorce(last_divorce_time);

				//江湖传闻
				{
					if (m_role->GetSex() == FEMALE)
					{
						GuildID guild_id = m_role->GetGuildID();
						if (INVALID_GUILD_ID != guild_id)
						{
							if (guild_id == target->GetGuildID())
							{
								EventHandler::Instance().OnAddGuildRumorEvent(guild_id, target->GetName(), m_role->GetName(), GUILD_EVENT_TYPE_RUMOR_DIVORCE);
							}
							else
							{
								EventHandler::Instance().OnAddGuildRumorEvent(guild_id, m_role->GetName(), target->GetName(), GUILD_EVENT_TYPE_RUMOR_DIVORCE);

								if (target->GetGuildID() != INVALID_GUILD_ID)
								{
									EventHandler::Instance().OnAddGuildRumorEvent(target->GetGuildID(), target->GetName(), m_role->GetName(), GUILD_EVENT_TYPE_RUMOR_DIVORCE);
								}
							}

						}
						else
						{
							if (target->GetGuildID() != INVALID_GUILD_ID)
							{
								EventHandler::Instance().OnAddGuildRumorEvent(target->GetGuildID(), target->GetName(), m_role->GetName(), GUILD_EVENT_TYPE_RUMOR_DIVORCE);
							}
						}
					}
				}
			}
			else
			{
				OfflineRegisterManager::Instance().OnDivorceReq(target_userid, last_divorce_time);
			}

			Protocol::SCMarryInformation di;
			memset(di.lover_name, 0 ,sizeof(di.lover_name));
			di.lover_uid = 0;
			EngineAdapter::Instance().NetSend(divorce_target_role->GetNetId(), (const char*)&di, sizeof(di));
			EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&di, sizeof(di));
			this->OnDivorce(last_divorce_time);
		}
	}
	else
	{
		divorce_target_role->NoticeNum(errornum::EN_DIVORCE_RET_NOT_ACCEPT);
		return;
	}
}

void Qingyuan::OnDivorce(unsigned int divorce_time)
{
	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();

	// 减少情缘值、好友亲密度
	this->AddQingyuanValue(-other_cfg.divorce_qingyuan_dec);
	m_role->GetFriendManager()->IntimacyIncreaseHelper(IntToUid(m_role->GetLoverUid()), -other_cfg.divorce_intimacy_dec, false, "qingyuanDivorce", false);

	m_role->SetDivorce(divorce_time);
	{
		//清除相关标记、状态
		m_param.marry_type = -1;
//		m_param.has_marry_huili_type_flag = 0; // 结婚奖励标记离婚也不清除
		m_param.cur_marry_stauts = QINGYUAN_MARRY_STATUS_INVAILD;
		m_param.is_have_hunyan_times = 0;
		m_param.is_hunyan_fb_already_open = 0;

		m_param.today_hunyan_open_times = 0;
		m_param.can_open_hunyan = 0;
	}

	EventHandler::Instance().OnDivorce(m_role);
}

void Qingyuan::SendBaseFBInfo()
{
	Protocol::SCQingyuanBaseFBInfo qyi;
	qyi.buy_fb_join_times = m_role->GetCommonDataParam()->hunyan_buy_fb_join_times;
	qyi.join_fb_times = m_param.join_fb_times;
	qyi.qingyuan_value = m_param.qingyuan_value;
	qyi.is_hunyan_already_open = m_param.is_hunyan_fb_already_open;
	qyi.yanhui_fb_key = m_param.yanhui_fb_key;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&qyi, sizeof(qyi));
}

void Qingyuan::SetHunyanFBOpenStatus(int is_open)
{
	m_param.is_hunyan_fb_already_open = is_open;

	this->SendBaseFBInfo();
}

bool Qingyuan::CanJoinFB(bool is_leader)
{
	if (!m_role->GetRoleStatusManager()->CanEnterFB(true, false))
	{
		return false;
	}
	
	if (is_leader)
	{
		const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
		if (m_param.join_fb_times >= m_role->GetCommonDataParam()->hunyan_buy_fb_join_times + other_cfg.fb_free_times_limit)
		{
			return false;
		}
	}

	return true;
}

bool Qingyuan::CanJoinHunyanFB()
{	
	if (!m_role->GetRoleStatusManager()->CanEnterFB(true, false))
	{
		return false;
	}

	if (this->IsHunyanFBAlreadyOpen()) // 已经开始过了婚宴
	{
		m_role->NoticeNum(errornum::EN_HUNYAN_ALREADY_START);

		return false;
	}

	return true;
}

void Qingyuan::BuyFBTimesReq()
{
	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();

	if (m_role->GetCommonDataParam()->hunyan_buy_fb_join_times >= other_cfg.fb_buy_times_limit)
	{
		m_role->NoticeNum(errornum::EN_QINGYUAN_MAX_BUY_COUNT);
		return;
	}

	if (m_role->GetKnapsack()->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	//if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg.fb_buy_times_gold_cost))
	//{
	//	m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
	//	return;
	//}

	int need_consume_gold = 0;

	switch (m_role->GetCommonDataParam()->hunyan_buy_fb_join_times)
	{
		case QINGYUAN_FB_JOIN_TIMES_ONE:
		{
			if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg.first_buy_cost))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
			else 
			{
				need_consume_gold = other_cfg.first_buy_cost;
			}
		}
		break;

		case QINGYUAN_FB_JOIN_TIMES_TWO:
		{
			if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg.second_buy_cost))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
			else
			{
				need_consume_gold = other_cfg.second_buy_cost;
			}
		}
		break;

		case QINGYUAN_FB_JOIN_TIMES_THREE:
		{
			if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg.third_buy_cost))
			{
				m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
				return;
			}
			else
			{
				need_consume_gold = other_cfg.third_buy_cost;
			}
		}
		break;
	}

	//if (m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg.fb_buy_times_gold_cost, "qingyuan_fb"))
	if (m_role->GetKnapsack()->GetMoney()->UseGold(need_consume_gold, "qingyuan_fb"))
	{
		m_role->GetCommonDataParam()->hunyan_buy_fb_join_times++;
		m_role->GetCommonDataParam()->last_buy_hunyan_fb_times_dayid = EngineAdapter::Instance().DayID();
		m_role->UpdateUserCache();
		
		// 同步伴侣的购买副本次数
		UserID lover_userid = IntToUid(m_role->GetLoverUid());
		Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(lover_userid);
		if (NULL != lover_role)
		{
			lover_role->GetCommonDataParam()->hunyan_buy_fb_join_times++;
			lover_role->UpdateUserCache();
			lover_role->GetQingyuan()->SendBaseFBInfo();
		}
	}

	m_role->NoticeNum(noticenum::NT_QINGYUAN_BUY_FB_TIMES_SUCC);

	this->SendBaseFBInfo();

	//m_role->GetRoleActivity()->AddTurntableScore(other_cfg.fb_buy_times_gold_cost);
	m_role->GetRoleActivity()->AddTurntableScore(need_consume_gold);
}

void Qingyuan::OnBuyDoubleReward()
{
	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(other_cfg.lover_fb_double_reward_need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	bool is_succ = m_role->GetKnapsack()->GetMoney()->UseGold(other_cfg.lover_fb_double_reward_need_gold, "SpecialLoversFB::OnBuyDoubleReward");
	if (!is_succ)
	{
		return;
	}

	const QingyuanFBRewardConfig &fb_reward = LOGIC_CONFIG->GetQingyuanCfg().GetFBRewardCfg();

	ItemConfigData stuff;
	stuff.item_id = fb_reward.cfg_list[0].stuff_id;
	stuff.num = fb_reward.cfg_list[0].stuff_num;
	stuff.is_bind = true;

	m_role->GetKnapsack()->PutOrMail(stuff, PUT_REASON_QINGYUAN_FB_REWARD);
}

void Qingyuan::AddQingyuanValue(int add_value)
{	
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG))
	{
		if (add_value > 0)
		{
			add_value *= 2;
		}
	}

	m_param.qingyuan_value += add_value;
	if(m_param.qingyuan_value <= 0)
	{
		m_param.qingyuan_value = 0;
	}

	this->SendBaseFBInfo();
}

void Qingyuan::QueryMateQingyuanValue()
{
	if (!m_role->IsMarry())
	{
		return;
	}

	UserID targer_userid = IntToUid(m_role->GetLoverUid());
	Role *targer_role = World::GetInstWorld()->GetSceneManager()->GetRole(targer_userid);
	if (NULL == targer_role)
	{
		return;
	}

	Protocol::SCQingyuanMateQingyuanValue qymqyv;
	qymqyv.mate_qingyuan_value = targer_role->GetQingyuan()->GetQingyuanValue();
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&qymqyv, sizeof(qymqyv));
}

bool Qingyuan::ChoseShici(int shici_type)
{
	if (shici_type < SHICI_ONE || shici_type > SHICI_THREE)
	{
		this->ResetAllData();
		return false;
	}

	if (0 == m_lover_id || m_marry_type < 0 || m_marry_type >= QINGYUAN_HUNLI_MAX_TYPE)
	{
		//清空数据
		this->ResetAllData();
		return false;
	}

	Role *love_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_lover_id));
	if (NULL == love_role)
	{
		//清空数据
		this->ResetAllData();
		m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return false;
	}

	m_shici_type = shici_type;
	love_role->GetQingyuan()->SetShiciType(shici_type);

	love_role->GetQingyuan()->SendMarryOperaRet(love_role, Protocol::MARRY_CHOSE_SHICI, m_shici_type);

	return true;
}

void Qingyuan::OnPressFinger()
{
	if (0 == m_lover_id || m_marry_type < 0 || m_marry_type >= QINGYUAN_HUNLI_MAX_TYPE)
	{
		this->ResetAllData();
		return;
	}

	Role *love_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_lover_id));
	if (NULL == love_role)
	{
		//清空数据
		this->ResetAllData();
		m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	const QingyuanHunliTypeConfig *hunli_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanHunliTypeConfig(m_marry_type);
	if (NULL == hunli_cfg)
	{
		return;
	}

	//设置为已同意
	m_is_sure = true;

	//检查对方是否已同意
	if (love_role->GetQingyuan()->IsSureMarry())
	{
		Role *req_role = NULL;			// 发起者
		Role *target_role = NULL;		// 被求婚者

										//判断应该扣谁的钱
		if (m_marry_req_id == m_role->GetUID())
		{
			req_role = m_role;
			target_role = love_role;
		}
		else
		{
			req_role = love_role;
			target_role = m_role;
		}

		if (NULL == req_role)
		{
			return;
		}

		{
			// 检查
			if (love_role->GetKnapsack()->IsLock())
			{
				love_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
				return;
			}

			if (m_role->GetKnapsack()->IsLock())
			{
				m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
				return;
			}

			const QingyuanHunliTypeConfig *hunli_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanHunliTypeConfig(m_marry_type);
			if (NULL == hunli_cfg)
			{
				return;
			}

			// 扣除元宝
			{
				int real_consume_bind_gold = hunli_cfg->need_bind_gold;
				int real_consume_gold = hunli_cfg->need_gold;

				if (!req_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(real_consume_bind_gold + real_consume_gold))
				{
					req_role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
					return;
				}

				if (!req_role->GetKnapsack()->GetMoney()->GoldMoreThan(real_consume_gold))
				{
					req_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
					return;
				}

				req_role->GetKnapsack()->GetMoney()->UseGold(real_consume_gold, "Qingyuan::OnPressFinger");
				req_role->GetKnapsack()->GetMoney()->UseAllGold(real_consume_bind_gold, "Qingyuan::OnPressFinger");
			}

			// 已结婚重复按手印走另一套逻辑
			if (m_role->IsMarry())
			{
				this->OnPressFingerRepeat(req_role, target_role);
				return;
			}

			if (NULL != req_role && NULL != target_role)
			{
				// 触发求婚成功事件
				EventHandler::Instance().OnRaProposeSucc(req_role, target_role);

				{
					bool role_sucess3 = req_role->GetRoleActivity()->OnRAPerfectMarry(m_marry_type, target_role->GetUID());
					bool role_sucess4 = target_role->GetRoleActivity()->OnRAPerfectMarry(m_marry_type, req_role->GetUID());

					if (role_sucess3 && role_sucess4)
					{
						req_role->GetRoleActivity()->Brocast();
						RandActivityPerfectLover *rand_activity = RandActivityManager::Instance().GetRandActivityPerfectLover();
						if (rand_activity != NULL)
						{
							rand_activity->OnCoupleMarryRAPerfectLover(req_role->GetUID(), target_role->GetUID());
						}
					}
				}
			}

			// 发结婚奖品
			this->GiveMarryReward(m_marry_type);
			love_role->GetQingyuan()->GiveMarryReward(m_marry_type);

			WorldStatus::Instance().GetCommonData().marry_all_times++;
			WorldStatus::Instance().GetCommonData().marry_zhufu_count = World::GetInstWorld()->GetSceneManager()->GetOnlineRoleNum();
			this->MarryNotice(love_role);

		}

		//通知摁手指成功
		this->SendMarryOperaRet(m_role, Protocol::MARRY_PRESS_FINGER, 2);   //2:表示两个人都已摁过
		love_role->GetQingyuan()->SendMarryOperaRet(love_role, Protocol::MARRY_PRESS_FINGER, 2);

		if (m_marry_type >= 0)
		{
			love_role->GetQingyuan()->SetIsHaveHunYanTimes(1);
			m_param.is_have_hunyan_times = 1;

			love_role->GetQingyuan()->SetCurMarryStauts(QINGYUAN_MARRY_STATUS_WEDDING_YUYUE);
			m_param.cur_marry_stauts = QINGYUAN_MARRY_STATUS_WEDDING_YUYUE;
		}

		{
			// 播放特效
			Protocol::SCMarrySpecialEffect mse;
			mse.marry_type = m_marry_type;
			World::GetInstWorld()->SendToAllGateway((const char *)&mse, sizeof(mse));
		}

		{
			// 设置结婚状态
			GameName role_name = { 0 }, marry_target_name = { 0 };
			m_role->GetName(role_name); love_role->GetName(marry_target_name);

			time_t now_second = EngineAdapter::Instance().Time();
			m_role->SetMarryInfo(UidToInt(love_role->GetUserId()), marry_target_name, now_second);
			love_role->SetMarryInfo(UidToInt(m_role->GetUserId()), role_name, now_second);

			m_role->GetQingyuan()->SetHunyanMarryType(m_marry_type);
			love_role->GetQingyuan()->SetHunyanMarryType(m_marry_type);

			{
				this->SendHunjieEquipInfo();
				love_role->GetQingyuan()->SendHunjieEquipInfo();
			}

			{
				m_role->GetQingyuan()->SetHunyanFlag(0);
				love_role->GetQingyuan()->SetHunyanFlag(0);

				m_role->GetQingyuan()->SetOpenHunyanFB(0);
				love_role->GetQingyuan()->SetOpenHunyanFB(0);

				m_role->GetQingyuan()->SetHunyanFBOpenStatus(0);
				love_role->GetQingyuan()->SetHunyanFBOpenStatus(0);
			}

			//传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_marry_success_notice,
				UidToInt(love_role->GetUserId()), love_role->GetName(), love_role->GetCamp(), UidToInt(m_role->GetUserId()), m_role->GetName(), m_role->GetCamp(), WorldStatus::Instance().GetCommonData().marry_all_times);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL);
			}

			love_role->GetFriendManager()->AddFriend(m_role->GetUserId(), "Marry");
			m_role->GetFriendManager()->AddFriend(love_role->GetUserId(), "Marry");

		}

		EventHandler::Instance().OnMarry(love_role);
		EventHandler::Instance().OnMarry(m_role);

		// 设置全服第几对情侣
		int wedding_index = WorldStatus2::Instance().AddTotalWeddingNum();
		m_param.wedding_index = wedding_index;
		love_role->GetQingyuan()->SetWeddingIndex(m_param.wedding_index);

		// 预约弹窗协议
		{
			this->SendWeddingInfo(Protocol::QINGYUAN_INFO_TYPE_ROLE_INFO);
			love_role->GetQingyuan()->SendWeddingInfo(Protocol::QINGYUAN_INFO_TYPE_ROLE_INFO);

			this->SendWeddingInfo(Protocol::QINGYUAN_INFO_TYPE_YUYUE_POPUP);
			love_role->GetQingyuan()->SendWeddingInfo(Protocol::QINGYUAN_INFO_TYPE_YUYUE_POPUP);
		}

		Protocol::SCMarryInformation mi;
		mi.lover_uid = m_role->GetUID();
		m_role->GetName(mi.lover_name);
		EngineAdapter::Instance().NetSend(love_role->GetNetId(), (const char*)&mi, sizeof(mi));

		mi.lover_uid = love_role->GetUID();
		love_role->GetName(mi.lover_name);
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&mi, sizeof(mi));

		this->SendBlessInfo();

		{
			// 日志
			gamelog::g_log_marry.printf(LL_INFO, "MarryRet::user[%d, %s], target_user[%d, %s], marry_succ",
				m_role->GetUID(), m_role->GetName(), love_role->GetUID(), love_role->GetName());
		}

	}
	else
	{
		this->SendMarryOperaRet(m_role, Protocol::MARRY_PRESS_FINGER, 1);							//1:表示自己摁了 等待对方
		love_role->GetQingyuan()->SendMarryOperaRet(love_role, Protocol::MARRY_PRESS_FINGER, 1);

		gstr::SendNotice(m_role, "wait_press_finger");
	}
}

void Qingyuan::OnPressFingerRepeat(Role *req_role, Role *target_role)
{
	if (NULL == req_role || NULL == target_role)
	{
		return;
	}

	if (NULL != req_role && NULL != target_role)
	{
		// 触发求婚成功事件
//		EventHandler::Instance().OnRaProposeSucc(req_role, target_role);
	}

	// 发结婚奖品
	req_role->GetQingyuan()->GiveMarryReward(m_marry_type);
	target_role->GetQingyuan()->GiveMarryReward(m_marry_type);

	//通知摁手指成功
	req_role->GetQingyuan()->SendMarryOperaRet(req_role, Protocol::MARRY_PRESS_FINGER, 2);   //2:表示两个人都已摁过
	target_role->GetQingyuan()->SendMarryOperaRet(target_role, Protocol::MARRY_PRESS_FINGER, 2);

	EventHandler::Instance().OnMarryRepeat(req_role);
	EventHandler::Instance().OnMarryRepeat(target_role);

	// 播放特效
	{
		Protocol::SCMarrySpecialEffect mse;
		mse.marry_type = m_marry_type;
		World::GetInstWorld()->SendToAllGateway((const char *)&mse, sizeof(mse));
	}

	{
		bool role_sucess3 = req_role->GetRoleActivity()->OnRAPerfectMarry(m_marry_type, target_role->GetUID());
		bool role_sucess4 = target_role->GetRoleActivity()->OnRAPerfectMarry(m_marry_type, req_role->GetUID());

		if (role_sucess3 && role_sucess4)
		{
			req_role->GetRoleActivity()->Brocast();
			RandActivityPerfectLover *rand_activity = RandActivityManager::Instance().GetRandActivityPerfectLover();
			if (rand_activity != NULL)
			{
				rand_activity->OnCoupleMarryRAPerfectLover(req_role->GetUID(), target_role->GetUID());
			}
		}
	}

	{
		// 清除相关数据
		req_role->GetQingyuan()->ResetAllData();
		target_role->GetQingyuan()->ResetAllData();
	}
}

void Qingyuan::SyncHaloType(int halo_type)
{
	// 	QingyuanGlobal *qingyuan_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetUID());
	// 	QingyuanGlobal *lover_qingyuan_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetLoverUid());
	// 
	// 	// 顺序不能乱，先修改双方 后通知双方
	// 	if (NULL != qingyuan_global)
	// 	{
	// 		qingyuan_global->SetCoupleHaloType(halo_type);
	// 	}
	// 	if (NULL != lover_qingyuan_global)
	// 	{
	// 		lover_qingyuan_global->SetCoupleHaloType(halo_type);
	// 	}
	// 
	// 	// 顺序不能乱，先修改双方 后通知双方
	// 	if (NULL != qingyuan_global)
	// 	{
	// 		qingyuan_global->SendCoupleHaloInfo(m_role);
	// 	}
	// 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);
	// 	if (NULL != lover_qingyuan_global)
	// 	{
	// 		int lover_id = m_role->GetLoverUid();
	// 		Role *love_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(lover_id));
	// 
	// 		if (NULL != love_role)
	// 		{
	// 			lover_qingyuan_global->SendCoupleHaloInfo(love_role);
	// 			love_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);
	// 		}
	// 	}
}

void Qingyuan::SendMarryOperaRet(Role *role, int ret_type, int ret_val)
{
	if (NULL == role)
	{
		return;
	}

	static Protocol::SCMarryRetInfo mri;
	mri.ret_type = ret_type;
	mri.ret_val = ret_val;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&mri, sizeof(mri));
}

void Qingyuan::ResetAllData()
{
	m_marry_type = -1;
	m_lover_id = 0;
	m_marry_req_id = 0;
	m_shici_type = SHICI_INVALID;
	m_is_sure = false;
}

void Qingyuan::SetSkipFBWaves(bool is_skip)
{
	if (is_skip)
	{
		m_param.is_skip_fb_waves = 1;
	}
	else
	{
		m_param.is_skip_fb_waves = 0;
	}
}

void Qingyuan::GMResetHunyan()
{
	if (0 == m_role->GetLoverUid())
	{
		return;
	}

	if (FBManager::Instance().IsHunyanRunning(m_role))
	{
		int fb_key = FBManager::Instance().GetMyHunyanFBKey(m_role->GetUID());
		if (fb_key > 0)
		{
			Scene *hunyan_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig().hunyan_scene_id, fb_key);
			if (NULL != hunyan_scene && Scene::SCENE_TYPE_HUN_YAN_FB == hunyan_scene->GetSceneType())
			{
				SpecialHunyan *special_hunyan = dynamic_cast<SpecialHunyan*>(hunyan_scene->GetSpecialLogic());
				if (nullptr != special_hunyan)
					special_hunyan->GMForceStopHunyan();
			}
		}
	}

	this->SetHunyanFBOpenStatus(0);
	
	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role->GetLoverUid()));
	if (NULL != target_role)
	{
		target_role->GetQingyuan()->SetHunyanFBOpenStatus(0);
	}
}

int Qingyuan::GetHunjieStarLevel()
{
	const Equipment *equip = (const Equipment*)ITEMPOOL->GetItem(m_role->GetCommonDataParam()->hunjie_item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT || equip->GetEquipType() != Equipment::E_TYPE_SPECIAL_HUNJIE)
	{
		return 0;
	}

	return m_role->GetCommonDataParam()->hunjie_star_level;
}

int Qingyuan::PutOn(const ItemDataWrapper &item_wrapper)
{
	const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_wrapper.item_id);
	if (NULL == equip || ItemBase::I_TYPE_EQUIPMENT != equip->GetItemType()) return -1;

	if (0 == item_wrapper.has_param) return -2;

	if (Equipment::E_TYPE_SPECIAL_HUNJIE != equip->GetEquipType()) return -3;

	EquipmentParam *equip_param = equip->CreateParamFromStruct(&item_wrapper.param_data);
	if (NULL == equip_param) return -6;

	if (equip_param->star_level <= 0)
	{
		equip_param->star_level = 1;
	}
	
	if (0 == m_role->GetCommonDataParam()->hunjie_item_id)
	{
		m_role->GetCommonDataParam()->hunjie_item_id = item_wrapper.item_id;
		m_role->GetCommonDataParam()->hunjie_star_level = equip_param->star_level;
		m_param.hunjie_cur_exp = equip_param->param1;
		//m_grid_data.num = 1;
		//m_grid_data.invalid_time = item_wrapper.invalid_time;
		//m_grid_data.is_bind = (item_wrapper.is_bind == 1);
	}
	else
	{
		m_role->NoticeNum(errornum::EN_QINGYUAN_EQUIP_ALREADY_HAVE);
		return -7;
	}
	
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);

	this->SendHunjieEquipInfo();
	return 0;
}

int Qingyuan::TakeOff()
{
//  	if (m_grid_data.Invalid())
//  	{
//  		return -2;
//  	}
//  
//  	EquipmentParam *equip_param = (EquipmentParam *)m_grid_data.param;
//  	if (NULL == equip_param) return -3;
//  
//  	const Equipment *equip = (const Equipment*)ITEMPOOL->GetItem(m_grid_data.item_id);
//  	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT || equip->GetEquipType() != Equipment::E_TYPE_SPECIAL_HUNJIE)
//  	{
//  		return -4;
//  	}
//  
//  	Knapsack *knapsack = m_role->GetKnapsack();
//  	if (!knapsack->HaveSpace())
//  	{
//  		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
//  		return -5;
//  	}
//  
//  	ItemDataWrapper item_wrapper(m_grid_data.item_id, 1, (short)m_grid_data.is_bind, 
//  		m_grid_data.param, m_grid_data.invalid_time, 0);
//  
//  	int put_index = -1;
//  	if (!knapsack->Put(item_wrapper, PUT_REASON_NO_NOTICE, &put_index))
//  	{
//  		return -6;
//  	}
//  	
//  	m_grid_data.Reset();
//  	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);
// 	this->SendEquipInfo();

	return 0;
}

bool Qingyuan::HunjieEquipUpLevel(ItemID stuff_id, short repeat_times, bool is_auto_buy)
{
	if (stuff_id <= 0 || NULL == ITEMPOOL->GetItem(stuff_id))
	{
		return false;
	}

	const Equipment *equip = (const Equipment*)ITEMPOOL->GetItem(m_role->GetCommonDataParam()->hunjie_item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT || equip->GetEquipType() != Equipment::E_TYPE_SPECIAL_HUNJIE)
	{
		return false;
	}

	if (m_role->GetCommonDataParam()->hunjie_item_id >= LOGIC_CONFIG->GetQingyuanCfg().GetEquipMaxId() && m_role->GetCommonDataParam()->hunjie_star_level >= QINGYUAN_EQUIP_MAX_STAR)
	{
		m_role->NoticeNum(errornum::EN_QINGYUAN_EQUIP_LEVEL_MAX);
		return false;
	}

	const EquipUpLevelConfig::EquipStarItemConfig *star_item_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetEquipStarCfg(m_role->GetCommonDataParam()->hunjie_item_id, m_role->GetCommonDataParam()->hunjie_star_level);
	if (NULL == star_item_cfg)
	{
		return false;
	}

	const short old_star = m_role->GetCommonDataParam()->hunjie_star_level;
	const ItemID old_equip_id = m_role->GetCommonDataParam()->hunjie_item_id;
	
	//检查是否是升级材料,并计算暴击增加经验数量
	//Knapsack *knapsack = m_role->GetKnapsack();
	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();
	{
		// 进阶符
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = stuff_id;
		stuff_list[0].num = repeat_times;
		stuff_list[0].buyable = is_auto_buy;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, repeat_times, stuff_list, &consume_list, true))
		{
			return false;
		}
	}

	int add_exp = 0;
	int baoji_num = 0;

	for(int i = 1; i <= repeat_times; ++i)
	{
		bool is_baoji =  RandomNum(100) <= star_item_cfg->baoji_rate;
		LOGIC_CONFIG->GetQingyuanCfg().GetEquipUpLevelStuffAddExp(stuff_id, &add_exp, is_baoji);

		if (is_baoji)
		{
			baoji_num += 1;
		}
	}

	if (add_exp <= 0)
	{
		m_role->NoticeNum(errornum::EN_QINGYUAN_EQUIP_STUFF_ERROR);
		return false;
	}

	bool is_success = false;

	// 消耗
	if (ItemExtern::ConsumeItem(m_role, consume_list, "Qingyuan::HunjieEquipUpLevel"))
	{
		is_success = true;
	}

	if (is_success) 
	{
		++ m_param.today_hunjie_uplevel_times;
		this->AddUpLevelExp(add_exp);

		if (old_star != m_role->GetCommonDataParam()->hunjie_star_level || old_equip_id != m_role->GetCommonDataParam()->hunjie_item_id)
		{
			m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);
		}
	}
	else
	{
		return false;
	}

	static Protocol::SCHunjieEquipInfo cmd;
	cmd.consume_num = repeat_times;
	cmd.baoji_num = baoji_num;
	cmd.exp = m_param.hunjie_cur_exp;
	cmd.star = m_role->GetCommonDataParam()->hunjie_star_level;
	cmd.hunjie_item_id = m_role->GetCommonDataParam()->hunjie_item_id;

	UserCacheNode *lover_target_cache = UserCacheManager::Instance().GetUserNode(m_role->GetLoverUid());
	if (NULL != lover_target_cache)
	{
		cmd.lover_level = lover_target_cache->level;
		cmd.lover_hunjie_item_id = lover_target_cache->hunjie_item_id;
		cmd.lover_hunjie_star_level = lover_target_cache->hunjie_star_level;
		cmd.lover_prof = lover_target_cache->prof;
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sizeof(cmd));

	// 戒指升级，同步给伴侣
	Role *lover_target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role->GetLoverUid()));//世界里的
	if (NULL != lover_target_role && old_star != m_role->GetCommonDataParam()->hunjie_star_level)
	{
		static Protocol::SCHunjieEquipUpLevelSendLover cmd;
		cmd.level = m_role->GetLevel();
		cmd.hunjie_item_id = m_role->GetCommonDataParam()->hunjie_item_id;
		cmd.hunjie_star_level = m_role->GetCommonDataParam()->hunjie_star_level;
		EngineAdapter::Instance().NetSend(lover_target_role->GetNetId(), (const char *)&cmd, sizeof(cmd));
	}

	//日志
	gamelog::g_log_marry.printf(LL_INFO, "UpLevel::user[%d, %s], use_stuff_id[%d], use_stuff_num[%d], baoji_num[%d]",
		m_role->GetUID(), m_role->GetName(), stuff_id, repeat_times, baoji_num);

	return true;
}

void Qingyuan::SendHunjieEquipInfo()
{
	static Protocol::SCHunjieEquipInfo cmd;
	cmd.exp = m_param.hunjie_cur_exp;

	if (0 == m_role->GetCommonDataParam()->hunjie_star_level)
	{
		m_role->GetCommonDataParam()->hunjie_star_level = 1;
	}
	cmd.star = m_role->GetCommonDataParam()->hunjie_star_level;
	cmd.hunjie_item_id = m_role->GetCommonDataParam()->hunjie_item_id;

	UserCacheNode *lover_target_cache = UserCacheManager::Instance().GetUserNode(m_role->GetLoverUid());
	if (NULL != lover_target_cache)
	{
		cmd.lover_level = lover_target_cache->level;
		cmd.lover_hunjie_item_id = lover_target_cache->hunjie_item_id;
		cmd.lover_hunjie_star_level = lover_target_cache->hunjie_star_level;
		cmd.lover_prof = lover_target_cache->prof;
	}
	else
	{
		Role *lover_target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role->GetLoverUid()));//世界里的
		if(NULL != lover_target_role)
		{
			cmd.lover_level = lover_target_role->GetLevel();
			cmd.lover_hunjie_item_id = lover_target_role->GetCommonDataParam()->hunjie_item_id;
			cmd.lover_hunjie_star_level = lover_target_role->GetCommonDataParam()->hunjie_star_level;
		}
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sizeof(cmd));
}

void Qingyuan::AddUpLevelExp(int add_val)
{
	const Equipment *equip = (const Equipment*)ITEMPOOL->GetItem(m_role->GetCommonDataParam()->hunjie_item_id);
	if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT || equip->GetEquipType() != Equipment::E_TYPE_SPECIAL_HUNJIE)
	{
		return;
	}

	int round_count = 0; 
	while (add_val > 0 && round_count ++ < 100)
	{
		const EquipUpLevelConfig::EquipStarItemConfig *star_item_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetEquipStarCfg(m_role->GetCommonDataParam()->hunjie_item_id, m_role->GetCommonDataParam()->hunjie_star_level);
		if (NULL == star_item_cfg)
		{
			break;
		}

		if (add_val + m_param.hunjie_cur_exp >= star_item_cfg->max_exp)
		{
			add_val -= (star_item_cfg->max_exp - m_param.hunjie_cur_exp);

			const EquipUpLevelConfig::EquipStarItemConfig *next_star_item_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetNextEquipStarCfg(m_role->GetCommonDataParam()->hunjie_item_id, m_role->GetCommonDataParam()->hunjie_star_level);
			if (NULL == next_star_item_cfg)
			{
				m_param.hunjie_cur_exp = star_item_cfg->max_exp;
				break;
			}
			else
			{
				if (m_role->GetCommonDataParam()->hunjie_star_level >= QINGYUAN_EQUIP_MAX_STAR)
				{
					m_role->GetCommonDataParam()->hunjie_item_id = next_star_item_cfg->equip_id;
					m_role->GetCommonDataParam()->hunjie_star_level = 1;
					m_param.hunjie_cur_exp = 0;
				}
				else
				{
					m_role->GetCommonDataParam()->hunjie_star_level += 1;
					m_param.hunjie_cur_exp = 0;
				}
			}
		}
		else
		{
			m_param.hunjie_cur_exp += add_val;
			add_val = 0;
			break;
		}
	}
}

void Qingyuan::SendBlessInfo()
{
	static Protocol::SCQingyuanBlessInfo bi;
	
	Role *lover_target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role->GetLoverUid()));
	if (NULL != lover_target_role)
	{
		bi.bless_days = lover_target_role->GetQingyuan()->GetBlessDays();
		bi.is_fetch_bless_reward = lover_target_role->GetQingyuan()->GetQingyuanParam().is_fetch_bless_reward;
		bi.lover_bless_days = m_param.bless_days;
		EngineAdapter::Instance().NetSend(lover_target_role->GetNetId(), (const char *)&bi, sizeof(bi));
	}
	
	bi.bless_days = m_param.bless_days;
	bi.is_fetch_bless_reward = m_param.is_fetch_bless_reward;
	OfflineRegisterManager::Instance().OnGetQingyuanBlessDays(IntToUid(m_role->GetLoverUid()), &bi.lover_bless_days);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&bi, sizeof(bi));

}

void Qingyuan::SendYanhuiFBKey()
{
	if (FBManager::Instance().IsHunyanRunning(m_role))
	{
		this->SendBaseFBInfo();

		if (m_role->IsMarry())
		{
			Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role->GetLoverUid()));
			if (NULL != lover_role)
			{
				lover_role->GetQingyuan()->SendBaseFBInfo();
			}
		}
	}
}

void Qingyuan::SendCoupleHaloInfo()
{
	static Protocol::SCQingyuanCoupleHaloInfo qychi;

	qychi.equiped_couple_halo_type = m_param.equiped_couple_halo_type;
	qychi.cur_tejia_halo_type = m_param.cur_tejia_halo_type;
	qychi.couple_halo_max_type_count = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanCoupleHaloMaxType();
	qychi.tejie_halo_invalid_time = m_param.tejie_halo_invalid_time;
	memcpy(qychi.couple_halo_level_list, m_param.couple_halo_level_list, sizeof(qychi.couple_halo_level_list));
	memcpy(qychi.couple_halo_exp_list, m_param.couple_halo_exp_list, sizeof(qychi.couple_halo_exp_list));

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&qychi, sizeof(qychi));
}

void Qingyuan::CoupleHaloUse(int halo_type)
{
	if (halo_type < -1 || halo_type >= QINGYUAN_COUPLE_HALO_MAX_TYPE)
	{
		return;
	}

	if (-1 == halo_type)
	{
		m_role->NoticeNum(noticenum::NT_QINGYUAN_COUPLE_HALO_EQUIPED_OFF);
	}
	else
	{
		m_role->NoticeNum(noticenum::NT_QINGYUAN_COUPLE_HALO_EQUIPED_ON);
	}

	m_param.equiped_couple_halo_type = halo_type;

	QingyuanGlobal *self_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetUID());
	if (nullptr != self_global)
	{
		self_global->SetFightOutCouplehalo(halo_type);
	}

	QingyuanGlobal *lover_blobal = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetLoverUid());
	if (nullptr != lover_blobal)
	{
		lover_blobal->SetFightOutCouplehalo(halo_type);
	}

	// 策划需求，显示最新装备的光环
	if (this->IsCoupleHaloTrigger())
	{
		this->BroadcastCoupleHaloTrigger(halo_type);
	}
	this->SendCoupleHaloInfo();

	return;
}

void Qingyuan::CheckCoupleHaloEffect(unsigned int now_second)
{
	if (m_next_check_couple_halo_effect_time > now_second)
	{
		return;
	}
	m_next_check_couple_halo_effect_time = now_second + 1;

	if (this->IsCoupleHaloTrigger() && !m_is_in_couple_halo_effect)
	{
		m_is_in_couple_halo_effect = true;
	
		int couple_halo_type = m_param.equiped_couple_halo_type;
		QingyuanGlobal *self_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetUID());
		if (nullptr != self_global)
		{
			int fight_out_couple_halo_type = self_global->GetFightOutCouplehalo();
			if (-1 != fight_out_couple_halo_type)
			{
				couple_halo_type = fight_out_couple_halo_type;
			}
		}

		this->BroadcastCoupleHaloTrigger(couple_halo_type);
	}
	else if(!this->IsCoupleHaloTrigger() && m_is_in_couple_halo_effect)
	{
		m_is_in_couple_halo_effect = false;
		this->BroadcastCoupleHaloTrigger(-1);
	}
}

bool Qingyuan::IsCoupleHaloTrigger()
{
	if (NULL == m_role->GetScene())
	{
		return false;
	}

	if (0 == m_role->GetLoverUid())
	{
		return false;
	}

	Role *lover = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role->GetLoverUid()));
	if (NULL == lover)
	{
		return false;
	}

	// 战斗状态不检查
	if (RoleStatusManager::ROLE_ACTION_STATUS_FIGHT == m_role->GetRoleStatusManager()->GetRoleActionStatus() || 
		RoleStatusManager::ROLE_ACTION_STATUS_FIGHT == lover->GetRoleStatusManager()->GetRoleActionStatus())
	{
		return false;
	}

	if (-1 == m_param.equiped_couple_halo_type /*|| lover->GetQingyuan()->GetCoupleHaloType() > m_param.equiped_couple_halo_type*/)
	{
		return false;
	}
	// 测试两者距离
	int trigger = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg().couple_halo_trigger;

	const Posi &p1 = m_role->GetPos();
	const Posi &p2 = lover->GetPos();

	int distance1 = abs(p1.x - p2.x);
	int distance2 = abs(p1.y - p2.y);

	if (distance1 > trigger || distance2 > trigger)
	{
		return false;
	}

	return true;
}

void Qingyuan::BroadcastCoupleHaloTrigger(int halo_type)
{
	static Protocol::SCQingyuanCoupleHaloTrigger cht;
	cht.role1_uid = m_role->GetUID();
	cht.role2_uid = m_role->GetLoverUid();
	cht.halo_type = halo_type;

	Scene *scene = m_role->GetScene();
	if (NULL != scene)
	{
		scene->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&cht, sizeof(cht));
	}
}


void Qingyuan::UplevelCoupleHalo(int halo_type, bool is_auto_buy)
{
	if (halo_type < 0 || halo_type >= QINGYUAN_COUPLE_HALO_MAX_TYPE)
	{
		return;
	}

	int old_level = m_param.couple_halo_level_list[halo_type];

	const QingyuanCoupleHaloLevelConfig *cur_level_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanCoupleHaloLevelCfg(halo_type, old_level);
	if (NULL == cur_level_cfg)
	{
		m_role->NoticeNum(errornum::EN_QINGYUAN_HALO_LEVEL_LIMIT);
		return;
	}

	const QingyuanCoupleHaloLevelConfig *next_level_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanCoupleHaloLevelCfg(halo_type, old_level + 1);
	if (NULL == next_level_cfg)
	{
		m_role->NoticeNum(errornum::EN_QINGYUAN_HALO_LEVEL_LIMIT);
		return;
	}

	if (cur_level_cfg->pre_halo_type < 0 ||cur_level_cfg->pre_halo_type >= QINGYUAN_COUPLE_HALO_MAX_TYPE)
	{
		return;
	}

	int pre_halo_type_level = m_param.couple_halo_level_list[cur_level_cfg->pre_halo_type];
	if (pre_halo_type_level < cur_level_cfg->pre_halo_level)
	{
		m_role->NoticeNum(errornum::EN_QINGYUAN_PRE_HALO_LEVEL_LIMIT);
		return;
	}

	int need_stuff_num = cur_level_cfg->stuff_count - m_param.couple_halo_exp_list[halo_type];
	if (need_stuff_num <= 0)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return;
	}

	int comsume_item_id = MALE == m_role->GetSex() ? cur_level_cfg->stuff_id_man : cur_level_cfg->stuff_id_woman;
	int cur_has_num = m_role->GetKnapsack()->GetItemNum(comsume_item_id);
	if (!is_auto_buy && cur_has_num < need_stuff_num)
	{
		need_stuff_num = cur_has_num;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	//检查材料满足
	{
		short stuff_count = 0;
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

		stuff_list[stuff_count].item_id = comsume_item_id;
		stuff_list[stuff_count].num = need_stuff_num;
		stuff_list[stuff_count].buyable = is_auto_buy;
		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list))
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "Qingyuan::UplevelCoupleHalo"))
	{
		return;
	}

	m_param.couple_halo_exp_list[halo_type] += need_stuff_num;

	//升级
	if (m_param.couple_halo_exp_list[halo_type] >= cur_level_cfg->stuff_count)
	{
		m_param.couple_halo_exp_list[halo_type] = 0;
		++m_param.couple_halo_level_list[halo_type];
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);
	m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_UPLEVEL_QINGYUAN_HALO);
	m_role->GetMagicalPreciousManager()->SendMagicalPreciousConditionChange(CONDITION_TYPE_ACTIVE_QINGYUAN_HALO);
	this->SendCoupleHaloInfo();
}

void Qingyuan::OnBuyHalo(int halo_type)
{
	if (halo_type <= 0 || halo_type >= QINGYUAN_COUPLE_HALO_MAX_TYPE)
	{
		return;
	}

	const HaloSaleCfg *halo_sale_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHaloSaleCfg(halo_type);
	if (nullptr == halo_sale_cfg)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return;
	}

// 	time_t create_role_time = m_role->GetCreateRoleTimestamp();
// 	int create_day = GetDayIndex(create_role_time, EngineAdapter::Instance().Time()) + 1;

	int halo_price = halo_sale_cfg->normal_price;
	if (halo_type == m_param.cur_tejia_halo_type)
	{
		halo_price = halo_sale_cfg->off_price;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(halo_price, "Qingyuan::OnBuyHalo"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (halo_price == halo_sale_cfg->off_price)
	{
		const QingyuanCoupleHaloLevelConfig *couple_halo_level_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanCoupleHaloLevelCfg(m_param.cur_tejia_halo_type + 1, 0);
		if (NULL != couple_halo_level_cfg)
		{
			m_param.cur_tejia_halo_type++;
			m_param.tejie_halo_invalid_time = static_cast<unsigned int>(EngineAdapter::Instance().Time() + SECOND_PER_DAY);
		}
		else
		{
			m_param.cur_tejia_halo_type = -1;
			m_param.tejie_halo_invalid_time = 0;
		}
	}

	gamecommon::SendMultiMailToRole(m_role->GetUserId(), halo_sale_cfg->reward_item_list, halo_sale_cfg->reward_item_count, NULL, false, "buy_halo_mail");
	if (m_role->IsMarry())
	{
		gamecommon::SendMultiMailToRole(IntToUid(m_role->GetLoverUid()), halo_sale_cfg->reward_item_list, halo_sale_cfg->reward_item_count, NULL, false, "buy_halo_mail");
	}

	this->SendCoupleHaloInfo();

	m_role->NoticeNum(noticenum::NT_SHOP_BUY_SUCC);
}

int Qingyuan::GetCoupleHaloLevel(int halo_type)
{
	if (halo_type < 0 || halo_type >= QINGYUAN_COUPLE_HALO_MAX_TYPE)
	{
		return 0;
	}
	return m_param.couple_halo_level_list[halo_type];
}

int Qingyuan::GetActiveCoupleHaloCount(int color)
{
	int count = 0;
	for (unsigned int i = 0; i < static_array_size(m_param.couple_halo_level_list); i ++ )
	{
		int level = m_param.couple_halo_level_list[i];
		const QingyuanCoupleHaloLevelConfig *cur_level_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanCoupleHaloLevelCfg(i, level);
		if (NULL == cur_level_cfg)
		{
			continue;
		}

		if (cur_level_cfg->color >= color && level > 0)
		{
			count++;
		}
	}

	return count;
}

void Qingyuan::CheckTejiaHalo(unsigned int now_second)
{
	if (now_second >= m_param.tejie_halo_invalid_time && m_param.cur_tejia_halo_type >= 0)
	{
		int change_day_num = 0;
		if (m_param.tejie_halo_invalid_time > 0)
		{
			change_day_num = (now_second - m_param.tejie_halo_invalid_time) / SECOND_PER_DAY;
		}

		int add_halo_type_num = 1 + change_day_num;
		const QingyuanCoupleHaloLevelConfig *couple_halo_level_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanCoupleHaloLevelCfg(m_param.cur_tejia_halo_type + add_halo_type_num, 0);
		if (NULL != couple_halo_level_cfg)
		{
			m_param.cur_tejia_halo_type += add_halo_type_num;
			m_param.tejie_halo_invalid_time = now_second + SECOND_PER_DAY;

			this->SendCoupleHaloInfo();
		}
		else
		{
			m_param.cur_tejia_halo_type = -1;
			m_param.tejie_halo_invalid_time = 0;

			this->SendCoupleHaloInfo();
		}
	}
}

void Qingyuan::CheckTejiaHaloActivity(bool is_login, unsigned int last_save_time)
{
	bool is_change = false;
	time_t now = EngineAdapter::Instance().Time();
	time_t open_time = static_cast<time_t>(RandActivityManager::Instance().GetRanadActivityOpenTime(RAND_ACTIVITY_TYPE_COUPLE_HALO_SELL));
	if (!is_login)
	{
		if (m_param.cur_tejia_halo_type < 0)
		{
			m_param.cur_tejia_halo_type = 0;
			m_param.tejie_halo_invalid_time = 0;
			is_change = true;
		}
	}
	else
	{
		// 如果玩家离线期间开启了活动
		if (last_save_time < open_time && 
			open_time < now && 
			m_param.cur_tejia_halo_type < 0)
		{
			m_param.cur_tejia_halo_type = 0;
			m_param.tejie_halo_invalid_time = 0;
			is_change = true;
		}
	}

	if (is_change)
	{
		this->SendCoupleHaloInfo();
	}
}

void Qingyuan::FetchBlessReward()
{
	if (m_param.bless_days <= 0)
	{
		m_role->NoticeNum(errornum::EN_MARRY_BLESS_DAYS_NOT_ENOUGH);
		return;
	}

	if (0 != m_param.is_fetch_bless_reward)
	{
		m_role->NoticeNum(errornum::EN_MARRY_BLESS_ALREADY_FETCH);
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (NULL == knapsack) return;

	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!knapsack->HaveSpace())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	m_param.bless_days--;
	m_param.is_fetch_bless_reward = 1;

	const QingyuanBlessRewardConfig &bless_reward_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetBlessRewardCfg();

	if (knapsack->Put(bless_reward_cfg.reward_item, PUT_REASON_QINGYUAN_BLESS_REWARD))
	{
		m_role->NoticeNum(noticenum::NT_MARRY_HONEYMOON_FETCH_REWARD_SUCC);
	}

	{
		// 日志
		gamelog::g_log_marry.printf(LL_INFO, "FetchBlessReward::user[%d, %s], item_id[%d], item_num[%d], FetchBlessReward_Succ",
			m_role->GetUID(), m_role->GetName(), (int)bless_reward_cfg.reward_item.item_id, bless_reward_cfg.reward_item.num);
	}

	this->SendBlessInfo();
	return;
}

bool Qingyuan::CheckTeamInformationCorrect(int uid)
{
	if (m_role->GetUID() == uid)
	{
		return false;
	}
	
	Team *team = m_role->GetTeamInfo()->GetMyTeam();
	if(NULL == team)
	{
		m_role->NoticeNum(errornum::EN_MARRY_TARGET_MUST_IN_TEAM);
		return false;
	}

	if(team->GetMemberNum() != QINGYUAN_FB_TEAM_MEMBER_COUNT)
	{
		m_role->NoticeNum(errornum::EN_MARRY_TEAM_MEMBER_MUNBER_ERROR);
		return false;
	}

	if (NULL == team->GetTeamMember(IntToUid(uid)))
	{
		m_role->NoticeNum(errornum::EN_HE_IS_NOT_IN_YOUR_TEAM);
		return false;
	}

	if (team->GetMemberSceneId(IntToUid(uid)) != m_role->GetScene()->GetSceneID())
	{
		m_role->NoticeNum(errornum::EN_MARRY_TEAM_MEMBER_MUST_IN_SAME_MAP);
		return false;
	}

	return true;
}

void Qingyuan::QingYuanOperaReq(int opera_type, int param1, int param2)
{
	if (opera_type < Protocol::QINGYUAN_OPERA_TYPE_WEDDING_YUYUE || opera_type > Protocol::QINGYUAN_OPERA_TYPE_MAX)
	{
		return;
	}

	switch (opera_type)
	{
	case Protocol::QINGYUAN_OPERA_TYPE_WEDDING_YUYUE:
	{
		this->MakeAnAppointmantMarryTimeReq(param1, param2);
	}
	break;
	case Protocol::QINGYUAN_OPERA_TYPE_WEDDING_INVITE_GUEST:
	{
		this->WeddingInviteGuest(param1);
	}
	break;

	case Protocol::QINGYUAN_OPERA_TYPE_WEDDING_REMOVE_GUEST:
	{
		this->WeddingRemoveGuest(param1);
	}
	break;

	case Protocol::QINGYUAN_OPERA_TYPE_WEDDING_BUY_GUEST_NUM:
	{
		this->BuyWeddingInviteGuestNum();
	}
	break;

	case Protocol::QINGYUAN_OPERA_TYPE_WEDDING_GET_YUYUE_INFO:
	{
		this->SendWeddingYuYueInfo();
	}
	break;

	case Protocol::QINGYUAN_OPERA_TYPE_WEDDING_GET_ROLE_INFO:
	{
		this->SendWeddingInfo(Protocol::QINGYUAN_INFO_TYPE_ROLE_INFO);
	}
	break;

	case Protocol::QINGYUAN_OPERA_TYPE_WEDDING_YUYUE_FLAG:
	{
		this->SendWeddingInfo(Protocol::QINGYUAN_INFO_TYPE_WEDDING_YUYUE_FLAG);
	}
	break;

	case Protocol::QINGYUAN_OPERA_TYPE_WEDDING_YUYUE_RESULT:
	{
		this->AppointmantMarryTimeRet(param1, param2);
	}
	break;

	case Protocol::QINGYUAN_OPERA_TYPE_LOVER_INFO_REQ:
	{
		this->SendWeddingInfo(Protocol::QINGYUAN_INFO_TYPE_LOVER_INFO);
	}
	break;

	case Protocol::QINGYUAN_OPERA_TYPE_XUNYOU_ROLE_INFO:
	{
		this->SendWeddingInfo(Protocol::QINGYUAN_INFO_TYPE_XUNYOU_INFO);
	}
	break;

	case Protocol::QINGYUAN_OPERA_TYPE_XUNYOU_SA_HONGBAO:
	{
		if (1 == param1)
		{
			this->OnBuySaHongBaoTimes();
		}
		else
		{
			if (this->CheckCanSaHongbao())
			{
				this->OnXunyouSaHongbao();
			}
		}
	}
	break;

	case Protocol::QINGYUAN_OPERA_TYPE_XUNYOU_GIVE_FLOWER:
	{
		this->OnBuyGiveFlowerTimes();
	}
	break;

	case Protocol::QINGYUAN_OPERA_TYPE_XUNYOU_OBJ_POS:
	{
		this->OnSendXunyouObjInfo(param1);
	}
	break;

	case Protocol::QINGYUAN_OPERA_TYPE_LOVER_TITLE_INFO:
	{
//		this->SendLoverTitleInfo();
	}
	break;

	case Protocol::QINGYUAN_OPERA_TYPE_FETCH_LOVER_TITLE:
	{
//		this->FecthLoverTitle(param1);
	}
	break;

	case Protocol::QINGYUAN_OPERA_TYPE_BUY_AND_PUTON_EQUIP:
	{
//		this->OnBuyAndPutOn(param1);
	}
	break;

	default:
		break;
	}
}

void Qingyuan::MakeAnAppointmantMarryTimeReq(int seq, int hunyan_type)
{
	if (seq < 0 || seq >= WEDDING_YUYUE_TIME_MAX)
	{
		gstr::SendError(m_role, "illegal_opera");
		return;
	}

	if (!IsValidYuyueYanhuiType(hunyan_type))
	{
		gstr::SendError(m_role, "illegal_opera");
		return;
	}

	if (QINGYUAN_MARRY_STATUS_WEDDING_YUYUE != m_param.cur_marry_stauts)
	{
		gstr::SendError(m_role, "hunyan_yuyue_not_allow");
		return;
	}

	if (IsHunyanFBAlreadyOpen())
	{
		gstr::SendError(m_role, "hunyan_already_open");
		return;
	}

	if (0 == m_param.is_have_hunyan_times)
	{
		gstr::SendError(m_role, "hunyan_yuyue_time_not_enough");
		return;
	}

	if (0 != (WorldStatus2::Instance().GetWeddingParam().yuyue_info[seq].role_id))
	{
		gstr::SendError(m_role, "hunyan_yueyue_time_already_yuyue");
		return;
	}

	const WeddingYuYueTimeCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingYuYueTimeBySeq(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	unsigned int zero_time = static_cast<unsigned int>(GetZeroTime(now));
	if (now >= cfg_item->begin_time / 100 * 3600 + (cfg_item->begin_time % 100) * 60 + zero_time)
	{
		gstr::SendError(m_role, "hunyan_yuyue_time_has_passed");
		return;
	}


	UserID lover_user_id = IntToUid(m_role->GetLoverUid());
	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(lover_user_id);
	if (NULL == lover_role)
	{
		m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	this->SetHunyanType(hunyan_type);			// 记录预约婚宴类型

	static Protocol::SCQingYuanAllInfo qyai;

	qyai.info_type = Protocol::QINGYUAN_INFO_TYPE_YUYUE_RET;
	qyai.param_ch1 = (char)seq;
	qyai.param_ch2 = (char)hunyan_type;

	EngineAdapter::Instance().NetSend(lover_role->GetNetId(), (const char*)&qyai, sizeof(qyai));

}

void Qingyuan::AppointmantMarryTimeRet(int seq, int is_agree)
{
	if (seq < 0 || seq >= WEDDING_YUYUE_TIME_MAX)
	{
		return;
	}

	bool lover_is_online = true;
	UserID lover_user_id = IntToUid(m_role->GetLoverUid());
	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(lover_user_id);
	if (NULL == lover_role)
	{
		m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		lover_is_online = false;
		return;
	}

	if (QINGYUAN_MARRY_STATUS_WEDDING_YUYUE != m_param.cur_marry_stauts)
	{
		gstr::SendError(m_role, "hunyan_yuyue_not_allow");

		if (lover_is_online)
		{
			gstr::SendError(lover_role, "lover_hunyan_yuyue_not_allow");
		}
		return;
	}

	if (0 == m_param.is_have_hunyan_times)
	{
		gstr::SendError(m_role, "hunyan_yuyue_time_not_enough");

		if (lover_is_online)
		{
			gstr::SendError(lover_role, "lover_hunyan_yuyue_time_not_enough");
		}
		return;
	}

	if (0 != (WorldStatus2::Instance().GetWeddingParam().yuyue_info[seq].role_id))
	{
		gstr::SendError(m_role, "hunyan_yueyue_time_already_yuyue");
		return;
	}

	const WeddingYuYueTimeCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingYuYueTimeBySeq(seq);
	if (NULL == cfg_item)
	{
		return;
	}

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	unsigned int zero_time = static_cast<unsigned int>(GetZeroTime(now));
	if (now >= cfg_item->begin_time / 100 * 3600 + (cfg_item->begin_time % 100) * 60 + zero_time)
	{
		gstr::SendError(m_role, "en_hunyan_yuyue_time_has_passed");
		return;
	}

	// 无论成功与否都先清掉双方的预约类型
	int yuyue_type = lover_role->GetQingyuan()->GetHunyuanType();
	SetHunyanType(YUYUE_HUNYAN_INVALUE);
	lover_role->GetQingyuan()->SetHunyanType(YUYUE_HUNYAN_INVALUE);

	if (1 == is_agree)
	{
		{
			// 检查消耗
			const QingyuanHunyanConfig::ConfigItem * hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanCfg(yuyue_type);
			if (NULL == hunyan_cfg)
			{
				return;
			}
			if (lover_role->GetKnapsack()->IsLock())
			{
				lover_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
				return;
			}

			{
				if (YUYUE_HUNYAN_TYPE_1 == yuyue_type)
				{
					if (hunyan_cfg->need_goldbind > 0 && !lover_role->GetKnapsack()->GetMoney()->AllGoldMoreThan(hunyan_cfg->need_goldbind))
					{
						lover_role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
						return;
					}

					if (hunyan_cfg->need_gold > 0 && !lover_role->GetKnapsack()->GetMoney()->GoldMoreThan(hunyan_cfg->need_gold))
					{
						lover_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
						return;
					}

					if (hunyan_cfg->need_goldbind > 0 && !lover_role->GetKnapsack()->GetMoney()->UseAllGold(hunyan_cfg->need_goldbind, "Qingyuan::AppointmantMarryTimeRet"))
					{
						return;
					}

					if (hunyan_cfg->need_gold > 0 && !lover_role->GetKnapsack()->GetMoney()->UseGold(hunyan_cfg->need_gold, "Qingyuan::AppointmantMarryTimeRet"))
					{
						return;
					}

					lover_role->GetQingyuan()->SetHunyanType(YUYUE_HUNYAN_TYPE_1);
					m_role->GetQingyuan()->SetHunyanType(YUYUE_HUNYAN_TYPE_1);
				}

				if (YUYUE_HUNYAN_TYPE_2 == yuyue_type)
				{
					if (!lover_role->GetKnapsack()->GetMoney()->GoldMoreThan(hunyan_cfg->need_gold))
					{
						lover_role->NoticeNum(errornum::EN_CHONGZHI_WANT_MONEY_HAS_NOT_ENOUGH);
						return;
					}

					if (!lover_role->GetKnapsack()->GetMoney()->UseGold(hunyan_cfg->need_gold, "Qingyuan::AppointmantMarryTimeRet"))
					{
						return;
					}

					lover_role->GetQingyuan()->SetHunyanType(YUYUE_HUNYAN_TYPE_2);
					m_role->GetQingyuan()->SetHunyanType(YUYUE_HUNYAN_TYPE_2);
				}
			}
			// 发奖励
			m_role->GetKnapsack()->PutListOrMail(hunyan_cfg->reward_item_list, PUT_REASON_YUYUE_HUNYAN_REWARD);
			lover_role->GetKnapsack()->PutListOrMail(hunyan_cfg->reward_item_list, PUT_REASON_YUYUE_HUNYAN_REWARD);
		}

		// 消耗成功设置双方预约类型
		m_role->GetQingyuan()->SetHunyanType(yuyue_type);
		lover_role->GetQingyuan()->SetHunyanType(yuyue_type);

		const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
		WeddingYuYueInfo &yuyueInfo = WorldStatus2::Instance().GetWeddingParam().yuyue_info[seq];
		yuyueInfo.role_id = m_role->GetUID();
		yuyueInfo.lover_role_id = m_role->GetCommonDataParam()->lover_uid;
		yuyueInfo.wedding_type = m_param.hunyan_type;
		yuyueInfo.can_invite_guest_num = other_cfg.wedding_guest_num;
		yuyueInfo.wedding_yuyue_seq = (char)seq;
		yuyueInfo.wedding_index = m_param.wedding_index;
		yuyueInfo.is_has_done = 0;
		memset(yuyueInfo.guest_role_id_list, 0, sizeof(yuyueInfo.guest_role_id_list));
		WorldStatus2::Instance().GetWeddingParam().wedding_yuyue_flag |= ((long long)1 << seq);

		gstr::SendNotice(m_role, "nt_wedding_yuyue_succ");
		gstr::SendNotice(lover_role, "nt_wedding_yuyue_succ");

		m_param.cur_marry_stauts = QINGYUAN_MARRY_STATUS_INVITE_GUESTS;
		lover_role->GetQingyuan()->SetCurMarryStauts(QINGYUAN_MARRY_STATUS_INVITE_GUESTS);

		m_param.wedding_yuyue_seq = (char)seq;
		lover_role->GetQingyuan()->SetWeddingYuYueSeq((char)seq);

		m_param.is_have_hunyan_times = 0;
		lover_role->GetQingyuan()->SetIsHaveHunYanTimes(0);

		this->SendWeddingInfo(Protocol::QINGYUAN_INFO_TYPE_ROLE_INFO, true);
		lover_role->GetQingyuan()->SendWeddingInfo(Protocol::QINGYUAN_INFO_TYPE_ROLE_INFO, true);

		static Protocol::SCQingYuanAllInfo qyai;

		qyai.info_type = Protocol::QINGYUAN_INFO_TYPE_YUYUE_SUCC;

		EngineAdapter::Instance().NetSend(lover_role->GetNetId(), (const char*)&qyai, sizeof(qyai));
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&qyai, sizeof(qyai));

		{
			// 巡游通知邮件
			const WeddingYuYueTimeCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingYuYueTimeBySeq(m_param.wedding_yuyue_seq);
			if (NULL != cfg_item)
			{
				static MailContentParam content_param; content_param.Reset();

				int length = gstr::MailContent(content_param.contenttxt, sizeof(content_param.contenttxt), "wedding_xunyou_notice_mail", cfg_item->begin_time / 100, cfg_item->begin_time % 100, cfg_item->end_time / 100, cfg_item->end_time % 100);
				if (length > 0)
				{
					MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, content_param);
					MailRoute::MailToUser(lover_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, content_param);
				}
			}
		}
	}
	else
	{
		gstr::SendError(lover_role, "en_wedding_yuyue_disagree");
	}

	{
		// 日志
		gamelog::g_log_qingyuan.printf(LL_INFO, "AppointmantMarryTimeRet::user[%d, %s], target_user[%d, %s], seq[%d], is_agree[%d]",
			m_role->GetUID(), m_role->GetName(), lover_role->GetUID(), lover_role->GetName(), seq, is_agree);
	}
}

bool Qingyuan::IsValidYuyueSeq(char seq)
{
	if (seq > 0 && seq < WEDDING_YUYUE_TIME_MAX)
	{
		return true;
	}
	return false;
}

void Qingyuan::WeddingInviteGuest(int uid)
{
	if (QINGYUAN_MARRY_STATUS_INVITE_GUESTS != m_param.cur_marry_stauts && QINGYUAN_MARRY_STATUS_WEDDING != m_param.cur_marry_stauts)
	{
		gstr::SendError(m_role, "en_wedding_not_allow_invite_guest");
		return;
	}

	if (WorldStatus2::Instance().IsInWeddingGuests(m_role->GetUID(), uid))
	{
		gstr::SendError(m_role, "en_already_in_wedding_guests");
		return;
	}

	int yuyue_seq = m_param.wedding_yuyue_seq;
	if (yuyue_seq < 0 || yuyue_seq >= WEDDING_YUYUE_TIME_MAX)
	{
		gstr::SendError(m_role, "illegal_opera");
		return;
	}

	WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[yuyue_seq];
	int has_invite_guest_num = WorldStatus2::Instance().GetWeddingHasInviteGuestNum(yuyue_info.role_id);
	if (has_invite_guest_num >= yuyue_info.can_invite_guest_num)
	{
		gstr::SendNotice(m_role, "en_guests_has_full");
		return;
	}

	Role *guest_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (NULL == guest_role)
	{
		m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	const WeddingYuYueTimeCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingYuYueTimeBySeq(m_param.wedding_yuyue_seq);
	if (NULL == cfg_item)
	{
		return;
	}

	int lover_camp = 0;
	UserCacheNode *lover_cache_node = UserCacheManager::Instance().GetUserNode(m_role->GetLoverUid());
	if (NULL != lover_cache_node)
	{
		lover_camp = (int)lover_cache_node->camp;
	}

	if (!WorldStatus2::Instance().AddWeddingGeust(m_role->GetUID(), uid))
	{
		return;
	}

	SpecialHunyan *hunyan_scene = World::GetInstWorld()->GetSceneManager()->GetSpecialHunyan(COMMON_SCENE_KEY, false);
	if (NULL != hunyan_scene)
	{
		hunyan_scene->AddApplicantToGuestList(m_role, uid, 1);
	}
	
	const QingyuanHunyanConfig::ConfigItem * hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanCfg(m_param.hunyan_type);
	if (NULL != hunyan_cfg)
	{
		// 邀请邮件
		gamecommon::SendMultiMailToRole(guest_role->GetUserId(), hunyan_cfg->join_hunyan_item_list, hunyan_cfg->join_hunyan_item_count, NULL, false, "invalite_join_hunyan_mail", yuyue_info.wedding_index,
			m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), m_role->GetLoverUid(), m_role->GetCommonDataParam()->lover_name, lover_camp,
			cfg_item->begin_time / 100, cfg_item->begin_time % 100, cfg_item->end_time / 100, cfg_item->end_time % 100);
	}

	gstr::SendNotice(m_role, "invite_succ");

	this->SendWeddingYuYueInfo();

	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role->GetLoverUid()));
	if (NULL != lover_role)
	{
		lover_role->GetQingyuan()->SendWeddingYuYueInfo();
	}

	{
		// 日志
		gamelog::g_log_qingyuan.printf(LL_INFO, "WeddingInviteGuest::user[%d, %s], target_user[%d, %s], has_invite_guest_num[%d], can_invite_guest_num[%d]",
			m_role->GetUID(), m_role->GetName(), guest_role->GetUID(), guest_role->GetName(), has_invite_guest_num, yuyue_info.can_invite_guest_num);
	}
}

void Qingyuan::WeddingRemoveGuest(int uid)
{
	if (QINGYUAN_MARRY_STATUS_INVITE_GUESTS != m_param.cur_marry_stauts && QINGYUAN_MARRY_STATUS_WEDDING != m_param.cur_marry_stauts)
	{
		gstr::SendError(m_role, "en_wedding_not_allow_invite_guest");
		return;
	}

	if (!WorldStatus2::Instance().IsInWeddingGuests(m_role->GetUID(), uid))
	{
		gstr::SendError(m_role, "en_not_in_wedding_guests");
		return;
	}

	if (!WorldStatus2::Instance().RemoveWeddingGeust(m_role->GetUID(), uid))
	{
		return;
	}

	gstr::SendNotice(m_role, "remove_succ");
	this->SendWeddingYuYueInfo();

	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role->GetLoverUid()));
	if (NULL != lover_role)
	{
		lover_role->GetQingyuan()->SendWeddingYuYueInfo();
	}

	{
		UserCacheNode *guest_node = UserCacheManager::Instance().GetUserNode(uid);

		int yuyue_seq = m_param.wedding_yuyue_seq;
		if (yuyue_seq < 0 || yuyue_seq >= WEDDING_YUYUE_TIME_MAX)
		{
			return;
		}

		WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[yuyue_seq];
		int has_invite_guest_num = WorldStatus2::Instance().GetWeddingHasInviteGuestNum(yuyue_info.role_id);
		if (NULL != guest_node)
		{
			// 日志
			gamelog::g_log_qingyuan.printf(LL_INFO, "WeddingInviteGuest::user[%d, %s], target_user[%d, %s], has_invite_guest_num[%d], can_invite_guest_num[%d]",
				m_role->GetUID(), m_role->GetName(), uid, guest_node->user_name, has_invite_guest_num, yuyue_info.can_invite_guest_num);
		}
	}
}

void Qingyuan::BuyWeddingInviteGuestNum()
{
	if (QINGYUAN_MARRY_STATUS_INVITE_GUESTS != m_param.cur_marry_stauts && QINGYUAN_MARRY_STATUS_WEDDING != m_param.cur_marry_stauts)
	{
		gstr::SendError(m_role, "en_qingyuan_not_yuyue_wedding");
		return;
	}

	int yuyue_seq = m_param.wedding_yuyue_seq;
	if (yuyue_seq < 0 || yuyue_seq >= WEDDING_YUYUE_TIME_MAX)
	{
		return;
	}

	WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[yuyue_seq];
	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
	if (yuyue_info.can_invite_guest_num >= other_cfg.wedding_guest_num + other_cfg.wedding_buy_guest_limit_1 + other_cfg.wedding_buy_guest_limit_2)
	{
		gstr::SendError(m_role, "en_guests_buy_limit");
		return;
	}

	int price = other_cfg.wedding_buy_guest_price_1; // 第一档价格
	if (yuyue_info.can_invite_guest_num >= other_cfg.wedding_guest_num + other_cfg.wedding_buy_guest_limit_1)
	{
		price = other_cfg.wedding_buy_guest_price_2; // 第二档价格
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(price))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->UseGold(price, "WeddingBuyGuestNum"))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!WorldStatus2::Instance().AddWeddingCanInviteGuestNum(m_role->GetUID()))
	{
		return;
	}

	m_role->NoticeNum(noticenum::NT_SHOP_BUY_SUCC);
	this->SendWeddingYuYueInfo();

	// 日志
	gamelog::g_log_qingyuan.printf(LL_INFO, "BuyWeddingInviteGuestNum::user[%d, %s], can_invite_extra_guest_num[%d]",
		m_role->GetUID(), m_role->GetName(), yuyue_info.can_invite_guest_num);
}

void Qingyuan::SendWeddingYuYueInfo()
{
	static Protocol::SCQingYuanWeddingAllInfo qywai;

	int yuyue_seq = m_param.wedding_yuyue_seq;
	if (yuyue_seq < 0 || yuyue_seq >= WEDDING_YUYUE_TIME_MAX)
	{
		return;
	}

	WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[yuyue_seq];

	qywai.role_id = yuyue_info.role_id;
	UserCacheNode *role_node = UserCacheManager::Instance().GetUserNode(yuyue_info.role_id);
	if (NULL != role_node)
	{
		role_node->GetName(qywai.role_name);
		qywai.role_prof = role_node->prof;
	}

	qywai.lover_role_id = yuyue_info.lover_role_id;
	UserCacheNode *lover_role_node = UserCacheManager::Instance().GetUserNode(yuyue_info.lover_role_id);
	if (NULL != lover_role_node)
	{
		lover_role_node->GetName(qywai.lover_role_name);
		qywai.lover_role_prof = lover_role_node->prof;
	}
	
	qywai.can_invite_guest_num = yuyue_info.can_invite_guest_num;
	qywai.wedding_type = yuyue_info.wedding_type;
	qywai.wedding_yuyue_seq = yuyue_info.wedding_yuyue_seq;
	qywai.wedding_index = yuyue_info.wedding_index;

	int count = 0;

	for (int i = 0; i < WEDDING_GUESTS_MAX_NUM; i++)
	{
		if (0 != yuyue_info.guest_role_id_list[i])
		{
			UserCacheNode *guest_cache_node = UserCacheManager::Instance().GetUserNode(yuyue_info.guest_role_id_list[i]);
			if (NULL != guest_cache_node)
			{
				qywai.guests_list[count].user_id = yuyue_info.guest_role_id_list[i];
				guest_cache_node->GetName(qywai.guests_list[count].name);

				count++;
			}
		}
	}

	qywai.count = count;
	qywai.has_invite_guests_num = (char)count;

	int length = sizeof(qywai) - sizeof(qywai.guests_list[0]) * (WEDDING_GUESTS_MAX_NUM - count);
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&qywai, length);
}

void Qingyuan::SendWeddingInfo(int info_type, bool is_popup)
 {
	static Protocol::SCQingYuanAllInfo qyai;
	qyai.info_type = 0;
	qyai.param_ch1 = 0;
	qyai.param_ch2 = 0;
	qyai.param_ch3 = 0;
	qyai.param_ch4 = 0;
	qyai.param_ch5 = 0;
	qyai.param_ch6 = 0;
	qyai.param2 = 0;
	qyai.param3 = 0;
	qyai.param4 = 0;
	qyai.param5 = 0;
	qyai.param6 = 0;
	memset(qyai.role_name, 0, sizeof(qyai.role_name));

	switch (info_type)
	{
	case Protocol::QINGYUAN_INFO_TYPE_ROLE_INFO:
	{
		qyai.info_type = info_type;
		qyai.param_ch1 = m_param.marry_type;
		qyai.param_ch2 = m_param.is_have_hunyan_times;
		qyai.param_ch3 = m_param.cur_marry_stauts;
		qyai.param_ch4 = m_param.wedding_yuyue_seq;
		qyai.param_ch5 = m_param.has_marry_huili_type_flag;
		qyai.param2 = (int)(is_popup ? 1 : 0);
	}
	break;

	case Protocol::QINGYUAN_INFO_TYPE_WEDDING_YUYUE_FLAG:
	{
		WeddingParam &wedding_param = WorldStatus2::Instance().GetWeddingParam();

		qyai.info_type = info_type;
		qyai.param6 = wedding_param.wedding_yuyue_flag;
	}
	break;

	case Protocol::QINGYUAN_INFO_TYPE_YUYUE_POPUP:
	{
		qyai.info_type = info_type;
	}
	break;

	case Protocol::QINGYUAN_INFO_TYPE_LOVER_INFO:
	{
		UserCacheNode *lover_node = UserCacheManager::Instance().GetUserNode(m_role->GetLoverUid());
		if (NULL == lover_node)
		{
			return;
		}

		qyai.info_type = Protocol::QINGYUAN_INFO_TYPE_LOVER_INFO;
		qyai.param5 = lover_node->uid;
		qyai.param_ch1 = lover_node->prof;
		lover_node->GetName(qyai.role_name);
	}
	break;

	case Protocol::QINGYUAN_INFO_TYPE_XUNYOU_INFO:
	{
		qyai.info_type = Protocol::QINGYUAN_INFO_TYPE_XUNYOU_INFO;
		qyai.param_ch1 = m_param.today_xunyou_sa_hongbao_count;
		qyai.param_ch2 = m_param.today_xunyou_sa_hongbao_buy_count;
		qyai.param_ch3 = m_param.today_xunyou_give_flower_count;
		qyai.param_ch4 = m_param.today_xunyou_give_flower_buy_count;
		qyai.param_ch5 = m_param.today_xunyou_gather_hongbao_count;
		qyai.param_ch6 = m_param.hunyan_type;
	}
	break;

	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&qyai, sizeof(qyai));
}

int Qingyuan::GetOtherHunyuanType()
{
	int hunyan_type = YUYUE_HUNYAN_INVALUE;
	SpecialHunyan *hunyan_scene = World::GetInstWorld()->GetSceneManager()->GetSpecialHunyan(COMMON_SCENE_KEY, false);
	if (NULL != hunyan_scene)
	{
		hunyan_type = hunyan_scene->GetYanHuiType();
	}

	return hunyan_type;
}

bool Qingyuan::IsHadPropose(int marry_type)
{
	const QingyuanHunliTypeConfig * hunli_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanHunliTypeConfig(marry_type);
	if (NULL == hunli_cfg) return false;

	bool is_propose = IS_BIT_SET(m_param.has_marry_huili_type_flag, marry_type);

	return is_propose;
}

bool Qingyuan::IsInXunyouStatus()
{
	int hunyan_status = HUNYAN_STATE_TYPE_INVALID;
	bool is_marry_user = false;

	SpecialHunyan *hunyan_scene = World::GetInstWorld()->GetSceneManager()->GetSpecialHunyan(COMMON_SCENE_KEY, false);
	if (NULL != hunyan_scene)
	{
		hunyan_status = hunyan_scene->GetHunyanState();
		is_marry_user = hunyan_scene->IsMarryUser(m_role->GetUID(), NULL);
	}

	return (HUNYAN_STATE_TYPE_XUNYOU == hunyan_status) && is_marry_user;
}

bool Qingyuan::CheckCanGatherXunyouHongbao(int gather_id)
{
	int hunyan_type = GetOtherHunyuanType();

	const XunyouCfg *hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouCfg(hunyan_type);
	if (nullptr == hunyan_cfg)
	{
		return true;
	}

	if (gather_id != hunyan_cfg->xunyou_red_bag_gather_id)
	{
		return true;
	}

	if (m_param.today_xunyou_gather_hongbao_count < hunyan_cfg->per_day_gather_limit)
	{
		return true;
	}

	gstr::SendError(m_role, "xunyou_gather_redbag_count_max");
	return false;
}

void Qingyuan::OnGatherXunyouHongbao(int gather_id)
{
	int hunyan_type = GetOtherHunyuanType();

	const XunyouCfg *hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouCfg(hunyan_type);
	if (nullptr == hunyan_cfg)
	{
		return;
	}

	if (gather_id != hunyan_cfg->xunyou_red_bag_gather_id)
	{
		return;
	}

	m_param.today_xunyou_gather_hongbao_count ++;

	m_role->GetKnapsack()->GetMoney()->AddGoldBind(hunyan_cfg->every_time_bind_gold, "Qingyuan::OnGatherXunyouHongbao");
}

bool Qingyuan::CheckCanSaHongbao()
{
	if (!IsInXunyouStatus())
	{
		gstr::SendError(m_role, "xunyou_opera_limit");
		return false;
	}

	const XunyouCfg *hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouCfg(m_param.hunyan_type);
	if (nullptr == hunyan_cfg)
	{
		return false;
	}
	
	int today_max_count = hunyan_cfg->free_sa_red_bag_count + m_param.today_xunyou_sa_hongbao_buy_count;
	if (m_param.today_xunyou_sa_hongbao_count >= today_max_count)
	{
		gstr::SendError(m_role, "xunyou_opera_count_limit");
		return false;
	}

	return true;
}

void Qingyuan::OnBuySaHongBaoTimes()
{
	if (!IsInXunyouStatus())
	{
		gstr::SendError(m_role, "xunyou_opera_limit");
		return;
	}

	const XunyouCfg *hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouCfg(m_param.hunyan_type);
	if (nullptr == hunyan_cfg)
	{
		return;
	}

	if (m_param.today_xunyou_sa_hongbao_buy_count >= hunyan_cfg->buy_sa_red_bag_count)
	{
		m_role->NoticeNum(errornum::NT_MYSTERIOUS_SHOP_BUY_LIMIT);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(hunyan_cfg->red_bag_need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_role->GetKnapsack()->GetMoney()->UseGold(hunyan_cfg->red_bag_need_gold, "Xunyou::SaHongbao");

	m_param.today_xunyou_sa_hongbao_buy_count++;

	this->SendWeddingInfo(Protocol::QINGYUAN_INFO_TYPE_XUNYOU_INFO);
}

void Qingyuan::OnXunyouSaHongbao()
{
	const XunyouCfg *hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouCfg(m_param.hunyan_type);
	if (nullptr == hunyan_cfg)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return;
	}

	int xunyou_scene_id = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouSceneId();
	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(xunyou_scene_id, 0);
	if (nullptr == scene)
	{
		return;
	}

	if (hunyan_cfg->red_bag_num <= 0)
	{
		gstr::SendError(m_role, "get_cfg_error");
		return;
	}

	Posi init_pos = m_role->GetPos();
	std::queue<Posi> pos_list;	// 刷红包坐标列表
	int min_dis = hunyan_cfg->red_bag_drop_range * hunyan_cfg->red_bag_drop_range / hunyan_cfg->red_bag_num + 1;
	gamecommon::GetValidRandPosList(init_pos, hunyan_cfg->red_bag_drop_range, scene, min_dis, pos_list);

	for (int i = 0; i < hunyan_cfg->red_bag_num; ++i)
	{
		Posi rand_pos = init_pos;
		if (pos_list.size() > 0)
		{
			rand_pos = pos_list.front();
			pos_list.pop();
		}
		else
		{
			rand_pos = gamecommon::GetValidRandPos(init_pos, hunyan_cfg->red_bag_drop_range, scene);
		}

		// 刷新红包对象
		GatherObj *gather_obj = new GatherObj();
		gather_obj->Init(NULL, hunyan_cfg->xunyou_red_bag_gather_id, hunyan_cfg->gather_time_ms, 0, true, 180);
		gather_obj->SetPos(rand_pos);
		scene->AddObj(gather_obj);
	}

	m_param.today_xunyou_sa_hongbao_count++;

	this->SendWeddingInfo(Protocol::QINGYUAN_INFO_TYPE_XUNYOU_INFO);

	{
		// 传闻
		int scene_id = m_role->GetScene()->GetSceneID();
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "wedding_xunyou_sahongbao_notice", m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), 
			scene_id, m_role->GetPos().x, m_role->GetPos().y);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}
}

bool Qingyuan::CheckCanGiveFlower()
{
	if (!IsInXunyouStatus())
	{
		gstr::SendError(m_role, "xunyou_opera_limit");
		return false;
	}

	const XunyouCfg *hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouCfg(m_param.hunyan_type);
	if (nullptr == hunyan_cfg)
	{
		return false;
	}

	int today_max_count = hunyan_cfg->free_give_flower_count + m_param.today_xunyou_give_flower_buy_count;
	if (m_param.today_xunyou_give_flower_count >= today_max_count)
	{
		gstr::SendError(m_role, "xunyou_opera_count_limit");
		return false;
	}

	return true;
}

void Qingyuan::OnBuyGiveFlowerTimes()
{
	if (!IsInXunyouStatus())
	{
		gstr::SendError(m_role, "xunyou_opera_limit");
		return;
	}

	const XunyouCfg *hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouCfg(m_param.hunyan_type);
	if (nullptr == hunyan_cfg)
	{
		return;
	}

	if (m_param.today_xunyou_give_flower_buy_count >= hunyan_cfg->buy_give_flower_count)
	{
		m_role->NoticeNum(errornum::NT_MYSTERIOUS_SHOP_BUY_LIMIT);
		return;
	}

	if (!m_role->GetKnapsack()->GetMoney()->GoldMoreThan(hunyan_cfg->flower_need_gold))
	{
		m_role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_role->GetKnapsack()->GetMoney()->UseGold(hunyan_cfg->flower_need_gold, "Xunyou::GiveFlower");

	m_param.today_xunyou_give_flower_buy_count++;

	this->SendWeddingInfo(Protocol::QINGYUAN_INFO_TYPE_XUNYOU_INFO);
}

void Qingyuan::OnXunyouGiveFlower(ItemID *item_id, int *flower_num)
{
	if (nullptr == item_id || nullptr == flower_num)
	{
		return;
	}

	const XunyouCfg *hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouCfg(m_param.hunyan_type);
	if (nullptr == hunyan_cfg)
	{
		return;
	}

	const OtherItem *item = (const OtherItem*)ITEMPOOL->GetItem(hunyan_cfg->flower_id);
	if (NULL == item || ItemBase::I_TYPE_OTHER != item->GetItemType() || OtherItem::I_SUB_TYPE_FLOWER != item->GetSubType() || item->GetValue() <= 0)
	{
		m_role->NoticeNum(errornum::EN_FLOWER_ITEM_TYPE_ERROR);
		return;
	}

	*item_id = hunyan_cfg->flower_id;
	*flower_num = item->GetValue();

	m_param.today_xunyou_give_flower_count++;

	this->SendWeddingInfo(Protocol::QINGYUAN_INFO_TYPE_XUNYOU_INFO);
}

void Qingyuan::CheckTransToXunyouPos()
{
	if (this->IsInXunyouStatus() && m_role->GetScene()->InStaticScene())
	{
		int scene_id = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouSceneId();
		const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();
		Posi pos = (m_role->GetSex() == FEMALE) ? GetXunyouObjCurPos(hunyan_cfg.female_marryobj_seq) : GetXunyouObjCurPos(hunyan_cfg.male_marryobj_seq);
		bool is_trans = false;
		if (m_role->GetScene()->GetSceneID() != scene_id || m_role->GetScene()->GetSceneKey() != 0)
		{
			is_trans = true;
		}
		else if (m_role->GetPos() != pos && !m_is_trans_to_xunyou_scene)
		{
			is_trans = true;
		}

		if (is_trans)
		{
			m_is_trans_to_xunyou_scene = true;
			World::GetInstWorld()->GetSceneManager()->GoTo(m_role, scene_id, 0, pos);
		}
	}
}

void Qingyuan::CheckSyncAOIInXunyou(unsigned int now_second)
{
	if (this->IsInXunyouStatus() && now_second >= m_next_sync_aoi_timestamp)
	{
		int scene_id = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouSceneId();
		auto my_id = m_role->GetScene()->GetSceneID();

		if (my_id == scene_id)
		{
			const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();
			Posi pos = (m_role->GetSex() == FEMALE) ? GetXunyouObjCurPos(hunyan_cfg.female_marryobj_seq) : GetXunyouObjCurPos(hunyan_cfg.male_marryobj_seq);

			if (pos.x != 0 && pos.y != 0)
			{
				m_role->ResetPos(pos.x, pos.y);
			}

			m_next_sync_aoi_timestamp = now_second + 1;
		}
	}
}

void Qingyuan::OnSendXunyouObjInfo(int seq)
{
	Posi obj_pos(0, 0);
	ObjID obj_id = INVALID_OBJ_ID;
	int scene_id = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouSceneId();

	SpecialHunyan *hunyan_scene = World::GetInstWorld()->GetSceneManager()->GetSpecialHunyan(COMMON_SCENE_KEY, false);
	if (NULL != hunyan_scene)
	{
		obj_pos = hunyan_scene->GetXunyouObjCurPos(seq, obj_id);
	}

	{
		Protocol::SCQingYuanAllInfo msg;
		msg.info_type = Protocol::QINGYUAN_INFO_TYPE_XUNYOU_OBJ_POS;
		msg.param_ch1 = (IsInXunyouStatus() ? 1 : 0);
		msg.param2 = obj_pos.x;
		msg.param3 = obj_pos.y;
		msg.param4 = obj_id;
		msg.param5 = scene_id;

		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&msg, sizeof(msg));
	}
}

const Posi Qingyuan::GetXunyouObjCurPos(int seq)
{
	Posi obj_pos(0, 0);
	ObjID obj_id = INVALID_OBJ_ID;
	SpecialHunyan *hunyan_scene = World::GetInstWorld()->GetSceneManager()->GetSpecialHunyan(COMMON_SCENE_KEY, false);
	if (NULL != hunyan_scene)
	{
		obj_pos = hunyan_scene->GetXunyouObjCurPos(seq, obj_id);
	}

	return obj_pos;
}

// 表白墙

void Qingyuan::OnProfessWallReq(Protocol::CSProfessWallReq* msg)
{
	if (nullptr == msg)
		return;

	switch (msg->oper_type)
	{
	case Protocol::PROFESS_WALL_REQ_INFO:	// 请求表白墙信息
	{
		this->SendProfessWallInfo(static_cast<unsigned short>(msg->param_1), Protocol::PROFESS_INFO_NOTIFY_TYPE_NORMAL);
	}
	break;

	case Protocol::PROFESS_WALL_REQ_LEVEL_INFO:	// 请求表白等级信息
	{
		this->SendProfessLevelInfo();
	}
	break;

	case Protocol::PROFESS_WALL_REQ_DELETE:
	{
		this->OnDeleteProfessItem(msg->param_1, msg->param_2, static_cast<int>(msg->param_3));
	}
	break;

	default:
		break;
	}
}

void Qingyuan::OnProfessTo(int other_role_id, int gift_type, bool is_auto_buy, ProfessContent& content)
{
	if (nullptr == content || other_role_id <= 0)
	{
		return;
	}
		
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now < m_last_profess_time + 1)
	{
		gstr::SendError(m_role, "opera_interval_limit");
		return;
	}

	// 判断是否在线
	UserID role_uid_to = IntToUid(other_role_id);
	Role* role_to = World::GetInstWorld()->GetSceneManager()->GetRole(role_uid_to);
	if (nullptr == role_to)
	{
		m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	// 礼物配置
	const QingyuanProfessGiftConfig *cur_gift_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetProfessGift(gift_type);
	if (nullptr == cur_gift_cfg)
	{
		return;
	}

	// 消耗礼物
	ItemExtern::ItemConsumeConfigList stuff_list;
	stuff_list.add_item(ItemExtern::ItemConsumeConfig(cur_gift_cfg->gift_id, is_auto_buy, 1));

	if (!stuff_list.done(*m_role, "OnLoveProfessTo"))
	{
		return;
	}

	// 记录新表白item
	static PersonProfessItem profess_item; profess_item.Reset();
	{
		profess_item.other_role_id = other_role_id;
		profess_item.profess_time = now;
		profess_item.gift_type = gift_type;
		UNSTD_STATIC_CHECK(sizeof(profess_item.content) == sizeof(content));
		F_STRNCPY(profess_item.content, content, sizeof(profess_item.content));
	}

	m_last_profess_time = now;

	if (m_profess_to.size() >= PERSON_PROFESS_MAX_NUM)
	{
		m_profess_to.pop_front();
	}

	m_profess_to.push_back(profess_item);

	// 增加自身魅力
	m_role->GetFriendManager()->AddAllCharm(cur_gift_cfg->self_charm);

	// 增加自身表白经验
	this->AddProfessExp(cur_gift_cfg->exp);

	// 增加 "表白排行榜活动" 积分
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PROFESS_RANK))
	{
		m_role->GetRoleActivity()->OnRAProfessAddScore(cur_gift_cfg->score, true);
	}

	// 更新对方数据
	role_to->GetQingyuan()->OnProfessFrom(m_role->GetUID(), profess_item);
// 	UserID role_uid_to = IntToUid(other_role_id);
// 	{
// 		Role* role_to = World::GetInstWorld()->GetSceneManager()->GetRole(role_uid_to);
// 		if (nullptr != role_to)
// 		{
// 			role_to->GetQingyuan()->OnProfessFrom(m_role->GetUID(), profess_item);
// 		}
// 		else 
// 		{
// 			// 离线记录
// 			OfflineRegisterManager::Instance().OnProfessFrom(role_uid_to, m_role, profess_item);
// 		}
//	}

	// 世界特效
	{
		Protocol::SCProfessWallEffect mse;
		mse.effect_type = cur_gift_cfg->gift_effect;
		World::GetInstWorld()->SendToAllGateway((const char *)&mse, sizeof(mse));
	}

	// 记录到公共表白墙
	WorldStatus2::Instance().RecoredProfession(m_role, profess_item);

	// 更新客户端表白墙信息
	SendProfessWallInfo(PROFESS_WALL_TYPE_SELF, Protocol::PROFESS_INFO_NOTIFY_TYPE_NORMAL);

	// 表白传闻
	{
		int length = 0;
		switch (gift_type)
		{
		case PROFESS_GIFT_TYPE_CHAMPAGNE:
		{
			length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "profess_notice_anonymity1",
				UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(),
				UidToInt(role_to->GetUserId()), role_to->GetName(), (int)role_to->GetCamp(),
				cur_gift_cfg->gift_id);
		}
		break;

		case PROFESS_GIFT_TYPE_HOUSEBOAT:
		{
			length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "profess_notice_anonymity2",
				UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(),
				UidToInt(role_to->GetUserId()), role_to->GetName(), (int)role_to->GetCamp(),
				cur_gift_cfg->gift_id);
		}
		break;

		case PROFESS_GIFT_TYPE_ROCKET:
		{
			length = gstr::SysMsgContent(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), "profess_notice_anonymity3",
				UidToInt(m_role->GetUserId()), m_role->GetName(), (int)m_role->GetCamp(),
				UidToInt(role_to->GetUserId()), role_to->GetName(), (int)role_to->GetCamp(),
				cur_gift_cfg->gift_id);
		}
		break;

		default:
			break;
		}

		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_NOT_CHAT);
		}
	}

//	gstr::SendNotice(m_role, "profess_to_succ");
}

void Qingyuan::OnProfessFrom(int other_role_id, PersonProfessItem& profess_item)
{
	if (other_role_id <= 0)
		return;

	// 礼物配置
	const QingyuanProfessGiftConfig *cur_gift_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetProfessGift(profess_item.gift_type);
	if (nullptr == cur_gift_cfg)
	{
		return;
	}

	// 记录新表白item
	static PersonProfessItem profess_item_from; profess_item_from.Reset();
	{
		profess_item_from.other_role_id = other_role_id;
		profess_item_from.profess_time = profess_item.profess_time;
		profess_item_from.gift_type = profess_item.gift_type;
		UNSTD_STATIC_CHECK(sizeof(profess_item_from.content) == sizeof(profess_item.content));
		F_STRNCPY(profess_item_from.content, profess_item.content, sizeof(profess_item_from.content));
	}

	if (m_profess_from.size() >= PERSON_PROFESS_MAX_NUM)
	{
		m_profess_from.pop_front();
	}

	m_profess_from.push_back(profess_item_from);

	// 增加自身魅力
	m_role->GetFriendManager()->AddAllCharm(cur_gift_cfg->other_charm);

	// 增加表白经验
	double add_per = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanProfessExpAddPer() * ROLE_ATTR_PER;
	if (add_per > 0)
	{
		int add_exp = static_cast<int>(cur_gift_cfg->exp * add_per);
		this->AddProfessExp(add_exp);
	}

	// 增加 "表白排行榜活动" 积分
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_PROFESS_RANK))
	{
		m_role->GetRoleActivity()->OnRAProfessAddScore(cur_gift_cfg->score, false);
	}

	// 更新客户端表白墙信息
	SendProfessWallInfo(PROFESS_WALL_TYPE_OTHER, Protocol::PROFESS_INFO_NOTIFY_TYPE_PROFESS_FROM);
}

void Qingyuan::OnDeleteProfessItem(unsigned short profess_wall_type, unsigned int profess_time, int role_id)
{
	if (profess_wall_type >= PROFESS_WALL_TYPE_MAX)
	{
		return;// 表白墙类型错误！！！
	}

	switch (profess_wall_type)
	{
	case PROFESS_WALL_TYPE_SELF:
	{
		this->DeleteProfessItemFunc(m_profess_to, profess_time, role_id);
		this->SendProfessWallInfoFunc(m_profess_to, profess_wall_type, Protocol::PROFESS_INFO_NOTIFY_TYPE_NORMAL);
	}
	break;

	case PROFESS_WALL_TYPE_OTHER:
	{
		this->DeleteProfessItemFunc(m_profess_from, profess_time, role_id);
		this->SendProfessWallInfoFunc(m_profess_from, profess_wall_type, Protocol::PROFESS_INFO_NOTIFY_TYPE_NORMAL);
	}
	break;

	default:
		break;
	}
}

void Qingyuan::SendProfessWallInfo(unsigned short profess_wall_type, short notify_type)
{
	if (profess_wall_type < 0 || profess_wall_type >= PROFESS_WALL_TYPE_MAX)
	{
		return;// 表白墙类型错误！！！
	}

	switch (profess_wall_type)
	{
	case PROFESS_WALL_TYPE_SELF:
	{
		this->SendProfessWallInfoFunc(m_profess_to, profess_wall_type, notify_type);
	}
	break;

	case PROFESS_WALL_TYPE_OTHER:
	{
		this->SendProfessWallInfoFunc(m_profess_from, profess_wall_type, notify_type);
	}
	break;

	case PROFESS_WALL_TYPE_GLOBAL:
	{
		WorldStatus2::Instance().SendProfessionInfo(m_role);
	}
	break;

	default:
		break;
	}
}

void Qingyuan::DeleteProfessItemFunc(std::deque<PersonProfessItem>& profess_deque, unsigned int profess_time, int role_id)
{
	if (profess_time <= 0 || role_id <= 0)
	{
		m_role->SendOperateResult(Protocol::SCOperateResult::OP_DELETE_PROFESS, false);
		return;
	}

	// 遍历删除
	auto iter = profess_deque.begin();
	for (; iter != profess_deque.end(); ++iter)
	{
		if (iter->other_role_id == role_id && iter->profess_time == profess_time)
		{
			profess_deque.erase(iter);
			m_role->SendOperateResult(Protocol::SCOperateResult::OP_DELETE_PROFESS, true, (int)profess_time, role_id);
			break;
		}
	}
}

void Qingyuan::SendProfessWallInfoFunc(std::deque<PersonProfessItem>& profess_deque, unsigned short profess_wall_type, short notify_type)
{
	static Protocol::SCPersonProfessWallInfo profess_wall_info;

	// 发给客户端的表白item数量
	short item_count = 0;

	for (auto& person_item : profess_deque)
	{
		Protocol::SCPersonProfessWallInfo::ProfessItem& msg_item = profess_wall_info.items[item_count];

		if (person_item.profess_time != 0)// person_item.profess_time为0，则说明无表白
		{
			// 对方名字、头像
			if (Role* other_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(person_item.other_role_id)))
			{
				msg_item.avatar_timestamp = other_role->GetAvatarTimeStamp();
				F_STRNCPY(msg_item.other_name, other_role->GetName(), sizeof(GameName));
			}
			else if (UserCacheNode *other_role_node = UserCacheManager::Instance().GetUserNode(IntToUid(person_item.other_role_id)))
			{
				msg_item.avatar_timestamp = other_role_node->avatar_timestamp;
				F_STRNCPY(msg_item.other_name, other_role_node->user_name, sizeof(GameName));
			}
			else
			{
				continue;
			}

			// 对方id、表白时间、礼物类型、表白内容
			msg_item.other_role_id = person_item.other_role_id;
			msg_item.profess_time = person_item.profess_time;
			msg_item.gife_type = person_item.gift_type;
			UNSTD_STATIC_CHECK(sizeof(msg_item.content) == sizeof(person_item.content));
			F_STRNCPY(msg_item.content, person_item.content, sizeof(msg_item.content));

			// 数量增加
			item_count++;

			// 限制检测
			if (item_count >= PERSON_PROFESS_MAX_NUM)
			{
				break;
			}
		}
	}

	// 其他信息
	profess_wall_info.profess_count = item_count;
	profess_wall_info.timestamp = (unsigned int)EngineAdapter::Instance().Time();
	profess_wall_info.profess_type = static_cast<char>(profess_wall_type);
	profess_wall_info.notify_type = static_cast<char>(notify_type);

	// 发送
	int send_size_byte = sizeof(Protocol::SCPersonProfessWallInfo) - (PERSON_PROFESS_MAX_NUM - item_count) * sizeof(Protocol::SCPersonProfessWallInfo::ProfessItem);
	if (send_size_byte > 0)
	{
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&profess_wall_info, send_size_byte);
	}
}

// 表白等级

void Qingyuan::AddProfessExp(int exp)
{
	if (exp <= 0)
	{
		return;
	}

	QingyuanGlobal *qy_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetUID());
	if (nullptr == qy_global)
	{
		return;
	}

	const QingyuanProfessGradeConfig *cur_grade_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetProfessGrade(qy_global->GetProfessLevel());
	if (nullptr == cur_grade_cfg)
	{
		return;
	}

	const QingyuanProfessGradeConfig *next_cur_grade_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetProfessGrade(qy_global->GetProfessLevel() + 1);
	if (nullptr == next_cur_grade_cfg)
	{
		return;
	}

	bool is_level_up = false;

	// 当前
	UInt32 current_exp = qy_global->GetProfessExp();
	current_exp += exp;

	// 下一级所需经验
	unsigned int levelup_need_exp = cur_grade_cfg->exp;
	if (levelup_need_exp <= 0)
	{
		return;
	}

	for (; current_exp >= levelup_need_exp;)
	{
		// 升级
		current_exp -= levelup_need_exp;
		qy_global->ProfessLevelUp();
		is_level_up = true;

		// 下一级
		cur_grade_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetProfessGrade(qy_global->GetProfessLevel());
		if (nullptr == cur_grade_cfg)
		{
			break;
		}

		next_cur_grade_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetProfessGrade(qy_global->GetProfessLevel() + 1);
		if (nullptr == next_cur_grade_cfg)
		{
			current_exp = 0;
			break;
		}

		levelup_need_exp = cur_grade_cfg->exp;
		if (levelup_need_exp <= 0)
		{
			break;
		}
	}

	// 保存剩余的exp
	if (current_exp < 0)
	{
		current_exp = 0;
	}
	qy_global->SetProfessExp(current_exp);

	// 升级通知
	if (is_level_up)
	{
		this->SendProfessLevelInfo();
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);

		Role* lover = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role->GetLoverUid()));
		if (nullptr != lover)
		{
			lover->GetQingyuan()->SendProfessLevelInfo();
			lover->ReCalcAttr(CharIntAttrs::RECALC_REASON_QINGYUAN);
		}
	}
}

void Qingyuan::SendProfessLevelInfo()
{
	static Protocol::SCProfessLevelInfo msg;

	QingyuanGlobal *qy_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetUID());
	if (nullptr == qy_global)
	{
		return;
	}

	msg.my_grade = qy_global->GetProfessLevel();
	msg.my_exp = qy_global->GetProfessExp();
	msg.other_grade = 0;
	msg.other_exp = 0;

	QingyuanGlobal *lover_qy_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(m_role->GetLoverUid());
	if (nullptr != lover_qy_global)
	{
		msg.other_grade = lover_qy_global->GetProfessLevel();
		msg.other_exp = lover_qy_global->GetProfessExp();
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&msg, sizeof(msg));
}
