#include "specialhunyan.hpp"

#include "gamecommon.h"

#include "obj/character/role.h"
#include "obj/otherobj/gatherobj.h"
#include "engineadapter.h"

#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "scene/marryshadow/marryshadow.h"

#include "config/marryconfig.hpp"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/string/globalstr.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/commondata.hpp"
#include "other/route/mailroute.hpp"
#include "npc/transmit/fbmanager.hpp"
#include "item/itempool.h"

#include "config/logicconfigmanager.hpp"
#include "other/qingyuan/qingyuanconfig.hpp"

#include "world.h"
#include "gstr.h"
#include "global/worldstatus2/worldstatus2.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/usercache/usercache.hpp"
#include "gameworld/protocal/msgscguild.hpp"

#include "obj/character/marryobj.hpp"
#include "obj/otherobj/fallingitem.h"
#include "item/knapsack.h"
#include "friend/friendmanager.h"
#include "other/qingyuan/qingyuan.hpp"
#include "other/itemlevelreward/itemlevelreward.hpp"
 
SpecialHunyan::SpecialHunyan(Scene *scene)
	: SpecialLogic(scene), m_state(HUNYAN_STATE_TYPE_INVALID), m_hunyan_type(-1), m_next_state_time(0), m_next_add_exp_time(0), m_baitang_end_time(0), m_wedding_liveness(0), m_wedding_liveness_reward_flag(0), m_cur_wedding_seq(0), m_red_bag_num(0), m_cur_red_bag_turn(-1), m_is_in_red_bag_flush_time(false), m_wedding_liveness_is_change(false), m_bai_tang_status(HUANYAN_BAITANG_STATUS_INVALID), m_is_first_role_enter(0),
	m_cur_xunyou_point_index(0), m_xunyou_obj_stop_time(0), m_next_report_xunyou_time(0), m_next_sync_xunyou_pos_time(0)
{
	memset(m_mem_name, 0, sizeof(m_mem_name));
	memset(m_women_name, 0, sizeof(m_women_name));
	memset(m_xunyou_obj_id_list, 0, sizeof(m_xunyou_obj_id_list));
}

SpecialHunyan::~SpecialHunyan()
{
}

void SpecialHunyan::Update(unsigned long interval, time_t now_second)
{
	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
	const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();

	if (HUNYAN_STATE_TYPE_START == m_state && now_second >= m_next_add_exp_time)
	{
		for (int i = 0; i < (int)m_scene->RoleNum(); i++)
		{
			Role *role = m_scene->GetRoleByIndex(i);
			if (NULL != role)
			{
				//long long add_exp = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingAddExp(role->GetLevel(), m_hunyan_type);

				// 热度加成
				//const WeddingLivenessCfg &liveness_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingLivenessCfg();
				//for (int i = WEDDING_MAX_LIVENESS_TYPE - 1; i >= 0; --i)
				//{
				//	if (m_wedding_liveness >= liveness_cfg.cfg_list[i].liveness_var)
				//	{
				//		add_exp += add_exp * liveness_cfg.cfg_list[i].wedding_rewrad_exp_add_per / 100;
				//		break;
				//	}
				//}

				//策划需求，修改增加经验规则（2018-1-3）
				//long long add_exp = static_cast<long long>(LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg().exp_factor * (LOGIC_CONFIG->GetItemLevelRewardConfig().GetExpDanReward(role->GetLevel(), 0)));

				long long add_exp = static_cast<long long>(LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig().per_jingyan);

				if (add_exp <= 0)
				{
					continue;
				}

				if (role->GetQingyuan()->GetTodayHunyanJoinTimes() <= hunyan_cfg.give_exp_hunyan_times_limit)
				{
					role->AddExp(add_exp, "QingYuanWedding", Role::EXP_ADD_REASON_DEFAULT);

					std::map<int, long long>::iterator it = m_wedding_role_exp_map.find(role->GetUID());
					if (it == m_wedding_role_exp_map.end())
					{
						m_wedding_role_exp_map[role->GetUID()] = add_exp;
					}
					else
					{
						it->second += add_exp;
					}
				}

				this->SendWeddingRoleInfo(role);
			}
		}

		m_next_add_exp_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + other_cfg.wedding_add_exp_cd_s;

	}
	if (HUNYAN_STATE_TYPE_START == m_state && m_wedding_liveness_is_change)
	{
		const WeddingLivenessCfg &liveness_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingLivenessCfg();
		for (int i = 0; i < WEDDING_MAX_LIVENESS_TYPE; ++i)
		{
			// 达到热度，刷采集物
			if (liveness_cfg.cfg_list[i].liveness_var > 0
				&& m_wedding_liveness >= liveness_cfg.cfg_list[i].liveness_var 
				&& (0 == (m_wedding_liveness_reward_flag & (1 << i)))
				)
			{
				m_scene->DeleteGatherObjByGatherId(other_cfg.gather_rb_id);
				m_cur_red_bag_turn = i;
				m_red_bag_num = liveness_cfg.cfg_list[i].red_bag_num;
				m_is_in_red_bag_flush_time = true;

				Posi init_pos(other_cfg.wedding_red_bag_x, other_cfg.wedding_red_bag_y);
				int offset = other_cfg.wedding_red_bag_offset;

				for (int seq = 0; seq < m_red_bag_num && seq < 100; ++seq)
				{
					GatherObj *gather_obj = new GatherObj();
					gather_obj->Init(NULL, other_cfg.gather_rb_id, 5 * 1000, 0, true, 180);

					Posi red_bag_pos = init_pos;

					const LivenessGatherPosCfg *pos_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetLivenessGatherPosCfg(seq);
					if (NULL != pos_cfg)
					{
						red_bag_pos.x = pos_cfg->x_pos;
						red_bag_pos.y = pos_cfg->y_pos;
					}

					bool is_set_pos = false;
					for (int k = 0; k < 10; ++k)
					{
						if (m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, red_bag_pos.x, red_bag_pos.y))
						{
							gather_obj->SetPos(red_bag_pos);
							is_set_pos = true;
							break;
						}

						red_bag_pos.x += (RandomNum(offset) - offset / 2);
						red_bag_pos.y += (RandomNum(offset) - offset / 2);
					}

					if (!is_set_pos)
					{
						gather_obj->SetPos(init_pos);
					}

					m_scene->AddObj(gather_obj);
				}

				// 同步信息
				m_wedding_liveness_reward_flag |= (1 << i);
				for (int i = 0; i < (int)m_scene->RoleNum(); i++)
				{
					Role *role = m_scene->GetRoleByIndex(i);
					if (NULL != role)
					{
						this->SendWeddingRoleInfo(role);
					}
				}

				this->BrocastLiveness(liveness_cfg.cfg_list[i].liveness_var);
			}
		}

		m_wedding_liveness_is_change = false;
	}

	if (HUNYAN_STATE_TYPE_START == m_state && m_bai_tang_status == HUANYAN_BAITANG_STATUS_START)
	{
		if (now_second >= m_baitang_end_time)
		{
			m_bai_tang_status = HUANYAN_BAITANG_STATUS_END;
			for (int i = 0; i < (int)m_scene->RoleNum(); i++)
			{
				Role *role = m_scene->GetRoleByIndex(i);
				if (NULL != role)
				{
					this->SendWeddingRoleInfo(role);
				}
			}
		}
	}

	// 巡游
	this->CheckUpdateXunyou(interval, now_second);
}

void SpecialHunyan::OnRoleEnterScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	std::set<int>::iterator it = m_enter_role_set.find(role->GetUID());
	if(it == m_enter_role_set.end())
	{
		role->GetQingyuan()->AddTodayHunyanJoinTimes();
	}

	m_enter_role_set.insert(role->GetUID());
	
	if (!m_is_first_role_enter)
	{
		FBManager::Instance().SaveHunYanInviteInfo(role, m_scene->GetSceneKey());
	}

	if (0 == m_marryuser_list[0].marry_uid)
	{
		UNSTD_STATIC_CHECK(2 == sizeof(m_marryuser_list) / sizeof(m_marryuser_list[0]));
		m_marryuser_list[0].marry_uid = role->GetUID();
		role->GetName(m_marryuser_list[0].marry_name);
		m_marryuser_list[0].prof = role->GetProf();
		m_marryuser_list[0].sex = role->GetSex();
		m_marryuser_list[0].camp = role->GetCamp();
		m_marryuser_list[0].hongbao_count = 0;
		m_marryuser_list[0].avator_timestamp = role->GetAvatarTimeStamp();

		Role *marry_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_marryuser_list[0].marry_uid));
		if (NULL != marry_role)
		{
			if (FEMALE == m_marryuser_list[0].sex)
			{
				F_STRNCPY(m_women_name, m_marryuser_list[0].marry_name, sizeof(m_women_name));
				marry_role->GetLoverName(m_mem_name);
			}
			else if (MALE == m_marryuser_list[0].sex)
			{
				F_STRNCPY(m_mem_name, m_marryuser_list[0].marry_name, sizeof(m_mem_name));
				marry_role->GetLoverName(m_women_name);
			}

			marry_role->GetLoverName(m_marryuser_list[1].marry_name);
			m_marryuser_list[1].marry_uid = marry_role->GetLoverUid();
		}

// 		Protocol::SCSpecialParamChange spc;
// 		spc.obj_id = role->GetId();
// 		spc.change_type = 0;
// 		spc.param = 1;
// 		role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&spc, sizeof(spc));
	}

	if (!this->IsMarryUser(role->GetUID(), NULL))
	{
		this->SendRoleYanhuaInfo(role);
	}

	this->SendHunyanInfo(role, Protocol::HUNYAN_NOTIFY_REASON_ENTER_HUNYAN);
	this->SendWeddingRoleInfo(role);
	
	if (!m_is_first_role_enter)
	{
		FBManager::Instance().SendHunYanFbInviteInfoToWorld();
		m_is_first_role_enter = true;
	}
}

void SpecialHunyan::OnRoleLeaveScene(Role *role, bool is_logout)
{
	if (NULL == role)
	{
		return;
	}

	//this->SendHunyanInfo(NULL, Protocol::HUNYAN_NOTIFY_REASON_STATE_CHANGE, role);
	this->SendHunyanInfo(role, Protocol::HUNYAN_NOTIFY_REASON_LEAVE);
// 	for (int i = 0; i < Protocol::HUNYAN_MARRY_USER_COUNT; ++i)
// 	{
// 		Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_marryuser_list[i].marry_uid));
// 		if (NULL != lover_role)
// 		{
// 			this->SendHunyanInfo(lover_role, Protocol::HUNYAN_NOTIFY_REASON_GIVE_HONGBAO);
// 		}
// 	}
}

int SpecialHunyan::GetGatherByUid(int uid)
{
	std::map<int, int>::iterator it = m_gathar_num_map.find(uid);
	if (it != m_gathar_num_map.end())
	{
		const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();
		if (YUYUE_HUNYAN_TYPE_1 == m_hunyan_type)
		{
			if (it->second > hunyan_cfg.bind_diamonds_hy_gather_max)
			{
				return 0; 
			}
		}
		else
		{
			if (it->second > hunyan_cfg.diamonds_hy_gather_max)
			{
				return 0; 
			}
		}

		return it->second;
	}
	return 0;
}

