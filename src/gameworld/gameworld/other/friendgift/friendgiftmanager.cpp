#include "friendgiftmanager.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/commondata.hpp"
#include "servercommon/commondataglobaldef.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/struct/friendgiftparam.hpp"
#include "servercommon/userprotocal/msgsystem.h"
#include "other/event/eventhandler.hpp"
#include "gameworld/scene/scenemanager.h"
#include "gameworld/world.h"
#include "gamecommon.h"
#include "protocal/msgother.h"
#include "serverlogic.h"
#include "effect/effectbase.hpp"
#include "protocal/msgfriendgift.hpp"
#include "chat/chatmanager.h"
#include "protocal/msgchatmsg.h"
#include "skill/skillpool.hpp"
#include "skill/skillbase.hpp"
#include "item/knapsack.h"
#include "item/itempool.h"
#include "item/itembase.h"
#include "other/friendgift/friendgiftconfig.hpp"
#include "other/route/mailroute.hpp"
#include "global/usercache/usercache.hpp"
#include "global/globalrole/commondata/commondataglobal.hpp"
#include "friend/friendmanager.h"
#include "equipment/equipment.h"
#include <set>

FriendGiftManager::FriendGiftManager() : m_role(NULL)
{
	m_friend_gift_param.Reset();
	memset(m_friend_heli_list, 0, sizeof(m_friend_heli_list));
}

FriendGiftManager::~FriendGiftManager()
{
}

void FriendGiftManager::Init(Role *role, const FriendGiftParam &friend_gift_param)
{
	m_role = role;
	m_friend_gift_param = friend_gift_param;
}

void FriendGiftManager::GetInitParam(FriendGiftParam *friend_gift_param)
{
	*friend_gift_param = m_friend_gift_param;
}

// 总信息
void FriendGiftManager::SendFriendGiftInfo()
{
	static Protocol::SCFriendGiftAllInfo pro;
	memset(pro.shou_gift_list, 0, sizeof(pro.shou_gift_list));
	pro.shou_gift_list_count = 0;

	pro.song_gift_count = m_friend_gift_param.song_gift_count;
	pro.shou_gift_count = m_friend_gift_param.shou_gift_count;
	
	for (int i = 0; i < FriendGiftParam::FRIEND_GIFT_LIST_COUNT; i++)
	{
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(m_friend_gift_param.shou_gift_info_list[i].uid);
		if (NULL != user_node)
		{
			user_node->GetName(pro.shou_gift_list[i].name);
			pro.shou_gift_list[i].uid = m_friend_gift_param.shou_gift_info_list[i].uid;
			pro.shou_gift_list[i].is_return = m_friend_gift_param.shou_gift_info_list[i].is_return;
			pro.shou_gift_list[i].shou_gift_time = m_friend_gift_param.shou_gift_info_list[i].shou_gift_time;
			
			pro.shou_gift_list_count ++;
		}
	}
	
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));
}

void FriendGiftManager::OnFriendGiftSongGift(int uid, int is_all_return, int is_return_gift)
{
	if (0 == is_all_return)
	{
		this->OnFriendGiftSongGiftSingle(uid, is_return_gift);
	}else
	{
		this->OnFriendGiftSongGiftAll();
	}
}

// 一键回礼
void FriendGiftManager::OnFriendGiftSongGiftAll()
{
	for (int i = 0 ; i < FriendGiftParam::FRIEND_GIFT_LIST_COUNT; i++)
	{
		if (0 == m_friend_gift_param.shou_gift_info_list[i].is_return)
		{
			Role * role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_friend_gift_param.shou_gift_info_list[i].uid));
			if (role != NULL)
			{
				this->OnFriendGiftSongGiftSingle(m_friend_gift_param.shou_gift_info_list[i].uid);
			}
		}
	}
}

