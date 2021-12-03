

#ifndef REGISTERTABLES_H
#define REGISTERTABLES_H

#include "dbopera/dataadapter.h"
#include "fastdb.h"
USE_FASTDB_NAMESPACE

namespace tabledef
{


class accountgold
{
public:
	int8 accountgold_id;
	const char* plat_user_name;
	int4 role_id;
	int8 gold;
	int8 gold_history;
	int8 last_get_gold_time;
	int4 authority_type;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return accountgold_id; }


	TYPE_DESCRIPTOR((
		KEY(accountgold_id, INDEXED),
		KEY(plat_user_name, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(gold),
		FIELD(gold_history),
		FIELD(last_get_gold_time),
		FIELD(authority_type)
		));

};


class accountidentity
{
public:
	int4 idAccountIdentity;
	const char* plat_user_name;
	const char* name;
	const char* identity_num;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idAccountIdentity; }


	TYPE_DESCRIPTOR((
		KEY(idAccountIdentity, INDEXED),
		FIELD(plat_user_name),
		FIELD(name),
		KEY(identity_num, INDEXED)
		));

};


class black_lists
{
public:
	int8 idblack;
	int4 role_id;
	int4 black_index;
	int8 FOR_INDEX;
	int4 black_uid;
	const char* black_name;
	int1 sex;
	int4 prof;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idblack; }


	TYPE_DESCRIPTOR((
		KEY(idblack, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(black_index),
		KEY(FOR_INDEX, INDEXED),
		FIELD(black_uid),
		FIELD(black_name),
		FIELD(sex),
		FIELD(prof)
		));

};


class camp
{
public:
	int4 idcamp;
	int4 camp_type;
	int4 role_count;
	int4 guild_count;
	int4 rich_level;
	int8 earnings_end_time1;
	int4 earnings1;
	int8 earnings_end_time2;
	int4 earnings2;
	int8 total_capability;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idcamp; }


	TYPE_DESCRIPTOR((
		KEY(idcamp, INDEXED),
		KEY(camp_type, INDEXED),
		FIELD(role_count),
		FIELD(guild_count),
		FIELD(rich_level),
		FIELD(earnings_end_time1),
		FIELD(earnings1),
		FIELD(earnings_end_time2),
		FIELD(earnings2),
		FIELD(total_capability)
		));

};


class challengeruser
{
public:
	int8 challenger_user_id;
	int8 role_id;
	int4 rankpos;
	const char* challenge_user_data;
	const char* common_data;
	const char* report_info_data;
	const char* role_appearance_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return challenger_user_id; }


	TYPE_DESCRIPTOR((
		KEY(challenger_user_id, INDEXED),
		FIELD(role_id),
		KEY(rankpos, INDEXED),
		FIELD(challenge_user_data),
		FIELD(common_data),
		FIELD(report_info_data),
		FIELD(role_appearance_data)
		));

};


class command
{
public:
	int4 idcommand;
	const char* creator;
	int8 createtime;
	int1 type;
	const char* cmd;
	int8 confirmtime;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idcommand; }


	TYPE_DESCRIPTOR((
		KEY(idcommand, INDEXED),
		FIELD(creator),
		FIELD(createtime),
		FIELD(type),
		FIELD(cmd),
		KEY(confirmtime, INDEXED)
		));

};


class cross_personrank
{
public:
	int8 idcrosspersonrank;
	int4 rank_index;
	int8 uuid;
	const char* user_name;
	int4 role_sex;
	int4 role_prof;
	int4 role_camp;
	int8 role_exp;
	int4 role_level;
	int4 rank_type;
	int8 rank_value;
	int4 record_index;
	int8 flexible_ll;
	const char* flexible_name;
	int4 flexible_int;
	int4 vip_level;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idcrosspersonrank; }


	TYPE_DESCRIPTOR((
		KEY(idcrosspersonrank, INDEXED),
		KEY(rank_index, INDEXED),
		FIELD(uuid),
		FIELD(user_name),
		FIELD(role_sex),
		FIELD(role_prof),
		FIELD(role_camp),
		FIELD(role_exp),
		FIELD(role_level),
		FIELD(rank_type),
		FIELD(rank_value),
		FIELD(record_index),
		FIELD(flexible_ll),
		FIELD(flexible_name),
		FIELD(flexible_int),
		FIELD(vip_level)
		));

};


class crossrecord
{
public:
	int8 idcrossrecord;
	int4 role_id;
	const char* role_name;
	int4 level;
	int1 sex;
	int1 prof;
	int1 camp;
	const char* record_data;
	const char* appearance_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idcrossrecord; }


	TYPE_DESCRIPTOR((
		KEY(idcrossrecord, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(role_name),
		FIELD(level),
		FIELD(sex),
		FIELD(prof),
		FIELD(camp),
		FIELD(record_data),
		FIELD(appearance_data)
		));

};


class crossstatus
{
public:
	int8 idcrossstatus;
	int8 FOR_INDEX;
	int4 last_update_dayid;
	const char* cross_person_rank_title_data;
	const char* cross_activity_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idcrossstatus; }


	TYPE_DESCRIPTOR((
		KEY(idcrossstatus, INDEXED),
		FIELD(FOR_INDEX),
		FIELD(last_update_dayid),
		FIELD(cross_person_rank_title_data),
		FIELD(cross_activity_data)
		));

};


