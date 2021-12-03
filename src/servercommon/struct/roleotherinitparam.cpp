#include "roleotherinitparam.hpp"

#define UNSERIALIZE_DATA_EX(PARAM, LENGTH, LENGTH_PARAM) \
{ \
	TLVUnserializer tlv_data; \
	if (!instream.Pop(&tlv_data)) return false; \
	if (tlv_data.Size() <= LENGTH) \
	{ \
		memcpy(PARAM, tlv_data.Ptr(), tlv_data.Size()); \
		LENGTH_PARAM = tlv_data.Size(); \
	} \
}

inline bool SerializeAutoData(TLVSerializer &outstream, const DataPageParam &data_page)
{
	TLVSerializer data_page_stream;
	data_page_stream.Reset((void*)data_page.data, data_page.len);
	data_page_stream.MoveCurPos(data_page.len);

	// serialize data page to outstream
	if (!outstream.Push(data_page.len) || !outstream.Push(data_page_stream))
	{
		return false;
	}

	return true;
}

inline bool UnserializeAutoData(TLVUnserializer &instream, DataPageParam &data_page)
{
	// unserialize data page from instream
	if (!instream.Pop(&data_page.len))
	{
		return false;
	}

	TLVUnserializer data_page_stream;
	if (!instream.Pop(&data_page_stream))
	{
		return false;
	}

	if (sizeof(data_page.data) >= data_page_stream.Size())
	{
		memcpy((void*)data_page.data, data_page_stream.Ptr(), data_page_stream.Size());
	}
	else
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool RoleOtherInitParam::Serialize(TLVSerializer &outstream) const
{
	{
		const char *p_last_singlechat_monitor_msg = last_singlechat_monitor_msg;
		const char *p_last_worldchat_monitor_msg = last_worldchat_monitor_msg;
		const char *p_last_mail_monitor_subject = last_mail_monitor_subject;

		bool ret = outstream.Push(last_singlechat_monitor_time) &&
			outstream.Push(p_last_singlechat_monitor_msg) &&
			outstream.Push(last_worldchat_monitor_time) &&
			outstream.Push(p_last_worldchat_monitor_msg) &&
			outstream.Push(last_mail_monitor_time) &&
			outstream.Push(p_last_mail_monitor_subject);
		if (!ret)
		{
			return false;
		}
	}

	SERIALIZE_DATA(chest_buff, chest_buff_len);
	SERIALIZE_DATA(effect_buff, effect_buff_len);
	SERIALIZE_DATA(taskrecord_buff, taskrecord_buff_len);

	SERIALIZE_DATA(&mentality_param, sizeof(mentality_param));
	SERIALIZE_DATA(&vip_param, sizeof(vip_param));
	SERIALIZE_DATA(&roleshop_param, sizeof(roleshop_param));
	SERIALIZE_DATA(&active_degree_param, sizeof(active_degree_param));
	SERIALIZE_DATA(&welfare_param, sizeof(welfare_param));
	SERIALIZE_DATA(&rolehangup_param, sizeof(rolehangup_param));
	SERIALIZE_DATA(&itemcolddown_param, sizeof(itemcolddown_param));
	SERIALIZE_DATA(&evaluate_param, sizeof(evaluate_param));
	SERIALIZE_DATA(&monitor_param, sizeof(monitor_param));
	SERIALIZE_DATA(&title_param, sizeof(title_param));
	SERIALIZE_DATA(&husong_param, sizeof(husong_param));
	SERIALIZE_DATA(&roleactivity_param, sizeof(roleactivity_param));
	SERIALIZE_DATA(&towerdefend_param, sizeof(towerdefend_param));
	SERIALIZE_DATA(&common_data_param, sizeof(common_data_param));
	SERIALIZE_DATA(&wing_param, sizeof(wing_param));

	SERIALIZE_DATA(&equipfb_param, sizeof(equipfb_param));
	SERIALIZE_DATA(&guild_task_param, sizeof(guild_task_param));
	SERIALIZE_DATA(&newequipfb_param, sizeof(newequipfb_param));
	SERIALIZE_DATA(&stone_param, sizeof(stone_param));
	SERIALIZE_DATA(&guild_xianshu_param, sizeof(guild_xianshu_param));
	SERIALIZE_DATA(&task_param, sizeof(task_param));
	SERIALIZE_DATA(&role_supply_param, sizeof(role_supply_param));
	SERIALIZE_DATA(&maze_param, sizeof(maze_param));
	SERIALIZE_DATA(&xiannv_param, sizeof(xiannv_param));
	SERIALIZE_DATA(&storyfb_param, sizeof(storyfb_param));
	SERIALIZE_DATA(&challengefb_param, sizeof(challengefb_param));
	SERIALIZE_DATA(&convert_record_param, sizeof(convert_record_param));
	SERIALIZE_DATA(&fight_mount_param, sizeof(fight_mount_param));
	SERIALIZE_DATA(&mojiefb_param, sizeof(mojiefb_param));
	SERIALIZE_DATA(&equipsuit_param, sizeof(equipsuit_param));
	SERIALIZE_DATA(&shenzhuang_param, sizeof(shenzhuang_param));
	SERIALIZE_DATA(&csa_equip_param, sizeof(csa_equip_param));

	SERIALIZE_DATA(&discount_buy_param, sizeof(discount_buy_param));
	SERIALIZE_DATA(&other_module_param, sizeof(other_module_param));
	SERIALIZE_DATA(&qingyuan_param, sizeof(qingyuan_param));
	SERIALIZE_DATA(&role_goal_param, sizeof(role_goal_param));
	SERIALIZE_DATA(&shizhuang_param, sizeof(shizhuang_param));
	SERIALIZE_DATA(&history_camp_param, sizeof(history_camp_param));
	SERIALIZE_DATA(&skill_other_param, sizeof(skill_other_param));
	SERIALIZE_DATA(&role_cross_param, sizeof(role_cross_param));
	SERIALIZE_DATA(&role_guild_param, sizeof(role_guild_param));
	SERIALIZE_DATA(&shengwang_param, sizeof(shengwang_param));
	SERIALIZE_DATA(&chengjiu_param, sizeof(chengjiu_param));
	SERIALIZE_DATA(&dailyfb_param, sizeof(dailyfb_param));
	SERIALIZE_DATA(&lifeskill_param, sizeof(lifeskill_param));
	SERIALIZE_DATA(&baby_param, sizeof(baby_param));
	SERIALIZE_DATA(&shenge_system_param, sizeof(shenge_system_param));
	SERIALIZE_DATA(&zero_gift_param, sizeof(zero_gift_param));
	SERIALIZE_DATA(&mysterious_shop_in_mall_param, sizeof(mysterious_shop_in_mall_param));
	SERIALIZE_DATA(&friend_exp_bottle_param, sizeof(friend_exp_bottle_param));
	SERIALIZE_DATA(&shenshou_param, sizeof(shenshou_param));
	SERIALIZE_DATA(&img_fuling_param, sizeof(img_fuling_param));

	SERIALIZE_DATA(&tuhaojin_param, sizeof(tuhaojin_param));
	SERIALIZE_DATA(&bigchatface_param, sizeof(bigchatface_param));
	SERIALIZE_DATA(&pet_param, sizeof(pet_param));
	SERIALIZE_DATA(&phase_fb_param, sizeof(phase_fb_param));
	SERIALIZE_DATA(&vipfb_param, sizeof(vipfb_param));
	SERIALIZE_DATA(&huashen_param, sizeof(huashen_param));
	SERIALIZE_DATA(&magic_card_param, sizeof(magic_card_param));
	SERIALIZE_DATA(&mitama_param, sizeof(mitama_param));
	SERIALIZE_DATA(&equipment_param, sizeof(equipment_param));
	SERIALIZE_DATA(&chinese_zodiac_param, sizeof(chinese_zodiac_param));
	SERIALIZE_DATA(&friend_gift_param, sizeof(friend_gift_param));
	SERIALIZE_DATA(&follow_boss_param, sizeof(follow_boss_param));
	SERIALIZE_DATA(&gold_vip_param, sizeof(gold_vip_param));
	SERIALIZE_DATA(&rune_system_param, sizeof(rune_system_param));
	SERIALIZE_DATA(&magical_precious_param, sizeof(magical_precious_param));
	SERIALIZE_DATA(&feixian_param, sizeof(feixian_param));
	
	SERIALIZE_DATA(&multi_mount_param, sizeof(multi_mount_param));
	SERIALIZE_DATA(&personalize_window_param, sizeof(personalize_window_param));
	SERIALIZE_DATA(&xunzhang_param, sizeof(xunzhang_param));
	SERIALIZE_DATA(&zhibao_param, sizeof(zhibao_param));
	SERIALIZE_DATA(&shengong_param, sizeof(shengong_param));
	SERIALIZE_DATA(&shenyi_param, sizeof(shenyi_param));
	SERIALIZE_DATA(&jingling_param, sizeof(jingling_param));
	SERIALIZE_DATA(&xiannv_shouhu_param, sizeof(xiannv_shouhu_param));
	SERIALIZE_DATA(&jingling_fazhen_param, sizeof(jingling_fazhen_param));
	SERIALIZE_DATA(&jingling_guanghuan_param, sizeof(jingling_guanghuan_param));
	SERIALIZE_DATA(&shenzhou_weapon_param, sizeof(shenzhou_weapon_param));
	SERIALIZE_DATA(&lieming_param, sizeof(lieming_param));
	SERIALIZE_DATA(&lieming_bag_param, sizeof(lieming_bag_param));
	SERIALIZE_DATA(&cardzu_param, sizeof(cardzu_param));
	SERIALIZE_DATA(&zhuansheng_equip_param, sizeof(zhuansheng_equip_param));
	SERIALIZE_DATA(&mojie_param, sizeof(mojie_param));
	SERIALIZE_DATA(&shenbing_param, sizeof(shenbing_param));
	SERIALIZE_DATA(&tuitu_fb_param, sizeof(tuitu_fb_param));
	SERIALIZE_DATA(&cloak_param, sizeof(cloak_param));
	SERIALIZE_DATA(&precious_boss_param, sizeof(precious_boss_param));
	SERIALIZE_DATA(&couple_mount_param, sizeof(couple_mount_param));
	SERIALIZE_DATA(&shenyin_param, sizeof(shenyin_param));
	SERIALIZE_DATA(&element_heart_param, sizeof(element_heart_param));
	SERIALIZE_DATA(&shengxing_zhuli_param, sizeof(shengxing_zhuli_param));
	SERIALIZE_DATA(&fabao_param, sizeof(fabao_param));
	SERIALIZE_DATA(&talent_param, sizeof(talent_param));
	SERIALIZE_DATA(&jinjie_talent_param, sizeof(jinjie_talent_param));
	SERIALIZE_DATA(&armor_fb_param, sizeof(armor_fb_param));
	SERIALIZE_DATA(&bosscard_param, sizeof(bosscard_param));
	SERIALIZE_DATA(&task_zhuanzhi_param, sizeof(task_zhuanzhi_param));
	SERIALIZE_DATA(&cross_global_data_param, sizeof(cross_global_data_param));
	SERIALIZE_DATA(&yaoshi_param, sizeof(yaoshi_param));
	SERIALIZE_DATA(&toushi_param, sizeof(toushi_param));
	SERIALIZE_DATA(&qilinbi_param, sizeof(qilinbi_param));
	SERIALIZE_DATA(&mask_param, sizeof(mask_param));
	SERIALIZE_DATA(&zhuanzhi_equip_param, sizeof(zhuanzhi_equip_param));
	SERIALIZE_DATA(&heshen_luoshu_param, sizeof(heshen_luoshu_param));
	SERIALIZE_DATA(&shengqi_param, sizeof(shengqi_param));
	SERIALIZE_DATA(&seal_param, sizeof(seal_param));
	SERIALIZE_DATA(&dressing_room_param, sizeof(dressing_room_param));
	SERIALIZE_DATA(&upgrade_manager_param, sizeof(upgrade_manager_param));
	SERIALIZE_DATA(&jinjiesys_reward_param, sizeof(jinjiesys_reward_param));
	SERIALIZE_DATA(&role_big_small_goal_param, sizeof(role_big_small_goal_param));
	SERIALIZE_DATA(&shenqi_param, sizeof(shenqi_param));
	SERIALIZE_DATA(&lieming_param2, sizeof(lieming_param2));
	SERIALIZE_DATA(&profess_param, sizeof(profess_param));
	SERIALIZE_DATA(&red_equip_collect_param, sizeof(red_equip_collect_param));
	SERIALIZE_DATA(&mount_param, sizeof(mount_param));
	SERIALIZE_DATA(&footprint_param, sizeof(footprint_param));
	SERIALIZE_DATA(&halo_param, sizeof(halo_param));
	SERIALIZE_DATA(&orange_equip_collect_param, sizeof(orange_equip_collect_param));
	SERIALIZE_DATA(&xianzunka_param, sizeof(xianzunka_param));
	SERIALIZE_DATA(&equipbaptize_param, sizeof(equipbaptize_param));
	SERIALIZE_DATA(&personboss_param, sizeof(personboss_param));
	SERIALIZE_DATA(&greate_soldier_param, sizeof(greate_soldier_param));
	SERIALIZE_DATA(&role_lottery_count_param, sizeof(role_lottery_count_param));
	SERIALIZE_DATA(&role_cross_rand_activity_param, sizeof(role_cross_rand_activity_param));
	SERIALIZE_DATA(&upgrade_card_buy_param, sizeof(upgrade_card_buy_param));
	SERIALIZE_DATA(&huguozhili_param, sizeof(huguozhili_param));
	SERIALIZE_DATA(&cross_goal_param, sizeof(cross_goal_param));
	SERIALIZE_DATA(&baizhan_equip_param, sizeof(baizhan_equip_param));
	SERIALIZE_DATA(&discountbuy_param, sizeof(discountbuy_param));
	SERIALIZE_DATA(&myth_param, sizeof(myth_param));
	SERIALIZE_DATA(&zodiac_param, sizeof(zodiac_param));
	SERIALIZE_DATA(&cross_equip_param, sizeof(cross_equip_param));

	// ±í10 BLOB×Ö¶Î
	if (!SerializeAutoData(outstream, data_page_10)) return false;
	
	return true;
}

bool RoleOtherInitParam::Unserialize(TLVUnserializer &instream)
{
	{
		const char *p_last_singlechat_monitor_msg = NULL;
		const char *p_last_worldchat_monitor_msg = NULL;
		const char *p_last_mail_monitor_subject = NULL;

		bool ret = instream.Pop(&last_singlechat_monitor_time) &&
			instream.Pop(&p_last_singlechat_monitor_msg) &&
			instream.Pop(&last_worldchat_monitor_time) &&
			instream.Pop(&p_last_worldchat_monitor_msg) &&
			instream.Pop(&last_mail_monitor_time) &&
			instream.Pop(&p_last_mail_monitor_subject);
		if (!ret)
		{
			return false;
		}

		STRNCPY(last_singlechat_monitor_msg, p_last_singlechat_monitor_msg, sizeof(last_singlechat_monitor_msg));
		STRNCPY(last_worldchat_monitor_msg, p_last_worldchat_monitor_msg, sizeof(last_worldchat_monitor_msg));
		STRNCPY(last_mail_monitor_subject, p_last_mail_monitor_subject, sizeof(last_mail_monitor_subject));
	}

	UNSERIALIZE_DATA_EX(chest_buff, sizeof(chest_buff), chest_buff_len);
	UNSERIALIZE_DATA_EX(effect_buff, sizeof(effect_buff), effect_buff_len);
	UNSERIALIZE_DATA_EX(taskrecord_buff, sizeof(taskrecord_buff), taskrecord_buff_len);

	UNSERIALIZE_DATA(&mentality_param, sizeof(mentality_param));
	UNSERIALIZE_DATA(&vip_param, sizeof(vip_param));
	UNSERIALIZE_DATA(&roleshop_param, sizeof(roleshop_param));
	UNSERIALIZE_DATA(&active_degree_param, sizeof(active_degree_param));
	UNSERIALIZE_DATA(&welfare_param, sizeof(welfare_param));
	UNSERIALIZE_DATA(&rolehangup_param, sizeof(rolehangup_param));
	UNSERIALIZE_DATA(&itemcolddown_param, sizeof(itemcolddown_param));
	UNSERIALIZE_DATA(&evaluate_param, sizeof(evaluate_param));
	UNSERIALIZE_DATA(&monitor_param, sizeof(monitor_param));
	UNSERIALIZE_DATA(&title_param, sizeof(title_param));
	UNSERIALIZE_DATA(&husong_param, sizeof(husong_param));
	UNSERIALIZE_DATA(&roleactivity_param, sizeof(roleactivity_param));
	UNSERIALIZE_DATA(&towerdefend_param, sizeof(towerdefend_param));
	UNSERIALIZE_DATA(&common_data_param, sizeof(common_data_param));
	UNSERIALIZE_DATA(&wing_param, sizeof(wing_param));

	UNSERIALIZE_DATA(&equipfb_param, sizeof(equipfb_param));
	UNSERIALIZE_DATA(&guild_task_param, sizeof(guild_task_param));
	UNSERIALIZE_DATA(&newequipfb_param, sizeof(newequipfb_param));
	UNSERIALIZE_DATA(&stone_param, sizeof(stone_param));
	UNSERIALIZE_DATA(&guild_xianshu_param, sizeof(guild_xianshu_param));
	UNSERIALIZE_DATA(&task_param, sizeof(task_param));
	UNSERIALIZE_DATA(&role_supply_param, sizeof(role_supply_param));
	UNSERIALIZE_DATA(&maze_param, sizeof(maze_param));
	UNSERIALIZE_DATA(&xiannv_param, sizeof(xiannv_param));
	UNSERIALIZE_DATA(&storyfb_param, sizeof(storyfb_param));
	UNSERIALIZE_DATA(&challengefb_param, sizeof(challengefb_param));
	UNSERIALIZE_DATA(&convert_record_param, sizeof(convert_record_param));
	UNSERIALIZE_DATA(&fight_mount_param, sizeof(fight_mount_param));
	UNSERIALIZE_DATA(&mojiefb_param, sizeof(mojiefb_param));
	UNSERIALIZE_DATA(&equipsuit_param, sizeof(equipsuit_param));
	UNSERIALIZE_DATA(&shenzhuang_param, sizeof(shenzhuang_param));
	UNSERIALIZE_DATA(&csa_equip_param, sizeof(csa_equip_param));

	UNSERIALIZE_DATA(&discount_buy_param, sizeof(discount_buy_param));
	UNSERIALIZE_DATA(&other_module_param, sizeof(other_module_param));
	UNSERIALIZE_DATA(&qingyuan_param, sizeof(qingyuan_param));
	UNSERIALIZE_DATA(&role_goal_param, sizeof(role_goal_param));
	UNSERIALIZE_DATA(&shizhuang_param, sizeof(shizhuang_param));
	UNSERIALIZE_DATA(&history_camp_param, sizeof(history_camp_param));
	UNSERIALIZE_DATA(&skill_other_param, sizeof(skill_other_param));
	UNSERIALIZE_DATA(&role_cross_param, sizeof(role_cross_param));
	UNSERIALIZE_DATA(&role_guild_param, sizeof(role_guild_param));
	UNSERIALIZE_DATA(&shengwang_param, sizeof(shengwang_param));
	UNSERIALIZE_DATA(&chengjiu_param, sizeof(chengjiu_param));
	UNSERIALIZE_DATA(&dailyfb_param, sizeof(dailyfb_param));
	UNSERIALIZE_DATA(&lifeskill_param, sizeof(lifeskill_param));
	UNSERIALIZE_DATA(&baby_param, sizeof(baby_param));
	UNSERIALIZE_DATA(&shenge_system_param, sizeof(shenge_system_param));
	UNSERIALIZE_DATA(&zero_gift_param, sizeof(zero_gift_param));
	UNSERIALIZE_DATA(&mysterious_shop_in_mall_param, sizeof(mysterious_shop_in_mall_param));
	UNSERIALIZE_DATA(&friend_exp_bottle_param, sizeof(friend_exp_bottle_param));
	UNSERIALIZE_DATA(&shenshou_param, sizeof(shenshou_param));
	UNSERIALIZE_DATA(&img_fuling_param, sizeof(img_fuling_param));

	UNSERIALIZE_DATA(&tuhaojin_param, sizeof(tuhaojin_param));
	UNSERIALIZE_DATA(&bigchatface_param, sizeof(bigchatface_param));
	UNSERIALIZE_DATA(&pet_param, sizeof(pet_param));
	UNSERIALIZE_DATA(&phase_fb_param, sizeof(phase_fb_param));
	UNSERIALIZE_DATA(&vipfb_param, sizeof(vipfb_param));
	UNSERIALIZE_DATA(&huashen_param, sizeof(huashen_param));
	UNSERIALIZE_DATA(&magic_card_param, sizeof(magic_card_param));
	UNSERIALIZE_DATA(&mitama_param, sizeof(mitama_param));
	UNSERIALIZE_DATA(&equipment_param, sizeof(equipment_param));
	UNSERIALIZE_DATA(&chinese_zodiac_param, sizeof(chinese_zodiac_param));
	UNSERIALIZE_DATA(&friend_gift_param, sizeof(friend_gift_param));
	UNSERIALIZE_DATA(&follow_boss_param, sizeof(follow_boss_param));
	UNSERIALIZE_DATA(&gold_vip_param, sizeof(gold_vip_param));
	UNSERIALIZE_DATA(&rune_system_param, sizeof(rune_system_param));
	UNSERIALIZE_DATA(&magical_precious_param, sizeof(magical_precious_param));
	UNSERIALIZE_DATA(&feixian_param, sizeof(feixian_param));

	UNSERIALIZE_DATA(&multi_mount_param, sizeof(multi_mount_param));
	UNSERIALIZE_DATA(&personalize_window_param, sizeof(personalize_window_param));
	UNSERIALIZE_DATA(&xunzhang_param, sizeof(xunzhang_param));
	UNSERIALIZE_DATA(&zhibao_param, sizeof(zhibao_param));
	UNSERIALIZE_DATA(&shengong_param, sizeof(shengong_param));
	UNSERIALIZE_DATA(&shenyi_param, sizeof(shenyi_param));
	UNSERIALIZE_DATA(&jingling_param, sizeof(jingling_param));
	UNSERIALIZE_DATA(&xiannv_shouhu_param, sizeof(xiannv_shouhu_param));
	UNSERIALIZE_DATA(&jingling_fazhen_param, sizeof(jingling_fazhen_param));
	UNSERIALIZE_DATA(&jingling_guanghuan_param, sizeof(jingling_guanghuan_param));
	UNSERIALIZE_DATA(&shenzhou_weapon_param, sizeof(shenzhou_weapon_param));
	UNSERIALIZE_DATA(&lieming_param, sizeof(lieming_param));
	UNSERIALIZE_DATA(&lieming_bag_param, sizeof(lieming_bag_param));
	UNSERIALIZE_DATA(&cardzu_param, sizeof(cardzu_param));
	UNSERIALIZE_DATA(&zhuansheng_equip_param, sizeof(zhuansheng_equip_param));
	UNSERIALIZE_DATA(&mojie_param, sizeof(mojie_param));
	UNSERIALIZE_DATA(&shenbing_param, sizeof(shenbing_param));
	UNSERIALIZE_DATA(&tuitu_fb_param, sizeof(tuitu_fb_param));
	UNSERIALIZE_DATA(&cloak_param, sizeof(cloak_param));
	UNSERIALIZE_DATA(&precious_boss_param, sizeof(precious_boss_param));
	UNSERIALIZE_DATA(&couple_mount_param, sizeof(couple_mount_param));
	UNSERIALIZE_DATA(&shenyin_param, sizeof(shenyin_param));
	UNSERIALIZE_DATA(&element_heart_param, sizeof(element_heart_param));
	UNSERIALIZE_DATA(&shengxing_zhuli_param, sizeof(shengxing_zhuli_param));
	UNSERIALIZE_DATA(&fabao_param, sizeof(fabao_param));
	UNSERIALIZE_DATA(&talent_param, sizeof(talent_param));
	UNSERIALIZE_DATA(&jinjie_talent_param, sizeof(jinjie_talent_param));
	UNSERIALIZE_DATA(&armor_fb_param, sizeof(armor_fb_param));
	UNSERIALIZE_DATA(&bosscard_param, sizeof(bosscard_param));
	UNSERIALIZE_DATA(&task_zhuanzhi_param, sizeof(task_zhuanzhi_param));
	UNSERIALIZE_DATA(&cross_global_data_param, sizeof(cross_global_data_param));
	UNSERIALIZE_DATA(&yaoshi_param, sizeof(yaoshi_param));
	UNSERIALIZE_DATA(&toushi_param, sizeof(toushi_param));
	UNSERIALIZE_DATA(&qilinbi_param, sizeof(qilinbi_param));
	UNSERIALIZE_DATA(&mask_param, sizeof(mask_param));
	UNSERIALIZE_DATA(&zhuanzhi_equip_param, sizeof(zhuanzhi_equip_param));
	UNSERIALIZE_DATA(&heshen_luoshu_param, sizeof(heshen_luoshu_param));
	UNSERIALIZE_DATA(&shengqi_param, sizeof(shengqi_param));
	UNSERIALIZE_DATA(&seal_param, sizeof(seal_param));
	UNSERIALIZE_DATA(&dressing_room_param, sizeof(dressing_room_param));
	UNSERIALIZE_DATA(&upgrade_manager_param, sizeof(upgrade_manager_param));
	UNSERIALIZE_DATA(&jinjiesys_reward_param, sizeof(jinjiesys_reward_param));
	UNSERIALIZE_DATA(&role_big_small_goal_param, sizeof(role_big_small_goal_param));
	UNSERIALIZE_DATA(&shenqi_param, sizeof(shenqi_param));
	UNSERIALIZE_DATA(&lieming_param2, sizeof(lieming_param2));
	UNSERIALIZE_DATA(&profess_param, sizeof(profess_param));
	UNSERIALIZE_DATA(&red_equip_collect_param, sizeof(red_equip_collect_param));
	UNSERIALIZE_DATA(&mount_param, sizeof(mount_param));
	UNSERIALIZE_DATA(&footprint_param, sizeof(footprint_param));
	UNSERIALIZE_DATA(&halo_param, sizeof(halo_param));
	UNSERIALIZE_DATA(&orange_equip_collect_param, sizeof(orange_equip_collect_param));
	UNSERIALIZE_DATA(&xianzunka_param, sizeof(xianzunka_param));
	UNSERIALIZE_DATA(&equipbaptize_param, sizeof(equipbaptize_param));
	UNSERIALIZE_DATA(&personboss_param, sizeof(personboss_param));
	UNSERIALIZE_DATA(&greate_soldier_param, sizeof(greate_soldier_param));
	UNSERIALIZE_DATA(&role_lottery_count_param, sizeof(role_lottery_count_param));
	UNSERIALIZE_DATA(&role_cross_rand_activity_param, sizeof(role_cross_rand_activity_param));
	UNSERIALIZE_DATA(&upgrade_card_buy_param, sizeof(upgrade_card_buy_param));
	UNSERIALIZE_DATA(&huguozhili_param, sizeof(huguozhili_param));
	UNSERIALIZE_DATA(&cross_goal_param, sizeof(cross_goal_param));
	UNSERIALIZE_DATA(&baizhan_equip_param, sizeof(baizhan_equip_param));
	UNSERIALIZE_DATA(&discountbuy_param, sizeof(discountbuy_param));
	UNSERIALIZE_DATA(&myth_param, sizeof(myth_param));
	UNSERIALIZE_DATA(&zodiac_param, sizeof(zodiac_param));
	UNSERIALIZE_DATA(&cross_equip_param, sizeof(cross_equip_param));

	// ±í10 BLOB×Ö¶Î
	if (!UnserializeAutoData(instream, data_page_10)) return false;
	
	return true;
}

void RoleOtherInitParam::Reset()
{
	{
		last_singlechat_monitor_time = 0;
		memset(last_singlechat_monitor_msg, 0, sizeof(last_singlechat_monitor_msg));
		
		last_worldchat_monitor_time = 0;
		memset(last_worldchat_monitor_msg, 0, sizeof(last_worldchat_monitor_msg));

		last_mail_monitor_time = 0;
		memset(last_mail_monitor_subject, 0, sizeof(last_mail_monitor_subject));

		chest_buff_len = 0;
		memset(chest_buff, 0, sizeof(chest_buff));

		effect_buff_len = 0;
		memset(effect_buff, 0, sizeof(effect_buff));

		taskrecord_buff_len = 0;
		memset(taskrecord_buff, 0, sizeof(taskrecord_buff));
	}

	mentality_param.Reset();
	vip_param.Reset();
	roleshop_param.Reset();
	active_degree_param.Reset();
	welfare_param.Reset();
	rolehangup_param.ResetHangupData();
	rolehangup_param.ResetCustomData();
	itemcolddown_param.Reset();
	evaluate_param.Reset();
	monitor_param.Reset();
	title_param.Reset();
	husong_param.Reset();
	roleactivity_param.Reset();
	towerdefend_param.Reset();
	common_data_param.Reset();
	wing_param.Reset();

	equipfb_param.Reset();
	guild_task_param.Reset();
	newequipfb_param.Reset();
	stone_param.Reset();
	guild_xianshu_param.Reset();
	task_param.Reset();
	role_supply_param.Reset();
	maze_param.Reset();
	xiannv_param.Reset();
	storyfb_param.Reset();
	challengefb_param.Reset();
	convert_record_param.Reset();
	fight_mount_param.Reset();
	mojiefb_param.Reset();
	equipsuit_param.Reset();
	shenzhuang_param.Reset();
	csa_equip_param.Reset();

	discount_buy_param.Reset();
	other_module_param.Reset();
	qingyuan_param.Reset();
	role_goal_param.Reset();
	shizhuang_param.Reset();
	history_camp_param.Reset();
	skill_other_param.Reset();
	role_cross_param.Reset();
	role_guild_param.Reset();
	shengwang_param.Reset();
	chengjiu_param.Reset();
	dailyfb_param.Reset();
	lifeskill_param.Reset();
	baby_param.Reset();
	shenge_system_param.Reset();
	zero_gift_param.Reset();
	mysterious_shop_in_mall_param.Reset();
	friend_exp_bottle_param.Reset();
	shenshou_param.Reset();
	img_fuling_param.Reset();

	tuhaojin_param.Reset();
	bigchatface_param.Reset();
	pet_param.Reset();
	phase_fb_param.Reset();
	vipfb_param.Reset();
	huashen_param.Reset();
	magic_card_param.Reset();
	mitama_param.Reset();
	equipment_param.Reset();
	chinese_zodiac_param.Reset();
	friend_gift_param.Reset();
	follow_boss_param.Reset();
	gold_vip_param.Reset();
	rune_system_param.Reset();
	magical_precious_param.Reset();
	feixian_param.Reset();

	multi_mount_param.Reset();
	personalize_window_param.Reset();
	xunzhang_param.Reset();
	zhibao_param.Reset();
	shengong_param.Reset();
	shenyi_param.Reset();
	jingling_param.Reset();
	xiannv_shouhu_param.Reset();
	jingling_fazhen_param.Reset();
	jingling_guanghuan_param.Reset();
	shenzhou_weapon_param.Reset();
	lieming_param.Reset();
	lieming_bag_param.Reset();
	cardzu_param.Reset();
	zhuansheng_equip_param.Reset();
	mojie_param.Reset();
	shenbing_param.Reset();
	tuitu_fb_param.Reset();
	cloak_param.Reset();
	precious_boss_param.Reset();
	couple_mount_param.Reset();
	shenyin_param.Reset();
	element_heart_param.Reset();
	shengxing_zhuli_param.Reset();
	fabao_param.Reset();
	talent_param.Reset();
	jinjie_talent_param.Reset();
	armor_fb_param.Reset();
	bosscard_param.Reset();
	task_zhuanzhi_param.Reset();
	cross_global_data_param.Reset();
	yaoshi_param.Reset();
	toushi_param.Reset();
	qilinbi_param.Reset();
	mask_param.Reset();
	zhuanzhi_equip_param.Reset();
	heshen_luoshu_param.Reset();
	shengqi_param.Reset();
	seal_param.Reset();
	dressing_room_param.Reset();
	upgrade_manager_param.Reset();
	jinjiesys_reward_param.Reset();
	role_big_small_goal_param.Reset();
	shenqi_param.Reset();
	lieming_param2.Reset();
	profess_param.Reset();
	red_equip_collect_param.Reset();
	mount_param.Reset();
	footprint_param.Reset();
	halo_param.Reset();
	orange_equip_collect_param.Reset();
	xianzunka_param.Reset();
	equipbaptize_param.Reset();
	personboss_param.Reset();
	greate_soldier_param.Reset();
	role_lottery_count_param.Reset();
	role_cross_rand_activity_param.Reset();
	upgrade_card_buy_param.Reset();
	huguozhili_param.Reset();
	cross_goal_param.Reset();
	baizhan_equip_param.Reset();
	discountbuy_param.Reset();
	myth_param.Reset();
	zodiac_param.Reset();
	cross_equip_param.Reset();

	// ±í10 BLOB×Ö¶Î
	data_page_10.Reset();
}
