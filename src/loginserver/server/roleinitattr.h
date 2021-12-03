#ifndef ROLEINITATTR_H
#define ROLEINITATTR_H

#include "servercommon/struct/roleinitparam.h"

#include <string.h>

class RoleInitAttr
{
public:
	static RoleInitAttr & Instance();

	static const int NEWBIE_SCENE_NUM = 16;

	struct RoleInitScene
	{
		RoleInitScene() : id(0), x(0), y(0), range(0) {}
		int id;
		int x;
		int y;
		int range;
	};

	bool Init(const std::string& path, const std::string& configname, std::string* err);

	const RoleInitParam & GetInitParam(int prof);

private:
	RoleInitAttr();
	~RoleInitAttr();

	RoleInitParam m_role_init_attr;

	RoleInitScene m_newbie_scene[NEWBIE_SCENE_NUM];
	unsigned short m_scenecount;
};

#endif
