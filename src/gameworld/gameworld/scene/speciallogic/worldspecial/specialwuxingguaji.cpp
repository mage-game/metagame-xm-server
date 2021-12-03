#include "specialwuxingguaji.hpp"

#include "obj/character/role.h"
#include "obj/character/monster.h"

#include "scene/activityshadow/activityshadow.hpp"
#include "item/itempool.h"
#include "protocal/msgactivity.hpp"
#include "protocal/msgfight.h"

#include "engineadapter.h"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "servercommon/string/gameworldstr.h"
#include "servercommon/userprotocal/msgsystem.h"

#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/wuxingguajiconfig.hpp"
#include "globalconfig/globalconfig.h"

#include "monster/monsterpool.h"
#include "scene/worldshadow/worldshadow.hpp"
#include "scene/scenemanager.h"
#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include <algorithm> 

#include "global/activity/activitymanager.hpp"

#include "obj/otherobj/fallingitem.h"
#include "item/knapsack.h"

SpecialWuXingGuaJi::SpecialWuXingGuaJi(Scene *scene) 
	: SpecialLogic(scene), m_isopen(false)
{
	
}

SpecialWuXingGuaJi::~SpecialWuXingGuaJi()
{

}

void SpecialWuXingGuaJi::Update(unsigned long interval, time_t now_second)
{
	SpecialLogic::Update(interval, now_second);

	if (m_isopen)
	{
		bool refresh_boss = false;

		for (int i = 0; i < WUXINGGUAJI_BOSS_NUM; i++)
		{
			if (0 != m_boss_info_list[i].next_boss_flush_time && now_second >= m_boss_info_list[i].next_boss_flush_time && 
				INVALID_OBJ_ID == m_boss_info_list[i].boss_objid)
			{
				int boss_id = 0; Posi boss_pos;
				if (LOGIC_CONFIG->GetWuXingGuaJiConfig().GetBossCfg(i, &boss_id, &boss_pos))
				{
					Monster *monster = MONSTERPOOL->CreateMonster(boss_id, m_scene, boss_pos);
					if (NULL != monster)
					{
						refresh_boss = true;

						m_boss_info_list[i].monster_id = boss_id;
						m_boss_info_list[i].boss_objid = monster->GetId();
						m_boss_info_list[i].next_boss_flush_time = 0;

						int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wuxingguaji_boss_flush_notice, monster->GetName());
						if (str_len > 0)
						{
							this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
						}
					}
				}
			}
		}

		if (refresh_boss)
		{
			this->SendUserInfo(NULL, Protocol::SCWuXingGuaJiInfo::WUXINGGUAJI_REASON_BOSS_REFRESH);
		}

		if (m_wxgjli.next_lucky_timestamp > 0 && now_second >= m_wxgjli.next_lucky_timestamp)
		{
			m_wxgjli.next_lucky_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetWuXingGuaJiConfig().GetLuckyInterval();

			this->GaveLuckyItemToUser();
		}
	}
}

void SpecialWuXingGuaJi::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
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

void SpecialWuXingGuaJi::OnCharacterDie(Character *dead_cha, ObjID killer_objid)
{
	if (Obj::OBJ_TYPE_MONSTER == dead_cha->GetObjType())
	{
		Monster *dead_monster = (Monster*)dead_cha;

		Role *kill_role = m_scene->GetPrincipal(killer_objid);
		if (NULL != kill_role)
		{
			if (dead_monster->IsBossMonster())
			{
				int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wuxingguaji_kill_boss_notice, 
					UidToInt(kill_role->GetUserId()), kill_role->GetName(), (int)kill_role->GetCamp(), dead_monster->GetName());
				if (str_len > 0)
				{
					this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_ACTIVE_NOTICE);
				}
			}
		}

		for (int i = 0; i < WUXINGGUAJI_BOSS_NUM; i++)
		{
			if (m_boss_info_list[i].boss_objid == dead_monster->GetId())
			{
				m_boss_info_list[i].boss_objid = INVALID_OBJ_ID;
				m_boss_info_list[i].next_boss_flush_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + WUXINGGUAJI_FLUSH_BOSS_INTERVAL;

				for (int i = 0; i < (int)m_scene->RoleNum(); ++i)
				{
					Role *role = m_scene->GetRoleByIndex(i);
					if (NULL != role)
					{
						this->SendUserInfo(role, Protocol::SCWuXingGuaJiInfo::WUXINGGUAJI_REASON_BOSS_DEAD);
					}
				}

				break;
			}
		}
	}
}

