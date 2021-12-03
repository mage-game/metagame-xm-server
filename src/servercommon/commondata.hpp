
#ifndef __COMMON_DATA_HPP__
#define __COMMON_DATA_HPP__

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/activitydef.hpp"

#pragma pack(push) 
#pragma pack(4)

struct CommonDataParam
{
	enum INITFLAG
	{
		INITFLAG_NUQI = 0,
		INITFLAG_ENERGY,
		INITFLAG_MAX,
	};

	static const int SHANGGU_BOSS_MAX_LAYER = 6;						// 上古BOSS - 最大层数（同上）
	static const int EQUIP_IMPGUARD_MAX_NUM = 2;						// 最大可装备小鬼数量
	static const int NOTICE_DAY_MAX_NUM = 16;							// 天数预告最大字节数
	static const int NOTICE_DAY_MAX_ID = NOTICE_DAY_MAX_NUM * 8;		// 天数预告最大id [1-127]
	UNSTD_STATIC_CHECK(INITFLAG_MAX <= 30)
	static const int BOSS_FAMILY_INFO_MAX_COUNT = 128;		

	CommonDataParam() 
	{
		this->Reset();
	}

	void Reset()
	{
		init_flag = 0;
		nuqi = 0;
		energy = 0;
		last_give_energy = 0;
		shengwang = 0;

		lover_uid = 0;
		memset(lover_name, 0, sizeof(lover_name));

		last_marry_time = 0;
		last_divorce_time = 0;

		week_kill_boss_count = 0;

		red_paper_consume_gold_num = 0;
		red_paper_lucky_value = 0;

		next_fake_private_chat_timestamp = 0;
		fake_private_chat_times = 0;

		last_logout_timestamp = 0;

		day_hunyan_gather_bind_coin = 0;
		day_hunyan_add_nvwashi = 0;

		plat_spid = 0;

		has_send_welcome_mail = 0;
		is_fetch_worldboss_week_reward = 0;
		cross_equip_itemid = 0;

		cross_honor = 0;

		cross_equip_gain_timestamp = 0;

		m_day_revival_times = 0;
		daily_task_star_level = 0;
		recored_use_norex_num = 0;
		memset(use_norex_day_times, 0, sizeof(use_norex_day_times));

		yaoshouguangchang_score = 0;
		yaoshouguangchang_lv = 0;

		suoyaota_score = 0;
		suoyaota_lv = 0;

		yaoshouguangchang_max_score = 0;
		suoyaota_max_score = 0;

		expbottle_use_count = 0;
		expbottle_transform_count = 0;

		space_renqi = 0;
		space_getgift_count = 0;
		space_day_cai_addhuoli = 0;
		space_huoli = 0;
		memset(space_xinqing, 0, sizeof(space_xinqing));

		dabaoboss_validtimestamp = 0;

		space_last_seek_gift_timestamp = 0;
		space_last_seek_msg_timestamp = 0;

		zhuagui_day_gethunli = 0;
		zhuagui_day_catch_count = 0;

		next_extra_drop_timestamp = 0;

		paohuan_complete_count = 0;
		paohuan_getphase_reward_idx = 0;

		jinghua_husong_commit_count = 0;
		jinghua_husong_status = 0;
		jinghua_husong_invalid_time = 0;	
		
		day_guild_red_paper_consume_gold_num = 0;
		day_hunyan_fa_blessgift_count = 0;
		m_next_canchangecamp_timestamp = 0;

		cur_use_bubble_type = -1;

		memset(reserve_list, 0, sizeof(reserve_list));

		day_gongxun = 0;

		equip_xunbao_times = 0;
		jl_xunbao_times = 0;
		hchz_kill_count = 0;
		gcz_kill_count = 0;

		rare_chestshop_times = 0;

		jump_remain_times = 0;
		jump_last_recover_time = 0;

		move_chess_free_times = 0;				
		move_chess_reset_times = 0;	
		move_chess_cur_step = 0;
		move_chess_next_free_time = 0;
		millionaire_valid_perform_skill_time = 0;
		millionaire_gather_count = 0;
		is_turn_millionaire = 0;
		is_turn = 0;

		miku_boss_weary = 0;						
		dabao_angry_value = 0;						

		world_chat_count1 = 0;
		world_chat_count2 = 0;
		world_chat_record_time1 = 0;
		world_chat_record_time2 = 0;

		zhuagui_get_drop_time = 0;
		zhuagui_score = 0;				

		hunjie_item_id = 0;								
		hunjie_star_level = 1;
		hunyan_buy_fb_join_times = 0;
		last_buy_hunyan_fb_times_dayid = 0;

		millionaire_last_join_time = 0;
		millionaire_reward_index = -1;
		accept_task_change_appearn = 0;

		water_self_love_tree_time = 0;
		water_other_love_tree_time = 0;

		zhuagui_get_couple_drop_time = 0;

		next_recover_hp_time = 0;

		zhuagui_kill_boss_count = 0;

		dabao_kick_time = 0;

		mikuboss_relive_weary = 0;
		miku_relive_weary_last_die_time = 0;

		auto_join_guild_last_time = 0;

		rev_sh = 0;

		daily_guild_redbag_reward_fetch_flag = 0;

		team_equip_fb_pass_flag = 0;
		team_equip_fb_day_count = 0;
		team_equip_fb_day_buy_count = 0;
		daily_use_guild_relive_times = 0;
		daily_use_free_song_hua_times = 0;
		today_use_free_relive_num = 0;

		upgrade_skill_next_skill_index = -1;	
		upgrade_skill_skill_active_flag = 0;	
		upgrade_skill_attack_count = 0;	

		millionaire_use_skill_times = 0;
		red_paper_be_singal_chat_count = 0;

		advance_notice_last_fectch_id = 0;
		today_putong_hunyan_times = 0;	

		world_question_last_answer_time = 0;
		world_question_right_num = 0;
		world_question_right_num_change_time = 0;

		guild_question_last_answer_time = 0;
		guild_question_right_num = 0;
		red_paper_be_singal_last_be_chat_time = 0;	
		last_send_heli_notice_time = 0;	
		is_auto_dec_offline_friend = 1;

		covertshop_guild_last_notice_time = 0;

		collect_drop_num = 0;
		jingling_advantage_gather_blue = 0;

		last_signin_timestamp = 0;
		guild_signin_fetch_reward_flag = 0;
		famous_man_flag = 0;
		guild_battle_fetch_daily_reward = 0;

		accept_task_change_appearn_param = 0;
		challenge_field_best_rank_break_level = 0;
		jingjie_level = 0;
		
		baby_boss_kick_time = 0;
		baby_boss_angry_value = 0;
		baby_boss_enter_times = 0;

		active_boss_weary = 0;
		is_pass_build_tower_fb = 0;
		appearacne_body_use_type = 0;

		res_sh_1 = 0;
		memset(shanggu_boss_concern_flag_list, 0, sizeof(shanggu_boss_concern_flag_list));

		imp_guard_mail_flag = 0;                            // 小鬼

		qingyuan_capablity = 0;
		baby_capablity = 0;
		little_pet_capablity = 0;

		dabao_next_add_angry_timestamp = 0;

		appearacne_wuqi_use_type = 0;
		avatar_changed = 0;								// 是否领取过仙盟换头像奖励
		cross_boss_relive_tire = 0;						// 跨服boss复活疲劳

		cross_boss_relive_tire_reset_time = 0;		// 复活疲劳重置时间
		cross_boss_can_realive_time = 0;

		cur_use_avatar_type = -1;

		memset(imp_guard_type, 0, sizeof(imp_guard_type));
		memset(imp_guard_end_timestamp, 0, sizeof(imp_guard_end_timestamp));

		jinjiesys_reward_flag = 0;
		jinjiesys_can_reward_flag = 0;
		jinjiesys_reward_flag_1 = 0;
		jinjiesys_can_reward_flag_1 = 0;
		
		shenbing_use_image_id = 0;
		baojia_use_image_id = 0;
		shizhuang_wuqi = 0;
		shizhuang_body = 0;
		mojie_skill_active_flag = 0;

		new_max_cap = 0;
		add_cap_for_rank = 0;
		next_clear_add_cap_time = 0;
		had_reset_new_max_cap = 0;

		res_ch = 0;
		res_sh_2 = 0;
		add_charm_for_rank = 0;
		next_clear_add_charm_time = 0;

		miku_relive_weary_last_relive_time = 0;

		res_sh = 0;
		cross_mizang_boss_relive_tire = 0;
		cross_mizang_boss_relive_tire_reset_time = 0;
		cross_mizang_boss_can_realive_time = 0;

		res_sh_3 = 0;
		cross_youming_boss_relive_tire = 0;
		cross_youming_boss_relive_tire_reset_time = 0;
		cross_youming_boss_can_realive_time = 0;

		reserve_int = 0;
		memset(imp_guard_itemid, 0, sizeof(imp_guard_itemid));
		kill_active_boss_count = 0;
		kill_miku_boss_count = 0;

		memset(notice_day_fectch_flag_list, 0, sizeof(notice_day_fectch_flag_list));

		godmagic_today_kill_boss_num =0 ;
		today_gather_ordinary_crystal_num = 0;
		today_gather_treasure_crystal_num = 0;

		trigger_continue_kill_invalid_timestamp = 0;		// 连斩时间触发失效时间
		last_add_continue_kill_buff_timestamp = 0;			// 最后加连斩buff时间

		continue_kill_count = 0;							// 连续击杀个数

		last_send_continue_kill_timestamp = 0;				// 最后发送连斩信息时间

		last_accept_task_change_appearn = 0;
		last_accept_task_change_appearn_param = 0;

		uuid= 0;
		uu_last_login_serverid = 0;
		memset(tianshu_xunzhu_active_flag_list, 0, sizeof(tianshu_xunzhu_active_flag_list));
		memset(tianshu_xunzhu_goal_fetch_flag_list, 0, sizeof(tianshu_xunzhu_goal_fetch_flag_list));
		tianshu_xunzhu_zhuanzhi_equip_fangyu = 0;
		memset(today_boss_family_kill_info, 0, sizeof(today_boss_family_kill_info));
		has_xianzunka_flag = 0;
		reserve_sh = 0;
	}

