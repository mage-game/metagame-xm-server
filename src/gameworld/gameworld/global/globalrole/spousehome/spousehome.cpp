#include "spousehome.hpp"
#include "servercommon/spousehomedef.hpp"
#include "config/logicconfigmanager.hpp"
#include "obj/character/role.h"
#include "spousehomeconfig.hpp"
#include "other/capability/capability.hpp"
#include "item/knapsack.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "world.h"
#include "global/globalrole/globalrolemanager.hpp"
#include "config/joinlimitconfig.hpp"
#include "item/itempool.h"
#include "item/itembase.h"
#include "global/globalrole/litttlepet/littlepet.hpp"
#include "friend/friendmanager.h"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/usercache/usercache.hpp"
#include "gamecommon.h"
#include "gameworld/scene/activityshadow/activityshadow.hpp"
#include "other/event/eventhandler.hpp"

SpouseHome::SpouseHome(GlobalRole *g_role) : IGlobalRoleItem(g_role)
{
}

SpouseHome::~SpouseHome()
{
}

void SpouseHome::Init(const SpouseHomeParam &param)
{
	m_param = param;
}

void SpouseHome::GetInit(SpouseHomeParam &param) const
{
	param = m_param;
}

void SpouseHome::SetLoverId(int lover_id, bool is_calc)
{
	Role * role = this->GetRole();
	if (nullptr == role) return;

	if (m_param.lover_id != lover_id)
	{
		m_param.lover_id = lover_id;
		m_param.is_cap_upgrade = 0;

		this->CheckCoupleRoomAddCapPer();

		this->OnDataChange();
	}

	if (is_calc)
	{
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SPOUSE_HOME);
	}
}

bool SpouseHome::GetTargetSpouseHomeInfo(SpouseHomeRoomParam *target_room_info, int length)
{
	if (nullptr == target_room_info || length <= 0)
	{
		return false;
	}

	int i = 0;
	for (; i < SPOUSE_HOME_MAX_ROOM_NUM && i < length; ++i)
	{
		if (m_param.room_list[i].is_open > 0)
		{
			target_room_info[i] = m_param.room_list[i];
		}
	}

	if (i > 0)
	{
		return true;
	}

	return false;
}

void SpouseHome::OnRoleLogin(Role * role)
{
	if (nullptr == role)
	{
		return;
	}

	if (m_param.lover_id != role->GetLoverUid())
	{
		this->SetLoverId(role->GetLoverUid());
	}

	if (0 == m_param.has_check_shared_attr_flag)
	{
		m_param.has_check_shared_attr_flag = 1;
		this->CheckCoupleRoomAddCapPer(true);
	}

	this->OnDataChange();
	this->SendAllInfo();
}

void SpouseHome::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc, Role *role)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		this->GetTotalAttr(m_attrs_add);

		SpouseHome * target_info = GLOBAL_ROLE_MGR.GetSpouseHomeGlobal(this->GetLoverId());
		if (nullptr != target_info)
		{
			CharIntAttrs tmp_base_add;
			target_info->GetTotalAttr(tmp_base_add, true);
			m_attrs_add.Add(tmp_base_add.m_attrs);
		}
	}

	base_add.Add(m_attrs_add.m_attrs, true);
}

void SpouseHome::ReqOpera(int info_type, int param1, int param2, int param3, int param4)
{
	switch (info_type)
	{
	case Protocol::CS_SPOUSE_HOME_TYPE_ALL_INFO:					// 请求所有信息
	{
		this->SendAllInfo();
	}
	break;

	case Protocol::CS_SPOUSE_HOME_TYPE_FURNITURE_EQUIT:				// 装备
	{
		this->SpouseHomeEquip(param1, param2, param3);
	}
	break;

	case Protocol::CS_SPOUSE_HOME_TYPE_FURNITURE_TAKE_OFF:			// 拆下
	{
		this->SpouseHomeTakeOff(param1, param2);
	}
	break;

	case Protocol::CS_SPOUSE_HOME_TYPE_VIEW_OTHER_PEOPLE_ROOM:		// 查看他人房间
	{
		this->SendViewOtherPeopleRoom(param1);
	}
	break;

	case Protocol::CS_SPOUSE_HOME_TYPE_BUY_THEME:					// 购买房间
	{
		this->OnBuySpecialRoom(param1);
	}
	break;

	case Protocol::CS_SPOUSE_HOME_TYPE_BUY_FURNITURE_ITEM:			// 购买道具
	{
		this->OnBuyFurniture(param1, param2);
	}
	break;

	case Protocol::CS_SPOUSE_HOME_TYPE_FRIEND_LIST_INFO:
	{
		this->SendFriendCountInfo();
	}
	break;

	case Protocol::CS_SPOUSE_HOME_TYPE_GUILD_MEMBER_INFO:
	{
		this->SendGuildMemberInfo();
	}
	break;

	case Protocol::CS_SPOUSE_HOME_TYPE_BUY_THEME_FOR_LOVER:
	{
		this->OnBuySpecialRoomForLover(param1, param2);
	}
	break;

	case Protocol::CS_SPOUSE_HOME_TYPE_FURNITURE_EQUIT_FOR_LOVER:
	{
		this->SpouseHomeEquipForLover(param1, param2, param3, param4);
	}
	break;

	default:
		break;
	}
}