// 送礼物
void FriendGiftManager::OnFriendGiftSongGiftSingle(int uid, int is_return_gift)
{
	Role * role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (NULL == role)
	{
		// 好友不在线
		m_role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	int song_count = m_role->GetFriendManager()->GetFriendGiftCount(role->GetUserId());
	if (song_count >= FriendGiftParam::FRIEND_GIFT_MAX_SONG_COUNT)
	{
		// 今天对此好友的送礼次数已经用完
		m_role->NoticeNum(errornum::EN_FRIEND_GIFT_NOT_TIMES);
		return;
	}

	m_role->GetFriendManager()->SetFriendGiftCount(role->GetUserId(), song_count + 1);

	role->GetFriendGiftManager()->OnFriendGiftShouGift(m_role->GetUID());

	const FfOtherCfg &star_item_cfg = LOGIC_CONFIG->getFriendGiftCfg().GetOtherCfg();

	// 增加亲密度
	m_role->GetFriendManager()->IntimacyIncreaseHelper(role->GetUserId(), star_item_cfg.add_intimacy, false, "FriendGiftShou", false);
	role->GetFriendManager()->IntimacyIncreaseHelper(m_role->GetUserId(), star_item_cfg.add_intimacy, false, "FriendGiftSong", false);

	for (int i = 0 ; i < FriendGiftParam::FRIEND_GIFT_LIST_COUNT; i++)
	{
		if (m_friend_gift_param.shou_gift_info_list[i].uid == uid)
		{
			m_friend_gift_param.shou_gift_info_list[i].is_return = 1; // 已回礼
		}
	}

	m_friend_gift_param.song_gift_count ++;

	if (1 == is_return_gift)
	{
		m_role->NoticeNum(errornum::EN_FRIEND_GIFT_RETURN_SUCCESS);
	}
	else if (0 == is_return_gift)
	{
		m_role->NoticeNum(errornum::EN_FRIEND_GIFT_SONG_SUCCESS);
	}

	this->SendFriendGiftInfo();

	// 送礼成功自动私聊
	Protocol::CSSingleChat *sc = (Protocol::CSSingleChat *)gamestring::GAMESTRING_BUF;
	sc->msg_length = SNPRINTF(gamestring::GAMESTRING_BUF + sizeof(Protocol::CSSingleChat),
		sizeof(gamestring::GAMESTRING_BUF) - sizeof(Protocol::CSSingleChat), gamestring::g_msg_gift_send_singlechat);
	if (sc->msg_length > 0)
	{
		sc->to_uid = role->GetUID();
		ChatManager::Instance().OnSingleChat(m_role, sc, sizeof(Protocol::CSSingleChat) + sc->msg_length, true);
	}
}

// 收礼物
void FriendGiftManager::OnFriendGiftShouGift(int uid)
{
	Role * role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (role != NULL)
	{	
		unsigned int min_time = 0;
		int index = 0;
	
		for (int i = FriendGiftParam::FRIEND_GIFT_LIST_COUNT - 1 ; i >= 0 ; i--)
		{
			if (m_friend_gift_param.shou_gift_info_list[i].uid == uid) // 已存在
			{
				index = i;
				break;
			}

			if (m_friend_gift_param.shou_gift_info_list[i].shou_gift_time <= min_time)
			{
				min_time = m_friend_gift_param.shou_gift_info_list[i].shou_gift_time;
				index = i;
			}
		}

		m_friend_gift_param.shou_gift_info_list[index].uid = uid;
		m_friend_gift_param.shou_gift_info_list[index].shou_gift_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

		int is_return = 0;
		if (m_role->GetFriendManager()->GetFriendGiftCount(role->GetUserId()) > 0)
		{
			is_return = 1;
		}
		m_friend_gift_param.shou_gift_info_list[index].is_return = is_return;

		const FfOtherCfg &star_item_cfg = LOGIC_CONFIG->getFriendGiftCfg().GetOtherCfg();
		
		// 邮件发物品
		if (m_friend_gift_param.shou_gift_count < star_item_cfg.receive_gift_count_limit)
		{
			static MailContentParam contentparam; 
			contentparam.Reset();

			UNSTD_STATIC_CHECK(MAX_ATTACHMENT_ITEM_NUM >= 1);
			contentparam.item_list[0].item_id = star_item_cfg.gift_item.item_id;
			contentparam.item_list[0].is_bind = star_item_cfg.gift_item.is_bind ? 1 : 0;
			contentparam.item_list[0].num = star_item_cfg.gift_item.num;

			int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_friend_gift_shou_content, role->GetUID(), role->GetName(), (int)role->GetCamp());
			if (length > 0)
			{
				MailRoute::MailToUser(m_role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}

			m_friend_gift_param.shou_gift_count ++;
		}

		static Protocol::SCFriendGiftShouNotice pro;
		EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&pro, sizeof(pro));

		this->SendFriendGiftInfo();
	}
}

void FriendGiftManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	if (old_dayid == now_dayid) return;

	m_friend_gift_param.Reset();

	this->SendFriendGiftInfo();
	m_role->GetFriendManager()->SendFriendListToRole();
}

void FriendGiftManager::OnRoleLevelChange(int level)
{
	this->ReqHeliNotice(Protocol::SC_FRIEND_HELI_UPLEVEL_REQ, level);
}