	int init_flag;								// 某些字段初始值不是0，就用它控制初始化
	short nuqi;
	short energy;
	unsigned int last_give_energy;
	int shengwang;								// 声望

	int lover_uid;								// 伴侣UID
	GameName lover_name;						// 伴侣名字
	unsigned int last_marry_time;				// 结婚时间
	unsigned int last_divorce_time;				// 离婚时间

	int week_kill_boss_count;					// 每周击杀的世界boss数量，每周清0
	int red_paper_consume_gold_num;				// 发红包消耗的元宝数量
	int red_paper_lucky_value;					// 红包幸运指数

	unsigned int next_fake_private_chat_timestamp; // 下次伪造私聊时间戳 
	int	fake_private_chat_times;				// 伪造私聊次数

	unsigned int last_logout_timestamp;			// 上次下线时间

	int day_hunyan_gather_bind_coin;			// 每日在婚宴中获得仙魂数量

	int plat_spid;								// 平台渠道id

	char has_send_welcome_mail;					// 已经发送欢迎邮件
	char is_fetch_worldboss_week_reward;		// 本周是否领取世界BOSS
	ItemID cross_equip_itemid;					// 跨服装备ID

	int cross_honor;							// 跨服荣誉

	unsigned int cross_equip_gain_timestamp;	// 跨服装备获得时间

