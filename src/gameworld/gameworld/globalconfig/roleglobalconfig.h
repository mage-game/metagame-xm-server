#ifndef __ROLEGLOABALCONFIG_H__
#define __ROLEGLOABALCONFIG_H__

#include <string>

#include "gamedef.h"
#include "servercommon/servercommon.h"

#include "obj/character/attribute.hpp"

class TiXmlElement;

namespace globalconfig
{
	static const int MAX_DEFAULT_SKILL_COUNT = 30;			// Ĭ�ϼ���������

	struct RoleGlobalConfigForbidNeiwanAccount
	{
		RoleGlobalConfigForbidNeiwanAccount() : is_forbid(false), no_login_sec(0), chongzhi_max(0) {}

		bool is_forbid;										// �Ƿ�ﵽһ�������ͷ�����棨������������
		unsigned int no_login_sec;							// �ϴε�¼�������ڵ��������
		int chongzhi_max;									// ��ֵ��������������Ԫ����
	};

	struct RoleForbidChannel
	{
		RoleForbidChannel() : role_level_limit(0), max_world_chat_count1(0), max_world_chat_count2(0) {}

		int role_level_limit;										
		int max_world_chat_count1;							
		int max_world_chat_count2;									
	};

	struct RoleContinueKillCfg
	{
		RoleContinueKillCfg() : kill_count(0), buff_time(0), gongji_add_per(0)
		{}

		int kill_count;
		int buff_time;
		int gongji_add_per;
	};

	class RoleGlobalConfig
	{
	public:
		RoleGlobalConfig();
		~RoleGlobalConfig();

		bool Init(const std::string &path, std::string *err);

		// ���ؿ���ѧϰ��Ĭ�ϼ��ܵ�������֧�ֶ��Ĭ�ϼ��ܣ�
		int GetDefaultSkillID(int prof, int default_skill_id_list[MAX_DEFAULT_SKILL_COUNT]);
		int GetFlyIncreaseSpeedPercent();
		bool GetIsForbidNeiwanAction();
		float GetProfHurtFactor(int attack_prof, int defend_prof);
		float GetProfMingZhongFactor(int prof);
		RoleGlobalConfigForbidNeiwanAccount & GetForbidNeiwanAccountCfg() { return m_forbid_neiwan_account; }
		RoleForbidChannel & GetRoleForbidChannel() { return m_forbid_channel; }
		RoleContinueKillCfg * GetContinueKillCfg(int kill_count);
		unsigned int GetTriggerContinueKillMaxInterval() { return m_trigger_continue_kill_interval_max; }

	public:

		Attribute m_cooldown;									// ȫ��CD
		int m_xinshuo_protected_level;							// ���ֱ����ȼ�
		int m_upgrade_log_level;								// ��������־��ʼ�ȼ�
		int m_peace_mode_interval;								// �л�����ƽģʽʱ����
		int m_realive_interval;									// ��������ʱ����
		int m_in_fight_ms;										// ����ս��״̬��ʱ�� ����
		Posi m_in_team_distance;								// �����״̬���ξ���뾶
		unsigned int m_recover_hp_interval;						// ��ս��״̬��Ѫ��� ��
		int m_recover_hp_per;									// ��ս��״̬��Ѫ��ֱ�
		int m_recover_hp_fight_per;								// ս��״̬��Ѫ��ֱ�

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		struct ProfCfg
		{
			ProfCfg() : default_skill_id_count(0)
			{
				memset(default_skill_id_list, 0, sizeof(default_skill_id_list));
			}

			int default_skill_id_count;
			int default_skill_id_list[MAX_DEFAULT_SKILL_COUNT];
		};
		ProfCfg * GetProfCfg(int prof);
		ProfCfg m_prof_cfg_list[PROF_TYPE_MAX];

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		int m_fly_increase_speed_percent;									// ����ģʽ����ٶȰٷֱ�

		bool m_forbid_neiwan_action;										// �Ƿ�����������һЩ���������罻�ף����ʼ��������г���Ʒ�ȵ� 

		RoleGlobalConfigForbidNeiwanAccount m_forbid_neiwan_account;		// ����������
		RoleForbidChannel m_forbid_channel;									// ��������

		unsigned int m_trigger_continue_kill_interval_max;					// ��ն�������ʱ����
		std::map<int, RoleContinueKillCfg> m_continue_kill_cfg_map;			// ��նЧ������

	private:
		int InitContinueKillCfg(TiXmlElement *RootElement);
	};
};

#endif // __ROLEGLOABALCONFIG_H__

