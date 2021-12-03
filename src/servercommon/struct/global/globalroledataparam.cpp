#include "globalroledataparam.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

#define SERIALIZE_GLOBAL_TOLE_DATA_PARAM(PARAM)						\
{																	\
	TLVSerializer tlv_serializer;									\
	tlv_serializer.Reset((void*)&PARAM, sizeof(PARAM));				\
	tlv_serializer.MoveCurPos(sizeof(PARAM));						\
	if (!outstream.Push(tlv_serializer))							\
	{																\
		return false;												\
	}																\
}																	

#define DESERIALIZE_GLOBAL_TOLE_DATA_PARAM(PARAM)					\
{																	\
	TLVUnserializer tlv_unserializer;								\
	if (!instream.Pop(&tlv_unserializer))							\
	{																\
		return false;												\
	}																\
	if (tlv_unserializer.Size() <= sizeof(PARAM))					\
	{																\
		memcpy(&PARAM, tlv_unserializer.Ptr(), tlv_unserializer.Size());	\
	}																\
}

bool GlobalRoleDatalListParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < GLOBAL_ROLE_DATA_ONCE_LOAD_MAX_COUNT; ++ i)
	{
		const RoleDataAttr &role_data_param = role_data_list[i];

		ret = outstream.Push(role_data_param.id_global_data) &&
			outstream.Push(role_data_param.change_state) && 
			outstream.Push(role_data_param.role_id);
		if (!ret)
		{
			return false;
		}

		if (structcommon::CS_DELETE == role_data_param.change_state) 
		{
			continue;
		}

		SERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.little_pet_param);
		SERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.love_tree_param);
		SERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.love_contract_param);
		SERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.jing_ling_home_param);
		SERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.fighting_data);
		SERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.qingyuan_global_param);
		SERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.role_appearance);
		SERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.common_data_global_param);
		SERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.spouse_home_param);
	}

	return true;
}

bool GlobalRoleDatalListParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret)
	{
		return false;
	}

	for (int i = 0; i < count && i < GLOBAL_ROLE_DATA_ONCE_LOAD_MAX_COUNT; ++ i)
	{
		RoleDataAttr &role_data_param = role_data_list[i];

		ret = instream.Pop(&role_data_param.id_global_data) &&
			instream.Pop(&role_data_param.change_state) &&
			instream.Pop(&role_data_param.role_id);
		if (!ret)
		{
			return false;
		}

		if (structcommon::CS_DELETE == role_data_param.change_state)
		{
			continue;
		}

		DESERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.little_pet_param);
		DESERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.love_tree_param);
		DESERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.love_contract_param);
		DESERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.jing_ling_home_param);
		DESERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.fighting_data);
		DESERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.qingyuan_global_param);
		DESERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.role_appearance);
		DESERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.common_data_global_param);
		DESERIALIZE_GLOBAL_TOLE_DATA_PARAM(role_data_param.spouse_home_param);
	}

	return true;
}

