#ifndef __NOTICENUM_H__
#define __NOTICENUM_H__

namespace noticenum
{
	//�ɹ���ʾ�� ��10000��ʼ ��Ҫ����100000

	enum NOTICE_NUM_DEF
	{
		NT_GUILD_UNION_APPLYFOR_SUCC = 10000,						//�����������˳ɹ�
		NT_GUILD_RESET_NAME_SUCC,									//���˸����ɹ�
		NT_GUILD_APPLYFOR_SETUP_SAVE_SUCC,							//�������ñ���ɹ�
		NT_GUILD_GET_FULI_GIFT_SUCC,								//��ȡ�ɹ�
		NT_GUILD_SET_AUTO_KICKOUT_SETUP_SUCC,						//���óɹ�
		NT_GUILD_INVITE_SUCC,										//��������ɹ�
		NT_GUILD_GET_BUFF_SUCC,										//����BUFF��ȡ�ɹ�
		NT_GUILD_BOSS_CALL_SUCC,									//����BOSS�ٻ��ɹ�

		NT_EQUIP_COMPOSE_SUCC = 10500,								//�ϳɳɹ�
		NT_EQUIP_EXCHANGE_SUCC,										//�һ��ɹ�

		NT_EQUIP_INLAY_GEMSTONE_SUCC = 10600,						//װ����Ƕ�ɹ�
		NT_EQUIP_UNINLAY_GEMSTONE_SUCC,								//��ʯ����ɹ�
		NT_EQUIP_UP_STAR_SUCC,										//װ�����ǳɹ�
		NT_EQUIP_FLUSH_SUCC,										//װ��ϴ���ɹ�
		NT_EQUIP_FLUSH_REPLACE_SUCC,								//װ��ϴ�������滻�ɹ�
		NT_EQUIP_UPQUALITY_SUCC,									//װ������Ʒ�ʳɹ�
		NT_EQUIP_UPLEVEL_SUCC,										//װ�������ɹ�
		NT_EQUIP_DECOMPOSE_SUCC,									//װ���ֽ�ɹ�
		NT_EQUIP_DRESS_APPE_SELECT_SUCC,							//ʱװװ�籣��ɹ�
		NT_EQUIP_REFINE_SUCC,										//װ�������ɹ� 
		NT_EQUIP_EXTEND_SUCC,										//װ���̳гɹ�
		NT_SZ_EXPIRED,												//ʱװ�Ѿ�����
		NT_SZ_STRENGTH_SUCC,										//ʱװ�����ɹ�
		NT_SZ_FOREVER,												//��ϲ�㣬ʱװ�������ʱЧ
		NT_EQUIP_REPAIR_SUCC,										//װ������ɹ�
		NT_WEDDINGRING_LIGHTEN_SUCC,								//�����ɹ�
		NT_WEDDINGRING_UP_SUCC,										//��������ɹ�
		NT_WEDDINGRING_LIGHTEN_FAIL,								//����ʧ��
		NT_INLAY_GEMSTONE_SUCC,										//��ʯ��Ƕ�ɹ�
		NT_UNINLAY_GEMSTONE_SUCC,									//��ʯժ���ɹ�
		NT_EQUIP_SHENZHU_SUCC,										//װ�������ɹ�
		NT_EQUIP_FULING_SUCC,										//װ������ɹ�
		NT_STONE_UP_SUCC,											//��ʯ�����ɹ�

		NT_NO_UES_10700 = 10700,									//������
		NT_MOUNT_STRENGTHEN_SUCC,									//����ǿ���ɹ�
		NT_MOUNT_STRENGTHEN_FAIL,									//����ǿ��ʧ��
		NT_MOUNT_ACTIVE_IMG_FAIL,									//��������ʧ��
		NT_MOUNT_LIFT_CRIT_1,										//�������С������
		NT_MOUNT_LIFT_CRIT_2,										//������״󱩻���
		NT_MOUNT_FEED_SUCC,											//����ι���ɹ�
		NT_MOUNT_FEED_SUCC_AND_LEVELUP,								//����ι���ɹ���ι���ȼ�������
		NT_MOUNT_FEED_CRIT_1,										//����ι��С������
		NT_MOUNT_FEED_CRIT_2,										//����ι���󱩻���
		NT_MOUNT_FLUSH_SUCC,										//����ѱ���ɹ�
		NT_MOUNT_FUSION_SUCC,										//�����ںϳɹ�
		NT_MOUNT_ACTIVE_IMG_SUCC,									//��������ɹ�
		NT_MOUNT_IMG_EXPIRED,										//�û������Ѿ�����
		NT_MOUNT_UP_VAL_ADD_1_SUCC,									//���1��ͻ��ֵ
		NT_QIBING_LIFT_SUCC,										//��������ɹ�
		NT_SELECT_APPERANCE_SUCC,									//ѡ������ɹ�

		NT_JINGLING_GUANGHUAN_LIFT_SUCC,							//����⻷���׳ɹ�
		NT_JINGLING_FAZHEN_LIFT_SUCC,								//���鷨����׳ɹ�

		NT_SHOP_BUY_SUCC = 10800,									//����ɹ�
		NT_NPCSHOP_SELL_SUCC,										//���۳ɹ�
		NT_CONVERTSHOP_CONVERT_SUCC,								//�һ��ɹ�
		NT_READE_REQ_SUCC,											//�����׳ɹ����ȴ��Է�ȷ��
		NT_PASS_TOWER_GET_REWARD_SUCC,								//��ȡ�����ɹ�
		NT_EXPDITION_GET_REWARD_SUCC,								//��ȡ�����ɹ�
		NT_SELECT_PROF_1_SUCC,										//��ϲ��ɹ�ѡ����ְҵ
		NT_SELECT_PROF_2_SUCC,										//��ϲ��ɹ�ѡ��Ӱ��ְҵ
		NT_SELECT_PROF_3_SUCC,										//��ϲ��ɹ�ѡ�����ְҵ
		NT_SELECT_PROF_4_SUCC,										//��ϲ��ɹ�ѡ������ְҵ
		NT_SELECT_CAMP_1_SUCC,										//��ϲ��ɹ�ѡ����ɽʦ��
		NT_SELECT_CAMP_2_SUCC,										//��ϲ��ɹ�ѡ����ʦ��
		NT_SELECT_CAMP_3_SUCC,										//��ϲ��ɹ�ѡ��ٻ�ʦ��
		NT_TRADE_OTHER_IS_TRAING,									//��������ڽ�����

		NT_GUILD_SIGNIN_SUCC = 10900,								//����ǩ���ɹ�
		NT_GUILD_GET_SHENLU_BIND_COIN_SUCC,							//��ȡ������¯ͭ�ҳɹ�
		EN_GUILD_BLESSING_SUCC,										//ף���ɹ�
		EN_GUILD_INVITE_BLESSING_SUCC,								//����ף���ɹ�
		EN_GUILD_CALL_IN_SUCC,										//������ļ�ɹ�
		EN_GUILD_FETCH_REDBAG_SUCC,									//��ȡ����ɹ�

		NT_NO_USE_11000 = 11000,									//������
		NT_NO_USE_11001,											//������
		NT_PET_LIFT_WUXING_SUCC,									//�����������Գɹ�
		NT_PET_LIFT_LINGXING_CRIT,									//�����������Ա�����
		NT_PET_LIFT_LINGXING_SUCC,									//�����������Գɹ�
		NT_PET_PROMOTE_SUCC,										//���������ɳ��ɹ�
		NT_PET_CHEAT_ROLLING,										//ҡ�������ױ�ץס
		NT_PET_SKILL_LEARNT_SUCC,									//����ѧϰ�ɹ�
		NT_PET_SKILL_UPGRADE_SUCC,									//���������ɹ�
		NT_PET_SKILL_FORGET_SUCC,									//���������ɹ�
		NT_PET_MERGE_SUCC,											//�����ںϳɹ�
		NT_PET_FEED_SUCC,											//����ι���ɹ�
		NT_PET_LINGXUE_PROMOTE_SUCC,								//������Ѩ�����ɹ�
		NT_PET_IMAGE_ACTIVATE_SUCC,									//����ͼ������ɹ�
		NT_PET_IMAGE_PROMOTE_SUCC,									//����ͼ�������ɹ�
		NT_PET_IMAGE_TRANSFORM_SUCC,								//����ͼ���û��ɹ�
		NT_PET_IMAGE_RESTORE_SUCC,									//����ͼ����ԭ�ɹ�
		NT_PET_CONVERT_SUCC,										//����ת���ɹ�
		NT_PET_CONVERT_ITEM_TAKE_SUCC,								//�ɹ���ȡ����ת����
		
		NT_MENTALITY_SPEED_SUCC = 11100,							//���������ȴʱ��ɹ�
		NT_NO_USE_11101,											//������
		NT_MENTALITY_VIP_FREE_SPEED_SUCC,							//VIP��Ѽ��ٳɹ�

		NT_FRIEND_ADD_SUCCESS = 11200,								//��Ӻ��ѳɹ�
		NT_FRIEND_BLESS_FETCH_TOTAL_EXP_SUCCESS,					//��ȡ��«����ɹ�
		NT_OFFICIAL_GET_SALARY_SUCCESS,								//��ְٺ»��ȡ�ɹ�
		NT_WELFARE_GET_ACCOUNT_LOGIN_SUCCESS,						//�ۼƵ�½������ȡ�ɹ�
		EN_WELFARE_GET_WEEKLY_ACCLOGIN_SUCCESS,						//���ۼƸ�����ȡ�ɹ�
		NT_VIP_GET_WELFARE_SUCCESS,									//��ȡVIP�����ɹ�
		NT_WELFARE_GET_DAILYFIND_SUCCESS,							//�һ���ȡ�ɹ�
		NT_WELFARE_GET_DAILY_ONLINE_SUCCESS,						//��ȡ���߽����ɹ�
		NT_GET_OFFLINE_EXP_SUCCESS,									//��ȡ���߾���ɹ�
		NT_WELFARE_GET_PLAT_VIP_GIFT_SUCCESS,						//��ȡ����ɹ�
		NT_WELFARE_HAPPY_TREE_UPLEVEL,								//���ֹ��������ɹ�
		NT_WELFARE_EXCHANG_SUCCESS,									//�һ��ɹ�
		NT_WELFARE_SIGN_SUCCESS,									//ǩ���ɹ�
		NT_WELFARE_BEFORE_SIGN_SUCCESS,								//��ǩ�ɹ�
		NT_WELFARE_QUICK_SIGN_SUCCESS,								//һ��ǩ���ɹ�

		NT_RUNE_SYSTEM_SET_RUNE_SUCC,								//ս��װ���ɹ�
		NT_RUNE_SYSTEM_COMPOSE_SUCC,								//ս��ϳɳɹ�
		NT_RUNE_SYSTEM_DECOMPOSE_SUCC,								//ս��ֽ�ɹ�
		NT_RUNE_SYSTEM_RUNE_UP_LEVEL_SUCC,							//ս�������ɹ�
		NT_RUNE_SYSTEM_RUNE_UNLOAD_RUNE_SUCC,						//ս���ж�ɹ�
		NT_RUNE_SYSTEM_CONVERT_SUCC,								//�һ�ս��ɹ�

		NT_CHANGE_TITLE_SUCC = 11300,								//ѡ��ƺųɹ�
		NT_GET_TITLE_SUCC,											//��ϲ��óƺ�
		NT_TITLE_UPGRADE_SUCC,										//�ƺŽ��׳ɹ�
		NT_NEWHAND_GUILD_SUCC,										//��ϲ����ѡ��Ϊ����ָ��Ա

		NT_ADD_BLACK_SUCC = 11400,									//��Ӻ������ɹ�
		NT_REMOVE_BLACK_SUCC,										//ɾ���������ɹ�
		NT_REMOVE_FRIEND_SUCC,										//ɾ�����ѳɹ�
		NT_REMOVE_ENEMY_SUCC,										//ɾ�����˳ɹ�
		NT_GIVE_FLOWER_SUCCESS,										//�����ʻ��ɹ�
		NT_THANK_BLESS_SUCCESS,										//����ɹ�
		NT_SEND_ADD_FRIEND_REQ_SUCC,								//�������뷢�ͳɹ�

		NT_REST_INVITE_SUCC = 11500,								//˫�������ѷ���
		NT_TEAM_CALL_TOGETHER_SUCC,									//�ټ����ͳɹ�
		NT_TEAM_JOIN_SUCC,											//�ɹ��������
		NT_TEAM_SET_TOWERDEFEND_ATTRTYPE_SUCC,						//�������ͳɹ�
		NT_TEAM_INVITE_REQ_SUCC,									//������뷢�ͳɹ�
		NT_TEAM_JOIN_REQ_SUCC,										//�ѷ����������

		NT_LEARN_SKILL_SUCC = 11600,								//����ѧϰ�ɹ�
		NT_SKILL_UP_SUCC,											//���������ɹ�

		NT_CARD_TAKEOFF_SUCC = 11650,								//���Ƴɹ�ж��
		NT_CARD_PUTON_SUCC,											//���Ƴɹ�װ��
		NT_CARD_STRENGTH_SUCC,										//����ǿ���ɹ�
		NT_CARD_TAKE_REWARD_SUCC,									//���ƽ�����ȡ�ɹ�

		NT_DUOBAOXIANBING_CONVERT_SUCC = 11700,						//�һ�����ɹ�

		NT_LINGMAI_GATHER_SUCC = 11800,								//�����ɼ��ɹ�
		NT_LINGZHUPLANT_SUCC,										//����������óɹ�
		NT_ZHADAN_PUT_SUCC,											//��������óɹ�
		NT_SOULING_FIND_LINGZHU,									//�ɹ��ѵ�һ������	

		NT_LINGSHANXIANQI_CAPTURE_SUCC,								//��ϲ������ɹ�
		NT_XIANBANG_TASK_REFRESH_SUCC,								//ˢ�³ɹ�������1���Ұ���
		NT_XIANBANG_TASK_FREE_REFRESH_SUCC,							//���ˢ�³ɹ�
		NT_TUMO_TASK_ITEM_REFRESH_COLOR_CHANGE_SUCC,				//������ħ�����ˢ�³ɹ�
		NT_TUMO_TASK_COIN_REFRESH_COLOR_CHANGE_SUCC,				//����ͭ�ң�����ˢ�³ɹ�
		NT_HUSONG_REFRESH_COLOR_SUCC,								//��Ůˢ�³ɹ�
		NT_TUMO_TASK_REFRESH_SUCC,									//��ħ����ˢ�³ɹ�
		NT_TUMO_TASK_ITEM_REFRESH_SUCC,								//������ħ�����Ʒ�ʲ���
		NT_TUMO_TASK_COIN_REFRESH_SUCC,								//����Ԫ��������Ʒ�ʲ���

		NT_NINGJU_SUCC,												//��ϲ�����۳ɹ�
		NT_ITEM_USE_SUCC,											//��Ʒʹ�óɹ�

		NT_GET_REWARD_SUCC,											//��ȡ�����ɹ�
		NT_GET_FIRST_CHONGZHI_GIFT_SUCC,							//��ȡ����ɹ�
		NT_ADD_SHENLONG_BAOZANG,									//�����������
		NT_ADD_YUANGU_BAOZANG,										//���Զ�ű���
		NT_GET_CLOSED_BETA_REWARD_SUCC,								//��ȡ��⽱���ɹ�
		NT_BUY_GONGJI_ADD_SUCC,										//����ɹ������100��ն

		NT_BUILDING_SUCC,											//���������ɹ�����ø�ǿ���ս������
		NT_TOUZIJIHUA_REWARD_SUCC,									//��ȡͶ�ʼƻ������ɹ�
		NT_XIANMENGZHAN_USE_DANYAO_SUCC,							//�ύ��ҩ�ɹ�
		NT_TRAIN_LONGHUI_SUCC,										//����ע��ɹ�
		NT_TIANCI_TONGBI_GATHER_SUCC,								//�ɼ��ɹ�
		NT_TIANCI_TONGBI_USE_GATHER_SUCC,							//�ύ�ɹ�
		
		NT_GET_COMMON_ACTIVITY_LOGIN_REWARD_SUCCESS,				//��ȡ��¼�����ɹ�
		NT_GET_COMMON_ACTIVITY_DUR_LOGIN_REWARD_SUCCESS,			//��ȡ������¼�����ɹ�
		NT_GET_COMMON_ACTIVITY_CONSUME_GOLD_REWARD_SUCCESS,			//��ȡ�������ѽ����ɹ�
		NT_COMMON_ACTIVITY_PICK_UP_HONGBAO_SUCCESS,					//ʰȡ�ɹ�
		NT_GET_COMMON_ACTIVITY_MEIGUI_DINGQIN_REWARD_SUCCESS,		//��ȡõ�嶨�齱���ɹ�
		NT_GET_COMMON_ACTIVITY_ACTIVE_REWARD_SUCCESS,				//��ȡ��Ծ�����ɹ�
		NT_GET_COMMON_ACTIVITY_SHIZHUANG_REWARD_SUCCESS,			//��ȡʱװ��ƴ�����ɹ�

		EN_GUILD_TASK_GIVEUP_TASK_SUCC,								//������������ɹ�
		EN_TASK_GIVEUP_SUCC,										//��������ɹ�

		NT_GET_COMMON_ACTIVITY_CYCLE_CONSUME_REWARD_SUCCESS,		//��ȡ�����������ɹ�
		NT_MENTALITY_COMBINE_UPGRADE_SUCCESS,						//�����ɹ�
		NT_MENTALITY_COMBINE_SUCCESS,								//����ɹ���Ů���ѳ�ս��
		NT_1VN_BET_SUCC,											//��л����֧��
		NT_MENTALITY_CANCLE_COMBINE_SUCCESS,						//�������ɹ�

		NT_BUY_XIUWEI_SUCCESS,										//Ĥ�ݳɹ�
		NT_GET_COMMON_ACTIVITY_CHEST_SHOP_BUY_REWARD_SUCCESS,		//��ȡ�Ź�Ѱ�������ɹ�

		NT_YUJIANQIUMINGSHAN_PICK_SKILL_SUCCESS,					//ʰȡ���ܳɹ�
		NT_DATING_INVITE_SUCCESS,									//Լ�����뷢�ͳɹ�
		NT_DATING_INVITE_CANCELED,									//Լ��������ȡ��

		NT_TAKE_REWARD_SUCC,										//��ȡ�����ɹ�
		NT_GET_COMMON_ACTIVITY_DAY_FIRST_CHONGZHI_REWARD_SUCCESS,	//��ȡÿ���׳佱���ɹ�
		NT_GET_COMMON_ACTIVITY_MOUNT_UPGRADE_REWARD_SUCCESS,		//��ȡ������׽����ɹ�
		NT_USE_YANHUA_SUCCESS,										//�ɹ���ȼ�����5000����
		NT_USE_MARRY_YANHUA_SUCCESS,								//�ɹ���ȼ
		NT_NEQ_AUTO_SUCC,											//ɨ�����
		NT_AUTO_ACTIIVTY_SUCC,										//��ȡ�ɹ�
		NT_ACTIVITY_FIND_SUCCESS,									//��ȡ�ɹ�

		NT_XIANNV_ONEKEY_PROMOTE_LEVEL_SUCC,						//Ů��һ�������ɹ�
		NT_PRAISE_SUCC,												//���޳ɹ�
		NT_TOWERDEFEND_NEXT_WAVE_SUCC,								//ˢ�³ɹ�����ע�����
		NT_XIANNV_ACTIVE_SUCC,										//��Ů����ɹ�
		NT_XIANNV_ADD_ZIZHI_SUCC,									//���������ɹ�
		NT_MAZE_FETCH_REWARD_SUCC,									//��ȡ�ɹ�
		NT_MAZE_BUY_MOVE_TIMES_SUCC,								//�����ж����ɹ�

		NT_QIFU_ROLL_COIN_SUCC,										//���ɹ�,��ô���ͭ��
		NT_QIFU_ROLL_XIANHUN,										//���ɹ�,��ô����ɻ�
		NT_QIFU_ROLL_YUANLI,										//���ɹ�,��ô�������

		NT_SPEAK_TALK_SUCC,											//�������ͳɹ�
		NT_SETATTACKMODE_PEACE,										//��ǰ���ں�ƽģʽ�����ɹ���������ң������ܱ�������ҹ���
		NT_SETATTACKMODE_GUILD,										//��ǰ��������ģʽ��ֻ�ɹ����������˵����
		NT_SETATTACKMODE_TEAM,										//��ǰ�������ģʽ��ֻ�ɹ�����������������
		NT_SETATTACKMODE_ALL,										//��ǰ����ȫ��ģʽ���ɹ����������
		NT_SETATTACKMODE_NAMECOLOR,									//��ǰ�����ƶ�ģʽ��ֻ���Թ������������
		NT_SETATTACKMODE_CAMP,										//��ǰ��������ģʽ��ֻ�ɹ����������ɵ����

		NT_XIANMENGZHAN_CALL_MEMBER_SUCC,							//�ټ����ͳɹ�
		NT_FOOTPRINT_ACTIVE_SUCC,									//�ɹ������㼣
		NT_MARRY_REQ_SUCC,											//��鷢��ɹ����ȴ��Է��ظ�
		NT_DIVORCE_REQ_SUCC,										//��鷢��ɹ�
		NT_MARRY_HONEYMOON_FETCH_REWARD_SUCC,						//����ף����Ʒ��ȡ�ɹ�
		NT_MARRY_HONEYMOON_ADD_DAYS_SUCC,							//����ף���������ӳɹ�
		NT_TRADE_ANOTHER_HAS_LOCK,									//�Է���������Ʒ
		NT_TRADE_ANOTHER_HAS_AFFIRM,								//�Է���ȷ�Ͻ���
		NT_TRADE_HAS_AFFIRM,										//��ȷ�Ͻ��ף��ȴ��Է��ظ�
		NT_TRADE_SUCC,												//���׳ɹ�
		NT_BUY_ENERGY_SUCC,											//���������ɹ�
		NT_BUY_ROLE_HP_SUCC,										//���򲹸��ɹ�
		NT_FRIEND_BLESS_SUCC,										//ף���ɹ���
		NT_FRIEND_FETCH_BLESS_REWARD,								//��ȡ�ɹ���
		NT_FRIEND_BLESS_INVATE_BLESS_SUCC,							//�ɹ��������ף��
		NT_HUNYAN_INVATE_SUCC,										//����ɹ�
		NT_HUNYAN_GATHAR_SUCC,										//�ɼ��ɹ�
		NT_HUNYAN_FA_HONGBAO_SUCC,									//������ɹ�
		NT_HUNYAN_PAO_HUAQIU_LEN_QIU,								//�׻�����ȴ��
		NT_HUNYAN_PAO_HUAQIU_SUCC,									//�׻���ɹ�
		NT_HUNYAN_PAO_HUAQIU_FAIL,									//�׻�������Ѿ�����
		NT_HUNYAN_SAHUA_SUCC,										//�����ɹ�
		NT_FISHPOOL_RAISE_SUCC,										//�����ɹ�
		NT_FISHPOOL_HARVEST_SUCC,									//����ɹ�
		NT_HUNYAN_ZHUFU_SUCC,										//ף���ѷ���
		NT_TITLE_USE_SUCC,											//����ɹ�
		NT_RAND_ACTIVITY_FETCH_REWARD_SUCC,			                //������ȡ�ɹ�
		NT_YIZHANDAODI_GUWU_SUCC,									//����ɹ�
		NT_MENTALITY_WUXING_UPLEVEL_SUCC,							//�����ɹ�
		NT_MENTALITY_WUXING_UPLEVEL_FAIL,							//����ʧ��
		NT_WUXINGGUAJI_GET_REWARD_SUCC,								//��ȡ�����ɹ���
		NT_INVITE_LUCKY_ZHUFU_SUCC,									//����ɹ���
		NT_ATTACK_MODE_FANJI,										//���Ѿ��л���ȫ��ģʽ���з���
		NT_MONTH_CARD_REWARD_FETCH_SUCCESS,							//��ȡ�ɹ�
		NT_MOUNT_FLY_UP_SUCC,										//��������ɹ�
		NT_CSA_FETCH_REWARD_SUCC,									//������ȡ�ɹ�
		NT_XIANNV_HALO_UPGRADE_SUCC,								//Ů��⻷�����ɹ�
		NT_ZHUANZHI_SUCC,											//�ɹ�����
		NT_WUSHUANG_EQUIP_DAZAO_SUCC,								//����ɹ�
		NT_FBGUWU_SUCC,												//����ɹ�����������50%
		NT_FISH_POOL_UP_FISH_QUALITY_SUCC,							//�����ɹ�

		NT_CARDZU_HUALING_SUCC,										//����ɹ�
		NT_CARDZU_LINGZHU_SUCC,										//�����ɹ�
		NT_CARDZU_ACTIVE_ZUHE_SUCC,									//��ϼ���ɹ�
		NT_CARDZU_UPGRADE_ZUHE_SUCC,								//��������ɹ�
		NT_CARDZU_BUY_LINGLI_SUCC,									//��������ɹ������Ϳ���

		NT_QINGYUAN_CARD_UPGRADE_SUCC,								//��Ե���������ɹ�
		NT_QINGYUAN_BUY_FB_TIMES_SUCC,								//��Ե������������ɹ�

		NT_CROSS_XIULUOTA_GUWU_SUCC,								//����ɹ�
		NT_CROSS_BUY_REALIVE_GUWU_SUCC,								//���򸴻�ɹ�

		NT_FETCH_CROSS_MULTIUSER_CHALLENGE_DAYCOUNT_REWARD_SUCCESS,	//��ȡ���3V3ÿ����뽱���ɹ�

		NT_LIFESKILL_MAKE_SUCC,										//�����ɹ�
		NT_OPERATE_SUCC,											//�����ɹ�
		NT_SPACE_CAI_SUCC,											//�ɹ��ڶԷ��ռ����½�ӡ
		NT_SAVE_SUCC,												//����ɹ�
		NT_PRESENT_SUCC,											//���ͳɹ�
		NT_COMMIT_SUCC,												//�ύ�ɹ�
		NT_STRENGTH_SUCC,											//ǿ���ɹ�
		NT_SELECT_SUCC,												//ѡ��ɹ�
		NT_STRENGTH_TAOZHUANG_SUCC,									//�ɹ�������װ����

		NT_SHENZHOU_WEAPON_UPGRADE_IDENTIFY_SUCC,					//��ϲ�㣬�����ɹ�
		NT_SHENZHOU_WEAPON_RECYCLE_SUCC,							//��ϲ�㣬���׳ɹ�
		NT_CAMP_BEAST_LEVEL_LIMIT,									//ʦ�������Ѵ���ߵȼ�
		NT_SHEN_ZHOU_HELP_BOX_SUCCESS,								//Э���ɹ�
		NT_SHEN_ZHOU_OPEN_BOX_SUCCESS,								//������ɹ�

		NT_BABY_RENAME_SUCC,										//���������ɹ�
		NT_BABY_UPGRADE_SUCC,										//�������׳ɹ�
		NT_BABY_BORN_SUCC,											//���޳ɹ���ϲ�񱦱�
		NT_BABY_CHAOSHENG_SUCC,										//���������ȡ�ɹ�
		NT_BABY_SPIRIT_UPGRADE_SUCC,								//�ػ���������

		NT_PET_FIGHT_OUT_SUCC,										//������ս�ɹ�
		NT_PET_ACTIVE_SUCC,											//��������ɹ�
		NT_PET_UPGRADE_SUCC,										//�������׳ɹ�
		NT_PET_RENAME_SUCC,											//���������ɹ�
		NT_PET_CALL_BACK_SUCC,										//����ȡ����ս�ɹ�

		NT_MINE_GATHER_SUCC,										//���Գɹ�

		NT_HUASHEN_CHANGE_IMAGE,									//�л���������ɹ�
		NT_HUASHEN_PUTOFF_IMAGE,									//ȡ����������

		NT_QINGYUAN_COUPLE_HALO_ICON_ACTIVATE_SUCC,					//���޹⻷ͼ�꼤��ɹ�
		NT_QINGYUAN_COUPLE_HALO_EQUIPED_ON,							//��װ�����޹⻷
		NT_QINGYUAN_COUPLE_HALO_EQUIPED_OFF,						//��ж�·��޹⻷

		NT_PET_QINMI_UPSTAR_SUCC,									//��ϲ�㣬�ɹ��������϶�
		NT_PET_QINMI_UPLEVEL_SUCC,									//��ϲ�㣬�ɹ��������ϵȼ�

		NT_PASTURE_SPIRIT_UPGRADE_SUCC,								//�������������ɹ�
		NT_PASTURE_SPIRIT_PROMOTE_QUALITY_SUCC,						//����������ʾƷ�ʳɹ�
		NT_PASTURE_FIRST_ENTER_GIVE_SCORE,							//��ϲ���ϵͳ������͵�500���֣�

		NT_PERSONALIZE_WINDOW_BUBBLE_RIM_SUIT_ACTIVE_SUCC,			//�׼���λ����ɹ�

		NT_HUASHEN_SPIRIT_UPGRADE_SUCC,								//��ϲ������������ɹ�

		NT_QUESTION_USE_MIANDA_CAR_SUCC,							//ʹ����𿨳ɹ�
		NT_QUESTION_USE_BIANSHEN_CAR_SUCC,							//ʹ�ñ����ɹ�

		NT_MAGICCARD_SKILL_SUCC,									//ħ������ʹ�óɹ�
		NT_MAGICCARD_CARD_USE_SUCC,									//ħ������ɹ�
		NT_MAGICCARD_CARD_EXCHANGE_SUCC,							//�һ��ɹ�

		NT_JINGLING_HALO_UPGRADE_SUCC,								//����⻷�����ɹ�

		NT_CROSS_BOSS_BUY_RELIVE_TIME_SUCC,							//����ɹ�
		NT_WING_FUMO_SUCC,											//����ħ�ɹ�

		NT_MITAMA_UPGRADE_SUCC,										//��ϲ�㣬�ɹ�����������
		NT_MITAMA_TASK_FIGHTING_SUCC,								//���̳�����
		NT_MITAMA_TASK_FETCH_REWARD_SUCC,							//�ɹ���ȡ����ս��Ʒ

		NT_FAIRY_TREE_FETCH_MONEY_SUCC,								//��ϲ����ȡ�������߽����ɹ�
		NT_FAIRY_TREE_FETCH_GIFT_SUCC,								//��ϲ����ȡ��������ɹ�
		NT_FAIRY_TREE_UPLEVEL_SUCC,									//��ϲ�����������ɹ�
		NT_FAIRY_TREE_UPGRADE_SUCC,									//��ϲ���������׳ɹ�

		NT_RA_BLACK_MARKET_OFFER_ITEM_SUCC,							//��ϲ�㣬���۳ɹ���

		NT_LIEMING_UP_LEVEL_SUCC,									//���������ɹ�

		NT_XIANNV_ACTIVEHUANHUA_SUCC,								//��Ů�û��ɹ�

		NT_ZHIBAO_HUANHUA_GARDE_UPLEVEL_SUCC,						//ʥ���������׳ɹ�
		NT_ZHIBAO_HUANHUA_JIHUO_SUCC,								//ʥ�����󼤻�ɹ�
		NT_YANHUI_START_SUCC,										//��Ὺ���ɹ�
		NT_TIANJIANGCAIBAO_GET_REWARD_ITEM,							//����콵�Ʊ���Ʒ����
		NT_TIANJIANGCAIBAO_GATHER_SUCC,								//�ɼ��ɹ�
		NT_YANHUI_BLESS_GET_BIND_GOLD,								//ף����ð���
		NT_WANGLINGEXPLORE_GATHER_SUCC,								//�ɼ��ɹ�
		NT_WANGLINGEXPLORE_REWARD_ITEM,								//��ý���
		NT_TIANJIANGCAIBAO_GET_HONOR,								//����콵�Ʊ���������
		NT_CROSS_HS_ADD_PARTNER_SUCCESS,							//�Ļ�ɹ�
		NT_CROSS_HS_GIVE_PRESENT_SUCCESS,							//��������ɹ�
		NT_CROSS_HS_CANCEL_PARTNER_SUCCESS,							//ȡ���Ļ�ɹ�

		NT_CHAT_ACTIVE_BUBBLE_SUCCESS,								//�������ݿ�ɹ�
		NT_CHAT_USE_BUBBLE_SUCCESS,									//ʹ�����ݿ�ɹ�
		NT_CHAT_UN_USE_BUBBLE_SUCCESS,								//ȡ��ʹ�����ݿ�ɹ�

		NT_ZHUANSHENG_AUTO_CONBIME_SUCC,							//��ϲ��һ���ϲ��ɹ�

		NT_LITTLE_PET_FEED_SUCC,									//С����ι���ɹ�
		NT_LITTLE_PET_INTERACT_SUCC,								//С���ﻥ���ɹ�
		NT_LITTLE_PET_EXCHANGE_SUCC,								//�һ��ɹ�
		NT_LITTLE_PET_RENAME_SUCC,									//С��������ɹ�


		NT_LOVE_TREE_RENAME_SUCC,									//��˼�������ɹ�

		NT_ZHUANSHEN_SUCC,											//��ɫת���ɹ�

		NT_MOJIE_ACTIVE_SUCC,										//�ؽ伤��ɹ�
		NT_MOJIE_UP_LEVEL_SUCC,										//�ؽ������ɹ�

		NT_TEAM_CHANGE_CAPTAIN_SUCC,								//���Ѿ���Ϊ�˶ӳ�

		NT_CHINESE_ZODIAC_UP_LEVEL_SUCC,							//��Ф�ȼ������ɹ�
		NT_CHINESE_XINGLING_UP_LEVEL_SUCC,							//����ȼ������ɹ�

		NT_USE_TUHAOJIN_SCUCC,										//ʹ��������ɹ�
		NT_NOT_USE_TUHAOJIN_SCUCC,									//�ѻָ�Ĭ��������ɫ

		NT_GOLD_VIP_ACTIVE_SUCC,									//��ϲ���ɹ�����ƽ��Ա

		NT_KICK_OUT_FROM_DABAO,										//�����ŭֵ�������߳�
		NT_BUY_LOVE_CONTRACT_SUCCESS,								//������Լ����ɹ�
		NT_SHENWU_HUANHUA_SUCCESS,									//���������Ѿ�����

		NT_EQUI_EXP_FB_GUWU_SUCC,									//����ɹ�
		NT_EQUI_EXP_FB_AUTO_GUWU_SUCC,								//�Զ�����ɹ�

		NT_SHENGE_SYSTEM_SET_SHENGE_SUCC,							//�ǻ�װ���ɹ�
		NT_SHENGE_SYSTEM_COMPOSE_SUCC,								//�ǻԺϳɳɹ�
		NT_SHENGE_SYSTEM_DECOMPOSE_SUCC,							//�ǻԷֽ�ɹ�
		NT_SHENGE_SYSTEM_SHENGE_UP_LEVEL_SUCC,						//�ǻ������ɹ�
		NT_SHENGE_SYSTEM_SHENGE_UNLOAD_SHENGE_SUCC,					//�ǻԲ�ж�ɹ�
		NT_SHENGE_SYSTEM_SHENGE_PAGE_CLEAR_SUCC,					//�ǻ�ҳ����ɹ�
		NT_SHENGE_SYSTEM_BAOJI_SUCC,								//�ǻԱ������˳ɹ�����
		NT_SHENGE_SYSTEM_ZHANGKONG_UPLEVEL_SUCC,					//���ǽ��׳ɹ�

		NT_TIAN_XIANG_ACTIVE_BEAD_SUCC,								//�ɹ���������
		NT_TIAN_XIANG_CHANGE_BEAD_TYPE_SUCC,						//�ı��������ͳɹ�
		NT_TIAN_XIANG_MOVE_BEAD_SUCC,								//�����ƶ��ɹ�

		NT_ZODIAC_MIJI_PUT_SUCC,									//�ؼ����óɹ�
		NT_ZODIAC_MIJI_CHANGE_SUCC,									//�ؼ��滻�ɹ�
		NT_ZODIAC_MIJI_COMPOUND_SUCC,								//�ؼ��ϳɳɹ�

		NT_RED_PAPER_NOTICE_SUCC,									//���ѷ�����ɹ�

		NT_YIZHANDAODI_ADD_SPEED,									//������˼���Ч��
		NT_YIZHANDAODI_DEC_SPEED,									//������˼���Ч��
		NT_YIZHANDAODI_ADD_GONGJI,									//������˹�������Ч��
		NT_YIZHANDAODI_DEC_GONGJI,									//������˹����½�Ч��
		NT_YIZHANDAODI_ADD_FANGYU,									//������˷�������Ч��
		NT_YIZHANDAODI_DEC_FANGYU,									//������˷����½�Ч��
		NT_YIZHANDAODI_WUDI,										//��������޵�Ч��
		NT_YIZHANDAODI_RECOVER,										//������˻ظ�Ч��
		NT_YIZHANDAODI_BIANSHEN,									//������˱���Ч��
		NT_YIZHANDAODI_DEADLY_ATTACK,								//�����������һ��Ч��

		NT_MARRY_UP_LEVEL_SUCC,										//���ȼ�����

		NT_RUSH_BUYING_BUY_ITEM_SUCC,								//��ϲ��������Ʒ�ɹ�
		NT_MAP_HUNT_GET_RARE_CITY,									//��ϲ�㷢�ֱ��عų�
		NT_JUHUN_PUTON_SUCC,										//����װ���ɹ�
		NT_JUHUN_TAKE_OFF_SUCC,										//����װ�����³ɹ�

		NT_SHEN_YIN_ACTIVE_BEAD_SUCC,								//�ɹ�����Դ��
		NT_SHEN_YIN_XILIAN_SUCC,									//�����ɹ�
		NT_SHEN_YIN_CHANGE_BEAD_TYPE_SUCC,							//�ı�Դ�����ͳɹ�
		NT_SHEN_YIN_MOVE_BEAD_SUCC,									//Դ���ƶ��ɹ�

		NT_FISHING_TIME_LIMIT,										//���յ������������
		NT_ROLE_LOGOUT,												//��������ߣ��޷�͵ȡ
		NT_STEAL_FISH_TIEM_LIMIT,									//�����յ�͵�����������
		NT_BE_STEAL_FISH_TIME_LIMIT,								//��ǰ��ҵı�͵�����Ѵ�����
		NT_HAVE_NO_FISH,											//��ǰ��һ�û������
		NT_STEAL_FISH_FAIL,											//��̫�������������
		NT_FISH_NOT_ENOUGH,											//�������
		NT_IS_NOT_FISHING,											//���û�ڵ���

		NT_PLANTING_TREE_PLANTING_SUCC,								//��ϲ�㣬��׽���óɹ�
		NT_PLANTING_TREE_WATERING_SUCC,								//�ٻ����óɹ�

		NT_STEAL_TIMES_LIMIT,										//����͵ȡ����������
		NT_STEAL_COIN_LIMIT,										//�����û������,�ѱ�͵ȡ10��,Ϊ��Ĭ����
		NT_STEAL_ONE_PEOPLE_COUNT_LIMIT,							//����°�,���Ѿ���͵����ʣ�޼���
		NT_HAS_NOT_ENOUGH_CRYSTAL_DALARAN,							//����Ǯ���Ѿ��տ���Ҳ��

		NT_ELEMENT_HEART_UP_SPEED_SUCC,								//����֮�Ʋ���ʱ����ٳɹ�
		NT_ELEMENT_HEART_UP_GRADE_GHOST_SUCC,						//������׳ɹ�
		NT_ELEMENT_HEART_FEED_SUCC,									//����ι���ɹ�

		NT_ELEMENT_SHOP_BUY_SUCC,									//����ɹ�
		EN_ELEMENT_SHOP_HAS_ALEADY_BUY,								//�Ѿ����������Ʒ

		NT_UPGRADE_RUSH_RETURN_REWARD,								//��ϲ����ý��׷�������
		NT_KICK_OUT_FROM_BABY_BOSS,									//�����ŭֵ�������߳�

		NT_BUILD_TOWER_BUILD_SUCC,									//����ɹ�
		NT_BUILD_TOWER_UPLEVEL_SUCC,								//�����ɹ�
		NT_BUILD_TOWER_REMOVE_SUCC,									//����ɹ�
		NT_BUILD_TOWER_CALL_SUCC,									//�ٻ��ɹ�
		
		NT_FISHING_WATCH_YOUR_FISH,									//�������ڽ����Ķ����㣬С�������Ӵ
		NT_FISHING_EVENT_GET_FISH,									//��ϲ�����һ���ɰ�����
		NT_FISHING_CONFIRM_GET_FISH,								//�ٺٺ٣�����ֶ���
		NT_FISHING_CONFIRM_GET_FISH_GIVE_SUCC,						//���͸��˶���һ��С���
		NT_FISHING_CONFIRM_GET_FISH_BE_GIVIEN,						//��Ķ���������һ��С���
		NT_FISHING_BIG_FISH_HELP_SUCC,								//���ӰѾ��������Ҫ�Ϲ���
		NT_FISHING_BIG_FISH_BE_HELPED_SUCC,							//���һ��ͬ��ݺݵذ�������һ��
		NT_FISHING_EVENT_GET_BIG_FISH_SUCC,							//̫���ˣ��Ϲ�һ������
		NT_FISHING_CONFIRM_BIG_FISH_TO_HELPER,						//��Ķ�������Э����������һ������
		NT_FISHING_STEAL_FISH_SUCC,									//��ϲ�㣬����һ������
		NT_FISHING_EXCHANGE_SUCC,									//�һ��ɹ���

		NT_WORLD_BOSS_CONCERN_SUCC,                                 //��ע���óɹ� 
		NT_RENEW_SUCC,												//���ѳɹ�

		NT_LIGHT_TIANMING_SUCC,										//���������ɹ�
		NT_LIGHT_CANGLONG_SUCC,										//���������ɹ�
		NT_LIGHT_SHENGXIAO_SUCC,									//������Ф�ɹ�
		NT_LIGHT_YUANQI_SUCC,									    //��������ɹ�
		NT_LIGHT_SHENQI_SUCC,									    //����ʥ���ɹ�
		NT_LIGHT_XINGXIU_SUCC,                                      //�������޳ɹ�

		NT_TICKET_NO_ENOUGH,                                        //��Ʊ��������

		NT_SHENGQI_STUFF_DECOMPOSE_SUCC,							//�ֽ�ɹ�

		NT_BABY_SPIRIT_WEN_UPGRADE_SUCC,                            //����ֵ����
		NT_BABY_SPIRIT_WU_UPGRADE_SUCC,                             //����ֵ����

	    NT_RED_EQUIP_COLLECT_SUCC,									//��װ�ռ��ɹ���
		NT_ORANGE_EQUIP_COLLECT_SUCC,								//��װ�ռ��ɹ���

		NT_EQUIP_BAPTIZE_OPEN_SUCC,									//�����ɹ�

		NT_SPOUSE_HOME_LOVER_ROOM_BUY_SUCC,                         //�ɹ�Ϊ���¹�����һ�䷿
		NT_SPOUSE_HOME_LOVER_ROOM_EQUIP_SUCC,                       //�ɹ�Ϊ����װ�緿��
		NT_SPOUSE_HOME_LOVER_ROOM_EQUIP_FAIL,                       //��λ�����б仯

		NT_GREATESOLDIER_EXCHANGE_SUCC,								//�һ��ɹ�
		NT_GREATESOLDIER_PUTON_SUCC,								//��ħװ���ɹ�
		NT_GREATESOLDIER_TAKE_OFF_SUCC,								//��ħװ�����³ɹ�

		NT_GUILD_BOX_OPEN,											//���ı���������������ͨ���ʼ�����ʽ����

		NT_GUILD_BOX_UPLEVEL_SUCC,									//��¯�����ɹ�
		NT_CROSS_LIEKUN_FB_GATHER_SUCC,								//�ɼ��ɹ�

		NT_SHENYIN_EQUIP_SUCC,										//����װ���ɹ�
		NT_UPLEVEL_RETURN_RWARD_MAIL,								//�н��׷���δ�죬��ͨ���ʼ�����
	};


	enum reply_type
	{
		NT_REPLY_TYPE_SHENGE = 1,//���ϵͳ
	}; //REPLY_TYPE
}

#endif

