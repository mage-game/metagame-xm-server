
#include "gamelog.h"
#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"

namespace gamelog
{
	// static const char DEFAULT_LOG_FILE[]		= "gamelog/gamelog.txt";

	static const char DEFAULT_LOG_FILE[]		= "1";			// ��׼���

	LogAgent g_log_world;
	LogAgent g_log_scenemanager;
	LogAgent g_log_scene;
	LogAgent g_log_messagehandler;

	LogAgent g_log_scenestatus;						// ����״̬��־
	LogAgent g_log_save_error;						// RMI�洢����
	LogAgent g_log_msgmoniter;						// �ڴ�ؼ��

	LogAgent g_log_moneygold;						// Ԫ����־
	LogAgent g_log_moneycoin;						// ͭǮ��־
	LogAgent g_log_knapsack;						// ��ɫ�����ֿ���־
	LogAgent g_log_item;							// ��Ʒ������־
	LogAgent g_log_xiannv;							// ��Ů��־
	LogAgent g_log_wing;							// ������־
	LogAgent g_log_halo;							// �⻷��־
	LogAgent g_log_shengong;						// ����־
	LogAgent g_log_shenyi;							// ������־
	LogAgent g_log_footprint;						// �㼣��־
	LogAgent g_log_cloak;							// ������־
	LogAgent g_log_equipment;						// װ��������־
	LogAgent g_log_task;							// ������־
	LogAgent g_log_task_stats;						// ����ͳ����־
	LogAgent g_log_vip;								// vip��־
	LogAgent g_log_chestshop;						// ������־
	LogAgent g_log_movechess;						// ��������־
	LogAgent g_log_mount;							// ������־
	LogAgent g_log_publicsale;						// ������־
	LogAgent g_log_mail;							// �ʼ���־
	LogAgent g_log_guild_route;						// ������־
	LogAgent g_log_npcshop;							// npc�̵�
	LogAgent g_log_shop;							// �̳�
	LogAgent g_log_convertshop;						// �һ��̵�
	LogAgent g_log_roleactivity;					// ��ɫ�
	LogAgent g_log_rand_activity;					// ����
	LogAgent g_log_qingyuan;						// ��Ե
	LogAgent g_log_roledata;						// �ɻ�
	LogAgent g_log_marry;							// Ԫ������

	LogAgent g_log_mentality;						// ����

	LogAgent g_log_function_stats;					// ����ͳ����־
	LogAgent g_log_role_rename;						// ��ɫ����
	LogAgent g_log_role_upgrade;					// ��ɫ����
	LogAgent g_log_friends;							// ����ϵͳ��־
	LogAgent g_log_drop;							// ������־

	LogAgent g_log_welfare;							// ������־
	LogAgent g_log_monitor;							// �����־
	LogAgent g_log_sundry;							// ������־

