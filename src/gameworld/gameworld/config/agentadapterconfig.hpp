#pragma once
#ifndef __AGENT_ADAPTER_CONFIG_HPP__
#define __AGENT_ADAPTER_CONFIG_HPP__

#include <unordered_map>
#include "servercommon/configcommon.h"
#include "servercommon/configbase.h"
#include "servercommon/chatserverdef.h"

union PlatUnion
{
	int id;
	char spid[4];
};

struct AgentAdapterOtherCfg
{
	AgentAdapterOtherCfg() : gm_account_forbid_offline_interval(0) {}

	int gm_account_forbid_offline_interval;		//gm 测试账号封禁离线间隔
};

struct ChannelChatLimitCfg
{
	ChannelChatLimitCfg() : level_limit(0), vip_level_limit(0), forbid_chat_begin_hour(0), forbid_chat_end_hour(0), not_save_chat(0)
	{
	}

	int level_limit;
	int vip_level_limit;
	int forbid_chat_begin_hour;			//禁言开始时钟
	int forbid_chat_end_hour;			//禁言结束时钟
	int not_save_chat;					//不保存聊天
};

// 跨服聊天配置
struct CrossChatLimitCfg
{
	CrossChatLimitCfg()
	{
	}

	int platform_type = 0;
	int is_forbid_cross_chat = 0;
	int can_be_view_other_server = 0;
};

// 私聊配置
struct SingleChatLimitCfg
{
	SingleChatLimitCfg()
	{
	}

	int platform_type = 0;
	int can_offline_chat = 0;		// 能否离线聊天
	int can_ack_chat = 0;			// 能否反向聊天
};

// 首冲团购假人数配置
struct FirstChargeFakeCfg
{
	FirstChargeFakeCfg():is_open(false), min_percent(0), max_percent(0)
	{
	}

	bool is_open;
	int min_percent;
	int max_percent;
};

struct AutoClearGuildMemberCfg
{
	AutoClearGuildMemberCfg() : is_auto_clear(false) {}

	bool is_auto_clear;
};

class AgentAdapterConfig : public ConfigBase
{
public:
	AgentAdapterConfig();
	virtual ~AgentAdapterConfig();

	bool Init(const std::string &path, std::string *err);

	const AgentAdapterOtherCfg & GetOtherCfg() { return m_other_cfg; }

	bool IsSingleChatMustFirend() { return 0 != m_single_chat_must_be_friend_flag; }
	bool IsSingleChatMustVip() { return 0 != m_single_chat_must_be_vip_flag; }
	bool IsSingleChatCanOffline();
	bool IsSingleChatCanAckChat();		// 不满足私聊条件，能否反向聊天

	const ChannelChatLimitCfg * GetChannelChatLimitCfg(int channel_type);
	void CmdSetChannelChatLevel(int channel_type, int level);
	int GetChatLimitConditionTypeFlag() { return m_chat_limit_condition_type_flag; }

	bool IsForbidGmAccountOfflineTooLong() { return m_is_forbid_gm_account_offline_too_long; }
	bool IsCreateRoleMailNotice() { return m_is_create_role_mail_notice; }
	bool IsForbidChagneAvatar() { return m_is_forbid_change_avatar; }
	bool IsForBidCrossSpeaker() { return m_is_forbid_cross_speaker;}

	// 跨服中的平台类型是000，所以只能传角色身上的plat_type
	bool IsForbidCrossChat(int platform_type);
	bool IsCanbeViewOtherServer(int platform_type);			// 跨服世界聊天时能否被其他服玩家看到

	bool GetOpenFirstChargeFakeCfgByPlatName(std::string plat_name);
	const FirstChargeFakeCfg * GetOpenFirstChargeFakeCfg(std::string plat_name);
	const AutoClearGuildMemberCfg *GetAutoClearGuildMemberCfg(const std::string &plat_name);

private:
	int InitOtherCfg(TiXmlElement *RootElement);
	int InitSingleChatFriendLimitCfg(TiXmlElement *RootElement);
	int InitChannelChatLimitCfg(TiXmlElement *RootElement);
	int InitSingleChatVipLimitCfg(TiXmlElement *RootElement);
	int InitGmAccountForbidCfg(TiXmlElement *RootElement);
	int InitCreateRoleMailNoticeCfg(TiXmlElement *RootElement);
	int InitForbidChangeAvatarCfg(TiXmlElement *RootElement);
	int InitCrossChatCfg(TiXmlElement *RootElement);
	int InitSingleChatCfg(TiXmlElement *RootElement);
	int InitOpenFirstChargeFakeCfg(TiXmlElement *RootElement);
	int InitAutoClearGuildMemberCfg(TiXmlElement * RootElement);
	int InitForbidCrossSpeaker(TiXmlElement *RootElement);

	AgentAdapterOtherCfg m_other_cfg;

	int m_single_chat_must_be_friend_flag;				// 私聊必须是好友关系

	int m_single_chat_must_be_vip_flag;					// vip才允许私聊 或者 vip私聊过你，你才允许跟这个玩家私聊

	int m_chat_limit_condition_type_flag;				// 聊天限制类型标记
	ChannelChatLimitCfg m_channel_chat_limit_cfg_list[OPENLEVEL_LIMIT_TYPE_MAX];		// 频道聊天限制

	bool m_is_forbid_gm_account_offline_too_long;		// 是否封禁长时间离线gm账号

	bool m_is_create_role_mail_notice;					// 是否创角邮件提醒
	bool m_is_forbid_change_avatar;						// 是否禁止更换头像
	std::unordered_map<int, CrossChatLimitCfg> m_platform_cross_chat_map;		// 跨服聊天标记map,需要保存所有platform的

	std::map<int, SingleChatLimitCfg> m_platform_single_chat_map;				// 私聊

	std::map<std::string, FirstChargeFakeCfg> m_plat_open_first_charge_fake_count_map;			// 首冲团购假人数开启
	std::map <std::string, AutoClearGuildMemberCfg> m_plat_auto_clear_guild_member_map;			// 自动清理公会成员

	bool m_is_forbid_cross_speaker;			//是否禁止跨服喇叭
};

#endif