void SpouseHome::SendAllInfo()
{
	this->SendSpouseHomeInfo();
}

void SpouseHome::GetTotalAttr(CharIntAttrs &tmp_attr, bool is_spouse)
{
	const SpouseHomeOtherCfg &other_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetOtherCfg();
	int add_cap_shared_per = other_cfg.spouse_capability_shared_per;

	if (m_param.is_cap_upgrade > 0)
	{
		add_cap_shared_per += other_cfg.together_have_x_room_cap_add;
	}

	for (int index = 0; index < SPOUSE_HOME_MAX_ROOM_NUM && index < other_cfg.room_limit_count; ++index)
	{
		const SpouseHomeRoomParam &room = m_param.room_list[index];
		if (room.is_open > 0)
		{
			for (int i = 0; i < SPOUSE_HOME_FURNITURE_MAX_ITEM_SLOT && i < other_cfg.max_equip_item_count; ++i)
			{
				const ItemID &slot_furniture_id = room.furniture_slot_list[i];
				if (slot_furniture_id <= 0)
				{
					continue;
				}

				const int combin_item_id = LOGIC_CONFIG->GetSpouseHomeConfig().GetCombinItemId(slot_furniture_id, i);

				//furniture_base_attr
				AttrConfig tmp_attr_cfg; tmp_attr_cfg.Reset();
				const auto furn_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetFurnitureBaseAttr(combin_item_id);
				if (nullptr != furn_cfg)
				{
					if (is_spouse)
					{
						tmp_attr_cfg.Add(*furn_cfg, add_cap_shared_per * ROLE_ATTR_PER);
					}
					else
					{
						tmp_attr_cfg.Add(*furn_cfg, 1);
					}
				}

				tmp_attr.AddAttrFromConfig(tmp_attr_cfg);
			}

			// room_attr
			const AttrConfig *room_attr = LOGIC_CONFIG->GetSpouseHomeConfig().GetRoomAttrCfg(room.special_theme);
			if (nullptr != room_attr)
			{
				AttrConfig tmp_attr_cfg; tmp_attr_cfg.Reset();
				if (is_spouse)
				{
					tmp_attr_cfg.Add(*room_attr, add_cap_shared_per * ROLE_ATTR_PER);
				}
				else
				{
					tmp_attr_cfg.Add(*room_attr, 1);
				}

				tmp_attr.AddAttrFromConfig(tmp_attr_cfg);
			}
		}
	}

	Role *role = this->GetRole();
	if (nullptr != role)
	{
		role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_SPOUSEHOME, m_attrs_add);
	}
}