void SpecialHunyan::OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &pos)
{
	if (NULL == role)
	{
		return;
	}

	if (HUNYAN_STATE_TYPE_START != m_state)
	{
		role->NoticeNum(errornum::EN_HUNYAN_NOT_START);
		return;
	}

	const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();
	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
	const QingyuanHunyanActiviryConfig * hunyan_act_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanHunyanActivityCfg();
	if (NULL == hunyan_act_cfg)
	{
		return;
	}

	if (gather_id != hunyan_cfg.gathar_id && gather_id != hunyan_act_cfg->paohuaqui_gather_id && gather_id != other_cfg.gather_rb_id)
	{
		return;
	}

	//酒席食物
	if (gather_id == hunyan_cfg.gathar_id)	
	{
		m_role_gather_record[role->GetUID()].insert(obj_id);

//		bool bind_gold_succ = RandomNum(100) <= hunyan_act_cfg->food_get_diamonds_per;
//		bool other_stuff_succ = RandomNum(100) <= hunyan_act_cfg->food_other_reward_item_per;

		int need_space_num = 1;
// 		if (other_stuff_succ)
// 		{
// 			need_space_num += 1;
// 		}

		if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(need_space_num))
		{
			role->NoticeNum(errornum::EN_HAS_NO_SPACING);
			return;
		}

		std::map<int, int>::iterator it = m_gathar_num_map.find(role->GetUID());
		if (it != m_gathar_num_map.end())
		{
			if (YUYUE_HUNYAN_TYPE_1 == m_hunyan_type)
			{
				if (it->second >= hunyan_cfg.bind_diamonds_hy_gather_max)
				{
					gstr::SendError(role, "en_hunyan_liveness_gather_turn_limit");
					return; 
				}
			}
			else
			{
				if (it->second >= hunyan_cfg.diamonds_hy_gather_max)
				{
					gstr::SendError(role, "en_hunyan_liveness_gather_turn_limit");
					return; 
				}
			}

			++ it->second;
			FBManager::Instance().SendHunYanFbInviteInfoToRole(role);
		}
		else 
		{
			m_gathar_num_map[role->GetUID()] = 1;
		}

// 		if (role->GetQingyuan()->GetMaxGetExps() >= hunyan_act_cfg->per_day_max_exp)
// 		{
// 			role->NoticeNum(errornum::EN_HUNYAN_GET_NUM_MAX_EXP);
// 		}
// 		else
// 		{
// 			if (role->GetQingyuan()->GetMaxGetExps() + hunyan_act_cfg->food_get_exp > hunyan_act_cfg->per_day_max_exp)
// 			{
// 				int add_exp = hunyan_act_cfg->per_day_max_exp - role->GetQingyuan()->GetMaxGetExps();
// 
// 				role->GetQingyuan()->AddMaxGetExps(add_exp);
// 				role->AddExp(add_exp, "hunyan_food_reward");
// 			}
// 			else
// 			{
// 				role->GetQingyuan()->AddMaxGetExps(hunyan_act_cfg->food_get_exp);
// 				role->AddExp(hunyan_act_cfg->food_get_exp, "hunyan_food_reward");
// 			}
// 		}

		if (role->GetQingyuan()->GetMaxGetGatherDayMaxStuffs() >= hunyan_act_cfg->hunyan_gather_day_max_stuff_num)
		{
			role->NoticeNum(errornum::EN_HUNYAN_GATHER_XIANHUAN_LIMIT);
		}
		else
		{
			ItemConfigData stuff;
			stuff.item_id = hunyan_act_cfg->uplevel_stuff_id;
			stuff.is_bind = true;
			stuff.num = hunyan_act_cfg->food_get_hunjie_stuf_num;

			if (role->GetQingyuan()->GetMaxGetGatherDayMaxStuffs() + hunyan_act_cfg->food_get_hunjie_stuf_num > hunyan_act_cfg->hunyan_gather_day_max_stuff_num)
			{
				stuff.num = hunyan_act_cfg->hunyan_gather_day_max_stuff_num - role->GetQingyuan()->GetMaxGetGatherDayMaxStuffs();

				role->GetQingyuan()->AddMaxGetGatherDayMaxStuffs(stuff.num);
				role->GetKnapsack()->Put(stuff, PUT_REASON_HUNYAN_GATHER);
			}
			else
			{
				role->GetQingyuan()->AddMaxGetGatherDayMaxStuffs(stuff.num);
				role->GetKnapsack()->Put(stuff, PUT_REASON_HUNYAN_GATHER);
			}
		}

// 		if (bind_gold_succ)
// 		{
// 			if (role->GetQingyuan()->GetMaxGetDiamonds() >= hunyan_act_cfg->per_day_max_bind_diamonds)
// 			{
// 				role->NoticeNum(errornum::EN_HUNYAN_GET_NUM_MAX_BIND_GOLD);
// 			}
// 			else
// 			{
// 				int add_diamond_num;
// 				if (role->GetQingyuan()->GetMaxGetDiamonds() + hunyan_act_cfg->food_get_diamonds > hunyan_act_cfg->per_day_max_bind_diamonds)
// 				{
// 					add_diamond_num = hunyan_act_cfg->per_day_max_bind_diamonds - role->GetQingyuan()->GetMaxGetDiamonds();
// 
// 					role->GetQingyuan()->AddMaxGetDiamonds(add_diamond_num);
// 					role->GetKnapsack()->GetMoney()->AddGoldBind(add_diamond_num, "hunyan_gather");
// 				}
// 				else
// 				{
// 					role->GetQingyuan()->AddMaxGetDiamonds(hunyan_act_cfg->food_get_diamonds);
// 					role->GetKnapsack()->GetMoney()->AddGoldBind(hunyan_act_cfg->food_get_diamonds, "hunyan_gather");
// 				}
// 			}
// 
// 		}

// 		if (other_stuff_succ)
// 		{
// 			if (role->GetQingyuan()->GetMaxGetOtherStuffs() >= hunyan_act_cfg->per_day_max_other_stuff_num)
// 			{
// 				role->NoticeNum(errornum::EN_HUNYAN_GET_NUM_MAX);
// 			}
// 			else
// 			{
// 				ItemConfigData stuff;
// 				stuff.item_id = hunyan_act_cfg->food_other_reward_item.item_id;
// 				stuff.is_bind = hunyan_act_cfg->food_other_reward_item.is_bind;
// 				stuff.num = hunyan_act_cfg->food_other_reward_item.num;
// 
// 				if (role->GetQingyuan()->GetMaxGetOtherStuffs() + hunyan_act_cfg->food_other_reward_item.num > hunyan_act_cfg->per_day_max_other_stuff_num)
// 				{
// 					stuff.num = hunyan_act_cfg->per_day_max_other_stuff_num - role->GetQingyuan()->GetMaxGetOtherStuffs();
// 
// 					role->GetQingyuan()->AddMaxGetOtherStuffs(stuff.num);
// 					role->GetKnapsack()->Put(stuff, PUT_REASON_HUNYAN_GATHER);
// 				}
// 				else
// 				{
// 					role->GetQingyuan()->AddMaxGetOtherStuffs(hunyan_act_cfg->food_other_reward_item.num);
// 					role->GetKnapsack()->Put(hunyan_act_cfg->food_other_reward_item, PUT_REASON_HUNYAN_GATHER);
// 				}
// 			}
// 		}

		role->NoticeNum(noticenum::NT_HUNYAN_GATHAR_SUCC);
		this->SendHunyanInfo(role, Protocol::HUNYAN_NOTIFY_REASON_GATHER);
		this->SendWeddingRoleInfo(role);
	}

	//热度采集红包
	if (gather_id == other_cfg.gather_rb_id)
	{
		if (m_red_bag_num <= 0)
		{
			return;
		}

		int cur_turn = -1;
		const WeddingLivenessCfg &liveness_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingLivenessCfg();
		for (int i = WEDDING_MAX_LIVENESS_TYPE - 1; i >= 0; --i)
		{
			if (m_wedding_liveness >= liveness_cfg.cfg_list[i].liveness_var)
			{
				cur_turn = liveness_cfg.cfg_list[i].seq;
				break;
			}
		}
		if (cur_turn < 0 || cur_turn >= WEDDING_MAX_LIVENESS_TYPE || liveness_cfg.cfg_list[cur_turn].every_time_bind_gold <= 0)
		{
			return;
		}

		std::map<int, RoleGatherRedBagNum>::iterator it = m_gathar_red_bag_num_map.find(role->GetUID());
		if (it != m_gathar_red_bag_num_map.end())
		{
			if (it->second.gather_num_list[cur_turn] >= liveness_cfg.cfg_list[cur_turn].every_turn_gather_limit)
			{
				gstr::SendError(role, "en_hunyan_liveness_gather_turn_limit");
				return;
			}
			else
			{
				int add_gold_bind = RandomNum(liveness_cfg.cfg_list[cur_turn].every_time_bind_gold) + 1;
				role->GetKnapsack()->GetMoney()->AddGoldBind(add_gold_bind, "HunyanGatherRedBag");

				++it->second.gather_num_list[cur_turn];
			}
		}
		else
		{
			int add_gold_bind = RandomNum(liveness_cfg.cfg_list[cur_turn].every_time_bind_gold) + 1;
			role->GetKnapsack()->GetMoney()->AddGoldBind(add_gold_bind, "HunyanGatherRedBag");

			m_gathar_red_bag_num_map[role->GetUID()].gather_num_list[cur_turn] = 1;
		}

		m_red_bag_num--;

		role->NoticeNum(noticenum::NT_HUNYAN_GATHAR_SUCC);
		this->SendWeddingRoleInfo(role);

		if (m_red_bag_num <= 0)
		{
			m_is_in_red_bag_flush_time = false;
		}
	}

	//检花球
// 	if (gather_id == hunyan_act_cfg->paohuaqui_gather_id)
// 	{
// 		if (!role->GetKnapsack()->HaveSpace())
// 		{
// 			role->NoticeNum(errornum::EN_HAS_NO_SPACING);
// 			return;
// 		}
// 
// 		std::map<int, int>::iterator it = m_paohuaqiu_num_map.find(role->GetUID());
// 		if (it != m_paohuaqiu_num_map.end())
// 		{
// 			if (it->second >= hunyan_act_cfg->phq_gather_max_count)
// 			{
// 				role->NoticeNum(errornum::EN_HUNYAN_GATHAR_TOO_MUCH);
// 				return; 
// 			}
// 
// 			++ it->second;
// 		}
// 		else 
// 		{
// 			m_paohuaqiu_num_map[role->GetUID()] = 1;
// 		}
// 
// 		if (role->GetQingyuan()->GetMaxGetDiamonds() >= hunyan_act_cfg->per_day_max_bind_diamonds)
// 		{
// 			role->NoticeNum(errornum::EN_HUNYAN_GET_NUM_MAX_BIND_GOLD);
// 		}
// 		else
// 		{
// 			int add_diamond_num = 0;
// 			if (role->GetQingyuan()->GetMaxGetDiamonds() + hunyan_act_cfg->phq_reward_diamonds > hunyan_act_cfg->per_day_max_bind_diamonds)
// 			{
// 				add_diamond_num = hunyan_act_cfg->per_day_max_bind_diamonds - role->GetQingyuan()->GetMaxGetDiamonds();
// 
// 				role->GetQingyuan()->AddMaxGetDiamonds(add_diamond_num);
// 				role->GetKnapsack()->GetMoney()->AddGoldBind(add_diamond_num, "HunyanGather");
// 			}
// 			else
// 			{
// 				role->GetQingyuan()->AddMaxGetDiamonds(hunyan_act_cfg->phq_reward_diamonds);
// 				role->GetKnapsack()->GetMoney()->AddGoldBind(hunyan_act_cfg->phq_reward_diamonds, "HunyanGather");
// 			}
// 		}
// 
// 		role->NoticeNum(noticenum::NT_HUNYAN_GATHAR_SUCC);	
// 	}

	this->SendHunyanInfo(role, Protocol::HUNYAN_NOTIFY_REASON_GATHER);
}

