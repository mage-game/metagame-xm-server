#include "droplogmanager.hpp"
#include "protocal/msgother.h"
#include "obj/character/role.h"
#include <algorithm>
#include "servercommon/serverconfig/crossconfig.hpp"
#include "protocal/msgfb.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "internalcomm.h"
#include "item/knapsack.h"

DropLogManager::DropLogManager()
{
}

DropLogManager::~DropLogManager()
{
}

DropLogManager & DropLogManager::Instance()
{
	static DropLogManager manager;
	return manager;
}

void DropLogManager::AddLog(int uid, const GameName role_name, int monster_id, ItemID item_id, int item_num, int scene_id, unsigned short* xianpin_type_list,int list_num)
{
	if (m_log_deque.size() >= DROP_LOG_NUM_MAX)
	{
		m_log_deque.pop_front();
		m_log_deque.emplace_back(uid, role_name, monster_id, item_id, item_num, scene_id, 0,xianpin_type_list,list_num);
	}
	else
	{
		m_log_deque.emplace_back(uid, role_name, monster_id, item_id, item_num, scene_id, 0,xianpin_type_list,list_num);
	}
}

bool FbLogFunc(const DropLogItem & item1, const DropLogItem & item2)
{
	if (item1.color == item2.color)
	{
		return item1.timestamp > item2.timestamp;
	}

	return item1.color > item2.color;
}
void DropLogManager::AddFbLog(int uid, const GameName role_name, int monster_id, ItemID item_id, int item_num, int scene_id, short color)
{
	unsigned short xianpin_type_list[ItemNamespace::MAX_XIANPIN_NUM] = { 0 };
	if (m_log_deque_fb.size() >= DROP_LOG_NUM_FB_MAX)
	{
		if (color >= m_log_deque_fb[DROP_LOG_NUM_FB_MAX - 1].color)
		{
			m_log_deque_fb[DROP_LOG_NUM_FB_MAX - 1] = DropLogItem(uid, role_name, monster_id, item_id, item_num, scene_id, color, xianpin_type_list, ItemNamespace::MAX_XIANPIN_NUM);
		}
	}
	else
	{
		m_log_deque_fb.push_back(DropLogItem(uid, role_name, monster_id, item_id, item_num, scene_id, color, xianpin_type_list, ItemNamespace::MAX_XIANPIN_NUM));
	}

	std::sort(m_log_deque_fb.begin(), m_log_deque_fb.end(), FbLogFunc);
}

void DropLogManager::AddCrossLog(long long uuid, const GameName role_name, int monster_id, ItemID item_id, int item_num, int scene_id, unsigned short* xianpin_type_list,int list_num)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	static crossgameprotocal::CrossGameSyncCrossCommonBossDropInfo ccbdi;
	ccbdi.uuid = uuid;
	F_STRNCPY(ccbdi.role_name, role_name, sizeof(GameName));
	ccbdi.scene_id = scene_id;
	ccbdi.monster_id = (unsigned short)monster_id;
	ccbdi.item_id = item_id;
	ccbdi.item_num = item_num;
	memset(ccbdi.xianpin_type_list, 0, sizeof(ccbdi.xianpin_type_list));
	int copy_num = list_num < ItemNamespace::MAX_XIANPIN_NUM ? list_num : ItemNamespace::MAX_XIANPIN_NUM;
	memcpy(ccbdi.xianpin_type_list, xianpin_type_list, sizeof(unsigned short)*copy_num);

	InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_COMMON, 0, (const char *)&ccbdi, sizeof(ccbdi));
}

void DropLogManager::SendLog(Role * role)
{
	if (nullptr == role)
	{
		return;
	}

	static Protocol::SCDropLogRet msg;
	msg.type = Protocol::DOPE_LOG_TYPE_BOSS;
	msg.count = 0;

	for (auto &log_item : m_log_deque)
	{
		if (msg.count >= DROP_LOG_NUM_MAX)
		{
			break;
		}
		msg.log_item[msg.count++] = log_item;
	}

	int sendlen = sizeof(msg) - sizeof(msg.log_item[0]) * (DROP_LOG_NUM_MAX - msg.count);
	if (sendlen > 0)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&(msg), sendlen);
	}
}

void DropLogManager::SendLogFb(Role *role)
{
	if (nullptr == role)
	{
		return;
	}

	static Protocol::SCDropLogRet msg;
	msg.type = Protocol::DOPE_LOG_TYPE_FB;
	msg.count = 0;

	for (auto &log_item : m_log_deque_fb)
	{
		if (msg.count >= DROP_LOG_NUM_FB_MAX)
		{
			break;
		}
		msg.log_item[msg.count++] = log_item;
	}

	int sendlen = sizeof(msg) - sizeof(msg.log_item[0]) * (DROP_LOG_NUM_FB_MAX - msg.count);
	if (sendlen > 0)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&(msg), sendlen);
	}
}

void DropLogManager::Init(const DropLogListParam &param, const DropLogFbParam & fb_param)
{
	m_log_deque.clear();
	m_log_deque_fb.clear();

	for (int idx = 0; idx < param.log_count && idx < DROP_LOG_NUM_MAX; ++idx)
	{
		m_log_deque.push_back(param.item_list[idx]);
	}

	for (int idx = 0; idx < fb_param.log_count && idx < DROP_LOG_NUM_FB_MAX; ++idx)
	{
		m_log_deque_fb.push_back(fb_param.item_list[idx]);
	}
}

