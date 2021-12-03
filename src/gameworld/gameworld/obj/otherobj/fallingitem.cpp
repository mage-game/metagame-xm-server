#include "fallingitem.h"
#include "scene/scene.h"
#include "obj/character/role.h"
#include "item/itempool.h"
#include "item/knapsack.h"
#include "servercommon/errornum.h"
#include "globalconfig/globalconfig.h"
#include "protocal/msgitem.h"
#include "engineadapter.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "scene/scenemanager.h"
#include "scene/speciallogic/speciallogic.hpp"

#include "monster/monsterinitparam.h"
#include "monster/monsterpool.h"
#include "world.h"
#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"

#include "npc/transmit/fbmanager.hpp"
#include "equipment/equipmentfunction.h"
#include "equipment/equipmentmanager.h"

#include "chat/chatmanager.h"
#include "protocal/msgfriendgift.hpp"
#include "task/newtaskmanager.h"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/friendgift/friendgiftmanager.hpp"
#include "other/shenshou/shenshou.hpp"
#include "item/expense/norexitem.hpp"
#include "global/droplog/droplogmanager.hpp"
#include "scene/speciallogic/worldspecial/specialcrossboss.hpp"
#include "other/cross/rolecross.h"
#include "scene/speciallogic/worldspecial/specialcrossmizangboss.hpp"
#include "scene/speciallogic/worldspecial/specialcrossyoumingboss.hpp"
#include "other/runesystem/runesystem.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/fallingmsgconfig.hpp"
#include "config/guildconfig.hpp"
#include "internalcomm.h"
#include "other/tianshenhuti/tianshenhuti.hpp"

FallingItem::FallingItem()
	: Obj(OBJ_TYPE_FALLINGITEM), m_is_create(true), m_item_id(ItemBase::INVALID_ITEM_ID), m_item_num(0), m_item_is_bind(false),
	m_owner_user_id(), m_scene_id(0), m_drop_time(0), m_disappear_time(0), m_coin_or_gold(0), m_has_pick(false), m_monster_id(0),
	m_broadcast(0), m_is_from_body(false), m_is_buff_falling(false), m_buff_appearan(FALLING_APPEARAN_INVALID)
{

}

FallingItem::~FallingItem()
{

}

void FallingItem::Init(UInt16 item_id, int item_num, bool item_is_bind, int coin_or_gold, const UserID &owner_user_id, int scene_id, unsigned int disappear_time,
					   UInt16 monster_id, short is_broadcast, bool is_from_body)
{
	m_is_create = true;

	m_drop_time = EngineAdapter::Instance().Time();

	m_coin_or_gold = coin_or_gold;
	m_item_id = item_id;
	m_item_num = item_num;
	m_item_is_bind = item_is_bind;
	m_owner_user_id = owner_user_id;
	m_scene_id = scene_id;
	m_monster_id = monster_id;
	m_broadcast = is_broadcast;
	m_is_from_body = is_from_body;

	if (disappear_time > 0)
	{
		m_disappear_time = m_drop_time + disappear_time;
	}
	else
	{
		m_disappear_time = 0;
	}
}

void FallingItem::Update(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	m_is_create = false;
	if (m_disappear_time > 0 && now_second > m_disappear_time)	
	{
		m_scene->DeleteObj(m_obj_id);
	}
}

