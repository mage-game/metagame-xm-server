#include "npcconfig.h"
#include "servercommon/configcommon.h"

#include "npc/npc.h"
#include "task/taskpool.h"
#include "monster/monsterpool.h"
#include "item/itempool.h"
#include "item/itembase.h"
#include "globalconfig/globalconfig.h"

#include "checkresourcecenter.hpp"

#include "other/daycounter/daycounter.hpp"

bool ConfigSceneNpc::Load(const std::string &configname, std::string &err)
{
	PugiXmlDocument m_document;
	char errinfo[1024];
	PUGI_XML_LOAD_AND_CHECK(m_document, configname, "ConfigSceneNpc", err);

	PugiXmlNode RootElement = m_document.document_element();

	if (0 == RootElement)
	{
		sprintf(errinfo,"Load npc config [%s] Error.cannot find root node.", configname.c_str());
		err = errinfo;
		return false;
	}

	int tmpnpcid;
	if (!PugiGetSubNodeValue(RootElement, "id", tmpnpcid ))
	{
		err = configname + ": xml node error in node [npc->id] ";
		return false;
	}
	if (tmpnpcid!=id)
	{
		sprintf(errinfo,"npc id are not same in [%s] and scene config xml.", configname.c_str());
		err = errinfo;
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "default_dialog_id", default_dialog_id))
	{
		err = configname + ": xml node error in node [npc->default_dialog_id] ";
		return false;
	}

	if (NPC::MAX_FUNCTION_NUM < (int)functions.size())
	{
		err = configname + ": config error, too many npc functions.";
		return false;
	}

	return true;
}

