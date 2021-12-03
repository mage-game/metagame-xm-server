#ifndef __PROTOCAL_H__
#define __PROTOCAL_H__

namespace Protocol
{
	enum _MsgType
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ENTER_GS_ACK_SC = 1000,										// ������볡���������ظ�
		MT_CHANGE_GS_SC = 1001,
		MT_SERVER_BUSY_SC = 1002,										// ������̫æ�������û����ﵽ�������ޣ�

		MT_ENTER_GS_REQ_CS = 1050,										// ������볡��������
		MT_LOGOUT_CS = 1051,
		MT_TRANSPORT_REQ = 1052,
		MT_HEARTBEAT_CS = 1053,

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ENTER_SCENE_SC = 1100,
		MT_LEAVE_SCENE_SC = 1101,
		MT_VISIBLE_ENTER_ROLE_SC = 1102,								// Role��������������
		MT_VISIBLE_LEAVE_SC = 1103,										// �����˳���������
		MT_OBJ_MOVE_SC = 1104,											// ����λ���ƶ�ͬ��
		MT_VISIBLE_ENTER_FALLING_SC = 1105,								// FallingItem��������������
		MT_VISIBLE_ENTER_MONSTER_SC = 1106,								// Monster��������������
		MT_VISIBLE_ENTER_PET_SC = 1107,									// Pet��������������
		MT_ROLE_VISIBLE_CHANGE_SC = 1108,								// ��ɫװ���ı�������Ľ�ɫ��ʾ�仯
		MT_VISIBLE_ENTER_ROLE_STALL_SC = 1109,							// Role��̯��������������
		MT_VISIBLE_ENTER_GATHER_OBJ_SC = 1110,							// �ɲɼ���Ʒ�����������
		MT_GATHER_BEGIN_SC = 1111,										// ��ʼ�ɼ�
		MT_GATHER_STOP_SC = 1112,										// �����ɼ�
		MT_GATHER_TIMER_SC = 1113,										// ��ʼ�ɼ���ʱ
		MT_HOOK_STATUS_SC = 1114,										// ��ʼ�һ�����
		MT_RESET_POST_SC = 1115,										// ǿ������λ��
		MT_OBJ_EFFECT_SHOW_SC = 1116,									// OBJ���ϲ���Ч��
		MT_SCENE_FB_DYNINFO_SC = 1117,									// ������̬��Ϣ
		MT_TEAM_MEMBER_POS_SC = 1118,									// ͬ��ͬ��������λ��
		MT_SKILL_RESET_POS_SC = 1119,									// ����ǿ���������� ����༼��
		MT_VISIBLE_ENTER_BATTLEFIELD_SHENSHI_SC = 1120,					// ս����ʯ��������������
		MT_REST_STATUS_SC = 1121,										// ��ʼ��������
		MT_REST_INVITE_NOTIFY_SC = 1122,								// ѯ���Ƿ�ͬ��˫��
		MT_REST_INVITE_RESULT_SC = 1123,								// ����˫�޵Ľ��
		MT_USER_SCENE_SKILL_STATUS_SC = 1124,							// ��ҳ�������״̬
		MT_ALL_OBJ_MOVE_INFO_SC = 1125,									// �������ж�����˶���Ϣ����
		MT_VISIBLE_ENTER_ROLE_SHADOW_SC = 1126,							// RoleShadow��������������
		MT_OBJ_MOVE_MODE_SC = 1127,										// �����˶�ģʽ֪ͨ
		MT_SCENE_MONSTER_DIE_SC = 1128,									// �����ڹ�������
		MT_VISIBLE_ENTER_EFFECT_OBJ_SC = 1129,							// Ч����Ʒ�����������
		MT_FB_FOLLOW_NPC_INFO_SC = 1130,								// ��������NPC��Ϣ
		MT_TRIGGER_NOTIFY_SC = 1131,									// �����������㲥
		MT_FIX_POS_SC = 1132,											// λ�þ�ƫ֪ͨ
		MT_PERFORM_SCENE_SKILL_SC = 1133,								// �ͷų�������֪ͨ
		MT_ACTION_REJECT_SC = 1134,										// �����ܾ�֪ͨ
		MT_TEAM_CALL_TOGETHER_SC = 1135,								// �����ټ��㲥
		MT_TEAM_CALL_IN_SC = 1136,										// ������ļ�㲥
		MT_VISIBLE_ENTER_BIG_COIN_OBJ_SC = 1137,						// ��ͭ�ҽ����������
		MT_PICK_BIG_COIN_SUCC_SC = 1138,								// �����ͭ�ҳɹ�
		MT_CHANGE_SCENE_NOTIFY_SC = 1139,								// �г���֪ͨ
		MT_VISIBLE_ENTER_MARRY_OBJ_SC = 1140,							// ����������������
		MT_VISIBLE_ENTER_TRIGGER_OBJ_SC = 1141,							// Trigger�����������
		MT_VISIBLE_ENTER_MONSTER_HALO_OBJ_SC = 1142,					// ����⻷�����������
		MT_YANHUA_EXPLODE_SC = 1143,									// �ͷ��̻�
		MT_VISIBLE_ENTER_ROLE_SHADOW_OBJ_SC = 1144,						// ��ɫӰ�ӽ����������
		MT_OPERATE_RET_SC = 1145,										// �������
		MT_VISIBLE_ENTER_WORLD_EVENT_OJB_SC = 1146,						// �����¼���������������
		MT_REQ_TEAM_MEMBER_POS_LIST_SC = 1147,							// ��������Աλ���б���
		MT_GATHER_INFO_CHANGE_SC = 1148,								// �ɼ�����Ϣ�仯
		MT_BOSS_DPS_FLAG_SC = 1149,										// dps���

		MT_OBJ_MOVE_CS = 1150,											// �ƶ�
		MT_START_GATHER_REQ_CS = 1151,									// ����ʼ�ɼ�
		MT_ENTER_FB_CS = 1152,											// ���븱��
		MT_LEAVE_FB_CS = 1153,											// �뿪����
		MT_CANCEL_MONSTER_STATIC_STATE_CS = 1154,						// ȡ���������ﾲֹ״̬
		MT_REST_REQ_CS = 1155,											// ����ʼ�������������
		MT_REST_INVITE_CS = 1156,										// ����˫��
		MT_REST_INVITE_ACK_CS = 1157,									// ˫�������Ӧ��
		MT_GET_ALL_OBJ_MOVE_INFO_REQ_CS = 1158,							// �������������ж�����˶���Ϣ
		MT_REST_START_AFFIRM_CS = 1159,									// ȷ�Ͽ�ʼ˫��
		MT_TEAM_CALL_TOGETHER_CS = 1160,								// �ټ���Ա
		MT_TEAM_CALL_IN_CS = 1161,										// ��ļ��Ա
		MT_PICK_BIG_COIN_CS = 1162,										// �����ͭ��
		MT_AUTO_FB_CS = 1163,											// ɨ������
		MT_REQ_TEAM_MEMBER_POS_CS = 1164,								// ����ͬ������Աλ����Ϣ
		MT_STOP_GATHER_REQ_CS = 1165,									// ����ֹͣ�ɼ�
		MT_CHANGE_SCENE_LINE_REQ_CS = 1166,								// ��������
		MT_GATHER_BE_GATHER_SC = 1167,									// �ɼ��ﱻ�ɼ���Ϣ
		MT_ROLE_TASK_APPEARN_SC = 1168,									// ������������

		MT_OBJ_FLY_HEIGHT_CHANGE_CS = 1171,								// ��ɫ���и߶ȱ仯����
		MT_OBJ_FLY_HEIGHT_CHANGE_SC = 1172,								// ��ɫ���и߶ȱ仯�㲥

		MT_BOSS_FIRST_HIT_FLAG_SC = 1173,								// boss�׵����
		MT_BOSS_FIRST_HIT_INFO_SC = 1174,								// boss��һ����Ϣ
		MT_BOSS_DPS_TITLE_SC = 1175,									// boss�����ƺ���Ϣ

		MT_ROLE_RETURN_REALIVE_POSI_CS = 1176,							// �����������ǻظ����

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_OBJ_CHANGE_BLOOD_SC = 1300,									// �㲥hp�仯
		MT_PERFORM_SKILL_SC = 1301,										// �ͷż���
		MT_ROLE_REALIVE_SC = 1302,										// ����
		MT_PERFORM_AOE_SKILL_SC = 1303,									// ʩ��AOE����
		MT_OBJ_CHANGE_MP_SC = 1304,										// �㲥mp�仯
		MT_BUFF_ADD_SC = 1305,											// ����buff
		MT_BUFF_REMOVE_SC = 1306,										// ɾ��buff
		MT_EFFECT_LIST_SC = 1307,										// Effect�б�
		MT_EFFECT_INFO_SC = 1308,										// Effect��Ϣ
		MT_EFFECT_REMOVE_SC = 1309,										// ɾ��Effect֪ͨ
		MT_ITEM_EFFECT_PACKAGE_CD = 1310,								// ��Ѫ��������Effect CD��Ϣ
		MT_BUFF_MARK_SC = 1311,											// buff mark��Ϣ
		MT_SKILL_TARGET_POS_SC = 1312,									// ����Ŀ����������
		MT_SKILL_EFFECT_SC = 1312,										// ������Ч����
		MT_EXPLODE_SC = 1313,											// ��ը
		MT_SKILL_PHASE_SC = 1314,										// ����ʩ���׶ι㲥
		MT_REALIVE_WAITING_SC = 1315,									// �ȴ�����
		MT_QTE_READY_SC = 1316,											// QTE���ܵȴ��ͷ�
		MT_SKILL_TIP_SC = 1317,											// ����tips
		MT_FIGHT_SPECIAL_FLOAT_SC = 1318,								// ս������Ʈ�֣����з����ֱ���ʱƮ���յ��˺���
		MT_WORLD_BOSS_SPECIAL_SHIELD_SC = 1319,							// �㲥����BOSS���ܱ仯
		MT_BIANSHEN_EFFECT_IMAGE_BROADCAST_SC = 1320,					// �㲥����effect����
		MT_UPGRADE_SKILL_INFO_SC = 1321,								// ���׼�����Ϣ
		MT_CONTINUE_KILL_INFO_SC = 1322,								// ��ն��Ϣ
		MT_INVISIBLE_VIEW_SC = 1323,									// �㲥������Ϣ

		MT_PERFORM_SKILL_CS = 1350,										// ʩ�ż���
		MT_REALIVE_REQ_CS = 1351,										// �����ɫ����
		MT_GET_EFFECT_LIST_REQ_CS = 1352,								// ������ȡEffect�б�
		MT_GET_EFFECT_INFO_REQ_CS = 1353,								// ��ȡEffect��Ϣ
		MT_PERFORM_QTE_SKILL_CS = 1354,									// �ͷ�QTE����
		MT_SKILL_CD_CS = 1355,											// ��ʼ����CD

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ROLE_INFO_ACK_SC = 1400,										// �����ɫ������Ϣ����
		MT_SKILL_LIST_INFO_ACK_SC = 1401,								// �����ɫ������Ϣ����
		MT_ROLE_ATTR_VALUE_SC = 1402,									// ��ɫ������ֵ֪ͨ
		MT_CHA_EXP_CHANGE_SC = 1403,									// ������֪ͨ
		MT_CHA_LEVEL_UPGRADE_SC = 1404,									// ���������㲥
		MT_ROLE_CHANGE_PROFESSIONAL_SC = 1405,							// תְ�㲥
		MT_ROLE_ADD_COUNTRY_SC = 1406,									// ������ҹ㲥
		MT_VIP_INFO_SC = 1407,											// ��ɫVip��Ϣ
		MT_ROLE_NAME_COLOR_CHANGE_SC = 1408,							// ��ɫ������ɫ�ı�
		MT_FIND_ROLE_BY_NAME_RET_SC = 1409,								// ��������Ʋ�����ҷ���
		MT_CHANGE_EVIL_SC = 1410,										// �ƶ�ֵ�ı�
		MT_ROLE_SET_COUNTRY_POST_SC = 1411,								// ��ɫ����ְλ��Ϣ
		MT_GET_ROLE_INFO_ACK_SC = 1412,									// �鿴������Ϣ����
		MT_SKILL_INFO_ACK_SC = 1413,									// ����������Ϣ����
		MT_ROLE_CAPABILITY_SC = 1414,									// ��ɫս����
		MT_ROLE_SET_ATTACK_MODE_SC = 1415,								// ���ù���ģʽ����
		MT_ROLE_GUILD_INFO_CHANGE = 1416,								// ��Ҿ�����Ϣ����㲥
		MT_ROLE_AUTHORITY_TYPE_SC = 1417,								// ��ɫ����Ȩ�ޱ���㲥
		MT_ROLE_TEAM_LEADER_INFO_SC = 1418,								// ��ɫ��Ӷӳ���־����㲥
		MT_ROLE_RESET_NAME_SC = 1419,									// ��ɫ����
		MT_ROLE_DAY_COUNT_SC = 1420,									// ��ɫ������Ϣ
		MT_INVEST_COUNT_INFO_SC = 1421,									// Ͷ�ʼƻ�����
		MT_ROLE_CHANGE_SEX_SC = 1422,									// ��ɫ�Ա�ı�
		MT_ROLE_CHANGE_XIANHUN_SC = 1423,								// ��ɫ�ɻ�ı�
		MT_ROLE_CHANGE_YUANLI_SC = 1424,								// ��ɫԪ���ı�
		MT_CHANGE_SPECIAL_APPEARANCE_SC = 1425,							// ������۸ı�
		MT_ROLE_CHANGE_NUQI_SC = 1426,									// ��ɫŭ���ı�
		MT_ROLE_CHANGE_CAMP_HONOUR_SC = 1427,							// ��ɫ��Ӫ�����ı�
		MT_NEWHAND_GUILD_ENDTIME_SC = 1428,								// ����ָ��Ա����ʱ��
		MT_MOUNT_BIANSHEN_INFO_SC = 1429,								// ���������Ϣ
		MT_ROLE_MARRY_INFO_CHANGE_SC = 1430,							// ��ɫ�����Ϣ�ı�
		MT_ROLE_ENERGY_SC = 1431,										// �����ı�
		MT_ROLE_SHENGWANG_SC = 1432,									// �����ı�
		MT_ROLE_SKILL_OTHER_SKILL_INFO = 1433,							// ����������Ϣ
		MT_ROLE_CHANGE_NV_WA_SHI_SC = 1434,								// Ů�ʯ�ı�
		MT_ROLE_EXP_EXTRA_PER_SC = 1435,								// ����ӳ�
		MT_ROLE_CHANGE_HUNLI_SC = 1436,									// �����ı�
		MT_ROLE_CHANGE_LINGJING_SC = 1437,								// �龫�ı�
		MT_ROLE_CHANGE_CHENGJIU_SC = 1438,								// �ɾ͸ı�
		MT_ROLE_CHANGE_DAY_REVIVAL_TIMES_SC = 1439,						// ÿ�θ�������ı�
		MT_ROLE_CHANGE_DAY_ZHUAGUI_INFO_SC = 1440,						// ץ������ʹ���
		MT_ROLE_JINGJIE_INFO_SC = 1442,									// ������Ϣ

		MT_ROLE_INFO_REQ_CS = 1450,										// �����ɫ������Ϣ
		MT_SKILL_INFO_REQ_CS = 1451,									// �����ɫ������Ϣ
		MT_ROLE_SKILL_LEARN_REQ_CS = 1452,								// ������ѧϰ
		MT_ROLE_SET_ATTACK_MODE_CS = 1453,								// ���ù���ģʽF
		MT_ALL_INFO_REQ_CS = 1454,										// ���ڵ�½ʱ����һϵ����Ϣ(ԭ��ģ��ĵ���������Ϣ����)
		MT_QUERY_ROLE_INFO_CS = 1455,									// ��ѯ��ɫ��Ϣ
		MT_QUERY_ROLE_INFO_BY_NAME_CS = 1456,							// ͨ����ɫ����ѯ��ɫ��Ϣ
		MT_CHOOSE_CAMP_CS = 1457,										// ѡ����Ӫ
		MT_CHOOSE_PROF_CS = 1458,										// ѡ������
		MT_GET_ROLE_CAPABILITY_CS = 1459,								// �����ɫս����
		MT_SET_MOVE_MODE_CS = 1460,										// ���������˶�ģʽ
		MT_ROLE_RESET_NAME_CS = 1461,									// ��ɫ��������
		MT_ROLE_SET_ZOMBIE_REQ_CS = 1462,								// ���ý�ʬ�������
		MT_MOUNT_BIANSHEN_OPERA_CS = 1463,								// ����������
		MT_SPEEDUP_HELLO_CS = 1464,										// ���ټ������
		MT_SET_AVATAR_TIMESTAMP_CS = 1465,								// ͷ���޸�ʱ����������
		MT_CROSS_QUERY_ROLE_INFO_CS = 1466,								// ��ѯ�����ɫ��Ϣ
		MT_CROSS_QUERY_EXP_EXTRA_CS = 1467,								// ������ӳ�
		MT_ROLE_GONGXUN_SC = 1468,										// ����ѫֵ
		MT_SEND_AVATAR_TIMESTAMP_INFO_CS = 1469,						// ���͵�һ�θı�ͷ����Ϣ
		MT_CROSS_FIND_ROLE_BY_UUID = 1470,								// ������ҽ�ɫ


		MT_ROLE_JINGJIE_REQ_CS = 1472,									// ��������

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_KNAPSACK_INFO_ACK_SC = 1500,									// ���󱳰���Ϣ����
		MT_KNAPSACK_GRID_CHANGE_SC = 1501,								// �����������ӱ仯
		MT_KNAPSACK_PARAM_SC = 1502,									// ���������б�
		MT_KNAPSACK_GRID_CHANGE_PARAM_SC = 1503,						// �в����ĸ��ӱ仯
		MT_USE_ITEM_SUC_SC = 1504,										// ʹ����Ʒ�ɹ�
		MT_KNAPSACK_MAX_GRID_NUM_SC = 1505,								// ��������������
		MT_STORAGE_MAX_GRID_NUM_SC = 1506,								// �ֿ����������
		MT_ITEM_MULTI_INVALID_SC = 1507,								// ��Ʒ����ʧЧ
		MT_TASK_KNAPSACK_INFO_SC = 1508,								// ���񱳰���Ϣ
		MT_ITEM_COLDDOWN_INFO_SC = 1509,								// ��Ʒʹ��CD��Ϣ
 		MT_TEAM_MEMBER_PICK_ITEM_SC = 1510,								// �����Աʰȡ������Ʒ
		MT_RESERVE_1511_SC = 1511,										// ����
		MT_LACK_ITEM_SC = 1512,											// ȱ����Ʒ��Ϣ
		MT_KNAPSACK_GRID_EXTEND_AUTO_SC = 1513,							// ���������Զ��������
		MT_KNAPSACK_GRID_EXTEND_AUTO_CS = 1514,							// ���󱳰������Զ��������

		MT_KNAPSACK_INFO_REQ_CS = 1550,									// ���󱳰���Ϣ
		MT_USE_ITEM_CS = 1551,											// ʹ����Ʒ
		MT_MOVE_ITEM_CS = 1552,											// �ƶ�������Ԫ
		MT_DICARD_ITEM_CS = 1553,										// ������Ʒ
		MT_KNAPSACK_STORAGE_PUTINORDER_CS = 1554,						// ������
		MT_KNAPSACKSTORAGE_EXTEND_GRID_NUM_CS = 1555,					// �������ֿ���չ
		MT_SPLIT_ITEM_CS = 1556,										// ��ֱ�����Ʒ
		MT_ITEM_COMPOSE_CS = 1557,										// ��Ʒ�ϳ�
		MT_BLUEPRINT_COMPOSE_CS = 1558,									// ͼֽ�ϳ�
		MT_PICK_ITEM_CS = 1559,											// ���������Ʒ
		MT_TAKE_OUT_ALL_ITEM = 1560,									// ȡ���ֿ�������Ʒ
		MT_GEMSTONE_BATCH_COMPOSE_CS = 1561,							// ��ʯ�����ϳ�
		MT_PICK_ITEM_ACT_SC = 1562,										// ���������Ʒ����
		MT_REWARD_ITEM_LIST_ACT_SC = 1563,								// ������Ʒ�з���
		MT_USE_ITEM_MAX_NUM_CS = 1564,									// ʹ����Ʒ(����item_id�������Ȱ�)
		MT_BATCH_DICARD_ITEM_CS = 1565,									// ����������Ʒ


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_EQUIPMENT_INFO_ACK_SC = 1700,								// ��ȡװ����Ϣ����
		MT_EQUIPMENT_ITEM_CHANGE_SC = 1701,								// װ����Ϣ�ı�
		MT_GET_ONE_EQUIPMENT_INFO_ACK_SC = 1702,						// ���󵥸�װ����Ϣ����
		MT_STONE_INFO_SC = 1703,										// �·���ʯ��Ϣ
		MT_EQUIP_SUIT_INFO_SC = 1704,									// װ����װ��Ϣ
		MT_DUANZAO_SUIT_INFO_SC = 1705,									// ������װ�ȼ���Ϣ
		MT_OTHER_USER_ONLINE_STATUS_SC = 1706,							// ������ɫ����״̬
		MT_ROLE_CHANGE_CAMP_SC = 1711,									// ��ɫת��Ӫ
		MT_CHANGE_PROF_RESULT_SC = 1712,								// ��ɫתְ
		MT_SHENBING_INFO_SC = 1713,										// �����Ϣ
		MT_SHENBING_APPE_CHG_SC = 1714,									// �����۱仯
		MT_EXPBOTTLE_INFO_SC = 1715,									// ����ƿ��Ϣ
		MT_ROLE_SPACE_MSG_INFO_SC = 1716,								// ��ɫ�ռ���Ϣ
		MT_ROLE_SPACE_SELF_INFO_SC = 1717,								// ��ɫ�Լ��ռ���Ϣ
		MT_ROLE_LIFE_SKILL_INFO_SC = 1718,								// �������Ϣ
		MT_WASH_INFO_SC = 1719,											// �·�ϴ����Ϣ
		MT_MOVE_CHESS_INFO_SC = 1720,									// ��������Ϣ

		MT_MILLIONAIRE_INFO_CS = 1721,									// ����󸻺���Ϣ
		MT_MILLIONAIRE_INFO_SC = 1722,									// �󸻺���Ϣ
		MT_MOVE_CHESS_STEP_REWARD_INFO_SC = 1723,						// �����Ӹ��ӽ�����Ϣ

