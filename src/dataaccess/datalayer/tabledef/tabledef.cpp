
#include "tabledef.h"


namespace tabledef
{

TableInitArg GetaccountgoldTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "accountgold";
	arg.mata_data.resize(7);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "accountgold_id";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_STRING;
	arg.mata_data[1].name = "plat_user_name";
	arg.mata_data[1].length = 64;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "role_id";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_LONG64;
	arg.mata_data[3].name = "gold";
	arg.mata_data[3].length = 8;

	arg.mata_data[4].type = DATYPE_LONG64;
	arg.mata_data[4].name = "gold_history";
	arg.mata_data[4].length = 8;

	arg.mata_data[5].type = DATYPE_LONG64;
	arg.mata_data[5].name = "last_get_gold_time";
	arg.mata_data[5].length = 8;

	arg.mata_data[6].type = DATYPE_INT;
	arg.mata_data[6].name = "authority_type";
	arg.mata_data[6].length = 4;

	return arg;
}

TableInitArg GetaccountidentityTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "accountidentity";
	arg.mata_data.resize(4);

	arg.mata_data[0].type = DATYPE_INT;
	arg.mata_data[0].name = "idAccountIdentity";
	arg.mata_data[0].length = 4;

	arg.mata_data[1].type = DATYPE_STRING;
	arg.mata_data[1].name = "plat_user_name";
	arg.mata_data[1].length = 64;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "name";
	arg.mata_data[2].length = 20;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "identity_num";
	arg.mata_data[3].length = 20;

	return arg;
}

TableInitArg Getblack_listsTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "black_lists";
	arg.mata_data.resize(8);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idblack";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "black_index";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_LONG64;
	arg.mata_data[3].name = "FOR_INDEX";
	arg.mata_data[3].length = 8;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "black_uid";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "black_name";
	arg.mata_data[5].length = 32;

	arg.mata_data[6].type = DATYPE_CHAR;
	arg.mata_data[6].name = "sex";
	arg.mata_data[6].length = 1;

	arg.mata_data[7].type = DATYPE_INT;
	arg.mata_data[7].name = "prof";
	arg.mata_data[7].length = 4;

	return arg;
}

TableInitArg GetcampTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "camp";
	arg.mata_data.resize(10);

	arg.mata_data[0].type = DATYPE_INT;
	arg.mata_data[0].name = "idcamp";
	arg.mata_data[0].length = 4;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "camp_type";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "role_count";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_INT;
	arg.mata_data[3].name = "guild_count";
	arg.mata_data[3].length = 4;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "rich_level";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_LONG64;
	arg.mata_data[5].name = "earnings_end_time1";
	arg.mata_data[5].length = 8;

	arg.mata_data[6].type = DATYPE_INT;
	arg.mata_data[6].name = "earnings1";
	arg.mata_data[6].length = 4;

	arg.mata_data[7].type = DATYPE_LONG64;
	arg.mata_data[7].name = "earnings_end_time2";
	arg.mata_data[7].length = 8;

	arg.mata_data[8].type = DATYPE_INT;
	arg.mata_data[8].name = "earnings2";
	arg.mata_data[8].length = 4;

	arg.mata_data[9].type = DATYPE_LONG64;
	arg.mata_data[9].name = "total_capability";
	arg.mata_data[9].length = 8;

	return arg;
}

TableInitArg GetchallengeruserTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "challengeruser";
	arg.mata_data.resize(7);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "challenger_user_id";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_LONG64;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 8;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "rankpos";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "challenge_user_data";
	arg.mata_data[3].length = 2048;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "common_data";
	arg.mata_data[4].length = 256;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "report_info_data";
	arg.mata_data[5].length = 2304;

	arg.mata_data[6].type = DATYPE_STRING;
	arg.mata_data[6].name = "role_appearance_data";
	arg.mata_data[6].length = 256;

	return arg;
}

TableInitArg GetcommandTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "command";
	arg.mata_data.resize(6);

	arg.mata_data[0].type = DATYPE_INT;
	arg.mata_data[0].name = "idcommand";
	arg.mata_data[0].length = 4;

	arg.mata_data[1].type = DATYPE_STRING;
	arg.mata_data[1].name = "creator";
	arg.mata_data[1].length = 64;

	arg.mata_data[2].type = DATYPE_LONG64;
	arg.mata_data[2].name = "createtime";
	arg.mata_data[2].length = 8;

	arg.mata_data[3].type = DATYPE_CHAR;
	arg.mata_data[3].name = "type";
	arg.mata_data[3].length = 1;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "cmd";
	arg.mata_data[4].length = 256;

	arg.mata_data[5].type = DATYPE_LONG64;
	arg.mata_data[5].name = "confirmtime";
	arg.mata_data[5].length = 8;

	return arg;
}

TableInitArg Getcross_personrankTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "cross_personrank";
	arg.mata_data.resize(16);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idcrosspersonrank";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "rank_index";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_LONG64;
	arg.mata_data[2].name = "uuid";
	arg.mata_data[2].length = 8;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "user_name";
	arg.mata_data[3].length = 32;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "role_sex";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_INT;
	arg.mata_data[5].name = "role_prof";
	arg.mata_data[5].length = 4;

	arg.mata_data[6].type = DATYPE_INT;
	arg.mata_data[6].name = "role_camp";
	arg.mata_data[6].length = 4;

	arg.mata_data[7].type = DATYPE_LONG64;
	arg.mata_data[7].name = "role_exp";
	arg.mata_data[7].length = 8;

	arg.mata_data[8].type = DATYPE_INT;
	arg.mata_data[8].name = "role_level";
	arg.mata_data[8].length = 4;

	arg.mata_data[9].type = DATYPE_INT;
	arg.mata_data[9].name = "rank_type";
	arg.mata_data[9].length = 4;

	arg.mata_data[10].type = DATYPE_LONG64;
	arg.mata_data[10].name = "rank_value";
	arg.mata_data[10].length = 8;

	arg.mata_data[11].type = DATYPE_INT;
	arg.mata_data[11].name = "record_index";
	arg.mata_data[11].length = 4;

	arg.mata_data[12].type = DATYPE_LONG64;
	arg.mata_data[12].name = "flexible_ll";
	arg.mata_data[12].length = 8;

	arg.mata_data[13].type = DATYPE_STRING;
	arg.mata_data[13].name = "flexible_name";
	arg.mata_data[13].length = 32;

	arg.mata_data[14].type = DATYPE_INT;
	arg.mata_data[14].name = "flexible_int";
	arg.mata_data[14].length = 4;

	arg.mata_data[15].type = DATYPE_INT;
	arg.mata_data[15].name = "vip_level";
	arg.mata_data[15].length = 4;

	return arg;
}

TableInitArg GetcrossrecordTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "crossrecord";
	arg.mata_data.resize(9);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idcrossrecord";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "role_name";
	arg.mata_data[2].length = 32;

	arg.mata_data[3].type = DATYPE_INT;
	arg.mata_data[3].name = "level";
	arg.mata_data[3].length = 4;

	arg.mata_data[4].type = DATYPE_CHAR;
	arg.mata_data[4].name = "sex";
	arg.mata_data[4].length = 1;

	arg.mata_data[5].type = DATYPE_CHAR;
	arg.mata_data[5].name = "prof";
	arg.mata_data[5].length = 1;

	arg.mata_data[6].type = DATYPE_CHAR;
	arg.mata_data[6].name = "camp";
	arg.mata_data[6].length = 1;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "record_data";
	arg.mata_data[7].length = 512;

	arg.mata_data[8].type = DATYPE_STRING;
	arg.mata_data[8].name = "appearance_data";
	arg.mata_data[8].length = 256;

	return arg;
}

TableInitArg GetcrossstatusTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "crossstatus";
	arg.mata_data.resize(5);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idcrossstatus";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_LONG64;
	arg.mata_data[1].name = "FOR_INDEX";
	arg.mata_data[1].length = 8;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "last_update_dayid";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "cross_person_rank_title_data";
	arg.mata_data[3].length = 8192;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "cross_activity_data";
	arg.mata_data[4].length = 2048;

	return arg;
}

TableInitArg GetdailyfindTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "dailyfind";
	arg.mata_data.resize(9);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "iddailyfind";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_LONG64;
	arg.mata_data[2].name = "FOR_INDEX";
	arg.mata_data[2].length = 8;

	arg.mata_data[3].type = DATYPE_INT;
	arg.mata_data[3].name = "find_time";
	arg.mata_data[3].length = 4;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "role_level";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_INT;
	arg.mata_data[5].name = "find_index";
	arg.mata_data[5].length = 4;

	arg.mata_data[6].type = DATYPE_INT;
	arg.mata_data[6].name = "param";
	arg.mata_data[6].length = 4;

	arg.mata_data[7].type = DATYPE_LONG64;
	arg.mata_data[7].name = "param1";
	arg.mata_data[7].length = 8;

	arg.mata_data[8].type = DATYPE_INT;
	arg.mata_data[8].name = "find_count";
	arg.mata_data[8].length = 4;

	return arg;
}

TableInitArg GetenemyTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "enemy";
	arg.mata_data.resize(9);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idenemy";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_LONG64;
	arg.mata_data[2].name = "FOR_INDEX";
	arg.mata_data[2].length = 8;

	arg.mata_data[3].type = DATYPE_INT;
	arg.mata_data[3].name = "enemy_uid";
	arg.mata_data[3].length = 4;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "enemy_plat_type";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_INT;
	arg.mata_data[5].name = "kill_count";
	arg.mata_data[5].length = 4;

	arg.mata_data[6].type = DATYPE_LONG64;
	arg.mata_data[6].name = "last_kill_time";
	arg.mata_data[6].length = 8;

	arg.mata_data[7].type = DATYPE_INT;
	arg.mata_data[7].name = "enemy_index";
	arg.mata_data[7].length = 4;

	arg.mata_data[8].type = DATYPE_INT;
	arg.mata_data[8].name = "be_kill_count";
	arg.mata_data[8].length = 4;

	return arg;
}