void SpecialWuXingGuaJi::OnRoleEnterScene(Role *role)
{
	WuXingGuaJiUser *wuxingguaji_user = this->GetWuXingGuaJiUser(role->GetUserId());
	if (NULL != wuxingguaji_user)
	{
		wuxingguaji_user->net_id = role->GetNetId();
	}

	this->SendUserInfo(role, Protocol::SCWuXingGuaJiInfo::WUXINGGUAJI_REASON_ENTER);	
	this->SendLuckyInfoToUser(role);
}

void SpecialWuXingGuaJi::OnRoleLeaveScene(Role *role, bool is_logout)
{
	WuXingGuaJiUser *wuxingguaji_user = this->GetWuXingGuaJiUser(role->GetUserId());
	if (NULL != wuxingguaji_user)
	{
		wuxingguaji_user->net_id = INVALID_GSNETID;
	}
}

void SpecialWuXingGuaJi::OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper)
{
	bool is_notify = false;

	WuXingGuaJiUser *wuxingguaji_user = this->GetWuXingGuaJiUser(role->GetUserId());
	if (NULL != wuxingguaji_user)
	{
		for (int i = 0; i < WUXINGGUAJI_TARGET_MAX; i++)
		{
			WuXingGuaJiTargetCfg *target_cfg = LOGIC_CONFIG->GetWuXingGuaJiConfig().GetWuXingGuaJiTargetCfg(i);
			WuXingGuaJiTargetInfo *my_target = &wuxingguaji_user->target_list[i];
			if (NULL != target_cfg && NULL != my_target && !my_target->is_finish)
			{
				int stuff_idx = target_cfg->GetStuffIdx(falling_item->GetItemId());
				if (stuff_idx >= 0 && stuff_idx < WUXINGGUAJI_STUFF_MAX)
				{
					is_notify = true;

					my_target->stuff_id_list[stuff_idx] = falling_item->GetItemId();
					my_target->stuff_num_list[stuff_idx] ++;
					if (LOGIC_CONFIG->GetWuXingGuaJiConfig().CheckTargetIsFinish(i, my_target))
					{
						my_target->is_finish = 1;
					}
				}
			}
		}

		if (is_notify)
		{
			this->SendUserInfo(role, Protocol::SCWuXingGuaJiInfo::WUXINGGUAJI_REASON_GET_ITEM);
		}
	}
}

void SpecialWuXingGuaJi::OnActivityStatusChange(int status)
{
	switch (status)
	{
	case ACTIVITY_STATUS_OPEN:
		{
			this->OnActivityStart();
		}
		break;

	case ACTIVITY_STATUS_CLOSE:
		{
			this->OnActivityClose();
		}
		break;
	}
}

void SpecialWuXingGuaJi::WuXingGuaJiGetReward(Role *role, Protocol::CSWuXingGuaJiGetReward *wxgjgr)
{
	if (role == nullptr || wxgjgr == nullptr || wxgjgr->target_idx < 0 || wxgjgr->target_idx >= WUXINGGUAJI_TARGET_MAX)
	{
		return;
	}

	WuXingGuaJiUser *wuxingguaji_user = this->GetWuXingGuaJiUser(role->GetUserId());
	if (NULL != wuxingguaji_user)
	{
		WuXingGuaJiTargetInfo &target_info = wuxingguaji_user->target_list[wxgjgr->target_idx];
		if (target_info.IsFinish())
		{
			WuXingGuaJiTargetCfg *target_cfg = LOGIC_CONFIG->GetWuXingGuaJiConfig().GetWuXingGuaJiTargetCfg(wxgjgr->target_idx);
			if (NULL != target_cfg)
			{
				if (role->GetKnapsack()->Put(target_cfg->reward_cfg, PUT_REASON_WUXINGGUAJI_GETREWARD))
				{
					target_info.is_getreward = 1;
					role->NoticeNum(noticenum::NT_WUXINGGUAJI_GET_REWARD_SUCC);

					this->SendUserInfo(role, Protocol::SCWuXingGuaJiInfo::WUXINGGUAJI_REASON_FETCH_REWARD);
				}
			}
		}
	}	
}

void SpecialWuXingGuaJi::OnActivityStart()
{
	m_isopen = true;
	m_wuxingguaji_user_map.clear();

	m_wxgjli.next_lucky_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + LOGIC_CONFIG->GetWuXingGuaJiConfig().GetLuckyInterval();
	m_wxgjli.luck_user_count = 0;

	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < WUXINGGUAJI_BOSS_NUM; i++)
	{
		m_boss_info_list[i].monster_id = 0;
		m_boss_info_list[i].boss_objid = INVALID_OBJ_ID;
		m_boss_info_list[i].next_boss_flush_time = now + WUXINGGUAJI_FLUSH_BOSS_INTERVAL;
	}
}

