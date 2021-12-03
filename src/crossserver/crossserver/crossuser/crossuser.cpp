#include <memory.h>

#include "crossuser.h"

#include "servercommon/crossdef.hpp"

#include "engineadapter.h"
#include "internalcomm.h"

CrossUser::CrossUser(const UniqueUserID &orignal_unique_user_id, GameName original_user_name, const UserID &cross_user_id)
	: m_original_unique_user_id(orignal_unique_user_id), m_cross_user_id(cross_user_id)
{
	F_STRNCPY(m_original_user_name, original_user_name, sizeof(GameName));
}

CrossUser::~CrossUser()
{

}