TableInitArg GetfishpoolTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "fishpool";
	arg.mata_data.resize(5);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idfishpool";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "fish_role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "common_data";
	arg.mata_data[2].length = 4096;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "raise_fish_data";
	arg.mata_data[3].length = 2048;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "today_raise_record";
	arg.mata_data[4].length = 512;

	return arg;
}

TableInitArg GetfriendsTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "friends";
	arg.mata_data.resize(11);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idfriend";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_LONG64;
	arg.mata_data[2].name = "FOR_INDEX";
	arg.mata_data[2].length = 8;

	arg.mata_data[3].type = DATYPE_INT;
	arg.mata_data[3].name = "friend_uid";
	arg.mata_data[3].length = 4;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "intimacy";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_INT;
	arg.mata_data[5].name = "team_today_intimacy";
	arg.mata_data[5].length = 4;

	arg.mata_data[6].type = DATYPE_INT;
	arg.mata_data[6].name = "friend_index";
	arg.mata_data[6].length = 4;

	arg.mata_data[7].type = DATYPE_INT;
	arg.mata_data[7].name = "gift_count";
	arg.mata_data[7].length = 4;

	arg.mata_data[8].type = DATYPE_INT;
	arg.mata_data[8].name = "favorable_impression";
	arg.mata_data[8].length = 4;

	arg.mata_data[9].type = DATYPE_INT;
	arg.mata_data[9].name = "day_chat_count";
	arg.mata_data[9].length = 4;

	arg.mata_data[10].type = DATYPE_LONG64;
	arg.mata_data[10].name = "add_timestamp";
	arg.mata_data[10].length = 8;

	return arg;
}

TableInitArg GetglobalroleTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "globalrole";
	arg.mata_data.resize(11);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "id_globalrole";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "little_pet_data";
	arg.mata_data[2].length = 10240;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "love_tree_data";
	arg.mata_data[3].length = 128;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "love_contract_data";
	arg.mata_data[4].length = 5000;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "jing_ling_home_data";
	arg.mata_data[5].length = 2048;

	arg.mata_data[6].type = DATYPE_STRING;
	arg.mata_data[6].name = "role_fighting_data";
	arg.mata_data[6].length = 4096;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "qingyuan_global_data";
	arg.mata_data[7].length = 2048;

	arg.mata_data[8].type = DATYPE_STRING;
	arg.mata_data[8].name = "role_appearance_data";
	arg.mata_data[8].length = 256;

	arg.mata_data[9].type = DATYPE_STRING;
	arg.mata_data[9].name = "common_data_global_data";
	arg.mata_data[9].length = 4096;

	arg.mata_data[10].type = DATYPE_STRING;
	arg.mata_data[10].name = "spousehome_data";
	arg.mata_data[10].length = 1024;

	return arg;
}

TableInitArg GetguildTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "guild";
	arg.mata_data.resize(25);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "guild_id_inc";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "guild_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "guild_name";
	arg.mata_data[2].length = 32;

	arg.mata_data[3].type = DATYPE_LONG64;
	arg.mata_data[3].name = "create_time";
	arg.mata_data[3].length = 8;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "guild_level";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_INT;
	arg.mata_data[5].name = "guild_exp";
	arg.mata_data[5].length = 4;

	arg.mata_data[6].type = DATYPE_INT;
	arg.mata_data[6].name = "camp";
	arg.mata_data[6].length = 4;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "notice";
	arg.mata_data[7].length = 256;

	arg.mata_data[8].type = DATYPE_LONG64;
	arg.mata_data[8].name = "last_update_dayid";
	arg.mata_data[8].length = 8;

	arg.mata_data[9].type = DATYPE_INT;
	arg.mata_data[9].name = "warning_dismiss_days";
	arg.mata_data[9].length = 4;

	arg.mata_data[10].type = DATYPE_INT;
	arg.mata_data[10].name = "applyfor_setup";
	arg.mata_data[10].length = 4;

	arg.mata_data[11].type = DATYPE_LONG64;
	arg.mata_data[11].name = "last_active_time";
	arg.mata_data[11].length = 8;

	arg.mata_data[12].type = DATYPE_INT;
	arg.mata_data[12].name = "auto_kickout_setup";
	arg.mata_data[12].length = 4;

	arg.mata_data[13].type = DATYPE_INT;
	arg.mata_data[13].name = "guild_max_capability";
	arg.mata_data[13].length = 4;

	arg.mata_data[14].type = DATYPE_INT;
	arg.mata_data[14].name = "applyfor_need_capability";
	arg.mata_data[14].length = 4;

	arg.mata_data[15].type = DATYPE_INT;
	arg.mata_data[15].name = "applyfor_need_level";
	arg.mata_data[15].length = 4;

	arg.mata_data[16].type = DATYPE_LONG64;
	arg.mata_data[16].name = "last_uplevel_timestamp";
	arg.mata_data[16].length = 8;

	arg.mata_data[17].type = DATYPE_INT;
	arg.mata_data[17].name = "callin_times";
	arg.mata_data[17].length = 4;

	arg.mata_data[18].type = DATYPE_INT;
	arg.mata_data[18].name = "today_monster_finish_seq";
	arg.mata_data[18].length = 4;

	arg.mata_data[19].type = DATYPE_STRING;
	arg.mata_data[19].name = "today_monster_finish_username";
	arg.mata_data[19].length = 32;

	arg.mata_data[20].type = DATYPE_STRING;
	arg.mata_data[20].name = "common_data";
	arg.mata_data[20].length = 512;

	arg.mata_data[21].type = DATYPE_INT;
	arg.mata_data[21].name = "active_degree";
	arg.mata_data[21].length = 4;

	arg.mata_data[22].type = DATYPE_STRING;
	arg.mata_data[22].name = "boss_list";
	arg.mata_data[22].length = 2048;

	arg.mata_data[23].type = DATYPE_STRING;
	arg.mata_data[23].name = "storge";
	arg.mata_data[23].length = 24575;

	arg.mata_data[24].type = DATYPE_STRING;
	arg.mata_data[24].name = "enemy_list";
	arg.mata_data[24].length = 13200;

	return arg;
}

TableInitArg GetguildapplyforTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "guildapplyfor";
	arg.mata_data.resize(6);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idguildapplyfor";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "guild_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "apply_for_index";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_LONG64;
	arg.mata_data[3].name = "FOR_INDEX";
	arg.mata_data[3].length = 8;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "uid";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_LONG64;
	arg.mata_data[5].name = "applyfor_time";
	arg.mata_data[5].length = 8;

	return arg;
}

TableInitArg GetguildeventTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "guildevent";
	arg.mata_data.resize(15);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idguildevent";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "guild_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "event_index";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_LONG64;
	arg.mata_data[3].name = "FOR_INDEX";
	arg.mata_data[3].length = 8;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "event_type";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "event_owner";
	arg.mata_data[5].length = 32;

	arg.mata_data[6].type = DATYPE_INT;
	arg.mata_data[6].name = "event_owner_post";
	arg.mata_data[6].length = 4;

	arg.mata_data[7].type = DATYPE_LONG64;
	arg.mata_data[7].name = "event_time";
	arg.mata_data[7].length = 8;

	arg.mata_data[8].type = DATYPE_INT;
	arg.mata_data[8].name = "big_event";
	arg.mata_data[8].length = 4;

	arg.mata_data[9].type = DATYPE_INT;
	arg.mata_data[9].name = "cannot_remove";
	arg.mata_data[9].length = 4;

	arg.mata_data[10].type = DATYPE_INT;
	arg.mata_data[10].name = "param0";
	arg.mata_data[10].length = 4;

	arg.mata_data[11].type = DATYPE_INT;
	arg.mata_data[11].name = "param1";
	arg.mata_data[11].length = 4;

	arg.mata_data[12].type = DATYPE_INT;
	arg.mata_data[12].name = "param2";
	arg.mata_data[12].length = 4;

	arg.mata_data[13].type = DATYPE_INT;
	arg.mata_data[13].name = "param3";
	arg.mata_data[13].length = 4;

	arg.mata_data[14].type = DATYPE_STRING;
	arg.mata_data[14].name = "sparam0";
	arg.mata_data[14].length = 32;

	return arg;
}

TableInitArg GetguildmemberTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "guildmember";
	arg.mata_data.resize(17);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idguildmember";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "guild_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "member_index";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_LONG64;
	arg.mata_data[3].name = "FOR_INDEX";
	arg.mata_data[3].length = 8;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "uid";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_INT;
	arg.mata_data[5].name = "post";
	arg.mata_data[5].length = 4;

	arg.mata_data[6].type = DATYPE_INT;
	arg.mata_data[6].name = "gongxian";
	arg.mata_data[6].length = 4;

	arg.mata_data[7].type = DATYPE_INT;
	arg.mata_data[7].name = "total_gongxian";
	arg.mata_data[7].length = 4;

	arg.mata_data[8].type = DATYPE_LONG64;
	arg.mata_data[8].name = "join_time";
	arg.mata_data[8].length = 8;

	arg.mata_data[9].type = DATYPE_LONG64;
	arg.mata_data[9].name = "last_login_time";
	arg.mata_data[9].length = 8;

	arg.mata_data[10].type = DATYPE_INT;
	arg.mata_data[10].name = "lucky_color";
	arg.mata_data[10].length = 4;

	arg.mata_data[11].type = DATYPE_INT;
	arg.mata_data[11].name = "today_guild_pao_saizi_times";
	arg.mata_data[11].length = 4;

	arg.mata_data[12].type = DATYPE_LONG64;
	arg.mata_data[12].name = "today_last_guild_pao_saizi_time";
	arg.mata_data[12].length = 8;

	arg.mata_data[13].type = DATYPE_INT;
	arg.mata_data[13].name = "today_guild_saizi_score";
	arg.mata_data[13].length = 4;

	arg.mata_data[14].type = DATYPE_INT;
	arg.mata_data[14].name = "maze_layer";
	arg.mata_data[14].length = 4;

	arg.mata_data[15].type = DATYPE_LONG64;
	arg.mata_data[15].name = "maze_complete_time";
	arg.mata_data[15].length = 8;

	arg.mata_data[16].type = DATYPE_STRING;
	arg.mata_data[16].name = "common_data";
	arg.mata_data[16].length = 256;

	return arg;
}

