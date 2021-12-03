#ifndef __GIFT_HARVEST_CONFIG_HPP__
#define __GIFT_HARVEST_CONFIG_HPP__

#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverdef.h"
#include "gamedef.h"


// ��������
struct GiftHarvestOtherCfg
{
	GiftHarvestOtherCfg() : is_open(false), scene_id(0), day_max_get_score_times(0), monster_create_interval_s(0) {}

	bool is_open;						// �Ƿ񿪷ű�����-�ܿ���
	int scene_id;						// �����ո��ID
	Posi role_pos;						// ��ҳ�����
	int day_max_get_score_times;		// ÿ������ȡ���ִ���	
	int monster_create_interval_s;		// �����������ɼ�����룩
	ItemConfigData  reward_item_list[MAX_ATTACHMENT_ITEM_NUM];			// ���뽱
};

// �ִο���ʱ������
struct GiftHarvestRoundOpenTimeInfo
{
	GiftHarvestRoundOpenTimeInfo() : round(0), round_ready_time(0), round_start_time(0), round_end_time(0), ready_dur_s(0), open_dur_s(0) {}


	int round;							// �ִ�
	int round_ready_time;				// ׼��ʱ��
	int round_start_time;				// ��ʼʱ��
	int round_end_time;					// ����ʱ��

	int ready_dur_s;					// ��ȴ�ʱ��
	int open_dur_s;						// ��������ʱ��
};

// ������Ϣ����
struct GiftHarvestMonsterInfoConfig
{
	GiftHarvestMonsterInfoConfig() : monster_id(0), score(0), max_num(0) {}

	int monster_id;						// ����id
	int score;							// �������ڿɵ÷���
	int max_num;						// ����
};

// Χ��������
struct GiftHarvestFenceConfig
{
	GiftHarvestFenceConfig() : fence_id(0), x1(0), x2(0), y1(0), y2(0) {}

	int fence_id;						// Χ����ȫ��id
	int x1;								
	int x2;
	int y1;
	int y2;
};

// ��������
struct GiftHarvestSkillConfig
{
	GiftHarvestSkillConfig() : index(0), skill_id(0) {}

	int index;
	int skill_id;
};


class GiftHarvestConfig : public ConfigBase
{
public:
	GiftHarvestConfig();
	~GiftHarvestConfig();

	bool Init(const std::string &path, std::string *err);

	Posi GetRandomMonsterCreatePos();													// �������������
	const GiftHarvestOtherCfg & GetOtherCfg() { return m_other_cfg; }					// ��������
	const GiftHarvestRoundOpenTimeInfo *GetGiftHarvestActivityOpenTimeInfo(int round);	// �ִο���ʱ����������
	const GiftHarvestMonsterInfoConfig *GetMonsterInfoCfgWithIndex(int index);			// ������Ϣ����
	const GiftHarvestMonsterInfoConfig *GetMonsterInfoCfgWithMonsterID(int monster_id); // ������Ϣ����
	const GiftHarvestFenceConfig *GetFenceCfg(int index);								// ��ȫ������
	const GiftHarvestSkillConfig *GetSkillCfgList() { return m_skill_cfg_list; }		// ��������

	int GetRoundCount() {return m_activity_open_time_cfg_list_count;}
	int GetMonsterInfoCfgCount() { return m_monster_info_cfg_count; }	// ������Ϣ��������
	int GetFenceCfgCount() { return m_fence_cfg_count; }				// ��ȫ����Ϣ��������

	int GetEnterInfo(Posi *pos)
	{
		if(nullptr == pos) return 0;
		*pos = m_other_cfg.role_pos;
		return m_other_cfg.scene_id;
	}

private:
	int InitOtherCfg(TiXmlElement *RootElement);
	int InitActivityOpenTimeCfg(TiXmlElement *RootElement);
	int InitReliveCfg(TiXmlElement *RootElement);
	int InitMonsterInfoCfg(TiXmlElement *RootElement);
	int InitFenceInfoCfg(TiXmlElement *RootElement);
	int InitSkillInfoCfg(TiXmlElement *RootElement);

	bool DayTimeFormatInvalid(int day_time);

	GiftHarvestOtherCfg m_other_cfg;																// ��������

	int m_activity_open_time_cfg_list_count;														// �ִο���ʱ����������
	GiftHarvestRoundOpenTimeInfo m_activity_open_time_cfg_list[GIFT_HARVEST_OPEN_TIME_MAX_COUNT];	// �ִο���ʱ������

	int m_monster_create_pos_list_count;															// ���︴�������
	Posi m_monster_create_pos_list[GIFT_HARVEST_RELIVE_POS_MAX_COUNT];								// ���︴���

	int m_monster_info_cfg_count;																	// ������������
	GiftHarvestMonsterInfoConfig m_monster_info_cfg_list[GIFT_HARVEST_MONSTER_CFG_MAX_COUNT];		// ������Ϣ����
	
	int m_fence_cfg_count;																			// Χ��������
	GiftHarvestFenceConfig m_fence_cfg_list[GIFT_HARVEST_FENCE_CFG_MAX_COUNT];						// Χ������Ϣ����

	GiftHarvestSkillConfig m_skill_cfg_list[GIFT_HARVEST_SKILL_CFG_MAX_COUNT];						// ������Ϣ����
};

#endif // __CROSS_1V1_COMMON_CONFIG_HPP__

