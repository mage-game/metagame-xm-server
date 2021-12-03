#ifndef __JINJIE_TALENT_CONFIG_HPP__
#define __JINJIE_TALENT_CONFIG_HPP__

#include <map>
#include <vector>

#include "servercommon/talentdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/configcommon.h"



struct JinJieActivateTalentGridCondCfg
{
	int need_grade;																//需要阶级
	int pre_index1;																//前置序号格子开启
	int pre_index2;																//前置序号格子开启
	int pre_index3;																//前置序号格子开启
	int per_gird_quality;														//前置格子品质
};

struct JinJieTalenSkillParamCfg
{
	int skill_id;
	int skill_type;																//技能类型
	int book_id;																//天赋书
	int skill_quality;															//技能品质
	int skill_star;																//技能星级
	int need_item_id;															//升级所需要的材料
	int need_item_count;														//升级所需要的材料数量
	int forget_gold;															//遗忘费
	Attribute value;															//属性值
	int per;																	//属性万分比
	int param1;																	//特殊技能参数1
	int param2;																	//特殊技能参数2
	int param3;																	//特殊技能参数3
	int param4;																	//特殊技能参数4
	int capability;																//额外战力
	int cd;																		//CD时间
};

struct JinJieTalentOtherCfg
{
	JinJieTalentOtherCfg() : free_count(0) {}

	int free_count;
};

typedef std::map<int, JinJieTalenSkillParamCfg> TalentSkillStarMp;
//typedef std::map<int, TalentSkillStarMp> TalentSkillQualityMp;

struct JinJieTalentOtherConfig
{
	std::map<int, std::pair<int,int> > tl_type_skill_final_skill;				//终极技能 天赋类型:[技能类型,格子index]
	std::map<int, int> skill_tl_type_final_skill;								//终极技能 技能类型:天赋类型
	std::map<int, JinJieActivateTalentGridCondCfg> activate_cond;						//激活格字需要的条件
	std::map<int, TalentSkillStarMp> skill_info;								//技能信息 {skill_id::{star:skill_param}}
	std::map<int, int> bookid_skillid;											//bookId对应的技能
	std::map<int, std::pair<int, int> > stage_array;								//阶段:[min_count, max_count]
	std::map<int, std::map<int, std::pair<int,int> > > stage_skill_weight;		//阶段:{skill:{min_weight,max_weight}}
	std::map<int, int> flush_cost_gold;											//刷新消耗的元宝
	JinJieTalentOtherCfg other_cfg;
};

class JinJieTalentConfig : public ConfigBase
{
public:
	JinJieTalentConfig();
	~JinJieTalentConfig();

	bool Init(const std::string &configname, std::string *err);
	int InitTalentList(TiXmlElement *RootElement);
	int InitGridList(TiXmlElement *RootElement);
	int InitTalentSkill(TiXmlElement *RootElement);
	int InitChoujiangStage(TiXmlElement *RootElement);
	int InitSkillWeight(TiXmlElement *RootElement);
	int InitFlushCost(TiXmlElement *RootElement);
	int InitOther(TiXmlElement *RootElement);

	const JinJieTalentOtherConfig& GetTalentOtherCfg();

	const JinJieActivateTalentGridCondCfg* GetActivateGridCondCfg(int index);			//激活条件
	const JinJieTalenSkillParamCfg* GetTalenSkillInfoCfg(int skill_id, int star);		//技能参数
	int GetTalenSkillId(int skill_type, int skill_quality, int star);			//技能
	short GetSkillByBookId(int book_id);										//获取技能id
	std::vector<int> RandomSkill(int stage,int count);							//随机技能
	bool IsFinalSkill(int skill_type);											//是否终极技能
	std::pair<int,int> GetTalentFinalSkill(int tl_type);						//终极技能
	short GetStage(int flush_count);											//根据次数找阶段
	short GetFlushGold(int flush_number);										//需要的元宝
	int GetSkillStarAllStuff(int skill_id, int skill_star, ItemConfigData* data, size_t max_count);
	
	const JinJieTalentOtherCfg* GetOtherCfg();

private:
	JinJieTalentOtherConfig m_other_cfg;
};

#endif //__TALENT_CONFIG_HPP__