bool SpecialHunyan::SpecialCanGather(Role *role, GatherObj *gather)
{
	if (NULL == role || NULL == gather) return false;

	if (HUNYAN_STATE_TYPE_START != m_state)
	{
		role->NoticeNum(errornum::EN_HUNYAN_NOT_START);
		return false;
	}

	// 提醒玩家有人正在采集
// 	if (gather->GetGatherIngObjId() != INVALID_OBJ_ID && gather->GetGatherIngObjId() != role->GetId())
// 	{
// 		unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 		std::map<int, RoleNoticeErrorNumInfo>::iterator it = m_role_notice_errornum_info_map.find(role->GetUID());
// 		if (it == m_role_notice_errornum_info_map.end())
// 		{
// 			gstr::SendError(role, "someone_gather_ing");
// 			RoleNoticeErrorNumInfo temp_info;
// 			temp_info.last_notice_gather_obj_id = gather->GetId();
// 			temp_info.next_notice_gather_timestamp = now + 1;
// 			m_role_notice_errornum_info_map[role->GetUID()] = temp_info;
// 			return false;
// 		}
// 		else
// 		{
// 			if (now >= it->second.next_notice_gather_timestamp || gather->GetId() != it->second.last_notice_gather_obj_id)
// 			{
// 				gstr::SendError(role, "someone_gather_ing");
// 				it->second.last_notice_gather_obj_id = gather->GetId();
// 				it->second.next_notice_gather_timestamp = now + 1;
// 				return false;
// 			}
// 		}
// 	}

	const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();
	const QingyuanOtherConfig &other_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetOtherCfg();
	const QingyuanHunyanActiviryConfig * hunyan_act_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanHunyanActivityCfg();
	if (NULL == hunyan_act_cfg) return false;

	// 酒席
	if (gather->GatherId() == hunyan_cfg.gathar_id)
	{
		if (role->GetQingyuan()->GetMaxGetGatherDayMaxStuffs() >= hunyan_act_cfg->hunyan_gather_day_max_stuff_num)
		{
			role->NoticeNum(errornum::EN_HUNYAN_GATHER_XIANHUAN_LIMIT);
			return false;
		}

		std::map<int, std::set<ObjID> >::iterator it = m_role_gather_record.find(role->GetUID());
		if (it != m_role_gather_record.end())
		{
			if (YUYUE_HUNYAN_TYPE_1 == m_hunyan_type)
			{
				if ((int)it->second.size() >= hunyan_cfg.bind_diamonds_hy_gather_max)
				{
					gstr::SendError(role, "en_hunyan_gather_feast_limit");
					return false; 
				}
			}
			else
			{
				if ((int)it->second.size() >= hunyan_cfg.diamonds_hy_gather_max)
				{
					gstr::SendError(role, "en_hunyan_gather_feast_limit");
					return false; 
				}
			}

			if (it->second.find(gather->GetId()) != it->second.end())
			{
				role->NoticeNum(errornum::EN_MARRY_HUNYAN_GATHER_THIS_FOOD);
				return false; 
			}
		}
	}

	// 热度红包
	else if (gather->GatherId() == other_cfg.gather_rb_id)
	{
		int cur_turn = -1;
		const WeddingLivenessCfg &liveness_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingLivenessCfg();
		for (int i = WEDDING_MAX_LIVENESS_TYPE - 1; i >= 0; --i)
		{
			if (m_wedding_liveness >= liveness_cfg.cfg_list[i].liveness_var)
			{
				cur_turn = liveness_cfg.cfg_list[i].seq;
				break;
			}
		}
		if (cur_turn < 0 || cur_turn >= WEDDING_MAX_LIVENESS_TYPE || liveness_cfg.cfg_list[cur_turn].every_time_bind_gold <= 0)
		{
			gamelog::g_log_debug.printf(LL_INFO, "SpecialHunyan::SpecialCanGather.role(%d,%s),cur_turn[%d]",
				role->GetUID(), role->GetName(), cur_turn);
			return false;
		}

		std::map<int, RoleGatherRedBagNum>::iterator it = m_gathar_red_bag_num_map.find(role->GetUID());
		if (it != m_gathar_red_bag_num_map.end())
		{
			if (it->second.gather_num_list[cur_turn] >= liveness_cfg.cfg_list[cur_turn].every_turn_gather_limit)
			{
				gstr::SendError(role, "en_hunyan_liveness_gather_turn_limit");
				return false;
			}
		}
	}

	// 抛花球
// 	else if (gather->GatherId() == hunyan_act_cfg->paohuaqui_gather_id)
// 	{
// 		std::map<int, int>::iterator it = m_paohuaqiu_num_map.find(role->GetUID());
// 		if (it != m_paohuaqiu_num_map.end())
// 		{
// 			if (it->second >= hunyan_act_cfg->phq_gather_max_count)
// 			{
// 				role->NoticeNum(errornum::EN_HUNYAN_GATHAR_TOO_MUCH);
// 				return false; 
// 			}
// 		}
// 	}
	else
	{
		return false;
	}
	
	return true;
}

void SpecialHunyan::OnRemoveObj(Obj *obj)
{

}

bool SpecialHunyan::SpecialIsEnemy(Role *role, Obj *obj)
{
	return false;
}

bool SpecialHunyan::SpecialIsFriend(Role *role, Obj *obj, bool check_alive)
{
	return true;
}

int SpecialHunyan::GetSpecialParam(Role *role)
{
	if (NULL != role)
	{
		if (IsMarryUser(role->GetUID(), NULL))
		{
			const int HUNYAN_MARRY_USER_FLAG = 1;

			return HUNYAN_MARRY_USER_FLAG;
		}
	}

	return 0;
}

bool SpecialHunyan::IsCanEnterHunYanSence(Role *role)
{
	if (NULL == role)
	{
		return false;
	}

	if (m_state != HUNYAN_STATE_TYPE_START)
	{
		gstr::SendNotice(role, "nt_wedding_not_begin");
		return false;
	}

	if (m_cur_wedding_seq < 0 || m_cur_wedding_seq >= WEDDING_YUYUE_TIME_MAX)
	{
		return false;
	}

	WeddingParam &wedding_param = WorldStatus2::Instance().GetWeddingParam();

	if (role->GetUID() == wedding_param.yuyue_info[m_cur_wedding_seq].role_id
		|| role->GetUID() == wedding_param.yuyue_info[m_cur_wedding_seq].lover_role_id
		|| (WorldStatus2::Instance().IsInWeddingGuests(wedding_param.yuyue_info[m_cur_wedding_seq].role_id, role->GetUID())))
	{
		return true;
	}

	gstr::SendError(role, "hunyan_not_be_invalite");
	return false;
}

void SpecialHunyan::OnHunyanStandBy()
{
	m_state = HUNYAN_STATE_TYPE_STANDBY;

	int cur_wedding_seq = this->GetCurWeddingSeq();
	if (cur_wedding_seq < 0 || cur_wedding_seq >= WEDDING_YUYUE_TIME_MAX)
	{
		this->OnHunyanStop();
		return;
	}

	m_cur_wedding_seq = cur_wedding_seq;
	WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[m_cur_wedding_seq];

	m_hunyan_type = yuyue_info.wedding_type;			// 设置当前婚宴类型

	UserCacheNode *role_node = UserCacheManager::Instance().GetUserNode(yuyue_info.role_id);
	UserCacheNode *lover_role_node = UserCacheManager::Instance().GetUserNode(yuyue_info.lover_role_id);

	if (NULL != role_node && NULL != lover_role_node)
	{
		// 保存双方信息
		{
			m_marryuser_list[0].marry_uid = role_node->uid;
			role_node->GetName(m_marryuser_list[0].marry_name);
			m_marryuser_list[0].camp = role_node->camp;

			m_marryuser_list[1].marry_uid = lover_role_node->uid;
			lover_role_node->GetName(m_marryuser_list[1].marry_name);
			m_marryuser_list[1].camp = lover_role_node->camp;
		}

		// 传闻
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "wedding_stand_by_notice_content", yuyue_info.wedding_index,
			role_node->uid, role_node->user_name, (int)role_node->camp, lover_role_node->uid, lover_role_node->user_name, (int)lover_role_node->camp);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	static Protocol::SCQingYuanAllInfo qyai;

	qyai.info_type = Protocol::QINGYUAN_INFO_TYPE_WEDDING_STANDBY;

	int role_num = m_scene->RoleNum();
	for (int i = 0; i < role_num; ++i)
	{
		Role *scene_role = m_scene->GetRoleByIndex(i);
		if (NULL != scene_role)
		{
			EngineAdapter::Instance().NetSend(scene_role->GetNetId(), (const char *)&qyai, sizeof(qyai));
		}
	}

	Role *owner_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(yuyue_info.role_id));
	if (NULL != owner_role)
	{
		owner_role->GetQingyuan()->SetHunyanFBOpenStatus(0);
	}

	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(yuyue_info.lover_role_id));
	if (NULL != lover_role)
	{
		lover_role->GetQingyuan()->SetHunyanFBOpenStatus(0);
	}

	this->SendHunyanInfo(NULL, Protocol::HUNYAN_NOTIFY_REASON_STATE_CHANGE);
}

void SpecialHunyan::OnHunyanXunyou()
{
	m_state = HUNYAN_STATE_TYPE_XUNYOU;

	this->CreateXunyouObj();

	const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();
	int marryobj_cfg_count = hunyan_cfg.xunyou_marryobj_dis_list.Count();

	m_next_report_xunyou_time = EngineAdapter::Instance().Time() + hunyan_cfg.report_interval;

	Posi bai_tiandi_pos = hunyan_cfg.bai_tiandi_pos.GetPos();
	Posi start_pos = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouPos(0);
	Posi deltaP = bai_tiandi_pos - start_pos;
	Scalar distance = 0;
	Dir dir = gamecommon::GetMoveDir(deltaP, &distance);

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(hunyan_cfg.xunyou_scene_id, 0);
	if (NULL != scene)
	{
		int marry_xunyou_marryobj_dis = 0;
		for (int i = 1; i < marryobj_cfg_count && i < HUNYAN_XUNYOU_MARRYOBJ_MAX_NUM; i++)
		{
			if (INVALID_OBJ_ID == m_xunyou_obj_id_list[i])
			{
				MarryObj *marry_obj = new MarryObj();

				marry_xunyou_marryobj_dis += LOGIC_CONFIG->GetQingyuanCfg().GetXunyouObjDis(i);

				Posi pos = bai_tiandi_pos;
				pos.x = (int)(pos.x + (marry_xunyou_marryobj_dis * cos(dir)));
				pos.y = (int)(pos.y + (marry_xunyou_marryobj_dis * sin(dir)));

				marry_obj->SetPos(pos);
				marry_obj->Init(i);
				scene->AddObj(marry_obj);

				m_xunyou_obj_id_list[i] = marry_obj->GetId();
			}
		}
	}

	//巡游开始广播
	this->BrocastXunyouBegin();
}

void SpecialHunyan::OnHunyanStart()
{
	this->OnDeleteXunyouObj();

	m_state = HUNYAN_STATE_TYPE_START;

	const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();
	const HunyanGatherPos &gather_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanGatherCfg();

	if (YUYUE_HUNYAN_TYPE_1 == m_hunyan_type)
	{
		for (int i = 0; i < hunyan_cfg.bind_diamonds_hy_gather_max && i < gather_cfg.pos_count; ++i)
		{
			GatherObj *gather_obj = new GatherObj();
			gather_obj->Init(NULL, hunyan_cfg.gathar_id, 5 * 1000, 0, false);
			gather_obj->SetPos(gather_cfg.pos_list[i]);
			m_scene->AddObj(gather_obj);
		}
	}

	if (YUYUE_HUNYAN_TYPE_2 == m_hunyan_type)
	{
		for (int i = 0; i < hunyan_cfg.diamonds_hy_gather_max && i < gather_cfg.pos_count; ++i)
		{
			GatherObj *gather_obj = new GatherObj();
			gather_obj->Init(NULL, hunyan_cfg.gathar_id, 5 * 1000, 0, false);
			gather_obj->SetPos(gather_cfg.pos_list[i]);
			m_scene->AddObj(gather_obj);
		}
	}

	if (m_cur_wedding_seq < 0 || m_cur_wedding_seq >= WEDDING_YUYUE_TIME_MAX)
	{
		return;
	}

	WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[m_cur_wedding_seq];

	UserCacheNode *role_node = UserCacheManager::Instance().GetUserNode(yuyue_info.role_id);
	UserCacheNode *lover_role_node = UserCacheManager::Instance().GetUserNode(yuyue_info.lover_role_id);

	if (NULL != role_node && NULL != lover_role_node)
	{
		// 传闻
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "wedding_start_notice_content", yuyue_info.wedding_index,
			role_node->uid, role_node->user_name, (int)role_node->camp, lover_role_node->uid, lover_role_node->user_name, (int)lover_role_node->camp);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	static Protocol::SCQingYuanAllInfo qyai;

	qyai.info_type = Protocol::QINGYUAN_INFO_TYPE_WEDDING_BEGIN_NOTICE;

	Role *owner_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(yuyue_info.role_id));
	if (NULL != owner_role)
	{
		owner_role->GetQingyuan()->SetHunyanFBOpenStatus(0);
		owner_role->GetQingyuan()->SetCurMarryStauts(QINGYUAN_MARRY_STATUS_WEDDING);

		EngineAdapter::Instance().NetSend(owner_role->GetNetId(), (const char*)&qyai, sizeof(qyai));
	}

	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(yuyue_info.lover_role_id));
	if (NULL != lover_role)
	{
		lover_role->GetQingyuan()->SetHunyanFBOpenStatus(0);
		lover_role->GetQingyuan()->SetCurMarryStauts(QINGYUAN_MARRY_STATUS_WEDDING);

		EngineAdapter::Instance().NetSend(lover_role->GetNetId(), (const char*)&qyai, sizeof(qyai));
	}

	// 通知在线玩家婚宴开启
	for (int i = 0; i < WEDDING_GUESTS_MAX_NUM; i++)
	{
		if (yuyue_info.guest_role_id_list[i] != 0)
		{
			Role *guest_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(yuyue_info.guest_role_id_list[i]));
			if (NULL != guest_role)
			{
				EngineAdapter::Instance().NetSend(guest_role->GetNetId(), (const char*)&qyai, sizeof(qyai));
			}
		}
	}

	this->SendHunyanInfo(NULL, Protocol::HUNYAN_NOTIFY_REASON_STATE_CHANGE);
}

