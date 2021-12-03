#include "rmiglobal.hpp"

#include "servercommon/darmi/daglobal.hpp"
#include "servercommon/darmi/damodule.h"
#include "servercommon/struct/structcommon.h"
#include "databaselayer.h"
#include "engineadapter.h"
#include "servercommon/serverclmempool.h"
#include "servercommon/servercommon.h"
#include "servercommon/guilddef.hpp"
#include "servercommon/tuodandef.hpp"
#include "servercommon/packetstream.hpp"
#include "servercommon/challengefielddef.hpp"
#include "dataaccesslog.hpp"
#include "common/bintohex.h"
#include "rmidef.hpp"

USE_DATAACCESS_VAR();
OLD_USE_INTERNAL_NETWORK();

extern char g_struct_code_buffer[MAX_STRUCT_CODE_BUFFER_LEN];

UNSTD_STATIC_CHECK(3 == MAX_ATTACHMENT_ITEM_NUM);

static short MAIL_ITEM_ID_DEF[] = {tabledef::MAIL_ITEM_ID1,tabledef::MAIL_ITEM_ID2,tabledef::MAIL_ITEM_ID3};
static short MAIL_ITEM_IS_BIND_DEF[] = {tabledef::MAIL_ITEM_IS_BIND1, tabledef::MAIL_ITEM_IS_BIND2, tabledef::MAIL_ITEM_IS_BIND3};
static short MAIL_ITEM_NUM_DEF[] = {tabledef::MAIL_ITEM_NUM1, tabledef::MAIL_ITEM_NUM2, tabledef::MAIL_ITEM_NUM3};
static short MAIL_ITEM_INVALID_TIME_DEF[] = {tabledef::MAIL_ITEM_INVALID_TIME1, tabledef::MAIL_ITEM_INVALID_TIME2, tabledef::MAIL_ITEM_INVALID_TIME3};
static short MAIL_ITEM_PARAM_DEF[] = {tabledef::MAIL_ITEM_PARAM1, tabledef::MAIL_ITEM_PARAM2, tabledef::MAIL_ITEM_PARAM3};

static short SYSMAIL_ITEM_ID_DEF[] = {tabledef::SYSTEMMAIL_ITEM_ID1,tabledef::SYSTEMMAIL_ITEM_ID2,tabledef::SYSTEMMAIL_ITEM_ID3};
static short SYSMAIL_ITEM_IS_BIND_DEF[] = {tabledef::SYSTEMMAIL_ITEM_IS_BIND1,tabledef::SYSTEMMAIL_ITEM_IS_BIND2,tabledef::SYSTEMMAIL_ITEM_IS_BIND3};
static short SYSMAIL_ITEM_NUM_DEF[] = {tabledef::SYSTEMMAIL_ITEM_NUM1, tabledef::SYSTEMMAIL_ITEM_NUM2, tabledef::SYSTEMMAIL_ITEM_NUM3};
static short SYSMAIL_ITEM_INVALID_TIME_DEF[] = {tabledef::SYSTEMMAIL_ITEM_INVALID_TIME1, tabledef::SYSTEMMAIL_ITEM_INVALID_TIME2, tabledef::SYSTEMMAIL_ITEM_INVALID_TIME3};
static short SYSMAIL_ITEM_PARAM_DEF[] = {tabledef::SYSTEMMAIL_ITEM_PARAM1, tabledef::SYSTEMMAIL_ITEM_PARAM2, tabledef::SYSTEMMAIL_ITEM_PARAM3};

RMIGlobalObject::RMIGlobalObject()
{
	m_method_list[dataaccessrmi::global::INIT_CAMP] = __InitCamp;
	m_method_list[dataaccessrmi::global::SAVE_CAMP] = __SaveCamp;

	m_method_list[dataaccessrmi::global::CREATE_GUILD] = __CreateGuild;
	m_method_list[dataaccessrmi::global::DESTROY_GUILD] = __DestroyGuild;
	m_method_list[dataaccessrmi::global::INIT_GUILD] = __InitGuild;
	m_method_list[dataaccessrmi::global::SAVE_GUILD] = __SaveGuild;

	m_method_list[dataaccessrmi::global::SAVE_PUBLIC_SALE] = __SavePublicSale;
	m_method_list[dataaccessrmi::global::INIT_PUBLIC_SALE] = __InitPublicSale;

	m_method_list[dataaccessrmi::global::INIT_MAILS] = __InitRoleMails;
	m_method_list[dataaccessrmi::global::SAVE_MAILS] = __SaveRoleMails;
	m_method_list[dataaccessrmi::global::GET_SYSTEM_MAILS] = __GetRoleSystemMails;
	m_method_list[dataaccessrmi::global::REMOVE_SYSTEM_MAILS] = __RemoveRoleSystemMails;

	m_method_list[dataaccessrmi::global::INIT_PERSON_RANK] = __InitPersonRank;
	m_method_list[dataaccessrmi::global::SAVE_PERSON_RANK] = __SavePersonRank;

	m_method_list[dataaccessrmi::global::INIT_TEAM_RANK] = __InitTeamRank;
	m_method_list[dataaccessrmi::global::SAVE_TEAM_RANK] = __SaveTeamRank;

	m_method_list[dataaccessrmi::global::INIT_TUODAN] = __InitTuodan;
	m_method_list[dataaccessrmi::global::SAVE_TUODAN] = __SaveTuodan;

	m_method_list[dataaccessrmi::global::INIT_WORLDSTATUS] = __InitWorldStatus;
	m_method_list[dataaccessrmi::global::SAVE_WORLDSTATUS] = __SaveWorldStatus;

	m_method_list[dataaccessrmi::global::INIT_OFFLINE_REGISTER] = __InitOfflineRegister;
	m_method_list[dataaccessrmi::global::SAVE_OFFLINE_REGISTER] = __SaveOfflineRegister;

	m_method_list[dataaccessrmi::global::INIT_SPACE_RECORD] = __InitSpaceRecord;
	m_method_list[dataaccessrmi::global::SAVE_SPACE_RECORD] = __SaveSpaceRecord;

	m_method_list[dataaccessrmi::global::INIT_CHALLENGE_USER] = __InitChallengeUser;
	m_method_list[dataaccessrmi::global::SAVE_CHALLENGE_USER] = __SaveChallengeUser;

	m_method_list[dataaccessrmi::global::INIT_FISH_POOL] = __InitFishPool;
	m_method_list[dataaccessrmi::global::SAVE_FISH_POOL] = __SaveFishPool;

	m_method_list[dataaccessrmi::global::INIT_CROSSRECORD] = __InitCrossRecord;
	m_method_list[dataaccessrmi::global::SAVE_CROSSRECORD] = __SaveCrossRecord;

	m_method_list[dataaccessrmi::global::INIT_GLOBAL_ROLE] = __InitGlobalRole;
	m_method_list[dataaccessrmi::global::SAVE_GLOBAL_ROLE] = __SaveGlobalRole;

	m_method_list[dataaccessrmi::global::INIT_WORLDSTATUS2] = __InitWorldStatus2;
	m_method_list[dataaccessrmi::global::SAVE_WORLDSTATUS2] = __SaveWorldStatus2;

	m_method_list[dataaccessrmi::global::INIT_WORLDSTATUS3] = __InitWorldStatus3;
	m_method_list[dataaccessrmi::global::SAVE_WORLDSTATUS3] = __SaveWorldStatus3;
}

RMIGlobalObject::~RMIGlobalObject()
{
}

void RMIGlobalObject::__free()
{
	delete this;
}

const char * RMIGlobalObject::__get_obj_id()
{
	return dataaccessrmi::GLOBAL;
}

int RMIGlobalObject::__InitCamp(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(CampParam) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitCamp_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static CampParam camp_param;
	camp_param.count = 0;

	int result = InitCamp(&camp_param);

	dataaccesslog::g_log_global.printf(LL_MAINTANCE, "__InitCamp ret:%d!", result);

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitCamp_2 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!camp_param.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitCamp_3 result:%d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitCamp_4 result:%d", result);
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::InitCamp(CampParam *camp_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *camp_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_CAMP);

	QueryCondition camp_condition;
	CmpNode camp_node1(tabledef::CAMP_CAMP_TYPE);
	camp_node1.SetInt32(0);
	camp_node1.SetEqualType(EQUAL_TYPE_GRANTER);
	camp_condition.cmp_list.push_back(camp_node1);
	camp_condition.query_num = CAMP_TYPE_MAX;

	DataAdapterList camp_list;
	int ret = camp_command->Find(stmt, camp_condition, &camp_list);
	if (ret < 0)
	{
		camp_param->count = 0;
		return -1;
	}

	ret = 0;

	int camp_count = 0;
	DataAdapterList::iterator i = camp_list.begin();

	for (; i != camp_list.end() && camp_count < CAMP_TYPE_MAX; ++i)
	{
		int camp_type = i->m_data_area[tabledef::CAMP_CAMP_TYPE].vint;

		CampBaseParam &base_info = camp_param->camp_list[camp_count].base_info;

		base_info.camp_type = camp_type;
		base_info.role_count = i->m_data_area[tabledef::CAMP_ROLE_COUNT].vint;
		base_info.guild_count = i->m_data_area[tabledef::CAMP_GUILD_COUNT].vint;
		base_info.rich_level = i->m_data_area[tabledef::CAMP_RICH_LEVEL].vint;
		base_info.earnings_end_time1 = (unsigned int)i->m_data_area[tabledef::CAMP_EARNINGS_END_TIME1].vint64;
		base_info.earnings1 = i->m_data_area[tabledef::CAMP_EARNINGS1].vint;
		base_info.earnings_end_time2 = (unsigned int)i->m_data_area[tabledef::CAMP_EARNINGS_END_TIME2].vint64;
		base_info.earnings2 = i->m_data_area[tabledef::CAMP_EARNINGS2].vint;
		base_info.total_capability = i->m_data_area[tabledef::CAMP_TOTAL_CAPABILITY].vint64;

		++camp_count;
		i->Free();
	}

	camp_param->count = camp_count;

	for (; i != camp_list.end(); ++i)
	{
		i->Free();
	}

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__SaveCamp(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveCamp_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static CampParam camp_param;
	camp_param.count = 0;

	if (!camp_param.Unserialize(in_param)) 
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveCamp_2");
		return rmi::DispatchParamError;
	}

	int result = SaveCamp(camp_param);

	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveCamp_3 result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveCamp_4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}
	
	return rmi::DispatchOK;
}

int RMIGlobalObject::SaveCamp(const CampParam &camp_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *camp_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_CAMP);

	conn->begin();

	{
		for (int i = 0; i < camp_param.count && i < CAMP_TYPE_MAX; i++)
		{
			const CampBaseParam &base_info = camp_param.camp_list[i].base_info;

			DataNodeList attr_list;
			attr_list.reserve(tabledef::TABLE_CAMP_MAXINDEX - 2);

			DataNode role_count(tabledef::CAMP_ROLE_COUNT);
			role_count.SetInt32(base_info.role_count);
			attr_list.push_back(role_count);

			DataNode total_capability(tabledef::CAMP_TOTAL_CAPABILITY);
			total_capability.SetLong64(base_info.total_capability);
			attr_list.push_back(total_capability);

			DataNode guild_count(tabledef::CAMP_GUILD_COUNT);
			guild_count.SetInt32(base_info.guild_count);
			attr_list.push_back(guild_count);

			DataNode rich_level(tabledef::CAMP_RICH_LEVEL);
			rich_level.SetInt32(base_info.rich_level);
			attr_list.push_back(rich_level);

			DataNode earnings_end_time1(tabledef::CAMP_EARNINGS_END_TIME1);
			earnings_end_time1.SetLong64(base_info.earnings_end_time1);
			attr_list.push_back(earnings_end_time1);

			DataNode earnings1(tabledef::CAMP_EARNINGS1);
			earnings1.SetInt32(base_info.earnings1);
			attr_list.push_back(earnings1);

			DataNode earnings_end_time2(tabledef::CAMP_EARNINGS_END_TIME2);
			earnings_end_time2.SetLong64(base_info.earnings_end_time2);
			attr_list.push_back(earnings_end_time2);

			DataNode earnings2(tabledef::CAMP_EARNINGS2);
			earnings2.SetInt32(base_info.earnings2);
			attr_list.push_back(earnings2);

			CmpNodeList cmp_node;

			CmpNode camp_type_node(tabledef::CAMP_CAMP_TYPE);
			camp_type_node.SetInt32(base_info.camp_type);
			cmp_node.push_back(camp_type_node);

			int count = camp_command->Update(stmt, cmp_node, attr_list);
			if (count <= 0) // 没有则插入数据
			{
				const MataTableDef *camp_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_CAMP);

				DataAdapter camp_item = camp_table->GetPrototype();
				camp_item.Malloc();

				camp_item.m_data_area[tabledef::CAMP_CAMP_TYPE].vint = base_info.camp_type;
				camp_item.m_data_area[tabledef::CAMP_ROLE_COUNT].vint = base_info.role_count;
				camp_item.m_data_area[tabledef::CAMP_GUILD_COUNT].vint = base_info.guild_count;
				camp_item.m_data_area[tabledef::CAMP_RICH_LEVEL].vint = base_info.rich_level;
				camp_item.m_data_area[tabledef::CAMP_EARNINGS_END_TIME1].vint64 = base_info.earnings_end_time1;
				camp_item.m_data_area[tabledef::CAMP_EARNINGS1].vint = base_info.earnings1;
				camp_item.m_data_area[tabledef::CAMP_EARNINGS_END_TIME2].vint64 = base_info.earnings_end_time2;
				camp_item.m_data_area[tabledef::CAMP_EARNINGS2].vint = base_info.earnings2;
				camp_item.m_data_area[tabledef::CAMP_TOTAL_CAPABILITY].vint64 = base_info.total_capability;

				if (DBCommand::RESULT_SUC != camp_command->Save(stmt, &camp_item))
				{
					camp_item.Free();
					conn->rollback();
					return -1;
				}

				camp_item.Free();
			}
		}
	}

	conn->commit();

	return 0;
}

int RMIGlobalObject::__CreateGuild(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__CreateGuild_1");
		return rmi::DispatchOutParamBuffTooShort;
	}
	
	int create_db_index = 0;
	if (!in_param.Pop(&create_db_index))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__CreateGuild_2");
		return rmi::DispatchParamError;
	} 

	static GuildBaseParam base_param;
	if (!base_param.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__CreateGuild_3");
		return rmi::DispatchParamError;
	}

	GuildID guild_id = 0; 

	int result = CreateGuild(create_db_index, base_param, &guild_id);

	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__CreateGuild_3 guild_id:%d result:%d", guild_id, result);
	}
	
	if (!out_param->Push(result) || !out_param->Push(guild_id))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__CreateGuild_4 guild_id:%d result:%d", guild_id, result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::CreateGuild(int create_db_index, const GuildBaseParam &base_param, GuildID *guild_id)
{
	INIT_STMT(conn, stmt, -1);

	conn->begin();

	const MataTableDef *guild_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_GUILD);
	DBCommand *guild_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_GUILD);

	DataAdapter guild_data = guild_table->GetPrototype();
	guild_data.Malloc();

	STRNCPY(guild_data.m_data_area[tabledef::GUILD_GUILD_NAME].vcharp, base_param.guild_name, guild_data.m_data_area[tabledef::GUILD_GUILD_NAME].length);
	guild_data.m_data_area[tabledef::GUILD_CREATE_TIME].vint64 = EngineAdapter::Instance().Time();

	guild_data.m_data_area[tabledef::GUILD_GUILD_LEVEL].vint = base_param.guild_level;
	guild_data.m_data_area[tabledef::GUILD_GUILD_EXP].vint = base_param.guild_exp;
	
	guild_data.m_data_area[tabledef::GUILD_CAMP].vint = base_param.camp;
	STRNCPY(guild_data.m_data_area[tabledef::GUILD_NOTICE].vcharp, base_param.notice, guild_data.m_data_area[tabledef::GUILD_NOTICE].length);
	guild_data.m_data_area[tabledef::GUILD_LAST_UPDATE_DAYID].vint64 = base_param.last_update_dayid;
	guild_data.m_data_area[tabledef::GUILD_LAST_ACTIVE_TIME].vint64 = base_param.last_active_time;
	guild_data.m_data_area[tabledef::GUILD_WARNING_DISMISS_DAYS].vint = base_param.warning_dismiss_days;
	guild_data.m_data_area[tabledef::GUILD_APPLYFOR_SETUP].vint = base_param.applyfor_setup;
	guild_data.m_data_area[tabledef::GUILD_LAST_UPLEVEL_TIMESTAMP].vint64 = base_param.guild_last_uplevel_timestamp;

	guild_data.m_data_area[tabledef::GUILD_AUTO_KICKOUT_SETUP].vint = base_param.auto_kickout_setup;
	guild_data.m_data_area[tabledef::GUILD_GUILD_MAX_CAPABILITY].vint = base_param.guild_max_capability;

	guild_data.m_data_area[tabledef::GUILD_APPLYFOR_NEED_CAPABILITY].vint = base_param.applyfor_need_capability;
	guild_data.m_data_area[tabledef::GUILD_APPLYFOR_NEED_LEVEL].vint = base_param.applyfor_need_level;

	guild_data.m_data_area[tabledef::GUILD_CALLIN_TIMES].vint = base_param.callin_times;
	guild_data.m_data_area[tabledef::GUILD_TODAY_MONSTER_FINISH_SEQ].vint = base_param.monster_finish_seq;
	guild_data.m_data_area[tabledef::GUILD_ACTIVE_DEGREE].vint = base_param.active_degree;
	STRNCPY(guild_data.m_data_area[tabledef::GUILD_TODAY_MONSTER_FINISH_USERNAME].vcharp, base_param.monster_finish_username, guild_data.m_data_area[tabledef::GUILD_TODAY_MONSTER_FINISH_USERNAME].length);

	// commondata
	{
		GuildCommonParamDataHex guild_common_data_hex; memset(guild_common_data_hex, 0, sizeof(guild_common_data_hex));

		BinToHex((const char *)&base_param.common_data_param, sizeof(base_param.common_data_param), guild_common_data_hex);
		guild_common_data_hex[sizeof(guild_common_data_hex) - 1] = '\0';
		STRNCPY(guild_data.m_data_area[tabledef::GUILD_COMMON_DATA].vcharp, guild_common_data_hex, guild_data.m_data_area[tabledef::GUILD_COMMON_DATA].length);
	}

	{
		GuildBossParamDataHex guild_boss_data_hex;

		BinToHex((const char *)&base_param.guild_boss_list, sizeof(base_param.guild_boss_list), guild_boss_data_hex);
		guild_boss_data_hex[sizeof(guild_boss_data_hex) - 1] = '\0';
		STRNCPY(guild_data.m_data_area[tabledef::GUILD_BOSS_LIST].vcharp, guild_boss_data_hex, guild_data.m_data_area[tabledef::GUILD_BOSS_LIST].length);
	}

	{
		GuildStorgeDataHex storge_item_list_data_hex;

		BinToHex((const char *)&base_param.storge_item_list, sizeof(base_param.storge_item_list), storge_item_list_data_hex);
		storge_item_list_data_hex[sizeof(storge_item_list_data_hex) - 1] = '\0';
		STRNCPY(guild_data.m_data_area[tabledef::GUILD_STORGE].vcharp, storge_item_list_data_hex, guild_data.m_data_area[tabledef::GUILD_STORGE].length);
	}

	{
		GuildEnemyInfoHex guild_enemy_list_data_hex;

		BinToHex((const char *)&base_param.guild_enemy_list, sizeof(base_param.guild_enemy_list), guild_enemy_list_data_hex);
		guild_enemy_list_data_hex[sizeof(guild_enemy_list_data_hex) - 1] = '\0';
		STRNCPY(guild_data.m_data_area[tabledef::GUILD_ENEMY_LIST].vcharp, guild_enemy_list_data_hex, guild_data.m_data_area[tabledef::GUILD_ENEMY_LIST].length);
	}

	if (DBCommand::RESULT_SUC != guild_command->Save(stmt, &guild_data))
	{
		guild_data.Free();
		conn->rollback();
		return -1;
	}
	
	int guild_id_tmp = (int)guild_data.m_data_area[tabledef::GUILD_GUILD_ID_INC].vint64;
	
	UserID guild_user_id(create_db_index, guild_id_tmp);
	int real_guild_id = UidToInt(guild_user_id);

	guild_data.m_data_area[tabledef::GUILD_GUILD_ID].vint = real_guild_id;
	if (DBCommand::RESULT_SUC != guild_command->Update(stmt, guild_data))
	{
		guild_data.Free();
		conn->rollback();
		return -2;
	}

	*guild_id = real_guild_id;

	guild_data.Free();

	conn->commit();

	return 0;
}

int RMIGlobalObject::__DestroyGuild(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__DestroyGuild_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	GuildID guild_id = 0;

	if (!in_param.Pop(&guild_id))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__DestroyGuild_2");
		return rmi::DispatchParamError;
	}

	int result = DestoryGuild(guild_id);

	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__DestroyGuild_3 guild_id:%d result:%d", guild_id, result);
	}	

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__DestroyGuild_4 guild_id:%d result:%d", guild_id, result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::DestoryGuild(GuildID guild_id)
{	
	INIT_STMT(conn, stmt, -1);

	conn->begin();

	{
		CmpNodeList guild_cmp_list;
		CmpNode guild_node(tabledef::GUILD_GUILD_ID);
		guild_node.SetInt32(guild_id);
		guild_cmp_list.push_back(guild_node);

		DBCommand *guild_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_GUILD);
		int count = guild_command->Remove(stmt, guild_cmp_list);
		if (count < 1)
		{
			conn->rollback();
			return -1;
		}
	}

	{
		CmpNodeList apply_cmp_list;
		CmpNode apply_node(tabledef::GUILDAPPLYFOR_GUILD_ID);
		apply_node.SetInt32(guild_id);
		apply_cmp_list.push_back(apply_node);

		DBCommand *apply_commond = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_GUILDAPPLYFOR);
		int count = apply_commond->Remove(stmt, apply_cmp_list);
		if (count < 0)
		{
			conn->rollback();
			return -2;
		}
	}

	{
		CmpNodeList member_cmp_list;
		CmpNode member_node(tabledef::GUILDMEMBER_GUILD_ID);
		member_node.SetInt32(guild_id);
		member_cmp_list.push_back(member_node);

		DBCommand *member_commond = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_GUILDMEMBER);
		int count = member_commond->Remove(stmt, member_cmp_list);
		if (count < 0)
		{
			conn->rollback();
			return -3;
		}
	}

	{
		CmpNodeList event_cmp_list;
		CmpNode event_node(tabledef::GUILDEVENT_GUILD_ID);
		event_node.SetInt32(guild_id);
		event_cmp_list.push_back(event_node);

		DBCommand *event_commond = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_GUILDEVENT);
		int count = event_commond->Remove(stmt, event_cmp_list);
		if (count < 0)
		{
			conn->rollback();
			return -4;
		}
	}

	conn->commit();
	
	return 0;
}

