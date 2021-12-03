#ifndef __MONSTERINITPARAM_H__
#define __MONSTERINITPARAM_H__

#include <vector>
#include <string>

#include "gamedef.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"


class MonsterInitParam
{
public:
	MonsterInitParam() : monster_type(0), monster_id(0), prof(0), ai_type(0), cold_down(0), capability(0), hp(0), boss_type(0),
		gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0), per_jingzhun(0), per_baoji(0), per_kangbao(0), per_pofang(0), per_mianshang(0),
		level(0), move_speed(0), skillid(0), 
		run_range(0), aoi_range(0), boss_skill_cond_id(0), drop_type(0),
		drop_money_count(0), drop_money_prob(0),
		exp_reward(0), yuanli_reward(0), disappear_time(0), die_notice(false), dead_notice(false), forbid_capture_by_skill(false), is_fix_hurt_on_attack(false),
		is_forbid_recover(false), flush_notice(false), walk_point_num(0), walk_range(0), auto_reover_per(0), auto_reover_time_gap(0)
	{
		memset(monster_name, 0, sizeof(monster_name));
	}

	bool Init(const std::string &configname, std::string &err);

	enum MONSTER_TYPE
	{
		MONSTER_TYPE_NORMAL = 0,								// ��ͨ����
		MONSTER_TYPE_BOSS,										// BOSS���� 
		MONSTER_TYPE_MAX,
	};

	enum BOSS_TYPE
	{
		BOSS_TYPE_INVALID = 0,
		BOSS_TYPE_NORMAL = 1,									// ��ͨboss
		BOSS_TYPE_WORLD = 2,									// ����boss
		BOSS_TYPE_SPECIAL = 3,                                  // ��ϧboss
		BOSS_TYPE_CROSS_BOSS = 4,                               // ���boss
		BOSS_TYPE_MAX,
	};

	// �����boss����
	enum BOSS_TASK_TYPE
	{
		BOSS_TYPE_TASK_INVALID_TYPE = -1,
		BOSS_TYPE_TASK_NORMAL,								// ��ͨ��
		BOSS_TYPE_TASK_MIKU,								// �ؿ�boss
		BOSS_TYPE_TASK_ACTIVE,								// ��Ծboss
		BOSS_TYPE_TASK_VIP,									// VIPboss
		BOSS_TYPE_TASK_PERSON,								// ����boss

		BOSS_TYPE_TASK_MAX_TYPE,
	};

	enum BOSS_DROP_OWN_TYPE
	{
		BOSS_DROP_OWN_TYPE_NONE = 0,							// �޹�����˭�����Լ죩
		BOSS_DROP_OWN_TYPE_MAX_HURT_ROLE = 1,					// ����˺� ���˹�������
		BOSS_DROP_OWN_TYPE_MAX_HURT_TEAM = 2,					// ����˺� �����������(��������˺��������������һ��)
		BOSS_DROP_OWN_TYPE_KILLER = 3,                          // ��ɱ���� ˭ɱ��˭
		BOSS_DROP_OWN_TYPE_ALL_KILL_TEAM = 4,                   // ��ɱ���� ȫ��
		BOSS_DROP_OWN_TYPE_FIRST_ATTACK = 5,					// �׵����䣨��������˺��������������һ��)
		BOSS_DROP_OWN_TYPE_FIRST_ALL_TEAM = 6,					// �׵����� ȫ��ÿ��һ�ݣ���������˺���
		BOSS_DROP_OWN_TYPE_MAX,
	};

	enum AITYPE
	{
		AITYPE_INVALID = -1,			

		AITYPE_FOOL = 0,										// ���߶� ������ 
		AITYPE_STATIC,											// ��ֹ�� ��������
		AITYPE_PASSIVE,											// Ѳ�߷�������
		AITYPE_INITIATIVE,										// Ѳ��������
		AITYPE_STAT_INITIAT,									// ��ֹ������
		AITYPE_JUST_RUN,										// �漴���ܹ�
		AITYPE_JUST_FLEE,										// �������ܹ�
		AITYPE_STATIC_ADD_MIANSHANG_BUFF,						// ������buff��
		AITYPE_STATCI_ADD_RECOVER_BUFF,							// ��Ѫ��

		AITYPE_SPECIALAI_LINGSHANXIANQI = 10000,				// ��ɽ���� ����AI
		AITYPE_SPECIALAI_BATTLEFIELD,							// ս�� ����AI
		AITYPE_SPECIALAI_GONGCHENGZHAN,							// ����ս

		AITYPE_SPECIALAI_TOWERDEFEND_TEAM_ATTACK = 10100,		// ��������������
		AITYPE_SPECIALAI_TOWERDEFEND_TEAM_SHOUHU = 10101,		// �����ػ���

		AITYPE_SPECIALAI_XIANMENGZHAN_GUARDBOSS = 10200,		// ����ս - ��������
		AITYPE_SPECIALAI_XIANMENGZHAN_AREABOSS  = 10201,        // ����ս - �ݵ��
		AITYPE_SPECIALAI_XIANMENGZHAN_DANYAOBOSS  = 10202,      // ����ս - ��ҩ��

		AITYPE_SPECIALAI_GUILD_TOWERDEFEND_ATTACK = 10300,		// �������� - ������
		AITYPE_SPECIALAI_GUILD_TOWERDEFEND_SHOUHU = 10301,		// �������� - �ػ���
		AITYPE_SPECIALAI_GUILD_TOWERDEFEND_MT = 10302,			// �������� - MT��

		AITYPE_CISHA_CAMP1 = 10401,								// ��ɱ����� ��Ӫ1
		AITYPE_CISHA_CAMP2 = 10402,								// ��ɱ����� ��Ӫ2
		AITYPE_CISHA_CAMP3 = 10403,								// ��ɱ����� ��Ӫ3

		AITYPE_GUILDFB_GUARD = 10501,							// ���˸��� ����boss
		AITYPE_GUILDFB_ENEMY = 10502,							// ���˸��� ����

		AITYPE_COIN = 10601,									// ͭ�ұ�����AI
		AITYPE_CROSS_PASTURE_ANIMAL = 10602,					// ������� ����

		AITYPE_BUILD_TOWER_TOWER = 10604,						// ������ ����
		AITYPE_BUILD_TOWER_RUNNING = 10605,						// ������ �ܹ�

		AITYPE_SPECIALAI_TERRITORYWAR_TOWER = 10701,			// ����ս-������
		AITYPE_SPECIALAI_GIFT_HARVEST = 10702,					// �����ո�ai
		
		AITYPE_MAX,
	};

	static const int DROP_LIST_MAX_COUNT = 64;					// �����б�������16��
	static const int MAX_DROP_ITEM_COUNT = 64;

	int				monster_type;								// ����
	UInt16			monster_id;									// ID

	int				prof;										// ְҵ
	GameName		monster_name;								// ����
	int				ai_type;									// AI����
	long long		cold_down;									// ��ȴʱ��

	int				capability;									// ս����
	Attribute		hp;											// Ѫ��
	int				boss_type;									// BOSS����
	Attribute		gongji;										// ����
	Attribute		fangyu;										// ����

	Attribute		mingzhong;									// ����
	Attribute		shanbi;										// ����
	Attribute		baoji;										// ����
	Attribute		jianren;									// ����
	
	Attribute		per_jingzhun;								// ��׼��ֱ�
	Attribute		per_baoji;									// ������ֱ�
	Attribute		per_kangbao;								// ������ֱ�
	Attribute		per_pofang;									// �Ʒ���ֱ�
	Attribute		per_mianshang;								// ������ֱ�

	int				level;										// �ȼ�
	Attribute		move_speed;									// �ƶ��ٶ�

	unsigned short	skillid;									// ��ͨ����ID

	unsigned int	run_range;									// ���ƶ���Χ
	unsigned int	aoi_range;									// ������̽�ⷶΧ
	unsigned int	boss_skill_cond_id;							// Boss��������ID

	int				drop_type;									// ��������
	std::vector<UInt16>	dropid_list;							// ����ID�б�
	std::vector<UInt16> firsthit_dropid_list;					// ��һ������ID�б�
	std::vector<UInt16> dps_dropid_list;						// DPS����ID�б�
	std::vector<UInt16> rand_dropid_list;						// �����������ID�б�
	std::vector<UInt16> task_dropid_list;                       // ���������
	int					drop_money_count;						// ��Ǯ����
	int					drop_money_prob;						// ��Ǯ����

	long long		exp_reward;									// ���齱��
	int				yuanli_reward;								// Ԫ������
	unsigned int	disappear_time;								// 0��ʾ����ʧ ��λ��

	bool			die_notice;									// �����Ƿ񹫸�
	bool            dead_notice;                                // ��ɱ��ʾ
	bool			forbid_capture_by_skill;					// ���ܱ��������������
	bool			is_fix_hurt_on_attack;						// ����ʱ�Ƿ�̶�Ϊ1�˺�
	bool			is_forbid_recover;							// ��ֹ�Զ���Ѫ
	bool			flush_notice;								// �Ƿ�ˢ�¹���

	unsigned int	walk_point_num;								// �������ɢ���ص���
	unsigned int	walk_range;									// ����ɢ����Χ

	int				auto_reover_per;							// һֱ�Զ���Ѫ��ֱ�
	int				auto_reover_time_gap;						// �Զ���Ѫ���ʱ�䣨���룩

private:
	bool ReadDropIDList(PugiXmlNode dropListElement, std::vector<UInt16> &dropid_list);
};

#endif


