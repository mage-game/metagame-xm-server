#ifndef __GONGCHENGZHAN_CONFIG_HPP__
#define __GONGCHENGZHAN_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/activitydef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "engineadapter.h"
#include <vector>
#include <map>

struct GczDailyRewardItemCfg
{
	GczDailyRewardItemCfg() : reward_type(0){}

	enum REWARD_TYPE
	{
		REWARD_TYPE_CHENZHU = 0,
		REWARD_TYPE_NORMAL = 1,
		REWARD_TYPE_MAX,
	};

	int reward_type;

	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];				// �ʼ����Ž���
};

struct GczZhangongRewardItemCfg
{
	GczZhangongRewardItemCfg() : zhangong(0), shengwang_reward(0) {}
	
	int zhangong;							// ս��
	int shengwang_reward;					// ��������
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

struct GongChengZhanWorshipExpConfig
{
	GongChengZhanWorshipExpConfig() : min_level(0),level(0), interval_addexp(0), click_addexp(0) {}

	int min_level;
	int level;
	int interval_addexp;
	long long click_addexp;
};

struct GczOtherCfg
{
	GczOtherCfg() : scene_id(0), zhangong_rank_refresh_time(0), boss1_id(0), boos1_pos(0, 0), boss2_id(0), boos2_pos(0, 0), relive1_pos(0, 0), relive2_pos(0, 0), 
		resource_zuo_xia_pos(0, 0), resource_you_shang_pos(0, 0), resource_pos(0, 0), gong_qiang_pos(0, 0), monster1_id(0) ,monster2_id(0), kill_monster1_zhangong(0), 
		kill_monster2_zhangong(0), hurt_role_01_zhangong(0), kill_role_zhangong(0), hurt_boos_01_zhangong(0), kill_boos_zhangong(0), kill_boos_win_zhangong(0), 
		kill_boos_attack_zhangong(0), online_will_get_zhangong(0), how_much_time_will_get_zhangong(0), chengzhu_skill_id(0), cz_fashion_yifu_id(0), cz_fashion_equi_id(0),
		cz_lover_fashion_yifu_id(0), cz_lover_fashion_equi_id(0), cz_gold_reward(0),
		cz_shengwang_reward(0), win_shengwang_reward(0), win_reward_gongzi(0), fail_shengwang_reward(0), huizhang_extra_reward_item_count(0), lucky_interval(0), luck_people_num(0), opengame_day(0),
		cz_wife_title_id(0), cz_husband_title_id(0), cz_guild_member_title_id(0),
		worship_scene_id(0), worship_start_time_s(0), worship_duration(0), worship_addexp_interval(0), worship_range(0), worship_click_time_limit(0),
		worship_click_cd(0), worship_gather_time(0), exp_factor(0), exp_factor_time(0), exp_factor_click(0), team_reward_item_count(0)
	{}

	int scene_id;						// ����id
	int zhangong_rank_refresh_time;		// ˢ�¸���ս���������ʱ��

	int boss1_id;						// ��ǽid
	Posi boos1_pos;						// ��ǽλ��

	int boss2_id;						// ����id
	Posi boos2_pos;						// ����λ��

	Posi relive1_pos;					// ���������λ��
	Posi relive2_pos;					// �ط������λ��

	Posi resource_zuo_xia_pos;			// ��Դ�����½�����
	Posi resource_you_shang_pos;		// ��Դ�����Ͻ�����

	Posi resource_pos;					// ��Դ��λ��
	Posi gong_qiang_pos;				// ������ǽ��

	int monster1_id;					// ������Դ��С��id
	int monster2_id;					// �ط���Դ��С��id

	int kill_monster1_zhangong;			// ɱ������Դ��С�ֻ�ȡս��
	int kill_monster2_zhangong;			// ɱ������Դ��С�ֻ�ȡս��
	
