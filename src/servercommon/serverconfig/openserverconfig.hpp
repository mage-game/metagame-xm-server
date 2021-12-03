#ifndef __OPEN_SERVER_CONFIG_HPP__
#define __OPEN_SERVER_CONFIG_HPP__

#include "servercommon/configcommon.h"
#include "servercommon/basedef.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class OpenServerConfig
{
public:
	OpenServerConfig();
	~OpenServerConfig();

	bool Init(const std::string &path, std::string *err);

	UInt32 GetServerVersion() const { return m_server_version; }

	time_t GetServerRealStartTime() const { return m_server_real_start_time; }
	time_t GetServerRealCombineTime() const { return m_server_real_combine_time; }
	time_t GetNewhandGuideEndTime() const { return m_newhand_guide_end_time; } 
	time_t GetOpenServerZeroTime() const;
	int GetOpenServerDayIndex();

	void GetOpenGameActivityTime(unsigned int *begin_time, unsigned int *end_time);
	void GetOpenGameChongCiTime(unsigned int *begin_time, unsigned int *end_time);
	void GetCombineServerActivityTime(unsigned int *begin_time, unsigned int *end_time);
	void GetKillMonsterDoubleExpTime(unsigned int *begin_time, unsigned int *end_time);
	void GetCombineQingdianActivityTime(unsigned int *begin_time, unsigned int *end_time);

	bool IsPingbiWorldExpAdd() { return 0 != m_pingbi_worldexp_add; }

	int GetChannelOpenLevelWorld() { return m_channel_open_level_world; }
	void CmdSetWorldChatLevel(int level) { m_channel_open_level_world = level; }

	int GetChannelOpenLevelScene() { return m_channel_open_level_scene; }
	void CmdSetSceneChatLevel(int level) { m_channel_open_level_scene = level; }

	int GetChannelOpenLevelCamp() { return m_channel_open_level_camp; }
	void CmdSetCampChatLevel(int level) { m_channel_open_level_camp = level; }

	int GetChannelOpenLevelTeam() { return m_channel_open_level_team; }
	void CmdSetTeamChatLevel(int level) { m_channel_open_level_team = level; }
	
	int GetChannelOpenLevelGuild() { return m_channel_open_level_guild; }
	void CmdSetGuildChatLevel(int level) { m_channel_open_level_guild = level; }

	int GetSingleChatLevel() { return m_single_chat_level; }
	void CmdSetSingleChatLevel(int level) { m_single_chat_level = level; }

	int GetSendMailLevel() { return m_send_mail_level; }
	void CmdSetSendMailLevel(int level) { m_send_mail_level = level; }

	int GetLineNum() { return m_line_num; }
	bool IsIosExamine() { return m_is_ios_examine; }

private:

	time_t		m_server_real_start_time;				// ��ʽ����ʱ��
	time_t		m_server_real_combine_time;				// ��ʽ�Ϸ�ʱ��

	UInt32		m_server_version;						// �������汾��

	time_t		m_opengame_activity_end_time;			// ���������ʱ��
	time_t		m_opengame_chongci_end_time;			// ������̽���ʱ��
	time_t		m_combine_server_activity_end_time;		// �Ϸ������ʱ��
	time_t		m_newhand_guide_end_time;				// ����ָ��Ա����ʱ��
	time_t		m_kill_monster_double_exp_end_time;		// ���˫������ʱ��
	time_t		m_combine_qingdian_activity_end_time;	// �Ϸ�������ʱ��

	int			m_pingbi_worldexp_add;					// �Ƿ�������ȼ��ӳ�
	int			m_channel_open_level_world;
	int			m_channel_open_level_scene;
	int			m_channel_open_level_camp;
	int			m_channel_open_level_team;
	int			m_channel_open_level_guild;

	int			m_single_chat_level;
	int			m_send_mail_level;

	int			m_line_num;
	bool		m_is_ios_examine;						// �Ƿ�ios��˷�
};

#endif // __OPEN_SERVER_CONFIG_HPP__

