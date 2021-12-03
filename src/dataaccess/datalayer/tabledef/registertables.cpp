

#include "registertables.h"
#include <string.h>

namespace tabledef
{


REGISTER(accountgold);
void accountgold::Update(const DataAdapter& v)
{
	accountgold_id = v.m_data_area[0].vint64;
	plat_user_name = v.m_data_area[1].vcharp;
	role_id = v.m_data_area[2].vint;
	gold = v.m_data_area[3].vint64;
	gold_history = v.m_data_area[4].vint64;
	last_get_gold_time = v.m_data_area[5].vint64;
	authority_type = v.m_data_area[6].vint;
}
void accountgold::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = accountgold_id;
	v->m_data_area[1].length = (unsigned int)strlen(plat_user_name); 
	memcpy(v->m_data_area[1].vcharp, plat_user_name, v->m_data_area[1].length);
	v->m_data_area[2].vint = role_id;
	v->m_data_area[3].vint64 = gold;
	v->m_data_area[4].vint64 = gold_history;
	v->m_data_area[5].vint64 = last_get_gold_time;
	v->m_data_area[6].vint = authority_type;
}

REGISTER(accountidentity);
void accountidentity::Update(const DataAdapter& v)
{
	idAccountIdentity = v.m_data_area[0].vint;
	plat_user_name = v.m_data_area[1].vcharp;
	name = v.m_data_area[2].vcharp;
	identity_num = v.m_data_area[3].vcharp;
}
void accountidentity::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint = idAccountIdentity;
	v->m_data_area[1].length = (unsigned int)strlen(plat_user_name); 
	memcpy(v->m_data_area[1].vcharp, plat_user_name, v->m_data_area[1].length);
	v->m_data_area[2].length = (unsigned int)strlen(name); 
	memcpy(v->m_data_area[2].vcharp, name, v->m_data_area[2].length);
	v->m_data_area[3].length = (unsigned int)strlen(identity_num); 
	memcpy(v->m_data_area[3].vcharp, identity_num, v->m_data_area[3].length);
}

REGISTER(black_lists);
void black_lists::Update(const DataAdapter& v)
{
	idblack = v.m_data_area[0].vint64;
	role_id = v.m_data_area[1].vint;
	black_index = v.m_data_area[2].vint;
	FOR_INDEX = v.m_data_area[3].vint64;
	black_uid = v.m_data_area[4].vint;
	black_name = v.m_data_area[5].vcharp;
	sex = v.m_data_area[6].vchar;
	prof = v.m_data_area[7].vint;
}
void black_lists::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idblack;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].vint = black_index;
	v->m_data_area[3].vint64 = FOR_INDEX;
	v->m_data_area[4].vint = black_uid;
	v->m_data_area[5].length = (unsigned int)strlen(black_name); 
	memcpy(v->m_data_area[5].vcharp, black_name, v->m_data_area[5].length);
	v->m_data_area[6].vchar = sex;
	v->m_data_area[7].vint = prof;
}

REGISTER(camp);
void camp::Update(const DataAdapter& v)
{
	idcamp = v.m_data_area[0].vint;
	camp_type = v.m_data_area[1].vint;
	role_count = v.m_data_area[2].vint;
	guild_count = v.m_data_area[3].vint;
	rich_level = v.m_data_area[4].vint;
	earnings_end_time1 = v.m_data_area[5].vint64;
	earnings1 = v.m_data_area[6].vint;
	earnings_end_time2 = v.m_data_area[7].vint64;
	earnings2 = v.m_data_area[8].vint;
	total_capability = v.m_data_area[9].vint64;
}
void camp::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint = idcamp;
	v->m_data_area[1].vint = camp_type;
	v->m_data_area[2].vint = role_count;
	v->m_data_area[3].vint = guild_count;
	v->m_data_area[4].vint = rich_level;
	v->m_data_area[5].vint64 = earnings_end_time1;
	v->m_data_area[6].vint = earnings1;
	v->m_data_area[7].vint64 = earnings_end_time2;
	v->m_data_area[8].vint = earnings2;
	v->m_data_area[9].vint64 = total_capability;
}

REGISTER(challengeruser);
void challengeruser::Update(const DataAdapter& v)
{
	challenger_user_id = v.m_data_area[0].vint64;
	role_id = v.m_data_area[1].vint64;
	rankpos = v.m_data_area[2].vint;
	challenge_user_data = v.m_data_area[3].vcharp;
	common_data = v.m_data_area[4].vcharp;
	report_info_data = v.m_data_area[5].vcharp;
	role_appearance_data = v.m_data_area[6].vcharp;
}
void challengeruser::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = challenger_user_id;
	v->m_data_area[1].vint64 = role_id;
	v->m_data_area[2].vint = rankpos;
	v->m_data_area[3].length = (unsigned int)strlen(challenge_user_data); 
	memcpy(v->m_data_area[3].vcharp, challenge_user_data, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(common_data); 
	memcpy(v->m_data_area[4].vcharp, common_data, v->m_data_area[4].length);
	v->m_data_area[5].length = (unsigned int)strlen(report_info_data); 
	memcpy(v->m_data_area[5].vcharp, report_info_data, v->m_data_area[5].length);
	v->m_data_area[6].length = (unsigned int)strlen(role_appearance_data); 
	memcpy(v->m_data_area[6].vcharp, role_appearance_data, v->m_data_area[6].length);
}

REGISTER(command);
void command::Update(const DataAdapter& v)
{
	idcommand = v.m_data_area[0].vint;
	creator = v.m_data_area[1].vcharp;
	createtime = v.m_data_area[2].vint64;
	type = v.m_data_area[3].vchar;
	cmd = v.m_data_area[4].vcharp;
	confirmtime = v.m_data_area[5].vint64;
}
void command::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint = idcommand;
	v->m_data_area[1].length = (unsigned int)strlen(creator); 
	memcpy(v->m_data_area[1].vcharp, creator, v->m_data_area[1].length);
	v->m_data_area[2].vint64 = createtime;
	v->m_data_area[3].vchar = type;
	v->m_data_area[4].length = (unsigned int)strlen(cmd); 
	memcpy(v->m_data_area[4].vcharp, cmd, v->m_data_area[4].length);
	v->m_data_area[5].vint64 = confirmtime;
}

REGISTER(cross_personrank);
void cross_personrank::Update(const DataAdapter& v)
{
	idcrosspersonrank = v.m_data_area[0].vint64;
	rank_index = v.m_data_area[1].vint;
	uuid = v.m_data_area[2].vint64;
	user_name = v.m_data_area[3].vcharp;
	role_sex = v.m_data_area[4].vint;
	role_prof = v.m_data_area[5].vint;
	role_camp = v.m_data_area[6].vint;
	role_exp = v.m_data_area[7].vint64;
	role_level = v.m_data_area[8].vint;
	rank_type = v.m_data_area[9].vint;
	rank_value = v.m_data_area[10].vint64;
	record_index = v.m_data_area[11].vint;
	flexible_ll = v.m_data_area[12].vint64;
	flexible_name = v.m_data_area[13].vcharp;
	flexible_int = v.m_data_area[14].vint;
	vip_level = v.m_data_area[15].vint;
}
void cross_personrank::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idcrosspersonrank;
	v->m_data_area[1].vint = rank_index;
	v->m_data_area[2].vint64 = uuid;
	v->m_data_area[3].length = (unsigned int)strlen(user_name); 
	memcpy(v->m_data_area[3].vcharp, user_name, v->m_data_area[3].length);
	v->m_data_area[4].vint = role_sex;
	v->m_data_area[5].vint = role_prof;
	v->m_data_area[6].vint = role_camp;
	v->m_data_area[7].vint64 = role_exp;
	v->m_data_area[8].vint = role_level;
	v->m_data_area[9].vint = rank_type;
	v->m_data_area[10].vint64 = rank_value;
	v->m_data_area[11].vint = record_index;
	v->m_data_area[12].vint64 = flexible_ll;
	v->m_data_area[13].length = (unsigned int)strlen(flexible_name); 
	memcpy(v->m_data_area[13].vcharp, flexible_name, v->m_data_area[13].length);
	v->m_data_area[14].vint = flexible_int;
	v->m_data_area[15].vint = vip_level;
}

REGISTER(crossrecord);
void crossrecord::Update(const DataAdapter& v)
{
	idcrossrecord = v.m_data_area[0].vint64;
	role_id = v.m_data_area[1].vint;
	role_name = v.m_data_area[2].vcharp;
	level = v.m_data_area[3].vint;
	sex = v.m_data_area[4].vchar;
	prof = v.m_data_area[5].vchar;
	camp = v.m_data_area[6].vchar;
	record_data = v.m_data_area[7].vcharp;
	appearance_data = v.m_data_area[8].vcharp;
}
void crossrecord::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idcrossrecord;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].length = (unsigned int)strlen(role_name); 
	memcpy(v->m_data_area[2].vcharp, role_name, v->m_data_area[2].length);
	v->m_data_area[3].vint = level;
	v->m_data_area[4].vchar = sex;
	v->m_data_area[5].vchar = prof;
	v->m_data_area[6].vchar = camp;
	v->m_data_area[7].length = (unsigned int)strlen(record_data); 
	memcpy(v->m_data_area[7].vcharp, record_data, v->m_data_area[7].length);
	v->m_data_area[8].length = (unsigned int)strlen(appearance_data); 
	memcpy(v->m_data_area[8].vcharp, appearance_data, v->m_data_area[8].length);
}

REGISTER(crossstatus);
void crossstatus::Update(const DataAdapter& v)
{
	idcrossstatus = v.m_data_area[0].vint64;
	FOR_INDEX = v.m_data_area[1].vint64;
	last_update_dayid = v.m_data_area[2].vint;
	cross_person_rank_title_data = v.m_data_area[3].vcharp;
	cross_activity_data = v.m_data_area[4].vcharp;
}
void crossstatus::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idcrossstatus;
	v->m_data_area[1].vint64 = FOR_INDEX;
	v->m_data_area[2].vint = last_update_dayid;
	v->m_data_area[3].length = (unsigned int)strlen(cross_person_rank_title_data); 
	memcpy(v->m_data_area[3].vcharp, cross_person_rank_title_data, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(cross_activity_data); 
	memcpy(v->m_data_area[4].vcharp, cross_activity_data, v->m_data_area[4].length);
}

REGISTER(dailyfind);
void dailyfind::Update(const DataAdapter& v)
{
	iddailyfind = v.m_data_area[0].vint64;
	role_id = v.m_data_area[1].vint;
	FOR_INDEX = v.m_data_area[2].vint64;
	find_time = v.m_data_area[3].vint;
	role_level = v.m_data_area[4].vint;
	find_index = v.m_data_area[5].vint;
	param = v.m_data_area[6].vint;
	param1 = v.m_data_area[7].vint64;
	find_count = v.m_data_area[8].vint;
}
void dailyfind::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = iddailyfind;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].vint64 = FOR_INDEX;
	v->m_data_area[3].vint = find_time;
	v->m_data_area[4].vint = role_level;
	v->m_data_area[5].vint = find_index;
	v->m_data_area[6].vint = param;
	v->m_data_area[7].vint64 = param1;
	v->m_data_area[8].vint = find_count;
}

REGISTER(enemy);
void enemy::Update(const DataAdapter& v)
{
	idenemy = v.m_data_area[0].vint64;
	role_id = v.m_data_area[1].vint;
	FOR_INDEX = v.m_data_area[2].vint64;
	enemy_uid = v.m_data_area[3].vint;
	enemy_plat_type = v.m_data_area[4].vint;
	kill_count = v.m_data_area[5].vint;
	last_kill_time = v.m_data_area[6].vint64;
	enemy_index = v.m_data_area[7].vint;
	be_kill_count = v.m_data_area[8].vint;
}
void enemy::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idenemy;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].vint64 = FOR_INDEX;
	v->m_data_area[3].vint = enemy_uid;
	v->m_data_area[4].vint = enemy_plat_type;
	v->m_data_area[5].vint = kill_count;
	v->m_data_area[6].vint64 = last_kill_time;
	v->m_data_area[7].vint = enemy_index;
	v->m_data_area[8].vint = be_kill_count;
}

