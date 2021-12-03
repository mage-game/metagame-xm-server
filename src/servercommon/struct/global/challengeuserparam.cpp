#include "challengeuserparam.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

OLD_USE_INTERNAL_NETWORK();

bool ChallengeUserListParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < CHALLENGE_FIELD_ONCE_OPER_USER_COUNT; ++i)
	{
		ret = outstream.Push(user_list[i].change_state) &&
			outstream.Push(user_list[i].rankpos) &&
			outstream.Push(user_list[i].role_id);
		if (!ret) 
		{
			return false;
		}

		if (user_list[i].change_state == structcommon::CS_DELETE) continue;

		{
			TLVSerializer challenge_data;
			challenge_data.Reset((void*)&user_list[i].challenge_p, sizeof(user_list[i].challenge_p));
			challenge_data.MoveCurPos(sizeof(user_list[i].challenge_p));

			TLVSerializer common_data;
			common_data.Reset((void*)&user_list[i].common_data, sizeof(user_list[i].common_data));
			common_data.MoveCurPos(sizeof(user_list[i].common_data));

			TLVSerializer report_info_data;
			report_info_data.Reset((void*)&user_list[i].report_info, sizeof(user_list[i].report_info));
			report_info_data.MoveCurPos(sizeof(user_list[i].report_info));

			TLVSerializer role_appearance_data;
			role_appearance_data.Reset((void*)&user_list[i].role_appearance, sizeof(user_list[i].role_appearance));
			role_appearance_data.MoveCurPos(sizeof(user_list[i].role_appearance));

			ret = outstream.Push(challenge_data) &&
				outstream.Push(common_data) &&
				outstream.Push(report_info_data) &&
				outstream.Push(role_appearance_data);
			if (!ret)
			{
				return false;
			}
		}
	}

	return true;
}

bool ChallengeUserListParam::Unserialize(TLVUnserializer &instream) 
{
	bool ret = instream.Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < CHALLENGE_FIELD_ONCE_OPER_USER_COUNT; ++i)
	{
		ret = instream.Pop(&user_list[i].change_state)
			&& instream.Pop(&user_list[i].rankpos)
			&& instream.Pop(&user_list[i].role_id);
		if (!ret) 
		{
			return false;
		}

		if (user_list[i].change_state == structcommon::CS_DELETE) continue;

		TLVUnserializer challenge_data;
		TLVUnserializer common_data;
		TLVUnserializer report_info_data;
		TLVUnserializer role_appearance_data;

		ret = instream.Pop(&challenge_data) &&
			instream.Pop(&common_data) &&
			instream.Pop(&report_info_data) &&
			instream.Pop(&role_appearance_data);
		if (!ret)
		{
			return false;
		}
		
		if (challenge_data.Size() <= sizeof(user_list[i].challenge_p))
		{
			memcpy(&user_list[i].challenge_p, challenge_data.Ptr(), challenge_data.Size());
			user_list[i].challenge_p.role_name[sizeof(user_list[i].challenge_p.role_name) - 1] = '\0';
		}

		if (common_data.Size() <= sizeof(user_list[i].common_data))
		{
			memcpy(&user_list[i].common_data, common_data.Ptr(), common_data.Size());
		}

		if (report_info_data.Size() <= sizeof(user_list[i].report_info))
		{
			memcpy(&user_list[i].report_info, report_info_data.Ptr(), report_info_data.Size());
		}

		if (role_appearance_data.Size() <= sizeof(user_list[i].role_appearance))
		{
			memcpy(&user_list[i].role_appearance, role_appearance_data.Ptr(), role_appearance_data.Size());
		}
	}

	return true;
}