int FallingItem::Pick(Role *role, bool is_auto_pick, MailContentParam *contentparam)
{
	if (m_has_pick) return PR_HAS_PICK;

	if (!is_auto_pick && !role->IsAlive())
	{
		role->NoticeNum(errornum::EN_FALLING_ROLE_DEAD);
		return PR_ROLE_DEAD;
	}

	if (!this->IsOwner(role)) 
	{
		if (!is_auto_pick) role->NoticeNum(errornum::EN_FALLING_NOT_YOURS);
		return PR_NOT_YOURS;
	}

	if (!is_auto_pick)
	{
		const unsigned int pick_length = GLOBALCONFIG->GetOtherConfig_ItemDrop_Length();
		if ((role->GetPos() - m_posi).IsLongerThan(pick_length))
		{
			role->NoticeNum(errornum::EN_FALLING_TOO_FAR);
			return PR_TOO_FAR;
		}
	}

	if (!m_scene->GetSpecialLogic()->SpecialCanPick(role, this))
	{
		return PR_SCENE_SPECLAI_CAN_NOT_PICK;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(m_item_id);
	if (NULL != item_base)
	{
		EquipmentParam equip_param;
		ItemDataWrapper item_wrapper;

		if (!this->GetPickItem(item_wrapper, &equip_param))
		{
			return PR_EXCEPTION;
		}

		bool is_put_in_knapsack = true;
 		if (ItemBase::I_TYPE_EXPENSE == item_base->GetItemType())
 		{
			const NorexItem *norex_item = dynamic_cast<const NorexItem *>(item_base);

//			//神兽系统，拾取相关主动消耗类时直接放入神兽背包
//  			const NorexItem *norex_item = dynamic_cast<const NorexItem *>(item_base);
//  			if (NorexItem::I_NOREX_ITEM_SHENSHOU == norex_item->GetUseType())
//  			{
//  				if (!role->GetShenshou()->TransformFromKnapsackItem(m_item_id, m_item_num,
//  					static_cast<short>(norex_item->GetParam1()), static_cast<short>(norex_item->GetParam2()),
//  					norex_item->GetParam3() > 0, norex_item->GetParam4(), "Pick Shenshou"))
//  				{
//  					return PR_KNAPSACK_NO_SPACE;
//  				}
//  				is_put_in_knapsack = false;
//  			}
//  
//  			//圣印直接放入圣印背包
//  			if (NorexItem::I_NOREX_ITEM_ITEM_SEAL == norex_item->GetUseType())
//  			{
//  				if (!role->GetSeal()->AddSealByItemId(m_item_id, m_item_num, norex_item->GetParam1()))
//  				{
//  					return PR_KNAPSACK_NO_SPACE;
//  				}
//  				is_put_in_knapsack = false;
//  			}

			if (nullptr != norex_item)
			{
				// 战魂放入战魂背包
				if (NorexItem::I_NOREX_ITEM_RUNE == norex_item->GetUseType())
				{
					if (!role->GetRuneSystem()->AddRuneByItemId(m_item_id, m_item_num))
					{
						return PR_KNAPSACK_NO_SPACE;
					}
					is_put_in_knapsack = false;
				}
				// 周末装备X阶以下自动使用
				else if (NorexItem::I_NOREX_ITEM_TIANSHENHUTI_EQUIP == norex_item->GetUseType())
				{
					TianshenHuti* tianshenhuti = role->GetOtherModule<TianshenHuti>(ROLE_MODULE_TYPE_TIANSHENHUTI);
					if (nullptr != tianshenhuti && tianshenhuti->OnAutoAddEquipByItem(norex_item->GetParam1(), "Pick"))
					{
						is_put_in_knapsack = false;
					}
				}
			}
 		}

		if (is_put_in_knapsack)
		{
			if (!role->GetKnapsack()->Put(item_wrapper, PUT_REASON_PICK, NULL, m_monster_id))
			{
				if (NULL == contentparam || contentparam->item_count >= MAX_ATTACHMENT_ITEM_NUM)
				{
					return PR_KNAPSACK_NO_SPACE;
				}

				// 自动拾取的时候背包已满邮件补发
				contentparam->item_list[contentparam->item_count++] = item_wrapper;
			}
		}

		role->GetTaskManager()->OnPickUpItem(m_item_id, m_item_num);

		const MonsterInitParam *monster_param = MONSTERPOOL->GetMonsterParam(m_monster_id);
		if (NULL != monster_param && MonsterInitParam::MONSTER_TYPE_BOSS != monster_param->monster_type)
		{
			m_broadcast = FALLING_ITEM_BROADCAST_TYPE_NO_BROADCAST;
		}
		const FallingMSG * falling_msg = LOGIC_CONFIG->GetFallingMSGConfig().GetFallingMSG(m_broadcast);
		if (m_broadcast > 0 && nullptr != falling_msg && item_base->GetColor() >= falling_msg->color)
		{
			int length = 0;

			if (m_is_from_body)
			{
				if (item_base->GetItemType() == ItemBase::I_TYPE_EQUIPMENT)  //装备类要带天赋属性显示
				{
					const char *equip_param_string = EquipFunction::EquipParamDataClientString(UidToInt(role->GetUserId()), m_item_id, true, 0, &equip_param, NULL);
					if (NULL != equip_param_string)
					{
						length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), falling_msg->game_string[FALLING_ITEM_GAME_STRING_TYPE_EQUIP_FROM_BODY].c_str(),
							UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), m_monster_id, (int)m_item_id, equip_param_string);
					}
				}
				else
				{
					length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), falling_msg->game_string[FALLING_ITEM_GAME_STRING_TYPE_NORMAL_FROM_BODY].c_str(),
						UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), m_monster_id, (int)m_item_id,
						m_scene->GetSceneID(), m_posi.x, m_posi.y);
				}
			}
			else
			{
				if (item_base->GetItemType() == ItemBase::I_TYPE_EQUIPMENT)  //装备类要带天赋属性显示
				{
					const char *equip_param_string = EquipFunction::EquipParamDataClientString(UidToInt(role->GetUserId()), m_item_id, true, 0, &equip_param, NULL);
					if (NULL != equip_param_string)
					{
						length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), falling_msg->game_string[FALLING_ITEM_GAME_STRING_TYPE_EQUIP].c_str(),
							UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), m_monster_id, (int)m_item_id, equip_param_string);
					}
				}
				else
				{
					length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), falling_msg->game_string[FALLING_ITEM_GAME_STRING_TYPE_NORMAL].c_str(),
						UidToInt(role->GetUserId()), role->GetName(), role->GetCamp(), m_monster_id, (int)m_item_id);
				}

				if (Scene::SCENE_TYPE_CROSS_BOSS == role->GetScene()->GetSceneType())
				{
					SpecialCrossBoss *cross_boss = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossBoss(role->GetScene()->GetSceneID(), CROSS_PUBLIC_SCENE_KEY);
					if (NULL != cross_boss)
					{
						cross_boss->OnPickFallingItem(role, role->GetScene()->GetSceneID(), m_monster_id, m_item_id, item_wrapper);
					}
				}
				else if (Scene::SCENE_TYPE_CROSS_MIZANG_BOSS == role->GetScene()->GetSceneType())
				{
					SpecialCrossMiZangBoss *cross_boss = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossMizangBoss(role->GetScene()->GetSceneID(), CROSS_PUBLIC_SCENE_KEY);
					if (NULL != cross_boss)
					{
						cross_boss->OnPickFallingItem(role, role->GetScene()->GetSceneID(), m_monster_id, m_item_id, item_wrapper);
					}
				}
				else if (Scene::SCENE_TYPE_CROSS_YOUMING_BOSS == role->GetScene()->GetSceneType())
				{
					SpecialCrossYouMingBoss *cross_boss = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossYoumingBoss(role->GetScene()->GetSceneID(), CROSS_PUBLIC_SCENE_KEY);
					if (NULL != cross_boss)
					{
						cross_boss->OnPickFallingItem(role, role->GetScene()->GetSceneID(), m_monster_id, m_item_id, item_wrapper);
					}
				}
				else if (role->GetScene()->IsDropRecordBossScene())
				{
					if (CrossConfig::Instance().IsHiddenServer())
					{
						DropLogManager::Instance().AddCrossLog(UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()), role->GetName(), m_monster_id, (int)m_item_id, m_item_num, m_scene->GetSceneID(),equip_param.xianpin_type_list, ItemNamespace::MAX_XIANPIN_NUM);
					}
					else
					{
						DropLogManager::Instance().AddLog(role->GetUID(), role->GetName(), m_monster_id, (int)m_item_id, m_item_num, m_scene->GetSceneID(),equip_param.xianpin_type_list, ItemNamespace::MAX_XIANPIN_NUM);
					}
				}
			}

			if (length > 0)
			{
				if (!CrossConfig::Instance().IsHiddenServer())
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, falling_msg->sys_msg_type);
				}
				else
				{
					World::GetInstWorld()->GetSceneManager()->SystemMsgToAllGameWorld(gamestring::GAMESTRING_BUF, length, falling_msg->sys_msg_type);
				}
			}
		}

		bool is_notice_to_guild = false;
		//if (m_broadcast)
		//{
		//	is_notice_to_guild = true;
		//}

		if (item_base->GetColor() >= ItemBase::I_COLOR_RED && ItemBase::I_TYPE_EQUIPMENT == item_base->GetItemType())
		{
			auto guild_rumor_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildRumorCfg(GUILD_EVENT_TYPE_RUMOR_GET_RED_EQUIP);
			if (guild_rumor_cfg != NULL)
			{
				const Equipment *equip = (const Equipment *)item_base;

				if (equip->GetOrder() >= guild_rumor_cfg->param_1)
				{
					//江湖传闻
					GuildID guild_id = role->GetGuildID();
					if (INVALID_GUILD_ID != guild_id)
					{
						EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_GET_RED_EQUIP, item_base->GetItemId(), 
							(int)equip_param.xianpin_type_list[0], (int)equip_param.xianpin_type_list[1], (int)equip_param.xianpin_type_list[2]);
					}
				}

				if (equip->GetOrder() >= LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig().broadcast_grade)
				{
					is_notice_to_guild = true;		//策划需求，红色X阶装备才推送到仙盟聊天
				}
			}
		}

		//获得红色以及橙色二星以上装备时，在公会里通告		
		if (!is_notice_to_guild && ItemBase::I_TYPE_EQUIPMENT == item_base->GetItemType() && item_base->GetColor() >= ItemBase::I_COLOR_ORANGE)
		{
			//if (item_base->GetColor() >= ItemBase::I_COLOR_RED)			//红色以上装备		//4.28策划需求，红色X阶装备才推送到仙盟聊天，所以先屏蔽这里
			//{
			//	is_notice_to_guild = true;
			//}

			// 先屏蔽
//			else
//			{
//				int good_xianpin_count = 0;
//				for (int index = 0; index < ItemNamespace::MAX_XIANPIN_NUM; ++index)
//				{
//					const EquipXianPinConfig *xianpin_cfg = GLOBALCONFIG->GetEquipConfig().GetXianPinEquipCfg(equip_param.xianpin_type_list[index]);
//					if (NULL == xianpin_cfg)
//					{
//						continue;
//					}
//
//					if (xianpin_cfg->color == QUALITY_COLOR_TYPE_GOOD)
//					{
//						++good_xianpin_count;
//
//						//橙色二星以上装备
//						if (good_xianpin_count >= 2)
//						{
//							is_notice_to_guild = true;
//							break;
//						}
//					}
//				}
//			}
		}

		if (is_notice_to_guild)
		{
			const char *equip_param_string = EquipFunction::EquipParamDataClientString(role->GetUID(), m_item_id, true, 0, &equip_param, NULL);
			if (NULL != equip_param_string)
			{
				Protocol::CSChannelChat *cc = (Protocol::CSChannelChat *)gamestring::GAMESTRING_BUF;
				cc->msg_length = SNPRINTF(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSChannelChat),
					sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSChannelChat), gamestring::g_item_get_equipment_send_to_guild,
					m_scene->GetSceneName(), role->GetPos().x, role->GetPos().y, m_scene->GetSceneID(), m_scene->GetSceneKey(), role->IsInCross() ? 1 : 0,
					m_monster_id, m_item_id, equip_param_string);

				if (cc->msg_length > 0)
				{
					cc->channel_type = chatdef::CHANNEL_TYPE_GUILD;
					ChatManager::Instance().OnChannelChat(role, cc, sizeof(Protocol::CSChannelChat) + cc->msg_length, chatdef::ORIGIN_TYPE_FALLING_ITEM);
				}
			}
		}

		if (NULL != item_base)
		{
			Role * role = World::GetInstWorld()->GetSceneManager()->GetRole(m_owner_user_id);
			if (NULL != role)
			{
				role->GetFriendGiftManager()->ReqHeliNotice(Protocol::SC_FRIEND_HELI_SKILL_BOSS_FETCH_EQUI_REQ, m_monster_id, m_item_id);
			}
		}

		{
			if (!item_base->IsBind())
			{
				if (!CrossConfig::Instance().IsHiddenServer())
				{
					//仙盟珍稀日志
					GuildID guild_id = role->GetGuildID();
					if (INVALID_GUILD_ID != guild_id)
					{
						//GuildRareLogManager::Instance().AddLog(guild_id, role->GetUID(), role->GetName(), m_item_id, m_item_num, m_scene_id, m_monster_id);

						GuildRareLogItem temp_item;
						//temp_item.uid = role->GetUID();
						temp_item.uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
						strcpy(temp_item.role_name, role->GetName());
						temp_item.item_id = m_item_id;
						temp_item.item_num = m_item_num;
						temp_item.monster_id = m_monster_id;
						temp_item.scene_id = m_scene_id;
						temp_item.timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

						if (ItemBase::I_TYPE_EQUIPMENT == item_base->GetItemType())
						{
							for (int i = 0; i < ItemNamespace::MAX_XIANPIN_NUM; ++i)
							{
								temp_item.xianpin_type_list[i] = equip_param.xianpin_type_list[i];
							}
						}

						GuildRareLogManager::Instance().AddLog(guild_id, temp_item);
					}
				}
				else
				{
					//仙盟珍稀日志
					GuildID guild_id = role->GetGuildID();
					if (INVALID_GUILD_ID != guild_id)
					{
						this->OnPickGuildRareItemInHidden(role);
					}
				}
			}
		}

		EventHandler::Instance().OnPickFallingItem(role, this, &item_wrapper);
	}
	else 
	{
		if (m_coin_or_gold > 0)
		{
			if (COIN_ITEM_ID == m_item_id)
			{
				role->GetKnapsack()->GetMoney()->AddCoinBind(m_coin_or_gold, "Pick");
			}
			else if (BIND_GOLD_ITEM_ID == m_item_id)
			{
				role->GetKnapsack()->GetMoney()->AddGoldBind(m_coin_or_gold, "Pick");
			}
		}

		EventHandler::Instance().OnPickFallingItem(role, this, NULL);
	}

	{
		static Protocol::SCPickItemAck pi_ack;
		pi_ack.count = 1;
		pi_ack.obj_id_list[0] = this->GetId();

		UInt32 sendlen = sizeof(Protocol::SCPickItemAck) - sizeof(ObjID) * (Protocol::SCPickItemAck::MAX_PICK_TIME_ACK - pi_ack.count);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pi_ack, sendlen);
	}
	
	m_has_pick = true;
	m_scene->DeleteObj(m_obj_id);

	return PR_SUC;
}