void SpecialHunyan::OnHunyanStop()
{
	if (m_state != HUNYAN_STATE_TYPE_STOP) m_state = HUNYAN_STATE_TYPE_STOP;
	if (m_cur_wedding_seq < 0 || m_cur_wedding_seq >= WEDDING_YUYUE_TIME_MAX)
	{
		return;
	}

//	FBManager::Instance().DeleteHunYanInviteInfo(NULL, m_scene->GetSceneKey());

	// 该场婚礼的预约信息
	WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[m_cur_wedding_seq];
	yuyue_info.is_has_done = 1;  // 婚礼已经举办

	// 设置个人状态
	Role *owner_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(yuyue_info.role_id));
	if (NULL != owner_role)
	{
		owner_role->GetQingyuan()->SetHunyanFBOpenStatus(1);
		owner_role->GetQingyuan()->SetCurMarryStauts(QINGYUAN_MARRY_STATUS_INVAILD);

	}

	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(yuyue_info.lover_role_id));
	if (NULL != lover_role)
	{
		lover_role->GetQingyuan()->SetHunyanFBOpenStatus(1);
		lover_role->GetQingyuan()->SetCurMarryStauts(QINGYUAN_MARRY_STATUS_INVAILD);
	}

	// 下发场景信息
	{
		for (std::set<int>::iterator it = m_enter_role_set.begin(), end = m_enter_role_set.end(); it != end; ++it)
		{
			if (0 != *it && *it != m_marryuser_list[0].marry_uid && *it != m_marryuser_list[1].marry_uid)
			{
				Role *enter_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(*it));
				if (NULL != enter_role)
				{
					this->SendHunyanInfo(enter_role, Protocol::HUNYAN_NOTIFY_REASON_STATE_CHANGE); // 发送个人信息
				}
			}
		}

		this->SendHunyanInfo(NULL, Protocol::HUNYAN_NOTIFY_REASON_STATE_CHANGE);
	}

	// 清除场景信息
	this->HunyanStopResetData();
	if (owner_role)
	{
		this->SendWeddingApplicantInfo(owner_role);
	}
	if (lover_role)
	{
		this->SendWeddingApplicantInfo(lover_role);
	}

	//这里清掉该场婚宴的全局信息
	WorldStatus2::Instance().ClearWeddingGuests(m_cur_wedding_seq);

	this->DelayKickOutAllRole();
}

void SpecialHunyan::ZhuFuHelper(Role *role, Protocol::CSMarryHunyanBless *mhb)
{
// 	if (NULL == role) return;
// 
// 	if (this->IsMarryUser(role->GetUID(), NULL))
// 	{
// 		return;
// 	}
// 
// 	std::map<int, int>::iterator it = m_zhufu_num_map.find(role->GetUID());
// 	if (it == m_zhufu_num_map.end())
// 	{
// 		m_zhufu_num_map[role->GetUID()] = 0;
// 		it = m_zhufu_num_map.find(role->GetUID());
// 	}
// 
// 	if (it != m_zhufu_num_map.end())
// 	{
// 		if (it->second >= 1)
// 		{
// 			role->NoticeNum(errornum::EN_MARRY_HUNYAN_ALREADY_BLESS);
// 			return; 
// 		}
// 
// 		const QingyuanHunyanActiviryConfig * hunyan_act_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanHunyanActivityCfg();
// 		Knapsack *knapsack = role->GetKnapsack();
// 		if (NULL == hunyan_act_cfg || NULL == knapsack)
// 		{
// 			return;
// 		}
// 
// 		++ it->second;
// 
// 		ItemConfigData stuff;
// 		stuff.is_bind = true;
// 		stuff.item_id = hunyan_act_cfg->uplevel_stuff_id;
// 		stuff.num = hunyan_act_cfg->zhufu_get_hunjie_stuff_count;
// 
// 		if (role->GetQingyuan()->GetMaxZhufuMaxStuffs() >= hunyan_act_cfg->zhufu_get_hunjie_stuff_limit)
// 		{
// 			role->NoticeNum(errornum::EN_HUNYAN_GATHER_XIANHUAN_LIMIT);
// 		}
// 		else
// 		{
// 			if (role->GetQingyuan()->GetMaxZhufuMaxStuffs() + hunyan_act_cfg->zhufu_get_hunjie_stuff_count > hunyan_act_cfg->zhufu_get_hunjie_stuff_limit)
// 			{
// 				stuff.num = hunyan_act_cfg->zhufu_get_hunjie_stuff_limit - role->GetQingyuan()->GetMaxZhufuMaxStuffs();
// 				role->GetQingyuan()->AddMaxZhufuMaxStuffs(stuff.num);
// 				knapsack->Put(stuff, PUT_REASON_HUNYAN_ZHUFU);
// 			}
// 			else
// 			{
// 				role->GetQingyuan()->AddMaxZhufuMaxStuffs(stuff.num);
// 				knapsack->Put(stuff, PUT_REASON_HUNYAN_ZHUFU);
// 			}
// 		}
// 
// 		int length = 0;
// 		int rand_zhufu_content = RandomNum(3);
// 		if (0 == rand_zhufu_content)
// 		{
// 			length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_hunyan_zhufu1, role->GetUID(), role->GetName(), (int)role->GetCamp());
// 		}
// 		else if (1 == rand_zhufu_content)
// 		{
// 			length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_hunyan_zhufu2, role->GetUID(), role->GetName(), (int)role->GetCamp());
// 		}
// 		else if (2 == rand_zhufu_content)
// 		{
// 			length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_hunyan_zhufu3, role->GetUID(), role->GetName(), (int)role->GetCamp());
// 		}
// 		
// 		if (length > 0)
// 		{
// 			this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 		}
// 
// 		//}
// 
// 		// 发给结婚双方
// 		{
// 			for (int i = 0; i < Protocol::HUNYAN_MARRY_USER_COUNT; ++i)
// 			{	
// 				Role * role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_marryuser_list[i].marry_uid));
// 				if (NULL != role)
// 				{
// 					Knapsack *knapsack = role->GetKnapsack();
// 					if (NULL != knapsack)
// 					{
// 						ItemConfigData stuff;
// 						stuff.is_bind = true;
// 						stuff.item_id = hunyan_act_cfg->uplevel_stuff_id;
// 						stuff.num = hunyan_act_cfg->be_zhufu_get_hunjie_stuff_count;
// 
// 						if (role->GetQingyuan()->GetMaxBeZhufuMaxStuffs() >= hunyan_act_cfg->be_zhufu_get_hunjie_stuff_limit)
// 						{
// 							role->NoticeNum(errornum::EN_HUNYAN_GATHER_XIANHUAN_LIMIT);
// 						}
// 						else
// 						{
// 							if (role->GetQingyuan()->GetMaxBeZhufuMaxStuffs() + hunyan_act_cfg->be_zhufu_get_hunjie_stuff_count > hunyan_act_cfg->be_zhufu_get_hunjie_stuff_limit)
// 							{
// 								stuff.num = hunyan_act_cfg->be_zhufu_get_hunjie_stuff_limit - role->GetQingyuan()->GetMaxBeZhufuMaxStuffs();
// 								role->GetQingyuan()->AddMaxBeZhufuMaxStuffs(stuff.num);
// 								knapsack->Put(stuff, PUT_REASON_HUNYAN_ZHUFU);
// 							}
// 							else
// 							{
// 								role->GetQingyuan()->AddMaxBeZhufuMaxStuffs(stuff.num);
// 								knapsack->Put(stuff, PUT_REASON_HUNYAN_ZHUFU);
// 							}
// 						}
// 					}
// 				}
// 			}
// 		}
// 	}
}

void SpecialHunyan::OnHunyanOpera(Role *role, int opera_type, int param_1, int param_2)
{
	if (NULL == role) return;
	if (opera_type <= HUNYAN_OPERA_TYPE_INVALID || opera_type >= HUNYAN_OPERA_TYPE_MAX) return;

	bool oper_succ = false;

	switch (opera_type)
	{
	case HUNYAN_OPERA_TYPE_RED_BAG:
	{
		this->GiveRedBag(role, param_1, param_2);
	}
	break;

	case HUNYAN_OPERA_TYPE_FOLWER:
	{
		this->GiveFolwer(role, param_1, param_2);
	}
	break;

	case HUNYAN_OPERA_TYPE_USE_YANHUA:
	{
		this->OnUseYanHua(role, param_1, 1 == param_2);
	}
	break;

	case HUNYAN_OPERA_TYPE_BAITANG_REQ:
	{
		this->OnBaiTangReq(role);
	}
	break;

	case HUNUAN_OPERA_TYPE_BAITANG_RET:
	{
		this->OnBaiTangRet(role, param_1);
	}
	break;

	case HUNYAN_OPERA_TYPE_APPLY:
	{
		this->ApplyJoinWedding(role);
	}
	break;

	case HUNYAN_GET_BLESS_RECORD_INFO:
	{
		this->SendBlessRecordInfo(role);
	}
	break;

	case HUNYAN_GET_APPLICANT_INFO:
	{
		this->SendWeddingApplicantInfo(role);
	}
	break;

	case HUNYAN_GET_WEDDING_INFO:
	{
		this->SendCurWeddingAllInfo(role);
	}
	break;

	case HUNYAN_GET_WEDDING_ROLE_INFO:
	{
		this->SendWeddingRoleInfo(role);
	}
	break;

	case HUNYAN_OPERA_APPLICANT_OPERA:
	{
		this->AddApplicantToGuestList(role, param_1, param_2);
	}
	break;

	default:
		break;
	}

	if (oper_succ)
	{
		if (HUNYAN_OPERA_TYPE_HUAYU == opera_type || HUNYAN_OPERA_TYPE_YANHUA == opera_type || HUNYAN_OPERA_TYPE_SAXIANHUA == opera_type)
		{
			static Protocol::SCMarryHunyanOpera mho;
			mho.obj_id = role->GetId();
			mho.opera_type = opera_type;
			mho.opera_param = param_1;
			m_scene->SendToRole((const char*)&mho, sizeof(mho));
		}
	}
}

void SpecialHunyan::GiveRedBag(Role *role, int target_uid, int seq)
{
	if (NULL == role || target_uid <= 0 || seq < 0 || seq >= WEDDING_MAX_LIVENESS_TYPE || m_cur_wedding_seq < 0 || m_cur_wedding_seq >= WEDDING_YUYUE_TIME_MAX)
	{
		return;
	}

	WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[m_cur_wedding_seq];

	if (target_uid != yuyue_info.role_id && target_uid != yuyue_info.lover_role_id)
	{
		return;
	}

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(target_uid));
	if (NULL == target_role)
	{
		role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	const WeddingBlessingCfg::BlessingCfg *blessing_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingBlessingCfgBySeq(HUNYAN_ZHUFU_TYPE_RED_BAG, seq);
	if (NULL == blessing_cfg)
	{
		return;
	}

	// 发红包不算消费
	bool is_consume_sys = false;
	if (!role->GetKnapsack()->GetMoney()->UseGold(blessing_cfg->param1, "WeddingGiveRedBag", is_consume_sys))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_wedding_liveness += blessing_cfg->add_liveness;
	m_wedding_liveness_is_change = true;

	target_role->GetKnapsack()->GetMoney()->AddGold(blessing_cfg->param1, "WeddingGiveRedBag");

	gstr::SendNotice(role, "give_succ");

	{
		// 传闻
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "wedding_give_redbag_notice", role->GetUID(), role->GetName(), (int)role->GetCamp(), target_role->GetUID(), target_role->GetName(), (int)target_role->GetCamp(), blessing_cfg->param1);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	int index = m_bless_record_info.record_cur_index % WEDDING_BLESSSING_MAX_RECORD_NUM;

	m_bless_record_info.bless_record_list[index].timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_bless_record_info.bless_record_list[index].bless_type = HUNYAN_ZHUFU_TYPE_RED_BAG;
	role->GetName(m_bless_record_info.bless_record_list[index].role_name);
	target_role->GetName(m_bless_record_info.bless_record_list[index].to_role_name);
	m_bless_record_info.bless_record_list[index].param = blessing_cfg->param1;
	m_bless_record_info.record_cur_index = index + 1;

	if (m_bless_record_info.record_count < WEDDING_BLESSSING_MAX_RECORD_NUM)
	{
		++m_bless_record_info.record_count;
	}

	this->SendBlessRecordInfo(role);

	static Protocol::SCQingYuanAllInfo qyai;

	qyai.info_type = Protocol::QINGYUAN_INFO_TYPE_GET_BLESSING;
	qyai.param_ch1 = HUNYAN_ZHUFU_TYPE_RED_BAG;
	qyai.param2 = blessing_cfg->param1;

	EngineAdapter::Instance().NetSend(target_role->GetNetId(), (const char *)&qyai, sizeof(qyai));

	this->SendWeddingLivenessToAll();
}

