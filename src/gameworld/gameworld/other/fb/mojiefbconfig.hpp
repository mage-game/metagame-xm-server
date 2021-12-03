#ifndef __MOJIE_FB_CONFIG_HPP__
#define __MOJIE_FB_CONFIG_HPP__

#include "common/tinyxml/tinyxml.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/fbdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

// 人类篇
struct HumanPianCfg
{
	HumanPianCfg() : fb_id(0), pretaskid(0), enter_pos(0, 0), scene_id(0), boss_id(0), born_pos(0, 0), mojie_id(0){}
	int fb_id;						// 副本id
	int pretaskid;					// 前置任务
	Posi enter_pos;					// 进入点
	int scene_id;					// 场景id
	int boss_id;					// bossId
	Posi born_pos;					// boss出生点
	int mojie_id;					// 获取魔戒id
};

// 矮人篇
struct AiRenPianCfg
{
	AiRenPianCfg() : fb_id(0), pretaskid(0), enter_pos(0, 0), scene_id(0), boss_id(0), born_pos(0, 0), mojie_id(0){}
	int fb_id;						// 副本id
	int pretaskid;					// 前置任务
	Posi enter_pos;					// 进入点
	int scene_id;					// 场景id
	int boss_id;					// bossId
	Posi born_pos;					// boss出生点
	int mojie_id;					// 获取魔戒id
};

// 精灵篇
struct JingLingPianCfg
{
	
	JingLingPianCfg() : chapter(0), task_list_count(0){}

	struct TaskConfig
	{
		TaskConfig() : id(0), task(0), param1(0), param2(0), param3(0){}
			
		int id;							// 索引
		int task;						// 任务
		int param1;						// 参数1
		int param2;						// 参数2
		int param3;						// 参数3
		ItemConfigData reward1;			// 奖励1
		ItemConfigData reward2;			// 奖励2
		int mojie_id;					// 魔戒id
		std::string task_desc;			// 任务描述
	};

	int chapter;						// 章节
	int task_list_count;				// 任务数max
	
	TaskConfig task_list[MOJIE_FB_JINGLING_PIAN_ZHANGJIE_TASK_MAX_COUNT];

};

// 魔戒技能
struct MoJieSkillCfg
{
	MoJieSkillCfg() : mojie_id(0), mojie_jineng_type(0), param_a(0), param_b(0), param_c(0), param_d(0){}
	int mojie_id;					// 魔戒id
	int mojie_jineng_type;			// 魔戒技能类型
	int param_a;					// 参数A
	int param_b;					// 参数B
	int param_c;					// 参数C
	int param_d;					// 参数D
};

class MoJieFbConfig : public ConfigBase
{
public:
	MoJieFbConfig();
	~MoJieFbConfig();

	bool Init(std::string path, std::string *err);
		
	const JingLingPianCfg::TaskConfig *  GetJingLingCfgToParam(int chapter, int param1 = -1, int param2 = -1, int param3 = -1);

	int getHumanPianCfgCount(){return m_human_pian_cfg_count;};
	int getAiRenCfgCount(){return m_ai_ren_pian_cfg_count;};
	int getJingLingCfgCount(){return m_jing_ling_pian_cfg_count;};
	int getJingLingChapterCfgCount(){return m_jing_ling_pian_chapter_cfg_count;};
	int getSkillCfgCount(){return m_skill_cfg_count;};
	
	const HumanPianCfg * GetHumanPianCfg(int fb_id);
	const AiRenPianCfg * GetAiRenCfg(int fb_id);
	const JingLingPianCfg * GetJingLingCfg(int fb_id);
	const MoJieSkillCfg * GetMoJieCfg(int fb_id);

private:
	
	int InitHumanPian(PugiXmlNode RootElement);
	int InitAiRenPian(PugiXmlNode RootElement);
	int InitJingLingPian(PugiXmlNode RootElement);
	int InitSkillPian(PugiXmlNode RootElement);
	
	int m_human_pian_cfg_count;
	HumanPianCfg m_human_pian_cfg[MOJIE_FB_HUMAN_PIAN_MAX_COUNT];

	int m_ai_ren_pian_cfg_count;
	AiRenPianCfg m_ai_ren_pian_cfg[MOJIE_FB_AIREN_PIAN_MAX_COUNT];

	int m_jing_ling_pian_cfg_count;			// 任务总数
	int m_jing_ling_pian_chapter_cfg_count;	// 章节总数
	JingLingPianCfg m_jing_ling_pian_cfg[MOJIE_FB_SKILL_PIAN_ZHANGJIE_MAX_COUNT];

	int m_skill_cfg_count;
	MoJieSkillCfg m_skill_cfg[MOJIE_FB_SKILL_MAX_COUNT];
	
};

#endif // __MOJIE_FB_CONFIG_HPP__