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

	int gm_account_forbid_offline_interval;		//gm �����˺ŷ�����߼��
};

struct ChannelChatLimitCfg
{
	ChannelChatLimitCfg() : level_limit(0), vip_level_limit(0), forbid_chat_begin_hour(0), forbid_chat_end_hour(0), not_save_chat(0)
	{
	}

	int level_limit;
	int vip_level_limit;
	int forbid_chat_begin_hour;			//���Կ�ʼʱ��
	int forbid_chat_end_hour;			//���Խ���ʱ��
	int not_save_chat;					//����������
};

// �����������
struct CrossChatLimitCfg
{
	CrossChatLimitCfg()
	{
	}

	int platform_type = 0;
	int is_forbid_cross_chat = 0;
	int can_be_view_other_server = 0;
};

// ˽������
struct SingleChatLimitCfg
{
	SingleChatLimitCfg()
	{
	}

	int platform_type = 0;
	int can_offline_chat = 0;		// �ܷ���������
	int can_ack_chat = 0;			// �ܷ�������
};

// �׳��Ź�����������
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
	bool IsSingleChatCanAckChat();		// ������˽���������ܷ�������

	const ChannelChatLimitCfg * GetChannelChatLimitCfg(int channel_type);
	void CmdSetChannelChatLevel(int channel_type, int level);
	int GetChatLimitConditionTypeFlag() { return m_chat_limit_condition_type_flag; }

	bool IsForbidGmAccountOfflineTooLong() { return m_is_forbid_gm_account_offline_too_long; }
	bool IsCreateRoleMailNotice() { return m_is_create_role_mail_notice; }
	bool IsForbidChagneAvatar() { return m_is_forbid_change_avatar; }
	bool IsForBidCrossSpeaker() { return m_is_forbid_cross_speaker;}

	// ����е�ƽ̨������000������ֻ�ܴ���ɫ���ϵ�plat_type
	bool IsForbidCrossChat(int platform_type);
	bool IsCanbeViewOtherServer(int platform_type);			// �����������ʱ�ܷ���������ҿ���

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

	int m_single_chat_must_be_friend_flag;				// ˽�ı����Ǻ��ѹ�ϵ

	int m_single_chat_must_be_vip_flag;					// vip������˽�� ���� vip˽�Ĺ��㣬��������������˽��

	int m_chat_limit_condition_type_flag;				// �����������ͱ��
	ChannelChatLimitCfg m_channel_chat_limit_cfg_list[OPENLEVEL_LIMIT_TYPE_MAX];		// Ƶ����������

	bool m_is_forbid_gm_account_offline_too_long;		// �Ƿ�����ʱ������gm�˺�

	bool m_is_create_role_mail_notice;					// �Ƿ񴴽��ʼ�����
	bool m_is_forbid_change_avatar;						// �Ƿ��ֹ����ͷ��
	std::unordered_map<int, CrossChatLimitCfg> m_platform_cross_chat_map;		// ���������map,��Ҫ��������platform��

	std::map<int, SingleChatLimitCfg> m_platform_single_chat_map;				// ˽��

	std::map<std::string, FirstChargeFakeCfg> m_plat_open_first_charge_fake_count_map;			// �׳��Ź�����������
	std::map <std::string, AutoClearGuildMemberCfg> m_plat_auto_clear_guild_member_map;			// �Զ��������Ա

	bool m_is_forbid_cross_speaker;			//�Ƿ��ֹ�������
};

#endif