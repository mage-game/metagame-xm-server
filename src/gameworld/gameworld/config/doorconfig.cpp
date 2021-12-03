#include "doorconfig.h"
#include "servercommon/configcommon.h"
#include "scene/sceneregister/sceneregister.hpp"

#include "checkresourcecenter.hpp"

bool ConfigSceneDoor::Load(const std::string &configname, std::string &err)
{
// 	PugiXmlDocument m_document;
// 	char errinfo[1024];
// 
// 	if (configname == "" || !m_document.LoadFile(configname) )
// 	{
// 		sprintf(errinfo,"Load door config [%s] Error.%s", configname.c_str(),m_document.ErrorDesc());
// 		err = errinfo;
// 		return false;
// 	}
// 
// 	PugiXmlNode RootElement = m_document.document_element();
// 
// 	if (0 == RootElement)
// 	{
// 		sprintf(errinfo,"Load door config [%s] Error.cannot find root node.", configname.c_str());
// 		err = errinfo;
// 		return false;
// 	}
// 
// 	int tmpnpcid = 0;
// 	if (!PugiGetSubNodeValue(RootElement, "id", tmpnpcid ))
// 	{
// 		err = configname + ": xml node error in node [door->id] ";
// 		return false;
// 	}
// 	if (tmpnpcid!=id)
// 	{
// 		sprintf(errinfo,"door id are not same in [%s] and scene config xml.", configname.c_str());
// 		err = errinfo;
// 		return false;
// 	}
// 
// 	if (!PugiGetSubNodeValue(RootElement, "level", level) || 0 > level)
// 	{
// 		err = configname + ": xml node error in node [door->level] ";
// 		return false;
// 	}
// 
// 	PugiXmlNode subElement = RootElement.child("targetscene");
// 	if (!PugiGetSubNodeValue(subElement, "sceneid", targetscene.sceneid))
// 	{
// 		err = configname + ": xml node error in node [door->targetscene->targetid] ";
// 		return false;
// 	}
// 	CheckResourceCenter::Instance().GetSceneCheck()->Add(targetscene.sceneid);
// 
// 	if (!PugiGetSubNodeValue(subElement, "x", targetscene.x))
// 	{
// 		err = configname + ": xml node error in node [door->targetscene->x] ";
// 		return false;
// 	}
// 	if (!PugiGetSubNodeValue(subElement, "y", targetscene.y))
// 	{
// 		err = configname + ": xml node error in node [door->targetscene->y] ";
// 		return false;
// 	}
// 
// 	if (!SceneRegister::Instance().AddSceneLevelLimit(targetscene.sceneid, level))
// 	{
// 		err = configname + ": xml node error door->level disaccord";
// 		return false;
// 	}

	return true;
}