REGISTER(fishpool);
void fishpool::Update(const DataAdapter& v)
{
	idfishpool = v.m_data_area[0].vint64;
	fish_role_id = v.m_data_area[1].vint;
	common_data = v.m_data_area[2].vcharp;
	raise_fish_data = v.m_data_area[3].vcharp;
	today_raise_record = v.m_data_area[4].vcharp;
}
void fishpool::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idfishpool;
	v->m_data_area[1].vint = fish_role_id;
	v->m_data_area[2].length = (unsigned int)strlen(common_data); 
	memcpy(v->m_data_area[2].vcharp, common_data, v->m_data_area[2].length);
	v->m_data_area[3].length = (unsigned int)strlen(raise_fish_data); 
	memcpy(v->m_data_area[3].vcharp, raise_fish_data, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(today_raise_record); 
	memcpy(v->m_data_area[4].vcharp, today_raise_record, v->m_data_area[4].length);
}

REGISTER(friends);
void friends::Update(const DataAdapter& v)
{
	idfriend = v.m_data_area[0].vint64;
	role_id = v.m_data_area[1].vint;
	FOR_INDEX = v.m_data_area[2].vint64;
	friend_uid = v.m_data_area[3].vint;
	intimacy = v.m_data_area[4].vint;
	team_today_intimacy = v.m_data_area[5].vint;
	friend_index = v.m_data_area[6].vint;
	gift_count = v.m_data_area[7].vint;
	favorable_impression = v.m_data_area[8].vint;
	day_chat_count = v.m_data_area[9].vint;
	add_timestamp = v.m_data_area[10].vint64;
}
void friends::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idfriend;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].vint64 = FOR_INDEX;
	v->m_data_area[3].vint = friend_uid;
	v->m_data_area[4].vint = intimacy;
	v->m_data_area[5].vint = team_today_intimacy;
	v->m_data_area[6].vint = friend_index;
	v->m_data_area[7].vint = gift_count;
	v->m_data_area[8].vint = favorable_impression;
	v->m_data_area[9].vint = day_chat_count;
	v->m_data_area[10].vint64 = add_timestamp;
}

REGISTER(globalrole);
void globalrole::Update(const DataAdapter& v)
{
	id_globalrole = v.m_data_area[0].vint64;
	role_id = v.m_data_area[1].vint;
	little_pet_data = v.m_data_area[2].vcharp;
	love_tree_data = v.m_data_area[3].vcharp;
	love_contract_data = v.m_data_area[4].vcharp;
	jing_ling_home_data = v.m_data_area[5].vcharp;
	role_fighting_data = v.m_data_area[6].vcharp;
	qingyuan_global_data = v.m_data_area[7].vcharp;
	role_appearance_data = v.m_data_area[8].vcharp;
	common_data_global_data = v.m_data_area[9].vcharp;
	spousehome_data = v.m_data_area[10].vcharp;
}
void globalrole::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = id_globalrole;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].length = (unsigned int)strlen(little_pet_data); 
	memcpy(v->m_data_area[2].vcharp, little_pet_data, v->m_data_area[2].length);
	v->m_data_area[3].length = (unsigned int)strlen(love_tree_data); 
	memcpy(v->m_data_area[3].vcharp, love_tree_data, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(love_contract_data); 
	memcpy(v->m_data_area[4].vcharp, love_contract_data, v->m_data_area[4].length);
	v->m_data_area[5].length = (unsigned int)strlen(jing_ling_home_data); 
	memcpy(v->m_data_area[5].vcharp, jing_ling_home_data, v->m_data_area[5].length);
	v->m_data_area[6].length = (unsigned int)strlen(role_fighting_data); 
	memcpy(v->m_data_area[6].vcharp, role_fighting_data, v->m_data_area[6].length);
	v->m_data_area[7].length = (unsigned int)strlen(qingyuan_global_data); 
	memcpy(v->m_data_area[7].vcharp, qingyuan_global_data, v->m_data_area[7].length);
	v->m_data_area[8].length = (unsigned int)strlen(role_appearance_data); 
	memcpy(v->m_data_area[8].vcharp, role_appearance_data, v->m_data_area[8].length);
	v->m_data_area[9].length = (unsigned int)strlen(common_data_global_data); 
	memcpy(v->m_data_area[9].vcharp, common_data_global_data, v->m_data_area[9].length);
	v->m_data_area[10].length = (unsigned int)strlen(spousehome_data); 
	memcpy(v->m_data_area[10].vcharp, spousehome_data, v->m_data_area[10].length);
}

REGISTER(guild);
void guild::Update(const DataAdapter& v)
{
	guild_id_inc = v.m_data_area[0].vint64;
	guild_id = v.m_data_area[1].vint;
	guild_name = v.m_data_area[2].vcharp;
	create_time = v.m_data_area[3].vint64;
	guild_level = v.m_data_area[4].vint;
	guild_exp = v.m_data_area[5].vint;
	camp = v.m_data_area[6].vint;
	notice = v.m_data_area[7].vcharp;
	last_update_dayid = v.m_data_area[8].vint64;
	warning_dismiss_days = v.m_data_area[9].vint;
	applyfor_setup = v.m_data_area[10].vint;
	last_active_time = v.m_data_area[11].vint64;
	auto_kickout_setup = v.m_data_area[12].vint;
	guild_max_capability = v.m_data_area[13].vint;
	applyfor_need_capability = v.m_data_area[14].vint;
	applyfor_need_level = v.m_data_area[15].vint;
	last_uplevel_timestamp = v.m_data_area[16].vint64;
	callin_times = v.m_data_area[17].vint;
	today_monster_finish_seq = v.m_data_area[18].vint;
	today_monster_finish_username = v.m_data_area[19].vcharp;
	common_data = v.m_data_area[20].vcharp;
	active_degree = v.m_data_area[21].vint;
	boss_list = v.m_data_area[22].vcharp;
	storge = v.m_data_area[23].vcharp;
	enemy_list = v.m_data_area[24].vcharp;
}
void guild::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = guild_id_inc;
	v->m_data_area[1].vint = guild_id;
	v->m_data_area[2].length = (unsigned int)strlen(guild_name); 
	memcpy(v->m_data_area[2].vcharp, guild_name, v->m_data_area[2].length);
	v->m_data_area[3].vint64 = create_time;
	v->m_data_area[4].vint = guild_level;
	v->m_data_area[5].vint = guild_exp;
	v->m_data_area[6].vint = camp;
	v->m_data_area[7].length = (unsigned int)strlen(notice); 
	memcpy(v->m_data_area[7].vcharp, notice, v->m_data_area[7].length);
	v->m_data_area[8].vint64 = last_update_dayid;
	v->m_data_area[9].vint = warning_dismiss_days;
	v->m_data_area[10].vint = applyfor_setup;
	v->m_data_area[11].vint64 = last_active_time;
	v->m_data_area[12].vint = auto_kickout_setup;
	v->m_data_area[13].vint = guild_max_capability;
	v->m_data_area[14].vint = applyfor_need_capability;
	v->m_data_area[15].vint = applyfor_need_level;
	v->m_data_area[16].vint64 = last_uplevel_timestamp;
	v->m_data_area[17].vint = callin_times;
	v->m_data_area[18].vint = today_monster_finish_seq;
	v->m_data_area[19].length = (unsigned int)strlen(today_monster_finish_username); 
	memcpy(v->m_data_area[19].vcharp, today_monster_finish_username, v->m_data_area[19].length);
	v->m_data_area[20].length = (unsigned int)strlen(common_data); 
	memcpy(v->m_data_area[20].vcharp, common_data, v->m_data_area[20].length);
	v->m_data_area[21].vint = active_degree;
	v->m_data_area[22].length = (unsigned int)strlen(boss_list); 
	memcpy(v->m_data_area[22].vcharp, boss_list, v->m_data_area[22].length);
	v->m_data_area[23].length = (unsigned int)strlen(storge); 
	memcpy(v->m_data_area[23].vcharp, storge, v->m_data_area[23].length);
	v->m_data_area[24].length = (unsigned int)strlen(enemy_list); 
	memcpy(v->m_data_area[24].vcharp, enemy_list, v->m_data_area[24].length);
}

REGISTER(guildapplyfor);
void guildapplyfor::Update(const DataAdapter& v)
{
	idguildapplyfor = v.m_data_area[0].vint64;
	guild_id = v.m_data_area[1].vint;
	apply_for_index = v.m_data_area[2].vint;
	FOR_INDEX = v.m_data_area[3].vint64;
	uid = v.m_data_area[4].vint;
	applyfor_time = v.m_data_area[5].vint64;
}
void guildapplyfor::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idguildapplyfor;
	v->m_data_area[1].vint = guild_id;
	v->m_data_area[2].vint = apply_for_index;
	v->m_data_area[3].vint64 = FOR_INDEX;
	v->m_data_area[4].vint = uid;
	v->m_data_area[5].vint64 = applyfor_time;
}

REGISTER(guildevent);
void guildevent::Update(const DataAdapter& v)
{
	idguildevent = v.m_data_area[0].vint64;
	guild_id = v.m_data_area[1].vint;
	event_index = v.m_data_area[2].vint;
	FOR_INDEX = v.m_data_area[3].vint64;
	event_type = v.m_data_area[4].vint;
	event_owner = v.m_data_area[5].vcharp;
	event_owner_post = v.m_data_area[6].vint;
	event_time = v.m_data_area[7].vint64;
	big_event = v.m_data_area[8].vint;
	cannot_remove = v.m_data_area[9].vint;
	param0 = v.m_data_area[10].vint;
	param1 = v.m_data_area[11].vint;
	param2 = v.m_data_area[12].vint;
	param3 = v.m_data_area[13].vint;
	sparam0 = v.m_data_area[14].vcharp;
}
void guildevent::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idguildevent;
	v->m_data_area[1].vint = guild_id;
	v->m_data_area[2].vint = event_index;
	v->m_data_area[3].vint64 = FOR_INDEX;
	v->m_data_area[4].vint = event_type;
	v->m_data_area[5].length = (unsigned int)strlen(event_owner); 
	memcpy(v->m_data_area[5].vcharp, event_owner, v->m_data_area[5].length);
	v->m_data_area[6].vint = event_owner_post;
	v->m_data_area[7].vint64 = event_time;
	v->m_data_area[8].vint = big_event;
	v->m_data_area[9].vint = cannot_remove;
	v->m_data_area[10].vint = param0;
	v->m_data_area[11].vint = param1;
	v->m_data_area[12].vint = param2;
	v->m_data_area[13].vint = param3;
	v->m_data_area[14].length = (unsigned int)strlen(sparam0); 
	memcpy(v->m_data_area[14].vcharp, sparam0, v->m_data_area[14].length);
}

REGISTER(guildmember);
void guildmember::Update(const DataAdapter& v)
{
	idguildmember = v.m_data_area[0].vint64;
	guild_id = v.m_data_area[1].vint;
	member_index = v.m_data_area[2].vint;
	FOR_INDEX = v.m_data_area[3].vint64;
	uid = v.m_data_area[4].vint;
	post = v.m_data_area[5].vint;
	gongxian = v.m_data_area[6].vint;
	total_gongxian = v.m_data_area[7].vint;
	join_time = v.m_data_area[8].vint64;
	last_login_time = v.m_data_area[9].vint64;
	lucky_color = v.m_data_area[10].vint;
	today_guild_pao_saizi_times = v.m_data_area[11].vint;
	today_last_guild_pao_saizi_time = v.m_data_area[12].vint64;
	today_guild_saizi_score = v.m_data_area[13].vint;
	maze_layer = v.m_data_area[14].vint;
	maze_complete_time = v.m_data_area[15].vint64;
	common_data = v.m_data_area[16].vcharp;
}
void guildmember::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idguildmember;
	v->m_data_area[1].vint = guild_id;
	v->m_data_area[2].vint = member_index;
	v->m_data_area[3].vint64 = FOR_INDEX;
	v->m_data_area[4].vint = uid;
	v->m_data_area[5].vint = post;
	v->m_data_area[6].vint = gongxian;
	v->m_data_area[7].vint = total_gongxian;
	v->m_data_area[8].vint64 = join_time;
	v->m_data_area[9].vint64 = last_login_time;
	v->m_data_area[10].vint = lucky_color;
	v->m_data_area[11].vint = today_guild_pao_saizi_times;
	v->m_data_area[12].vint64 = today_last_guild_pao_saizi_time;
	v->m_data_area[13].vint = today_guild_saizi_score;
	v->m_data_area[14].vint = maze_layer;
	v->m_data_area[15].vint64 = maze_complete_time;
	v->m_data_area[16].length = (unsigned int)strlen(common_data); 
	memcpy(v->m_data_area[16].vcharp, common_data, v->m_data_area[16].length);
}