int RMIGlobalObject::__InitGuild(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(AllGuildParam) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitGuild_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	GuildID guild_id_from = 0;

	if (!in_param.Pop(&guild_id_from))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitGuild_2");
		return rmi::DispatchParamError;
	}

	static AllGuildParam guild_list;
	guild_list.count = 0;

	int result = InitGuild(guild_id_from, &guild_list);

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitGuild_3 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!guild_list.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitGuild_4 result:%d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitGuild_5 result:%d", result);
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::InitGuild(GuildID guild_id_from, AllGuildParam *guild_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *guild_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_GUILD);

	QueryCondition guild_condition;
	CmpNode guild_attr_node1(tabledef::GUILD_GUILD_ID);
	guild_attr_node1.SetInt32(guild_id_from);
	guild_attr_node1.SetEqualType(EQUAL_TYPE_GRANTER);
	guild_condition.cmp_list.push_back(guild_attr_node1);
	guild_condition.query_num = MAX_ONCE_GUILD_COUNT;
	
	DataAdapterList guild_list;
	int ret = guild_command->Find(stmt, guild_condition, &guild_list);
	if (ret < 0)
	{
		guild_param->count = 0;
		return -1;
	}
	
	ret = 0;

	int count = 0;
	DataAdapterList::iterator i = guild_list.begin();
	for (; i != guild_list.end() && count < MAX_ONCE_GUILD_COUNT; ++i)
	{
		GuildID guild_id = i->m_data_area[tabledef::GUILD_GUILD_ID].vint;

		guild_param->guild_list[count].base_param.guild_id = guild_id;
		STRNCPY(guild_param->guild_list[count].base_param.guild_name, i->m_data_area[tabledef::GUILD_GUILD_NAME].vcharp, sizeof(guild_param->guild_list[0].base_param.guild_name));
		guild_param->guild_list[count].base_param.create_time = i->m_data_area[tabledef::GUILD_CREATE_TIME].vint64;
		guild_param->guild_list[count].base_param.guild_level = i->m_data_area[tabledef::GUILD_GUILD_LEVEL].vint;
		guild_param->guild_list[count].base_param.guild_exp = i->m_data_area[tabledef::GUILD_GUILD_EXP].vint;
		
		guild_param->guild_list[count].base_param.camp = i->m_data_area[tabledef::GUILD_CAMP].vint;
		STRNCPY(guild_param->guild_list[count].base_param.notice, i->m_data_area[tabledef::GUILD_NOTICE].vcharp,
			sizeof(guild_param->guild_list[0].base_param.notice));
		guild_param->guild_list[count].base_param.last_update_dayid = static_cast<unsigned int>(i->m_data_area[tabledef::GUILD_LAST_UPDATE_DAYID].vint64);
		guild_param->guild_list[count].base_param.last_active_time = static_cast<unsigned int>(i->m_data_area[tabledef::GUILD_LAST_ACTIVE_TIME].vint64);
		guild_param->guild_list[count].base_param.warning_dismiss_days = i->m_data_area[tabledef::GUILD_WARNING_DISMISS_DAYS].vint;
		guild_param->guild_list[count].base_param.applyfor_setup = i->m_data_area[tabledef::GUILD_APPLYFOR_SETUP].vint;
		guild_param->guild_list[count].base_param.guild_last_uplevel_timestamp = static_cast<unsigned int>(i->m_data_area[tabledef::GUILD_LAST_UPLEVEL_TIMESTAMP].vint64);

		guild_param->guild_list[count].base_param.auto_kickout_setup = i->m_data_area[tabledef::GUILD_AUTO_KICKOUT_SETUP].vint;
		guild_param->guild_list[count].base_param.guild_max_capability = i->m_data_area[tabledef::GUILD_GUILD_MAX_CAPABILITY].vint;
		guild_param->guild_list[count].base_param.applyfor_need_capability = i->m_data_area[tabledef::GUILD_APPLYFOR_NEED_CAPABILITY].vint;
		guild_param->guild_list[count].base_param.applyfor_need_level = i->m_data_area[tabledef::GUILD_APPLYFOR_NEED_LEVEL].vint;
		guild_param->guild_list[count].base_param.callin_times = i->m_data_area[tabledef::GUILD_CALLIN_TIMES].vint;
		guild_param->guild_list[count].base_param.monster_finish_seq = i->m_data_area[tabledef::GUILD_TODAY_MONSTER_FINISH_SEQ].vint;
		guild_param->guild_list[count].base_param.active_degree = i->m_data_area[tabledef::GUILD_ACTIVE_DEGREE].vint;

		STRNCPY(guild_param->guild_list[count].base_param.monster_finish_username, i->m_data_area[tabledef::GUILD_TODAY_MONSTER_FINISH_USERNAME].vcharp,
			sizeof(guild_param->guild_list[0].base_param.monster_finish_username));

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		{
			guild_param->guild_list[count].base_param.common_data_param.Reset();

			int length = i->m_data_area[tabledef::GUILD_COMMON_DATA].length;
			if (length <= (int)sizeof(guild_param->guild_list[count].base_param.common_data_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::GUILD_COMMON_DATA].vcharp, length, (char*)&guild_param->guild_list[count].base_param.common_data_param);
			}
		}

		{
			memset(guild_param->guild_list[count].base_param.guild_boss_list, 0, sizeof(guild_param->guild_list[count].base_param.guild_boss_list));

			int length = i->m_data_area[tabledef::GUILD_BOSS_LIST].length;
			if (length <= (int)sizeof(guild_param->guild_list[count].base_param.guild_boss_list) * 2)
			{
				HexToBin(i->m_data_area[tabledef::GUILD_BOSS_LIST].vcharp, length, (char*)&guild_param->guild_list[count].base_param.guild_boss_list);
			}
		}

		{
			memset(guild_param->guild_list[count].base_param.storge_item_list, 0, sizeof(guild_param->guild_list[count].base_param.storge_item_list));

			int length = i->m_data_area[tabledef::GUILD_STORGE].length;
			if (length <= (int)sizeof(guild_param->guild_list[count].base_param.storge_item_list) * 2)
			{
				HexToBin(i->m_data_area[tabledef::GUILD_STORGE].vcharp, length, (char*)&guild_param->guild_list[count].base_param.storge_item_list);
			}
		}

		{
			memset(guild_param->guild_list[count].base_param.guild_enemy_list, 0, sizeof(guild_param->guild_list[count].base_param.guild_enemy_list));

			int length = i->m_data_area[tabledef::GUILD_ENEMY_LIST].length;
			if (length <= (int)sizeof(guild_param->guild_list[count].base_param.guild_enemy_list) * 2)
			{
				HexToBin(i->m_data_area[tabledef::GUILD_ENEMY_LIST].vcharp, length, (char*)&guild_param->guild_list[count].base_param.guild_enemy_list);
			}
		}

		{
			DBCommand *apply_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_GUILDAPPLYFOR);

			QueryCondition apply_condition;
			CmpNode applynode1(tabledef::GUILDAPPLYFOR_GUILD_ID);
			applynode1.SetInt32(guild_id);
			apply_condition.cmp_list.push_back(applynode1);

			DataAdapterList apply_list;
			ret = apply_command->Find(stmt, apply_condition, &apply_list);
			if (ret < 0)
			{
				guild_param->guild_list[count].apply_for_param.count = 0;
				ret = -2;
				break;
			}

			int apply_count = 0;
			DataAdapterList::iterator i_apply = apply_list.begin();

			for (; apply_count < MAX_APPLY_FOR_COUNT && i_apply != apply_list.end(); ++i_apply, ++apply_count)
			{
				guild_param->guild_list[count].apply_for_param.apply_for_list[apply_count].change_state = structcommon::CS_NONE;
				guild_param->guild_list[count].apply_for_param.apply_for_list[apply_count].index = i_apply->m_data_area[tabledef::GUILDAPPLYFOR_APPLY_FOR_INDEX].vint;
				guild_param->guild_list[count].apply_for_param.apply_for_list[apply_count].uid = i_apply->m_data_area[tabledef::GUILDAPPLYFOR_UID].vint;
				guild_param->guild_list[count].apply_for_param.apply_for_list[apply_count].applyfor_time = (unsigned int)i_apply->m_data_area[tabledef::GUILDAPPLYFOR_APPLYFOR_TIME].vint64;

				i_apply->Free();
			}

			for (; i_apply != apply_list.end(); ++i_apply)
			{
				i_apply->Free();
			}

			guild_param->guild_list[count].apply_for_param.count = apply_count;
		}

		{
			DBCommand *member_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_GUILDMEMBER);

			QueryCondition member_condition;
			CmpNode membernode1(tabledef::GUILDMEMBER_GUILD_ID);
			membernode1.SetInt32(guild_id);
			member_condition.cmp_list.push_back(membernode1);

			DataAdapterList member_list;
			ret = member_command->Find(stmt, member_condition, &member_list);
			if (ret < 0)
			{
				guild_param->guild_list[count].member_param.count = 0;
				ret = -3;
				break;
			}

			int member_count = 0;
			DataAdapterList::iterator i_member = member_list.begin();

			for (; member_count < MAX_MEMBER_COUNT && i_member != member_list.end(); ++i_member, ++member_count)
			{
				guild_param->guild_list[count].member_param.member_list[member_count].change_state = structcommon::CS_NONE;
				guild_param->guild_list[count].member_param.member_list[member_count].index = i_member->m_data_area[tabledef::GUILDMEMBER_MEMBER_INDEX].vint;
				guild_param->guild_list[count].member_param.member_list[member_count].uid = i_member->m_data_area[tabledef::GUILDMEMBER_UID].vint;
		
				guild_param->guild_list[count].member_param.member_list[member_count].post = i_member->m_data_area[tabledef::GUILDMEMBER_POST].vint;

				guild_param->guild_list[count].member_param.member_list[member_count].gongxian = i_member->m_data_area[tabledef::GUILDMEMBER_GONGXIAN].vint;
				guild_param->guild_list[count].member_param.member_list[member_count].total_gongxian = i_member->m_data_area[tabledef::GUILDMEMBER_TOTAL_GONGXIAN].vint;

				guild_param->guild_list[count].member_param.member_list[member_count].join_time = i_member->m_data_area[tabledef::GUILDMEMBER_JOIN_TIME].vint64;
				guild_param->guild_list[count].member_param.member_list[member_count].last_login_time = i_member->m_data_area[tabledef::GUILDMEMBER_LAST_LOGIN_TIME].vint64;

				guild_param->guild_list[count].member_param.member_list[member_count].lucky_color = i_member->m_data_area[tabledef::GUILDMEMBER_LUCKY_COLOR].vint;

				guild_param->guild_list[count].member_param.member_list[member_count].today_guild_pao_saizi_times = i_member->m_data_area[tabledef::GUILDMEMBER_TODAY_GUILD_PAO_SAIZI_TIMES].vint;
				guild_param->guild_list[count].member_param.member_list[member_count].today_last_guild_pao_saizi_time = i_member->m_data_area[tabledef::GUILDMEMBER_TODAY_LAST_GUILD_PAO_SAIZI_TIME].vint64;
				guild_param->guild_list[count].member_param.member_list[member_count].today_guild_saizi_score = i_member->m_data_area[tabledef::GUILDMEMBER_TODAY_GUILD_SAIZI_SCORE].vint;

				guild_param->guild_list[count].member_param.member_list[member_count].maze_layer = i_member->m_data_area[tabledef::GUILDMEMBER_MAZE_LAYER].vint;
				guild_param->guild_list[count].member_param.member_list[member_count].maze_complete_time = i_member->m_data_area[tabledef::GUILDMEMBER_MAZE_COMPLETE_TIME].vint64;
				GET_BINARY_DATA((*i_member), tabledef::GUILDMEMBER_COMMON_DATA, guild_param->guild_list[count].member_param.member_list[member_count].common_data);
				
				i_member->Free();
			}

			for (; i_member != member_list.end(); ++i_member)
			{
				i_member->Free();
			}

			guild_param->guild_list[count].member_param.count = member_count;
		}

		{
			DBCommand *event_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_GUILDEVENT);

			QueryCondition event_condition;
			CmpNode eventnode1(tabledef::GUILDEVENT_GUILD_ID);
			eventnode1.SetInt32(guild_id);
			event_condition.cmp_list.push_back(eventnode1);

			DataAdapterList event_list;
			ret = event_command->Find(stmt, event_condition, &event_list);
			if (ret < 0)
			{
				guild_param->guild_list[count].event_param.count = 0;
				ret = -4;
				break;
			}

			int event_count = 0;
			DataAdapterList::iterator i_event = event_list.begin();

			for (; event_count < MAX_ALL_GUILD_EVENT_COUNT && i_event != event_list.end(); ++i_event, ++event_count)
			{
				guild_param->guild_list[count].event_param.event_list[event_count].change_state = structcommon::CS_NONE;
				guild_param->guild_list[count].event_param.event_list[event_count].index = i_event->m_data_area[tabledef::GUILDEVENT_EVENT_INDEX].vint;
				guild_param->guild_list[count].event_param.event_list[event_count].event_type = i_event->m_data_area[tabledef::GUILDEVENT_EVENT_TYPE].vint;
				STRNCPY(guild_param->guild_list[count].event_param.event_list[event_count].event_owner, i_event->m_data_area[tabledef::GUILDEVENT_EVENT_OWNER].vcharp,
					sizeof(guild_param->guild_list[0].event_param.event_list[0].event_owner));
				guild_param->guild_list[count].event_param.event_list[event_count].event_owner_post = i_event->m_data_area[tabledef::GUILDEVENT_EVENT_OWNER_POST].vint;
				guild_param->guild_list[count].event_param.event_list[event_count].event_time = static_cast<unsigned int>(i_event->m_data_area[tabledef::GUILDEVENT_EVENT_TIME].vint64);
				guild_param->guild_list[count].event_param.event_list[event_count].big_event = i_event->m_data_area[tabledef::GUILDEVENT_BIG_EVENT].vint;
				guild_param->guild_list[count].event_param.event_list[event_count].cannot_remove = i_event->m_data_area[tabledef::GUILDEVENT_CANNOT_REMOVE].vint;
				guild_param->guild_list[count].event_param.event_list[event_count].param0 = i_event->m_data_area[tabledef::GUILDEVENT_PARAM0].vint;
				guild_param->guild_list[count].event_param.event_list[event_count].param1 = i_event->m_data_area[tabledef::GUILDEVENT_PARAM1].vint;
				guild_param->guild_list[count].event_param.event_list[event_count].param2 = i_event->m_data_area[tabledef::GUILDEVENT_PARAM2].vint;
				guild_param->guild_list[count].event_param.event_list[event_count].param3 = i_event->m_data_area[tabledef::GUILDEVENT_PARAM3].vint;
				STRNCPY(guild_param->guild_list[count].event_param.event_list[event_count].sparam0, i_event->m_data_area[tabledef::GUILDEVENT_SPARAM0].vcharp,
					sizeof(guild_param->guild_list[0].event_param.event_list[0].sparam0));

				i_event->Free();
			}

			for (; i_event != event_list.end(); ++i_event)
			{
				i_event->Free();
			}

			guild_param->guild_list[count].event_param.count = event_count;
		}

		++count;
		i->Free();
	}

	guild_param->count = count;

	for (; i != guild_list.end(); ++i)
	{
		i->Free();
	}

	return ret < 0 ? ret : 0;
}	

int RMIGlobalObject::__SaveGuild(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveGuild_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	GuildID guild_id = 0;
	static GuildBaseParam base_param;
	static GuildApplyForParam apply_for_param;
	static GuildMemberParam member_param;
	static GuildEventParam event_param;

	if (!in_param.Pop(&guild_id) || !base_param.Unserialize(in_param) || !apply_for_param.Unserialize(in_param) || 
		!member_param.Unserialize(in_param) || !event_param.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveGuild_2");
		return rmi::DispatchParamError;
	}

	int result = SaveGuild(guild_id, base_param, apply_for_param, member_param, event_param);

	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveGuild_3 result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveGuild_4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::SaveGuild(GuildID guild_id, 
							   const GuildBaseParam &base_param, 
							   const GuildApplyForParam &apply_for_param,
							   const GuildMemberParam &member_param, 
							   const GuildEventParam &event_param
							   )
{
	INIT_STMT(conn, stmt, -1);

	conn->begin();

	int ret = 0;

	{
		DBCommand *guild_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_GUILD);

		DataNodeList node_list;
		node_list.reserve(tabledef::TABLE_GUILD_MAXINDEX - 1);

		DataNode name_node(tabledef::GUILD_GUILD_NAME);
		name_node.SetString(base_param.guild_name);
		node_list.push_back(name_node);

		DataNode level(tabledef::GUILD_GUILD_LEVEL);
		level.SetInt32(base_param.guild_level);
		node_list.push_back(level);

		DataNode exp(tabledef::GUILD_GUILD_EXP);
		exp.SetInt32(base_param.guild_exp);
		node_list.push_back(exp);

		DataNode camp(tabledef::GUILD_CAMP);
		camp.SetInt32(base_param.camp);
		node_list.push_back(camp);

		DataNode notice(tabledef::GUILD_NOTICE);
		notice.SetString(base_param.notice);
		node_list.push_back(notice);

		DataNode last_update_dayid(tabledef::GUILD_LAST_UPDATE_DAYID);
		last_update_dayid.SetLong64(base_param.last_update_dayid);
		node_list.push_back(last_update_dayid);

		DataNode last_uplevel_dayid(tabledef::GUILD_LAST_UPLEVEL_TIMESTAMP);
		last_uplevel_dayid.SetLong64(base_param.guild_last_uplevel_timestamp);
		node_list.push_back(last_uplevel_dayid);

		DataNode last_active_time(tabledef::GUILD_LAST_ACTIVE_TIME);
		last_active_time.SetLong64(base_param.last_active_time);
		node_list.push_back(last_active_time);

		UPDATE_INT_DATA_NODE(warning_dismiss_node, tabledef::GUILD_WARNING_DISMISS_DAYS, base_param.warning_dismiss_days);
		UPDATE_INT_DATA_NODE(applyfor_setup_node, tabledef::GUILD_APPLYFOR_SETUP, base_param.applyfor_setup);

		UPDATE_INT_DATA_NODE(applyfor_need_capability_node, tabledef::GUILD_APPLYFOR_NEED_CAPABILITY, base_param.applyfor_need_capability);
		UPDATE_INT_DATA_NODE(applyfor_need_level_node, tabledef::GUILD_APPLYFOR_NEED_LEVEL, base_param.applyfor_need_level);
		UPDATE_INT_DATA_NODE(callin_times_node, tabledef::GUILD_CALLIN_TIMES, base_param.callin_times);
		UPDATE_INT_DATA_NODE(monster_finish_seq_node, tabledef::GUILD_TODAY_MONSTER_FINISH_SEQ, base_param.monster_finish_seq);
		UPDATE_INT_DATA_NODE(monster_finish_seq_node, tabledef::GUILD_ACTIVE_DEGREE, base_param.active_degree);
		UPDATE_STRING_DATA_NODE(monster_finish_username_node, tabledef::GUILD_TODAY_MONSTER_FINISH_USERNAME, base_param.monster_finish_username);
		UPDATE_BINARY_DATA_TO_LIST(common_data_node, tabledef::GUILD_COMMON_DATA, base_param.common_data_param, GuildCommonParamDataHex, node_list);
		UPDATE_BINARY_DATA_TO_LIST(boss_list, tabledef::GUILD_BOSS_LIST, base_param.guild_boss_list, GuildBossParamDataHex, node_list);
		UPDATE_BINARY_DATA_TO_LIST(storge, tabledef::GUILD_STORGE, base_param.storge_item_list, GuildStorgeDataHex, node_list);
		UPDATE_BINARY_DATA_TO_LIST(storge, tabledef::GUILD_ENEMY_LIST, base_param.guild_enemy_list, GuildEnemyInfoHex, node_list);

		DataNode auto_kickout_setup(tabledef::GUILD_AUTO_KICKOUT_SETUP);
		auto_kickout_setup.SetInt32(base_param.auto_kickout_setup);
		node_list.push_back(auto_kickout_setup);

		DataNode guild_max_capability(tabledef::GUILD_GUILD_MAX_CAPABILITY);
		guild_max_capability.SetInt32(base_param.guild_max_capability);
		node_list.push_back(guild_max_capability);

		CmpNodeList condition;
		CmpNode id_node(tabledef::GUILD_GUILD_ID);
		id_node.SetInt32(guild_id);
		condition.push_back(id_node);

		int count = guild_command->Update(stmt, condition, node_list);
		if (count < 1)
		{
			ret = -1;
		}
	}
	
	{
		DBCommand *apply_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_GUILDAPPLYFOR);
		for (int i = 0; i < apply_for_param.count; ++i)
		{
			if (apply_for_param.apply_for_list[i].change_state == structcommon::CS_NONE) continue;

			long long guildapply_for_index = TranslateInt64(guild_id, apply_for_param.apply_for_list[i].index);

			switch (apply_for_param.apply_for_list[i].change_state)
			{
			case structcommon::CS_DELETE:
				{
					CmpNodeList cmp_node;

					CmpNode for_index_node(tabledef::GUILDAPPLYFOR_FOR_INDEX);
					for_index_node.SetLong64(guildapply_for_index);
					cmp_node.push_back(for_index_node);

					int count = apply_command->Remove(stmt, cmp_node);
					if (count < 1)
					{
						ret = -2;
					}
				}
				break;

			case structcommon::CS_UPDATE:
				{
					DataNodeList attr_list;
					attr_list.reserve(tabledef::TABLE_GUILDAPPLYFOR_MAXINDEX - 4);

					DataNode uid(tabledef::GUILDAPPLYFOR_UID);
					uid.SetInt32(apply_for_param.apply_for_list[i].uid);
					attr_list.push_back(uid);

					DataNode applyfor_time(tabledef::GUILDAPPLYFOR_APPLYFOR_TIME);
					applyfor_time.SetLong64(apply_for_param.apply_for_list[i].applyfor_time);
					attr_list.push_back(applyfor_time);

					CmpNodeList cmp_node;

					CmpNode for_index_node(tabledef::GUILDAPPLYFOR_FOR_INDEX);
					for_index_node.SetLong64(guildapply_for_index);
					cmp_node.push_back(for_index_node);

					int count = apply_command->Update(stmt, cmp_node, attr_list);
					if (count < 1)
					{
						ret = -3;
						// 此时不break，update不成功时则继续insert，以兼容某些错误
					}
					else 
					{
						break;
					}
				}

			case  structcommon::CS_INSERT:
				{
					const MataTableDef *apply_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_GUILDAPPLYFOR);

					DataAdapter apply_item = apply_table->GetPrototype();
					apply_item.Malloc();

					apply_item.m_data_area[tabledef::GUILDAPPLYFOR_GUILD_ID].vint = guild_id;
					apply_item.m_data_area[tabledef::GUILDAPPLYFOR_APPLY_FOR_INDEX].vint = apply_for_param.apply_for_list[i].index;
					apply_item.m_data_area[tabledef::GUILDAPPLYFOR_FOR_INDEX].vint64 = guildapply_for_index;
					apply_item.m_data_area[tabledef::GUILDAPPLYFOR_UID].vint = apply_for_param.apply_for_list[i].uid;
					apply_item.m_data_area[tabledef::GUILDAPPLYFOR_APPLYFOR_TIME].vint64 = apply_for_param.apply_for_list[i].applyfor_time;

					if (DBCommand::RESULT_SUC != apply_command->Save(stmt, &apply_item))
					{
						ret = -4;
					}

					apply_item.Free();
				}
				break;
			}
		}
	}

	{
		DBCommand *member_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_GUILDMEMBER);

		for (int i = 0; i < member_param.count; ++i)
		{
			if (member_param.member_list[i].change_state == structcommon::CS_NONE) continue;

			long long guildmember_for_index = TranslateInt64(guild_id, member_param.member_list[i].index);

			switch (member_param.member_list[i].change_state)
			{
			case structcommon::CS_DELETE:
				{
					CmpNodeList cmp_node;

					CmpNode for_index_node(tabledef::GUILDMEMBER_FOR_INDEX);
					for_index_node.SetLong64(guildmember_for_index);
					cmp_node.push_back(for_index_node);

					int count = member_command->Remove(stmt, cmp_node);
					if (count < 1)
					{
						ret = -5;
					}
				}
				break;

			case structcommon::CS_UPDATE:
				{
					DataNodeList node_list;
					node_list.reserve(tabledef::TABLE_GUILDMEMBER_MAXINDEX - 4);

					DataNode uid(tabledef::GUILDMEMBER_UID);
					uid.SetInt32(member_param.member_list[i].uid);
					node_list.push_back(uid);

					DataNode post(tabledef::GUILDMEMBER_POST);
					post.SetInt32(member_param.member_list[i].post);
					node_list.push_back(post);
				
					UPDATE_INT_DATA_NODE(gongxian_node, tabledef::GUILDMEMBER_GONGXIAN, member_param.member_list[i].gongxian);
					UPDATE_INT_DATA_NODE(total_gongxian_node, tabledef::GUILDMEMBER_TOTAL_GONGXIAN, member_param.member_list[i].total_gongxian);

					DataNode join_time(tabledef::GUILDMEMBER_JOIN_TIME);
					join_time.SetLong64(member_param.member_list[i].join_time);
					node_list.push_back(join_time);

					DataNode last_login(tabledef::GUILDMEMBER_LAST_LOGIN_TIME);
					last_login.SetLong64(member_param.member_list[i].last_login_time);
					node_list.push_back(last_login);

					DataNode lucky_color(tabledef::GUILDMEMBER_LUCKY_COLOR);
					lucky_color.SetInt32(member_param.member_list[i].lucky_color);
					node_list.push_back(lucky_color);

					DataNode today_guild_pao_saizi_times(tabledef::GUILDMEMBER_TODAY_GUILD_PAO_SAIZI_TIMES);
					today_guild_pao_saizi_times.SetInt32(member_param.member_list[i].today_guild_pao_saizi_times);
					node_list.push_back(today_guild_pao_saizi_times);

					DataNode today_last_guild_pao_saizi_time(tabledef::GUILDMEMBER_TODAY_LAST_GUILD_PAO_SAIZI_TIME);
					today_last_guild_pao_saizi_time.SetLong64(member_param.member_list[i].today_last_guild_pao_saizi_time);
					node_list.push_back(today_last_guild_pao_saizi_time);

					DataNode today_guild_saizi_score(tabledef::GUILDMEMBER_TODAY_GUILD_SAIZI_SCORE);
					today_guild_saizi_score.SetInt32(member_param.member_list[i].today_guild_saizi_score);
					node_list.push_back(today_guild_saizi_score);

					DataNode maze_layer(tabledef::GUILDMEMBER_MAZE_LAYER);
					maze_layer.SetInt32(member_param.member_list[i].maze_layer);
					node_list.push_back(maze_layer);

					DataNode maze_complete_time(tabledef::GUILDMEMBER_MAZE_COMPLETE_TIME);
					maze_complete_time.SetLong64(member_param.member_list[i].maze_complete_time);
					node_list.push_back(maze_complete_time);

					CmpNodeList cmp_node;

					CmpNode for_index_node(tabledef::GUILDMEMBER_FOR_INDEX);
					for_index_node.SetLong64(guildmember_for_index);
					cmp_node.push_back(for_index_node);

					UPDATE_BINARY_DATA_TO_LIST(data_node, tabledef::GUILDMEMBER_COMMON_DATA, member_param.member_list[i].common_data, GuildMemberParam::MemberCommonDataParamHex, node_list);

					int count = member_command->Update(stmt, cmp_node, node_list);
					if (count < 1)
					{
						ret = -6;
						// 此时不break，update不成功时则继续insert，以兼容某些错误
					}
					else 
					{
						break;
					}
				}

			case  structcommon::CS_INSERT:
				{
					const MataTableDef *member_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_GUILDMEMBER);

					DataAdapter member_item = member_table->GetPrototype();
					member_item.Malloc();

					member_item.m_data_area[tabledef::GUILDMEMBER_GUILD_ID].vint = guild_id;
					member_item.m_data_area[tabledef::GUILDMEMBER_MEMBER_INDEX].vint = member_param.member_list[i].index;
					member_item.m_data_area[tabledef::GUILDMEMBER_FOR_INDEX].vint64 = guildmember_for_index;
					member_item.m_data_area[tabledef::GUILDMEMBER_UID].vint = member_param.member_list[i].uid;
					
					member_item.m_data_area[tabledef::GUILDMEMBER_POST].vint = member_param.member_list[i].post;

					member_item.m_data_area[tabledef::GUILDMEMBER_GONGXIAN].vint = member_param.member_list[i].gongxian;
					member_item.m_data_area[tabledef::GUILDMEMBER_TOTAL_GONGXIAN].vint = member_param.member_list[i].total_gongxian;

					member_item.m_data_area[tabledef::GUILDMEMBER_JOIN_TIME].vint64 = member_param.member_list[i].join_time;
					member_item.m_data_area[tabledef::GUILDMEMBER_LAST_LOGIN_TIME].vint64 = member_param.member_list[i].last_login_time;

					member_item.m_data_area[tabledef::GUILDMEMBER_LUCKY_COLOR].vint = member_param.member_list[i].lucky_color;

					member_item.m_data_area[tabledef::GUILDMEMBER_TODAY_GUILD_PAO_SAIZI_TIMES].vint = member_param.member_list[i].today_guild_pao_saizi_times;
					member_item.m_data_area[tabledef::GUILDMEMBER_TODAY_LAST_GUILD_PAO_SAIZI_TIME].vint64 = member_param.member_list[i].today_last_guild_pao_saizi_time;
					member_item.m_data_area[tabledef::GUILDMEMBER_TODAY_GUILD_SAIZI_SCORE].vint = member_param.member_list[i].today_guild_saizi_score;

					member_item.m_data_area[tabledef::GUILDMEMBER_MAZE_LAYER].vint = member_param.member_list[i].maze_layer;
					member_item.m_data_area[tabledef::GUILDMEMBER_MAZE_COMPLETE_TIME].vint64 = member_param.member_list[i].maze_complete_time;

					INSERT_BINARY_DATA_TO_ADAPTER(member_item, tabledef::GUILDMEMBER_COMMON_DATA, GuildMemberParam::MemberCommonDataParamHex, member_param.member_list[i].common_data);
					
					if (DBCommand::RESULT_SUC != member_command->Save(stmt, &member_item))
					{
						ret = -7;
					}

					member_item.Free();
				}
				break;
			}
		}
	}

	{
		DBCommand *event_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_GUILDEVENT);

		for (int i = 0; i < event_param.count; ++i)
		{
			if (event_param.event_list[i].change_state == structcommon::CS_NONE) continue;

			long long guildevent_for_index = TranslateInt64(guild_id, event_param.event_list[i].index);

			switch (event_param.event_list[i].change_state)
			{
			case structcommon::CS_DELETE:
				{
					CmpNodeList cmp_node;

					CmpNode for_index_node(tabledef::GUILDEVENT_FOR_INDEX);
					for_index_node.SetLong64(guildevent_for_index);
					cmp_node.push_back(for_index_node);

					int count = event_command->Remove(stmt, cmp_node);
					if (count < 1)
					{
						ret = -8;
					}
				}
				break;

			case structcommon::CS_UPDATE:
				{
					DataNodeList attr_list;
					attr_list.reserve(tabledef::TABLE_GUILDEVENT_MAXINDEX - 4);

					DataNode event_type(tabledef::GUILDEVENT_EVENT_TYPE);
					event_type.SetInt32(event_param.event_list[i].event_type);
					attr_list.push_back(event_type);

					DataNode event_owner(tabledef::GUILDEVENT_EVENT_OWNER);
					event_owner.SetString(event_param.event_list[i].event_owner);
					attr_list.push_back(event_owner);

					DataNode event_owner_post(tabledef::GUILDEVENT_EVENT_OWNER_POST);
					event_owner_post.SetInt32(event_param.event_list[i].event_owner_post);
					attr_list.push_back(event_owner_post);

					DataNode event_time(tabledef::GUILDEVENT_EVENT_TIME);
					event_time.SetLong64(event_param.event_list[i].event_time);
					attr_list.push_back(event_time);

					DataNode bigevent(tabledef::GUILDEVENT_BIG_EVENT);
					bigevent.SetInt32(event_param.event_list[i].big_event);
					attr_list.push_back(bigevent);

					DataNode cannot_remove(tabledef::GUILDEVENT_CANNOT_REMOVE);
					cannot_remove.SetInt32(event_param.event_list[i].cannot_remove);
					attr_list.push_back(cannot_remove);

					DataNode param0(tabledef::GUILDEVENT_PARAM0);
					param0.SetInt32(event_param.event_list[i].param0);
					attr_list.push_back(param0);

					DataNode param1(tabledef::GUILDEVENT_PARAM1);
					param1.SetInt32(event_param.event_list[i].param1);
					attr_list.push_back(param1);

					DataNode param2(tabledef::GUILDEVENT_PARAM2);
					param2.SetInt32(event_param.event_list[i].param2);
					attr_list.push_back(param2);

					DataNode param3(tabledef::GUILDEVENT_PARAM3);
					param3.SetInt32(event_param.event_list[i].param3);
					attr_list.push_back(param3);

					DataNode sparam0(tabledef::GUILDEVENT_SPARAM0);
					sparam0.SetString(event_param.event_list[i].sparam0);
					attr_list.push_back(sparam0);

					CmpNodeList cmp_node;

					CmpNode for_index_node(tabledef::GUILDEVENT_FOR_INDEX);
					for_index_node.SetLong64(guildevent_for_index);
					cmp_node.push_back(for_index_node);

					int count = event_command->Update(stmt, cmp_node, attr_list);
					if (count < 1)
					{
						ret = -9;
						// 此时不break，update不成功时则继续insert，以兼容某些错误
					}
					else 
					{
						break;
					}
				}

			case  structcommon::CS_INSERT:
				{
					const MataTableDef *event_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_GUILDEVENT);

					DataAdapter event_item = event_table->GetPrototype();
					event_item.Malloc();

					event_item.m_data_area[tabledef::GUILDEVENT_GUILD_ID].vint = guild_id;
					event_item.m_data_area[tabledef::GUILDEVENT_EVENT_INDEX].vint = event_param.event_list[i].index;
					event_item.m_data_area[tabledef::GUILDEVENT_FOR_INDEX].vint64 = guildevent_for_index;
					event_item.m_data_area[tabledef::GUILDEVENT_EVENT_TYPE].vint = event_param.event_list[i].event_type;
					STRNCPY(event_item.m_data_area[tabledef::GUILDEVENT_EVENT_OWNER].vcharp, event_param.event_list[i].event_owner,
						event_item.m_data_area[tabledef::GUILDEVENT_EVENT_OWNER].length);
					event_item.m_data_area[tabledef::GUILDEVENT_EVENT_OWNER_POST].vint = event_param.event_list[i].event_owner_post;
					event_item.m_data_area[tabledef::GUILDEVENT_EVENT_TIME].vint64 = event_param.event_list[i].event_time;
					event_item.m_data_area[tabledef::GUILDEVENT_BIG_EVENT].vint = event_param.event_list[i].big_event;
					event_item.m_data_area[tabledef::GUILDEVENT_CANNOT_REMOVE].vint = event_param.event_list[i].cannot_remove;
					event_item.m_data_area[tabledef::GUILDEVENT_PARAM0].vint = event_param.event_list[i].param0;
					event_item.m_data_area[tabledef::GUILDEVENT_PARAM1].vint = event_param.event_list[i].param1;
					event_item.m_data_area[tabledef::GUILDEVENT_PARAM2].vint = event_param.event_list[i].param2;
					event_item.m_data_area[tabledef::GUILDEVENT_PARAM3].vint = event_param.event_list[i].param3;
					STRNCPY(event_item.m_data_area[tabledef::GUILDEVENT_SPARAM0].vcharp, event_param.event_list[i].sparam0,
						event_item.m_data_area[tabledef::GUILDEVENT_SPARAM0].length);
					if (DBCommand::RESULT_SUC != event_command->Save(stmt, &event_item))
					{
						ret = -10;
					}

					event_item.Free();
				}
				break;
			}
		}
	}

	conn->commit();

	return ret < 0 ? ret : 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