class dailyfind
{
public:
	int8 iddailyfind;
	int4 role_id;
	int8 FOR_INDEX;
	int4 find_time;
	int4 role_level;
	int4 find_index;
	int4 param;
	int8 param1;
	int4 find_count;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return iddailyfind; }


	TYPE_DESCRIPTOR((
		KEY(iddailyfind, INDEXED),
		KEY(role_id, INDEXED),
		KEY(FOR_INDEX, INDEXED),
		FIELD(find_time),
		FIELD(role_level),
		FIELD(find_index),
		FIELD(param),
		FIELD(param1),
		FIELD(find_count)
		));

};


class enemy
{
public:
	int8 idenemy;
	int4 role_id;
	int8 FOR_INDEX;
	int4 enemy_uid;
	int4 enemy_plat_type;
	int4 kill_count;
	int8 last_kill_time;
	int4 enemy_index;
	int4 be_kill_count;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idenemy; }


	TYPE_DESCRIPTOR((
		KEY(idenemy, INDEXED),
		KEY(role_id, INDEXED),
		KEY(FOR_INDEX, INDEXED),
		FIELD(enemy_uid),
		FIELD(enemy_plat_type),
		FIELD(kill_count),
		FIELD(last_kill_time),
		FIELD(enemy_index),
		FIELD(be_kill_count)
		));

};


class fishpool
{
public:
	int8 idfishpool;
	int4 fish_role_id;
	const char* common_data;
	const char* raise_fish_data;
	const char* today_raise_record;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idfishpool; }


	TYPE_DESCRIPTOR((
		KEY(idfishpool, INDEXED),
		FIELD(fish_role_id),
		FIELD(common_data),
		FIELD(raise_fish_data),
		FIELD(today_raise_record)
		));

};


class friends
{
public:
	int8 idfriend;
	int4 role_id;
	int8 FOR_INDEX;
	int4 friend_uid;
	int4 intimacy;
	int4 team_today_intimacy;
	int4 friend_index;
	int4 gift_count;
	int4 favorable_impression;
	int4 day_chat_count;
	int8 add_timestamp;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idfriend; }


	TYPE_DESCRIPTOR((
		KEY(idfriend, INDEXED),
		KEY(role_id, INDEXED),
		KEY(FOR_INDEX, INDEXED),
		FIELD(friend_uid),
		FIELD(intimacy),
		FIELD(team_today_intimacy),
		FIELD(friend_index),
		FIELD(gift_count),
		FIELD(favorable_impression),
		FIELD(day_chat_count),
		FIELD(add_timestamp)
		));

};


class globalrole
{
public:
	int8 id_globalrole;
	int4 role_id;
	const char* little_pet_data;
	const char* love_tree_data;
	const char* love_contract_data;
	const char* jing_ling_home_data;
	const char* role_fighting_data;
	const char* qingyuan_global_data;
	const char* role_appearance_data;
	const char* common_data_global_data;
	const char* spousehome_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return id_globalrole; }


	TYPE_DESCRIPTOR((
		KEY(id_globalrole, INDEXED),
		FIELD(role_id),
		FIELD(little_pet_data),
		FIELD(love_tree_data),
		FIELD(love_contract_data),
		FIELD(jing_ling_home_data),
		FIELD(role_fighting_data),
		FIELD(qingyuan_global_data),
		FIELD(role_appearance_data),
		FIELD(common_data_global_data),
		FIELD(spousehome_data)
		));

};


class guild
{
public:
	int8 guild_id_inc;
	int4 guild_id;
	const char* guild_name;
	int8 create_time;
	int4 guild_level;
	int4 guild_exp;
	int4 camp;
	const char* notice;
	int8 last_update_dayid;
	int4 warning_dismiss_days;
	int4 applyfor_setup;
	int8 last_active_time;
	int4 auto_kickout_setup;
	int4 guild_max_capability;
	int4 applyfor_need_capability;
	int4 applyfor_need_level;
	int8 last_uplevel_timestamp;
	int4 callin_times;
	int4 today_monster_finish_seq;
	const char* today_monster_finish_username;
	const char* common_data;
	int4 active_degree;
	const char* boss_list;
	const char* storge;
	const char* enemy_list;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return guild_id_inc; }


	TYPE_DESCRIPTOR((
		KEY(guild_id_inc, INDEXED),
		KEY(guild_id, INDEXED),
		FIELD(guild_name),
		FIELD(create_time),
		FIELD(guild_level),
		FIELD(guild_exp),
		FIELD(camp),
		FIELD(notice),
		FIELD(last_update_dayid),
		FIELD(warning_dismiss_days),
		FIELD(applyfor_setup),
		FIELD(last_active_time),
		FIELD(auto_kickout_setup),
		FIELD(guild_max_capability),
		FIELD(applyfor_need_capability),
		FIELD(applyfor_need_level),
		FIELD(last_uplevel_timestamp),
		FIELD(callin_times),
		FIELD(today_monster_finish_seq),
		FIELD(today_monster_finish_username),
		FIELD(common_data),
		FIELD(active_degree),
		FIELD(boss_list),
		FIELD(storge),
		FIELD(enemy_list)
		));

};


class guildapplyfor
{
public:
	int8 idguildapplyfor;
	int4 guild_id;
	int4 apply_for_index;
	int8 FOR_INDEX;
	int4 uid;
	int8 applyfor_time;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idguildapplyfor; }


	TYPE_DESCRIPTOR((
		KEY(idguildapplyfor, INDEXED),
		KEY(guild_id, INDEXED),
		FIELD(apply_for_index),
		KEY(FOR_INDEX, INDEXED),
		FIELD(uid),
		FIELD(applyfor_time)
		));

};