	LogAgent g_log_cmd;								// �ⲿϵͳ������־
	LogAgent g_log_debug;							// ����һЩ������־���
	LogAgent g_log_serious_error;					// ���ش��� 
	LogAgent g_log_disconnect;						// �ϵ�������־
	LogAgent g_log_login;							// ��¼�ǳ���־
	LogAgent g_log_gm;								// GM������־
	LogAgent g_log_strengthen_test;					// ǿ��ͳ����־
	LogAgent g_log_forbid;							// ���Է����־
	LogAgent g_log_quick;							// ������־
	LogAgent g_log_guild;							// ������־
	LogAgent g_log_activity;						// ���־
	LogAgent g_log_p2ptrade;						// p2p������־
	LogAgent g_log_challengefield;					// ��������־ 
	LogAgent g_log_other;							// һЩ����
	LogAgent g_log_fb;								// ������־
	LogAgent g_log_worldboss;						// ����boss��־
	LogAgent g_log_wabao;							// �ڱ���־
	LogAgent g_log_question;						// ������־
	LogAgent g_log_role_num;						// ����������־ ��login���ƹ�����
	LogAgent g_log_global_user_state;				// �û���������־ ��login���ƹ�����
	LogAgent g_log_husong_task;						// ������־
	LogAgent g_log_guild_task;						// ����������־
	LogAgent g_log_daily_task;						// �ճ�������־
	LogAgent g_log_month_card;						// �¿�
	LogAgent g_log_touzijihua;						// Ͷ�ʼƻ�
	LogAgent g_log_fish_pool;						// ���
	LogAgent g_log_combine_server;					// �Ϸ��
	LogAgent g_log_cardzu;							// �������
	LogAgent g_log_rank;							// ���а�
	LogAgent g_log_cross;							// ���
	LogAgent g_log_lieming;							// ����
	LogAgent g_log_jingling;						// ����
	LogAgent g_log_chengjiu;						// �ɾ�
	LogAgent g_log_shengwang;						// ����
	LogAgent g_log_luaerror;						// luaerror
	LogAgent g_log_luaprogram;						// lua������־
	LogAgent g_log_tuhaojin;						// ������
	LogAgent g_log_bigchatface;						// ��������
	LogAgent g_log_shenzhou_weapon;					// ��������
	LogAgent g_log_baby;							// ����
	LogAgent g_log_pet;								// ����
	LogAgent g_log_appearance;						// ������
	LogAgent g_log_huashen;							// ����ϵͳ
	LogAgent g_log_multi_mount;						// ˫������
	LogAgent g_log_personalize_window;				// ���Ի�����
	LogAgent g_log_magic_card;						// ħ��
	LogAgent g_log_mitama;							// ����
	LogAgent g_log_chinese_zodiac;					// ����ϵͳ
	LogAgent g_log_xiannv_shouhu;					// ��Ů�ػ�
	LogAgent g_log_jingling_fazhen;					// ���鷨��
	LogAgent g_log_jingling_guanghuan;				// ����⻷
	LogAgent g_log_zhuansheng_equip;				// ת��ϵͳ
	LogAgent g_log_little_pet;						// С����
	LogAgent g_log_equip_suit;						// ������װ
	LogAgent g_log_rune_system;						// ����ϵͳ
	LogAgent g_log_shenge_system;					// ���ϵͳ
	LogAgent g_log_shenbing;						// ���ϵͳ
	LogAgent g_log_title;							// �ƺ�ϵͳ
	LogAgent g_log_fighting;						// ������ϵͳ
	LogAgent g_log_shenshou;						// ����
	LogAgent g_log_new_protocol;					// ��Э����־
	LogAgent g_log_tianxiang;						// ����ϵͳ
	LogAgent g_log_fishing_exchange;				// ����һ�
	LogAgent g_log_fishing;							// ����
	LogAgent g_log_element_heart;					// Ԫ��֮��
	LogAgent g_log_yaoshi;							// ������־
	LogAgent g_log_toushi;							// ͷ����־
	LogAgent g_log_qilinbi;							// �������־
	LogAgent g_log_mask;							// �����־
	LogAgent g_log_zhaunzhi_equip;					// תְװ����־
	LogAgent g_log_upgrade;							// ����ϵͳ
	LogAgent g_log_shengqi;							// ʥ��ϵͳ
	LogAgent g_log_seal;							// ʥӡϵͳ
	LogAgent g_log_role_big_small_goal;             // ��ɫ��СĿ��
	LogAgent g_log_shenqi;							// ����
	LogAgent g_log_xianzunka;						// ����
	LogAgent g_log_equip_baptize;                   // װ��ϴ��
	LogAgent g_log_greate_soldier;					// ����
	LogAgent g_log_cross_rand_activity;				// �������
	LogAgent g_log_jinjie_sys_reward;				// ����ϵͳ��СĿ��
	LogAgent g_log_baizhan_equip;					// ��սװ����־
	LogAgent g_log_global_role;						// ȫ�ֽ�ɫ��־
	LogAgent g_log_zodiac;							// ʮ����Ф
	LogAgent g_log_tianshenhuti;					// ��ĩװ��

	bool g_open_console_log = false; 