int RMIGlobalObject::__SavePublicSale(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SavePublicSale_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static PublicSaleParam publicsale_param;
	publicsale_param.count = 0;

	if (!publicsale_param.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SavePublicSale_2");
		return rmi::DispatchParamError;
	}

	int result = SavePublicSale(publicsale_param);
	
	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SavePublicSale_3 result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SavePublicSale_4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::SavePublicSale(const PublicSaleParam & publicsale_param)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *publicsale_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_PUBLICSALE);
	DBCommand *public_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_PUBLICSALE);

	conn->begin();

	for (int i = 0; i < publicsale_param.count; ++i)
	{
		long long for_index = TranslateInt64(publicsale_param.sale_list[i].seller_uid, publicsale_param.sale_list[i].item_param.sale_index);

		switch (publicsale_param.sale_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_node;

				CmpNode for_index_node(tabledef::PUBLICSALE_FOR_INDEX);
				for_index_node.SetLong64(for_index);
				cmp_node.push_back(for_index_node);

				int count = public_command->Remove(stmt, cmp_node);
				if (count < 1)
				{
					ret = -1;
				}
			}
			break; // case structcommon::CS_DELETE

		case structcommon::CS_INSERT:
			{
				DataAdapter publicsale_item = publicsale_table->GetPrototype();

				publicsale_item.Malloc();

				publicsale_item.m_data_area[tabledef::PUBLICSALE_SELLER_UID].vint = publicsale_param.sale_list[i].seller_uid;
				publicsale_item.m_data_area[tabledef::PUBLICSALE_SALE_INDEX].vint = publicsale_param.sale_list[i].item_param.sale_index;
				publicsale_item.m_data_area[tabledef::PUBLICSALE_FOR_INDEX].vint64 = for_index;
				STRNCPY(publicsale_item.m_data_area[tabledef::PUBLICSALE_SELLER_NAME].vcharp, publicsale_param.sale_list[i].seller_name,
					publicsale_item.m_data_area[tabledef::PUBLICSALE_SELLER_NAME].length);

				publicsale_item.m_data_area[tabledef::PUBLICSALE_ITEM_ID].vint = publicsale_param.sale_list[i].item_param.item_wrapper.item_id;
				publicsale_item.m_data_area[tabledef::PUBLICSALE_ITEM_IS_BIND].vchar = static_cast<char>(publicsale_param.sale_list[i].item_param.item_wrapper.is_bind);
				publicsale_item.m_data_area[tabledef::PUBLICSALE_ITEM_SALE_TYPE].vint = publicsale_param.sale_list[i].item_param.sale_type;
				publicsale_item.m_data_area[tabledef::PUBLICSALE_ITEM_NUM].vint = publicsale_param.sale_list[i].item_param.item_wrapper.num;
				publicsale_item.m_data_area[tabledef::PUBLICSALE_COLOR].vint = publicsale_param.sale_list[i].item_param.color;
				publicsale_item.m_data_area[tabledef::PUBLICSALE_LEVEL].vint = publicsale_param.sale_list[i].item_param.level;
				publicsale_item.m_data_area[tabledef::PUBLICSALE_PROF].vint = publicsale_param.sale_list[i].item_param.prof;
				publicsale_item.m_data_area[tabledef::PUBLICSALE_EQUIP_ORDER].vint = publicsale_param.sale_list[i].item_param.order;
				publicsale_item.m_data_area[tabledef::PUBLICSALE_ITEM_INVALID_TIME].vint64 = publicsale_param.sale_list[i].item_param.item_wrapper.invalid_time;
				publicsale_item.m_data_area[tabledef::PUBLICSALE_GOLD_PRICE].vint = publicsale_param.sale_list[i].item_param.price;
				publicsale_item.m_data_area[tabledef::PUBLICSALE_SALE_VALUE].vint = publicsale_param.sale_list[i].item_param.sale_value;
				publicsale_item.m_data_area[tabledef::PUBLICSALE_PRICE_TYPE].vint = publicsale_param.sale_list[i].item_param.price_type;
				publicsale_item.m_data_area[tabledef::PUBLICSALE_SALE_ITEM_TYPE].vint = publicsale_param.sale_list[i].item_param.sale_item_type;
				publicsale_item.m_data_area[tabledef::PUBLICSALE_SALE_TIME].vint64 = publicsale_param.sale_list[i].item_param.sale_time;
				publicsale_item.m_data_area[tabledef::PUBLICSALE_DUE_TIME].vint64 = publicsale_param.sale_list[i].item_param.due_time;

				if (0 != publicsale_param.sale_list[i].item_param.item_wrapper.has_param)
				{
					RMIGlobalObject::ItemParamInsert(publicsale_item.m_data_area[tabledef::PUBLICSALE_ITEM_PARAM].vcharp,
						publicsale_item.m_data_area[tabledef::PUBLICSALE_ITEM_PARAM].length, &publicsale_param.sale_list[i].item_param.item_wrapper.param_data);
				}

				if (DBCommand::RESULT_SUC != public_command->Save(stmt, &publicsale_item))
				{
					ret = -2;
				}

				publicsale_item.Free();
			}
			break; // case structcommon::CS_INSERT

		case structcommon::CS_UPDATE:	
			{		
				DataNodeList sale_item_list;
				sale_item_list.reserve(tabledef::TABLE_PUBLICSALE_MAXINDEX);

				DataNode seller_name(tabledef::PUBLICSALE_SELLER_NAME);
				seller_name.SetString((const char *)publicsale_param.sale_list[i].seller_name);
				sale_item_list.push_back(seller_name);
				
				DataNode item_id(tabledef::PUBLICSALE_ITEM_ID);
				item_id.SetInt32(publicsale_param.sale_list[i].item_param.item_wrapper.item_id);
				sale_item_list.push_back(item_id);

				DataNode is_bind(tabledef::PUBLICSALE_ITEM_IS_BIND);
				is_bind.SetByte(static_cast<char>(publicsale_param.sale_list[i].item_param.item_wrapper.is_bind));
				sale_item_list.push_back(is_bind);

				DataNode item_saletype(tabledef::PUBLICSALE_ITEM_SALE_TYPE);
				item_saletype.SetInt32(publicsale_param.sale_list[i].item_param.sale_type);
				sale_item_list.push_back(item_saletype);

				DataNode item_num(tabledef::PUBLICSALE_ITEM_NUM);
				item_num.SetInt32(publicsale_param.sale_list[i].item_param.item_wrapper.num);
				sale_item_list.push_back(item_num);

				DataNode color(tabledef::PUBLICSALE_COLOR);
				color.SetInt32(publicsale_param.sale_list[i].item_param.color);
				sale_item_list.push_back(color);

				DataNode level(tabledef::PUBLICSALE_LEVEL);
				level.SetInt32(publicsale_param.sale_list[i].item_param.level);
				sale_item_list.push_back(level);

				DataNode prof(tabledef::PUBLICSALE_PROF);
				prof.SetInt32(publicsale_param.sale_list[i].item_param.prof);
				sale_item_list.push_back(prof);

				DataNode order(tabledef::PUBLICSALE_EQUIP_ORDER);
				order.SetInt32(publicsale_param.sale_list[i].item_param.order);
				sale_item_list.push_back(order);

				DataNode gold_price(tabledef::PUBLICSALE_GOLD_PRICE);
				gold_price.SetInt32(publicsale_param.sale_list[i].item_param.price);
				sale_item_list.push_back(gold_price);

				DataNode sale_value(tabledef::PUBLICSALE_SALE_VALUE);
				sale_value.SetInt32(publicsale_param.sale_list[i].item_param.sale_value);
				sale_item_list.push_back(sale_value);

				DataNode price_type(tabledef::PUBLICSALE_PRICE_TYPE);
				price_type.SetInt32(publicsale_param.sale_list[i].item_param.price_type);
				sale_item_list.push_back(price_type);

				DataNode sale_item_type(tabledef::PUBLICSALE_SALE_ITEM_TYPE);
				sale_item_type.SetInt32(publicsale_param.sale_list[i].item_param.sale_item_type);
				sale_item_list.push_back(sale_item_type);

				DataNode sale_time(tabledef::PUBLICSALE_SALE_TIME);
				sale_time.SetLong64(publicsale_param.sale_list[i].item_param.sale_time);
				sale_item_list.push_back(sale_time);

				DataNode due_time(tabledef::PUBLICSALE_DUE_TIME);
				due_time.SetLong64(publicsale_param.sale_list[i].item_param.due_time);
				sale_item_list.push_back(due_time);

				DataNode invalid_time(tabledef::PUBLICSALE_ITEM_INVALID_TIME);
				invalid_time.SetLong64(publicsale_param.sale_list[i].item_param.item_wrapper.invalid_time);
				sale_item_list.push_back(invalid_time);

				DataNode item_param(tabledef::PUBLICSALE_ITEM_PARAM);
				RMIGlobalObject::ItemParamUpdate(&item_param, 0 != publicsale_param.sale_list[i].item_param.item_wrapper.has_param, &publicsale_param.sale_list[i].item_param.item_wrapper.param_data);
				sale_item_list.push_back(item_param);

				CmpNodeList cmp_node;

				CmpNode for_index_node(tabledef::PUBLICSALE_FOR_INDEX);
				for_index_node.SetLong64(for_index);
				cmp_node.push_back(for_index_node);

				int count = public_command->Update(stmt, cmp_node, sale_item_list);
				if (count < 0)
				{
					ret = -3; 
				}
			}
			break; // case structcommon::CS_UPDATE

		} // switch end

	} // for end

	conn->commit();

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__InitPublicSale(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(PublicSaleParam) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitPublicSale_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	long long id_from = 0;

	if (!in_param.Pop(&id_from))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitPublicSale_2");
		return rmi::DispatchParamError;
	}

	static PublicSaleParam public_sale_param;
	public_sale_param.count = 0;

	int result = InitPublicSale(id_from, &public_sale_param);

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitPublicSale_3 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!public_sale_param.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitPublicSale_4 result:%d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitPublicSale_5 result:%d", result);
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::InitPublicSale(long long id_from, PublicSaleParam *publicsale_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *publicsale_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_PUBLICSALE);

	QueryCondition publicsale_condition;
	CmpNode publicsale_node1(tabledef::PUBLICSALE_IDPUBLICSALE);
	publicsale_node1.SetLong64(id_from);
	publicsale_node1.SetEqualType(EQUAL_TYPE_GRANTER);
	publicsale_condition.cmp_list.push_back(publicsale_node1);
	publicsale_condition.query_num = PublicSaleParam::MAX_PUBLIC_SALE_COUNT;

	DataAdapterList publicsale_list;
	int ret = publicsale_command->Find(stmt, publicsale_condition, &publicsale_list);
	if (ret < 0)
	{
		publicsale_param->count = 0;
		return -1;
	}

	ret = 0;

	int count = 0;
	DataAdapterList::iterator i = publicsale_list.begin();
	for (; i != publicsale_list.end() && count < PublicSaleParam::MAX_PUBLIC_SALE_COUNT; ++i)
	{
		publicsale_param->sale_list[count].change_state = structcommon::CS_NONE;
		publicsale_param->sale_list[count].id_pulicsale = i->m_data_area[tabledef::PUBLICSALE_IDPUBLICSALE].vint64;

		publicsale_param->sale_list[count].seller_uid = i->m_data_area[tabledef::PUBLICSALE_SELLER_UID].vint;
		publicsale_param->sale_list[count].item_param.sale_index = i->m_data_area[tabledef::PUBLICSALE_SALE_INDEX].vint;
		STRNCPY(publicsale_param->sale_list[count].seller_name, i->m_data_area[tabledef::PUBLICSALE_SELLER_NAME].vcharp,
			sizeof(publicsale_param->sale_list[0].seller_name));
		publicsale_param->sale_list[count].item_param.item_wrapper.item_id = i->m_data_area[tabledef::PUBLICSALE_ITEM_ID].vint;
		publicsale_param->sale_list[count].item_param.item_wrapper.is_bind = i->m_data_area[tabledef::PUBLICSALE_ITEM_IS_BIND].vchar;
		publicsale_param->sale_list[count].item_param.sale_type = i->m_data_area[tabledef::PUBLICSALE_ITEM_SALE_TYPE].vint;
		publicsale_param->sale_list[count].item_param.item_wrapper.num = i->m_data_area[tabledef::PUBLICSALE_ITEM_NUM].vint;
		publicsale_param->sale_list[count].item_param.color = i->m_data_area[tabledef::PUBLICSALE_COLOR].vint;
		publicsale_param->sale_list[count].item_param.level = i->m_data_area[tabledef::PUBLICSALE_LEVEL].vint;
		publicsale_param->sale_list[count].item_param.prof = i->m_data_area[tabledef::PUBLICSALE_PROF].vint;
		publicsale_param->sale_list[count].item_param.order = i->m_data_area[tabledef::PUBLICSALE_EQUIP_ORDER].vint;
		publicsale_param->sale_list[count].item_param.price = i->m_data_area[tabledef::PUBLICSALE_GOLD_PRICE].vint;
		publicsale_param->sale_list[count].item_param.sale_value = i->m_data_area[tabledef::PUBLICSALE_SALE_VALUE].vint;
		publicsale_param->sale_list[count].item_param.price_type = i->m_data_area[tabledef::PUBLICSALE_PRICE_TYPE].vint;
		publicsale_param->sale_list[count].item_param.sale_item_type = i->m_data_area[tabledef::PUBLICSALE_SALE_ITEM_TYPE].vint;
		publicsale_param->sale_list[count].item_param.sale_time = static_cast<unsigned int>(i->m_data_area[tabledef::PUBLICSALE_SALE_TIME].vint64);
		publicsale_param->sale_list[count].item_param.due_time = static_cast<unsigned int>(i->m_data_area[tabledef::PUBLICSALE_DUE_TIME].vint64);
		publicsale_param->sale_list[count].item_param.item_wrapper.invalid_time = static_cast<unsigned int>(i->m_data_area[tabledef::PUBLICSALE_ITEM_INVALID_TIME].vint64);


		if (RMIGlobalObject::ItemParamInit(&publicsale_param->sale_list[count].item_param.item_wrapper.param_data, 
			i->m_data_area[tabledef::PUBLICSALE_ITEM_PARAM].vcharp, i->m_data_area[tabledef::PUBLICSALE_ITEM_PARAM].length))
		{
			publicsale_param->sale_list[count].item_param.item_wrapper.has_param = 1;
		}
		else
		{
			publicsale_param->sale_list[count].item_param.item_wrapper.has_param = 0;
		}

		++count;
		i->Free();
	}

	publicsale_param->count = count;

	for (; i != publicsale_list.end(); ++i)
	{
		i->Free();
	}

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__InitRoleMails(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(MailListParam) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitRoleMails_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	int uid = 0;

	if (!in_param.Pop(&uid))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitRoleMails_2");
		return rmi::DispatchParamError;
	}

	static MailListParam mail_param;
	mail_param.count = 0;

	int result = InitRoleMails(uid, &mail_param);

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitRoleMails_3 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!mail_param.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitRoleMails_4 result:%d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitRoleMails_5 result:%d", result);
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::InitRoleMails(int uid,  MailListParam *mail_list)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *mail_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_MAIL);

	QueryCondition mail_condition;

	CmpNode mail_uid(tabledef::MAIL_UID);
	mail_uid.SetInt32(uid);
	mail_condition.cmp_list.push_back(mail_uid);

	DataAdapterList mails;

	int ret = mail_command->Find(stmt, mail_condition, &mails);
	if (ret < 0) 
	{
		mail_list->count = 0;
		return -1;
	}

	int count = 0;
	DataAdapterList::iterator iter = mails.begin(); 

	for (; iter != mails.end() && count < MAX_MAIL_NUM;	++iter, ++count)
	{
		mail_list->mail_list[count].mail_param.status.sender_uid = iter->m_data_area[tabledef::MAIL_SENDER_UID].vint;
		STRNCPY(mail_list->mail_list[count].mail_param.status.sender_name, iter->m_data_area[tabledef::MAIL_SENDER_NAME].vcharp,
			sizeof(mail_list->mail_list[0].mail_param.status.sender_name));
		mail_list->mail_list[count].change_state = structcommon::CS_NONE;
		mail_list->mail_list[count].mail_param.mail_index = iter->m_data_area[tabledef::MAIL_MAIL_INDEX].vint;
		mail_list->mail_list[count].mail_param.status.recv_time = iter->m_data_area[tabledef::MAIL_RECV_TIME].vint;
		mail_list->mail_list[count].mail_param.status.kind = iter->m_data_area[tabledef::MAIL_KIND].vint;
		mail_list->mail_list[count].mail_param.status.is_read = iter->m_data_area[tabledef::MAIL_IS_READ].vint;
		mail_list->mail_list[count].mail_param.status.is_lock = iter->m_data_area[tabledef::MAIL_IS_LOCK].vint;
		mail_list->mail_list[count].mail_param.contentparam.coin = iter->m_data_area[tabledef::MAIL_COIN].vint;
		mail_list->mail_list[count].mail_param.contentparam.coin_bind = iter->m_data_area[tabledef::MAIL_COIN_BIND].vint;
		mail_list->mail_list[count].mail_param.contentparam.gold = iter->m_data_area[tabledef::MAIL_GOLD].vint;
		mail_list->mail_list[count].mail_param.contentparam.gold_bind = iter->m_data_area[tabledef::MAIL_GOLD_BIND].vint;

		STRNCPY(mail_list->mail_list[count].mail_param.contentparam.subject, iter->m_data_area[tabledef::MAIL_SUBJECT].vcharp,
			sizeof(mail_list->mail_list[0].mail_param.contentparam.subject));
		STRNCPY(mail_list->mail_list[count].mail_param.contentparam.contenttxt, iter->m_data_area[tabledef::MAIL_CONTENT].vcharp,
			sizeof(mail_list->mail_list[0].mail_param.contentparam.contenttxt));

		for (int item_index = 0; item_index < MAX_ATTACHMENT_ITEM_NUM; item_index++)
		{
			mail_list->mail_list[count].mail_param.contentparam.item_list[item_index].item_id = iter->m_data_area[MAIL_ITEM_ID_DEF[item_index]].vint;
			mail_list->mail_list[count].mail_param.contentparam.item_list[item_index].is_bind = iter->m_data_area[MAIL_ITEM_IS_BIND_DEF[item_index]].vchar;
			mail_list->mail_list[count].mail_param.contentparam.item_list[item_index].num = iter->m_data_area[MAIL_ITEM_NUM_DEF[item_index]].vint;
			mail_list->mail_list[count].mail_param.contentparam.item_list[item_index].invalid_time = static_cast<unsigned int>(iter->m_data_area[MAIL_ITEM_INVALID_TIME_DEF[item_index]].vint64);

			if (RMIGlobalObject::ItemParamInit(&mail_list->mail_list[count].mail_param.contentparam.item_list[item_index].param_data, 
				iter->m_data_area[MAIL_ITEM_PARAM_DEF[item_index]].vcharp, iter->m_data_area[MAIL_ITEM_PARAM_DEF[item_index]].length))
			{
				mail_list->mail_list[count].mail_param.contentparam.item_list[item_index].has_param = 1;
			}
			else
			{
				mail_list->mail_list[count].mail_param.contentparam.item_list[item_index].has_param = 0;
			}
		}

		{
			memset((void*)&mail_list->mail_list[count].mail_param.contentparam.virtual_item_list, 0, sizeof(MailVirtualItemList));

			unsigned int length = iter->m_data_area[tabledef::MAIL_VIRTUAL_ITEM_DATA].length;
			if (length > sizeof(MailVirtualItemList) * 2)
			{
				iter->Free();
				continue;
			}
			HexToBin(iter->m_data_area[tabledef::MAIL_VIRTUAL_ITEM_DATA].vcharp, length, (char*)&mail_list->mail_list[count].mail_param.contentparam.virtual_item_list);
		}

		iter->Free();
	}

	for (; iter != mails.end(); ++iter)
	{
		iter->Free();
	}

	mail_list->count = count;

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__SaveRoleMails(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveRoleMails_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	int uid = 0;

	static MailListParam mail_param;
	mail_param.count = 0;

	if (!in_param.Pop(&uid) || !mail_param.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveRoleMails_2");
		return rmi::DispatchParamError;
	}

	int result = SaveRoleMails(uid, mail_param);
	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveRoleMails_3 result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveRoleMails_4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::SaveRoleMails(int uid, const MailListParam &mail_param)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *mail_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_MAIL);
	DBCommand *mail_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_MAIL);

	conn->begin();

	for (int i = 0; i < mail_param.count; ++i)
	{
		long long mail_for_index = TranslateInt64(uid, mail_param.mail_list[i].mail_param.mail_index);

		switch (mail_param.mail_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_node;

				CmpNode for_index_node(tabledef::MAIL_FOR_INDEX);
				for_index_node.SetLong64(mail_for_index);
				cmp_node.push_back(for_index_node);

				int count = mail_command->Remove(stmt, cmp_node);
				if (count < 1)
				{
					ret = -1;
				}
			}
			break; // case structcommon::CS_DELETE

		case structcommon::CS_INSERT:
			{
				DataAdapter mail_item = mail_table->GetPrototype();
				mail_item.Malloc();
				
				mail_item.m_data_area[tabledef::MAIL_UID].vint = uid;
				mail_item.m_data_area[tabledef::MAIL_MAIL_INDEX].vint = mail_param.mail_list[i].mail_param.mail_index;
				mail_item.m_data_area[tabledef::MAIL_FOR_INDEX].vint64 = mail_for_index;
				mail_item.m_data_area[tabledef::MAIL_SENDER_UID].vint = mail_param.mail_list[i].mail_param.status.sender_uid;
				STRNCPY(mail_item.m_data_area[tabledef::MAIL_SENDER_NAME].vcharp, mail_param.mail_list[i].mail_param.status.sender_name,
					mail_item.m_data_area[tabledef::MAIL_SENDER_NAME].length);
				mail_item.m_data_area[tabledef::MAIL_RECV_TIME].vint = mail_param.mail_list[i].mail_param.status.recv_time;
				mail_item.m_data_area[tabledef::MAIL_KIND].vint = mail_param.mail_list[i].mail_param.status.kind;
				mail_item.m_data_area[tabledef::MAIL_IS_READ].vint = mail_param.mail_list[i].mail_param.status.is_read;
				mail_item.m_data_area[tabledef::MAIL_IS_LOCK].vint = mail_param.mail_list[i].mail_param.status.is_lock;
				mail_item.m_data_area[tabledef::MAIL_COIN].vint = mail_param.mail_list[i].mail_param.contentparam.coin;
				mail_item.m_data_area[tabledef::MAIL_COIN_BIND].vint = mail_param.mail_list[i].mail_param.contentparam.coin_bind;
				mail_item.m_data_area[tabledef::MAIL_GOLD].vint = mail_param.mail_list[i].mail_param.contentparam.gold;
				mail_item.m_data_area[tabledef::MAIL_GOLD_BIND].vint = mail_param.mail_list[i].mail_param.contentparam.gold_bind;

				STRNCPY(mail_item.m_data_area[tabledef::MAIL_SUBJECT].vcharp, mail_param.mail_list[i].mail_param.contentparam.subject,
					mail_item.m_data_area[tabledef::MAIL_SUBJECT].length);
				STRNCPY(mail_item.m_data_area[tabledef::MAIL_CONTENT].vcharp, mail_param.mail_list[i].mail_param.contentparam.contenttxt,
					mail_item.m_data_area[tabledef::MAIL_CONTENT].length);

				for (int item_index = 0; item_index < MAX_ATTACHMENT_ITEM_NUM; item_index++)
				{
					mail_item.m_data_area[MAIL_ITEM_ID_DEF[item_index]].vint = mail_param.mail_list[i].mail_param.contentparam.item_list[item_index].item_id;
					mail_item.m_data_area[MAIL_ITEM_IS_BIND_DEF[item_index]].vchar = (char)mail_param.mail_list[i].mail_param.contentparam.item_list[item_index].is_bind;
					mail_item.m_data_area[MAIL_ITEM_NUM_DEF[item_index]].vint = mail_param.mail_list[i].mail_param.contentparam.item_list[item_index].num;
					mail_item.m_data_area[MAIL_ITEM_INVALID_TIME_DEF[item_index]].vint64 = mail_param.mail_list[i].mail_param.contentparam.item_list[item_index].invalid_time;

					if (0 != mail_param.mail_list[i].mail_param.contentparam.item_list[item_index].has_param)
					{
						RMIGlobalObject::ItemParamInsert(mail_item.m_data_area[MAIL_ITEM_PARAM_DEF[item_index]].vcharp,
							mail_item.m_data_area[MAIL_ITEM_PARAM_DEF[item_index]].length, &(mail_param.mail_list[i].mail_param.contentparam.item_list[item_index].param_data));
					}
				}

				{
					static MailVirtualItemListHex mail_virtual_item_hex;
					BinToHex((const char*)&mail_param.mail_list[i].mail_param.contentparam.virtual_item_list, sizeof(MailVirtualItemList), mail_virtual_item_hex);
					mail_virtual_item_hex[sizeof(mail_virtual_item_hex) - 1] = '\0';

					STRNCPY(mail_item.m_data_area[tabledef::MAIL_VIRTUAL_ITEM_DATA].vcharp, (const char*)&mail_virtual_item_hex, mail_item.m_data_area[tabledef::MAIL_VIRTUAL_ITEM_DATA].length);
				}

				if (DBCommand::RESULT_SUC != mail_command->Save(stmt, &mail_item))
				{
					ret = -2;
				}

				mail_item.Free();
			}
			break; // case structcommon::CS_INSERT

		case structcommon::CS_UPDATE:
			{
				DataNodeList mail_item_list;
				mail_item_list.reserve(tabledef::TABLE_MAIL_MAXINDEX);

				DataNode uid_node(tabledef::MAIL_UID);
				uid_node.SetInt32(uid);
				mail_item_list.push_back(uid_node);

				DataNode sender_uid(tabledef::MAIL_SENDER_UID);
				sender_uid.SetInt32(mail_param.mail_list[i].mail_param.status.sender_uid);
				mail_item_list.push_back(sender_uid);

				DataNode sender_name(tabledef::MAIL_SENDER_NAME);
				sender_name.SetString((const char *)mail_param.mail_list[i].mail_param.status.sender_name);
				mail_item_list.push_back(sender_name);

				DataNode recv_time(tabledef::MAIL_RECV_TIME);
				recv_time.SetInt32(mail_param.mail_list[i].mail_param.status.recv_time);
				mail_item_list.push_back(recv_time);

				DataNode kind(tabledef::MAIL_KIND);
				kind.SetInt32(mail_param.mail_list[i].mail_param.status.kind);
				mail_item_list.push_back(kind);

				DataNode is_read(tabledef::MAIL_IS_READ);
				is_read.SetInt32(mail_param.mail_list[i].mail_param.status.is_read);
				mail_item_list.push_back(is_read);

				DataNode is_lock(tabledef::MAIL_IS_LOCK);
				is_lock.SetInt32(mail_param.mail_list[i].mail_param.status.is_lock);
				mail_item_list.push_back(is_lock);

				DataNode coin(tabledef::MAIL_COIN);
				coin.SetInt32(mail_param.mail_list[i].mail_param.contentparam.coin);
				mail_item_list.push_back(coin);

				DataNode coin_bind(tabledef::MAIL_COIN_BIND);
				coin_bind.SetInt32(mail_param.mail_list[i].mail_param.contentparam.coin_bind);
				mail_item_list.push_back(coin_bind);

				DataNode gold(tabledef::MAIL_GOLD);
				gold.SetInt32(mail_param.mail_list[i].mail_param.contentparam.gold);
				mail_item_list.push_back(gold);

				DataNode gold_bind(tabledef::MAIL_GOLD_BIND);
				gold_bind.SetInt32(mail_param.mail_list[i].mail_param.contentparam.gold_bind);
				mail_item_list.push_back(gold_bind);

				DataNode subject(tabledef::MAIL_SUBJECT);
				subject.SetString((const char *)mail_param.mail_list[i].mail_param.contentparam.subject);
				mail_item_list.push_back(subject);

				DataNode content(tabledef::MAIL_CONTENT);
				content.SetString((const char *)mail_param.mail_list[i].mail_param.contentparam.contenttxt);
				mail_item_list.push_back(content);

				for (int item_index = 0; item_index < MAX_ATTACHMENT_ITEM_NUM; item_index++)
				{
					DataNode item_id(MAIL_ITEM_ID_DEF[item_index]);
					item_id.SetInt32(mail_param.mail_list[i].mail_param.contentparam.item_list[item_index].item_id);
					mail_item_list.push_back(item_id);

					DataNode is_bind(MAIL_ITEM_IS_BIND_DEF[item_index]);
					is_bind.SetByte((char)mail_param.mail_list[i].mail_param.contentparam.item_list[item_index].is_bind);
					mail_item_list.push_back(is_bind);

					DataNode item_num(MAIL_ITEM_NUM_DEF[item_index]);
					item_num.SetInt32(mail_param.mail_list[i].mail_param.contentparam.item_list[item_index].num);
					mail_item_list.push_back(item_num);

					DataNode invalid_time(MAIL_ITEM_INVALID_TIME_DEF[item_index]);
					invalid_time.SetLong64(mail_param.mail_list[i].mail_param.contentparam.item_list[item_index].invalid_time);
					mail_item_list.push_back(invalid_time);

					DataNode item_param(MAIL_ITEM_PARAM_DEF[item_index]);
					RMIGlobalObject::ItemParamUpdate(&item_param, 0 != mail_param.mail_list[i].mail_param.contentparam.item_list[item_index].has_param,
						&mail_param.mail_list[i].mail_param.contentparam.item_list[item_index].param_data);
					mail_item_list.push_back(item_param);
				}

				{
					static MailVirtualItemListHex mail_virtual_item_hex;
					BinToHex((const char*)&mail_param.mail_list[i].mail_param.contentparam.virtual_item_list, sizeof(MailVirtualItemList), mail_virtual_item_hex);
					mail_virtual_item_hex[sizeof(mail_virtual_item_hex) - 1] = '\0';

					DataNode mail_virtual_item_node(tabledef::MAIL_VIRTUAL_ITEM_DATA);
					mail_virtual_item_node.SetString(mail_virtual_item_hex);
					mail_item_list.push_back(mail_virtual_item_node);
				}

				CmpNodeList cmp_node;

				CmpNode for_index_node(tabledef::MAIL_FOR_INDEX);
				for_index_node.SetLong64(mail_for_index);
				cmp_node.push_back(for_index_node);

				int count = mail_command->Update(stmt, cmp_node, mail_item_list);
				if (count < 0)
				{
					ret = -3; 
				}
			}
			break; // case structcommon::CS_UPDATE

		} // switch end

	} // for end

	conn->commit();

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__GetRoleSystemMails(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(MailListParam) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__GetRoleSystemMails_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	int uid = 0;
	if (!in_param.Pop(&uid))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__GetRoleSystemMails_2");
		return rmi::DispatchParamError;
	}

	MailListParam *mail_param = (MailListParam *)new ServerMsgMem[sizeof(MailListParam)];
	mail_param->count = 0;

	int result = GetRoleSystemMails(uid, mail_param);

	if (!out_param->Push(result))
	{
		ServerMsgMem *tmp = (ServerMsgMem*)mail_param;
		delete []tmp;

		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__GetRoleSystemMails_3 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!mail_param->Serialize(*out_param))
		{
			ServerMsgMem *tmp = (ServerMsgMem*)mail_param;
			delete []tmp;

			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__GetRoleSystemMails_4 result:%d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__GetRoleSystemMails_5 result:%d", result);
	}

	ServerMsgMem *tmp = (ServerMsgMem*)mail_param;
	delete []tmp;

	return rmi::DispatchOK;
}