class guildevent
{
public:
	int8 idguildevent;
	int4 guild_id;
	int4 event_index;
	int8 FOR_INDEX;
	int4 event_type;
	const char* event_owner;
	int4 event_owner_post;
	int8 event_time;
	int4 big_event;
	int4 cannot_remove;
	int4 param0;
	int4 param1;
	int4 param2;
	int4 param3;
	const char* sparam0;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idguildevent; }


	TYPE_DESCRIPTOR((
		KEY(idguildevent, INDEXED),
		KEY(guild_id, INDEXED),
		FIELD(event_index),
		KEY(FOR_INDEX, INDEXED),
		FIELD(event_type),
		FIELD(event_owner),
		FIELD(event_owner_post),
		FIELD(event_time),
		FIELD(big_event),
		FIELD(cannot_remove),
		FIELD(param0),
		FIELD(param1),
		FIELD(param2),
		FIELD(param3),
		FIELD(sparam0)
		));

};


class guildmember
{
public:
	int8 idguildmember;
	int4 guild_id;
	int4 member_index;
	int8 FOR_INDEX;
	int4 uid;
	int4 post;
	int4 gongxian;
	int4 total_gongxian;
	int8 join_time;
	int8 last_login_time;
	int4 lucky_color;
	int4 today_guild_pao_saizi_times;
	int8 today_last_guild_pao_saizi_time;
	int4 today_guild_saizi_score;
	int4 maze_layer;
	int8 maze_complete_time;
	const char* common_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idguildmember; }


	TYPE_DESCRIPTOR((
		KEY(idguildmember, INDEXED),
		KEY(guild_id, INDEXED),
		FIELD(member_index),
		KEY(FOR_INDEX, INDEXED),
		FIELD(uid),
		FIELD(post),
		FIELD(gongxian),
		FIELD(total_gongxian),
		FIELD(join_time),
		FIELD(last_login_time),
		FIELD(lucky_color),
		FIELD(today_guild_pao_saizi_times),
		FIELD(today_last_guild_pao_saizi_time),
		FIELD(today_guild_saizi_score),
		FIELD(maze_layer),
		FIELD(maze_complete_time),
		FIELD(common_data)
		));

};


class hotkey
{
public:
	int8 hotkey_id;
	int4 role_id;
	int4 hotkey_index;
	int8 FOR_INDEX;
	int1 hotkey_item_type;
	int4 hotkey_item_id;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return hotkey_id; }


	TYPE_DESCRIPTOR((
		KEY(hotkey_id, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(hotkey_index),
		KEY(FOR_INDEX, INDEXED),
		FIELD(hotkey_item_type),
		FIELD(hotkey_item_id)
		));

};


class itemlist
{
public:
	int8 iditemlist;
	int4 role_id;
	int4 itemlist_index;
	int8 FOR_INDEX;
	int4 item_id;
	int1 is_bind;
	int4 num;
	int8 invalid_time;
	int4 gold_price;
	const char* param;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return iditemlist; }


	TYPE_DESCRIPTOR((
		KEY(iditemlist, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(itemlist_index),
		KEY(FOR_INDEX, INDEXED),
		FIELD(item_id),
		FIELD(is_bind),
		FIELD(num),
		FIELD(invalid_time),
		FIELD(gold_price),
		FIELD(param)
		));

};


class login
{
public:
	int8 login_id;
	const char* plat_user_name;
	int4 db_index;
	int4 role_id_1;
	int4 role_id_2;
	int4 role_id_3;
	int8 lastlogintime;
	int8 createtime;
	int8 forbidtime;
	int1 is_anti_wallow;
	int8 forbid_time_1;
	int8 forbid_time_2;
	int8 forbid_time_3;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return login_id; }


	TYPE_DESCRIPTOR((
		KEY(login_id, INDEXED),
		KEY(plat_user_name, INDEXED),
		FIELD(db_index),
		FIELD(role_id_1),
		FIELD(role_id_2),
		FIELD(role_id_3),
		FIELD(lastlogintime),
		FIELD(createtime),
		FIELD(forbidtime),
		FIELD(is_anti_wallow),
		FIELD(forbid_time_1),
		FIELD(forbid_time_2),
		FIELD(forbid_time_3)
		));

};


class mail
{
public:
	int8 idmail;
	int4 uid;
	int4 mail_index;
	int8 FOR_INDEX;
	int4 sender_uid;
	const char* sender_name;
	int8 recv_time;
	int4 kind;
	int4 is_read;
	int4 is_lock;
	int4 coin;
	int4 coin_bind;
	int4 gold;
	int4 gold_bind;
	const char* subject;
	const char* content;
	int4 item_id1;
	int1 item_is_bind1;
	int4 item_num1;
	int8 item_invalid_time1;
	const char* item_param1;
	int4 item_id2;
	int1 item_is_bind2;
	int4 item_num2;
	int8 item_invalid_time2;
	const char* item_param2;
	int4 item_id3;
	int1 item_is_bind3;
	int4 item_num3;
	int8 item_invalid_time3;
	const char* item_param3;
	const char* virtual_item_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idmail; }


	TYPE_DESCRIPTOR((
		KEY(idmail, INDEXED),
		KEY(uid, INDEXED),
		FIELD(mail_index),
		KEY(FOR_INDEX, INDEXED),
		FIELD(sender_uid),
		FIELD(sender_name),
		FIELD(recv_time),
		FIELD(kind),
		FIELD(is_read),
		FIELD(is_lock),
		FIELD(coin),
		FIELD(coin_bind),
		FIELD(gold),
		FIELD(gold_bind),
		FIELD(subject),
		FIELD(content),
		FIELD(item_id1),
		FIELD(item_is_bind1),
		FIELD(item_num1),
		FIELD(item_invalid_time1),
		FIELD(item_param1),
		FIELD(item_id2),
		FIELD(item_is_bind2),
		FIELD(item_num2),
		FIELD(item_invalid_time2),
		FIELD(item_param2),
		FIELD(item_id3),
		FIELD(item_is_bind3),
		FIELD(item_num3),
		FIELD(item_invalid_time3),
		FIELD(item_param3),
		FIELD(virtual_item_data)
		));

};


