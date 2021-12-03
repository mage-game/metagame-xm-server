#ifndef __ERRORNUM_H__
#define __ERRORNUM_H__

namespace errornum
{

	enum ERROR_NUM_DEF
	{
		//////////////////////////////////////  �����������    ////////////////////////////////////
		EN_KNAPSACK_USE_NOT_DIRECT = 100,				//����Ʒ����ֱ��ʹ��
		EN_KNAPSACK_IS_LOCK,							//�����Ѿ���ס
		EN_KNAPSACK_USE_ITEM_ERROR,						//ʹ����Ʒ����
		EN_KNAPSACK_USE_ITEM_NUM_NOT_ENOUGH,			//ʹ����Ʒ��������
		EN_KNAPSACK_LOAD_ERROR,							//��ȡ��Ʒ����
		EN_KNAPSACK_CANNOT_DISCARD,						//��Ʒ���ܶ���
		EN_KNAPSACK_MOVE_ERROR,							//��Ʒ�ƶ�����
		EN_KNAPSACK_SPLIT_CANNOT,						//��Ʒ���ܲ��
		EN_KNAPSACK_SPLIT_NUM_ERROR,	                //��Ʒ�����������
		EN_KNAPSACK_PUT_ERROR,							//��Ʒ�������
		EN_KNAPSACK_NO_SAPCE,							//�����ռ䲻��
		EN_KNAPSACK_EQUIP_TAKEOFF_ERROR,				//װ�����´���
		EN_KNAPSACK_USE_XUANYUN_STATE,					//ѣ��״̬������ʹ����Ʒ
		EN_KNAPSACK_STORAGE_EXTEND_MAX,					//�����ֿ���չ�Ѵ����ֵ
		EN_ITEM_PROF_LIMIT,								//ְҵ������
		EN_ITEM_LEVEL_LIMIT,							//��ǰ�ȼ�����
		EN_ITEM_CANNOT_MULTI_USE,						//����Ʒ��������ʹ��
		EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE,				//�ֿ�ռ䲻��
		EN_ITEM_COLDDOWN,								//��Ʒ��ȴ��
		EN_KNAPSACK_USE_EVIEL_DECCARD,					//��ǰ���ֵΪ0������ʹ��
		EN_ITEM_USE_DAY_TEIMS_LIMIT,					//����ʹ�ô���������
		EN_KNAPSACK_USE_SCENE_LIMIT,					//��ǰ����������ʹ�ø���Ʒ
		EN_ITEM_SEX_LIMIT,								//�Ա𲻷���
		EN_KNAPSACK_CANNOT_DISCARD_INLAY,				//����Ƕ��ʯ���ܶ���
		EN_KNAPSACK_USE_DEAD,							//�������޷�ʹ����Ʒ
		EN_SUBLOCK_IS_LOCKED,							//������δ�⿪
		EN_SUBLOCK_ERROR_TIMES_LIMIT,					//�ѵ��������ÿ���������������
		EN_SUBLOCK_PASSWORD_ILLEGAL,					//����������Ƿ�
		EN_ITEM_TIME_NO_IN_STORAGE,						//ʱЧ����Ʒ���ɷ���ֿ�
		EN_STOP_FLY_AREA_LIMIT,							//������������ֹͣ����
		EN_KNAPSACK_STORAGE_NO_SPACE,					//�����ֿ�ռ䲻��

		//////////////////////////////////////  �������    ////////////////////////////////////
		EN_FALLING_NOT_YOURS = 200,						//����Ʒ��������
		EN_FALLING_TOO_FAR,								//ʰȡ��Ʒ�ľ����Զ
		EN_FALLING_ROLE_DEAD,							//����״̬������ʰȡ��Ʒ

		//////////////////////////////////////  ���� && npc ���    ////////////////////////////////////
		EN_TASK_MAX_NUM = 400,							//�ѽ�����ﵽ�������
		EN_TOO_FAR_FROM_NPC,							//����npc̫Զ
		EN_TASK_ACCEPT_PRE_TASK,						//���������ǰ������
		EN_TASK_ACCEPT_LEVEL,							//�ȼ����㣬��Ŭ��������
		EN_TASK_ACCEPT_MAX_LEVEL,						//�ȼ������޷����ܸ�����
		EN_TASK_ACCEPT_FLY_MODE_LIMIT,					//����״̬���޷��Ӵ�����
		EN_TASK_ACCEPT_JINGHUA_LIMIT,					//���ھ��������в����ٽӴ�����
		EN_TUMO_TASK_ACCEPT_NO_COUNT,					//�����ճ������ȡ��������
		EN_TUMO_TASK_NO_TASK,							//��û�п��ύ���ճ�����
		EN_TUMO_TASK_NO_COMPLETE,						//�ճ�����δ���
		EN_HUSONG_TASK_COLOR_TYPE_SATISFY,				//�����������ɫ�Ѿ�����Ҫ��
		EN_HUSONG_TASK_COLOR_IS_RED,					//���������Ѿ��Ǻ�ɫ
		EN_HUSONG_TASK_ACCEPT_NO_COUNT,					//�Ѿ����ȫ����������
		EN_HUSONG_TASK_ACCEPT_FLY_LIMIT,				//����ʱ���ܽӻ�������
		EN_HUSONG_TASK_VIP_BUY_LIMIT,					//�㵱ǰVIP�ȼ����ɹ���Ĵ����Ѵ����ޣ�
		EN_HUSONG_TASK_REFRESH_COLOR_FAIL,				//ˢ��ʧ�ܣ�Ʒ�ʲ���
		EN_HUSONG_TASK_SCENE_KEY_LIMIT,					//��������ֻ����1�߽�ȡ
		EN_GUILD_TERRITORY_EXTRA_LIMIT,					//�����ǻ᳤��������ȡ���⽱��
		EN_GUILD_TERRITORY_REWARD_LIMIT,				//������ȡ���ý���
		EN_GUILD_TERRITORY_REWARD_DAY_BANGGONG_LIMIT,	//ÿ�հﹱ����
		EN_TUMO_PRETASK_UNCOMPLETE,						//�ճ�����ǰ������δ���
		EN_TUMO_ALREADY_HAS_TASK,						//���Ѿ���ȥ���ճ�����
		EN_HUSONG_TASK_CAN_BUY_LIMIT,					//������⻤�ʹ������ɻ�÷����
		EN_GUILD_JOIN_GUILD_LIMIT,						//��������������˵�����
		EN_PAOHUAN_TASK_ACCEPT_NO_COUNT,				//�����ܻ������ȡ��������
		EN_PAOHUAN_ALREADY_HAS_TASK,					//�������Ѿ����ܻ�����
		EN_PAOHUAN_TASK_NO_TASK,						//��û�п��ύ���ܻ�����
		EN_TASK_APPERAN_CHANGED_GATHER,					//������ɲɼ�����
		EN_TASK_APPERAN_CHANGED_IMAGE,					//������ɱ�������
		EN_TASK_APPERAN_CHANGED_FLY,					//������ɷ�������

		//////////////////////////////////////  ��Ʒ/װ������ ���    ////////////////////////////////////
		EN_FUNC_ITEM_LOAD = 500,						//��Ʒ��ȡ����
		EN_FUNC_STUFF_NOT_ENOUGH,						//������Ʒ����

		EN_ITEM_FUNC_ITEM_COMPOSE_CANNOT,				//��Ʒ���ɺϳ�
		EN_ITEM_FUNC_ITEM_COMPOSE_NOT_ENOUGH,			//������ϲ���

		EN_EQUIP_FUNC_CONFIG_ERROR,						//װ�����ô���
		EN_EQUIP_FUNC_INLAY_HOLE_NOT_EMPTY,				//�ÿ��Ѿ���Ƕ��ʯ��
		EN_EQUIP_FUNC_INLAY_EQUIP_LEVEL,				//װ���ȼ�����
		EN_EQUIP_FUNC_QUALITY_NOT_ENOUGH,				//װ�����������ȼ�
		EN_EQUIP_FUNC_INLAY_HAVE_STONE_IN_SLOT,			//�ò���װ����ʯ
		EN_EQUIP_FUNC_CANNOT_STRENGTHEN,				//��װ������ǿ��
		EN_EQUIP_FUNC_STRENGTHEN_REACH_LEVELBOUND,		//�Ѿ�ǿ������ߵȼ���
		EN_ITEM_FUNC_ITEM_UPLEVEL_NOT_ENOUGH,			//װ������������ϲ���
		EN_EQUIP_FUNC_CANNOT_UPLEVEL,					//��װ����������
		EN_ITEM_FUNC_ITEM_INHERIT_NEED_SAME_TYPE,		//װ��������Ҫͬ�����͵�װ��
		EN_EQUIP_FUNC_CANNOT_SHENZHU,					//װ���޷�����
		EN_EQUIP_NOT_EXIST,								//װ��������
		EN_EQUIP_CANNOT_FULING,							//װ���޷�����
		EN_EFFECT_NOT_ACTIVE_TO_SAVE,					//��Чδ�������ʧ��
		EN_STRENGTH_STUFF_NOT_ENOUGH,					//ǿ��ʯ����
		EN_EQUIP_FUNC_CANNOT_UPSTAR,					//װ���޷�����
		EN_EQUIP_SUIT_CANNOT_UP_TO_SUIT,				//��װ����������Ϊ��װװ��
		EN_EQUIP_SUIT_UPTO_MAX_SUIT_LEVEL,				//��װ���Ѿ����������������װ
		EN_EQUIP_BAY_STONE_FAIL,						//����ʧ�ܣ����౦ʯ�������̵깺��
		EN_EQUIP_FUNC_ETERNITY_EQUIP_LEVEL_LIMIT,		//װ���ȼ�����
		EN_EQUIP_FUNC_ETERNITY_LEVEL_MAX,				//�Ѵﵽ���ȼ�
		EN_EQUIP_FUNC_CANNOT_UPQUALITY,					//װ���޷�����Ʒ��
		EN_EQUIP_FUNC_LEVELUP_IS_MAX_LEVEL,				//�Ѿ�������ߵȼ�
		EN_EQUIP_FUNC_LEVELUP_ODDS_FAILD,				//��������ʧ��
		EN_EQUIP_FUNC_LEVELUP_SUCC,						//�����ɹ�


		//////////////////////////////////////  ���ѡ������� ���    ////////////////////////////////////
		EN_FRIEND_NUM_MAX = 600,						//���������Ѵﵽ����
		EN_FRIEND_EXIST,								//�Է��������ĺ���
		EN_ROLE_NOT_ONLINE,								//�Է�������
		EN_FRIEND_OTHER_REJECT_REQ,						//�Է��ܾ������ĺ�������
		EN_FRIEND_OTHER_FULL,							//�Է���������
		EN_FRIEND_BLESS_NOT_FRIEND,						//�Է��������ĺ���
		EN_FRIEND_BLESS_THANK_BLESS_TIMES_ZERO,			//�������Ϊ�㣬���ܼ���������
		EN_FRIEND_REMOVE_LOVER_LIMIT,					//���ܺͷ��޽�����ѹ�ϵ
		EN_ADD_BLACK_LOVER_LIMIT,						//���ܽ����˼��������
		EN_IN_YOUR_BLACKLISTS,							//�Է������ĺ�������
		EN_BLACKLISTS_NUM_MAX,							//�������Ѵ�����
		EN_YOU_ARE_IN_BLACKLISTS,						//���ڶԷ��ĺ�������
		EN_MAIL_FRIEND_LIMIT,							//ֻ�ܷ��ʼ�������

		//////////////////////////////////////   �ʼ�ϵͳ ���    ////////////////////////////////////
		EN_HAS_NO_SPACING = 650,						//�����ռ䲻��
		EN_ITEM_IS_NOT_EXIST,							//������Ʒ������

		//////////////////////////////////////   �������� ���    ////////////////////////////////////
		EN_PUBLICSALE_ADD_ITEM_FAIL = 700,				//������Ʒ����
		EN_PUBLICSALE_ADD_ITEM_FULL,					//���۵���Ʒ�����Ѵ�����
		EN_PUBLICSALE_REMOVE_ITEM_NOT_EXIST,			//����Ʒ���۳�
		EN_PUBLICSALE_NOT_HAVE_ITEM,					//û�г��۵���Ʒ
		EN_PUBLICSALE_NOT_ENOUGH_ITEM,					//���۵���Ʒ��������
		EN_PUBLICSALE_BUY_ITEM_NOT_EXIST,				//��Ʒ������
		EN_PUBLICSALE_ITEM_CANNOT_SALE,					//��Ʒ���ܳ���
		EN_PUBLICSALE_EQUIPMENT_CANNOT_SALE,			//װ�����ܳ���
		EN_PUBLICSALE_SYSTEM_ERROR,						//����ϵͳ��æ�����Ժ�����
		EN_PUBLICSALE_ROLE_LEVEL_LIMIT,					//��ɫ�ȼ�40���ſɽ��г���

		//////////////////////////////////////   �������� ���    ////////////////////////////////////
		EN_FB_REST_MONSTER_NUM_LIMIT = 800,				//��������ʣ�����
		EN_FB_RANDOM_TRANSMIT_FAIL,						//�����������ʧ��
		EN_FB_IN_CD,									//����������ȴ��
		EN_SCENE_TRANSPORT_LEVEL_LIMIT,					//���ĵȼ����㣬���ܽ���ó���
		EN_SCENE_TRANSPORT_WEARY_LIMIT,					//ƣ��ֵ����
		EN_SCENE_TRANSPORT_ANGRY_LIMIT,					//��ŭֵ����
		EN_FB_ENTER_HOSONG_LIMIT,						//����ִ�л�������
		EN_FB_ENTER_FB_ERROR,							//���븱������
		EN_SCENE_TRANSPORT_TOO_FAR,						//���봫�͵�̫Զ
		EN_GATHER_TOO_FAR,								//�ɼ���̫Զ
		EN_STATUS_CAN_NOT_GATHER,						//��ǰ״̬���ܲɼ�
		EN_SCENE_FORBID_PK,								//�ó���������ս��
		EN_SAVEAREA_FIGHT,								//��ȫ���ڲ��ܹ���
		EN_SAVEAREA_SET_LANDMINE,						//��ȫ���ڲ��ܷ��õ���
		EN_SAVEAREA_FIGHT_TARGET,						//���ܹ�����ȫ���ڵ����
		EN_FB_DAY_COUNT_LIMIT,							//���ս�������Ѿ�����
		EN_TRANSMIT_LIMIT,								//��������
		EN_FB_NOT_TEAM_LEADER,							//�����ɶӳ�������븱��
		EN_FB_TEAM_MEMBER_TOO_FEW,						//������������
		EN_FB_TEAM_MEMBER_TOO_FAR,						//�ж�Ա���ڸ��� ���ܽ�����Ӹ���
		EN_FB_TEAM_LEVEL_REQUIRE,						//�������г�Ա�ȼ�����ﵽҪ��
		EN_FB_TEAMMATE_CANOT_TRAINSMIT,					//�����д��ڶ�Ա���ڲ��ܴ���״̬
		EN_FB_TEAM_DAY_COUNT_LIMIT,						//�������г�Ա���벻��������ÿ�մ�������
		EN_FB_TEAMMATE_CANOT_ENTER_FB,					//�����д��ڶ�Ա���ڲ��ܽ��븱��״̬
		EN_FB_NOT_IN_TEAM,								//û����Ӳ��ܽ�����Ӹ���
		EN_FB_PERSONAL_DENY_TEAM,						//���˸���������ӽ���
		EN_CANNOT_INVITE_HAS_PARTNER_ROLE,				//�Է�����˫�ޣ��޷�������
		EN_REST_OTHER_NOT_ONLINE,						//�Է��Ѿ�������
		EN_REST_OTHER_CAN_NOT_REST,						//�Է�����˫��
		EN_REST_ALREADY_HAS_PARNTER,					//�Է��Ѿ��ҵ�˫����Ů��
		EN_ROLE_ALREADY_HAS_REST_PARNTER,				//���Ѿ���˫�޵���Ů��
		EN_ROLE_SHUANGXIU_LEVEL_NOT_ENOUGH,				//�ȼ����� ����˫��
		EN_ROLE_OTHER_SHUANGXIU_LEVEL_NOT_ENOUGH,		//�Է��ȼ����� ����˫��
		EN_NPC_FUNC_TRASMIT_RED_NAME,					//����״̬���ܴ���
		EN_CLIENT_POS_NOT_MATCH,						//�ͻ��˽�ɫλ�ô���ƫ��
		EN_FB_ENTER_CAN_NOT_IN_FB,						//�����в��ܽ�����������
		EN_SPECIAL_SCENE_OPERATE_LIMIT,					//�������ⳡ���У�����ִ�д˲���
		EN_ENTER_ACTIVITY_NOT_START,					//���δ��ʼ
		EN_ENTER_ACTIVITY_FB_LEVEL_LIMIT,				//�ȼ�����,���ܽ�������
		EN_QUESTION_ACTIVITY_QUESTION_DAY_COUNT_LIMIT,	//���մ����Ѿ���� 
		EN_QUESTION_CAN_NOT_USE_BIANSHEN_CARD,			//����Ʒֻ���ڴ��⸱��ʹ��
		EN_QUESTION_HAS_ANSER,							//���Ѿ��ش����
		EN_QUESTION_USE_DATI_CARD_LIMIT,				//׼���׶β���ʹ�����
		EN_NOT_QUESTION_RANK,							//��δ������
		EN_QUESTION_TARGE_NOT_IN_CUR_SCENE,				//Ŀ�겻�ڵ�ǰ����
		EN_QUESTION_OTHER_USE_BIANSHEN_CARD_ON_YOU,		//������Ҷ���ʹ���˱���
		EN_FETCH_ZHUXIE_TASK_REWARD_FAIL,				//��ȡ������ʧ��
		EN_FETCH_TIANJIANGCAIBAO_REWARD_FAIL,			//�콵�Ʊ�������ʧ��
		EN_TIANJIANGCAIBAO_ENTER_LEVEL_LIMIT,			//�ȼ����㲻�ܽ���
		EN_ZHUXIE_TASK_UNFINISH,						//��а����δ���
		EN_ZHUXIE_NO_SUCH_TASK,							//��û�������а����
		EN_NONEXIST_TO_BUY,								//���������Ʒ������
		EN_FB_YOU_CANOT_ENTER_FB,						//�����ڲ��ܽ��븱��״̬	
		EN_FB_NEQ_NOT_MAX_STAR,							//û�дﵽ����
		EN_FB_NEQ_BUY_TIMES_NOT_VIP,					//vip���ܹ������
		EN_FB_NEQ_BUY_TIMES_UP,							//�㵱ǰVIP�ȼ����ɹ���Ĵ����Ѵ����ޣ�
		EN_CANNOT_CHG_PEACE_MODE,						//��ǰ���������л���ƽģʽ
		EN_TOWER_DEFEND_NEXT_WAVE_REQ_NOT_TEAM_LEADER,	//ֻ�жӳ�����������һ��
		EN_TOWER_DEFEND_CURR_WAVE_NOT_DONE,				//ˢ��̫��Ƶ�������Ժ�����
		EN_TOWER_DEFEND_ONLY_LEADER_CHOOSE_TYPE,		//ֻ�жӳ�������������
		EN_TOWER_DEFEND_MEMBER_TOO_FAR_TO_CHOOSE_TYPE,	//���Ѳ��ڸ�����������������
		EN_TOWER_DEFEND_CHOOSE_TYPE_DUP,				//����������ͬ����
		EN_TOWER_DEFEND_ATTRTYPE_ERR,					//�ӳ����ʹ���
		EN_TOWER_DEFEND_BUILDING_ERR,					//�������ʹ���
		EN_TOWER_DEFEND_BUILDING_PRELEVEL_REQUIRED,		//��Ҫ�Ƚ���͵ȼ�����
		EN_TOWER_DEFEND_BUILDING_SCORE_LESS,			//����Ҫ�Ļ��ֲ���
		EN_TOWER_DEFEND_BUILDING_CLEARWAVE_LESS,		//�ۼ�����Ĳ�������
		EN_TOWER_DEFEND_BUILDING_KILL_SUPERBOSS_LESS,	//��ɱ�ĳ���boss��������
		EN_FB_PRE_LEVEL_NOT_FINISH,						//ͨ����ħ֮���������ս��ħ֮����
		EN_ACTIVITY_NOT_ENROLLED,						//���ȱ����ٲμӻ
		EN_LEVEL_LESS_THAN_50_TO_JOIN,					//���ĵȼ�����50���������Ŭ��
		EN_TOO_FAR_FROM_SHENSHI,						//������ʯ̫Զ
		EN_FB_DAY_COUNT_NOT_ENOUGH,						//���ո���ʣ���������
		EN_XIANMENGZHAN_NO_GUILD,						//����û�м�������
		EN_XIANMENGZHAN_ROLE_LEVEL_LIMIT,				//��ɫ�ȼ�����
		EN_XIANMENGZHAN_GUILD_LEVEL_LIMIT,				//���˵ȼ�����
		EN_XIANMENGZHAN_GUILD_SIGNUP_COUNT_LIMIT,		//�����˱�������������
		EN_GUILD_RELIVE_ALL_NOT_TIMES,					//�����ܸ����������
		EN_GUILD_RELIVE_SELF_NOT_TIMES,					//���˸��˸����������
		EN_XIANMENGZHAN_NOT_PRIVILEGE,					//û���ٻ�Ȩ��
		EN_XIANMENGZHAN_CALL_COUNT_LIMIT,				//����ٻ�����������
		EN_XIANMENGZHAN_CALL_CD_LIMIT,					//�ٻ�CD��ȴ��
		EN_XIANMENGZHAN_LEADER_NOT_EXIST,				//�������뿪
		EN_TRANSMIT_LIMIT_IN_DUOQUZHENBAO,				//��ȡ��ʯ�в�������
		EN_XIANMENGZHAN_DANYAO_BOSS_EXIST,				//�ɵ������Ѿ��ٻ�������
		EN_XIANMENGZHAN_XIANLI_NOT_ENOUGH,				//��������100���ύ��ҩ���Ի������
		EN_XIANMENGZHAN_NO_DEFEND_AREA,					//��Ҫռ�����оݵ������ٻ�����
		EN_NATIONALBOSS_BUY_TIMES_UP,					//�����������Ѵ����ֵ
		EN_TRANSMIT_LIMIT_IN_MARRY,						//�����в�������
		EN_TOWER_DEFEND_FROZEN_IN_CD,					//��������CDʱ����
		EN_TOWER_DEFEND_FROZEN_COUNT_LIMIT,				//������ʹ�ô����ѵ�����
		EN_YAOSHOUGUANGCHANG_JOIN_LIMIT,				//���ղ������޹㳡�����Ѵ�����
		EN_BUY_YAOSHOUGUANGCHANG_TIME_LIMIT,			//�㵱ǰVIP�ȼ����ɹ���Ĵ����Ѵ����ޣ�
		EN_YAOSHOUGUANGCHANG_NOT_STANDBY,				//���޹㳡���ǰ����׼��״̬����������룡
		EN_SUOYAOTA_JOIN_LIMIT,							//���ղ��������������Ѵ�����
		EN_BUY_SUOYAOTA_TIME_LIMIT,						//�㵱ǰVIP�ȼ����ɹ���Ĵ����Ѵ����ޣ�
		EN_BUY_EXP_FB_TIME_LIMIT,						//�Ѵﵽ���չ����������
		EN_BUY_EXP_FB_NO_TIME,							//��û�����ô���
		EN_BUY_STROY_FB_TIME_LIMIT,						//�㵱ǰVIP�ȼ����ɹ���Ĵ����Ѵ����ޣ�
		EN_BUY_PAHSE_FB_TIME_LIMIT,						//�㵱ǰVIP�ȼ����ɹ���Ĵ����Ѵ����ޣ�
		EN_SUOYAOTA_NOT_STANDBY,						//���������ǰ����׼��״̬����������룡
		EN_SUOYAOTA_THIS_TIME_ALREADY_JOIN,				//���Ѿ������˴����������
		EN_YAOSHOUGUANGCHANG_THIS_TIME_ALREADY_JOIN,	//���Ѿ������˴����޹㳡���
		EN_MARRY_BLESS_DAYS_NOT_ENOUGH,					//����ף����������
		EN_MARRY_BLESS_ALREADY_FETCH,					//�����Ѿ���ȡ����
		EN_MARRY_BLESS_DAYS_EQUAL,						//˫������ף��������Ȳ��ܹ���
		EN_GOLD_VIP_AUTH_LIMIT,							//�������ǻƽ��Ա
		EN_CHARGE4_REWARD_LIMIT,                        //����������500�����ѷ���
		EN_CONSUM_GIFT_ROLL_NO_TIMES,                   //û��ҡ������
		EN_CONSUM_GIFT_ROOL_INSUFFICIENT_NUMBER,		//ҡ����������

		//////////////////////////////////////   ��ɫ ���    ////////////////////////////////////
		EN_ROLE_NOT_EXIST = 1000,						//��ɫ������
		EN_ROLE_NOT_IN_CACHE,							//��ɫ������Ŷ
		EN_ROLE_HAS_CHOOSE_PROF,						//���Ѿ���ְҵ��
		EN_ROLE_XINSHOU_PROTECT,						//���ֱ��� �������𹥻�
		EN_ROLE_PROF_NOT_SUITABLE,						//ְҵ������
		EN_VIP_LEVEL_NOT_ENOUGH,						//VIP�ȼ�����
		EN_STATUS_CAN_NOT_REST,							//���ڲ����������Ϣ��״̬
		EN_ROLE_REST_LEVEL_NOT_ENOUGH,					//�����ȼ�������
		EN_CAN_NOT_GATHER_IN_FLY,						//����ʱ���ܲɼ�
		EN_ROLE_NAME_REPEAT,							//��ɫ���ѱ�ʹ��
		EN_ROLE_IS_MARRY_CAN_NOT_CHANGE_SEX,			//���ѽ�飬���ܸı��Ա�
		EN_ROLE_REST_FORBID_IN_FIGHT,					//ս��״̬���ܴ���
		EN_ROLE_FIGHT_TARGET_NOT_ENEMY,					//���������ǵ���
		EN_ROLE_FIGHT_CHENMO_STATUS,					//��������Ĭ��
		EN_ROLE_FIGHT_DINGSHEN_STATUS,					//������������
		EN_ROLE_FIGHT_DISTANCE_NOT_ENOUGH,				//�������벻��
		EN_ROLE_FIGHT_CD_NOT_ENOUGH,					//��ȴ��
		EN_ROLE_FIGHT_GLOBALCD_NOT_ENOUGH,				//ȫ����ȴ��
		EN_ROLE_LEVEL_NOT_ENOUGH,						//��ɫ�ȼ�����
		EN_ROLE_TRANSMIT_FORBID_IN_FIGHT,				//ս��״̬���ܴ���
		EN_ROLE_NUQI_NOT_ENOUGH,						//ŭ������
		EN_ROLE_ZHANYI_NOT_ENOUGH,						//ս�ⲻ��
		EN_ROLE_HUNZHI_NOT_ENOUGH,						//��ֵ����
		EN_SPEAKER_LEVEL_NOT_ENOUGH,					//��ɫ200��֮�������ȹ���
		EN_FIGHT_BIANXING_FOOL,							//���������ˣ����ܹ���
		EN_LOVER_SCENE_CANNOT_TRANSMIT,					//�������ڳ������ܴ���
		EN_QUESTION_LEVEL_LIMIT,						//���⹦��δ����
		EN_QUESTION_HAS_ANSWER_LIMIT,					//���Ѿ��ش������Ŀ��
		EN_QUESTION_TIME_LIMIT,							//�ش�ʱ��δ��������������
		EN_ROLE_STATE_CANNT_RECOVER,					//��ǰ���ڽ���״̬
		EN_TATGET_ROLE_MASSAGE_CD_NOT_ENOUGH,			//��������ڰ�Ħ��
		EN_ROLE_MASSAGE_CD_NOT_ENOUGH,					//���ڰ�Ħ��
		EN_ROLE_FLY_PROTECT,							//������ �������𹥻�

		///////////////////////////////////  ���˽���   //////////////////////////////////////////////////
		EN_ITEM_CANNOT_TRADE = 1100,					//����Ʒ���ܽ���
		EN_ITEM_NUM_NOT_ENOUGH,							//��Ʒ��������

		///////////////////////////////////  ħ��   //////////////////////////////////////////////////
		EN_MOJIE_LEVEL_LIMIT = 1150,					//��ħ��ȼ�����
		EN_MOJIE_ROLE_LEVEL_LIMIT,						//��ɫ�ȼ�����
		EN_MOJIE_UP_LEVEL_STUFF_NOT_ENOUGH,				//�������ϲ���

		//////////////////////////////////////   ���� ���    ////////////////////////////////////
		EN_ALREADY_IN_TEAM = 1200,						//�����ж���
		EN_TEAM_IS_FULL,								//���������Ѵ�����
		EN_USER_IS_NOT_EXIST,							//�û�������
		EN_USER_IS_NOT_IS_YOUR_SCENE,					//�������ߺ�������ͬ������
		EN_HE_IS_IN_TEAM,								//�Է��Ѿ��ж�����
		EN_HE_IS_REJUCT,								//�Է��ܾ���������
		EN_TEAM_LEADER_IS_REJUCT,						//����ܾ�������
		EN_TEAM_IS_NOT_EXIST,							//���鲻����
		EN_YOU_ARE_NOT_IN_TEAM,							//�����ڶ�����
		EN_YOU_HAVE_NO_POWER,							//��û��Ȩ�޽��иò���
		EN_HE_IS_NOT_IN_YOUR_TEAM,						//�Է��������Ķ�����
		EN_USER_IS_NOT_ONLINE,							//�Է�������
		EN_TEAM_JOIN_LIMIT,								//����������δ��꣬�ٽ������ɣ�
		EN_TEAM_CALL_TOGETHER_LIMIT_IN_FB,				//�����в����ټ���Ա
		EN_TEAM_CALL_IN_LIMIT_IN_FB,					//�����в�����ļ��Ա
		EN_TEAM_SPECIAL_TEAM_JOIN_LIMIT,				//�ö����Ѿ���ʼ������
		EN_TEAM_TEAM_FB_OPERA_LIMIT,					//��ǰ�����²�����ӣ����˻��߽�ɢ����
		EN_TEAM_JOIN_OPERA_LIMIT_IN_TEAM_MATCHING,		//��������ƥ���У����ɼ���˶���
		EN_TEAM_JOIN_OPERA_LIMIT_IN_ROLE_MATCHING,		//����ƥ���У����ɼ������
		EN_USER_IS_NOT_IN_SAME_SERVER,					//�Է����㲻��ͬ��������
		EN_USER_IS_NOT_IN_SAME_SERVER_SCENE,			//�����ֻ����ͬ��������������
		EN_TEAM_TOO_MANY_MEMBERS,						//�����������࣬�޷����
		EN_TEAM_LEADER_NOT_FOUND,						//�ӳ�������
		EN_HE_IS_IN_SELF_TEAM,							//�Է��Ѿ������Ķ�����
		EN_YOU_ARE_IN_HIS_TEAM,							//���Ѿ��ڶԷ��Ķ�����
		EN_TEAM_JOIN_LIMIT_OF_MERGE,					//�������г�Ա������������������ܺϲ�����
		EN_ROLE_IS_IN_THIS_TEAM,						//���ڸö�����
		EN_YOU_HAVE_NO_POWER_MERGE,						//��û��Ȩ�޽��ж���ϲ�

		//////////////////////////////////////   ���� ���    ////////////////////////////////////
		EN_GUILD_SYSTEM_ERROR = 1300,					//����ϵͳ����
		EN_GUILD_CREATE_NO_ITEM,						//û�н�����
		EN_GUILD_NO_ATUO_JOIN_GUILD,					//û�м�����ϵ�����
		EN_GUILD_ATUO_JOIN_TIME_LIMIT,					//����Ƶ�����룬���Ժ��ٳ���
		EN_GUILD_YOU_ALREADY_IN_GUILD,					//���Ѿ���������
		EN_GUILD_NOT_ENOUGH_LEVEL,						//�ȼ����㣬��Ŭ��������
		EN_GUILD_INVILD_NAME,							//���������Ѵ���
		EN_GUILD_YOU_NOT_IN_GUILD,						//������������
		EN_GUILD_YOU_NOT_HAVE_POWER,					//��ûȨ�����д˲���
		EN_GUILD_NO_EXIST,								//���˲�����
		EN_GUILD_REPEAT_APPLY_FOR,						//���Ѿ��������
		EN_GUILD_APPLY_FOR_FULL,						//�����˵������б�����
		EN_GUILD_NOT_IN_SAME_CAMP,						//û����ͬһʦ����
		EN_GUILD_ROLE_COUNT_LIMIT,						//������������
		EN_GUILD_OTHER_ALREADY_IN_GUILD,				//�Է��Ѽ�������
		EN_GUILD_APPLY_FOR_NOT_EXIST,					//������ʧЧ
		EN_GUILD_CANNOT_QUIT,							//�����˳�����
		EN_GUILD_INVITE_INVALID,						//������ʧЧ
		EN_GUILD_OTHER_NOT_IN_YOUR_GUILD,				//�Է�������������
		EN_GUILD_CANNOT_LEAVE_POST,						//������ж��
		EN_GUILD_POST_IS_SAME,							//��ѡ��ְͬλ��������
		EN_GUILD_POST_FULL,								//ְλ��������
		EN_GUILD_NOT_JOIN_GUILD,						//���ȼ���һ������
		EN_GUILD_DELATE_FAIL,							//�������ڣ����ɺ���������ʧ��
		EN_GUILD_DELATE_NO_ITEM,						//û�е�����
		EN_GUILD_IS_REJUCT,								//���˾ܾ�������
		EN_GUILD_STORAGE_LOCKED,						//���˲ֿ�������
		EN_GUILD_STORAGE_NOT_HAVE_ITEM,					//û�з������˲ֿ����Ʒ
		EN_GUILD_STORAGE_PUTON_ITEM_NOT_ENOUGH,			//�������˲ֿ����Ʒ����
		EN_GUILD_STORAGE_ITEM_CANNOT_SALE,				//����Ʒ���ܷ������˲ֿ�
		EN_GUILD_STORAGE_PUT_ERROR,						//������Ʒ����
		EN_GUILD_STORAGE_TAKE_ERROR,					//ȡ����Ʒ����
		EN_GUILD_STORAGE_FULL,							//���˲ֿ�����
		EN_GUILD_STORAGE_NO_ITEM,						//���˲ֿ����û��Ʒ
		EN_GUILD_STORAGE_ITEM_BE_TAKEN,					//����Ʒ�ѱ�ȡ��
		EN_GUILD_STORAGE_PUTOFF_ITEM_NOT_ENOUGH,		//ȡ�����˲ֿ����Ʒ����
		EN_GUILD_STORAGE_DISCARD_NOPOWER,				//��û��Ȩ�����ٲֿ���Ʒ
		EN_GUILD_STORAGE_ITEM_NOT_PUT,					//���˲ֿⲻ�ܷ������Ʒ
		EN_GUILD_STORAGE_TAKE_SCORE_NOT_ENOUGH,			//�ֿ���ֲ��㣬�޷�ȡ����Ʒ
		EN_GUILD_YAOJIANG_DAYLIMIT,						//��������ҡ������
		EN_GUILD_ALREADY_IN_GUILD_STATION,				//�Ѿ�������פ��
		EN_GUILD_CAN_NOT_INVITE_FOR_CAMP_NOT_SAME,		//��ͬʦ�Ų��ɱ�����
		EN_GUILD_CAN_NOT_INVITE_TARGET_LEVEL_LIMIT,		//�Է��ȼ�����
		EN_GUILD_CAN_NOT_INVITE_FOR_MEMBER_FULL,		//������������ ��������
		EN_GUILD_YOU_NOT_JOIN_GUILD,					//��δ��������
		EN_GUILD_RESET_NAME_ERROR,						//�����������޸�
		EN_GUILD_FORBIT_APPLYFOR,						//�����˽�ֹ����
		EN_GUILD_APPLYFOR_SETUP_ERROR,					//�������ñ���ʧ��
		EN_GUILD_JIANZHULEVEL_MUST_LESSTHAN_GUILDLEVEL, //�����ȼ��������������ȼ�
		EN_GUILD_NO_FULI_GIFT,							//��ǰû�п���ȡ�ĸ���
		EN_GUILD_BLESSING_CD_LIMIT,						//���һ���ٶԶԷ�����ף��
		EN_GUILD_NO_BLESSING_COUNT,						//���Ѿ�û��ף��������
		EN_GUILD_TARGET_NO_BLESSING_COUNT,				//�Է�û��ף��������

		EN_GUILD_GCZ_LIMIT,								//����֮ս�ڼ��ֹ�˲���
		EN_GUILD_XMZ_LIMIT,								//����ս�ڼ��ֹ�˲���
		EN_GUILD_TW_LIMIT,								//����ս�ڼ��ֹ�˲���
		EN_GUILD_BATTLE_LIMIT,							//���������ڼ��ֹ�˲���
		EN_GUILD_BOSS_LIMIT,							//����Boss�ڼ��ֹ�˲���
		EN_GUILD_LIEKUN_ZHUMO_LIMIT,					//����֮ս����ħ�ڼ��ֹ�˲���
		EN_CROSS_GUILD_BATTLE_LIMIT,					//��������ڼ��ֹ�˲���						
		EN_CROSS_LIEKUN_FB_GATHER_TOO_MUCH,				//����������Ѿ��ɼ�����

		EN_GUILD_GUILD_BOSS_CALL_TOWER_DEFEND_LIMIT,	//�����ػ��в����ٻ�����
		EN_GUILD_TOWER_DEFEND_SCENE_LIMIT,				//ֻ��������פ�ؿ���
		EN_GUILD_TOWER_DEFEND_GUILD_BOSS_LIMIT,			//�ٻ����޹����в��ܿ��������ػ�
		EN_GUILD_TOWER_DEFEND_DAYCOUNT_LIMIT,			//���������ػ��ѿ���
		EN_GUILD_TOWER_DEFEND_GUILD_LEVEL_LIMIT,		//����Ҫ�ﵽ3�����ܿ��������ػ�
		EN_GUILD_TOWER_DEFEND_MONSTER_NOT_FINISH,		//��ǰ���ι�����������ǰ
		EN_GUILD_UNION_BACK_STATION_FB,					//�����в��ܽ���פ��
		EN_GUILD_OPERATOR_TOO_FAST,						//��������Ƶ��
		EN_GUILD_GET_REWARD_FAILED,						//��ȡ����ʧ��
		EN_GUILD_ALREADY_IN_GUILD_FB,					//�Ѿ��ڸ�����
		EN_GUILD_ALREADY_JOIN_GUILD_FB,					//������Ѿ��������������
		EN_GUILD_FB_OUT_OF_TIME,						//��δ�������ʱ��
		EN_GUILD_UPLELVEL_TO_MAX,						//�Ѵﵽ��ߵȼ����޷�����
		EN_GUILD_UPLELVEL_NOT_ENOUGH_EXP,				//���˲Ƹ����㣬�޷�����
		EN_GUILD_UPLELVEL_NOT_ENOUGH_STUFF,				//�����������ϲ��㣬�޷�����
		EN_GUILD_UPLELVEL_NEED_UPLEVEL_OTHER,			//��Ҫ��������������
		EN_GUILD_UPLELVEL_NEED_UPLEVEL_MAIN_HALL,		//��Ҫ��������������
		EN_GUILD_GET_BUFF_NOT_EXISTS,					//��ȡ������buff������
		EN_GUILD_GET_BUFF_NOT_ENOUGH_GONG_XIAN,			//����ս�����㣬�޷���ȡ
		EN_GUILD_EXCHANGE_NOT_ENOUGH_GONG_XIAN,			//����ս�����㣬�޷��һ�
		EN_GUILD_LEVEL_LIMIT,							//���˵ȼ�����
		EN_GUILD_OVER_EXCHANGE_TIMES,					//�������һ��������������ٶһ�
		EN_GUILD_FETCH_REWARD_BEFORE,					//������ȡ���ý������޷��ٴ���ȡ
		EN_GUILD_BOSS_NOT_BEKILLED,						//����bossδ����ɱ��������ȡ
		EN_GUILD_NOT_KILL_PERE_BOSS,					//���Ȼ�ɱ��һ��boss
		EN_GUILD_FB_LV_LIMIT,							//�������˵ȼ����㣬�޷�������������
		EN_GUILD_FB_OPEN_EXP_LIMIT,						//���������ʽ��㣬�޷�������������
		EN_GUILD_BONFIRE_OPEN_EXP_LIMIT,				//���������ʽ��㣬�޷�������Ůף���
		EN_GUILD_BONFIRE_MOVE_MODE_LIMIT,				//�����в��ܿ�����Ůף���
		EN_GUILD_CALL_IN_LEVEL_LIMIT,					//�ȼ�����199�޷�������ļ
		EN_GUILD_BOX_NOT_START,							//��δ��ʼ����
		EN_GUILD_BOX_UPLEVEL_FAIL,						//��¯����ʧ��
		EN_GUILD_BOX_UPLEVEL_VIP_COUNT_LIMIT,			//VIP�ȼ����㣬���������ý�¯
		EN_GUILD_BOX_UPLEVEL_TIME_LIMIT,				//�뿪����24Сʱ�ڲ���������¯
		EN_GUILD_BOX_OPENING,							//������
		EN_GUILD_BOX_REWARDED,							//��¯�ѿ���
		EN_GUILD_BOX_PRE_BOX_OPEN_LIMIT,				//��һ����¯��û����
		EN_GUILD_BOX_ASSIST_COUNT_LIMIT,				//��¯Э�������Ѵ�������
		EN_GUILD_BOX_OPEN_VIP_COUNT_LIMIT,				//VIP�ȼ����㣬��������
		EN_GUILD_BOX_OPEN_TIME_LIMIT,					//�뿪����24Сʱ�ڲ�������
		EN_GUILD_BOX_ASSIST_TIME_LIMIT,					//����Э��������ȴ��
		EN_GUILD_BOX_HAVE_ASSIST_LIMIT,					//�ɹ�Э���ý�¯����
		EN_GUILD_BOX_BE_ASSIST_TIMES_LIMIT,				//�ý�¯Э�������Ѵ�����
		EN_GUILD_BOX_CAN_OPEN,							//��¯�Ѿ���ʱ����Դ�
		EN_GUILD_BOX_ASSIST_NOT_HAVE,					//û�н�¯��Э��
		EN_GUILD_BOSS_UPLEVEL_MAX,						//����BOSS��������߼�
		EN_GUILD_BOSS_NORMAL_CALL_COUNT_LIMIT,			//��ͨBOSS�ٻ��Ѵ�������
		EN_GUILD_BOSS_SUPER_CALL_LIMIT,					//�������ٻ���ͨBOSS�����ٻ�����BOSS
		EN_GUILD_BOSS_SUPER_CALL_COUNT_LIMIT,			//����BOSS�ٻ��Ѵ�������
		EN_GUILD_REWARD_TIME_LIMIT,						//�뿪����24Сʱ�ڲ�����ȡ����
		EN_GUILD_REWARD_FETCHED,						//��������ý���
		EN_GUILD_BOSS_ACTIVITY_NOT_OPEN,				//����BOSSʱ��δ��
		EN_GUILD_BOSS_NOT_IN_STATION,					//�ٻ�����BOSS��Ҫ������פ��
		EN_GUILD_BOSS_CALL_AUTORITY_LIMIT,				//����Ȩ�޲������ٻ�BOSS
		EN_GUILD_BOSS_BE_BEING,							//��ͨBOSS��ɱ�󣬲����ٻ�����BOSS
		EN_GUILD_BOSS_CALLED,							//���ٻ�������BOSS������ι��
		EN_GUILD_UNION_BACK_STATION_BOSS,				//BOSS��ͼ�в��ܽ���פ��
		EN_GUILD_EXTEND_MEMBER_MAX,						//���˳�Ա������չ�Ѵ����ֵ
		EN_GUILD_RED_PAPER_FETCH_LIMIT,					//����������ܳ���������������
		EN_GUILD_RED_PAPER_CREATE_TIME_LIMIT,			//����ѹ��ڣ������ɷ�
		EN_COMMAND_SPEAKER_RED_PAPER_TIME_LIMIT,		//����ѹ��ڣ���ȡʧ��

		EN_WELF_SIGNIN_DAYLIMIT,						//ǩ���ﵽÿ���������
		EN_WELF_FILL_SIGNIN_LIMIT,						//û�пɲ�ǩ����
		EN_GUILD_TOTAY_IS_SIGNIN,						//������ǩ��
		EN_GUILD_CREATE_CD_TIME,						//����������Ҫ���5����

		/////////////////////////////////////	 Ԫ��	///////////////////////////////////////////////
		EN_MENTALITY_PROTECT_ITEM_NOT_EXIST = 1500,		//���Ǳ���������
		EN_MENTALITY_UPGRADE_ITEM_NOT_ENOUGH,			//���ǵ�����
		EN_MENTALITY_UPGRADE_GENGU_FAIL,				//����ʧ��
		EN_MENTALITY_MENTALITY_LEVEL_ZERO,				//������Ӧ������Ϊ0��������������
		EN_MENTYALITY_LEVEL_LIMIT,						//��߽�ɫ�ȼ����ܼ�����������
		EN_MENTALITY_NO_NEED_CLEAR_CD,					//����������CD
		EN_MENTALITY_WUXING_LEVEL_LIMIT,				//���еȼ��Ѿ��ﵽ���ֵ
		EN_MENTALITY_WUXING_JINGMAI_LIMIT,				//�����ܵȼ�����
		EN_MENTALITY_NOT_NEED_PROTECT_ITEM,				//����Ҫ������

		/////////////////////////////////////	 ����	///////////////////////////////////////////////
		EN_MOUNT_NOT_EXIST = 1550,						//����δ����
		EN_MOUNT_CANNOT_UPGRADE,						//�����Ѵ�����
		EN_MOUNT_UP_ONE_GRADE_LIMIT,					//����׼�������
		EN_MOUNT_CANNOT_MOUNTON,						//�����ڲ���������
		EN_MOUNT_CANNOT_GOON_IN_FLY,					//���ڷ����в�����������
		EN_MOUNT_IMG_NOT_ACTIVE,						//����δ����
		EN_MOUNT_TEMPORARY_IMAGE_TIME_LIMIT,			//�������������ѵ��ڣ�ͨ��������׿ɻ�ø�������
		EN_MOUNT_PROF_NOT_ENOUGH,						//δ�ﵽתְҪ��

		/////////////////////////////////////	 ս��	///////////////////////////////////////////////
		EN_FIGHT_MOUNT_NOT_EXIST = 1570,				//ս��δ����
		EN_FIGHT_MOUNT_CANNOT_UPGRADE,					//ս���Ѵ�����
		EN_FIGHT_MOUNT_UP_ONE_GRADE_LIMIT,				//ս��׼�������
		EN_FIGHT_MOUNT_CANNOT_GOON_IN_FLY,				//���ڷ����в�������ս��
		EN_FIGHT_MOUNT_IMG_NOT_ACTIVE,					//����δ����
		EN_FIGHT_MOUNT_PROF_NOT_ENOUGH,					//δ�ﵽתְҪ��

		/////////////////////////////////////	 ����	///////////////////////////////////////////////
		EN_WING_NOT_EXIST = 1590,						//����δ����
		EN_WING_CANNOT_UPGRADE,							//�����Ѵ�����
		EN_WING_UP_ONE_GRADE_LIMIT,						//����׼�������
		EN_WING_IMG_NOT_ACTIVE,							//����δ����
		EN_WING_TEMPORARY_IMAGE_TIME_LIMIT,				//�������������ѵ��ڣ�ͨ��������׿ɻ�ø�������
		EN_WING_PROF_NOT_ENOUGH,						//δ�ﵽתְҪ��

		/////////////////////////////////////	 �⻷	///////////////////////////////////////////////
		EN_HALO_NOT_EXIST = 1600,						//�⻷δ����
		EN_HALO_CANNOT_UPGRADE,							//�⻷�Ѵ�����
		EN_HALO_UP_ONE_GRADE_LIMIT,						//�⻷�׼�������
		EN_HALO_IMG_NOT_ACTIVE,							//����δ����
		EN_HALO_SPECIAL_IMG_GRADE_LIMIT,				//����⻷����������

		/////////////////////////////////////	 ��Ů�⻷	///////////////////////////////////////////////
		EN_SHENGONG_NOT_EXIST = 1610,					//��Ů�⻷δ����
		EN_SHENGONG_CANNOT_UPGRADE,						//��Ů�⻷�Ѵ�����
		EN_SHENGONG_UP_ONE_GRADE_LIMIT,					//��Ů�⻷�׼�������
		EN_SHENGONG_IMG_NOT_ACTIVE,						//����δ����
		EN_SHENGONG_SPECIAL_IMG_GRADE_LIMIT,			//������Ů�⻷����������

		/////////////////////////////////////	 ��Ů����	///////////////////////////////////////////////
		EN_SHENYI_NOT_EXIST = 1620,						//��Ů����δ����
		EN_SHENYI_CANNOT_UPGRADE,						//��Ů�����Ѵ�����
		EN_SHENYI_UP_ONE_GRADE_LIMIT,					//��Ů����׼�������
		EN_SHENYI_IMG_NOT_ACTIVE,						//����δ����
		EN_SHENYI_SPECIAL_IMG_GRADE_LIMIT,				//������Ů��������������

		/////////////////////////////////////	 �㼣	///////////////////////////////////////////////
		EN_FOOTPRINT_NOT_EXIST = 1630,					//�㼣δ����
		EN_FOOTPRINT_CANNOT_UPGRADE,					//�㼣�Ѵ�����
		EN_FOOTPRINT_UP_ONE_GRADE_LIMIT,				//�㼣�׼�������
		EN_FOOTPRINT_IMG_NOT_ACTIVE,					//����δ����
		EN_FOOTPRINT_SPECIAL_IMG_GRADE_LIMIT,			//�����㼣����������

		/////////////////////////////////////	 ����	///////////////////////////////////////////////
		EN_CLOAK_NOT_EXIST = 1640,						//����δ����
		EN_CLOAK_IMG_NOT_ACTIVE,						//����δ����
		EN_CLOAK_SPECIAL_IMG_GRADE_LIMIT,				//������������������
		
		/////////////////////////////////////	����	///////////////////////////////////////////////
		EN_JINGLING_NOT_EMPTY_GRID = 1730,				//�����������������ȡ����������
		EN_JINGLING_SAME_TYPE,							//����װ����ͬ���͵ĳ���
		EN_JINGLING_MAX_LEVEL,							//�Ѵﵽ��ߵȼ����޷�����
		EN_LINGJING_NOT_ENOUGH,							//�龧���㣬�޷�����
		EN_JINGLING_CARD_MAX_LEVEL,						//�Ѵﵽ���ȼ�
		EN_JINGLING_ALREADY_ACTIVE,						//�Ѿ����������
		EN_JINGLING_WUXING_LEVEL_LIMIT,					//���������Ѵ����ȼ�
		EN_JINGLING_XIANZHEN_LEVEL_LIMIT,				//���������Ѵ����ȼ�
		EN_JINGLING_HUNYU_LEVEL_LIMIT,					//��������Ѵ����ȼ�
		EN_JINGLING_SKILL_PRE,							//����ѧϰǰ�ü���
		EN_JINGLING_SKILL_STORAGE_FULL,					//���＼�ܱ�������
		EN_JINGLING_SKILL_GRID_NOT_OPEN,				//�ü��ܲ�δ����
		EN_JINGLING_WUXING_UP_FAIL,						//��������ʧ��
		EN_JINGLING_SKILL_TYPE_LIMIT,					//�ó�����ӵ��ͬ�༼��
		EN_JINGLING_HOME_ROB_TIMES_NOT_ENOUGH,			//�����԰�Ӷ��������
		EN_JINGLING_HOME_ROB_REPEAT_TOO_MORE,			//������������̫����ˣ�����������
		EN_JINGLING_HOME_GET_TIME_LIMIT,				//��ȡʱ��δ��
		EN_JINGLING_EXPLORE_TIMES_NOT_ENOUGH,			//��ս��������
		EN_JINGLING_EXPLORE_NOT_PASS_LEVEL,				//����ͨ�ظùؿ�
		EN_JINGLING_ADVANTAGE_BOSS_LIMIT,				//��������BOSS����������������ɱ�������ý���

		/////////////////////////////////////	����	///////////////////////////////////////////////
		EN_SHENGWANG_XIANJIE_MAX_LEVEL = 1750,			//�ɽ��Ѵﵽ��ߵȼ����޷�����
		EN_SHENGWANG_XIANDAN_MAX_LEVEL,					//�ɵ��Ѵﵽ��ߵȼ����޷�����
		EN_SHENGWANG_XIANJIE_LIMIT,						//�ɽ׵ȼ����㣬�޷�����

		/////////////////////////////////////	 �ɾ�	///////////////////////////////////////////////
		EN_CHENGJIU_NOT_OPENED = 1800,					//�ɾ�δ����
		EN_CHENGJIU_ALREADY_REWARD,						//�ý�������ȡ
		EN_CHENGJIU_NOT_COMPLETE,						//�ɾ�δ���
		EN_CHENGJIU_TITLE_MAX_LEVEL,					//�ƺ��Ѵﵽ��ߵȼ����޷�����
		EN_CHENGJIU_FUWEN_MAX_LEVEL,					//ս���Ѵﵽ��ߵȼ����޷�����
		EN_CHENGJIU_TITLE_LIMIT,						//�ɾͳƺŵȼ����㣬�޷�����
		EN_CHENGJIU_CHENGJIU_NOT_ENOUGH,				//�ɾͲ��㣬�޷�����

		/////////////////////////////////////	 ����	///////////////////////////////////////////////
		EN_OTHER_CANNOT_EVALUATE_SELF = 1900,			//���������Լ�
		EN_OTHER_NO_EVALUATE_NUM,						//���������۴���������
		EN_OTHER_ROLE_EVALUATEED,						//�����ѵ���
		EN_TITLE_NO_EXIST,								//�óƺ�δ����
		EN_NOT_TANSMIT_XUANYUN,							//ѣ��״̬���ɴ���
		EN_NOT_TANSMIT_DINGSHEN,						//����״̬���ɴ���
		EN_NOT_FIGHT_IN_FLY,							//����ʱ���ܷ��𹥻�
		EN_NOT_TRANSMIT_FLY_MODE,						//����״̬���ɴ���
		EN_NOT_FLY_BY_SHOE_SCENE_LIMIT,					//�ó����޷�����
		EN_NOT_FLY_BY_SHOE_IN_FB,						//��ǰ��ͼ���ܴ���
		EN_FORBID_TALK,									//���ѱ�����
		EN_TITLE_ALREADY_HAVE,							//���Ѿ�ӵ��������ƺ�
		EN_TITLE_ALREADY_FULL,							//�ƺ���������
		EN_GIVE_FLOWER_CAN_NOT_SELF,					//�����ͻ����Լ�
		EN_TARGET_NOT_ONLINE,							//�Է�������
		EN_FLOWER_ITEM_TYPE_ERROR,						//�ʻ����ʹ���
		EN_BUY_FLOWER_ERROR,							//�����ʻ�ʧ��
		EN_ATTACK_MODE_NOT_ALLOW_IN,					//��ǰ����ģʽ���ܽ���
		EN_HANGUP_SCENE_VIP_LIMIT,						//ֻ��VIP�û����ܽ���һ���ͼ
		EN_HANGUP_SCENE_LEVEL_LIMIT,					//����û�ﵽ����ȼ�
		EN_CAN_NOT_ENTER_HANGUP_IN_FB_SCENE,			//�ڸ����ڲ��ܽ���һ���ͼ
		EN_OTHER_EVALUATE_LEVEL_LIMIT,					//50�����Ͽ��������
		EN_HAS_FETCH_THIS_REWARD,						//�Ѿ���ȡ���ý���
		EN_LOGIN_DAY_COUNT_DISSATISFY,					//��¼����������Ҫ��
		EN_REWARD_NOT_SATISFY,							//��û�����ȡ����
		EN_MOUNT_BIANSHEN_TIME_LIMIT,					//�����ѵ��ڣ��������������ſ���������
		EN_SPECIAL_IMG_ALREADY_ACTIVE,					//���������Ѿ�����
		EN_SHENBING_ALREADY_ACTIVE,						//����Ѿ�����
		EN_SHENBING_STUFF_ERR,							//���������ϴ���
		EN_SHENBING_NOT_ACTIVE_TO_OP,					//��Ҫ�ȶ����һ���������ܸ���
		EN_NO_YAOJIANG_ITEM,							//��İ������̲��㣬��ͨ��������ã�
		EN_ACTIVITY_FIND_REWARED,						//�������ѻ�ù��û�Ľ���
		EN_NEQ_PRELEVEL_NOT_PASS,						//����ͨ��ǰһ��
		EN_NEQ_TIMES_LIMIT,								//������ս�����Ѿ�����
		EN_ACTIVITY_HAS_ENTER,							//�������Ѿ�������û�����������콱
		EN_ACTIVITY_ACTIVITY_NOT_START,					//���δ����������ί��
		EN_ENERGY_IS_FULL,								//����ǰ��������
		EN_BUY_ENERGY_TIMES_LIMIT,						//�������������ﵽ����
		EN_NEQ_LEVEL_TIMES_LIMIT,						//��Ұ��ԭÿ���������սһ��
		EN_MOUNT_UPLEVEL_EQUIP_LEVEL_LIMIT,				//�ȼ�����-����
		EN_EQUIP_LEVEL_MAX,								//�Ѵﵽ���ȼ�
		EN_WING_UPLEVEL_EQUIP_LEVEL_LIMIT,				//�ȼ�����-����
		EN_BOSS_NOT_IN_BOSS_FAMILY,						//��ǰboss����boss֮�ҵ�
		EN_GATHAR_SELF_BONFIRE_LIMIT,					//һ��ֻ�����Լ����˵���Ů��5�Σ�
		EN_GATHAR_BONFIRE_LIMIT,						//һ���������Ů��10�Σ�
		EN_GATHAR_GUILD_LIMIT,							//�������˺󣬿ɵõ���Ůף����
		EN_GATHAR_ADD_MUCAI_LIMIT,						//��ľ�Ĵ����Ѵ�����
		EN_EXPBOTTLE_TRANSFORM_LIMIT,					//��������۾���ƿ�Ѵ����ޣ�
		EN_EXPBOTTLE_USE_LIMIT,							//�����ʹ�þ���ƿ�Ѵ����ޣ�
		EN_EXPBOTTLE_EXP_LIMIT,							//���۾���ƿ����ľ��鲻�㣡
		EN_BOSS_DONGKU_WEARY_LIMIT,						//ƣ��ֵΪ0���޷���boss����˺�
		EN_NOT_FLY_BY_SHOE_BOSS_FAMILY_LIMIT,			//boss֮�Ҳ�����ֱ�Ӵ��ͣ�
		EN_NOT_FLY_BY_SHOE_DABAO_LIMIT,					//�򱦵�ͼ������ֱ�Ӵ��ͣ�
		EN_NOT_FLY_BY_SHOE_ACTIVE_LIMIT,				//��Ծboss��ͼ������ֱ�Ӵ��ͣ�
		EN_NOT_FLY_BY_SHOE_BOSS_DONGKU_LIMIT,			//�ؿ�boss��ͼ������ֱ�Ӵ��ͣ�
		EN_NOT_FLY_BY_SHOE_CAMPDUOBAO_LIMIT,			//ʦ�Ŷᱦ��ͼ������ֱ�Ӵ��ͣ�
		EN_ADD_MUCAI_SUCC,								//�ػ���Ů�Ѿ��յ�������⣬������һ��С���
		EN_NOT_FAMILY_BOSS_BUY_WEARY_TIME_LIMIT,		//ƣ��ֵ��������Ѵ�����
		EN_ADD_MUCAI_NOT_YOUR_GUILD,					//�����ܶ��������˵��������ľ�ģ�
		EN_EQUIPMENT_FUMO_FAIL,							//�봩�϶�Ӧװ���ٽ��и�ħ
		EN_MILLIONAIRE_GATHER_LIMIT,					//�ɼ������Ѵ�����
		EN_MILLIONAIRE_GATHER_LEVEL_LIMIT,				//�ȼ����㲻�ܲɼ�����
		EN_MILLIONAIRE_SKILL_TIME_LIMIT,				//����ʹ�ô�������
		EN_TITLE_HAS_ACTIVE,							//�óƺ��Ѽ���
		EN_TITLE_GRADE_LIMIT,							//�ƺŽ����Ѵ����ȼ�
		EN_CAN_NOT_ENTER_BOSS,							//�ó�������ֱ�ӽ���boss��ͼ
		EN_MAGICAL_PRECIOUS_FETCHT_REWARD_LIMIT,		//�Ѿ���ȡ���ý���
		EN_MAGICAL_PRECIOUS_CAN_NOT_FETCHT_REWARD,		//δ�ﵽ��ȡ����
		EN_MAGICAL_PRECIOUS_PRE_CHAPTER_NOT_FINISH,		//δ�����һ�½��������񣬲�����ȡ
		EN_SHANGGU_BOSS_ENTER_TIMES_FULL,               //���ս������������
		EN_BOSS_SHANGGU_WEARY_LIMIT,					//ƣ��ֵ���ߣ��޷�����˺�
		EN_EXPENSE_NICE_GIFT_DAILY_LIMIT,	            //�Ѵ���ճ齱������
		EN_NOT_FIGHT_IN_JUMP,							//����ʱ���ܷ��𹥻�
		EN_NOT_FIGHT_IN_JUMP2,							//����ʱ���ܷ��𹥻�

		/////////////////////////////////////	 �̵� ���	///////////////////////////////////////////////
		EN_SHOP_HAS_BOUGHT_LIMIT_ITEM = 2000,			//���ڱ����޹��Ѿ����������Ʒ 
		EN_SHOP_ITEM_NOT_EXIST,							//�̵��в����ڸ���Ʒ
		NT_CONVERTSHOP_CONVERT_COUNT_LIMIT,				//���նһ������Ѵ�����
		NT_CONVERTSHOP_LIFETIME_CONVERT_COUNT_LIMIT,	//�һ������Ѵ�����
		NT_CONVERTSHOP_LEVEL_LIMIT,						//�ȼ����������ܶһ�
		EN_CONVERTSHOP_LINGZHI_NOT_ENOUGH,				//��ֵ����
		EN_CONVERTSHOP_LIMIT_TIME_INVALID,				//����Ʒ�ѹ��ڣ����ɶһ�
		NT_MYSTERIOUS_SHOP_BUY_LIMIT,					//��������Ѵ�����
		EN_MYSTERIOUS_SHOP_CREDIT_NOT_ENOUGH,			//�����̳ǻ��ֲ���
		EN_PRECIOUS_BOSS_SCORE_NOT_ENOUGH,				//�ر����ֲ���
		EN_BUY_TOO_MUCH_ONCE,                           //�������������
		EN_SHENZHOUWENPON_SCORE_NOT_ENOUGH,				//�����ֲ���

		/////////////////////////////////////	 �	///////////////////////////////////////////////
		EN_LINGMAI_LINGMAI_DAYCOUNT_LIMIT = 2100,		//����ʹ�ô�������
		EN_LINGMAI_NOT_IN_WANGCHENG,					//ֻ���ڶ�ʢ��������ʹ���������������Ʒ
		EN_LINGZHU_ITEM_NOT_ENOUGH,						//û�е�������
		EN_ZHADAN_ITEM_NOT_ENOUGH,						//û�б�����
		EN_SOULINGFU_ITEM_NOT_ENOUGH,					//��������㣬��ͨ����Ծ�Ƚ������
		EN_LINGMAI_NOTHING_FOUND,						//����û��ร�����ĵط����������ɡ�
		EN_QUNXIANLUANDOU_CONTROLLING_SHENSHI,			//���Ѿ�ӵ����ʯ
		EN_GONGCHENGZHAN_IN_CARRIER,					//���Ѿ���ľ������
		EN_GONGCHENGZHAN_TOWER_TIME_LIMIT,				//���һ���ٽ�������֮��
		EN_GONGCHENGZHAN_TOWER_SIDE,					//���Ƿ�����ʹ������֮��
		EN_GONGCHENGZHAN_CARRIER_STAGE,					//���������׶β����ٻ�ľ����
		EN_GONGCHENGZHAN_CARRIER_TOO_FAR,				//����ľ����̫Զ
		EN_GONGCHENGZHAN_CHANGE_CAMP,					//����ʹ������ʦ��ľ����
		EN_GONGCHENGZHAN_REWARD_ERROR,					//��������ȡ�ý���
		EN_GONGCHENGZHAN_REWARD_COUNT_LIMIT,			//�������Ѿ�����˸ý���
		EN_GONGCHENGZHAN_ITEM_BUY_MULTI,				//����е�������ι���
		EN_GONGCHENGZHAN_ITEM_BUY_LIMIT,				//����ʣ�๺���������
		EN_GONGCHENGZHAN_NOT_EMPTYSLOT_INDEX,			//���ܲ�����
		EN_BATTLE_FIELD_KILL_USER_INVALID,				//��ʱ���ڻ�ɱ��ͬ�����޽���
		EN_LINGMAI_PLANT_OUT_OF_RANGE,					//���ڷ��������ڲ��ɷ���
		EN_ENTER_NOT_GUILD,								//û�м�������
		EN_GONGCHENGZHAN_TOWER_TOO_FAR,					//���������̫Զ
		EN_1VN_ALREADY_SIGN,							//���Ѿ�������
		EN_XIANMENGZHAN_INVALID_GUILD,					//û�м������˲��ܲμ�����ս
		EN_RA_SERVER_NIUEGG_TIMES_LIMIT,				//ȫ��Ť������δ�ﵽҪ��
 		EN_RA_SERVER_NIUEGG_HAS_FATCHED,				//�Ѿ���ȡ���ý���
 		EN_RA_SERVER_NIUEGG_VIP_LIMIT,					//���VIP�ȼ����㣬�޷���ȡ�ý���
 		EN_RA_SERVER_NIUEGG_LEFT_CHARGE_LIMIT,			//���ʣ���ܳ�ֵ��Ȳ���
		EN_RA_ZHENBAOGE_VIP_LIMIT,						//����VIP�ȼ����㣬�޷���ȡ�ý���
		EN_RA_ZHENBAOGE_TIMES_NOT_ENOUGH,				//��ǰ����������ȡ�����
		EN_RA_ZHENBAOGE_HAVE_FETCH,						//�ý����Ѿ���ȡ����
		EN_RA_ZHENBAOGE_HAVE_ALL_BUY,					//�Ѿ�ȫ��������
		EN_RA_SERVER_MONEYTREE_TIMES_LIMIT,				//��ǰ�齱����δ�ﵽҪ��
		EN_RA_SERVER_MONEYTREE_HAS_FATCHED,				//�Ѿ���ȡ���ý���
		EN_RA_SERVER_MONEYTREE_VIP_LIMIT,				//����VIP�ȼ����㣬�޷���ȡ�ý���
		EN_RA_TIAN_MING_DIVINATION_MAX_ADD_LOT,			//��ռ��ƿ��ע����
		NT_WELFARE_HAPPY_TREE_VALUE_TOO_LARGE,			//���մ˷�ʽ��óɳ�ֵ����
		NT_WELFARE_GROWTH_VAL_NOT_ENOUGTH,				//���ֹ�������ֵ����
		NT_WELFARE_ALREADY_FETCH_HAPPYTREE_REWARD,		//�Ѿ���ȡ��
		EN_ENTER_GUILD_NOT_HAV_QUALIFICATION,			//û�л������ս�ʸ�
		EN_GUILD_BATTLE_HAS_USE_SHILD,					//���Ѿ�ʹ�ù�����
		EN_TERRITORYWAR_BUY_CAR_LIMIT,					//��ǰ�ѹ����ؾ�
		EN_TERRITORYWAR_BUY_FIRE_LANDMINE_LIMIT,		//���ѵ���Я�������Ѵ�����
		EN_TERRITORYWAR_BUY_ICE_LANDMINE_LIMIT,			//��˪����Я�������Ѵ�����
		EN_RA_TIAN_MING_DIVINATION_ALL_MAX_ADD_LOT,		//ռ��ƿ��ע����������
		EN_ACTIVITY_ENTER_TIME_OUT,                     //�����ʱ���ѹ�
		EN_RA_LIMIT_TIME_REBATE_CHONGZHI_DAY_LIMIT,		//��ֵ��������

		/////////////////////////////////////	 ����	///////////////////////////////////////////////
		EN_WELFARE_ACCLOGIN_DAYS_NOT_ENOUGH = 2300,		//�ۼƵ�½��������
		EN_WELFARE_CHONGJIHAOLI_NOT_ENOUGH,				//�����Ѿ�����ȡ���

		////////////////////////////////////  ��ȷ��� //////////////////////////////////////////////
		EN_DAILYBUYCOIN_LEVEL_LIMIT = 2400,				//��ɫ�ȼ����㲻���в�
		EN_DAILYBUYCOIN_LEFT_COUNT_NOT_ENOUGH,			//ʣ���вƴ�������

		////////////////////////////////////    ����    //////////////////////////////////////////////
		EN_RESERVE_2600 = 2600,							//����2600

		/////////////////////////////////////	 ���	///////////////////////////////////////////////	
		EN_MARRY_THIS_TIME_IS_ALREADY_RESERVE = 2700,	//��ʱ���Ѿ���ԤԼ
		EN_MARRY_HUNYAN_NOT_BE_INVITED,					//��]��ϲ������������������ȡϲ����
		EN_MARRY_HUNYAN_CAN_NOT_OPERA_TIANMI_WUSHUANG,	//���İ��²��ڸ���
		EN_MARRY_HUNYAN_ALREADY_BLESS,					//ף�������Ѿ�����
		EN_MARRY_NO_IN_TEAM,							//��ǰ״̬δ���
		EN_MARRY_TEAM_MEMBER_MUNBER_ERROR,				//��ǰ�����������Ϊ2��
		EN_MARRY_TARGET_MUST_IN_TEAM,					//��Ա������ȷ
		EN_MARRY_HUNYAN_GATHER_THIS_FOOD,				//���Ѿ��Թ�����ʳ����

		/////////////////////////////////////	 ����	///////////////////////////////////////////////	
		EN_TASK_NOT_COMPLETED = 2720,					//���븱����������δ���
		EN_PRVE_FB_NOT_COMPLETED,						//���������һ������
		EN_FB_DAYTIMES_NOTENOUGH,						//���븱������������
		EN_FB_DAILYTIMES_BUY_TIMES_MAX,					//���չ������������
		EN_FB_ATUO_NOT_PASS_FB,							//��Ҫͨ���˸������ܽ���ɨ��
		EN_FB_ROLE_LEVEL_LIMIT,							//�ȼ����㣬��Ŭ��������
		EN_FB_AUTO_PASS_TIME_LIMIT,						//ͨ��ʱ��δ�ﵽɨ��Ҫ��
		EN_FB_EXP_NOT_REWARD_TO_FETCH,					//û�п���ȡ�Ľ���
		EN_FB_EXP_NOT_SWEEPING_CHECKPOINT,				//��ǰ���޿�ɨ���ؿ�
		EN_FB_OPERATE_LIMIT,							//�����в���ִ�д˲���
		EN_FB_ATUO_NOT_IN_STATIC_SCENE,					//��ǰ�������ܽ��и���ɨ��

		/////////////////////////////////////    ��Ф   ///////////////////////////////////////////////
		EN_CHINESE_ZODIAC_LEVEL_LIMIT,					//����Ф�ȼ�����
		EN_CHINESE_ZODIAC_LEVEL_ZERO,					//��Ф�ȼ����㣬��������װ��
		EN_CHINESE_ZODIAC_XINGHUN_LEVEL_LIMIT,			//����Ф�ǻ�ȼ�����
		EN_CHINESE_ZODIAC_XINGHUN_LEVEL_UPGRADE_FAIL,	//��Ф�ǻ�ȼ�����ʧ��
		EN_CHINESE_ZODIAC_EQUIP_MAX_LEVEL,				//��Фװ���Ѵ���߼�
		EN_CHINESE_ZODIAC_UP_LEVEL_LIMIT,				//��һ����Ф�ȼ����㣬��������
		EN_CHINESE_ZODIAC_MIJI_PUT_FAIL,				//ԭ���ؼ��ȼ��ϴ󣬷���ʧ��
		EN_CHINESE_ZODIAC_LIMIT_LEVEL_LIMIT,			//����Фδ������ܴ���װ��
		EN_CHINESE_ZODIAC_MIJI_COMBINE_LEVEL_NOT_SAME,	//�ؼ��ĵȼ���һ��
		EN_CHINESE_ZODIAC_MIJI_NOT_SAME_TYPE,			//�����͵��ؼ��Ѿ�����
		EN_CHINESE_ZODIAC_ITEM_NOT_MIJI,				//��Ʒ�����ؼ�
		EN_CHINESE_LAST_ZODIAC_MIJI_NUM_LIMIT,			//��һ����Ф�ؼ���������
		EN_CHINESE_XINGLING_LEVEL_LIMIT,				//�����Ѿ�����ߵȼ�
		EN_CHINESE_XINGLING_CHAPTER_LIMIT,				//��Ӧ����ͼ�½�û�п���
		EN_CHINESE_XINGLING_UPLEVEL_FAIL,               //��������ʧ�ܣ�ף��ֵ����
		EN_CHINESE_XINGTU_ACTIVITE_FAIL,				//û�п�λ
		EN_CHINESE_ZODIAC_EQUIP_ERROR,					//��Фװ������
		EN_CHINESE_ZOIDAC_NOT_EQUIP,					//������Фװ��

		/////////////////////////////////////	 ����2	///////////////////////////////////////////////	
		EN_XIANNV_ACTIVE_FIRST = 3100,					//���ȼ���һ����Ů
		EN_JOIN_CAMP_FIRST,								//���ȼ���һ��ʦ��
		EN_QIFU_BUYCOIN_NO_TIMES_LEFT,					//ͭ���������Ѿ�����
		EN_QIFU_BUYYUANLI_NO_TIMES_LEFT,				//�����������Ѿ�����
		EN_QIFU_BUYXIANHUN_NO_TIMES_LEFT,				//�����ɻ��������Ѿ�����
		EN_QIFU_TIMES_NOT_ENOUGH,						//������û�дﵽ��������ȡ�ý���
		EN_ACTIVITY_CANNOT_FIND,						//�û�����һ�
		EN_NATIONAL_BOSS_CAMP_FINISH,					//������ʦ�ŵ�ħ������Ѿ�����
		EN_OPEN_GAME_ACTIVITY_NOT_OPEN,					//�����û�п���
		EN_OPEN_GAME_ACTIVITY_TOTAL_CHONGZHI_NOT_ENOUGH,//�ۼƳ�ֵ����
		EN_OPEN_GAME_ACTIVITY_CAPABILITY_LIMIT,			//ս�������㣬������ȡ
		EN_OPEN_GAME_ACTIVITY_ROLELEVEL_LIMIT,			//����ȼ����㣬������ȡ
		EN_OPEN_GAME_ACTIVITY_PUTON_EQUIPMENT_LIMIT,	//װ��δ�ռ���������ȡ
		EN_OPEN_GAME_ACTIVITY_ALREADY_FETCH_REWARD,		//���Ѿ���ȡ���ý���
		EN_OPEN_GAME_ACTIVITY_GLOBAL_FETCH_TIMES_LIMIT,	//����ȡ�����Ѳ���
		EN_OPEN_GAME_ACTIVITY_ALREADY_BUY,				//���Ѿ����������Ʒ
		EN_OPEN_GAME_ACTIVITY_ALREADY_BUY_ONE,				//���Ѿ��������Ʒ
		EN_OPEN_GAME_ACTIIVTY_XIANNV_CHANMIAN_LEVEL_LIMIT,	//��Ů����ȼ�����
		EN_OPEN_GAME_ACTIVITY_MOUNT_GRADE_LIMIT,		//�����������
		EN_OPEN_GAME_ACTIVITY_STONE_LEVEL_LIMIT,		//��ʯ�ܵȼ�����
		EN_OPEN_GAME_ACTIVITY_XUNBAO_LIMIT_TIME,		//ȫ��Ѱ����������
		EN_OPEN_GAME_ACTIVITY_MONTHCARD_BUY_NUM_LIMIT,	//ȫ�����������Ա��������
		EN_OPEN_GAME_ACTIVITY_VIPLEVEL_NUM_LIMIT,		//�õȼ���Vip��������
		EN_OPEN_GAME_ACTIVITY_SHEN_FANGJU_NUM_LIMIT,	//��װ������������
		EN_MAZE_CANNOT_BUY_MOVE_TIMES,					//���չ����ж��������Ѿ����� 
		EN_ACTIVE_DEGREE_ALREADY_FETCH_REWARD,			//���Ѿ�����ý���
		EN_EXP_FB_TODAY_FREE_TIMES_LIMIT,				//�������Ѵ����Ѿ�����
		EN_CHALLENGE_FB_JOIN_TIMES_LIMIT,				//��ս�����Ѿ�����
		EN_CHALLENGE_FB_BUY_TIMES_LIMIT,				//�㵱ǰVIP�ȼ����ɹ���Ĵ����Ѵ����ޣ�
		EN_CHALLENGE_FB_RESET_LAYER_LIMIT,				//��ǰ����������
		EN_CHALLENGE_FB_CURR_LEVEL_LOCK,				//��ǰ�ؿ���δ����
		EN_CHALLENGE_FB_LEVEL_LIMIT,					//�ȼ����㣬��Ŭ��������
		EN_DISCOUNT_BUY_CLOSE_TIME_LIMIT,				//��ǰ�׶ε�һ�������Ѿ�����
		EN_DISCOUNT_BUY_BUY_COUNT_LIMIT,				//����Ʒ�Ĺ�������Ѵ�����
		EN_ZERO_GIFT_BUY_TIME_LIMIT,					//����ѹ���
		EN_ZERO_GIFT_BUY_LEVEL_LIMIT,					//����ȼ�����
		EN_ZERO_GIFT_BUY_DAY_LIMIT,						//����ڿ���������
		EN_ZERO_GIFT_GOD_COSTUME_FETCH_LEVEL_LIMIT,		//��ȡ�ȼ�����
		EN_ZERO_GIFT_FETCH_TIME_LIMIT,					//��ȡʱ�仹û��
		EN_QUESTION_ROLE_LEVEL_LIMIT,					//�ȼ����㣬���ܲ������
		EN_QUESTION_FETCH_QUESTION_TIMES_LIMIT,			//��������Ѿ�����
		EN_TOWERDEFEND_FB_BUY_LIMIT_TIMES,				//�㵱ǰVIP�ȼ����ɹ���Ĵ����Ѵ����ޣ�
		EN_WING_NOT_ACTIVE,								//����û����
		EN_MAZE_MOVE_TIMES_FULL,						//��ǰ�ж����Ѿ��������ܹ���
		EN_CHALLENGE_FIELD_OPPONENT_NOT_EXIST,			//���ֲ�����
		EN_CHALLENGE_FIELD_JOIN_TIMES_LIMIT,			//��ս�������㣬�빺�������ս����
		EN_CHALLENGE_FIELD_JIFEN_LIMIT,					//���ֲ��㣬��ս������ҿɻ�û���
		EN_CHALLENGE_FIELD_FETCH_EMPTY,					//��ǰ��������ȡ����ȴ���һ����������
		EN_PHASE_FB_FUN_OPEN_LEVEL_LIMIT,				//δ�ﵽ���������ȼ�
		EN_PHASE_FB_FUN_OPEN_PRE_TASK_LIMIT,			//δ��ɸ�����������
		EN_PHASE_FB_TIMES_LIMIT,						//��ս�����Ѿ�����
		EN_PHASE_FB_NO_PASS,							//����ͨ�ظùؿ�
		EN_PHASE_FB_AUTO_LEVEL_LIMIT,					//δ�ﵽɨ�������ȼ�
		EN_PHASE_FB_PASS_ALL_LIMIT,						//���Ѿ�ͨ�����йؿ�
		EN_SKILL_MAX_LEVEL_LIMIT,						//�����Ѿ�������߼�
		EN_MAZE_HIT_BOMB,								//����ը�����۳�һ���ж���
		EN_REALIVE_HERE_IN_CD,							//ԭ�ظ���������CD��
		EN_STORY_FB_FUN_OPEN_LEVEL_LIMIT,				//δ�ﵽ���������ȼ�
		EN_STORY_FB_TIMES_LIMIT,						//��ս�����Ѿ�����
		EN_STORY_FB_NO_PASS,							//����ͨ�ظùؿ�
		EN_STORY_FB_NOT_LEVEL,							//��ǰ�޿�ɨ���ؿ�

		EN_WABAO_JOIN_TIME_LIMIT,						//��ͼ����������
		EN_WABAO_BAOZANG_NOT_IN_SAME_SCENE,				//���ز������������
		EN_DIG_BAOZANG_LONG_LONG_AWAY,					//���ػ��ں�Զ�ĵط�
		EN_DIG_BAOZANG_LONG_AWAY,						//�����ƺ����ڲ�Զ����
		EN_DIG_BAOZANG_MID_LONG_AWAY,					//���ؾ��ڸ�����
		EN_DIG_BAOZANG_SMALL_LONG_AWAY,					//Խ��Խ�ӽ������ˣ�
		EN_DIG_BAOZANG_NEARBY,							//���ؾ����Ա��ˣ�
		EN_QIFU_FREE_TIMES_LIMIT,						//����������Ѿ�����
		EN_QIFU_FREE_CHANCE_CD,							//��������ỹ����ȴ��
		EN_TUMO_COMPLETE_ALL_TASK_FIRST,				//������������ճ�����
		EN_TUMO_HAS_FETCH_COMPLETE_ALL_REWARD,			//�Ѿ���ȡ���ý���
		EN_ROLE_TILI_LIMIT,								//��������
		EN_EXP_FB_LEVEL_UNLOCK,							//��������ͨ��ǰһ�ؿ�
		EN_MAZE_GAME_FINISH,							//�Թ���Ϸ�Ѿ������������ٹ����ƶ�����
		EN_RENAME_NAME_INVALID,							//���Ƿ��ַ�������������
		EN_TEAM_FB_OPEN_TIMES_LIMIT,					//�����쿪����������������
		EN_TEAM_FB_PARTNER_NOT_ONLINE,					//�ж��Ѳ�����
		EN_TEAM_FB_JOIN_LEVEL_LIMIT,					//�ж��ѵȼ������Խ��븱��
		EN_TEAM_FB_ASSIST_TIMES_LIMIT,					//�ж��ѵ�Э����������
		EN_TEAM_FB_ENTER_FB_LIMIT,						//�ж��ѵ�ǰ���ܽ��븱������
		EN_SHENGWANG_LESS_LIMIT,						//��������
		EN_XIANMENGZHAN_GATHER_LIMIT,					//�ݵ��Ѿ���ռ��
		EN_FB_TEAM_TYPE_YAOSHOUJITAN_LIMIT,				//���������޼�̳���ģʽ�²��ܽ��븱��
		EN_FB_TEAM_TYPE_TOWERDEFEND_LIMIT,				//����������������ģʽ�²��ܽ��븱��
		EN_FB_TEAM_TYPE_MIGONGXIANFU_LIMIT,				//�������Թ��ɸ����ģʽ�²��ܽ��븱��
		EN_TEAM_FB_OPEN_FB_LIMIT,						//ֻ�жӳ����ܿ���
		EN_CHONGZHI_HAS_FETCH_REWARD,					//���Ѿ���ȡ���ý��� 
		EN_CHONGZHI_CAN_NOT_FETCH_REWARD,				//����������ȡ�ý���
		EN_CHONGZHI_TOTAL_RECHARGE_LIMIT,				//��ֵ���㣬������ȡ�ý���
		EN_CHONGZHI_NOT_RECHARGE_TODAY,					//���컹û�г�ֵ
		EN_CHONGZHI_FIRST_RECHARGE_NOT_OPEN,			//�׳���δ��ʼ
		EN_CISHA_TASK_ALREAD_OWN_CISHA_TASK,			//���Ѿ���ȡ��һ����ɱ����
		EN_CISHA_TASK_ACCEPT_TIMES_LIMIT,				//����Ĵ�ɱ�����Ѿ����
		EN_JILIAN_OUT_OF_RANGE,							//�����ɼ����ɵ���Χ
		EN_JILIAN_JOIN_LIMIT_TIMES,						//����ļ��������Ѿ�����
		EN_JILIAN_START_JILIAN_FIRST,					//���ȿ�ʼ����
		EN_JILIAN_FINISH_JILIAN_FIRST,					//������ɱ��μ���
		EN_CHALLENGE_FB_RESET_LEVEL_FIRST,				//�������ø���
		EN_WING_SPECIAL_IMG_ALREAD_ACTIVE,				//�������������Ѿ�����
		EN_FOOTPRINT_ALREAD_ACTIVE,						//���㼣�Ѿ�����
		EN_FOOTPRINT_NOT_ACTIVE,						//���㼣��δ����
		EN_MARRY_TEAM_MEMBER_MUST_IN_SAME_MAP,			//�Է�������ͬһ��ͼ
		EN_MARRY_MUST_DIFFERENT_SEX_IN_TEAM,			//ͬ�Ա𲻿��Խ��
		EN_MARRY_RESERVE_MUST_BE_TEAM_LEADER,			//ֻ�жӳ��������뿪ʼ
		EN_TEAM_MUST_HAVE_MEMEBER,						//ץ�����������˲��ܿ�ʼŶ��
		EN_MARRY_HAS_BEEN_MARRY,						//˫�������Ѿ������
		EN_MARRY_LEVEL_NOT_ENOUGH,						//�ȼ����㣬��������
		EN_MARRY_INTIMACY_NOT_ENOUGH,					//�������ܶȲ���
		EN_MARRY_RET_NOT_ACCEPT,						//�Է��ܾ����������
		EN_DIVORCE_RET_NOT_ACCEPT,						//�Է��ܾ����������
		EN_MARRY_TEAM_MEMBER_NUM_INVALID,				//�뵥���������ӽ��븱��
		EN_QINGYUAN_EQUIP_STUFF_ERROR,					//������ǲ���ʹ�ô���
		EN_QINGYUAN_EQUIP_NOT_ACTIVE,					//��Եװ����δ����
		EN_QINGYUAN_EQUIP_NOT_GOOD_AND_LEVEL_MAX,		//����Ʒ�Ĳ�������Ʒ�ʲ���װ���ȼ��Ѿ��ﵽ���
		EN_QINGYUAN_EQUIP_QUALITY_ALREADY_MAX,			//Ʒ���Ѿ����������
		EN_QINGYUAN_EQUIP_SLOT_OPEN_LIMIT,				//���󿪷Ÿ�װ��
		EN_QINGYUAN_EQUIP_LEVEL_MAX,					//��Եװ���Ѵﵽ��ߵȼ�
		EN_QINGYUAN_EQUIP_ALREADY_HAVE,					//������Եװ��
		EN_QINGYUAN_MAX_BUY_COUNT,						//��Ե�����Ѵ��������
		EN_QINGYUAN_TEAM_MEMBER_ERROR,					//��Ե������������Ϊ2��

		EN_QINGYUAN_EQUIP_NO_MARRY,						//����δ�а���
		EN_QINGYUAN_MARRY_LEVEL_NOT_ENOUGH,				//���ȼ����㣬�޷�����
		EN_QINGYUAN_EQUIP_SUIT_ACTIVE_LIMIT,			//����װ��λ�Ѽ���
		EN_QINGYUAN_EQUIP_SUIT_EQUIP_ATTR_LIMIT,		//���װ�����Բ������㼤������

		EN_QINGYUAN_TUODAN_TIMES_MAX,					//ÿ��ֻ�ܷ��������ѵ�����
		EN_QINGYUAN_TUODAN_SUCCESS,						//�ѵ����Է����ɹ�

		EN_QINGYUAN_TREE_LEVEL_MAX,						//��˼���Ѿ��ﵽ��ߵȼ�
		
		EN_MARRY_HUNYAN_OPEN_LIMIT,						//���쿪����������Ѿ�����
		EN_MARRY_HUNYAN_JOIN_LIMIT,						//��������������Ѿ�����
		EN_MARRY_NOT_MARRIED,							//��û�н��
		EN_MARRY_HUNYAN_WITH_OTHER,						//�������Ӻ���ܽ������
		EN_TRADE_ITEM_IS_CHANGE,						//������Ʒ�Ѿ��ı�
		EN_NOT_AT_TRADING,								//�����ʵ��Ľ���״̬
		EN_REJECT_TRADE,								//�Է��ܾ������Ľ�������
		EN_QINGYUAN_TARGET_NOT_ON_LINE,					//���²�����
		EN_QINGYUAN_TARGET_IN_TEAM,						//���²��ڶ�����
		EN_QINGYUAN_FB_CAN_NOT_SKIP,					//��������,��������
		EN_TRADE_CHECK_TRADE_FAIL,						//��齻��ʧ��
		EN_QINGYUAN_DIVORCE_DAYS_LIMIT,					//���1��󣬲������
		EN_QINGYUAN_REMARRY_DAYS_LIMIT,					//��鵱�첻�ܽ��
		EN_QINGYUAN_ANOTHER_REMARRY_LIMIT_DAYS,			//�Է������
		EN_TRADE_LOCK_STATE_CAN_NOT_CHANGE,				//��ǰ�������������޷�������Ʒ
		EN_TRADE_FAILED_OWN_BAG_HAS_FULL,				//�������㣬����ʧ��
		EN_TRADE_FAILED_ANOTHER_BAG_HAS_FULL,			//�Է��������㣬����ʧ��
		EN_TRADE_FAILED_CANCLE_SELF,					//������ȡ��
		EN_TRADE_FAILED_CANCLE_ANOTHER,					//�Է�ȡ�����ף�����ʧ��
		EN_TRADE_PUTITEM_MAX_COUNT,						//��ǰ������Ʒ�����ﵽ���ֵ
		EN_TRADE_WAIT_FOR_ANOTHER_LOCK,					//�ȴ��Է�����
		EN_TRADE_LOCK_FIRST,							//��������������Ʒ
		EN_TRADE_LEVEL_LIMIT,							//����˫���ȼ�����ﵽ31��
		EN_ZHANSHENDIAN_LEVEL_LIMIT,					//δ�ﵽ�μ���ħ������ȼ�
		EN_ZHANSHENDIAN_MAX_LEVEL,						//����ͨ����ħ����߲�
		EN_LUCKYROLL_TIMES_NOT_ENOUGH,					//�ۼ�ת���������㣬�޷���ȡ�������
		EN_CLOSE_BETA_ACTIVITY_NOT_OPEN,				//���û�п���
		EN_CLOSE_BETA_HAS_FETCH_REWARD,					//�Ѿ���ȡ���ý���
		EN_CLOSE_BETA_FETCH_REWARD_LIMIT,				//��û�����ȡ��������
		EN_CLOSE_BETA_HAS_NOT_MARRY,					//����û�����
		EN_CLOSE_BETA_JOIN_GUILD_FIRST,					//����һ�����˲�����ȡ�ý���
		EN_CLOSE_BETA_ONLINE_TIMES_LIMIT,				//����ʱ�䲻�㣬���Ժ�����
		EN_CLOSE_LUCKYROLL_NOT_OPEN,					//����ת�̻δ����
		EN_GUILD_TASK_ALL_COMPLETE,						//���������Ѿ�ȫ�����
		EN_GUILD_TASK_ONE_KEY_VIP_LEVEL_LIMIT,			//VIP�ȼ�̫�Ͳ���һ�����
		EN_FRIEND_BLESS_FRIEND_LIMIT,					//�Է�������ĺ��ѣ�����ף��
		EN_FRIEND_BLESS_HAS_BLESS,						//��ף���ú��ѣ�����������
		EN_FRIEND_BLESS_NOT_BLESS,						//�Է�û��ף����
		EN_FRIEND_BLESS_HAS_FETCH_REWARD,				//����ȡ����ף������
		EN_FRIEND_BLESS_FETCH_REWARD_TIMES_LIMIT,		//�����ף�����������Ѿ�����
		EN_FRIEND_BLESS_BLESS_TIMES_LIMIT,				//ף�������Ѿ��ﵽ����
		EN_FRIEND_BLESS_ENERGY_IS_FULL,					//��������
		EN_TOUZIJIHUA_ACTIVITY_NOT_OPEN,				//�δ�������ܼ���
		EN_TOUZIJIHUA_LEVEL_LIMIT,						//����Ͷ�ʵȼ�
		EN_TOUZIJIHUA_PLAN_NOT_ACTIVE,					//�ƻ���û����
		EN_TOUZIJIHUA_PLAN_HAS_ACTIVE,					//�Ѿ������Ͷ�ʼƻ�
		EN_TOUZIJIHUA_HAS_FETCH_REWARD,					//���Ѿ���ȡ���ý���
		EN_TOUZIJIHUA_FETCH_REWARD_LIMIT,				//�ȼ����㣬�޷���ȡ�ý���
		EN_TOUZIJIHUA_FOUNDATION_BUY_LEVEL_LIMIT,		//��ǰ�ȼ������㹺���������޷�����
		EN_TOUZIJIHUA_FOUNDATION_HAD_BUY,				//���Ѿ��������
		EN_TOUZIJIHUA_FOUNDATION_FETCH_LEVEL_LIMIT,		//�ȼ����㣬�޷���ȡ�ý���
		EN_TOUZIJIHUA_FOUNDATION_FETCH_NOT_BUY,			//�㻹û�й����޷���ȡ�ý���
		EN_TOUZIJIHUA_TIME_LIMIT,						//�������㣬�޷���ȡ�ý���
		EN_ROLL_MONEY_ROLL_TIMES_LIMIT,					//ҡ������������
		EN_ROLL_MONEY_DAILY_ROLL_TIMES_LIMIT,			//����ҡ������������
		EN_ROLL_MONEY_HAS_FETCH_ALL_REWARD,				//�Ѿ����������н�������
		EN_ROLL_MONEY_ACTIVE_DEDREE_LIMIT,				//��Ծ��δ�ﵽҪ��
		EN_ROLL_MONEY_ALREADY_ROLL_TODAY,				//����ɼ���ҡ��
		EN_ROLL_MONEY_LOGIN_DAYS_NOT_ENOUGH,			//��¼��������
		EN_FISH_POOL_DAY_RAISE_COUNT_LIMIT,				//������ÿ�շ�����������
		EN_FISH_POOL_POOL_RAISE_COUNT_LIMIT,			//��ز����ٷ���������
		EN_FISH_POOL_POOL_LEVEL_LIMIT,					//��صȼ�����
		EN_FISH_POOL_CAPACITY_LIMIT,					//�����������
		EN_FISH_POOL_GUARD_FISH_CAPACITY_LIMIT,			//���������������������
		EN_FISH_POOL_EXTEND_CAPACITY_MAX_LIMIT,			//��չ�����ﵽ����
		EN_FISH_POOL_HARVEST_GRADE_LIMIT,				//����������ջ�
		EN_FISH_POOL_STEAL_GRADE_LIMIT,					//��������ܱ�͵	
		EN_FISH_POOL_LACK_BULLET,						//ȱ���ӵ�
		EN_FISH_POOL_BUY_BULLET_LIMIT_TIMES,			//�����ӵ���������
		EN_FISH_POOL_CANNOT_STEAL_SELF,					//����͵�Լ�����
		EN_FISH_POOL_FISH_TOO_LITTLE,					//���̫С���޷�����
		EN_FISH_POOL_STEAL_ERROR,						//͵��ʧ�ܣ�����������͵��
		EN_FISH_POOL_IS_CHANGE,							//�����ѱ仯�������²���
		EN_FISH_POOL_STEAL_FAIL,						//���������۵���������
		EN_FISH_POOL_TEALFISH_TIME_LIMIT,				//����������͵��

		EN_FISH_POOL_FANG_FISH_TIMES_LIMIT,			    //��������Ѿ�����
		EN_FISH_POOL_UP_FISH_LIMIT,						//���ﻹ���㣬��������
		EN_FISH_POOL_UP_FISH_FAIL,						//����ʧ��
		EN_FISH_POOL_BUY_FANG_FISH_TIMES_LIMIT,			//���첻���ٹ����������

		EN_ZHANSHENDIAN_ALREADY_FETCH_DAY_REWARD,		//��������ȡ������
		EN_HUNYAN_HONGBAO_BUY_TOOMUCH,					//�������Ѵﵽ����
		EN_HUNYAN_NOT_START,							//���绹û��ʼ�����Ժ�
		EN_HUNYAN_INVITE_FAIL,							//����ʧ��
		EN_HUNYAN_GATHAR_TOO_MUCH,						//�ɼ��Ѵﵽ����
		EN_HUNYAN_GATHAR_FOOD_TOO_MUCH,					//��ʳ���Ѵﵽ����
		EN_HUNYAN_SAXIANHUA_MAX_COUNT,					//���ʻ�����������
		EN_HUNYAN_ALREADY_START,						//����ֻ�ܾ���һ��
		EN_HUNYAN_ALREADY_STOP,							//�����Ѿ�����
		EN_HUNYAN_IS_RUNNING,							//�����Ѿ���ʼ��
		EN_WORLD_EVENT_HAS_FIND_NPC,					//���Ѿ��ҵ������NPC
		EN_QINGYUAN_DIVORCE_LIMIT,						//���翪���ڼ䲻�������
		EN_QINGYUAN_HUNYAN_INVITE_TOO_MUCH,				//�Է�������������
		EN_FISHPOOL_HARVEST_LIMIT,						//��ǰ�׶β��ܲ���
		EN_FISHPOOL_STEAL_LIMIT,						//�����޷�����,����ʧ��
		EN_WORLD_EVENT_NOT_FIT_NPC,						//�ⲻ������ǰ����ҪѰ�ҵ�NPC
		EN_WORLD_EVENT_LIGHT_WORD_STUFF_LIMIT,			//������ϲ���
		EN_WORLD_EVENT_NOT_REWARD_TO_FETCH,				//û�п���ȡ�Ľ���
		EN_WORLD_EVENT_DISTANCE_LIMIT,					//�����Զ
		EN_WORLD_EVENT_USE_FOOL_CARD_LIMIT,				//����û�п��Ա���������
		EN_WORLD_EVENT_SHENMEZHIXI_FB_LEVEL_LIMIT,		//30�����ϲ��ܽ�����ħ֮϶
		EN_RAND_ACTIVITY_NOT_OPEN,						//���δ����
		EN_RAND_ACTIVITY_PERSONAL_BUY_NUM_LIMIT,		//����Ʒ��������Ѿ��ﵽ����
		EN_RAND_ACTIVITY_SERVER_BUY_NUM_LIMIT,			//���簡���õ����Ѿ���������
		EN_FBMANAGER_JOIN_FB_LIMIT,						//���ڻ����ܲ���˸���
		EN_RAND_ACTIVITY_HAS_FETCH_REWARD,				//�Ѿ���ȡ���ý���
		EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD,			//����δ��ɣ��޷���ȡ
		EN_ROLE_GOAL_HAS_FETCH_REWARD,					//û�п���ȡ�Ľ���
		EN_ROLE_GOAL_CAN_NOT_FETCH_REWARD,				//��������ȡ�ý���
		EN_ROLE_GOAL_SKILL_LEVEL_MAX,					//�����Ѵ����
		EN_RAND_ACTIVITY_MENTALITY_CAN_NOT_FETCH_REWARD,//��������ȡ�ý���
		EN_RAND_ACTIVITY_MENTALITY_HAS_FETCH_REWARD,	//����ȡ
		EN_RAND_ACTIVITY_NOT_GIVE_FLOWER,				//���Ȳ����ͻ��
		EN_RAND_ACTIVITY_CORNUCOPIA_VALUE_LIMIT,		//�۱�ֵ����
		EN_RED_PAPER_CAN_FETCH_TIMES_LIMIT,				//ÿ�պ����ȡ�������ܳ���100

		EN_RAND_ACTIVITY_LOTTERY_TOTAL_TIMES_LIMIT,		//�ۼƹ����������
		EN_RAND_ACTIVITY_LOTTERY_HAS_FETCH_LIMIT,		//���Ѿ���ȡ���ý���
		EN_RAND_ACTIVITY_LOTTERY_VIP_LIMIT,				//vip�ȼ����㣬������ȡ

		EN_RED_PAPER_ONCE_GOLD_NUM_LIMIT,				//����������ܳ���200
		EN_HUNYAN_FA_BLESS_GIFT_COUNT_LIMIT,			//ÿ�ջ��緢�����������

		EN_YIZHANDAODI_GUWU_MAX,						//�����Ѵ�����
		EN_YIZHANDAODI_DEAD_TIMES_LIMIT,				//�ô�һս�����У����Ѿ���������8�Σ����´���ս
		EN_TEAM_TOWERDEFEND_MUST_BE_TEAM_LEADER,		//ֻ�жӳ���������
		EN_TEAM_TOWERDEFEND_MEMEBER_COUNT_LIMIT,		//���鲻��3��
		EN_TEAM_TOWERDEFEND_JOIN_TIMES_LIMIT,			//���Ѿ�������ø���
		EN_TEAM_TOWERDEFEND_PARTNER_NOT_ONLINE,			//�ж��Ѳ�����
		EN_SKILL_NOT_LEARNT,							//û���������
		EN_SCORE_NOT_ENOUGH,							//���ֲ���
		EN_TEAM_TOWERDEFEND_REPEAT_ATTR_TYPE,			//�����Ѷ�Ա��ȷѡ������
		EN_CISHA_TASK_ACTIVITY_OPEN_LIMIT,				//��ɱ���δ���������ܽ��ܴ�ɱ����
		EN_MARRIAGE_SEEKING_SUCCESS,					//���Գɹ�
		EN_MARRIAGE_SEEKING_MARRIED,					//���Ѿ���飬���ܽ�������
		EN_LINGYUSHOUSHUI_HAS_FETCH_REWARD,				//���Ѿ���ȡ�˸ý���
		EN_LINGYUSHOUSHUI_GCZ_WIN_REWARD_CAMP_LIMIT,	//���ͳ�������ͬһ��ʦ��
		EN_LINGYUSHOUSHUI_XMZ_REWARD_RANK_LIMIT,		//�������˵�ǰû��ռ���κ�����
		EN_GUILD_FB_START_TUANZHANG_LIMIT,				//��û��Ȩ�޿������������
		EN_GUILD_FB_IS_OPEN,							//�����������ڽ�����
		EN_GUILD_FB_ALREADY_START_ONCE,					//���������Ѿ���������
		EN_GUILD_BONFIRE_START_TUANZHANG_LIMIT,			//��û��Ȩ�޿�����Ůף���
		EN_GUILD_BONFIRE_ALREADY_START_ONCE,			//��Ůף����Ѿ���������
		EN_GUILD_BONFIRE_SCENE_LIMIT,					//��ǰ��������������Ůף���
		EN_GUILD_BONFIRE_SAVE_AREA_LIMIT,				//��ȫ������������Ůף���
		EN_GUILD_BONFIRE_IS_OPEN,						//��Ůף������ڽ�����
		EN_GUILD_ZHUFU_LUCKY_NOT_IN_YOURGUILD,			//�Է����������������
		EN_GUILD_ZHUFU_LUCKY_FLUSH_ISSAME,				//�ǳ��ź������Ʋ���
		EN_GUILD_ZHUFU_LUCKY_FLUSH_UPGRADE,				//���˵�ͷ����������
		EN_GUILD_ZHUFU_LUCKY_IS_FULL,					//�Է����������
		EN_GUILD_ZHUFU_LUCKY_TIMES_IS_OUT,				//������ף������������
		EN_GUILD_FB_IS_OPEN_CANNOT_DISMISS,				//������������ڽ��У��������ɢ����
		EN_GUILD_BONFIRE_IS_OPEN_CANNOT_DISMISS,		//��Ůף������ڽ��У��������ɢ����
		EN_QIBING_MOUNT_NOT_ACHIVE,						//��Ҫ����������������﹦��
		EN_HUNYAN_GATHER_XIANHUAN_LIMIT,				//���տɻ�ȡ���罱���Ѵ����ֵ
		EN_HUNYAN_GET_NUM_MAX,							//�������õ��ý���������
		EN_HUNYAN_GET_NUM_MAX_EXP,						//�������õ����齱��������
		EN_HUNYAN_GET_NUM_MAX_BIND_GOLD,				//�������õ����꽱��������
		EN_HUNYAN_GET_NUM_MAX_HUNJIE_STUFF,				//�������õ���佱��������
		EN_HUNYAN_GET_NUM_MAX_ROSES,					//�������õ�õ�廨����������
		EN_YANHUI_INVITE_NUMBER_MAX,					//������������Ѵﵽ���ֵ
		EN_YANHUI_TEAM_MEMBER_ERROR,					//��������������ϣ������Ǹ��˻�Ͱ�����Ӳ��ܽ��룡
		EN_YANHUI_NOT_RUNNING,							//����Ѿ�����
		EN_YANHUI_PART_BE_INVITED,						//�����������Ѵ����ֻ��һ�����˱����룡
		EN_SHIZHUANG_NOT_ACTIVE,						//ʱװδ����
		EN_SHIZHUANG_HAS_ACTIVE,						//ʱװ�Ѿ�����
		EN_CHANGE_CAMP_NOCAMP_LIMIT,					//���ȼ���ʦ��
		EN_CHANGE_CAMP_LEVEL_LIMIT,						//�ȼ����㣬����תʦ��
		EN_CHANGE_CAMP_OPEN_DAY_LIMIT,					//������һ�첻��תʦ��
		EN_CHANGE_CAMP_LIMIT_CHENGZHU,					//���ǳ�������תʦ��
		EN_CHANGE_CAMP_LIMIT_HAS_TASK,					//���ͺʹ�ɱ�����ڼ䲻��תʦ��
		EN_CHANGE_CAMP_LIMIT_HAS_WABAO,					//��Ů�Ӷ��ڼ䲻��תʦ��
		EN_CHANGE_CAMP_LIMIT_CAMPDEFEND,				//���������ڼ䲻��תʦ��
		EN_ZHUANZHI_MARRY_LIMIT,						//����ɫ����ת�Ա�
		EN_ZHUANZHI_LEVEL_LIMIT,						//�ȼ����㣬����תְ
		EN_ALL_REWARD_HAS_FETCHED,						//���н�����������
		EN_CHONGZHI_WANT_MONEY_HAS_NOT_ENOUGH_CHARGE,	//δ�ﵽĿ���ֵ���
		EN_CHONGZHI_WANT_MONEY_HAS_NOT_ENOUGH,			//ʣ��Ԫ������
		EN_ZAI_CHONGZHI_FETCH_LIMIT,					//����û������ȡ����
		EN_ZAI_CHONGZHI_HAS_FETCH,						//������ȡ���ٳ�ֵ����
		EN_WING_CAN_NOT_UPGRADE,						//�����ܽ���
		EN_WING_GRADE_NOT_ACTIVE,						//��������δ����
		EN_MONTH_CARD_HAS_NOT_ACTIVITY_FETCH,			//����û�й���򼤻��¿�
		EN_MONTH_CARD_REWARD_HAS_FETCH,					//����ȡ��Ԫ��
		EN_CHARGE_REPAYMENT_NO_CFG,						//�ý���������
		EN_CHARGE_REPAYMENT_NOT_ACTIVE,					//δ�ﵽ��ֵ���
		EN_CHARGE_REPAYMENT_HAS_FETCHED,				//�ý�������ȡ
		EN_SHUXINGDAN_USE_LIMIT_NUM,					//���Ե�ʹ�������Ѵ�����
		EN_FIGHTMOUNT_SHUXINGDAN_USE_LIMIT_NUM,			//ս�����ʵ�ʹ�������Ѵ�����
		EN_FIGHTMOUNT_SHUXINGDAN_USE_ORDER_LIMIT,		//ս���������
		EN_MOUNT_SHUXINGDAN_USE_LIMIT_NUM,				//�������ʵ�ʹ�������Ѵ�����
		EN_MOUNT_SHUXINGDAN_USE_ORDER_LIMIT,			//�����������
		EN_WING_SHUXINGDAN_USE_LIMIT_NUM,				//�������ʵ�ʹ�������Ѵ�����
		EN_WING_SHUXINGDAN_USE_ORDER_LIMIT,				//�����������
		EN_HALO_SHUXINGDAN_USE_LIMIT_NUM,				//�⻷���ʵ�ʹ�������Ѵ�����
		EN_HALO_SHUXINGDAN_USE_ORDER_LIMIT,				//�⻷��������
		EN_SHENGONG_SHUXINGDAN_USE_LIMIT_NUM,			//��Ů�⻷���ʵ�ʹ�������Ѵ�����
		EN_SHENGONG_SHUXINGDAN_USE_ORDER_LIMIT,			//��Ů�⻷��������
		EN_SHENYI_SHUXINGDAN_USE_LIMIT_NUM,				//�������ʵ�ʹ�������Ѵ�����
		EN_SHENYI_SHUXINGDAN_USE_ORDER_LIMIT,			//�����������
		EN_FOOTPRINT_SHUXINGDAN_USE_LIMIT_NUM,			//�㼣���ʵ�ʹ�������Ѵ�����
		EN_FOOTPRINT_SHUXINGDAN_USE_ORDER_LIMIT,		//�㼣��������
		EN_CLOAK_SHUXINGDAN_USE_LIMIT_NUM,				//�������ʵ�ʹ�������Ѵ�����
		EN_CLOAK_SHUXINGDAN_USE_ORDER_LIMIT,			//�����������
		EN_CSA_QIANGGOU_NUM_LIMIT,						//����Ʒ�Ѿ�������һ��
		EN_CSA_ROLL_CHONGZHI_LIMIT,						//��ֵ���㣬����ҡ��
		EN_CSA_CAN_NOT_FETCH_REWARD,					//����δ��ɣ��޷���ȡ
		EN_CSA_HAS_FETCH_REWARD,						//�Ѿ���ȡ���ý���
		EN_SUPPLY_MAX_LIMIT,							//�Ѿ�����̫�ಹ��
		EN_CSA_PERSONAL_BUY_NUM_LIMIT,					//����Ʒ��������Ѿ��ﵽ����
		EN_CSA_SERVER_BUY_NUM_LIMIT,					//���簡���õ����Ѿ���������
		EN_DANBI_CHONGZHI_REWARD_HAS_NOT_ACTIVE,		//����δ����
		EN_DANBI_CHONGZHI_REWARD_HAS_FETCH,				//��������ȡ��
		EN_ZHUANZHI_COMPLETE_TASK_FIRST,				//�������תְ����
		EN_RA_TOTAL_CHARGE_DAY_CAN_NOT_FETCH_REWARD,	//�����޷���ȡ
		EN_RA_TOTAL_CHARGE_DAY_REWARD_NOT_ACTIVE,		//����δ����
		EN_RA_TOTAL_CHARGE_DAY_REWARD_HAS_FETCH,		//��������ȡ��
		EN_ONLY_TEAM_LEADER_CAN_REQUEST_START,			//ֻ�жӳ��������뿪ʼ
		EN_SOME_TEAMMATE_NOT_ONLINE,					//�ж��Ѳ�����
		EN_CHENGZHANGDAN_USE_LIMIT_NUM,					//�ɳ���ʹ�������Ѵ�����
		EN_CSA_TOUZIJIHUA_NOT_BUY,						//��û�й���
		EN_CSA_TOUZIJIHUA_HAD_BUY,						//�Ѿ��������
		EN_CSA_TOUZIJIHUA_HAD_FETCH,					//�Ѿ���ȡ���ý���
		EN_CSA_TOUZIJIHUA_FETCH_LIMIT,					//��������ȡ�ý���
		EN_CSA_LEVEL_LIMIT,								//���ĵȼ������Ϲ�������

		EN_WUSHUANG_LIEHUN_POOL_FULL,					//�Ի������
		EN_WUSHUANG_ALREADY_MAX_CHOUHUN_COLOR,			//��ֱ�ӳ��
		EN_WUSHUANG_BAG_ALREAD_FULL,					//��������
		EN_WUSHUANG_EQUIP_FULING_SLOT_USED,				//�ò�λ�Ѿ���ʹ��
		EN_WUSHUANG_EQUIP_FULING_SAME_HUNSHOU_LIMIT,	//ͬһ������ֻ��װ��һ��
		EN_WUSHUANG_FUHUN_ADD_EXP_ALREADY_REACHED_LEVEL,//�Ѿ�����ָ���ȼ�
		EN_WUSHUANG_FUHUN_EXP_LIMIT,					//���꾭�鲻��
		EN_WUSHUANG_FB_HAS_PASSED_MAX_LEVEL,			//����ͨ����˫������߲�
		EN_WUSHUANG_FB_NOT_ENOUGH_LEVEL,				//�ȼ����㣬�޷���ս
		EN_WUSHUANG_FB_TODAY_HAS_FETCH_REWARD,			//�����������Ľ���
		EN_WUSHUANG_FB_HAS_NOT_PASS,					//����û��ͨ�ع��ø���
		EN_WUSHUANG_QUALITY_IS_MAX,						//��װ����ǰƷ���������
		EN_WUSHUANG_DAZAO_LEVEL_IS_MAX,					//��װ������ȼ��������

		EN_LIEMING_LIEHUN_POOL_FULL,					//�Ի������
		EN_LIEMING_ALREADY_MAX_CHOUHUN_COLOR,			//��ֱ�ӳ��
		EN_LIEMING_BAG_ALREAD_FULL,						//��������
		EN_LIEMING_EQUIP_FULING_SLOT_USED,				//�ò�λ�Ѿ���ʹ��
		EN_LIEMING_EQUIP_FULING_SAME_HUNSHOU_LIMIT,		//ͬһ�����ֻ��װ��һ��
		EN_LIEMING_FUHUN_ADD_EXP_ALREADY_REACHED_LEVEL, //�Ѿ��������ȼ�
		EN_LIEMING_FUHUN_EXP_LIMIT,						//��꾭�鲻��
		EN_LIEMING_SOLT_NOT_ACTIVTY,					//�ò�λδ����
		EN_LIEMING_SOLT_NO_SPACE,						//û�п��ò�λ

		EN_CARDZU_HUALING_CARD_NUM_LIMIT,				//������������
		EN_CARDZU_ZHULING_LINGLI_LIMIT,					//��ǰ��������
		EN_CARDZU_HAS_NOT_ACTIVED,						//�ÿ������δ����
		EN_CARDZU_HAS_ACTIVED,							//�ÿ�������Ѽ����
		EN_CARDZU_ACTIVED_ZUHE_CARD_NUM_LIMIT,			//������������
		EN_CARDZU_UPGRADE_ZUHE_LINGLI_LIMIT,			//��ǰ��������
		EN_CARDZU_CHOUKA_TIME_LIMIT,					//�鿨�����Ѵ�����
		EN_FB_TEAM_TYPE_ZHUANZHI_LIMIT,					//������תְ�������ģʽ�²��ܽ��븱��
		EN_CHAT_WORD_LEVEL_LIMIT,						//80��������������
		EN_SINGLE_CHAT_OTHER_LEVEL_LIMIT,				//�Է�˽�ĵȼ�����
		EN_NOT_START_CROSS_IN_FB,						//�����в��ܽ�����
		EN_CAN_NOT_START_CROSS,							//���޷��μӵ�ǰ����
		EN_CROSS_HONOR_LESS_LIMIT,						//�����������
		EN_CAN_NOT_USE_ITEM_IN_CROSS,					//�ڿ���в�����ʹ�õ���
		EN_GUILD_CURRENT_GUILD_STATION_BUSY,			//����������������
		EN_GUILD_STATION_BUSY_CANNOT_DISMISS,			//���˻�����У����ܽ�ɢ����
		EN_GUILD_ACTIVE_DEGREE_LIMIT,					//���˻�Ծ�Ȳ���
		EN_GUILD_TOTEM_EXP_NOT_ENOGHT,					//ͼ�ھ��鲻��
		EN_RA_TOMORROW_REWARD_COUNT_LIMIT,				//ʣ��齱��������
		EN_QINGYUAN_CARD_ID_INVALID,					//�ÿ��Ʋ�����
		EN_QINGYUAN_CARD_HAS_FULL_LEVEL,				//�ÿ���������ߵȼ�
		EN_QINGYUAN_CARD_UPGRADE_ITEM_LIMIT,			//�ϳɲ��ϲ���
		EN_EQUIP_CROSS_EQUIP_CANNOT_PUTON,				//����Ʒ����װ��
		EN_EQUIP_CROSS_EQUIP_ALREAD_EQUIP,				//�Ѿ�װ����װ��
		EN_CROSS_XIULUOTA_BUY_REALIVE_LIMIT_TIMES,		//��ǰ�����Ѵ����ֵ
		EN_CROSS_XIULUOTA_GUWU_LIMIT_TIMES,				//�ɹ��򸴻�����Ѵ����ֵ
		EN_CROSS_XIULUOTA_LEVEL_LIMIT,					//�ȼ�����301�����ܽ����޾�֮��
		EN_CROSS_XIULUOTOWER_HAS_BUY_BUFF,				//���Ѿ��������buff
		EN_QINGYUAN_CARD_UPGRADE_LEVEL_LIMIT,			//��ɫ�ȼ�����
		EN_CROSS_NOT_CONNECTED_TO_CROSS_SERVER,			//û�����ӵ����������
		EN_CROSS_1V1_CAPABILITY_LIMIT,					//��������ս���ſɲ���
		EN_CROSS_1V1_REWARD_SCORE_LIMIT,				//���ֲ���
		EN_CROSS_1V1_REWARD_FETCHED,					//������ȡ���ý���
		EN_1V1_NOT_MATCH,								//��û��ƥ�����
		EN_1V1_MATCHING_CANNOT_ENTER_FB,				//�����ƥ���У����ܽ��븱��
		EN_1V1_CANNOT_MATCH_STATUS,						//���ⳡ�������⡢ս��״̬�²��ܽ���ƥ��
		EN_1V1_MATCHING_CANNOT_HUSONG,					//�����ƥ���У����ܻ�����Ů
		EN_1V1_MATCHING_CANNOT_DAOQULINGSHI,			//�����ƥ���У����ܶ�ȡ��ʯ
		EN_1V1_DAILYTIMES_BUY_TIMES_MAX,				//���չ������������
		EN_1VN_NOT_SIGN_PHASE,							//���ڲ��Ǳ����׶�
		EN_1VN_ALREADY_BET,								//ÿ��ֻ��֧��һ��ѡ��
		EN_1VN_BET_TOO_MUCH,							//��֧�ֵ�����̫����
		EN_1VN_BET_TOO_LATE,							//�������֧�ֵ�ʱ��
		EN_1VN_NOT_SIGN,								//��û�б�����������
		EN_WUSHUANG_FUMO_ITEM_INVALID,					//�����Ʒ�������ڸ�ħ
		EN_WUSHUANG_FUMO_HAS_FUMO,						//���λ���Ѿ���ħ
		EN_WUSHUANG_FUMO_HAS_NOT_FUMO,					//���λ��û�и�ħ
		EN_WUSHUANG_FUMO_HAS_SAME_TYPE_FUMO,			//�Ѿ���ͬ���͵ĸ�ħ
		EN_MULTIUSER_CHALLENGE_MEMBER_NOT_IN_SAME_SCENE,//�г�Ա����ͬһ������
		EN_MULTIUSER_CHALLENGE_MEMBER_CAN_NOT_START_CROSS, //�г�Ա���ܽ��п��
		EN_MULTIUSER_CHALLENGE_MEMBER_LEVEL_LIMIT,		//�г�Ա�ȼ�����
		EN_MULTIUSER_CHALLENGE_MEMBER_CAPABILITY_LIMIT, //�г�Աս��������
		EN_MULTIUSER_CHALLENGE_MEMBER_IS_NOT_TEAM_LEADER, //���״ֻ̬�жӳ���������ƥ��
		EN_MULTIUSER_CHALLENGE_MEMBER_NO_DAYCOUNT,		//�г�Ա�޲�������
		EN_MULTIUSER_CHALLENGE_MEMBER_IN_MATCH,			//�����г�Ա����ƥ����ȴʱ��
		EN_MULTIUSER_CHALLENGE_MEMBER_IN_MATCHING,		//�г�Ա����ƥ����
		EN_MULTIUSER_CHALLENGE_MEMBER_END_NOT_GATHER,	//���������ѽ��������ܲɼ�
		EN_MULTIUSER_CHALLENGE_WAER_CARD_LIMIT,			//���ֻ������ĸ�����
		EN_MULTIUSER_CHALLENGE_MEMBER_IN_FB,			//�г�Ա�ڸ����У����ܽ���ƥ��
		EN_MULTIUSER_CHALLENGE_SELF_IN_MATCH,			//�ϴβμӵ�ս����δ������ƥ����ȴ��
		EN_TIANGONG_SHILIAN_MAX_LEVEL,					//����ͨ���칬������߲�
		EN_WUSHUANG_NOT_ACTIVE,							//��װ����δ����
		EN_NV_WA_SHI_NOT_ENOUGH,						//Ů�ʯ����
		EN_MOJING_NOT_ENOUGH,							//ħ������
		EN_GONGXUN_NOT_ENOUGH,							//��ѫ����
		EN_WEIWANG_NOT_ENOUGH,							//��������
		EN_TREASURE_HUNT_CREDIT_NOT_ENOUGH,				//Ѱ�����ֲ���
		EN_JINGLING_CREDIT_NOT_ENOUGH,					//����Ѱ�����ֲ���
		EN_HAPPYTREE_GROW_NOT_ENOUGH,					//���ֹ����ɳ�ֵ����
		EN_CROSS_HONOR_NOT_ENOUGH,						//�����������
		EN_USE_ITEM_SCENE_LIMIT,						//�ó�������ʹ�ø���Ʒ
		EN_HAS_BUFF_MORE_LEVEL,							//���ϴ��ڸ��߼���BUFF
		EN_TASK_MAX_STAR,								//��ǰ����������ǵȼ�
		EN_TEAM_ROOM_LEADER_LIMIT,						//�����Ƕӳ����ܿ�������
		EN_MGXF_NOT_NEAR_DOOR,							//���ڴ��͵㸽��
		EN_MGXF_DOOR_NOT_EXIST,							//���͵㲻����
		EN_MGXF_LAYER_NOT_EXIST,						//���͵����ӵĳ���������
		EN_SPACE_REMOVE_RECORD_SUCC,					//��Ϣɾ���ɹ���
		EN_TODAY_U_CAI_TARGET_LIMIT,					//�������Ѳȹ��Է�����
		EN_ACTIVEDEGREE_NOT_ENOUGH,						//��Ծ�Ȳ��㣬������ȡ������
		EN_ACTIVEDEGREE_TARGET_LIMIT,					//δ�ﵽ��ȡ����
		EN_ACTIVEDEGREE_HAVE_FETCH_LIMIT,				//���Ѿ���ȡ���û�Ծ����
		EN_ENTER_DABAO_MAP_COUNT_LIMIT,					//��װBoss��ͼ���������������
		EN_LIFESKILL_MAX_SKLL_LEVEL,					//�Ѵﵽ����ܵȼ�
		EN_LIFESKILL_HUOLI_NOT_ENOUGH,					//��������
		EN_LIFESKILL_LEVEL_LIMIT,						//���ܵȼ����㣬�޷�����
		EN_LIFESKILL_MAKE_FAIL,							//���ź����˴�����ʧ�ܣ���������
		EN_UPDATE_SPACE_XINQING_SUCC,					//�޸�ǩ���ɹ���
		EN_CARD_NOT_ACTIVE,								//���ƻ�δ�����������
		EN_CARD_NOT_SAME_STUFF,							//���Ƕ�Ӧ�Ŀ�����Ƭ�����ܻ���
		EN_CARD_MAX_LEVEL,								//������������߼�
		EN_CARD_UPLEVEL_STUFF_NOT_ENOUGH,				//�����������ϲ���
		EN_JINGHUA_GATHER_DAYCOUNT_NOT_ENOUGH,			//���մ���������
		EN_JINGHUA_GATHER_NONE,							//�����Ѿ����꣬����´�ˢ��
		EN_JINGHUA_NOT_IN_ROLE,							//����ȥ�ɼ�����
		EN_JINGHUA_HUSONG_TASK_LIMIT,					//���л��������ܻ��;���

		EN_JINGHUA_ALREADY_HAVE,						//���Ѿ��о������Ի��ͣ������ٲɼ�
		EN_JINGHUA_GATHER_FLY_LIMIT,					//����ʱ���ܲɼ�
		EN_FORBID_MAILGOLD,								//Ԫ�����ܼ���
		EN_MIAL_LOGIN_TIME_LIMIT,						//����ʱ������10���Ӳ��ܷ��ʼ�
		EN_TRADE_LOGIN_TIME_LIMIT,						//����ʱ������10���Ӳ��ܽ��н���
		EN_BETRADE_LOGIN_TIME_LIMIT,					//�Է�����ʱ������10����,���ܽ��н���
		EN_PUBLICSALE_LOGIN_TIME_LIMIT,					//����ʱ������10����,���ܽ����г�����
		EN_PUBLICSALE_HIGGHEST_PRICE,					//���ɼ���10��Ԫ��
		EN_SHENZHUANG_MAX_LEVEL,						//��װ�Ѵﵽ���ȼ�
		EN_MYSTERIOUSSHOP_BUY_NUM_LIMIT,				//��ʣ�๺����������
		EN_MYSTERIOUSSHOP_NPC_DISSAPPEAR,				//������������ʧ������´�ˢ��
		EN_MYSTERIOUSSHOP_ITEM_NOTEXIST,				//��Ҫ�������Ʒ������
		EN_CHANGE_CAMP_LIMIT_TIME,						//���ϴ�תʦ��24Сʱ�ڲ�����ת
		EN_CHANGE_CAMP_LIMIT_DUOBAOSCENE,				//�ڶᱦ��ͼ�в���תʦ��
		EN_CAMPEQUIP_LIMIT_CAMP,						//��Ҫ����ʦ�Ų��ܴ���ʦ��װ��
		EN_CAMPNORMALDUOBAO_LIMIT_CAMP,					//����ʦ�ź���ܽ���ᱦ
		EN_CAMPDUOBAO_LIMIT_LEVEL,						//�ȼ����㣬���ܽ���
		EN_CAMPDUOBAO_COUNT_LIMIT,						//���մ���������
		EN_DIFF_WEEKDAY_TOTAL_CHONGZHI_HAS_FETCH,		//���Ѿ���ȡ���ý�����
		EN_DIFF_WEEKDAY_TOTAL_CHONGZHI_NOT_ENOUGH,		//ÿ���۳���δ�ﵽ��ȡ����
		EN_JINGLING_MAX_GRADE,							//������������߽�
		EN_JINGLING_IMG_NOT_ACTIVE,						//����δ����
		EN_JINGLING_EQUIP_MAX_STRENGTH,					//�Ѵﵽ��ߵȼ�
		EN_SHENZHUANG_JINJIE_MAX_LEVEL,					//��װ�����Ѵﵽ��ߵȼ�
		EN_SHENZHUANG_SHENZHU_MAX_LEVEL,				//�����Ѵﵽ��ߵȼ�
		EN_ROLE_UPLEVEL_FAIL,							//��������ʧ��
		EN_SHIMEN_HUNLIAN_MAX_LEVEL,					//�����Ѵﵽ��ߵȼ�
		EN_PHANTOM_MAX_LEVEL,							//�û��Ѵ����ȼ�
		EN_PHANTOM_IMG_NOT_ACTIVE,						//�û�����δ����
		EN_PHANTOM_CARD_NOT_ENOUGH,						//���ϲ���
		EN_JINGLING_SOUL_MAX_LEVEL,						//�����Ѵ���ߵȼ�
		EN_JINGLING_SOUL_NOT_ENOUGH,					//���ǲ��ϲ���
		EN_LEVEL_NOT_ENOUGH_TO_SEND_EMAIL,				//�ȼ�����199,���ܷ��ʼ�!
		EN_LEVEL_NOT_ENOUGH_TO_SIGNAL_CHAT,				//�ȼ�����199,���ܽ���˽��!
		EN_TUHAOJIN_LEVEL_LIMIT,						//�������Ѵ���߼�
		EN_TUHAOJIN_COLOR_NOT_ACTIVE,					//��������ɫδ����
		EN_BIG_CHATFACE_LEVEL_LIMIT,					//������Ѵ���߼�
		EN_SHENZHOU_WEAPON_SLOT_LEVEL_MAX,				//��ǰ�����Ƭ�Ѵﵽ���ȼ�
		EN_SHENZHOU_WEAPON_ELEMENT_LEVEL_MAX,			//��ǰ���֮���Ѵﵽ���ȼ�	
		EN_SHENZHOU_WEAPON_LINGSHU_LEVEL_MAX,			//��ǰ����Ѵﵽ���ȼ�
		EN_SHENZHOU_WEAPON_WOLF_NOT_INLAY_LIMIT,		//���ǻ��δ��Ƕ��ʯ����������
		EN_SHENZHOU_WEAPON_LEOPARD_NOT_INLAY_LIMIT,		//�ñ����δ��Ƕ��ʯ����������
		EN_SHENZHOU_WEAPON_BEAR_NOT_INLAY_LIMIT,		//���ܻ��δ��Ƕ��ʯ����������
		EN_SHENZHOU_WEAPON_LION_NOT_INLAY_LIMIT,		//��ʨ���δ��Ƕ��ʯ����������
		EN_SHENZHOU_WEAPON_TIGER_NOT_INLAY_LIMIT,		//�û����δ��Ƕ��ʯ����������
		EN_SHENZHOU_WEAPON_KYLIN_NOT_INLAY_LIMIT,		//��������δ��Ƕ��ʯ����������
		EN_SHENZHOU_WEAPON_PHOENIX_NOT_INLAY_LIMIT,		//�÷�˻��δ��Ƕ��ʯ����������
		EN_SHENZHOU_WEAPON_DRAGON_NOT_INLAY_LIMIT,		//�������δ��Ƕ��ʯ����������
		EN_SHENZHOU_WEAPON_WOLF_COLOR_LIMIT,			//�ǻ��ǿ���Ѵ�ƿ������Ƕ����Ʒ�ʻ�ʯ����ͻ��
		EN_SHENZHOU_WEAPON_LEOPARD_COLOR_LIMIT,			//�����ǿ���Ѵ�ƿ������Ƕ����Ʒ�ʻ�ʯ����ͻ��
		EN_SHENZHOU_WEAPON_BEAR_COLOR_LIMIT,			//�ܻ��ǿ���Ѵ�ƿ������Ƕ����Ʒ�ʻ�ʯ����ͻ��
		EN_SHENZHOU_WEAPON_LION_COLOR_LIMIT,			//ʨ���ǿ���Ѵ�ƿ������Ƕ����Ʒ�ʻ�ʯ����ͻ��
		EN_SHENZHOU_WEAPON_TIGER_COLOR_LIMIT,			//�����ǿ���Ѵ�ƿ������Ƕ����Ʒ�ʻ�ʯ����ͻ��
		EN_SHENZHOU_WEAPON_KYLIN_COLOR_LIMIT,			//������ǿ���Ѵ�ƿ������Ƕ����Ʒ�ʻ�ʯ����ͻ��
		EN_SHENZHOU_WEAPON_PHOENIX_COLOR_LIMIT,			//��˻��ǿ���Ѵ�ƿ������Ƕ����Ʒ�ʻ�ʯ����ͻ��
		EN_SHENZHOU_WEAPON_DRAGON_COLOR_LIMIT,			//�����ǿ���Ѵ�ƿ������Ƕ����Ʒ�ʻ�ʯ����ͻ��
		EN_SHENZHOU_WEAPON_HUNYIN_TYPE_LIMIT,			//�û�ʯ�޷���Ƕ�ڵ�ǰ���	
		EN_SHENZHOU_WEAPON_LINGSHU_EXP_NOT_ENOUGH,		//����ֵ����		
		EN_SHENZHOU_WEAPON_HUNQI_LEVEL_LIMIT,			//���ȼ�����	
		EN_SHENZHOU_WEAPON_JINGHUA_NOT_ENOUGH,			//��𾫻�����	
		EN_SHENZHOU_WEAPON_IDENTIFY_LEVEL_ALEADY_MAX,	//�Ѵﵽ����������ȼ�			
		EN_SHENZHOU_WEAPON_EXCHANGE_TIME_MAX,			//����һ��Ѿ�����	
		EN_SHENZHOU_WEAPON_TODAY_GATHER_TIME_MAX,		//����ɼ������Ѵ�����		
		EN_SHENZHOU_WEAPON_BUY_TODAY_GATHER_TIME_LIMIT,	//���칺������Ѵ�����			
		EN_SHENZHOU_WEAPON_CANT_HELP_SELF_BOX,			//����Э���Լ�����	
		EN_SHENZHOU_WEAPON_CANT_BOUBLE_HELP_SELF_BOX,	//�����ظ�Э��			
		EN_SHENZHOU_WEAPON_OTHER_BOX_YET_OPEN,			//�Է������Ѿ���	
		EN_SHENZHOU_WEAPON_HELP_BOX_LIMIT,				//Э�������Ѿ���������
		EN_SHENZHOU_WEAPON_XILIAN_OPEN_SLOT_LIMIT,		//��ǰ���Բ��ѿ���		
		EN_SHENZHOU_WEAPON_XILIAN_FREE_TIMES_LIMIT,		//ÿ����������������		
		EN_SHENZHOU_WEAPON_XILIAN_NOT_OPEN,				//��ǰ��������δ����
		EN_SHENZHOU_WEAPON_HUNYIN_OPEN_LIMIT_1,			//ǰһ�����δ������ܿ����˻��
		EN_SHENZHOU_WEAPON_HUNYIN_OPEN_LIMIT_2,			//���ȿ���ǰһ�����
		EN_BABY_BIRTH_REQ_SED_SUCCESS,					//�����������ͳɹ�
		EN_JINJIE_EQUIP_LEVEL_LIMIT,					//���׵ȼ�����
		EN_BABY_HAVE_TO_IN_SAME_TEAM,					//��������Ҫ�������
		EN_BABY_TEAM_MEMBER_NUM_INVALID,				//ֻ�ܷ����������������
		EN_BABY_CAN_NOT_LITTLE_THREE,					//ֻ�ܷ����������������
		EN_BABY_UGRADE_STUFF_NOT_ENOUGHT,				//���ײ��ϲ���
		EN_BABY_RENAME_STUFF_NOT_ENOUGHT,				//����������
		EN_BABY_NOT_ENOUGH_LEVEL,						//���б���δ�ﵽ4�ף������ܼ�������Ŷ��
		EN_BABY_COUNT_FULL,								//����������
		EN_BABY_OTHER_COUNT_FULL,						//���İ��µı�����������������������
		EN_BABY_GRADE_LIMIT,							//����ѧʶ����
		EN_BABY_LEVEL_LIMIT,							//�������ʵȼ�����
		EN_BABY_LEVEL_NOT_ENOUGH_OTHER,					//���İ��µȼ���δ��55��
		EN_BABY_LEVEL_NOT_ENOUGH,						//���ĵȼ���δ�ﵽ55��
		EN_BABY_UN_ACCEPT,								//���İ���ȡ���������
		EN_BABY_IS_OVERDUE,								//����ı�����������
		EN_BABY_LOVER_NOT_ONLINE,						//���²�����
		EN_BABY_COUNT_NOT_FULL,							//������δ��������Ҫ����
		EN_BABY_CHAOSHENG_LIMIT,						//����������������
		EN_BABY_SPIRIT_LEVEL_FULL,						//�����ػ���������
		EN_BABY_SPIRIT_CONSUME_NOT_ENOUGH,				//�����ػ�����������Ʒ����
		EN_BABY_CAN_NOT_REMOVE_BABY_WHICH_AND_YOUR_LOVER,//���ܶ����뵱ǰ�������ı���
		EN_SHIZHUANG_GRADE_LIMIT,						//ʱװ�����Ѵ����ȼ�
		EN_PET_HAS_NOT_ACTIVE,							//����δ����
		EN_PET_HAS_ACTIVE,								//�����Ѿ�����
		EN_PET_EGG_NOT_ENOUGH,							//��������ٻ�������
		EN_PET_BACKPACK_FULL,							//������������Ѿ���������
		EN_PET_LEVEL_LIMIT,								//�����Ѵ����ȼ�
		EN_PET_GRADE_LIMIT,								//�����Ѿ�������
		EN_PET_NULL_REWARD,								//ʲô��û�г鵽
		EN_PET_SKILL_ALEADY_LEARN,						//�ü�����ѧϰ��
		EN_PET_SKILL_HAS_NOT_LEARN,						//�ü���δ����
		EN_PET_CUR_PET_CANNOT_LEARN_THIS_SKILL,			//�ü��ܲ�ר���ڵ�ǰ��ս����
		EN_PET_CUR_PET_THIS_SKILL_HAS_MAX_LEVEL,		//�ü����Ѿ�����߼�
		EN_PET_CUR_PET_THIS_SKILL_NOT_MATCH_SLOT,		//�ü������͸����ܲ۲�ƥ��
		EN_PET_CUR_PET_THIS_SKILL_ARE_IN_USED,			//�ü����Ѿ�װ����
		EN_PET_CUR_PET_HAS_NO_THIS_SKILL_SLOT,			//�ó���û��������ܲ�
		EN_PET_SPECIAL_IMG_ALREADY_ACTIVE,			    //�������������Ѿ�����
		EN_PET_SPECIAL_IMG_NOT_ACTIVE,				    //������������δ����
		EN_PET_SPECIAL_IMG_UN_USE_SUCC,					//ȡ���û��ɹ�
		EN_RA_MONSTER_DROP_ITEM_USE_TIMES_LIMIT,		//���ո���Ʒʹ�ô����ѵ�������
		EN_RA_MONSTER_DROP_ITEM_USE_LIMIT,				//����Ʒֻ���ڻ�ڼ�ʹ��
		EN_MOUNT_SPECIAL_IMG_GRADE_LIMIT,				//�������������Ѵ���߼�
		EN_RA_MINE_EXCHANGE_REWARD_SEND_BY_MAIL,		//�������㣬�ڿ�һ�������ͨ���ʼ�����
		EN_RA_MINE_FETCH_SERVER_REWARD_VIP_LIMIT,		//��ȡȫ���ڿ���VIP�ȼ�����
		EN_RA_MINE_EXCHANGE_COUNT_NOT_ENOUGH,			//���ﲻ�㣬�޷��һ�
		EN_RA_MINE_FETCH_SERVER_REWARD_LEVEL_LIMIT,		//��ȡȫ�������ȼ�����
		EN_HUASHEN_ID_HAVE_ACTIVE,						//���������Ѽ���
		EN_HUASHEN_ID_NOT_ACTIVE,						//��������δ����
		EN_HUASHEN_LEVEL_FULL,							//����ȼ�����
		EN_HUASHEN_LEVEL_STUFF_NOT_ENOUGH,				//�����������ϲ���
		EN_HUASHEN_SPIRIT_LEVEL_FULL,					//�������������
		EN_HUASHEN_GRADE_FULL,							//��������Ѵ����
		EN_RA_COUPLE_HALO_ACTIVATE_STUFF_NOT_ENOUGH,	//���޹⻷������ϲ���
		EN_WING_SPECIAL_IMG_GRADE_LIMIT,				//������������������
		EN_PET_QINMI_STUFF_ID_NOT_ENOUGH,				//�ף�û���㹻�ķ۳�������������
		EN_PET_QINMI_LEVEL_FULL,						//�������϶��Ѿ�����
		EN_RA_FANFAN_WORD_REWARD_FULL,					//���ȶһ����齱��
		EN_RA_FANFAN_CARD_LIST_FULL,					//������������
		EN_RA_FANFAN_WORD_NOT_ACTIVE,					//���黹û�б�����
		EN_CONTINUE_CHONGZHI_REWARD_NOT_ACTIVE,			//����δ�����������ȡ
		EN_CONTINUE_CHONGZHI_REWARD_HAS_FETCH,			//��������ȡ��
		EN_CONTINUE_CONSUME_REWARD_NOT_ACTIVE,			//����δ�����������ȡ
		EN_CONTINUE_CONSUME_NOT_REACH_NEED_CONSUME_GOLD,//��Ŀǰ����Ԫ�����㣬��������ȡ�ý���
		EN_CONTINUE_CONSUME_NOT_CONSUME_ANY_GOLD,		//������û�������κ�Ԫ������������ȡ�ý���
		EN_ARMY_DAY_EXCHANGE_LIMIT,						//�Ѿ��þ���һ�����
		EN_ARMY_DAY_ACTIVITY_DEGREE_NOT_ENOUGH,			//��Ծ�Ȳ���
		EN_ARMY_DAY_FLAG_NOT_ENOUGH,					//���첻��
		EN_PASTURE_SPIRIT_LEVEL_FULL,					//��������ȼ�����
		EN_PASTURE_SPIRIT_QUALITY_FULL,					//��������Ʒ������
		EN_PASTURE_SPIRIT_NOT_ACTIVE,					//�������ﻹδ����
		EN_PASTURE_SPIRIT_POINT_NOT_ENOUGH,				//���л��ֲ���
		EN_CROSS_PASTURE_CD_NOT_ENOUGH,					//��ȴʱ��δ��
		EN_CROSS_PASTURE_DISTANCE_NOT_ENOUGH,			//����̫Զ
		EN_CROSS_PASTURE_GET_SCORE_TIMES_FULL,			//�����������޷���û���
		EN_OTHER_XUNZHANG_STUFF_NOT_ENOUGH,				//ѫ�������󶨲��ϲ���
		EN_SHEN_ZHOU_HELP_BOX_FAIL,						//�Է��ı���Э����������
		EN_SHEN_ZHOU_INVITE_HELP_BOX_FAIL,				//û�п���Э��������
		EN_SHEN_ZHOU_INVITE_HELP_BOX_SUCC,				//����Э���ɹ�

		EN_MULTI_MOUNT_NOT_ACTIVE,						//������δ����
		EN_MULTI_MOUNT_IMAGE_NOT_ACTIVE,				//����������δ����
		EN_MULTI_MOUNT_ALEADY_MAX_GRADE,				//�������Ѵﵽ��߽�
		EN_MULTI_MOUNT_IS_NOT_RIDING,					//����δ��˫������
		EN_MULTI_MOUNT_ALEADY_TOW_RIDE,					//���Ѿ�����˫�����״̬
		EN_MULTI_MOUNT_OWNER_CHANGE_INVITE,				//�Է��Ѿ���������������
		EN_MULTI_MOUNT_OWNER_NOT_RIDING,				//�Է���������
		EN_MULTI_MOUNT_OWNER_NOT_INVITE,				//�Է�û�з�������
		EN_MULTI_MOUNT_OWNER_MOUNT_IS_FULL,				//�Է�������˰���
		EN_MULTI_MOUNT_PARTNER_TOO_FARAWAY,				//�Է��������̫Զ
		EN_MULTI_MOUNT_NORMAL_SCENE_CAN_INVITE,			//��ͨ�����ſ�������˫�����
		EN_MULTI_MOUNT_CANNOT_FLY,						//˫�����ﲻ�ܷ���Ŷ
		EN_MULTI_MOUNT_MY_MOUNT_IS_FULL,				//��������˰���
		EN_MULTI_MOUNT_GRADE_IS_FULL,					//�������Ѿ�����߽�
		EN_MULTI_MOUNT_ID_INVALID,						//����ID�Ƿ�
		EN_MULTI_MOUNT_OTHER_CANNOT_MOUNT_ON,			//�Է�����������
		EN_MULTI_MOUNT_SPECIAL_IMG_ALREADY_ACTIVE,	    //˫���������������Ѿ�����
		EN_MULTI_MOUNT_SPECIAL_IMG_NOT_ACTIVE,		    //˫��������������δ����
		EN_MULTI_MOUNT_EQUIP_ALEADY_MAX_LEVEL,			//������װ���Ѵﵽ��ߵȼ�
		EN_CIRCULATION_CHONGZHI_REWARD_NOT_ACTIVE,		//����δ�����������ȡ
		EN_CIRCULATION_CHONGZHI_NOT_CHONGZHI_ANY_GOLD,	//������û�г�ֵ�κ�Ԫ������������ȡ�ý���
		EN_CIRCULATION_CHONGZHI_NOT_REACH_NEED_CHONGZHI_GOLD,//��Ŀǰ��ֵԪ�����㣬��������ȡ�ý���
		EN_PERSONALIZE_BUBBLE_WINDOW_LEVEL_LIMIT,		//���ݿ��Ѵ����ȼ�
		EN_PERSONALIZE_BUBBLE_WINDOW_NOT_ACTIVE,		//���ݿ�δ����
		EN_GUILD_DAY_INVITE_COUNT_LIMIT,				//ÿ�������������������
		EN_MAGIC_CARD_SLOT_ALEADY_HAS_CARD,				//���Ѿ�����ÿ���
		EN_MAGIC_CARD_SPRITE_NOT_ENOUGH,				//ħ�겻��
		EN_MAGIC_CARD_NOT_ENOUGH,						//ħ������
		EN_MAGIC_CARD_STRENGTH_LEVEL_FULL,				//��ħ��������
		EN_MAIGC_CARD_STRENGTH_LEVEL_NO_ENOUGH,			//��ħ�������ȼ������
		EN_CHESTSHOP_NOT_ENOUGH_SPACE,					//Ѱ���ֿ�����
		EN_WUSHANG_EQUIP_NOT_ENOUGH,					//��Ʒ����
		EN_WUSHANG_EQUIP_NOT_HAVE_EQUIP,				//��λ��û��װ��
		EN_WUSHANG_EQUIP_ALREADY_PUTON,					//��װ������Ʒ
		EN_WUSHANG_EQUIP_JIFEN_NOT_ENOUGH,				//���ֲ���
		EN_WUSHANG_EQUIP_NOT_ACTIVE,					//�������δ����
		EN_WUSHANG_EQUIP_STRONG_LEVEL_LIMIT,			//�������ǿ���ȼ�����
		EN_WUSHANG_EQUIP_STRONG_STUFF_NOT_ENOUGH,		//�������ǿ�����ϲ���
		EN_WUSHANG_EQUIP_STRONG_FAILD,					//�������ǿ��ʧ��
		EN_WUSHANG_EQUIP_STAR_LEVEL_LIMIT,				//��������Ǽ�����
		EN_WUSHANG_EQUIP_STAR_STUFF_NOT_ENOUGH,			//����������ǲ��ϲ���
		EN_WUSHANG_EQUIP_GLORY_NOT_ENOUGH,				//���������ҫ����
		EN_WUSHANG_EQUIP_JYGLORY_NOT_ENOUGH,			//���������Ӣ��ҫ����
		EN_JINGLING_HALO_LEVEL_LIMIT,					//����⻷�ȼ�����
		EN_CROSS_ACTIVITY_LEVEL_LIMIT,					//�ȼ����㣬���ܲ���ÿ��
		EN_CROSS_BOSS_GATHER_TIME_LIMIT,				//�����Ѿ�û�вɼ�����
		EN_CROSSBOSS_GATHER_REDUCE_HP_NOTICE,			//�ɼ�ʱ��ٷֱȿ۳���ǰ����ֵ
		EN_SOMEONE_GATHER_ING,							//�òɼ������ڱ������˲ɼ���
		EN_CROSS_HS_LEVEL_LIMIT,						//�ȼ����㣬���μ���Ȫ�
		EN_CROSS_HS_CANCEL_PARTNER,						//�Է�ȡ�����Ļ�
		EN_CROSS_HS_FREE_PRESETN_TIME_LIMIT,			//����������������
		EN_CROSS_HS_HAS_PARTNER,						//�����Ļ���
		EN_CROSS_HS_NOT_IN_SCENES,						//�Է����ڵ�ǰ����
		EN_CROSS_HS_TARGET_HAS_PARTNER,					//����������Ļ���
		EN_CROSS_HS_REJECT_PARTNER_INVITE,				//�Է��ܾ����������
		EN_CROSS_HS_SKILL_TIME_LIMIT,					//����ʹ�ô���������
		EN_CROSS_HS_SHUANGXIU_CANNOT_MASSAGE,			//�Ļ��в���ʹ�ð�Ħ����
		EN_CROSS_HS_TARGET_SHUANGXIU_CANNOT_MASSAGE,	//����������Ļ���
		EN_WING_FUMO_LEVEL_FULL,						//��ħ�ȼ�����
		EN_WING_FUMO_FAIL,								//����ħʧ��
		EN_MITAMA_LEVEL_FULL,							//�����̵ȼ�����
		EN_MITAMA_JINGHUN_LEVEL_FULL,					//�þ���ȼ�����
		EN_MITAMA_TASK_LEVEL_LIMIT,						//��ǰ���̵ȼ����㣬����ִ������
		EN_MITAMA_EXCHANGE_SCORE_NOT_ENOUGH,			//��ǰ��Ȫ���ֲ��㣬���ܶһ���Ʒ
		EN_MITAMA_TASK_CANT_FIGHTING,					//���ڻ����ܳ�������
		EN_HOT_SPRING_GAME_TIME_MAX,					//��λ������Ϸ�����ѵ�����
		EN_HOT_SPRING_GAME_ALEADY_START,				//��Ϸ�Ѿ����ڽ���
		EN_HOT_SPRING_GAME_ALEADY_STOP,					//��Ϸ�Ѿ�����
		EN_FAIRY_TREE_MONEY_CANT_FETCH,					//��Ǹ������ʱ�䲻������ȡ��������
		EN_FAIRY_TREE_GIFT_CANT_FETCH,					//��Ǹ������ʱ�䲻������ȡ�������
		EN_FAIRY_TREE_FUNNY_FARM_SCORE_NOT_ENOUGH,		//��Ǹ������ũ�����ֲ���
		EN_FAIRY_TREE_LEVEL_FULL,						//�����ȼ�����
		EN_FAIRY_TREE_GRADE_FULL,						//������������
		EN_MAGIC_EQUIPMENT_HAS_ACTIVE,					//��ħ���Ѿ������
		EN_MAGIC_EQUIPMENT_HAS_NOT_ACTIVE,				//��ħ��δ����
		EN_MAGIC_EQUIPMENT_ALEADY_MAX_LEVEL,			//��ħ���Ѿ��ﵽ���ȼ�
		EN_MAGIC_EQUIPMENT_ALEADY_HAS_STONE,			//�ÿ�λ����ħʯ
		EN_CROSS_TEAM_FB_NOT_READY,						//�ж�Ա��δ׼��
		EN_CROSS_TEAM_FB_HAS_START,						//�÷����ѿ�ʼ����
		EN_MAGIC_EQUIPMENT_SLOT_HAS_NO_STONE,			//�ÿ�λ�ǿյ�
		EN_RA_BLACK_MARKET_CANT_OFFER_ITEM,				//��Ǹ������ʧ����
		EN_RA_BLACK_MARKET_OFFER_GOLD_PRICE_LIMIT,		//���ĳ��۵��ڵ�ǰ���ļ�
		EN_QINGYUAN_HALO_LEVEL_LIMIT,					//�⻷�Ѵ����ȼ�
		EN_QINGYUAN_PRE_HALO_LEVEL_LIMIT,				//ǰ�ù⻷�ȼ�����
		EN_RA_ZHENBAOSHANGCHENG_SCORE_NOT_ENOUGH,		//�䱦�̳ǻ��ֲ���
		EN_XIANNV_NOT_ACTIVE,							//����Ůδ����
		EN_XIANNV_LEVEL_MAX,							//��Ů�ȼ��Ѵ����
		EN_XIANNV_ZIZHI_MAX,							//��Ů�����Ѵ����
		EN_XIANNV_HUANHUAN_LEVEL_MAX,					//��Ů�û��ȼ��Ѵ����
		EN_XIANNV_ALREADY_ACTIVE,						//�Ѿ����������
		EN_ZHIBAO_HUANHUA_GARDE_MAX,					//�û��Ѵ�������
		EN_ZHIBAO_ALREADY_ACTIVE,						//�Ѿ����������
		EN_WANGLINGEXPLORE_ENTER_LEVEL_LIMIT,			//�ȼ����㲻�ܽ���
		EN_ZHIBAO_LEVEL_TOO_LARGE,						//ʥ��ȼ�����
		EN_MILLIONAIRE_HAD_TURN,						//ת������������
		EN_OPEN_GAME_ACTIVITY_FETCHED,					//������ȡ���ý���
		EN_OPEN_GAME_ACTIVITY_COND_LIMIT,				//δ�ﵽ��ȡ����
		EN_CHONGZHI_COND_LIMIT,							//δ�ﵽ��ȡ����
		EN_CHONGZHI_HAS_FETCH,							//���Ѿ���ȡ���ý�����
		EN_CHAT_LEVEL_LIMIT,							//��Ʒʹ�õȼ�����
		EN_TW_WAR_IS_FINISH,							//��ս�ѽ���
		EN_TW_WAR_CREDIT_NO_ENOUGHT,					//���ֲ���
		EN_EXP_REFINE_TODAY_TIME_MAX,					//���������Ѵ�������

		EN_CROSS_TEAM_LEVEL_LIMIT,						//�ȼ����㣬���ܲ���ÿ��
		EN_CROSS_TEAM_COUNT_LIMIT,						//���տ����Ӵ����Ѵ����
		EN_CROSS_TEAM_HUSONG_LIMIT,						//�����в��ܽ��п����Ӳ���

		EN_TW_WAR_TOWER_WUDI_CLOD_LIMIT,				//��Ʒ��ȴ�У����ܹ���
		EN_TW_WAR_TRANSPORT_NOT_TARGET,					//�޿ɴ���Ŀ��
		EN_TW_WAR_TRANSPORT_CENTER_LIMIT,				//δռ�����븴��㣬���ܴ���

		EN_MOVE_CHESS_ARRIVE_END_POINT,					//�ѵ����յ�
		EN_MOVE_CHESS_RESET_TIME_LIMIT,					//�Ѵ�������ô���

		EN_CROSS_GUILDBATTLE_REWARD_HAD_FETCHED,		//�ý�������ȡ
		EN_CROSS_GUILDBATTLE_REWARD_FETCH_LIMIT,		//��ȡ��������
		EN_CROSS_GUILDBATTLE_GUILD_LIMIT,				//���������˲��ܲ���û
		EN_CROSS_GUILDBATTLE_GATHER_LIMIT,				//����������Ѿ��ɼ�����
		EN_EXP_JADE_FETCH_TIME_LIMIT,					//��������ȡ��������

		EN_WORLD_BOSS_REWARD_IS_GIVE,					//�����ѱ�ȡ��
		EN_CROSS_XIULUO_TOWER_SCORE_NOT_ENOUGH,			//���ֲ���
		EN_CROSS_XIULUO_TOWER_SCORE_FETCHED,			//������ȡ���ý���
		EN_CROSS_SHUIJING_GATHER_TIMES_LIMIT,			//����Ĳɼ�����������
		EN_CROSS_XIULUO_TOWER_REALIVE_TIME_LIMIT,		//����ʱ��δ��
		EN_SHUIJING_GATHER_TIMES_LIMIT,					//����Ĳɼ�����������

		EN_CHONGZHI_7DAY_TODAY_FETCH,					//��������ȡ����
		EN_CHALLENGE_FIELD_BUFF_TIMES_LIMIT,			//�Ѵﵽ��������
		EN_FETCH_CONDITION_NOENOUGH,                    //��������ȡ����       