void SpouseHome::OnBuyFurniture(ItemID item_id, int item_num)
{
	const int buy_need_gold = LOGIC_CONFIG->GetSpouseHomeConfig().GetFurnitureShopCfg(item_id);
	if (buy_need_gold <= 0)
	{
		return;
	}

	Role *role = this->GetRole();
	if (nullptr == role)
	{
		return;
	}

	const ItemBase *item_base = ITEMPOOL->GetItem(item_id);
	if (nullptr == item_base || item_base->GetPileLimit() <= 0)
	{
		return;
	}

	int get_num = item_num;
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DISCOUNT_BUY_FURNITURE))
	{
		get_num *= 2;
	}

	int num = get_num / item_base->GetPileLimit();
	if ((get_num % item_base->GetPileLimit()) > 0)
	{
		++num;
	}

	
	if (!role->GetKnapsack()->CheckEmptyGridNoLessThan(num))
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->GoldMoreThan(buy_need_gold * item_num))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (!role->GetKnapsack()->GetMoney()->UseGold(buy_need_gold * item_num, "SpouseHome::OnBuyFurniture"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	ItemConfigData item_data;
	item_data.item_id = item_id;
	item_data.num = get_num;
	item_data.is_bind = 0;

	role->GetKnapsack()->Put(item_data, PUT_REASON_SPOUSE_HOME_BUY);

	role->NoticeNum(noticenum::NT_SHOP_BUY_SUCC);
}

void SpouseHome::SpouseHomeEquip(int room_index, int furniture_type, int knapsack_index)
{
	const SpouseHomeOtherCfg & other_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetOtherCfg();

	Role *role = this->GetRole();
	if (nullptr == role)
	{
		return;
	}

	const ItemGridData* item_data = role->GetKnapsack()->GetItemGridData(knapsack_index);
	if (nullptr == item_data)
	{
		return;
	}

	if (room_index < 0 || room_index >= SPOUSE_HOME_MAX_ROOM_NUM || room_index >= other_cfg.room_limit_count)
	{
		return;
	}

	if (furniture_type < 0 || furniture_type >= other_cfg.max_equip_item_count || furniture_type >= SPOUSE_HOME_FURNITURE_MAX_ITEM_SLOT)
	{
		return;
	}

	if (m_param.room_list[room_index].is_open <= 0)
	{
		return;
	}

	int item_id = item_data->item_id;

	const int combin_item_id = LOGIC_CONFIG->GetSpouseHomeConfig().GetCombinItemId(item_id, furniture_type);

	const auto furn_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetFurnitureBaseAttr(combin_item_id);
	if (nullptr == furn_cfg)
	{
		return;
	}

	ItemID &slot_furniture_id = m_param.room_list[room_index].furniture_slot_list[furniture_type];

	if (slot_furniture_id != 0)
	{
		if (!this->SpouseHomeTakeOff(room_index, furniture_type, true) || slot_furniture_id != 0)
		{
			return;
		}
	}

	if (!role->GetKnapsack()->ConsumeItem(item_data->item_id, 1, "SpouseHome::SpouseHomeEquip"))
	{
		role->NoticeNum(errornum::EN_KNAPSACK_USE_ITEM_ERROR);
		return;
	}

	slot_furniture_id = item_id;

	this->OnDataChange();

	this->SendSpouseHomeSingleRoomInfo(room_index);

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SPOUSE_HOME);

	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(this->GetLoverId()));
	if (nullptr != lover_role)
	{
		lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SPOUSE_HOME);
	}
}

bool SpouseHome::SpouseHomeTakeOff(int room_index, int furniture_type, bool is_replacce)
{
	const SpouseHomeOtherCfg & other_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetOtherCfg();

	Role *role = this->GetRole();
	if (nullptr == role)
	{
		return false;
	}

	if (room_index < 0 || room_index >= SPOUSE_HOME_MAX_ROOM_NUM ||
		furniture_type < 0 || furniture_type >= other_cfg.max_equip_item_count || furniture_type >= SPOUSE_HOME_FURNITURE_MAX_ITEM_SLOT)
	{
		return false;
	}

	ItemID &slot_item_id = m_param.room_list[room_index].furniture_slot_list[furniture_type];
	if (0 >= slot_item_id)
	{
		return false;
	}

	const int combin_item_id = LOGIC_CONFIG->GetSpouseHomeConfig().GetCombinItemId(slot_item_id, furniture_type);

	const auto furn_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetFurnitureBaseAttr(combin_item_id);
	if (nullptr == furn_cfg)
	{
		return false;
	}

	if (!role->GetKnapsack()->HaveSpace())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return false;
	}

	ItemConfigData item_data;
	item_data.item_id = slot_item_id;
	item_data.num = 1;
	item_data.is_bind = 0;

	slot_item_id = 0;

	role->GetKnapsack()->Put(item_data, PUT_REASON_NO_NOTICE);

	this->OnDataChange();

	if (!is_replacce)
	{
		this->SendSpouseHomeSingleRoomInfo(room_index);
	}

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SPOUSE_HOME);

	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(this->GetLoverId()));
	if (nullptr != lover_role)
	{
		lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SPOUSE_HOME);
	}

	return true;
}