TableInitArg GethotkeyTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "hotkey";
	arg.mata_data.resize(6);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "hotkey_id";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "hotkey_index";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_LONG64;
	arg.mata_data[3].name = "FOR_INDEX";
	arg.mata_data[3].length = 8;

	arg.mata_data[4].type = DATYPE_CHAR;
	arg.mata_data[4].name = "hotkey_item_type";
	arg.mata_data[4].length = 1;

	arg.mata_data[5].type = DATYPE_INT;
	arg.mata_data[5].name = "hotkey_item_id";
	arg.mata_data[5].length = 4;

	return arg;
}

TableInitArg GetitemlistTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "itemlist";
	arg.mata_data.resize(10);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "iditemlist";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "itemlist_index";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_LONG64;
	arg.mata_data[3].name = "FOR_INDEX";
	arg.mata_data[3].length = 8;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "item_id";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_CHAR;
	arg.mata_data[5].name = "is_bind";
	arg.mata_data[5].length = 1;

	arg.mata_data[6].type = DATYPE_INT;
	arg.mata_data[6].name = "num";
	arg.mata_data[6].length = 4;

	arg.mata_data[7].type = DATYPE_LONG64;
	arg.mata_data[7].name = "invalid_time";
	arg.mata_data[7].length = 8;

	arg.mata_data[8].type = DATYPE_INT;
	arg.mata_data[8].name = "gold_price";
	arg.mata_data[8].length = 4;

	arg.mata_data[9].type = DATYPE_STRING;
	arg.mata_data[9].name = "param";
	arg.mata_data[9].length = 1024;

	return arg;
}

TableInitArg GetloginTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "login";
	arg.mata_data.resize(13);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "login_id";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_STRING;
	arg.mata_data[1].name = "plat_user_name";
	arg.mata_data[1].length = 64;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "db_index";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_INT;
	arg.mata_data[3].name = "role_id_1";
	arg.mata_data[3].length = 4;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "role_id_2";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_INT;
	arg.mata_data[5].name = "role_id_3";
	arg.mata_data[5].length = 4;

	arg.mata_data[6].type = DATYPE_LONG64;
	arg.mata_data[6].name = "lastlogintime";
	arg.mata_data[6].length = 8;

	arg.mata_data[7].type = DATYPE_LONG64;
	arg.mata_data[7].name = "createtime";
	arg.mata_data[7].length = 8;

	arg.mata_data[8].type = DATYPE_LONG64;
	arg.mata_data[8].name = "forbidtime";
	arg.mata_data[8].length = 8;

	arg.mata_data[9].type = DATYPE_CHAR;
	arg.mata_data[9].name = "is_anti_wallow";
	arg.mata_data[9].length = 1;

	arg.mata_data[10].type = DATYPE_LONG64;
	arg.mata_data[10].name = "forbid_time_1";
	arg.mata_data[10].length = 8;

	arg.mata_data[11].type = DATYPE_LONG64;
	arg.mata_data[11].name = "forbid_time_2";
	arg.mata_data[11].length = 8;

	arg.mata_data[12].type = DATYPE_LONG64;
	arg.mata_data[12].name = "forbid_time_3";
	arg.mata_data[12].length = 8;

	return arg;
}

TableInitArg GetmailTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "mail";
	arg.mata_data.resize(32);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idmail";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "uid";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "mail_index";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_LONG64;
	arg.mata_data[3].name = "FOR_INDEX";
	arg.mata_data[3].length = 8;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "sender_uid";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "sender_name";
	arg.mata_data[5].length = 32;

	arg.mata_data[6].type = DATYPE_LONG64;
	arg.mata_data[6].name = "recv_time";
	arg.mata_data[6].length = 8;

	arg.mata_data[7].type = DATYPE_INT;
	arg.mata_data[7].name = "kind";
	arg.mata_data[7].length = 4;

	arg.mata_data[8].type = DATYPE_INT;
	arg.mata_data[8].name = "is_read";
	arg.mata_data[8].length = 4;

	arg.mata_data[9].type = DATYPE_INT;
	arg.mata_data[9].name = "is_lock";
	arg.mata_data[9].length = 4;

	arg.mata_data[10].type = DATYPE_INT;
	arg.mata_data[10].name = "coin";
	arg.mata_data[10].length = 4;

	arg.mata_data[11].type = DATYPE_INT;
	arg.mata_data[11].name = "coin_bind";
	arg.mata_data[11].length = 4;

	arg.mata_data[12].type = DATYPE_INT;
	arg.mata_data[12].name = "gold";
	arg.mata_data[12].length = 4;

	arg.mata_data[13].type = DATYPE_INT;
	arg.mata_data[13].name = "gold_bind";
	arg.mata_data[13].length = 4;

	arg.mata_data[14].type = DATYPE_STRING;
	arg.mata_data[14].name = "subject";
	arg.mata_data[14].length = 128;

	arg.mata_data[15].type = DATYPE_STRING;
	arg.mata_data[15].name = "content";
	arg.mata_data[15].length = 512;

	arg.mata_data[16].type = DATYPE_INT;
	arg.mata_data[16].name = "item_id1";
	arg.mata_data[16].length = 4;

	arg.mata_data[17].type = DATYPE_CHAR;
	arg.mata_data[17].name = "item_is_bind1";
	arg.mata_data[17].length = 1;

	arg.mata_data[18].type = DATYPE_INT;
	arg.mata_data[18].name = "item_num1";
	arg.mata_data[18].length = 4;

	arg.mata_data[19].type = DATYPE_LONG64;
	arg.mata_data[19].name = "item_invalid_time1";
	arg.mata_data[19].length = 8;

	arg.mata_data[20].type = DATYPE_STRING;
	arg.mata_data[20].name = "item_param1";
	arg.mata_data[20].length = 1024;

	arg.mata_data[21].type = DATYPE_INT;
	arg.mata_data[21].name = "item_id2";
	arg.mata_data[21].length = 4;

	arg.mata_data[22].type = DATYPE_CHAR;
	arg.mata_data[22].name = "item_is_bind2";
	arg.mata_data[22].length = 1;

	arg.mata_data[23].type = DATYPE_INT;
	arg.mata_data[23].name = "item_num2";
	arg.mata_data[23].length = 4;

	arg.mata_data[24].type = DATYPE_LONG64;
	arg.mata_data[24].name = "item_invalid_time2";
	arg.mata_data[24].length = 8;

	arg.mata_data[25].type = DATYPE_STRING;
	arg.mata_data[25].name = "item_param2";
	arg.mata_data[25].length = 1024;

	arg.mata_data[26].type = DATYPE_INT;
	arg.mata_data[26].name = "item_id3";
	arg.mata_data[26].length = 4;

	arg.mata_data[27].type = DATYPE_CHAR;
	arg.mata_data[27].name = "item_is_bind3";
	arg.mata_data[27].length = 1;

	arg.mata_data[28].type = DATYPE_INT;
	arg.mata_data[28].name = "item_num3";
	arg.mata_data[28].length = 4;

	arg.mata_data[29].type = DATYPE_LONG64;
	arg.mata_data[29].name = "item_invalid_time3";
	arg.mata_data[29].length = 8;

	arg.mata_data[30].type = DATYPE_STRING;
	arg.mata_data[30].name = "item_param3";
	arg.mata_data[30].length = 1024;

	arg.mata_data[31].type = DATYPE_STRING;
	arg.mata_data[31].name = "virtual_item_data";
	arg.mata_data[31].length = 128;

	return arg;
}

TableInitArg GetofflineregisterTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "offlineregister";
	arg.mata_data.resize(12);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idofflineregister";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "last_divorce_time";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_INT;
	arg.mata_data[3].name = "increase_charm";
	arg.mata_data[3].length = 4;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "chongzhi_cmd_flag";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_INT;
	arg.mata_data[5].name = "red_paper_consume_gold";
	arg.mata_data[5].length = 4;

	arg.mata_data[6].type = DATYPE_INT;
	arg.mata_data[6].name = "red_paper_lucky_value";
	arg.mata_data[6].length = 4;

	arg.mata_data[7].type = DATYPE_INT;
	arg.mata_data[7].name = "space_cai_conut";
	arg.mata_data[7].length = 4;

	arg.mata_data[8].type = DATYPE_INT;
	arg.mata_data[8].name = "space_gift_count";
	arg.mata_data[8].length = 4;

	arg.mata_data[9].type = DATYPE_INT;
	arg.mata_data[9].name = "qingyuan_bless_days";
	arg.mata_data[9].length = 4;

	arg.mata_data[10].type = DATYPE_STRING;
	arg.mata_data[10].name = "guild_box_info";
	arg.mata_data[10].length = 512;

	arg.mata_data[11].type = DATYPE_STRING;
	arg.mata_data[11].name = "common_data";
	arg.mata_data[11].length = 128;

	return arg;
}