int RMIGlobalObject::GetRoleSystemMails(int uid, MailListParam *mail_list)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *mail_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_SYSTEMMAIL);

	QueryCondition systemmail_condition;

	CmpNode systemmail_uid(tabledef::SYSTEMMAIL_UID);
	systemmail_uid.SetInt32(uid);
	systemmail_condition.cmp_list.push_back(systemmail_uid);

	DataAdapterList mails;

	int ret = mail_command->Find(stmt, systemmail_condition, &mails);
	if(ret < 0) 
	{
		mail_list->count = 0;
		return -1;
	}

	int count = 0;
	DataAdapterList::iterator iter = mails.begin(); 

	for (; iter != mails.end() && count < MAX_MAIL_NUM;	++iter, ++count)
	{
		mail_list->mail_list[count].system_mail_id = iter->m_data_area[tabledef::SYSTEMMAIL_SYSTEMMAIL_ID].vint64;

		mail_list->mail_list[count].check_uid = iter->m_data_area[tabledef::SYSTEMMAIL_UID].vint;

		mail_list->mail_list[count].mail_param.status.sender_uid = 0;
		mail_list->mail_list[count].mail_param.status.sender_name[0] = 0;
		mail_list->mail_list[count].change_state = structcommon::CS_NONE;
		mail_list->mail_list[count].mail_param.mail_index = -1;
		mail_list->mail_list[count].mail_param.status.recv_time = iter->m_data_area[tabledef::SYSTEMMAIL_RECV_TIME].vint;
		mail_list->mail_list[count].mail_param.status.kind = iter->m_data_area[tabledef::SYSTEMMAIL_KIND].vchar;
		mail_list->mail_list[count].mail_param.status.is_read = iter->m_data_area[tabledef::SYSTEMMAIL_IS_READ].vchar;
		mail_list->mail_list[count].mail_param.status.is_lock = iter->m_data_area[tabledef::SYSTEMMAIL_IS_LOCK].vchar;
		mail_list->mail_list[count].mail_param.contentparam.coin = iter->m_data_area[tabledef::SYSTEMMAIL_COIN].vint;
		mail_list->mail_list[count].mail_param.contentparam.coin_bind = iter->m_data_area[tabledef::SYSTEMMAIL_COIN_BIND].vint;
		mail_list->mail_list[count].mail_param.contentparam.gold = iter->m_data_area[tabledef::SYSTEMMAIL_GOLD].vint;
		mail_list->mail_list[count].mail_param.contentparam.gold_bind = iter->m_data_area[tabledef::SYSTEMMAIL_GOLD_BIND].vint;

		STRNCPY(mail_list->mail_list[count].mail_param.contentparam.subject, iter->m_data_area[tabledef::SYSTEMMAIL_SUBJECT].vcharp,
			sizeof(mail_list->mail_list[0].mail_param.contentparam.subject));
		STRNCPY(mail_list->mail_list[count].mail_param.contentparam.contenttxt, iter->m_data_area[tabledef::SYSTEMMAIL_CONTENT].vcharp,
			sizeof(mail_list->mail_list[0].mail_param.contentparam.contenttxt));

		for (int item_index = 0; item_index < MAX_ATTACHMENT_ITEM_NUM; item_index++)
		{
			mail_list->mail_list[count].mail_param.contentparam.item_list[item_index].item_id = iter->m_data_area[SYSMAIL_ITEM_ID_DEF[item_index]].vint;
			mail_list->mail_list[count].mail_param.contentparam.item_list[item_index].is_bind = iter->m_data_area[SYSMAIL_ITEM_IS_BIND_DEF[item_index]].vchar;
			mail_list->mail_list[count].mail_param.contentparam.item_list[item_index].num = iter->m_data_area[SYSMAIL_ITEM_NUM_DEF[item_index]].vint;
			mail_list->mail_list[count].mail_param.contentparam.item_list[item_index].invalid_time = iter->m_data_area[SYSMAIL_ITEM_INVALID_TIME_DEF[item_index]].vint;

			if (RMIGlobalObject::ItemParamInit(&mail_list->mail_list[count].mail_param.contentparam.item_list[item_index].param_data, 
				iter->m_data_area[SYSMAIL_ITEM_PARAM_DEF[item_index]].vcharp, iter->m_data_area[SYSMAIL_ITEM_PARAM_DEF[item_index]].length))
			{
				mail_list->mail_list[count].mail_param.contentparam.item_list[item_index].has_param = 1;
			}
			else
			{
				mail_list->mail_list[count].mail_param.contentparam.item_list[item_index].has_param = 0;
			}
		}

		{
			memset((void*)&mail_list->mail_list[count].mail_param.contentparam.virtual_item_list, 0, sizeof(MailVirtualItemList));

			unsigned int length = iter->m_data_area[tabledef::SYSTEMMAIL_VIRTUAL_ITEM_DATA].length;
			if (length > sizeof(MailVirtualItemList) * 2)
			{
				dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::GetRoleSystemMails error: length > sizeof(MailVirtualItemList) * 2");
				iter->Free();
				continue;
			}

			HexToBin(iter->m_data_area[tabledef::SYSTEMMAIL_VIRTUAL_ITEM_DATA].vcharp, length, (char*)&mail_list->mail_list[count].mail_param.contentparam.virtual_item_list);
		}

		iter->Free();
	}

	for (; iter != mails.end(); ++iter)
	{
		iter->Free();
	}

	mail_list->count = count;

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__RemoveRoleSystemMails(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__RemoveRoleSystemMails_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	int count;
	if (!in_param.Pop(&count) || count < 0 || count > MAX_MAIL_NUM)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__RemoveRoleSystemMails_2");
		return rmi::DispatchParamError;
	}

	long long systemmail_id = -1;
	int result = 0;

	for (int i = 0; i < count; ++i)
	{
		if (!in_param.Pop(&systemmail_id))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__RemoveRoleSystemMails_3");
			return rmi::DispatchParamError;
		}
		
		int remove_result = 0;
		if (0 != (remove_result = RemoveRoleSystemMail(systemmail_id)))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__RemoveRoleSystemMails_4 systemmail_id:%lld remove_result:%d", systemmail_id, remove_result);
			result = -1;
		}
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__RemoveRoleSystemMails_5");
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::RemoveRoleSystemMail(long long systemmail_id)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *systemmailcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_SYSTEMMAIL);

	conn->begin(false);

	int result = systemmailcommand->Remove(stmt, systemmail_id);
	if (result != DBCommand::RESULT_SUC)
	{
		conn->rollback();
		return result;
	}

	conn->commit();

	return 0;
}

int RMIGlobalObject::__InitPersonRank(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(PersonRankParam) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitPersonRank_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static PersonRankParam person_rank_param;
	person_rank_param.count = 0;

	int result = InitPersonRank(&person_rank_param);

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitPersonRank_2 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!person_rank_param.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitPersonRank_3 result:%d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitPersonRank_4 result:%d", result);
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::InitPersonRank(PersonRankParam *person_rank_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *rank_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_PERSONRANK);

	QueryCondition rank_condition;
	CmpNode rank_cmp(tabledef::PERSONRANK_RANK_INDEX);
	rank_cmp.SetInt32(0);
	rank_cmp.SetEqualType(EQUAL_TYPE_GRANTER_EQUAL);
	rank_condition.cmp_list.push_back(rank_cmp);

	CmpNode uid_cmp(tabledef::PERSONRANK_UID);
	uid_cmp.SetInt32(0);
	uid_cmp.SetEqualType(EQUAL_TYPE_GRANTER);
	rank_condition.cmp_list.push_back(uid_cmp);
	rank_condition.query_num = PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE;

	DataAdapterList rank_list;
	int ret = rank_command->Find(stmt, rank_condition, &rank_list);
	if (ret < 0)
	{
		person_rank_param->count = 0;
		return -1;
	}

	int count = 0;
	DataAdapterList::iterator i = rank_list.begin(); 

	for (; i != rank_list.end() && count < (PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE); ++i)
	{
		person_rank_param->person_rank_list[count].change_state = structcommon::CS_NONE;
		person_rank_param->person_rank_list[count].index = i->m_data_area[tabledef::PERSONRANK_RANK_INDEX].vint;
		person_rank_param->person_rank_list[count].uid = i->m_data_area[tabledef::PERSONRANK_UID].vint;
		STRNCPY(person_rank_param->person_rank_list[count].user_name, i->m_data_area[tabledef::PERSONRANK_USER_NAME].vcharp,
			sizeof(person_rank_param->person_rank_list[0].user_name));
		
		STRNCPY(person_rank_param->person_rank_list[count].flexible_name, i->m_data_area[tabledef::PERSONRANK_FLEXIBLE_NAME].vcharp,
			sizeof(person_rank_param->person_rank_list[0].flexible_name));

		person_rank_param->person_rank_list[count].sex = i->m_data_area[tabledef::PERSONRANK_ROLE_SEX].vint;
		person_rank_param->person_rank_list[count].prof = i->m_data_area[tabledef::PERSONRANK_ROLE_PROF].vint;
		person_rank_param->person_rank_list[count].camp = i->m_data_area[tabledef::PERSONRANK_ROLE_CAMP].vint;
		person_rank_param->person_rank_list[count].vip_level = i->m_data_area[tabledef::PERSONRANK_ROLE_VIP_LEVEL].vint;

		person_rank_param->person_rank_list[count].exp = i->m_data_area[tabledef::PERSONRANK_ROLE_EXP].vint64;
		person_rank_param->person_rank_list[count].level = i->m_data_area[tabledef::PERSONRANK_ROLE_LEVEL].vint;
		person_rank_param->person_rank_list[count].rank_type = i->m_data_area[tabledef::PERSONRANK_RANK_TYPE].vint;
		person_rank_param->person_rank_list[count].rank_value = i->m_data_area[tabledef::PERSONRANK_RANK_VALUE].vint64;
		person_rank_param->person_rank_list[count].record_index = i->m_data_area[tabledef::PERSONRANK_RECORD_INDEX].vint;
		person_rank_param->person_rank_list[count].flexible_int = i->m_data_area[tabledef::PERSONRANK_FLEXIBLE_INT].vint;
		person_rank_param->person_rank_list[count].flexible_ll = i->m_data_area[tabledef::PERSONRANK_FLEXIBLE_LL].vint64;

		++count;
		i->Free();
	}

	for (; i != rank_list.end(); ++i)
	{
		i->Free();
	}

	person_rank_param->count = count;

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__SavePersonRank(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SavePersonRank_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static PersonRankParam person_rank_param;
	person_rank_param.count = 0;

	if (!person_rank_param.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SavePersonRank_2");
		return rmi::DispatchParamError;
	}

	int result = SavePersonRank(person_rank_param);

	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SavePersonRank_3 result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SavePersonRank_4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::SavePersonRank(const PersonRankParam &person_rank_param)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *rank_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_PERSONRANK);
	DBCommand *rank_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_PERSONRANK);

	conn->begin();

	for (int i = 0; i < person_rank_param.count && i < PERSON_RANK_TYPE_MAX * PERSON_RANK_MAX_SIZE; ++i)
	{
		switch (person_rank_param.person_rank_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_nodes;

				CmpNode index_node(tabledef::PERSONRANK_RANK_INDEX);
				index_node.SetInt32(person_rank_param.person_rank_list[i].index);
				cmp_nodes.push_back(index_node);

				int count = rank_command->Remove(stmt, cmp_nodes);
				if (count < 1)
				{
					ret = -1;
				}
			}
			break; // case structcommon::CS_DELETE

		case structcommon::CS_INSERT:
			{
				DataAdapter rank_item = rank_table->GetPrototype();
				rank_item.Malloc();
				
				rank_item.m_data_area[tabledef::PERSONRANK_RANK_INDEX].vint = person_rank_param.person_rank_list[i].index;
				rank_item.m_data_area[tabledef::PERSONRANK_UID].vint = person_rank_param.person_rank_list[i].uid;
				STRNCPY(rank_item.m_data_area[tabledef::PERSONRANK_USER_NAME].vcharp, person_rank_param.person_rank_list[i].user_name,
					rank_item.m_data_area[tabledef::PERSONRANK_USER_NAME].length);
				
				STRNCPY(rank_item.m_data_area[tabledef::PERSONRANK_FLEXIBLE_NAME].vcharp, person_rank_param.person_rank_list[i].flexible_name,
					rank_item.m_data_area[tabledef::PERSONRANK_FLEXIBLE_NAME].length);

				rank_item.m_data_area[tabledef::PERSONRANK_ROLE_SEX].vint = person_rank_param.person_rank_list[i].sex;
				rank_item.m_data_area[tabledef::PERSONRANK_ROLE_PROF].vint = person_rank_param.person_rank_list[i].prof;
				rank_item.m_data_area[tabledef::PERSONRANK_ROLE_CAMP].vint = person_rank_param.person_rank_list[i].camp;
				rank_item.m_data_area[tabledef::PERSONRANK_ROLE_VIP_LEVEL].vint = person_rank_param.person_rank_list[i].vip_level;
				
				rank_item.m_data_area[tabledef::PERSONRANK_ROLE_EXP].vint64 = person_rank_param.person_rank_list[i].exp;
				rank_item.m_data_area[tabledef::PERSONRANK_ROLE_LEVEL].vint = person_rank_param.person_rank_list[i].level;
				rank_item.m_data_area[tabledef::PERSONRANK_RANK_TYPE].vint = person_rank_param.person_rank_list[i].rank_type;
				rank_item.m_data_area[tabledef::PERSONRANK_RANK_VALUE].vint64 = person_rank_param.person_rank_list[i].rank_value;
				rank_item.m_data_area[tabledef::PERSONRANK_RECORD_INDEX].vint = person_rank_param.person_rank_list[i].record_index;
				rank_item.m_data_area[tabledef::PERSONRANK_FLEXIBLE_INT].vint = person_rank_param.person_rank_list[i].flexible_int;
				rank_item.m_data_area[tabledef::PERSONRANK_FLEXIBLE_LL].vint64 = person_rank_param.person_rank_list[i].flexible_ll;

				if (DBCommand::RESULT_SUC != rank_command->Save(stmt, &rank_item))
				{
					ret = -2;
				}

				rank_item.Free();
			}
			break; // case structcommon::CS_INSERT

		case structcommon::CS_UPDATE:
			{
				DataNodeList set_nodes;

				DataNode uid_node(tabledef::PERSONRANK_UID);
				uid_node.SetInt32(person_rank_param.person_rank_list[i].uid);
				set_nodes.push_back(uid_node);

				DataNode user_name_node(tabledef::PERSONRANK_USER_NAME);
				user_name_node.SetString((const char *)person_rank_param.person_rank_list[i].user_name);
				set_nodes.push_back(user_name_node);

				DataNode flexible_name_node(tabledef::PERSONRANK_FLEXIBLE_NAME);
				flexible_name_node.SetString((const char *)person_rank_param.person_rank_list[i].flexible_name);
				set_nodes.push_back(flexible_name_node);

				DataNode role_gender_node(tabledef::PERSONRANK_ROLE_SEX);
				role_gender_node.SetInt32(person_rank_param.person_rank_list[i].sex);
				set_nodes.push_back(role_gender_node);

				DataNode role_prof_node(tabledef::PERSONRANK_ROLE_PROF);
				role_prof_node.SetInt32(person_rank_param.person_rank_list[i].prof);
				set_nodes.push_back(role_prof_node);

				DataNode role_camp_node(tabledef::PERSONRANK_ROLE_CAMP);
				role_camp_node.SetInt32(person_rank_param.person_rank_list[i].camp);
				set_nodes.push_back(role_camp_node);

				DataNode role_vip_level_node(tabledef::PERSONRANK_ROLE_VIP_LEVEL);
				role_vip_level_node.SetInt32(person_rank_param.person_rank_list[i].vip_level);
				set_nodes.push_back(role_vip_level_node);

				DataNode role_exp_node(tabledef::PERSONRANK_ROLE_EXP);
				role_exp_node.SetLong64((person_rank_param.person_rank_list[i].exp));
				set_nodes.push_back(role_exp_node);

				DataNode role_level_node(tabledef::PERSONRANK_ROLE_LEVEL);
				role_level_node.SetInt32((int)(person_rank_param.person_rank_list[i].level));
				set_nodes.push_back(role_level_node);

				DataNode rank_type_node(tabledef::PERSONRANK_RANK_TYPE);
				rank_type_node.SetInt32(person_rank_param.person_rank_list[i].rank_type);
				set_nodes.push_back(rank_type_node);

				DataNode rank_value_node(tabledef::PERSONRANK_RANK_VALUE);
				rank_value_node.SetLong64(person_rank_param.person_rank_list[i].rank_value);
				set_nodes.push_back(rank_value_node);

				DataNode record_index_node(tabledef::PERSONRANK_RECORD_INDEX);
				record_index_node.SetInt32(person_rank_param.person_rank_list[i].record_index);
				set_nodes.push_back(record_index_node);

				DataNode flexible_int_node(tabledef::PERSONRANK_FLEXIBLE_INT);
				flexible_int_node.SetInt32(person_rank_param.person_rank_list[i].flexible_int);
				set_nodes.push_back(flexible_int_node);

				DataNode flexible_ll_node(tabledef::PERSONRANK_FLEXIBLE_LL);
				flexible_ll_node.SetLong64(person_rank_param.person_rank_list[i].flexible_ll);
				set_nodes.push_back(flexible_ll_node);

				CmpNodeList cmp_nodes;
				CmpNode index_node(tabledef::PERSONRANK_RANK_INDEX);
				index_node.SetInt32(person_rank_param.person_rank_list[i].index);
				cmp_nodes.push_back(index_node);

				CmpNode cmp_uid_node(tabledef::PERSONRANK_UID);
				cmp_uid_node.SetInt32(0);
				cmp_uid_node.SetEqualType(EQUAL_TYPE_GRANTER);
				cmp_nodes.push_back(cmp_uid_node);

				int count = rank_command->Update(stmt, cmp_nodes, set_nodes);
				if (count < 0)
				{
					ret = -3; 
				}
			}
			break; // case structcommon::CS_UPDATE

		} // switch end

	} // for end

	conn->commit();

	return ret < 0 ? ret : 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