void FriendGiftManager::ReqHeliNotice(int type, int param1, int param2)
{
	CommonDataParam *common_data = m_role->GetCommonDataParam();
	if (NULL == common_data)
	{
		return;
	}

	const FfOtherCfg &star_item_cfg = LOGIC_CONFIG->getFriendGiftCfg().GetOtherCfg();
	if (static_cast<int>(EngineAdapter::Instance().Time()) < common_data->last_send_heli_notice_time + star_item_cfg.heli_notice_time_interval)
	{
		return;
	}

	static Protocol::SCFriendHeliNotice pro; 
	pro.heli_type = type;
	pro.uid = m_role->GetUID();
	pro.param1 = 0;
	pro.param2 = 0;

	if (Protocol::SC_FRIEND_HELI_UPLEVEL_REQ == type)
	{
		if (!LOGIC_CONFIG->getFriendGiftCfg().IsReqFriendHeli(type, param1))
		{
			return;
		}

		pro.param1 = param1;
	}
	else if (Protocol::SC_FRIEND_HELI_SKILL_BOSS_FETCH_EQUI_REQ == type)
	{	
		const ItemBase *item_base = ITEMPOOL->GetItem(param2);
		if (NULL == item_base || ItemBase::I_TYPE_EQUIPMENT != item_base->GetItemType())
		{
			return;
		}

		Equipment *equip = (Equipment *)item_base;
		if (!LOGIC_CONFIG->getFriendGiftCfg().IsReqFriendHeli(type, equip->GetColor()))
		{
			return;
		}

		pro.param1 = param1;
		pro.param2 = param2;
	}

	common_data->last_send_heli_notice_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	static FriendListParam friend_list_param; 
	friend_list_param.count = 0;
	m_role->GetFriendManager()->GetFriendList(&friend_list_param);
	
	memset(m_friend_heli_list, 0, sizeof(m_friend_heli_list));
	for (int i = 0; i < friend_list_param.count && i < MAX_FRIEND_NUM; ++ i)
	{
		Role * friend_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(friend_list_param.friend_list[i].friend_uid));
		if (NULL != friend_role)
		{
			m_friend_heli_list[i] = friend_role->GetUID();
			EngineAdapter::Instance().NetSend(friend_role->GetNetId(), (const char *)&pro, sizeof(pro));
		}
	}
}

void FriendGiftManager::OnHeliRet(int uid, int type)
{
	Role * role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (NULL != role && role->GetFriendGiftManager()->IsCanSendHeli(m_role->GetUID()))
	{
		long long heli_exp = LOGIC_CONFIG->getFriendGiftCfg().GetOtherCfg().heli_exp;
		role->AddExp((role->GetLevel() + 50) * heli_exp, "heli_exp", Role::EXP_ADD_REASON_EXP_FRIEND_HELI);

		long long self_add_exp = (m_role->GetLevel() + 50) * heli_exp;
		m_role->AddExp(self_add_exp, "heli_exp", Role::EXP_ADD_REASON_EXP_FRIEND_HELI);

		int length = 0;
		if (1 == type)
		{
			length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_friend_heli_egg_content,
				UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(), self_add_exp);
		}
		else if (2 == type)
		{
			length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_friend_heli_songhua_content,
				UidToInt(role->GetUserId()), role->GetName(), (int)role->GetCamp(), self_add_exp);
		}

		if (length > 0)
		{
			static Protocol::SCSystemMsg sm;
			int sendlen = 0;
			if (SceneManager::CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_PERSONAL_NOTICE))
			{	
				if (sendlen > 0)
				{
					EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char*)&sm, sendlen);
				}
			}
		}

		static Protocol::SCFriendHeliSend pro;
		pro.uid = m_role->GetUID();
		pro.type = type;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&pro, sizeof(pro));
	}
}

bool FriendGiftManager::IsCanSendHeli(int uid)
{
	for (int i = 0; i < MAX_FRIEND_NUM; i++)
	{
		if (m_friend_heli_list[i] == uid)
		{
			m_friend_heli_list[i] = 0;
			return true;
		}
	}

	return false;
}

