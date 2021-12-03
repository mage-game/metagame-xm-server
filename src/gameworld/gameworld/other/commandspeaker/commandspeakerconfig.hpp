#ifndef __COMMAND_SPEAKER_CONFIG_HPP__
#define __COMMAND_SPEAKER_CONFIG_HPP__

#include "gamedef.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"


struct CommandSpeakerOtherCfg
{
	CommandSpeakerOtherCfg() : consume(0), reward_role_limit(0), bind_gold_num(0)
	{}

	int consume;
	short reward_role_limit;
	short bind_gold_num;
};

class CommandSpeakerConfig : public ConfigBase
{
public:
	CommandSpeakerConfig();
	~CommandSpeakerConfig();

	bool Init(const std::string &configname, std::string *err);

	const CommandSpeakerOtherCfg * GetCommandSpeakerOtherCfg() { return &m_other_cfg; }

private:	
	int InitOtherConfig(PugiXmlNode RootElement);

	CommandSpeakerOtherCfg	m_other_cfg;
};


#endif //__COMMAND_SPEAKER_CONFIG_HPP__