int RMIGlobalObject::__InitTuodan(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(TuodanParam) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitTuodan_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	int tuodan_id_from = 0;

	if (!in_param.Pop(&tuodan_id_from))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitTuodan_2");
		return rmi::DispatchParamError;
	}

	static TuodanParam tuodan_list;
	tuodan_list.count = 0;

	int result = InitTuodan(tuodan_id_from, &tuodan_list);

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitTuodan_3 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!tuodan_list.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitTuodan_4 result:%d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitTuodan_5 result:%d", result);
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::InitTuodan(int tuodan_id_from, TuodanParam *tuodan_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *tuodan_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_TUODAN);

	QueryCondition tuodan_condition;

	CmpNode tuodan_attr_node1(tabledef::TUODAN_TUODAN_ID);
	tuodan_attr_node1.SetInt32(tuodan_id_from);
	tuodan_attr_node1.SetEqualType(EQUAL_TYPE_GRANTER);

	tuodan_condition.cmp_list.push_back(tuodan_attr_node1);
	tuodan_condition.query_num = MAX_ONCE_TUODAN_COUNT;

	DataAdapterList tuodan_list;
	int ret = tuodan_command->Find(stmt, tuodan_condition, &tuodan_list);
	if (ret < 0)
	{
		tuodan_param->count = 0;
		return -1;
	}

	ret = 0;

	int count = 0;
	DataAdapterList::iterator i = tuodan_list.begin();
	for (; i != tuodan_list.end() && count < MAX_ONCE_TUODAN_COUNT; ++i)
	{
		tuodan_param->tuodan_list[count].change_state = structcommon::CS_NONE;
		tuodan_param->tuodan_list[count].index = i->m_data_area[tabledef::TUODAN_TUODAN_ID].vint;
		tuodan_param->tuodan_list[count].uid = i->m_data_area[tabledef::TUODAN_ROLE_ID].vint;
		tuodan_param->tuodan_list[count].create_time = i->m_data_area[tabledef::TUODAN_CREATE_TIME].vint;
		STRNCPY(tuodan_param->tuodan_list[count].notice, i->m_data_area[tabledef::TUODAN_NOTICE].vcharp,
			sizeof(tuodan_param->tuodan_list[0].notice));

		++count;
		i->Free();
	}

	tuodan_param->count = count;

	for (; i != tuodan_list.end(); ++i)
	{
		i->Free();
	}

	return ret < 0 ? ret : 0;
}	

int RMIGlobalObject::__SaveTuodan(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveTuodan_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static TuodanParam tuodan_param;
	tuodan_param.count = 0;

	if (!tuodan_param.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveTuodan_2");
		return rmi::DispatchParamError;
	}

	int result = SaveTuodan(tuodan_param);

	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveTuodan_3 result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveTuodan_4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::SaveTuodan(const TuodanParam &tuodan_param)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *tuodan_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_TUODAN);
	DBCommand *tuodan_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_TUODAN);

	conn->begin();

	for (int i = 0; i < tuodan_param.count && i < MAX_ONCE_TUODAN_COUNT; ++i)
	{
		switch (tuodan_param.tuodan_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_nodes;

				CmpNode cmp_uid_node(tabledef::TUODAN_ROLE_ID);
				cmp_uid_node.SetInt32(tuodan_param.tuodan_list[i].uid);
				cmp_uid_node.SetEqualType(EQUAL_TYPE_EQUAL);
				cmp_nodes.push_back(cmp_uid_node);

				int count = tuodan_command->Remove(stmt, cmp_nodes);
				if (count < 1)
				{
					ret = -1;
				}
			}
			break; // case structcommon::CS_DELETE

		case structcommon::CS_INSERT:
			{
				DataAdapter tuodan_item = tuodan_table->GetPrototype();
				tuodan_item.Malloc();

				tuodan_item.m_data_area[tabledef::TUODAN_ROLE_ID].vint = tuodan_param.tuodan_list[i].uid;
				tuodan_item.m_data_area[tabledef::TUODAN_CREATE_TIME].vint = tuodan_param.tuodan_list[i].create_time;
				STRNCPY(tuodan_item.m_data_area[tabledef::TUODAN_NOTICE].vcharp, tuodan_param.tuodan_list[i].notice,
					tuodan_item.m_data_area[tabledef::TUODAN_NOTICE].length);

				if (DBCommand::RESULT_SUC != tuodan_command->Save(stmt, &tuodan_item))
				{
					ret = -2;
				}

				tuodan_item.Free();
			}
			break; // case structcommon::CS_INSERT

		case structcommon::CS_UPDATE:
			{
				DataNodeList set_nodes;
				
				DataNode create_time_node(tabledef::TUODAN_CREATE_TIME);
				create_time_node.SetInt32(tuodan_param.tuodan_list[i].create_time);
				set_nodes.push_back(create_time_node);

				DataNode notice_node(tabledef::TUODAN_NOTICE);
				notice_node.SetString(tuodan_param.tuodan_list[i].notice);
				set_nodes.push_back(notice_node);

				CmpNodeList cmp_nodes;
				CmpNode cmp_uid_node(tabledef::TUODAN_ROLE_ID);
				cmp_uid_node.SetInt32(tuodan_param.tuodan_list[i].uid);
				cmp_uid_node.SetEqualType(EQUAL_TYPE_EQUAL);
				cmp_nodes.push_back(cmp_uid_node);

				int count = tuodan_command->Update(stmt, cmp_nodes, set_nodes);
				if (count < 0)
				{
					ret = -3; 
				}
			}
			break; // case structcommon::CS_UPDATE

		} // switch end

	} // for end

	conn->commit();

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__InitTeamRank(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(TeamRankParam) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitTeamRank_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static TeamRankParam team_rank_param;
	team_rank_param.count = 0;

	int result = InitTeamRank(&team_rank_param);

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitTeamRank_2 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!team_rank_param.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitTeamRank_3 result:%d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitTeamRank_4 result:%d", result);
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::InitTeamRank(TeamRankParam *team_rank_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *rank_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_TEAMRANK);

	QueryCondition rank_condition;
	CmpNode rank_cmp(tabledef::TEAMRANK_RANK_INDEX);
	rank_cmp.SetInt32(0);
	rank_cmp.SetEqualType(EQUAL_TYPE_GRANTER_EQUAL);
	rank_condition.cmp_list.push_back(rank_cmp);

	CmpNode uid_cmp(tabledef::TEAMRANK_UID1);
	uid_cmp.SetInt32(0);
	uid_cmp.SetEqualType(EQUAL_TYPE_GRANTER);
	rank_condition.cmp_list.push_back(uid_cmp);
	rank_condition.query_num = TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE;

	DataAdapterList rank_list;
	int ret = rank_command->Find(stmt, rank_condition, &rank_list);
	if (ret < 0)
	{
		team_rank_param->count = 0;
		return -1;
	}

	int count = 0;
	DataAdapterList::iterator i = rank_list.begin(); 

	for (; i != rank_list.end() && count < (TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE); ++i)
	{
		team_rank_param->team_rank_list[count].change_state = structcommon::CS_NONE;
		team_rank_param->team_rank_list[count].index = i->m_data_area[tabledef::TEAMRANK_RANK_INDEX].vint;

		UNSTD_STATIC_CHECK(sizeof(team_rank_param->team_rank_list[0].member_list) / sizeof(team_rank_param->team_rank_list[0].member_list[0]) == 3);
		team_rank_param->team_rank_list[count].member_list[0].uid = i->m_data_area[tabledef::TEAMRANK_UID1].vint;
		team_rank_param->team_rank_list[count].member_list[1].uid = i->m_data_area[tabledef::TEAMRANK_UID2].vint;
		team_rank_param->team_rank_list[count].member_list[2].uid = i->m_data_area[tabledef::TEAMRANK_UID3].vint;
		STRNCPY(team_rank_param->team_rank_list[count].member_list[0].user_name, i->m_data_area[tabledef::TEAMRANK_USER_NAME1].vcharp, sizeof(team_rank_param->team_rank_list[0].member_list[0].user_name));
		STRNCPY(team_rank_param->team_rank_list[count].member_list[1].user_name, i->m_data_area[tabledef::TEAMRANK_USER_NAME2].vcharp, sizeof(team_rank_param->team_rank_list[0].member_list[0].user_name));
		STRNCPY(team_rank_param->team_rank_list[count].member_list[2].user_name, i->m_data_area[tabledef::TEAMRANK_USER_NAME3].vcharp, sizeof(team_rank_param->team_rank_list[0].member_list[0].user_name));
		team_rank_param->team_rank_list[count].member_list[0].camp = i->m_data_area[tabledef::TEAMRANK_CAMP1].vchar;
		team_rank_param->team_rank_list[count].member_list[1].camp = i->m_data_area[tabledef::TEAMRANK_CAMP2].vchar;
		team_rank_param->team_rank_list[count].member_list[2].camp = i->m_data_area[tabledef::TEAMRANK_CAMP3].vchar;

		team_rank_param->team_rank_list[count].rank_type = i->m_data_area[tabledef::TEAMRANK_RANK_TYPE].vint;
		team_rank_param->team_rank_list[count].rank_value = i->m_data_area[tabledef::TEAMRANK_RANK_VALUE].vint64;
		team_rank_param->team_rank_list[count].flexible_int = i->m_data_area[tabledef::TEAMRANK_FLEXIBLE_INT].vint;
		team_rank_param->team_rank_list[count].flexible_ll = i->m_data_area[tabledef::TEAMRANK_FLEXIBLE_LL].vint64;

		++count;
		i->Free();
	}

	for (; i != rank_list.end(); ++i)
	{
		i->Free();
	}

	team_rank_param->count = count;

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__SaveTeamRank(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveTeamRank_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static TeamRankParam team_rank_param;
	team_rank_param.count = 0;

	if (!team_rank_param.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveTeamRank_2");
		return rmi::DispatchParamError;
	}

	int result = SaveTeamRank(team_rank_param);

	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveTeamRank_3 result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveTeamRank_4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::SaveTeamRank(const TeamRankParam &team_rank_param)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *rank_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_TEAMRANK);
	DBCommand *rank_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_TEAMRANK);

	conn->begin();

	for (int i = 0; i < team_rank_param.count && i < TEAM_RANK_TYPE_MAX * TEAM_RANK_MAX_SIZE; ++i)
	{
		switch (team_rank_param.team_rank_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_nodes;

				CmpNode index_node(tabledef::TEAMRANK_RANK_INDEX);
				index_node.SetInt32(team_rank_param.team_rank_list[i].index);
				cmp_nodes.push_back(index_node);

				int count = rank_command->Remove(stmt, cmp_nodes);
				if (count < 1)
				{
					ret = -1;
				}
			}
			break; // case structcommon::CS_DELETE

		case structcommon::CS_INSERT:
			{
				DataAdapter rank_item = rank_table->GetPrototype();
				rank_item.Malloc();

				rank_item.m_data_area[tabledef::TEAMRANK_RANK_INDEX].vint = team_rank_param.team_rank_list[i].index;

				UNSTD_STATIC_CHECK(sizeof(team_rank_param.team_rank_list[0].member_list) / sizeof(team_rank_param.team_rank_list[0].member_list[0]) == 3);
				rank_item.m_data_area[tabledef::TEAMRANK_UID1].vint = team_rank_param.team_rank_list[i].member_list[0].uid;
				rank_item.m_data_area[tabledef::TEAMRANK_UID2].vint = team_rank_param.team_rank_list[i].member_list[1].uid;
				rank_item.m_data_area[tabledef::TEAMRANK_UID3].vint = team_rank_param.team_rank_list[i].member_list[2].uid;
				STRNCPY(rank_item.m_data_area[tabledef::TEAMRANK_USER_NAME1].vcharp, team_rank_param.team_rank_list[i].member_list[0].user_name, rank_item.m_data_area[tabledef::TEAMRANK_USER_NAME1].length);
				STRNCPY(rank_item.m_data_area[tabledef::TEAMRANK_USER_NAME2].vcharp, team_rank_param.team_rank_list[i].member_list[1].user_name, rank_item.m_data_area[tabledef::TEAMRANK_USER_NAME2].length);
				STRNCPY(rank_item.m_data_area[tabledef::TEAMRANK_USER_NAME3].vcharp, team_rank_param.team_rank_list[i].member_list[2].user_name, rank_item.m_data_area[tabledef::TEAMRANK_USER_NAME3].length);
				rank_item.m_data_area[tabledef::TEAMRANK_CAMP1].vchar = static_cast<char>(team_rank_param.team_rank_list[i].member_list[0].camp);
				rank_item.m_data_area[tabledef::TEAMRANK_CAMP2].vchar = static_cast<char>(team_rank_param.team_rank_list[i].member_list[1].camp);
				rank_item.m_data_area[tabledef::TEAMRANK_CAMP3].vchar = static_cast<char>(team_rank_param.team_rank_list[i].member_list[2].camp);

				rank_item.m_data_area[tabledef::TEAMRANK_RANK_TYPE].vint = team_rank_param.team_rank_list[i].rank_type;
				rank_item.m_data_area[tabledef::TEAMRANK_RANK_VALUE].vint64 = team_rank_param.team_rank_list[i].rank_value;
				rank_item.m_data_area[tabledef::TEAMRANK_FLEXIBLE_INT].vint = team_rank_param.team_rank_list[i].flexible_int;
				rank_item.m_data_area[tabledef::TEAMRANK_FLEXIBLE_LL].vint64 = team_rank_param.team_rank_list[i].flexible_ll;

				if (DBCommand::RESULT_SUC != rank_command->Save(stmt, &rank_item))
				{
					ret = -2;
				}

				rank_item.Free();
			}
			break; // case structcommon::CS_INSERT

		case structcommon::CS_UPDATE:
			{
				DataNodeList set_nodes;

				UNSTD_STATIC_CHECK(sizeof(team_rank_param.team_rank_list[0].member_list) / sizeof(team_rank_param.team_rank_list[0].member_list[0]) == 3);

				DataNode uid_node1(tabledef::TEAMRANK_UID1);
				uid_node1.SetInt32(team_rank_param.team_rank_list[i].member_list[0].uid);
				set_nodes.push_back(uid_node1);

				DataNode uid_node2(tabledef::TEAMRANK_UID2);
				uid_node2.SetInt32(team_rank_param.team_rank_list[i].member_list[1].uid);
				set_nodes.push_back(uid_node2);

				DataNode uid_node3(tabledef::TEAMRANK_UID3);
				uid_node3.SetInt32(team_rank_param.team_rank_list[i].member_list[2].uid);
				set_nodes.push_back(uid_node3);

				DataNode user_name_node1(tabledef::TEAMRANK_USER_NAME1);
				user_name_node1.SetString((const char *)team_rank_param.team_rank_list[i].member_list[0].user_name);
				set_nodes.push_back(user_name_node1);

				DataNode user_name_node2(tabledef::TEAMRANK_USER_NAME2);
				user_name_node2.SetString((const char *)team_rank_param.team_rank_list[i].member_list[1].user_name);
				set_nodes.push_back(user_name_node2);

				DataNode user_name_node3(tabledef::TEAMRANK_USER_NAME3);
				user_name_node3.SetString((const char *)team_rank_param.team_rank_list[i].member_list[2].user_name);
				set_nodes.push_back(user_name_node3);

				DataNode camp_node1(tabledef::TEAMRANK_CAMP1);
				camp_node1.SetByte(static_cast<char>(team_rank_param.team_rank_list[i].member_list[0].camp));
				set_nodes.push_back(camp_node1);

				DataNode camp_node2(tabledef::TEAMRANK_CAMP2);
				camp_node2.SetByte(static_cast<char>(team_rank_param.team_rank_list[i].member_list[1].camp));
				set_nodes.push_back(camp_node2);

				DataNode camp_node3(tabledef::TEAMRANK_CAMP3);
				camp_node3.SetByte(static_cast<char>(team_rank_param.team_rank_list[i].member_list[2].camp));
				set_nodes.push_back(camp_node3);

				DataNode rank_type_node(tabledef::TEAMRANK_RANK_TYPE);
				rank_type_node.SetInt32(team_rank_param.team_rank_list[i].rank_type);
				set_nodes.push_back(rank_type_node);

				DataNode rank_value_node(tabledef::TEAMRANK_RANK_VALUE);
				rank_value_node.SetLong64(team_rank_param.team_rank_list[i].rank_value);
				set_nodes.push_back(rank_value_node);

				DataNode flexible_int_node(tabledef::TEAMRANK_FLEXIBLE_INT);
				flexible_int_node.SetInt32(team_rank_param.team_rank_list[i].flexible_int);
				set_nodes.push_back(flexible_int_node);

				DataNode flexible_ll_node(tabledef::TEAMRANK_FLEXIBLE_LL);
				flexible_ll_node.SetLong64(team_rank_param.team_rank_list[i].flexible_ll);
				set_nodes.push_back(flexible_ll_node);

				CmpNodeList cmp_nodes;
				CmpNode index_node(tabledef::TEAMRANK_RANK_INDEX);
				index_node.SetInt32(team_rank_param.team_rank_list[i].index);
				cmp_nodes.push_back(index_node);

				CmpNode cmp_uid_node(tabledef::TEAMRANK_UID1);
				cmp_uid_node.SetInt32(0);
				cmp_uid_node.SetEqualType(EQUAL_TYPE_GRANTER);
				cmp_nodes.push_back(cmp_uid_node);

				int count = rank_command->Update(stmt, cmp_nodes, set_nodes);
				if (count < 0)
				{
					ret = -3; 
				}
			}
			break; // case structcommon::CS_UPDATE

		} // switch end

	} // for end

	conn->commit();

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__InitWorldStatus(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(WorldStatusParam) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitWorldStatus_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	long long id_from = 0;

	if (!in_param.Pop(&id_from))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitWorldStatus_2");
		return rmi::DispatchParamError;
	}

	static WorldStatusParam worldstatus_param;
	memset(&worldstatus_param, 0, sizeof(worldstatus_param));

	int result = InitWorldStatus(id_from, &worldstatus_param);

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitWorldStatus_3 result %d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!worldstatus_param.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitWorldStatus_4 result %d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else 
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitWorldStatus_5 result %d", result);
	}

	return rmi::DispatchOK;	
}

int RMIGlobalObject::InitWorldStatus(long long id_from, WorldStatusParam *worldstatus_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *worldstatus_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_WORLDSTATUS);

	QueryCondition worldstatus_cond;
	CmpNode worldstate_node1(tabledef::WORLDSTATUS_IDWORLDSTATUS);
	worldstate_node1.SetLong64(id_from);
	worldstate_node1.SetEqualType(EQUAL_TYPE_GRANTER);
	worldstatus_cond.cmp_list.push_back(worldstate_node1);
	worldstatus_cond.query_num = 1;

	DataAdapterList worldstate_list;
	int ret = worldstatus_command->Find(stmt, worldstatus_cond, &worldstate_list);
	if (ret < 0)
	{
		return ret;
	}

	ret = 0;

	DataAdapterList::iterator i = worldstate_list.begin();
	for (; i != worldstate_list.end(); ++i)
	{
		worldstatus_param->start_time = static_cast<unsigned int>(i->m_data_area[tabledef::WORLDSTATUS_START_TIME].vint64);
		worldstatus_param->world_time = static_cast<unsigned int>(i->m_data_area[tabledef::WORLDSTATUS_WORLD_TIME].vint);
		worldstatus_param->last_update_dayid = static_cast<unsigned int>(i->m_data_area[tabledef::WORLDSTATUS_LAST_UPDATE_DAYID].vint64);
		worldstatus_param->xianmengzhan_mengzhu_id = i->m_data_area[tabledef::WORLDSTATUS_XIANMENGZHAN_MENGZHU_ID].vint;

		///////////////////////////////////////////////////////////////////////////////////
		{
			memset(&worldstatus_param->activity_open_records, 0, sizeof(worldstatus_param->activity_open_records));

			int length = i->m_data_area[tabledef::WORLDSTATUS_ACTIVITY_OPEN_RECORDS].length;
			if (length <= (int)sizeof(worldstatus_param->activity_open_records) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS_ACTIVITY_OPEN_RECORDS].vcharp, length, (char*)&worldstatus_param->activity_open_records);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////
		{
			memset(&worldstatus_param->common_data, 0, sizeof(worldstatus_param->common_data));

			int length = i->m_data_area[tabledef::WORLDSTATUS_COMMON_DATA].length;
			if (length <= (int)sizeof(worldstatus_param->common_data) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS_COMMON_DATA].vcharp, length, (char*)&worldstatus_param->common_data);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////
		{
			memset(&worldstatus_param->challenge_field_data, 0, sizeof(worldstatus_param->challenge_field_data));

			int length = i->m_data_area[tabledef::WORLDSTATUS_CHALLENGE_FIELD_DATA].length;
			if (length <= (int)sizeof(worldstatus_param->challenge_field_data) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS_CHALLENGE_FIELD_DATA].vcharp, length, (char*)&worldstatus_param->challenge_field_data);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////
		{
			memset(&worldstatus_param->world_event_param, 0, sizeof(worldstatus_param->world_event_param));

			int length = i->m_data_area[tabledef::WORLDSTATUS_WORLD_EVENT_DATA].length;
			if (length <= (int)sizeof(worldstatus_param->world_event_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS_WORLD_EVENT_DATA].vcharp, length, (char*)&worldstatus_param->world_event_param);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////
		{
			memset(&worldstatus_param->csa_activity_rank_param, 0, sizeof(worldstatus_param->csa_activity_rank_param));

			int length = i->m_data_area[tabledef::WORLDSTATUS_CSA_RANK_DATA].length;
			if (length <= (int)sizeof(worldstatus_param->csa_activity_rank_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS_CSA_RANK_DATA].vcharp, length, (char*)&worldstatus_param->csa_activity_rank_param);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////
		{
			memset(&worldstatus_param->csa_activity_other_param, 0, sizeof(worldstatus_param->csa_activity_other_param));

			int length = i->m_data_area[tabledef::WORLDSTATUS_CSA_OTHER_DATA].length;
			if (length <= (int)sizeof(worldstatus_param->csa_activity_other_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS_CSA_OTHER_DATA].vcharp, length, (char*)&worldstatus_param->csa_activity_other_param);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////
// 		{
// 			memset(&worldstatus_param->rand_activity_param, 0, sizeof(worldstatus_param->rand_activity_param));
// 
// 			int length = i->m_data_area[tabledef::WORLDSTATUS_RAND_ACTIVITY_DATA].length;
// 			if (length <= (int)sizeof(worldstatus_param->rand_activity_param) * 2)
// 			{
// 				HexToBin(i->m_data_area[tabledef::WORLDSTATUS_RAND_ACTIVITY_DATA].vcharp, length, (char*)&worldstatus_param->rand_activity_param);
// 			}
// 		}

		//////////////////////////////////////////////////////////////////////////////////
		{
			memset(&worldstatus_param->boss_refresh_param, 0, sizeof(worldstatus_param->boss_refresh_param));

			int length = i->m_data_area[tabledef::WORLDSTATUS_BOSS_REFRESH_DATA].length;
			if (length <= (int)sizeof(worldstatus_param->boss_refresh_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS_BOSS_REFRESH_DATA].vcharp, length, (char*)&worldstatus_param->boss_refresh_param);
			}
		}

		//////////////////////////////////////////////////////////////////////////////////
		{
			memset(&worldstatus_param->cross_guildbattle_param, 0, sizeof(worldstatus_param->cross_guildbattle_param));

			int length = i->m_data_area[tabledef::WORLDSTATUS_CROSS_GUILDBATTLE_DATA].length;
			if (length <= (int)sizeof(worldstatus_param->cross_guildbattle_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS_CROSS_GUILDBATTLE_DATA].vcharp, length, (char*)&worldstatus_param->cross_guildbattle_param);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////
		{
			memset(&worldstatus_param->csa_boss_rank_param, 0, sizeof(worldstatus_param->csa_boss_rank_param));

			int length = i->m_data_area[tabledef::WORLDSTATUS_CSA_BOSS_RANK_DATA].length;
			if (length <= (int)sizeof(worldstatus_param->csa_boss_rank_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS_CSA_BOSS_RANK_DATA].vcharp, length, (char*)&worldstatus_param->csa_boss_rank_param);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////
		{
			memset(&worldstatus_param->cross_rand_activity_param, 0, sizeof(worldstatus_param->cross_rand_activity_param));

			int length = i->m_data_area[tabledef::WORLDSTATUS_CROSS_RAND_ACTIVITY_DATA].length;
			if (length <= (int)sizeof(worldstatus_param->cross_rand_activity_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS_CROSS_RAND_ACTIVITY_DATA].vcharp, length, (char*)&worldstatus_param->cross_rand_activity_param);
			}
		}

		{
			memset(&worldstatus_param->cross_rand_activity_big_param, 0, sizeof(worldstatus_param->cross_rand_activity_big_param));

			int length = i->m_data_area[tabledef::WORLDSTATUS_CROSS_RAND_ACTIVITY_BIG_DATA].length;
			if (length <= (int)sizeof(worldstatus_param->cross_rand_activity_big_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS_CROSS_RAND_ACTIVITY_BIG_DATA].vcharp, length, (char*)&worldstatus_param->cross_rand_activity_big_param);
			}
		}

		///////////////////////////////////////////////////////////////////////////////////
		{
			memset(&worldstatus_param->cross_rand_activity_big_param2, 0, sizeof(worldstatus_param->cross_rand_activity_big_param2));

			int length = i->m_data_area[tabledef::WORLDSTATUS_CROSS_RAND_ACTIVITY_BIG_DATA2].length;
			if (length <= (int)sizeof(worldstatus_param->cross_rand_activity_big_param2) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS_CROSS_RAND_ACTIVITY_BIG_DATA2].vcharp, length, (char*)&worldstatus_param->cross_rand_activity_big_param2);
			}
		}

		{
			memset(&worldstatus_param->cross_cloudpurchase_param, 0, sizeof(worldstatus_param->cross_cloudpurchase_param));

			int length = i->m_data_area[tabledef::WORLDSTATUS_CROSS_RAND_ACTIVITY_CLOUD_PURCHASE_DATA].length;
			if (length <= (int)sizeof(worldstatus_param->cross_cloudpurchase_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS_CROSS_RAND_ACTIVITY_CLOUD_PURCHASE_DATA].vcharp, length, (char*)&worldstatus_param->cross_cloudpurchase_param);
			}
		}

		// i->Free(); TODO 非常严重 这种错误相当严重！！！
		break;  // 只取一条记录 
	}

	for (; i != worldstate_list.end(); ++i)
	{
		i->Free();
	}

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__SaveWorldStatus(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveWorldStatus_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static WorldStatusParam worldstatus_param;

	if (!worldstatus_param.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveWorldStatus_2");
		return rmi::DispatchParamError;
	}

	int result = SaveWorldStatus(worldstatus_param);
	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveWorldStatus_3 result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "v::__SaveWorldStatus_4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::SaveWorldStatus(const WorldStatusParam &worldstatus_param)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *worldstatus_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_WORLDSTATUS);
	DBCommand *worldstatus_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_WORLDSTATUS);

	DataNodeList worldstatus_item_list;
	worldstatus_item_list.reserve(tabledef::TABLE_WORLDSTATUS_MAXINDEX);

	//////////////////////////////////////////////////////////////////////////
	DataNode start_time(tabledef::WORLDSTATUS_START_TIME);
	start_time.SetLong64(worldstatus_param.start_time);
	worldstatus_item_list.push_back(start_time);

	//////////////////////////////////////////////////////////////////////////
	DataNode world_time(tabledef::WORLDSTATUS_WORLD_TIME);
	world_time.SetInt32(worldstatus_param.world_time);
	worldstatus_item_list.push_back(world_time);

	//////////////////////////////////////////////////////////////////////////
	DataNode last_update_dayid(tabledef::WORLDSTATUS_LAST_UPDATE_DAYID);
	last_update_dayid.SetLong64(worldstatus_param.last_update_dayid);
	worldstatus_item_list.push_back(last_update_dayid);

	//////////////////////////////////////////////////////////////////////////
	DataNode mengzhu_id(tabledef::WORLDSTATUS_XIANMENGZHAN_MENGZHU_ID);
	mengzhu_id.SetInt32(worldstatus_param.xianmengzhan_mengzhu_id);
	worldstatus_item_list.push_back(mengzhu_id);

	//////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(activity_open_record_node, tabledef::WORLDSTATUS_ACTIVITY_OPEN_RECORDS, 
		worldstatus_param.activity_open_records, ActivityOpenRecordsHex, worldstatus_item_list);

	//////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(common_data_node, tabledef::WORLDSTATUS_COMMON_DATA, 
		worldstatus_param.common_data, WorldStatusCommonDataParamHex, worldstatus_item_list);

	//////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(challenge_field_data_node, tabledef::WORLDSTATUS_CHALLENGE_FIELD_DATA, 
		worldstatus_param.challenge_field_data, ChallengeFieldDataParamHex, worldstatus_item_list);

	//////////////////////////////////////////////////////////////////////////