		MT_EQUIPMENT_INFO_REQ_CS = 1750,								// ����װ����Ϣ
		MT_TAKE_OFF_EQUIPMENT_CS = 1751,								// ����װ��
		MT_EQUIP_STRENGTHEN_REQ_CS = 1752,								// ��������ǿ��
		MT_EQUIP_UPQUALITY_REQ_CS = 1753,								// ����װ������Ʒ��
		MT_EQUIP_UPLEVEL_REQ_CS = 1754,									// ����װ������
		MT_EQUIP_SHENZHU_REQ_CS = 1755,									// ����װ������
		MT_STONE_UPLVL_CS = 1756,										// ����ʯ����
		MT_DUANZAO_SUIT_REQ_CS = 1757,									// ������װ����
		MT_SHENBING_REQ_CS = 1761,										// ���������Ϣ
		MT_SHENBING_COMPOSE_CS = 1762,									// ����ϳ����
		MT_SHENBING_OP_CS = 1763,										// �����������
		MT_SHENBING_UP_LEVEL_CS = 1764,									// ������Ǽ�������
		MT_SHENBING_ALL_INFO_SC = 1766,									// ���������Ϣ
		MT_ROLE_CHANGE_CAMP_CS = 1769,									// ת��Ӫ����
		MT_CHANGE_PROF_REQ_CS = 1770,									// ��ɫתְ
		MT_EQUIP_FULING_CS = 1771,										// װ������
		MT_EQUIP_CROSS_EQUIP_OPERA_CS = 1772,							// ���װ������
		MT_EQUIP_UP_STAR_CS = 1774,										// װ������
		MT_EQUIP_INHERIT_REQ_CS = 1775,									// ����װ������
		MT_ROLE_ZHUAN_SHENG = 1776,										// ��ɫת��
		MT_ROLE_NOREXITEM_USE_TIMES = 1777,								// ������Ʒ����ʹ�ô���
		MT_TRANSFORM_EXP_REQ_CS = 1778,									// ���۾���
		MT_GET_ROLE_SPACE_MSG_INFO_CS = 1779,							// ��ȡ��ɫ�ռ���Ϣ�����ԣ��ظ�����ӡ���ͻ���¼��
		MT_OPER_SPACE_CS = 1780,										// ������ɫ�ռ����󣨣�
		MT_REMOVE_RECORD_CS = 1781,										// ɾ����¼����
		MT_ROLE_LIFESKILL_OPERA_CS = 1782,								// ������������
		MT_WASH_CS = 1783,												// ����ϴ��
		MT_STONE_INLAY_CS = 1784,										// ����ʯ��Ƕ
		MT_EQUIPMENT_GRID_INFO_SC = 1785,								// װ����������
		MT_EQUIPMENT_WUQI_COLOR_SC = 1786,								// ȫ����ǿ���ȼ��㲥
		MT_STONE_INFO_REQ_CS = 1787,									// ����ʯ��Ϣ
		MT_EQUIP_COMPOUND_CS = 1788,									// ����װ���ϳ�
		MT_EQUIP_UP_ETERNITY_CS = 1789,									// ����װ����������ȼ�
		MT_FRIEND_EXP_BOTTLE_OPERATE_REQ_CS = 1790,						// ���Ѿ���ƿ��ȡ����
		MT_FRIEND_EXP_BOTTLE_ADD_EXP = 1791,							// ���Ѿ���ƿ��������
		MT_EQUIP_USE_ETERNITY_LEVEL_CS = 1792,							// ʹ������ȼ�
		MT_SKIP_REQ_CS = 1793,											// һ������
		MT_EQUIP_EXTEND_INFO_SC = 1794,									// װ����չ��Ϣ
		MT_FEIXIAN_OP_CS = 1795,										// ���ɲ���
		MT_FEIXIAN_EQUIP_INFO_SC = 1796,								// ����װ����Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_NPC_MARK_LIST_SC = 1800,										// npc �б�
		MT_TASK_LIST_ACK_SC = 1801,										// ���������б���
		MT_TASK_INFO_SC = 1802,											// �����ѽ�������Ϣ
		MT_TASK_RECORDER_LIST_SC = 1803,								// ���������¼�б�
		MT_TASK_RECORDER_INFO_SC = 1804,								// ���������¼��Ϣ
		MT_TUMO_TASK_INFO_SC = 1805,									// ��ħ������Ϣ
		MT_HUSONG_TASK_INFO_SC = 1806,									// ����������Ϣ
		MT_GUILD_TASK_INFO_SC = 1807,									// ����������Ϣ
		MT_TASK_ACCEPTABLE_TASK_LIST_SC = 1808,							// �ɽ������б�
		MT_CISHA_TASK_INFO_SC = 1809,									// ��ɱ������Ϣ
		MT_PAOHUAN_TASK_INFO_SC = 1810,									// �ܻ�������Ϣ
		MT_PAOHUAN_TASK_ROLL_POOL_SC = 1811,							// �ܻ��׶ν���
		MT_PAOHUAN_TASK_ROLL_INFO_SC = 1812,							// �ܻ��׶η�����Ϣ
		MT_COMMON_INFO_SC = 1814,										// ͬ����ͨ��Ϣ
		MT_JINGHUAHUSONG_VIEW_CHANGE_SC = 1815,							// �㲥��������״̬�ı�
		MT_SEEKROLE_WHERE_INFO_SC = 1817,								// ��ѯ������ķ���
		MT_HUANGCHENGHUIZHAN_INFO_SC = 1818,							// �ʳǻ�ս
		MT_CAMPEQUIP_INFO_SC = 1819,									// ʦ��װ��
		MT_CAMPDEFEND_INFO_SC = 1820,									// ����������Ϣ
		MT_HUSONG_TASK_COMSUEM_INFO_SC = 1821,							// ������ƷԪ��������Ϣ
		MT_TASK_RANDOM_REWARD = 1822,									// ��������
		MT_JINJIESYS_REWARD_OPERA_REQ_CS = 1824,						// ����ϵͳ��������
		MT_JINJIESYS_REWARD_INFO_SC = 1825,								// ����ϵͳ������Ϣ
		MT_JINJIESYS_REWARD_TIMESTAMP_SC = 1826,						// ����ϵͳ����ʱ��

		MT_TASK_LIST_REQ_CS = 1850,										// ���������б�
		MT_TASK_GIVEUP_CS = 1851,										// �ͻ��˷�������
		MT_FLY_BY_SHOE_CS = 1852,										// ʹ�÷�����ѥ
		MT_TASK_ACCEPT_CS = 1853,										// ��������
		MT_TASK_COMMIT_CS = 1854,										// �ύ����
		MT_TASK_TALK_TO_NPC = 1855,										// ��npc�Ի�
		MT_TUMO_TASK_OPERA_CS = 1856,									// ��ħ�����������
		MT_HUSONG_TASK_REFRESH_CS = 1857,								// ˢ�»�������
		MT_RESERVE_1858 = 1858,											// ��ȡ���ֻ�������
		MT_HUSONG_BUY_TIMES_CS = 1859,									// ����
		MT_RESERVE_1860 = 1860,											// ����
		MT_GUILD_TASK_FINISH_ALL_CS = 1861,								// �������� һ�����
		MT_GUILD_TASK_DOUBLE_REWARD_CS = 1862,							// ����˫������
		MT_RESERVE_1863 = 1863,											// ����
		MT_CISHA_TASK_OPERA_CS = 1864,									// ��ɱ�����������
		MT_RESERVE_1865 = 1865,											// ����
		MT_PAOHUAN_TASK_ROLL_REQ_CS = 1866,								// �ܻ��׶�������
		MT_PAOHUAN_TASK_INFO_REQ_CS = 1867,								// �ܻ�������Ϣ����
		MT_PAOHUAN_TASK_COMMIT_REQ_CS = 1868,							// �ܻ�����������������
		MT_COMMON_OPERATE_CS = 1869,									// ��ͨ��������
		MT_CS_SHENZHUANG_OPER_CS = 1870,								// ��װ��������
		MT_CS_SEEK_ROLE_WHERE_CS = 1871,								// �鿴��ɫ���ĸ��ط�
		MT_CS_CAMPEQUIP_OPERATE_CS = 1872,								// ʦ��װ����������
		MT_CS_CAMP_NORMALDUOBAO_OPERATE_CS = 1873,						// ʦ����ͨ�ᱦ��������
		MT_HUSONG_ADD_SHIIELD_CS = 1874,								// ���ͣ��ӻ���
		MT_FLY_SYNC_JUMP_CS = 1875,										// ��Ь����������Ծͬ��
		MT_FAMOUS_MAN_OPERA_REQ_CS = 1876,								// �����ò�������
		MT_FAMOUS_MAN_INFO_SC = 1877,									// ��������Ϣ����
		MT_FORCE_FINISH_SPECIAL_TASK_SC = 1878,							// ָ�����ĳ����������
		MT_SHENZHUANG_INFO_CS = 1879,									// ��װ��Ϣ
		MT_CSA_EQUIP_INFO_SC = 1880,									// �Ϸ�װ����Ϣ
		MT_CSA_EQUIP_OPERA_CS = 1881,									// �Ϸ�װ����������
		MT_CSA_EQUIP_ACTIVE_SC = 1882,									// �Ϸ�װ��������Ȩ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_HOTKEY_INFO_ACK_SC = 1900,									// ���ؿ�ݼ���Ϣ�б�

		MT_HOTKEY_INFO_REQ_CS = 1950,									// �����ݼ��б�
		MT_CHANGE_HOTKEY_CS = 1951,										// �ı��ݼ�

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MONEY_SC = 2000,												// Ǯ
		MT_ACCOUNT_GOLD_ACK_SC = 2001,									// �˺Ž������
		MT_ACCOUNT_GET_GOLD_ACK_SC = 2002,								// ���˺�����ȡ��ҷŻ�

		MT_ACCOUNT_GOLD_REQ_CS = 2050,									// ��ȡ�˺Ž������
		MT_ACCOUNT_GET_GOLD_REQ_CS = 2051,								// ���˺�����ȡ���

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FRIEND_LIST_ACK_SC = 2100,									// ���غ����б�
		MT_ADD_FRIEND_ROUTE_SC = 2101,									// ������ѯ������һ������
		MT_CHANGE_FRIEND_SC = 2102,										// ������֪ͨ�ͻ��˺��Ѹı�
		MT_ADD_FRIEND_RET_SC = 2103,									// �ܾ��Ӻ��ѻظ�
		MT_ENEMY_LIST_ACK_SC = 2104,									// ���س����б�
		MT_ENEMY_CHANGE_SC = 2105,										// ������֪ͨ�ͻ��˳��˸ı�
		MT_GIVE_FLOWER_KISS_ACK_SC = 2106,								// �ͻ����Ƿ���
		MT_GIVE_FLOWER_SC = 2107,										// �ͻ�֪ͨ
		MT_ALL_CHARM_CHANGE_SC = 2108,									// ����ֵ�ı�
		MT_FRIEND_BLESS_ALL_INFO_SC = 2109,								// ����ף����Ϣ
		MT_FRIEND_BLESS_CHANGE_INFO_SC = 2110,							// ����ף�������Ϣ
		MT_FRIEND_BLESS_INVITE_BLESS_SC = 2111,							// �������ף����
		MT_SONG_HUA_INFO = 2112,										// �ͻ���Ϣ
		MT_SONG_HUA_INFO_REQ = 2113,									// �ͻ���Ϣ����
		MT_OFFLINE_FRIEND_AUTO_DEC_FALG = 2114,							// �Ƿ��Զ�ɾ������x����ѵ�	

		MT_FRIEND_LIST_REQ_CS = 2150,									// �����ȡ�����б�
		MT_ADD_FRIEND_REQ_CS = 2151,									// �����Ϊ����
		MT_ADD_FRIEND_RET_CS = 2152,                    				// �Ƿ���ܼӺ���
		MT_FRIEND_DELETE_CS = 2153,										// ɾ������
		MT_ENEMY_DELETE_CS = 2154,										// ����ɾ������
		MT_GIVE_FLOWER_CS = 2155,										// �ͻ�
		MT_GIVE_FLOWER_KISS_REQ_CS = 2156,								// �ͻ���������
		MT_FRIEND_BLESS_OPERA_REQ_CS = 2157,							// ����ף����������
		MT_BLACKLISTS_SC = 2158,										// ���غ�����
		MT_ADD_BLACK_CS = 2159,											// ��ӵ�������
		MT_DELETE_BLACK_CS = 2160,										// �Ӻ�������ɾ��
		MT_CHANGE_BLACKLISTS_SC = 2161,									// ������֪ͨ�ͻ��˺������ı�

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_XIANNV_INFO_SC = 2200,										// ������Ů��Ϣ
		MT_XIANNV_ALL_INFO_SC = 2201,									// ������Ů��Ϣ
		MT_XIANNV_VIEW_CHANGE_SC = 2203,								// ��Ů�����Ϣ�㲥
		MT_RA_SERVER_PANIC_BUY_INFO_SC = 2204,							// ȫ��������Ϣ
		MT_RA_PERSONAL_PANIC_BUY_INFO_SC = 2205,						// ���˷������Ϣ
		MT_RA_CHONGZHI_RANK_INFO_SC = 2206,								// ������ֵ��Ϣ
		MT_RA_CONSUME_RANK_INFO_SC = 2207,								// ����������Ϣ
		MT_RA_CONSUME_GOLD_FANLI_INFO_SC = 2208,						// ����������Ϣ
		MT_RA_DAY_CHONGZHI_FANLI_INFO_SC = 2209,						// ��ֵ�������Ϣ
		MT_RA_DAY_CONSUME_GOLD_INFO_SC = 2210,							// ÿ�����ѻ��Ϣ
		MT_RA_TOTAL_CONSUME_GOLD_INFO_SC = 2211,						// �ۼ����ѻ��Ϣ
		MT_RA_DAY_ACTIVE_DEGREE_INFO_SC = 2212,							// ÿ�ջ�Ծ���Ϣ
		MT_RA_KILL_BOSS_INFO_SC = 2213,									// ��ɱboss���Ϣ
		MT_RA_CHESTSHOP_INFO_SC = 2214,									// �����챦���Ϣ
		MT_RA_STONE_UPLEVEL_INFO_SC = 2215,								// ��ʯ�������Ϣ
		MT_RA_XN_CHANMIAN_UPLEVEL_INFO_SC = 2216,						// ��Ů�����������Ϣ
		MT_RA_MOUNT_UPGRADE_INFO_SC = 2217,								// ������׻��Ϣ
		MT_RA_QIBING_UPGRADE_INFO_SC = 2218,							// ������׻��Ϣ
		MT_RA_MENTALITY_TOTAL_LEVEL_INFO_SC = 2219,						// ȫ����ǵȼ����Ϣ
		MT_RA_WING_UPGRADE_INFO_SC = 2220,								// ������׻��Ϣ
		MT_RA_QUANMIN_QIFU_INFO_SC = 2221,								// �����Ϣ
		MT_RA_SHOUYOU_YUXIANGNG_INFO_SC = 2222,							// ����������Ϣ
		MT_RA_LOGIN_GIFT_INFO_SC = 2223,								// ��¼������Ϣ
		MT_RAND_ACTIVITY_XIANMENG_BIPIN_SC = 2224,						// �������˱�ƴ
		MT_RAND_ACTIVITY_XIANMENG_JUEQI_SC = 2225,						// ������������
		MT_ACTIVITY_MONSTER_INVADE_TIME_NOTICE_SC = 2226,				// �������ֻ��Ϣ
		MT_YIZHANDAODI_KICKOUT_SC = 2227,								// ���߳�һվ����
		MT_YIZHANDAODI_LUCKYINFO_SC = 2228,								// ������Ϣ
		MT_YIZHANDAODI_TITLE_CHANGE_SC = 2229,							// ��ն��Ϣ
		MT_YIZHANDAODI_USER_INFO_SC = 2230,								// �û���Ϣ
		MT_YIZHANDAODI_RANKINIO_SC = 2231,								// ���а���Ϣ
		MT_WUXINGGUAJI_INFO_SC = 2232,									// ���л����Ϣ
		MT_WUXINGGUAJI_LUCKY_INFO_SC = 2233,							// ���л��������Ϣ
		MT_GUAJI_BOSS_BORN_NOTICE_SC = 2234,							// �һ�boss����֪ͨ
		MT_RA_BIPIN_CAPABILITY_INFO_SC = 2235,							// ������ƴս����Ϣ
		MT_RA_CHARGE_REPAYMENT_INFO_SC = 2236,							// ������ֵ������Ϣ
		MT_CSA_SUB_ACTIVITY_STATE_SC = 2237,							// �Ϸ���ӻ״̬
		MT_CSA_ACTIVITY_INFO_SC = 2238,									// �Ϸ����Ϣ
		MT_CSA_ROLE_INFO_SC = 2239,										// �Ϸ����ɫ��Ϣ
		MT_CSA_ROLL_RESULT_SC = 2240,									// ҡ�����
		MT_RA_DANBICHONGZHI_INFO_SC = 2241,								// �������ʳ�ֵ��Ϣ
		MT_RA_TOTAL_CHARGE_DAY_INFO_SC = 2242,							// �����ۼƳ�ֵ����������Ϣ
		MT_RA_TOMORROW_REWARD_INFO_SC = 2243,							// �������ո�����Ϣ
		MT_RA_DAY_CHONGZHI_RANK_INFO_SC = 2244,							// ����ÿ�ճ�ֵ������Ϣ
		MT_RA_DAY_CONSUME_RANK_INFO_SC = 2245,							// ����ÿ������������Ϣ
		MT_RA_TOTAL_CHARGE_INFO_SC = 2246,								// �����ۼƳ�ֵ��Ϣ
		MT_RA_TIME_LIMIT_EXCHANGE_EQUI_INFO_SC = 2247,					// ����װ���һ���Ϣ
		MT_RA_TIME_LIMIT_EXCHANGE_JL_INFO_SC = 2248,					// ��������һ���Ϣ
		MT_ACTIVIT_MILLIONAIRE_TIME_NOTICE_SC = 2249,					// �󸻺����Ϣ

		MT_XIANNV_CALL_CS = 2250,										// �ٻ���Ů
		MT_XIANNV_RENAME_CS = 2251,										// ��Ů����
		MT_XIANNV_ACTIVE_REQ_CS = 2252,									// ����
		MT_XIANNV_ADD_ZIZHI_REQ_CS = 2253,								// ������
		MT_RAND_ACTIVITY_OPERA_REQ_CS = 2257,							// �����������
		MT_YIZHANDAODI_GUWU_CS = 2258,									// һվ���׹���
		MT_WUXINGGUAJI_GET_REWARD_CS = 2259,							// ���йһ���ȡ����
		MT_XIANNV_HALO_UPGRTADE_CS = 2260,								// ��Ů�������⻷
		MT_XIANNV_HALO_USE_IMAGE_CS = 2261,								// ��Ů��ʹ�ù⻷����
		MT_RA_NEW_TOTAL_CHARGE_INFO_SC = 2262,							// �������ۼƳ�ֵ��Ϣ
		MT_CSA_ROLE_OPERA_REQ_CS = 2263,								// �Ϸ����ɫ��������
		MT_XIANNV_UPGRADE_JIE_CS = 2264,								// ��Ů����
		MT_XIANNV_ACTIVE_HUANHUA_REQ_CS = 2265,							// ��Ů����û�
		MT_XIANNV_CHANGEIMAGE_REQ_CS = 2266,							// ��Ů�ı�����
		MT_XIANNV_HUANGUA_UPLEVEL_REQ_CS = 2267,						// ��Ů�û���������
		MT_XIANNV_CHANGE_SKILL_REQ_CS = 2268,							// ����ı���Ů����
		MT_ACTIVIT_MILLIONAIRE_USE_FROZEN_SKILL = 2269,					// �󸻺�ʹ�ñ�������
		MT_XIANNV_SHENGWU_REQ_CS = 2270,								// Ů��ʥ������
		MT_XIANNV_SHENGWU_CHANGE_INFO_SC = 2271,						// Ů��ʥ����Ϣ�������֪ͨ
		MT_XIANNV_SHENGWU_MILING_LIST_SC = 2272,						// Ů��ʥ�������б�
		MT_XIANNV_SHENGWU_CHOU_EXP_LIST_SC = 2273,						// Ů��ʥ��龭���б�
		MT_XIANNV_SHENGWU_CHOU_EXP_RESULT_SC = 2274,					// Ů��ʥ��龭���������ڿͻ��˲�����Ч��
		MT_YIZHANDAODI_LAST_FIRST_USER_INFO_SC = 2275,					// ��һ��һս���׵�һ����Ϣ
		MT_CSA_BOSS_INFO_SC = 2276,										// �Ϸ�boss��Ϣ
		MT_CSA_BOSS_RANK_SC = 2277,										// �Ϸ�boss���а�
		MT_CSA_BOSS_ROLE_INFO_SC = 2278,								// �Ϸ�boss��ɱ��
		MT_RA_LOTTERY_1_INFO_SC = 2279,									// ����-��ĩ����Ϣ
		MT_RA_ZERO_BUY_RETURN_SC = 2280,								// ����-��Ԫ����Ϣ
		MT_RA_KUANG_HAI_QING_DIAN_INFO_SC = 2281,						// ���������Ϣ
	
		MT_RA_GIFT_HARVEST_PLAYER_INFO_SC = 2282,						// �����ո�������Ϣ
		MT_RA_GIFT_HARVEST_RANK_INFO_SC = 2283,							// �����ո�����а�
		MT_RA_GIFT_HARVEST_SKILL_INFO_SC = 2284,						// �����ո�ļ�����Ϣ
		MT_RA_GIFT_HARVEST_ROUND_INFO_SC = 2285,						// �����ո���ִ�״̬
		MT_RA_TOTAL_CHARGE_MULTI_INFO_SC = 2286,						// �������ۼƳ�ֵ��Ϣ
		MT_RA_DOUBLE_GET_INFO_SC = 2287,								// ˫��Ԫ�����Ϣ
		MT_RA_SINGLE_CHARGE_INFO_MULTI_SC = 2288,						// ���ʳ�ֵ���Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_REQ_TRADE_ROUTE_SC = 2300,									// ת������������Է�
		MT_TRADE_PUT_GOLD_SC = 2301,									// ��Ԫ�����Ͻ��׼�
		MT_TRADE_PUT_ITEM_SC = 2302,									// ����Ʒ���Ͻ��׼�
		MT_TRADE_PUT_ITEM_PARAM_SC = 2303,								// ����Ʒ���Ͻ��׼� (��������Ʒ[װ��])
		MT_TRADE_STATE_SC = 2304,										// ����״̬����

		MT_REQ_TRADE_CS = 2350,											// ������ĳ�˽���
		MT_TRADE_RET_CS = 2351,											// �������󷵻�
		MT_TRADE_LOCK_REQ_CS = 2352,									// ����������
		MT_TRADE_AFFIRM_REQ_CS = 2353,									// ������ȷ��
		MT_TRADE_CANCLE_CS = 2354,										// ȡ������
		MT_TRADE_PUT_GOLD_REQ_CS = 2355,								// ����Ԫ�����Ͻ��׼�
		MT_TRADE_PUT_ITEM_REQ_CS = 2356,								// ������Ʒ���Ͻ��׼�

		MT_TUHAOJIN_USE_REQ_CS = 2399,									// ʹ������������

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_LEVEL_LOTTERY_INFO_SC = 2400,								// �����������齱��Ϣ
		MT_RA_LEVEL_LOTTERY_REWARD_LIST_SC = 2401,						// ����������������Ϣ
		MT_RA_CHARGE_NIU_EGG_INFO_SC = 2402,							// ������ֵŤ����Ϣ
		MT_RA_CHARGE_NIU_EGG_REWARD_SC = 2403,							// ������ֵŤ���齱�����Ϣ
		MT_RA_ZHENBAOGE_INFO_SC = 2404,									// ����-�䱦����Ϣ
		MT_TUHAOJIN_ALL_INFO_SC = 2405,									// ��������Ϣ
		MT_BIG_CHATFACE_ALL_INFO_SC = 2406,								// �������Ϣ
		MT_SHENZHOU_WEAPON_ALL_INFO_SC = 2407,							// ��������������Ϣ
		MT_SHENZHOU_WEAPON_GATHER_INFO_SC = 2408,						// ���������ɼ���Ϣ
		MT_RA_MIJINGXUNBAO_INFO_SC = 2409,								// �ؾ�Ѱ����Ϣ
		MT_RA_MIJINGXUNBAO_TAO_REWARD_INFO_SC = 2410,					// �ؾ�Ѱ�������Ϣ
		MT_RA_MONEY_TREE_INFO_SC = 2411,								// ҡǮ����Ϣ
		MT_RA_MONEY_TREE_CHOU_REWARD_INFO_SC = 2412,					// ҡǮ��������Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_BABY_INFO_SC = 2413,											// ����������Ϣ
		MT_BABY_ROUTE_SC = 2414,										// ����������Ϣת��
		MT_BABY_ALL_INFO_SC = 2415,										// ���б���������Ϣ
		MT_BABY_SPIRIT_INFO_SC = 2416,									// ����������Ϣ

		MT_BABY_OPERA_REQ_CS = 2417,									// ������������
		MT_BABY_UPGRADE_REQ_CS = 2418,									// ������������
		MT_BABY_RENAME_REQ_CS = 2419,									// ������������
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_RA_KING_DRAW_INFO_SC = 2420,									// ����-�����뷭�ƻ��Ϣ
		MT_RA_KING_DRAW_PLAY_TIMES_REWARD_SC = 2421,					// ����-�����뷭�ƻ�����εĽ�����Ϣ
		MT_RA_TOTAL_CHARGE_3_INFO_SC = 2422,							// ����-�ۼƳ�ֵ3
		MT_RA_TOTAL_CHARGE_3_INFO_CS = 2423,							// ����-�ۼƳ�ֵ3 ����
		MT_RA_TOTAL_CHARGE_4_INFO_CS = 2424,							// ����-�ۼƳ�ֵ4 ������Ϣ
		MT_RA_TOTAL_CHARGE_4_INFO_SC = 2425,							// �ۼƳ�ֵ4��Ϣ
		MT_RA_MINE_ALL_INFO_SC = 2426,									// Ȥζ�ڿ���Ϣ
		MT_OTHERCAPABILITY_INFO_SC = 2427,								// �����׼�����Ϣ
		MT_RA_GUAGUA_INFO_SC = 2428,									// ���ι���Ϣ
		MT_RA_GUAGUA_PLAY_TIMES_REWARD_SC = 2429,						// ����-���ιλ�����εĽ�����Ϣ
		MT_RA_TIAN_MING_DIVINATION_INFO_SC = 2430,						// ����-�������Ի��Ϣ
		MT_RA_TIANMING_DIVINATION_START_CHOU_RESULT_SC = 2431,  		// ����-�������Բ��Խ��
		MT_RA_FANFAN_ALL_INFO_SC = 2432,								// ����-����ת ������Ϣ
		MT_RA_FANFAN_WORD_EXCHANGE_RESULT_SC = 2433,					// ����-����ת �һ����
		MT_RA_CONTINUE_CHONGZHI_INFO_SC = 2434,							// ����-������ֵ
		MT_RA_CONTINUE_CONSUME_INFO_SC = 2435,							// ����-��������
		MT_RA_ARMY_DAY_INFO_SC = 2436,									// ����-��������������Ϣ
		MT_RA_CIRCULATION_CHONGZHI_INFO_SC = 2437,						// ����-ѭ����ֵ��Ϣ
		MT_RA_EXTREME_LUCKY_ALL_INFO_SC = 2438,							// ����-��������������Ϣ
		MT_RA_EXTREME_LUCKY_SINGLE_INFO_SC = 2439,						// ����-�������˵�����Ʒ��Ϣ
		MT_RA_MIJINGXUNBAO2_INFO_SC = 2440,								// �ؾ�Ѱ��2��Ϣ
		MT_RA_MIJINGXUNBAO2_TAO_REWARD_INFO_SC = 2441,					// �ؾ�Ѱ��2�����Ϣ
		MT_RA_MONEY_TREE2_INFO_SC = 2442,								// ҡǮ��2��Ϣ
		MT_RA_MONEY_TREE2_CHOU_REWARD_INFO_SC = 2443,					// ҡǮ��2������Ϣ
		MT_RA_ZHENBAOGE2_INFO_SC = 2444,								// �䱦��2
		MT_RA_PROMOTING_POSITION_ALL_INFO_SC = 2445,					// ��������������Ϣ
		MT_RA_PROMOTING_POSITION_REWARD_INFO_SC = 2446,					// ��������������Ʒ��Ϣ
		MT_RA_BLACK_MARKET_ALL_INFO_SC = 2447,							// ���о���������Ϣ
		MT_RA_TREASURES_MALL_ALL_INFO_SC = 2448,						// �䱦�̳�������Ϣ
		MT_GOLD_VIP_INFO_SC = 2451,										// �ƽ��Ա��Ϣ����
		MT_WORLD_BOSS_WEARY_INFO_SC = 2452,								// ����bossƣ��ֵ��Ϣ
		MT_RA_LEVEL_LOTTERY_ACTIVITY_INFO_SC = 2453,					// �������������Ϣ
		MT_RA_CONTINUE_CHONGZHI_INFO_CHU_SC = 2454,						// ����-�����ػݳ�
		MT_RA_CONTINUE_CHONGZHI_INFO_GAO_SC = 2455,						// ����-�����ػݸ�
		MT_RA_SINGLE_CHONGZHI_INFO_SC = 2456,							// ����-��������
		MT_RA_XIANYUAN_TREAS_INFO_SC = 2457,							// ����-�ۻ���
		MT_RA_RUSH_BUYING_ALL_INFO_SC = 2458,							// ����-��ʱ��ɱ������Ϣ
		MT_RA_MAP_HUNT_ALL_INFO_SC = 2459,								// ����-��ͼѰ��������Ϣ
		MT_RA_LIGHT_TOWER_ALL_INFO_SC = 2460,							// ����Ѱ��������Ϣ
		MT_RA_LIGHT_TOWER_DRAW_INFO_SC = 2461,							// ����Ѱ���齱��Ϣ
		MT_CSA_QUERY_ACTIVITY_INFO_CS = 2462,							// ��ѯ�Ϸ���Ϣ
		MT_RA_MAGIC_SHOP_ALL_INFO_SC = 2463,							// ��װ�̵���Ϣ����
		MT_RA_PROFESS_RANK_INFO_SC = 2464,								// ������л������Ϣ����
		MT_RA_CONTINUE_CHONGZHI_2_INFO_GAO_SC = 2466,					// ����-��ʱ����

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FIGHT_MOUNT_GOON_REQ_CS = 2500,								// ������/��ս������
		MT_FIGHT_MOUNT_UPGRADE_REQ_CS = 2501,							// ����ս���������
		MT_FIGHT_MOUNT_USE_IMAGE_CS = 2502,								// ����ʹ������
		MT_FIGHT_MOUNT_GET_MOUNT_INFO_CS = 2503,						// ����ս��������Ϣ
		MT_FIGHT_MOUNT_UPLEVEL_EQUIP_CS = 2504,							// ս����������װ������
		MT_FIGHT_MOUNT_SKILL_UPLEVEL_REQ_CS = 2505,						// ս�����＼����������
		MT_FIGHT_MOUNT_SPECIAL_IMG_UPGRADE_CS = 2506,					// ս�����������������
		MT_FIGHT_MOUNT_UP_STAR_LEVEL_CS = 2507,							// ս���������Ǽ�����

		MT_FIGHT_MOUNT_INFO_SC = 2550,									// ս��������Ϣ
		MT_FIGHT_MOUNT_APPE_CHANGE_SC = 2551,							// ս��������۸ı�

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SHENSHOU_OPERA_REQ_CS = 2560,								// ���޲�������
		MT_SHENSHOU_REQ_UPGRADE_EQUIP_CS = 2561,						// ��������ǿ��װ��
		MT_SHENSHOU_BACKPACK_INFO_SC = 2562,							// ���ޱ�����Ϣ
		MT_SHENSHOU_SHENSHOU_INFO_SC = 2563,							// ������Ϣ
		MT_SHENSHOU_SHENSHOU_BASE_INFO_SC = 2564,						// ���޻�����Ϣ
		MT_SHENSHOU_HUANLING_LIST_INFO_SC = 2565,						// ���޻�����Ϣ
		MT_SHENSHOU_HUANLING_DRAW_RESULT_SC = 2566,						// ���޻���齱���

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_IMG_FULING_INFO_SC = 2570,									// ��������Ϣ
		MT_IMG_FULING_OPERA_CS = 2571,									// �������������

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MENTALITY_LIST_SC=2700,										// ��������Ԫ����Ϣ
		MT_GUILDFB_REWARD_INFO_SC = 2701,								// ���˸���������Ϣ
		MT_RESERVE_2702_SC = 2702,										// ����
		MT_RESERVE_2703_SC = 2703,										// ����
		MT_RESERVE_2704_SC = 2704,										// ����
		MT_ROLE_SHENGWANG_CHANGE_SC = 2705,								// �����ı�
		MT_WELFARE_INFO_SC = 2706,										// ������Ϣ֪ͨ
		MT_ACTIVEDEFREE_INFO_SC = 2707,									// ��Ծ����Ϣ
		MT_HANGUP_SET_INFO_SC = 2708,									// �һ����÷���
		MT_ROLE_ZHANHUN_CHANGE_SC = 2709,								// ս��ı�
		MT_DAILY_BUY_COIN_INFO_SC = 2710,								// ��ȷ�����Ϣ
		MT_WELFARE_DAILYFIND_CHANGE_SC = 2711,							// �ճ��һص�����
		RESERVE_MT_DAY_TASK_COUNT_ACK_SC = 2712,						// ����
		MT_FUNCTION_SUCC_NOTICE_SC = 2713,								// ���졢�������ȹ��ܳɹ���ʾ
		MT_MENTALITY_YJTS_SC = 2714,									// һ��������Ӧ
		MT_ROLE_SUNDRY_INFO_SC = 2715,									// ������Ϣ
		MT_TEST_ACTIVITY_INFO_SC = 2716,								// ���ͷ����Ϣ
		MT_FUNCTION_FAIL_NOTICE_SC = 2717,								// ���졢�������ȹ���ʧ����ʾ
		MT_OPEN_GAME_ACTIVITY_INFO_SC = 2718,							// �����״̬
		MT_OPER_SUCC_SC = 2719,											// �����ɹ�
		MT_TOUZIJIHUA_INFO_SC = 2720,									// Ͷ�ʼƻ���Ϣ
		MT_SEVEN_DAY_LOGIN_REWARD_INFO_SC = 2721,						// �����¼������Ϣ
		MT_WELFARE_PLAT_VIP_INFO_SC = 2722,								// ƽ̨VIP������Ϣ
		MT_GENGU_INFO_SC = 2723,										// ������Ϣ
		MT_YAOJIANG_INFO_SC = 2724,										// ҡ����Ϣ
		MT_RESERVE_2725_SC = 2725,										// ����
		MT_SUPPLY_INFO_SC = 2726,										// �·�������Ϣ
		MT_TOTAL_LOGIN_DAYS_SC = 2727,									// �ܵ�¼����
		MT_TITLE_OWNER_INFO_SC = 2728,									// �ƺ�ӵ������Ϣ
		MT_SPECIAL_PARAM_CHANGE_SC = 2729,								// ����߽��볡��
		MT_GOLD_VIP_OPERA_REQ_CS = 2730,								// �ƽ��Ա��������
		MT_TOUZIJIHUA_FB_BOSS_OPERATE_CS = 2731,						// ������bossͶ�ʼƻ�����
		MT_TOUZIJIHUA_FB_BOSS_INFO_SC = 2732,							// ������bossͶ�ʼƻ���Ϣ

		MT_LOUD_SPEAKER_CS=2750,										// ʹ������ȫ���㲥
		MT_GET_OFFLINE_EXP_CS = 2751,									// ��ȡ���߾���
		MT_WELFARE_GET_DAILY_INFO_CS = 2752,							// ����ÿ�յ�½����
		MT_WELFARE_GET_DAILY_FIND_CS = 2753,							// �ճ��һ�
		MT_WELFARE_FETCH_SEVEN_DAY_LOGIN_REWARD_CS = 2754,				// ������ȡ�����¼����
		MT_WELFARE_UPLEVEL_REWARD_CS = 2755,							// ������ȡ������������
		MT_WELFARE_SIGN_IN_REWARD_CS = 2756,							// ������ȡǩ������
		MT_WELFARE_SIGN_IN_FIND_BACK_CS = 2757,							// ����ǩ���һ�
		MT_WELFARE_ACTIVITY_FIND_CS = 2758,								// �����һ�
		MT_WELFARE_AUTO_ACTIVITY_CS = 2759,								// ����ί��
		MT_MENTALITY_TRAIN_CS = 2760,									// ����Ԫ������
		MT_MENTALITY_UPGRADE_GENGU_CS = 2761,							// ����Ԫ���������
		MT_MENTALITY_SPEED_UP_TRAIN_CS = 2762,							// ������������
		MT_RESERVE_2763_CS = 2763,										// ����
		MT_RESERVE_2764_CS = 2764,										// ����
		MT_VIP_GET_WELFARE_CS = 2765,									// ��ȡVIP����
		MT_STRATEGIES_UPGRADE_CS = 2766,								// ��������
		MT_ACTIVEDEFREE_FETCH_REWARD_CS = 2767,							// ��ȡ��Ծ�Ƚ���
		MT_SAVE_HANGUP_SET_CS = 2768,									// ����һ���������
		MT_FETCH_VIP_LEVEL_REWARD = 2769,								// ��ȡvip�ȼ�����
		MT_DAILY_BUY_COIN_GET_CS = 2770,								// ��ȷ����ջ�
		MT_EVALUATE_ROLE_CS = 2771,										// ���۱���
		MT_SUPPLY_BUY_IYEM_CS = 2772,									// ��������
		MT_SUPPLY_SET_RECOVER_RANGE_PER_CS = 2773,						// ���ò����ٷֱ�
		MT_MENTALITY_YJTS_CS = 2774,									// һ������Ԫ��
		MT_QIFU_REQ_CS = 2775,											// ������
		MT_FETCH_QIFU_REWARD_CS = 2776,									// ����ȡ����
		MT_MENTALITY_CLEAR_TRAIN_CD_CS = 2777,							// Ԫ�����cd����
		MT_MARRY_REQ_CS = 2778,											// �������
		MT_MARRY_RET_CS = 2779,											// ���ظ�
		MT_FETCH_TOUZIJIHUA_REWARD_CS = 2780,							// ��ȡͶ�ʼƻ�����
		MT_TITLE_OWNER_INFO_REQ_CS = 2781,								// ����ƺ�ӵ������Ϣ
		MT_MARRY_HUNYAN_OPERA_CS = 2782,								// ���������
		MT_MARRY_HUNYAN_BLESS_CS = 2783,								// ������ף��
		MT_QINGYUAN_INFO_SC = 2784,										// ��Ե��Ϣ����
		MT_MENTALITY_UPLEVEL_WUXING_CS = 2785,							// �������еȼ�
		MT_TOUZIJIHUA_ACTIVE_CS = 2786,									// ����Ͷ�ʼƻ�
		MT_NEW_TOUZIJIHUA_OPERATE_CS = 2787,							// ��Ͷ�ʼƻ�����

		MT_CLIENT_SPECIAL_OPERATE_CS = 2788,							// �ͻ����������
		MT_SAVE_CUSTOM_SET_CS = 2789,									// �����Զ���������
		MT_ROLE_TASK_TRANSMIT_CS = 2790,								// ��ɫ������
		MT_TASK_OPEN_PANEL_CS = 2791,									// �����������
		MT_YAOJIANG_REQ_CS = 2792,										// ҡ������
		MT_MARRY_XUNYOU_OPERA_CS = 2793,								// ���Ѳ�β���
		MT_MARRY_HUNYAN_GET_BLESS_NEWS_CS = 2794,						// ��ȡ����ף����Ϣ
		MT_QINGYUAN_OPERA_CS = 2795,									// �����ز�������
		MT_RESET_LOVER_NAME_CS = 2796,									// �����������
		MT_DATING_INVITE_REQ_CS = 2797,									// ����Լ������
		MT_DATING_INVITE_ACK_CS = 2798,									// Լ�������
		MT_LIGHTEN_WEDDINGRING_CS = 2799,								// �������

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FISH_POOL_RECORD_AND_COMMONINFO_SC = 2800,					// ���ͨ����Ϣ����ʷ��¼
		MT_FISH_POOL_ALL_RAISE_INFO_SC = 2801,							// ��ط�����Ϣ
		MT_FISH_POOL_COMMON_INFO_SC = 2802,								// ���ͨ��������Ϣ
		MT_FISH_POOL_RAISE_INFO_CHANGE_SC = 2803,						// ��ط�����Ϣ�仯
		MT_FISH_POOL_WORLD_GENERAL_INFO_SC = 2804,						// ���������ؼ�Ҫ��Ϣ
		MT_FISH_POOL_STEAL_GENERAL_INFO_SC = 2805,						// ͵���߳�����Ҫ��Ϣ
		MT_WORLD_EVENT_COMMON_INFO_SC = 2806,							// �����¼�ͨ����Ϣ
		MT_WORLD_EVENT_ROLE_INFO_SC = 2807,								// �����¼���ɫ��Ϣ
		MT_WORLD_EVENT_NPC_LOCATE_INFO_SC = 2808,						// �����¼�NPCλ����Ϣ
		MT_WORLD_EVENT_KILL_FIND_NPC_MONSTER_SC = 2809,					// �����¼� ��ɱѰ��npc����
		MT_FISH_POOL_SHOU_FISH_REWARD_INFO_SC = 2810,					// ���㽱����Ϣ

		MT_FISH_POOL_RAISE_REQ_CS = 2850,								// �����������
		MT_FISH_POOL_BUY_BULLET_REQ_CS = 2851,							// ��ع����ӵ�����
		MT_FISH_POOL_QUERY_REQ_CS = 2852,								// ���������Ϣ����
		MT_FISH_POOL_STEAL_FISH_CS = 2853,								// ���͵������
		MT_FISH_POOL_HARVEST_FISH_CS = 2854,							// ����ջ�����
		MT_FISH_POOL_EXTEND_CAPACITY_CS = 2855,							// �����չ��������
		MT_WORLD_EVENT_COMMON_INFO_REQ_CS = 2856,						// �����¼�ͨ����Ϣ����
		MT_WORLD_EVENT_QUERY_ROLE_INFO_CS = 2857,						// �����¼������ɫ��Ϣ
		MT_WORLD_EVENT_FETCH_REWARD_CS = 2858,							// �����¼���ȡ����
		MT_WORLD_EVENT_OBJ_TOUCH_CS = 2859,								// ���������¼���������
		MT_WORLD_EVENT_LIGHT_WORD_CS = 2860,							// �����¼�������
		MT_WORLD_EVENT_GET_NPC_LOCATE_INFO_CS = 2861,					// �����¼����NPCλ����Ϣ

		MT_FISH_POOL_UP_FISH_QUALITY_RET_SC = 2862,						// ������Ʒ�ʽ��
		MT_FISH_POOL_FISH_POOL_CHANGE = 2863,							// �������仯

		MT_MARRY_RET_INFO_SC = 2864,									// �������������

		MT_PROFESS_WALL_REQ_CS = 2865,									// ���ǽͨ������
		MT_PROFESS_TO_REQ_CS = 2866,									// �������
		MT_GLOBAL_PROFESS_WALL_INFO_SC = 2867,							// �������ǽ��Ϣ
		MT_PERSON_PROFESS_WALL_INFO_SC = 2868,							// ���˱��ǽ��Ϣ
		MT_PROFESS_EFFECT_SC = 2869,									// ���˱��ǽ��Ч��Ϣ	
		MT_PROFESS_LEVEL_INFO_SC = 2870,								// ��׵ȼ���Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_GM_COMMAND_SC = 3000,
		MT_GM_TESTSPEED_SC = 3001,										// ����

		MT_GM_COMMAND_CS = 3050,
		MT_GM_TESTSPEED_CS = 3051,										// ����

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ��һЩͨ�õ�Э��

		MT_DRAW_RESULT_SC = 3100,										// �齱���

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ALL_TITLE_LIST_SC = 3600,									// �������гƺ��б�
		MT_USED_TITLE_LIST_SC = 3601,									// ����ʹ�õĳƺ��б�
		MT_USED_TITLE_CHANGE_SC = 3602,									// ʹ�õĳƺű仯�㲥

		MT_GET_ALL_TITLE_CS = 3650,										// �������гƺ��б�
		MT_USE_TITLE_CS = 3651,											// ����ʹ�óƺ�
		MT_TITLE_UPGRADE_CS = 3652,										// ����ƺŽ���

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MAIL_SEND_CS = 3750,											// �����ʼ�
		MT_MAIL_DELETE_CS = 3751,										// ɾ���ʼ�
		MT_MAIL_GETLIST_CS = 3752,										// ��ȡ�ʼ��б�
		MT_MAIL_READ_CS = 3753,											// ��ȡ�ʼ�
		MT_MAIL_ATTACHMENT_CS = 3754,									// ��ȡ����
		MT_MAIL_CLEAN_CS = 3755,										// ����ʼ�
		MT_MAIL_ONE_KEY_FETCH_ATTACHMENT = 3756,						// һ����ȡ�ʼ�

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_GUILD_BACK_TO_STATION_SC = 4200,								// ���ſ��ٻ�Ӫ���
		MT_GUILD_ADD_GUILD_EXP_SC = 4201,								// ���ž��׽��
		MT_GUILD_XIANSHU_INFO_SC = 4202,								// ����������Ϣ
		MT_GUILD_FB_STATUS_SC = 4205,									// �����ܾ�״̬��Ϣ
		MT_GUILD_BOSS_SC = 4206,										// ����boss��Ϣ
		MT_GUILD_BONFIRE_STATUS_SC = 4208,								// ��������״̬��Ϣ
		MT_GUILD_STORGE_SC = 4209,										// ���˲ֿ���Ϣ
		MT_GUILD_STORGE_CHANGE_SC = 4210,								// ���˲ֿ�ı�
		MT_GUILD_BOX_INFO_SC = 4211,									// ���˱�����Ϣ
		MT_GUILD_BOX_NEED_ASSIST_INFO_SC = 4212,						// ���˱�����ҪЭ����Ϣ
		MT_GUILD_BOSS_ACTIVITY_SC = 4213,								// ����boss���Ϣ
		MT_GUILD_RED_PAPER_REQ_LIST_INFO_CS = 4214,						// ���˺�����������б�
		MT_GUILD_RED_PAPER_LIST_INFO_SC = 4215,							// ���˺��������б���Ϣ
		MT_GUILD_STORGE_LOG_LIST_SC = 4216,								// ���˲ֿ���־��Ϣ

		MT_CREATE_GUILD_CS = 4250,										// ��������
		MT_DISMISS_GUILD_CS = 4251,										// ��ɢ����
		MT_APPLY_FOR_JOIN_GUILD_CS = 4252,								// ����������
		MT_APPLY_FOR_JOIN_GUILD_ACK_CS = 4253,							// �ظ���������
		MT_QUIT_GUILD_CS = 4254,										// �����˳�����
		MT_INVITE_GUILD_CS = 4255,										// ����������
		MT_INVITE_GUILD_ACK_CS = 4256,									// �ظ�����
		MT_KICKOUT_GUILD_CS = 4257,										// ��������
		MT_APPOINT_GUILD_CS = 4258,										// ����
		MT_LEAVE_GUILD_POST_CS = 4259,									// ж��
		MT_CHANGE_GUILD_NOTICE_CS = 4260,								// �޸ľ��Ź���
		MT_GUILD_MAIL_ALL_CS = 4261,									// Ⱥ���ʼ�
		MT_GET_GUILD_INFO_CS = 4262,									// ��ȡ���������Ϣ
		MT_GUILD_DELATE_CS = 4263,										// ����
		MT_GUILD_BACK_TO_STATION_CS = 6264,								// ���ٻ�Ӫ
		MT_GUILD_RESET_NAME_CS = 4265,									// ���Ÿ�������
		MT_GUILD_APPLYFOR_SETUP_CS = 4266,								// ���ľ�������
		MT_GUILD_ADDEXP_CS = 4267,										// �����żӾ���
		MT_GUILD_SET_AUTO_KICKOUT_SETUP_CS = 4268,						// ���������Ƿ��Զ�����
		MT_GUILD_CALL_IN_CS = 4269,										// ������ļ
		MT_GUILD_XIANSHU_INFO_REQ_CS = 4270,							// ���������Ϣ
		MT_GUILD_XIANSHU_UPLEVEL_CS = 4271,								// ��������
		MT_GUILD_CHECK_CAN_DELATE_CS = 4272,							// ����Ƿ���Ե���
		MT_GUILD_FB_START_REQ_CS = 4275,								// �����ܾ���������
		MT_GUILD_FB_ENTER_REQ_CS = 4276,								// �����ܾ���������
		MT_GUILD_ZHUFU_LUCKY_REQ_CS = 4277,								// ����ף������
		MT_GUILD_GET_GUILD_ALL_LUCKY_REQ_CS = 4278,						// ����ף������
		MT_GUILD_INVITE_ZHUFU_LUCKY_CS = 4279,							// ��������ף������
		MT_GUILD_CALL_BEAST_CS = 4280,									// �����ٻ���������
		MT_GUILD_ACTIVE_DEGREE_INFO_CS = 4281,							// ���˻�Ծ����Ϣ����
		MT_GUILD_UP_LEVEL_CS = 4282,									// ��������
		MT_GUILD_GET_BUFF_CS = 4283,									// ��ȡbuff
		MT_GUILD_EXCHANGE_CS = 4284,									// �һ���Ʒ
		MT_GUILD_BOSS_CS = 4285,										// ����boss��������
		MT_GUILD_BONFIRE_START_REQ_CS = 4286,							// ��������������
		MT_GUILD_BONFIRE_GOTO_REQ_CS = 4287,							// ��������ǰ������
		MT_GUILD_BONFIRE_ADDMUCAI_REQ_CS = 4288,						// �����������ľ������
		MT_GUILD_STORGE_REQ_CS = 4289,									// ���˲ֿ��������
		MT_GUILD_SKILL_UPLEVEL_CS = 4290,								// ���˼�����������
		MT_GUILD_TOTEM_UP_LEVEL_CS = 4291,								// ����ͼ������
		MT_GUILD_BOX_OPERATE_CS = 4292,									// ���˱������
		MT_GUILD_COMMON_REQ_CS = 4293,									// ����ͨ������
		MT_GUILD_FETCH_TERRITORY_REWARD_CS = 4294,						// ������ؽ���
		MT_GUILD_STORGE_ONE_KEY_REQ_CS = 4295,							// ���˲ֿ�һ����������
		MT_GUILD_EXTEND_MEMBER_CS = 4296,								// ������չ��Ա����
		MT_GUILD_FETCH_GUILD_BOSS_REDBAG = 4297,						// �칫��ɱboss���
		MT_GUILD_FETCH_GUILD_RELIVE_TIMES_INFO = 4298,					// ���Ḵ�������Ϣ
		MT_GUILD_FETCH_GUILD_BOSS_REDBAG_INFO = 4299,					// ����boss�����Ϣ
		MT_GUILD_FETCH_GUILD_BOSS_FETCH_INFO = 4300,					// ����boss�����Ա��ȡ��Ϣ
		MT_GUILD_FETCH_GUILD_SAIZI_INFO = 4301,							// ����������Ϣ
		MT_GUILD_FETCH_GUILD_PAO_SAIZI = 4302,							// ����������
		MT_GUILD_FETCH_GUILD_SAIZI_INFO_REQ = 4303,						// ����������Ϣ
		MT_GUILD_SET_AUTO_CLEAR_REQ = 4304,								// �����Զ��������Ա