class offlineregister
{
public:
	int8 idofflineregister;
	int4 role_id;
	int4 last_divorce_time;
	int4 increase_charm;
	int4 chongzhi_cmd_flag;
	int4 red_paper_consume_gold;
	int4 red_paper_lucky_value;
	int4 space_cai_conut;
	int4 space_gift_count;
	int4 qingyuan_bless_days;
	const char* guild_box_info;
	const char* common_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idofflineregister; }


	TYPE_DESCRIPTOR((
		KEY(idofflineregister, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(last_divorce_time),
		FIELD(increase_charm),
		FIELD(chongzhi_cmd_flag),
		FIELD(red_paper_consume_gold),
		FIELD(red_paper_lucky_value),
		FIELD(space_cai_conut),
		FIELD(space_gift_count),
		FIELD(qingyuan_bless_days),
		FIELD(guild_box_info),
		FIELD(common_data)
		));

};


class personrank
{
public:
	int8 idpersonrank;
	int4 rank_index;
	int4 uid;
	const char* user_name;
	const char* flexible_name;
	int4 role_sex;
	int4 role_prof;
	int4 role_camp;
	int4 role_vip_level;
	int8 role_exp;
	int4 role_level;
	int4 rank_type;
	int8 rank_value;
	int4 record_index;
	int4 flexible_int;
	int8 flexible_ll;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idpersonrank; }


	TYPE_DESCRIPTOR((
		KEY(idpersonrank, INDEXED),
		KEY(rank_index, INDEXED),
		FIELD(uid),
		FIELD(user_name),
		FIELD(flexible_name),
		FIELD(role_sex),
		FIELD(role_prof),
		FIELD(role_camp),
		FIELD(role_vip_level),
		FIELD(role_exp),
		FIELD(role_level),
		FIELD(rank_type),
		FIELD(rank_value),
		FIELD(record_index),
		FIELD(flexible_int),
		FIELD(flexible_ll)
		));

};


class publicnotice
{
public:
	int4 idpublicnotice;
	const char* creator;
	int8 createtime;
	int8 begin_send_time;
	int8 send_interval;
	int8 end_send_time;
	int8 next_send_time;
	const char* content;
	int4 type;
	int4 color;
	int4 is_forbid;
	int4 is_urgent;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idpublicnotice; }


	TYPE_DESCRIPTOR((
		KEY(idpublicnotice, INDEXED),
		FIELD(creator),
		FIELD(createtime),
		FIELD(begin_send_time),
		FIELD(send_interval),
		KEY(end_send_time, INDEXED),
		KEY(next_send_time, INDEXED),
		FIELD(content),
		FIELD(type),
		FIELD(color),
		FIELD(is_forbid),
		FIELD(is_urgent)
		));

};


class publicsale
{
public:
	int8 idpublicsale;
	int4 seller_uid;
	const char* seller_name;
	int4 sale_index;
	int8 FOR_INDEX;
	int4 item_id;
	int1 item_is_bind;
	int4 item_sale_type;
	int4 item_num;
	int4 color;
	int4 level;
	int4 prof;
	int4 equip_order;
	int4 gold_price;
	int4 sale_value;
	int4 price_type;
	int4 sale_item_type;
	int8 sale_time;
	int8 due_time;
	int8 item_invalid_time;
	const char* item_param;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idpublicsale; }


	TYPE_DESCRIPTOR((
		KEY(idpublicsale, INDEXED),
		KEY(seller_uid, INDEXED),
		FIELD(seller_name),
		FIELD(sale_index),
		KEY(FOR_INDEX, INDEXED),
		FIELD(item_id),
		FIELD(item_is_bind),
		FIELD(item_sale_type),
		FIELD(item_num),
		FIELD(color),
		FIELD(level),
		FIELD(prof),
		FIELD(equip_order),
		FIELD(gold_price),
		FIELD(sale_value),
		FIELD(price_type),
		FIELD(sale_item_type),
		FIELD(sale_time),
		FIELD(due_time),
		FIELD(item_invalid_time),
		FIELD(item_param)
		));

};


class role
{
public:
	int8 role_inc_id;
	int4 role_id;
	const char* role_name;
	int1 avatar;
	int1 sex;
	int4 scene_id;
	int4 last_scene_id;
	int4 level;
	int4 professional;
	int8 create_time;
	int4 online_time;
	int1 is_online;
	int8 last_save_time;
	int1 country;
	int1 is_chongzhi;
	int1 is_micro_pc;
	int4 plat_spid;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return role_inc_id; }


	TYPE_DESCRIPTOR((
		KEY(role_inc_id, INDEXED),
		KEY(role_id, INDEXED),
		KEY(role_name, INDEXED),
		FIELD(avatar),
		FIELD(sex),
		FIELD(scene_id),
		FIELD(last_scene_id),
		FIELD(level),
		FIELD(professional),
		FIELD(create_time),
		FIELD(online_time),
		FIELD(is_online),
		FIELD(last_save_time),
		FIELD(country),
		FIELD(is_chongzhi),
		FIELD(is_micro_pc),
		FIELD(plat_spid)
		));

};