TableInitArg GetpersonrankTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "personrank";
	arg.mata_data.resize(16);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idpersonrank";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "rank_index";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "uid";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "user_name";
	arg.mata_data[3].length = 32;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "flexible_name";
	arg.mata_data[4].length = 32;

	arg.mata_data[5].type = DATYPE_INT;
	arg.mata_data[5].name = "role_sex";
	arg.mata_data[5].length = 4;

	arg.mata_data[6].type = DATYPE_INT;
	arg.mata_data[6].name = "role_prof";
	arg.mata_data[6].length = 4;

	arg.mata_data[7].type = DATYPE_INT;
	arg.mata_data[7].name = "role_camp";
	arg.mata_data[7].length = 4;

	arg.mata_data[8].type = DATYPE_INT;
	arg.mata_data[8].name = "role_vip_level";
	arg.mata_data[8].length = 4;

	arg.mata_data[9].type = DATYPE_LONG64;
	arg.mata_data[9].name = "role_exp";
	arg.mata_data[9].length = 8;

	arg.mata_data[10].type = DATYPE_INT;
	arg.mata_data[10].name = "role_level";
	arg.mata_data[10].length = 4;

	arg.mata_data[11].type = DATYPE_INT;
	arg.mata_data[11].name = "rank_type";
	arg.mata_data[11].length = 4;

	arg.mata_data[12].type = DATYPE_LONG64;
	arg.mata_data[12].name = "rank_value";
	arg.mata_data[12].length = 8;

	arg.mata_data[13].type = DATYPE_INT;
	arg.mata_data[13].name = "record_index";
	arg.mata_data[13].length = 4;

	arg.mata_data[14].type = DATYPE_INT;
	arg.mata_data[14].name = "flexible_int";
	arg.mata_data[14].length = 4;

	arg.mata_data[15].type = DATYPE_LONG64;
	arg.mata_data[15].name = "flexible_ll";
	arg.mata_data[15].length = 8;

	return arg;
}

TableInitArg GetpublicnoticeTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "publicnotice";
	arg.mata_data.resize(12);

	arg.mata_data[0].type = DATYPE_INT;
	arg.mata_data[0].name = "idpublicnotice";
	arg.mata_data[0].length = 4;

	arg.mata_data[1].type = DATYPE_STRING;
	arg.mata_data[1].name = "creator";
	arg.mata_data[1].length = 64;

	arg.mata_data[2].type = DATYPE_LONG64;
	arg.mata_data[2].name = "createtime";
	arg.mata_data[2].length = 8;

	arg.mata_data[3].type = DATYPE_LONG64;
	arg.mata_data[3].name = "begin_send_time";
	arg.mata_data[3].length = 8;

	arg.mata_data[4].type = DATYPE_LONG64;
	arg.mata_data[4].name = "send_interval";
	arg.mata_data[4].length = 8;

	arg.mata_data[5].type = DATYPE_LONG64;
	arg.mata_data[5].name = "end_send_time";
	arg.mata_data[5].length = 8;

	arg.mata_data[6].type = DATYPE_LONG64;
	arg.mata_data[6].name = "next_send_time";
	arg.mata_data[6].length = 8;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "content";
	arg.mata_data[7].length = 512;

	arg.mata_data[8].type = DATYPE_INT;
	arg.mata_data[8].name = "type";
	arg.mata_data[8].length = 4;

	arg.mata_data[9].type = DATYPE_INT;
	arg.mata_data[9].name = "color";
	arg.mata_data[9].length = 4;

	arg.mata_data[10].type = DATYPE_INT;
	arg.mata_data[10].name = "is_forbid";
	arg.mata_data[10].length = 4;

	arg.mata_data[11].type = DATYPE_INT;
	arg.mata_data[11].name = "is_urgent";
	arg.mata_data[11].length = 4;

	return arg;
}

TableInitArg GetpublicsaleTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "publicsale";
	arg.mata_data.resize(21);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idpublicsale";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "seller_uid";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "seller_name";
	arg.mata_data[2].length = 32;

	arg.mata_data[3].type = DATYPE_INT;
	arg.mata_data[3].name = "sale_index";
	arg.mata_data[3].length = 4;

	arg.mata_data[4].type = DATYPE_LONG64;
	arg.mata_data[4].name = "FOR_INDEX";
	arg.mata_data[4].length = 8;

	arg.mata_data[5].type = DATYPE_INT;
	arg.mata_data[5].name = "item_id";
	arg.mata_data[5].length = 4;

	arg.mata_data[6].type = DATYPE_CHAR;
	arg.mata_data[6].name = "item_is_bind";
	arg.mata_data[6].length = 1;

	arg.mata_data[7].type = DATYPE_INT;
	arg.mata_data[7].name = "item_sale_type";
	arg.mata_data[7].length = 4;

	arg.mata_data[8].type = DATYPE_INT;
	arg.mata_data[8].name = "item_num";
	arg.mata_data[8].length = 4;

	arg.mata_data[9].type = DATYPE_INT;
	arg.mata_data[9].name = "color";
	arg.mata_data[9].length = 4;

	arg.mata_data[10].type = DATYPE_INT;
	arg.mata_data[10].name = "level";
	arg.mata_data[10].length = 4;

	arg.mata_data[11].type = DATYPE_INT;
	arg.mata_data[11].name = "prof";
	arg.mata_data[11].length = 4;

	arg.mata_data[12].type = DATYPE_INT;
	arg.mata_data[12].name = "equip_order";
	arg.mata_data[12].length = 4;

	arg.mata_data[13].type = DATYPE_INT;
	arg.mata_data[13].name = "gold_price";
	arg.mata_data[13].length = 4;

	arg.mata_data[14].type = DATYPE_INT;
	arg.mata_data[14].name = "sale_value";
	arg.mata_data[14].length = 4;

	arg.mata_data[15].type = DATYPE_INT;
	arg.mata_data[15].name = "price_type";
	arg.mata_data[15].length = 4;

	arg.mata_data[16].type = DATYPE_INT;
	arg.mata_data[16].name = "sale_item_type";
	arg.mata_data[16].length = 4;

	arg.mata_data[17].type = DATYPE_LONG64;
	arg.mata_data[17].name = "sale_time";
	arg.mata_data[17].length = 8;

	arg.mata_data[18].type = DATYPE_LONG64;
	arg.mata_data[18].name = "due_time";
	arg.mata_data[18].length = 8;

	arg.mata_data[19].type = DATYPE_LONG64;
	arg.mata_data[19].name = "item_invalid_time";
	arg.mata_data[19].length = 8;

	arg.mata_data[20].type = DATYPE_STRING;
	arg.mata_data[20].name = "item_param";
	arg.mata_data[20].length = 1024;

	return arg;
}

TableInitArg GetroleTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "role";
	arg.mata_data.resize(17);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "role_inc_id";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "role_name";
	arg.mata_data[2].length = 32;

	arg.mata_data[3].type = DATYPE_CHAR;
	arg.mata_data[3].name = "avatar";
	arg.mata_data[3].length = 1;

	arg.mata_data[4].type = DATYPE_CHAR;
	arg.mata_data[4].name = "sex";
	arg.mata_data[4].length = 1;

	arg.mata_data[5].type = DATYPE_INT;
	arg.mata_data[5].name = "scene_id";
	arg.mata_data[5].length = 4;

	arg.mata_data[6].type = DATYPE_INT;
	arg.mata_data[6].name = "last_scene_id";
	arg.mata_data[6].length = 4;

	arg.mata_data[7].type = DATYPE_INT;
	arg.mata_data[7].name = "level";
	arg.mata_data[7].length = 4;

	arg.mata_data[8].type = DATYPE_INT;
	arg.mata_data[8].name = "professional";
	arg.mata_data[8].length = 4;

	arg.mata_data[9].type = DATYPE_LONG64;
	arg.mata_data[9].name = "create_time";
	arg.mata_data[9].length = 8;

	arg.mata_data[10].type = DATYPE_INT;
	arg.mata_data[10].name = "online_time";
	arg.mata_data[10].length = 4;

	arg.mata_data[11].type = DATYPE_CHAR;
	arg.mata_data[11].name = "is_online";
	arg.mata_data[11].length = 1;

	arg.mata_data[12].type = DATYPE_LONG64;
	arg.mata_data[12].name = "last_save_time";
	arg.mata_data[12].length = 8;

	arg.mata_data[13].type = DATYPE_CHAR;
	arg.mata_data[13].name = "country";
	arg.mata_data[13].length = 1;

	arg.mata_data[14].type = DATYPE_CHAR;
	arg.mata_data[14].name = "is_chongzhi";
	arg.mata_data[14].length = 1;

	arg.mata_data[15].type = DATYPE_CHAR;
	arg.mata_data[15].name = "is_micro_pc";
	arg.mata_data[15].length = 1;

	arg.mata_data[16].type = DATYPE_INT;
	arg.mata_data[16].name = "plat_spid";
	arg.mata_data[16].length = 4;

	return arg;
}