void SpouseHome::OnBuySpecialRoom(int buy_theme_index)
{
	const SpouseHomeOtherCfg &other_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetOtherCfg();

	Role *role = this->GetRole();
	if (nullptr == role)
	{
		return;
	}

	if (m_param.open_room_count >= other_cfg.room_limit_count || m_param.open_room_count >= SPOUSE_HOME_MAX_ROOM_NUM)
	{
		role->NoticeNum(errornum::EN_SPOUSE_HOME_ROOM_BUY_LIMIT);
		return;
	}

	const RoomThemeCfg * room_theme_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetSpecialThemeCfg(buy_theme_index);
	if (nullptr == room_theme_cfg)
	{
		return;
	}

	int nee_gold = 0;
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DISCOUNT_BUY_HOME))
	{
		nee_gold = room_theme_cfg->need_gold * LOGIC_CONFIG->GetSpouseHomeConfig().GetOtherCfg().discount_rand_activiy / 100;
	}
	else
	{
		nee_gold = room_theme_cfg->need_gold;
	}
		

	if (room_theme_cfg->price_type == 0 && !role->GetKnapsack()->GetMoney()->AllGoldMoreThan(nee_gold))
	{
		role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
		return;
	}

	if (room_theme_cfg->price_type == 0 && !role->GetKnapsack()->GetMoney()->UseAllGold(nee_gold, "SpouseHome::OnBuySpecialTheme"))
	{
		role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
		return;
	}

	// 消耗元宝
	if (room_theme_cfg->price_type == 1 && !role->GetKnapsack()->GetMoney()->GoldMoreThan(nee_gold))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (room_theme_cfg->price_type == 1 && !role->GetKnapsack()->GetMoney()->UseGold(nee_gold, "SpouseHome::OnBuySpecialTheme"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	m_param.room_list[m_param.open_room_count].is_open = 1;
	m_param.room_list[m_param.open_room_count].special_theme = buy_theme_index;


	// 发送房间信息
	this->SendSpouseHomeSingleRoomInfo(m_param.open_room_count);

	++m_param.open_room_count;

	this->CheckCoupleRoomAddCapPer();
	this->OnDataChange();

	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SPOUSE_HOME);

	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(this->GetLoverId()));
	if (nullptr != lover_role)
	{
		lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SPOUSE_HOME);
	}

	//江湖传闻
	GuildID guild_id = role->GetGuildID();
	if (INVALID_GUILD_ID != guild_id)
	{
		EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(),NULL, GUILD_EVENT_TYPE_RUMOR_BUY_HOUSE, buy_theme_index);
	}
}

void SpouseHome::SendSpouseHomeInfo() const
{
	const SpouseHomeOtherCfg &other_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetOtherCfg();

	static Protocol::SCSpouseHomeRoomInfo psibi;
	psibi.room_cout = 0;
	psibi.uid = this->m_role_id;
	psibi.pet_id = 0;
	LittlePet * target = GLOBAL_ROLE_MGR.GetLittlePet(this->m_role_id);
	if (nullptr != target)
	{
		psibi.pet_id = target->GetPetId(0);
	}

	psibi.room_furniture_limit = other_cfg.max_equip_item_count;

	for (int index = 0; index < SPOUSE_HOME_MAX_ROOM_NUM && index < other_cfg.room_limit_count; ++index)
	{
		if (m_param.room_list[index].is_open > 0)
		{
			const auto &room = m_param.room_list[index];
			auto &room_msg = psibi.room_msg[index];

			for (int type = 0; type < SPOUSE_HOME_FURNITURE_MAX_ITEM_SLOT && type < other_cfg.max_equip_item_count; ++type)
			{
				room_msg.furniture_slot_list[type] = room.furniture_slot_list[type];
			}

			room_msg.room_index = index;
			room_msg.theme_type = room.special_theme;
			++psibi.room_cout;
		}
		else
		{
			break;
		}
	}

	Role *role = this->GetRole();
	if (nullptr == role)
	{
		return;
	}

	const auto length = sizeof(psibi) - sizeof(psibi.room_msg[0]) * (SPOUSE_HOME_MAX_ROOM_NUM - psibi.room_cout);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&psibi, length);
}