class role_attr_detail
{
public:
	int4 role_attr_detail_id;
	int4 role_id;
	const char* attr_data;
	const char* role_name;
	int4 level;
	int1 sex;
	int1 prof;
	int1 camp;
	int8 gold;
	int8 gold_bind;
	int8 coin;
	int8 coin_bind;
	int4 guild_id;
	int1 avatar;
	int8 avatar_timestamp;
	int1 vip_type;
	int4 evil;
	int8 hp;
	int8 max_hp;
	int8 mp;
	int8 max_mp;
	int8 gongji;
	int8 fangyu;
	int8 mingzhong;
	int8 shanbi;
	int8 baoji;
	int8 jianren;
	int8 fujia_shanghai;
	int8 dikang_shanghai;
	int8 per_jingzhun;
	int8 per_baoji;
	int8 per_kangbao;
	int8 per_pofang;
	int8 per_mianshang;
	int4 capability;
	int4 max_capability;
	int4 lover_uid;
	const char* lover_name;
	int1 authority_type;
	int8 forbid_talk_time;
	const char* sublock_pw;
	int4 rolechest_data_len;
	const char* mentality_data;
	const char* vip_data;
	const char* active_degree_data;
	const char* roleshop_data;
	const char* welfare_data;
	const char* rolehangup_data;
	const char* rolecustom_data;
	const char* itemcolddown_data;
	const char* evaluate_data;
	const char* monitor_data;
	const char* title_data;
	const char* husongtask_data;
	const char* roleactivity_data;
	const char* fbtowerdefend_data;
	int8 last_single_chat_monitor_time;
	const char* last_single_chat_monitor_msg;
	int8 last_world_chat_monitor_time;
	const char* last_world_chat_monitor_msg;
	int8 last_mail_monitor_time;
	const char* last_mail_monitor_subject;
	int4 today_online_time;
	int4 lastday_online_time;
	const char* common_data;
	const char* wing_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return role_attr_detail_id; }


	TYPE_DESCRIPTOR((
		KEY(role_attr_detail_id, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(attr_data),
		FIELD(role_name),
		FIELD(level),
		FIELD(sex),
		KEY(prof, INDEXED),
		FIELD(camp),
		FIELD(gold),
		FIELD(gold_bind),
		FIELD(coin),
		FIELD(coin_bind),
		FIELD(guild_id),
		FIELD(avatar),
		FIELD(avatar_timestamp),
		FIELD(vip_type),
		FIELD(evil),
		FIELD(hp),
		FIELD(max_hp),
		FIELD(mp),
		FIELD(max_mp),
		FIELD(gongji),
		FIELD(fangyu),
		FIELD(mingzhong),
		FIELD(shanbi),
		FIELD(baoji),
		FIELD(jianren),
		FIELD(fujia_shanghai),
		FIELD(dikang_shanghai),
		FIELD(per_jingzhun),
		FIELD(per_baoji),
		FIELD(per_kangbao),
		FIELD(per_pofang),
		FIELD(per_mianshang),
		FIELD(capability),
		FIELD(max_capability),
		FIELD(lover_uid),
		FIELD(lover_name),
		FIELD(authority_type),
		FIELD(forbid_talk_time),
		FIELD(sublock_pw),
		FIELD(rolechest_data_len),
		FIELD(mentality_data),
		FIELD(vip_data),
		FIELD(active_degree_data),
		FIELD(roleshop_data),
		FIELD(welfare_data),
		FIELD(rolehangup_data),
		FIELD(rolecustom_data),
		FIELD(itemcolddown_data),
		FIELD(evaluate_data),
		FIELD(monitor_data),
		FIELD(title_data),
		FIELD(husongtask_data),
		FIELD(roleactivity_data),
		FIELD(fbtowerdefend_data),
		FIELD(last_single_chat_monitor_time),
		FIELD(last_single_chat_monitor_msg),
		FIELD(last_world_chat_monitor_time),
		FIELD(last_world_chat_monitor_msg),
		FIELD(last_mail_monitor_time),
		FIELD(last_mail_monitor_subject),
		FIELD(today_online_time),
		FIELD(lastday_online_time),
		FIELD(common_data),
		FIELD(wing_data)
		));

};


class role_attr_detail10
{
public:
	int4 role_attr_detail10_id;
	int4 role_id;
	const char* orange_equip_collect_data;
	const char* yaoshi_data;
	const char* toushi_data;
	const char* qilinbi_data;
	const char* mask_data;
	const char* jinjiesys_reward_data;
	const char* role_big_small_goal_data;
	int4 data_page_len;
	const char* data_page;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return role_attr_detail10_id; }


	TYPE_DESCRIPTOR((
		KEY(role_attr_detail10_id, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(orange_equip_collect_data),
		FIELD(yaoshi_data),
		FIELD(toushi_data),
		FIELD(qilinbi_data),
		FIELD(mask_data),
		FIELD(jinjiesys_reward_data),
		FIELD(role_big_small_goal_data),
		FIELD(data_page_len),
		FIELD(data_page)
		));

};


class role_attr_detail2
{
public:
	int4 role_attr_detail2_id;
	int4 role_id;
	const char* rolechest_data;
	const char* effect_data;
	const char* equipfb_data;
	const char* guild_task_data;
	const char* newequipfb_data;
	const char* stone_data;
	const char* guild_xianshu_data;
	const char* taskrecord_data;
	const char* taskinfo_data;
	const char* role_supply_data;
	const char* maze_data;
	const char* xiannv_data;
	const char* story_fb_data;
	const char* convert_record_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return role_attr_detail2_id; }


	TYPE_DESCRIPTOR((
		KEY(role_attr_detail2_id, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(rolechest_data),
		FIELD(effect_data),
		FIELD(equipfb_data),
		FIELD(guild_task_data),
		FIELD(newequipfb_data),
		FIELD(stone_data),
		FIELD(guild_xianshu_data),
		FIELD(taskrecord_data),
		FIELD(taskinfo_data),
		FIELD(role_supply_data),
		FIELD(maze_data),
		FIELD(xiannv_data),
		FIELD(story_fb_data),
		FIELD(convert_record_data)
		));

};


