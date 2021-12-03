/******************************************************************************************
FileName: fbmanager.hpp
Author: dreamwang
Description: ��������  
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/10/25/ 09:04:05     1.0           ����
*****************************************************************************************/

#ifndef __FB_MANAGER_HPP__
#define __FB_MANAGER_HPP__

#include <map>

#include "protocal/msgother.h"

const static int DROP_INFO_SEND_DELAY_TIME = 2;
const static int DROP_INFO_SEND_DELAY_TIME_GUILD_MONSTER = 3;
const static int DROP_INFO_SEND_DELAY_TIME_TOWER_DEFEND = 3;
const static int DROP_INFO_SEND_DELAY_TIME_ARMOR_DEFEND = 3;
const static int DROP_INFO_SEND_DELAY_TIME_PHASE = 3;
const static int FB_GUIDE_GATHER_NUM_MAX = 10;

class Role; 

class FBManager
{
public:
	enum FB_CHECK_TYPE
	{
		FBCT_DAILY_FB = 1,							// �ճ�����
		FBCT_STORY_FB = 2,							// ���鸱��
		FBCT_CHALLENGE = 3,							// ��ս����
		FBCT_PHASE = 4,								// �׶θ���
		FBCT_FUN_OPEN_FB = 5,						// ���ܿ������˸���
		FBCT_TOWERDEFEND_PERSONAL = 6,				// ��������
		FBCT_YAOSHOUJITANG_TEAM = 8,				// ���޼�̳��ӱ�
		FBCT_QINGYUAN = 9,							// ��Ե����
		FBCT_RESERVE_1 = 10,						// 
		FBCT_HUNYAN = 11,							// ���縱��
		FBCT_RESERVE_2 = 12,						// 
		FBCT_ZHUANZHI_PERSONAL = 13,				// ����תְ����
		FBCT_MIGONGXIANFU_TEAM = 14,				// �Թ��ɸ�����
		FBCT_WUSHUANG = 15,							// ��˫����
		FBCT_PATAFB = 16,							// ��������
		FBCT_CAMPGAOJIDUOBAO = 17,					// ʦ�Ÿ߼��ᱦ
		FBCT_VIPFB = 18,							// VIP����
		FBCT_MOJIERENLEIFB = 19,					// ħ�丱������ƪ
		FBCT_MOJIEAIRENFB = 20,						// ħ�丱������ƪ
		FBCT_FBGUIDE = 21,							// ��������
		FBCT_RUNE_TOWER = 22,						// ������
		FBCT_TEAM_EQUIP_FB = 23,					// ���װ������
		FBCT_DAILY_TASK_FB = 24,					// �ճ����񸱱�
		FBCT_TUITU_TUITU_FB = 25,					// ��ͼ����
		FBCT_QING_YUAN_SHENG_DI_FB = 26,			// ��Եʥ��
		FBCT_NEQ_FB = 27,							// ����װ���� ��������
		FBCT_ARMOR_FB = 29,							// ����װ���� ���߲���
		FBCT_BUILD_TOWER = 30,						// ������������
		FBCT_PERSON_BOSS = 31,                      // ����boss
		FBCT_XINMO_FB = 32,                         // ��ħ����(תְ����)
		FBCT_TOWERDEFEND_TEAM = 33,					// ����ػ�
		FBCT_EQUIP_TEAM_FB = 34,					// �����������Ӣ���װ������

		FBCT_COUNT,
	};

	enum PHASE_FB_TYPE
	{
		PFT_XIANNV = 1,
		PFT_COIN = 2,
		PFT_WING = 3,
		PFT_XIULIAN = 4,
		PFT_QIBING = 5,

		PFT_COUNT,
	};

	static FBManager & Instance();

	void EnterFB(Role *role, short fb_type, int param1, int param2, int param3);
	void LeaveFB(Role *role);
	void AutoFB(Role *role, short fb_type, int param1, int param2, int param3);
	
	void ReqNextLevel(Role *role);
	
	void OnUserLogin(Role *role);