void SpecialWuXingGuaJi::OnActivityClose()
{
	m_isopen = false;

	{
		// 补发奖励
		for (WuXingGuaJiUserMapIt it = m_wuxingguaji_user_map.begin(); it != m_wuxingguaji_user_map.end(); ++it)
		{
			WuXingGuaJiUser *wuxingguaji_user = &(it->second);

			for (int i = 0; i < WUXINGGUAJI_TARGET_MAX; i++)
			{
				WuXingGuaJiTargetInfo *my_target = &wuxingguaji_user->target_list[i];
				if (NULL != my_target && my_target->IsFinish() && !my_target->IsGetReward())
				{
					WuXingGuaJiTargetCfg *target_cfg = LOGIC_CONFIG->GetWuXingGuaJiConfig().GetWuXingGuaJiTargetCfg(i);
					if (NULL != target_cfg)
					{
						my_target->is_getreward = 1;

						static MailContentParam reward_mail;  reward_mail.Reset();

						reward_mail.item_list[0].item_id = target_cfg->reward_cfg.item_id;
						reward_mail.item_list[0].num = target_cfg->reward_cfg.num;
						reward_mail.item_list[0].is_bind = (target_cfg->reward_cfg.is_bind ? 1 : 0);
						const ItemBase *itembase = ITEMPOOL->GetItem(target_cfg->reward_cfg.item_id);
						if (NULL != itembase) reward_mail.item_list[0].invalid_time = itembase->CalInvalidTime();

						int l1 = SNPRINTF(reward_mail.contenttxt, sizeof(reward_mail.contenttxt), gamestring::g_wuxingguaji_reward_content);
						if (l1 > 0)
						{
							MailRoute::MailToUser(wuxingguaji_user->user_id, SYSTEM_MAIL_REASON_INVALID, reward_mail);
						}
					}
				}
			}
		}
	}

	{
		for (int i = 0; i < WUXINGGUAJI_BOSS_NUM; i++)
		{
			if (INVALID_OBJ_ID != m_boss_info_list[i].boss_objid)
			{
				Obj *obj = m_scene->GetObj(m_boss_info_list[i].boss_objid);
				if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())								
				{
					Monster *monster = (Monster*)obj;
					if (monster->IsAlive()) monster->ForceSetDead();
				}
			}
			
			m_boss_info_list[i].monster_id = 0;
			m_boss_info_list[i].boss_objid = INVALID_OBJ_ID;
			m_boss_info_list[i].next_boss_flush_time = 0;
		}
	}

	this->DelayKickOutAllRole();
}

void SpecialWuXingGuaJi::SendUserInfo(Role *role, int reason)
{
	static Protocol::SCWuXingGuaJiInfo wgji;
	wgji.reason = reason;

	if (NULL != role)
	{
		WuXingGuaJiUser *wuxingguaji_user = this->GetWuXingGuaJiUser(role->GetUserId());
		if (NULL != wuxingguaji_user)
		{
			for (int i = 0; i < WUXINGGUAJI_BOSS_NUM; i++)
			{
				wgji.boss_list[i].monster_id = m_boss_info_list[i].monster_id;
				wgji.boss_list[i].next_boss_flush_time = m_boss_info_list[i].next_boss_flush_time;
			}

			wgji.my_lucky_count = wuxingguaji_user->lucky_count;
			memcpy(wgji.target_list, wuxingguaji_user->target_list, sizeof(wgji.target_list));

			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&wgji, sizeof(wgji));
		}
	}
	else
	{
		for (WuXingGuaJiUserMapIt it = m_wuxingguaji_user_map.begin(); it != m_wuxingguaji_user_map.end(); ++ it)
		{
			const WuXingGuaJiUser *wuxingguaji_user = &it->second;
			if (NULL != wuxingguaji_user && INVALID_GSNETID != wuxingguaji_user->net_id)
			{
				for (int i = 0; i < WUXINGGUAJI_BOSS_NUM; i++)
				{
					wgji.boss_list[i].monster_id = m_boss_info_list[i].monster_id;
					wgji.boss_list[i].next_boss_flush_time = m_boss_info_list[i].next_boss_flush_time;
				}

				wgji.my_lucky_count = wuxingguaji_user->lucky_count;
				memcpy(wgji.target_list, wuxingguaji_user->target_list, sizeof(wgji.target_list));

				EngineAdapter::Instance().NetSend(wuxingguaji_user->net_id, (const char*)&wgji, sizeof(wgji));
			}
		}
	}
}

void SpecialWuXingGuaJi::SendLuckyInfoToUser(Role *role)
{
	if (NULL == role)
	{
		int sendlen = sizeof(m_wxgjli) - (WUXINGGUAJI_LUACY_ROLE_MAX - m_wxgjli.luck_user_count) * sizeof(m_wxgjli.luck_user_namelist[0]);
		m_scene->SendToRole((const char*)&m_wxgjli, sendlen);
	}
	else
	{
		int sendlen = sizeof(m_wxgjli) - (WUXINGGUAJI_LUACY_ROLE_MAX - m_wxgjli.luck_user_count) * sizeof(m_wxgjli.luck_user_namelist[0]);
		if (sendlen > 0 && sendlen <= static_cast<int>(sizeof(m_wxgjli)))
		{
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&m_wxgjli, sendlen);
		}
	}
}