void SpecialHunyan::GiveFolwer(Role *role, int target_uid, int seq)
{
	if (NULL == role || target_uid <= 0 || seq < 0 || seq >= WEDDING_BLESSING_MAX_CFG_NUM || m_cur_wedding_seq < 0 || m_cur_wedding_seq >= WEDDING_YUYUE_TIME_MAX)
	{
		return;
	}

	WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[m_cur_wedding_seq];
	if (target_uid != yuyue_info.role_id && target_uid != yuyue_info.lover_role_id)
	{
		return;
	}

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(target_uid));
	if (NULL == target_role)
	{
		role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	//if (!role->GetFriendManager()->IsFriend(IntToUid(target_uid)))
	//{
	//	role->NoticeNum(errornum::EN_FRIEND_BLESS_FRIEND_LIMIT);
	//	return;
	//}

	const WeddingBlessingCfg::BlessingCfg *blessing_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingBlessingCfgBySeq(HUNYAN_ZHUFU_TYPE_GIVE_FLOWER, seq);
	if (NULL == blessing_cfg)
	{
		return;
	}

	if (role->GetKnapsack()->GetItemNum(blessing_cfg->param1) <= 0)
	{
		// 物品不足
		role->NoticeNum(errornum::EN_ITEM_NOT_ENOUGH);
		return;
	}

	if (!role->GetKnapsack()->ConsumeItem(blessing_cfg->param1, 1, "WeddingGiveFolwer"))
	{
		role->NoticeNum(errornum::EN_ITEM_NOT_ENOUGH);
		return;
	}

	m_wedding_liveness += blessing_cfg->add_liveness;
	m_wedding_liveness_is_change = true;

	if (role->GetFriendManager()->IsFriend(IntToUid(target_uid)))
	{
		role->GetFriendManager()->IntimacyIncreaseHelper(IntToUid(target_uid), blessing_cfg->param2, false, "GiveFlower", false);
	}

	if (target_role->GetFriendManager()->IsFriend(role->GetUserId()))
	{
		target_role->GetFriendManager()->IntimacyIncreaseHelper(role->GetUserId(), blessing_cfg->param2, false, "BeGiveFlower", false);
	}

	gstr::SendNotice(role, "give_succ");

	{
		// 传闻
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "wedding_give_flower_notice", role->GetUID(), role->GetName(), (int)role->GetCamp(), target_role->GetUID(), target_role->GetName(), (int)target_role->GetCamp(), blessing_cfg->param1);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL);
		}
	}

	int index = m_bless_record_info.record_cur_index % WEDDING_BLESSSING_MAX_RECORD_NUM;

	m_bless_record_info.bless_record_list[index].timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_bless_record_info.bless_record_list[index].bless_type = HUNYAN_ZHUFU_TYPE_GIVE_FLOWER;
	role->GetName(m_bless_record_info.bless_record_list[index].role_name);
	target_role->GetName(m_bless_record_info.bless_record_list[index].to_role_name);
	m_bless_record_info.bless_record_list[index].param = blessing_cfg->param1;
	m_bless_record_info.record_cur_index = index + 1;

	if (m_bless_record_info.record_count < WEDDING_BLESSSING_MAX_RECORD_NUM)
	{
		++m_bless_record_info.record_count;
	}

	this->SendBlessRecordInfo(role);

	static Protocol::SCQingYuanAllInfo qyai;

	qyai.info_type = Protocol::QINGYUAN_INFO_TYPE_GET_BLESSING;
	qyai.param_ch1 = HUNYAN_ZHUFU_TYPE_GIVE_FLOWER;
	qyai.param2 = blessing_cfg->param1;

	EngineAdapter::Instance().NetSend(target_role->GetNetId(), (const char *)&qyai, sizeof(qyai));

	this->SendWeddingLivenessToAll();
}

bool SpecialHunyan::OnUseYanHua(Role *role, int seq, bool is_auto_buy)
{
	if (NULL == role || seq < 0 || seq >= WEDDING_BLESSING_MAX_CFG_NUM)
	{
		return false;
	}

	const WeddingBlessingCfg::BlessingCfg *blessing_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingBlessingCfgBySeq(HUNYAN_ZHUFU_TYPE_USE_YANHUA, seq);
	if (NULL == blessing_cfg)
	{
		return false;
	}

	ItemExtern::ItemConsumeConfigList stuff_list;

	stuff_list.add_item(ItemExtern::ItemConsumeConfig(blessing_cfg->param1, is_auto_buy, 1));

	if (!stuff_list.done(*role, "SpecialHunyan::OnUseYanHua"))
	{
		return false;
	}

	m_wedding_liveness += blessing_cfg->add_liveness;
	m_wedding_liveness_is_change = true;

	int index = m_bless_record_info.record_cur_index % WEDDING_BLESSSING_MAX_RECORD_NUM;

	m_bless_record_info.bless_record_list[index].timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_bless_record_info.bless_record_list[index].bless_type = HUNYAN_ZHUFU_TYPE_USE_YANHUA;
	m_bless_record_info.bless_record_list[index].param = blessing_cfg->add_liveness;
	role->GetName(m_bless_record_info.bless_record_list[index].role_name);
	m_bless_record_info.record_cur_index = index + 1;

	if (m_bless_record_info.record_count < WEDDING_BLESSSING_MAX_RECORD_NUM)
	{
		++m_bless_record_info.record_count;
	}

	{
		// 传闻
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "wedding_use_yanhua_notice", role->GetUID(), role->GetName(), (int)role->GetCamp(), blessing_cfg->param1, blessing_cfg->add_liveness);
		if (length > 0)
		{
			m_scene->NotifySystemMsg(gstr::GSTR_BUF, length, SYS_MSG_CENTER_NOTICE);
		}
	}

	this->SendBlessRecordInfo(role);

	static Protocol::SCQingYuanAllInfo qyai;

	qyai.info_type = Protocol::QINGYUAN_INFO_TYPE_GET_BLESSING;
	qyai.param_ch1 = HUNYAN_ZHUFU_TYPE_USE_YANHUA;
	qyai.param2 = static_cast<short>(blessing_cfg->param1);

	int role_num = m_scene->RoleNum();
	for (int i = 0; i < role_num; ++i)
	{
		Role *scene_role = m_scene->GetRoleByIndex(i);
		if (NULL != scene_role)
		{
			EngineAdapter::Instance().NetSend(scene_role->GetNetId(), (const char *)&qyai, sizeof(qyai));
		}
	}

	role->GetKnapsack()->PutList(blessing_cfg->item_count, blessing_cfg->reward_item_list, PUT_REASON_HUNYAN_USE_YANHUA);

	this->SendWeddingLivenessToAll();

	return true;
}

void SpecialHunyan::OnBaiTangReq(Role *role)
{
	if (NULL == role || m_cur_wedding_seq < 0 || m_cur_wedding_seq >= WEDDING_YUYUE_TIME_MAX || m_bai_tang_status != HUANYAN_BAITANG_STATUS_INVALID)
	{
		return;
	}

	WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[m_cur_wedding_seq];
	const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();
	if (role->GetUID() != yuyue_info.role_id && role->GetUID() != yuyue_info.lover_role_id)
	{
		return;
	}

	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role->GetCommonDataParam()->lover_uid));
	if (NULL == lover_role)
	{
		role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	static Protocol::SCQingYuanAllInfo qyai;

	if (lover_role->GetScene()->GetSceneID() != hunyan_cfg.hunyan_scene_id)
	{
		qyai.info_type = Protocol::QINGYUAN_INFO_TYPE_BAITANG_RET;
		qyai.param_ch1 = 1;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&qyai, sizeof(qyai));
	}
	else
	{
		qyai.info_type = Protocol::QINGYUAN_INFO_TYPE_BAITANG_RET;
		qyai.param_ch1 = 0;

		EngineAdapter::Instance().NetSend(lover_role->GetNetId(), (const char *)&qyai, sizeof(qyai));
	}
}

void SpecialHunyan::OnBaiTangRet(Role *role, int is_accept)
{
	if (NULL == role || m_cur_wedding_seq < 0 || m_cur_wedding_seq >= WEDDING_YUYUE_TIME_MAX || m_bai_tang_status != HUANYAN_BAITANG_STATUS_INVALID)
	{
		return;
	}

	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role->GetLoverUid()));
	if (NULL == lover_role)
	{
		role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	if (1 != is_accept)
	{
		gstr::SendError(lover_role, "en_wedding_baitang_disagree");
		return;
	}

	WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[m_cur_wedding_seq];
	if (role->GetUID() != yuyue_info.role_id
		&& role->GetUID() != yuyue_info.lover_role_id)
	{
		return;
	}

	m_bai_tang_status = HUANYAN_BAITANG_STATUS_START;
	m_baitang_end_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + 6;

	int role_num = m_scene->RoleNum();
	for (int i = 0; i < role_num; ++i)
	{
		Role *scene_role = m_scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			this->SendWeddingRoleInfo(scene_role);
		}
	}

	{
		// 场景内传闻
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "wedding_baitang_content");
		if (length > 0)
		{
			static Protocol::SCSystemMsg sm;

			int sendlen = 0;
			if (SceneManager::CreateSystemMsg(&sm, &sendlen, gstr::GSTR_BUF, length, SYS_MSG_CENTER_NOTICE))
			{
				m_scene->SendToRole((const char*)&sm, sendlen);
			}
		}
	}
}

void SpecialHunyan::SendWeddingLivenessToAll()
{
	int role_num = m_scene->RoleNum();
	for (int i = 0; i < role_num; ++i)
	{
		Role *scene_role = m_scene->GetRoleByIndex(i);
		if (NULL != scene_role)
		{
			this->SendWeddingRoleInfo(scene_role);
		}
	}
}

void SpecialHunyan::HunyanStopResetData()
{
	m_next_add_exp_time = 0;
	m_wedding_liveness = 0;
	m_wedding_liveness_reward_flag = 0;
	m_gathar_red_bag_num_map.clear();
	memset(m_applicant_list, 0, sizeof(m_applicant_list));
}

void SpecialHunyan::SendBlessRecordInfo(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	static Protocol::SCWeddingBlessingRecordInfo wbri;

	wbri.count = m_bless_record_info.record_count;
	if (wbri.count < 0 || wbri.count > WEDDING_BLESSSING_MAX_RECORD_NUM)
	{
		wbri.count = WEDDING_BLESSSING_MAX_RECORD_NUM;
	}

	memcpy(wbri.record_info_list, m_bless_record_info.bless_record_list, sizeof(wbri.record_info_list));

	int length = sizeof(wbri) - sizeof(wbri.record_info_list[0]) * (WEDDING_BLESSSING_MAX_RECORD_NUM - wbri.count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&wbri, length);
}

void SpecialHunyan::ApplyJoinWedding(Role *role)
{
	if (NULL == role || m_cur_wedding_seq < 0 || m_cur_wedding_seq >= WEDDING_YUYUE_TIME_MAX)
	{
		return;
	}

	WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[m_cur_wedding_seq];

	if (role->GetUID() == yuyue_info.role_id || role->GetUID() == yuyue_info.lover_role_id)
	{
		return;
	}

	if (WorldStatus2::Instance().IsInWeddingGuests(yuyue_info.role_id, role->GetUID()))
	{
		gstr::SendError(role, "en_guests_had_invited");
		return;
	}

	bool is_in_applicant_list = this->IsInApplicantList(role->GetUID());

	if (!is_in_applicant_list)
	{
		for (int i = 0; i < WEDDING_APPLICANT_MAX_NUM; ++i)
		{
			if (0 == m_applicant_list[i].user_id)
			{
				m_applicant_list[i].user_id = role->GetUID();
				role->GetName(m_applicant_list[i].user_name);
				break;
			}
		}
	}

	gstr::SendNotice(role, "apply_succ");

	static Protocol::SCQingYuanAllInfo qyai;

	qyai.info_type = Protocol::QINGYUAN_INFO_TYPE_HAVE_APPLICANT;
	qyai.param2 = role->GetUID();
	role->GetName(qyai.role_name);

	Role *owner_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(yuyue_info.role_id));
	if (NULL != owner_role && !is_in_applicant_list)
	{
		EngineAdapter::Instance().NetSend(owner_role->GetNetId(), (const char *)&qyai, sizeof(qyai));
		this->SendWeddingApplicantInfo(owner_role);
	}

	Role *owner_lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(yuyue_info.lover_role_id));
	if (NULL != owner_lover_role && !is_in_applicant_list)
	{
		EngineAdapter::Instance().NetSend(owner_lover_role->GetNetId(), (const char *)&qyai, sizeof(qyai));
		this->SendWeddingApplicantInfo(owner_lover_role);
	}
}