	bool IsHunyanRunning(Role *role);
	void RegisterHunyanKey(Role *role, int fb_key);
	void UnRegisterHunyanKey(int uid);

	int GetMyHunyanFBKey(int uid);

	// ����-------------------------------------
	void SaveHunYanInviteInfo(Role *role, int scene_key);
	void DeleteHunYanInviteInfo(int uid, int scene_key);
	void GetAllHunYanFbInviteInfo(Protocol::SCQingyuanHunyanInviteInfo &qhii, Role *role);

	void SendHunYanFbInviteInfoToRole(Role *role);
	void SendHunYanFbInviteInfoToWorld();

	//------------------------------------------
	void SendTeamEquipFBCountInfo(Role *role);
	void TeamEquipFbBuyDropTimes(Role *role);

	bool CheckTeamEnterDailyFB(Role *role, int dailyfb_type);
	bool CheckTeamEnterEquipFb(Role *role, int layer);

	void LogFunctionStats(Role *role, int fb_type, int param1, int param2, int param3);

	void BuildTowerFbBuyTimes(Role *role);
	bool AutoBuildTowerFB(Role *role, int flush_extra_num);

	void OnPersonalBossBuyEnterTimes(Role *role);

private:
	FBManager();
	~FBManager();

	int GetHunyanFBKey() { return ++m_hunyan_fb_key_inc; }

	void EnterQingyuanFB(Role *leader_role, bool is_skip);
	void EnterQingyuanFBPersonal(Role *leader_role, bool is_skip);
	void EnterQingyuanFBWithLover(Role *leader_role, bool is_skip);

	void EnterHunyanFB(Role *role, int yanhui_type, int yanhui_fb_key);			// ���縱���ĳ�ԤԼ��������
	void EnterTeamFB(Role *leader_role, int fb_type, int mode);
	void EnterStoryFb(Role *role, int fb_index);
	void EnterMoJieHumanFb(Role *role, int fb_index);
	void EnterDailyFb(Role *role, int dailyfb_type, int is_team);
	void EnterTeamEquipFb(Role *role, int layer);
	void EnterDailyFbReal(Role *role, int dailyfb_type, int scene_key);
	void EnterChallengeFB(Role *role, int fb_index);
	void EnterPhaseFB(Role *role, int fb_index);
	void EnterPataFB(Role *role);
	void EnterVipFB(Role *role, int fb_level);

	void EnterFunOpenFB(Role *role, int fb_type);
	void EnterGuideFB(Role *role, int fb_type);
	void EnterRuneTowerFB(Role *role);

	void EnterDailyTaskFb(Role *role, int scene_id);
	void EnterTuituFb(Role* role, int fb_type, int chapter, int level);
	void EnterPersonalDefendFB(Role *role, int level);
	void EnterPersonalHolidayGuardFB(Role *role, int level, int npc_index);

	void EnterQingYuanShengDiFb(Role* role, int layer);

	void EnterNewEquipFb(Role* role, int chapter, int level);
	void EnterArmorDefendFb(Role* role, int level);

	bool EnterPersonBossFB(Role * role, int layer);

	bool EnterXinMoFB(Role * role);

	void EnterBuildTowerFB(Role * role);
	void EnterTeamTowerDefendFb(Role *leader_role, int mode);
	void EnterEquipTeamFb(Role *role, int param1);

	void NotifySystemMsg(char *str_buff, int str_len, int notice_type, Role * role);
	void AutoFbAddLog(Role * role, int monster_id, ItemID item_id, int num,int scene_id);

	int m_hunyan_fb_key_inc;
	int m_daily_fb_key_inc;
	int m_team_equip_fb_key_inc;
	
	std::map<int, int> m_opening_hunyan_register;

// 	struct HunYanInviteInfo
// 	{
// 		std::vector<int> invite_list;		// ���汻������Ļ���
// 	};
// 
// 	std::map<int, HunYanInviteInfo *> m_hunyan_info_map;

	std::vector<int> m_all_hunyan_info_list;		// �������п����еĻ���
};

#endif // __FB_KEY_HPP__
