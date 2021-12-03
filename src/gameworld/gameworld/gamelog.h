#ifndef __GAMELOG_H__
#define __GAMELOG_H__

#include "ilogmodule.h"
#include "servercommon/servercommon.h"
#include "servercommon/logdef.h"

class ILogModule;
struct ItemParamDataStruct;

namespace gamelog
{
	extern LogAgent g_log_world;
	extern LogAgent g_log_scenemanager;
	extern LogAgent g_log_scene;
	extern LogAgent g_log_messagehandler;

	extern LogAgent g_log_scenestatus;			// ����״̬��־
	extern LogAgent g_log_save_error;			// RMI�洢����
	extern LogAgent g_log_msgmoniter;			// �ڴ�ؼ��

	extern LogAgent g_log_moneygold;			// Ԫ����־
	extern LogAgent g_log_moneycoin;			// ͭǮ��־
	extern LogAgent g_log_knapsack;				// ��ɫ�����ֿ���־
	extern LogAgent g_log_item;					// ��Ʒ������־
	extern LogAgent g_log_xiannv;				// ��Ů��־
	extern LogAgent g_log_wing;					// ������־
	extern LogAgent g_log_halo;					// �⻷��־
	extern LogAgent g_log_shengong;				// ����־
	extern LogAgent g_log_shenyi;				// ������־
	extern LogAgent g_log_footprint;			// �㼣��־
	extern LogAgent g_log_cloak;				// ������־
	extern LogAgent g_log_equipment;			// װ��������־
	extern LogAgent g_log_task;					// ������־
	extern LogAgent g_log_task_stats;			// ����ͳ����־
	extern LogAgent g_log_vip;					// VIP��־
	extern LogAgent g_log_chestshop;			// �����̵�
	extern LogAgent g_log_movechess;			// ������
	extern LogAgent g_log_mount;				// ������־
	extern LogAgent g_log_publicsale;			// ������־
	extern LogAgent g_log_mail;					// �ʼ���־
	extern LogAgent g_log_guild_route;			// ������־
	extern LogAgent g_log_npcshop;				// npc�̵�
	extern LogAgent g_log_shop;					// �̳�
	extern LogAgent g_log_convertshop;			// �һ��̵�
	extern LogAgent g_log_roleactivity;			// ��ɫ�
	extern LogAgent g_log_rand_activity;		// ����
	extern LogAgent g_log_qingyuan;				// ��Ե
	extern LogAgent g_log_roledata;				// �ɻ�
	extern LogAgent g_log_marry;				// ���

	extern LogAgent g_log_mentality;			// ����
	extern LogAgent g_log_function_stats;		// ����ͳ����־
	extern LogAgent g_log_role_rename;			// ��ɫ����
	extern LogAgent g_log_role_upgrade;			// ��ɫ����
	extern LogAgent g_log_friends;				// ����ϵͳ��־
	extern LogAgent g_log_drop;					// ������־

	extern LogAgent g_log_welfare;				// ������־
	extern LogAgent g_log_monitor;				// �����־
	extern LogAgent g_log_sundry;				// ������־

	extern LogAgent g_log_cmd;					// �ⲿϵͳ������־
	extern LogAgent g_log_debug;				// ����һЩ������־���
	extern LogAgent g_log_serious_error;		// ���ش��� 
	extern LogAgent g_log_disconnect;			// �ϵ�������־
	extern LogAgent g_log_login;				// ��¼�ǳ���־
	extern LogAgent g_log_gm;					// GM������־
	extern LogAgent g_log_strengthen_test;		// ǿ��ͳ����־
	extern LogAgent g_log_forbid;				// ���Է����־
	extern LogAgent g_log_quick;				// ������־
		
	extern LogAgent g_log_guild;				// ������־
	extern LogAgent g_log_activity;				// ���־
	extern LogAgent g_log_p2ptrade;				// p2p������־
	extern LogAgent g_log_challengefield;		// ��������־
	extern LogAgent g_log_other;				// һЩ����
	extern LogAgent g_log_fb;					// ������־
	extern LogAgent g_log_worldboss;			// ����boss��־
	extern LogAgent g_log_wabao;				// �ڱ���־
	extern LogAgent g_log_question;				// ������־

	extern LogAgent g_log_role_num;				// ����������־ ��login���ƹ�����
	extern LogAgent	g_log_global_user_state;	// �û���������־ ��login���ƹ�����

	extern LogAgent g_log_husong_task;			// ����������־
	extern LogAgent g_log_guild_task;			// ����������־
	extern LogAgent g_log_daily_task;			// �ճ�������־
	extern LogAgent g_log_month_card;			// �¿�
	extern LogAgent g_log_touzijihua;			// Ͷ�ʼƻ�
	extern LogAgent g_log_fish_pool;			// ���
	extern LogAgent g_log_combine_server;		// �Ϸ��
	extern LogAgent g_log_cardzu;				// �������
	extern LogAgent g_log_rank;					// ���а�
	extern LogAgent g_log_cross;				// ���
	extern LogAgent g_log_lieming;				// ����
	extern LogAgent g_log_jingling;				// ����
	extern LogAgent g_log_chengjiu;				// �ɾ�
	extern LogAgent g_log_shengwang;			// ����
	extern LogAgent g_log_tuhaojin;				// ������
	extern LogAgent g_log_bigchatface;			// ��������
	extern LogAgent g_log_shenzhou_weapon;		// ��������
	extern LogAgent g_log_baby;					// ����
	extern LogAgent g_log_pet;					// ����
	extern LogAgent g_log_appearance;			// ������
	extern LogAgent g_log_huashen;				// ����ϵͳ
	extern LogAgent g_log_multi_mount;			// ˫������
	extern LogAgent g_log_personalize_window;	// ���Ի�����
	extern LogAgent g_log_magic_card;			// ħ��
	extern LogAgent g_log_mitama;				// ����
	extern LogAgent g_log_chinese_zodiac;		// ����ϵͳ
	extern LogAgent g_log_xiannv_shouhu;		// ��Ů�ػ�
	extern LogAgent g_log_jingling_fazhen;		// ���鷨��
	extern LogAgent g_log_jingling_guanghuan;	// ����⻷
	extern LogAgent g_log_zhuansheng_equip;		// ת��ϵͳ
	extern LogAgent g_log_little_pet;			// С����
	extern LogAgent g_log_equip_suit;			// ������װ
	extern LogAgent g_log_rune_system;			// ����ϵͳ
	extern LogAgent g_log_shenge_system;		// ���ϵͳ
	extern LogAgent g_log_shenbing;				// ���ϵͳ
	extern LogAgent g_log_title;				// �ƺ�
	extern LogAgent g_log_fighting;				// ������ϵͳ
	extern LogAgent g_log_shenshou;				// ����
	extern LogAgent g_log_new_protocol;			// ��Э����־
	extern LogAgent g_log_tianxiang;			// ����ϵͳ

	extern LogAgent g_log_fishing_exchange;		// ����һ�
	extern LogAgent g_log_fishing;				// ������־
	extern LogAgent g_log_element_heart;		// Ԫ��֮��
	extern LogAgent g_log_upgrade;				// ����ϵͳ

	extern LogAgent g_log_shengqi;				// ʥ��ϵͳ

	extern LogAgent g_log_yaoshi;				// ����
	extern LogAgent g_log_toushi;				// ͷ��
	extern LogAgent g_log_qilinbi;				// �����
	extern LogAgent g_log_mask;					// �����־
	extern LogAgent g_log_zhaunzhi_equip;		// תְװ����־
	extern LogAgent g_log_seal;					// ʥӡϵͳ

	extern LogAgent g_log_role_big_small_goal;  // ��ɫ��СĿ��
	
	extern LogAgent g_log_shenqi;				// ����

	extern LogAgent g_log_xianzunka;			// ����

	extern LogAgent g_log_equip_baptize;        // װ��ϴ��
	extern LogAgent g_log_greate_soldier;		// ����

	extern LogAgent g_log_cross_rand_activity;	// �������

	extern LogAgent g_log_jinjie_sys_reward;	// ����ϵͳ��СĿ��

	extern LogAgent g_log_baizhan_equip;		// ��սװ����־

	extern LogAgent g_log_global_role;			// ȫ�ֽ�ɫ��־
	extern LogAgent g_log_tianshenhuti;			// ��ĩװ��

	extern LogAgent g_log_zodiac;				// ʮ����Ф

	extern bool g_open_console_log; 

	void LogInit(ILogModule *log, const std::string &logdir, bool open_console_log, int server_index);
}

#define CONSOLE_DEBUG(format, ...) \
	if (gamelog::g_open_console_log) printf("File: " __FILE__ ", Line: %05d: " format "\n", __LINE__, ##__VA_ARGS__); fflush(stdout);

#define ROLE_LOG_QUICK6(t, r, n1, n2, z1, z2) \
	LOG_QUICK8(gamelog::g_log_quick, (t), UidToInt((r)->GetUserId()), (r)->GetName(), (r)->GetPlatName(), (n1), (n2), (z1), (z2))

#define ROLE_LOG_QUICK10(t, r, n1, n2, z1, z2, n3, n4, n5, n6) \
	LOG_QUICK12(gamelog::g_log_quick, (t), UidToInt((r)->GetUserId()), (r)->GetName(), (r)->GetPlatName(), (n1), (n2), (z1), (z2), (n3), (n4), (n5), (n6))

#endif