bool SpecialHunyan::IsInApplicantList(int target_uid)
{
	for (int i = 0; i < WEDDING_GUESTS_MAX_NUM; ++i)
	{
		if (target_uid == m_applicant_list[i].user_id)
		{
			return true;
		}
	}

	return false;
}

bool SpecialHunyan::AddApplicantToGuestList(Role *role, int target_uid, int is_agree)
{
	if (NULL == role || target_uid <= 0 || m_cur_wedding_seq < 0 || m_cur_wedding_seq >= WEDDING_YUYUE_TIME_MAX)
	{
		return false;
	}

	WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[m_cur_wedding_seq];

	if (role->GetUID() != yuyue_info.role_id && role->GetUID() != yuyue_info.lover_role_id)
	{
		return false;
	}

	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role->GetLoverUid()));

	if (1 == is_agree)
	{
		int has_invite_guest_num = WorldStatus2::Instance().GetWeddingHasInviteGuestNum(yuyue_info.role_id);
		if (has_invite_guest_num >= yuyue_info.can_invite_guest_num)
		{
			gstr::SendError(role, "en_guests_has_full");
			return false;
		}

		if (WorldStatus2::Instance().IsInWeddingGuests(role->GetUID(), target_uid))
		{
			role->GetQingyuan()->SendWeddingYuYueInfo();
			if (NULL != lover_role)
			{
				lover_role->GetQingyuan()->SendWeddingYuYueInfo();
			}
		}
		else
		{
			if (!WorldStatus2::Instance().AddWeddingGeust(role->GetUID(), target_uid))
			{
				return false;
			}
			else
			{
				role->GetQingyuan()->SendWeddingYuYueInfo();
				if (NULL != lover_role)
				{
					lover_role->GetQingyuan()->SendWeddingYuYueInfo();
				}
			}

			const WeddingYuYueTimeCfg::ConfigItem *cfg_item = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingYuYueTimeBySeq(m_cur_wedding_seq);
			if (NULL != cfg_item)
			{
				int lover_camp = 0;
				UserCacheNode *lover_cache_node = UserCacheManager::Instance().GetUserNode(role->GetLoverUid());
				if (NULL != lover_cache_node)
				{
					lover_camp = (int)lover_cache_node->camp;
				}

				const QingyuanHunyanConfig::ConfigItem * hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanCfg(m_hunyan_type);
				if (NULL != hunyan_cfg)
				{
					// 邀请邮件
					gamecommon::SendMultiMailToRole(IntToUid(target_uid), hunyan_cfg->join_hunyan_item_list, hunyan_cfg->join_hunyan_item_count, NULL, false, "invalite_join_hunyan_mail", yuyue_info.wedding_index,
						role->GetUID(), role->GetName(), (int)role->GetCamp(), role->GetLoverUid(), role->GetCommonDataParam()->lover_name, lover_camp,
						cfg_item->begin_time / 100, cfg_item->begin_time % 100, cfg_item->end_time / 100, cfg_item->end_time % 100);

					std::string reward_item_list_str;
					ItemConfigDataListLog(reward_item_list_str, hunyan_cfg->join_hunyan_item_count, hunyan_cfg->join_hunyan_item_list);
					gamelog::g_log_qingyuan.printf(LL_INFO, "SpecialHunyan::AddApplicantToGuestList::user[%d], hunyan_type[%d], join_hunyan_item_list[%s]",
						target_uid, m_hunyan_type, reward_item_list_str.c_str());
				}
			}
		}
	}

	for (int i = 0; i < WEDDING_GUESTS_MAX_NUM; ++i)
	{
		if (m_applicant_list[i].user_id == target_uid)
		{
			m_applicant_list[i].Reset();
			break;
		}
	}

	this->SendWeddingApplicantInfo(role);
	if (NULL != lover_role)
	{
		this->SendWeddingApplicantInfo(lover_role);
	}

	return true;
}


void SpecialHunyan::BrocastLiveness(int liveness_val)
{
	int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "wedding_liveness_notice", liveness_val);
	if (length > 0)
	{
		m_scene->NotifySystemMsg(gstr::GSTR_BUF, length, SYS_MSG_CENTER_ROLL);
	}
}

void SpecialHunyan::CreateXunyouObj()
{
	for (int i = 0; i < HUNYAN_XUNYOU_MARRYOBJ_MAX_NUM; i++)
	{
		m_xunyou_obj_id_list[i] = INVALID_OBJ_ID;
	}

	// 1: 创建一个结婚对象
	Scene *scene = this->GetXunyouScene();
	if (NULL != scene)
	{
		MarryObj *marry_obj = new MarryObj();

		const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();

		hunyan_cfg.bai_tiandi_pos.GetPos();
		marry_obj->SetPos(hunyan_cfg.bai_tiandi_pos.GetPos());
		marry_obj->Init(0);
		scene->AddObj(marry_obj);

		m_xunyou_obj_id_list[0] = marry_obj->GetId();
	}

	// 2: 客户端隐藏掉对应的玩家
}

void SpecialHunyan::CheckUpdateXunyou(unsigned long inteval, time_t now)
{
	if (HUNYAN_STATE_TYPE_XUNYOU == m_state && m_cur_xunyou_point_index < LOGIC_CONFIG->GetQingyuanCfg().GetXunyouPosCount())
	{
		Scene *scene = GetXunyouScene();
		if (NULL != scene)
		{
			Obj *obj = scene->GetObj(m_xunyou_obj_id_list[0]);
			if (NULL != obj && Obj::OBJ_TYPE_MARRY == obj->GetObjType())
			{
				// 巡游时候 需要定期移动
				MarryObj *marryobj = (MarryObj*)obj;
				if (marryobj->IsStatic())
				{
					m_xunyou_obj_stop_time += inteval;
					unsigned long point_stop_time = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouPosStopTimeS(m_cur_xunyou_point_index) * 1000;
					if (m_xunyou_obj_stop_time >= point_stop_time)
					{
						Posi deltaP = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouPos(m_cur_xunyou_point_index) - marryobj->GetPos();
						Scalar distance = 0;
						Dir dir = gamecommon::GetMoveDir(deltaP, &distance);
						marryobj->SetDir(dir, distance);

						for (int i = 1; i < HUNYAN_XUNYOU_MARRYOBJ_MAX_NUM; i++)
						{
							Obj *obj = scene->GetObj(m_xunyou_obj_id_list[i]);
							if (NULL != obj && Obj::OBJ_TYPE_MARRY == obj->GetObjType())
							{
								MarryObj *other_marryobj = (MarryObj*)obj;
								other_marryobj->OnFirstMarryobjMove();
							}
						}
					}
				}

				// 同步巡游对象坐标
				if (now >= m_next_sync_xunyou_pos_time)
				{
					m_xunyou_info.marryobjpos_x = obj->GetPos().x;
					m_xunyou_info.marryobjpos_y = obj->GetPos().y;

					m_next_sync_xunyou_pos_time = now + 2;
				}
			}
		}

		//传闻
		if (m_next_report_xunyou_time > 0 && now >= m_next_report_xunyou_time)
		{
			this->CheckReportXunyou();

			int inerval_s = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouReportInterval();
			m_next_report_xunyou_time = now + inerval_s;
		}
	}
}

void SpecialHunyan::OnCharacterMoveStop(Character *cha)
{
	if (NULL == cha) return;

	if (Obj::OBJ_TYPE_MARRY == cha->GetObjType())
	{
		MarryObj *marryobj = (MarryObj*)cha;
		if (marryobj->IsFirstMarryobj())
		{
			++m_cur_xunyou_point_index;
			m_xunyou_obj_stop_time = 0;

			Scene *scene = GetXunyouScene();
			if (NULL != scene)
			{
				for (int i = 1; i < HUNYAN_XUNYOU_MARRYOBJ_MAX_NUM; i++)
				{
					Obj *obj = scene->GetObj(m_xunyou_obj_id_list[i]);
					if (NULL != obj && Obj::OBJ_TYPE_MARRY == obj->GetObjType())
					{
						MarryObj *other_marryobj = (MarryObj*)obj;
						other_marryobj->OnFirstMarryobjMoveStop();
					}
				}
			}
		}

		if (m_cur_xunyou_point_index >= LOGIC_CONFIG->GetQingyuanCfg().GetXunyouPosCount())
		{
			this->BrocastXunyouEnd();
		}
	}
}

void SpecialHunyan::BrocastXunyouBegin()
{
	int xunyou_scene_id = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouSceneId();

	for (unsigned int i = 0; i < static_array_size(m_marryuser_list); i ++)
	{
		Role *marry_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_marryuser_list[i].marry_uid));
		if (NULL != marry_role && (marry_role->GetScene()->GetSceneID() == xunyou_scene_id))
		{
			this->BrocastSpecialParamChange(marry_role, Protocol::SPECIAL_PARAM_CHANGE_TYPE_XUNYOU, 1);
		}
	}

	int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "wedding_xunyou_begin_notice", m_marryuser_list[0].marry_uid, m_marryuser_list[0].marry_name, (int)m_marryuser_list[0].camp,
		m_marryuser_list[1].marry_uid, m_marryuser_list[1].marry_name, (int)m_marryuser_list[1].camp, xunyou_scene_id, (int)m_xunyou_info.marryobjpos_x, (int)m_xunyou_info.marryobjpos_y);
	if (length > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_ONLY_CHAT_WORLD);
	}
}

void SpecialHunyan::CheckReportXunyou()
{
	int xunyou_scene_id = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouSceneId();

	int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "wedding_xunyou_notice", m_marryuser_list[0].marry_uid, m_marryuser_list[0].marry_name, (int)m_marryuser_list[0].camp, 
		m_marryuser_list[1].marry_uid, m_marryuser_list[1].marry_name, (int)m_marryuser_list[1].camp, xunyou_scene_id, (int)m_xunyou_info.marryobjpos_x, (int)m_xunyou_info.marryobjpos_y);
	if (length > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_ONLY_CHAT_WORLD);
	}
}

void SpecialHunyan::BrocastXunyouEnd()
{
	int xunyou_scene_id = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouSceneId();

	for (unsigned int i = 0; i < static_array_size(m_marryuser_list); i++)
	{
		Role *marry_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_marryuser_list[i].marry_uid));
		if (NULL != marry_role && (marry_role->GetScene()->GetSceneID() == xunyou_scene_id) )
		{
			this->BrocastSpecialParamChange(marry_role, Protocol::SPECIAL_PARAM_CHANGE_TYPE_XUNYOU, 0);
		}
	}

	int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "wedding_xunyou_end_notice", m_marryuser_list[0].marry_uid, m_marryuser_list[0].marry_name, (int)m_marryuser_list[0].camp,
		m_marryuser_list[1].marry_uid, m_marryuser_list[1].marry_name, (int)m_marryuser_list[1].camp);
	if (length > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_ONLY_CHAT_WORLD);
	}
}

void SpecialHunyan::BrocastSpecialParamChange(Role *role, int change_type, int param)
{
	if (nullptr == role)
	{
		return;
	}

	Protocol::SCSpecialParamChange spc;
	spc.change_type = change_type;
	spc.obj_id = role->GetId();
	spc.param = param;
	role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), (void*)&spc, sizeof(spc));
}

void SpecialHunyan::OnDeleteXunyouObj()
{
	Scene *scene = this->GetXunyouScene();
	if (NULL != scene)
	{
		for (int i = 0; i < HUNYAN_XUNYOU_MARRYOBJ_MAX_NUM; i++)
		{
			Obj *obj = scene->GetObj(m_xunyou_obj_id_list[i]);
			if (NULL != obj && Obj::OBJ_TYPE_MARRY == obj->GetObjType())
			{
				scene->DeleteObj(m_xunyou_obj_id_list[i]);
				m_xunyou_obj_id_list[i] = INVALID_OBJ_ID;
			}
		}
	}
}

Scene * SpecialHunyan::GetXunyouScene()
{
	int xunyou_scene_id = LOGIC_CONFIG->GetQingyuanCfg().GetXunyouSceneId();
	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(xunyou_scene_id, 0);

	return scene;
}