		MT_GUILD_TIANCI_TONGBI_RESULT_SC = 4305,						// �������ͭ�ҽ��
		MT_GUILD_SYNC_TIANCI_TONGBI = 4306,								// ����ͬ�����ͭ��״̬
		MT_GUILD_TIANCI_TONGBI_USER_GATHER_CHANGE_SC = 4307,			// �������ͭ����Ҳɼ���ı�
		MT_GUILD_TIANCI_TONGBI_RANK_INFO_SC = 4308,						// �������ͭ�����а�
		MT_GUILD_TIANCI_TONGBI_REQ_CS = 4309,							// �������ͭ�ҿ�������
		MT_GUILD_TIANCI_TONGBI_NPC_INFO = 4310,							// �������ͭ��npc��Ϣ
		MT_GUILD_TIANCI_TONGBI_AOI_GATHER_INFO = 4311,					// �������ͭ�Ҳɼ������㲥
		MT_GUILD_GONGZI_LIST_INFO = 4312,								// ���˹����б���Ϣ
		MT_GUILD_ENEMY_RANK_LIST_SC = 4313,								// �·����˳����б���Ϣ
		MT_GUILD_ENEMY_RANK_LIST_CS = 4314,								// ��ȡ���˳����б�

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ADD_PUBLICSALE_ITEM_CS = 4450,								// ������Ʒ�ϼ�
		MT_REMOVE_PUBLICSALE_ITEM_CS = 4451,							// ������Ʒ�¼�
		MT_BUY_PUBLICSALE_BUY_CS = 4452,								// ����������Ʒ
		MT_PUBLICSALE_GET_ITEM_LIST_CS = 4453,							// ����Լ�������������Ʒ��Ϣ
		MT_PBULICSALE_SEND_ITEM_INFO_TO_WORLD_CS = 4454,				// �����Լ���������Ʒ��Ϣ���������촰
		MT_PBULICSALE_TYPE_COUNT_REQ_CS = 4455,							// ��������Ʒ��������
		MT_PBULICSALE_TYPE_COUNT_ACK_CS = 4456,							// ����Ʒ���������·�
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SYSTEM_ROLE_MSG_REPLY = 4500,								// ����ͻ�����������첽����,�ͻ��˽ӵ�ָ��reply�ż�������

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MOUNT_INFO_SC = 4700,										// ����������Ϣ
		MT_MOUNT_APPE_CHANGE_SC = 4701,									// ������۸ı�
		MT_WING_INFO_SC = 4703,											// ������Ϣ
		MT_WING_RESERVE_1_SC = 4704,									// ����ı�����
		MT_DISCOUNT_BUY_INFO_SC = 4706,									// һ��������Ϣ
		MT_MONTH_CARD_INFO_SC = 4707,									// �¿���Ϣ
		MT_WING_POINT_INCREASE = 4708,									// �����������
		MT_ROLL_MONEY_INFO_SC = 4709,									// ҡǮ��Ϣ
		MT_ROLE_GOAL_INFO_SC = 4710,									// ��ɫĿ����Ϣ
		MT_ROLE_GOAL_CHANGE_SC = 4711,									// ��ɫĿ����Ϣ�ı�
		MT_RED_PAPER_DETAIL_INFO_SC = 4712,								// �����ϸ��Ϣ
		MT_RED_PAPER_FETCH_RESULT_SC = 4713,							// �����ȡ���
		MT_FAKE_PRIVATE_CHAT_SC = 4714,									// α��˽��
		MT_MARRIAGE_SEEKING_INFO_SC = 4715,								// �������Խ����Ϣ
		MT_GUILD_GUILD_ALL_LUCKY_SC = 4716,								// ���˳�Ա��������Ϣ����
		MT_GUILD_INVITE_ZHUFU_LUCKY_SC = 4717,							// ��������ף��֪ͨ
		MT_GUILD_LUCKY_CHANGE_NOTICE_SC = 4718,							// ����ף�����֪ͨ
		MT_SHIZHUANG_INFO_SC = 4719,									// ʱװ��Ϣ
		MT_CARD_INFO_SC = 4720,											// ������Ϣ
		MT_CARD_UPLEVEL_SUCC_SC = 4721,									// ���������ɹ�
		MT_ROLE_SHADOW_BOSS_INFO_SC = 4722,								// Ӱ��boss��Ϣ
		MT_GUILD_ACTIVE_DEGREE_INFO_SC = 4723,							// ���˻�Ծ����Ϣ
		MT_QINGYUAN_CARD_LEVEL_LIST_SC = 4724,							// ��Ե���Ƶȼ��б�
		MT_QINGYUAN_CARD_LEVEL_UPDATE_SC = 4725,						// ��Ե���Ƶȼ����
		MT_RED_PAPER_ROLE_INFO_SC = 4726,								// ���˺����Ϣ
		MT_RED_PAPER_SEND_COMMAND_CS = 4727,							// ����������
		MT_RED_PAPER_COMMAND_REWARD_CS = 4728,							// ��ȡ������
		MT_RED_PAPER_COMMAND_CHECK_INFO_CS = 4729,						// �鿴������
		MT_RED_PAPER_COMMAND_SEND_INFO_SC = 4730,						// ���Ϳ�������Ϣ

		MT_MOUNT_GOON_REQ_CS = 4750,									// ������/������
		MT_MOUNT_UPGRADE_REQ_CS = 4751,									// �����������
		MT_MOUNT_USE_IMAGE_CS = 4752,									// ����ʹ������
		MT_MOUNT_SPECIAL_IMG_UPGRADE_CS = 4753,							// ���������������
		MT_MOUNT_UPLEVEL_EQUIP_CS = 4754,								// ��������װ������
		MT_MOUNT_SKILL_UPLEVEL_REQ_CS = 4755,							// ���＼����������
		MT_MOUNT_GET_MOUNT_INFO_CS = 4756,								// ����������Ϣ
		MT_WING_UPGRADE_REQ_CS = 4757,									// �����������
		MT_MOUNT_UP_STAR_LEVEL_CS = 4758,								// �������Ǽ�����
		MT_DISCOUNT_BUY_GET_INFO_CS = 4760,								// ���һ��������Ϣ
		MT_DISCOUNT_BUY_REQ_BUY_CS = 4761,								// һ������ ����
		MT_ROLL_MONEY_OPERA_REQ_CS = 4762,								// ҡԪ������
		MT_ZHANSHENDIAN_FETCH_DAY_REWARD_CS = 4763,						// ��ȡս���ÿ�ս���
		MT_ROLE_GOAL_OPERA_REQ_CS = 4764,								// ��ɫĿ���������
		MT_RED_PAPER_CREATE_REQ_CS = 4765,								// �����������
		MT_RED_PAPER_FETCH_REQ_CS = 4766,								// ��ȡ�������
		MT_RED_PAPER_QUERY_DETAIL_REQ_CS = 4767,						// ��ѯ�����ϸ��Ϣ����
		MT_MARRIAGE_SEEKING_REQ_CS = 4768,								// ��ѯ�����ϸ��Ϣ����
		MT_SHIZHUANG_USE_REQ_CS = 4771,									// ʱװʹ������
		MT_CARD_OPERATE_CS = 4772,										// ���Ʋ�������
		MT_MONTH_CARD_FETCH_DAY_REWARD_CS = 4773,						// �¿�����ȡÿ�ս���
		MT_WING_USE_IMAGE_CS = 4774,									// ��������ʹ������
		MT_WING_GET_INFO_CS = 4775,										// ����������Ϣ
		MT_WING_UP_STAR_LEVEL_CS = 4776,								// �������Ǽ�����
		MT_DISCONNECT_REQ_CS = 4778,									// ��������
		MT_ZHUANZHI_REQ_CS = 4779,										// תְ����
		MT_QINGYUAN_CARD_UPGRADE_REQ_CS = 4780,							// ��Ե������������
		MT_SHIZHUANG_UPGRADE_REQ_CS = 4781,								// ʱװ��������
		MT_QINGYUAN_COUPLE_HALO_INFO_SC = 4783,							// ��Ե���޹⻷��Ϣ
		MT_QINGYUAN_COUPLE_HALO_TRIGGER_SC = 4784,						// ��Ե���޹⻷������Ϣ
		MT_QINGYUAN_COUPLE_HALO_OPERA_REQ_CS = 4785,					// ���޹⻷����
		MT_WING_UPLEVEL_EQUIP_CS = 4786,								// ��������װ������
		MT_WING_SKILL_UPLEVEL_REQ_CS = 4787,							// ��������������
		MT_WING_SPECIAL_IMG_UPGRADE_CS = 4789,							// ���������������
		MT_QINGYUAN_BLESS_REWARD_REQ_CS = 4790,							// ��Եף����������
		MT_QINGYUAN_ADD_BLESSDAYS_REQ_CS = 4791,						// ������Եף������
		MT_LOVE_CONTRACT_OPERA_REQ_CS = 4792,							// ������Լ��������
		MT_QINGYUAN_FETCH_LOVE_CONTRACT_CS = 4793,						// ��ȡ������Լ
		MT_QINGYUAN_LOVE_CONTRACT_INFO_SC = 4794,						// ������Լ��Ϣ
		MT_RESERVE_4795_CS = 4795,										// ����
		MT_ZERO_GIFT_INFO_SC = 4796,									// ��Ԫ�����Ϣ
		MT_ZERO_GIFT_OPERATE_REQ_CS = 4797,								// ��Ԫ�������
		MT_ADVANCE_NOTIC_OPERATE_CS = 4798,								// ����Ԥ�����
		MT_ADVANCE_NOTICE_INFO_SC = 4799,								// ����Ԥ����Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CHEST_SHOP_SELF_ITEM_LIST_SC = 4800,							// �����Լ������̵���Ʒ�б�
		MT_CHEST_SHOP_ITEM_LIST_PER_BUY_SC = 4801,						// ����ÿ�ο�����õ�����Ʒ
		MT_CHEST_SHOP_FREE_INFO_SC = 4802,								// ���ͱ��������Ϣ
		MT_CONVERT_RECORD_INFO_SC = 4803,								// �һ���¼��Ϣ
		MT_MYSTERIOUS_NPC_REFRESH_SC = 4804,							// �����̵�����ˢ��
		MT_MYSTERIOUS_SHOPITEM_INFO_SC = 4805,							// �����̵���Ϣ
		MT_RARE_CHEST_SHOP_BUY_CS = 4806,								// �����챦�齱����
		MT_MYSTERIOUS_SHOP_BUY_OPERATE_CS = 4807,						// �����̵�������Ϣ
		MT_MYSTERIOUS_SHOP_SEQ_INFO_SC= 4808,							// �����̵���Ʒ��Ϣ
		MT_SHIZHUANG_SPECIAL_IMG_UPGRADE_CS = 4809,						// ʱװ-����������������

		MT_CHEST_SHOP_GET_SELF_ITEM_LIST_CS = 4850,						// ��ȡ�Լ������̵���Ʒ�б�
		MT_CHEST_SHOP_BUY_ITEM_CS = 4851,								// �����̵깺��
		MT_CHEST_SHOP_FETCH_ITEM_CS = 4852,								// ��ȡ�����̵���Ʒ
		MT_SHOP_BUY_ITEM_CS = 4854,										// �����̳���Ʒ
		MT_CONVERT_SCORE_TO_ITEM_CS = 4855,								// �һ��̵����Ļ��ֶһ���Ʒ
		MT_CHEST_SHOP_GET_FREE_INFO = 4856,								// ��ȡ�����̵����ʱ��
		MT_CONVERT_RECORD_GET_INFO_CS = 4857,							// �һ���¼��Ϣ����
		MT_CHEST_SHOP_AUTO_RECYCLE_CS = 4858,							// �����Զ�����
		MT_MYSTERIOUSSHOP_OPERATE_CS = 4859,							// �������˲���
		MT_CONVERT_GET_SOCRE_INFO_CS = 4860,							// ��ȡ������Ϣ����
		MT_CONVERT_GET_SOCRE_INFO_SC = 4861,							// ���ͻ�����Ϣ
		MT_CONVERT_GET_SOCRE_INFO_NOTICE_SC = 4862,						// ���ͻ�����Ϣ���������½���ʾ
		MT_CHEST_SHOP_GET_RECORD_LIST_CS = 4863,						// ��ȡѰ����Ʒ��¼�б�
		MT_CHEST_SHOP_SEND_RECORD_LIST_SC = 4864,						// ����Ѱ����Ʒ��¼�б�

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_QUNXIANLUANDOU_LUCK_REWARD_SC = 4950,						// Ⱥ���Ҷ� ���˽���
		MT_QUNXIANLUANDOU_PICKUP_SHENSHI_CS = 4951,						// Ⱥ���Ҷ� ������ʯ����
		MT_GET_BATTLEFIELD_HONOR_INFO_REQ_CS = 4952,					// ����ս��������Ϣ
		MT_XIANMENGZHAN_GUILD_CALL_CS = 4953,                   		// ����ս �����ٻ�
		MT_XIANMENGZHAN_GUILD_FOLLOW_CS = 4954,                 		// ����ս ���Ѹ���
		MT_NATIONAL_BOSS_BUY_BUFF_CS = 4955,							// ����boss ����Buff����
		MT_OPEN_GAME_ACTIVITY_REQ_INFO_CS = 4956,						// �����������Ϣ
		MT_OPEN_GAME_ACTIVITY_FETCH_REWARD_CS = 4957,					// �������ݰ��Ǹ�����
		MT_CHALLENGE_FIELD_GET_USER_INFO_CS = 4958,						// ���������������û���Ϣ
		MT_CHALLENGE_FIELD_RESET_OPPONENT_REQ_CS = 4959,				// ����ˢ�¾�����������Ϣ
		MT_CHALLENGE_FIELD_FIGHT_REQ_CS = 4960,							// ������������������
		MT_CHALLENGE_FIELD_FETCH_JIFEN_REWARD_REQ_CS = 4961,			// ������������ȡ���ֽ���
		MT_CHALLENGE_FIELD_GET_RANK_INFO_CS = 4962,						// ��þ�����������Ϣ
		MT_CHALLENGE_FIELD_GET_LUCKY_INFO_CS = 4963,					// ��þ���������������Ϣ
		MT_CHALLENGE_FIELD_FETCH_GUANGHUI_CS = 4965,					// ������������ȡ���
		MT_CHALLENGE_FIELD_BUY_JOIN_TIMES_CS = 4966,					// ����������
		MT_CHALLENGE_FIELD_GET_OPPONENT_INFO_CS = 4967,					// ��ö�����Ϣ
		MT_CLOSE_BETA_ACTIVITY_OPERA_REQ_CS = 4968,						// �����������
		MT_LUCKY_ROLL_ACTIVITY_OPERA_REQ_CS = 4969,						// ����ת�̻��������
		MT_UPDATE_NOTICE_FETCH_REWARD_CS = 4970,						// ���¹�����ȡ��������
		MT_COMMON_ACTIVITY_FETCH_REWARD_CS = 4971,						// ��ȡͨ�û����
		MT_CHALLENGE_FIELD_BUY_BUFF_CS = 4972,							// ��������������buff
		MT_GUILD_BATTLE_DAILY_REWARD_FETCH_CS = 4975,					// ����������ȡÿ�ս���
		MT_GUILD_BATTLE_DAILY_REWARD_INFO_SC = 4976,					// ����������ȡÿ�ս�����Ϣ

		MT_BATTLEFIELD_HONOR_CHANGE_SC = 4902,							// ս������ֵ�ı�
		MT_QUNXIANLUANDOU_USER_INFO_SC = 4903,							// Ⱥ���Ҷ� �û���Ϣ
		MT_QUNXIANLUANDOU_RANK_INFO_SC = 4904,							// Ⱥ���Ҷ� ���а���Ϣ
		MT_QUNXIANLUANDOU_SIDE_INFO_SC = 4905,							// Ⱥ���Ҷ� ��Ӫ��Ϣ
		MT_QUNXIANLUANDOU_SHENSHI_STATUS_SC = 4906,						// Ⱥ���Ҷ� ��ʯ״̬
		MT_XIANMENGZHAN_USER_INFO_SC = 4907,                    		// ����ս �û���Ϣ
		MT_XIANMENGZHAN_GUILD_INFO_SC = 4908,                   		// ����ս ������Ϣ
		MT_XIANMENGZHAN_DEFEND_AREA_INFO_SC = 4909,             		// ����ս �ݵ���Ϣ
		MT_XIANMENGZHAN_GUILD_CALL_NOTICE_SC = 4910,            		// ����ս �����ٻ�֪ͨ
		MT_XIANMENGZHAN_RESULT_SC = 4911,								// ����ս ���֪ͨ
		MT_XIANMENGZHAN_DANYAO_BOSS_INFO_SC = 4912,						// ����ս ��ҩ����Ϣ
		MT_XIANMENGZHAN_DEFEND_AREA_BE_ATTACK_NOTICE_SC = 4913,			// ����ս �ݵ㱻����
		MT_CHONG_ZHI_INFO_SC = 4914,									// ��ֵ��Ϣ
		MT_NATIONAL_BOSS_USER_INFO_SC = 4915,							// ȫ��boss �û���Ϣ
		MT_NATIONAL_BOSS_RANK_INFO_SC = 4916,							// ȫ��boss ������Ϣ
		MT_CHALLENGE_FIELD_STATUS_INFO_SC = 4917,						// ������1v1����״̬��Ϣ
		MT_CHALLENGE_FIELD_USER_INFO_SC = 4918,							// ������1v1�û���Ϣ
		MT_CHALLENGE_FIELD_OPPONENT_RANK_POS_CHANGE = 4919,				// ���ֵ����������仯
		MT_CHALLENGE_FIELD_REPORT_INFO_SC = 4920,						// ������ս����Ϣ
		MT_CHALLENGE_FIELD_RANK_INFO_SC = 4921,							// ������������Ϣ
		MT_CHALLENGE_FIELD_LUCKY_POS_INFO_SC = 4922,					// ������������Ϣ
		MT_GCZ_BUY_WEAPON_CS = 4923,									// ��������
		MT_QUNXIANLUANDOU_LIANZHAN_CHANGE_SC = 4928,					// Ⱥ���Ҷ� ��ն���ı�
		MT_XIANMENGZHAN_LIANZHAN_CHANGE_SC = 4930,						// ����ս��ն�����ı�
		MT_CHALLENGE_FIELD_OPPONENT_INFO_SC = 4931,						// ������1v1������ϸ��Ϣ
		MT_CHALLENGE_BE_DEFEAT_NOTICE_SC = 4932,						// �����������֪ͨ
		MT_CLOSE_BETA_ACTIVITY_INFO_SC = 4933,							// �����Ϣ
		MT_LUCKY_ROLL_ACTIVITY_INFO_SC = 4934,							// ����ת�̻��Ϣ
		MT_LUCKY_ROLL_ACTIVITY_ROLL_RESULT_SC = 4935,					// ����ת�̽������
		MT_UPDATE_NOTICE_INFO_SC = 4936,								// ���¹�����Ϣ
		MT_CHALLENGE_FIELD_REWARD_BIND_GOLD_INFO_SC = 4937,				// ������ʤ������Ԫ����Ϣ
		MT_COMMON_ACTIVITY_OPEN_UNIT_INFO_SC = 4938,					// ͨ�û������UNIT��Ϣ
		MT_XIANMENGZHEN_GUILD_RANK_INFO_SC = 4939,						// ����ս����������Ϣ
		MT_XINGZUOYIJI_BOX_AND_BOSS_INFO_SC = 4940,						// �����ż�������boss��Ϣ
		MT_CHALLENGE_FIELD_WIN = 4941,									// 1V1������ֱ��ʤ��

		MT_CHALLENGE_FIELD_BEST_RANK_BREAK_REQ_CS = 4942,				// 1V1��������ʷ���ͻ������
		MT_CHALLENGE_FIELD_BEST_RANK_BREAK_INFO_SC = 4943,				// 1V1��������ʷ���ͻ����Ϣ
		MT_CHALLENGE_FIELD_READY_START_FIGHT_CS = 4944,					// 1V1����������׼����ʽ��ʼս������ʱ

		MT_QUNXIANLUANDOU_RANK_FIRST_REQ_CS = 4945,						// Ⱥ���Ҷ���һ��������
		MT_QUNXIANLUANDOU_RANK_FIRST_INFO_SC = 4946,					// Ⱥ���Ҷ���һ������Ϣ


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_DAYCOUNTER_INFO_SC = 5000,									// ���ռ�����Ϣ
		MT_DAYCOUNTER_ITEM_INFO_SC = 5001,								// ���ռ���������Ϣ
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MOJIE_INFO_SC = 5050,										// ����ħ����Ϣ

		M_MOJIE_INFO_REQ_CS = 5075,										// ����ħ����Ϣ
		M_MOJIE_UP_LEVEL_REQ_CS = 5076,									// ��������ħ��
		M_MOJIE_CHANGE_SKILL_REQ_CS = 5077,								// ����ı�ħ�似��

		//////////////////////////////////////////////////////////////////////////
		MT_SUBLOCK_OPERATE_SUCC_SC = 5100,								// ����������ȷ
		MT_SUBLOCK_OPERATE_FAIL_SC = 5101,								// ����������ʧ��
		MT_SUBLOCK_INFO_SC = 5102,										// ��������Ϣ

		MT_SUBLOCK_CREATE_PW_CS = 5150,									// ��������
		MT_SUBLOCK_CLEAR_PW_CS = 5151,									// �������
		MT_SUBLOCK_LOCK_CS = 5152,										// ����
		MT_SUBLOCK_UNLOCK_CS = 5153,									// ����

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ACTIVITY_ENTER_REQ_CS = 5253,								// �������
		MT_ZHUXIE_FETCH_TASK_REWARD_CS = 5254,							// ��ȡ��аս��������
		MT_SEND_GUILD_SOS_REQ = 5255,									// ���Ͱ�������ź�����
		MT_HUNYAN_BUY_XUNYOU_OPERA_CS = 5256,							// ������Ѳ�β���
		MT_TURNTABLE_ROLL_REQ_CS = 5257,								// ����ת��ת��
		MT_WABAO_OPERA_REQ_CS = 5258,									// �ڱ���������
		MT_CHONGZHI_FETCH_REWARD_CS = 5259,								// ��ȡ��ֵ����
		MT_JILIAN_OPERA_CS = 5260,										// ������������
		MT_TOTAL_CHONGZHI_WANT_MONEY_FETCH_MSG_CS = 5262,				// ��Ҫ��Ԫ��-��ȡ��ҵ�ǰ��ֵ״̬����
		MT_TOTAL_CHONGZHI_WANT_MONEY_FETCH_REWARD_CS = 5263,			// ��Ҫ��Ԫ��-��ȡ��ҵ�ǰ��������Ľ���
		MT_ZAI_CHONGZHI_FETCH_REWARD_CS = 5264,							// �ٳ�ֵ����ȡ����
		MT_REPLY_GUILD_SOS_REQ = 5265,									// ��Ӧ�������

		MT_ZHUXIE_USER_INFO_SC = 5200,									// ��аս����ɫ��Ϣ
		MT_TURNTABLE_MILLIONAIRE_VIEW_SC = 5201,						// �󸻺��������㲥
		MT_ACTIVITY_SKILL_USE_INFO_SC = 5202,							// ����ʹ��֪ͨ
		MT_HUNYAN_XUNYOU_OPERA_INFO_SC = 5203,							// ���Ѳ�� ������Ϣ
		MT_TURNTABLE_REWARD_SC = 5204,									// ת�̽����·�
		MT_TURNTABLE_INFO_SC = 5205,									// ת�̻�����Ϣ
		MT_WABAO_INFO_SC = 5206,										// �ڱ���Ϣ
		MT_JILIAN_INFO_SC = 5207,										// ������Ϣ
		MT_JILIAN_VIEW_CHANGE_SC = 5208,								// ������ͼ�ı�
		MT_TOTAL_CHONGZHI_WANT_MONEY_INFO_SC = 5210,					// ��Ҫ��Ԫ��-��ȡ��ҵ�ǰ��ֵ״̬����
		MT_TOTAL_CHONGZHI_WANT_MONEY_FETCH_REWARD_SC = 5211,			// ��Ҫ��Ԫ��-��ȡ��ҵ�ǰ��������Ľ���
		MT_RA_CORNUCOPIA_FETCH_INFO_SC = 5212,							// �۱��裬��ȡ��ǰ״̬��Ϣ
		MT_RA_CORNUCOPIA_FETCH_REWARD_SC = 5213,						// �۱��裬��ȡ��ҵ�ǰ��������Ľ���
		MT_TIANJIANGCAIBAO_USER_INFO_SC = 5214,							// �콵�Ʊ���ɫ��Ϣ

		MT_MOVE_CHESS_RESET_REQ_CS = 5221,								// ������-��������
		MT_MOVE_CHESS_SHAKE_REQ_CS = 5222,								// ������-����ҡ��
		MT_MOVE_CHESS_FREE_INFO_REQ_CS = 5223,							// ������-������������Ϣ
		MT_MOVE_CHESS_SHAKE_SC = 5224,									// ������-ҡ����������

		MT_RA_NEW_CORNUCOPIA_INFO_SC = 5226,							// �¾۱�����Ϣ

		MT_RA_COLLECT_EXCHANGE_INFO_SC = 5230,							//���ֻ��Ϣ
		
		MT_RA_CONSUME_FOR_GIFT_ALL_INFO_SC = 5235,						// ����������Ϣ
																		
		MT_RA_MIJINGXUNBAO3_INFO_SC = 5236,								// �ؾ�Ѱ��3��Ϣ
		MT_RA_MIJINGXUNBAO3_TAO_REWARD_INFO_SC = 5237,					// �ؾ�Ѱ��3�����Ϣ

		MT_RA_HUANLEZADAN_INFO_SC = 5238,								// �����ҵ���Ϣ
		MT_RA_HUANLEZADAN_TAO_REWARD_INFO_SC = 5239,					// �����ҵ������Ϣ

		MT_RA_HUANLEYAOJIANG_INFO_SC = 5240,							// ����ҡ����Ϣ
		MT_RA_HUANLEYAOJIANG_TAO_REWARD_INFO_SC = 5241,					// ����ҡ�������Ϣ
		
		MT_DRESSING_ROOM_OPEAR_CS = 5245,								// �¹�����
		MT_DRESSING_ROOM_INFO_SC = 5246,								// �¹���Ϣ�·�
		MT_DRESSING_ROOM_SINGLE_INFO_SC = 5247,							// �¹�����Ϣ�·�
		MT_DRESSING_ROOM_EXCHANGE_CS = 5248,							// �¹�һ�

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_HOTSPRING_GIVE_PRESEN_CS = 5300,								// ��Ȫ����������
		MT_HOTSPRING_ADD_PARTNER_CS = 5301,								// �һ������
		MT_HOTSPRING_ADD_PARTNER_RET_CS = 5302,							// ��������������Ҵ�������������
		MT_HOTSPRING_DELETE_PARTNER_CS = 5303,							// ȡ�����
		MT_HOTSPRING_QA_FIRST_REQ_CS = 5304,							// ����������λ����Ϣ
		MT_HOTSPRING_QA_ANSWER_REQ_CS = 5305,							// �������
		MT_HOTSPRING_QA_USE_CAR_REQ_CS = 5306,							// ����ʹ�ñ���

		MT_HOTSPRING_SEND_PARTNER_INFO_SC = 5350,						// ���ͻ����Ϣ
		MT_HOTSPRING_SEND_EXP_INFO_SC = 5351,							// ������ʷ������Ϣ
		MT_HOTSPRINT_PLAYER_INFO_SC = 5352,								// ��Ȫ�����Ϣ
		MT_HOTSPRINT_RANK_INFO_SC = 5353,								// ��Ȫ���������Ϣ
		MT_HOTSPRING_ADD_PARTNER_ROUT_SC = 5354,						// ѯ�ʱ��������������
		MT_HOTSPRING_SEND_SHUANGXIU_INFO_SC = 5355,						// �㲥������Ȫ˫����Ϣ
		MT_HOTSPRING_SEND_QA_RANK_INFO_SC = 5356,						// ������Ȫ����������Ϣ
		MT_HOTSPRING_SEND_QA_QUESTION_INFO_SC = 5357,					// ������Ȫ������Ŀ��Ϣ
		MT_HOTSPRING_SEND_QA_FIRST_POST_SC = 5358,						// ������Ȫ�����һ��λ����Ϣ
		MT_HOTSPRING_SEND_QA_RESULT_SC = 5359,							// ������Ȫ����ش�����Ϣ

		MT_HOTSPRING_USE_SKILL_REQ_CS = 5360,							// ��Ȫʹ�ü�������
		MT_HOTSPRING_NOTICE_SKILL_INFO_SC = 5361,						// ���ͼ���ʹ����Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CROSS_GUILDBATTLE_SCENE_GUILDER_NUM_SC = 5368,				// �������ս��������������
		MT_CROSS_GUILDBATTLE_BOSS_HURT_INFO_SC = 5369,					// �������ս��BOSS�˺���Ϣ
		MT_CROSS_GUILDBATTLE_OPERA_REQ_CS = 5370,						// �������ս����������
		MT_CROSS_GUILDBATTLE_GET_RANK_INFO_REQ_CS = 5371,				// �������ս�������ȡ���а���
		MT_CROSS_GUILDBATTLE_GET_RANK_INFO_RESP_SC = 5372,				// �������ս�����ػ�ȡ���а���
		MT_CROSS_GUILDBATTLE_SCENE_INFO_SC = 5373,						// �������ս������Ϣ
		MT_CROSS_GUILDBATTLE_NOTIFY_INFO_SC = 5374,						// �������ս֪ͨ��Ϣ
		MT_CROSS_GUILDBATTLE_INFO_SC = 5375,							// �������ս��Ϣ
		MT_CROSS_GUILDBATTLE_TASK_INFO_SC = 5376,						// �������ս��������Ϣ
		MT_CROSS_GUILDBATTLE_BOSS_INFO_SC = 5377,						// �������ս��Boss��Ϣ
		MT_CROSS_GUILDBATTLE_FLAG_INFO_SC = 5378,						// �������ս������������Ϣ
		MT_CROSS_GUILDBATTLE_MONSTER_INFO_SC = 5379,					// �������ս����Ӣ����Ϣ
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_QUESTION_RIGHT_BROCAST_SC = 5380,							// ���ʹ�������
		MT_QUESTION_ANSWER_REQ_CS = 5381,								// ����ش�����
		MT_QUESTION_ANSWER_RESULT_SC = 5382,							// ���ʹ���ش�����Ϣ
		MT_QUESTION_ANSWER_GUILD_RANK_SC = 5383,						// ���͹������ش�������Ϣ
		MT_QUESTION_ANSWER_RIGHT_NUM_SC = 5384,							// ���͹������ش�������Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FB_TOWERDEFEND_ROLE_INFO_CS = 5453,							// ����������ɫ��Ϣ
		MT_FB_TOWERDEFEND_BUY_JOIN_TIMES_CS = 5454,						// �����������������
		MT_FB_EQUIP_GETINFO_CS = 5457,									// װ������������װ��������Ϣ

		MT_GET_BOSS_INFO_REQ_CS = 5463,									// ����boss��Ϣ
		MT_FB_GUILDMONSTER_CS = 5464,									// ��������������Ϣ
		MT_FB_PHASE_INFO_CS = 5465,										// �׶θ���������Ϣ����
		MT_BOSS_KILLER_INFO_REQ = 5466,									// Boss��ɱ�б���Ϣ����
		MT_FB_CHALLENGE_FB_OPERATE_CS = 5467,							// ��ս������������
		MT_QINGYUAN_FB_OPERA_REQ_CS = 5469,								// ��Ե������������
		MT_RESERVE_5470 = 5470,											// ������Ե�����������
		MT_HUNJIE_UP_LEVEl_CS = 5471,									// ��Ե����
		MT_QINGYUAN_TAKE_OFF_EQUIP = 5472,								// ������Եװ��
		MT_HUNJIE_REQ_EQUIP_INFO_CS = 5473,								// ��Եװ����Ϣ����
		MT_QINGYUAN_QUERY_MATE_QINGYUAN_VALUE_CS = 5474,				// ���������Եֵ��ѯ
		MT_QINGYUAN_DIVORCE_REQ_CS = 5475,								// �������
		MT_FB_REQ_NEXT_LEVEL_CS = 5476,									// ͨ�ø���������һ��
		MT_ENTER_BOSS_FAMILY_CS = 5477,									// ����boss֮������
		MT_GUILDFB_INFO_SC = 5478,										// ���˸�����Ϣ
		MT_GET_SCENE_MONSTER_GENERATE_INFO_CS = 5479,					// ���� ����ˢ�� boss��Ϣ
		MT_GET_SCENE_MONSTER_GENERATE_INFO_SC = 5480,					// ���� ����ˢ�� boss��Ϣ
		MT_GET_YAOSHOUGUANGCHANG_STATE_CS = 5481,						// ��ȡ���޹㳡״̬
		MT_YAOSHOUGUANGCHANG_STATE_SC = 5482,							// ���� ���޹㳡״̬
		MT_YAOSHOUGUANGCHANG_FBINFO_SC = 5483,							// ���� ���޹㳡 ������Ϣ
		MT_GET_YAOSHOUGUANGCHANG_REWARD_CS = 5484,						// ���� ���޹㳡 ������Ϣ
		MT_NOTIFY_YAOSHOUGUANGCHANG_REWARD_SC = 5485,					// ֪ͨ ���޹㳡 ������Ϣ
		MT_ENTER_YAOSHOUGUANGCHANG_CS = 5486,							// �������޹㳡
		MT_ZHUA_GUI_FBINFO_CS = 5487,									// ץ����״̬
		MT_WORLD_BOSS_DEAD_SC = 5488,									// ����boss����
		MT_ZHUA_GUI_ADD_PER_INFO_CS = 5489,								// ץ��ӳ���Ϣ
		MT_GET_SUOYAOTA_STATE_CS = 5490,								// ��ȡ������״̬
		MT_SUOYAOTA_STATE_SC = 5491,									// ���� ������״̬
		MT_SUOYAOTA_FBINFO_SC = 5492,									// ���� ������ ������Ϣ
		MT_GET_SUOYAOTA_REWARD_CS = 5493,								// ���� ������ ������Ϣ
		MT_NOTIFY_SUOYAOTA_REWARD_SC = 5494,							// ֪ͨ ������ ������Ϣ
		MT_ENTER_SUOYAOTA_CS = 5495,									// ����������
		MT_GET_GUILDFB_GUARD_POS_CS = 5496,								// ��ȡ���˸����ػ�λ��
		MT_GUILDFB_GUARD_POS_SC = 5497,									// ��ȡ���˸����ػ�λ��
		MT_CROSS_TEAM_FB_INFO_SC = 5498,								// �����Ӹ�����Ϣ

		MT_FB_STORY_REQ_INFO_CS = 5400,									// ���鱾:������鱾������Ϣ
		MT_FB_STORY_INFO_SC = 5401,										// ���鱾:���鱾��Ϣ
		MT_FB_DAILY_REQ_ROLE_INFO_CS = 5410,							// �ճ�����:�����ճ�����������Ϣ
		MT_FB_DAILY_ROLE_INFO_SC = 5411,								// �ճ�����:������Ϣ
		MT_FB_TOWERDEFEND_INFO_SC = 5413,								// �ճ���������������������Ϣ

		MT_FB_PATA_REQ_ALL_INFO_CS = 5420,								// ��������:��������������Ϣ
		MT_FB_PATA_ALL_INFO_SC = 5421,									// ��������:��Ϣ����
		MT_FB_VIP_ALL_INFO_REQ_CS = 5422,								// VIP����:���󸱱���Ϣ����
		MT_FB_VIP_ALL_INFO_SC = 5423,									// VIP����:��Ϣ����
		MT_FB_TOWERDEFEND_WARNING_SC = 5431,							// �������������������򱨾�
		MT_FB_TOWERDEFEND_RESULT_SC = 5433,								// ����������� ��������ͨ�ؽ��
		MT_FB_YSJT_TEAM_FB_LOGIC_INFO_SC = 5434,						// ������޼�̳����:������Ϣ
		MT_FB_MGXF_TOUCH_DOOR_CS = 5445,								// ����Թ��ɸ����������봫�͵�
		MT_FB_MGXF_TEAM_FB_LOGIC_INFO_SC = 5446,						// ����Թ��ɸ�����:������Ϣ
		MT_FB_TEAM_ROOM_ENTER_AFFIRM_SC = 5448,							// ��Ӹ����������ȷ��
		MT_FB_TEAM_ROOM_LIST_SC = 5449,									// ��Ӹ��������б���
		MT_FB_TEAM_ROOM_OPERATE_CS = 5450,								// ��Ӹ��������������

		MT_FB_TOWERDEFEND_ROLE_INFO_SC = 5404,							// ������ɫ��Ϣ
		MT_FB_CHALLENGE_INFO_SC = 5407,									// ��ս������Ϣ
		MT_FB_CHALLENGE_PASS_LEVEL_SC = 5408,							// ��ս����ͨ������
		MT_FB_EQUIP_RESULT_SC = 5409,									// װ��������ͨ�ؽ��
		MT_FB_EQUIP_EXP_SC = 5412,                                      // װ������������
		MT_FB_EQUIP_INFO_SC = 5414,                                     // װ����������Ϣ
		MT_FB_GUILDMONSTER_INFO_SC = 5415,								// ����������Ϣ�·�
		MT_FB_GUILDMONSTER_RESULT_SC = 5416,							// ��������ͨ�ؽ��
		MT_FB_AUTO_FB_REWARD_DETAIL_SC = 5417,							// ɨ����������
		MT_FB_PHASE_INFO_SC = 5418,										// �׶θ���������Ϣ
		MT_FB_CHALLENGE_LAYER_INFO_SC = 5419,							// ��ս��������Ϣ
		MT_QINGYUAN_BASE_FB_INFO_SC = 5425,								// ��Ե����������Ϣ�·�
		MT_QINGYUAN_FB_INFO_SC = 5426,									// ��Ե����������Ϣ�·�
		MT_HUNJIE_EQUIP_INFO_SC = 5427,									// ��Եװ����Ϣ
		MT_QINGYUAN_MATE_QINGYUAN_VALUE_SC = 5428,						// ������Եֵ�·�
		MT_FB_BOSS_HP_PERCENT_INFO_SC = 5429,							// ����ͨ�ػ���ʧ�ܼ�¼
		MT_ZHANSHENDIAN_FB_INFO_SC = 5437,								// ս������Ϣ�·�
		MT_ZHANSHENDIAN_FB_RESULT_SC = 5438,							// ս����ͨ�ؽ���·�
		MT_FB_TIANGONGSHILIAN_FB_RESULT_SC = 5436,						// �칬���� �������
		MT_FB_TIANGONGSHILIAN_FB_INFO_SC = 5437,						// �칬���� ������Ϣ
		MT_QINGYUAN_BLESS_INFO_SC = 5438,								// ��Եף����Ϣ
		MT_QINGYUAN_HUNYAN_INVITE_SC = 5439,							// ��Ե������Ϣ
		MT_HUNYAN_ROLE_INFO_SC = 5440,									// ���糡��������Ϣ
		MT_HUNJIE_EQUIP_UPLEVEL_SEND_LOVER_INFO_SC = 5441,				// ������Եװ��ͬ��������
		MT_QINGYUAN_FB_REWARD_RECORD_INFO_SC = 5442,					// ��Ե��������������Ϣ
		MT_FB_PICK_ITEM_INFO_SC = 5443,									// ��������ʰȡ��Ϣ

		MT_FB_SCENE_GUWU_CS = 5498,										// ��������
		MT_FB_SCENE_LGIC_INFO_SC = 5499,								// ����ͨ����Ϣ
		////////////////////////////////////////////////////////////////
		// 5400�ε�Э��id�Ƚ��ң��¼�Э�鲻�������
		////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_WUSHUANG_EQUIP_INFO_SC = 5500,								// ��˫װ����Ϣ
		MT_WUSHUANG_FB_KICKOUT_NOTIFY_SC = 5501,						// ��˫��������ԭ��
		MT_WUSHUANG_FB_INFO_SC = 5502,									// ��˫������Ϣ
		MT_WUSHUANG_BAG_INFO_SC = 5503,									// ��˫������Ϣ
		MT_HUNSHOU_OPERA_STATUS_SC = 5504,								// ���޲���״̬
		MT_ZHUANZHI_FB_NOTIFY_SC = 5505,								// תְ����֪ͨ
		MT_ENTER_BOSS_FAMILY_OPERATE_CS = 5506,							// boss֮�Ҳ�������

		MT_GET_SCENE_GATHER_GENERATE_INFO_CS = 5525,					// ���� ����ˢ�� �ɼ�����Ϣ
		MT_GET_SCENE_GATHER_GENERATE_INFO_SC = 5526,					// ���� ����ˢ�� �ɼ�����Ϣ
		MT_FB_DROP_INFO_SC = 5527,										// �����������ͳ��
		MT_FB_FINISH_SC = 5528,											// ����ͨ��ͨ��֪ͨ
		MT_FB_TOWERDEFEND_REFRESH_REQ_CS = 5529,						// �������� ����ˢ��

		MT_WUSHUANG_OPERA_REQ_CS = 5550,								// ��˫װ������
		MT_WUSHUANG_FB_INFO_CS = 5551,									// ��˫������Ϣ����
		MT_HUNSHOU_OPERA_REQ_CS = 5552,									// ���޲�������

		MT_EXP_FB_FETCH_FIRST_REWARD_REQ_CS = 5554,						// �ճ�����:���鱾��ȡ��ͨ����
		MT_EXP_FB_INFO = 5555,											// ���鸱����Ϣ
		MT_EXP_FB_PAY_BUFF = 5556,										// ���鸱���������effect

		MT_TEAM_EQUIP_FB_INFO_SC = 5570,								// ���װ��������Ϣ
		MT_TEAM_EQUIP_FB_DROP_TIMES_INFO_SC = 5571,						// ���װ���������������Ϣ
		MT_TEAM_EQUIP_FB_BUY_DROP_TIMES_CS = 5572,						// ���װ����������������

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CARDZU_BASE_INFO_SC = 5600,									// ���������Ϣ
		MT_CARDZU_LEVEL_LIST_INFO_SC = 5601,							// ָ������������ϵȼ���Ϣ
		MT_CARDZU_CARD_LIST_INFO_SC = 5602,								// ָ���������п���������Ϣ
		MT_JINGLING_EXPLORE_OPER_REQ_CS = 5603,							// ����̽�ղ�������
		MT_JINGLING_EXPLORE_INFO_SC = 5604,								// ����̽����Ϣ
		MT_JINGLING_HOME_ROB_ACK_SC = 5605,								// �����԰�Ӷ���
		MT_JINGLING_ADVANTAGE_INFO_SC = 5606,							// ����������Ϣ
		MT_JINGLING_ADVANTAGE_COUNT_SC = 5607,							// ����������ɫ��������
		MT_JINGLING_ADVANTAGE_BOSS_POS_SC = 5608,						// ��������boss��Ϣ
		MT_JINGLING_ADVANTAGE_BOSS_ENTER_CS = 5609,						// ��������boss
		MT_JINGLING_EGG_POS_INFO = 5610,                                // ���鵰λ��

		MT_CARDZU_OPERA_REQ_CS = 5650,									// ������ز�������
		MT_LIEMING_HUNSHOU_OPERA_REQ_CS = 5655,							// �Ի�����
		MT_LIEMING_SLOT_INFO_SC = 5656,									// ��˫��������Ϣ
		MT_LIEMING_BAG_INFO_SC = 5657,									// ��˫������Ϣ
		MT_LIEMING_HUNSHOU_EXCHANGE_CS = 5658,							// ���꽻������
		MT_JINGLING_OPER_REQ_CS = 5670,									// �����������
		MT_JINGLING_INFO_SC = 5671,										// ������Ϣ
		MT_JINGLING_VIEW_CHANGE_SC = 5672,								// ��������ı�㲥
		MT_SHENGWANG_OPERA_CS = 5675,									// ������������
		MT_SHENGWANG_INFO_SC = 5676,									// ������Ϣ
		MT_XIANJIE_VIEW_CHANGE_SC = 5677,								// �ɽױ仯
		MT_CHENGJIU_OPERA_CS = 5680,									// �ɾͲ�������
		MT_CHENGJIU_INFO_SC = 5681,										// �ɾ���Ϣ
		MT_CHENGJIU_TITLE_VIEW_CHANGESC = 5682,							// �ɾͳƺű仯
		MT_CHENGJIU_REWARD_CHANGESC = 5683,								// �ɾͳƺű仯
		MT_SHENZHOU_WEAPON_OPERA_REQ_CS = 5684,							// ����������ز�������
		MT_PASTURE_SPIRIT_OPERA_REQ_CS = 5685,							// ����������ز�������
		MT_MULTI_MOUNT_OPERA_REQ_CS = 5686,								// ˫�������������
		MT_PERSONALIZE_OPERA_REQ_CS = 5687,								// �������������
		MT_MAGIC_CARD_OPERA_REQ_CS = 5688,								// ħ������
		MT_FAIRY_TREE_OPERA_REQ_CS = 5689,								// ������������
		MT_MAGIC_EQUIPMENT_OPERA_REQ_CS = 5690,							// ħ����������
		MT_CHENGJIU_REWARD_INFO_SC = 5691,								// ���߳ɾ���Ϣ
		MT_SHEN_ZHOU_BOX_INFO_SC = 5692,								// ��������������Ϣ
		MT_SHEN_ZHOU_ONE_KEY_IDENTIFY_CS = 5693,						// ��������һ������
		MT_SHEN_ZHOU_HUNYIN_RESOLVE_CS = 5694,							// ����������ӡ�ֽ�
		MT_JINGLING_HOME_OPER_REQ_CS = 5695,							// �����������
		MT_JINGLING_HOME_INFO_SC = 5696,								// �����԰��Ϣ
		MT_JINGLING_HOME_LINT_INFO_SC = 5697,							// �����԰�б���Ϣ
		MT_JINGLING_HOME_ROB_RECORD_SC = 5698,							// �����԰�Ӷ��¼

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CROSS_START_REQ_CS = 5750,									// ����ʼ���
		MT_CROSS_XIULUO_TOWER_JOIN_REQ_CS = 5751,						// �������������
		MT_CROSS_XIULUO_TOWER_BUY_BUFF_CS = 5752,						// �������������buff
		MT_CROSS_1V1_MATCH_REQ_CS = 5753,								// ���1v1����ƥ��
		MT_CROSS_1V1_FIGHT_READY_CS = 5754,								// ���1v1׼��ս��
		MT_CROSS_1V1_FETCH_REWARD_REQ_CS = 5755,						// ���1v1��ȡ����
		MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_REQ_CS = 5756,			// ���3V3����ƥ��
		MT_CROSS_MULTIUSER_CHALLENGE_GET_BASE_SELF_SIDE_INFO_CS = 5757,	// ���3V3�����ʼ������Ϣ
		MT_CROSS_MULTIUSER_CHALLENGE_FETCH_DAYCOUNT_REWARD_CS = 5758,	// ���3V3��ȡÿ�մ�������
		MT_CROSS_MULTIUSER_CHALLENGE_CANCLE_MATCHING_CS = 5759,			// ���3V3ȡ��ƥ��
		MT_CROSS_MULTIUSER_CHALLENGE_REQ_SIDE_MEMBER_POS_CS = 5760,		// ���3v3����ȡ����λ����Ϣ
		MT_CROSS_TUANZHAN_FETCH_SCOR_REWARD_CS = 5761,					// �����ս��ȡ���ֽ���
		MT_CROSS_1V1_BUY_TIME_REQ_CS = 5762,							// ���1v1���������
		MT_CROSS_1V1_RING_OPER_CS = 5763,								// ���1v1�����ָ����  
		MT_CROSS_1V1_MATCH_RESULT_REQ_CS = 5764,					    // ���1v1ƥ��������

		MT_CROSS_ENTER_SERVER_SC = 5700,								// ֪ͨ�ͻ��˽������ط�
		MT_CROSS_XIULUO_TOWER_SELF_ACTIVITY_INFO_SC = 5701,				// ������������˻��Ϣ
		MT_CROSS_XIULUO_TOWER_RANK_INFO_SC = 5702,						// ������������а���Ϣ
		MT_CROSS_XIULUO_TOWER_CHANGE_LAYER_NOTICE_SC = 5703,			// ����������ı����ʾ
		MT_CROSS_XIULUO_TOWER_USER_RESULT_SC = 5704,					// ������������
		MT_CROSS_XIULUO_TOWER_INFO_SC = 5705,							// �����������Ϣ
		MT_CROSS_HONOR_CHANGE_SC = 5706,								// ����������
		MT_CROSS_1V1_SELF_INFO_SC = 5707,								// ���1V1������Ϣ
		MT_CROSS_1V1_FIGHT_START_SC = 5708,								// ���1V1ս����ʼ
		MT_CROSS_MULTIUSER_CHALLENGE_SELF_INFO_REFRESH_SC = 5709,		// ���3V3������Ϣ����
		MT_CROSS_MULTIUSER_CHALLENGE_MATCH_INFO_REFRESH_SC = 5710,		// ���3V3������Ϣ����
		MT_CROSS_MULTIUSER_CHALLENGE_MATCH_STATE_SC = 5711,				// ���3V3״̬֪ͨ
		MT_CROSS_MULTIUSER_CHALLENGE_BASE_SELF_SIDE_INFO_SC = 5712,		// ���3V3��ʼ������Ϣ
		MT_CROSS_MULTIUSER_CHALLENGE_SELF_ACTIVITY_INFO_SC = 5713,		// ���3V3���˻��Ϣ
		MT_CROSS_MULTIUSER_CHALLENGE_REQ_SIDE_MEMBER_POS_SC = 5714,		// ���3v3����ȡ����λ����Ϣ
		MT_CROSS_TUANZHAN_STATE_NOTIFY_SC = 5715,						// �����ս������״̬֪ͨ
		MT_CROSS_TUANZHAN_PLAYER_INFO_SC = 5716,						// �����ս�������Ϣ
		MT_CROSS_TUANZHAN_RANK_INFO_SC = 5717,							// �����ս��������Ϣ
		MT_CROSS_TUANZHAN_SIDE_INFO_SC = 5718,							// �����ս����Ӫ������Ϣ
		MT_CROSS_TUANZHAN_PILLA_INFO_SC = 5719,							// �����ս��ͨ��������Ϣ
		MT_CROSS_TUANZHAN_DUR_KILL_INFO_SC = 5720,						// �����ս����ɱ��Ϣ���
		MT_CROSS_TUANZHAN_RESULT_INFO_SC = 5721,						// �����ս���������
		MT_CROSS_PASTURE_PLAYER_INFO_SC = 5722,							// ��������������Ϣ
		MT_CROSS_BOSS_BUY_RELIVE_TIMES_CS = 5723,						// ���BOSS�������򸴻����
		MT_CROSS_BOSS_PLAYER_INFO_SC = 5724,							// ���BOSS�������Ϣ
		MT_CROSS_BOSS_SCENE_PLAYER_INFO_SC = 5725,						// ���BOSS����������������Ϣ
		MT_SERVER_CLOSE_NOTIFY_SC = 5726,								// �����������ر�֪ͨ
		MT_CROSS_XIULUO_TOWER_BUFF_INFO_SC = 5727,						// ���������BUFF��Ϣ
		MT_CROSS_XIULUO_TOWER_SCORE_REWARD_REQ_CS = 5728,				// �����������������ֽ���
		MT_CROSS_XIULUO_TOWER_GATHER_INFO_SC = 5729,					// ������������ɼ�����Ϣ
		MT_CROSS_BOSS_BOSS_INFO_REQ_CS = 5730,							// ���BOSS��BOSS��Ϣ����
		MT_CROSS_BOSS_BOSS_INFO_ACK_SC = 5731,							// ���BOSS��BOSS��Ϣ
		MT_CROSS_PASTURE_ATTACH_ANIMAL_NOTIC = 5732,					// ���������������֪ͨ
		MT_CROSS_CROSS_GUILD_BATTLE_SPECIAL_TIME_NOTICE_SC = 5733,		// �������ս������������
		MT_CROSS_PASTURE_RANK_INFO_SC = 5734,							// ������������������Ϣ
		MT_CROSS_XIULUO_TOWER_BUY_BUFF_REQ_CS = 5735,					// �������������ɼ�buff
		MT_CROSS_MULTIUSER_CHALLENGE_CARD_OPER_REQ = 5736,				// ���3v3��������
		MT_CROSS_MULTIUSER_CHALLENGE_FETCH_GONGXUN_REWARD_SC = 5737,	// ���3v3��ȡ��ѫ����
		MT_CROSS_BOSS_KILLER_RECORD_SC = 5738,                          // ���BOSS,��ɱ��Ϣ
		MT_CROSS_BOSS_DROP_RECORD_SC = 5739,                            // ���BOSS,������Ϣ
		MT_RELIVE_TIRE_INFO_SC = 5740,                                  // ����ƣ��
		MT_CROSS_XIULUO_RNAK_TITLE_INFO_SC = 5741,						// ���������ǰ��������10�����
		MT_CROSS_XIULUO_TOWER_BOSS_INFO = 5742,							// ������boss��Ϣ