REGISTER(hotkey);
void hotkey::Update(const DataAdapter& v)
{
	hotkey_id = v.m_data_area[0].vint64;
	role_id = v.m_data_area[1].vint;
	hotkey_index = v.m_data_area[2].vint;
	FOR_INDEX = v.m_data_area[3].vint64;
	hotkey_item_type = v.m_data_area[4].vchar;
	hotkey_item_id = v.m_data_area[5].vint;
}
void hotkey::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = hotkey_id;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].vint = hotkey_index;
	v->m_data_area[3].vint64 = FOR_INDEX;
	v->m_data_area[4].vchar = hotkey_item_type;
	v->m_data_area[5].vint = hotkey_item_id;
}

REGISTER(itemlist);
void itemlist::Update(const DataAdapter& v)
{
	iditemlist = v.m_data_area[0].vint64;
	role_id = v.m_data_area[1].vint;
	itemlist_index = v.m_data_area[2].vint;
	FOR_INDEX = v.m_data_area[3].vint64;
	item_id = v.m_data_area[4].vint;
	is_bind = v.m_data_area[5].vchar;
	num = v.m_data_area[6].vint;
	invalid_time = v.m_data_area[7].vint64;
	gold_price = v.m_data_area[8].vint;
	param = v.m_data_area[9].vcharp;
}
void itemlist::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = iditemlist;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].vint = itemlist_index;
	v->m_data_area[3].vint64 = FOR_INDEX;
	v->m_data_area[4].vint = item_id;
	v->m_data_area[5].vchar = is_bind;
	v->m_data_area[6].vint = num;
	v->m_data_area[7].vint64 = invalid_time;
	v->m_data_area[8].vint = gold_price;
	v->m_data_area[9].length = (unsigned int)strlen(param); 
	memcpy(v->m_data_area[9].vcharp, param, v->m_data_area[9].length);
}

REGISTER(login);
void login::Update(const DataAdapter& v)
{
	login_id = v.m_data_area[0].vint64;
	plat_user_name = v.m_data_area[1].vcharp;
	db_index = v.m_data_area[2].vint;
	role_id_1 = v.m_data_area[3].vint;
	role_id_2 = v.m_data_area[4].vint;
	role_id_3 = v.m_data_area[5].vint;
	lastlogintime = v.m_data_area[6].vint64;
	createtime = v.m_data_area[7].vint64;
	forbidtime = v.m_data_area[8].vint64;
	is_anti_wallow = v.m_data_area[9].vchar;
	forbid_time_1 = v.m_data_area[10].vint64;
	forbid_time_2 = v.m_data_area[11].vint64;
	forbid_time_3 = v.m_data_area[12].vint64;
}
void login::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = login_id;
	v->m_data_area[1].length = (unsigned int)strlen(plat_user_name); 
	memcpy(v->m_data_area[1].vcharp, plat_user_name, v->m_data_area[1].length);
	v->m_data_area[2].vint = db_index;
	v->m_data_area[3].vint = role_id_1;
	v->m_data_area[4].vint = role_id_2;
	v->m_data_area[5].vint = role_id_3;
	v->m_data_area[6].vint64 = lastlogintime;
	v->m_data_area[7].vint64 = createtime;
	v->m_data_area[8].vint64 = forbidtime;
	v->m_data_area[9].vchar = is_anti_wallow;
	v->m_data_area[10].vint64 = forbid_time_1;
	v->m_data_area[11].vint64 = forbid_time_2;
	v->m_data_area[12].vint64 = forbid_time_3;
}

REGISTER(mail);
void mail::Update(const DataAdapter& v)
{
	idmail = v.m_data_area[0].vint64;
	uid = v.m_data_area[1].vint;
	mail_index = v.m_data_area[2].vint;
	FOR_INDEX = v.m_data_area[3].vint64;
	sender_uid = v.m_data_area[4].vint;
	sender_name = v.m_data_area[5].vcharp;
	recv_time = v.m_data_area[6].vint64;
	kind = v.m_data_area[7].vint;
	is_read = v.m_data_area[8].vint;
	is_lock = v.m_data_area[9].vint;
	coin = v.m_data_area[10].vint;
	coin_bind = v.m_data_area[11].vint;
	gold = v.m_data_area[12].vint;
	gold_bind = v.m_data_area[13].vint;
	subject = v.m_data_area[14].vcharp;
	content = v.m_data_area[15].vcharp;
	item_id1 = v.m_data_area[16].vint;
	item_is_bind1 = v.m_data_area[17].vchar;
	item_num1 = v.m_data_area[18].vint;
	item_invalid_time1 = v.m_data_area[19].vint64;
	item_param1 = v.m_data_area[20].vcharp;
	item_id2 = v.m_data_area[21].vint;
	item_is_bind2 = v.m_data_area[22].vchar;
	item_num2 = v.m_data_area[23].vint;
	item_invalid_time2 = v.m_data_area[24].vint64;
	item_param2 = v.m_data_area[25].vcharp;
	item_id3 = v.m_data_area[26].vint;
	item_is_bind3 = v.m_data_area[27].vchar;
	item_num3 = v.m_data_area[28].vint;
	item_invalid_time3 = v.m_data_area[29].vint64;
	item_param3 = v.m_data_area[30].vcharp;
	virtual_item_data = v.m_data_area[31].vcharp;
}
void mail::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idmail;
	v->m_data_area[1].vint = uid;
	v->m_data_area[2].vint = mail_index;
	v->m_data_area[3].vint64 = FOR_INDEX;
	v->m_data_area[4].vint = sender_uid;
	v->m_data_area[5].length = (unsigned int)strlen(sender_name); 
	memcpy(v->m_data_area[5].vcharp, sender_name, v->m_data_area[5].length);
	v->m_data_area[6].vint64 = recv_time;
	v->m_data_area[7].vint = kind;
	v->m_data_area[8].vint = is_read;
	v->m_data_area[9].vint = is_lock;
	v->m_data_area[10].vint = coin;
	v->m_data_area[11].vint = coin_bind;
	v->m_data_area[12].vint = gold;
	v->m_data_area[13].vint = gold_bind;
	v->m_data_area[14].length = (unsigned int)strlen(subject); 
	memcpy(v->m_data_area[14].vcharp, subject, v->m_data_area[14].length);
	v->m_data_area[15].length = (unsigned int)strlen(content); 
	memcpy(v->m_data_area[15].vcharp, content, v->m_data_area[15].length);
	v->m_data_area[16].vint = item_id1;
	v->m_data_area[17].vchar = item_is_bind1;
	v->m_data_area[18].vint = item_num1;
	v->m_data_area[19].vint64 = item_invalid_time1;
	v->m_data_area[20].length = (unsigned int)strlen(item_param1); 
	memcpy(v->m_data_area[20].vcharp, item_param1, v->m_data_area[20].length);
	v->m_data_area[21].vint = item_id2;
	v->m_data_area[22].vchar = item_is_bind2;
	v->m_data_area[23].vint = item_num2;
	v->m_data_area[24].vint64 = item_invalid_time2;
	v->m_data_area[25].length = (unsigned int)strlen(item_param2); 
	memcpy(v->m_data_area[25].vcharp, item_param2, v->m_data_area[25].length);
	v->m_data_area[26].vint = item_id3;
	v->m_data_area[27].vchar = item_is_bind3;
	v->m_data_area[28].vint = item_num3;
	v->m_data_area[29].vint64 = item_invalid_time3;
	v->m_data_area[30].length = (unsigned int)strlen(item_param3); 
	memcpy(v->m_data_area[30].vcharp, item_param3, v->m_data_area[30].length);
	v->m_data_area[31].length = (unsigned int)strlen(virtual_item_data); 
	memcpy(v->m_data_area[31].vcharp, virtual_item_data, v->m_data_area[31].length);
}

REGISTER(offlineregister);
void offlineregister::Update(const DataAdapter& v)
{
	idofflineregister = v.m_data_area[0].vint64;
	role_id = v.m_data_area[1].vint;
	last_divorce_time = v.m_data_area[2].vint;
	increase_charm = v.m_data_area[3].vint;
	chongzhi_cmd_flag = v.m_data_area[4].vint;
	red_paper_consume_gold = v.m_data_area[5].vint;
	red_paper_lucky_value = v.m_data_area[6].vint;
	space_cai_conut = v.m_data_area[7].vint;
	space_gift_count = v.m_data_area[8].vint;
	qingyuan_bless_days = v.m_data_area[9].vint;
	guild_box_info = v.m_data_area[10].vcharp;
	common_data = v.m_data_area[11].vcharp;
}
void offlineregister::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idofflineregister;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].vint = last_divorce_time;
	v->m_data_area[3].vint = increase_charm;
	v->m_data_area[4].vint = chongzhi_cmd_flag;
	v->m_data_area[5].vint = red_paper_consume_gold;
	v->m_data_area[6].vint = red_paper_lucky_value;
	v->m_data_area[7].vint = space_cai_conut;
	v->m_data_area[8].vint = space_gift_count;
	v->m_data_area[9].vint = qingyuan_bless_days;
	v->m_data_area[10].length = (unsigned int)strlen(guild_box_info); 
	memcpy(v->m_data_area[10].vcharp, guild_box_info, v->m_data_area[10].length);
	v->m_data_area[11].length = (unsigned int)strlen(common_data); 
	memcpy(v->m_data_area[11].vcharp, common_data, v->m_data_area[11].length);
}

REGISTER(personrank);
void personrank::Update(const DataAdapter& v)
{
	idpersonrank = v.m_data_area[0].vint64;
	rank_index = v.m_data_area[1].vint;
	uid = v.m_data_area[2].vint;
	user_name = v.m_data_area[3].vcharp;
	flexible_name = v.m_data_area[4].vcharp;
	role_sex = v.m_data_area[5].vint;
	role_prof = v.m_data_area[6].vint;
	role_camp = v.m_data_area[7].vint;
	role_vip_level = v.m_data_area[8].vint;
	role_exp = v.m_data_area[9].vint64;
	role_level = v.m_data_area[10].vint;
	rank_type = v.m_data_area[11].vint;
	rank_value = v.m_data_area[12].vint64;
	record_index = v.m_data_area[13].vint;
	flexible_int = v.m_data_area[14].vint;
	flexible_ll = v.m_data_area[15].vint64;
}
void personrank::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idpersonrank;
	v->m_data_area[1].vint = rank_index;
	v->m_data_area[2].vint = uid;
	v->m_data_area[3].length = (unsigned int)strlen(user_name); 
	memcpy(v->m_data_area[3].vcharp, user_name, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(flexible_name); 
	memcpy(v->m_data_area[4].vcharp, flexible_name, v->m_data_area[4].length);
	v->m_data_area[5].vint = role_sex;
	v->m_data_area[6].vint = role_prof;
	v->m_data_area[7].vint = role_camp;
	v->m_data_area[8].vint = role_vip_level;
	v->m_data_area[9].vint64 = role_exp;
	v->m_data_area[10].vint = role_level;
	v->m_data_area[11].vint = rank_type;
	v->m_data_area[12].vint64 = rank_value;
	v->m_data_area[13].vint = record_index;
	v->m_data_area[14].vint = flexible_int;
	v->m_data_area[15].vint64 = flexible_ll;
}

REGISTER(publicnotice);
void publicnotice::Update(const DataAdapter& v)
{
	idpublicnotice = v.m_data_area[0].vint;
	creator = v.m_data_area[1].vcharp;
	createtime = v.m_data_area[2].vint64;
	begin_send_time = v.m_data_area[3].vint64;
	send_interval = v.m_data_area[4].vint64;
	end_send_time = v.m_data_area[5].vint64;
	next_send_time = v.m_data_area[6].vint64;
	content = v.m_data_area[7].vcharp;
	type = v.m_data_area[8].vint;
	color = v.m_data_area[9].vint;
	is_forbid = v.m_data_area[10].vint;
	is_urgent = v.m_data_area[11].vint;
}
void publicnotice::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint = idpublicnotice;
	v->m_data_area[1].length = (unsigned int)strlen(creator); 
	memcpy(v->m_data_area[1].vcharp, creator, v->m_data_area[1].length);
	v->m_data_area[2].vint64 = createtime;
	v->m_data_area[3].vint64 = begin_send_time;
	v->m_data_area[4].vint64 = send_interval;
	v->m_data_area[5].vint64 = end_send_time;
	v->m_data_area[6].vint64 = next_send_time;
	v->m_data_area[7].length = (unsigned int)strlen(content); 
	memcpy(v->m_data_area[7].vcharp, content, v->m_data_area[7].length);
	v->m_data_area[8].vint = type;
	v->m_data_area[9].vint = color;
	v->m_data_area[10].vint = is_forbid;
	v->m_data_area[11].vint = is_urgent;
}