class role_attr_detail3
{
public:
	int4 role_attr_detail3_id;
	int4 role_id;
	const char* other_module_data;
	const char* qingyuan_data;
	const char* role_goal_data;
	const char* history_cap_data;
	const char* skill_other_data;
	const char* role_guild_data;
	const char* shengwang_data;
	const char* chengjiu_data;
	const char* dailyfb_data;
	const char* lifeskill_data;
	const char* baby_data;
	const char* shenge_system_data;
	const char* zero_gift_data;
	const char* mysterious_shop_in_mall_data;
	const char* friend_exp_bottle_data;
	const char* shenshou;
	const char* img_fuling_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return role_attr_detail3_id; }


	TYPE_DESCRIPTOR((
		KEY(role_attr_detail3_id, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(other_module_data),
		FIELD(qingyuan_data),
		FIELD(role_goal_data),
		FIELD(history_cap_data),
		FIELD(skill_other_data),
		FIELD(role_guild_data),
		FIELD(shengwang_data),
		FIELD(chengjiu_data),
		FIELD(dailyfb_data),
		FIELD(lifeskill_data),
		FIELD(baby_data),
		FIELD(shenge_system_data),
		FIELD(zero_gift_data),
		FIELD(mysterious_shop_in_mall_data),
		FIELD(friend_exp_bottle_data),
		FIELD(shenshou),
		FIELD(img_fuling_data)
		));

};


class role_attr_detail4
{
public:
	int4 role_attr_detail4_id;
	int4 role_id;
	const char* tuhaojin_data;
	const char* bigchatface_data;
	const char* pet_data;
	const char* phase_fb_data;
	const char* vip_fb_data;
	const char* huashen_data;
	const char* magic_card_data;
	const char* mitama_data;
	const char* equipment_data;
	const char* chinese_zodiac_data;
	const char* friend_gift;
	const char* follow_boss;
	const char* gold_vip;
	const char* rune_system;
	const char* magical_precious_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return role_attr_detail4_id; }


	TYPE_DESCRIPTOR((
		KEY(role_attr_detail4_id, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(tuhaojin_data),
		FIELD(bigchatface_data),
		FIELD(pet_data),
		FIELD(phase_fb_data),
		FIELD(vip_fb_data),
		FIELD(huashen_data),
		FIELD(magic_card_data),
		FIELD(mitama_data),
		FIELD(equipment_data),
		FIELD(chinese_zodiac_data),
		FIELD(friend_gift),
		FIELD(follow_boss),
		FIELD(gold_vip),
		FIELD(rune_system),
		FIELD(magical_precious_data)
		));

};


class role_attr_detail5
{
public:
	int4 role_attr_detail5_id;
	int4 role_id;
	const char* multi_mount_data;
	const char* personalize_window_data;
	const char* xunzhang;
	const char* zhibao;
	const char* jingling;
	const char* xiannv_shouhu;
	const char* jingling_fazhen;
	const char* jingling_guanghuan;
	const char* shenzhou_weapon_data;
	const char* lieming;
	const char* lieming_bag;
	const char* cardzu;
	const char* zhuansheng_equip;
	const char* shenbing;
	const char* tuitufb_data;
	const char* cloak;
	const char* role_precious_boss;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return role_attr_detail5_id; }


	TYPE_DESCRIPTOR((
		KEY(role_attr_detail5_id, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(multi_mount_data),
		FIELD(personalize_window_data),
		FIELD(xunzhang),
		FIELD(zhibao),
		FIELD(jingling),
		FIELD(xiannv_shouhu),
		FIELD(jingling_fazhen),
		FIELD(jingling_guanghuan),
		FIELD(shenzhou_weapon_data),
		FIELD(lieming),
		FIELD(lieming_bag),
		FIELD(cardzu),
		FIELD(zhuansheng_equip),
		FIELD(shenbing),
		FIELD(tuitufb_data),
		FIELD(cloak),
		FIELD(role_precious_boss)
		));

};


class role_attr_detail6
{
public:
	int4 role_attr_detail6_id;
	int4 role_id;
	const char* couple_mount_data;
	const char* shenyin_data;
	const char* element_heart_data;
	const char* shengxingzhuli;
	const char* shizhuang_data;
	const char* fabao_data;
	const char* feixian_data;
	const char* talent_data;
	const char* jinjie_talent_data;
	const char* armor_fb_data;
	const char* bosscard_data;
	const char* task_zhuanzhi_data;
	const char* discount_buy_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return role_attr_detail6_id; }


	TYPE_DESCRIPTOR((
		KEY(role_attr_detail6_id, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(couple_mount_data),
		FIELD(shenyin_data),
		FIELD(element_heart_data),
		FIELD(shengxingzhuli),
		FIELD(shizhuang_data),
		FIELD(fabao_data),
		FIELD(feixian_data),
		FIELD(talent_data),
		FIELD(jinjie_talent_data),
		FIELD(armor_fb_data),
		FIELD(bosscard_data),
		FIELD(task_zhuanzhi_data),
		FIELD(discount_buy_data)
		));

};