	unsigned short m_day_revival_times;			// 每日复活次数
	char daily_task_star_level;					// 日常任务星星等级(废弃)
	unsigned char recored_use_norex_num;		// 记录的主动消耗类物品个数
	NorextItemUseDayTimes use_norex_day_times[ItemNamespace::MAX_USE_NOREXITEM_RECORD_NUM];	//使用主动消耗类物品每日次数限制

	int yaoshouguangchang_score;				// 妖兽广场积分
	int yaoshouguangchang_lv;					// 妖兽广场等级

	int suoyaota_score;							// 锁妖塔积分
	int suoyaota_lv;							// 锁妖塔等级

	int yaoshouguangchang_max_score;			// 妖兽广场最大积分
	int suoyaota_max_score;						// 锁妖塔最大积分

	int expbottle_use_count;					// 经验瓶使用次数
	int expbottle_transform_count;				// 经验瓶转换次数

	int space_renqi;							// 空间人气
	int space_getgift_count;					// 空间获得礼物次数
	int space_day_cai_addhuoli;					// 空间每日踩获得的活力
	int space_huoli;							// 空间活力
	SpaceMessage space_xinqing;					// 空间心情

	int day_hunyan_add_nvwashi;					// 每日在婚宴中获得女娲石

	unsigned int dabaoboss_validtimestamp;		// 打宝boss有效时间戳 