TableInitArg Getrole_attr_detailTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "role_attr_detail";
	arg.mata_data.resize(66);

	arg.mata_data[0].type = DATYPE_INT;
	arg.mata_data[0].name = "role_attr_detail_id";
	arg.mata_data[0].length = 4;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "attr_data";
	arg.mata_data[2].length = 4096;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "role_name";
	arg.mata_data[3].length = 32;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "level";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_CHAR;
	arg.mata_data[5].name = "sex";
	arg.mata_data[5].length = 1;

	arg.mata_data[6].type = DATYPE_CHAR;
	arg.mata_data[6].name = "prof";
	arg.mata_data[6].length = 1;

	arg.mata_data[7].type = DATYPE_CHAR;
	arg.mata_data[7].name = "camp";
	arg.mata_data[7].length = 1;

	arg.mata_data[8].type = DATYPE_LONG64;
	arg.mata_data[8].name = "gold";
	arg.mata_data[8].length = 8;

	arg.mata_data[9].type = DATYPE_LONG64;
	arg.mata_data[9].name = "gold_bind";
	arg.mata_data[9].length = 8;

	arg.mata_data[10].type = DATYPE_LONG64;
	arg.mata_data[10].name = "coin";
	arg.mata_data[10].length = 8;

	arg.mata_data[11].type = DATYPE_LONG64;
	arg.mata_data[11].name = "coin_bind";
	arg.mata_data[11].length = 8;

	arg.mata_data[12].type = DATYPE_INT;
	arg.mata_data[12].name = "guild_id";
	arg.mata_data[12].length = 4;

	arg.mata_data[13].type = DATYPE_CHAR;
	arg.mata_data[13].name = "avatar";
	arg.mata_data[13].length = 1;

	arg.mata_data[14].type = DATYPE_LONG64;
	arg.mata_data[14].name = "avatar_timestamp";
	arg.mata_data[14].length = 8;

	arg.mata_data[15].type = DATYPE_CHAR;
	arg.mata_data[15].name = "vip_type";
	arg.mata_data[15].length = 1;

	arg.mata_data[16].type = DATYPE_INT;
	arg.mata_data[16].name = "evil";
	arg.mata_data[16].length = 4;

	arg.mata_data[17].type = DATYPE_LONG64;
	arg.mata_data[17].name = "hp";
	arg.mata_data[17].length = 8;

	arg.mata_data[18].type = DATYPE_LONG64;
	arg.mata_data[18].name = "max_hp";
	arg.mata_data[18].length = 8;

	arg.mata_data[19].type = DATYPE_LONG64;
	arg.mata_data[19].name = "mp";
	arg.mata_data[19].length = 8;

	arg.mata_data[20].type = DATYPE_LONG64;
	arg.mata_data[20].name = "max_mp";
	arg.mata_data[20].length = 8;

	arg.mata_data[21].type = DATYPE_LONG64;
	arg.mata_data[21].name = "gongji";
	arg.mata_data[21].length = 8;

	arg.mata_data[22].type = DATYPE_LONG64;
	arg.mata_data[22].name = "fangyu";
	arg.mata_data[22].length = 8;

	arg.mata_data[23].type = DATYPE_LONG64;
	arg.mata_data[23].name = "mingzhong";
	arg.mata_data[23].length = 8;

	arg.mata_data[24].type = DATYPE_LONG64;
	arg.mata_data[24].name = "shanbi";
	arg.mata_data[24].length = 8;

	arg.mata_data[25].type = DATYPE_LONG64;
	arg.mata_data[25].name = "baoji";
	arg.mata_data[25].length = 8;

	arg.mata_data[26].type = DATYPE_LONG64;
	arg.mata_data[26].name = "jianren";
	arg.mata_data[26].length = 8;

	arg.mata_data[27].type = DATYPE_LONG64;
	arg.mata_data[27].name = "fujia_shanghai";
	arg.mata_data[27].length = 8;

	arg.mata_data[28].type = DATYPE_LONG64;
	arg.mata_data[28].name = "dikang_shanghai";
	arg.mata_data[28].length = 8;

	arg.mata_data[29].type = DATYPE_LONG64;
	arg.mata_data[29].name = "per_jingzhun";
	arg.mata_data[29].length = 8;

	arg.mata_data[30].type = DATYPE_LONG64;
	arg.mata_data[30].name = "per_baoji";
	arg.mata_data[30].length = 8;

	arg.mata_data[31].type = DATYPE_LONG64;
	arg.mata_data[31].name = "per_kangbao";
	arg.mata_data[31].length = 8;

	arg.mata_data[32].type = DATYPE_LONG64;
	arg.mata_data[32].name = "per_pofang";
	arg.mata_data[32].length = 8;

	arg.mata_data[33].type = DATYPE_LONG64;
	arg.mata_data[33].name = "per_mianshang";
	arg.mata_data[33].length = 8;

	arg.mata_data[34].type = DATYPE_INT;
	arg.mata_data[34].name = "capability";
	arg.mata_data[34].length = 4;

	arg.mata_data[35].type = DATYPE_INT;
	arg.mata_data[35].name = "max_capability";
	arg.mata_data[35].length = 4;

	arg.mata_data[36].type = DATYPE_INT;
	arg.mata_data[36].name = "lover_uid";
	arg.mata_data[36].length = 4;

	arg.mata_data[37].type = DATYPE_STRING;
	arg.mata_data[37].name = "lover_name";
	arg.mata_data[37].length = 32;

	arg.mata_data[38].type = DATYPE_CHAR;
	arg.mata_data[38].name = "authority_type";
	arg.mata_data[38].length = 1;

	arg.mata_data[39].type = DATYPE_LONG64;
	arg.mata_data[39].name = "forbid_talk_time";
	arg.mata_data[39].length = 8;

	arg.mata_data[40].type = DATYPE_STRING;
	arg.mata_data[40].name = "sublock_pw";
	arg.mata_data[40].length = 64;

	arg.mata_data[41].type = DATYPE_INT;
	arg.mata_data[41].name = "rolechest_data_len";
	arg.mata_data[41].length = 4;

	arg.mata_data[42].type = DATYPE_STRING;
	arg.mata_data[42].name = "mentality_data";
	arg.mata_data[42].length = 512;

	arg.mata_data[43].type = DATYPE_STRING;
	arg.mata_data[43].name = "vip_data";
	arg.mata_data[43].length = 128;

	arg.mata_data[44].type = DATYPE_STRING;
	arg.mata_data[44].name = "active_degree_data";
	arg.mata_data[44].length = 256;

	arg.mata_data[45].type = DATYPE_STRING;
	arg.mata_data[45].name = "roleshop_data";
	arg.mata_data[45].length = 256;

	arg.mata_data[46].type = DATYPE_STRING;
	arg.mata_data[46].name = "welfare_data";
	arg.mata_data[46].length = 512;

	arg.mata_data[47].type = DATYPE_STRING;
	arg.mata_data[47].name = "rolehangup_data";
	arg.mata_data[47].length = 256;

	arg.mata_data[48].type = DATYPE_STRING;
	arg.mata_data[48].name = "rolecustom_data";
	arg.mata_data[48].length = 128;

	arg.mata_data[49].type = DATYPE_STRING;
	arg.mata_data[49].name = "itemcolddown_data";
	arg.mata_data[49].length = 256;

	arg.mata_data[50].type = DATYPE_STRING;
	arg.mata_data[50].name = "evaluate_data";
	arg.mata_data[50].length = 128;

	arg.mata_data[51].type = DATYPE_STRING;
	arg.mata_data[51].name = "monitor_data";
	arg.mata_data[51].length = 256;

	arg.mata_data[52].type = DATYPE_STRING;
	arg.mata_data[52].name = "title_data";
	arg.mata_data[52].length = 5120;

	arg.mata_data[53].type = DATYPE_STRING;
	arg.mata_data[53].name = "husongtask_data";
	arg.mata_data[53].length = 64;

	arg.mata_data[54].type = DATYPE_STRING;
	arg.mata_data[54].name = "roleactivity_data";
	arg.mata_data[54].length = 10240;

	arg.mata_data[55].type = DATYPE_STRING;
	arg.mata_data[55].name = "fbtowerdefend_data";
	arg.mata_data[55].length = 64;

	arg.mata_data[56].type = DATYPE_LONG64;
	arg.mata_data[56].name = "last_single_chat_monitor_time";
	arg.mata_data[56].length = 8;

	arg.mata_data[57].type = DATYPE_STRING;
	arg.mata_data[57].name = "last_single_chat_monitor_msg";
	arg.mata_data[57].length = 512;

	arg.mata_data[58].type = DATYPE_LONG64;
	arg.mata_data[58].name = "last_world_chat_monitor_time";
	arg.mata_data[58].length = 8;

	arg.mata_data[59].type = DATYPE_STRING;
	arg.mata_data[59].name = "last_world_chat_monitor_msg";
	arg.mata_data[59].length = 128;

	arg.mata_data[60].type = DATYPE_LONG64;
	arg.mata_data[60].name = "last_mail_monitor_time";
	arg.mata_data[60].length = 8;

	arg.mata_data[61].type = DATYPE_STRING;
	arg.mata_data[61].name = "last_mail_monitor_subject";
	arg.mata_data[61].length = 128;

	arg.mata_data[62].type = DATYPE_INT;
	arg.mata_data[62].name = "today_online_time";
	arg.mata_data[62].length = 4;

	arg.mata_data[63].type = DATYPE_INT;
	arg.mata_data[63].name = "lastday_online_time";
	arg.mata_data[63].length = 4;

	arg.mata_data[64].type = DATYPE_STRING;
	arg.mata_data[64].name = "common_data";
	arg.mata_data[64].length = 4096;

	arg.mata_data[65].type = DATYPE_STRING;
	arg.mata_data[65].name = "wing_data";
	arg.mata_data[65].length = 512;

	return arg;
}

TableInitArg Getrole_attr_detail10TableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "role_attr_detail10";
	arg.mata_data.resize(11);

	arg.mata_data[0].type = DATYPE_INT;
	arg.mata_data[0].name = "role_attr_detail10_id";
	arg.mata_data[0].length = 4;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "orange_equip_collect_data";
	arg.mata_data[2].length = 52000;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "yaoshi_data";
	arg.mata_data[3].length = 512;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "toushi_data";
	arg.mata_data[4].length = 512;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "qilinbi_data";
	arg.mata_data[5].length = 512;

	arg.mata_data[6].type = DATYPE_STRING;
	arg.mata_data[6].name = "mask_data";
	arg.mata_data[6].length = 512;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "jinjiesys_reward_data";
	arg.mata_data[7].length = 1024;

	arg.mata_data[8].type = DATYPE_STRING;
	arg.mata_data[8].name = "role_big_small_goal_data";
	arg.mata_data[8].length = 1024;

	arg.mata_data[9].type = DATYPE_INT;
	arg.mata_data[9].name = "data_page_len";
	arg.mata_data[9].length = 4;

	arg.mata_data[10].type = DATYPE_STRING;
	arg.mata_data[10].name = "data_page";
	arg.mata_data[10].length = 2097153;

	return arg;
}