// 	UPDATE_BINARY_DATA_TO_LIST(rand_activity_data_node, tabledef::WORLDSTATUS_RAND_ACTIVITY_DATA, 
// 		worldstatus_param.rand_activity_param, RandActivityParamHex, worldstatus_item_list);

	//////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(world_event_data_node, tabledef::WORLDSTATUS_WORLD_EVENT_DATA, 
		worldstatus_param.world_event_param, WorldEventParamHex, worldstatus_item_list);

	//////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(csa_rank_data_node, tabledef::WORLDSTATUS_CSA_RANK_DATA, 
		worldstatus_param.csa_activity_rank_param, CombineServerActivityRankParamHex, worldstatus_item_list);

	//////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(csa_other_data_node, tabledef::WORLDSTATUS_CSA_OTHER_DATA, 
		worldstatus_param.csa_activity_other_param, CombineServerActivityOtherParamHex, worldstatus_item_list);

	//////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(boss_refresh_data_node, tabledef::WORLDSTATUS_BOSS_REFRESH_DATA,
		worldstatus_param.boss_refresh_param, BossSpecialRefreshParamHex, worldstatus_item_list);

	//////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(cross_guildbattle_data_node, tabledef::WORLDSTATUS_CROSS_GUILDBATTLE_DATA,
		worldstatus_param.cross_guildbattle_param, CrossGuildBattleParamHex, worldstatus_item_list);

	//////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(csa_boss_rank_data_node, tabledef::WORLDSTATUS_CSA_BOSS_RANK_DATA,
		worldstatus_param.csa_boss_rank_param, CombineServerActivityBossRankParamHex, worldstatus_item_list);

	//////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(cross_randactivity_data_node, tabledef::WORLDSTATUS_CROSS_RAND_ACTIVITY_DATA,
		worldstatus_param.cross_rand_activity_param, CrossRandActivityParamHex, worldstatus_item_list);

	//////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(cross_randactivity_big_data_node, tabledef::WORLDSTATUS_CROSS_RAND_ACTIVITY_BIG_DATA,
		worldstatus_param.cross_rand_activity_big_param, CrossRandActivityBigParamHex, worldstatus_item_list);

	UPDATE_BINARY_DATA_TO_LIST(cross_randactivity_big_data_node, tabledef::WORLDSTATUS_CROSS_RAND_ACTIVITY_BIG_DATA2,
		worldstatus_param.cross_rand_activity_big_param2, CrossRandActivityBigParam2Hex, worldstatus_item_list);

	UPDATE_BINARY_DATA_TO_LIST(cross_cloudpurchase_data_node, tabledef::WORLDSTATUS_CROSS_RAND_ACTIVITY_CLOUD_PURCHASE_DATA,
		worldstatus_param.cross_cloudpurchase_param, CrossCloudPurchaseParamHex, worldstatus_item_list);

	//////////////////////////////////////////////////////////////////////////
	CmpNodeList condition;

	CmpNode for_index_node(tabledef::WORLDSTATUS_FOR_INDEX);
	for_index_node.SetLong64(1);  // 仅有一条记录 
	condition.push_back(for_index_node);

	int count = worldstatus_command->Update(stmt, condition, worldstatus_item_list);
	if (count < 0)
	{
		ret = -1;
	}
	else if (0 == count)
	{
		DataAdapter worldstatus_item = worldstatus_table->GetPrototype();
		worldstatus_item.Malloc();

		worldstatus_item.m_data_area[tabledef::WORLDSTATUS_FOR_INDEX].vint64 = 1;
		worldstatus_item.m_data_area[tabledef::WORLDSTATUS_START_TIME].vint64 = worldstatus_param.start_time;
		worldstatus_item.m_data_area[tabledef::WORLDSTATUS_WORLD_TIME].vint = worldstatus_param.world_time;
		worldstatus_item.m_data_area[tabledef::WORLDSTATUS_LAST_UPDATE_DAYID].vint64 = worldstatus_param.last_update_dayid;
		worldstatus_item.m_data_area[tabledef::WORLDSTATUS_XIANMENGZHAN_MENGZHU_ID].vint = worldstatus_param.xianmengzhan_mengzhu_id;
		
		if (DBCommand::RESULT_SUC != worldstatus_command->Save(stmt, &worldstatus_item))
		{
			ret = -2;
		}

		worldstatus_item.Free();
	}

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__InitOfflineRegister(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(OfflineRegisterParam) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitOfflineRegister_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	long long id_from = 0;

	if (!in_param.Pop(&id_from))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitOfflineRegister_2");
		return rmi::DispatchParamError;
	}

	static OfflineRegisterParam offline_reg_param;
	offline_reg_param.count = 0;

	int result = InitOfflineRegister(id_from, &offline_reg_param);
	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitOfflineRegister_3 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!offline_reg_param.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitOfflineRegister_4 result:%d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else 
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitOfflineRegister_5 result:%d", result);
	}

	return rmi::DispatchOK;	
}

int RMIGlobalObject::InitOfflineRegister(long long id_from, OfflineRegisterParam *offline_reg_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *offline_reg_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_OFFLINEREGISTER);

	QueryCondition offline_reg_condition;
	CmpNode offline_reg_node1(tabledef::OFFLINEREGISTER_IDOFFLINEREGISTER);
	offline_reg_node1.SetLong64(id_from);
	offline_reg_node1.SetEqualType(EQUAL_TYPE_GRANTER);
	offline_reg_condition.cmp_list.push_back(offline_reg_node1);
	offline_reg_condition.query_num = OfflineRegisterParam::MAX_OFFLINE_REGISTER_COUNT;

	DataAdapterList offline_reg_list;
	int ret = offline_reg_command->Find(stmt, offline_reg_condition, &offline_reg_list);
	if (ret < 0)
	{
		offline_reg_param->count = 0;
		return ret;
	}

	ret = 0;

	int count = 0;
	DataAdapterList::iterator i = offline_reg_list.begin();

	for (; i != offline_reg_list.end() && count < OfflineRegisterParam::MAX_OFFLINE_REGISTER_COUNT; ++i)
	{
		offline_reg_param->offline_register_list[count].change_state = structcommon::CS_NONE;
		offline_reg_param->offline_register_list[count].id_offline_register = i->m_data_area[tabledef::OFFLINEREGISTER_IDOFFLINEREGISTER].vint64;

		offline_reg_param->offline_register_list[count].role_id = i->m_data_area[tabledef::OFFLINEREGISTER_ROLE_ID].vint;
		offline_reg_param->offline_register_list[count].increase_charm = i->m_data_area[tabledef::OFFLINEREGISTER_INCREASE_CHARM].vint;
		offline_reg_param->offline_register_list[count].last_divorce_time = i->m_data_area[tabledef::OFFLINEREGISTER_LAST_DIVORCE_TIME].vint;
		offline_reg_param->offline_register_list[count].chongzhi_cmd_flag = i->m_data_area[tabledef::OFFLINEREGISTER_CHONGZHI_CMD_FLAG].vint;
		offline_reg_param->offline_register_list[count].red_paper_consume_gold = i->m_data_area[tabledef::OFFLINEREGISTER_RED_PAPER_CONSUME_GOLD].vint;
		offline_reg_param->offline_register_list[count].red_paper_lucky_value = i->m_data_area[tabledef::OFFLINEREGISTER_RED_PAPER_LUCKY_VALUE].vint;
		offline_reg_param->offline_register_list[count].space_cai_count = i->m_data_area[tabledef::OFFLINEREGISTER_SPACE_CAI_CONUT].vint;
		offline_reg_param->offline_register_list[count].space_gift_count = i->m_data_area[tabledef::OFFLINEREGISTER_SPACE_GIFT_COUNT].vint;
		offline_reg_param->offline_register_list[count].qingyuan_bless_days = i->m_data_area[tabledef::OFFLINEREGISTER_QINGYUAN_BLESS_DAYS].vint;

		GET_BINARY_DATA((*i), tabledef::OFFLINEREGISTER_GUILD_BOX_INFO, offline_reg_param->offline_register_list[count].guild_box_info);
//		GET_BINARY_DATA((*i), tabledef::OFFLINEREGISTER_PROFESS_DATA, offline_reg_param->offline_register_list[count].profess_param);
		GET_BINARY_DATA((*i), tabledef::OFFLINEREGISTER_COMMON_DATA, offline_reg_param->offline_register_list[count].common_param);

		++count;
		i->Free();
	}

	offline_reg_param->count = count;

	for (;i != offline_reg_list.end(); ++i)
	{
		i->Free();
	}

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__SaveOfflineRegister(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveOfflineRegister_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static OfflineRegisterParam offline_reg_param;
	offline_reg_param.count = 0;

	if (!offline_reg_param.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveOfflineRegister_2");
		return rmi::DispatchParamError;
	}

	int result = SaveOfflineRegister(offline_reg_param);
	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveOfflineRegister_3 result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveOfflineRegister_4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::SaveOfflineRegister(const OfflineRegisterParam &offline_reg_param)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *offline_reg_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_OFFLINEREGISTER);
	DBCommand *offline_reg_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_OFFLINEREGISTER);

	conn->begin();

	for (int i = 0; i < offline_reg_param.count; ++i)
	{
		switch (offline_reg_param.offline_register_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_node;

				CmpNode for_index_node(tabledef::OFFLINEREGISTER_ROLE_ID);
				for_index_node.SetInt32(offline_reg_param.offline_register_list[i].role_id);
				cmp_node.push_back(for_index_node);

				int count = offline_reg_command->Remove(stmt, cmp_node);
				if (count < 1)
				{
					ret = -1;
				}
			}
			break; // case structcommon::CS_DELETE

		case structcommon::CS_INSERT:
			{
				DataAdapter offline_reg_item = offline_reg_table->GetPrototype();

				offline_reg_item.Malloc();

				offline_reg_item.m_data_area[tabledef::OFFLINEREGISTER_ROLE_ID].vint = offline_reg_param.offline_register_list[i].role_id;
				offline_reg_item.m_data_area[tabledef::OFFLINEREGISTER_INCREASE_CHARM].vint = offline_reg_param.offline_register_list[i].increase_charm;
				offline_reg_item.m_data_area[tabledef::OFFLINEREGISTER_LAST_DIVORCE_TIME].vint = offline_reg_param.offline_register_list[i].last_divorce_time;
				offline_reg_item.m_data_area[tabledef::OFFLINEREGISTER_CHONGZHI_CMD_FLAG].vint = offline_reg_param.offline_register_list[i].chongzhi_cmd_flag;
				offline_reg_item.m_data_area[tabledef::OFFLINEREGISTER_RED_PAPER_CONSUME_GOLD].vint = offline_reg_param.offline_register_list[i].red_paper_consume_gold;
				offline_reg_item.m_data_area[tabledef::OFFLINEREGISTER_RED_PAPER_LUCKY_VALUE].vint = offline_reg_param.offline_register_list[i].red_paper_lucky_value;
				offline_reg_item.m_data_area[tabledef::OFFLINEREGISTER_SPACE_CAI_CONUT].vint = offline_reg_param.offline_register_list[i].space_cai_count;
				offline_reg_item.m_data_area[tabledef::OFFLINEREGISTER_SPACE_GIFT_COUNT].vint = offline_reg_param.offline_register_list[i].space_gift_count;
				offline_reg_item.m_data_area[tabledef::OFFLINEREGISTER_QINGYUAN_BLESS_DAYS].vint = offline_reg_param.offline_register_list[i].qingyuan_bless_days;

				{
					static GuildBoxInfoHex hex_buff; memset(hex_buff, 0, sizeof(hex_buff));
					BinToHex((const char *)&offline_reg_param.offline_register_list[i].guild_box_info, sizeof(GuildBoxInfo), hex_buff);
					hex_buff[sizeof(hex_buff) - 1] = 0;
					STRNCPY(offline_reg_item.m_data_area[tabledef::OFFLINEREGISTER_GUILD_BOX_INFO].vcharp, hex_buff, offline_reg_item.m_data_area[tabledef::OFFLINEREGISTER_GUILD_BOX_INFO].length);
				}

// 				{
// 					static OfflineProfessParamHex hex_buff; memset(hex_buff, 0, sizeof(hex_buff));
// 					BinToHex((const char *)&offline_reg_param.offline_register_list[i].profess_param, sizeof(OfflineProfessParam), hex_buff);
// 					hex_buff[sizeof(hex_buff) - 1] = 0;
// 					STRNCPY(offline_reg_item.m_data_area[tabledef::OFFLINEREGISTER_PROFESS_DATA].vcharp, hex_buff, offline_reg_item.m_data_area[tabledef::OFFLINEREGISTER_PROFESS_DATA].length);
// 				}
				{
					static OfflineRegisterCommonParamHex hex_buff; memset(hex_buff, 0, sizeof(hex_buff));
					BinToHex((const char *)&offline_reg_param.offline_register_list[i].common_param, sizeof(OfflineRegisterCommonParam), hex_buff);
					hex_buff[sizeof(hex_buff) - 1] = 0;
					STRNCPY(offline_reg_item.m_data_area[tabledef::OFFLINEREGISTER_COMMON_DATA].vcharp, hex_buff, offline_reg_item.m_data_area[tabledef::OFFLINEREGISTER_COMMON_DATA].length);
				}

				if (DBCommand::RESULT_SUC != offline_reg_command->Save(stmt, &offline_reg_item))
				{
					ret = -2;
				}

				offline_reg_item.Free();
			}
			break; // case structcommon::CS_INSERT

		case structcommon::CS_UPDATE:	
			{		
				DataNodeList offline_reg_item_list;
				offline_reg_item_list.reserve(tabledef::TABLE_OFFLINEREGISTER_MAXINDEX);

				DataNode node_increase_charm(tabledef::OFFLINEREGISTER_INCREASE_CHARM);
				node_increase_charm.SetInt32(offline_reg_param.offline_register_list[i].increase_charm);
				offline_reg_item_list.push_back(node_increase_charm);

				DataNode node_last_divorce_time(tabledef::OFFLINEREGISTER_LAST_DIVORCE_TIME);
				node_last_divorce_time.SetInt32(offline_reg_param.offline_register_list[i].last_divorce_time);
				offline_reg_item_list.push_back(node_last_divorce_time);

				DataNode node_chongzhi_cmd_flag(tabledef::OFFLINEREGISTER_CHONGZHI_CMD_FLAG);
				node_chongzhi_cmd_flag.SetInt32(offline_reg_param.offline_register_list[i].chongzhi_cmd_flag);
				offline_reg_item_list.push_back(node_chongzhi_cmd_flag);

				DataNode node_red_paper_consume_gold(tabledef::OFFLINEREGISTER_RED_PAPER_CONSUME_GOLD);
				node_red_paper_consume_gold.SetInt32(offline_reg_param.offline_register_list[i].red_paper_consume_gold);
				offline_reg_item_list.push_back(node_red_paper_consume_gold);

				DataNode node_red_paper_lucky_value(tabledef::OFFLINEREGISTER_RED_PAPER_LUCKY_VALUE);
				node_red_paper_lucky_value.SetInt32(offline_reg_param.offline_register_list[i].red_paper_lucky_value);
				offline_reg_item_list.push_back(node_red_paper_lucky_value);

				{
					DataNode node_space_cai_count(tabledef::OFFLINEREGISTER_SPACE_CAI_CONUT);
					node_space_cai_count.SetInt32(offline_reg_param.offline_register_list[i].space_cai_count);
					offline_reg_item_list.push_back(node_space_cai_count);
				}

				{
					DataNode node_space_gift_count(tabledef::OFFLINEREGISTER_SPACE_GIFT_COUNT);
					node_space_gift_count.SetInt32(offline_reg_param.offline_register_list[i].space_gift_count);
					offline_reg_item_list.push_back(node_space_gift_count);
				}

				{
					DataNode node_qingyuan_bless_days(tabledef::OFFLINEREGISTER_QINGYUAN_BLESS_DAYS);
					node_qingyuan_bless_days.SetInt32(offline_reg_param.offline_register_list[i].qingyuan_bless_days);
					offline_reg_item_list.push_back(node_qingyuan_bless_days);
				}

				UPDATE_BINARY_DATA_TO_LIST(node, tabledef::OFFLINEREGISTER_GUILD_BOX_INFO, offline_reg_param.offline_register_list[i].guild_box_info, GuildBoxInfoHex, offline_reg_item_list);
//				UPDATE_BINARY_DATA_TO_LIST(node, tabledef::OFFLINEREGISTER_PROFESS_DATA, offline_reg_param.offline_register_list[i].profess_param, OfflineProfessParamHex, offline_reg_item_list);
				UPDATE_BINARY_DATA_TO_LIST(node, tabledef::OFFLINEREGISTER_COMMON_DATA, offline_reg_param.offline_register_list[i].common_param, OfflineRegisterCommonParamHex, offline_reg_item_list);

				CmpNodeList cmp_node;
				CmpNode for_index_node(tabledef::OFFLINEREGISTER_ROLE_ID);
				for_index_node.SetInt32(offline_reg_param.offline_register_list[i].role_id);
				cmp_node.push_back(for_index_node);

				int count = offline_reg_command->Update(stmt, cmp_node, offline_reg_item_list);
				if (count < 0)
				{
					ret = -3; 
				}
			}
			break; // case structcommon::CS_UPDATE

		} // switch end

	} // for end

	conn->commit();

	return ret < 0 ? ret : 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int RMIGlobalObject::__InitSpaceRecord(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(SpaceParam) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitSpaceRecord");
		return rmi::DispatchOutParamBuffTooShort;
	}

	long long id_from = 0;

	if (!in_param.Pop(&id_from))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitSpaceRecord_2");
		return rmi::DispatchParamError;
	}

	static SpaceParam space_param;
	space_param.count = 0;

	int result = InitSpaceRecord(id_from, &space_param);
	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitSpaceRecord_3 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!space_param.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitSpaceRecord_4 result:%d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else 
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitSpaceRecord_5 result:%d", result);
	}

	return rmi::DispatchOK;	
}

int RMIGlobalObject::InitSpaceRecord(long long id_from, SpaceParam *space_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *space_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_SPACERECORD);

	QueryCondition space_condition;
	CmpNode space_node1(tabledef::SPACERECORD_ID_RECORD);
	space_node1.SetLong64(id_from);
	space_node1.SetEqualType(EQUAL_TYPE_GRANTER);
	space_condition.cmp_list.push_back(space_node1);

	CmpNode owner_role_id(tabledef::SPACERECORD_OWNER_ID);
	owner_role_id.SetInt32(0);
	owner_role_id.SetEqualType(EQUAL_TYPE_GRANTER);
	space_condition.cmp_list.push_back(owner_role_id);

	space_condition.query_num = SpaceParam::SPACE_MAX_RECORD_COUNT;

	DataAdapterList space_record_list;
	int ret = space_command->Find(stmt, space_condition, &space_record_list);
	if (ret < 0)
	{
		space_param->count = 0;
		return ret;
	}

	ret = 0;

	int count = 0;
	DataAdapterList::iterator i = space_record_list.begin();

	for (; i != space_record_list.end() && count < SpaceParam::SPACE_MAX_RECORD_COUNT; ++i)
	{
		space_param->record_list[count].id_record = i->m_data_area[tabledef::SPACERECORD_ID_RECORD].vint64;
		space_param->record_list[count].change_state = structcommon::CS_NONE;

		space_param->record_list[count].record_unq_id = i->m_data_area[tabledef::SPACERECORD_RECORD_UNQ_ID].vint64;
		space_param->record_list[count].owner_uid = i->m_data_area[tabledef::SPACERECORD_OWNER_ID].vint;
		space_param->record_list[count].from_uid = i->m_data_area[tabledef::SPACERECORD_FROM_ROLE_ID].vint;
		space_param->record_list[count].to_uid = i->m_data_area[tabledef::SPACERECORD_TO_ROLE_ID].vint;
		space_param->record_list[count].happen_timestamp = static_cast<unsigned int>(i->m_data_area[tabledef::SPACERECORD_HAPPEN_TIMESTAMP].vint64);
		space_param->record_list[count].happen_space_uid = static_cast<unsigned int>(i->m_data_area[tabledef::SPACERECORD_HAPPEN_SPACE_UID].vint);
		space_param->record_list[count].type = (short)i->m_data_area[tabledef::SPACERECORD_TYPE].vint;
		space_param->record_list[count].is_huifu = (short)i->m_data_area[tabledef::SPACERECORD_IS_HUIFU].vint;
		STRNCPY(space_param->record_list[count].msg, i->m_data_area[tabledef::SPACERECORD_MSG].vcharp, sizeof(space_param->record_list[0].msg));
	
		++count;
		i->Free();
	}

	space_param->count = count;

	for (;i != space_record_list.end(); ++i)
	{
		i->Free();
	}

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__SaveSpaceRecord(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveSpaceRecord_1");
		return rmi::DispatchOutParamBuffTooShort; 
	}

	static SpaceParam space_param;
	space_param.count = 0;

	if (!space_param.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveSpaceRecord_2");
		return rmi::DispatchParamError;
	}

	int result = SaveSpaceRecord(space_param);
	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveSpaceRecord_3 result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveSpaceRecord_4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::SaveSpaceRecord(const SpaceParam &space_param)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *space_record_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_SPACERECORD);
	DBCommand *space_record_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_SPACERECORD);

	conn->begin();

	for (int i = 0; i < space_param.count && i < SpaceParam::SPACE_MAX_RECORD_COUNT; ++i)
	{
		switch (space_param.record_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_node;

				CmpNode unq_id_node(tabledef::SPACERECORD_RECORD_UNQ_ID);
				unq_id_node.SetLong64(space_param.record_list[i].record_unq_id);
				cmp_node.push_back(unq_id_node);

				int count = space_record_command->Remove(stmt, cmp_node);
				if (count < 1)
				{
					ret = -1;
				}
			}
			break; // case structcommon::CS_DELETE

		case structcommon::CS_INSERT:
			{
				DataAdapter record_item = space_record_table->GetPrototype();

				record_item.Malloc();

				record_item.m_data_area[tabledef::SPACERECORD_RECORD_UNQ_ID].vint64 = space_param.record_list[i].record_unq_id;
				record_item.m_data_area[tabledef::SPACERECORD_OWNER_ID].vint = space_param.record_list[i].owner_uid;
				record_item.m_data_area[tabledef::SPACERECORD_FROM_ROLE_ID].vint = space_param.record_list[i].from_uid;
				record_item.m_data_area[tabledef::SPACERECORD_TO_ROLE_ID].vint = space_param.record_list[i].to_uid;
				record_item.m_data_area[tabledef::SPACERECORD_HAPPEN_TIMESTAMP].vint64 = space_param.record_list[i].happen_timestamp;
				record_item.m_data_area[tabledef::SPACERECORD_HAPPEN_SPACE_UID].vint = space_param.record_list[i].happen_space_uid;
				record_item.m_data_area[tabledef::SPACERECORD_TYPE].vint = space_param.record_list[i].type;
				record_item.m_data_area[tabledef::SPACERECORD_IS_HUIFU].vint = space_param.record_list[i].is_huifu;
				STRNCPY(record_item.m_data_area[tabledef::SPACERECORD_MSG].vcharp, space_param.record_list[i].msg, record_item.m_data_area[tabledef::SPACERECORD_MSG].length);

				if (DBCommand::RESULT_SUC != space_record_command->Save(stmt, &record_item))
				{
					ret = -2;
				}

				record_item.Free();
			}
			break; // case structcommon::CS_INSERT

		case structcommon::CS_UPDATE:	
			{		
				DataNodeList node_list;
				node_list.reserve(tabledef::TABLE_SPACERECORD_MAXINDEX);

				UPDATE_INT64_DATA_NODE(record_unq_id, tabledef::SPACERECORD_RECORD_UNQ_ID, space_param.record_list[i].record_unq_id);
				UPDATE_INT_DATA_NODE(owner_uid, tabledef::SPACERECORD_OWNER_ID, space_param.record_list[i].owner_uid);
				UPDATE_INT_DATA_NODE(from_uid, tabledef::SPACERECORD_FROM_ROLE_ID, space_param.record_list[i].from_uid);
				UPDATE_INT_DATA_NODE(to_uid, tabledef::SPACERECORD_TO_ROLE_ID, space_param.record_list[i].to_uid);
				UPDATE_INT64_DATA_NODE(happen_timestamp, tabledef::SPACERECORD_HAPPEN_TIMESTAMP, space_param.record_list[i].happen_timestamp);
				UPDATE_INT_DATA_NODE(happen_space_uid, tabledef::SPACERECORD_HAPPEN_SPACE_UID, space_param.record_list[i].happen_space_uid);
				UPDATE_INT_DATA_NODE(type, tabledef::SPACERECORD_TYPE, space_param.record_list[i].type);
				UPDATE_INT_DATA_NODE(is_huifu, tabledef::SPACERECORD_IS_HUIFU, space_param.record_list[i].is_huifu);
				UPDATE_STRING_DATA_NODE(msg, tabledef::SPACERECORD_MSG, space_param.record_list[i].msg);

				CmpNodeList cmp_node;
				CmpNode unq_id_node(tabledef::SPACERECORD_RECORD_UNQ_ID);
				unq_id_node.SetLong64(space_param.record_list[i].record_unq_id);
				cmp_node.push_back(unq_id_node);

				int count = space_record_command->Update(stmt, cmp_node, node_list);
				if (count < 0)
				{
					ret = -3; 
				}
			}
			break; // case structcommon::CS_UPDATE

		} // switch end

	} // for end

	conn->commit();

	return ret < 0 ? ret : 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool RMIGlobalObject::ItemParamInit(ItemParamDataStruct *param, const char *vcharp, int length)
{
	if (NULL == param || NULL == vcharp || length <= 0 || length > int(sizeof(ItemParamDataStruct) * 2)) return false;

	param->Reset();
	HexToBin(vcharp, length, (char *)param);

	return true;
}