		//////////////////////////////////////  ����ϵͳ    ////////////////////////////////////
		EN_RUNE_SYSTEM_BAG_NO_ENOUGH_SPACE,				//ս�걳������
		EN_RUNE_SYSTEM_MAX_LEVEL,						//ս����ߵȼ�
		EN_RUNE_SYSTEM_NOR_ENOUGH_SCORE,				//ս�꾫������
		EN_RUNE_SYSTEM_AWAKEN_LIMIT,					//ս����Ѵ��������
		EN_RUNE_SYSTEM_COMPOSE_FAILE,					//�ϳ�ʧ��
		EN_RUNE_SYSTEM_MAX_COMPOSE_QUALITY,				//��ս�겻�ܺϳ�
		EN_RUNE_SYSTEM_MAX_QUALITY,						//ս�����Ʒ��
		EN_RUNE_SYSTEM_CAN_NOR_COMPOSE_DIFF_QUALITY,	//��ͬƷ��ս�겻�ܺϳ�
		EN_RUNE_SYSTEM_CAN_NOR_COMPOSE_DIFF_KIND,		//��ͬ����ս�겻�ܺϳ�
		EN_RUNE_SYSTEM_CAN_NOR_DECOMPOSE,				//��ս�겻�ֽܷ�
		EN_RUNE_SYSTEM_KIND_MAX_COUNT,					//������ս���Ѵ����װ������
		EN_RUNE_SYSTEM_SLOT_NOT_OPEN,					//��ս���û�п���
		EN_RUNE_SYSTEM_ATTR_IS_SAME,					//����װ��������ͬ��ս��
		EN_RUNE_SYSTEM_TYPE_IS_SAME,					//����װ��������ͬ��ս��
		EN_RUNE_SYSTEM_SUIPIAN_NOT_ENOUGH,				//ս����Ƭ����
		EN_RUNE_SYSTEM_ITEM_NOT_ENOUGH,					//��Ʒ����
		EN_RUNE_SYSTEM_MAGIC_CRYSTAL_ENOUGH,			//ս��ˮ����������
		EN_RUNE_SYSTEM_RUNE_NOT_OPEN,					//��ս�껹δ����
		EN_RUNE_SYSTEM_RUNE_JINGHUA_NOT_PUTON,			//ս�꾫������װ��
		EN_RUNE_SYSTEM_RUNE_COMPOSE_NUM_LESS,			//����Ҫ������ս��
		EN_RUNE_TOWER_OFFLINE_TIME_MAX,					//����ʱ���Ѵ����
		EN_RUNE_TOWER_OFFLINE_TIME_FETCHED,				//��������ȡ����ʱ��
		EN_RUNE_TOWER_LAYER_LIMIT,						//ս������ͨ����߲�

		EN_XINGZUOYIJI_GATHER_LIMIT,					//�Ѵﵽ���ɼ���
		EN_NO_JOIN_TIMES,                               //û�в������


		//////////////////////////////////////  ��Ů�ػ������﷨�󡢳���⻷    ////////////////////////////////////

		EN_XIANNV_SHOUHU_NOT_EXIST,						//��Ů�ػ�������

		EN_JINGLING_FAZHEN_NOT_EXIST,					//���﷨�󲻴���
		EN_JINGLING_FAZHEN_CANNOT_UPGRADE,				//���﷨����������������

		EN_JINGLING_GUANGHUAN_NOT_EXIST,				//����⻷������
		EN_JINGLING_GUANGHUAN_CANNOT_UPGRADE,			//����⻷��������������
		EN_JINGLING_ADVANTAGE_TIMES_LIMIT,				//���տɷ������ﵰ�����Ѵ�����
		EN_JINGLING_ADVANTAGE_LEVEL_LIMIT,				//�ȼ����㣬120���������������ﵰ��

		//////////////////////////////////////  ת��ϵͳ    ////////////////////////////////////
		EN_ZHUANSHENG_BACKPACK_FULL,					//ת�������ռ䲻��
		EN_ZHUANSHENG_LEVEL_FULL,						//ת���ȼ��Ѿ�����
		EN_ZHUANSHENG_XIUWEI_NOT_ENOUGH,				//��Ϊ����
		EN_ZHUANSHENG_CHANGE_TIMES_FULL,				//����Ķһ���������
		EN_ZHUANSHENG_NOT_HAVE_EQUIP,					//��λ��û��װ��
		EN_ZHUANSHENG_LEVEL_NOT_ENOUGH,					//ת���ȼ�����
		EN_ZHUANSHENG_EQUIP_IS_BIND,					//ת��װ���Ѿ���
		EN_ZHUANSHENG_IS_XIUWEI_DAN,					//�����ת����Ϊ��
		EN_ZHUANSHENG_KNAPSACK_NOT_ENOUGH, 				//���ﱳ���ռ䲻��
		EN_ZHUANSHENG_AUTO_CONBIME_EQUIP_NOT_ENOUGTH,	//��Ʒ����

		//////////////////////////////////////  С����    ////////////////////////////////////
		EN_LITTLE_PET_NO_PET_INTERACT_TIMES,			//С���ﻥ����������
		EN_LITTLE_PET_NO_PERSONAL_INTERACT_TIMES,		//���˻�����������
		EN_LITTLE_PET_NO_ENOUGH_QIANGHUA_DAN,			//С����ǿ��������
		EN_LITTLE_PET_MAX_FEED_DEGREE,					//С���ﱥʳ������
		EN_LITTLE_PET_MAX_NUM,							//С������������
		EN_LITTLE_PET_NO_ENOUGH_SCORE,					//С��������ֵ����
		EN_LITTLE_PET_CAN_NOT_INTERACT_SEFL_PET,		//���ܺ��Լ���С���ﻥ��


		//////////////////////////////////////  ħ��    ////////////////////////////////////
		EN_MOJIE_NOT_FINISH_PRETASK,					//û�����ǰ������
		EN_MOJIE_TASK_BEEN_FINISH,						//�������Ѿ�ͨ��

		//////////////////////////////////////  ���    ////////////////////////////////////////
		EN_SHENBING_LEVEL_TOO_LARGE,					//����ȼ�����
		EN_SHENBING_SHUXINGDAN_USE_LIMIT_NUM,			//������Ե��Ѵ�����

		//////////////////////////////////////  ���ѻ���    ////////////////////////////////////
		EN_FRIEND_GIFT_NOT_TIMES,						//����Դ˺��ѵ���������Ѿ�����
		EN_FRIEND_GIFT_SONG_SUCCESS,					//����ɹ�
		EN_FRIEND_GIFT_RETURN_SUCCESS,					//����ɹ�

		//////////////////////////////////////  װ���ϳ�    ////////////////////////////////////
		EN_EQUI_COMPOUND_LEVEL_LIMIT,					//�ȼ����㣬���ܺϳ�
		EN_EQUI_COMPOUND_STUFF_ERROR,					//����Ʒ���ܺϳ�

		//////////////////////////////////////  ��������   ////////////////////////////////////
		EN_TACK_FINISH_YET_ERROR,						//�Ѿ���ɴ�����
		EN_GOAL_FINISH_YET_ERROR,						//�Ѿ���ɴ�Ŀ��

		//////////////////////////////////////  ��ʱvip   ////////////////////////////////////
		EN_TIME_LIMIT_NOT_FETCH_REWARD,					//��ʱvip��������ȡ����

		//////////////////////////////////////  ���鸱��  ////////////////////////////////////
		EN_EXP_FB_GUWU_LIMIT,							//�����Ѿ���������
		EN_EXP_FB_GUWU_GOLD_NOT_ENOUGH,					//�Զ�����Ԫ������
		EN_FB_ENTER_FB_CONDITMONS_ERROR,				//û��������븱��������
		EN_FB_ENTER_FB_IN_TEAM,							//��ǰ�ڶ�����
		EN_FB_ENTER_FB_NOT_LEADER,						//�㲻�Ƕӳ�
		EN_FB_ENTER_FB_PLEAR_TEAM,						//�����ĺ���һ����ӽ���
		EN_FB_ENTER_FB_NOT_TIMES,						//�������㣬�޷����뾭�鸱��
		EN_FB_ENTER_FB_TEAM_NUM_INSU,					//������������
		EN_FB_EXP_TIME_LIMIT,							//������ȴʱ��δ�����޷����뾭�鸱��
		EN_FB_EXP_PAY_TIME_LIMIT,						//�㵱ǰVIP�ȼ����ɹ���Ĵ����Ѵ����ޣ�
		EN_FB_EXP_PAY_TIME_VIP_LIMIT,					//�㵱ǰVIP�ȼ����ɹ���Ĵ����Ѵ����ޣ�
		EN_FB_ROLE_LEVEL_LIMIT_EXP_FB,					//�ȼ����㣬�޷����뾭�鸱��
		EN_TASK_NOT_COMPLETED_EXP_FB,					//ǰ������δ��ɣ��޷����뾭�鸱��

		//////////////////////////////////////  ���װ������  ////////////////////////////////////
		EN_ENTER_TEAM_EQUIP_FB_TEAM_LIMIT,				//���ڶ����У����ܽ��븱��
		EN_ENTER_TEAM_EQUIP_FB_LEVEL_LIMIT,				//�ȼ����㣬���ܽ��븱��
		EN_ENTER_TEAM_EQUIP_FB_LEADER_LIMIT,			//�����Ƕӳ������ܴ�������
		EN_TEAM_EQUIP_FB_BUY_TIME_VIP_LIMIT,			//�㵱ǰVIP�ȼ����ɹ���Ĵ����Ѵ����ޣ�

		//////////////////////////////////////  ���װ������  ////////////////////////////////////
		EN_SHENGE_SYSTEM_BAG_NO_ENOUGH_SPACE,			//�ǻԱ�������
		EN_SHENGE_SYSTEM_MAX_LEVEL,						//�ǻ���ߵȼ�
		EN_SHENGE_SYSTEM_NOR_ENOUGH_SCORE,				//�ǻԾ�������
		EN_SHENGE_SYSTEM_COMPOSE_FAILE,					//�ϳ�ʧ��
		EN_SHENGE_SYSTEM_MAX_COMPOSE_QUALITY,			//���ǻԲ��ܺϳ�
		EN_SHENGE_SYSTEM_MAX_QUALITY,					//�ǻ����Ʒ��
		EN_SHENGE_SYSTEM_CAN_NOR_COMPOSE_DIFF_QUALITY,	//��ͬƷ���ǻԲ��ܺϳ�
		EN_SHENGE_SYSTEM_CAN_NOR_COMPOSE_DIFF_KIND,		//��ͬ�����ǻԲ��ܺϳ�
		EN_SHENGE_SYSTEM_CAN_NOR_DECOMPOSE,				//���ǻԲ��ֽܷ�
		EN_SHENGE_SYSTEM_KIND_MAX_COUNT,				//�������ǻ��Ѵ����װ������

		EN_TIAN_XIANG_BEAN_NUM_LIMIT,					//�����ǻ������ﵽ����
		EN_XIANNV_SHENGWU_EXP_HAS_NOT_FETCH,			//������ȡ���ϴεĽ���
		EN_XIANNV_SHENGWU_LEVEL_HAS_FULL,				//�Ѵﵽ��߼�
		EN_XIANNV_COUNT_NOT_ENOUGH,						//��Ů��������
		EN_XIANNV_SHENGWU_GRID_LINGYE_NOT_ENOUGH,		//��Һ����������
		EN_XIANNV_SHENGWU_GRID_DEPEND_LEVEL_LIMIT,		//��������ǰ�ýڵ�ĵȼ�
		EN_XIANNV_SHENGWU_ALL_FULL_LEVEL,				//����ʥ�ﶼ�Ѵﵽ��߼�
		EN_XIANNV_SHENGWU_ESSENCE_NOT_ENOUGH,			//������������
		EN_XIANNV_SHENGWU_DAY_FETCH_LING_TIMES_LIMIT,	//������������Ѿ���������
		EN_SHENGE_SYSTEM_ZHANGKONG_FULL_LEVEL,			//�ǻ��Ѿ�����


		////////////////////////////////////// ���Ѿ���ƿ ////////////////////////////////////
		EN_EXP_BOTTLE_NOT_ACTIVE,						//����ƿδ����
		EN_EXP_BOTTLE_NOT_FULL,							//����ƿδ��
		EN_EXP_BOTTLE_FRIEND_NOT_ENOUGH,				//������������

		//////////////////////////////////////  ��ͼ����  ////////////////////////////////////
		EN_TUITU_FB_UNLOCK_LAST_LEVEL_LIMIT,			//����ͨ����һ���ؿ�
		EN_TUITU_FB_FULL_STAR_PASS_FIRST,				//��������ͨ����һ�ؿ�
		EN_TUITU_FB_ENTER_TIMES_LIMIT,					//���븱����������
		EN_TUITU_FB_BUY_TIMES_NOT_ENOUGH,				//����ʣ�๺���������
		EN_TUITU_FB_CHAPTER_STAR_NUM_NOT_ENOUGH,		//�����½����������㣬�޷���ȡ
		EN_TUITU_FB_CANT_SAODANG,						//����ɨ������δ����
		EN_TUITU_FB_CANT_SAODANG_ENTER_TIMES_LIMIT,		//���븱�����������޷�ɨ��
		EN_TUITU_FB_ALL_COMPLETE,						//��ͨ�������½�

		EN_FIGHTING_MINING_QUALITY_FULL,				//�Ѿ������Ʒ����
		EN_FIGHTING_MINING_ALEADY,						//���Ĺ��˵�ǰ�����ڿ���
		EN_FIGHTING_MINING_TIMES_LIMIT,					//�������ڿ�����Ѵ�����
		EN_FIGHTING_MINING_BEEN_ROB_LIMIT,				//�Է��ܲ��ˣ�����������
		EN_FIGHTING_MINING_ROB_LIMIT,					//��������Ӷ�����Ѿ��ﵽ������
		EN_FIGHTING_MINING_FETCH_TIME_LIMIT,			//ʱ��δ�����޷���ȡ����
		EN_FIGHTING_MINING_REFLUSH_FAIL,				//ˢ��ʧ��
		EN_FIGHTING_MINING_ALEADY_ROB,					//�Ѿ��Ӷ���Է���
		EN_FIGHTING_MINING_TIME_END,					//�Է��Ŀ��Ѿ��������

		EN_FIGHTING_SAILING_QUALITY_FULL,				//�Ѿ������Ʒ����
		EN_FIGHTING_SAILING_ALEADY,						//���Ĺ��˵�ǰ���ں�����
		EN_FIGHTING_SAILING_TIMES_LIMIT,				//�����캽�������Ѵ�����
		EN_FIGHTING_SAILING_BEEN_ROB_LIMIT,				//�Է��ܲ��ˣ�����������
		EN_FIGHTING_SAILING_ROB_LIMIT,					//��������Ӷ�����Ѿ��ﵽ������
		EN_FIGHTING_SAILING_FETCH_TIME_LIMIT,			//ʱ��δ�����޷���ȡ����
		EN_FIGHTING_SAILING_REFLUSH_FAIL,				//ˢ��ʧ��
		EN_FIGHTING_SAILING_ALEADY_ROB,					//�Ѿ��Ӷ���Է���
		EN_FIGHTING_SAILING_TIME_END,					//�Է��Ĵ��Ѿ�����

		EN_FIGHTING_CHALLENGE_ALEADY_WIN,				//�Ѿ�սʤ���˶���
		EN_FIGHTING_CHALLENGE_LEFT_TIMES_LIMIT,			//�Ѿ�û����ս������
		EN_FIGHTING_VIP_BUY_LIMIT,						//�㵱ǰVIP�ȼ����ɹ���Ĵ����Ѵ����ޣ�

		//////////////////////////////////////  �䱦��    ///////////////////////////////////
		EN_ZHENBAOGE_HAD_REFRESH,						//����ˢ�³ɹ�
		EN_ZHENBAOGE_NOT_REFRESH,						//�ñ�����δˢ��

		//////////////////////////////////////	�����ٻ�	  ///////////////////////////////////
		EN_GOLDEN_PIG_EXIST,							//��boss�ѱ��ٻ�

		//////////////////////////////////////  ����Ͷ��   //////////////////////////////////
		EN_OPEN_SERVER_INVEST_REWARD,					//�ý����ѱ���ȡ
		EN_OPEN_SERVER_INVEST_LAST_REWARD,				//������ȡ��һ�㽱��
		EN_OPEN_SERVER_INVEST_UNFINISH,					//��δ���Ͷ��Ŀ��
		EN_OPEN_SERVER_INVEST_HAD_INVEST,				//�ѽ��и�Ͷ��
		EN_OPEN_SERVER_INVEST_NOT_INVEST,				//��δ����Ͷ��

		//////////////////////////////////////  �ۻ���   //////////////////////////////////
		EN_RA_XIANYUAN_TREAS_ALREADY_BUY,				//������Ѿ��������
		EN_RA_XIANYUAN_TREAS_PASS_BUY_DAY,				//�Ѿ������˿ɹ�������
		EN_RA_XIANYUAN_TREAS_HAVE_ALL_BUY,				//����Ѿ�ȫ��������
		EN_RA_XIANYUAN_TREAS_NO_BUY,					//�����û�й���
		EN_RA_XIANYUAN_TREAS_ALREADY_FETCH,				//�����Ѿ���ȡ���ý�����
		EN_RA_XIANYUAN_TREAS_GIFT_FULL,					//�����ȡ��������

		//////////////////////////////////////  ��ʱ��ɱ   //////////////////////////////////
		EN_RA_RUSH_BUYING_NOT_IN_TIME,					//��ʱ��ɱ����ʱ��δ��
		EN_RA_RUSH_BUYING_SERVER_BUY_TIMES_FULL,		//����Ʒ�Ѿ�������һ����
		EN_RA_RUSH_BUYING_ROLE_BUY_TIMES_FULL,			//���Ѿ���������������Ʒ��

		///////////////////////////////////// �������� ////////////////////////////////////
		EN_RA_CONSUME_FOR_GIFT_POINTS_NOT_ENOUGH,		//���Ļ��ֲ��㣬�в��ܶһ�����Ʒ


		//////////////////////////////////////  ��Եʥ��   //////////////////////////////////
		EN_QING_YUAN_SHENG_DI_TIME_LIMIT,				//��Եʥ��δ������ʱ��
		EN_QING_YUAN_SHENG_DI_LEVEL_LIMIT,				//���ȼ������޷�����
		EN_QING_YUAN_SHENG_DI_LAYER_LIMIT,				//���Ľ��ȼ����ߣ��޷����볡��
		EN_QING_YUAN_SHENG_DI_TASK_FETCHED,				//����ȡ���ý���
		EN_QING_YUAN_SHENG_DI_TASK_NOT_COMPLETE,		//����δ���

		//////////////////////////////////////  �ز�boss	  /////////////////////////////////
		EN_PRECIOUS_BOSS_IN_SAVE_AREA,					//���İ�ȫ��������PK

		//////////////////////////////////////  ����	      /////////////////////////////////
		EN_SHENSHOU_BACKPACK_NOT_ENOUGH,				//������������
		EN_SHENSHOU_ITEM_IS_NOT_EQUIP,					//����Ʒ����װ��
		EN_SHENSHOU_ZHUZHAN_COUNT_LIMIT,				//��սλ����
		EN_SHENSHOU_NOT_ACTIVE,							//������δ����
		EN_SHENSHOU_QUALITY_NOT_ENOUGH,					//������Ʒ��Ҫ��
		EN_ITEM_NOT_ENOUGH,								//��Ʒ����
		EN_SHENSHOU_SCORE_NOT_ENOUGH,					//�������ֲ���
		EN_SHENSHOU_TIMES_LIMIT,						//���������Ѵ�����

		//////////////////////////////////////  ʥ��	      /////////////////////////////////
		EN_SHENGQI_OPERATION_FAIL,						//ϵͳ�쳣�����Ժ�����
		EN_SHENGQI_SPIRIT_MAX,							//����������

		/////////////////////////////////////// �Ϸ�װ��  ///////////////////////////////////
		EN_CSA_EQUIP_MAX_LEVEL,							//װ���Ѵ����ȼ�

		/////////////////////////////////////// ˫������  ///////////////////////////////////
		EN_FOLLOWER_LEVEL_NOT_ENOUGH,					//��ĸ����ߵȼ����㣬�޷�����ó���

		EN_SHENQU_XILIAN_CAP_NOT_ENOUGH,				//ǰһ������ս�����㣬�޷�����������

		/////////////////////////////////////// ����ϵͳ ////////////////////////////////////
		EN_SHEN_YIN_BEAN_NUM_LIMIT,						//����Դ�������ﵽ����
		EN_SHEN_YIN_XILIAN_UNSUCCESS,					//����ʧ��
		EN_ITEM_NOT_PUT_IN_BAG,							//����Ʒ���ܷ������Ʊ���
		EN_SHEN_YIN_PROTECT_NOT_ENOUGH,					//����������
		EN_SHEN_YIN_UPLEVEL_UNSUCCESS,					//ע��ʧ��
		EN_SHEN_YIN_MAX_LEVEL,							//�����Ѵ���ߵȼ�

		EN_RA_PLANTING_TREE_ACTIVITY_NOT_BEGIN,			//��׽���û��û��ʼ��
		EN_RA_PLANTING_TREE_CANT_PLANTING,				//�������������ٻ�����
		EN_RA_PLANTING_TREE_PLANTING_COUNT_FULL,		//����׽�˺ö������ˣ���Ϣһ�����
		EN_RA_PLANTING_TREE_CANT_WATERING,				//���Ѿ���׽����ֻ������
		EN_RA_PLANTING_TREE_NOT_SUIT_SCENE,				//��ǰ���������ٻ�����

		EN_ELEMENT_HEART_ALREADY_ACTIVE,				//�������Ѿ�����
		EN_ELEMENT_HEART_NOT_ACTIVE,					//��������δ����
		EN_ELEMENT_HEART_LAST_ELEMENT_HEART_NOT_ACTIVE,	//��Ҫ����ǰ������
		EN_ELEMENT_HEART_LAST_ELEMENT_HEART_LIMIT_GRADE,//�����Ѿ��ﵽ���ȼ�
		EN_ELEMENT_HEART_LAST_ELEMENT_HEART_LIMIT_FEED,	//��ʳ���Ѿ��ﵽ���ι����
		EN_ELEMENT_HEART_LAST_CAN_NOT_GET_PRODUCT_TIME,	//��û�е���ɼ�ʱ��
		EN_ELEMENT_HEART_KNAPSACK_NO_SPACE,				//�����ռ䲻��
		EN_ELEMENT_HEART_NOT_ACTIVE_GHOST,				//���ȼ�������
		EN_ELEMENT_HEART_FUZHOU_MAX_GRADE,				//����֮���Ѵ����ȼ�
		EN_ELEMENT_TEXTURE_RESTRAINT,					//������ˣ����ܽ�������
		EN_ELEMENT_HEART_RESTRAIN,						//������˲���ι��
		EN_ELEMENT_HEART_MAX_WUXING_ZHI,				//�ѵ�������ֵ���ֵ
		EN_ELEMENT_HEART_NO_FOOD,						//û�п���ι����ʳ��
		EN_ELEMENT_HEART_ITEM_NOT_ENOGH,				//��Ʒ���ϲ���

		EN_ELEMENT_HEART_EQUIP_UPGRADE_ALEADY_PUT_SLOT,	//�ø����Ѿ�����
		EN_ELEMENT_HEART_EQUIP_ITEM_NOT_ENOUTH,			//������Ʒ����
		EN_ELEMENT_HEART_EQUIP_UPGRADE_ALEADY_MAX_LEVEL,//�Ѿ��ﵽ���
		EN_ELEMENT_EQUIP_CANNOT_UPGRADE,				//װ�����ܽ���
		EN_ELEMENT_EQUIP_UPGRADE_HAS_SLOT_NOT_ACTIVE,	//��ʯδ��Ƕ������������Ʒ��
		EN_ELEMENT_NO_EMPTY_GRID,						//�������Ӳ���
		EN_ELEMENT_HEART_XILIAN_NOT_OPEN,				//ϴ����δ����
		EN_ELEMENT_HEART_FEED_LEVEL_NOT_ENOGH,			//ι���ȼ�����
		EN_ELEMENT_HEART_LAST_ELEMENT_GRADE_NOT_ENOUGH,	//ǰ�ý��׵ȼ�����

		//////////////////////////////////////  ��������    ////////////////////////////////////
		EN_SHENGXINGZHULI_FETCH_TOO_MUCH,				//��ǰ�޿���������
		EN_SHENGXINGZHULI_CHONGZHI_NOT_ENOUGH,			//����ĳ�ֵ�������!

		EN_SHEN_YIN_LIEMING_LIEHUN_POOL_FULL,			//���Ƴ�����
		EN_FABAO_CANNOT_UPGRADE,                        //�����Ѵ�������
		EN_FABAO_IMG_NOT_ACTIVE,                        //��������δ����
		EN_FABAO_IMG_HAS_ACTIVE,                        //���������Ѿ�����
		EN_FABAO_SHUXINGDAN_USE_LIMIT_NUM,              //�������Ե�(���ʵ�)�Ѵ�����
		EN_SHIZHUANF_SHUXINGDAN_USE_LIMIT_NUM,          //ʱװ���Ե�(���ʵ�)�Ѵ�����
		EN_SHIZHUANF_SHENBING_SHUXINGDAN_USE_LIMIT_NUM, //������Ե�(���ʵ�)�Ѵ�����
		EN_FABAO_EQUIP_LEVEL_MAX,                       //����װ���ȼ��ﵽ���
		EN_FABAO_UP_ONE_GRADE_LIMIT,					//�����׼�������
	
		//////////////////////////////////////  ����װ��  ////////////////////////////////////
		EN_FEIXIAN_EQUIP_NOEXIT,						//װ��������
		EN_FEIXIAN_EQUIP_TYPEERR,						//װ�����ʹ���
		EN_FEIXIAN_EQUIP_COMP_TWICE,					//�����ظ��ϳ�
		EN_FEIXIAN_EQUIP_COMP_DIFF,						//����װ����һ��
		EN_FEIXIAN_EQUIP_COMP_COLORERR,					//��ɫ����
		EN_FEIXIAN_EQUIP_COMP_STARERR,					//����������
		EN_FEIXIAN_EQUIP_UP_ERR,						//�������װ���쳣
		EN_SHIZHUANG_IMG_GRADE_LIMIT,					//ʱװ�����Ѵ����ȼ�

		//////////////////////////////////////  1v1 ����������    /////////////////////////////////
		EN_CHALLENGE_FIELD_RANK_POS_LIMIT,				//����������������������
		EN_CHALLENGE_FIELD_IS_CLOSED,					//�۽��ѽ���
			

