#include "crossrecord.hpp"

#include "servercommon/internalprotocal/ggprotocal.h"
#include "world.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"

CrossRecord::CrossRecord() 
	: user_id(INVALID_USER_ID), level(0), avatar(0), sex(0), prof(0), camp(CAMP_TYPE_INVALID), order_view_match_index(-1)
{
	memset(role_name, 0, sizeof(role_name));
	user_record_data.Reset();
	user_appearance_data.Reset();
}

void CrossRecord::Init(const CrossRecordListParam::CrossRecordAttr &attr)
{
	user_id = IntToUid(attr.role_id);
	F_STRNCPY(role_name, attr.role_name, sizeof(GameName));
	level = attr.level;
	avatar = attr.avatar;
	sex = attr.sex;
	prof = attr.prof;
	camp = attr.camp;
	order_view_match_index = attr.order_view_match_index;
	user_record_data = attr.user_record_data;
	user_appearance_data = attr.user_appearance_data;
}

void CrossRecord::GetAttr(CrossRecordListParam::CrossRecordAttr *attr)
{
	attr->role_id = UidToInt(user_id);
	F_STRNCPY(attr->role_name, role_name, sizeof(GameName));
	attr->level = level;
	attr->avatar = avatar;
	attr->sex = sex;
	attr->prof = prof;
	attr->camp = camp;
	attr->order_view_match_index = static_cast<char>(order_view_match_index);
	attr->user_record_data = user_record_data;
	attr->user_appearance_data = user_appearance_data;
}

void CrossRecord::AddCrossMultiuserChallengeScore(int add_score)
{
	user_record_data.cross_multiuser_challenge_score += add_score;
	if (user_record_data.cross_multiuser_challenge_score < CROSS_MULTIUSER_CHALLENGE_MIN_SCORE)
	{
		user_record_data.cross_multiuser_challenge_score = CROSS_MULTIUSER_CHALLENGE_MIN_SCORE;
	}
}