	int hurt_role_01_zhangong;			// ����0.1Ѫ���ս��
	int kill_role_zhangong;				// ɱ�˻��ս��
	int hurt_boos_01_zhangong;			// ����0.1Ѫ���ս��
	int kill_boos_zhangong;				// �������һ�����ս��
	int kill_boos_win_zhangong;			// �����ռ�췽���ս��
	int kill_boos_attack_zhangong;		// ����������������ս��
	int online_will_get_zhangong;		// ����һ��ʱ����Ի�ö���ս��
	int how_much_time_will_get_zhangong;// ÿ����������Ի��ս��

	int chengzhu_skill_id;				// ������������id
	int cz_fashion_yifu_id;				// �������ʱװ��,�·�
	int cz_fashion_equi_id;				// �������ʱװ��,����
	int cz_lover_fashion_yifu_id;		// ��������ʱװ�·�
	int cz_lover_fashion_equi_id;		// ��������ʱװ����

	int cz_gold_reward;					// ������÷ǰ�Ԫ��
	int cz_shengwang_reward;            // �����������
	ItemConfigData cz_reward_item[MAX_ATTACHMENT_ITEM_NUM];			// ʤ�����᳤��Ʒ����

	int win_shengwang_reward;           // ʤ������������
	int win_reward_gongzi;				// ʤ�������ʽ���
	ItemConfigData win_reward_item[MAX_ATTACHMENT_ITEM_NUM];		// ʤ������Ʒ����

	int fail_shengwang_reward;          // ʧ�ܷ���������
	ItemConfigData fail_reward_item[MAX_ATTACHMENT_ITEM_NUM];	// ʧ�ܷ���Ʒ����

	int huizhang_extra_reward_item_count;
	ItemConfigData huizhang_extra_reward[MAX_ATTACHMENT_ITEM_NUM];

	int lucky_interval;					// ���˽������ʱ��
	ItemConfigData lucky_item;			// ���˽�����Ʒ
	int luck_people_num;				// ��������
	int opengame_day;

	int cz_wife_title_id;				// �������˳ƺ�
	int cz_husband_title_id;			// ��������ƺ�
	int cz_guild_member_title_id;		// ���������Ա�ƺ�

	int worship_scene_id;
	Posi worship_pos;
	int worship_start_time_s;
	int worship_duration;
	int worship_addexp_interval;
	int worship_range;
	int worship_click_time_limit;		// ����սĤ�ݵ����������
	int worship_click_cd;				// ����սĤ�ݵ��cd
	int worship_gather_time;			// Ĥ�ݲɼ�ʱ��

	double exp_factor;
	double exp_factor_time;
	double exp_factor_click;

	int team_reward_item_count;											// ���影��
	ItemConfigData team_reward_item[MAX_ATTACHMENT_ITEM_NUM];			// ���影��
};

class GongChengZhanConfig : public ConfigBase
{
public:
	GongChengZhanConfig();
	~GongChengZhanConfig();

	bool Init(const std::string &path, std::string *err);
	const GczOtherCfg &GetOtherCfg() { return m_other_cfg; }
	GczZhangongRewardItemCfg *GetGczZhangongRewardItemCfgForZhangong(int zhangong);
	int GetEnterPos(bool is_shou_site, Posi *enter_pos);
	const GczDailyRewardItemCfg *GetDailyRewardItemCfg(int reward_type);
	bool GetChengZhuShizhuang(int *yf_part_type, int *yf_part_index, int *wq_part_type, int *wq_part_index, bool is_lover = false);
	const GongChengZhanWorshipExpConfig * GetWorshipExpCfg(const int role_level);
	int GetSosCost(int times);

private:

	GczOtherCfg m_other_cfg;
	
	int InitOther(PugiXmlNode RootElement);
	int InitZhangongReward(PugiXmlNode RootElement);
	int InitDailyReward(PugiXmlNode RootElement);
	int InitWorshipCfg(PugiXmlNode RootElement);
	int InitSosCfg(PugiXmlNode RootElemen);
	std::vector<GczZhangongRewardItemCfg> m_zhangong_reward_vec;
	std::vector<GczDailyRewardItemCfg> m_daily_reward_vec;

	std::vector<GongChengZhanWorshipExpConfig> m_worship_exp_cfg_vec;
	std::vector<int> m_sos_cost_cfg_vec;
};

#endif // __GONGCHENGZHAN_CONFIG_HPP__