REGISTER(publicsale);
void publicsale::Update(const DataAdapter& v)
{
	idpublicsale = v.m_data_area[0].vint64;
	seller_uid = v.m_data_area[1].vint;
	seller_name = v.m_data_area[2].vcharp;
	sale_index = v.m_data_area[3].vint;
	FOR_INDEX = v.m_data_area[4].vint64;
	item_id = v.m_data_area[5].vint;
	item_is_bind = v.m_data_area[6].vchar;
	item_sale_type = v.m_data_area[7].vint;
	item_num = v.m_data_area[8].vint;
	color = v.m_data_area[9].vint;
	level = v.m_data_area[10].vint;
	prof = v.m_data_area[11].vint;
	equip_order = v.m_data_area[12].vint;
	gold_price = v.m_data_area[13].vint;
	sale_value = v.m_data_area[14].vint;
	price_type = v.m_data_area[15].vint;
	sale_item_type = v.m_data_area[16].vint;
	sale_time = v.m_data_area[17].vint64;
	due_time = v.m_data_area[18].vint64;
	item_invalid_time = v.m_data_area[19].vint64;
	item_param = v.m_data_area[20].vcharp;
}
void publicsale::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idpublicsale;
	v->m_data_area[1].vint = seller_uid;
	v->m_data_area[2].length = (unsigned int)strlen(seller_name); 
	memcpy(v->m_data_area[2].vcharp, seller_name, v->m_data_area[2].length);
	v->m_data_area[3].vint = sale_index;
	v->m_data_area[4].vint64 = FOR_INDEX;
	v->m_data_area[5].vint = item_id;
	v->m_data_area[6].vchar = item_is_bind;
	v->m_data_area[7].vint = item_sale_type;
	v->m_data_area[8].vint = item_num;
	v->m_data_area[9].vint = color;
	v->m_data_area[10].vint = level;
	v->m_data_area[11].vint = prof;
	v->m_data_area[12].vint = equip_order;
	v->m_data_area[13].vint = gold_price;
	v->m_data_area[14].vint = sale_value;
	v->m_data_area[15].vint = price_type;
	v->m_data_area[16].vint = sale_item_type;
	v->m_data_area[17].vint64 = sale_time;
	v->m_data_area[18].vint64 = due_time;
	v->m_data_area[19].vint64 = item_invalid_time;
	v->m_data_area[20].length = (unsigned int)strlen(item_param); 
	memcpy(v->m_data_area[20].vcharp, item_param, v->m_data_area[20].length);
}

REGISTER(role);
void role::Update(const DataAdapter& v)
{
	role_inc_id = v.m_data_area[0].vint64;
	role_id = v.m_data_area[1].vint;
	role_name = v.m_data_area[2].vcharp;
	avatar = v.m_data_area[3].vchar;
	sex = v.m_data_area[4].vchar;
	scene_id = v.m_data_area[5].vint;
	last_scene_id = v.m_data_area[6].vint;
	level = v.m_data_area[7].vint;
	professional = v.m_data_area[8].vint;
	create_time = v.m_data_area[9].vint64;
	online_time = v.m_data_area[10].vint;
	is_online = v.m_data_area[11].vchar;
	last_save_time = v.m_data_area[12].vint64;
	country = v.m_data_area[13].vchar;
	is_chongzhi = v.m_data_area[14].vchar;
	is_micro_pc = v.m_data_area[15].vchar;
	plat_spid = v.m_data_area[16].vint;
}
void role::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = role_inc_id;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].length = (unsigned int)strlen(role_name); 
	memcpy(v->m_data_area[2].vcharp, role_name, v->m_data_area[2].length);
	v->m_data_area[3].vchar = avatar;
	v->m_data_area[4].vchar = sex;
	v->m_data_area[5].vint = scene_id;
	v->m_data_area[6].vint = last_scene_id;
	v->m_data_area[7].vint = level;
	v->m_data_area[8].vint = professional;
	v->m_data_area[9].vint64 = create_time;
	v->m_data_area[10].vint = online_time;
	v->m_data_area[11].vchar = is_online;
	v->m_data_area[12].vint64 = last_save_time;
	v->m_data_area[13].vchar = country;
	v->m_data_area[14].vchar = is_chongzhi;
	v->m_data_area[15].vchar = is_micro_pc;
	v->m_data_area[16].vint = plat_spid;
}

REGISTER(role_attr_detail);
void role_attr_detail::Update(const DataAdapter& v)
{
	role_attr_detail_id = v.m_data_area[0].vint;
	role_id = v.m_data_area[1].vint;
	attr_data = v.m_data_area[2].vcharp;
	role_name = v.m_data_area[3].vcharp;
	level = v.m_data_area[4].vint;
	sex = v.m_data_area[5].vchar;
	prof = v.m_data_area[6].vchar;
	camp = v.m_data_area[7].vchar;
	gold = v.m_data_area[8].vint64;
	gold_bind = v.m_data_area[9].vint64;
	coin = v.m_data_area[10].vint64;
	coin_bind = v.m_data_area[11].vint64;
	guild_id = v.m_data_area[12].vint;
	avatar = v.m_data_area[13].vchar;
	avatar_timestamp = v.m_data_area[14].vint64;
	vip_type = v.m_data_area[15].vchar;
	evil = v.m_data_area[16].vint;
	hp = v.m_data_area[17].vint64;
	max_hp = v.m_data_area[18].vint64;
	mp = v.m_data_area[19].vint64;
	max_mp = v.m_data_area[20].vint64;
	gongji = v.m_data_area[21].vint64;
	fangyu = v.m_data_area[22].vint64;
	mingzhong = v.m_data_area[23].vint64;
	shanbi = v.m_data_area[24].vint64;
	baoji = v.m_data_area[25].vint64;
	jianren = v.m_data_area[26].vint64;
	fujia_shanghai = v.m_data_area[27].vint64;
	dikang_shanghai = v.m_data_area[28].vint64;
	per_jingzhun = v.m_data_area[29].vint64;
	per_baoji = v.m_data_area[30].vint64;
	per_kangbao = v.m_data_area[31].vint64;
	per_pofang = v.m_data_area[32].vint64;
	per_mianshang = v.m_data_area[33].vint64;
	capability = v.m_data_area[34].vint;
	max_capability = v.m_data_area[35].vint;
	lover_uid = v.m_data_area[36].vint;
	lover_name = v.m_data_area[37].vcharp;
	authority_type = v.m_data_area[38].vchar;
	forbid_talk_time = v.m_data_area[39].vint64;
	sublock_pw = v.m_data_area[40].vcharp;
	rolechest_data_len = v.m_data_area[41].vint;
	mentality_data = v.m_data_area[42].vcharp;
	vip_data = v.m_data_area[43].vcharp;
	active_degree_data = v.m_data_area[44].vcharp;
	roleshop_data = v.m_data_area[45].vcharp;
	welfare_data = v.m_data_area[46].vcharp;
	rolehangup_data = v.m_data_area[47].vcharp;
	rolecustom_data = v.m_data_area[48].vcharp;
	itemcolddown_data = v.m_data_area[49].vcharp;
	evaluate_data = v.m_data_area[50].vcharp;
	monitor_data = v.m_data_area[51].vcharp;
	title_data = v.m_data_area[52].vcharp;
	husongtask_data = v.m_data_area[53].vcharp;
	roleactivity_data = v.m_data_area[54].vcharp;
	fbtowerdefend_data = v.m_data_area[55].vcharp;
	last_single_chat_monitor_time = v.m_data_area[56].vint64;
	last_single_chat_monitor_msg = v.m_data_area[57].vcharp;
	last_world_chat_monitor_time = v.m_data_area[58].vint64;
	last_world_chat_monitor_msg = v.m_data_area[59].vcharp;
	last_mail_monitor_time = v.m_data_area[60].vint64;
	last_mail_monitor_subject = v.m_data_area[61].vcharp;
	today_online_time = v.m_data_area[62].vint;
	lastday_online_time = v.m_data_area[63].vint;
	common_data = v.m_data_area[64].vcharp;
	wing_data = v.m_data_area[65].vcharp;
}
void role_attr_detail::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint = role_attr_detail_id;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].length = (unsigned int)strlen(attr_data); 
	memcpy(v->m_data_area[2].vcharp, attr_data, v->m_data_area[2].length);
	v->m_data_area[3].length = (unsigned int)strlen(role_name); 
	memcpy(v->m_data_area[3].vcharp, role_name, v->m_data_area[3].length);
	v->m_data_area[4].vint = level;
	v->m_data_area[5].vchar = sex;
	v->m_data_area[6].vchar = prof;
	v->m_data_area[7].vchar = camp;
	v->m_data_area[8].vint64 = gold;
	v->m_data_area[9].vint64 = gold_bind;
	v->m_data_area[10].vint64 = coin;
	v->m_data_area[11].vint64 = coin_bind;
	v->m_data_area[12].vint = guild_id;
	v->m_data_area[13].vchar = avatar;
	v->m_data_area[14].vint64 = avatar_timestamp;
	v->m_data_area[15].vchar = vip_type;
	v->m_data_area[16].vint = evil;
	v->m_data_area[17].vint64 = hp;
	v->m_data_area[18].vint64 = max_hp;
	v->m_data_area[19].vint64 = mp;
	v->m_data_area[20].vint64 = max_mp;
	v->m_data_area[21].vint64 = gongji;
	v->m_data_area[22].vint64 = fangyu;
	v->m_data_area[23].vint64 = mingzhong;
	v->m_data_area[24].vint64 = shanbi;
	v->m_data_area[25].vint64 = baoji;
	v->m_data_area[26].vint64 = jianren;
	v->m_data_area[27].vint64 = fujia_shanghai;
	v->m_data_area[28].vint64 = dikang_shanghai;
	v->m_data_area[29].vint64 = per_jingzhun;
	v->m_data_area[30].vint64 = per_baoji;
	v->m_data_area[31].vint64 = per_kangbao;
	v->m_data_area[32].vint64 = per_pofang;
	v->m_data_area[33].vint64 = per_mianshang;
	v->m_data_area[34].vint = capability;
	v->m_data_area[35].vint = max_capability;
	v->m_data_area[36].vint = lover_uid;
	v->m_data_area[37].length = (unsigned int)strlen(lover_name); 
	memcpy(v->m_data_area[37].vcharp, lover_name, v->m_data_area[37].length);
	v->m_data_area[38].vchar = authority_type;
	v->m_data_area[39].vint64 = forbid_talk_time;
	v->m_data_area[40].length = (unsigned int)strlen(sublock_pw); 
	memcpy(v->m_data_area[40].vcharp, sublock_pw, v->m_data_area[40].length);
	v->m_data_area[41].vint = rolechest_data_len;
	v->m_data_area[42].length = (unsigned int)strlen(mentality_data); 
	memcpy(v->m_data_area[42].vcharp, mentality_data, v->m_data_area[42].length);
	v->m_data_area[43].length = (unsigned int)strlen(vip_data); 
	memcpy(v->m_data_area[43].vcharp, vip_data, v->m_data_area[43].length);
	v->m_data_area[44].length = (unsigned int)strlen(active_degree_data); 
	memcpy(v->m_data_area[44].vcharp, active_degree_data, v->m_data_area[44].length);
	v->m_data_area[45].length = (unsigned int)strlen(roleshop_data); 
	memcpy(v->m_data_area[45].vcharp, roleshop_data, v->m_data_area[45].length);
	v->m_data_area[46].length = (unsigned int)strlen(welfare_data); 
	memcpy(v->m_data_area[46].vcharp, welfare_data, v->m_data_area[46].length);
	v->m_data_area[47].length = (unsigned int)strlen(rolehangup_data); 
	memcpy(v->m_data_area[47].vcharp, rolehangup_data, v->m_data_area[47].length);
	v->m_data_area[48].length = (unsigned int)strlen(rolecustom_data); 
	memcpy(v->m_data_area[48].vcharp, rolecustom_data, v->m_data_area[48].length);
	v->m_data_area[49].length = (unsigned int)strlen(itemcolddown_data); 
	memcpy(v->m_data_area[49].vcharp, itemcolddown_data, v->m_data_area[49].length);
	v->m_data_area[50].length = (unsigned int)strlen(evaluate_data); 
	memcpy(v->m_data_area[50].vcharp, evaluate_data, v->m_data_area[50].length);
	v->m_data_area[51].length = (unsigned int)strlen(monitor_data); 
	memcpy(v->m_data_area[51].vcharp, monitor_data, v->m_data_area[51].length);
	v->m_data_area[52].length = (unsigned int)strlen(title_data); 
	memcpy(v->m_data_area[52].vcharp, title_data, v->m_data_area[52].length);
	v->m_data_area[53].length = (unsigned int)strlen(husongtask_data); 
	memcpy(v->m_data_area[53].vcharp, husongtask_data, v->m_data_area[53].length);
	v->m_data_area[54].length = (unsigned int)strlen(roleactivity_data); 
	memcpy(v->m_data_area[54].vcharp, roleactivity_data, v->m_data_area[54].length);
	v->m_data_area[55].length = (unsigned int)strlen(fbtowerdefend_data); 
	memcpy(v->m_data_area[55].vcharp, fbtowerdefend_data, v->m_data_area[55].length);
	v->m_data_area[56].vint64 = last_single_chat_monitor_time;
	v->m_data_area[57].length = (unsigned int)strlen(last_single_chat_monitor_msg); 
	memcpy(v->m_data_area[57].vcharp, last_single_chat_monitor_msg, v->m_data_area[57].length);
	v->m_data_area[58].vint64 = last_world_chat_monitor_time;
	v->m_data_area[59].length = (unsigned int)strlen(last_world_chat_monitor_msg); 
	memcpy(v->m_data_area[59].vcharp, last_world_chat_monitor_msg, v->m_data_area[59].length);
	v->m_data_area[60].vint64 = last_mail_monitor_time;
	v->m_data_area[61].length = (unsigned int)strlen(last_mail_monitor_subject); 
	memcpy(v->m_data_area[61].vcharp, last_mail_monitor_subject, v->m_data_area[61].length);
	v->m_data_area[62].vint = today_online_time;
	v->m_data_area[63].vint = lastday_online_time;
	v->m_data_area[64].length = (unsigned int)strlen(common_data); 
	memcpy(v->m_data_area[64].vcharp, common_data, v->m_data_area[64].length);
	v->m_data_area[65].length = (unsigned int)strlen(wing_data); 
	memcpy(v->m_data_area[65].vcharp, wing_data, v->m_data_area[65].length);
}