TableInitArg Getrole_attr_detail2TableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "role_attr_detail2";
	arg.mata_data.resize(16);

	arg.mata_data[0].type = DATYPE_INT;
	arg.mata_data[0].name = "role_attr_detail2_id";
	arg.mata_data[0].length = 4;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "rolechest_data";
	arg.mata_data[2].length = 25800;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "effect_data";
	arg.mata_data[3].length = 8192;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "equipfb_data";
	arg.mata_data[4].length = 512;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "guild_task_data";
	arg.mata_data[5].length = 128;

	arg.mata_data[6].type = DATYPE_STRING;
	arg.mata_data[6].name = "newequipfb_data";
	arg.mata_data[6].length = 2048;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "stone_data";
	arg.mata_data[7].length = 2048;

	arg.mata_data[8].type = DATYPE_STRING;
	arg.mata_data[8].name = "guild_xianshu_data";
	arg.mata_data[8].length = 16;

	arg.mata_data[9].type = DATYPE_STRING;
	arg.mata_data[9].name = "taskrecord_data";
	arg.mata_data[9].length = 4096;

	arg.mata_data[10].type = DATYPE_STRING;
	arg.mata_data[10].name = "taskinfo_data";
	arg.mata_data[10].length = 2048;

	arg.mata_data[11].type = DATYPE_STRING;
	arg.mata_data[11].name = "role_supply_data";
	arg.mata_data[11].length = 128;

	arg.mata_data[12].type = DATYPE_STRING;
	arg.mata_data[12].name = "maze_data";
	arg.mata_data[12].length = 128;

	arg.mata_data[13].type = DATYPE_STRING;
	arg.mata_data[13].name = "xiannv_data";
	arg.mata_data[13].length = 2048;

	arg.mata_data[14].type = DATYPE_STRING;
	arg.mata_data[14].name = "story_fb_data";
	arg.mata_data[14].length = 2048;

	arg.mata_data[15].type = DATYPE_STRING;
	arg.mata_data[15].name = "convert_record_data";
	arg.mata_data[15].length = 4096;

	return arg;
}

TableInitArg Getrole_attr_detail3TableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "role_attr_detail3";
	arg.mata_data.resize(19);

	arg.mata_data[0].type = DATYPE_INT;
	arg.mata_data[0].name = "role_attr_detail3_id";
	arg.mata_data[0].length = 4;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "other_module_data";
	arg.mata_data[2].length = 1024;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "qingyuan_data";
	arg.mata_data[3].length = 4096;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "role_goal_data";
	arg.mata_data[4].length = 768;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "history_cap_data";
	arg.mata_data[5].length = 256;

	arg.mata_data[6].type = DATYPE_STRING;
	arg.mata_data[6].name = "skill_other_data";
	arg.mata_data[6].length = 64;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "role_guild_data";
	arg.mata_data[7].length = 2048;

	arg.mata_data[8].type = DATYPE_STRING;
	arg.mata_data[8].name = "shengwang_data";
	arg.mata_data[8].length = 64;

	arg.mata_data[9].type = DATYPE_STRING;
	arg.mata_data[9].name = "chengjiu_data";
	arg.mata_data[9].length = 8192;

	arg.mata_data[10].type = DATYPE_STRING;
	arg.mata_data[10].name = "dailyfb_data";
	arg.mata_data[10].length = 512;

	arg.mata_data[11].type = DATYPE_STRING;
	arg.mata_data[11].name = "lifeskill_data";
	arg.mata_data[11].length = 128;

	arg.mata_data[12].type = DATYPE_STRING;
	arg.mata_data[12].name = "baby_data";
	arg.mata_data[12].length = 2048;

	arg.mata_data[13].type = DATYPE_STRING;
	arg.mata_data[13].name = "shenge_system_data";
	arg.mata_data[13].length = 8192;

	arg.mata_data[14].type = DATYPE_STRING;
	arg.mata_data[14].name = "zero_gift_data";
	arg.mata_data[14].length = 256;

	arg.mata_data[15].type = DATYPE_STRING;
	arg.mata_data[15].name = "mysterious_shop_in_mall_data";
	arg.mata_data[15].length = 256;

	arg.mata_data[16].type = DATYPE_STRING;
	arg.mata_data[16].name = "friend_exp_bottle_data";
	arg.mata_data[16].length = 64;

	arg.mata_data[17].type = DATYPE_STRING;
	arg.mata_data[17].name = "shenshou";
	arg.mata_data[17].length = 15360;

	arg.mata_data[18].type = DATYPE_STRING;
	arg.mata_data[18].name = "img_fuling_data";
	arg.mata_data[18].length = 512;

	return arg;
}

TableInitArg Getrole_attr_detail4TableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "role_attr_detail4";
	arg.mata_data.resize(17);

	arg.mata_data[0].type = DATYPE_INT;
	arg.mata_data[0].name = "role_attr_detail4_id";
	arg.mata_data[0].length = 4;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "tuhaojin_data";
	arg.mata_data[2].length = 64;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "bigchatface_data";
	arg.mata_data[3].length = 64;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "pet_data";
	arg.mata_data[4].length = 4096;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "phase_fb_data";
	arg.mata_data[5].length = 2048;

	arg.mata_data[6].type = DATYPE_STRING;
	arg.mata_data[6].name = "vip_fb_data";
	arg.mata_data[6].length = 64;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "huashen_data";
	arg.mata_data[7].length = 1024;

	arg.mata_data[8].type = DATYPE_STRING;
	arg.mata_data[8].name = "magic_card_data";
	arg.mata_data[8].length = 512;

	arg.mata_data[9].type = DATYPE_STRING;
	arg.mata_data[9].name = "mitama_data";
	arg.mata_data[9].length = 512;

	arg.mata_data[10].type = DATYPE_STRING;
	arg.mata_data[10].name = "equipment_data";
	arg.mata_data[10].length = 512;

	arg.mata_data[11].type = DATYPE_STRING;
	arg.mata_data[11].name = "chinese_zodiac_data";
	arg.mata_data[11].length = 9216;

	arg.mata_data[12].type = DATYPE_STRING;
	arg.mata_data[12].name = "friend_gift";
	arg.mata_data[12].length = 2048;

	arg.mata_data[13].type = DATYPE_STRING;
	arg.mata_data[13].name = "follow_boss";
	arg.mata_data[13].length = 2048;

	arg.mata_data[14].type = DATYPE_STRING;
	arg.mata_data[14].name = "gold_vip";
	arg.mata_data[14].length = 128;

	arg.mata_data[15].type = DATYPE_STRING;
	arg.mata_data[15].name = "rune_system";
	arg.mata_data[15].length = 8192;

	arg.mata_data[16].type = DATYPE_STRING;
	arg.mata_data[16].name = "magical_precious_data";
	arg.mata_data[16].length = 2048;

	return arg;
}

TableInitArg Getrole_attr_detail5TableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "role_attr_detail5";
	arg.mata_data.resize(19);

	arg.mata_data[0].type = DATYPE_INT;
	arg.mata_data[0].name = "role_attr_detail5_id";
	arg.mata_data[0].length = 4;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "multi_mount_data";
	arg.mata_data[2].length = 1024;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "personalize_window_data";
	arg.mata_data[3].length = 768;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "xunzhang";
	arg.mata_data[4].length = 128;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "zhibao";
	arg.mata_data[5].length = 256;

	arg.mata_data[6].type = DATYPE_STRING;
	arg.mata_data[6].name = "jingling";
	arg.mata_data[6].length = 3072;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "xiannv_shouhu";
	arg.mata_data[7].length = 512;

	arg.mata_data[8].type = DATYPE_STRING;
	arg.mata_data[8].name = "jingling_fazhen";
	arg.mata_data[8].length = 512;

	arg.mata_data[9].type = DATYPE_STRING;
	arg.mata_data[9].name = "jingling_guanghuan";
	arg.mata_data[9].length = 512;

	arg.mata_data[10].type = DATYPE_STRING;
	arg.mata_data[10].name = "shenzhou_weapon_data";
	arg.mata_data[10].length = 8192;

	arg.mata_data[11].type = DATYPE_STRING;
	arg.mata_data[11].name = "lieming";
	arg.mata_data[11].length = 512;

	arg.mata_data[12].type = DATYPE_STRING;
	arg.mata_data[12].name = "lieming_bag";
	arg.mata_data[12].length = 1024;

	arg.mata_data[13].type = DATYPE_STRING;
	arg.mata_data[13].name = "cardzu";
	arg.mata_data[13].length = 2048;

	arg.mata_data[14].type = DATYPE_STRING;
	arg.mata_data[14].name = "zhuansheng_equip";
	arg.mata_data[14].length = 2048;

	arg.mata_data[15].type = DATYPE_STRING;
	arg.mata_data[15].name = "shenbing";
	arg.mata_data[15].length = 256;

	arg.mata_data[16].type = DATYPE_STRING;
	arg.mata_data[16].name = "tuitufb_data";
	arg.mata_data[16].length = 20480;

	arg.mata_data[17].type = DATYPE_STRING;
	arg.mata_data[17].name = "cloak";
	arg.mata_data[17].length = 512;

	arg.mata_data[18].type = DATYPE_STRING;
	arg.mata_data[18].name = "role_precious_boss";
	arg.mata_data[18].length = 64;

	return arg;
}

TableInitArg Getrole_attr_detail6TableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "role_attr_detail6";
	arg.mata_data.resize(15);

	arg.mata_data[0].type = DATYPE_INT;
	arg.mata_data[0].name = "role_attr_detail6_id";
	arg.mata_data[0].length = 4;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "couple_mount_data";
	arg.mata_data[2].length = 2560;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "shenyin_data";
	arg.mata_data[3].length = 5120;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "element_heart_data";
	arg.mata_data[4].length = 2048;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "shengxingzhuli";
	arg.mata_data[5].length = 32;

	arg.mata_data[6].type = DATYPE_STRING;
	arg.mata_data[6].name = "shizhuang_data";
	arg.mata_data[6].length = 4096;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "fabao_data";
	arg.mata_data[7].length = 512;

	arg.mata_data[8].type = DATYPE_STRING;
	arg.mata_data[8].name = "feixian_data";
	arg.mata_data[8].length = 2048;

	arg.mata_data[9].type = DATYPE_STRING;
	arg.mata_data[9].name = "talent_data";
	arg.mata_data[9].length = 256;

	arg.mata_data[10].type = DATYPE_STRING;
	arg.mata_data[10].name = "jinjie_talent_data";
	arg.mata_data[10].length = 1024;

	arg.mata_data[11].type = DATYPE_STRING;
	arg.mata_data[11].name = "armor_fb_data";
	arg.mata_data[11].length = 64;

	arg.mata_data[12].type = DATYPE_STRING;
	arg.mata_data[12].name = "bosscard_data";
	arg.mata_data[12].length = 512;

	arg.mata_data[13].type = DATYPE_STRING;
	arg.mata_data[13].name = "task_zhuanzhi_data";
	arg.mata_data[13].length = 64;

	arg.mata_data[14].type = DATYPE_STRING;
	arg.mata_data[14].name = "discount_buy_data";
	arg.mata_data[14].length = 20480;

	return arg;
}