	unsigned int space_last_seek_gift_timestamp;// 空间-最后一次查看礼物内容的时间
	unsigned int space_last_seek_msg_timestamp;	// 空间-最后一次查看信息的时间

	int zhuagui_day_gethunli;					// 每天抓鬼获得活力累计
	int zhuagui_day_catch_count;				// 每天抓鬼个数
	unsigned int next_extra_drop_timestamp;		// 下次额外掉落的时间戳

	short paohuan_complete_count;				// 跑环任务完成次数
	short paohuan_getphase_reward_idx;			// 跑环任务 获取奖励索引

	unsigned short jinghua_husong_commit_count;	// 精华护送当天提交次数
	short jinghua_husong_status;				// 精华护送状态
	UInt32 jinghua_husong_invalid_time;			// 精华护送的失效时间

	short day_guild_red_paper_consume_gold_num;			//每日仙盟红包花费元宝数,每天清零
	short day_hunyan_fa_blessgift_count;				//每日在婚宴发贺礼次数,每天清零	

	unsigned int m_next_canchangecamp_timestamp;		// 下一次可以转阵营的时间
	short cur_use_bubble_type;							// 当前使用的聊天框
	short reserve_list[7];								// 预留
	
	int day_gongxun;									// 功勋值

	unsigned short equip_xunbao_times;					// 装备寻宝次数
	unsigned short jl_xunbao_times;						// 精灵寻宝次数
	unsigned short hchz_kill_count;						// 皇城会战杀人数
	unsigned short gcz_kill_count;						// 攻城战杀人数

	int rare_chestshop_times;							// 奇珍异宝寻宝次数

	int jump_remain_times;								// 跳跃剩余次数
	unsigned int jump_last_recover_time;				// 最后恢复跳跃时间

	short move_chess_free_times;						// 走棋子每日免费次数
	short move_chess_reset_times;						// 走棋子每日重置次数
	int move_chess_cur_step;							// 当前所在格子
	unsigned int move_chess_next_free_time;				// 下一次免费走棋子时间戳

	unsigned int millionaire_valid_perform_skill_time;	// 大富豪冰冻技能冷却时间						
	short millionaire_gather_count;						// 大富豪宝箱采集次数
	char is_turn_millionaire;							// 转到大富豪为1 ，否则为0
	char is_turn;										// 是否转动转盘

	short miku_boss_weary;								// 秘窟boss疲劳值
	unsigned short dabao_angry_value;					// 打宝愤怒值

	int world_chat_count1;								// 世界聊天数量
	int world_chat_count2;								// 世界聊天数量
	unsigned int world_chat_record_time1;
	unsigned int world_chat_record_time2;

	int zhuagui_get_drop_time;							// 抓鬼获取掉落物次数						
	int zhuagui_score;									// 抓鬼积分

	ItemID hunjie_item_id;								// 情缘婚戒物品id
	char accept_task_change_appearn;					// 接受任务变身形象		
	char hunyan_buy_fb_join_times;						// 情缘副本购买次数
	unsigned int last_buy_hunyan_fb_times_dayid;		// 最后一次购买婚宴副本次数的dayid

	unsigned int millionaire_last_join_time;			// 大富豪活动最后参与活动时间
	short millionaire_reward_index;						// 大富豪奖励索引
	short hunjie_star_level;							// 情缘婚戒星等级

	short water_self_love_tree_time;					// 给自己相思树浇水次数
	short water_other_love_tree_time;					// 给伴侣相思树浇水次数

	int zhuagui_get_couple_drop_time;					// 抓鬼获取夫妻掉落物次数

	unsigned int next_recover_hp_time;					// 下一次恢复气血时间

	int zhuagui_kill_boss_count;						// 抓鬼击杀boss个数

	unsigned int dabao_kick_time;						// 打宝愤怒值满了踢出副本时间

	int mikuboss_relive_weary;							// 秘窟boss复活疲劳值
	unsigned int miku_relive_weary_last_die_time;		// 秘窟boss最后死亡时间

	unsigned int auto_join_guild_last_time;				// 快速加入公会最后申请时间
	short rev_sh;							            // 
	short daily_guild_redbag_reward_fetch_flag;			// 当天公会的boss红包领取标记