void RMIGlobalObject::ItemParamUpdate(DataNode *data_node, bool has_param, const ItemParamDataStruct *param)
{
	if (NULL == data_node || (has_param && NULL == param)) return;

	if (has_param)
	{
		static ItemParamDataStructHex hex_buff;

		BinToHex((const char *)param, sizeof(ItemParamDataStruct), hex_buff); 
		hex_buff[sizeof(hex_buff) - 1] = 0;

		data_node->SetString(hex_buff);
	}
	else
	{
		data_node->SetString("");
	}
}

void RMIGlobalObject::ItemParamInsert(char *vcharp, int length, const ItemParamDataStruct *param)
{
	if (NULL == vcharp || length <= 0 || NULL == param) return;

	static ItemParamDataStructHex hex_buff;

	BinToHex((const char *)param, sizeof(ItemParamDataStruct), hex_buff);
	hex_buff[sizeof(hex_buff) - 1] = 0;

	STRNCPY(vcharp, hex_buff, length);
}

int RMIGlobalObject::__InitChallengeUser(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(ChallengeUserListParam) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitChallengeUser_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	int rank_from = 0;
	if (!in_param.Pop(&rank_from))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitChallengeUser_2");
		return rmi::DispatchParamError;
	}

	static ChallengeUserListParam challenge_user_param;
	challenge_user_param.count = 0;

	int result = InitChallengeUser(rank_from, &challenge_user_param);
	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitChallengeUser_3 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!challenge_user_param.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitChallengeUser_4 result:%d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else 
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitChallengeUser_5 result:%d", result);
	}

	return rmi::DispatchOK;	
}

int RMIGlobalObject::InitChallengeUser(int rank_from, ChallengeUserListParam *challenge_user_p)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *challenge_user_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_CHALLENGERUSER);

	QueryCondition condition;
	CmpNode node_rankpos(tabledef::CHALLENGERUSER_RANKPOS);
	node_rankpos.SetInt32(rank_from);
	node_rankpos.SetEqualType(EQUAL_TYPE_GRANTER);
	condition.cmp_list.push_back(node_rankpos);

	CmpNode node_role_id(tabledef::CHALLENGERUSER_ROLE_ID);
	node_role_id.SetLong64(0);
	node_role_id.SetEqualType(EQUAL_TYPE_GRANTER);
	condition.cmp_list.push_back(node_role_id);
	condition.query_num = CHALLENGE_FIELD_ONCE_OPER_USER_COUNT;

	DataAdapterList user_list;
	int ret = challenge_user_command->Find(stmt, condition, &user_list);
	if (ret < 0)
	{
		challenge_user_p->count = 0;
		return ret;
	}

	ret = 0;

	int count = 0;
	DataAdapterList::iterator i = user_list.begin();

	for (; i != user_list.end() && count < CHALLENGE_FIELD_ONCE_OPER_USER_COUNT; ++i)
	{
		challenge_user_p->user_list[count].change_state = structcommon::CS_NONE;
		challenge_user_p->user_list[count].rankpos = i->m_data_area[tabledef::CHALLENGERUSER_RANKPOS].vint;
		challenge_user_p->user_list[count].role_id = i->m_data_area[tabledef::CHALLENGERUSER_ROLE_ID].vint64;

		{
			memset((void*)&challenge_user_p->user_list[count].challenge_p, 0, sizeof(challenge_user_p->user_list[count].challenge_p));

			unsigned int length = i->m_data_area[tabledef::CHALLENGERUSER_CHALLENGE_USER_DATA].length;
			if (length > sizeof(challenge_user_p->user_list[count].challenge_p) * 2)
			{
				dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::InitChallengeUser error: length > sizeof(ChallengeParam) * 2");
				i->Free();
				continue;
			}

			HexToBin(i->m_data_area[tabledef::CHALLENGERUSER_CHALLENGE_USER_DATA].vcharp, length, (char*)&challenge_user_p->user_list[count].challenge_p);
		}

		{
			memset((void*)&challenge_user_p->user_list[count].common_data, 0, sizeof(challenge_user_p->user_list[count].common_data));

			unsigned int length = i->m_data_area[tabledef::CHALLENGERUSER_COMMON_DATA].length;
			if (length > sizeof(challenge_user_p->user_list[count].common_data) * 2)
			{
				dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::InitChallengeUser error: length > sizeof(ChallengeParam) * 2");
				i->Free();
				continue;
			}

			HexToBin(i->m_data_area[tabledef::CHALLENGERUSER_COMMON_DATA].vcharp, length, (char*)&challenge_user_p->user_list[count].common_data);
		}

		{
			memset((void*)&challenge_user_p->user_list[count].report_info, 0, sizeof(challenge_user_p->user_list[count].report_info));

			unsigned int length = i->m_data_area[tabledef::CHALLENGERUSER_REPORT_INFO_DATA].length;
			if (length > sizeof(challenge_user_p->user_list[count].report_info) * 2)
			{
				dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::InitChallengeUser error: length > sizeof(ChallengeParam) * 2");
				i->Free();
				continue;
			}

			HexToBin(i->m_data_area[tabledef::CHALLENGERUSER_REPORT_INFO_DATA].vcharp, length, (char*)&challenge_user_p->user_list[count].report_info);
		}

		{
			memset((void*)&challenge_user_p->user_list[count].role_appearance, 0, sizeof(challenge_user_p->user_list[count].role_appearance));

			unsigned int length = i->m_data_area[tabledef::CHALLENGERUSER_ROLE_APPEARANCE_DATA].length;
			if (length > sizeof(challenge_user_p->user_list[count].role_appearance) * 2)
			{
				dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::InitChallengeUser error: length > sizeof(RoleAppearance) * 2");
				i->Free();
				continue;
			}

			HexToBin(i->m_data_area[tabledef::CHALLENGERUSER_ROLE_APPEARANCE_DATA].vcharp, length, (char*)&challenge_user_p->user_list[count].role_appearance);
		}

		++count;
		i->Free();
	}

	challenge_user_p->count = count;

	for (;i != user_list.end(); ++i)
	{
		i->Free();
	}

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__SaveChallengeUser(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveChallengeUser_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static ChallengeUserListParam challenge_user_p;
	challenge_user_p.count = 0;

	if (!challenge_user_p.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveChallengeUser_2");
		return rmi::DispatchParamError;
	}

	int result = SaveChallengeUser(challenge_user_p);
	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveChallengeUser_3 result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveChallengeUser_4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::SaveChallengeUser(const ChallengeUserListParam &challenge_user_p)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *challenge_user_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_CHALLENGERUSER);
	DBCommand *challenge_user_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_CHALLENGERUSER);

	conn->begin();

	for (int i = 0; i < challenge_user_p.count && i < CHALLENGE_FIELD_ONCE_OPER_USER_COUNT; ++i)
	{
		switch (challenge_user_p.user_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_node;

				CmpNode rankpos_node(tabledef::CHALLENGERUSER_RANKPOS);
				rankpos_node.SetInt32(challenge_user_p.user_list[i].rankpos);
				cmp_node.push_back(rankpos_node);

				int count = challenge_user_command->Remove(stmt, cmp_node);
				if (count < 1)
				{
					ret = -1;
				}
			}
			break; // case structcommon::CS_DELETE

		case structcommon::CS_INSERT:
			{
				DataAdapter item_data = challenge_user_table->GetPrototype();

				item_data.Malloc();

				item_data.m_data_area[tabledef::CHALLENGERUSER_RANKPOS].vint = challenge_user_p.user_list[i].rankpos;
				item_data.m_data_area[tabledef::CHALLENGERUSER_ROLE_ID].vint64 = challenge_user_p.user_list[i].role_id;
				
				{
					static ChallengeParamHex challenge_param_hex;
					BinToHex((const char*)&challenge_user_p.user_list[i].challenge_p, sizeof(challenge_user_p.user_list[i].challenge_p), challenge_param_hex);
					challenge_param_hex[sizeof(challenge_param_hex) - 1] = '\0';

					STRNCPY(item_data.m_data_area[tabledef::CHALLENGERUSER_CHALLENGE_USER_DATA].vcharp, (const char*)&challenge_param_hex, item_data.m_data_area[tabledef::CHALLENGERUSER_CHALLENGE_USER_DATA].length);
				}

				{
					static ChallengeUserCommonDataHex common_data_param_hex;
					BinToHex((const char*)&challenge_user_p.user_list[i].common_data, sizeof(challenge_user_p.user_list[i].common_data), common_data_param_hex);
					common_data_param_hex[sizeof(common_data_param_hex) - 1] = '\0';

					STRNCPY(item_data.m_data_area[tabledef::CHALLENGERUSER_COMMON_DATA].vcharp, (const char*)&common_data_param_hex, item_data.m_data_area[tabledef::CHALLENGERUSER_COMMON_DATA].length);
				}

				{
					static ChallengeUserReportHex report_info_hex;
					BinToHex((const char*)&challenge_user_p.user_list[i].report_info, sizeof(challenge_user_p.user_list[i].report_info), report_info_hex);
					report_info_hex[sizeof(report_info_hex) - 1] = '\0';

					STRNCPY(item_data.m_data_area[tabledef::CHALLENGERUSER_REPORT_INFO_DATA].vcharp, (const char*)&report_info_hex, item_data.m_data_area[tabledef::CHALLENGERUSER_REPORT_INFO_DATA].length);
				}

				{
					static RoleAppearanceHex role_appearance_hex;
					BinToHex((const char*)&challenge_user_p.user_list[i].role_appearance, sizeof(challenge_user_p.user_list[i].role_appearance), role_appearance_hex);
					role_appearance_hex[sizeof(role_appearance_hex) - 1] = '\0';

					STRNCPY(item_data.m_data_area[tabledef::CHALLENGERUSER_ROLE_APPEARANCE_DATA].vcharp, (const char*)&role_appearance_hex, item_data.m_data_area[tabledef::CHALLENGERUSER_ROLE_APPEARANCE_DATA].length);
				}

				if (DBCommand::RESULT_SUC != challenge_user_command->Save(stmt, &item_data))
				{
					ret = -2;
				}

				item_data.Free();
			}
			break; // case structcommon::CS_INSERT

		case structcommon::CS_UPDATE:	
			{		
				DataNodeList node_list;
				node_list.reserve(tabledef::TABLE_CHALLENGERUSER_MAXINDEX);

				DataNode rankpos(tabledef::CHALLENGERUSER_RANKPOS);
				rankpos.SetInt32(challenge_user_p.user_list[i].rankpos);
				node_list.push_back(rankpos);

				DataNode roleid(tabledef::CHALLENGERUSER_ROLE_ID);
				roleid.SetLong64(challenge_user_p.user_list[i].role_id);
				node_list.push_back(roleid);

				UPDATE_BINARY_DATA_TO_LIST(challenge_param_node, tabledef::CHALLENGERUSER_CHALLENGE_USER_DATA, challenge_user_p.user_list[i].challenge_p, ChallengeParamHex, node_list);
				UPDATE_BINARY_DATA_TO_LIST(common_data_node, tabledef::CHALLENGERUSER_COMMON_DATA, challenge_user_p.user_list[i].common_data, ChallengeUserCommonDataHex, node_list);
				UPDATE_BINARY_DATA_TO_LIST(report_info_node, tabledef::CHALLENGERUSER_REPORT_INFO_DATA, challenge_user_p.user_list[i].report_info, ChallengeUserReportHex, node_list);
				UPDATE_BINARY_DATA_TO_LIST(role_appearance_node, tabledef::CHALLENGERUSER_ROLE_APPEARANCE_DATA, challenge_user_p.user_list[i].role_appearance, RoleAppearanceHex, node_list);

				CmpNodeList cmp_node_list;
				CmpNode cmp_node_rankpos(tabledef::CHALLENGERUSER_RANKPOS);
				cmp_node_rankpos.SetInt32(challenge_user_p.user_list[i].rankpos);
				cmp_node_list.push_back(cmp_node_rankpos);

				CmpNode cmp_node_role_id(tabledef::CHALLENGERUSER_ROLE_ID);
				cmp_node_role_id.SetLong64(0);
				cmp_node_role_id.SetEqualType(EQUAL_TYPE_GRANTER);
				cmp_node_list.push_back(cmp_node_role_id);

				int count = challenge_user_command->Update(stmt, cmp_node_list, node_list);
				if (count < 0)
				{
					ret = -3; 
				}
			}
			break; // case structcommon::CS_UPDATE

		} // switch end

	} // for end

	conn->commit();

	return ret < 0 ? ret : 0;
}


int RMIGlobalObject::__InitFishPool(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(FishPoolListParam) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitFishPool 1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	long long id_from = 0;
	if (!in_param.Pop(&id_from))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitFishPool 2");
		return rmi::DispatchParamError;
	}

	static FishPoolListParam fish_pool_list_param;
	fish_pool_list_param.count = 0;

	int result = InitFishPool(id_from, &fish_pool_list_param);
	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitFishPool 3 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!fish_pool_list_param.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitFishPool 4 result:%d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else 
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitFishPool 5 result:%d", result);
	}

	return rmi::DispatchOK;	
}

int RMIGlobalObject::InitFishPool(long long id_from, FishPoolListParam *fish_pool_list_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *fish_pool_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_FISHPOOL);

	QueryCondition condition;
	CmpNode node_fish_pool_id(tabledef::FISHPOOL_IDFISHPOOL);
	node_fish_pool_id.SetLong64(id_from);
	node_fish_pool_id.SetEqualType(EQUAL_TYPE_GRANTER);
	condition.cmp_list.push_back(node_fish_pool_id);

	CmpNode node_fish_role_id(tabledef::FISHPOOL_FISH_ROLE_ID);
	node_fish_role_id.SetLong64(0);
	node_fish_role_id.SetEqualType(EQUAL_TYPE_GRANTER);
	condition.cmp_list.push_back(node_fish_role_id);
	condition.query_num = FISH_POOL_ONCE_LOAD_MAX_COUNT;

	DataAdapterList fish_pool_list;
	int ret = fish_pool_command->Find(stmt, condition, &fish_pool_list);
	if (ret < 0)
	{
		fish_pool_list_param->count = 0;
		return ret;
	}

	ret = 0;

	int count = 0;
	DataAdapterList::iterator i = fish_pool_list.begin();

	for (; i != fish_pool_list.end() && count < FISH_POOL_ONCE_LOAD_MAX_COUNT; ++i)
	{
		FishPoolListParam::FishPoolAttr &fish_pool_attr = fish_pool_list_param->fish_pool_list[count];

		fish_pool_attr.id_fishpool = i->m_data_area[tabledef::FISHPOOL_IDFISHPOOL].vint64;
		fish_pool_attr.change_state = structcommon::CS_NONE;
		fish_pool_attr.role_id = i->m_data_area[tabledef::FISHPOOL_FISH_ROLE_ID].vint;

		{
			memset((void*)&fish_pool_attr.common_data, 0, sizeof(fish_pool_attr.common_data));

			unsigned int length = i->m_data_area[tabledef::FISHPOOL_COMMON_DATA].length;
			if (length > sizeof(fish_pool_attr.common_data) * 2)
			{
				dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::InitFishPool error: 1");
				i->Free();
				continue;
			}

			HexToBin(i->m_data_area[tabledef::FISHPOOL_COMMON_DATA].vcharp, length, (char*)&fish_pool_attr.common_data);
		}

		++count;
		i->Free();
	}

	fish_pool_list_param->count = count;

	for (;i != fish_pool_list.end(); ++i)
	{
		i->Free();
	}

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__SaveFishPool(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveFishPool 1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static FishPoolListParam fish_pool_list_param;
	fish_pool_list_param.count = 0;

	if (!fish_pool_list_param.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveFishPool 2");
		return rmi::DispatchParamError;
	}

	int result = SaveFishPool(fish_pool_list_param);
	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveFishPool 3  result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveFishPool 4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;	
}

int RMIGlobalObject::SaveFishPool(const FishPoolListParam &fish_pool_list_param)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *fish_pool_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_FISHPOOL);
	DBCommand *fish_pool_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_FISHPOOL);

	conn->begin();

	for (int i = 0; i < fish_pool_list_param.count && i < FISH_POOL_ONCE_LOAD_MAX_COUNT; ++i)
	{
		const FishPoolListParam::FishPoolAttr &fish_pool_attr = fish_pool_list_param.fish_pool_list[i];

		switch (fish_pool_attr.change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_node;

				CmpNode fish_role_id_node(tabledef::FISHPOOL_FISH_ROLE_ID);
				fish_role_id_node.SetInt32(fish_pool_attr.role_id);
				cmp_node.push_back(fish_role_id_node);

				int count = fish_pool_command->Remove(stmt, cmp_node);
				if (count < 1)
				{
					ret = -1;
				}
			}
			break; // case structcommon::CS_DELETE

		case structcommon::CS_INSERT:
			{
				DataAdapter item_data = fish_pool_table->GetPrototype();

				item_data.Malloc();

				item_data.m_data_area[tabledef::FISHPOOL_FISH_ROLE_ID].vint = fish_pool_attr.role_id;

				{
					static FishPoolCommonDataParamHex tmp_param_hex;
					BinToHex((const char*)&fish_pool_attr.common_data, sizeof(fish_pool_attr.common_data), tmp_param_hex);
					tmp_param_hex[sizeof(tmp_param_hex) - 1] = '\0';

					STRNCPY(item_data.m_data_area[tabledef::FISHPOOL_COMMON_DATA].vcharp, (const char*)&tmp_param_hex, item_data.m_data_area[tabledef::FISHPOOL_COMMON_DATA].length);
				}

				if (DBCommand::RESULT_SUC != fish_pool_command->Save(stmt, &item_data))
				{
					ret = -2;
				}

				item_data.Free();
			}
			break; // case structcommon::CS_INSERT

		case structcommon::CS_UPDATE:	
			{		
				DataNodeList node_list;
				node_list.reserve(tabledef::TABLE_FISHPOOL_MAXINDEX);

				DataNode fish_role_id_node(tabledef::FISHPOOL_FISH_ROLE_ID);
				fish_role_id_node.SetInt32(fish_pool_attr.role_id);
				node_list.push_back(fish_role_id_node);

				UPDATE_BINARY_DATA_TO_LIST(common_data_node, tabledef::FISHPOOL_COMMON_DATA, fish_pool_attr.common_data, FishPoolCommonDataParamHex, node_list);

				CmpNodeList cmp_node_list;
				CmpNode cmp_node_id(tabledef::FISHPOOL_FISH_ROLE_ID);
				cmp_node_id.SetInt32(fish_pool_attr.role_id);
				cmp_node_list.push_back(cmp_node_id);

				int count = fish_pool_command->Update(stmt, cmp_node_list, node_list);
				if (count < 0)
				{
					ret = -3; 
				}
			}
			break; // case structcommon::CS_UPDATE

		} // switch end

	} // for end

	conn->commit();

	return ret < 0 ? ret : 0;
}


int RMIGlobalObject::__InitCrossRecord(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(CrossRecordListParam) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitCrossRecord_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	long long id_from = 0;
	if (!in_param.Pop(&id_from))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitCrossRecord_2");
		return rmi::DispatchParamError;
	}

	static CrossRecordListParam crossrecord_param;
	crossrecord_param.count = 0;

	int result = InitCrossRecord(id_from, &crossrecord_param);
	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitCrossRecord_3 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!crossrecord_param.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitCrossRecord_4 result:%d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else 
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitCrossRecord_5 result:%d", result);
	}

	return rmi::DispatchOK;	
}