	void LogInit(ILogModule *log, const std::string &logdir, bool open_console_log, int server_index)
	{
		g_open_console_log = open_console_log;

		g_log_world.SetLogModule(log);
		g_log_world.SetCatagory("World", DEFAULT_LOG_FILE);

		g_log_scenemanager.SetLogModule(log);
		g_log_scenemanager.SetCatagory("SceneManager", DEFAULT_LOG_FILE);

		g_log_scene.SetLogModule(log);
		g_log_scene.SetCatagory("Scene", DEFAULT_LOG_FILE);

		char gamelog_dir[256] = {0};
		SNPRINTF(gamelog_dir, sizeof(gamelog_dir), "gamelog_%d", server_index);
		std::string real_logdir = logdir + std::string(gamelog_dir);

		g_log_messagehandler.SetLogModule(log);
		g_log_messagehandler.SetCatagory("MessageHandler", (real_logdir+"/message/messagehandler.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_scenestatus.SetLogModule(log);
		g_log_scenestatus.SetCatagory("SceneStatus", (real_logdir+"/scene_status/scene_status.txt").c_str(), ILogModule::NFIS_DAY);
		
		g_log_save_error.SetLogModule(log);
		g_log_save_error.SetCatagory("SaveError", (real_logdir+"/save_error/save_error.txt").c_str(), ILogModule::NFIS_DAY);
		
		g_log_msgmoniter.SetLogModule(log);
		g_log_msgmoniter.SetCatagory("MsgMoniter", (real_logdir+"/mem_moniter/mem_moniter.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_moneygold.SetLogModule(log);
		g_log_moneygold.SetCatagory("MoneyGold", (real_logdir+"/money_gold/money_gold.txt").c_str(), ILogModule::NFIS_HALF_HOUR);
		
		g_log_moneycoin.SetLogModule(log);
		g_log_moneycoin.SetCatagory("MoneyCoin", (real_logdir+"/money_coin/money_coin.txt").c_str(), ILogModule::NFIS_HALF_HOUR);
		
		g_log_knapsack.SetLogModule(log);
		g_log_knapsack.SetCatagory("Knapsack", (real_logdir+"/knapsack/knapsack.txt").c_str(), ILogModule::NFIS_HALF_HOUR);
		
		g_log_item.SetLogModule(log);
		g_log_item.SetCatagory("Item", (real_logdir+"/item/item.txt").c_str(), ILogModule::NFIS_HALF_HOUR);
		
		g_log_xiannv.SetLogModule(log);
		g_log_xiannv.SetCatagory("Xiannv",(real_logdir+"/xiannv/xiannv.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_wing.SetLogModule(log);
		g_log_wing.SetCatagory("Wing",(real_logdir+"/wing/wing.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_halo.SetLogModule(log);
		g_log_halo.SetCatagory("Halo",(real_logdir+"/halo/halo.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_shengong.SetLogModule(log);
		g_log_shengong.SetCatagory("Shengong",(real_logdir+"/shengong/shengong.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_shenyi.SetLogModule(log);
		g_log_shenyi.SetCatagory("Shenyi",(real_logdir+"/shenyi/shenyi.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_footprint.SetLogModule(log);
		g_log_footprint.SetCatagory("footprint", (real_logdir + "/footprint/footprint.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_cloak.SetLogModule(log);
		g_log_cloak.SetCatagory("cloak", (real_logdir + "/cloak/cloak.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_equipment.SetLogModule(log);
		g_log_equipment.SetCatagory("Equipment",(real_logdir+"/equipment/equipment.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_task.SetLogModule(log);
		g_log_task.SetCatagory("Task",(real_logdir+"/task/task.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_vip.SetLogModule(log);
		g_log_vip.SetCatagory("VIP",(real_logdir+"/vip/vip.txt").c_str(), ILogModule::NFIS_HALF_HOUR);
		
		g_log_chestshop.SetLogModule(log);
		g_log_chestshop.SetCatagory("ChestShop",(real_logdir+"/chestshop/chestshop.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_movechess.SetLogModule(log);
		g_log_movechess.SetCatagory("MoveChess",(real_logdir+"/movechess/movechess.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_mount.SetLogModule(log);
		g_log_mount.SetCatagory("Mount",(real_logdir+"/mount/mount.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_task_stats.SetLogModule(log);	
		g_log_task_stats.SetCatagory("TaskState", (real_logdir+"/taskstats/taskstats.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_publicsale.SetLogModule(log);
		g_log_publicsale.SetCatagory("PublicSale", (real_logdir+"/publicsale/publicsale.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_mail.SetLogModule(log);	
		g_log_mail.SetCatagory("MailRoute", (real_logdir+"/route/mailroute.txt").c_str(), ILogModule::NFIS_HALF_HOUR);
		
		g_log_guild_route.SetLogModule(log);	
		g_log_guild_route.SetCatagory("GuildRoute", (real_logdir+"/route/guildroute.txt").c_str(), ILogModule::NFIS_HALF_HOUR);
		
		g_log_npcshop.SetLogModule(log);
		g_log_npcshop.SetCatagory("NpcShop", (real_logdir +"/npcshop/npcshop.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_shop.SetLogModule(log);
		g_log_shop.SetCatagory("Shop", (real_logdir +"/shop/shop.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_convertshop.SetLogModule(log);
		g_log_convertshop.SetCatagory("ConvertShop", (real_logdir +"/convertshop/convertshop.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_roleactivity.SetLogModule(log);
		g_log_roleactivity.SetCatagory("RoleActivity", (real_logdir +"/roleactivity/roleactivity.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_rand_activity.SetLogModule(log);
		g_log_rand_activity.SetCatagory("RandActivity", (real_logdir +"/randactivity/randactivity.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_qingyuan.SetLogModule(log);
		g_log_qingyuan.SetCatagory("Qingyuan", (real_logdir +"/qingyuan/qingyuan.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_mentality.SetLogModule(log);
		g_log_mentality.SetCatagory("Mentality", (real_logdir +"/mentality/mentality.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_roledata.SetLogModule(log);
		g_log_roledata.SetCatagory("RoleData", (real_logdir +"/roledata/roledata.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_marry.SetLogModule(log);
		g_log_marry.SetCatagory("Marry", (real_logdir + "/marry/marry.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_function_stats.SetLogModule(log);
		g_log_function_stats.SetCatagory("FunctionStats", (real_logdir +"/functionstats/functionstats.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_role_rename.SetLogModule(log);
		g_log_role_rename.SetCatagory("RoleRename", (real_logdir +"/role_rename/role_rename.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_role_upgrade.SetLogModule(log);
		g_log_role_upgrade.SetCatagory("RoleUpgrade", (real_logdir +"/role_upgrade/role_upgrade.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_friends.SetLogModule(log);
		g_log_friends.SetCatagory("Friends", (real_logdir + "/friends/friends.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_drop.SetLogModule(log);
		g_log_drop.SetCatagory("Drop", (real_logdir + "/drop/drop.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_welfare.SetLogModule(log);
		g_log_welfare.SetCatagory("Welfare", (real_logdir + "/welfare/welfare.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_monitor.SetLogModule(log);
		g_log_monitor.SetCatagory("Monitor", (real_logdir + "/monitor/monitor.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_sundry.SetLogModule(log);
		g_log_sundry.SetCatagory("Sundry", (real_logdir + "/sundry/sundry.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_cmd.SetLogModule(log);
		g_log_cmd.SetCatagory("Cmd", (real_logdir+"/cmd/cmd.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_debug.SetLogModule(log);
		g_log_debug.SetCatagory("GameDebug",(real_logdir+"/gamedebug/gamedebug.txt").c_str());

		g_log_serious_error.SetLogModule(log);
		g_log_serious_error.SetCatagory("SeriousError",(real_logdir+"/seriouserror/seriouserror.txt").c_str());

		g_log_disconnect.SetLogModule(log);
		g_log_disconnect.SetCatagory("Disconnect", (real_logdir+"/disconnect/disconnect.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_login.SetLogModule(log);
		g_log_login.SetCatagory("Login", (real_logdir+"/login/login.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_gm.SetLogModule(log);
		g_log_gm.SetCatagory("GM", (real_logdir+"/gm/gm.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_strengthen_test.SetLogModule(log);
		g_log_strengthen_test.SetCatagory("StrengthenTest", (real_logdir+"/gm/strengthentest.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_forbid.SetLogModule(log);
		g_log_forbid.SetCatagory("Forbid", (real_logdir + "/forbid/forbid.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_quick.SetLogModule(log);
		g_log_quick.SetCatagory("quick", (real_logdir + "/quick/quick.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_guild.SetLogModule(log);
		g_log_guild.SetCatagory("Guild", (real_logdir+"/guild/guild.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_p2ptrade.SetLogModule(log);
		g_log_p2ptrade.SetCatagory("p2ptrade", (real_logdir+"/p2ptrade/p2ptrade.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_activity.SetLogModule(log);
		g_log_activity.SetCatagory("Activity", (real_logdir+"/activity/activity.txt").c_str(), ILogModule::NFIS_DAY);

		g_log_challengefield.SetLogModule(log);
		g_log_challengefield.SetCatagory("ChallengeField", (real_logdir+"/challengefield/challengefield.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_other.SetLogModule(log);
		g_log_other.SetCatagory("Other", (real_logdir+"/other/other.txt").c_str(), ILogModule::NFIS_HALF_HOUR);
		
		g_log_fb.SetLogModule(log);
		g_log_fb.SetCatagory("FB", (real_logdir+"/fb/fb.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_worldboss.SetLogModule(log);
		g_log_worldboss.SetCatagory("worldboss", (real_logdir+"/worldboss/worldboss.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_wabao.SetLogModule(log);
		g_log_wabao.SetCatagory("wabao", (real_logdir+"/wabao/wabao.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_question.SetLogModule(log);
		g_log_question.SetCatagory("question", (real_logdir+"/question/question.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_role_num.SetLogModule(log);
		g_log_role_num.SetCatagory("RoleNum", (real_logdir+"/rolenum/rolenum.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_global_user_state.SetLogModule(log);
		g_log_global_user_state.SetCatagory("GLOBAL_USER", (real_logdir+"/userstate/userstate.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_husong_task.SetLogModule(log);
		g_log_husong_task.SetCatagory("husongtask", (real_logdir+"/husongtask/husongtask.txt").c_str(), ILogModule::NFIS_HALF_HOUR);
		
		g_log_guild_task.SetLogModule(log);
		g_log_guild_task.SetCatagory("guildtask", (real_logdir+"/guildtask/guildtask.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_daily_task.SetLogModule(log);
		g_log_daily_task.SetCatagory("dailytask", (real_logdir+"/dailytask/dailytask.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_month_card.SetLogModule(log);
		g_log_month_card.SetCatagory("monthcard", (real_logdir+"/monthcard/monthcard.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_touzijihua.SetLogModule(log);
		g_log_touzijihua.SetCatagory("touzijihua", (real_logdir+"/touzijihua/touzijihua.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_fish_pool.SetLogModule(log);
		g_log_fish_pool.SetCatagory("fishpool", (real_logdir+"/fishpool/fishpool.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_combine_server.SetLogModule(log);
		g_log_combine_server.SetCatagory("combineserver", (real_logdir+"/combineserver/combineserver.txt").c_str(), ILogModule::NFIS_HALF_HOUR);
		
		g_log_cardzu.SetLogModule(log);
		g_log_cardzu.SetCatagory("cardzu", (real_logdir+"/cardzu/cardzu.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_rank.SetLogModule(log);
		g_log_rank.SetCatagory("rank", (real_logdir+"/rank/rank.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_cross.SetLogModule(log);
		g_log_cross.SetCatagory("cross", (real_logdir+"/cross/cross.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_lieming.SetLogModule(log);
		g_log_lieming.SetCatagory("lieming", (real_logdir+"/lieming/lieming.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_jingling.SetLogModule(log);
		g_log_jingling.SetCatagory("jingling", (real_logdir+"/jingling/jingling.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_chengjiu.SetLogModule(log);
		g_log_chengjiu.SetCatagory("chengjiu", (real_logdir+"/chengjiu/chengjiu.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_shengwang.SetLogModule(log);
		g_log_shengwang.SetCatagory("shengwang", (real_logdir+"/shengwang/shengwang.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_tuhaojin.SetLogModule(log);
		g_log_tuhaojin.SetCatagory("tuhaojin", (real_logdir+"/tuhaojin/tuhaojin.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_bigchatface.SetLogModule(log);
		g_log_bigchatface.SetCatagory("bigchatface", (real_logdir+"/bigchatface/bigchatface.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_shenzhou_weapon.SetLogModule(log);
		g_log_shenzhou_weapon.SetCatagory("shenzhou_weapon", (real_logdir+"/shenzhou_weapon/shenzhou_weapon.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_baby.SetLogModule(log);
		g_log_baby.SetCatagory("baby", (real_logdir+"/baby/baby.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_pet.SetLogModule(log);
		g_log_pet.SetCatagory("pet", (real_logdir+"/pet/pet.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_appearance.SetLogModule(log);
		g_log_appearance.SetCatagory("Appearance", (real_logdir +"/appearance/appearance.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_huashen.SetLogModule(log);
		g_log_huashen.SetCatagory("huashen", (real_logdir+"/huashen/huashen.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_multi_mount.SetLogModule(log);
		g_log_multi_mount.SetCatagory("multi_mount", (real_logdir+"/multi_mount/multi_mount.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_personalize_window.SetLogModule(log);
		g_log_personalize_window.SetCatagory("personalize_window", (real_logdir+"/personalize_window/personalize_window.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_magic_card.SetLogModule(log);
		g_log_magic_card.SetCatagory("magic_card", (real_logdir+"/magic_card/magic_card.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_mitama.SetLogModule(log);
		g_log_mitama.SetCatagory("mitama", (real_logdir+"/mitama/mitama.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_chinese_zodiac.SetLogModule(log);
		g_log_chinese_zodiac.SetCatagory("chinese_zodiac", (real_logdir+"/chinese_zodiac/chinese_zodiac.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_xiannv_shouhu.SetLogModule(log);
		g_log_xiannv_shouhu.SetCatagory("xiannv_shouhu", (real_logdir+"/xiannv_shouhu/xiannv_shouhu.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_jingling_fazhen.SetLogModule(log);
		g_log_jingling_fazhen.SetCatagory("jingling_fazhen", (real_logdir+"/jingling_fazhen/jingling_fazhen.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_jingling_guanghuan.SetLogModule(log);
		g_log_jingling_guanghuan.SetCatagory("jingling_guanghuan", (real_logdir+"/jingling_guanghuan/jingling_guanghuan.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_zhuansheng_equip.SetLogModule(log);
		g_log_zhuansheng_equip.SetCatagory("zhuansheng_equip", (real_logdir+"/zhuansheng_equip/zhuansheng_equip.txt").c_str(), ILogModule::NFIS_HALF_HOUR);
		
		g_log_little_pet.SetLogModule(log);
		g_log_little_pet.SetCatagory("littlepet", (real_logdir + "/littlepet/littlepet.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_equip_suit.SetLogModule(log);
		g_log_equip_suit.SetCatagory("equip_suit", (real_logdir+"/equip_suit/equip_suit.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_rune_system.SetLogModule(log);
		g_log_rune_system.SetCatagory("rune_system", (real_logdir+"/rune_system/rune_system.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_shenge_system.SetLogModule(log);
		g_log_shenge_system.SetCatagory("shenge_system", (real_logdir+"/rune_system/shenge_system.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_shenbing.SetLogModule(log);
		g_log_shenbing.SetCatagory("shenbing_system", (real_logdir+"/rune_system/shenbing_system.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_title.SetLogModule(log);
		g_log_title.SetCatagory("title", (real_logdir+"/title/title.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_fighting.SetLogModule(log);
		g_log_fighting.SetCatagory("fighting", (real_logdir + "/fighting/fighting.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_shenshou.SetLogModule(log);
		g_log_shenshou.SetCatagory("shenshou", (real_logdir + "/shenshou/shenshou.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_new_protocol.SetLogModule(log);
		g_log_new_protocol.SetCatagory("new_protocol", (real_logdir + "/new_protocol/new_protocol.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_tianxiang.SetLogModule(log);
		g_log_tianxiang.SetCatagory("new_protocol", (real_logdir + "/new_protocol/tianxiang.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_fishing_exchange.SetLogModule(log);
		g_log_fishing_exchange.SetCatagory("fish_exchange", (real_logdir + "/fish_exchange/fish_exchange.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_fishing.SetLogModule(log);
		g_log_fishing.SetCatagory("fishing", (real_logdir + "/fishing/fishing.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_element_heart.SetLogModule(log);
		g_log_element_heart.SetCatagory("element_heart", (real_logdir + "/element_heart/element_heart.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_upgrade.SetLogModule(log);
		g_log_upgrade.SetCatagory("upgrade", (real_logdir + "/upgrade/upgrade.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_shengqi.SetLogModule(log);
		g_log_shengqi.SetCatagory("shengqi", (real_logdir + "/shengqi/shengqi.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_yaoshi.SetLogModule(log);
		g_log_yaoshi.SetCatagory("yaoshi", (real_logdir + "/yaoshi/yaoshi.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_toushi.SetLogModule(log);
		g_log_toushi.SetCatagory("toushi", (real_logdir + "/toushi/toushi.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_qilinbi.SetLogModule(log);
		g_log_qilinbi.SetCatagory("qilinbi", (real_logdir + "/qilinbi/qilinbi.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_mask.SetLogModule(log);
		g_log_mask.SetCatagory("mask", (real_logdir + "/mask/mask.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_zhaunzhi_equip.SetLogModule(log);
		g_log_zhaunzhi_equip.SetCatagory("zhuanzhiequip", (real_logdir + "/zhuanzhiequip/zhuanzhiequip.txt").c_str(), ILogModule::NFIS_HALF_HOUR);
		
		g_log_seal.SetLogModule(log);
		g_log_seal.SetCatagory("seal", (real_logdir + "/seal/seal.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_role_big_small_goal.SetLogModule(log);
		g_log_role_big_small_goal.SetCatagory("seal", (real_logdir + "/role_big_small_goal/role_big_small_goal.txt").c_str(), ILogModule::NFIS_HALF_HOUR);
		
		g_log_shenqi.SetLogModule(log);
		g_log_shenqi.SetCatagory("shenqi", (real_logdir + "/shenqi/shenqi.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_xianzunka.SetLogModule(log);
		g_log_xianzunka.SetCatagory("xianzunka", (real_logdir + "/xianzunka/xianzunka.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_equip_baptize.SetLogModule(log);
		g_log_equip_baptize.SetCatagory("equipbaptize", (real_logdir + "/equipbaptize/equipbaptize.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_greate_soldier.SetLogModule(log);
		g_log_greate_soldier.SetCatagory("greate_soldier", (real_logdir + "/greate_soldier/greate_soldier.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_cross_rand_activity.SetLogModule(log);
		g_log_cross_rand_activity.SetCatagory("cross_randactivity", (real_logdir + "/cross_randactivity/cross_randactivity.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_jinjie_sys_reward.SetLogModule(log);
		g_log_jinjie_sys_reward.SetCatagory("jinjie_sys_reward", (real_logdir + "/jinjie_sys_reward/jinjie_sys_reward.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_baizhan_equip.SetLogModule(log);
		g_log_baizhan_equip.SetCatagory("baizhanequip", (real_logdir + "/baizhanequip/baizhanequip.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_global_role.SetLogModule(log);
		g_log_global_role.SetCatagory("globalrole", (real_logdir + "/globalrole/globalrole.txt").c_str(), ILogModule::NFIS_HALF_HOUR);

		g_log_zodiac.SetLogModule(log);
		g_log_zodiac.SetCatagory("zodiac", (real_logdir + "/zodiac/zodiac.txt").c_str(), ILogModule::NFIS_HALF_DAY);

		g_log_tianshenhuti.SetLogModule(log);
		g_log_tianshenhuti.SetCatagory("tianshenhuti", (real_logdir + "/tianshenhuti/tianshenhuti.txt").c_str(), ILogModule::NFIS_HALF_HOUR);
	}
}

