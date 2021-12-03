#ifndef __LOTTERY_COUNT_CONFIG_HPP__
#define __LOTTERY_COUNT_CONFIG_HPP__

#include <map>
#include <vector>
#include <string>

#include "servercommon/rolelotterycountdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class Role;

struct LotteryCountReplaceSequenceCfg
{
	LotteryCountReplaceSequenceCfg() : can_replace_need_count_person(0), replace_need_count_person(0), replace_need_count_world(0)
	{
		memset(replace_sequence, 0, sizeof(replace_sequence));
	}
	UserLotteryCount can_replace_need_count_person;									//�ܹ��鵽��Ʒ��Ҫ�Ĵ��������ˣ�
	UserLotteryCount replace_need_count_person;										//�س鵽��Ҫ�Ĵ��������ˣ�
	UserLotteryCount replace_need_count_world;										//�س鵽��Ҫ�Ĵ�����ȫ����
	LotterySequence replace_sequence[LOTTERY_SEQUENCE_TYPE_MAX];					//�����滻������
};

class LotteryCountConfig : public ConfigBase
{
public:
	LotteryCountConfig();
	~LotteryCountConfig();

	bool Init(const std::string &configname, std::string *err);

	const LotteryCountReplaceSequenceCfg * GetReplaceListCfg(int type, int seq);

private:

	int InitReplaceListCfg(PugiXmlNode RootElement);
	int CheckReplaceSequence(int type, LotterySequence replace_sequence);

	LotteryCountReplaceSequenceCfg lottery_count_replace_sequence_cfg[LOTTERY_REPLACE_TYPE_MAX][MAX_LOTTERY_REPLACE_SEQUENCE_COUNT];
};

#endif // __LOTTERY_COUNT_CONFIG_HPP__

