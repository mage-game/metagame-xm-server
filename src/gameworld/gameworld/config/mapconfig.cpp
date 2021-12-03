#include "mapconfig.h"
#include "servercommon/configcommon.h"

bool ConfigMap::Load(const std::string& configname)
{
	std::string err;
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "ConfigMap", err);

	PugiXmlNode RootElement = document.document_element();
	if (!RootElement)
	{
		lasterr = configname + ": xml node error in root";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "id", id))
	{
		lasterr = configname + ": xml node error in node [id] ";
		return false;
	}
	
	if (!PugiGetSubNodeValue(RootElement, "width", width))
	{
		lasterr = configname + ": xml node error in node [width] ";
		return false;
	}

	if (!PugiGetSubNodeValue(RootElement, "height", height))
	{
		lasterr = configname + ": xml node error in node [height] ";
		return false;
	}

	if (width <= 0 || height <= 0)
	{
		lasterr = configname + ": config error in node [height] and [width], low than 0 is not allowed.";
		return false;
	}

	// 处理mask，不可为空
	if (!PugiGetSubNodeValue(RootElement, "mask", mask ))
	{
		lasterr = configname + ": xml node error in node [mask] ";
		return false;
	}

	if (width*height != (int)mask.length())
	{
		lasterr = configname + ": xml node error: [width * height] not equal to [mask] ";
		return false;
	}

	return true;
}