class role_attr_detail7
{
public:
	int4 role_attr_detail7_id;
	int4 role_id;
	const char* cross_global_data;
	const char* zhuanzhi_equip_data;
	const char* role_cross_data;
	const char* heshenluoshu_data;
	const char* shengqi_data;
	const char* seal_data;
	const char* dressing_room_data;
	const char* challenge_fb_data;
	const char* role_cross_rand_activity_data;
	const char* cross_goal_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return role_attr_detail7_id; }


	TYPE_DESCRIPTOR((
		KEY(role_attr_detail7_id, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(cross_global_data),
		FIELD(zhuanzhi_equip_data),
		FIELD(role_cross_data),
		FIELD(heshenluoshu_data),
		FIELD(shengqi_data),
		FIELD(seal_data),
		FIELD(dressing_room_data),
		FIELD(challenge_fb_data),
		FIELD(role_cross_rand_activity_data),
		FIELD(cross_goal_data)
		));

};


class role_attr_detail8
{
public:
	int4 role_attr_detail8_id;
	int4 role_id;
	const char* upgrade_manager_data;
	const char* shenqi_data;
	const char* minghun2_data;
	const char* person_profess_data;
	const char* xianzunka_data;
	const char* equip_baptize_data;
	const char* mojie;
	const char* fb_personboss_data;
	const char* greate_soldier_data;
	const char* lottery_count_data;
	const char* upgrade_card_buy_data;
	const char* myth_data;
	const char* zodiac_data;
	const char* cross_equip_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return role_attr_detail8_id; }


	TYPE_DESCRIPTOR((
		KEY(role_attr_detail8_id, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(upgrade_manager_data),
		FIELD(shenqi_data),
		FIELD(minghun2_data),
		FIELD(person_profess_data),
		FIELD(xianzunka_data),
		FIELD(equip_baptize_data),
		FIELD(mojie),
		FIELD(fb_personboss_data),
		FIELD(greate_soldier_data),
		FIELD(lottery_count_data),
		FIELD(upgrade_card_buy_data),
		FIELD(myth_data),
		FIELD(zodiac_data),
		FIELD(cross_equip_data)
		));

};


class role_attr_detail9
{
public:
	int4 role_attr_detail9_id;
	int4 role_id;
	const char* red_equip_collect_data;
	const char* mount_data;
	const char* footprint;
	const char* halo;
	const char* huguozhili;
	const char* baizhan_equip_data;
	const char* fight_mount_data;
	const char* mojiefb_data;
	const char* equipsuit_data;
	const char* shenzhuang;
	const char* csa_equip_data;
	const char* shen_gong;
	const char* shen_yi;
	const char* discountshop_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return role_attr_detail9_id; }


	TYPE_DESCRIPTOR((
		KEY(role_attr_detail9_id, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(red_equip_collect_data),
		FIELD(mount_data),
		FIELD(footprint),
		FIELD(halo),
		FIELD(huguozhili),
		FIELD(baizhan_equip_data),
		FIELD(fight_mount_data),
		FIELD(mojiefb_data),
		FIELD(equipsuit_data),
		FIELD(shenzhuang),
		FIELD(csa_equip_data),
		FIELD(shen_gong),
		FIELD(shen_yi),
		FIELD(discountshop_data)
		));

};


class role_id_map
{
public:
	int8 idrole_id_map;
	int4 is_use;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idrole_id_map; }


	TYPE_DESCRIPTOR((
		KEY(idrole_id_map, INDEXED),
		FIELD(is_use)
		));

};


class role_name_map
{
public:
	int8 idrole_name_map;
	int4 db_index;
	int4 role_id;
	const char* role_name;
	const char* plat_user_name;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idrole_name_map; }


	TYPE_DESCRIPTOR((
		KEY(idrole_name_map, INDEXED),
		FIELD(db_index),
		KEY(role_id, INDEXED),
		KEY(role_name, INDEXED),
		FIELD(plat_user_name)
		));

};


class roleskill
{
public:
	int8 idroleskill;
	int4 role_id;
	int4 skill_index;
	int8 FOR_INDEX;
	int4 skill_level;
	int4 skill_id;
	int8 last_perform_time;
	int4 skill_exp;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idroleskill; }


	TYPE_DESCRIPTOR((
		KEY(idroleskill, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(skill_index),
		KEY(FOR_INDEX, INDEXED),
		FIELD(skill_level),
		FIELD(skill_id),
		FIELD(last_perform_time),
		FIELD(skill_exp)
		));

};


class spacerecord
{
public:
	int8 id_record;
	int8 record_unq_id;
	int4 owner_id;
	int4 from_role_id;
	int4 to_role_id;
	int8 happen_timestamp;
	int4 happen_space_uid;
	int4 type;
	int4 is_huifu;
	const char* msg;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return id_record; }


	TYPE_DESCRIPTOR((
		KEY(id_record, INDEXED),
		KEY(record_unq_id, INDEXED),
		FIELD(owner_id),
		FIELD(from_role_id),
		FIELD(to_role_id),
		FIELD(happen_timestamp),
		FIELD(happen_space_uid),
		FIELD(type),
		FIELD(is_huifu),
		FIELD(msg)
		));

};


