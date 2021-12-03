#ifndef XIANNVSHOUHUCONFIG_HPP
#define XIANNVSHOUHUCONFIG_HPP

#include "gamedef.h"
#include "servercommon/basedef.h"
#include <map>

#include "servercommon/struct/itemlistparam.h"

#include "item/itembase.h"

#include "obj/character/attribute.hpp"
#include "servercommon/xiannvshouhudef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

// ��Ů�ػ�����
struct XiannvShouhuGradeConfig
{
	XiannvShouhuGradeConfig() : grade(0), client_grade(0),active_image_id(0), bless_val_limit(0), is_broadcast(false)
	{
	}

	short grade;											// ����
	short client_grade;										// �ͻ��˽���
	short active_image_id;									// ��������ID
	int bless_val_limit;									// ף��ֵ����

	bool is_broadcast;										// �Ƿ���
};

// ��������
struct XiannvShouhuImageConfig								 
{
	XiannvShouhuImageConfig() : image_id(0)
	{
		memset(image_name, 0, sizeof(image_name));
	}

	int image_id;
	GameName image_name;
};

struct XiannvShouhuUpStarExpCfg
{
	XiannvShouhuUpStarExpCfg(): up_star_level_exp(0), up_start_total_exp(0), maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0){}

	int up_star_level_exp;
	int up_start_total_exp;

	int maxhp;
	int gongji;
	int fangyu;
	int mingzhong;
	int shanbi;
	int baoji;
	int jianren;
};

struct XiannvShouhuUpStarStuffCfg
{
	XiannvShouhuUpStarStuffCfg(): up_star_item_index(0), up_star_item_id(0), star_exp(0), baoji_per(0), baoji_mul(0){}

	short up_star_item_index;
	ItemID up_star_item_id;
	int star_exp;
	int baoji_per;
	int baoji_mul;
};

class XiannvShouhuConfig : public ConfigBase
{
public:
	XiannvShouhuConfig();
	~XiannvShouhuConfig();

	bool Init(const std::string &configname, std::string *err);
	const XiannvShouhuGradeConfig * GetGradeConfig(int grade) const;
	const XiannvShouhuImageConfig * GetImageCfg(int attr_id) const;
	const XiannvShouhuUpStarExpCfg * GetUpStarExpCfg(int star_level);
	const XiannvShouhuUpStarStuffCfg * GetUpStarStuffCfg(int stuff_index);

private:
	int InitOther(PugiXmlNode RootElement);
	int InitGradeCfg(PugiXmlNode RootElement);
	int InitImageCfg(PugiXmlNode RootElement);
	int InitUpStarExpCfg(PugiXmlNode RootElement);
	int InitUpStarStuffCfg(PugiXmlNode RootElement);

	XiannvShouhuGradeConfig m_grade_cfg_list[XiannvShouhuParam::MAX_GRADE];

	XiannvShouhuImageConfig m_image_list[XiannvShouhuParam::MAX_IMAGE_ID + 1];

	XiannvShouhuUpStarExpCfg m_upstar_exp_list[XiannvShouhuParam::MAX_STAR_LEVEL];
	XiannvShouhuUpStarStuffCfg m_upstar_item_list[XiannvShouhuParam::MAX_UP_STAR_ITEM_COUNT];
	
};

#endif