void SpouseHome::SendViewOtherPeopleRoom(int uid)
{
	if (uid <= 0)
	{
		return;
	}

	const SpouseHomeOtherCfg &other_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetOtherCfg();

	SpouseHome *target_spouse_home = GLOBAL_ROLE_MGR.GetSpouseHomeGlobal(uid);
	if (nullptr == target_spouse_home)
	{
		return;
	}

	SpouseHomeRoomParam target_room_info_list[SPOUSE_HOME_MAX_ROOM_NUM];
	bool ret = target_spouse_home->GetTargetSpouseHomeInfo(target_room_info_list, SPOUSE_HOME_MAX_ROOM_NUM);
	if (!ret)
	{
		return;
	}

	static Protocol::SCSpouseHomeRoomInfo psibi;
	psibi.room_cout = 0;
	psibi.uid = uid;
	psibi.pet_id = 0;
	LittlePet * target = GLOBAL_ROLE_MGR.GetLittlePet(uid);
	if (nullptr != target)
	{
		psibi.pet_id = target->GetPetId(0);
	}
	psibi.room_furniture_limit = other_cfg.max_equip_item_count;

	for (int index = 0; index < SPOUSE_HOME_MAX_ROOM_NUM && index < other_cfg.room_limit_count; ++index)	// 遍历房间
	{
		if (target_room_info_list[index].is_open > 0)
		{
			const auto &room = target_room_info_list[index];		// 拿到房间数据
			auto &room_msg = psibi.room_msg[index];					// 协议
			room_msg.theme_type = room.special_theme;				// 房间主题

			for (int type = 0; type < SPOUSE_HOME_FURNITURE_MAX_ITEM_SLOT && type < other_cfg.max_equip_item_count; ++type)		// 遍历房间内道具
			{
				room_msg.furniture_slot_list[type] = room.furniture_slot_list[type];
			}

			room_msg.room_index = index;
			++psibi.room_cout;
		}
		else
		{
			break;
		}
	}

	Role *role = this->GetRole();
	if (nullptr == role)
	{
		return;
	}

	const auto length = sizeof(psibi) - sizeof(psibi.room_msg[0]) * (SPOUSE_HOME_MAX_ROOM_NUM - psibi.room_cout);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&psibi, length);
}

void SpouseHome::SendFriendCountInfo()
{
	static Protocol::SCSpouseHomeFirendInfo scshoi;
	scshoi.firend_count = 0;

	Role *role = this->GetRole();
	if (nullptr == role)
	{
		return;
	}

	FriendManager *friend_manager = role->GetFriendManager();
	if (nullptr != friend_manager)
	{
		int count = 0;
		int friend_list[MAX_FRIEND_NUM];
		count = friend_manager->GetFriendUidList(friend_list, MAX_FRIEND_NUM);

		for (int i = 0; i < count && i < MAX_FRIEND_NUM; ++i)
		{
			if (role->GetUID() != friend_list[i])
			{
				const UserCacheNode * user_cache = UserCacheManager::Instance().GetUserNode(friend_list[i]);
				if (nullptr != user_cache)
				{
					SpouseHome *target = GLOBAL_ROLE_MGR.GetSpouseHomeGlobal(user_cache->uid);
					if (nullptr == target)
					{
						continue;
					}

					int ret_room_count = target->GetTargetSpouseHomeRoomCount();
					if (ret_room_count <= 0)
					{
						continue;
					}

					scshoi.firend_info_list[scshoi.firend_count].uid = user_cache->uid;
					scshoi.firend_info_list[scshoi.firend_count].room_count = ret_room_count;
					scshoi.firend_info_list[scshoi.firend_count].sex = user_cache->sex;
					scshoi.firend_info_list[scshoi.firend_count].prof = user_cache->prof;
					user_cache->GetName(scshoi.firend_info_list[scshoi.firend_count].name);

					++scshoi.firend_count;
				}
			}
		}
	}

	const auto length = sizeof(scshoi) - sizeof(scshoi.firend_info_list[0]) * (MAX_FRIEND_NUM - scshoi.firend_count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&scshoi, length);
}