void FriendGiftManager::OnGiveItemReq(Protocol::CSGiveItemReq *req)
{
	if (nullptr == req) return;

	if (req->target_uid <= 0)
	{
		gstr::SendError(m_role, "en_target_empty");
		return;
	}
	
	// 去重背包index
	std::set<int> bag_index_set;
	unsigned int item_count_unique = 0;
	Protocol::CSGiveItemReq::ItemInfo give_item_list_unique[GIVE_ITEM_MAX_COUNT];
	for (unsigned int i = 0; i < (unsigned int)req->item_count && i < static_array_size(req->give_item_list); i++)
	{
		int bag_index = req->give_item_list[i].index;
		if (bag_index_set.find(bag_index) != bag_index_set.end())
		{
			continue;
		}

		if (item_count_unique < 0 || item_count_unique >= GIVE_ITEM_MAX_COUNT)
		{
			break;
		}

		bag_index_set.insert(bag_index);

		give_item_list_unique[item_count_unique].index = bag_index;
		give_item_list_unique[item_count_unique].num = req->give_item_list[i].num;

		item_count_unique++;
	}

	// 检查物品是否够
	unsigned int consume_count = 0;
	static ItemDataWrapper consume_item_list[GIVE_ITEM_MAX_COUNT]; memset(consume_item_list, 0, sizeof(consume_item_list));
	for (unsigned int i = 0; i < item_count_unique && i < static_array_size(give_item_list_unique); i ++)
	{
		int bag_index = give_item_list_unique[i].index;
		int num = give_item_list_unique[i].num;
		if (!IsCanGive(bag_index, num))
		{
			return;
		}

		if (consume_count >= 0 && consume_count < static_array_size(consume_item_list))
		{
			ItemGridData grid_data;
			if (m_role->GetKnapsack()->GetItemGridData(bag_index, &grid_data))
			{
				grid_data.WrapTo(&consume_item_list[consume_count]);
				consume_item_list[consume_count].num = num;

				consume_count++;
			}
		}
	}

	// 消耗
	for (unsigned int i = 0; i < item_count_unique && i < static_array_size(give_item_list_unique); i++)
	{
		int bag_index = give_item_list_unique[i].index;
		int num = give_item_list_unique[i].num;

		if (!m_role->GetKnapsack()->ConsumeItemByIndex(bag_index, num, "FriendGiftManager::OnGiveItemReq"))
		{
			m_role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return;
		}
	}

	// 赠送发邮件
	gamecommon::SendMultiMailToRole(IntToUid(req->target_uid), consume_item_list, consume_count, false, "give_item_mail",
		m_role->GetName(), (int)m_role->GetSex());

	// 记录
	GameName my_name; memset(my_name, 0, sizeof(my_name));
	GameName target_name; memset(target_name, 0, sizeof(target_name));
	m_role->GetName(my_name);

	UserCacheNode *target_node = UserCacheManager::Instance().GetUserNode(req->target_uid);
	if (NULL != target_node)
	{
		target_node->GetName(target_name);
	}

	ItemGiftInfo info;
	unsigned int now_s = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	for (unsigned int i = 0; i < consume_count && i < static_array_size(consume_item_list); i ++)
	{
		info.Reset();
		info.uid = req->target_uid;
		F_STRNCPY(info.role_name, target_name, sizeof(info.role_name));
		info.item_id = consume_item_list[i].item_id;
		info.item_num = consume_item_list[i].num;
		info.timestamp = now_s;

		CommonDataGlobal *common_global = GlobalRoleManager::Instance().GetCommonDataGlobal(m_role->GetUID());
		if (nullptr != common_global)
		{
			common_global->AddGiveItemRecord(info, true);
		}

		CommonDataGlobal *target_common_global = GlobalRoleManager::Instance().GetCommonDataGlobal(req->target_uid);
		if (nullptr != target_common_global)
		{
			info.uid = m_role->GetUID();
			F_STRNCPY(info.role_name, my_name, sizeof(info.role_name));

			target_common_global->AddGiveItemRecord(info, false);
		}
	}

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(req->target_uid));
	if (NULL != target_role)
	{
		this->SendGiveItemInfo(target_role, false);
	}

	this->SendGiveItemInfo(m_role, true);

	gstr::SendNotice(m_role, "give_succ");
}

bool FriendGiftManager::IsCanGive(int bag_index, int num)
{
	ItemGridData grid_data;
	if (!m_role->GetKnapsack()->GetItemGridData(bag_index, &grid_data))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_LOAD_ERROR);
		return false;
	}

	if (!ItemBase::CanTrade(&grid_data))
	{
		m_role->NoticeNum(errornum::EN_ITEM_CANNOT_TRADE);
		return false;
	}

	if (num <= 0 || grid_data.num < num)
	{
		m_role->NoticeNum(errornum::EN_ITEM_NOT_ENOUGH);
		return false;
	}

	if (grid_data.is_bind)
	{
		gstr::SendError(m_role, "nobind_item_limit");
		return false;
	}

	return true;
}

void FriendGiftManager::SendGiveItemInfo(Role *role, bool is_give)
{
	if (NULL == role) return;

	CommonDataGlobal *common_global = GlobalRoleManager::Instance().GetCommonDataGlobal(role->GetUID());
	if (nullptr != common_global)
	{
		common_global->SendGiveItemRecord(role, is_give);
	}
}