REGISTER(role_attr_detail10);
void role_attr_detail10::Update(const DataAdapter& v)
{
	role_attr_detail10_id = v.m_data_area[0].vint;
	role_id = v.m_data_area[1].vint;
	orange_equip_collect_data = v.m_data_area[2].vcharp;
	yaoshi_data = v.m_data_area[3].vcharp;
	toushi_data = v.m_data_area[4].vcharp;
	qilinbi_data = v.m_data_area[5].vcharp;
	mask_data = v.m_data_area[6].vcharp;
	jinjiesys_reward_data = v.m_data_area[7].vcharp;
	role_big_small_goal_data = v.m_data_area[8].vcharp;
	data_page_len = v.m_data_area[9].vint;
	data_page = v.m_data_area[10].vcharp;
}
void role_attr_detail10::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint = role_attr_detail10_id;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].length = (unsigned int)strlen(orange_equip_collect_data); 
	memcpy(v->m_data_area[2].vcharp, orange_equip_collect_data, v->m_data_area[2].length);
	v->m_data_area[3].length = (unsigned int)strlen(yaoshi_data); 
	memcpy(v->m_data_area[3].vcharp, yaoshi_data, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(toushi_data); 
	memcpy(v->m_data_area[4].vcharp, toushi_data, v->m_data_area[4].length);
	v->m_data_area[5].length = (unsigned int)strlen(qilinbi_data); 
	memcpy(v->m_data_area[5].vcharp, qilinbi_data, v->m_data_area[5].length);
	v->m_data_area[6].length = (unsigned int)strlen(mask_data); 
	memcpy(v->m_data_area[6].vcharp, mask_data, v->m_data_area[6].length);
	v->m_data_area[7].length = (unsigned int)strlen(jinjiesys_reward_data); 
	memcpy(v->m_data_area[7].vcharp, jinjiesys_reward_data, v->m_data_area[7].length);
	v->m_data_area[8].length = (unsigned int)strlen(role_big_small_goal_data); 
	memcpy(v->m_data_area[8].vcharp, role_big_small_goal_data, v->m_data_area[8].length);
	v->m_data_area[9].vint = data_page_len;
	v->m_data_area[10].length = (unsigned int)strlen(data_page); 
	memcpy(v->m_data_area[10].vcharp, data_page, v->m_data_area[10].length);
}

REGISTER(role_attr_detail2);
void role_attr_detail2::Update(const DataAdapter& v)
{
	role_attr_detail2_id = v.m_data_area[0].vint;
	role_id = v.m_data_area[1].vint;
	rolechest_data = v.m_data_area[2].vcharp;
	effect_data = v.m_data_area[3].vcharp;
	equipfb_data = v.m_data_area[4].vcharp;
	guild_task_data = v.m_data_area[5].vcharp;
	newequipfb_data = v.m_data_area[6].vcharp;
	stone_data = v.m_data_area[7].vcharp;
	guild_xianshu_data = v.m_data_area[8].vcharp;
	taskrecord_data = v.m_data_area[9].vcharp;
	taskinfo_data = v.m_data_area[10].vcharp;
	role_supply_data = v.m_data_area[11].vcharp;
	maze_data = v.m_data_area[12].vcharp;
	xiannv_data = v.m_data_area[13].vcharp;
	story_fb_data = v.m_data_area[14].vcharp;
	convert_record_data = v.m_data_area[15].vcharp;
}
void role_attr_detail2::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint = role_attr_detail2_id;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].length = (unsigned int)strlen(rolechest_data); 
	memcpy(v->m_data_area[2].vcharp, rolechest_data, v->m_data_area[2].length);
	v->m_data_area[3].length = (unsigned int)strlen(effect_data); 
	memcpy(v->m_data_area[3].vcharp, effect_data, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(equipfb_data); 
	memcpy(v->m_data_area[4].vcharp, equipfb_data, v->m_data_area[4].length);
	v->m_data_area[5].length = (unsigned int)strlen(guild_task_data); 
	memcpy(v->m_data_area[5].vcharp, guild_task_data, v->m_data_area[5].length);
	v->m_data_area[6].length = (unsigned int)strlen(newequipfb_data); 
	memcpy(v->m_data_area[6].vcharp, newequipfb_data, v->m_data_area[6].length);
	v->m_data_area[7].length = (unsigned int)strlen(stone_data); 
	memcpy(v->m_data_area[7].vcharp, stone_data, v->m_data_area[7].length);
	v->m_data_area[8].length = (unsigned int)strlen(guild_xianshu_data); 
	memcpy(v->m_data_area[8].vcharp, guild_xianshu_data, v->m_data_area[8].length);
	v->m_data_area[9].length = (unsigned int)strlen(taskrecord_data); 
	memcpy(v->m_data_area[9].vcharp, taskrecord_data, v->m_data_area[9].length);
	v->m_data_area[10].length = (unsigned int)strlen(taskinfo_data); 
	memcpy(v->m_data_area[10].vcharp, taskinfo_data, v->m_data_area[10].length);
	v->m_data_area[11].length = (unsigned int)strlen(role_supply_data); 
	memcpy(v->m_data_area[11].vcharp, role_supply_data, v->m_data_area[11].length);
	v->m_data_area[12].length = (unsigned int)strlen(maze_data); 
	memcpy(v->m_data_area[12].vcharp, maze_data, v->m_data_area[12].length);
	v->m_data_area[13].length = (unsigned int)strlen(xiannv_data); 
	memcpy(v->m_data_area[13].vcharp, xiannv_data, v->m_data_area[13].length);
	v->m_data_area[14].length = (unsigned int)strlen(story_fb_data); 
	memcpy(v->m_data_area[14].vcharp, story_fb_data, v->m_data_area[14].length);
	v->m_data_area[15].length = (unsigned int)strlen(convert_record_data); 
	memcpy(v->m_data_area[15].vcharp, convert_record_data, v->m_data_area[15].length);
}

REGISTER(role_attr_detail3);
void role_attr_detail3::Update(const DataAdapter& v)
{
	role_attr_detail3_id = v.m_data_area[0].vint;
	role_id = v.m_data_area[1].vint;
	other_module_data = v.m_data_area[2].vcharp;
	qingyuan_data = v.m_data_area[3].vcharp;
	role_goal_data = v.m_data_area[4].vcharp;
	history_cap_data = v.m_data_area[5].vcharp;
	skill_other_data = v.m_data_area[6].vcharp;
	role_guild_data = v.m_data_area[7].vcharp;
	shengwang_data = v.m_data_area[8].vcharp;
	chengjiu_data = v.m_data_area[9].vcharp;
	dailyfb_data = v.m_data_area[10].vcharp;
	lifeskill_data = v.m_data_area[11].vcharp;
	baby_data = v.m_data_area[12].vcharp;
	shenge_system_data = v.m_data_area[13].vcharp;
	zero_gift_data = v.m_data_area[14].vcharp;
	mysterious_shop_in_mall_data = v.m_data_area[15].vcharp;
	friend_exp_bottle_data = v.m_data_area[16].vcharp;
	shenshou = v.m_data_area[17].vcharp;
	img_fuling_data = v.m_data_area[18].vcharp;
}
void role_attr_detail3::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint = role_attr_detail3_id;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].length = (unsigned int)strlen(other_module_data); 
	memcpy(v->m_data_area[2].vcharp, other_module_data, v->m_data_area[2].length);
	v->m_data_area[3].length = (unsigned int)strlen(qingyuan_data); 
	memcpy(v->m_data_area[3].vcharp, qingyuan_data, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(role_goal_data); 
	memcpy(v->m_data_area[4].vcharp, role_goal_data, v->m_data_area[4].length);
	v->m_data_area[5].length = (unsigned int)strlen(history_cap_data); 
	memcpy(v->m_data_area[5].vcharp, history_cap_data, v->m_data_area[5].length);
	v->m_data_area[6].length = (unsigned int)strlen(skill_other_data); 
	memcpy(v->m_data_area[6].vcharp, skill_other_data, v->m_data_area[6].length);
	v->m_data_area[7].length = (unsigned int)strlen(role_guild_data); 
	memcpy(v->m_data_area[7].vcharp, role_guild_data, v->m_data_area[7].length);
	v->m_data_area[8].length = (unsigned int)strlen(shengwang_data); 
	memcpy(v->m_data_area[8].vcharp, shengwang_data, v->m_data_area[8].length);
	v->m_data_area[9].length = (unsigned int)strlen(chengjiu_data); 
	memcpy(v->m_data_area[9].vcharp, chengjiu_data, v->m_data_area[9].length);
	v->m_data_area[10].length = (unsigned int)strlen(dailyfb_data); 
	memcpy(v->m_data_area[10].vcharp, dailyfb_data, v->m_data_area[10].length);
	v->m_data_area[11].length = (unsigned int)strlen(lifeskill_data); 
	memcpy(v->m_data_area[11].vcharp, lifeskill_data, v->m_data_area[11].length);
	v->m_data_area[12].length = (unsigned int)strlen(baby_data); 
	memcpy(v->m_data_area[12].vcharp, baby_data, v->m_data_area[12].length);
	v->m_data_area[13].length = (unsigned int)strlen(shenge_system_data); 
	memcpy(v->m_data_area[13].vcharp, shenge_system_data, v->m_data_area[13].length);
	v->m_data_area[14].length = (unsigned int)strlen(zero_gift_data); 
	memcpy(v->m_data_area[14].vcharp, zero_gift_data, v->m_data_area[14].length);
	v->m_data_area[15].length = (unsigned int)strlen(mysterious_shop_in_mall_data); 
	memcpy(v->m_data_area[15].vcharp, mysterious_shop_in_mall_data, v->m_data_area[15].length);
	v->m_data_area[16].length = (unsigned int)strlen(friend_exp_bottle_data); 
	memcpy(v->m_data_area[16].vcharp, friend_exp_bottle_data, v->m_data_area[16].length);
	v->m_data_area[17].length = (unsigned int)strlen(shenshou); 
	memcpy(v->m_data_area[17].vcharp, shenshou, v->m_data_area[17].length);
	v->m_data_area[18].length = (unsigned int)strlen(img_fuling_data); 
	memcpy(v->m_data_area[18].vcharp, img_fuling_data, v->m_data_area[18].length);
}