void SpecialWuXingGuaJi::GaveLuckyItemToUser()
{
	int lucky_role_count = 0;
	int lucky_role_idx_list[WUXINGGUAJI_LUACY_ROLE_MAX] = {0};

	{
		std::map<int, int> idx_map;

		int max_rolenum = m_scene->RoleNum();
		for (int i = 0; i < max_rolenum; i++)  // 通过map来打乱idx，用于随机
		{
			idx_map.insert(std::make_pair(RandomNum(1024), i));
		}

		int loop_count = 0;
		while (!idx_map.empty() && lucky_role_count < WUXINGGUAJI_LUACY_ROLE_MAX && ++ loop_count < 128)
		{
			int rand_index = RandomNum(static_cast<int>(idx_map.size()));
			std::map<int, int>::iterator it = idx_map.begin();
			std::advance(it, rand_index);

			lucky_role_idx_list[lucky_role_count ++] = it->second;
			idx_map.erase(it);
		}
	}

	m_wxgjli.luck_user_count = 0;

	char str_format[256] = {0};
	int max_length = sizeof(str_format) / sizeof(str_format[0]);
	int str_length = 0;

	const ItemConfigData &lucky_item = LOGIC_CONFIG->GetWuXingGuaJiConfig().GetLuckyItem();

	for (int i = 0; i < lucky_role_count && i < WUXINGGUAJI_LUACY_ROLE_MAX; i++)
	{
		int role_idx = lucky_role_idx_list[i];
		if (role_idx < (int)m_scene->RoleNum())
		{
			Role *lucky_role = m_scene->GetRoleByIndex(role_idx);
			if (NULL != lucky_role)
			{
				static MailContentParam contentparam; contentparam.Reset();

				const ItemBase *itembase = ITEMPOOL->GetItem(lucky_item.item_id);
				if (NULL != itembase)
				{
					contentparam.item_list[0].item_id = lucky_item.item_id;
					contentparam.item_list[0].is_bind = lucky_item.is_bind ? 1 : 0;
					contentparam.item_list[0].num = lucky_item.num;
					contentparam.item_list[0].invalid_time = itembase->CalInvalidTime();
				}

				MailRoute::MailToUser(lucky_role->GetUserId(), SYSTEM_MAIL_REASON_WUXINGGUAJI_LUCKY_REWARD, contentparam);

				if (m_wxgjli.luck_user_count < WUXINGGUAJI_LUACY_ROLE_MAX)
				{
					lucky_role->GetName(m_wxgjli.luck_user_namelist[m_wxgjli.luck_user_count]);
					++ m_wxgjli.luck_user_count;
				}

				WuXingGuaJiUser *wuxingguaji_user = this->GetWuXingGuaJiUser(lucky_role->GetUserId());
				if (NULL != wuxingguaji_user)
				{
					++ wuxingguaji_user->lucky_count;
				}

				if (i < 5)	// 最多打包5个名字 不用WUXINGGUAJI_LUACY_ROLE_MAX 防止改动会导致字符串不够
				{
					int temp_length = SNPRINTF(str_format + str_length, max_length - str_length, "{r;%d;%s;%d} ", 
						UidToInt(lucky_role->GetUserId()), lucky_role->GetName(), lucky_role->GetCamp());
					str_length += temp_length;
				}
			}
		}
	}

	this->SendLuckyInfoToUser();

	{
		int str_len = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_wuxingguaji_lucky_notice, 
			str_format);
		if (str_len > 0) 
		{
			//this->NotifySystemMsg(gamestring::GAMESTRING_BUF, str_len, SYS_MSG_CENTER_NOTICE);
		}
	}
}

WuXingGuaJiUser * SpecialWuXingGuaJi::GetWuXingGuaJiUser(const UserID &user_id)
{
	WuXingGuaJiUserMapIt it = m_wuxingguaji_user_map.find(user_id);
	if (it == m_wuxingguaji_user_map.end()) // 不存在 插入新的用户
	{
		WuXingGuaJiUser wuxingguaji_user;

		wuxingguaji_user.user_id = user_id;
		m_wuxingguaji_user_map[user_id] = wuxingguaji_user;
	}

	if (it == m_wuxingguaji_user_map.end()) it = m_wuxingguaji_user_map.find(user_id);

	if (it != m_wuxingguaji_user_map.end())
	{
		return &it->second;
	}

	return NULL;
}
















