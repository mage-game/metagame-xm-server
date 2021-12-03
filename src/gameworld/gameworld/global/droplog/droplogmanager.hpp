#ifndef __DROP_LOG_HPP__
#define __DROP_LOG_HPP__

#include <map>
#include <deque>
#include "servercommon/droploglistdef.hpp"

class Role;

class DropLogManager
{
	using LogItemDeque = std::deque<DropLogItem>;
	using FbLogItemDeque = std::vector<DropLogItem>;
public:
	DropLogManager();
	~DropLogManager();

	static DropLogManager & Instance();

	void AddLog(int uid, const GameName role_name, int monster_id, ItemID item_id, int item_num, int scene_id, unsigned short * xianpin_type_list, int list_num);
	void AddFbLog(int uid, const GameName role_name, int monster_id, ItemID item_id, int item_num, int scene_id,short color);
	void AddCrossLog(long long uuid, const GameName role_name, int monster_id, ItemID item_id, int item_num, int scene_id, unsigned short* xianpin_type_list, int list_num);
	void SendLog(Role *role);
	void SendLogFb(Role *role);

	void Init(const DropLogListParam &param,const DropLogFbParam & fb_param);
	void GetInitParam(DropLogListParam *param, DropLogFbParam *fb_param);


	void BeginLogGetItem();
	void AddLogGetItem(const ItemDataWrapper& data);
	void EndLogGetItem(Role* role);
	bool IsBopenBegin() { return m_bopen_logitem; }
	void SetBopenBegin(bool bopen) { m_bopen_logitem = bopen; }
	bool HookSendMail(const UserID *recver_list, int recver_num, const MailContentParam &mail);	//小助手执行期间拦截背包满了的邮件，避免一封邮件只发一个物品
protected:

	LogItemDeque m_log_deque;			// 日志列表 BOSS
	FbLogItemDeque m_log_deque_fb;		// 日志列表 副本

	std::vector<ItemDataWrapper> m_vlogitem;  // 临时储存用于一次
	std::map<ItemID, std::vector<ItemDataWrapper> > m_logitem_vec_map;  // 临时储存用于一次
	bool m_bopen_logitem = false;
};

#endif  // __DROP_LOG_HPP__