REGISTER(role_attr_detail4);
void role_attr_detail4::Update(const DataAdapter& v)
{
	role_attr_detail4_id = v.m_data_area[0].vint;
	role_id = v.m_data_area[1].vint;
	tuhaojin_data = v.m_data_area[2].vcharp;
	bigchatface_data = v.m_data_area[3].vcharp;
	pet_data = v.m_data_area[4].vcharp;
	phase_fb_data = v.m_data_area[5].vcharp;
	vip_fb_data = v.m_data_area[6].vcharp;
	huashen_data = v.m_data_area[7].vcharp;
	magic_card_data = v.m_data_area[8].vcharp;
	mitama_data = v.m_data_area[9].vcharp;
	equipment_data = v.m_data_area[10].vcharp;
	chinese_zodiac_data = v.m_data_area[11].vcharp;
	friend_gift = v.m_data_area[12].vcharp;
	follow_boss = v.m_data_area[13].vcharp;
	gold_vip = v.m_data_area[14].vcharp;
	rune_system = v.m_data_area[15].vcharp;
	magical_precious_data = v.m_data_area[16].vcharp;
}
void role_attr_detail4::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint = role_attr_detail4_id;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].length = (unsigned int)strlen(tuhaojin_data); 
	memcpy(v->m_data_area[2].vcharp, tuhaojin_data, v->m_data_area[2].length);
	v->m_data_area[3].length = (unsigned int)strlen(bigchatface_data); 
	memcpy(v->m_data_area[3].vcharp, bigchatface_data, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(pet_data); 
	memcpy(v->m_data_area[4].vcharp, pet_data, v->m_data_area[4].length);
	v->m_data_area[5].length = (unsigned int)strlen(phase_fb_data); 
	memcpy(v->m_data_area[5].vcharp, phase_fb_data, v->m_data_area[5].length);
	v->m_data_area[6].length = (unsigned int)strlen(vip_fb_data); 
	memcpy(v->m_data_area[6].vcharp, vip_fb_data, v->m_data_area[6].length);
	v->m_data_area[7].length = (unsigned int)strlen(huashen_data); 
	memcpy(v->m_data_area[7].vcharp, huashen_data, v->m_data_area[7].length);
	v->m_data_area[8].length = (unsigned int)strlen(magic_card_data); 
	memcpy(v->m_data_area[8].vcharp, magic_card_data, v->m_data_area[8].length);
	v->m_data_area[9].length = (unsigned int)strlen(mitama_data); 
	memcpy(v->m_data_area[9].vcharp, mitama_data, v->m_data_area[9].length);
	v->m_data_area[10].length = (unsigned int)strlen(equipment_data); 
	memcpy(v->m_data_area[10].vcharp, equipment_data, v->m_data_area[10].length);
	v->m_data_area[11].length = (unsigned int)strlen(chinese_zodiac_data); 
	memcpy(v->m_data_area[11].vcharp, chinese_zodiac_data, v->m_data_area[11].length);
	v->m_data_area[12].length = (unsigned int)strlen(friend_gift); 
	memcpy(v->m_data_area[12].vcharp, friend_gift, v->m_data_area[12].length);
	v->m_data_area[13].length = (unsigned int)strlen(follow_boss); 
	memcpy(v->m_data_area[13].vcharp, follow_boss, v->m_data_area[13].length);
	v->m_data_area[14].length = (unsigned int)strlen(gold_vip); 
	memcpy(v->m_data_area[14].vcharp, gold_vip, v->m_data_area[14].length);
	v->m_data_area[15].length = (unsigned int)strlen(rune_system); 
	memcpy(v->m_data_area[15].vcharp, rune_system, v->m_data_area[15].length);
	v->m_data_area[16].length = (unsigned int)strlen(magical_precious_data); 
	memcpy(v->m_data_area[16].vcharp, magical_precious_data, v->m_data_area[16].length);
}

REGISTER(role_attr_detail5);
void role_attr_detail5::Update(const DataAdapter& v)
{
	role_attr_detail5_id = v.m_data_area[0].vint;
	role_id = v.m_data_area[1].vint;
	multi_mount_data = v.m_data_area[2].vcharp;
	personalize_window_data = v.m_data_area[3].vcharp;
	xunzhang = v.m_data_area[4].vcharp;
	zhibao = v.m_data_area[5].vcharp;
	jingling = v.m_data_area[6].vcharp;
	xiannv_shouhu = v.m_data_area[7].vcharp;
	jingling_fazhen = v.m_data_area[8].vcharp;
	jingling_guanghuan = v.m_data_area[9].vcharp;
	shenzhou_weapon_data = v.m_data_area[10].vcharp;
	lieming = v.m_data_area[11].vcharp;
	lieming_bag = v.m_data_area[12].vcharp;
	cardzu = v.m_data_area[13].vcharp;
	zhuansheng_equip = v.m_data_area[14].vcharp;
	shenbing = v.m_data_area[15].vcharp;
	tuitufb_data = v.m_data_area[16].vcharp;
	cloak = v.m_data_area[17].vcharp;
	role_precious_boss = v.m_data_area[18].vcharp;
}
void role_attr_detail5::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint = role_attr_detail5_id;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].length = (unsigned int)strlen(multi_mount_data); 
	memcpy(v->m_data_area[2].vcharp, multi_mount_data, v->m_data_area[2].length);
	v->m_data_area[3].length = (unsigned int)strlen(personalize_window_data); 
	memcpy(v->m_data_area[3].vcharp, personalize_window_data, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(xunzhang); 
	memcpy(v->m_data_area[4].vcharp, xunzhang, v->m_data_area[4].length);
	v->m_data_area[5].length = (unsigned int)strlen(zhibao); 
	memcpy(v->m_data_area[5].vcharp, zhibao, v->m_data_area[5].length);
	v->m_data_area[6].length = (unsigned int)strlen(jingling); 
	memcpy(v->m_data_area[6].vcharp, jingling, v->m_data_area[6].length);
	v->m_data_area[7].length = (unsigned int)strlen(xiannv_shouhu); 
	memcpy(v->m_data_area[7].vcharp, xiannv_shouhu, v->m_data_area[7].length);
	v->m_data_area[8].length = (unsigned int)strlen(jingling_fazhen); 
	memcpy(v->m_data_area[8].vcharp, jingling_fazhen, v->m_data_area[8].length);
	v->m_data_area[9].length = (unsigned int)strlen(jingling_guanghuan); 
	memcpy(v->m_data_area[9].vcharp, jingling_guanghuan, v->m_data_area[9].length);
	v->m_data_area[10].length = (unsigned int)strlen(shenzhou_weapon_data); 
	memcpy(v->m_data_area[10].vcharp, shenzhou_weapon_data, v->m_data_area[10].length);
	v->m_data_area[11].length = (unsigned int)strlen(lieming); 
	memcpy(v->m_data_area[11].vcharp, lieming, v->m_data_area[11].length);
	v->m_data_area[12].length = (unsigned int)strlen(lieming_bag); 
	memcpy(v->m_data_area[12].vcharp, lieming_bag, v->m_data_area[12].length);
	v->m_data_area[13].length = (unsigned int)strlen(cardzu); 
	memcpy(v->m_data_area[13].vcharp, cardzu, v->m_data_area[13].length);
	v->m_data_area[14].length = (unsigned int)strlen(zhuansheng_equip); 
	memcpy(v->m_data_area[14].vcharp, zhuansheng_equip, v->m_data_area[14].length);
	v->m_data_area[15].length = (unsigned int)strlen(shenbing); 
	memcpy(v->m_data_area[15].vcharp, shenbing, v->m_data_area[15].length);
	v->m_data_area[16].length = (unsigned int)strlen(tuitufb_data); 
	memcpy(v->m_data_area[16].vcharp, tuitufb_data, v->m_data_area[16].length);
	v->m_data_area[17].length = (unsigned int)strlen(cloak); 
	memcpy(v->m_data_area[17].vcharp, cloak, v->m_data_area[17].length);
	v->m_data_area[18].length = (unsigned int)strlen(role_precious_boss); 
	memcpy(v->m_data_area[18].vcharp, role_precious_boss, v->m_data_area[18].length);
}

REGISTER(role_attr_detail6);
void role_attr_detail6::Update(const DataAdapter& v)
{
	role_attr_detail6_id = v.m_data_area[0].vint;
	role_id = v.m_data_area[1].vint;
	couple_mount_data = v.m_data_area[2].vcharp;
	shenyin_data = v.m_data_area[3].vcharp;
	element_heart_data = v.m_data_area[4].vcharp;
	shengxingzhuli = v.m_data_area[5].vcharp;
	shizhuang_data = v.m_data_area[6].vcharp;
	fabao_data = v.m_data_area[7].vcharp;
	feixian_data = v.m_data_area[8].vcharp;
	talent_data = v.m_data_area[9].vcharp;
	jinjie_talent_data = v.m_data_area[10].vcharp;
	armor_fb_data = v.m_data_area[11].vcharp;
	bosscard_data = v.m_data_area[12].vcharp;
	task_zhuanzhi_data = v.m_data_area[13].vcharp;
	discount_buy_data = v.m_data_area[14].vcharp;
}
void role_attr_detail6::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint = role_attr_detail6_id;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].length = (unsigned int)strlen(couple_mount_data); 
	memcpy(v->m_data_area[2].vcharp, couple_mount_data, v->m_data_area[2].length);
	v->m_data_area[3].length = (unsigned int)strlen(shenyin_data); 
	memcpy(v->m_data_area[3].vcharp, shenyin_data, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(element_heart_data); 
	memcpy(v->m_data_area[4].vcharp, element_heart_data, v->m_data_area[4].length);
	v->m_data_area[5].length = (unsigned int)strlen(shengxingzhuli); 
	memcpy(v->m_data_area[5].vcharp, shengxingzhuli, v->m_data_area[5].length);
	v->m_data_area[6].length = (unsigned int)strlen(shizhuang_data); 
	memcpy(v->m_data_area[6].vcharp, shizhuang_data, v->m_data_area[6].length);
	v->m_data_area[7].length = (unsigned int)strlen(fabao_data); 
	memcpy(v->m_data_area[7].vcharp, fabao_data, v->m_data_area[7].length);
	v->m_data_area[8].length = (unsigned int)strlen(feixian_data); 
	memcpy(v->m_data_area[8].vcharp, feixian_data, v->m_data_area[8].length);
	v->m_data_area[9].length = (unsigned int)strlen(talent_data); 
	memcpy(v->m_data_area[9].vcharp, talent_data, v->m_data_area[9].length);
	v->m_data_area[10].length = (unsigned int)strlen(jinjie_talent_data); 
	memcpy(v->m_data_area[10].vcharp, jinjie_talent_data, v->m_data_area[10].length);
	v->m_data_area[11].length = (unsigned int)strlen(armor_fb_data); 
	memcpy(v->m_data_area[11].vcharp, armor_fb_data, v->m_data_area[11].length);
	v->m_data_area[12].length = (unsigned int)strlen(bosscard_data); 
	memcpy(v->m_data_area[12].vcharp, bosscard_data, v->m_data_area[12].length);
	v->m_data_area[13].length = (unsigned int)strlen(task_zhuanzhi_data); 
	memcpy(v->m_data_area[13].vcharp, task_zhuanzhi_data, v->m_data_area[13].length);
	v->m_data_area[14].length = (unsigned int)strlen(discount_buy_data); 
	memcpy(v->m_data_area[14].vcharp, discount_buy_data, v->m_data_area[14].length);
}

REGISTER(role_attr_detail7);
void role_attr_detail7::Update(const DataAdapter& v)
{
	role_attr_detail7_id = v.m_data_area[0].vint;
	role_id = v.m_data_area[1].vint;
	cross_global_data = v.m_data_area[2].vcharp;
	zhuanzhi_equip_data = v.m_data_area[3].vcharp;
	role_cross_data = v.m_data_area[4].vcharp;
	heshenluoshu_data = v.m_data_area[5].vcharp;
	shengqi_data = v.m_data_area[6].vcharp;
	seal_data = v.m_data_area[7].vcharp;
	dressing_room_data = v.m_data_area[8].vcharp;
	challenge_fb_data = v.m_data_area[9].vcharp;
	role_cross_rand_activity_data = v.m_data_area[10].vcharp;
	cross_goal_data = v.m_data_area[11].vcharp;
}
void role_attr_detail7::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint = role_attr_detail7_id;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].length = (unsigned int)strlen(cross_global_data); 
	memcpy(v->m_data_area[2].vcharp, cross_global_data, v->m_data_area[2].length);
	v->m_data_area[3].length = (unsigned int)strlen(zhuanzhi_equip_data); 
	memcpy(v->m_data_area[3].vcharp, zhuanzhi_equip_data, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(role_cross_data); 
	memcpy(v->m_data_area[4].vcharp, role_cross_data, v->m_data_area[4].length);
	v->m_data_area[5].length = (unsigned int)strlen(heshenluoshu_data); 
	memcpy(v->m_data_area[5].vcharp, heshenluoshu_data, v->m_data_area[5].length);
	v->m_data_area[6].length = (unsigned int)strlen(shengqi_data); 
	memcpy(v->m_data_area[6].vcharp, shengqi_data, v->m_data_area[6].length);
	v->m_data_area[7].length = (unsigned int)strlen(seal_data); 
	memcpy(v->m_data_area[7].vcharp, seal_data, v->m_data_area[7].length);
	v->m_data_area[8].length = (unsigned int)strlen(dressing_room_data); 
	memcpy(v->m_data_area[8].vcharp, dressing_room_data, v->m_data_area[8].length);
	v->m_data_area[9].length = (unsigned int)strlen(challenge_fb_data); 
	memcpy(v->m_data_area[9].vcharp, challenge_fb_data, v->m_data_area[9].length);
	v->m_data_area[10].length = (unsigned int)strlen(role_cross_rand_activity_data); 
	memcpy(v->m_data_area[10].vcharp, role_cross_rand_activity_data, v->m_data_area[10].length);
	v->m_data_area[11].length = (unsigned int)strlen(cross_goal_data); 
	memcpy(v->m_data_area[11].vcharp, cross_goal_data, v->m_data_area[11].length);
}