	int team_equip_fb_pass_flag;						// 组队装备本通关标记
	int team_equip_fb_day_count;						// 组队装备本每日次数
	int team_equip_fb_day_buy_count;					// 组队装备本每日购买次数

	int	daily_use_guild_relive_times;					// 当天使用公会复活次数
	int daily_use_free_song_hua_times;					// 当天已经使用了多少次免费送花

	int today_use_free_relive_num;						// 每天免费复活次数

	short upgrade_skill_next_skill_index;				// 下一释放进阶系统装备技能index
	unsigned short upgrade_skill_skill_active_flag;		// 进阶系统技能激活标志
	int upgrade_skill_attack_count;						// 进阶系统装备技能触发计数

	short millionaire_use_skill_times;					// 大富豪使用冰冻技能次数
	short red_paper_be_singal_chat_count;				// 公会红包被私聊次数

	int advance_notice_last_fectch_id;					// 功能预告奖励标志					
	int today_putong_hunyan_times;						// 每天普通婚宴次数

	unsigned int world_question_last_answer_time;		// 最后回答世界题目时间
	int world_question_right_num;						// 世界答题对题数
	unsigned int world_question_right_num_change_time;  // 世界答题对题数最后改变时间戳

	unsigned int guild_question_last_answer_time;		// 最后回答公会题目时间
	int guild_question_right_num;						// 世界答题对题数
	unsigned int red_paper_be_singal_last_be_chat_time;	// 公会红包最后被私聊时间	
	int last_send_heli_notice_time;						// 最后一次向好友发送祝贺弹窗的时间
	int is_auto_dec_offline_friend;						// 是否自动删除离线x天的好友

	unsigned int covertshop_guild_last_notice_time;		// 兑换公会公告最后时间

	int collect_drop_num;								// 每日集字掉落个数

	int jingling_advantage_gather_blue;					// 每日精灵奇遇采集的蓝色精灵数量

	unsigned int last_signin_timestamp;					// 上次工会签到时间
	char guild_signin_fetch_reward_flag;				// 工会总签到领取标志
	char famous_man_flag;								// 自己是否取得名人堂称号奖励（用于存到角色身上，防止合服后公共数据不存在了）
	short guild_battle_fetch_daily_reward;				// 领取公会争霸每日奖励

	int accept_task_change_appearn_param;				// 接受任务变身形象参数
	int challenge_field_best_rank_break_level;			// 竞技场最高排名突破等级

	int jingjie_level;									// 境界进阶系统等级
	unsigned int baby_boss_kick_time;					// 宝宝boss愤怒值满了踢出副本时间
	short baby_boss_angry_value;						// 宝宝boss愤怒值
	short baby_boss_enter_times;						// 宝宝boss进入次数

	short active_boss_weary;							// 活跃boss疲劳值
	char is_pass_build_tower_fb;						// 是否通关建塔副本
	char appearacne_wuqi_use_type;						// 武器外观使用类型

	short imp_guard_mail_flag;                          // 小鬼  即将到期邮件提醒（低于12个小时的第一次上线进行提醒，之后上线或到期都不提醒）
	short res_sh_1;                      //

	unsigned int shanggu_boss_concern_flag_list[SHANGGU_BOSS_MAX_LAYER];	// 上古BOSS订阅状态

	int qingyuan_capablity;                             // 情缘战力
	int baby_capablity;                                 // 宝宝战力
	int little_pet_capablity;                           // 小宠物战力

	unsigned int dabao_next_add_angry_timestamp;        // 打宝场景每过n秒增加m点愤怒值时间戳

	char appearacne_body_use_type;						// 衣服外观使用类型
	char avatar_changed;								// 是否领取过仙盟换头像奖励
	short cross_boss_relive_tire;						// 跨服boss复活疲劳

	unsigned int cross_boss_relive_tire_reset_time;		// 复活疲劳重置时间
	unsigned int cross_boss_can_realive_time;			// 复活疲劳结束时
	
	short cur_use_avatar_type;							// 当前使用的头像框
	char imp_guard_type[EQUIP_IMPGUARD_MAX_NUM];        // 身上小鬼类型
	unsigned int imp_guard_end_timestamp[EQUIP_IMPGUARD_MAX_NUM]; // 身上小鬼精灵结束时间