TableInitArg Getrole_attr_detail7TableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "role_attr_detail7";
	arg.mata_data.resize(12);

	arg.mata_data[0].type = DATYPE_INT;
	arg.mata_data[0].name = "role_attr_detail7_id";
	arg.mata_data[0].length = 4;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "cross_global_data";
	arg.mata_data[2].length = 1024;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "zhuanzhi_equip_data";
	arg.mata_data[3].length = 8192;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "role_cross_data";
	arg.mata_data[4].length = 2048;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "heshenluoshu_data";
	arg.mata_data[5].length = 4096;

	arg.mata_data[6].type = DATYPE_STRING;
	arg.mata_data[6].name = "shengqi_data";
	arg.mata_data[6].length = 2048;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "seal_data";
	arg.mata_data[7].length = 512;

	arg.mata_data[8].type = DATYPE_STRING;
	arg.mata_data[8].name = "dressing_room_data";
	arg.mata_data[8].length = 1024;

	arg.mata_data[9].type = DATYPE_STRING;
	arg.mata_data[9].name = "challenge_fb_data";
	arg.mata_data[9].length = 768;

	arg.mata_data[10].type = DATYPE_STRING;
	arg.mata_data[10].name = "role_cross_rand_activity_data";
	arg.mata_data[10].length = 1024;

	arg.mata_data[11].type = DATYPE_STRING;
	arg.mata_data[11].name = "cross_goal_data";
	arg.mata_data[11].length = 512;

	return arg;
}

TableInitArg Getrole_attr_detail8TableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "role_attr_detail8";
	arg.mata_data.resize(16);

	arg.mata_data[0].type = DATYPE_INT;
	arg.mata_data[0].name = "role_attr_detail8_id";
	arg.mata_data[0].length = 4;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "upgrade_manager_data";
	arg.mata_data[2].length = 15360;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "shenqi_data";
	arg.mata_data[3].length = 3264;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "minghun2_data";
	arg.mata_data[4].length = 512;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "person_profess_data";
	arg.mata_data[5].length = 4096;

	arg.mata_data[6].type = DATYPE_STRING;
	arg.mata_data[6].name = "xianzunka_data";
	arg.mata_data[6].length = 128;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "equip_baptize_data";
	arg.mata_data[7].length = 1024;

	arg.mata_data[8].type = DATYPE_STRING;
	arg.mata_data[8].name = "mojie";
	arg.mata_data[8].length = 128;

	arg.mata_data[9].type = DATYPE_STRING;
	arg.mata_data[9].name = "fb_personboss_data";
	arg.mata_data[9].length = 128;

	arg.mata_data[10].type = DATYPE_STRING;
	arg.mata_data[10].name = "greate_soldier_data";
	arg.mata_data[10].length = 4096;

	arg.mata_data[11].type = DATYPE_STRING;
	arg.mata_data[11].name = "lottery_count_data";
	arg.mata_data[11].length = 2048;

	arg.mata_data[12].type = DATYPE_STRING;
	arg.mata_data[12].name = "upgrade_card_buy_data";
	arg.mata_data[12].length = 256;

	arg.mata_data[13].type = DATYPE_STRING;
	arg.mata_data[13].name = "myth_data";
	arg.mata_data[13].length = 10240;

	arg.mata_data[14].type = DATYPE_STRING;
	arg.mata_data[14].name = "zodiac_data";
	arg.mata_data[14].length = 4096;

	arg.mata_data[15].type = DATYPE_STRING;
	arg.mata_data[15].name = "cross_equip_data";
	arg.mata_data[15].length = 3072;

	return arg;
}

TableInitArg Getrole_attr_detail9TableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "role_attr_detail9";
	arg.mata_data.resize(16);

	arg.mata_data[0].type = DATYPE_INT;
	arg.mata_data[0].name = "role_attr_detail9_id";
	arg.mata_data[0].length = 4;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "red_equip_collect_data";
	arg.mata_data[2].length = 52000;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "mount_data";
	arg.mata_data[3].length = 768;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "footprint";
	arg.mata_data[4].length = 768;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "halo";
	arg.mata_data[5].length = 768;

	arg.mata_data[6].type = DATYPE_STRING;
	arg.mata_data[6].name = "huguozhili";
	arg.mata_data[6].length = 256;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "baizhan_equip_data";
	arg.mata_data[7].length = 4096;

	arg.mata_data[8].type = DATYPE_STRING;
	arg.mata_data[8].name = "fight_mount_data";
	arg.mata_data[8].length = 512;

	arg.mata_data[9].type = DATYPE_STRING;
	arg.mata_data[9].name = "mojiefb_data";
	arg.mata_data[9].length = 128;

	arg.mata_data[10].type = DATYPE_STRING;
	arg.mata_data[10].name = "equipsuit_data";
	arg.mata_data[10].length = 64;

	arg.mata_data[11].type = DATYPE_STRING;
	arg.mata_data[11].name = "shenzhuang";
	arg.mata_data[11].length = 256;

	arg.mata_data[12].type = DATYPE_STRING;
	arg.mata_data[12].name = "csa_equip_data";
	arg.mata_data[12].length = 128;

	arg.mata_data[13].type = DATYPE_STRING;
	arg.mata_data[13].name = "shen_gong";
	arg.mata_data[13].length = 512;

	arg.mata_data[14].type = DATYPE_STRING;
	arg.mata_data[14].name = "shen_yi";
	arg.mata_data[14].length = 512;

	arg.mata_data[15].type = DATYPE_STRING;
	arg.mata_data[15].name = "discountshop_data";
	arg.mata_data[15].length = 1024;

	return arg;
}

TableInitArg Getrole_id_mapTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "role_id_map";
	arg.mata_data.resize(2);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idrole_id_map";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "is_use";
	arg.mata_data[1].length = 4;

	return arg;
}

TableInitArg Getrole_name_mapTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "role_name_map";
	arg.mata_data.resize(5);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idrole_name_map";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "db_index";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "role_id";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "role_name";
	arg.mata_data[3].length = 32;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "plat_user_name";
	arg.mata_data[4].length = 64;

	return arg;
}

TableInitArg GetroleskillTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "roleskill";
	arg.mata_data.resize(8);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idroleskill";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "skill_index";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_LONG64;
	arg.mata_data[3].name = "FOR_INDEX";
	arg.mata_data[3].length = 8;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "skill_level";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_INT;
	arg.mata_data[5].name = "skill_id";
	arg.mata_data[5].length = 4;

	arg.mata_data[6].type = DATYPE_LONG64;
	arg.mata_data[6].name = "last_perform_time";
	arg.mata_data[6].length = 8;

	arg.mata_data[7].type = DATYPE_INT;
	arg.mata_data[7].name = "skill_exp";
	arg.mata_data[7].length = 4;

	return arg;
}

TableInitArg GetspacerecordTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "spacerecord";
	arg.mata_data.resize(10);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "id_record";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_LONG64;
	arg.mata_data[1].name = "record_unq_id";
	arg.mata_data[1].length = 8;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "owner_id";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_INT;
	arg.mata_data[3].name = "from_role_id";
	arg.mata_data[3].length = 4;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "to_role_id";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_LONG64;
	arg.mata_data[5].name = "happen_timestamp";
	arg.mata_data[5].length = 8;

	arg.mata_data[6].type = DATYPE_INT;
	arg.mata_data[6].name = "happen_space_uid";
	arg.mata_data[6].length = 4;

	arg.mata_data[7].type = DATYPE_INT;
	arg.mata_data[7].name = "type";
	arg.mata_data[7].length = 4;

	arg.mata_data[8].type = DATYPE_INT;
	arg.mata_data[8].name = "is_huifu";
	arg.mata_data[8].length = 4;

	arg.mata_data[9].type = DATYPE_STRING;
	arg.mata_data[9].name = "msg";
	arg.mata_data[9].length = 64;

	return arg;
}