void SpouseHome::SendGuildMemberInfo()
{
	Role *role = this->GetRole();
	if (nullptr == role)
	{
		return;
	}

	static Protocol::SCSpouseHomeGuildMemberInfo scshgmi;
	scshgmi.guild_member_count = 0;

	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (nullptr != guild)
	{
		int count = 0;
		int guild_member_uid_list[MAX_MEMBER_COUNT];
		GuildMemberManager &guild_member_manager = guild->GetGuildMemberManager();
		count = guild_member_manager.GetMemberList(guild_member_uid_list);

		for (int i = 0; i < count && i < MAX_MEMBER_COUNT; ++i)
		{
			if (role->GetUID() != guild_member_uid_list[i])
			{
				const UserCacheNode * user_cache = UserCacheManager::Instance().GetUserNode(guild_member_uid_list[i]);
				if (nullptr != user_cache)
				{
					SpouseHome *target = GLOBAL_ROLE_MGR.GetSpouseHomeGlobal(user_cache->uid);
					if (nullptr == target)
					{
						continue;
					}

					int ret_room_count = target->GetTargetSpouseHomeRoomCount();
					if (ret_room_count <= 0)
					{
						continue;
					}

					scshgmi.guild_member_info_list[scshgmi.guild_member_count].uid = user_cache->uid;
					scshgmi.guild_member_info_list[scshgmi.guild_member_count].room_count = ret_room_count;
					scshgmi.guild_member_info_list[scshgmi.guild_member_count].sex = user_cache->sex;
					scshgmi.guild_member_info_list[scshgmi.guild_member_count].prof = user_cache->prof;
					user_cache->GetName(scshgmi.guild_member_info_list[scshgmi.guild_member_count].name);

					++scshgmi.guild_member_count;
				}
			}
		}
	}

	const auto length = sizeof(scshgmi) - sizeof(scshgmi.guild_member_info_list[0]) * (MAX_MEMBER_COUNT - scshgmi.guild_member_count);
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&scshgmi, length);
}

void SpouseHome::SendSpouseHomeSingleRoomInfo(int room_index)
{
	const SpouseHomeOtherCfg &other_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetOtherCfg();

	if (room_index >= 0 && room_index < other_cfg.room_limit_count && room_index < SPOUSE_HOME_MAX_ROOM_NUM)
	{
		static Protocol::SCSpouseHomeSingleRoomInfo scshsri;
		scshsri.pet_id = 0;

		const auto &room = m_param.room_list[room_index];

		LittlePet *target = GLOBAL_ROLE_MGR.GetLittlePet(this->m_role_id);
		if (nullptr != target)
		{
			scshsri.pet_id = target->GetPetId(0);
		}

		scshsri.room_furniture_limit = other_cfg.max_equip_item_count;
		scshsri.room_msg.room_index = room_index;
		scshsri.room_msg.theme_type = room.special_theme;

		for (int type = 0; type < SPOUSE_HOME_FURNITURE_MAX_ITEM_SLOT && type < other_cfg.max_equip_item_count; ++type)
		{
			scshsri.room_msg.furniture_slot_list[type] = room.furniture_slot_list[type];
		}

		Role *role = this->GetRole();
		if (nullptr == role)
		{
			return;
		}

		SEND_TO_ROLE(role, scshsri);
	}
}

void SpouseHome::CheckCoupleRoomAddCapPer(bool is_need_recalc)
{
	char old_is_cap_upgrade = m_param.is_cap_upgrade;

	SpouseHome *target_spouse_home = GLOBAL_ROLE_MGR.GetSpouseHomeGlobal(m_param.lover_id);
	if (nullptr != target_spouse_home)
	{
		SpouseHomeRoomParam target_room_info_list[SPOUSE_HOME_MAX_ROOM_NUM];
		bool ret = target_spouse_home->GetTargetSpouseHomeInfo(target_room_info_list, SPOUSE_HOME_MAX_ROOM_NUM);
		if (ret)
		{
			int room_count = 0, spouse_room_count = 0;
			for (int index = 0; index < SPOUSE_HOME_MAX_ROOM_NUM; ++index)
			{
				if (m_param.room_list[index].is_open > 0)
				{
					++room_count;
				}

				if (target_room_info_list[index].is_open > 0)
				{
					++spouse_room_count;
				}
			}

			const SpouseHomeOtherCfg &other_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetOtherCfg();
			if (room_count >= other_cfg.together_have_x_room && spouse_room_count >= other_cfg.together_have_x_room)
			{
				m_param.is_cap_upgrade = 1;
				target_spouse_home->m_param.is_cap_upgrade = 1;
			}

			if (old_is_cap_upgrade != m_param.is_cap_upgrade && is_need_recalc)
			{
				Role *role = this->GetRole();
				if (role != nullptr)
				{
					role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SPOUSE_HOME);
				}
			}
		}
	}
}

