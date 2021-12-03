#ifndef __LITTLE_HELPER_CONFIG_H__
#define __LITTLE_HELPER_CONFIG_H__

#include <map>
#include <vector>
#include <string>

#include "servercommon/rolelotterycountdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class Role;

namespace lhns						//little helper name space
{
	typedef short					complete_type_t;		//�������
	typedef short					seq_t;					//����
	typedef short					opengame_day_t;			//��������
	typedef short					level_t;				//�ȼ�
	typedef int						money_type_t;			//��������
	typedef Int64					gold_t;					//Ԫ��
	typedef unsigned short			monster_id_t;			//����id
	typedef UInt16					drop_id_t;				//����id
	typedef std::vector<drop_id_t>	drop_id_vec_t;
	typedef long long				exp_t;					//����
	typedef int						prestige_t;				//����
	typedef int						cross_honor_t;			//�������

	enum CONSUME_TYPE
	{
		CONSUME_TYPE_BIND_GOLD = 0,							//��Ԫ
		CONSUME_TYPE_GOLD = 1,								//Ԫ��

		CONSUME_TYPE_MAX
	};

	const static int DROP_LIST_MAX_COUNT = 128;				//�����б�
	const static int MAX_DROP_ITEM_COUNT = 400;				//��������Ʒ������Э��������
}


struct LittleHelperCfg
{
	LittleHelperCfg() : opengame_day(0), level_left(0), level_right(0), money_type(0), gold(0), monster_id(0), exp(0), prestige(0), cross_honor(0)
	{
	}
	lhns::opengame_day_t		opengame_day;
	lhns::level_t				level_left;
	lhns::level_t				level_right;
	TaskID						task_id;
	lhns::money_type_t			money_type;
	lhns::gold_t				gold;
	lhns::monster_id_t			monster_id;
	lhns::drop_id_vec_t			drop_id_vec;
	lhns::exp_t					exp;
	lhns::prestige_t			prestige;
	lhns::cross_honor_t			cross_honor;
	ItemConfigData				reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

class LittleHelperConfig : public ConfigBase
{
public:
	LittleHelperConfig();
	~LittleHelperConfig();

	bool Init(const std::string &configname, std::string *err);

	const LittleHelperCfg * GetHelperCfg(lhns::complete_type_t type, lhns::seq_t seq);
	const LittleHelperCfg * GetHelperCfgByLevel(lhns::complete_type_t type, lhns::level_t level);
	size_t GetLittleHelperCfgVecSize(lhns::complete_type_t type);
private:
	int InitHelperCfg(PugiXmlNode RootElement);

	typedef std::vector<LittleHelperCfg> LittleHelperCfgVec;
	typedef std::map<int, LittleHelperCfgVec> LittleHelperCfgVecInMap;
	LittleHelperCfgVecInMap m_little_helper_cfg_vec_in_map;
};


#endif // __LITTLE_HELPER_CONFIG_H__