TableInitArg GetsystemmailTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "systemmail";
	arg.mata_data.resize(28);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "systemmail_id";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "uid";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_LONG64;
	arg.mata_data[2].name = "recv_time";
	arg.mata_data[2].length = 8;

	arg.mata_data[3].type = DATYPE_INT;
	arg.mata_data[3].name = "kind";
	arg.mata_data[3].length = 4;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "is_read";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_INT;
	arg.mata_data[5].name = "is_lock";
	arg.mata_data[5].length = 4;

	arg.mata_data[6].type = DATYPE_INT;
	arg.mata_data[6].name = "coin";
	arg.mata_data[6].length = 4;

	arg.mata_data[7].type = DATYPE_INT;
	arg.mata_data[7].name = "coin_bind";
	arg.mata_data[7].length = 4;

	arg.mata_data[8].type = DATYPE_INT;
	arg.mata_data[8].name = "gold";
	arg.mata_data[8].length = 4;

	arg.mata_data[9].type = DATYPE_INT;
	arg.mata_data[9].name = "gold_bind";
	arg.mata_data[9].length = 4;

	arg.mata_data[10].type = DATYPE_STRING;
	arg.mata_data[10].name = "subject";
	arg.mata_data[10].length = 128;

	arg.mata_data[11].type = DATYPE_STRING;
	arg.mata_data[11].name = "content";
	arg.mata_data[11].length = 512;

	arg.mata_data[12].type = DATYPE_INT;
	arg.mata_data[12].name = "item_id1";
	arg.mata_data[12].length = 4;

	arg.mata_data[13].type = DATYPE_CHAR;
	arg.mata_data[13].name = "item_is_bind1";
	arg.mata_data[13].length = 1;

	arg.mata_data[14].type = DATYPE_INT;
	arg.mata_data[14].name = "item_num1";
	arg.mata_data[14].length = 4;

	arg.mata_data[15].type = DATYPE_LONG64;
	arg.mata_data[15].name = "item_invalid_time1";
	arg.mata_data[15].length = 8;

	arg.mata_data[16].type = DATYPE_STRING;
	arg.mata_data[16].name = "item_param1";
	arg.mata_data[16].length = 1024;

	arg.mata_data[17].type = DATYPE_INT;
	arg.mata_data[17].name = "item_id2";
	arg.mata_data[17].length = 4;

	arg.mata_data[18].type = DATYPE_CHAR;
	arg.mata_data[18].name = "item_is_bind2";
	arg.mata_data[18].length = 1;

	arg.mata_data[19].type = DATYPE_INT;
	arg.mata_data[19].name = "item_num2";
	arg.mata_data[19].length = 4;

	arg.mata_data[20].type = DATYPE_LONG64;
	arg.mata_data[20].name = "item_invalid_time2";
	arg.mata_data[20].length = 8;

	arg.mata_data[21].type = DATYPE_STRING;
	arg.mata_data[21].name = "item_param2";
	arg.mata_data[21].length = 1024;

	arg.mata_data[22].type = DATYPE_INT;
	arg.mata_data[22].name = "item_id3";
	arg.mata_data[22].length = 4;

	arg.mata_data[23].type = DATYPE_CHAR;
	arg.mata_data[23].name = "item_is_bind3";
	arg.mata_data[23].length = 1;

	arg.mata_data[24].type = DATYPE_INT;
	arg.mata_data[24].name = "item_num3";
	arg.mata_data[24].length = 4;

	arg.mata_data[25].type = DATYPE_LONG64;
	arg.mata_data[25].name = "item_invalid_time3";
	arg.mata_data[25].length = 8;

	arg.mata_data[26].type = DATYPE_STRING;
	arg.mata_data[26].name = "item_param3";
	arg.mata_data[26].length = 1024;

	arg.mata_data[27].type = DATYPE_STRING;
	arg.mata_data[27].name = "virtual_item_data";
	arg.mata_data[27].length = 128;

	return arg;
}

TableInitArg GetteamrankTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "teamrank";
	arg.mata_data.resize(15);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idteamrank";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "rank_index";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_INT;
	arg.mata_data[2].name = "uid1";
	arg.mata_data[2].length = 4;

	arg.mata_data[3].type = DATYPE_INT;
	arg.mata_data[3].name = "uid2";
	arg.mata_data[3].length = 4;

	arg.mata_data[4].type = DATYPE_INT;
	arg.mata_data[4].name = "uid3";
	arg.mata_data[4].length = 4;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "user_name1";
	arg.mata_data[5].length = 32;

	arg.mata_data[6].type = DATYPE_STRING;
	arg.mata_data[6].name = "user_name2";
	arg.mata_data[6].length = 32;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "user_name3";
	arg.mata_data[7].length = 32;

	arg.mata_data[8].type = DATYPE_CHAR;
	arg.mata_data[8].name = "camp1";
	arg.mata_data[8].length = 1;

	arg.mata_data[9].type = DATYPE_CHAR;
	arg.mata_data[9].name = "camp2";
	arg.mata_data[9].length = 1;

	arg.mata_data[10].type = DATYPE_CHAR;
	arg.mata_data[10].name = "camp3";
	arg.mata_data[10].length = 1;

	arg.mata_data[11].type = DATYPE_INT;
	arg.mata_data[11].name = "rank_type";
	arg.mata_data[11].length = 4;

	arg.mata_data[12].type = DATYPE_LONG64;
	arg.mata_data[12].name = "rank_value";
	arg.mata_data[12].length = 8;

	arg.mata_data[13].type = DATYPE_INT;
	arg.mata_data[13].name = "flexible_int";
	arg.mata_data[13].length = 4;

	arg.mata_data[14].type = DATYPE_LONG64;
	arg.mata_data[14].name = "flexible_ll";
	arg.mata_data[14].length = 8;

	return arg;
}

TableInitArg GettuodanTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "tuodan";
	arg.mata_data.resize(4);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "tuodan_id";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_INT;
	arg.mata_data[1].name = "role_id";
	arg.mata_data[1].length = 4;

	arg.mata_data[2].type = DATYPE_LONG64;
	arg.mata_data[2].name = "create_time";
	arg.mata_data[2].length = 8;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "notice";
	arg.mata_data[3].length = 256;

	return arg;
}

TableInitArg GetworldstatusTableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "worldstatus";
	arg.mata_data.resize(20);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idworldstatus";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_LONG64;
	arg.mata_data[1].name = "FOR_INDEX";
	arg.mata_data[1].length = 8;

	arg.mata_data[2].type = DATYPE_LONG64;
	arg.mata_data[2].name = "start_time";
	arg.mata_data[2].length = 8;

	arg.mata_data[3].type = DATYPE_INT;
	arg.mata_data[3].name = "world_time";
	arg.mata_data[3].length = 4;

	arg.mata_data[4].type = DATYPE_LONG64;
	arg.mata_data[4].name = "last_update_dayid";
	arg.mata_data[4].length = 8;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "activity_open_records";
	arg.mata_data[5].length = 96;

	arg.mata_data[6].type = DATYPE_INT;
	arg.mata_data[6].name = "xianmengzhan_mengzhu_id";
	arg.mata_data[6].length = 4;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "common_data";
	arg.mata_data[7].length = 4800;

	arg.mata_data[8].type = DATYPE_STRING;
	arg.mata_data[8].name = "challenge_field_data";
	arg.mata_data[8].length = 1024;

	arg.mata_data[9].type = DATYPE_STRING;
	arg.mata_data[9].name = "world_event_data";
	arg.mata_data[9].length = 512;

	arg.mata_data[10].type = DATYPE_STRING;
	arg.mata_data[10].name = "csa_rank_data";
	arg.mata_data[10].length = 2048;

	arg.mata_data[11].type = DATYPE_STRING;
	arg.mata_data[11].name = "csa_other_data";
	arg.mata_data[11].length = 256;

	arg.mata_data[12].type = DATYPE_INT;
	arg.mata_data[12].name = "register_limit";
	arg.mata_data[12].length = 4;

	arg.mata_data[13].type = DATYPE_STRING;
	arg.mata_data[13].name = "boss_refresh_data";
	arg.mata_data[13].length = 1024;

	arg.mata_data[14].type = DATYPE_STRING;
	arg.mata_data[14].name = "cross_guildbattle_data";
	arg.mata_data[14].length = 2048;

	arg.mata_data[15].type = DATYPE_STRING;
	arg.mata_data[15].name = "csa_boss_rank_data";
	arg.mata_data[15].length = 512;

	arg.mata_data[16].type = DATYPE_STRING;
	arg.mata_data[16].name = "cross_rand_activity_data";
	arg.mata_data[16].length = 1024;

	arg.mata_data[17].type = DATYPE_STRING;
	arg.mata_data[17].name = "cross_rand_activity_big_data";
	arg.mata_data[17].length = 1024;

	arg.mata_data[18].type = DATYPE_STRING;
	arg.mata_data[18].name = "cross_rand_activity_big_data2";
	arg.mata_data[18].length = 4096;

	arg.mata_data[19].type = DATYPE_STRING;
	arg.mata_data[19].name = "cross_rand_activity_cloud_purchase_data";
	arg.mata_data[19].length = 30720;

	return arg;
}

TableInitArg Getworldstatus2TableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "worldstatus2";
	arg.mata_data.resize(8);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idworldstatus2";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_LONG64;
	arg.mata_data[1].name = "FOR_INDEX";
	arg.mata_data[1].length = 8;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "drop_log_list_data";
	arg.mata_data[2].length = 8192;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "wedding_data";
	arg.mata_data[3].length = 20992;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "common_data_2";
	arg.mata_data[4].length = 8192;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "global_profess_data";
	arg.mata_data[5].length = 5120;

	arg.mata_data[6].type = DATYPE_STRING;
	arg.mata_data[6].name = "drop_log_fb_data";
	arg.mata_data[6].length = 7680;

	arg.mata_data[7].type = DATYPE_STRING;
	arg.mata_data[7].name = "world_acquisition_log";
	arg.mata_data[7].length = 8000;

	return arg;
}

TableInitArg Getworldstatus3TableDef()
{
	TableInitArg arg;
	arg.table_id = 0;
	arg.key_id_index = 0;
	arg.table_name = "worldstatus3";
	arg.mata_data.resize(7);

	arg.mata_data[0].type = DATYPE_LONG64;
	arg.mata_data[0].name = "idworldstatus3";
	arg.mata_data[0].length = 8;

	arg.mata_data[1].type = DATYPE_LONG64;
	arg.mata_data[1].name = "FOR_INDEX";
	arg.mata_data[1].length = 8;

	arg.mata_data[2].type = DATYPE_STRING;
	arg.mata_data[2].name = "common_data";
	arg.mata_data[2].length = 1024;

	arg.mata_data[3].type = DATYPE_STRING;
	arg.mata_data[3].name = "toprank_data";
	arg.mata_data[3].length = 3072;

	arg.mata_data[4].type = DATYPE_STRING;
	arg.mata_data[4].name = "lottery_count_data";
	arg.mata_data[4].length = 256;

	arg.mata_data[5].type = DATYPE_STRING;
	arg.mata_data[5].name = "rand_activity_data";
	arg.mata_data[5].length = 58880;

	arg.mata_data[6].type = DATYPE_STRING;
	arg.mata_data[6].name = "lucky_cloud_buy_param_data";
	arg.mata_data[6].length = 2048;

	return arg;
}


}