void SpouseHome::OnBuySpecialRoomForLover(int lover_id, int buy_theme_index)
{
	Role *role = this->GetRole();
	if (role == nullptr)
	{
		return;
	}

	if (buy_theme_index < 0 || buy_theme_index >= ROOM_SPECIAL_ROOM_THEME_MAX_CFG_COUNT)
	{
		return;
	}

	if (this->GetLoverId() == 0 || this->GetLoverId() != lover_id) // 只能给伴侣购买
	{
		role->NoticeNum(errornum::EN_SPOUSE_HOME_ONLY_CAN_BUY_FOR_LOVER);
		return;
	}

	SpouseHome *lover_spouse_home = GLOBAL_ROLE_MGR.GetSpouseHomeGlobal(this->GetLoverId()); // 伴侣的家园
	if (lover_spouse_home == nullptr)
	{
		return;
	}

	int ret_room_count = lover_spouse_home->GetTargetSpouseHomeRoomCount();                // 伴侣的房间数量
	const SpouseHomeOtherCfg &other_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetOtherCfg();

	if (ret_room_count >= other_cfg.room_limit_count || ret_room_count >= SPOUSE_HOME_MAX_ROOM_NUM)   // 伴侣的房间数量已达上限
	{
		role->NoticeNum(errornum::EN_SPOUSE_HOME_LOVER_ROOM_BUY_LIMIT);
		return;
	}

	const RoomThemeCfg * room_theme_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetSpecialThemeCfg(buy_theme_index);
	if (nullptr == room_theme_cfg)
	{
		return;
	}

	int nee_gold = 0;
	if (ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_DISCOUNT_BUY_HOME))
	{
		nee_gold = room_theme_cfg->need_gold * LOGIC_CONFIG->GetSpouseHomeConfig().GetOtherCfg().discount_rand_activiy / 100;
	}
	else
	{
		nee_gold = room_theme_cfg->need_gold;
	}

	if (room_theme_cfg->price_type == 0 && !role->GetKnapsack()->GetMoney()->AllGoldMoreThan(nee_gold))
	{
		role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
		return;
	}

	if (room_theme_cfg->price_type == 0 && !role->GetKnapsack()->GetMoney()->UseAllGold(nee_gold, "SpouseHome::OnBuySpecialRoomForLover"))
	{
		role->NoticeNum(errornum::EN_BIND_GOLD_NOT_ENOUGH);
		return;
	}

	// 消耗元宝
	if (room_theme_cfg->price_type == 1 && !role->GetKnapsack()->GetMoney()->GoldMoreThan(nee_gold))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	if (room_theme_cfg->price_type == 1 && !role->GetKnapsack()->GetMoney()->UseGold(nee_gold, "SpouseHome::OnBuySpecialRoomForLover"))
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	lover_spouse_home->m_param.room_list[ret_room_count].is_open = 1;
	lover_spouse_home->m_param.room_list[ret_room_count].special_theme = buy_theme_index;
	++lover_spouse_home->m_param.open_room_count;

	lover_spouse_home->OnDataChange();
	lover_spouse_home->CheckCoupleRoomAddCapPer();
	lover_spouse_home->SendSpouseHomeSingleRoomInfo(ret_room_count);

	this->CheckCoupleRoomAddCapPer();
	this->OnDataChange();
	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SPOUSE_HOME);

	role->NoticeNum(noticenum::NT_SPOUSE_HOME_LOVER_ROOM_BUY_SUCC);
	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(this->GetLoverId()));
	if (lover_role != nullptr)
	{
		lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SPOUSE_HOME);
	}

	// 邮件提醒
	static MailContentParam contentparam; contentparam.Reset();
	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_spousehome_buy_room_for_lover, role->GetUID(), role->GetName(), (int)role->GetCamp(), buy_theme_index);
	if (length > 0)
	{
		MailRoute::MailToUser(IntToUid(this->GetLoverId()), SYSTEM_MAIL_REASON_INVALID, contentparam);
	}

	//江湖传闻
	GuildID guild_id = role->GetGuildID();
	if (INVALID_GUILD_ID != guild_id)
	{
		EventHandler::Instance().OnAddGuildRumorEvent(guild_id, role->GetName(), NULL, GUILD_EVENT_TYPE_RUMOR_BUY_HOUSE, buy_theme_index);
	}
}