const Posi SpecialHunyan::GetXunyouObjCurPos(int seq, ObjID &obj_id)
{
	if (seq < 0 || seq >= HUNYAN_XUNYOU_MARRYOBJ_MAX_NUM)
	{
		return Posi(0, 0);
	}

	Scene *scene = GetXunyouScene();
	if (NULL == scene)
	{
		return Posi(0, 0);
	}

	Obj *obj = scene->GetObj(m_xunyou_obj_id_list[seq]);
	if (NULL == obj || Obj::OBJ_TYPE_MARRY != obj->GetObjType())
	{
		return Posi(0, 0);
	}

	obj_id = m_xunyou_obj_id_list[seq];

	return obj->GetPos();
}

int SpecialHunyan::GetCurXunyouSeq()
{
	return m_cur_xunyou_point_index;
}

void SpecialHunyan::SendWeddingApplicantInfo(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[m_cur_wedding_seq];
	if (role->GetUID() != yuyue_info.role_id && role->GetUID() != yuyue_info.lover_role_id)
	{
		return;
	}

	static Protocol::SCWeddingApplicantInfo wai;

	int count = 0;
	for (int i = 0; i < WEDDING_APPLICANT_MAX_NUM; ++i)
	{
		if (0 != m_applicant_list[i].user_id)
		{
			wai.applicant_list[count].user_id = m_applicant_list[i].user_id;
			F_STRNCPY(wai.applicant_list[count].user_name, m_applicant_list[i].user_name, sizeof(GameName));
			count++;
		}
	}

	wai.count = count;
	int length = sizeof(wai) - sizeof(wai.applicant_list[0]) * (WEDDING_GUESTS_MAX_NUM - wai.count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&wai, length);
}

void SpecialHunyan::SendCurWeddingAllInfo(Role *role)
{
	if (NULL == role || m_cur_wedding_seq < 0 || m_cur_wedding_seq >= WEDDING_YUYUE_TIME_MAX)
	{
		return;
	}

	WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[m_cur_wedding_seq];

	static Protocol::SCHunYanCurWeddingAllInfo hycwai;

	hycwai.role_id = yuyue_info.role_id;
	hycwai.cur_wedding_seq = m_cur_wedding_seq;
	UserCacheNode *role_node = UserCacheManager::Instance().GetUserNode(yuyue_info.role_id);
	if (NULL != role_node)
	{
		role_node->GetName(hycwai.role_name);
		hycwai.role_prof = role_node->prof;
	}

	hycwai.lover_role_id = yuyue_info.lover_role_id;
	UserCacheNode *lover_role_node = UserCacheManager::Instance().GetUserNode(yuyue_info.lover_role_id);
	if (NULL != lover_role_node)
	{
		lover_role_node->GetName(hycwai.lover_role_name);
		hycwai.lover_role_prof = lover_role_node->prof;
	}

	hycwai.wedding_index = yuyue_info.wedding_index;

	int count = 0;

	for (int i = 0; i < WEDDING_GUESTS_MAX_NUM; i++)
	{
		if (0 != yuyue_info.guest_role_id_list[i])
		{
			hycwai.guests_uid[count] = yuyue_info.guest_role_id_list[i];

			count++;
		}
	}

	hycwai.count = count;

	int length = sizeof(hycwai) - sizeof(hycwai.guests_uid[0]) * (WEDDING_GUESTS_MAX_NUM - count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&hycwai, length);
}

void SpecialHunyan::SendWeddingRoleInfo(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	static Protocol::SCWeddingRoleInfo wri; wri.Reset();

	std::map<int, long long>::iterator it = m_wedding_role_exp_map.find(role->GetUID());
	if (it == m_wedding_role_exp_map.end())
	{
		m_wedding_role_exp_map[role->GetUID()] = 0;

		wri.total_exp = 0;
	}
	else
	{
		wri.total_exp = it->second;
	}

	std::map<int, int>::iterator it_gather = m_gathar_num_map.find(role->GetUID());
	if (it_gather == m_gathar_num_map.end())
	{
		wri.banquet_has_gather_num = 0;
	}
	else
	{
		wri.banquet_has_gather_num = it_gather->second;
	}

	std::map<int, RoleGatherRedBagNum>::iterator git = m_gathar_red_bag_num_map.find(role->GetUID());
	if (git == m_gathar_red_bag_num_map.end())
	{
		wri.cur_turn_has_gather_red_bag = 0;
	}
	else
	{
		if (m_cur_red_bag_turn >= 0 && m_cur_red_bag_turn < WEDDING_MAX_LIVENESS_TYPE)
		{
			wri.cur_turn_has_gather_red_bag = git->second.gather_num_list[m_cur_red_bag_turn];
		}
		else
		{
			wri.cur_turn_has_gather_red_bag = 0;
		}
	}

	{
		// 采过的酒席信息
		wri.hunyan_food_id_count = 0;

		std::map<int, std::set<ObjID> >::iterator iter = m_role_gather_record.find(role->GetUID());
		if (iter != m_role_gather_record.end())
		{
			std::set<ObjID>::iterator gather_obj_iter = iter->second.begin();
			for (; gather_obj_iter != iter->second.end() && wri.hunyan_food_id_count < Protocol::SCWeddingRoleInfo::MAX_GATHR_FOOD_COUNT; ++gather_obj_iter)
			{
				wri.hunyan_food_id_list[wri.hunyan_food_id_count++] = *gather_obj_iter;
			}
		}
	}

	wri.wedding_liveness = (short)m_wedding_liveness;
	wri.is_baitang = m_bai_tang_status;
	wri.is_in_red_bag_fulsh_time = m_is_in_red_bag_flush_time ? 1 : 0;

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&wri, sizeof(wri) - sizeof(wri.hunyan_food_id_list[0]) * (Protocol::SCWeddingRoleInfo::MAX_GATHR_FOOD_COUNT - wri.hunyan_food_id_count));
}

void SpecialHunyan::PackHunYanInfo(Protocol::SCHunyanStateInfo *hi, Role *role)
{
	hi->hunyan_state = m_state;
	hi->next_state_timestmp = m_next_state_time;
	hi->fb_key = m_scene->GetSceneKey();
	hi->yanhui_type = m_hunyan_type;

	UNSTD_STATIC_CHECK(sizeof(hi->marryuser_list) == sizeof(m_marryuser_list));
	memcpy(&hi->marryuser_list, &m_marryuser_list, sizeof(hi->marryuser_list));

	hi->roleuid_count = 0;
	int role_num = m_scene->RoleNum();
	for (int i = 0; i < role_num; i++)
	{
		Role *scene_role = m_scene->GetRoleByIndex(i);
		if (NULL != scene_role)
		{
			if (NULL != role && role->GetUID() == scene_role->GetUID())
			{
				continue;
			}

			hi->roleuid_list[hi->roleuid_count++] = scene_role->GetUID();
		}
	}
}

void SpecialHunyan::SendHunyanInfo(Role *role, int notify_reason, Role *leave_role)
{
	static Protocol::SCHunyanStateInfo hi;
	hi.paohuoqiu_timestmp = 0;
	hi.is_self_hunyan = 0;
	hi.notify_reason = notify_reason;
	this->PackHunYanInfo(&hi, leave_role);
	int lengh = sizeof(hi) - (MAX_ROLE_IN_YANHUI - hi.roleuid_count) * sizeof(hi.roleuid_list[0]);

	if (NULL != role)
	{
		int &gather_num = m_gathar_num_map[role->GetUID()];

		const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();
		if (YUYUE_HUNYAN_TYPE_1 == m_hunyan_type)
		{
			hi.remainder_eat_food_num = hunyan_cfg.bind_diamonds_hy_gather_max - gather_num;
		}
		else
		{
			hi.remainder_eat_food_num = hunyan_cfg.diamonds_hy_gather_max - gather_num;
		}

		if (role->GetUID() == m_marryuser_list[0].marry_uid || role->GetUID() == m_marryuser_list[1].marry_uid)
		{	
			hi.is_self_hunyan = 1;
		//	hi.paohuoqiu_timestmp = m_paohuoqiu_timestmp;
		}

		hi.total_gather_times = role->GetQingyuan()->GetMaxGetGatherDayMaxStuffs();
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&hi, lengh);
	}
	else
	{
		m_scene->SendToRole((const char*)&hi, lengh);
	}
		
	if (Protocol::HUNYAN_NOTIFY_REASON_STATE_CHANGE == notify_reason)
	{
		for (int i = 0; i < Protocol::HUNYAN_MARRY_USER_COUNT; i++)
		{
			if (0 != m_marryuser_list[i].marry_uid)
			{
				Role *marry_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_marryuser_list[i].marry_uid));
				if (NULL != marry_role)
				{
					hi.is_self_hunyan = 1;
					int lengh = sizeof(hi) - (MAX_ROLE_IN_YANHUI - hi.roleuid_count) * sizeof(hi.roleuid_list[0]);
					if (NULL != role)
					{
						hi.total_gather_times = role->GetQingyuan()->GetMaxGetGatherDayMaxStuffs();
					}

					EngineAdapter::Instance().NetSend(marry_role->GetNetId(), (const char*)&hi, lengh);
				}
			}
		}
	}
	
	if (Protocol::HUNYAN_NOTIFY_REASON_LOGIN == notify_reason && NULL != role)
	{
		if (role->GetUID() == m_marryuser_list[0].marry_uid || role->GetUID() == m_marryuser_list[1].marry_uid)
		{	
			hi.is_self_hunyan = 1;
			int lengh = sizeof(hi) - (MAX_ROLE_IN_YANHUI - hi.roleuid_count) * sizeof(hi.roleuid_list[0]);
			hi.total_gather_times = role->GetQingyuan()->GetMaxGetGatherDayMaxStuffs();

			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&hi, lengh);
		}
	}
}

bool SpecialHunyan::GiveHongbaoHelper(Role *role)
{
// 	if (NULL == role) return false;
// 
// 	const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();
// 
// 	int user_index = -1;
// 	if (this->IsMarryUser(role->GetUID(), &user_index))
// 	{
// 		if (user_index >= 0 && user_index < Protocol::HUNYAN_MARRY_USER_COUNT)
// 		{
// 			if (m_marryuser_list[user_index].hongbao_count >= hunyan_cfg.buy_max_hongbao + hunyan_cfg.free_fahongbao_num) // 达到最大的发红包数
// 			{
// 				role->NoticeNum(errornum::EN_HUNYAN_HONGBAO_BUY_TOOMUCH);
// 				return false;
// 			}
// 
// 			if (m_marryuser_list[user_index].hongbao_count >= hunyan_cfg.free_fahongbao_num) // 没有免费次数 
// 			{
// 				if (!role->GetKnapsack()->GetMoney()->UseGold(hunyan_cfg.buy_hongbao_gold, "HunyanBuyHongBao"))
// 				{
// 					role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 					return false;
// 				}
// 			}
// 
// 			++ m_marryuser_list[user_index].hongbao_count;
// 
// 			for (int i = 0; i < hunyan_cfg.give_hongbao_num; i++)
// 			{
// 				FallingItem *fallingitem = new FallingItem();
// 				fallingitem->Init(hunyan_cfg.hongbao_item.item_id, 1, true, 0, INVALID_USER_ID, 600, 0, false, false);
// 
// 				const Posi hongbao_pos = gamecommon::GetDisRandPos(role->GetPos(), hunyan_cfg.random_hongbao_dis);
// 				if (m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, hongbao_pos.x, hongbao_pos.y))
// 				{
// 					fallingitem->SetPos(hongbao_pos);
// 				}
// 				else
// 				{
// 					fallingitem->SetPos(role->GetPos());
// 				}
// 
// 				m_scene->AddObj(fallingitem);
// 			}
// 
// 			{
// 				// 红包传闻
// 				int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_hunyan_hongbao_notice, 
// 					role->GetUID(), role->GetName(), (int)role->GetCamp());
// 				if (length > 0) 
// 				{
// 					static Protocol::SCSystemMsg sm;
// 
// 					int sendlen = 0;
// 					if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE))
// 					{
// 						m_scene->SendToRole((const char*)&sm, sendlen);
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	role->NoticeNum(noticenum::NT_HUNYAN_FA_HONGBAO_SUCC);
// 
// 	this->SendHunyanInfo(role, Protocol::HUNYAN_NOTIFY_REASON_PAOHUAQIU);
// 
 	return true;
}