REGISTER(role_attr_detail8);
void role_attr_detail8::Update(const DataAdapter& v)
{
	role_attr_detail8_id = v.m_data_area[0].vint;
	role_id = v.m_data_area[1].vint;
	upgrade_manager_data = v.m_data_area[2].vcharp;
	shenqi_data = v.m_data_area[3].vcharp;
	minghun2_data = v.m_data_area[4].vcharp;
	person_profess_data = v.m_data_area[5].vcharp;
	xianzunka_data = v.m_data_area[6].vcharp;
	equip_baptize_data = v.m_data_area[7].vcharp;
	mojie = v.m_data_area[8].vcharp;
	fb_personboss_data = v.m_data_area[9].vcharp;
	greate_soldier_data = v.m_data_area[10].vcharp;
	lottery_count_data = v.m_data_area[11].vcharp;
	upgrade_card_buy_data = v.m_data_area[12].vcharp;
	myth_data = v.m_data_area[13].vcharp;
	zodiac_data = v.m_data_area[14].vcharp;
	cross_equip_data = v.m_data_area[15].vcharp;
}
void role_attr_detail8::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint = role_attr_detail8_id;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].length = (unsigned int)strlen(upgrade_manager_data); 
	memcpy(v->m_data_area[2].vcharp, upgrade_manager_data, v->m_data_area[2].length);
	v->m_data_area[3].length = (unsigned int)strlen(shenqi_data); 
	memcpy(v->m_data_area[3].vcharp, shenqi_data, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(minghun2_data); 
	memcpy(v->m_data_area[4].vcharp, minghun2_data, v->m_data_area[4].length);
	v->m_data_area[5].length = (unsigned int)strlen(person_profess_data); 
	memcpy(v->m_data_area[5].vcharp, person_profess_data, v->m_data_area[5].length);
	v->m_data_area[6].length = (unsigned int)strlen(xianzunka_data); 
	memcpy(v->m_data_area[6].vcharp, xianzunka_data, v->m_data_area[6].length);
	v->m_data_area[7].length = (unsigned int)strlen(equip_baptize_data); 
	memcpy(v->m_data_area[7].vcharp, equip_baptize_data, v->m_data_area[7].length);
	v->m_data_area[8].length = (unsigned int)strlen(mojie); 
	memcpy(v->m_data_area[8].vcharp, mojie, v->m_data_area[8].length);
	v->m_data_area[9].length = (unsigned int)strlen(fb_personboss_data); 
	memcpy(v->m_data_area[9].vcharp, fb_personboss_data, v->m_data_area[9].length);
	v->m_data_area[10].length = (unsigned int)strlen(greate_soldier_data); 
	memcpy(v->m_data_area[10].vcharp, greate_soldier_data, v->m_data_area[10].length);
	v->m_data_area[11].length = (unsigned int)strlen(lottery_count_data); 
	memcpy(v->m_data_area[11].vcharp, lottery_count_data, v->m_data_area[11].length);
	v->m_data_area[12].length = (unsigned int)strlen(upgrade_card_buy_data); 
	memcpy(v->m_data_area[12].vcharp, upgrade_card_buy_data, v->m_data_area[12].length);
	v->m_data_area[13].length = (unsigned int)strlen(myth_data); 
	memcpy(v->m_data_area[13].vcharp, myth_data, v->m_data_area[13].length);
	v->m_data_area[14].length = (unsigned int)strlen(zodiac_data); 
	memcpy(v->m_data_area[14].vcharp, zodiac_data, v->m_data_area[14].length);
	v->m_data_area[15].length = (unsigned int)strlen(cross_equip_data); 
	memcpy(v->m_data_area[15].vcharp, cross_equip_data, v->m_data_area[15].length);
}

REGISTER(role_attr_detail9);
void role_attr_detail9::Update(const DataAdapter& v)
{
	role_attr_detail9_id = v.m_data_area[0].vint;
	role_id = v.m_data_area[1].vint;
	red_equip_collect_data = v.m_data_area[2].vcharp;
	mount_data = v.m_data_area[3].vcharp;
	footprint = v.m_data_area[4].vcharp;
	halo = v.m_data_area[5].vcharp;
	huguozhili = v.m_data_area[6].vcharp;
	baizhan_equip_data = v.m_data_area[7].vcharp;
	fight_mount_data = v.m_data_area[8].vcharp;
	mojiefb_data = v.m_data_area[9].vcharp;
	equipsuit_data = v.m_data_area[10].vcharp;
	shenzhuang = v.m_data_area[11].vcharp;
	csa_equip_data = v.m_data_area[12].vcharp;
	shen_gong = v.m_data_area[13].vcharp;
	shen_yi = v.m_data_area[14].vcharp;
	discountshop_data = v.m_data_area[15].vcharp;
}
void role_attr_detail9::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint = role_attr_detail9_id;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].length = (unsigned int)strlen(red_equip_collect_data); 
	memcpy(v->m_data_area[2].vcharp, red_equip_collect_data, v->m_data_area[2].length);
	v->m_data_area[3].length = (unsigned int)strlen(mount_data); 
	memcpy(v->m_data_area[3].vcharp, mount_data, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(footprint); 
	memcpy(v->m_data_area[4].vcharp, footprint, v->m_data_area[4].length);
	v->m_data_area[5].length = (unsigned int)strlen(halo); 
	memcpy(v->m_data_area[5].vcharp, halo, v->m_data_area[5].length);
	v->m_data_area[6].length = (unsigned int)strlen(huguozhili); 
	memcpy(v->m_data_area[6].vcharp, huguozhili, v->m_data_area[6].length);
	v->m_data_area[7].length = (unsigned int)strlen(baizhan_equip_data); 
	memcpy(v->m_data_area[7].vcharp, baizhan_equip_data, v->m_data_area[7].length);
	v->m_data_area[8].length = (unsigned int)strlen(fight_mount_data); 
	memcpy(v->m_data_area[8].vcharp, fight_mount_data, v->m_data_area[8].length);
	v->m_data_area[9].length = (unsigned int)strlen(mojiefb_data); 
	memcpy(v->m_data_area[9].vcharp, mojiefb_data, v->m_data_area[9].length);
	v->m_data_area[10].length = (unsigned int)strlen(equipsuit_data); 
	memcpy(v->m_data_area[10].vcharp, equipsuit_data, v->m_data_area[10].length);
	v->m_data_area[11].length = (unsigned int)strlen(shenzhuang); 
	memcpy(v->m_data_area[11].vcharp, shenzhuang, v->m_data_area[11].length);
	v->m_data_area[12].length = (unsigned int)strlen(csa_equip_data); 
	memcpy(v->m_data_area[12].vcharp, csa_equip_data, v->m_data_area[12].length);
	v->m_data_area[13].length = (unsigned int)strlen(shen_gong); 
	memcpy(v->m_data_area[13].vcharp, shen_gong, v->m_data_area[13].length);
	v->m_data_area[14].length = (unsigned int)strlen(shen_yi); 
	memcpy(v->m_data_area[14].vcharp, shen_yi, v->m_data_area[14].length);
	v->m_data_area[15].length = (unsigned int)strlen(discountshop_data); 
	memcpy(v->m_data_area[15].vcharp, discountshop_data, v->m_data_area[15].length);
}

REGISTER(role_id_map);
void role_id_map::Update(const DataAdapter& v)
{
	idrole_id_map = v.m_data_area[0].vint64;
	is_use = v.m_data_area[1].vint;
}
void role_id_map::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idrole_id_map;
	v->m_data_area[1].vint = is_use;
}

REGISTER(role_name_map);
void role_name_map::Update(const DataAdapter& v)
{
	idrole_name_map = v.m_data_area[0].vint64;
	db_index = v.m_data_area[1].vint;
	role_id = v.m_data_area[2].vint;
	role_name = v.m_data_area[3].vcharp;
	plat_user_name = v.m_data_area[4].vcharp;
}
void role_name_map::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idrole_name_map;
	v->m_data_area[1].vint = db_index;
	v->m_data_area[2].vint = role_id;
	v->m_data_area[3].length = (unsigned int)strlen(role_name); 
	memcpy(v->m_data_area[3].vcharp, role_name, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(plat_user_name); 
	memcpy(v->m_data_area[4].vcharp, plat_user_name, v->m_data_area[4].length);
}

REGISTER(roleskill);
void roleskill::Update(const DataAdapter& v)
{
	idroleskill = v.m_data_area[0].vint64;
	role_id = v.m_data_area[1].vint;
	skill_index = v.m_data_area[2].vint;
	FOR_INDEX = v.m_data_area[3].vint64;
	skill_level = v.m_data_area[4].vint;
	skill_id = v.m_data_area[5].vint;
	last_perform_time = v.m_data_area[6].vint64;
	skill_exp = v.m_data_area[7].vint;
}
void roleskill::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idroleskill;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].vint = skill_index;
	v->m_data_area[3].vint64 = FOR_INDEX;
	v->m_data_area[4].vint = skill_level;
	v->m_data_area[5].vint = skill_id;
	v->m_data_area[6].vint64 = last_perform_time;
	v->m_data_area[7].vint = skill_exp;
}

REGISTER(spacerecord);
void spacerecord::Update(const DataAdapter& v)
{
	id_record = v.m_data_area[0].vint64;
	record_unq_id = v.m_data_area[1].vint64;
	owner_id = v.m_data_area[2].vint;
	from_role_id = v.m_data_area[3].vint;
	to_role_id = v.m_data_area[4].vint;
	happen_timestamp = v.m_data_area[5].vint64;
	happen_space_uid = v.m_data_area[6].vint;
	type = v.m_data_area[7].vint;
	is_huifu = v.m_data_area[8].vint;
	msg = v.m_data_area[9].vcharp;
}
void spacerecord::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = id_record;
	v->m_data_area[1].vint64 = record_unq_id;
	v->m_data_area[2].vint = owner_id;
	v->m_data_area[3].vint = from_role_id;
	v->m_data_area[4].vint = to_role_id;
	v->m_data_area[5].vint64 = happen_timestamp;
	v->m_data_area[6].vint = happen_space_uid;
	v->m_data_area[7].vint = type;
	v->m_data_area[8].vint = is_huifu;
	v->m_data_area[9].length = (unsigned int)strlen(msg); 
	memcpy(v->m_data_area[9].vcharp, msg, v->m_data_area[9].length);
}