void SpouseHome::SpouseHomeEquipForLover(int lover_id, int room_index, int furniture_type, int knapsack_index)
{
	Role *role = this->GetRole();
	if (role == nullptr)
	{
		return;
	}

	const SpouseHomeOtherCfg &other_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetOtherCfg();

	if (room_index < 0 || room_index >= SPOUSE_HOME_MAX_ROOM_NUM || room_index >= other_cfg.room_limit_count)
	{
		return;
	}

	if (furniture_type < 0 || furniture_type >= other_cfg.max_equip_item_count || furniture_type >= SPOUSE_HOME_FURNITURE_MAX_ITEM_SLOT)
	{
		return;
	}

	if (this->GetLoverId() == 0 || this->GetLoverId() != lover_id) // 只能给伴侣装备
	{
		role->NoticeNum(errornum::EN_SPOUSE_HOME_ONLY_CAN_EQUIP_FOR_LOVER);
		return;
	}

	SpouseHome *lover_spouse_home = GLOBAL_ROLE_MGR.GetSpouseHomeGlobal(this->GetLoverId()); // 伴侣的家园
	if (lover_spouse_home == nullptr)
	{
		return;
	}

	SpouseHomeRoomParam lover_room_info_list[SPOUSE_HOME_MAX_ROOM_NUM];
	bool ret = lover_spouse_home->GetTargetSpouseHomeInfo(lover_room_info_list, SPOUSE_HOME_MAX_ROOM_NUM);
	if (!ret) return;
	if (lover_room_info_list[ room_index].is_open <= 0)     // 尚未开启
	{
		role->NoticeNum(errornum::EN_SPOUSE_HOME_LOVER_ROOM_NO_OPEN);
		return;
	}

	const ItemGridData* item_data = role->GetKnapsack()->GetItemGridData(knapsack_index);
	if (nullptr == item_data || item_data->Invalid()) return;

	int item_id = item_data->item_id;

	const int combin_item_id = LOGIC_CONFIG->GetSpouseHomeConfig().GetCombinItemId(item_id, furniture_type);
	const auto furn_cfg = LOGIC_CONFIG->GetSpouseHomeConfig().GetFurnitureBaseAttr(combin_item_id);    // 不是家具
	if (nullptr == furn_cfg) return;

	ItemID &slot_furniture_id = lover_spouse_home->m_param.room_list[room_index].furniture_slot_list[furniture_type];

	if (slot_furniture_id != 0)
	{
		role->NoticeNum(noticenum::NT_SPOUSE_HOME_LOVER_ROOM_EQUIP_FAIL);
		this->SendViewOtherPeopleRoom(this->GetLoverId());
		return;
	}

	if (!role->GetKnapsack()->ConsumeItem(item_id, 1, "SpouseHome::SpouseHomeEquipForLover"))
	{
		role->NoticeNum(errornum::EN_KNAPSACK_USE_ITEM_ERROR);
		return;
	}

	slot_furniture_id = item_id;

	lover_spouse_home->OnDataChange();
	lover_spouse_home->SendSpouseHomeSingleRoomInfo(room_index);

	this->OnDataChange();
	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SPOUSE_HOME);
	this->SendViewOtherPeopleRoom(this->GetLoverId());

	Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(this->GetLoverId()));
	if (lover_role != nullptr)
	{
		lover_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SPOUSE_HOME);
	}

	role->NoticeNum(noticenum::NT_SPOUSE_HOME_LOVER_ROOM_EQUIP_SUCC);

	// 邮件提醒
	static MailContentParam contentparam; contentparam.Reset();
	int length = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_spousehome_equip_furniture_for_lover, role->GetUID(), role->GetName(), (int)role->GetCamp(), item_id);
	if (length > 0)
	{
		MailRoute::MailToUser(IntToUid(this->GetLoverId()), SYSTEM_MAIL_REASON_INVALID, contentparam);
	}
}