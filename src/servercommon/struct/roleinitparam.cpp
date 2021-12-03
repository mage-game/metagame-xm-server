#include "roleinitparam.h"

#include "servercommon/servercommon.h"
#include "../roleattrvermanager/roleattrvermanager.h"

#include "servercommon/struct/mountparam.hpp"
#include "servercommon/struct/itemlistparam.h"

OLD_USE_INTERNAL_NETWORK();

bool RoleInitParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Pushf(
		"isbbbbbbiiiiiii"
		"iiikkkkkiiki"
		"kkkkkkkkkkkkkkkkkkiiiiiisbii"
		"isiiibbkkbbbs"
		"ibkkkkkkkk"
		"kiikkkkkkk"
		"kkkkkkkkkk",
//		"kkkkkkkkkkk",
		role_id, role_name, avatar, sex, camp, camp_post, prof, authority_type, level, scene_id, scene_x, scene_y, last_scene_id, last_scene_x, last_scene_y,
		online_time, today_online_time, lastday_online_time, create_time, coin, coin_bind, gold, gold_bind, knapsack_valid_num, storage_valid_num, forbid_talk_time, last_online_dayid,
		hp, max_hp, mp, max_mp, gongji, fangyu, mingzhong, shanbi, baoji, jianren, move_speed, fujia_shanghai, dikang_shanghai, per_jingzhun, per_baoji, per_kangbao, per_pofang, per_mianshang, last_kill_me_time, nv_wa_shi, lingjing, chengjiu, fenchen, guild_id, guild_name, guild_post, guild_gongxian, guild_total_gongxian,
		yuanli, day_count_data, all_give_flower_num, day_give_flower_num, all_charm, sublock_has_lock, sublock_today_error_times, exp, max_exp, attack_mode, move_mode, move_mode_param, sublock_pw,
		capability, is_change_avatar, gm_change_maxhp, gm_change_gongji, gm_change_movespeed, gm_change_fangyu, gm_chnage_mingzhong, gm_change_capability_attr, skill_zengshang, skill_jianshang,
		avatar_timestamp, evil, max_capability, per_baoji_hurt, per_kangbao_hurt, zhufuyiji_per, gedang_per, gedang_dikang_per, gedang_jianshang, shanbi_per,
		constant_zengshang, constant_mianshang, mingzhong_per, skill_cap_per, pvp_zengshang, pvp_mianshang, pve_zengshang, pve_mianshang, huixinyiji, huixinyiji_hurt_per);
// 		per_baoji_hurt, per_kangbao_hurt, zhufuyiji_per, gedang_per, gedang_dikang_per, gedang_jianshang, skill_zengshang, skill_jianshang, mingzhong_per, shanbi_per, skill_cap_per);
	return ret;
}

bool RoleInitParam::Unserialize(TLVUnserializer &instream)
{
	const char *rolename_tmp = NULL;
	const char *day_count_recorder_tmp = NULL;
	const char *sublock_pw_md5_tmp = NULL;
	const char *guild_name_tmp = NULL;

	bool ret = instream.Popf(
		"isbbbbbbiiiiiii"
		"iiikkkkkiiki"
		"kkkkkkkkkkkkkkkkkkiiiiiisbii"
		"isiiibbkkbbbs"
		"ibkkkkkkkk"
		"kiikkkkkkk"
		"kkkkkkkkkk",
//		"kkkkkkkkkkk",
		&role_id, &rolename_tmp, &avatar, &sex, &camp, &camp_post, &prof, &authority_type, &level, &scene_id, &scene_x, &scene_y, &last_scene_id, &last_scene_x, &last_scene_y,
		&online_time, &today_online_time, &lastday_online_time, &create_time, &coin, &coin_bind, &gold, &gold_bind, &knapsack_valid_num, &storage_valid_num, &forbid_talk_time, &last_online_dayid,
		&hp, &max_hp, &mp, &max_mp, &gongji, &fangyu, &mingzhong, &shanbi, &baoji, &jianren, &move_speed, &fujia_shanghai, &dikang_shanghai, &per_jingzhun, &per_baoji, &per_kangbao, &per_pofang, &per_mianshang, &last_kill_me_time, &nv_wa_shi, &lingjing, &chengjiu, &fenchen, &guild_id, &guild_name_tmp, &guild_post, &guild_gongxian, &guild_total_gongxian,
		&yuanli, &day_count_recorder_tmp, &all_give_flower_num, &day_give_flower_num, &all_charm, &sublock_has_lock, &sublock_today_error_times, &exp, &max_exp, &attack_mode, &move_mode, &move_mode_param, &sublock_pw_md5_tmp,
		&capability, &is_change_avatar, &gm_change_maxhp, &gm_change_gongji, &gm_change_movespeed, &gm_change_fangyu, &gm_chnage_mingzhong, &gm_change_capability_attr, &shanbi_per, &skill_cap_per,
		&avatar_timestamp, &evil, &max_capability, &per_baoji_hurt, &per_kangbao_hurt, &zhufuyiji_per, &gedang_per, &gedang_dikang_per, &gedang_jianshang, &skill_zengshang,
		&constant_zengshang, &constant_mianshang, &skill_jianshang, &mingzhong_per, &pvp_zengshang, &pvp_mianshang, &pve_zengshang, &pve_mianshang, &huixinyiji, &huixinyiji_hurt_per);
//		&per_baoji_hurt, &per_kangbao_hurt, &zhufuyiji_per, &gedang_per, &gedang_dikang_per, &gedang_jianshang, &skill_zengshang, &skill_jianshang, &mingzhong_per, &shanbi_per, &skill_cap_per);

	STRNCPY(role_name, rolename_tmp, sizeof(role_name));
	STRNCPY(day_count_data, day_count_recorder_tmp, sizeof(day_count_data));
	STRNCPY(sublock_pw, sublock_pw_md5_tmp, sizeof(sublock_pw));
	STRNCPY(guild_name, guild_name_tmp, sizeof(guild_name));
	
	return ret;
}

bool RoleInitParam::Serialize(TLVSerializer &outstream, bool ver_check) const
{
	if (ver_check && outstream.Push((short)RMIRoleAttrVerManager::RMI_ROLE_ATTR_CURRENT_VERSION)) 
	{
		return this->Serialize(outstream);
	}

	return false;
}

bool RoleInitParam::Unserialize(TLVUnserializer &instream, bool ver_check)
{
	short data_cur_ver = 0;

	if (ver_check && instream.Pop(&data_cur_ver))
	{
		return RMIRoleAttrVerManager::Instance().HandlerVerUnSerializer(data_cur_ver, *this, instream);
	}

	return false;
}