void DropLogManager::GetInitParam(DropLogListParam *param, DropLogFbParam *fb_param)
{
	if (nullptr == param || nullptr == fb_param)
	{
		return;
	}

	param->log_count = 0;

	for (auto &item : m_log_deque)
	{
		if (param->log_count >= DROP_LOG_NUM_MAX)
		{
			break;
		}
		param->item_list[param->log_count++] = item;
	}

	fb_param->log_count = 0;

	for (auto &fb_item : m_log_deque_fb)
	{
		if (fb_param->log_count >= DROP_LOG_NUM_FB_MAX)
		{
			break;
		}
		fb_param->item_list[fb_param->log_count++] = fb_item;
	}
}

void DropLogManager::BeginLogGetItem()
{
	m_bopen_logitem = true;
	m_vlogitem.clear();
	m_logitem_vec_map.clear();
}

void DropLogManager::AddLogGetItem(const ItemDataWrapper& data)
{
	if (!m_bopen_logitem)
	{
		return;
	}

	if (data.Invalid())
	{
		return;
	}
	const size_t max_vlog_size = 1024;
	if (m_vlogitem.size() < max_vlog_size)
	{
		m_vlogitem.push_back(data);
	}

	if (m_logitem_vec_map.find(data.item_id) == m_logitem_vec_map.end())
	{
		std::vector<ItemDataWrapper> vec;
		vec.push_back(data);
		m_logitem_vec_map[data.item_id] = vec;
	}
	else
	{
		const ItemBase *item = ITEMPOOL->GetItem(data.item_id);
		if (item == NULL)
		{
			return;
		}
		const size_t size = m_logitem_vec_map[data.item_id].size();
		const size_t index = size - 1;
		if (index < 0 || index >= size)
		{
			return;
		}
		if (m_logitem_vec_map[data.item_id][index].num + data.num > item->GetPileLimit())
		{
			m_logitem_vec_map[data.item_id].push_back(data);
		}
		else
		{
			m_logitem_vec_map[data.item_id][index].num += data.num;
		}
	}
}

void DropLogManager::EndLogGetItem(Role* role)
{
	m_bopen_logitem = false;

	bool is_break = false;
	Protocol::SCLittleHelperItemInfo info;
	info.count = 0;
	for (auto &map_it : m_logitem_vec_map)
	{
		const size_t vec_size = map_it.second.size();
		for (size_t i = 0; i < vec_size; ++i)
		{
			if (info.count >= lhns::MAX_DROP_ITEM_COUNT)
			{
				is_break = true;
				break;
			}
			info.item_list[info.count].item_id = map_it.second[i].item_id;
			info.item_list[info.count].is_bind = map_it.second[i].is_bind;
			info.item_list[info.count].num = map_it.second[i].num;
			UNSTD_STATIC_CHECK(sizeof(info.item_list[info.count].xianpin_type_list) == sizeof(map_it.second[i].param_data.xianpin_type_list));
			memcpy(info.item_list[info.count].xianpin_type_list, map_it.second[i].param_data.xianpin_type_list, sizeof(info.item_list[info.count].xianpin_type_list));
			++info.count;
		}
		if (is_break)
		{
			break;
		}
	}
	/*const size_t size = m_vlogitem.size();
	for (size_t i = 0; i < size && i < lhns::MAX_DROP_ITEM_COUNT; ++i)
	{
		info.item_list[i].item_id = m_vlogitem[i].item_id;
		info.item_list[i].is_bind = m_vlogitem[i].is_bind;
		info.item_list[i].num = m_vlogitem[i].num;
		if (m_vlogitem[i].has_param)
		{
			UNSTD_STATIC_CHECK(sizeof(info.item_list[i].xianpin_type_list) == sizeof(m_vlogitem[i].param_data.xianpin_type_list));
			memcpy(info.item_list[i].xianpin_type_list, m_vlogitem[i].param_data.xianpin_type_list, sizeof(info.item_list[i].xianpin_type_list));
		}
		++info.count;
	}*/
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&info, sizeof(info) - (lhns::MAX_DROP_ITEM_COUNT - info.count) * sizeof(Protocol::SCLittleHelperItemInfo::RewardItem));
}

bool DropLogManager::HookSendMail(const UserID *recver_list, int recver_num, const MailContentParam & mail)
{
	if (NULL == recver_list || 0 == recver_num)
	{
		return false;
	}
	if (!m_bopen_logitem)
	{
		return false;
	}
	MailContentTxt contenttxt;
	int length2 = SNPRINTF(contenttxt, sizeof(contenttxt), gamestring::g_knapsack_put_content);
	if (length2 <= 0 || !strstr(mail.contenttxt, contenttxt))
	{
		return false;
	}
	for (int i = 0; i < recver_num; ++i)
	{
		Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(recver_list[i]);
		if (NULL == role)
		{
			continue;
		}
		role->GetKnapsack()->PushNoHaveSpaceWrapper(mail);
	}
	return true;
}