	unsigned int jinjiesys_reward_flag;					// 进阶奖励领取(或购买)标记
	unsigned int jinjiesys_can_reward_flag;				// 进阶奖励可否领取标记

	unsigned int jinjiesys_reward_flag_1;				// 进阶奖励领取(或购买)标记
	unsigned int jinjiesys_can_reward_flag_1;			// 进阶奖励可否领取标记

	char shenbing_use_image_id;							// 神兵使用id
	char baojia_use_image_id;							// 宝甲使用id
	char shizhuang_wuqi;								// 时装武器
	char shizhuang_body;								// 时装身体
	
	unsigned long long mojie_skill_active_flag;				// 魔戒技能激活标志

	unsigned int trigger_continue_kill_invalid_timestamp;	// 连斩时间触发失效时间
	unsigned int last_add_continue_kill_buff_timestamp;	// 最后加连斩buff时间
	int continue_kill_count;							// 连续击杀个数
	unsigned int last_send_continue_kill_timestamp;		// 最后发送连斩信息时间

	int new_max_cap;									// 历史最高战力(用于增加战力排行榜)
	int add_cap_for_rank;								// 增加战力
	unsigned int next_clear_add_cap_time;				// 下次清空增加战力的时间
	char had_reset_new_max_cap;                         // 重置
	char res_ch;
	short res_sh_2;
	int add_charm_for_rank;								// 增加魅力
	unsigned int next_clear_add_charm_time;				// 下次清空增加魅力的时间

	unsigned int miku_relive_weary_last_relive_time;    // 秘窟boss复活疲劳值最后变更时间

	short res_sh;
	short cross_mizang_boss_relive_tire;						// 跨服boss复活疲劳
	unsigned int cross_mizang_boss_relive_tire_reset_time;		// 复活疲劳重置时间
	unsigned int cross_mizang_boss_can_realive_time;			// 复活疲劳结束时

	short res_sh_3;
	short cross_youming_boss_relive_tire;						// 跨服boss复活疲劳
	unsigned int cross_youming_boss_relive_tire_reset_time;		// 复活疲劳重置时间
	unsigned int cross_youming_boss_can_realive_time;			// 复活疲劳结束时

	int reserve_int;											// 保留
	unsigned short imp_guard_itemid[EQUIP_IMPGUARD_MAX_NUM];    // 身上小鬼id
	unsigned short kill_active_boss_count;						// 已击杀活跃boss个数
	unsigned short kill_miku_boss_count;						// 已击杀秘窟bossboss个数
	unsigned char notice_day_fectch_flag_list[NOTICE_DAY_MAX_NUM];	// 天数预告奖励标志

	int godmagic_today_kill_boss_num;											// 神魔boss今日击杀boss数量
	int today_gather_ordinary_crystal_num;								// 神魔boss采集普通水晶次数
	int today_gather_treasure_crystal_num;								// 神魔boss采集珍惜水晶次数

	int last_accept_task_change_appearn;						// 上次接受任务变身形象（用于切进副本时保存之前任务变身状态）
	int last_accept_task_change_appearn_param;					// 上次接受任务变身形象参数

	long long uuid;												// 同步RoleCross中的UUID
	long long uu_last_login_serverid;							// 最后一次登陆的本地服ID,用于判断当前合到哪个服务器了
	
	UInt32 tianshu_xunzhu_active_flag_list[TIANSHU_XUNZHU_MAX_TYPE_COUNT];		// 天书寻主，激活标记
	UInt32 tianshu_xunzhu_goal_fetch_flag_list[TIANSHU_XUNZHU_MAX_TYPE_COUNT];	// 天书寻主，目标领取标记列表
	long long tianshu_xunzhu_zhuanzhi_equip_fangyu;								// 天书寻主，转职装防御
	struct TodayBossFamilyKillInfo
	{
		TodayBossFamilyKillInfo() : monster_id(0), kill_count(0) {}

		unsigned short monster_id;
		unsigned short kill_count;
	};
	TodayBossFamilyKillInfo today_boss_family_kill_info[BOSS_FAMILY_INFO_MAX_COUNT];
	short has_xianzunka_flag;									// 拥有仙尊卡的标记（临时或永久都算）
	short reserve_sh;
};

#pragma pack(pop)

typedef char CommonDataParamHex[sizeof(CommonDataParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(CommonDataParamHex) < 4096);

#endif  // __COMMON_DATA_HPP__
