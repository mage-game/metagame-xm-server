#include "sceneskill.hpp"
#include "sceneeffect.hpp"

#include "servercommon/configcommon.h"

// ---------------------------------------------------------------------------------------------------------------------------------------------
#define INIT_ROOT() \
	TiXmlDocument doc;\
	char filename_buf[512] = {0};\
	SNPRINTF(filename_buf, sizeof(filename_buf), "%s%d.xml", dir, this->GetSceneSkillId());\
	std::string filename(filename_buf);\
	if (!doc.LoadFile(filename))\
	{\
		*err=filename+" load fail.";\
		*err += doc.ErrorDesc();\
		return false;\
	}\
	TiXmlElement *root = doc.RootElement();\
	if (root == 0)\
	{\
		*err=filename+" load root fail: not root.";\
		return false;\
	}

#define GET_VALUE(NodeName, FieldName) \
	if (!GetSubNodeValue(root, NodeName, FieldName) )\
	{\
		*err=filename+" node["NodeName"] error.";\
		return false;\
	}

#define GET_PARAM(NodeName, FieldName) \
	if (!GetSubNodeValue(ParamElement, NodeName, FieldName) )\
	{\
		*err=filename+" node[param->"NodeName"] error.";\
		return false;\
	}

#define GET_CHECK_PARAM(NodeName, FieldName, MinValue) \
	if (!GetSubNodeValue(ParamElement, NodeName, FieldName) || FieldName < MinValue)\
	{\
		*err=filename+" node[param->"NodeName"] error. must >= "#MinValue;\
		return false;\
	}

#define CLEAR_ROOT()\
	root->Clear();

// ---------------------------------------------------------------------------------------------------------------------------------------------
