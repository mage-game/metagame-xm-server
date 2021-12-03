#include "roleallparam.hpp"

bool RoleAllParam::Serialize(TLVSerializer &outstream, bool ver_check) const
{
	bool ret = (ver_check ? role_param.Serialize(outstream, ver_check) : role_param.Serialize(outstream));

	return ret && 
		other_param.Serialize(outstream) &&
		item_param.Serialize(outstream) &&
		hotkey_param.Serialize(outstream) &&
		friend_param.Serialize(outstream) &&
		black_param.Serialize(outstream) &&
		enemy_param.Serialize(outstream) &&
		business_param.Serialize(outstream) &&
		skill_param.Serialize(outstream) &&
		daily_find_param.Serialize(outstream);
}

bool RoleAllParam::Unserialize(TLVUnserializer &instream, bool ver_check)
{
	bool ret = (ver_check ? role_param.Unserialize(instream, ver_check) : role_param.Unserialize(instream));

	return ret && 
		other_param.Unserialize(instream) &&
		item_param.Unserialize(instream) &&
		hotkey_param.Unserialize(instream) &&
		friend_param.Unserialize(instream) &&
		black_param.Unserialize(instream) &&
		enemy_param.Unserialize(instream) &&
		business_param.Unserialize(instream) &&
		skill_param.Unserialize(instream) &&
		daily_find_param.Unserialize(instream); 
}

void RoleAllParam::Reset()
{
	memset(&role_param, 0, sizeof(role_param));
	other_param.Reset();

	item_param.count = 0;
	hotkey_param.count = 0;
	friend_param.count = 0;
	enemy_param.count = 0;
	business_param.count = 0;
	skill_param.count = 0;
	daily_find_param.count = 0;
}