int RMIGlobalObject::InitCrossRecord(long long id_from, CrossRecordListParam *crossrecord_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *crossrecord_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_CROSSRECORD);

	QueryCondition condition;
	CmpNode nod_idcrossrecord(tabledef::CROSSRECORD_IDCROSSRECORD);
	nod_idcrossrecord.SetLong64(id_from);
	nod_idcrossrecord.SetEqualType(EQUAL_TYPE_GRANTER);
	condition.cmp_list.push_back(nod_idcrossrecord);

	CmpNode node_role_id(tabledef::CROSSRECORD_ROLE_ID);
	node_role_id.SetInt32(0);
	node_role_id.SetEqualType(EQUAL_TYPE_GRANTER);
	condition.cmp_list.push_back(node_role_id);
	condition.query_num = CrossRecordListParam::CROSS_RECORD_MAX_COUNT;

	DataAdapterList crossrecord_list;
	int ret = crossrecord_command->Find(stmt, condition, &crossrecord_list);
	if (ret < 0)
	{
		crossrecord_param->count = 0;
		return ret;
	}

	ret = 0;

	int count = 0;
	DataAdapterList::iterator i = crossrecord_list.begin();

	for (; i != crossrecord_list.end() && count < CrossRecordListParam::CROSS_RECORD_MAX_COUNT; ++i)
	{
		crossrecord_param->cross_record_list[count].change_state = structcommon::CS_NONE;
		crossrecord_param->cross_record_list[count].id_cross_record = i->m_data_area[tabledef::CROSSRECORD_IDCROSSRECORD].vint64;
		crossrecord_param->cross_record_list[count].role_id = i->m_data_area[tabledef::CROSSRECORD_ROLE_ID].vint;
		STRNCPY(crossrecord_param->cross_record_list[count].role_name, i->m_data_area[tabledef::CROSSRECORD_ROLE_NAME].vcharp, sizeof(crossrecord_param->cross_record_list[0].role_name));
		crossrecord_param->cross_record_list[count].level = static_cast<short>(i->m_data_area[tabledef::CROSSRECORD_LEVEL].vint);
		crossrecord_param->cross_record_list[count].sex = i->m_data_area[tabledef::CROSSRECORD_SEX].vchar;
		crossrecord_param->cross_record_list[count].prof = i->m_data_area[tabledef::CROSSRECORD_PROF].vchar;
		crossrecord_param->cross_record_list[count].camp = i->m_data_area[tabledef::CROSSRECORD_CAMP].vchar;

		{
			crossrecord_param->cross_record_list[count].user_record_data.Reset();

			int length = i->m_data_area[tabledef::CROSSRECORD_RECORD_DATA].length;
			if (length > (int)sizeof(UserCrossRecordData) * 2)
			{
				i->Free();
				continue;
			}

			HexToBin(i->m_data_area[tabledef::CROSSRECORD_RECORD_DATA].vcharp, length, (char *)&crossrecord_param->cross_record_list[count].user_record_data);
		}

		{
			crossrecord_param->cross_record_list[count].user_appearance_data.Reset();

			int length = i->m_data_area[tabledef::CROSSRECORD_APPEARANCE_DATA].length;
			if (length > (int)sizeof(RoleAppearance) * 2)
			{
				i->Free();
				continue;
			}

			HexToBin(i->m_data_area[tabledef::CROSSRECORD_APPEARANCE_DATA].vcharp, length, (char *)&crossrecord_param->cross_record_list[count].user_appearance_data);
		}

		++count;
		i->Free();
	}

	crossrecord_param->count = count;

	for (;i != crossrecord_list.end(); ++i)
	{
		i->Free();
	}

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__SaveCrossRecord(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveCrossRecord_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static CrossRecordListParam crossrecord_param;
	crossrecord_param.count = 0;

	if (!crossrecord_param.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveCrossRecord_2");
		return rmi::DispatchParamError;
	}

	int result = SaveCrossRecord(crossrecord_param);
	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveCrossRecord_3 result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveCrossRecord_4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::SaveCrossRecord(const CrossRecordListParam &crossrecord_param)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *crossrecord_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_CROSSRECORD);
	DBCommand *crossrecord_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_CROSSRECORD);

	conn->begin();

	for (int i = 0; i < crossrecord_param.count && i < CrossRecordListParam::CROSS_RECORD_MAX_COUNT; ++i)
	{
		switch (crossrecord_param.cross_record_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_node_list;

				CmpNode cmp_node_index(tabledef::CROSSRECORD_ROLE_ID);
				cmp_node_index.SetInt32(crossrecord_param.cross_record_list[i].role_id);
				cmp_node_list.push_back(cmp_node_index);

				int count = crossrecord_command->Remove(stmt, cmp_node_list);
				if (count < 1)
				{
					ret = -1;
				}
			}
			break; // case structcommon::CS_DELETE

		case structcommon::CS_INSERT:
			{
				DataAdapter item_data = crossrecord_table->GetPrototype();

				item_data.Malloc();

				item_data.m_data_area[tabledef::CROSSRECORD_ROLE_ID].vint = crossrecord_param.cross_record_list[i].role_id;
				STRNCPY(item_data.m_data_area[tabledef::CROSSRECORD_ROLE_NAME].vcharp, crossrecord_param.cross_record_list[i].role_name, item_data.m_data_area[tabledef::CROSSRECORD_ROLE_NAME].length);
				item_data.m_data_area[tabledef::CROSSRECORD_LEVEL].vint = crossrecord_param.cross_record_list[i].level;
				item_data.m_data_area[tabledef::CROSSRECORD_SEX].vchar = crossrecord_param.cross_record_list[i].sex;
				item_data.m_data_area[tabledef::CROSSRECORD_PROF].vchar = crossrecord_param.cross_record_list[i].prof;
				item_data.m_data_area[tabledef::CROSSRECORD_CAMP].vchar = crossrecord_param.cross_record_list[i].camp;

				{
					static UserCrossRecordDataHex hex_buff;
					BinToHex((const char *)(&crossrecord_param.cross_record_list[i].user_record_data), sizeof(UserCrossRecordData), hex_buff);
					hex_buff[sizeof(hex_buff) - 1] = 0;

					STRNCPY(item_data.m_data_area[tabledef::CROSSRECORD_RECORD_DATA].vcharp, hex_buff, item_data.m_data_area[tabledef::CROSSRECORD_RECORD_DATA].length);
				}

				{
					static RoleAppearanceHex hex_buff;
					BinToHex((const char *)(&crossrecord_param.cross_record_list[i].user_appearance_data), sizeof(RoleAppearance), hex_buff);
					hex_buff[sizeof(hex_buff) - 1] = 0;

					STRNCPY(item_data.m_data_area[tabledef::CROSSRECORD_APPEARANCE_DATA].vcharp, hex_buff, item_data.m_data_area[tabledef::CROSSRECORD_APPEARANCE_DATA].length);
				}

				if (DBCommand::RESULT_SUC != crossrecord_command->Save(stmt, &item_data))
				{
					ret = -2;
				}

				item_data.Free();
			}
			break; // case structcommon::CS_INSERT

		case structcommon::CS_UPDATE:
			{
				DataNodeList node_list;
				node_list.reserve(tabledef::TABLE_CROSSRECORD_MAXINDEX);

				DataNode node_name(tabledef::CROSSRECORD_ROLE_NAME);
				node_name.SetString(crossrecord_param.cross_record_list[i].role_name);
				node_list.push_back(node_name);

				DataNode node_level(tabledef::CROSSRECORD_LEVEL);
				node_level.SetInt32(crossrecord_param.cross_record_list[i].level);
				node_list.push_back(node_level);

				DataNode node_sex(tabledef::CROSSRECORD_SEX);
				node_sex.SetByte(crossrecord_param.cross_record_list[i].sex);
				node_list.push_back(node_sex);

				DataNode node_prof(tabledef::CROSSRECORD_PROF);
				node_prof.SetByte(crossrecord_param.cross_record_list[i].prof);
				node_list.push_back(node_prof);

				DataNode node_camp(tabledef::CROSSRECORD_CAMP);
				node_camp.SetByte(crossrecord_param.cross_record_list[i].camp);
				node_list.push_back(node_camp);

				{
					static UserCrossRecordDataHex hex_buff;
					BinToHex((const char *)(&crossrecord_param.cross_record_list[i].user_record_data), sizeof(UserCrossRecordData), hex_buff);
					hex_buff[sizeof(hex_buff) - 1] = 0;

					DataNode data(tabledef::CROSSRECORD_RECORD_DATA);
					data.SetString(hex_buff);
					node_list.push_back(data);
				}


				{
					static RoleAppearanceHex hex_buff;
					BinToHex((const char *)(&crossrecord_param.cross_record_list[i].user_appearance_data), sizeof(RoleAppearance), hex_buff);
					hex_buff[sizeof(hex_buff) - 1] = 0;

					DataNode data(tabledef::CROSSRECORD_APPEARANCE_DATA);
					data.SetString(hex_buff);
					node_list.push_back(data);
				}

				CmpNodeList cmp_node_list;
				CmpNode cmp_node_role_id(tabledef::CROSSRECORD_ROLE_ID);
				cmp_node_role_id.SetInt32(crossrecord_param.cross_record_list[i].role_id);
				cmp_node_list.push_back(cmp_node_role_id);

				int count = crossrecord_command->Update(stmt, cmp_node_list, node_list);
				if (count < 0)
				{
					ret = -3; 
				}
			}
			break; // case structcommon::CS_UPDATE

		} // switch end

	} // for end

	conn->commit();

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__InitGlobalRole(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(GlobalRoleDatalListParam) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitGlobalRole 1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	long long id_from = 0;
	if (!in_param.Pop(&id_from))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitGlobalRole 2");
		return rmi::DispatchParamError;
	}

	static GlobalRoleDatalListParam global_role_list_param;
	global_role_list_param.count = 0;

	int result = InitGlobalRole(id_from, &global_role_list_param);
	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitGlobalRole 3 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!global_role_list_param.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitGlobalRole 4 result:%d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitGlobalRole 5 result:%d", result);
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::InitGlobalRole(long long id_from, GlobalRoleDatalListParam *global_role_list_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *global_role_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_GLOBALROLE);

	QueryCondition condition;
	CmpNode node_globalrole_id(tabledef::GLOBALROLE_ID_GLOBALROLE);
	node_globalrole_id.SetLong64(id_from);
	node_globalrole_id.SetEqualType(EQUAL_TYPE_GRANTER);
	condition.cmp_list.push_back(node_globalrole_id);

	CmpNode node_role_id(tabledef::GLOBALROLE_ROLE_ID);
	node_role_id.SetLong64(0);
	node_role_id.SetEqualType(EQUAL_TYPE_GRANTER);
	condition.cmp_list.push_back(node_role_id);

	condition.query_num = GLOBAL_ROLE_DATA_ONCE_LOAD_MAX_COUNT;

	DataAdapterList global_role_list;
	int ret = global_role_command->Find(stmt, condition, &global_role_list);
	if (ret < 0)
	{
		global_role_list_param->count = 0;
		return ret;
	}

	ret = 0;

	int count = 0;
	DataAdapterList::iterator i = global_role_list.begin();

	for (; i != global_role_list.end() && count < GLOBAL_ROLE_DATA_ONCE_LOAD_MAX_COUNT; ++i)
	{
		GlobalRoleDatalListParam::RoleDataAttr &global_role_attr = global_role_list_param->role_data_list[count];

		global_role_attr.id_global_data = i->m_data_area[tabledef::GLOBALROLE_ID_GLOBALROLE].vint64;
		global_role_attr.change_state = structcommon::CS_NONE;
		global_role_attr.role_id = i->m_data_area[tabledef::GLOBALROLE_ROLE_ID].vint;

		GET_BINARY_DATA((*i), tabledef::GLOBALROLE_LITTLE_PET_DATA, global_role_attr.little_pet_param);
		GET_BINARY_DATA((*i), tabledef::GLOBALROLE_LOVE_TREE_DATA, global_role_attr.love_tree_param);
		GET_BINARY_DATA((*i), tabledef::GLOBALROLE_LOVE_CONTRACT_DATA, global_role_attr.love_contract_param);
		GET_BINARY_DATA((*i), tabledef::GLOBALROLE_JING_LING_HOME_DATA, global_role_attr.jing_ling_home_param);
		GET_BINARY_DATA((*i), tabledef::GLOBALROLE_ROLE_FIGHTING_DATA, global_role_attr.fighting_data);
		GET_BINARY_DATA((*i), tabledef::GLOBALROLE_QINGYUAN_GLOBAL_DATA, global_role_attr.qingyuan_global_param);
		GET_BINARY_DATA((*i), tabledef::GLOBALROLE_ROLE_APPEARANCE_DATA, global_role_attr.role_appearance);
		GET_BINARY_DATA((*i), tabledef::GLOBALROLE_COMMON_DATA_GLOBAL_DATA, global_role_attr.common_data_global_param);
		GET_BINARY_DATA((*i), tabledef::GLOBALROLE_SPOUSEHOME_DATA, global_role_attr.spouse_home_param);

		++count;
		i->Free();
	}

	global_role_list_param->count = count;

	for (; i != global_role_list.end(); ++i)
	{
		i->Free();
	}

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__SaveGlobalRole(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveGlobalRole 1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static GlobalRoleDatalListParam global_role_list_param;
	global_role_list_param.count = 0;

	if (!global_role_list_param.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveGlobalRole 2");
		return rmi::DispatchParamError;
	}

	int result = SaveGlobalRole(global_role_list_param);
	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveGlobalRole 3  result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveGlobalRole 4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::SaveGlobalRole(const GlobalRoleDatalListParam &global_role_list_param)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *global_role_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_GLOBALROLE);
	DBCommand *global_role_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_GLOBALROLE);

	conn->begin();

	for (int i = 0; i < global_role_list_param.count && i < GLOBAL_ROLE_DATA_ONCE_LOAD_MAX_COUNT; ++i)
	{
		const GlobalRoleDatalListParam::RoleDataAttr &global_role_attr = global_role_list_param.role_data_list[i];

		switch (global_role_attr.change_state)
		{
		case structcommon::CS_DELETE:
		{
			CmpNodeList cmp_node;

			CmpNode role_id_node(tabledef::GLOBALROLE_ROLE_ID);
			role_id_node.SetInt32(global_role_attr.role_id);
			cmp_node.push_back(role_id_node);

			int count = global_role_command->Remove(stmt, cmp_node);
			if (count < 1)
			{
				ret = -1;
			}
		}
		break; // case structcommon::CS_DELETE

		case structcommon::CS_INSERT:
		{
			DataAdapter item_data = global_role_table->GetPrototype();

			item_data.Malloc();

			item_data.m_data_area[tabledef::GLOBALROLE_ROLE_ID].vint = global_role_attr.role_id;

			INSERT_BINARY_DATA_TO_ADAPTER(item_data, tabledef::GLOBALROLE_LITTLE_PET_DATA, LittlePetDataParamHex, global_role_attr.little_pet_param);
			INSERT_BINARY_DATA_TO_ADAPTER(item_data, tabledef::GLOBALROLE_LOVE_TREE_DATA, LoveTreeDataHex, global_role_attr.love_tree_param);
			INSERT_BINARY_DATA_TO_ADAPTER(item_data, tabledef::GLOBALROLE_LOVE_CONTRACT_DATA, LoveContractDataHex, global_role_attr.love_contract_param);
			INSERT_BINARY_DATA_TO_ADAPTER(item_data, tabledef::GLOBALROLE_JING_LING_HOME_DATA, JingLingHomeParamHex, global_role_attr.jing_ling_home_param);
			INSERT_BINARY_DATA_TO_ADAPTER(item_data, tabledef::GLOBALROLE_ROLE_FIGHTING_DATA, FightingParamHex, global_role_attr.fighting_data);
			INSERT_BINARY_DATA_TO_ADAPTER(item_data, tabledef::GLOBALROLE_QINGYUAN_GLOBAL_DATA, QingyuanGlobalParamHex, global_role_attr.qingyuan_global_param);
			INSERT_BINARY_DATA_TO_ADAPTER(item_data, tabledef::GLOBALROLE_ROLE_APPEARANCE_DATA, RoleAppearanceHex, global_role_attr.role_appearance);
			INSERT_BINARY_DATA_TO_ADAPTER(item_data, tabledef::GLOBALROLE_COMMON_DATA_GLOBAL_DATA, CommonDataGlobalParamHex, global_role_attr.common_data_global_param);
			INSERT_BINARY_DATA_TO_ADAPTER(item_data, tabledef::GLOBALROLE_SPOUSEHOME_DATA, SpouseHomeParamHex, global_role_attr.spouse_home_param);

			if (DBCommand::RESULT_SUC != global_role_command->Save(stmt, &item_data))
			{
				ret = -2;
			}

			item_data.Free();
		}
		break; // case structcommon::CS_INSERT

		case structcommon::CS_UPDATE:
		{
			DataNodeList node_list;
			node_list.reserve(tabledef::TABLE_GLOBALROLE_MAXINDEX);

			DataNode role_id_node(tabledef::GLOBALROLE_ROLE_ID);
			role_id_node.SetInt32(global_role_attr.role_id);
			node_list.push_back(role_id_node);

			UPDATE_BINARY_DATA_TO_LIST(little_pet_data_node, tabledef::GLOBALROLE_LITTLE_PET_DATA, global_role_attr.little_pet_param, LittlePetDataParamHex, node_list);
			UPDATE_BINARY_DATA_TO_LIST(tmp_node_name, tabledef::GLOBALROLE_LOVE_TREE_DATA, global_role_attr.love_tree_param, LoveTreeDataHex, node_list);
			UPDATE_BINARY_DATA_TO_LIST(tmp_node_name, tabledef::GLOBALROLE_LOVE_CONTRACT_DATA, global_role_attr.love_contract_param, LoveContractDataHex, node_list);
			UPDATE_BINARY_DATA_TO_LIST(tmp_node_name, tabledef::GLOBALROLE_JING_LING_HOME_DATA, global_role_attr.jing_ling_home_param, JingLingHomeParamHex, node_list);
			UPDATE_BINARY_DATA_TO_LIST(tmp_node_name, tabledef::GLOBALROLE_ROLE_FIGHTING_DATA, global_role_attr.fighting_data, FightingParamHex, node_list);
			UPDATE_BINARY_DATA_TO_LIST(tmp_node_name, tabledef::GLOBALROLE_QINGYUAN_GLOBAL_DATA, global_role_attr.qingyuan_global_param, QingyuanGlobalParamHex, node_list);
			UPDATE_BINARY_DATA_TO_LIST(role_appearance_node, tabledef::GLOBALROLE_ROLE_APPEARANCE_DATA, global_role_attr.role_appearance, RoleAppearanceHex, node_list);
			UPDATE_BINARY_DATA_TO_LIST(tmp_node_name, tabledef::GLOBALROLE_COMMON_DATA_GLOBAL_DATA, global_role_attr.common_data_global_param, CommonDataGlobalParamHex, node_list);
			UPDATE_BINARY_DATA_TO_LIST(tmp_node_name, tabledef::GLOBALROLE_SPOUSEHOME_DATA, global_role_attr.spouse_home_param, SpouseHomeParamHex, node_list);

			CmpNodeList cmp_node_list;
			CmpNode cmp_node_id(tabledef::GLOBALROLE_ROLE_ID);
			cmp_node_id.SetInt32(global_role_attr.role_id);
			cmp_node_list.push_back(cmp_node_id);

			int count = global_role_command->Update(stmt, cmp_node_list, node_list);
			if (count < 0)
			{
				ret = -3;
			}
		}
		break; // case structcommon::CS_UPDATE

		} // switch end

	} // for end

	conn->commit();

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__InitWorldStatus2(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(WorldStatus2Param) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitWorldStatus2_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	long long id_from = 0;

	if (!in_param.Pop(&id_from))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitWorldStatus2_2");
		return rmi::DispatchParamError;
	}

	static WorldStatus2Param worldstatus2_param;
	memset(&worldstatus2_param, 0, sizeof(worldstatus2_param));

	int result = InitWorldStatus2(id_from, &worldstatus2_param);

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitWorldStatus2_3 result %d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!worldstatus2_param.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitWorldStatus2_4 result %d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitWorldStatus2_5 result %d", result);
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::InitWorldStatus2(long long id_from, WorldStatus2Param *worldstatus2_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *worldstatus2_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_WORLDSTATUS2);

	QueryCondition worldstatus2_cond;
	CmpNode worldstate2_node1(tabledef::WORLDSTATUS2_IDWORLDSTATUS2);
	worldstate2_node1.SetLong64(id_from);
	worldstate2_node1.SetEqualType(EQUAL_TYPE_GRANTER);
	worldstatus2_cond.cmp_list.push_back(worldstate2_node1);
	worldstatus2_cond.query_num = 1;

	DataAdapterList worldstate2_list;
	int ret = worldstatus2_command->Find(stmt, worldstatus2_cond, &worldstate2_list);
	if (ret < 0)
	{
		return ret;
	}

	ret = 0;

	DataAdapterList::iterator i = worldstate2_list.begin();
	for (; i != worldstate2_list.end(); ++i)
	{
		///////////////////////////////////////////////////////////////////////////////////

		GET_BINARY_DATA((*i), tabledef::WORLDSTATUS2_DROP_LOG_LIST_DATA, worldstatus2_param->drop_log_list_param);
		GET_BINARY_DATA((*i), tabledef::WORLDSTATUS2_WEDDING_DATA, worldstatus2_param->wedding_param);

		{
			memset(&worldstatus2_param->common_data_2, 0, sizeof(worldstatus2_param->common_data_2));

			int length = i->m_data_area[tabledef::WORLDSTATUS2_COMMON_DATA_2].length;
			if (length <= (int)sizeof(worldstatus2_param->common_data_2) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS2_COMMON_DATA_2].vcharp, length, (char*)&worldstatus2_param->common_data_2);
			}
		}

		{
			memset(&worldstatus2_param->global_profess_param, 0, sizeof(worldstatus2_param->global_profess_param));

			int length = i->m_data_area[tabledef::WORLDSTATUS2_GLOBAL_PROFESS_DATA].length;
			if (length <= (int)sizeof(worldstatus2_param->global_profess_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS2_GLOBAL_PROFESS_DATA].vcharp, length, (char*)&worldstatus2_param->global_profess_param);
			}
		}

		{
			memset(&worldstatus2_param->drop_log_fb_param, 0, sizeof(worldstatus2_param->drop_log_fb_param));

			int length = i->m_data_area[tabledef::WORLDSTATUS2_DROP_LOG_FB_DATA].length;
			if (length <= (int)sizeof(worldstatus2_param->drop_log_fb_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS2_DROP_LOG_FB_DATA].vcharp, length, (char*)&worldstatus2_param->drop_log_fb_param);
			}
		}

		{
			memset(&worldstatus2_param->world_acquisition_log_param, 0, sizeof(worldstatus2_param->world_acquisition_log_param));

			int length = i->m_data_area[tabledef::WORLDSTATUS2_WORLD_ACQUISITION_LOG].length;
			if (length <= (int)sizeof(worldstatus2_param->world_acquisition_log_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS2_WORLD_ACQUISITION_LOG].vcharp, length, (char*)&worldstatus2_param->world_acquisition_log_param);
			}
		}

		// i->Free(); TODO 非常严重 这种错误相当严重！！！
		break;  // 只取一条记录 
	}

	for (; i != worldstate2_list.end(); ++i)
	{
		i->Free();
	}

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__SaveWorldStatus2(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveWorldStatus2_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static WorldStatus2Param worldstatus2_param;

	if (!worldstatus2_param.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveWorldStatus2_2");
		return rmi::DispatchParamError;
	}

	int result = SaveWorldStatus2(worldstatus2_param);
	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveWorldStatus2_3 result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "v::__SaveWorldStatus2_4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::SaveWorldStatus2(const WorldStatus2Param &worldstatus2_param)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *worldstatus2_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_WORLDSTATUS2);
	DBCommand *worldstatus2_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_WORLDSTATUS2);

	DataNodeList worldstatus2_item_list;
	worldstatus2_item_list.reserve(tabledef::TABLE_WORLDSTATUS2_MAXINDEX);

	//////////////////////////////////////////////////////////////////////////

	UPDATE_BINARY_DATA_TO_LIST(drop_log_list_data, tabledef::WORLDSTATUS2_DROP_LOG_LIST_DATA,
		worldstatus2_param.drop_log_list_param, DropLogListParamHex, worldstatus2_item_list);

	UPDATE_BINARY_DATA_TO_LIST(common_data_2_node, tabledef::WORLDSTATUS2_COMMON_DATA_2,
		worldstatus2_param.common_data_2, WorldStatus2CommonDataParamHex, worldstatus2_item_list);

	UPDATE_BINARY_DATA_TO_LIST(wedding_data_node, tabledef::WORLDSTATUS2_WEDDING_DATA,
		worldstatus2_param.wedding_param, WeddingParamHex, worldstatus2_item_list);

	UPDATE_BINARY_DATA_TO_LIST(global_profess_data_node, tabledef::WORLDSTATUS2_GLOBAL_PROFESS_DATA,
		worldstatus2_param.global_profess_param, GlobalProfessParamHex, worldstatus2_item_list);

	UPDATE_BINARY_DATA_TO_LIST(drop_log_fb_data, tabledef::WORLDSTATUS2_DROP_LOG_FB_DATA,
		worldstatus2_param.drop_log_fb_param, DropLogFbParamHex, worldstatus2_item_list);

	UPDATE_BINARY_DATA_TO_LIST(world_acquisition_log_data, tabledef::WORLDSTATUS2_WORLD_ACQUISITION_LOG,
		worldstatus2_param.world_acquisition_log_param, WorldAcquisitionLogParamHex, worldstatus2_item_list);

	//////////////////////////////////////////////////////////////////////////
		
	CmpNodeList condition;

	CmpNode for_index_node(tabledef::WORLDSTATUS2_FOR_INDEX);
	for_index_node.SetLong64(1);  // 仅有一条记录 
	condition.push_back(for_index_node);

	int count = worldstatus2_command->Update(stmt, condition, worldstatus2_item_list);
	if (count < 0)
	{
		ret = -1;
	}
	else if (0 == count)
	{
		DataAdapter worldstatus2_item = worldstatus2_table->GetPrototype();
		worldstatus2_item.Malloc();

		worldstatus2_item.m_data_area[tabledef::WORLDSTATUS2_FOR_INDEX].vint64 = 1;

		if (DBCommand::RESULT_SUC != worldstatus2_command->Save(stmt, &worldstatus2_item))
		{
			ret = -2;
		}

		worldstatus2_item.Free();
	}

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__InitWorldStatus3(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) + sizeof(WorldStatus3Param) * 2)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitWorldStatus3_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	long long id_from = 0;

	if (!in_param.Pop(&id_from))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitWorldStatus3_2");
		return rmi::DispatchParamError;
	}

	static WorldStatus3Param worldstatus3_param;
	memset(&worldstatus3_param, 0, sizeof(worldstatus3_param));

	int result = InitWorldStatus3(id_from, &worldstatus3_param);

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitWorldStatus3_3 result %d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!worldstatus3_param.Serialize(*out_param))
		{
			dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitWorldStatus3_4 result %d", result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__InitWorldStatus3_5 result %d", result);
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::InitWorldStatus3(long long id_from, WorldStatus3Param *worldstatus3_param)
{
	INIT_STMT(conn, stmt, -1);

	DBCommand *worldstatus3_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_WORLDSTATUS3);

	QueryCondition worldstatus3_cond;
	CmpNode worldstatus3_node1(tabledef::WORLDSTATUS3_IDWORLDSTATUS3);
	worldstatus3_node1.SetLong64(id_from);
	worldstatus3_node1.SetEqualType(EQUAL_TYPE_GRANTER);
	worldstatus3_cond.cmp_list.push_back(worldstatus3_node1);
	worldstatus3_cond.query_num = 1;

	DataAdapterList worldstate3_list;
	int ret = worldstatus3_command->Find(stmt, worldstatus3_cond, &worldstate3_list);
	if (ret < 0)
	{
		return ret;
	}

	ret = 0;

	DataAdapterList::iterator i = worldstate3_list.begin();
	for (; i != worldstate3_list.end(); ++i)
	{
		///////////////////////////////////////////////////////////////////////////////////
		{
			memset(&worldstatus3_param->common_data, 0, sizeof(worldstatus3_param->common_data));

			int length = i->m_data_area[tabledef::WORLDSTATUS3_COMMON_DATA].length;
			if (length <= (int)sizeof(worldstatus3_param->common_data) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS3_COMMON_DATA].vcharp, length, (char*)&worldstatus3_param->common_data);
			}
		}

		{
			memset(&worldstatus3_param->toprank_param, 0, sizeof(worldstatus3_param->toprank_param));

			int length = i->m_data_area[tabledef::WORLDSTATUS3_TOPRANK_DATA].length;
			if (length <= (int)sizeof(worldstatus3_param->toprank_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS3_TOPRANK_DATA].vcharp, length, (char*)&worldstatus3_param->toprank_param);
			}
		}
		
		{
			memset(&worldstatus3_param->lottery_count_param, 0, sizeof(worldstatus3_param->lottery_count_param));

			int length = i->m_data_area[tabledef::WORLDSTATUS3_LOTTERY_COUNT_DATA].length;
			if (length <= (int)sizeof(worldstatus3_param->lottery_count_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS3_LOTTERY_COUNT_DATA].vcharp, length, (char*)&worldstatus3_param->lottery_count_param);
			}
		}

		{
			memset(&worldstatus3_param->rand_activity_param, 0, sizeof(worldstatus3_param->rand_activity_param));

			int lenth = i->m_data_area[tabledef::WORLDSTATUS3_RAND_ACTIVITY_DATA].length;
			if (lenth <= (int)sizeof(worldstatus3_param->rand_activity_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS3_RAND_ACTIVITY_DATA].vcharp, lenth, (char*)&worldstatus3_param->rand_activity_param);
			}
		}

		{
			memset(&worldstatus3_param->lucky_cloud_buy_param, 0, sizeof(worldstatus3_param->lucky_cloud_buy_param));

			int lenth = i->m_data_area[tabledef::WORLDSTATUS3_LUCKY_CLOUD_BUY_PARAM_DATA].length;
			if (lenth <= (int)sizeof(worldstatus3_param->lucky_cloud_buy_param) * 2)
			{
				HexToBin(i->m_data_area[tabledef::WORLDSTATUS3_LUCKY_CLOUD_BUY_PARAM_DATA].vcharp, lenth, (char*)&worldstatus3_param->lucky_cloud_buy_param);
			}
		}
		// i->Free(); TODO 非常严重 这种错误相当严重！！！
		break;  // 只取一条记录 
	}

	for (; i != worldstate3_list.end(); ++i)
	{
		i->Free();
	}

	return ret < 0 ? ret : 0;
}

int RMIGlobalObject::__SaveWorldStatus3(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveWorldStatus3_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static WorldStatus3Param worldstatus3_param;

	if (!worldstatus3_param.Unserialize(in_param))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveWorldStatus3_2");
		return rmi::DispatchParamError;
	}

	int result = SaveWorldStatus3(worldstatus3_param);
	if (0 != result)
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "RMIGlobalObject::__SaveWorldStatus3_3 result:%d", result);
	}

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_global.printf(LL_CRITICAL, "v::__SaveWorldStatus3_4 result:%d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIGlobalObject::SaveWorldStatus3(const WorldStatus3Param &worldstatus3_param)
{
	INIT_STMT(conn, stmt, -1);

	int ret = 0;

	const MataTableDef *worldstatus3_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_WORLDSTATUS3);
	DBCommand *worldstatus3_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_WORLDSTATUS3);

	DataNodeList worldstatus3_item_list;
	worldstatus3_item_list.reserve(tabledef::TABLE_WORLDSTATUS3_MAXINDEX);

	//////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(common_data, tabledef::WORLDSTATUS3_COMMON_DATA,
		worldstatus3_param.common_data, WorldStatus3CommomParamHex, worldstatus3_item_list);

	/////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(top_rank_node, tabledef::WORLDSTATUS3_TOPRANK_DATA,
		worldstatus3_param.toprank_param, TopRankParamHex, worldstatus3_item_list);

	/////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(lottery_count_node, tabledef::WORLDSTATUS3_LOTTERY_COUNT_DATA,
		worldstatus3_param.lottery_count_param, WorldLotteryCountParamHex, worldstatus3_item_list);

	/////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(rand_activity_data_node, tabledef::WORLDSTATUS3_RAND_ACTIVITY_DATA,
		worldstatus3_param.rand_activity_param, RandActivityParamHex, worldstatus3_item_list);

	/////////////////////////////////////////////////////////////////////////
	UPDATE_BINARY_DATA_TO_LIST(lucky_cloud_buy_param_data_node, tabledef::WORLDSTATUS3_LUCKY_CLOUD_BUY_PARAM_DATA,
		worldstatus3_param.lucky_cloud_buy_param, RandActivityLuckyCloudBuyParamHex, worldstatus3_item_list);

	CmpNodeList condition;

	CmpNode for_index_node(tabledef::WORLDSTATUS3_FOR_INDEX);
	for_index_node.SetLong64(1);  // 仅有一条记录 
	condition.push_back(for_index_node);

	int count = worldstatus3_command->Update(stmt, condition, worldstatus3_item_list);
	if (count < 0)
	{
		ret = -1;
	}
	else if (0 == count)
	{
		DataAdapter worldstatus3_item = worldstatus3_table->GetPrototype();
		worldstatus3_item.Malloc();

		worldstatus3_item.m_data_area[tabledef::WORLDSTATUS3_FOR_INDEX].vint64 = 1;

		if (DBCommand::RESULT_SUC != worldstatus3_command->Save(stmt, &worldstatus3_item))
		{
			ret = -2;
		}

		worldstatus3_item.Free();
	}

	return ret < 0 ? ret : 0;
}