bool FallingItem::GetPickItem(ItemDataWrapper &item_wrapper, EquipmentParam *equip_param)
{
	return this->GetPickItem(m_monster_id, m_item_id, m_item_num, m_item_is_bind, item_wrapper, equip_param, m_scene_id);
}

// 警告:不熟物品创建过程不要改这里
bool FallingItem::GetPickItem(int monster_id, ItemID item_id, int item_num, bool is_bind, ItemDataWrapper &item_wrapper, EquipmentParam *equip_param, int scene_id)
{
	const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
	if (NULL == itembase) return false;

	item_wrapper.item_id = item_id;
	item_wrapper.num = item_num;
	item_wrapper.is_bind = is_bind;
	item_wrapper.invalid_time = itembase->CalInvalidTime();
	item_wrapper.gold_price = 0;
	item_wrapper.has_param = false;
	item_wrapper.param_data.Reset();

	if (ItemBase::I_TYPE_EQUIPMENT == itembase->GetItemType())
	{
		static ItemGridData grid_data;
		grid_data.Reset();
		grid_data.Set(item_wrapper, itembase->GetColor());

		// 掉落权重优先级 monster_id > scene_id > color
		if (!EquipmentManager::GetFixSpecialAttr(&grid_data, Equipment::EQUIPMENT_FIX_GETWAY_MONSTER_ID_DROP, monster_id))
		{
			if (!EquipmentManager::GetFixSpecialAttr(&grid_data, Equipment::EQUIPMENT_FIX_GETWAY_SCENE_ID_DROP, scene_id))
			{
				EquipmentManager::GetFixSpecialAttr(&grid_data, Equipment::EQUIPMENT_FIX_GETWAY_COLOR_DROP, itembase->GetColor());
			}
		}

		if (NULL != grid_data.param)
		{
			if (NULL != equip_param) *equip_param = *((EquipmentParam *)grid_data.param);
			grid_data.param->SetInStructData(&item_wrapper.param_data);
			item_wrapper.has_param = true;

			delete grid_data.param;
			grid_data.param = NULL;
		}
	}

	return true;
}

bool FallingItem::IsOwner(Role *role)
{
	if (role->GetUserId() == m_owner_user_id || INVALID_USER_ID == m_owner_user_id) // 1：是该玩家的物品或没有属主
	{
		return true;
	}

	if (role->GetRoleStatusManager()->IsInFB()) // 2：在副本中不允许拾取别人的东西 
	{
		return false;
	}

	const int owner_interval = GLOBALCONFIG->GetOtherConfig_ItemDrop_Interval_Owner();
	if (EngineAdapter::Instance().Time() > (m_drop_time + owner_interval))
	{
		return true;
	}

	return false;
}

void FallingItem::OnPickGuildRareItemInHidden(Role *role)
{
	if(role == NULL) return;

	crossgameprotocal::PickGuildRareItemInHiddenGs info;
	
	info.item.item_id = m_item_id;
	info.item.item_num = m_item_num;
	info.item.uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	strcpy(info.item.role_name, role->GetName());
	info.guild_id = role->GetGuildID();
	info.item.monster_id = m_monster_id;
	info.item.scene_id = m_scene_id;
	info.item.timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_UUID,
		UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()),
		(const char*)&info, sizeof(info));
}