REGISTER(systemmail);
void systemmail::Update(const DataAdapter& v)
{
	systemmail_id = v.m_data_area[0].vint64;
	uid = v.m_data_area[1].vint;
	recv_time = v.m_data_area[2].vint64;
	kind = v.m_data_area[3].vint;
	is_read = v.m_data_area[4].vint;
	is_lock = v.m_data_area[5].vint;
	coin = v.m_data_area[6].vint;
	coin_bind = v.m_data_area[7].vint;
	gold = v.m_data_area[8].vint;
	gold_bind = v.m_data_area[9].vint;
	subject = v.m_data_area[10].vcharp;
	content = v.m_data_area[11].vcharp;
	item_id1 = v.m_data_area[12].vint;
	item_is_bind1 = v.m_data_area[13].vchar;
	item_num1 = v.m_data_area[14].vint;
	item_invalid_time1 = v.m_data_area[15].vint64;
	item_param1 = v.m_data_area[16].vcharp;
	item_id2 = v.m_data_area[17].vint;
	item_is_bind2 = v.m_data_area[18].vchar;
	item_num2 = v.m_data_area[19].vint;
	item_invalid_time2 = v.m_data_area[20].vint64;
	item_param2 = v.m_data_area[21].vcharp;
	item_id3 = v.m_data_area[22].vint;
	item_is_bind3 = v.m_data_area[23].vchar;
	item_num3 = v.m_data_area[24].vint;
	item_invalid_time3 = v.m_data_area[25].vint64;
	item_param3 = v.m_data_area[26].vcharp;
	virtual_item_data = v.m_data_area[27].vcharp;
}
void systemmail::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = systemmail_id;
	v->m_data_area[1].vint = uid;
	v->m_data_area[2].vint64 = recv_time;
	v->m_data_area[3].vint = kind;
	v->m_data_area[4].vint = is_read;
	v->m_data_area[5].vint = is_lock;
	v->m_data_area[6].vint = coin;
	v->m_data_area[7].vint = coin_bind;
	v->m_data_area[8].vint = gold;
	v->m_data_area[9].vint = gold_bind;
	v->m_data_area[10].length = (unsigned int)strlen(subject); 
	memcpy(v->m_data_area[10].vcharp, subject, v->m_data_area[10].length);
	v->m_data_area[11].length = (unsigned int)strlen(content); 
	memcpy(v->m_data_area[11].vcharp, content, v->m_data_area[11].length);
	v->m_data_area[12].vint = item_id1;
	v->m_data_area[13].vchar = item_is_bind1;
	v->m_data_area[14].vint = item_num1;
	v->m_data_area[15].vint64 = item_invalid_time1;
	v->m_data_area[16].length = (unsigned int)strlen(item_param1); 
	memcpy(v->m_data_area[16].vcharp, item_param1, v->m_data_area[16].length);
	v->m_data_area[17].vint = item_id2;
	v->m_data_area[18].vchar = item_is_bind2;
	v->m_data_area[19].vint = item_num2;
	v->m_data_area[20].vint64 = item_invalid_time2;
	v->m_data_area[21].length = (unsigned int)strlen(item_param2); 
	memcpy(v->m_data_area[21].vcharp, item_param2, v->m_data_area[21].length);
	v->m_data_area[22].vint = item_id3;
	v->m_data_area[23].vchar = item_is_bind3;
	v->m_data_area[24].vint = item_num3;
	v->m_data_area[25].vint64 = item_invalid_time3;
	v->m_data_area[26].length = (unsigned int)strlen(item_param3); 
	memcpy(v->m_data_area[26].vcharp, item_param3, v->m_data_area[26].length);
	v->m_data_area[27].length = (unsigned int)strlen(virtual_item_data); 
	memcpy(v->m_data_area[27].vcharp, virtual_item_data, v->m_data_area[27].length);
}

REGISTER(teamrank);
void teamrank::Update(const DataAdapter& v)
{
	idteamrank = v.m_data_area[0].vint64;
	rank_index = v.m_data_area[1].vint;
	uid1 = v.m_data_area[2].vint;
	uid2 = v.m_data_area[3].vint;
	uid3 = v.m_data_area[4].vint;
	user_name1 = v.m_data_area[5].vcharp;
	user_name2 = v.m_data_area[6].vcharp;
	user_name3 = v.m_data_area[7].vcharp;
	camp1 = v.m_data_area[8].vchar;
	camp2 = v.m_data_area[9].vchar;
	camp3 = v.m_data_area[10].vchar;
	rank_type = v.m_data_area[11].vint;
	rank_value = v.m_data_area[12].vint64;
	flexible_int = v.m_data_area[13].vint;
	flexible_ll = v.m_data_area[14].vint64;
}
void teamrank::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idteamrank;
	v->m_data_area[1].vint = rank_index;
	v->m_data_area[2].vint = uid1;
	v->m_data_area[3].vint = uid2;
	v->m_data_area[4].vint = uid3;
	v->m_data_area[5].length = (unsigned int)strlen(user_name1); 
	memcpy(v->m_data_area[5].vcharp, user_name1, v->m_data_area[5].length);
	v->m_data_area[6].length = (unsigned int)strlen(user_name2); 
	memcpy(v->m_data_area[6].vcharp, user_name2, v->m_data_area[6].length);
	v->m_data_area[7].length = (unsigned int)strlen(user_name3); 
	memcpy(v->m_data_area[7].vcharp, user_name3, v->m_data_area[7].length);
	v->m_data_area[8].vchar = camp1;
	v->m_data_area[9].vchar = camp2;
	v->m_data_area[10].vchar = camp3;
	v->m_data_area[11].vint = rank_type;
	v->m_data_area[12].vint64 = rank_value;
	v->m_data_area[13].vint = flexible_int;
	v->m_data_area[14].vint64 = flexible_ll;
}

REGISTER(tuodan);
void tuodan::Update(const DataAdapter& v)
{
	tuodan_id = v.m_data_area[0].vint64;
	role_id = v.m_data_area[1].vint;
	create_time = v.m_data_area[2].vint64;
	notice = v.m_data_area[3].vcharp;
}
void tuodan::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = tuodan_id;
	v->m_data_area[1].vint = role_id;
	v->m_data_area[2].vint64 = create_time;
	v->m_data_area[3].length = (unsigned int)strlen(notice); 
	memcpy(v->m_data_area[3].vcharp, notice, v->m_data_area[3].length);
}

REGISTER(worldstatus);
void worldstatus::Update(const DataAdapter& v)
{
	idworldstatus = v.m_data_area[0].vint64;
	FOR_INDEX = v.m_data_area[1].vint64;
	start_time = v.m_data_area[2].vint64;
	world_time = v.m_data_area[3].vint;
	last_update_dayid = v.m_data_area[4].vint64;
	activity_open_records = v.m_data_area[5].vcharp;
	xianmengzhan_mengzhu_id = v.m_data_area[6].vint;
	common_data = v.m_data_area[7].vcharp;
	challenge_field_data = v.m_data_area[8].vcharp;
	world_event_data = v.m_data_area[9].vcharp;
	csa_rank_data = v.m_data_area[10].vcharp;
	csa_other_data = v.m_data_area[11].vcharp;
	register_limit = v.m_data_area[12].vint;
	boss_refresh_data = v.m_data_area[13].vcharp;
	cross_guildbattle_data = v.m_data_area[14].vcharp;
	csa_boss_rank_data = v.m_data_area[15].vcharp;
	cross_rand_activity_data = v.m_data_area[16].vcharp;
	cross_rand_activity_big_data = v.m_data_area[17].vcharp;
	cross_rand_activity_big_data2 = v.m_data_area[18].vcharp;
	cross_rand_activity_cloud_purchase_data = v.m_data_area[19].vcharp;
}
void worldstatus::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idworldstatus;
	v->m_data_area[1].vint64 = FOR_INDEX;
	v->m_data_area[2].vint64 = start_time;
	v->m_data_area[3].vint = world_time;
	v->m_data_area[4].vint64 = last_update_dayid;
	v->m_data_area[5].length = (unsigned int)strlen(activity_open_records); 
	memcpy(v->m_data_area[5].vcharp, activity_open_records, v->m_data_area[5].length);
	v->m_data_area[6].vint = xianmengzhan_mengzhu_id;
	v->m_data_area[7].length = (unsigned int)strlen(common_data); 
	memcpy(v->m_data_area[7].vcharp, common_data, v->m_data_area[7].length);
	v->m_data_area[8].length = (unsigned int)strlen(challenge_field_data); 
	memcpy(v->m_data_area[8].vcharp, challenge_field_data, v->m_data_area[8].length);
	v->m_data_area[9].length = (unsigned int)strlen(world_event_data); 
	memcpy(v->m_data_area[9].vcharp, world_event_data, v->m_data_area[9].length);
	v->m_data_area[10].length = (unsigned int)strlen(csa_rank_data); 
	memcpy(v->m_data_area[10].vcharp, csa_rank_data, v->m_data_area[10].length);
	v->m_data_area[11].length = (unsigned int)strlen(csa_other_data); 
	memcpy(v->m_data_area[11].vcharp, csa_other_data, v->m_data_area[11].length);
	v->m_data_area[12].vint = register_limit;
	v->m_data_area[13].length = (unsigned int)strlen(boss_refresh_data); 
	memcpy(v->m_data_area[13].vcharp, boss_refresh_data, v->m_data_area[13].length);
	v->m_data_area[14].length = (unsigned int)strlen(cross_guildbattle_data); 
	memcpy(v->m_data_area[14].vcharp, cross_guildbattle_data, v->m_data_area[14].length);
	v->m_data_area[15].length = (unsigned int)strlen(csa_boss_rank_data); 
	memcpy(v->m_data_area[15].vcharp, csa_boss_rank_data, v->m_data_area[15].length);
	v->m_data_area[16].length = (unsigned int)strlen(cross_rand_activity_data); 
	memcpy(v->m_data_area[16].vcharp, cross_rand_activity_data, v->m_data_area[16].length);
	v->m_data_area[17].length = (unsigned int)strlen(cross_rand_activity_big_data); 
	memcpy(v->m_data_area[17].vcharp, cross_rand_activity_big_data, v->m_data_area[17].length);
	v->m_data_area[18].length = (unsigned int)strlen(cross_rand_activity_big_data2); 
	memcpy(v->m_data_area[18].vcharp, cross_rand_activity_big_data2, v->m_data_area[18].length);
	v->m_data_area[19].length = (unsigned int)strlen(cross_rand_activity_cloud_purchase_data); 
	memcpy(v->m_data_area[19].vcharp, cross_rand_activity_cloud_purchase_data, v->m_data_area[19].length);
}

REGISTER(worldstatus2);
void worldstatus2::Update(const DataAdapter& v)
{
	idworldstatus2 = v.m_data_area[0].vint64;
	FOR_INDEX = v.m_data_area[1].vint64;
	drop_log_list_data = v.m_data_area[2].vcharp;
	wedding_data = v.m_data_area[3].vcharp;
	common_data_2 = v.m_data_area[4].vcharp;
	global_profess_data = v.m_data_area[5].vcharp;
	drop_log_fb_data = v.m_data_area[6].vcharp;
	world_acquisition_log = v.m_data_area[7].vcharp;
}
void worldstatus2::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idworldstatus2;
	v->m_data_area[1].vint64 = FOR_INDEX;
	v->m_data_area[2].length = (unsigned int)strlen(drop_log_list_data); 
	memcpy(v->m_data_area[2].vcharp, drop_log_list_data, v->m_data_area[2].length);
	v->m_data_area[3].length = (unsigned int)strlen(wedding_data); 
	memcpy(v->m_data_area[3].vcharp, wedding_data, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(common_data_2); 
	memcpy(v->m_data_area[4].vcharp, common_data_2, v->m_data_area[4].length);
	v->m_data_area[5].length = (unsigned int)strlen(global_profess_data); 
	memcpy(v->m_data_area[5].vcharp, global_profess_data, v->m_data_area[5].length);
	v->m_data_area[6].length = (unsigned int)strlen(drop_log_fb_data); 
	memcpy(v->m_data_area[6].vcharp, drop_log_fb_data, v->m_data_area[6].length);
	v->m_data_area[7].length = (unsigned int)strlen(world_acquisition_log); 
	memcpy(v->m_data_area[7].vcharp, world_acquisition_log, v->m_data_area[7].length);
}

REGISTER(worldstatus3);
void worldstatus3::Update(const DataAdapter& v)
{
	idworldstatus3 = v.m_data_area[0].vint64;
	FOR_INDEX = v.m_data_area[1].vint64;
	common_data = v.m_data_area[2].vcharp;
	toprank_data = v.m_data_area[3].vcharp;
	lottery_count_data = v.m_data_area[4].vcharp;
	rand_activity_data = v.m_data_area[5].vcharp;
	lucky_cloud_buy_param_data = v.m_data_area[6].vcharp;
}
void worldstatus3::Save(DataAdapter *v) const
{
	v->m_data_area[0].vint64 = idworldstatus3;
	v->m_data_area[1].vint64 = FOR_INDEX;
	v->m_data_area[2].length = (unsigned int)strlen(common_data); 
	memcpy(v->m_data_area[2].vcharp, common_data, v->m_data_area[2].length);
	v->m_data_area[3].length = (unsigned int)strlen(toprank_data); 
	memcpy(v->m_data_area[3].vcharp, toprank_data, v->m_data_area[3].length);
	v->m_data_area[4].length = (unsigned int)strlen(lottery_count_data); 
	memcpy(v->m_data_area[4].vcharp, lottery_count_data, v->m_data_area[4].length);
	v->m_data_area[5].length = (unsigned int)strlen(rand_activity_data); 
	memcpy(v->m_data_area[5].vcharp, rand_activity_data, v->m_data_area[5].length);
	v->m_data_area[6].length = (unsigned int)strlen(lucky_cloud_buy_param_data); 
	memcpy(v->m_data_area[6].vcharp, lucky_cloud_buy_param_data, v->m_data_area[6].length);
}

}

