#ifndef __ROLE_OTHER_MGR_DEF_H__
#define __ROLE_OTHER_MGR_DEF_H__

static constexpr int OTHER_MODULE_COUNT = 1;

#define MODULE_FORWARD_DECLARATION()\
	class TianshenHuti;\

static_assert(1 == OTHER_MODULE_COUNT, "Are you forgot some code here?");

#define MODULE_GET_FUNCS_DECLARATION()\
	TianshenHuti *			GetTianshenHuti();\

static_assert(1 == OTHER_MODULE_COUNT, "Are you forgot some code here?");

#endif