class systemmail
{
public:
	int8 systemmail_id;
	int4 uid;
	int8 recv_time;
	int4 kind;
	int4 is_read;
	int4 is_lock;
	int4 coin;
	int4 coin_bind;
	int4 gold;
	int4 gold_bind;
	const char* subject;
	const char* content;
	int4 item_id1;
	int1 item_is_bind1;
	int4 item_num1;
	int8 item_invalid_time1;
	const char* item_param1;
	int4 item_id2;
	int1 item_is_bind2;
	int4 item_num2;
	int8 item_invalid_time2;
	const char* item_param2;
	int4 item_id3;
	int1 item_is_bind3;
	int4 item_num3;
	int8 item_invalid_time3;
	const char* item_param3;
	const char* virtual_item_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return systemmail_id; }


	TYPE_DESCRIPTOR((
		KEY(systemmail_id, INDEXED),
		KEY(uid, INDEXED),
		FIELD(recv_time),
		FIELD(kind),
		FIELD(is_read),
		FIELD(is_lock),
		FIELD(coin),
		FIELD(coin_bind),
		FIELD(gold),
		FIELD(gold_bind),
		FIELD(subject),
		FIELD(content),
		FIELD(item_id1),
		FIELD(item_is_bind1),
		FIELD(item_num1),
		FIELD(item_invalid_time1),
		FIELD(item_param1),
		FIELD(item_id2),
		FIELD(item_is_bind2),
		FIELD(item_num2),
		FIELD(item_invalid_time2),
		FIELD(item_param2),
		FIELD(item_id3),
		FIELD(item_is_bind3),
		FIELD(item_num3),
		FIELD(item_invalid_time3),
		FIELD(item_param3),
		FIELD(virtual_item_data)
		));

};


class teamrank
{
public:
	int8 idteamrank;
	int4 rank_index;
	int4 uid1;
	int4 uid2;
	int4 uid3;
	const char* user_name1;
	const char* user_name2;
	const char* user_name3;
	int1 camp1;
	int1 camp2;
	int1 camp3;
	int4 rank_type;
	int8 rank_value;
	int4 flexible_int;
	int8 flexible_ll;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idteamrank; }


	TYPE_DESCRIPTOR((
		KEY(idteamrank, INDEXED),
		KEY(rank_index, INDEXED),
		FIELD(uid1),
		FIELD(uid2),
		FIELD(uid3),
		FIELD(user_name1),
		FIELD(user_name2),
		FIELD(user_name3),
		FIELD(camp1),
		FIELD(camp2),
		FIELD(camp3),
		FIELD(rank_type),
		FIELD(rank_value),
		FIELD(flexible_int),
		FIELD(flexible_ll)
		));

};


class tuodan
{
public:
	int8 tuodan_id;
	int4 role_id;
	int8 create_time;
	const char* notice;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return tuodan_id; }


	TYPE_DESCRIPTOR((
		KEY(tuodan_id, INDEXED),
		KEY(role_id, INDEXED),
		FIELD(create_time),
		FIELD(notice)
		));

};


class worldstatus
{
public:
	int8 idworldstatus;
	int8 FOR_INDEX;
	int8 start_time;
	int4 world_time;
	int8 last_update_dayid;
	const char* activity_open_records;
	int4 xianmengzhan_mengzhu_id;
	const char* common_data;
	const char* challenge_field_data;
	const char* world_event_data;
	const char* csa_rank_data;
	const char* csa_other_data;
	int4 register_limit;
	const char* boss_refresh_data;
	const char* cross_guildbattle_data;
	const char* csa_boss_rank_data;
	const char* cross_rand_activity_data;
	const char* cross_rand_activity_big_data;
	const char* cross_rand_activity_big_data2;
	const char* cross_rand_activity_cloud_purchase_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idworldstatus; }


	TYPE_DESCRIPTOR((
		KEY(idworldstatus, INDEXED),
		FIELD(FOR_INDEX),
		FIELD(start_time),
		FIELD(world_time),
		FIELD(last_update_dayid),
		FIELD(activity_open_records),
		FIELD(xianmengzhan_mengzhu_id),
		FIELD(common_data),
		FIELD(challenge_field_data),
		FIELD(world_event_data),
		FIELD(csa_rank_data),
		FIELD(csa_other_data),
		FIELD(register_limit),
		FIELD(boss_refresh_data),
		FIELD(cross_guildbattle_data),
		FIELD(csa_boss_rank_data),
		FIELD(cross_rand_activity_data),
		FIELD(cross_rand_activity_big_data),
		FIELD(cross_rand_activity_big_data2),
		FIELD(cross_rand_activity_cloud_purchase_data)
		));

};


class worldstatus2
{
public:
	int8 idworldstatus2;
	int8 FOR_INDEX;
	const char* drop_log_list_data;
	const char* wedding_data;
	const char* common_data_2;
	const char* global_profess_data;
	const char* drop_log_fb_data;
	const char* world_acquisition_log;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idworldstatus2; }


	TYPE_DESCRIPTOR((
		KEY(idworldstatus2, INDEXED),
		FIELD(FOR_INDEX),
		FIELD(drop_log_list_data),
		FIELD(wedding_data),
		FIELD(common_data_2),
		FIELD(global_profess_data),
		FIELD(drop_log_fb_data),
		FIELD(world_acquisition_log)
		));

};


class worldstatus3
{
public:
	int8 idworldstatus3;
	int8 FOR_INDEX;
	const char* common_data;
	const char* toprank_data;
	const char* lottery_count_data;
	const char* rand_activity_data;
	const char* lucky_cloud_buy_param_data;


	void Update(const DataAdapter& v);
	void Save(DataAdapter *v) const;
	long long GetId()const { return idworldstatus3; }


	TYPE_DESCRIPTOR((
		KEY(idworldstatus3, INDEXED),
		FIELD(FOR_INDEX),
		FIELD(common_data),
		FIELD(toprank_data),
		FIELD(lottery_count_data),
		FIELD(rand_activity_data),
		FIELD(lucky_cloud_buy_param_data)
		));

};

}
#endif