bool SpecialHunyan::UseYanHua(Role *role, int item_id, int param1, int param2)
{
	if (NULL == role || m_cur_wedding_seq < 0 || m_cur_wedding_seq >= WEDDING_YUYUE_TIME_MAX || param1 <= 0)
	{
		return false;
	}

	WeddingYuYueInfo &yuyue_info = WorldStatus2::Instance().GetWeddingParam().yuyue_info[m_cur_wedding_seq];

	if (!WorldStatus2::Instance().IsInWeddingGuests(yuyue_info.role_id, role->GetUID())
		&& role->GetUID() != yuyue_info.role_id && role->GetUID() != yuyue_info.lover_role_id)
	{
		return false;
	}

	m_wedding_liveness += param1;
	m_wedding_liveness_is_change = true;

	int index = m_bless_record_info.record_cur_index % WEDDING_BLESSSING_MAX_RECORD_NUM;

	m_bless_record_info.bless_record_list[index].timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_bless_record_info.bless_record_list[index].bless_type = HUNYAN_ZHUFU_TYPE_USE_YANHUA;
	m_bless_record_info.bless_record_list[index].param = param1;
	role->GetName(m_bless_record_info.bless_record_list[index].role_name);
	m_bless_record_info.record_cur_index = index + 1;

	if (m_bless_record_info.record_count < WEDDING_BLESSSING_MAX_RECORD_NUM)
	{
		++m_bless_record_info.record_count;
	}

	{
		// 传闻
		int length = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), "wedding_use_yanhua_notice", role->GetUID(), role->GetName(), (int)role->GetCamp(), item_id, param1);
		if (length > 0)
		{
//			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, length, SYS_MSG_CENTER_NOTICE);
			m_scene->NotifySystemMsg(gstr::GSTR_BUF, length, SYS_MSG_CENTER_NOTICE);
		}
	}

	this->SendBlessRecordInfo(role);

	static Protocol::SCQingYuanAllInfo qyai;

	qyai.info_type = Protocol::QINGYUAN_INFO_TYPE_GET_BLESSING;
	qyai.param_ch1 = HUNYAN_ZHUFU_TYPE_USE_YANHUA;
	qyai.param2 = item_id;

	int role_num = m_scene->RoleNum();
	for (int i = 0; i < role_num; ++i)
	{
		Role *scene_role = m_scene->GetRoleByIndex(i);
		if (NULL != scene_role)
		{
			EngineAdapter::Instance().NetSend(scene_role->GetNetId(), (const char *)&qyai, sizeof(qyai));
		}
	}

//	role->AddExp(param2, "SpecialHunyan::UseYanHua");
	this->SendWeddingLivenessToAll();
	return true;
}

int SpecialHunyan::GetCurWeddingSeq()
{
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	unsigned int zero_time = static_cast<unsigned int>(GetZeroTime(now));

	const WeddingYuYueTimeCfg &time_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetWeddingYuYueTime();

	for (int i = WEDDING_YUYUE_TIME_MAX - 1; i >= 0; --i)
	{
		if (now >= zero_time + (time_cfg.cfg_list[i].apply_time / 100 * 3600 + (time_cfg.cfg_list[i].apply_time % 100) * 60))
		{
			return time_cfg.cfg_list[i].seq;
		}
	}

	return -1;
}

bool SpecialHunyan::HuaYuHelper(Role *role)
{
	return true;
}

bool SpecialHunyan::SaXianHuaHelper(Role *role)
{
// 	if (NULL == role) return false;
// 
// 	if (this->IsMarryUser(role->GetUID(), NULL))
// 	{
// 		return false;
// 	}
// 
// 	std::map<int, int>::iterator it = m_saxianhua_num_map.find(role->GetUID());
// 	if (it == m_saxianhua_num_map.end())
// 	{
// 		m_saxianhua_num_map[role->GetUID()] = 0;
// 	}
// 
// 	it = m_saxianhua_num_map.find(role->GetUID());
// 	if (it != m_saxianhua_num_map.end())
// 	{
// 		const QingyuanHunyanActiviryConfig * hunyan_act_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanHunyanActivityCfg();
// 		if (NULL == hunyan_act_cfg) return false;	
// 
// 		if (it->second >= hunyan_act_cfg->saxianhua_count)
// 		{
// 			role->NoticeNum(errornum::EN_HUNYAN_SAXIANHUA_MAX_COUNT);
// 			return false; 
// 		}
// 
// 		if (role->GetQingyuan()->GetTodaySaxianhuaTimes() >= hunyan_act_cfg->per_day_saxianhua_count)
// 		{
// 			role->NoticeNum(errornum::EN_HUNYAN_SAXIANHUA_MAX_COUNT);
// 			return false; 
// 		}
// 		
// 		++ it->second;
// 		role->GetQingyuan()->AddTodaySaxianhuaTimes();
// 
// 		Knapsack *knapsack = role->GetKnapsack();
// 		if (NULL == knapsack) return false;
// 
// 		if (role->GetQingyuan()->GetMaxGetDiamonds() >= hunyan_act_cfg->per_day_max_bind_diamonds)
// 		{
// 			role->NoticeNum(errornum::EN_HUNYAN_GET_NUM_MAX_BIND_GOLD);
// 		}
// 		else
// 		{
// 			int add_bind_diamond = 0;
// 			if (role->GetQingyuan()->GetMaxGetDiamonds() + hunyan_act_cfg->saxianhua_reward_diamonds > hunyan_act_cfg->per_day_max_bind_diamonds)
// 			{
// 				add_bind_diamond = hunyan_act_cfg->per_day_max_bind_diamonds - role->GetQingyuan()->GetMaxGetDiamonds();
// 
// 				role->GetQingyuan()->AddMaxGetDiamonds(add_bind_diamond);
// 				knapsack->GetMoney()->AddGoldBind(add_bind_diamond, "hunyan_saxianhua");
// 			}
// 			else
// 			{
// 				role->GetQingyuan()->AddMaxGetDiamonds(hunyan_act_cfg->saxianhua_reward_diamonds);
// 				knapsack->GetMoney()->AddGoldBind(hunyan_act_cfg->saxianhua_reward_diamonds, "hunyan_saxianhua");
// 			}
// 		}
// 
// 		//role->NoticeNum(noticenum::NT_HUNYAN_SAHUA_SUCC);
// 
// 		bool is_succ = RandomNum(100) <= hunyan_act_cfg->saxianhua_get_hunjie_stuff_per;
// 		if (is_succ)
// 		{
// 			if (!knapsack->HaveSpace())
// 			{
// 				role->NoticeNum(errornum::EN_HAS_NO_SPACING);
// 				return true;
// 			}
// 
// 			if (role->GetQingyuan()->GetMaxGetHunjieStuffs() >= hunyan_act_cfg->per_day_max_stuff_num)
// 			{
// 				role->NoticeNum(errornum::EN_HUNYAN_GATHER_XIANHUAN_LIMIT);
// 			}
// 			else
// 			{
// 				ItemConfigData stuff;
// 				stuff.is_bind = true;
// 				stuff.item_id = hunyan_act_cfg->uplevel_stuff_id;
// 				stuff.num = hunyan_act_cfg->saxainhua_get_stuff_num;
// 
// 				if (role->GetQingyuan()->GetMaxGetHunjieStuffs() + hunyan_act_cfg->saxainhua_get_stuff_num > hunyan_act_cfg->per_day_max_stuff_num)
// 				{
// 					stuff.num = hunyan_act_cfg->per_day_max_stuff_num - role->GetQingyuan()->GetMaxGetHunjieStuffs();
// 					role->GetQingyuan()->AddMaxGetHunjieStuffs(stuff.num);
// 					knapsack->Put(stuff, PUT_REASON_HUNYAN_ZHUFU);
// 				}
// 				else
// 				{
// 					role->GetQingyuan()->AddMaxGetHunjieStuffs(stuff.num);
// 					knapsack->Put(stuff, PUT_REASON_HUNYAN_ZHUFU);
// 				}
// 			}
// 
// 			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_hunyan_sahua, role->GetUID(), role->GetName(), (int)role->GetCamp());
// 			if (length > 0)
// 			{
// 				this->NotifySystemMsg(gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE);
// 			}
// 		}
// 	}

	return true;
}

bool SpecialHunyan::PaoHuaQiuHelper(Role *role)
{
// 	if (NULL == role) return false;
// 
// 	const QingyuanHunyanActiviryConfig *hunyan_act_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanHunyanActivityCfg();
// 	if (NULL == hunyan_act_cfg) return false;
// 
// 	if (m_hunyan_type != YUYUE_HUNYAN_TYPE_2 && 0 == hunyan_act_cfg->bind_diamonds_hy_can_phq)		//绑钻婚宴
// 	{
// 		return false;
// 	}
// 
// 	if (m_paohuoqiu_timestmp != 0 && static_cast<unsigned int>(EngineAdapter::Instance().Time()) < (m_paohuoqiu_timestmp + hunyan_act_cfg->paohuaqiu_cd_s))
// 	{
// 		role->NoticeNum(noticenum::NT_HUNYAN_PAO_HUAQIU_LEN_QIU);
// 		return false;
// 	}
// 
// 	if (m_paohuoqiu_times >= hunyan_act_cfg->paohuaqiu_times)
// 	{
// 		role->NoticeNum(noticenum::NT_HUNYAN_PAO_HUAQIU_FAIL);
// 		return false;
// 	}
// 	
// 	m_paohuoqiu_times ++;
// 	m_paohuoqiu_timestmp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
// 
// 	if (this->IsMarryUser(role->GetUID(), NULL))
// 	{
// 		int count = 0;
// 		const std::vector<Posi> &phq_pos_vec = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanPhqRandomVec();
// 		for (std::vector<Posi>::const_iterator it = phq_pos_vec.begin();
// 			it != phq_pos_vec.end() && count < hunyan_act_cfg->paohuaqiu_num; ++it)
// 		{
// 			if (!m_scene->GetMap()->IsValidatePos(Obj::OBJ_TYPE_ROLE, it->x, it->y))	// 不能抛在角色不可到达处
// 			{
// 				continue;
// 			}
// 
// 			GatherObj *gather_obj = new GatherObj();
// 			gather_obj->Init(NULL, hunyan_act_cfg->paohuaqui_gather_id, 1500, 0, true);
// 			gather_obj->SetPos(*it);
// 			m_scene->AddObj(gather_obj);
// 
// 			count++;
// 		}
// 	}
// 
// 	role->NoticeNum(noticenum::NT_HUNYAN_PAO_HUAQIU_SUCC);
// 
// 	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role->GetLoverUid()));
// 	if (NULL != lover_role)
// 	{
// 		this->SendHunyanInfo(lover_role, Protocol::HUNYAN_NOTIFY_REASON_PAOHUAQIU);
// 	}
// 
// 	this->SendHunyanInfo(role, Protocol::HUNYAN_NOTIFY_REASON_PAOHUAQIU);

// 	if (0 != hunyan_act_cfg->pao_hq_reward_item.item_id)
// 	{
// 		Knapsack *knapsack = role->GetKnapsack();
// 		if (knapsack->IsLock())
// 		{
// 			role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
// 			return true;
// 		}
// 
// 		if (!knapsack->HaveSpace())
// 		{
// 			role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
// 			return true;
// 		}
// 
// 		ItemConfigData stuff;
// 		stuff.item_id = hunyan_act_cfg->pao_hq_reward_item.item_id;
// 		stuff.is_bind = hunyan_act_cfg->pao_hq_reward_item.is_bind;
// 		stuff.num = hunyan_act_cfg->pao_hq_reward_item.num;
// 
// 		role->GetQingyuan()->AddMaxGetRoses(stuff.num);
// 		role->GetKnapsack()->Put(stuff, PUT_REASON_HUNYAN_GATHER);
// 	}

	return true;
}

void SpecialHunyan::SendRoleYanhuaInfo(Role *role)
{
// 	if (NULL == role) return;
// 
// 	int yanhua_count = 0;
// 
// 	std::map<int, int>::iterator it = m_yanhua_num_map.find(role->GetUID());
// 	if (it != m_yanhua_num_map.end())
// 	{
// 		yanhua_count = it->second;
// 	}
// 
// 	Protocol::SCHunyanGuestInfo hgi;
// 	hgi.yanhua_count = yanhua_count;
// 	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&hgi, sizeof(hgi));
}

bool SpecialHunyan::IsMarryUser(int uid, int *user_index)
{
	for (int i = 0; i < Protocol::HUNYAN_MARRY_USER_COUNT; i++)
	{
		if (m_marryuser_list[i].marry_uid == uid)
		{
			if (NULL != user_index) *user_index = i;

			return true;
		}
	}

	return false;
}

void SpecialHunyan::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
{
	if (NULL != str_buff && str_len > 0)
	{
		int sendlen = 0;
		static Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, notice_type))
		{
			m_scene->SendToRole((const char*)&sm, sendlen);
		}
	}
}