		MT_CROSS_DARK_NIGHT_USER_INFO_SC = 5770,						// �ºڷ�������Ϣ
		MT_CROSS_DARK_NIGHT_RANK_INFO_SC = 5771,						// �ºڷ��������Ϣ
		MT_CROSS_DARK_NIGHT_BOSS_INFO_SC = 5772,						// �ºڷ��boss��Ϣ
		MT_CROSS_DARK_NIGHT_PLAYER_INFO_BROADCAST_SC = 5773,			// �ºڷ�������Ϣ�㲥
		MT_CROSS_DARK_NIGHT_REWARD_TIMESTAMP_INFO_SC = 5774,			// �ºڷ�߷�����ʱ���
		MT_CROSS_DARK_NIGHT_TOP_PLAYERPOS_INFO_SC = 5775,				// �ºڷ�����а��һ���������Ϣ
		MT_CROSS_DARK_NIGHT_PLAYER_REQ_CS = 5776,						// �ºڷ���������

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_XUNZHANG_ALL_INFO_SC = 5800,									// ѫ��������Ϣ
		MT_XUNZHANG_UPLEVEL_REQ_CS = 5801,								// ѫ����������
		MT_ZHIBAO_ALL_INFO_SC = 5805,									// ����������Ϣ
		MT_ZHIBAO_UPLEVEL_CS = 5806,									// ������������
		MT_ZHIBAO_USEIMAGE_CS = 5807,									// ������������
		MT_ZHIBAO_HUANHUA_CS = 5808,									// �����û�����
		MT_ZHIBAO_ATTACK_SC = 5852,										// ����������Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_TW_RELIVE_SHOP_BUY_CS = 5900,								// ����ս �����̵깺����Ʒ
		MT_TW_FIGHT_SHOP_BUY_CS = 5901,									// ����ս ս���̵깺����Ʒ
		MT_TW_QUALIFICATION_CS = 5902,									// ����ս ��ȡ�����ʸ���Ϣ
		MT_TW_SET_LANDMINE_REQ_CS = 5903,								// ����ս ���õ�������

		MT_TW_ROLE_IMAGE_INFO_SC = 5950,								// ����ս �㲥��������
		MT_TW_GLOBE_INFO_SC = 5951,										// ����ս ȫ����Ϣ
		MT_TW_ROLE_INFO_SC = 5952,										// ����ս ������Ϣ
		MT_TW_QUALIFICATION_SC = 5953,									// ����ս ���Ͳ����ʸ���Ϣ
		MT_TW_LUCK_REWARD_SC = 5954,									// ����ս ���˽���

		MT_FITHTING_MINING_REQ_CS = 5955,								// ������ϵ�� - �ڿ�����
		MT_FIGHTING_MINING_BASE_INFO_SC = 5956,							// ������ϵ�� - �ڿ������Ϣ
		MT_FIGHTING_MINING_BEEN_ROB_LIST_SC = 5957,						// ������ϵ�� - �ڿ����б�
		MT_FIGHTING_MINING_LIST_SC = 5958,								// ������ϵ�� - �ڿ��б�
		MT_FIGHTING_CHALLENGE_BASE_INFO_SC = 5959,						// ������ϵ�� - ���ƻ�����Ϣ
		MT_FIGHTING_MINING_FIGHTING_RESULT = 5960,						// ������ϵ�� - ս�����
		MT_FIGHTING_CHALLENGE_LIST_SC = 5961,							// ������ϵ�� - ���ƶ����б�
		MT_FIGHTING_SAILING_BASE_INFO_SC = 5962,						// ������ϵ�� - ����������Ϣ
		MT_FIGHTING_SAILING_BEEN_ROB_LIST_SC = 5963,					// ������ϵ�� - ���������б�
		MT_FIGHTING_SAILING_LIST_SC = 5964,								// ������ϵ�� - �����б�
		MT_FIGHTING_COUNT_DOWN_NOTIFY_SC = 5965,						// ������ϵ�� - ս����ʼʱ��֪ͨ
		MT_FIGHTING_BEEN_ROB_NOTIFY_SC = 5966,							// ������ϵ�� - ������֪ͨ�����ˣ�
		MT_FIGHTING_ROBING_NOTIFY_SC = 5967,							// ������ϵ�� - ����㲥���������ˣ�

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CUR_WDDING_INFO_SC = 6000,									// ��ǰ���糡����Ϣ���󷵻�
		MT_MAZE_INFO_SC = 6001,											// �Թ���Ϣ
		MT_MAZE_MOVE_RET_SC = 6002,										// �Թ��ƶ����
		MT_MARRY_SPECIAL_EFFECT_SC = 6003,								// �����Ч
		MT_MARRY_REQ_ROUTE_SC = 6004,									// �����Ϣת�����Է�
		MT_DIVORCE_REQ_ROUTE_SC = 6005,									// �����Ϣת�����Է�
		MT_MARRY_XUNYOU_FIREWORK_SC = 6006,								// ���Ѳ���̻�
		MT_HUNYAN_YUEYUE_INFO_SC = 6007,								// ������ԤԼ��Ϣ����
		MT_MARRY_HUNYAN_OPERA_SC = 6008,								// ���������
		MT_MARRY_HUNYAN_STATE_SC = 6009,								// ���糡��״̬��Ϣ
		MT_HUNYAN_BLESS_RECORD_SC = 6010,								// ����ף����¼

		MT_ACTIVITY_FIND_INFO_SC = 6011,								// ��һ���Ϣ
		MT_WEDDINGRING_SKILL_INFO_SC = 6012,							// ��似����Ϣ

		MT_MARRY_NOTICE_SC = 6013,										// ���֪ͨ
		MT_MARRY_ZHUHE_SEND_CS = 6014,									// ���ף�ط���
		MT_MARRY_ZHUHE_SHOU_SC = 6015,									// ���ף�ؽ���
		MT_HUNYAN_APPLY_LIST_SC = 6016,									// �����������б���Ϣ

		MT_QING_YUAN_SHENG_DI_OPERA_REQ_CS = 6020,						// ��Եʥ�ز�������
		MT_QING_YUAN_SHENG_DI_TASK_INFO_SC = 6021,						// ��Եʥ��������Ϣ
		MT_QING_YUAN_SHENG_DI_BOSS_INFO_SC = 6022,						// ��Եʥ��boss��Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MAZE_QUERY_CS = 6100,										// �����Թ���Ϣ
		MT_MAZE_MOVE_CS = 6101,											// �����Թ��ƶ�
		MT_MAZE_BUY_CS = 6102,											// �����Թ��������
		MT_MAZE_FETCH_JIFEN_REWARD_CS = 6103,							// ��ȡ�Թ����ֽ���
		MT_MOLONG_INFO_SC = 6104,										// ħ��ϵͳ
		MT_PET_RENAME_NOTIFY_SC = 6105,									// ��������㲥
		MT_PET_BACKPACK_INFO_SC = 6106,									// ���ﱳ����Ϣ
		MT_PET_INFO_SC = 6107,											// ���������Ϣ
		MT_PET_CHOU_RESULT_SC = 6108,									// ����齱���
		MT_PET_FIGHT_OUT_INFO_SC = 6109,								// �����ս��Ϣ
		MT_PET_OPERA_REQ_CS = 6110,										// ������ز�������
		MT_PET_UPGRADE_REQ_CS = 6111,									// ������������
		MT_PET_USE_SPECIAL_IMG_CS = 6112,				     			// ʹ����������
		MT_RA_SPECIAL_APPEARACE_PASSIVE_RANK_INFO_SC = 6113,			// ���������
		MT_RA_SPECIAL_APPEARACE_RANK_INFO_SC = 6114,					// ���������
		MT_HUASHEN_ALL_INFO_SC = 6115,									// ������Ϣ
		MT_HUASHEN_OPERA_REQ_CS = 6116,									// ��������
		MT_PET_QINMI_ALL_INFO_SC = 6117,								// ��������������Ϣ
		MT_PET_QINMI_SINGLE_INFO_SC = 6118,								// �������ܵ�����Ϣ
		MT_PET_FOOD_MARKET_CHOU_RESULT_SC = 6119,						// ����Ի��г��齫���
		MT_PASTURE_SPIRIT_ALL_INFO_SC = 6120,							// ��������������Ϣ
		MT_PASTURE_SPIRIT_SINGLE_INFO_SC = 6121,						// �������鵥����Ϣ
		MT_PASTURE_SPIRIT_LUCKY_DRAW_RESULT_SC = 6122,					// ��������齱���
		MT_MULTI_MOUNT_ALL_INFO_SC = 6123,
		MT_MULTI_MOUNT_CHANGE_NOTIFY_SC = 6124,							// ˫��������������֪ͨ
		MT_MULTI_MOUNT_NOTIFY_AREA_SC = 6125,							// ˫��������Ϣ�������㲥
		MT_PERSONALIZE_WINDOW_ALL_INFO_SC = 6126,						// ���Ի�����������Ϣ
		MT_PERSONALIZE_AVATAR_WINDOW_ALL_INFO_SC = 6127,				// ���Ի�����-ͷ���������Ϣ
		MT_HUASHEN_SPIRIT_INFO_SC = 6129,								// �����ػ�����
		MT_MAGIC_CARD_ALL_INFO_SC = 6130,								// ħ��������Ϣ
		MT_MAGIC_CARD_CHOU_RESULT_SC = 6131,							// ħ���齱��Ϣ
		MT_WUSHANG_EQUIP_REQ_CS = 6132,									// �����������
		MT_WUSHANG_EQUIP_INFO_SC = 6133,								// ���������Ϣ
		MT_MITAMA_OPERA_REQ_CS = 6134,									// ��������
		MT_MITAMA_ALL_MITAMA_INFO_SC = 6135,							// ������Ϣ
		MT_MITAMA_SINGLE_MITAMA_INFO_SC = 6136,							// ���굥����Ϣ
		MT_MITAMA_HOT_SPRING_SCORE_SC = 6137,							// ��Ȫ����
		MT_FAIRY_TREE_ALL_INFO_SC = 6139,								// ����������Ϣ
		MT_FAIRY_TREE_DRAW_REWARD_INFO_SC = 6140,						// �����齱��Ϣ
		MT_MAGIC_EQUIPMENT_ALL_INFO_SC = 6141,							// ħ��������Ϣ
		MT_MAGIC_EQUIPMENT_CHANGE_NOTIFY_SC = 6142,						// ħ�����������Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_GB_ROLE_ADD_HUDUN_CS = 6200,									// ��������  �����ͷŻ���
		MT_GB_ROLE_SUBMIT_GOODS_CS = 6201,								// ��������  �ύ������Ʒ����
		MT_GB_WINNER_INFO_CS = 6202,									// ��������  ������һ�������Ϣ

		MT_GB_ROLE_BIANSHEN_IMAGE_SC = 6250,							// �������Թ㲥��������
		MT_GB_GLOBAL_INFO_SC = 6251,									// �������� ȫ����Ϣ���㲥��
		MT_GB_ROLE_INFO_SC = 6252,										// �������� ������Ϣ
		MT_GB_WINER_INFO_SC = 6253,										// �������� ������һ�������Ϣ
		MT_GB_LUCK_REWARD_SC = 6254,									// �������� ���˽���
		MT_GB_GLOBAL_INFO_NEW_SC = 6255,								// �¹������� ȫ����Ϣ���㲥��
		MT_GB_ROLE_INFO_NEW_SC = 6256,									// �¹������� ������Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_HALO_INFO_SC = 6400,											// �⻷��Ϣ
		MT_HALO_UPGRADE_REQ_CS = 6402,									// ����⻷����
		MT_HALO_USE_IMAGE_CS = 6403,									// �⻷����ʹ������
		MT_HALO_GET_INFO_CS = 6404,										// ����⻷��Ϣ
		MT_HALO_UPLEVEL_EQUIP_CS  = 6405,								// �⻷����װ������
		MT_HALO_SKILL_UPLEVEL_REQ_CS = 6406,							// �⻷������������
		MT_HALO_SPECIAL_IMG_UPGRADE_CS =6407,							// �⻷�����������
		MT_HALO_UP_STAR_LEVEL_CS = 6408,								// �⻷���Ǽ�����

		MT_SHENGONG_INFO_SC = 6450,										// ����Ϣ
		MT_SHENGONG_UPGRADE_REQ_CS = 6452,								// �񹭽���
		MT_SHENGONG_USE_IMAGE_CS = 6453,								// ������ʹ������
		MT_SHENGONG_GET_INFO_CS = 6454,									// ��������Ϣ
		MT_SHENGONG_UPLEVEL_EQUIP_CS  = 6455,							// ������װ������
		MT_SHENGONG_SKILL_UPLEVEL_REQ_CS = 6456,						// �񹭼�����������
		MT_SHENGONG_SPECIAL_IMG_UPGRADE_CS =6457,						// �������������
		MT_SHENGONG_UP_STAR_LEVEL_CS = 6458,							// �����Ǽ�����
		MT_SHENGONG_UNUSE_IMAGE_CS = 6459,								// ������ȡ��ʹ������

		MT_SHENYI_INFO_SC = 6500,										// ������Ϣ
		MT_SHENYI_UPGRADE_REQ_CS = 6502,								// �����������
		MT_SHENYI_USE_IMAGE_CS = 6503,									// ��������ʹ������
		MT_SHENYI_GET_INFO_CS = 6504,									// ����������Ϣ
		MT_SHENYI_UPLEVEL_EQUIP_CS  = 6505,								// ��������װ������
		MT_SHENYI_SKILL_UPLEVEL_REQ_CS = 6506,							// ��������������
		MT_SHENYI_SPECIAL_IMG_UPGRADE_CS =6507,							// ���������������
		MT_SHENYI_UP_STAR_LEVEL_CS = 6508,								// �������Ǽ�����
		MT_SHENYI_UNUSE_IMAGE_CS = 6509,								// ����ȡ������ʹ������

		MT_FOOTPRINT_INFO_SC = 6520,									// �㼣��Ϣ
		MT_FOOTPRINT_OPERATE_CS = 6521,									// �㼣��������

		MT_CLOAK_INFO_SC = 6525,										// ������Ϣ
		MT_CLOAK_OPERATE_CS = 6526,										// �����������

		MT_ENTER_BOSS_DABAO_INFO_SC = 6551,								// ��boss��Ϣ
		MT_ENTER_BOSS_FAMILY_INFO_SC = 6552,							// boss֮��boss��Ϣ
		MT_ENTER_BOSS_MIKU_INFO_SC = 6553,								// �ؿ�boss��Ϣ
		MT_ENTER_BOSS_ROLE_INFO_SC = 6554,								// boss��ظ�����Ϣ
		MT_DABAO_BOSS_NEXT_FLUSH_TIME_SC = 6555,						// ��bossˢ��ʱ����Ϣ
		MT_BOSS_INFO_TO_ALL_SC = 6556,									// boss�㲥��Ϣ
		MT_BOSS_KILLER_LIST_SC = 6557,									// boss��ɱ�б���Ϣ
		MT_FOLLOW_BOSS_INFO_SC = 6558,									// ��עboss�б���Ϣ
		MT_ACTIVE_BOSS_NEXT_FLUSH_TIME_SC = 6559,						// ��Ծbossˢ��ʱ����Ϣ
		MT_ENTER_BOSS_ACTIVE_INFO_SC = 6560,							// ��Ծboss��Ϣ
		MT_ENTER_BOSS_MIKU_MONSTER_INFO_SC = 6561,						// �ؿ߹�����Ϣ
		MT_PRECIOUS_BOSS_TASK_INFO_SC = 6562,							// �ز�boss������Ϣ
		MT_PRECIOUS_BOSS_INFO_SC = 6563,								// �ز�boss��Ϣ
		MT_PRECIOUS_BOSS_POS_REQ_CS = 6564,								// �ز�bossλ������
		MT_PRECIOUS_BOSS_POS_INFO_CS = 6565,							// �ز�bossλ����Ϣ
		MT_ACTIVE_BOSS_HURT_RANK_INFO_SC = 6566,						// ��Ծboss�˺�������Ϣ
		MT_ACTIVE_BOSS_LEAVE_INFO_SC  = 6567,							// �뿪��Ծboss�˺���Ϣ����
		MT_MIKU_BOSS_HURT_RANK_INFO_SC = 6568,							// ��Ӣboss�˺�������Ϣ
		MT_MIKU_BOSS_LEAVE_INFO_SC = 6569,								// �뿪��Ӣboss�˺���Ϣ����

		MT_FOLLOW_BOSS_REQ_CS = 6570,									// boss��ע����
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_UPLEVEL_WEDDINGRING_CS = 6600,								// �������
		MT_PERFORM_WEDDINGRING_SKILL_CS =6601,							// �ͷŻ�似��
		MT_WELFARE_ONLINE_REWARD_CS = 6602,								// �������߽���
		MT_DIVORCE_RET_CS = 6603,										// ���ظ�
		MT_MARRY_INFOMATION_SC = 6604,									// �����Ϣ
		MT_WELFARE_HAPPYTREE_REWARD_CS = 6605,							// ���ֹ�������
		MT_WANGLINGEXPLORE_USER_INFO_SC = 6606,							// ����̽�ս�ɫ��Ϣ
		MT_CROSS_TEAM_FB_OPTION_CS = 6607,								// �����ӱ�����
		MT_RA_OPEN_SERVER_INFO_SC = 6608,								// �����������Ϣ
		MT_RA_OPEN_SERVER_UPGRADE_INFO_SC = 6609,						// ���������ȫ��������Ϣ
		MT_RA_OPEN_SERVER_RANK_INFO_SC = 6610,							// ���������������Ϣ
		MT_BAI_BEI_FAN_LI_INFO_SC = 6611,								// �ٱ�������Ϣ
		MT_BAI_BEI_FAN_LI_BUY_CS = 6612,								// �ٱ�����������Ʒ
		MT_SHUIJING_TASK_INFO_SC = 6613,								// ˮ���þ���������Ϣ֪ͨ
		MT_CROSS_TEAM_FB_SELF_INFO_SC = 6614,							// �����ӱ��Լ���Ϣ
		MT_FETCH_VIP_WEEK_REWARD = 6615,								// ��ȡvip�ܽ���
		MT_IS_ACCEPT_MARRY_SC = 6616,									// �Ƿ�ͬ����
		MT_CROSS_SHUIJING_PLAYER_INFO_SC = 6617,						// ���ˮ���þ��������Ϣ֪ͨ
		MT_CROSS_SHUIJING_BUY_BUFF_CS = 6618,							// ���ˮ���þ�������buff
		MT_RA_OPEN_SERVER_BOSS_INFO_SC = 6619,							// ���������Boss������Ϣ
		MT_RA_OPEN_SERVER_BATTEL_INFO_SC = 6620,						// �������������������Ϣ
		MT_SHUIJING_PLAYER_INFO_SC = 6621,								// ˮ���þ��������Ϣ֪ͨ
		MT_SHUIJING_BUY_BUFF_CS = 6622,									// ˮ���þ�������buff
		MT_WELFARE_CHONGJIHAOLI_REWARD_CS = 6623,                       // ��ȡ�弶����
		MT_MAGICAL_PRECIOUS_FETCH_REWARD_CS = 6624,                     // ��ȡħ���ر�����
		MT_MAGICAL_PRECIOUS_ALL_INFO_REQ_SC = 6625,						// ħ���ر�������Ϣ
		MT_RA_OPEN_GAME_GIFT_SHOP_INFO_SC = 6626,						// ����޹����Ϣ
		MT_RA_OPEN_BUY_GAME_GIFT_SHOP_INFO_CS = 6627,					// �����޹��������
		MT_RA_OPEN_GAME_GIFT_SHOP_INFO_CS = 6628,						// ��ȡ����޹����Ϣ����
		MT_RA_EXP_REFIN_REQ_CS = 6629,									// ������������
		MT_RA_EXP_REFINE_INFO_SC = 6630,								// ����������Ϣ
		MT_FETCH_TIME_LIMIT_VIP_CS = 6631,								// ��ȡ��ʱvip
		MT_SHUIJING_FETCH_TASK_REWARD_CS = 6632,						// ˮ���þ�����ȡ������
		MT_SHUIJING_GATHER_WUDI_INFO = 6633,							// ˮ���þ����ɼ����ж�
		MT_MAGICAL_PRECIOUS_CUR_CHAPTER_INFO_REQ_SC = 6634,				// ħ���ر���ǰ�½���Ϣ
		MT_SHUIJING_SCENE_INFO_SC = 6635,								// ˮ���þ���������Ϣ֪ͨ
		MT_MAGICAL_PRECIOUS_CONDITION_CHANGE_SC = 6636,					// ħ���ر������仯��Ϣ
		MT_WANGLINGEXPLORE_BOSS_INFO_SC = 6637,							// ����̽��BOSS��Ϣ
		MT_WANGLINGEXPLORE_BUY_BUFF_CS = 6638,							// ����̽����buff

		MT_CROSS_BIANJINGZHIDI_USER_INFO_SC = 6640,						// ����߾�֮�ؽ�ɫ��Ϣ
		MT_CROSS_BIANJINGZHIDI_BOSS_INFO_SC = 6641,						// ����߾�֮��BOSS��Ϣ
		MT_CROSS_BIANJINGZHIDI_BOSS_HURT_INFO_SC = 6642,				// ����߾�֮��BOSS�˺�������Ϣ
		MT_CROSS_BIANJINGZHIDI_BUY_BUFF_CS = 6643,						// ����߾�֮����buff

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_LOVE_TREE_WATER_REQ_CS = 6650,								// ��˼��-����ˮ
		MT_LOVE_TREE_INFO_REQ_CS = 6651,								// ��˼��-������˼����Ϣ
		MT_LOVE_TREE_SEND_INFO_SC = 6675,							    // ��˼��-������˼����Ϣ
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_GUILD_INFO_STATISTIC_REQ_CS = 6690,							// �������ͳ����Ϣ����
		MT_GUILD_INFO_STATISTIC_SC = 6691,								// �������ͳ����Ϣ 
		MT_GUILD_MVP_INFO_SC = 6692,									// ����MVP��Ϣ����

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_GCZ_GLOBAL_INFO_SC = 6701,									// ����ս ȫ����Ϣ���㲥��
		MT_GCZ_ROLE_INFO_SC = 6702,										// ����ս ������Ϣ
		MT_GCZ_REWARD_INFO_SC = 6703,							        // ����ս ��������
		MT_GONGCHENGZHAN_OWNER_INFO_SC = 6704,							// ����ս ������Ϣ
		MT_GONGCHENGZHAN_GET_OWNER_INFO_REQ_CS = 6705,					// ����ս ��ȡ������Ϣ
		MT_GCZ_FLAG_INFO_SC = 6706,										// ����ս ������Ϣ
		MT_GCZ_CHANGE_PLACE_CS = 6751,									// ����ս �ı��ɫ���ڵ�ͼ�ϵ�λ��
		MT_GCZ_FETCH_DAILY_REWARD_CS = 6752,							// ����ս ��ȡÿ�ս���
		MT_GCZ_LUCK_REWARD_SC = 6753,									// ����ս ���˽���

		MT_GCZ_WORSHIP_INFO_SC = 6754,									// ����սĤ����Ϣ
		MT_GCZ_WORSHIP_ACTIVITY_INFO_SC = 6755,							// ����սĤ�ݻ��Ϣ
		MT_GCZ_WORSHIP_CS = 6756,										// ����սĤ�ݵ��

		MT_WORSHIP_COMMON_INFO_SC = 6757,								// �·�Ĥ����Ϣ(���������)
		MT_WORSHIP_COMMON_START_SC = 6758,								// �㲥��ɫ��ʼĤ��(���������)
		MT_WORSHIP_COMMON_CS = 6759,									// Ĥ�ݵ��(���������)

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_XN_SHOUHU_UPSTAR_CS = 6800,									// ��Ů�ػ���������
		MT_XN_SHOUHU_USE_IMAGE_CS = 6801,								// ��Ů�ػ�����ʹ������
		MT_XN_SHOUHU_INFO_REQ_CS = 6802,								// ��Ů�ػ������ػ���Ϣ

		MT_XN_SHOUHU_INFO_SC = 6825,									// ��Ů�ػ���Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_JL_GUANGHUAN_UPSTAR_CS = 6850,								// ����⻷��������
		MT_JL_GUANGHUAN_USE_IMAGE_CS = 6851,							// ����⻷����ʹ������
		MT_JL_GUANGHUAN_INFO_REQ_CS = 6852,								// ����⻷������Ϣ
		MT_JL_GUANGHUAN_SPECIAL_IMG_UPGRADE_CS =6853,					// ����⻷�����������

		MT_JL_GUANGHUAN_INFO_SC = 6875,									// ����⻷��Ϣ

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FAIRY_BUDDHA_CARD_REQ_CS = 6855,                  // ���𿨼�������
		MT_FAIRY_BUDDHA_CARD_INFO_SC = 6856,                 // ������Ϣ
		MT_FAIRY_BUDDHA_CARD_GOLD_REQ_CS = 6857,             // ��ȡ��Ԫ����

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_GUILD_QUESTION_ENTER_REQ_CS = 6860,					// ���ɴ��� ��������
		MT_GUILD_QUESTION_PLAYER_INFO_SC = 6861,				// ���ɴ��� ��Ҵ�����Ϣ
		MT_GUILD_QUESTION_GUILD_RANK_INFO_SC = 6862,			// ���ɴ��� ���ɻ�������
		MT_GUILD_QUESTION_QUESTION_INFO_SC = 6863,				// ���ɴ��� ��Ŀ��Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_JL_FAZHEN_UPSTAR_CS = 6900,									// ���鷨����������
		MT_JL_FAZHEN_USE_IMAGE_CS = 6901,								// ���鷨������ʹ������
		MT_JL_FAZHEN_INFO_REQ_CS = 6902,								// ���鷨��������Ϣ
		MT_JL_FAZHEN_SPECIAL_IMG_UPGRADE_CS =6903,						// ���鷨�������������

		MT_JL_FAZHEN_INFO_SC = 6925,									// ���鷨����Ϣ
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ZHUANSHENG_OPERA_REQ_CS = 6950,								// ת����������

		MT_ZHUANSHENG_ALL_INFO_SC = 6975,								// ת��������Ϣ
		MT_ZHUANSHENG_BACKPACK_INFO_SC = 6976,							// ת��������Ϣ
		MT_ZHUANSHENG_OTHER_INFO_SC = 6977,								// ת��������Ϣ
		MT_ZHUANSHENG_CHOU_RESULT_SC = 6978,							// ת���齱���
		MT_ZHUANSHENG_COMBINE_RESULT_SC = 6979,							// ת���ϳɽ��
		MT_ZHUANSHENG_XIUWEI_NOTICE_SC = 6980,							// ת����Ϊ��ʾ��Ϣ

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RUNE_SYSTEM_REQ_CS = 6981,									// ����ϵͳ ��������
		MT_RUNE_SYSTEM_BAG_INFO_SC = 6982,								// ����ϵͳ ������Ϣ
		MT_RUNE_SYSTEM_RUNE_GRID_INFO_SC = 6983,						// ����ϵͳ ���ĸ�����Ϣ
		MT_RUNE_SYSTEM_RUNE_OTHER_INFO_SC = 6984,						// ����ϵͳ ������Ϣ
		MT_RUNE_TOWER_FETCH_TIME_CS = 6985,								// ��������ȡ����ʱ��
		MT_RUNE_TOWER_INFO_SC = 6986,									// ������ ��Ϣ
		MT_RUNE_TOWER_FB_INFO_SC = 6987,								// ������ ������Ϣ
		MT_RUNE_TOWER_OFFLINE_INFO_SC = 6988,							// ���������߹һ���Ϣ
		MT_RUNE_SYSTEM_DISPOSE_ONE_KEY_CS = 6989,						// ����ϵͳ һ���ֽ�
		MT_RUNE_SYSTEM_COMPOSE_INFO_SC = 6990,							// ����ϵͳ ���ĺϳɽ��
		MT_RUNE_SYSTEM_AWAKEN_INFO_SC = 6991,							// ����ϵͳ ���ľ��ѽ��
		MT_RUNE_SYSTEM_AUTO_FB_CS = 6992,								// ������ ɨ��
		MT_RUNE_SYSTEM_AUTO_REWARD_SC = 6993,							// ������ ɨ������

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_TALENT_SKILL_TARGET_LIST = 7252,						// �츳�����˺��б�
		MT_TALENT_OPERATE_CS = 7260,							// �츳����
		MT_TALENT_INFO_SC = 7261,								// �츳��Ϣ

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_LITTLE_PET_OPERA_REQ_CS = 8001,								// С�����������
		MT_LITTLE_PET_RENAME_CS = 8002,									// С�����������cs

		MT_LITTLE_PET_ALL_INFO_SC = 8050,								// С����ȫ����Ϣ 
		MT_LITTLE_PET_SINGLE_INFO_SC = 8051,							// С���ﵥ����Ϣ
		MT_LITTLE_PET_PEI_CHANGE_INFO_SC = 8052,						// С����ı���Ϣ
		MT_LITTLE_PET_PEI_CHOUJIANG_INFO_SC = 8053,						// С����齱��Ϣ
		MT_LITTLE_PET_NOTIFY_INFO_SC = 8054,							// С����������Ϣ
		MT_LITTLE_PET_FRIEND_INFO_SC = 8055,							// С���������Ϣ
		MT_LITTLE_PET_USING_IMG_SC = 8056,								// ʹ�õ�С��������
		MT_LITTLE_PET_FRIEND_PET_LIST_SC = 8057,						// ���ѳ����б�
		MT_LITTLE_PET_INTERACT_LOG_SC = 8058,							// ������¼
		MT_LITTLE_PET_RENAME_SC = 8059,									// С�����������sc
		MT_LITTLE_PET_WALK_SC = 8060,									// ��С����
		
		MT_BABY_TYPE_WALK_SC = 8061,									// �ﱦ��

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CHINESE_ZODIAC_ALL_INFO_SC = 8200,							// ����ϵͳ������Ϣ
		MT_CHINESE_ZODIAC_EQUIP_INFO_SC = 8201,							// ����ϵͳװ����Ϣ
		MT_CHINESEZODIAC_PROMOTE_XINGHUN_LEVEL_CS = 8202,				// ���������ǻ�ȼ�
		MT_CHINESEZODIAC_PROMOTE_EQUIP_LEVEL_CS = 8203,					// ��������װ���ȼ�
		MT_CHINESEZODIAC_PROMOTE_LEVEL_CS = 8204,						// ���������ȼ�
		MT_CHINESEZODIAC_TIANXIANG_REQ_CS = 8205,						// ��������
		MT_CHINESEZODIAC_TIANXIANG_INFO_SC = 8206,						// ��������Ϣ
		MT_CHINESEZODIAC_TIANXIANG_BEAD_SC = 8207,						// ���󵥸�����Ϣ
		MT_CHINESEZODIAC_TIANXIANG_COMBIND_SC = 8208,					// ���������Ϣ
		MT_CHINESEZODIAC_GUNGUN_LE_SC = 8209,							// ��������Ϣ
		MT_CHINESEZODIAC_SINGLE_CHANGE_SC = 8210,						// �ؼ������ı���Ϣ
		MT_CHINESEZODIAC_COMBINE_SUCC_SC = 8211,						// �ؼ��ϳɳɹ�
		MT_CHINESEZODIAC_XINGLING_INFO_SC = 8212,						// ������Ϣ
		MT_CHINESEZODIAC_TAKE_OFF_EQUIP_CS = 8213,						// ж��װ��
		MT_CHINESEZODIAC_RECYCLE_CS = 8214,								// ��Фװ������

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MO_JIE_ALL_INFO_SC = 8250,									// ħ��������Ϣ

		MT_BIG_CHATFACE_UP_LEVEL_REQ_CS = 8275,							// �������������
		MT_TUHAOJIN_UP_LEVEL_REQ_CS = 8276,								// ��������������

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FUN_OPEN_MOUNT_INFO_SC = 8300,								// ���ܿ�������-����
		MT_FUN_OPEN_WING_INFO_SC = 8301,								// ���ܿ�������-����
		MT_FUN_OPEN_XIANNV_INFO_SC = 8302,								// ���ܿ�������-Ů��

		MT_FUN_OPEN_STORY_STEP_CS = 8325,								// ���ܿ�������-����ˢ��
		MT_FUN_OPEN_SET_OBJ_POS_CS = 8326,								// ���ܿ�������-����OBJλ��
		MT_FUN_OPEN_CUR_STEP_END = 8327,								// ���ܿ�������-��ǰ�������

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FRIEND_GIFT_INFO_SC = 8350,									// ��������-����Ϣ
		MT_FRIEND_GIFT_SONG_GIFT_CS = 8351,								// ��������-����
		MT_FRIEND_GIFT_ALL_INFO_GIFT_CS = 8352,							// ��������-����Ϣ����
		MT_FRIEND_GIFT_SHOU_NOTICE_CS = 8353,							// ��������-�յ��������ʾ
		MT_FRIEND_HELI_NOTICE_SC = 8354,								// ���Ѻ���-�����ͺ�������ѵ�֪ͨ
		MT_FRIEND_HELI_SEND_REQ_CS = 8355,								// ���Ѻ���-���󷢺���
		MT_FRIEND_HELI_SEND_SC = 8356,									// ���Ѻ���-������
		MT_GIVE_ITEM_OPERA_CS = 8357,									// ������Ʒ��������
		MT_GIVE_ITEM_REQ_CS = 8358,										// ����������Ʒ
		MT_GIVE_ITEM_RECORD_SC = 8359,									// ������Ʒ��¼

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FRIEND_TUODAN_ALL_INFO_CS = 8371,							// ��Ҫ�ѵ�-����������Ϣ
		MT_FRIEND_TUODAN_ALL_INFO_SC = 8372,							// ��Ҫ�ѵ�-ȫ����Ϣ
		MT_FRIEND_TUODAN_OPERA_REQ_CS = 8373,							// ��Ҫ�ѵ�-��������
		MT_FRIEND_TUODAN_SINGLE_INFO_SC = 8374,							// ��Ҫ�ѵ�-������Ϣ

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_QINGYUAN_EQUIP_OPERATE_CS = 8392,							// ��Ե�����ȼ�
		MT_QINGYUAN_EQUIP_INFO_SC = 8394,								// ��Եװ����Ϣ
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FB_GUIDE_FINISH = 8400,										// ��������-���
		MT_FB_GUIDE_CREATE_GATHER = 8401,								// ��������-�����ɼ���
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_RED_ENVELOPE_INFO = 8410,									// �������-������Ϣ
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_MARRY_ME_COUPLE_INFO_SC = 8416,							// ���ǽ���CP�б���Ϣ

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_OPEN_SERVER_INVEST_INFO_SC = 8417,						// ����Ͷ����Ϣ

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CHONGZHI_7DAY_FETCH_REWARD_CS = 8420,						// ��ֵ-���췵����ȡ����

		MT_SHENGE_SYSTEM_REQ_CS = 8421,									// ���ϵͳ����
		MT_SHENGE_SYSTEM_INFO_SC = 8422,								// �����Ϣ�·�
		MT_SHENGE_SYSTEM_ZHANGKONG_INFO_SC = 8423,						// ����ƿ���Ϣ
		MT_SHENGE_SYSTEM_ZHANGKONG_SIGN_INFO_SC = 8424,					// ����ƿص���������Ϣ

		MT_GOLD_TURNTABLE_OPERATE_REQ_CS = 8425,						// Ԫ��ת�� ��������
		MT_GOLD_TURNTABLE_REWARD_SC = 8426,								// Ԫ��ת�� ��Ʒ��Ϣ
		MT_GOLD_TURNTABLE_INFO_SC = 8427,								// Ԫ��ת�� ��Ϣ

		MT_GOLDEN_PIG_OPERATE_REQ_CS = 8428,							// �����ٻ���������
		MT_GOLDEN_PIG_SENG_INFO_SC = 8429,								// �����ٻ���Ϣ
		MT_GOLDEN_PIG_BOSS_STATE_SC = 8435,								// �����ٻ�boss״̬

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_TUITU_FB_OPERA_REQ_CS = 8430,								// ��ͼ��������
		MT_TUITU_FB_INFO_SC = 8431,										// ��ͼ����������Ϣ
		MT_TUITU_FB_RESULT_SC = 8432,									// ��ͼ���������Ϣ
		MT_TUITU_FB_SINGLE_INFO_SC = 8433,								// ��ͼ���������ؿ���Ϣ
		MT_TUITUI_FB_FETCH_RESULT_SC = 8434,							// ��ͼ������ȡ�Ǽ��������

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SHENGE_SYSTEM_SHENQU_XILIAN_ALL_INFO_SC = 8440,				// �������ϴ����Ϣ����
		MT_SHENGE_SYSTEM_SHENQU_XILIAN_INFO_SC = 8441,					// �����������ϴ����Ϣ����

		MT_RUNE_SYSTEM_ZHULING_NOTIFY_INFO_SC = 8442,					// ����ע��֪ͨ��Ϣ
		MT_RUNE_SYSTEM_ZHULING_ALL_INFO_SC = 8443,						// ����ע��������Ϣ����

		MT_SHEN_YIN_SCORE_INFO_SC = 8444,									// ��ӡ������Ϣ
		MT_SHEN_YIN_BAG_INFO_SC = 8445,										// ��ӡ���ⱳ����Ϣ
		MT_SHEN_YIN_SHOP_INFO_SC = 8446,									// ��ӡλ��Ϣ
		MT_SHEN_YIN_LIEHUN_POOL_INFO_SC = 8447,								// ��ӡ�Ի����Ϣ

		MT_RA_PLANTING_TREE_RANK_INFO = 8448,							// ����-Ȥζֲ�����а���Ϣ
		MT_RA_PLANTING_TREE_TREE_INFO = 8449,							// ����-Ȥζֲ��һ������Ϣ
		MT_RA_PLANTING_TREE_MINI_MAP_INFO = 8450,						// ����-Ȥζֲ��С��ͼ��Ϣ
		MT_RA_FISHING_ALL_INFO_SC = 8451,								// ����-������Ϣ
		MT_RA_FISHING_STATUS_SC = 8452,									// ����-������Ϣ�㲥
		MT_RA_CRYSTAL_DALARAN_INFO_SC = 8453,							// ����-����Ȼˮ����Ϣ

		MT_ELEMENT_HEART_REQ_CS = 8454,									// �������
		MT_ELEMENT_HEART_INFO_SC = 8455,								// Ԫ����Ϣ
		MT_ELEMENT_HEART_CHARM_INFO_SC = 8456,							// Ԫ��֮���б���Ϣ
		MT_ELEMENT_HEART_SINGLE_CHARM_INFO_SC = 8457,					// ����Ԫ��֮����Ϣ
		MT_ELEMENT_HEART_REWARD_INFO_SC = 8458,							// �齱��Ϣ
		MT_ELEMENT_HEART_PRODUCT_INFO_SC = 8459,						// ������Ʒ��Ϣ
		MT_ELEMENT_SHOP_INFO_SC = 8460,									// Ԫ��֮���̵���Ϣ
		MT_ELEMENT_XILIAN_SINGLE_INFO_SC = 8461,						// Ԫ��֮��ϴ��������Ϣ
		MT_ELEMENT_XILIAN_ALL_INFO_SC = 8462,							// Ԫ��֮��ϴ��ȫ����Ϣ
		MT_RA_CRAZY_REBATE_CHONGZHI_INFO = 8463,						// ��Ԫ����ֵ��Ϣ
		MT_RA_LOVE_DAILY_CHONGZHI_INFO = 8464,							// ÿ��һ����ֵ��Ϣ

		MT_SHEN_YIN_ONE_KEY_RECYLE_CS = 8465,							// ��ӡϵͳ�����������ղ���

		MT_UPGRADE_RUSH_INFO_SC = 8466,									// ���ٽ���

		MT_RA_CONSUM_GIFT_INFO_SC = 8467,                               // ����--�����콱
		MT_RA_CONSUM_GIFT_ROLL_REWARD_SC = 8469,                        // ����--�����콱ҡ������
		MT_RA_CRITICAL_STRIKE_INFO_SC = 8468,                           // ����--������
		MT_RA_LIMIT_BUY_INFO_SC = 8470,                                 // ����--ÿ���޹�
		MT_RA_COLLECT_TREASURE_INFO_SC = 8471,                          // ����--�۱�����
		MT_RA_COLLECT_TREASURE_RESULT_SC = 8472,                        // ����--�۱�����  ҡ�����
		MT_RA_HAPPY_CUMUL_CHONGZHI_INFO_SC = 8473,                      // ����--�����۳�  
		MT_RA_JINJIE_RETURN_INFO_SC = 8474,                             // ����--���׷���
		MT_RA_LIMIT_TIME_REBATE_INFO_SC = 8475,                         // ����--��ʱ����
		MT_RA_TIME_LIMIT_GIFT_INFO_SC = 8476,                           // ����--��ʱ���
		MT_RA_CIRCUL_CHONGZHI_INFO_SC = 8477,                           // ����--ѭ����ֵ
		MT_RA_SHAKE_MONEY_INFO_SC = 8478,                               // ����--���ҡǮ��
		MT_RA_TIME_LIMIT_LUXURY_GIFT_INFO_SC = 8479,                    // ����--��ʱ���� 
		MT_RA_DOUBLE_CHONGZHI_INFO_SC = 8480,                           // ����--����ͬ��
		MT_RA_RMB_CHEST_SHOP_INFO_SC = 8481,                            // ����--�����̵�
		MT_RA_CONSUME_GOLD_INFO_SC = 8482,                              // ����--���ѷ��� 
		MT_RA_BUY_ONE_GET_ONE_FREE_INFO_SC = 8483,						// ����--��һ��һ
		

		MT_RA_VERSION_TOTAL_CHARGE_INFO_SC = 8484,                      // �汾�--�ۼƳ�ֵ
		MT_RA_VERSION_CONTINUE_CHARGE_INFO_SC = 8485,                   // �汾�--������ֵ
		MT_RA_HUANLE_YAOJIANG_TWO_INFO_SC = 8486,                       // �汾�--����ҡ��2
		MT_RA_HUANLE_YAOJIANG_TWO_TAO_SC = 8487,                        // �汾�--����ҡ��2
		MT_RA_OFFLINE_SINGLE_CHARGE_INFO_SC = 8488,                     // �汾�--���µ��ʳ�ֵ
		MT_RA_EXTREME_CHALLENGE_INFO_SC = 8489,                         // �汾�--������ս
		MT_RA_COLLECT_SECOND_INFO_SC = 8490,                            // ����--���±��

		MT_RA_TOTAL_CHARGE_5_INFO_SC = 8491,							// �汾�--�ۼƳ�ֵ5 ��Ϣ

		MT_RA_CHONGZHI_GIFT_INFO_CS = 8492,                             // ����--�������

		MT_CSA_TOUZI_JIHUA_INFO_CS = 8493,                              // �Ϸ��--Ͷ�ʼƻ�
		MT_CSA_FOUNDATION_INFO_CS = 8494,                               // �Ϸ��--�ɳ����� 
		MT_CSA_EXPREFINE_INFO_CS = 8495,                                // �Ϸ��--��������
		MT_CSA_GONGCHENGZHAN_INFO_CS = 8496,                            // �Ϸ��--����ս
		MT_RA_TOTAL_CHARGE_5_INFO_CS = 8497,							// �汾�--�ۼƳ�ֵ5 ������Ϣ

		MT_RA_JINJIE_RETURN_INFO2_SC = 8498,                            // ����--���׷���2
		MT_RA_CRITICAL_STRIKE2_INFO_SC = 8499,                          // ����--������2

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FB_NEQ_INFO_SC = 8501,										// ��װ����������Ϣ
		MT_FB_NEQ_PASS_INFO_SC = 8502,									// ��װ����ͨ����Ϣ
		MT_FB_NEQ_ROLL_POOL_SC = 8503,									// ��װ��������
		MT_FB_NEQ_ROLL_INFO_SC = 8504,									// ��װ����������Ϣ

		MT_FB_NEQ_TAKEREWARD_CS = 8511,									// ��װ���������콱
		MT_FB_NEQ_EXCHANGE_CS = 8512,									// ��װ��������һ�
		MT_FB_NEQ_BUY_TIMES_CS = 8513,									// ��װ�������������
		MT_FB_NEQ_AUTO_CS = 8514,										// ��װ��������ɨ��
		MT_FB_NEQ_INFO_CS = 8515,										// ��װ��������ؿ���Ϣ
		MT_FB_NEQ_ROLL_CS = 8516,										// ��װ����������

		MT_GET_DROP_LOG_CS = 8521,										// ������־����
		MT_DROP_LOG_RET_SC = 8522,										// ������־����

		MT_BABY_BOSS_OPERATE_CS = 8525,									// ����boss����
		MT_BABY_BOSS_ROLE_INFO_SC = 8526,								// ����boss��ɫ��Ϣ
		MT_ALL_BABY_BOSS_INFO_SC = 8527,								// ����boss������Ϣ
		MT_SINGLE_BABY_BOSS_INFO_SC = 8528,								// ����boss��Ϣ

		MT_FB_ARMORDEFEND_ROLE_REQ_CS = 8535,							// ���߲��ϸ�������
		MT_FB_ARMORDEFEND_ROLE_INFO_SC = 8536,							// ���߲��ϸ�����ɫ��Ϣ
		MT_FB_ARMORDEFEND_RESULT_SC = 8537,								// ���߲��ϸ������
		MT_FB_ARMORDEFEND_INFO_SC = 8538,								// ���߲��ϸ�����Ϣ
		MT_FB_ARMORDEFEND_WARNING_SC = 8539,							// ���߲��ϸ�����ʾ
		MT_FB_ARMORDEFEND_PERFORM_SKILL_SC = 8540,						// ���߲��ϸ����ͷż���

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SHANGGU_BOSS_OPERATE_CS = 8545,                              // �Ϲ�boss����
		MT_SHANGGU_BOSS_ALL_INFO_SC = 8546,							    // �Ϲ�boss������Ϣ
		MT_SHANGGU_BOSS_LAYER_INFO_SC = 8547,							// �Ϲ�boss������Ϣ
		MT_SHANGGU_BOSS_SCENE_INFO_SC = 8548,							// �Ϲ�boss��������Ϣ
		MT_SHANGGU_BOSS_SCENE_OTHER_INFO_SC = 8549,						// �Ϲ�boss������������Ϣ

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MESS_BATTLE_ROLE_INFO_SC = 8560,										// �Ҷ�ս��������Ϣ
		MT_MESS_BATTLE_ENTER_REQ_CS = 8561,										// �Ҷ�ս����������
		MT_MESS_BATTLE_RANK_INFO_SC = 8562,										// �Ҷ�ս��������Ϣ
		MT_MESS_BATTLE_REWARD_INFO_SC = 8563,									// �Ҷ�ս��������Ϣ
		MT_MESS_BATTLE_HURT_RANK_LIST_INFO_SC = 8564,							// �Ҷ�ս���˺�����
		MT_MESS_BATTLE_HURT_ALL_ROLE_SCORE_INFO_SC = 8565,						// �Ҷ�ս����Ա������Ϣ
		MT_MESS_BATTLE_TOTAL_RANK_SC = 8566,									// �Ҷ�ս���ܻ������а�

		//////////////////////////////////////// ������������ //////////////////////////////////////////////////////////
		MT_BUILD_TOWER_OPERA_CS = 8570,									// ��������������
		MT_BUILD_TOWER_SCENE_INFO_SC = 8571,							// ��������Ϣ�·�
		MT_BUILD_TOWER_BUY_TIMES_CS = 8572,								// �������������		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CROSS_LIEKUN_FB_SCENE_INFO_SC = 8575,								// ����ش� ������Ϣ
		MT_CROSS_LIEKUN_FB_LIVE_FLAG_SC = 8576,									// ����ش� ��boss�������Ϣ
		MT_CROSS_LIEKUN_FB_BOSS_HURT_INFO = 8577,								// ����ش� boss�˺���Ϣ����

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FB_TEAMTOWERDEFEND_INFO_SC = 8580,					// ����ػ�������������Ϣ
		MT_FB_TEAMTOWERDEFEND_ATTR_TYPE_INFO_SC = 8581,			// ����ػ� ������Ϣ
		MT_FB_TEAMTOWERDEFEND_SKILL_INFO_SC = 8582,				// ����ػ� ������Ϣ
		MT_FB_TEAMTOWERDEFEND_OPREAT_REQ_CS = 8583,				// ����ػ� �ͻ�������
		MT_FB_TEAMTOWERDEFEND_ALL_ROLE_SC = 8584,				// ����ػ� ������Ϣ
		MT_FB_TEAMTOWERDEFEND_RESULTS_SC = 8585,				// ����ػ� ������Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SHENQI_OPERA_REQ_CS = 8586,							// ��������
		MT_SHENQI_ALL_INFO_SC = 8587,							// ����������Ϣ
		MT_SHENQI_SINGLE_INFO_SC = 8588,						// ����������Ϣ
		MT_SHENQI_IMAGE_INFO_SC = 8589,							// ����������Ϣ
		MT_SHENQI_DECOMPOSE_RESULT_SC = 8590,					// �����ֽ���

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_GREATE_SOLDIER_OPERA_CS = 8595,						// ������������
		MT_GREATE_SOLDIER_UPGREADE_EQUIP_CS = 8596,				// ��������װ��

		MT_GREATE_SOLDIER_ITEM_INFO_SC = 8600,					// ������Ϣ
		MT_GREATE_SOLDIER_OTHER_SC = 8601,						// ����������Ϣ
		MT_GREATE_SOLDIER_SLOT_SC = 8602,						// ������λ��Ϣ
		MT_GREATE_SOLDIER_FETCH_REWARD_SC = 8603,				// ������ȡ�׶ν���
		MT_GREATE_SOLDIER_GOAL_INFO_SC = 8604,					// ����Ŀ����Ϣ
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		MT_HUGUOZHILI_INFO_SC = 8610,							// ������ - ��Ϣ
		MT_HUGUOZHILI_REQ_CS = 8611,							// ������ - ����
	
		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_WORLD_ACQUISITION_LOG_SC = 8612,						// ȫ���չ��·� - ��Ϣ
		MT_WORLD_ACQUISITION_LOG_REQ_CS = 8613,					// ȫ���չ���� - ����

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FIGHT_BACK_ROLE_LIST_SC = 8615,						// �·������б����uid

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_LUCKY_LOG_ITEM_REQ_CS = 8620,						// ���˶���������
		MT_LUCKY_LOG_ITEM_RET_SC = 8621,						// ���˶����ݷ���

		MT_CROSS_GOAL_OPERA_REQ_CS = 8625,						// ���Ŀ������
		MT_CROSS_GOAL_INFO_SC = 8626,							// ���Ŀ����Ϣ
		MT_CROSS_GOAL_GUILD_NOTIFY = 8627,						// ���Ŀ��������Ϣ��ʾ
		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_TIANSHENHUTI_ALL_EQUIP_INFO_SC = 8628,				// ��ĩװ������װ����Ϣ
		MT_TIANSHENHUTI_ROLL_RESULT_SC = 8629,					// ��ĩװ���齱���
		MT_TIANSHENHUTI_OPERA_REQ_RESULT_SC = 8630,				// ��ĩװ�����������
		MT_TIANSHENHUTI_OPERA_REQ_CS = 8631,					// ��ĩװ����ز�������
		MT_TIANSHENHUTI_SCORE_CHANGE_CS = 8632,					// ��ĩװ�����ֱ䶯
		MT_TIANSHENHUTI_COMBINE_ONEKEY_RESULT_SC = 8633,		// ��ĩװ��һ���ϳɷ���
		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_SHENGXINGZHULI_INFO_REQ_CS = 8700,							// ����������Ϣ����
		MT_SHENGXINGZHULI_INFO_ACK_SC = 8701,							// ����������Ϣ�ظ�

		MT_SHENGXINGZHULI_GET_REWARD_REQ_CS = 8702,						// ���������콱����
		MT_SHENGXINGZHULI_GET_REWARD_ACK_SC = 8703,						// ���������콱�ظ�

	    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SHENGQI_INFO_SC = 8704,												// ʥ����Ϣ
		MT_SHENGQI_REQ_CS = 8705,												// ʥ����������																		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SEAL_REQ_CS = 8706,													// ʥӡϵͳ ��������
		MT_SEAL_BACKPACK_INFO_SC = 8707,										// ʥӡϵͳ ������Ϣ
		MT_SEAL_SLOT_INFO_SC = 8708,											// ʥӡϵͳ ʥӡ����Ϣ
		MT_SEAL_BASE_INFO_SC = 8709,											// ʥӡϵͳ ʥӡ������Ϣ
		MT_SEAL_RECYCLE_REQ_CS = 8710,											// ʥӡϵͳ �ֽ�

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_JINJIE_TALENT_INFO_REQ_CS = 8720,									// �����츳��Ϣ
		MT_JINJIE_TALENT_ALL_INFO_SC = 8721,									// �����츳��Ϣ
		MT_JINJIE_TALENT_UPDATE_GRID_SC = 8722,									// �����츳��
		MT_JINJIE_TALENT_CHOUJIANG_PAGE_INFO_SC = 8723,							// �츳�齱ҳ����Ϣ
		MT_TALENT_ATTENTION_SKILL_SC = 8724,									// �����츳��ע����ID

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_YAOSHI_INFO_SC = 8730,								// ������Ϣ
		MT_YAOSHI_APPE_CHANGE_SC = 8731,						// ������۸ı�
		MT_YAOSHI_OPERA_REQ_CS = 8732,							// �����������
		MT_UPGRADE_SYS_INFO_SC = 8733,							// ����ϵͳ��Ϣ
		MT_UPGRADE_SYS_APPE_CHANGE_SC = 8734,					// ����ϵͳ��۸ı�
		MT_UPGRADE_SYS_OPERA_REQ_CS = 8735,						// ����ϵͳ�������

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_TOUSHI_INFO_SC = 8740,								// ͷ����Ϣ
		MT_TOUSHI_APPE_CHANGE_SC = 8741,						// ͷ����۸ı�
		MT_TOUSHI_OPERA_REQ_CS = 8742,							// ͷ���������
		MT_RESERVE_8743 = 8743,									// ����
		MT_RESERVE_8744 = 8744,									// ����
		MT_RESERVE_8745 = 8745,									// ����

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_QILINBI_INFO_SC = 8750,								// �������Ϣ
		MT_QILINBI_APPE_CHANGE_SC = 8751,						// �������۸ı�
		MT_QILINBI_OPERA_REQ_CS = 8752,							// ������������
		MT_RESERVE_8753 = 8753,									// ����
		MT_RESERVE_8754 = 8754,									// ����
		MT_RESERVE_8755 = 8755,									// ����

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MASK_INFO_SC = 8760,									// �����Ϣ
		MT_MASK_APPE_CHANGE_SC = 8761,							// �����۸ı�
		MT_MASK_OPERA_REQ_CS = 8762,							// ����������
		MT_RESERVE_8763 = 8763,									// ����
		MT_RESERVE_8764 = 8764,									// ����
		MT_RESERVE_8765 = 8765,									// ����

		MT_RED_COLLECT_INFO_SC = 8766,                          // ��װ�ռ�
		MT_RED_COLLECT_OTHER_INFO_SC = 8767,                    // ��װ�ռ�
		MT_ORANGE_COLLECT_INFO_SC = 8768,                       // ��װ�ռ�
		MT_ORANGE_COLLECT_OTHER_INFO_SC = 8769,                 // ��װ�ռ�

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ZHUANZHI_EQUIP_REQ_CS = 8770,							// תְװ������
		MT_ZHUANZHI_EQUIP_INFO_SC = 8771,							// תְװ����Ϣ
		MT_ZHUANZHI_STONE_INFO_SC = 8772,							// תְ��ʯ��Ϣ
		MT_ZHUANZHI_SUIT_INFO_SC = 8773,							// תְ��װ��Ϣ

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_BOSS_CARD_REQ_CS = 8775,							    // bossͼ�� ����
		MT_BOSS_CARD_INFO_SC = 8776,							// bossͼ�� ��Ϣ����

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FISHING_OPERA_REQ_CS = 8777,							// �����������

		MT_FISHING_USER_INFO_SC = 8778,							// ���������Ϣ
		MT_FISHING_CHECK_EVENT_RESULT_SC = 8779,				// �����ü���¼����
		MT_FISHING_GEAR_USE_RESULT_SC = 8780,					// ��������ʹ�ý��
		MT_FISHING_EVENT_BIG_FISH_SC = 8781,					// �����������¼���˵�еĴ���
		MT_FISHING_TEAM_MEMBER_INFO_SC = 8782,					// ����������Ϣ
		MT_FISHING_FISH_INFO_SC = 8783,							// �������ϵ�����Ϣ
		MT_FISHING_RAND_USER_INFO_SC = 8784,					// ���������ɫ��Ϣ
		MT_FISHING_SCORE_INFO_SC = 8785,						// ����������Ϣ
		MT_FISHING_STEAL_RESULT_SC = 8786,						// ����͵����
		MT_FISHING_GET_FISH_BROCAST = 8787,						// ���������㲥
		MT_FISHING_SCORE_RANK = 8788,					    	// ��������������Ϣ
		MT_FISHING_STAGE_SCORE_INFO = 8789,						// ����׶λ�����Ϣ
		MT_FISHING_NOTIFY_STATUS_CHANGE_SC = 8790,				// ����״̬�ı�֪ͨ
		MT_FISHING_STEAL_INFO_SC = 8791,						// ����͵����Ϣ
		MT_FISHING_CONFIRM_RESULT_SC = 8792,					// ����ȷ���¼�����

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_NIGHT_FIGHT_ROLE_INFO_SC = 8795,						// ҹս����������Ϣ
		MT_NIGHT_FIGHT_REQ_CS = 8796,							// ҹս���ǲ�������
		MT_NIGHT_FIGHT_RANK_INFO_SC = 8797,						// ҹս����������Ϣ
		MT_NIGHT_FIGHT_REWARD_INFO_SC = 8798,					// ҹս���ǽ�����Ϣ
		MT_NIGHT_FIGHT_RED_SIDE_LIST_INFO_SC = 8799,			// ҹս���Ǻ췽obj_id�б���Ϣ
		MT_NIGHT_FIGHT_HURT_ALL_ROLE_SCORE_INFO_SC = 8800,		// ҹս������Ա������Ϣ
		MT_NIGHT_FIGHT_RANK_ROLE_POSI_INFO_SC = 8801,			// ҹս����ǰ����������Ϣ
		MT_NIGHT_FIGHT_HURT_BOSS_RANK_SC = 8802,				// ҹս����boss������Ϣ
		MT_NIGHT_FIGHT_TOTAL_RANK_SC = 8803,					// ҹս�����ܻ������а�

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_IMP_GURAD_OPERA_REQ_CS = 8805,						// ����С���ػ�����
		MT_IMP_GUARD_INFO_SC = 8806,							// С���ػ���Ϣ����
		MT_IMP_GUARD_APPE_CHANGE_SC = 8807,                     // С���ػ���۱任

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_HESHENLUOSHU_REQ_CS = 8808,											// ������������
		MT_HESHENLUOSHU_ALL_INFO_SC = 8809,										// ��������ȫ����Ϣ
		MT_HESHENLUOSHU_CHANGE_INFO_SC = 8810,									// ��������ı���Ϣ
		
		//////////////////////////////////////////////////////////////////////////
		MT_IMP_GUARD_EXPIRE_TIME_SC = 8811,                     // С����

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ROLE_ZHUANZHI_REQ_CS = 8812,                // תְ����
		MT_ROLE_ZHUANZHI_INFO_SC = 8813,               // תְ��Ϣ����
		MT_ROLE_ZHUANZHI_SKILL_TRIGGER_SC = 8814,      // תְ���ܴ���

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_PERFECT_LOVER_INFO_SC = 8815,                     // ����-��������

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_QUANMIN_JINJIE_INFO_SC = 8816,                     // ȫ�����

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_UPGRADE_GROUP_INFO_SC = 8817,                     // ȫ���ܶ�Ա

		MT_FUNE_TOWER_REWARD_INFO_SC = 8818,                    // ������������Ϣ

		MT_RA_LOGIN_GIFT_INFO_0_SC = 8819,						// ��¼������Ϣ

		MT_RA_EVERYDAY_NICE_GIFT_INFO_SC = 8820,				// ÿ�պ���
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_RA_ROLE_BIG_SMALL_GOAL_CS = 8821,                    // ��ɫ��СĿ������
		MT_RA_ROLE_BIG_SMALL_GOAL_INFO_SC = 8822,               // ��ɫ��СĿ����Ϣ

		MT_BOSS_DEAD_SC = 8824,                                  // boss����֪ͨͬ��������������
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FB_HOLIDAY_GUARD_ROLE_REQ_CS = 8825,							// ������������
		MT_FB_HOLIDAY_GUARD_ROLE_INFO_SC = 8826,						// ������ɫ��Ϣ
		MT_FB_HOLIDAY_GUARD_INFO_SC = 8827,								// �ճ���������������������Ϣ
		MT_FB_HOLIDAY_GUARD_DROP_INFO_SC = 8828,						// �����������ͳ��
		MT_FB_HOLIDAY_GUARD_RESULT_SC = 8829,							// ��������ͨ�ؽ��
		MT_FB_HOLIDAY_GUARD_WARNING_SC = 8830,							// �������������������򱨾�
		MT_RA_HOLIDAY_GUARD_KILL_RANK_INFO_SC = 8831,					// ��ɱ������Ϣ
		MT_RA_HOLIDAY_GUARD_INFO_CS = 8832,								// �����ػ�����Э���·�
		MT_RA_HOLIDAY_GUARD_NPC_INFO = 8833,							// �����ػ�npc��Ϣ�·�

		MT_RA_CONSUM_GIFT_ROLL_REWARD_TEN_SC = 8834,                    // ����--�����콱ҡ������
		MT_RA_COMBINEBUY_BUCKET_INFO_SC = 8835,                         // ������Ϲ���  
		MT_RA_COMBINEBUY_ITEM_INFO2_SC = 8836,                          // ������Ϲ���  

		MT_RA_DAY_CHONGZHI_REQ_CS = 8837,                               // ���췵��
		MT_RA_DAY_CHONGZHI_INFO_SC = 8838,                              // ���췵��

		MT_XIANZUNKA_REQ_CS = 8839,                                     // ��������
		MT_XIANZUNKA_INFO_SC = 8840,                                    // ������Ϣ����

		MT_ROLE_APPE_CHANGE_SC = 8841,                                  // ������۱仯

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SPOUSE_HOME_OPERE_REQ_CS = 8842,							// ���޼�԰��������
		MT_SPOUSE_HOME_ALL_ROOM_INFO_SC = 8843,						// ���з�����Ϣ
		MT_SPOUSE_HOME_FIREND_INFO_SC = 8844,								// ������Ϣ
		MT_SPOUSE_HOME_GUILD_MEMBER_INFO_SC = 8845,						// ������Ϣ
		MT_SPOUSE_HOME_GUILD_SINGLE_ROOM_INFO_SC = 8846,				// ����������Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_EQUIP_BAPTIZE_REQ_CS = 8847,                               // װ��ϴ������
		MT_EQUIP_BAPTIZE_INFO_SC = 8848,                               // װ��ϴ����Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SHENGE_SYSTEM_SUIT_INFO_SC = 8849,							//�ǻ���װ��Ϣ
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MIZANG_BOSS_RELIVE_TIRE_SC = 8860,                          // �ز�boss����ƣ��
		MT_MIZANG_BOSS_BOSS_INFO_CS = 8850,                            // �ز�boss����                      
		MT_MIZANG_BOSS_BOSS_INFO_SC = 8851,                            // �ز�boss������Ϣ����
		MT_MIZANG_BOSS_SCENE_INFO_SC = 8852,                           // �ز�boss������
		MT_MIZANG_BOSS_PLAYER_INFO_SC = 8853,                          // �ز�boss�����Ϣ

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_YOUMING_BOSS_RELIVE_TIRE_SC = 8855,                          // ��ڤboss����ƣ��
		MT_YOUMING_BOSS_BOSS_INFO_CS = 8856,                            // ��ڤboss����                      
		MT_YOUMING_BOSS_BOSS_INFO_SC = 8857,                            // ��ڤboss������Ϣ����
		MT_YOUMING_BOSS_SCENE_INFO_SC = 8858,                           // ��ڤboss������
		MT_YOUMING_BOSS_PLAYER_INFO_SC = 8859,                          // ��ڤboss�����Ϣ
			//8860  �ز�boss����ƣ��
		MT_RA_LUCKY_WISH_INFO_SC = 8861,								// ����-������Ը������Ϣ����
		MT_RA_LUCKY_WISH_OPERA_CS = 8862,								// ����-������Ը����
		MT_IMAGE_COMPETITION_INFO_SC = 8863,                            // �����ƴ�
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FETCH_DOUBLE_REWARD_CS = 8873,								// ������˫������ 
		MT_FETCH_DOUBLE_REWARD_RESULT_SC = 8874,						// ����˫������������� 
		MT_RAND_GIFT_REWARD_INFO_SC = 8875,								// ��������������Ʒ��Ϣ

		MT_PERSON_BOSS_INFO_CS = 8876,                                  // ����boss��Ϣ
		MT_PERSON_BOSS_INFO_SC = 8877,                                  // ����boss��Ϣ

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_UPGRADE_CARD_BUY_INFO_SC = 8878,						// ֱ����������Ϣ
		MT_UPGRADE_CARD_BUY_REQ_CS = 8879,						// ֱ������������

		MT_PERSON_AREAMSG_INFO_CS = 8888,                               // ���˹㲥��Ұ��Ϣ
		MT_PERSON_AREAMSG_INFO_SC = 8889,                               // ���˹㲥��Ұ��Ϣ

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CROSS_RA_REQUEST_OPERA_CS = 8895,					// �����������������
		MT_CROSS_RA_ACTIVITY_STATUS_SC = 8896,					// ����������״̬֪ͨ
		MT_CROSS_RA_CHONGZHI_RANK_INFO_SC = 8897,				// ��ֵ���У���ҳ�ֵ��Ϣ
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CLOUD_PURCHASE_TOTAL_INFO = 8900,					// һԪ�ᱦ���Ϣ
		MT_CLOUD_PURCHASE_CONVERT_INFO = 8901,					// һԪ�ᱦ�һ���Ϣ
		MT_CLOUD_PURCHASE_BUY_INFO = 8902,						// һԪ�ᱦ�����¼
		MT_CLOUD_PURCHASE_SERVER_RECORE_INFO = 8903,			// ȫ����¼
		MT_CLOUD_PURCHASE_USER_INFO_SC = 8904,					// һԪ�ᱦ������Ϣ

		MT_CROSS_RA_CONSUME_RANK_INFO_SC = 8905,				// �������У����������Ϣ

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_YUNYOU_BOSS_OPERA_CS = 8906,                          // ����boss����
		MT_YUNYOU_BOSS_INFO_SC = 8907,                           // ����boss��Ϣ 
		MT_YUNYOU_BOSS_SCENE_INFO_SC = 8908,                     // ����boss����������Ϣ 

		MT_GODMAGIC_BOSS_INFO_REQ_CS = 8910,					//��ħboss	
		MT_GODMAGIC_BOSS_INFO_ACK_SC = 8911,					//
		MT_GODMAGIC_PLAYER_INFO_SC = 8912,						//
		MT_GODMAGIC_BOSS_SCENE_INFO_SC = 8913,					//
		MT_GODMAGIC_BOSS_KILL_RECORD = 8914,					//
		MT_GODMAGIC_BOSS_DROP_RECORD = 8915,					//

		MT_TODAY_THEME_REWARD_REQ_CS = 8916,						// ���������콱����
		MT_TODAY_THEME_REWARD_FLAG_INFO_SC = 8917,					// ������������Ϣ

		MT_BAI_BEI_FAN_LI2_INFO_SC = 8918,								// �ٱ�����2��Ϣ
		MT_BAI_BEI_FAN_LI2_BUY_CS = 8919,								// �ٱ�����2������Ʒ

		MT_RA_OPEN_GAME_GIFT_SHOP2_INFO_SC = 8920,						// ����޹����Ϣ
		MT_RA_OPEN_BUY_GAME_GIFT_SHOP2_INFO_CS = 8921,					// �޹��������

		MT_ZERO_GIFT_GOD_COSTUME_INFO_SC = 8922,					// ��Ԫ����Ľ���װ��Ϣ

		MT_RA_LOVE_DAILY2_CHONGZHI_INFO = 8923,						// ÿ��һ��2

		MT_EQUIP_GRADE_SUIT_INFO_SC = 8924,							// ��ͨװ���ռ�
		MT_SCENE_ACTIVITY_REWARD_SC = 8925,							// �����չʾ

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_BAIZHAN_EQUIP_REQ_CS = 8926,								// ��սװ������
		MT_BAIZHAN_EQUIP_INFO_SC = 8927,							// ��սװ����Ϣ

		MT_ZHUANZHI_EQUIP_AWAKENING_ALL_INFO = 8928,				// ����תְװ��������Ϣ
		MT_ZHUANZHI_EQUIP_AWAKENING_INFO = 8929,					// һ��תְװ��������Ϣ

		MT_GUILD_RARE_LOG_RQE_CS = 8930,							// ������ϡ��־
		MT_GUILD_RARE_LOG_INFO_SC = 8931,							// ������ϡ��־

		MT_LITTLE_HELPER_REQ_CS = 8932,								// С��������

		MT_FIRST_RECHARGE_BUFF_FLAG_SC = 8933,						// �׳�buff���

		MT_TIANSHUXZ_INFO_SC=8935,									// ����Ѱ����Ϣ
		MT_TIANSHUXZ_FETCH_REWARD_CS = 8936,						// ����Ѱ����ȡ����

		MT_EQUIP_GRADE_SUIT_ACTIVE_CS = 8937,						// ��ͨװ���ռ�����

		MT_EXP_BUFF_INFO_SC = 8938,									// ����buff��Ϣ

		MT_LITTLE_HELPER_DROP_ITEM_INFO_SC = 8939,						// С���ֵ�����Ʒ��Ϣ

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CROSS_CHALLENGE_FIELD_REQ_CS = 8940,						// �������������
		MT_CROSS_CHALLENGE_FIELD_STATUS_INFO_SC = 8941,				// �������������״̬��Ϣ
		MT_CROSS_CHALLENGE_FIELD_USER_INFO_SC = 8942,				// ����������û���Ϣ
		MT_CROSS_CHALLENGE_FIELD_OPPONENT_INFO_SC = 8943,			// ���������������ϸ��Ϣ
		MT_CROSS_CHALLENGE_BE_DEFEAT_NOTICE_SC = 8944,				// ��������������֪ͨ
		MT_CROSS_CHALLENGE_FIELD_WIN = 8945,						// ���������ֱ��ʤ��
		MT_CROSS_CHALLENGE_FIELD_REPORT_INFO_SC = 8946,				// ���������ս����Ϣ
		MT_CROSS_CHALLENGE_FIELD_RANK_INFO_SC = 8947,				// ���������������Ϣ
		MT_CROSS_CHALLENGE_FIELD_OPPONENT_RANK_POS_CHANGE = 8948,	// ���ֵ����������仯

		MT_DISCOUNTSHOP_BUY_CS = 8950,								// �ػ��̳ǹ���
		MT_DISCOUNTSHOP_INFO_SC = 8951,								// �ػ��̳���Ϣ

		MT_ZHUANZHI_EQUIP_COMPOSE_REQ_CS = 8952,					// �ϳ�תְװ������

		MT_LITTLE_HELPER_INFO_SC = 8953,							// С������Ϣ

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_LUCKYCLOUDBUYINFO_SC = 8954,								// �����ƹ�������Ϣ
		MT_RA_LUCKYCLOUDBUYLIST_SC = 8955,								// �����ƹ������¼
		MT_RA_LUCKYCLOUDBUYOPENINFO_SC = 8956,							// �����ƹ������֪ͨ
		MT_ZHUANZHI_EQUIP_COMPOSE_SUCCEED_SC = 8957,					// תְװ���ϳɳɹ�
		MT_LITTLE_HELPER_REPEAT_REQ_CS = 8958,							// С�����ظ�����

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CRACY_BUY_ALL_INFO = 8959,								// �񻶴��ֹ������Ϣ
		MT_CRACY_BUY_LIMIT_INFO = 8960,								// �޹���Ϣ

		MT_CROSSEQUIP_REQ_CS = 8961,								    // ����װ���������
		MT_CROSSEQUIP_ALL_INFO_SC = 8962,							    // ����װ��������Ϣ�·�
		MT_CROSSEQUIP_ROLL_RESULT_SC = 8963,							// ����װ���齱������Ϣ
		MT_CROSS_EQUIP_BOSS_ONE_EQUIP_SC = 8964,						// ����װ������װ��
		MT_CROSS_EQUIP_BOSS_ALL_EQUIP_SC = 8965,						// ����װ������װ��
		MT_CROSS_EQUIP_BOSS_CHUANSHI_FRAG_CHANGE_SC = 8966,				// ������Ƭ�ı�
		MT_CROSS_EQUIP_DOUQI_EXP_CHANGE_SC = 8967,						// �������ָı�

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MYTH_OPERA_REQ_CS = 8968,									// ��ϵͳ��������
		MT_MYTH_CHPATER_INFO_SC = 8969,									// ��ƪ����Ϣ
		MT_MYTH_KNAPSACK_INTFO_SC = 8970,								// �񻰱�����Ϣ
		MT_MYTH_CHPATER_SINGLE_INFO_SC = 8971,							// ��ƪ�µ�����Ϣ

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ZODIAC_INFO_SC = 8972,												// ʮ����Ф��Ϣ
		MT_ZODIAC_BACKPACK_INFO_SC = 8973,										// ʮ����Ф������Ϣ
		MT_ZODIAC_REQ_CS = 8974,												// ʮ����Ф��������
		MT_ZODIAC_DECOMPOSE_REQ_CS = 8975,										// ʮ����Ф�ֽ�����
		MT_ZODIAC_BASE_INFO_SC = 8976,											// ʮ����Ф������Ϣ
		
		MT_CROSS_HUSONG_SHUIJING_REQ_CS = 8979,							// �������ˮ������
		MT_CROSS_HUSONG_SHUIJING_INFO_SC = 8980,						// �������ˮ����Ϣ
		MT_CROSS_HUSONG_SHUIJING_GATHER_INFO_SC = 8981,					// �������ˮ��ˮ����Ϣ

		MT_RA_CHONGZHI_RANK2_INFO_SC = 8982,							// ������ֵ��Ϣ

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_AUTO_PROTOCOL_RESERVE_MIN = 11000,							// �Զ�����Э��ռ��Э��Σ���ʽЭ�鲻Ҫռ��
		MT_AUTO_PROTOCOL_RESERVE_MAX = 13999,							// �Զ�����Э��ռ��Э��Σ���ʽЭ�鲻Ҫռ��
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	};
}

#endif