		//////////////////////////////////////  �������ϵͳ  /////////////////////////////////
		EN_JINGJIE_LEVEL_MAX,							//�Ѵ���߾���
		EN_JINGJIE_CAP_LIMIT,							//δ�ﵽ��������


		/////////////////////////////////////// ���� ////////////////////////////////////
		EN_XIANZUNKA_HAS_ACTIVE,						//�����Ѽ���
		EN_XIANZUNKA_NOT_ACTIVE,						//����δ����
		EN_XIANZUNKA_DAILY_REWARD_HAS_FETCH,			//ÿ�ս�������ȡ
		EN_XIANZUNKA_ACTIVE_ITEM_LACK,					//������û������

		//////////////////////////////////////  �츳 /////////////////////////////////
		EN_TALENT_ROLE_LEVEL_LIMIT,						//��ǰ�ȼ����㣬��Ŭ������
		EN_TALENT_IS_MAX_LEVEL,							//��ǰ�츳�Ѵﵽ���ȼ�
		EN_TALENT_NO_TALENT_POINT,						//�츳��������
		EN_TALENT_OTHER_TALENT_LEVEL_LIMIT,				//δ�ﵽ����Ҫ��
		EN_TALENT_OTHER_TALENT_PROFLEVEL_LIMIT,			//δ�ﵽ��תҪ��

		////////////////////////////////////// �츳���� //////////////////////////////////////
		EN_TALENT_NOT_FOUNT_TALENT,						//û������츳
		EN_TALENT_TALENT_GRID_ERROR,					//�츳���Ӵ���
		EN_TALENT_GRID_NOT_ACTIVATE,					//�츳��δ����
		EN_TALENT_GRID_SKILL_EXISTS,					//�츳���Ѵ��ڼ���
		EN_TALENT_GRID_SKILL_NOT_EXISTS,				//�츳�񲻴��ڼ���
		EN_TALENT_SKILL_ITEM_NOT_ENOUTH,				//�츳������Ʒ����
		EN_TALENT_NOT_SKILL_BOOK,						//û�����������
		EN_TALENT_FLUSH_NOT_CONFIG,						//δ���ÿ۳�Ԫ��
		EN_TALENT_KNAPSACK_GRID_NO_LESS,				//�������Ӳ���
		EN_TALENT_NOT_CHOUJIANG_GRID,					//û������齱����
		EN_TALENT_NOT_CHOUJIANG_SKILL,					//�齱��û�м���
		EN_TALENT_GRID_SKILL_TYPE_EXISTS,				//���������Ѵ���
		EN_TALENT_FINAL_SKILL_TYPE_ERROR,				//�ռ��������ʹ���
		EN_TALENT_FINAL_SKILL_GRID_ERROR,				//�ռ����ܸ��Ӵ���
		EN_TALENT_FINAL_NOT_CAN_UPGRADE,				//�츳���ܲ�������
		EN_TALENT_ITEM_NOT_ENOUGH,						//���ϲ���
		EN_TALENT_NEED_LEARN_JUNIOR_SKILL,				//��Ҫѧϰ�͵ȼ�����
		EN_TALENT_SKILL_ALREADY_ATTENTION,				//�Ѿ���ע���ü���
		EN_TALENT_SKILL_ATTENTION_TOO_MUCH,				//��ע����̫����
		EN_TALENT_SKILL_NOT_ATTENTION,					//��δ��ע���ü���
		///////////////////////////////////// �������� /////////////////////////////////////
		EN_BUILD_TOWER_TOO_FAR,							//����̫Զ
		EN_BUILD_TOWER_LOW_DOUHUN,						//���겻��
		EN_CUR_WAVE_HAS_CALL_EXTRA,						//��ǰ�����ٻ������

		/////////////////////////////////////	 ����	///////////////////////////////////////////////
		EN_YAOSHI_NOT_EXIST,							//����δ����
		EN_YAOSHI_CANNOT_UPGRADE,						//�����Ѵ�����
		EN_YAOSHI_UP_ONE_GRADE_LIMIT,					//���ν׼�������
		EN_YAOSHI_IMG_NOT_ACTIVE,						//����δ����
		EN_YAOSHI_SPECIAL_IMG_GRADE_LIMIT,				//�������������Ѵ���߼�
		EN_YAOSHI_SHUXINGDAN_USE_LIMIT_NUM,				//�������ʵ�ʹ�������Ѵ�����

		/////////////////////////////////////	 ͷ��	///////////////////////////////////////////////
		EN_TOUSHI_NOT_EXIST,							//ͷ��δ����
		EN_TOUSHI_CANNOT_UPGRADE,						//ͷ���Ѵ�����
		EN_TOUSHI_UP_ONE_GRADE_LIMIT,					//ͷ�ν׼�������
		EN_TOUSHI_IMG_NOT_ACTIVE,						//����δ����
		EN_TOUSHI_SPECIAL_IMG_GRADE_LIMIT,				//ͷ�����������Ѵ���߼�
		EN_TOUSHI_SHUXINGDAN_USE_LIMIT_NUM,				//ͷ�����ʵ�ʹ�������Ѵ�����

		/////////////////////////////////////	 �����	///////////////////////////////////////////////
		EN_QILINBI_NOT_EXIST,							//�����δ����
		EN_QILINBI_CANNOT_UPGRADE,						//������Ѵ�����
		EN_QILINBI_UP_ONE_GRADE_LIMIT,					//����۽׼�������
		EN_QILINBI_IMG_NOT_ACTIVE,						//����δ����
		EN_QILINBI_SPECIAL_IMG_GRADE_LIMIT,				//��������������Ѵ���߼�
		EN_QILINBI_SHUXINGDAN_USE_LIMIT_NUM,			//��������ʵ�ʹ�������Ѵ�����

		/////////////////////////////////////	 ���	///////////////////////////////////////////////
		EN_MASK_NOT_EXIST,								//���δ����
		EN_MASK_CANNOT_UPGRADE,							//����Ѵ�����
		EN_MASK_UP_ONE_GRADE_LIMIT,						//��߽׼�������
		EN_MASK_IMG_NOT_ACTIVE,							//����δ����
		EN_MASK_SPECIAL_IMG_GRADE_LIMIT,				//������������Ѵ���߼�
		EN_MASK_SHUXINGDAN_USE_LIMIT_NUM,				//������ʵ�ʹ�������Ѵ�����

		////////////////////////////����//////////////////////////////////////////////
		EN_FISHING_KNAPSACK_SPACE_NOT_ENOUGH,			//��������һ�±������������
		EN_FISHING_NOT_FISHING_AREA,					//��Ǹ�����ﲻ�ܵ���
		EN_FISHING_FISH_BAIT_NOT_ENOUGH,				//������㣬��ȥ����һ�°�
		EN_FISHING_CANT_CASTING_RODS,					//���ڻ������׸���
		EN_FISHING_CANT_PULL_RODS,						//���ڻ���������
		EN_FISHING_CANT_CONFIRM_RESULT,					//���ڻ�����ȷ��
		EN_FISHING_GET_FISH_FAIL,						//���ź������������
		EN_FISHING_TREASURE_REWARD_NO_SPACE,			//�����ռ䲻�㣬�Ѿ��ʼ����ű��佱��
		EN_FISHING_GEAR_NOT_ENOUGH,						//�������㣬����ʹ��
		EN_FISHING_OIL_NOT_ENOUGH,						//���Ͳ��㣬�޷�ʹ��
		EN_FISHING_SPEAR_NOT_ENOUGH,					//��治�㣬�޷�ʹ��
		EN_FISHING_NET_NOT_ENOUGH,						//�������㣬�޷�ʹ��
		EN_FISHING_BIG_FISH_ALREADY_HELPED,				//�Ѿ���æ������
		EN_FISHING_BIG_FISH_PULL_FAIL,					//���͵�һ���ͣ�����������
		EN_FISHING_THE_HELPED_PULLED,					//�����Ѿ����ˣ���û�ܰ���æ
		EN_FISHING_CONFIRM_GET_FISH_MEMBER_NOT_FISHING,	//����û�ڵ��㣬����������
		EN_FISHINT_STEAL_TARGET_NOT_IN_SCENE,			//�Է����뿪�������޷�͵��
		EN_FISHINT_STEAL_COUNT_LIMIT,					//����͵���������
		EN_FISHINT_BE_STEALED_COUNT_LIMIT,				//�Է��ı�͵��������
		EN_FISHINT_STEAL_FISH_FAIL,						//͵��ʧ��
		EN_FISHING_STEAL_TARGET_HAS_NO_FISH,			//�Է���û�����͵
		EN_FISHING_FISH_NOT_ENOUGH,						//������㣬�޷��һ�
		EN_FISHING_QUICK_FISHING_HAS_ACTIVED,			//���Ѿ�������ˣ������ٴι���
		EN_FISHING_FISHING_AREA_HAS_OTHER_ROLE,			//����΢Զһ����㣬��Ҫ���ŵ��������Ŷ
		EN_FISHING_REWARD_SOCRE_NOT_ENOUGH,				//���ֲ��㣬�޷���ȡ����

		//////////////////////////////////////  ����ϵͳ����    /////////////////////////////////
		EN_RUNE_SYSTEM_RUNE_WUSHIYIJI_EQUIP_FAIL,	//������һ����ս��ֻ��װ���ھ����λ
		EN_RUNE_SYSTEM_SLOT_LIMIT,					//��ս���ֻ����Ƕ����һ��ս��

		//////////////////////////////////////  ���޲���    /////////////////////////////////
		EN_SHENSHOU_EQUIP_STRENGTH_LEVEL_MAX,			//����װ��ǿ���ﵽ��ߵȼ�

		//////////////////////////////////////��������/////////////////////////////////
		EN_HESHENG_LUOSHU_YIJING_ALREADY_ACTIVATED,		//�Ѿ�����
		EN_HESHENG_LUOSHU_YIJING_NOT_ITEM,				//��Ʒ����
		EN_HESHENG_LUOSHU_YIJING_NOT_ACTIVATED,			//û�м���
		EN_HESHENG_LUOSHU_YIJING_UPGRADE_MAX,			//������
		EN_HESHENG_LUOSHU_YIJING_NOT_MAX_LEVEL,			//û������
		EN_HESHENG_LUOSHU_YIJING_MAX_LEVEL,				//��������

		////////////////////////////////////// ����ϵͳ���� //////////////////////////////////
		EN_JINJIESYS_REWARD_OWNED,						//���Ѿ���ø�����
		EN_SPECIAL_JINGLING_REWARD_OWNED,				//���Ѿ���ó������Ｄ����޷���������


		//////////////////////////////////////  תְ����  /////////////////////////////////
		EN_DAILY_WORK_EXP_NOT_ENOUGH,					//����ֵ����

		////////////////////////////////////// С���ػ�  /////////////////////////////////
		EN_INVALID_IMP_GUARD,                            //С����ʧЧ

		////////////////////////////////////// ��������  /////////////////////////////////
		EN_OPEN_GAME_ACTIVITY_GUILD_BATTLE_NOT_END,		//��û�в���������Ի

		////////////////////////////////////// ����ϵͳ�����䣩  /////////////////////////////////
		EN_RUNE_TOWER_LAYER_NOT_ENOUGH,					//ս����δ�ﵽ����

		//////////////////////////////// �¹�   //////////////////////////////////////////////
		EN_DRESSING_ROOM_EXCHANGE_ITEM_NOT_ENOUGH,		//�һ��ﲻ��

		////////////////////////////����//////////////////////////////////////////////
		EN_SHENQI_MAX_QUALITY_LIMIT,					//ֻ����Ƕ����Ʒ�ʵ�������
		EN_SHENQI_SHENBING_NOT_ACTIVE,					//�����δ����ܽ�������
		EN_SHENQI_BAOJIA_NOT_ACTIVE,					//������δ����ܽ�������
		EN_SHENQI_NOT_SUIT,								//�����뵱ǰ��������ף���������		
		EN_SHENQI_NOT_ACHIEVE_MAX_QUALITY,				//��װδ�ﵽ���Ʒ��
		EN_WUQI_HAS_ACTIVE,                             //����Ѿ�����

		//////////////////////////// ���ͼ¼ //////////////////////////////////////////////
		EN_RED_EQUIP_COLLECT_TAKEON_LIMIT,				//����װ��δ������ܴ���
		EN_RED_EQUIP_COLLECT_NOT_ACTIVE_FECTCH_REWARD,	//��װδ����,�޷���ȡ�ƺŽ���
		EN_RED_EQUIP_COLLECT_HAS_FETCH_REWARD,			//�ý����ƺ��Ѿ���ȡ��

		//////////////////////////////////////	��Ϲ���	///////////////////////////////////
		EN_RA_COMBINE_BUY_TOO_MUCH_ITEM,				//���ﳵ��Ʒ����
		EN_RA_COMBINE_BUY_NO_ITEM,						//���ﳵ�ǿյ�
		EN_RA_COMBINE_BUY_ITEM_TOO_MUCH,				//������Ʒ������������

		//////////////////////////////////////	���췵��	///////////////////////////////////
		EN_DAY_CHONGZHI_REWARD_HAD_FETCH_REWARD,		//����ȡ���ý���
		EN_DAY_CHONGZHI_REWARD_DAY_COUNT_NOT_ENOUGH,	//�ۼƳ�ֵ����δ�ﵽҪ��

		//////////////////////////////////////	����boss ///////////////////////////////////
		EN_PERSONAL_BOSS_BUY_LIMIT_VIP,		           //�㵱ǰVIP�ȼ����ɹ���Ĵ����Ѵ����ޣ�

		//////////////////////////////////////	���޼�԰	///////////////////////////////////
		EN_SPOUSE_HOME_ITEM_NOT_PUT_IN_BAG,				//����Ʒ���ɱ�װ��
		EN_SPOUSE_HOME_BEFORE_ROOM_NOT_OPEN,			//ǰ�淿��δ��
		EN_SPOUSE_HOME_ROOM_BUY_LIMIT,					//���乺���Ѵ�����
		EN_SPOUSE_HOME_ONLY_CAN_BUY_FOR_LOVER,          //ֻ��Ϊ�Լ��İ��¹��򷿼�
		EN_SPOUSE_HOME_LOVER_ROOM_BUY_LIMIT,			//���µķ��������Ѵ�����
		EN_SPOUSE_HOME_ONLY_CAN_EQUIP_FOR_LOVER,        //ֻ��Ϊ�Լ��İ���װ�緿��
		EN_SPOUSE_HOME_LOVER_ROOM_NO_OPEN,              //����û�пշ������װ��

		//////////////////////////////////////	װ��ϴ��	///////////////////////////////////
		EN_EQUIP_BAPTIZE_PART_NOT_OPEN,					//�ò�λ��δ��������
		EN_EQUIP_BAPTIZE_SLOT_ALREADY_OPEN,				//�ø�����ѿ���
		EN_EQUIP_BAPTIZE_SLOT_NOT_OPEN,					//�ø����δ����
		EN_EQUIP_BAPTIZE_CANT_LOCK_ALL_SLOT,			//�����������в�

		EN_REDUCE_ZERO_WEARY,                           //û�пɼ��ٵ�ƣ��
		EN_CROSS_ACTIVITY_LEVEL_MAX_LIMIT,				//�ȼ����ߣ����ܲ���ÿ��
		EN_CROSS_ACTIVITY_OPEN_SERVER_DAY_LIMIT,		//���δ����
		EN_ENTER_ACTIVITY_FB_LEVEL_MAX_LIMIT,			//�ȼ����ߣ����ܽ�������

		//////////////////////////////////////	�౶��ȡ��ʾ	///////////////////////////////////
		EN_BUY_MULTIPLE_REWARD_LIMIT,					//���칺��౶�����Ѵ�����
		EN_VIP_LEVEL_NOT_ENOUGHT,						//vip�ȼ����㣬���ܹ���౶����
		EN_PASS_LEVEL_NOT_ENOUGHT,						//����ͨ��һ����ܹ���౶����
		EN_BUY_MUITIPLE_SCENE_LIMIT,					//��ǰ������������౶����

		//////////////////////////////////////	ʱװ	///////////////////////////////////
		EN_SHIZHUANG_BODY_UP_ONE_GRADE_LIMIT,			//ʱװ�׼�������
		EN_SHIZHUANG_WUQI_UP_ONE_GRADE_LIMIT,			//����׼�������


		//////////////////////////////////////	����	///////////////////////////////////
		EN_GREATE_SOLDIER_HUANHUA_CARD_ALREADY_ACTIVED,	//���Ѽ��������ħ�û���
		EN_GREATESOLDIER_NOT_ALIVE,						//��ħδ����
		EN_GREATESOLDIER_POTENTIAL_UP_FAIL,				//Ǳ������ʧ��
		EN_BEAUTY_FETCH_SEQ_ERROR,						//��ȡ�±����
		EN_BEAUTY_FETCH_HAS_FETCHED,					//����ȡ������
		EN_BIANSHEN_LIMIT,								//����״̬���޷����иò���
		EN_GREATESOLDIER_QUALITY_NOT_ENOUGH,			//������Ʒ��Ҫ��
		EN_GREATESOLDIER_EQUIP_STRENGTH_LEVEL_MAX,		//��ħװ��ǿ���ﵽ��ߵȼ�
		EN_GREATESOLDIER_NO_EQUIP,						//�ò�λû��װ����ħװ��

		/////////////////////////////////// һԪ���� ////////////////////////////////////////
		EN_CLOUD_PURCHASE_BUY_TIME_LIMIT,				//δ�����Ź���ʱ��
		EN_CLOUD_PURCHASE_BUY_COUNT_LIMIT,				//��Ǹ���㹺��ķ�����������
		EN_CLOUD_PURCHASE_CONVERT_COUNT_LIMIT,			//�һ�������������
		EN_CLOUD_PURCHASE_CONVERT_SCORE_LIMIT,			//���ֲ���
		EN_CLOUD_PURCHASE_TICKET_LIMIT,					//����ȯ����

		////////////////////////////	ֱ��������	//////////////////////////////////////////////
		EN_UPGRADE_CARD_BUY_ALREADY_BUY,				//�����Ѿ�������ˣ�����������

		//////////////////////////////////////   �������� ���    ////////////////////////////////////
		EN_JOIN_LIMIT_LEVEL_NOT_ENOUGH,					//�ȼ�����
		EN_JOIN_LIMIT_LEVEL_EXCEEDS_MAXIMUM,			//�ȼ�����
		EN_JOIN_LIMIT_CAP_NOT_ENOUGH,					//ս������
		EN_JOIN_LIMIT_OPEN_DAY_LIMIT,					//���ղ����ţ��޷�����

		//////////////////////////////////////  ������    ////////////////////////////////////
		EN_HUGUOZHILI_CANNOT_ACTIVE,					//�����������㣬�޷�����������
		EN_HUGUOZHILI_ALREADY_ACTIVE,					//����ӵ��������
		EN_HUGUOZHILI_IN_CD,							//������������ȴ��
		EN_HUGUOZHILI_ALIVE,							//���Ѹ�����ܼ���������

		//////////////////////////////////////  ��������    ////////////////////////////////////
		EN_TODAY_THEME_FETCH_FAIL,						//�Ѿ���ȡ���ý���

		//////////////////////////////////////  ��Ԫ��    ////////////////////////////////////
		EN_ZERO_BUY_RETURN_ALREADY_BUY,					//�Ѿ�����������
		EN_ZERO_BUY_RETURN_NOT_BUY,						//��δ��������

		////////////////////////////////////////////////////////////////////////////////////////
		EN_BAIBEIFANLI2_ALREADY_BUY,					//�Ѿ����������Ʒ

		EN_GIFTSHOP_BUY2_NOT_START,						//����޹���δ����
		EN_GIFTSHOP_BUY2_SEQ_ERROR,						//seq����

		//////////////////////////////////////  תְװ������    ////////////////////////////////////
		EN_ZHUANZHI_EQUIP_WAKE_ORDER_NOT_ENOUGH,		//תְװ����������
		EN_ZHUANZHI_EQUIP_WAKE_COLOR_NOT_ENOUGH,		//תְװ��Ʒ�ʲ���
		EN_ZHUANZHI_EQUIP_WAKE_NOT_WEAR,				//����תְװ������ܾ���
		EN_ZHUANZHI_EQUIP_COMPOSE_FAIL,					//�ϳ�ʧ��
		EN_ZHUANZHI_EQUIP_COMPOSE_REQ_FAIL,				//�ϳ�����ʧ��
		EN_ZHUANZHI_EQUIP_COMPOSE_XIANPIN_NUM_ERROR,	//׿Խ������������
		EN_ZHUANZHI_EQUIP_COMPOSE_INDEX_ERROR,			//�ϳɲ��ϳ���

		//////////////////////////////////////	�����ƹ�	///////////////////////////////////
		EN_RA_LUCKY_CLOUD_BUY_ALL_SELL,					//���ֿ������������ȴ���һ�ֿ���
		EN_RA_LUCKY_CLOUD_ALL_ROUND_FINISH,				//�����ƹ���ȫ���ִ���ˢ���ˣ������
		EN_RA_LUCKY_CLOUD_THIS_ROUND_NO_OPEN,			//��һ����δ��ʼ
		EN_RA_LUCKY_CLOUD_KNAPSACK_NO_SPACE,			//�����������ƹ�������ͨ���ʼ�����

		//////////////////////////////////////�񻶴��ֹ�///////////////////////////////////
		EN_CRACY_BUY_LEVEL_LESS_THAN_ACTIVITY,			//�Ѿ���������

		//////////////////////////////////////  ��ĩװ��  ////////////////////////////////////
		EN_TIANSHENHUTI_SLOT_EMPTY,						//��λ��û��װ��
		EN_TIANSHENHUTI_BACKPACK_FULL,					//��ĩװ����������
		EN_TIANSHENHUTI_EQUIP_LEVEL_DIFFERENT,			//װ�����������
		EN_TIANSHENHUTI_BACKPACK_NO_SAPCE,				//�����ռ䲻��
		EN_TIANSHENHUTI_ROLL_NOT_OPEN,					//�ó齱��ĩ�ſ���
		EN_TIANSHENHUTI_EQUIP_ALEADY_MAX_LEVEL,			//װ���Ѿ�����߽�
		EN_TIANSHENHUTI_SCORE_NOT_ENOUGH,				//���ֲ���

		//////////////////////////////////////	��ϵͳ	///////////////////////////////////
		EN_MYTH_BACKPACK_NOT_ENOUGH,					//�񻰱�������
		EN_MYTH_HAD_RESONANCED_ALREAD,					//�Ѿ������ɹ���
		EN_MYTH_STUFF_NOT_ENOUGH,						//���ϲ���
		EN_MYTH_SOUL_ESSENCE_NOT_ENOUGH,				//��꾫������
		EN_MYTH_DIGEATION_LEVEL_LIMIT,					//��ǰ����ȼ��Ѵ�����

		//////////////////////////////////////  �������ˮ�� ////////////////////////////////////
		EN_SHUIJING_GATHER_NONE,						//�Ѿ����꣬����´�ˢ��
		EN_SHUIJING_NOT_IN_ROLE,						//����ȥ�ɼ�
		EN_SHUIJING_HUSONG_TASK_LIMIT,					//���л��������ܻ���
		EN_SHUIJING_ALREADY_HAVE,						//���Ѿ���ˮ�����Ի��ͣ������ٲɼ�

		//////////////////////////////////////  ˽��ʧ��ԭ�� ////////////////////////////////////
		EN_SINGLE_CHAT_FAIL_TYPE_FORMAT,				//˽�ĸ�ʽ����
		EN_SINGLE_CHAT_FAIL_TYPE_LENGTH,				//˽�ĳ��ȴ���
		EN_SINGLE_CHAT_FAIL_TYPE_AUDIO,					//������������˽��
		EN_SINGLE_CHAT_FAIL_TYPE_CHAT_USER,				//��ȡ˽����Ϣʧ��
		EN_SINGLE_CHAT_FAIL_TYPE_FORBID_CROSS_CHAT,		//��ֹ���˽��
		EN_SINGLE_CHAT_FAIL_TYPE_NOT_FRIEND,			//�Է��������Ǻ��ѹ�ϵ
		EN_SINGLE_CHAT_FAIL_TYPE_FORBID_CHANNEL,		//�������У��޷�����˽��
		EN_SINGLE_CHAT_FAIL_TYPE_FORBID_CHANNEL_TIME,	//��ʱ���޷�˽��


		//////////////////////////////////////  �̶�ֵ    ////////////////////////////////////
		EN_GET_ACCOUNT_GOLD_TOO_FAST = 100000,			//���˺���ȡԪ�����ʱ�䲻��
		EN_COIN_NOT_ENOUGH = 100001,					//ͭ�Ҳ���
		EN_GOLD_NOT_ENOUGH = 100002,					//��Ԫ�����㣬��ǰ����ֵ��
		EN_BIND_GOLD_NOT_ENOUGH = 100003,				//��Ԫ������
		EN_MONEY_IS_LOCK = 100004,						//��Ǯ�Ѿ�����
		EN_ROLE_ZHENQI_NOT_ENOUGH = 100005,				//�ɻ겻��
		EN_XIANNV_EXP_DAN_LIMIT = 100006,				//��Ů���鵤����
		EN_CONVERTSHOP_BATTLE_FIELD_HONOR_LESS = 100007,//ս����������
		EN_ROLE_NV_WA_SHI_NOT_ENOUGH = 100008,			//Ů��ʯ����
		EN_ROLE_HUNLI_NOT_ENOUGH = 100009,				//��������
		EN_SHENGWANG_NOT_ENOUGH = 100010,				//��������
		EN_HUOLI_NOT_ENOUGH = 100011,					//��������
		EN_GUANGHUI_NOT_ENOUGH = 100012,				//��Բ���
		EN_REWARD_HAS_FETCH = 100013,					//��������ȡ
		EN_CONFIG_ERROR = 100014,						//���ö�ȡ����
		EN_ILLEGAL_OPERA = 100015,						//�Ƿ��Ĳ���
		EN_PARAMETER_ERROR = 100016,					//�����������
		EN_LEVEL_LIMIT = 100017,						//������
		EN_NO_TIMES = 100018,							//�Ѿ�û�д�����
		EN_HIDDEN_SERVER_OPERA_ERROR = 100019,			//������޷����д˲���
	};
}

#endif
