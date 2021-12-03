#ifndef __PROTOCAL_H__
#define __PROTOCAL_H__

namespace Protocol
{
	enum _MsgType
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ENTER_GS_ACK_SC = 1000,										// 请求进入场景服务器回复
		MT_CHANGE_GS_SC = 1001,
		MT_SERVER_BUSY_SC = 1002,										// 服务器太忙（承载用户数达到配置上限）

		MT_ENTER_GS_REQ_CS = 1050,										// 请求进入场景服务器
		MT_LOGOUT_CS = 1051,
		MT_TRANSPORT_REQ = 1052,
		MT_HEARTBEAT_CS = 1053,

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ENTER_SCENE_SC = 1100,
		MT_LEAVE_SCENE_SC = 1101,
		MT_VISIBLE_ENTER_ROLE_SC = 1102,								// Role对象进入可视区域
		MT_VISIBLE_LEAVE_SC = 1103,										// 对象退出可视区域
		MT_OBJ_MOVE_SC = 1104,											// 对象位置移动同步
		MT_VISIBLE_ENTER_FALLING_SC = 1105,								// FallingItem对象进入可视区域
		MT_VISIBLE_ENTER_MONSTER_SC = 1106,								// Monster对象进入可视区域
		MT_VISIBLE_ENTER_PET_SC = 1107,									// Pet对象进入可视区域
		MT_ROLE_VISIBLE_CHANGE_SC = 1108,								// 角色装备改变所引起的角色显示变化
		MT_VISIBLE_ENTER_ROLE_STALL_SC = 1109,							// Role摆摊对象进入可视区域
		MT_VISIBLE_ENTER_GATHER_OBJ_SC = 1110,							// 可采集物品进入可是区域
		MT_GATHER_BEGIN_SC = 1111,										// 开始采集
		MT_GATHER_STOP_SC = 1112,										// 结束采集
		MT_GATHER_TIMER_SC = 1113,										// 开始采集计时
		MT_HOOK_STATUS_SC = 1114,										// 开始挂机修炼
		MT_RESET_POST_SC = 1115,										// 强设主角位置
		MT_OBJ_EFFECT_SHOW_SC = 1116,									// OBJ身上播放效果
		MT_SCENE_FB_DYNINFO_SC = 1117,									// 场景动态信息
		MT_TEAM_MEMBER_POS_SC = 1118,									// 同步同场景队友位置
		MT_SKILL_RESET_POS_SC = 1119,									// 技能强行设置坐标 冲锋类技能
		MT_VISIBLE_ENTER_BATTLEFIELD_SHENSHI_SC = 1120,					// 战场神石对象进入可视区域
		MT_REST_STATUS_SC = 1121,										// 开始打坐修炼
		MT_REST_INVITE_NOTIFY_SC = 1122,								// 询问是否同意双修
		MT_REST_INVITE_RESULT_SC = 1123,								// 邀请双修的结果
		MT_USER_SCENE_SKILL_STATUS_SC = 1124,							// 玩家场景技能状态
		MT_ALL_OBJ_MOVE_INFO_SC = 1125,									// 场景所有对象的运动信息返回
		MT_VISIBLE_ENTER_ROLE_SHADOW_SC = 1126,							// RoleShadow对象进入可视区域
		MT_OBJ_MOVE_MODE_SC = 1127,										// 对象运动模式通知
		MT_SCENE_MONSTER_DIE_SC = 1128,									// 场景内怪物死亡
		MT_VISIBLE_ENTER_EFFECT_OBJ_SC = 1129,							// 效果物品进入可视区域
		MT_FB_FOLLOW_NPC_INFO_SC = 1130,								// 副本跟随NPC信息
		MT_TRIGGER_NOTIFY_SC = 1131,									// 触发器触发广播
		MT_FIX_POS_SC = 1132,											// 位置纠偏通知
		MT_PERFORM_SCENE_SKILL_SC = 1133,								// 释放场景技能通知
		MT_ACTION_REJECT_SC = 1134,										// 动作拒绝通知
		MT_TEAM_CALL_TOGETHER_SC = 1135,								// 队伍召集广播
		MT_TEAM_CALL_IN_SC = 1136,										// 队伍招募广播
		MT_VISIBLE_ENTER_BIG_COIN_OBJ_SC = 1137,						// 大铜币进入可视区域
		MT_PICK_BIG_COIN_SUCC_SC = 1138,								// 捡起大铜币成功
		MT_CHANGE_SCENE_NOTIFY_SC = 1139,								// 切场景通知
		MT_VISIBLE_ENTER_MARRY_OBJ_SC = 1140,							// 结婚对象进入可视区域
		MT_VISIBLE_ENTER_TRIGGER_OBJ_SC = 1141,							// Trigger进入可视区域
		MT_VISIBLE_ENTER_MONSTER_HALO_OBJ_SC = 1142,					// 怪物光环进入可视区域
		MT_YANHUA_EXPLODE_SC = 1143,									// 释放烟花
		MT_VISIBLE_ENTER_ROLE_SHADOW_OBJ_SC = 1144,						// 角色影子进入可视区域
		MT_OPERATE_RET_SC = 1145,										// 操作结果
		MT_VISIBLE_ENTER_WORLD_EVENT_OJB_SC = 1146,						// 世界事件对象进入可视区域
		MT_REQ_TEAM_MEMBER_POS_LIST_SC = 1147,							// 请求队伍成员位置列表返回
		MT_GATHER_INFO_CHANGE_SC = 1148,								// 采集物信息变化
		MT_BOSS_DPS_FLAG_SC = 1149,										// dps标记

		MT_OBJ_MOVE_CS = 1150,											// 移动
		MT_START_GATHER_REQ_CS = 1151,									// 请求开始采集
		MT_ENTER_FB_CS = 1152,											// 进入副本
		MT_LEAVE_FB_CS = 1153,											// 离开副本
		MT_CANCEL_MONSTER_STATIC_STATE_CS = 1154,						// 取消场景怪物静止状态
		MT_REST_REQ_CS = 1155,											// 请求开始或结束打坐修炼
		MT_REST_INVITE_CS = 1156,										// 邀请双修
		MT_REST_INVITE_ACK_CS = 1157,									// 双修邀请的应答
		MT_GET_ALL_OBJ_MOVE_INFO_REQ_CS = 1158,							// 请求拉场景所有对象的运动信息
		MT_REST_START_AFFIRM_CS = 1159,									// 确认开始双修
		MT_TEAM_CALL_TOGETHER_CS = 1160,								// 召集队员
		MT_TEAM_CALL_IN_CS = 1161,										// 招募队员
		MT_PICK_BIG_COIN_CS = 1162,										// 捡起大铜币
		MT_AUTO_FB_CS = 1163,											// 扫荡副本
		MT_REQ_TEAM_MEMBER_POS_CS = 1164,								// 请求同场景队员位置信息
		MT_STOP_GATHER_REQ_CS = 1165,									// 请求停止采集
		MT_CHANGE_SCENE_LINE_REQ_CS = 1166,								// 场景切线
		MT_GATHER_BE_GATHER_SC = 1167,									// 采集物被采集信息
		MT_ROLE_TASK_APPEARN_SC = 1168,									// 接受任务形象

		MT_OBJ_FLY_HEIGHT_CHANGE_CS = 1171,								// 角色飞行高度变化请求
		MT_OBJ_FLY_HEIGHT_CHANGE_SC = 1172,								// 角色飞行高度变化广播

		MT_BOSS_FIRST_HIT_FLAG_SC = 1173,								// boss首刀标记
		MT_BOSS_FIRST_HIT_INFO_SC = 1174,								// boss第一刀信息
		MT_BOSS_DPS_TITLE_SC = 1175,									// boss归属称号信息

		MT_ROLE_RETURN_REALIVE_POSI_CS = 1176,							// 请求设置主角回复活点

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_OBJ_CHANGE_BLOOD_SC = 1300,									// 广播hp变化
		MT_PERFORM_SKILL_SC = 1301,										// 释放技能
		MT_ROLE_REALIVE_SC = 1302,										// 重生
		MT_PERFORM_AOE_SKILL_SC = 1303,									// 施放AOE技能
		MT_OBJ_CHANGE_MP_SC = 1304,										// 广播mp变化
		MT_BUFF_ADD_SC = 1305,											// 增加buff
		MT_BUFF_REMOVE_SC = 1306,										// 删除buff
		MT_EFFECT_LIST_SC = 1307,										// Effect列表
		MT_EFFECT_INFO_SC = 1308,										// Effect信息
		MT_EFFECT_REMOVE_SC = 1309,										// 删除Effect通知
		MT_ITEM_EFFECT_PACKAGE_CD = 1310,								// 气血包法力包Effect CD信息
		MT_BUFF_MARK_SC = 1311,											// buff mark信息
		MT_SKILL_TARGET_POS_SC = 1312,									// 攻击目标坐标修正
		MT_SKILL_EFFECT_SC = 1312,										// 技能特效播放
		MT_EXPLODE_SC = 1313,											// 爆炸
		MT_SKILL_PHASE_SC = 1314,										// 技能施法阶段广播
		MT_REALIVE_WAITING_SC = 1315,									// 等待复活
		MT_QTE_READY_SC = 1316,											// QTE技能等待释放
		MT_SKILL_TIP_SC = 1317,											// 技能tips
		MT_FIGHT_SPECIAL_FLOAT_SC = 1318,								// 战斗特殊飘字（如有防护罩被打时飘吸收的伤害）
		MT_WORLD_BOSS_SPECIAL_SHIELD_SC = 1319,							// 广播世界BOSS护盾变化
		MT_BIANSHEN_EFFECT_IMAGE_BROADCAST_SC = 1320,					// 广播变身effect外形
		MT_UPGRADE_SKILL_INFO_SC = 1321,								// 进阶技能信息
		MT_CONTINUE_KILL_INFO_SC = 1322,								// 连斩信息
		MT_INVISIBLE_VIEW_SC = 1323,									// 广播隐身信息

		MT_PERFORM_SKILL_CS = 1350,										// 施放技能
		MT_REALIVE_REQ_CS = 1351,										// 请求角色重生
		MT_GET_EFFECT_LIST_REQ_CS = 1352,								// 请求拉取Effect列表
		MT_GET_EFFECT_INFO_REQ_CS = 1353,								// 拉取Effect信息
		MT_PERFORM_QTE_SKILL_CS = 1354,									// 释放QTE技能
		MT_SKILL_CD_CS = 1355,											// 开始技能CD

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ROLE_INFO_ACK_SC = 1400,										// 请求角色基本信息返回
		MT_SKILL_LIST_INFO_ACK_SC = 1401,								// 请求角色技能信息返回
		MT_ROLE_ATTR_VALUE_SC = 1402,									// 角色属性数值通知
		MT_CHA_EXP_CHANGE_SC = 1403,									// 经验变更通知
		MT_CHA_LEVEL_UPGRADE_SC = 1404,									// 经验升级广播
		MT_ROLE_CHANGE_PROFESSIONAL_SC = 1405,							// 转职广播
		MT_ROLE_ADD_COUNTRY_SC = 1406,									// 加入国家广播
		MT_VIP_INFO_SC = 1407,											// 角色Vip信息
		MT_ROLE_NAME_COLOR_CHANGE_SC = 1408,							// 角色名字颜色改变
		MT_FIND_ROLE_BY_NAME_RET_SC = 1409,								// 按玩家名称查找玩家返回
		MT_CHANGE_EVIL_SC = 1410,										// 善恶值改变
		MT_ROLE_SET_COUNTRY_POST_SC = 1411,								// 角色国家职位信息
		MT_GET_ROLE_INFO_ACK_SC = 1412,									// 查看个人信息返回
		MT_SKILL_INFO_ACK_SC = 1413,									// 单个技能信息返回
		MT_ROLE_CAPABILITY_SC = 1414,									// 角色战斗力
		MT_ROLE_SET_ATTACK_MODE_SC = 1415,								// 设置攻击模式返回
		MT_ROLE_GUILD_INFO_CHANGE = 1416,								// 玩家军团信息变更广播
		MT_ROLE_AUTHORITY_TYPE_SC = 1417,								// 角色管理权限变更广播
		MT_ROLE_TEAM_LEADER_INFO_SC = 1418,								// 角色组队队长标志变更广播
		MT_ROLE_RESET_NAME_SC = 1419,									// 角色改名
		MT_ROLE_DAY_COUNT_SC = 1420,									// 角色计数信息
		MT_INVEST_COUNT_INFO_SC = 1421,									// 投资计划天数
		MT_ROLE_CHANGE_SEX_SC = 1422,									// 角色性别改变
		MT_ROLE_CHANGE_XIANHUN_SC = 1423,								// 角色仙魂改变
		MT_ROLE_CHANGE_YUANLI_SC = 1424,								// 角色元力改变
		MT_CHANGE_SPECIAL_APPEARANCE_SC = 1425,							// 特殊外观改变
		MT_ROLE_CHANGE_NUQI_SC = 1426,									// 角色怒气改变
		MT_ROLE_CHANGE_CAMP_HONOUR_SC = 1427,							// 角色阵营荣誉改变
		MT_NEWHAND_GUILD_ENDTIME_SC = 1428,								// 新手指导员结束时间
		MT_MOUNT_BIANSHEN_INFO_SC = 1429,								// 坐骑变身信息
		MT_ROLE_MARRY_INFO_CHANGE_SC = 1430,							// 角色结婚信息改变
		MT_ROLE_ENERGY_SC = 1431,										// 体力改变
		MT_ROLE_SHENGWANG_SC = 1432,									// 声望改变
		MT_ROLE_SKILL_OTHER_SKILL_INFO = 1433,							// 其他技能信息
		MT_ROLE_CHANGE_NV_WA_SHI_SC = 1434,								// 女娲石改变
		MT_ROLE_EXP_EXTRA_PER_SC = 1435,								// 经验加成
		MT_ROLE_CHANGE_HUNLI_SC = 1436,									// 魂力改变
		MT_ROLE_CHANGE_LINGJING_SC = 1437,								// 灵精改变
		MT_ROLE_CHANGE_CHENGJIU_SC = 1438,								// 成就改变
		MT_ROLE_CHANGE_DAY_REVIVAL_TIMES_SC = 1439,						// 每次复活次数改变
		MT_ROLE_CHANGE_DAY_ZHUAGUI_INFO_SC = 1440,						// 抓鬼活力和次数
		MT_ROLE_JINGJIE_INFO_SC = 1442,									// 境界信息

		MT_ROLE_INFO_REQ_CS = 1450,										// 请求角色基本信息
		MT_SKILL_INFO_REQ_CS = 1451,									// 请求角色技能信息
		MT_ROLE_SKILL_LEARN_REQ_CS = 1452,								// 请求技能学习
		MT_ROLE_SET_ATTACK_MODE_CS = 1453,								// 设置攻击模式F
		MT_ALL_INFO_REQ_CS = 1454,										// 用于登陆时请求一系列消息(原各模块的单独请求消息保留)
		MT_QUERY_ROLE_INFO_CS = 1455,									// 查询角色信息
		MT_QUERY_ROLE_INFO_BY_NAME_CS = 1456,							// 通过角色名查询角色信息
		MT_CHOOSE_CAMP_CS = 1457,										// 选择阵营
		MT_CHOOSE_PROF_CS = 1458,										// 选择门派
		MT_GET_ROLE_CAPABILITY_CS = 1459,								// 请求角色战斗力
		MT_SET_MOVE_MODE_CS = 1460,										// 请求设置运动模式
		MT_ROLE_RESET_NAME_CS = 1461,									// 角色改名请求
		MT_ROLE_SET_ZOMBIE_REQ_CS = 1462,								// 设置僵尸标记请求
		MT_MOUNT_BIANSHEN_OPERA_CS = 1463,								// 坐骑变身操作
		MT_SPEEDUP_HELLO_CS = 1464,										// 加速检测心跳
		MT_SET_AVATAR_TIMESTAMP_CS = 1465,								// 头像修改时间设置请求
		MT_CROSS_QUERY_ROLE_INFO_CS = 1466,								// 查询跨服角色信息
		MT_CROSS_QUERY_EXP_EXTRA_CS = 1467,								// 请求经验加成
		MT_ROLE_GONGXUN_SC = 1468,										// 请求功勋值
		MT_SEND_AVATAR_TIMESTAMP_INFO_CS = 1469,						// 发送第一次改变头像信息
		MT_CROSS_FIND_ROLE_BY_UUID = 1470,								// 跨服查找角色


		MT_ROLE_JINGJIE_REQ_CS = 1472,									// 境界请求

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_KNAPSACK_INFO_ACK_SC = 1500,									// 请求背包信息返回
		MT_KNAPSACK_GRID_CHANGE_SC = 1501,								// 背包单个格子变化
		MT_KNAPSACK_PARAM_SC = 1502,									// 背包参数列表
		MT_KNAPSACK_GRID_CHANGE_PARAM_SC = 1503,						// 有参数的格子变化
		MT_USE_ITEM_SUC_SC = 1504,										// 使用物品成功
		MT_KNAPSACK_MAX_GRID_NUM_SC = 1505,								// 背包格子扩充结果
		MT_STORAGE_MAX_GRID_NUM_SC = 1506,								// 仓库格子扩充结果
		MT_ITEM_MULTI_INVALID_SC = 1507,								// 物品批量失效
		MT_TASK_KNAPSACK_INFO_SC = 1508,								// 任务背包信息
		MT_ITEM_COLDDOWN_INFO_SC = 1509,								// 物品使用CD信息
 		MT_TEAM_MEMBER_PICK_ITEM_SC = 1510,								// 队伍成员拾取掉落物品
		MT_RESERVE_1511_SC = 1511,										// 保留
		MT_LACK_ITEM_SC = 1512,											// 缺少物品信息
		MT_KNAPSACK_GRID_EXTEND_AUTO_SC = 1513,							// 背包格子自动开启结果
		MT_KNAPSACK_GRID_EXTEND_AUTO_CS = 1514,							// 请求背包格子自动开启结果

		MT_KNAPSACK_INFO_REQ_CS = 1550,									// 请求背包信息
		MT_USE_ITEM_CS = 1551,											// 使用物品
		MT_MOVE_ITEM_CS = 1552,											// 移动背包单元
		MT_DICARD_ITEM_CS = 1553,										// 丢弃物品
		MT_KNAPSACK_STORAGE_PUTINORDER_CS = 1554,						// 整理背包
		MT_KNAPSACKSTORAGE_EXTEND_GRID_NUM_CS = 1555,					// 背包、仓库扩展
		MT_SPLIT_ITEM_CS = 1556,										// 拆分背包物品
		MT_ITEM_COMPOSE_CS = 1557,										// 物品合成
		MT_BLUEPRINT_COMPOSE_CS = 1558,									// 图纸合成
		MT_PICK_ITEM_CS = 1559,											// 捡起掉落物品
		MT_TAKE_OUT_ALL_ITEM = 1560,									// 取出仓库所有物品
		MT_GEMSTONE_BATCH_COMPOSE_CS = 1561,							// 宝石批量合成
		MT_PICK_ITEM_ACT_SC = 1562,										// 捡起掉落物品返回
		MT_REWARD_ITEM_LIST_ACT_SC = 1563,								// 奖励物品列返回
		MT_USE_ITEM_MAX_NUM_CS = 1564,									// 使用物品(根据item_id搜索优先绑定)
		MT_BATCH_DICARD_ITEM_CS = 1565,									// 批量丢弃物品


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_EQUIPMENT_INFO_ACK_SC = 1700,								// 获取装备信息返回
		MT_EQUIPMENT_ITEM_CHANGE_SC = 1701,								// 装备信息改变
		MT_GET_ONE_EQUIPMENT_INFO_ACK_SC = 1702,						// 请求单个装备信息返回
		MT_STONE_INFO_SC = 1703,										// 下发宝石信息
		MT_EQUIP_SUIT_INFO_SC = 1704,									// 装备套装信息
		MT_DUANZAO_SUIT_INFO_SC = 1705,									// 锻造套装等级信息
		MT_OTHER_USER_ONLINE_STATUS_SC = 1706,							// 其他角色在线状态
		MT_ROLE_CHANGE_CAMP_SC = 1711,									// 角色转阵营
		MT_CHANGE_PROF_RESULT_SC = 1712,								// 角色转职
		MT_SHENBING_INFO_SC = 1713,										// 神兵信息
		MT_SHENBING_APPE_CHG_SC = 1714,									// 神兵外观变化
		MT_EXPBOTTLE_INFO_SC = 1715,									// 经验瓶信息
		MT_ROLE_SPACE_MSG_INFO_SC = 1716,								// 角色空间信息
		MT_ROLE_SPACE_SELF_INFO_SC = 1717,								// 角色自己空间信息
		MT_ROLE_LIFE_SKILL_INFO_SC = 1718,								// 生活技能信息
		MT_WASH_INFO_SC = 1719,											// 下发洗炼信息
		MT_MOVE_CHESS_INFO_SC = 1720,									// 走棋子信息

		MT_MILLIONAIRE_INFO_CS = 1721,									// 请求大富豪信息
		MT_MILLIONAIRE_INFO_SC = 1722,									// 大富豪信息
		MT_MOVE_CHESS_STEP_REWARD_INFO_SC = 1723,						// 走棋子格子奖励信息

		MT_EQUIPMENT_INFO_REQ_CS = 1750,								// 请求装备信息
		MT_TAKE_OFF_EQUIPMENT_CS = 1751,								// 脱下装备
		MT_EQUIP_STRENGTHEN_REQ_CS = 1752,								// 请求武器强化
		MT_EQUIP_UPQUALITY_REQ_CS = 1753,								// 请求装备提升品质
		MT_EQUIP_UPLEVEL_REQ_CS = 1754,									// 请求装备升级
		MT_EQUIP_SHENZHU_REQ_CS = 1755,									// 请求装备神铸
		MT_STONE_UPLVL_CS = 1756,										// 请求宝石升级
		MT_DUANZAO_SUIT_REQ_CS = 1757,									// 锻造套装请求
		MT_SHENBING_REQ_CS = 1761,										// 请求神兵信息
		MT_SHENBING_COMPOSE_CS = 1762,									// 请求合成神兵
		MT_SHENBING_OP_CS = 1763,										// 请求神兵操作
		MT_SHENBING_UP_LEVEL_CS = 1764,									// 神兵升星级请求象
		MT_SHENBING_ALL_INFO_SC = 1766,									// 神兵所有信息
		MT_ROLE_CHANGE_CAMP_CS = 1769,									// 转阵营请求
		MT_CHANGE_PROF_REQ_CS = 1770,									// 角色转职
		MT_EQUIP_FULING_CS = 1771,										// 装备附灵
		MT_EQUIP_CROSS_EQUIP_OPERA_CS = 1772,							// 跨服装备操作
		MT_EQUIP_UP_STAR_CS = 1774,										// 装备升星
		MT_EQUIP_INHERIT_REQ_CS = 1775,									// 请求装备传承
		MT_ROLE_ZHUAN_SHENG = 1776,										// 角色转生
		MT_ROLE_NOREXITEM_USE_TIMES = 1777,								// 主动物品当日使用次数
		MT_TRANSFORM_EXP_REQ_CS = 1778,									// 凝聚经验
		MT_GET_ROLE_SPACE_MSG_INFO_CS = 1779,							// 获取角色空间消息（留言，回复，脚印，送花记录）
		MT_OPER_SPACE_CS = 1780,										// 操作角色空间请求（）
		MT_REMOVE_RECORD_CS = 1781,										// 删除记录请求
		MT_ROLE_LIFESKILL_OPERA_CS = 1782,								// 生活技能请求操作
		MT_WASH_CS = 1783,												// 请求洗炼
		MT_STONE_INLAY_CS = 1784,										// 请求宝石镶嵌
		MT_EQUIPMENT_GRID_INFO_SC = 1785,								// 装备格子属性
		MT_EQUIPMENT_WUQI_COLOR_SC = 1786,								// 全身总强化等级广播
		MT_STONE_INFO_REQ_CS = 1787,									// 请求宝石信息
		MT_EQUIP_COMPOUND_CS = 1788,									// 请求装备合成
		MT_EQUIP_UP_ETERNITY_CS = 1789,									// 请求装备提升永恒等级
		MT_FRIEND_EXP_BOTTLE_OPERATE_REQ_CS = 1790,						// 好友经验瓶获取经验
		MT_FRIEND_EXP_BOTTLE_ADD_EXP = 1791,							// 好友经验瓶经验增加
		MT_EQUIP_USE_ETERNITY_LEVEL_CS = 1792,							// 使用永恒等级
		MT_SKIP_REQ_CS = 1793,											// 一键跳过
		MT_EQUIP_EXTEND_INFO_SC = 1794,									// 装备扩展信息
		MT_FEIXIAN_OP_CS = 1795,										// 飞仙操作
		MT_FEIXIAN_EQUIP_INFO_SC = 1796,								// 飞仙装备信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_NPC_MARK_LIST_SC = 1800,										// npc 列表
		MT_TASK_LIST_ACK_SC = 1801,										// 请求任务列表返回
		MT_TASK_INFO_SC = 1802,											// 单条已接任务信息
		MT_TASK_RECORDER_LIST_SC = 1803,								// 发送任务记录列表
		MT_TASK_RECORDER_INFO_SC = 1804,								// 单条任务记录信息
		MT_TUMO_TASK_INFO_SC = 1805,									// 屠魔任务信息
		MT_HUSONG_TASK_INFO_SC = 1806,									// 护送任务信息
		MT_GUILD_TASK_INFO_SC = 1807,									// 仙盟任务信息
		MT_TASK_ACCEPTABLE_TASK_LIST_SC = 1808,							// 可接任务列表
		MT_CISHA_TASK_INFO_SC = 1809,									// 刺杀任务信息
		MT_PAOHUAN_TASK_INFO_SC = 1810,									// 跑环任务信息
		MT_PAOHUAN_TASK_ROLL_POOL_SC = 1811,							// 跑环阶段奖池
		MT_PAOHUAN_TASK_ROLL_INFO_SC = 1812,							// 跑环阶段翻牌信息
		MT_COMMON_INFO_SC = 1814,										// 同步普通信息
		MT_JINGHUAHUSONG_VIEW_CHANGE_SC = 1815,							// 广播精华护送状态改变
		MT_SEEKROLE_WHERE_INFO_SC = 1817,								// 查询玩家在哪返回
		MT_HUANGCHENGHUIZHAN_INFO_SC = 1818,							// 皇城会战
		MT_CAMPEQUIP_INFO_SC = 1819,									// 师门装备
		MT_CAMPDEFEND_INFO_SC = 1820,									// 雕象守卫信息
		MT_HUSONG_TASK_COMSUEM_INFO_SC = 1821,							// 护送物品元宝消耗信息
		MT_TASK_RANDOM_REWARD = 1822,									// 任务奖励池
		MT_JINJIESYS_REWARD_OPERA_REQ_CS = 1824,						// 进阶系统操作请求
		MT_JINJIESYS_REWARD_INFO_SC = 1825,								// 进阶系统奖励信息
		MT_JINJIESYS_REWARD_TIMESTAMP_SC = 1826,						// 进阶系统奖励时间

		MT_TASK_LIST_REQ_CS = 1850,										// 请求任务列表
		MT_TASK_GIVEUP_CS = 1851,										// 客户端放弃任务
		MT_FLY_BY_SHOE_CS = 1852,										// 使用飞天神靴
		MT_TASK_ACCEPT_CS = 1853,										// 接受任务
		MT_TASK_COMMIT_CS = 1854,										// 提交任务
		MT_TASK_TALK_TO_NPC = 1855,										// 和npc对话
		MT_TUMO_TASK_OPERA_CS = 1856,									// 屠魔任务请求操作
		MT_HUSONG_TASK_REFRESH_CS = 1857,								// 刷新护送任务
		MT_RESERVE_1858 = 1858,											// 领取新手护送任务
		MT_HUSONG_BUY_TIMES_CS = 1859,									// 保留
		MT_RESERVE_1860 = 1860,											// 保留
		MT_GUILD_TASK_FINISH_ALL_CS = 1861,								// 仙盟任务 一键完成
		MT_GUILD_TASK_DOUBLE_REWARD_CS = 1862,							// 仙盟双倍奖励
		MT_RESERVE_1863 = 1863,											// 保留
		MT_CISHA_TASK_OPERA_CS = 1864,									// 刺杀任务操作请求
		MT_RESERVE_1865 = 1865,											// 保留
		MT_PAOHUAN_TASK_ROLL_REQ_CS = 1866,								// 跑环阶段请求翻牌
		MT_PAOHUAN_TASK_INFO_REQ_CS = 1867,								// 跑环任务信息请求
		MT_PAOHUAN_TASK_COMMIT_REQ_CS = 1868,							// 跑环任务跳过任务请求
		MT_COMMON_OPERATE_CS = 1869,									// 普通操作请求
		MT_CS_SHENZHUANG_OPER_CS = 1870,								// 神装操作请求
		MT_CS_SEEK_ROLE_WHERE_CS = 1871,								// 查看角色在哪个地方
		MT_CS_CAMPEQUIP_OPERATE_CS = 1872,								// 师门装备操作请求
		MT_CS_CAMP_NORMALDUOBAO_OPERATE_CS = 1873,						// 师门普通夺宝操作请求
		MT_HUSONG_ADD_SHIIELD_CS = 1874,								// 护送，加护盾
		MT_FLY_SYNC_JUMP_CS = 1875,										// 飞鞋，仅用于跳跃同步
		MT_FAMOUS_MAN_OPERA_REQ_CS = 1876,								// 名人堂操作请求
		MT_FAMOUS_MAN_INFO_SC = 1877,									// 名人堂信息返回
		MT_FORCE_FINISH_SPECIAL_TASK_SC = 1878,							// 指定完成某个特殊任务
		MT_SHENZHUANG_INFO_CS = 1879,									// 神装信息
		MT_CSA_EQUIP_INFO_SC = 1880,									// 合服装备信息
		MT_CSA_EQUIP_OPERA_CS = 1881,									// 合服装备操作请求
		MT_CSA_EQUIP_ACTIVE_SC = 1882,									// 合服装备激活特权

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_HOTKEY_INFO_ACK_SC = 1900,									// 返回快捷键信息列表

		MT_HOTKEY_INFO_REQ_CS = 1950,									// 请求快捷键列表
		MT_CHANGE_HOTKEY_CS = 1951,										// 改变快捷键

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MONEY_SC = 2000,												// 钱
		MT_ACCOUNT_GOLD_ACK_SC = 2001,									// 账号金币数量
		MT_ACCOUNT_GET_GOLD_ACK_SC = 2002,								// 从账号中提取金币放回

		MT_ACCOUNT_GOLD_REQ_CS = 2050,									// 获取账号金币数量
		MT_ACCOUNT_GET_GOLD_REQ_CS = 2051,								// 从账号中提取金币

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FRIEND_LIST_ACK_SC = 2100,									// 返回好友列表
		MT_ADD_FRIEND_ROUTE_SC = 2101,									// 服务器询问另外一个对象
		MT_CHANGE_FRIEND_SC = 2102,										// 服务器通知客户端好友改变
		MT_ADD_FRIEND_RET_SC = 2103,									// 拒绝加好友回复
		MT_ENEMY_LIST_ACK_SC = 2104,									// 返回仇人列表
		MT_ENEMY_CHANGE_SC = 2105,										// 服务器通知客户端仇人改变
		MT_GIVE_FLOWER_KISS_ACK_SC = 2106,								// 送花回吻返回
		MT_GIVE_FLOWER_SC = 2107,										// 送花通知
		MT_ALL_CHARM_CHANGE_SC = 2108,									// 魅力值改变
		MT_FRIEND_BLESS_ALL_INFO_SC = 2109,								// 好友祝福信息
		MT_FRIEND_BLESS_CHANGE_INFO_SC = 2110,							// 好友祝福变更信息
		MT_FRIEND_BLESS_INVITE_BLESS_SC = 2111,							// 邀请好友祝福我
		MT_SONG_HUA_INFO = 2112,										// 送花信息
		MT_SONG_HUA_INFO_REQ = 2113,									// 送花信息请求
		MT_OFFLINE_FRIEND_AUTO_DEC_FALG = 2114,							// 是否自动删除离线x天好友的	

		MT_FRIEND_LIST_REQ_CS = 2150,									// 请求获取好友列表
		MT_ADD_FRIEND_REQ_CS = 2151,									// 请求加为好友
		MT_ADD_FRIEND_RET_CS = 2152,                    				// 是否接受加好友
		MT_FRIEND_DELETE_CS = 2153,										// 删除好友
		MT_ENEMY_DELETE_CS = 2154,										// 请求删除仇人
		MT_GIVE_FLOWER_CS = 2155,										// 送花
		MT_GIVE_FLOWER_KISS_REQ_CS = 2156,								// 送花回吻请求
		MT_FRIEND_BLESS_OPERA_REQ_CS = 2157,							// 好友祝福操作请求
		MT_BLACKLISTS_SC = 2158,										// 返回黑名单
		MT_ADD_BLACK_CS = 2159,											// 添加到黑名单
		MT_DELETE_BLACK_CS = 2160,										// 从黑名单中删除
		MT_CHANGE_BLACKLISTS_SC = 2161,									// 服务器通知客户端黑名单改变

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_XIANNV_INFO_SC = 2200,										// 单个仙女信息
		MT_XIANNV_ALL_INFO_SC = 2201,									// 所有仙女信息
		MT_XIANNV_VIEW_CHANGE_SC = 2203,								// 仙女外观信息广播
		MT_RA_SERVER_PANIC_BUY_INFO_SC = 2204,							// 全民疯抢活动信息
		MT_RA_PERSONAL_PANIC_BUY_INFO_SC = 2205,						// 个人疯抢活动信息
		MT_RA_CHONGZHI_RANK_INFO_SC = 2206,								// 随机活动充值信息
		MT_RA_CONSUME_RANK_INFO_SC = 2207,								// 随机活动消费信息
		MT_RA_CONSUME_GOLD_FANLI_INFO_SC = 2208,						// 随机活动返利信息
		MT_RA_DAY_CHONGZHI_FANLI_INFO_SC = 2209,						// 充值返利活动信息
		MT_RA_DAY_CONSUME_GOLD_INFO_SC = 2210,							// 每日消费活动信息
		MT_RA_TOTAL_CONSUME_GOLD_INFO_SC = 2211,						// 累计消费活动信息
		MT_RA_DAY_ACTIVE_DEGREE_INFO_SC = 2212,							// 每日活跃活动信息
		MT_RA_KILL_BOSS_INFO_SC = 2213,									// 击杀boss活动信息
		MT_RA_CHESTSHOP_INFO_SC = 2214,									// 奇珍异宝活动信息
		MT_RA_STONE_UPLEVEL_INFO_SC = 2215,								// 宝石升级活动信息
		MT_RA_XN_CHANMIAN_UPLEVEL_INFO_SC = 2216,						// 仙女缠绵升级活动信息
		MT_RA_MOUNT_UPGRADE_INFO_SC = 2217,								// 坐骑进阶活动信息
		MT_RA_QIBING_UPGRADE_INFO_SC = 2218,							// 骑兵进阶活动信息
		MT_RA_MENTALITY_TOTAL_LEVEL_INFO_SC = 2219,						// 全身根骨等级活动信息
		MT_RA_WING_UPGRADE_INFO_SC = 2220,								// 羽翼进阶活动信息
		MT_RA_QUANMIN_QIFU_INFO_SC = 2221,								// 祈福活动信息
		MT_RA_SHOUYOU_YUXIANGNG_INFO_SC = 2222,							// 手有余香活动信息
		MT_RA_LOGIN_GIFT_INFO_SC = 2223,								// 登录送礼信息
		MT_RAND_ACTIVITY_XIANMENG_BIPIN_SC = 2224,						// 随机活动仙盟比拼
		MT_RAND_ACTIVITY_XIANMENG_JUEQI_SC = 2225,						// 随机活动仙盟崛起
		MT_ACTIVITY_MONSTER_INVADE_TIME_NOTICE_SC = 2226,				// 怪物入侵活动信息
		MT_YIZHANDAODI_KICKOUT_SC = 2227,								// 被踢出一站到底
		MT_YIZHANDAODI_LUCKYINFO_SC = 2228,								// 幸运信息
		MT_YIZHANDAODI_TITLE_CHANGE_SC = 2229,							// 连斩信息
		MT_YIZHANDAODI_USER_INFO_SC = 2230,								// 用户信息
		MT_YIZHANDAODI_RANKINIO_SC = 2231,								// 排行榜信息
		MT_WUXINGGUAJI_INFO_SC = 2232,									// 五行活动的信息
		MT_WUXINGGUAJI_LUCKY_INFO_SC = 2233,							// 五行活动的幸运信息
		MT_GUAJI_BOSS_BORN_NOTICE_SC = 2234,							// 挂机boss出生通知
		MT_RA_BIPIN_CAPABILITY_INFO_SC = 2235,							// 随机活动比拼战力信息
		MT_RA_CHARGE_REPAYMENT_INFO_SC = 2236,							// 随机活动充值回馈信息
		MT_CSA_SUB_ACTIVITY_STATE_SC = 2237,							// 合服活动子活动状态
		MT_CSA_ACTIVITY_INFO_SC = 2238,									// 合服活动信息
		MT_CSA_ROLE_INFO_SC = 2239,										// 合服活动角色信息
		MT_CSA_ROLL_RESULT_SC = 2240,									// 摇奖结果
		MT_RA_DANBICHONGZHI_INFO_SC = 2241,								// 随机活动单笔充值信息
		MT_RA_TOTAL_CHARGE_DAY_INFO_SC = 2242,							// 随机活动累计充值（天数）信息
		MT_RA_TOMORROW_REWARD_INFO_SC = 2243,							// 随机活动次日福利信息
		MT_RA_DAY_CHONGZHI_RANK_INFO_SC = 2244,							// 随机活动每日充值排行信息
		MT_RA_DAY_CONSUME_RANK_INFO_SC = 2245,							// 随机活动每日消费排行信息
		MT_RA_TOTAL_CHARGE_INFO_SC = 2246,								// 随机活动累计充值信息
		MT_RA_TIME_LIMIT_EXCHANGE_EQUI_INFO_SC = 2247,					// 随机活动装备兑换信息
		MT_RA_TIME_LIMIT_EXCHANGE_JL_INFO_SC = 2248,					// 随机活动精灵兑换信息
		MT_ACTIVIT_MILLIONAIRE_TIME_NOTICE_SC = 2249,					// 大富豪活动信息

		MT_XIANNV_CALL_CS = 2250,										// 召唤仙女
		MT_XIANNV_RENAME_CS = 2251,										// 仙女改名
		MT_XIANNV_ACTIVE_REQ_CS = 2252,									// 激活
		MT_XIANNV_ADD_ZIZHI_REQ_CS = 2253,								// 加资质
		MT_RAND_ACTIVITY_OPERA_REQ_CS = 2257,							// 随机活动请求操作
		MT_YIZHANDAODI_GUWU_CS = 2258,									// 一站到底鼓舞
		MT_WUXINGGUAJI_GET_REWARD_CS = 2259,							// 五行挂机获取奖励
		MT_XIANNV_HALO_UPGRTADE_CS = 2260,								// 仙女，升级光环
		MT_XIANNV_HALO_USE_IMAGE_CS = 2261,								// 仙女，使用光环形象
		MT_RA_NEW_TOTAL_CHARGE_INFO_SC = 2262,							// 随机活动新累计充值信息
		MT_CSA_ROLE_OPERA_REQ_CS = 2263,								// 合服活动角色操作请求
		MT_XIANNV_UPGRADE_JIE_CS = 2264,								// 仙女升阶
		MT_XIANNV_ACTIVE_HUANHUA_REQ_CS = 2265,							// 仙女激活幻化
		MT_XIANNV_CHANGEIMAGE_REQ_CS = 2266,							// 仙女改变形象
		MT_XIANNV_HUANGUA_UPLEVEL_REQ_CS = 2267,						// 仙女幻化升级请求
		MT_XIANNV_CHANGE_SKILL_REQ_CS = 2268,							// 请求改变仙女技能
		MT_ACTIVIT_MILLIONAIRE_USE_FROZEN_SKILL = 2269,					// 大富豪使用冰冻技能
		MT_XIANNV_SHENGWU_REQ_CS = 2270,								// 女神圣物请求
		MT_XIANNV_SHENGWU_CHANGE_INFO_SC = 2271,						// 女神圣物信息变更增量通知
		MT_XIANNV_SHENGWU_MILING_LIST_SC = 2272,						// 女神圣物觅灵列表
		MT_XIANNV_SHENGWU_CHOU_EXP_LIST_SC = 2273,						// 女神圣物抽经验列表
		MT_XIANNV_SHENGWU_CHOU_EXP_RESULT_SC = 2274,					// 女神圣物抽经验结果（用于客户端播放特效）
		MT_YIZHANDAODI_LAST_FIRST_USER_INFO_SC = 2275,					// 上一届一战到底第一名信息
		MT_CSA_BOSS_INFO_SC = 2276,										// 合服boss信息
		MT_CSA_BOSS_RANK_SC = 2277,										// 合服boss排行榜
		MT_CSA_BOSS_ROLE_INFO_SC = 2278,								// 合服boss击杀数
		MT_RA_LOTTERY_1_INFO_SC = 2279,									// 随机活动-周末狂欢信息
		MT_RA_ZERO_BUY_RETURN_SC = 2280,								// 随机活动-零元购信息
		MT_RA_KUANG_HAI_QING_DIAN_INFO_SC = 2281,						// 狂嗨庆典信息
	
		MT_RA_GIFT_HARVEST_PLAYER_INFO_SC = 2282,						// 礼物收割的玩家信息
		MT_RA_GIFT_HARVEST_RANK_INFO_SC = 2283,							// 礼物收割的排行榜
		MT_RA_GIFT_HARVEST_SKILL_INFO_SC = 2284,						// 礼物收割的技能信息
		MT_RA_GIFT_HARVEST_ROUND_INFO_SC = 2285,						// 礼物收割的轮次状态
		MT_RA_TOTAL_CHARGE_MULTI_INFO_SC = 2286,						// 随机活动新累计充值信息
		MT_RA_DOUBLE_GET_INFO_SC = 2287,								// 双倍元宝活动信息
		MT_RA_SINGLE_CHARGE_INFO_MULTI_SC = 2288,						// 单笔充值活动信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_REQ_TRADE_ROUTE_SC = 2300,									// 转发交易请求给对方
		MT_TRADE_PUT_GOLD_SC = 2301,									// 将元宝放上交易架
		MT_TRADE_PUT_ITEM_SC = 2302,									// 将物品放上交易架
		MT_TRADE_PUT_ITEM_PARAM_SC = 2303,								// 将物品放上交易架 (带参数物品[装备])
		MT_TRADE_STATE_SC = 2304,										// 交易状态返回

		MT_REQ_TRADE_CS = 2350,											// 请求与某人交易
		MT_TRADE_RET_CS = 2351,											// 交易请求返回
		MT_TRADE_LOCK_REQ_CS = 2352,									// 请求交易锁定
		MT_TRADE_AFFIRM_REQ_CS = 2353,									// 请求交易确认
		MT_TRADE_CANCLE_CS = 2354,										// 取消交易
		MT_TRADE_PUT_GOLD_REQ_CS = 2355,								// 请求将元宝放上交易架
		MT_TRADE_PUT_ITEM_REQ_CS = 2356,								// 请求将物品放上交易架

		MT_TUHAOJIN_USE_REQ_CS = 2399,									// 使用土豪金请求

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_LEVEL_LOTTERY_INFO_SC = 2400,								// 随机活动金银塔抽奖信息
		MT_RA_LEVEL_LOTTERY_REWARD_LIST_SC = 2401,						// 随机活动金银塔奖励信息
		MT_RA_CHARGE_NIU_EGG_INFO_SC = 2402,							// 随机活动充值扭蛋信息
		MT_RA_CHARGE_NIU_EGG_REWARD_SC = 2403,							// 随机活动充值扭蛋抽奖结果信息
		MT_RA_ZHENBAOGE_INFO_SC = 2404,									// 随机活动-珍宝阁信息
		MT_TUHAOJIN_ALL_INFO_SC = 2405,									// 土豪金信息
		MT_BIG_CHATFACE_ALL_INFO_SC = 2406,								// 大表情信息
		MT_SHENZHOU_WEAPON_ALL_INFO_SC = 2407,							// 神州六器所有信息
		MT_SHENZHOU_WEAPON_GATHER_INFO_SC = 2408,						// 神州六器采集信息
		MT_RA_MIJINGXUNBAO_INFO_SC = 2409,								// 秘境寻宝信息
		MT_RA_MIJINGXUNBAO_TAO_REWARD_INFO_SC = 2410,					// 秘境寻宝结果信息
		MT_RA_MONEY_TREE_INFO_SC = 2411,								// 摇钱树信息
		MT_RA_MONEY_TREE_CHOU_REWARD_INFO_SC = 2412,					// 摇钱树奖励信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_BABY_INFO_SC = 2413,											// 宝宝基础信息
		MT_BABY_ROUTE_SC = 2414,										// 宝宝请求信息转发
		MT_BABY_ALL_INFO_SC = 2415,										// 所有宝宝基础信息
		MT_BABY_SPIRIT_INFO_SC = 2416,									// 宝宝精灵信息

		MT_BABY_OPERA_REQ_CS = 2417,									// 宝宝操作请求
		MT_BABY_UPGRADE_REQ_CS = 2418,									// 宝宝升阶请求
		MT_BABY_RENAME_REQ_CS = 2419,									// 宝宝改名请求
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_RA_KING_DRAW_INFO_SC = 2420,									// 随机活动-陛下请翻牌活动信息
		MT_RA_KING_DRAW_PLAY_TIMES_REWARD_SC = 2421,					// 随机活动-陛下请翻牌活动，玩多次的奖励信息
		MT_RA_TOTAL_CHARGE_3_INFO_SC = 2422,							// 随机活动-累计充值3
		MT_RA_TOTAL_CHARGE_3_INFO_CS = 2423,							// 随机活动-累计充值3 请求
		MT_RA_TOTAL_CHARGE_4_INFO_CS = 2424,							// 随机活动-累计充值4 请求信息
		MT_RA_TOTAL_CHARGE_4_INFO_SC = 2425,							// 累计充值4信息
		MT_RA_MINE_ALL_INFO_SC = 2426,									// 趣味挖矿信息
		MT_OTHERCAPABILITY_INFO_SC = 2427,								// 三件套激活信息
		MT_RA_GUAGUA_INFO_SC = 2428,									// 顶刮刮信息
		MT_RA_GUAGUA_PLAY_TIMES_REWARD_SC = 2429,						// 随机活动-顶刮刮活动，玩多次的奖励信息
		MT_RA_TIAN_MING_DIVINATION_INFO_SC = 2430,						// 随机活动-天命卜卦活动信息
		MT_RA_TIANMING_DIVINATION_START_CHOU_RESULT_SC = 2431,  		// 随机活动-天命卜卦卜卦结果
		MT_RA_FANFAN_ALL_INFO_SC = 2432,								// 随机活动-翻翻转 所有信息
		MT_RA_FANFAN_WORD_EXCHANGE_RESULT_SC = 2433,					// 随机活动-翻翻转 兑换结果
		MT_RA_CONTINUE_CHONGZHI_INFO_SC = 2434,							// 随机活动-连续充值
		MT_RA_CONTINUE_CONSUME_INFO_SC = 2435,							// 随机活动-连续消费
		MT_RA_ARMY_DAY_INFO_SC = 2436,									// 随机活动-军歌嘹亮所有信息
		MT_RA_CIRCULATION_CHONGZHI_INFO_SC = 2437,						// 随机活动-循环充值信息
		MT_RA_EXTREME_LUCKY_ALL_INFO_SC = 2438,							// 随机活动-至尊幸运所有信息
		MT_RA_EXTREME_LUCKY_SINGLE_INFO_SC = 2439,						// 随机活动-至尊幸运单个物品信息
		MT_RA_MIJINGXUNBAO2_INFO_SC = 2440,								// 秘境寻宝2信息
		MT_RA_MIJINGXUNBAO2_TAO_REWARD_INFO_SC = 2441,					// 秘境寻宝2结果信息
		MT_RA_MONEY_TREE2_INFO_SC = 2442,								// 摇钱树2信息
		MT_RA_MONEY_TREE2_CHOU_REWARD_INFO_SC = 2443,					// 摇钱树2奖励信息
		MT_RA_ZHENBAOGE2_INFO_SC = 2444,								// 珍宝阁2
		MT_RA_PROMOTING_POSITION_ALL_INFO_SC = 2445,					// 步步高升所有信息
		MT_RA_PROMOTING_POSITION_REWARD_INFO_SC = 2446,					// 步步高升奖励物品信息
		MT_RA_BLACK_MARKET_ALL_INFO_SC = 2447,							// 黑市竞拍所有信息
		MT_RA_TREASURES_MALL_ALL_INFO_SC = 2448,						// 珍宝商城所有信息
		MT_GOLD_VIP_INFO_SC = 2451,										// 黄金会员信息返回
		MT_WORLD_BOSS_WEARY_INFO_SC = 2452,								// 世界boss疲劳值信息
		MT_RA_LEVEL_LOTTERY_ACTIVITY_INFO_SC = 2453,					// 随机活动金银塔活动信息
		MT_RA_CONTINUE_CHONGZHI_INFO_CHU_SC = 2454,						// 随机活动-连充特惠初
		MT_RA_CONTINUE_CHONGZHI_INFO_GAO_SC = 2455,						// 随机活动-连充特惠高
		MT_RA_SINGLE_CHONGZHI_INFO_SC = 2456,							// 随机活动-单返豪礼
		MT_RA_XIANYUAN_TREAS_INFO_SC = 2457,							// 随机活动-聚划算
		MT_RA_RUSH_BUYING_ALL_INFO_SC = 2458,							// 随机活动-限时秒杀所有信息
		MT_RA_MAP_HUNT_ALL_INFO_SC = 2459,								// 随机活动-地图寻宝所有信息
		MT_RA_LIGHT_TOWER_ALL_INFO_SC = 2460,							// 灯塔寻宝所有信息
		MT_RA_LIGHT_TOWER_DRAW_INFO_SC = 2461,							// 灯塔寻宝抽奖信息
		MT_CSA_QUERY_ACTIVITY_INFO_CS = 2462,							// 查询合服信息
		MT_RA_MAGIC_SHOP_ALL_INFO_SC = 2463,							// 幻装商店信息返回
		MT_RA_PROFESS_RANK_INFO_SC = 2464,								// 表白排行活动个人信息返回
		MT_RA_CONTINUE_CHONGZHI_2_INFO_GAO_SC = 2466,					// 随机活动-限时连充

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FIGHT_MOUNT_GOON_REQ_CS = 2500,								// 请求上/下战斗坐骑
		MT_FIGHT_MOUNT_UPGRADE_REQ_CS = 2501,							// 请求战斗坐骑进阶
		MT_FIGHT_MOUNT_USE_IMAGE_CS = 2502,								// 请求使用形象
		MT_FIGHT_MOUNT_GET_MOUNT_INFO_CS = 2503,						// 请求战斗坐骑信息
		MT_FIGHT_MOUNT_UPLEVEL_EQUIP_CS = 2504,							// 战斗坐骑升级装备请求
		MT_FIGHT_MOUNT_SKILL_UPLEVEL_REQ_CS = 2505,						// 战斗坐骑技能升级请求
		MT_FIGHT_MOUNT_SPECIAL_IMG_UPGRADE_CS = 2506,					// 战斗坐骑特殊形象进阶
		MT_FIGHT_MOUNT_UP_STAR_LEVEL_CS = 2507,							// 战斗坐骑升星级请求

		MT_FIGHT_MOUNT_INFO_SC = 2550,									// 战斗坐骑信息
		MT_FIGHT_MOUNT_APPE_CHANGE_SC = 2551,							// 战斗坐骑外观改变

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SHENSHOU_OPERA_REQ_CS = 2560,								// 神兽操作请求
		MT_SHENSHOU_REQ_UPGRADE_EQUIP_CS = 2561,						// 神兽请求强化装备
		MT_SHENSHOU_BACKPACK_INFO_SC = 2562,							// 神兽背包信息
		MT_SHENSHOU_SHENSHOU_INFO_SC = 2563,							// 神兽信息
		MT_SHENSHOU_SHENSHOU_BASE_INFO_SC = 2564,						// 神兽基础信息
		MT_SHENSHOU_HUANLING_LIST_INFO_SC = 2565,						// 神兽唤灵信息
		MT_SHENSHOU_HUANLING_DRAW_RESULT_SC = 2566,						// 神兽唤灵抽奖结果

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_IMG_FULING_INFO_SC = 2570,									// 形象赋灵信息
		MT_IMG_FULING_OPERA_CS = 2571,									// 形象赋灵操作请求

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MENTALITY_LIST_SC=2700,										// 发送所有元神信息
		MT_GUILDFB_REWARD_INFO_SC = 2701,								// 仙盟副本奖励信息
		MT_RESERVE_2702_SC = 2702,										// 保留
		MT_RESERVE_2703_SC = 2703,										// 保留
		MT_RESERVE_2704_SC = 2704,										// 保留
		MT_ROLE_SHENGWANG_CHANGE_SC = 2705,								// 声望改变
		MT_WELFARE_INFO_SC = 2706,										// 福利信息通知
		MT_ACTIVEDEFREE_INFO_SC = 2707,									// 活跃度信息
		MT_HANGUP_SET_INFO_SC = 2708,									// 挂机设置发送
		MT_ROLE_ZHANHUN_CHANGE_SC = 2709,								// 战魂改变
		MT_DAILY_BUY_COIN_INFO_SC = 2710,								// 五谷丰收信息
		MT_WELFARE_DAILYFIND_CHANGE_SC = 2711,							// 日常找回单项变更
		RESERVE_MT_DAY_TASK_COUNT_ACK_SC = 2712,						// 保留
		MT_FUNCTION_SUCC_NOTICE_SC = 2713,								// 锻造、坐骑、宠物等功能成功提示
		MT_MENTALITY_YJTS_SC = 2714,									// 一键提升响应
		MT_ROLE_SUNDRY_INFO_SC = 2715,									// 杂项信息
		MT_TEST_ACTIVITY_INFO_SC = 2716,								// 发送封测活动信息
		MT_FUNCTION_FAIL_NOTICE_SC = 2717,								// 锻造、坐骑、宠物等功能失败提示
		MT_OPEN_GAME_ACTIVITY_INFO_SC = 2718,							// 开服活动状态
		MT_OPER_SUCC_SC = 2719,											// 操作成功
		MT_TOUZIJIHUA_INFO_SC = 2720,									// 投资计划信息
		MT_SEVEN_DAY_LOGIN_REWARD_INFO_SC = 2721,						// 七天登录奖励信息
		MT_WELFARE_PLAT_VIP_INFO_SC = 2722,								// 平台VIP福利信息
		MT_GENGU_INFO_SC = 2723,										// 根骨信息
		MT_YAOJIANG_INFO_SC = 2724,										// 摇奖信息
		MT_RESERVE_2725_SC = 2725,										// 保留
		MT_SUPPLY_INFO_SC = 2726,										// 下发补给信息
		MT_TOTAL_LOGIN_DAYS_SC = 2727,									// 总登录天数
		MT_TITLE_OWNER_INFO_SC = 2728,									// 称号拥有者信息
		MT_SPECIAL_PARAM_CHANGE_SC = 2729,								// 结婚者进入场景
		MT_GOLD_VIP_OPERA_REQ_CS = 2730,								// 黄金会员操作请求
		MT_TOUZIJIHUA_FB_BOSS_OPERATE_CS = 2731,						// 副本、boss投资计划请求
		MT_TOUZIJIHUA_FB_BOSS_INFO_SC = 2732,							// 副本、boss投资计划信息

		MT_LOUD_SPEAKER_CS=2750,										// 使用喇叭全服广播
		MT_GET_OFFLINE_EXP_CS = 2751,									// 获取离线经验
		MT_WELFARE_GET_DAILY_INFO_CS = 2752,							// 请求每日登陆福利
		MT_WELFARE_GET_DAILY_FIND_CS = 2753,							// 日常找回
		MT_WELFARE_FETCH_SEVEN_DAY_LOGIN_REWARD_CS = 2754,				// 请求领取七天登录奖励
		MT_WELFARE_UPLEVEL_REWARD_CS = 2755,							// 请求领取福利升级奖励
		MT_WELFARE_SIGN_IN_REWARD_CS = 2756,							// 请求领取签到奖励
		MT_WELFARE_SIGN_IN_FIND_BACK_CS = 2757,							// 请求签到找回
		MT_WELFARE_ACTIVITY_FIND_CS = 2758,								// 请求活动找回
		MT_WELFARE_AUTO_ACTIVITY_CS = 2759,								// 请求活动委托
		MT_MENTALITY_TRAIN_CS = 2760,									// 修炼元神请求
		MT_MENTALITY_UPGRADE_GENGU_CS = 2761,							// 升级元神根骨请求
		MT_MENTALITY_SPEED_UP_TRAIN_CS = 2762,							// 加速修炼请求
		MT_RESERVE_2763_CS = 2763,										// 保留
		MT_RESERVE_2764_CS = 2764,										// 保留
		MT_VIP_GET_WELFARE_CS = 2765,									// 领取VIP福利
		MT_STRATEGIES_UPGRADE_CS = 2766,								// 兵法提升
		MT_ACTIVEDEFREE_FETCH_REWARD_CS = 2767,							// 领取活跃度奖励
		MT_SAVE_HANGUP_SET_CS = 2768,									// 保存挂机设置请求
		MT_FETCH_VIP_LEVEL_REWARD = 2769,								// 领取vip等级奖励
		MT_DAILY_BUY_COIN_GET_CS = 2770,								// 五谷丰收收获
		MT_EVALUATE_ROLE_CS = 2771,										// 评价别人
		MT_SUPPLY_BUY_IYEM_CS = 2772,									// 补给购买
		MT_SUPPLY_SET_RECOVER_RANGE_PER_CS = 2773,						// 设置补给百分比
		MT_MENTALITY_YJTS_CS = 2774,									// 一键提升元神
		MT_QIFU_REQ_CS = 2775,											// 祈福请求
		MT_FETCH_QIFU_REWARD_CS = 2776,									// 祈福领取奖励
		MT_MENTALITY_CLEAR_TRAIN_CD_CS = 2777,							// 元神清除cd请求
		MT_MARRY_REQ_CS = 2778,											// 发起求婚
		MT_MARRY_RET_CS = 2779,											// 求婚回复
		MT_FETCH_TOUZIJIHUA_REWARD_CS = 2780,							// 领取投资计划奖励
		MT_TITLE_OWNER_INFO_REQ_CS = 2781,								// 请求称号拥有者信息
		MT_MARRY_HUNYAN_OPERA_CS = 2782,								// 结婚婚宴操作
		MT_MARRY_HUNYAN_BLESS_CS = 2783,								// 结婚婚宴祝福
		MT_QINGYUAN_INFO_SC = 2784,										// 情缘信息返回
		MT_MENTALITY_UPLEVEL_WUXING_CS = 2785,							// 提升五行等级
		MT_TOUZIJIHUA_ACTIVE_CS = 2786,									// 激活投资计划
		MT_NEW_TOUZIJIHUA_OPERATE_CS = 2787,							// 新投资计划操作

		MT_CLIENT_SPECIAL_OPERATE_CS = 2788,							// 客户端特殊操作
		MT_SAVE_CUSTOM_SET_CS = 2789,									// 保存自定设置请求
		MT_ROLE_TASK_TRANSMIT_CS = 2790,								// 角色任务传送
		MT_TASK_OPEN_PANEL_CS = 2791,									// 任务打开面板操作
		MT_YAOJIANG_REQ_CS = 2792,										// 摇奖请求
		MT_MARRY_XUNYOU_OPERA_CS = 2793,								// 结婚巡游操作
		MT_MARRY_HUNYAN_GET_BLESS_NEWS_CS = 2794,						// 获取婚宴祝福消息
		MT_QINGYUAN_OPERA_CS = 2795,									// 结婚相关操作请求
		MT_RESET_LOVER_NAME_CS = 2796,									// 重设结婚对象名
		MT_DATING_INVITE_REQ_CS = 2797,									// 发起约会邀请
		MT_DATING_INVITE_ACK_CS = 2798,									// 约会邀请答复
		MT_LIGHTEN_WEDDINGRING_CS = 2799,								// 点亮婚戒

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FISH_POOL_RECORD_AND_COMMONINFO_SC = 2800,					// 鱼池通用信息跟历史记录
		MT_FISH_POOL_ALL_RAISE_INFO_SC = 2801,							// 鱼池放养信息
		MT_FISH_POOL_COMMON_INFO_SC = 2802,								// 鱼池通用数据信息
		MT_FISH_POOL_RAISE_INFO_CHANGE_SC = 2803,						// 鱼池放养信息变化
		MT_FISH_POOL_WORLD_GENERAL_INFO_SC = 2804,						// 世界玩家鱼池简要信息
		MT_FISH_POOL_STEAL_GENERAL_INFO_SC = 2805,						// 偷鱼者池塘简要信息
		MT_WORLD_EVENT_COMMON_INFO_SC = 2806,							// 世界事件通用信息
		MT_WORLD_EVENT_ROLE_INFO_SC = 2807,								// 世界事件角色信息
		MT_WORLD_EVENT_NPC_LOCATE_INFO_SC = 2808,						// 世界事件NPC位置信息
		MT_WORLD_EVENT_KILL_FIND_NPC_MONSTER_SC = 2809,					// 世界事件 击杀寻找npc怪物
		MT_FISH_POOL_SHOU_FISH_REWARD_INFO_SC = 2810,					// 收鱼奖励信息

		MT_FISH_POOL_RAISE_REQ_CS = 2850,								// 放养鱼儿请求
		MT_FISH_POOL_BUY_BULLET_REQ_CS = 2851,							// 鱼池购买子弹请求
		MT_FISH_POOL_QUERY_REQ_CS = 2852,								// 鱼池请求信息操作
		MT_FISH_POOL_STEAL_FISH_CS = 2853,								// 鱼池偷鱼请求
		MT_FISH_POOL_HARVEST_FISH_CS = 2854,							// 鱼池收获请求
		MT_FISH_POOL_EXTEND_CAPACITY_CS = 2855,							// 鱼池拓展背包请求
		MT_WORLD_EVENT_COMMON_INFO_REQ_CS = 2856,						// 世界事件通用信息请求
		MT_WORLD_EVENT_QUERY_ROLE_INFO_CS = 2857,						// 世界事件请求角色信息
		MT_WORLD_EVENT_FETCH_REWARD_CS = 2858,							// 世界事件领取奖励
		MT_WORLD_EVENT_OBJ_TOUCH_CS = 2859,								// 触摸世界事件对象请求
		MT_WORLD_EVENT_LIGHT_WORD_CS = 2860,							// 世界事件点亮字
		MT_WORLD_EVENT_GET_NPC_LOCATE_INFO_CS = 2861,					// 世界事件获得NPC位置信息

		MT_FISH_POOL_UP_FISH_QUALITY_RET_SC = 2862,						// 升级鱼品质结果
		MT_FISH_POOL_FISH_POOL_CHANGE = 2863,							// 鱼数量变化

		MT_MARRY_RET_INFO_SC = 2864,									// 结婚操作结果返回

		MT_PROFESS_WALL_REQ_CS = 2865,									// 表白墙通用请求
		MT_PROFESS_TO_REQ_CS = 2866,									// 表白请求
		MT_GLOBAL_PROFESS_WALL_INFO_SC = 2867,							// 公共表白墙信息
		MT_PERSON_PROFESS_WALL_INFO_SC = 2868,							// 个人表白墙信息
		MT_PROFESS_EFFECT_SC = 2869,									// 个人表白墙特效信息	
		MT_PROFESS_LEVEL_INFO_SC = 2870,								// 表白等级信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_GM_COMMAND_SC = 3000,
		MT_GM_TESTSPEED_SC = 3001,										// 测速

		MT_GM_COMMAND_CS = 3050,
		MT_GM_TESTSPEED_CS = 3051,										// 测速

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 放一些通用的协议

		MT_DRAW_RESULT_SC = 3100,										// 抽奖结果

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ALL_TITLE_LIST_SC = 3600,									// 发送所有称号列表
		MT_USED_TITLE_LIST_SC = 3601,									// 发送使用的称号列表
		MT_USED_TITLE_CHANGE_SC = 3602,									// 使用的称号变化广播

		MT_GET_ALL_TITLE_CS = 3650,										// 请求所有称号列表
		MT_USE_TITLE_CS = 3651,											// 请求使用称号
		MT_TITLE_UPGRADE_CS = 3652,										// 请求称号进阶

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MAIL_SEND_CS = 3750,											// 发送邮件
		MT_MAIL_DELETE_CS = 3751,										// 删除邮件
		MT_MAIL_GETLIST_CS = 3752,										// 获取邮件列表
		MT_MAIL_READ_CS = 3753,											// 读取邮件
		MT_MAIL_ATTACHMENT_CS = 3754,									// 获取附件
		MT_MAIL_CLEAN_CS = 3755,										// 清空邮件
		MT_MAIL_ONE_KEY_FETCH_ATTACHMENT = 3756,						// 一键提取邮件

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_GUILD_BACK_TO_STATION_SC = 4200,								// 军团快速回营结果
		MT_GUILD_ADD_GUILD_EXP_SC = 4201,								// 军团捐献结果
		MT_GUILD_XIANSHU_INFO_SC = 4202,								// 仙盟仙术信息
		MT_GUILD_FB_STATUS_SC = 4205,									// 仙盟密境状态信息
		MT_GUILD_BOSS_SC = 4206,										// 仙盟boss信息
		MT_GUILD_BONFIRE_STATUS_SC = 4208,								// 仙盟篝火状态信息
		MT_GUILD_STORGE_SC = 4209,										// 仙盟仓库信息
		MT_GUILD_STORGE_CHANGE_SC = 4210,								// 仙盟仓库改变
		MT_GUILD_BOX_INFO_SC = 4211,									// 仙盟宝箱信息
		MT_GUILD_BOX_NEED_ASSIST_INFO_SC = 4212,						// 仙盟宝箱需要协助信息
		MT_GUILD_BOSS_ACTIVITY_SC = 4213,								// 仙盟boss活动信息
		MT_GUILD_RED_PAPER_REQ_LIST_INFO_CS = 4214,						// 仙盟红包，请求红包列表
		MT_GUILD_RED_PAPER_LIST_INFO_SC = 4215,							// 仙盟红包，红包列表信息
		MT_GUILD_STORGE_LOG_LIST_SC = 4216,								// 仙盟仓库日志信息

		MT_CREATE_GUILD_CS = 4250,										// 创建军团
		MT_DISMISS_GUILD_CS = 4251,										// 解散军团
		MT_APPLY_FOR_JOIN_GUILD_CS = 4252,								// 申请加入军团
		MT_APPLY_FOR_JOIN_GUILD_ACK_CS = 4253,							// 回复加入申请
		MT_QUIT_GUILD_CS = 4254,										// 申请退出军团
		MT_INVITE_GUILD_CS = 4255,										// 邀请加入军团
		MT_INVITE_GUILD_ACK_CS = 4256,									// 回复邀请
		MT_KICKOUT_GUILD_CS = 4257,										// 踢人申请
		MT_APPOINT_GUILD_CS = 4258,										// 任命
		MT_LEAVE_GUILD_POST_CS = 4259,									// 卸任
		MT_CHANGE_GUILD_NOTICE_CS = 4260,								// 修改军团公告
		MT_GUILD_MAIL_ALL_CS = 4261,									// 群发邮件
		MT_GET_GUILD_INFO_CS = 4262,									// 拉取军团相关信息
		MT_GUILD_DELATE_CS = 4263,										// 弹劾
		MT_GUILD_BACK_TO_STATION_CS = 6264,								// 快速回营
		MT_GUILD_RESET_NAME_CS = 4265,									// 军团改名请求
		MT_GUILD_APPLYFOR_SETUP_CS = 4266,								// 更改军团设置
		MT_GUILD_ADDEXP_CS = 4267,										// 给军团加经验
		MT_GUILD_SET_AUTO_KICKOUT_SETUP_CS = 4268,						// 军团设置是否自动踢人
		MT_GUILD_CALL_IN_CS = 4269,										// 军团招募
		MT_GUILD_XIANSHU_INFO_REQ_CS = 4270,							// 获得仙术信息
		MT_GUILD_XIANSHU_UPLEVEL_CS = 4271,								// 升级仙术
		MT_GUILD_CHECK_CAN_DELATE_CS = 4272,							// 检查是否可以弹劾
		MT_GUILD_FB_START_REQ_CS = 4275,								// 仙盟密境开启请求
		MT_GUILD_FB_ENTER_REQ_CS = 4276,								// 仙盟密境进入请求
		MT_GUILD_ZHUFU_LUCKY_REQ_CS = 4277,								// 仙盟祝福请求
		MT_GUILD_GET_GUILD_ALL_LUCKY_REQ_CS = 4278,						// 仙盟祝福请求
		MT_GUILD_INVITE_ZHUFU_LUCKY_CS = 4279,							// 邀请仙盟祝福请求
		MT_GUILD_CALL_BEAST_CS = 4280,									// 仙盟召唤凶兽请求
		MT_GUILD_ACTIVE_DEGREE_INFO_CS = 4281,							// 仙盟活跃度信息请求
		MT_GUILD_UP_LEVEL_CS = 4282,									// 军团升级
		MT_GUILD_GET_BUFF_CS = 4283,									// 领取buff
		MT_GUILD_EXCHANGE_CS = 4284,									// 兑换物品
		MT_GUILD_BOSS_CS = 4285,										// 仙盟boss操作请求
		MT_GUILD_BONFIRE_START_REQ_CS = 4286,							// 仙盟篝火开启请求
		MT_GUILD_BONFIRE_GOTO_REQ_CS = 4287,							// 仙盟篝火前往请求
		MT_GUILD_BONFIRE_ADDMUCAI_REQ_CS = 4288,						// 仙盟篝火添加木材请求
		MT_GUILD_STORGE_REQ_CS = 4289,									// 仙盟仓库操作请求
		MT_GUILD_SKILL_UPLEVEL_CS = 4290,								// 仙盟技能升级请求
		MT_GUILD_TOTEM_UP_LEVEL_CS = 4291,								// 仙盟图腾升级
		MT_GUILD_BOX_OPERATE_CS = 4292,									// 仙盟宝箱操作
		MT_GUILD_COMMON_REQ_CS = 4293,									// 仙盟通用请求
		MT_GUILD_FETCH_TERRITORY_REWARD_CS = 4294,						// 仙盟领地奖励
		MT_GUILD_STORGE_ONE_KEY_REQ_CS = 4295,							// 仙盟仓库一键操作请求
		MT_GUILD_EXTEND_MEMBER_CS = 4296,								// 仙盟扩展成员请求
		MT_GUILD_FETCH_GUILD_BOSS_REDBAG = 4297,						// 领公会杀boss红包
		MT_GUILD_FETCH_GUILD_RELIVE_TIMES_INFO = 4298,					// 公会复活次数信息
		MT_GUILD_FETCH_GUILD_BOSS_REDBAG_INFO = 4299,					// 公会boss红包信息
		MT_GUILD_FETCH_GUILD_BOSS_FETCH_INFO = 4300,					// 公会boss红包会员获取信息
		MT_GUILD_FETCH_GUILD_SAIZI_INFO = 4301,							// 公会骰子信息
		MT_GUILD_FETCH_GUILD_PAO_SAIZI = 4302,							// 公会抛骰子
		MT_GUILD_FETCH_GUILD_SAIZI_INFO_REQ = 4303,						// 请求骰子信息
		MT_GUILD_SET_AUTO_CLEAR_REQ = 4304,								// 请求自动清理公会成员

		MT_GUILD_TIANCI_TONGBI_RESULT_SC = 4305,						// 军团天赐铜币结果
		MT_GUILD_SYNC_TIANCI_TONGBI = 4306,								// 军团同步天赐铜币状态
		MT_GUILD_TIANCI_TONGBI_USER_GATHER_CHANGE_SC = 4307,			// 军团天赐铜币玩家采集物改变
		MT_GUILD_TIANCI_TONGBI_RANK_INFO_SC = 4308,						// 军团天赐铜币排行榜
		MT_GUILD_TIANCI_TONGBI_REQ_CS = 4309,							// 军团天赐铜币开启请求
		MT_GUILD_TIANCI_TONGBI_NPC_INFO = 4310,							// 军团天赐铜币npc信息
		MT_GUILD_TIANCI_TONGBI_AOI_GATHER_INFO = 4311,					// 军团天赐铜币采集动作广播
		MT_GUILD_GONGZI_LIST_INFO = 4312,								// 仙盟工资列表信息
		MT_GUILD_ENEMY_RANK_LIST_SC = 4313,								// 下发仙盟仇人列表信息
		MT_GUILD_ENEMY_RANK_LIST_CS = 4314,								// 获取仙盟仇人列表

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ADD_PUBLICSALE_ITEM_CS = 4450,								// 拍卖物品上架
		MT_REMOVE_PUBLICSALE_ITEM_CS = 4451,							// 拍卖物品下架
		MT_BUY_PUBLICSALE_BUY_CS = 4452,								// 购买拍卖物品
		MT_PUBLICSALE_GET_ITEM_LIST_CS = 4453,							// 获得自己的所有拍卖物品信息
		MT_PBULICSALE_SEND_ITEM_INFO_TO_WORLD_CS = 4454,				// 发送自己的拍卖物品信息到世界聊天窗
		MT_PBULICSALE_TYPE_COUNT_REQ_CS = 4455,							// 请求拍卖品种类数量
		MT_PBULICSALE_TYPE_COUNT_ACK_CS = 4456,							// 拍卖品种类数量下发
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SYSTEM_ROLE_MSG_REPLY = 4500,								// 解决客户端与服务器异步问题,客户端接到指定reply才继续发送

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MOUNT_INFO_SC = 4700,										// 单个坐骑信息
		MT_MOUNT_APPE_CHANGE_SC = 4701,									// 坐骑外观改变
		MT_WING_INFO_SC = 4703,											// 羽翼信息
		MT_WING_RESERVE_1_SC = 4704,									// 羽翼改变形象
		MT_DISCOUNT_BUY_INFO_SC = 4706,									// 一折抢购信息
		MT_MONTH_CARD_INFO_SC = 4707,									// 月卡信息
		MT_WING_POINT_INCREASE = 4708,									// 羽翼进化点数
		MT_ROLL_MONEY_INFO_SC = 4709,									// 摇钱信息
		MT_ROLE_GOAL_INFO_SC = 4710,									// 角色目标信息
		MT_ROLE_GOAL_CHANGE_SC = 4711,									// 角色目标信息改变
		MT_RED_PAPER_DETAIL_INFO_SC = 4712,								// 红包详细信息
		MT_RED_PAPER_FETCH_RESULT_SC = 4713,							// 红包领取结果
		MT_FAKE_PRIVATE_CHAT_SC = 4714,									// 伪造私聊
		MT_MARRIAGE_SEEKING_INFO_SC = 4715,								// 征婚宣言结果信息
		MT_GUILD_GUILD_ALL_LUCKY_SC = 4716,								// 仙盟成员的幸运信息返回
		MT_GUILD_INVITE_ZHUFU_LUCKY_SC = 4717,							// 邀请仙盟祝福通知
		MT_GUILD_LUCKY_CHANGE_NOTICE_SC = 4718,							// 仙盟祝福变更通知
		MT_SHIZHUANG_INFO_SC = 4719,									// 时装信息
		MT_CARD_INFO_SC = 4720,											// 卡牌信息
		MT_CARD_UPLEVEL_SUCC_SC = 4721,									// 卡牌升级成功
		MT_ROLE_SHADOW_BOSS_INFO_SC = 4722,								// 影子boss信息
		MT_GUILD_ACTIVE_DEGREE_INFO_SC = 4723,							// 仙盟活跃度信息
		MT_QINGYUAN_CARD_LEVEL_LIST_SC = 4724,							// 情缘卡牌等级列表
		MT_QINGYUAN_CARD_LEVEL_UPDATE_SC = 4725,						// 情缘卡牌等级变更
		MT_RED_PAPER_ROLE_INFO_SC = 4726,								// 个人红包信息
		MT_RED_PAPER_SEND_COMMAND_CS = 4727,							// 创建口令红包
		MT_RED_PAPER_COMMAND_REWARD_CS = 4728,							// 领取口令红包
		MT_RED_PAPER_COMMAND_CHECK_INFO_CS = 4729,						// 查看口令红包
		MT_RED_PAPER_COMMAND_SEND_INFO_SC = 4730,						// 发送口令红包信息

		MT_MOUNT_GOON_REQ_CS = 4750,									// 请求上/下坐骑
		MT_MOUNT_UPGRADE_REQ_CS = 4751,									// 请求坐骑进阶
		MT_MOUNT_USE_IMAGE_CS = 4752,									// 请求使用形象
		MT_MOUNT_SPECIAL_IMG_UPGRADE_CS = 4753,							// 坐骑特殊形象进阶
		MT_MOUNT_UPLEVEL_EQUIP_CS = 4754,								// 坐骑升级装备请求
		MT_MOUNT_SKILL_UPLEVEL_REQ_CS = 4755,							// 坐骑技能升级请求
		MT_MOUNT_GET_MOUNT_INFO_CS = 4756,								// 请求坐骑信息
		MT_WING_UPGRADE_REQ_CS = 4757,									// 请求羽翼进阶
		MT_MOUNT_UP_STAR_LEVEL_CS = 4758,								// 坐骑升星级请求
		MT_DISCOUNT_BUY_GET_INFO_CS = 4760,								// 获得一折抢购信息
		MT_DISCOUNT_BUY_REQ_BUY_CS = 4761,								// 一折抢购 购买
		MT_ROLL_MONEY_OPERA_REQ_CS = 4762,								// 摇元宝请求
		MT_ZHANSHENDIAN_FETCH_DAY_REWARD_CS = 4763,						// 领取战神殿每日奖励
		MT_ROLE_GOAL_OPERA_REQ_CS = 4764,								// 角色目标操作请求
		MT_RED_PAPER_CREATE_REQ_CS = 4765,								// 创建红包请求
		MT_RED_PAPER_FETCH_REQ_CS = 4766,								// 领取红包请求
		MT_RED_PAPER_QUERY_DETAIL_REQ_CS = 4767,						// 查询红包详细信息请求
		MT_MARRIAGE_SEEKING_REQ_CS = 4768,								// 查询红包详细信息请求
		MT_SHIZHUANG_USE_REQ_CS = 4771,									// 时装使用请求
		MT_CARD_OPERATE_CS = 4772,										// 卡牌操作请求
		MT_MONTH_CARD_FETCH_DAY_REWARD_CS = 4773,						// 月卡，领取每日奖励
		MT_WING_USE_IMAGE_CS = 4774,									// 羽翼请求使用形象
		MT_WING_GET_INFO_CS = 4775,										// 请求羽翼信息
		MT_WING_UP_STAR_LEVEL_CS = 4776,								// 羽翼升星级请求
		MT_DISCONNECT_REQ_CS = 4778,									// 离线请求
		MT_ZHUANZHI_REQ_CS = 4779,										// 转职请求
		MT_QINGYUAN_CARD_UPGRADE_REQ_CS = 4780,							// 情缘卡牌升级请求
		MT_SHIZHUANG_UPGRADE_REQ_CS = 4781,								// 时装进阶请求
		MT_QINGYUAN_COUPLE_HALO_INFO_SC = 4783,							// 情缘夫妻光环信息
		MT_QINGYUAN_COUPLE_HALO_TRIGGER_SC = 4784,						// 情缘夫妻光环触发信息
		MT_QINGYUAN_COUPLE_HALO_OPERA_REQ_CS = 4785,					// 夫妻光环请求
		MT_WING_UPLEVEL_EQUIP_CS = 4786,								// 羽翼升级装备请求
		MT_WING_SKILL_UPLEVEL_REQ_CS = 4787,							// 羽翼技能升级请求
		MT_WING_SPECIAL_IMG_UPGRADE_CS = 4789,							// 羽翼特殊形象进阶
		MT_QINGYUAN_BLESS_REWARD_REQ_CS = 4790,							// 情缘祝福奖励请求
		MT_QINGYUAN_ADD_BLESSDAYS_REQ_CS = 4791,						// 增加情缘祝福天数
		MT_LOVE_CONTRACT_OPERA_REQ_CS = 4792,							// 爱情契约操作请求
		MT_QINGYUAN_FETCH_LOVE_CONTRACT_CS = 4793,						// 领取爱情契约
		MT_QINGYUAN_LOVE_CONTRACT_INFO_SC = 4794,						// 爱情契约信息
		MT_RESERVE_4795_CS = 4795,										// 保留
		MT_ZERO_GIFT_INFO_SC = 4796,									// 零元礼包信息
		MT_ZERO_GIFT_OPERATE_REQ_CS = 4797,								// 零元礼包操作
		MT_ADVANCE_NOTIC_OPERATE_CS = 4798,								// 功能预告操作
		MT_ADVANCE_NOTICE_INFO_SC = 4799,								// 功能预告信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CHEST_SHOP_SELF_ITEM_LIST_SC = 4800,							// 返回自己宝箱商店物品列表
		MT_CHEST_SHOP_ITEM_LIST_PER_BUY_SC = 4801,						// 返回每次开宝箱得到的物品
		MT_CHEST_SHOP_FREE_INFO_SC = 4802,								// 发送宝箱免费信息
		MT_CONVERT_RECORD_INFO_SC = 4803,								// 兑换记录信息
		MT_MYSTERIOUS_NPC_REFRESH_SC = 4804,							// 神秘商店商人刷新
		MT_MYSTERIOUS_SHOPITEM_INFO_SC = 4805,							// 神秘商店信息
		MT_RARE_CHEST_SHOP_BUY_CS = 4806,								// 奇珍异宝抽奖请求
		MT_MYSTERIOUS_SHOP_BUY_OPERATE_CS = 4807,						// 神秘商店请求信息
		MT_MYSTERIOUS_SHOP_SEQ_INFO_SC= 4808,							// 神秘商店物品信息
		MT_SHIZHUANG_SPECIAL_IMG_UPGRADE_CS = 4809,						// 时装-请求升级特殊形象

		MT_CHEST_SHOP_GET_SELF_ITEM_LIST_CS = 4850,						// 获取自己宝箱商店物品列表
		MT_CHEST_SHOP_BUY_ITEM_CS = 4851,								// 宝箱商店购买
		MT_CHEST_SHOP_FETCH_ITEM_CS = 4852,								// 提取宝箱商店物品
		MT_SHOP_BUY_ITEM_CS = 4854,										// 购买商城物品
		MT_CONVERT_SCORE_TO_ITEM_CS = 4855,								// 兑换商店消耗积分兑换物品
		MT_CHEST_SHOP_GET_FREE_INFO = 4856,								// 获取宝箱商店免费时间
		MT_CONVERT_RECORD_GET_INFO_CS = 4857,							// 兑换记录信息请求
		MT_CHEST_SHOP_AUTO_RECYCLE_CS = 4858,							// 宝箱自动回收
		MT_MYSTERIOUSSHOP_OPERATE_CS = 4859,							// 神秘商人操作
		MT_CONVERT_GET_SOCRE_INFO_CS = 4860,							// 获取积分信息请求
		MT_CONVERT_GET_SOCRE_INFO_SC = 4861,							// 发送积分信息
		MT_CONVERT_GET_SOCRE_INFO_NOTICE_SC = 4862,						// 发送积分信息，用于右下角提示
		MT_CHEST_SHOP_GET_RECORD_LIST_CS = 4863,						// 获取寻宝物品记录列表
		MT_CHEST_SHOP_SEND_RECORD_LIST_SC = 4864,						// 返回寻宝物品记录列表

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_QUNXIANLUANDOU_LUCK_REWARD_SC = 4950,						// 群仙乱斗 幸运奖励
		MT_QUNXIANLUANDOU_PICKUP_SHENSHI_CS = 4951,						// 群仙乱斗 捡起神石请求
		MT_GET_BATTLEFIELD_HONOR_INFO_REQ_CS = 4952,					// 请求战场荣誉信息
		MT_XIANMENGZHAN_GUILD_CALL_CS = 4953,                   		// 仙盟战 盟主召唤
		MT_XIANMENGZHAN_GUILD_FOLLOW_CS = 4954,                 		// 仙盟战 盟友跟随
		MT_NATIONAL_BOSS_BUY_BUFF_CS = 4955,							// 世界boss 购买Buff请求
		MT_OPEN_GAME_ACTIVITY_REQ_INFO_CS = 4956,						// 开服活动请求活动信息
		MT_OPEN_GAME_ACTIVITY_FETCH_REWARD_CS = 4957,					// 开服活动另据爱那个请求
		MT_CHALLENGE_FIELD_GET_USER_INFO_CS = 4958,						// 请求排名竞技场用户信息
		MT_CHALLENGE_FIELD_RESET_OPPONENT_REQ_CS = 4959,				// 请求刷新竞技场对手信息
		MT_CHALLENGE_FIELD_FIGHT_REQ_CS = 4960,							// 排名竞技场决斗请求
		MT_CHALLENGE_FIELD_FETCH_JIFEN_REWARD_REQ_CS = 4961,			// 排名竞技场领取积分奖励
		MT_CHALLENGE_FIELD_GET_RANK_INFO_CS = 4962,						// 获得竞技场排名信息
		MT_CHALLENGE_FIELD_GET_LUCKY_INFO_CS = 4963,					// 获得竞技场幸运排名信息
		MT_CHALLENGE_FIELD_FETCH_GUANGHUI_CS = 4965,					// 排名竞技场领取光辉
		MT_CHALLENGE_FIELD_BUY_JOIN_TIMES_CS = 4966,					// 购买参与次数
		MT_CHALLENGE_FIELD_GET_OPPONENT_INFO_CS = 4967,					// 获得对手信息
		MT_CLOSE_BETA_ACTIVITY_OPERA_REQ_CS = 4968,						// 封测活动操作请求
		MT_LUCKY_ROLL_ACTIVITY_OPERA_REQ_CS = 4969,						// 幸运转盘活动操作请求
		MT_UPDATE_NOTICE_FETCH_REWARD_CS = 4970,						// 更新公告领取奖励请求
		MT_COMMON_ACTIVITY_FETCH_REWARD_CS = 4971,						// 领取通用活动奖励
		MT_CHALLENGE_FIELD_BUY_BUFF_CS = 4972,							// 排名竞技场购买buff
		MT_GUILD_BATTLE_DAILY_REWARD_FETCH_CS = 4975,					// 公会争霸领取每日奖励
		MT_GUILD_BATTLE_DAILY_REWARD_INFO_SC = 4976,					// 公会争霸领取每日奖励信息

		MT_BATTLEFIELD_HONOR_CHANGE_SC = 4902,							// 战场荣誉值改变
		MT_QUNXIANLUANDOU_USER_INFO_SC = 4903,							// 群仙乱斗 用户信息
		MT_QUNXIANLUANDOU_RANK_INFO_SC = 4904,							// 群仙乱斗 排行榜信息
		MT_QUNXIANLUANDOU_SIDE_INFO_SC = 4905,							// 群仙乱斗 阵营信息
		MT_QUNXIANLUANDOU_SHENSHI_STATUS_SC = 4906,						// 群仙乱斗 神石状态
		MT_XIANMENGZHAN_USER_INFO_SC = 4907,                    		// 仙盟战 用户信息
		MT_XIANMENGZHAN_GUILD_INFO_SC = 4908,                   		// 仙盟战 仙盟信息
		MT_XIANMENGZHAN_DEFEND_AREA_INFO_SC = 4909,             		// 仙盟战 据点信息
		MT_XIANMENGZHAN_GUILD_CALL_NOTICE_SC = 4910,            		// 仙盟战 盟主召唤通知
		MT_XIANMENGZHAN_RESULT_SC = 4911,								// 仙盟战 结果通知
		MT_XIANMENGZHAN_DANYAO_BOSS_INFO_SC = 4912,						// 仙盟战 丹药怪信息
		MT_XIANMENGZHAN_DEFEND_AREA_BE_ATTACK_NOTICE_SC = 4913,			// 仙盟战 据点被攻击
		MT_CHONG_ZHI_INFO_SC = 4914,									// 充值信息
		MT_NATIONAL_BOSS_USER_INFO_SC = 4915,							// 全服boss 用户信息
		MT_NATIONAL_BOSS_RANK_INFO_SC = 4916,							// 全服boss 排行信息
		MT_CHALLENGE_FIELD_STATUS_INFO_SC = 4917,						// 竞技场1v1副本状态信息
		MT_CHALLENGE_FIELD_USER_INFO_SC = 4918,							// 竞技场1v1用户信息
		MT_CHALLENGE_FIELD_OPPONENT_RANK_POS_CHANGE = 4919,				// 对手的排名发生变化
		MT_CHALLENGE_FIELD_REPORT_INFO_SC = 4920,						// 竞技场战报信息
		MT_CHALLENGE_FIELD_RANK_INFO_SC = 4921,							// 竞技场排名信息
		MT_CHALLENGE_FIELD_LUCKY_POS_INFO_SC = 4922,					// 竞技场幸运信息
		MT_GCZ_BUY_WEAPON_CS = 4923,									// 购买武器
		MT_QUNXIANLUANDOU_LIANZHAN_CHANGE_SC = 4928,					// 群仙乱斗 连斩数改变
		MT_XIANMENGZHAN_LIANZHAN_CHANGE_SC = 4930,						// 仙盟战连斩次数改变
		MT_CHALLENGE_FIELD_OPPONENT_INFO_SC = 4931,						// 竞技场1v1对手详细信息
		MT_CHALLENGE_BE_DEFEAT_NOTICE_SC = 4932,						// 竞技场被打败通知
		MT_CLOSE_BETA_ACTIVITY_INFO_SC = 4933,							// 封测活动信息
		MT_LUCKY_ROLL_ACTIVITY_INFO_SC = 4934,							// 幸运转盘活动信息
		MT_LUCKY_ROLL_ACTIVITY_ROLL_RESULT_SC = 4935,					// 幸运转盘奖励结果
		MT_UPDATE_NOTICE_INFO_SC = 4936,								// 更新公告信息
		MT_CHALLENGE_FIELD_REWARD_BIND_GOLD_INFO_SC = 4937,				// 竞技场胜利奖励元宝信息
		MT_COMMON_ACTIVITY_OPEN_UNIT_INFO_SC = 4938,					// 通用活动开启的UNIT信息
		MT_XIANMENGZHEN_GUILD_RANK_INFO_SC = 4939,						// 仙盟战仙盟排行信息
		MT_XINGZUOYIJI_BOX_AND_BOSS_INFO_SC = 4940,						// 星座遗迹宝箱与boss信息
		MT_CHALLENGE_FIELD_WIN = 4941,									// 1V1竞技场直接胜利

		MT_CHALLENGE_FIELD_BEST_RANK_BREAK_REQ_CS = 4942,				// 1V1竞技场历史最高突破请求
		MT_CHALLENGE_FIELD_BEST_RANK_BREAK_INFO_SC = 4943,				// 1V1竞技场历史最高突破信息
		MT_CHALLENGE_FIELD_READY_START_FIGHT_CS = 4944,					// 1V1竞技场请求准备正式开始战斗倒计时

		MT_QUNXIANLUANDOU_RANK_FIRST_REQ_CS = 4945,						// 群仙乱斗第一排名请求
		MT_QUNXIANLUANDOU_RANK_FIRST_INFO_SC = 4946,					// 群仙乱斗第一排名信息


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_DAYCOUNTER_INFO_SC = 5000,									// 当日计数信息
		MT_DAYCOUNTER_ITEM_INFO_SC = 5001,								// 当日计数单项信息
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MOJIE_INFO_SC = 5050,										// 发送魔戒信息

		M_MOJIE_INFO_REQ_CS = 5075,										// 请求魔戒信息
		M_MOJIE_UP_LEVEL_REQ_CS = 5076,									// 请求升级魔戒
		M_MOJIE_CHANGE_SKILL_REQ_CS = 5077,								// 请求改变魔戒技能

		//////////////////////////////////////////////////////////////////////////
		MT_SUBLOCK_OPERATE_SUCC_SC = 5100,								// 输入密码正确
		MT_SUBLOCK_OPERATE_FAIL_SC = 5101,								// 二级锁操作失败
		MT_SUBLOCK_INFO_SC = 5102,										// 二级锁信息

		MT_SUBLOCK_CREATE_PW_CS = 5150,									// 创建密码
		MT_SUBLOCK_CLEAR_PW_CS = 5151,									// 清除密码
		MT_SUBLOCK_LOCK_CS = 5152,										// 上锁
		MT_SUBLOCK_UNLOCK_CS = 5153,									// 解锁

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ACTIVITY_ENTER_REQ_CS = 5253,								// 请求进入活动
		MT_ZHUXIE_FETCH_TASK_REWARD_CS = 5254,							// 领取诛邪战场任务奖励
		MT_SEND_GUILD_SOS_REQ = 5255,									// 发送帮派求救信号请求
		MT_HUNYAN_BUY_XUNYOU_OPERA_CS = 5256,							// 请求购买巡游操作
		MT_TURNTABLE_ROLL_REQ_CS = 5257,								// 请求转盘转动
		MT_WABAO_OPERA_REQ_CS = 5258,									// 挖宝操作请求
		MT_CHONGZHI_FETCH_REWARD_CS = 5259,								// 领取充值奖励
		MT_JILIAN_OPERA_CS = 5260,										// 祭炼操作请求
		MT_TOTAL_CHONGZHI_WANT_MONEY_FETCH_MSG_CS = 5262,				// 我要变元宝-获取玩家当前充值状态数据
		MT_TOTAL_CHONGZHI_WANT_MONEY_FETCH_REWARD_CS = 5263,			// 我要变元宝-获取玩家当前随机出来的奖励
		MT_ZAI_CHONGZHI_FETCH_REWARD_CS = 5264,							// 再充值，领取奖励
		MT_REPLY_GUILD_SOS_REQ = 5265,									// 回应帮派求救

		MT_ZHUXIE_USER_INFO_SC = 5200,									// 诛邪战场角色信息
		MT_TURNTABLE_MILLIONAIRE_VIEW_SC = 5201,						// 大富豪外观形象广播
		MT_ACTIVITY_SKILL_USE_INFO_SC = 5202,							// 技能使用通知
		MT_HUNYAN_XUNYOU_OPERA_INFO_SC = 5203,							// 结婚巡游 操作信息
		MT_TURNTABLE_REWARD_SC = 5204,									// 转盘奖励下发
		MT_TURNTABLE_INFO_SC = 5205,									// 转盘基本信息
		MT_WABAO_INFO_SC = 5206,										// 挖宝信息
		MT_JILIAN_INFO_SC = 5207,										// 祭炼信息
		MT_JILIAN_VIEW_CHANGE_SC = 5208,								// 祭炼视图改变
		MT_TOTAL_CHONGZHI_WANT_MONEY_INFO_SC = 5210,					// 我要变元宝-获取玩家当前充值状态数据
		MT_TOTAL_CHONGZHI_WANT_MONEY_FETCH_REWARD_SC = 5211,			// 我要变元宝-获取玩家当前随机出来的奖励
		MT_RA_CORNUCOPIA_FETCH_INFO_SC = 5212,							// 聚宝盆，获取当前状态信息
		MT_RA_CORNUCOPIA_FETCH_REWARD_SC = 5213,						// 聚宝盆，获取玩家当前随机出来的奖励
		MT_TIANJIANGCAIBAO_USER_INFO_SC = 5214,							// 天降财宝角色信息

		MT_MOVE_CHESS_RESET_REQ_CS = 5221,								// 走棋子-请求重置
		MT_MOVE_CHESS_SHAKE_REQ_CS = 5222,								// 走棋子-请求摇骰
		MT_MOVE_CHESS_FREE_INFO_REQ_CS = 5223,							// 走棋子-请求走棋子信息
		MT_MOVE_CHESS_SHAKE_SC = 5224,									// 走棋子-摇骰点数返回

		MT_RA_NEW_CORNUCOPIA_INFO_SC = 5226,							// 新聚宝盆信息

		MT_RA_COLLECT_EXCHANGE_INFO_SC = 5230,							//集字活动信息
		
		MT_RA_CONSUME_FOR_GIFT_ALL_INFO_SC = 5235,						// 消费有礼信息
																		
		MT_RA_MIJINGXUNBAO3_INFO_SC = 5236,								// 秘境寻宝3信息
		MT_RA_MIJINGXUNBAO3_TAO_REWARD_INFO_SC = 5237,					// 秘境寻宝3结果信息

		MT_RA_HUANLEZADAN_INFO_SC = 5238,								// 欢乐砸蛋信息
		MT_RA_HUANLEZADAN_TAO_REWARD_INFO_SC = 5239,					// 欢乐砸蛋结果信息

		MT_RA_HUANLEYAOJIANG_INFO_SC = 5240,							// 欢乐摇奖信息
		MT_RA_HUANLEYAOJIANG_TAO_REWARD_INFO_SC = 5241,					// 欢乐摇奖结果信息
		
		MT_DRESSING_ROOM_OPEAR_CS = 5245,								// 衣柜请求
		MT_DRESSING_ROOM_INFO_SC = 5246,								// 衣柜信息下发
		MT_DRESSING_ROOM_SINGLE_INFO_SC = 5247,							// 衣柜单项信息下发
		MT_DRESSING_ROOM_EXCHANGE_CS = 5248,							// 衣柜兑换

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_HOTSPRING_GIVE_PRESEN_CS = 5300,								// 温泉请求送礼物
		MT_HOTSPRING_ADD_PARTNER_CS = 5301,								// 找伙伴请求
		MT_HOTSPRING_ADD_PARTNER_RET_CS = 5302,							// 被邀请作伙伴的玩家处理邀请伙伴请求
		MT_HOTSPRING_DELETE_PARTNER_CS = 5303,							// 取消伙伴
		MT_HOTSPRING_QA_FIRST_REQ_CS = 5304,							// 请求答题榜首位置信息
		MT_HOTSPRING_QA_ANSWER_REQ_CS = 5305,							// 请求答题
		MT_HOTSPRING_QA_USE_CAR_REQ_CS = 5306,							// 请求使用变身卡

		MT_HOTSPRING_SEND_PARTNER_INFO_SC = 5350,						// 发送伙伴信息
		MT_HOTSPRING_SEND_EXP_INFO_SC = 5351,							// 发送历史经验信息
		MT_HOTSPRINT_PLAYER_INFO_SC = 5352,								// 温泉玩家信息
		MT_HOTSPRINT_RANK_INFO_SC = 5353,								// 温泉玩家排名信息
		MT_HOTSPRING_ADD_PARTNER_ROUT_SC = 5354,						// 询问被邀请作伙伴的玩家
		MT_HOTSPRING_SEND_SHUANGXIU_INFO_SC = 5355,						// 广播发送温泉双休信息
		MT_HOTSPRING_SEND_QA_RANK_INFO_SC = 5356,						// 发送温泉答题排名信息
		MT_HOTSPRING_SEND_QA_QUESTION_INFO_SC = 5357,					// 发送温泉答题题目信息
		MT_HOTSPRING_SEND_QA_FIRST_POST_SC = 5358,						// 发送温泉答题第一名位置信息
		MT_HOTSPRING_SEND_QA_RESULT_SC = 5359,							// 发送温泉答题回答结果信息

		MT_HOTSPRING_USE_SKILL_REQ_CS = 5360,							// 温泉使用技能请求
		MT_HOTSPRING_NOTICE_SKILL_INFO_SC = 5361,						// 发送技能使用信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CROSS_GUILDBATTLE_SCENE_GUILDER_NUM_SC = 5368,				// 跨服帮派战，场景盟友数量
		MT_CROSS_GUILDBATTLE_BOSS_HURT_INFO_SC = 5369,					// 跨服帮派战，BOSS伤害信息
		MT_CROSS_GUILDBATTLE_OPERA_REQ_CS = 5370,						// 跨服帮派战，操作请求
		MT_CROSS_GUILDBATTLE_GET_RANK_INFO_REQ_CS = 5371,				// 跨服帮派战，请求获取排行榜结果
		MT_CROSS_GUILDBATTLE_GET_RANK_INFO_RESP_SC = 5372,				// 跨服帮派战，返回获取排行榜结果
		MT_CROSS_GUILDBATTLE_SCENE_INFO_SC = 5373,						// 跨服帮派战排行信息
		MT_CROSS_GUILDBATTLE_NOTIFY_INFO_SC = 5374,						// 跨服帮派战通知信息
		MT_CROSS_GUILDBATTLE_INFO_SC = 5375,							// 跨服帮派战信息
		MT_CROSS_GUILDBATTLE_TASK_INFO_SC = 5376,						// 跨服帮派战，任务信息
		MT_CROSS_GUILDBATTLE_BOSS_INFO_SC = 5377,						// 跨服帮派战，Boss信息
		MT_CROSS_GUILDBATTLE_FLAG_INFO_SC = 5378,						// 跨服帮派战，所有旗帜信息
		MT_CROSS_GUILDBATTLE_MONSTER_INFO_SC = 5379,					// 跨服帮派战，精英怪信息
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_QUESTION_RIGHT_BROCAST_SC = 5380,							// 发送答题问题
		MT_QUESTION_ANSWER_REQ_CS = 5381,								// 请求回答问题
		MT_QUESTION_ANSWER_RESULT_SC = 5382,							// 发送答题回答结果信息
		MT_QUESTION_ANSWER_GUILD_RANK_SC = 5383,						// 发送公会答题回答排名信息
		MT_QUESTION_ANSWER_RIGHT_NUM_SC = 5384,							// 发送公会答题回答排名信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FB_TOWERDEFEND_ROLE_INFO_CS = 5453,							// 请求塔防角色信息
		MT_FB_TOWERDEFEND_BUY_JOIN_TIMES_CS = 5454,						// 请求购买塔防参与次数
		MT_FB_EQUIP_GETINFO_CS = 5457,									// 装备副本：请求装备副本信息

		MT_GET_BOSS_INFO_REQ_CS = 5463,									// 请求boss信息
		MT_FB_GUILDMONSTER_CS = 5464,									// 仙盟神兽请求信息
		MT_FB_PHASE_INFO_CS = 5465,										// 阶段副本个人信息请求
		MT_BOSS_KILLER_INFO_REQ = 5466,									// Boss击杀列表信息请求
		MT_FB_CHALLENGE_FB_OPERATE_CS = 5467,							// 挑战副本操作请求
		MT_QINGYUAN_FB_OPERA_REQ_CS = 5469,								// 情缘副本操作请求
		MT_RESERVE_5470 = 5470,											// 购买情缘副本进入次数
		MT_HUNJIE_UP_LEVEl_CS = 5471,									// 情缘升级
		MT_QINGYUAN_TAKE_OFF_EQUIP = 5472,								// 脱下情缘装备
		MT_HUNJIE_REQ_EQUIP_INFO_CS = 5473,								// 情缘装备信息请求
		MT_QINGYUAN_QUERY_MATE_QINGYUAN_VALUE_CS = 5474,				// 请求伴侣情缘值查询
		MT_QINGYUAN_DIVORCE_REQ_CS = 5475,								// 离婚请求
		MT_FB_REQ_NEXT_LEVEL_CS = 5476,									// 通用副本请求下一关
		MT_ENTER_BOSS_FAMILY_CS = 5477,									// 进入boss之家请求
		MT_GUILDFB_INFO_SC = 5478,										// 仙盟副本信息
		MT_GET_SCENE_MONSTER_GENERATE_INFO_CS = 5479,					// 请求 场景刷新 boss信息
		MT_GET_SCENE_MONSTER_GENERATE_INFO_SC = 5480,					// 返回 场景刷新 boss信息
		MT_GET_YAOSHOUGUANGCHANG_STATE_CS = 5481,						// 获取妖兽广场状态
		MT_YAOSHOUGUANGCHANG_STATE_SC = 5482,							// 返回 妖兽广场状态
		MT_YAOSHOUGUANGCHANG_FBINFO_SC = 5483,							// 返回 妖兽广场 副本信息
		MT_GET_YAOSHOUGUANGCHANG_REWARD_CS = 5484,						// 返回 妖兽广场 奖励信息
		MT_NOTIFY_YAOSHOUGUANGCHANG_REWARD_SC = 5485,					// 通知 妖兽广场 奖励信息
		MT_ENTER_YAOSHOUGUANGCHANG_CS = 5486,							// 进入妖兽广场
		MT_ZHUA_GUI_FBINFO_CS = 5487,									// 抓鬼副本状态
		MT_WORLD_BOSS_DEAD_SC = 5488,									// 世界boss死亡
		MT_ZHUA_GUI_ADD_PER_INFO_CS = 5489,								// 抓鬼加成信息
		MT_GET_SUOYAOTA_STATE_CS = 5490,								// 获取锁妖塔状态
		MT_SUOYAOTA_STATE_SC = 5491,									// 返回 锁妖塔状态
		MT_SUOYAOTA_FBINFO_SC = 5492,									// 返回 锁妖塔 副本信息
		MT_GET_SUOYAOTA_REWARD_CS = 5493,								// 返回 锁妖塔 奖励信息
		MT_NOTIFY_SUOYAOTA_REWARD_SC = 5494,							// 通知 锁妖塔 奖励信息
		MT_ENTER_SUOYAOTA_CS = 5495,									// 进入锁妖塔
		MT_GET_GUILDFB_GUARD_POS_CS = 5496,								// 获取仙盟副本守护位置
		MT_GUILDFB_GUARD_POS_SC = 5497,									// 获取仙盟副本守护位置
		MT_CROSS_TEAM_FB_INFO_SC = 5498,								// 跨服组队副本信息

		MT_FB_STORY_REQ_INFO_CS = 5400,									// 剧情本:请求剧情本所有信息
		MT_FB_STORY_INFO_SC = 5401,										// 剧情本:剧情本信息
		MT_FB_DAILY_REQ_ROLE_INFO_CS = 5410,							// 日常副本:请求日常副本所有信息
		MT_FB_DAILY_ROLE_INFO_SC = 5411,								// 日常副本:所有信息
		MT_FB_TOWERDEFEND_INFO_SC = 5413,								// 日常副本：单个塔防副本信息

		MT_FB_PATA_REQ_ALL_INFO_CS = 5420,								// 爬塔副本:请求爬塔副本信息
		MT_FB_PATA_ALL_INFO_SC = 5421,									// 爬塔副本:信息返回
		MT_FB_VIP_ALL_INFO_REQ_CS = 5422,								// VIP副本:请求副本信息返回
		MT_FB_VIP_ALL_INFO_SC = 5423,									// VIP副本:信息返回
		MT_FB_TOWERDEFEND_WARNING_SC = 5431,							// 塔防副本：生命塔挨打报警
		MT_FB_TOWERDEFEND_RESULT_SC = 5433,								// 组队塔防副本 塔防副本通关结果
		MT_FB_YSJT_TEAM_FB_LOGIC_INFO_SC = 5434,						// 组队妖兽祭坛副本:副本信息
		MT_FB_MGXF_TOUCH_DOOR_CS = 5445,								// 组队迷宫仙府副本：进入传送点
		MT_FB_MGXF_TEAM_FB_LOGIC_INFO_SC = 5446,						// 组队迷宫仙府副本:副本信息
		MT_FB_TEAM_ROOM_ENTER_AFFIRM_SC = 5448,							// 组队副本房间进入确认
		MT_FB_TEAM_ROOM_LIST_SC = 5449,									// 组队副本房间列表返回
		MT_FB_TEAM_ROOM_OPERATE_CS = 5450,								// 组队副本房间请求操作

		MT_FB_TOWERDEFEND_ROLE_INFO_SC = 5404,							// 塔防角色信息
		MT_FB_CHALLENGE_INFO_SC = 5407,									// 挑战副本信息
		MT_FB_CHALLENGE_PASS_LEVEL_SC = 5408,							// 挑战副本通关评级
		MT_FB_EQUIP_RESULT_SC = 5409,									// 装备副本：通关结果
		MT_FB_EQUIP_EXP_SC = 5412,                                      // 装备副本：经验
		MT_FB_EQUIP_INFO_SC = 5414,                                     // 装备副本：信息
		MT_FB_GUILDMONSTER_INFO_SC = 5415,								// 仙盟神兽信息下发
		MT_FB_GUILDMONSTER_RESULT_SC = 5416,							// 仙盟神兽通关结果
		MT_FB_AUTO_FB_REWARD_DETAIL_SC = 5417,							// 扫荡奖励返回
		MT_FB_PHASE_INFO_SC = 5418,										// 阶段副本基本信息
		MT_FB_CHALLENGE_LAYER_INFO_SC = 5419,							// 挑战副本层信息
		MT_QINGYUAN_BASE_FB_INFO_SC = 5425,								// 情缘副本基本信息下发
		MT_QINGYUAN_FB_INFO_SC = 5426,									// 情缘副本场景信息下发
		MT_HUNJIE_EQUIP_INFO_SC = 5427,									// 情缘装备信息
		MT_QINGYUAN_MATE_QINGYUAN_VALUE_SC = 5428,						// 伴侣情缘值下发
		MT_FB_BOSS_HP_PERCENT_INFO_SC = 5429,							// 副本通关或者失败记录
		MT_ZHANSHENDIAN_FB_INFO_SC = 5437,								// 战神殿副本信息下发
		MT_ZHANSHENDIAN_FB_RESULT_SC = 5438,							// 战神殿副本通关结果下发
		MT_FB_TIANGONGSHILIAN_FB_RESULT_SC = 5436,						// 天宫试练 副本结果
		MT_FB_TIANGONGSHILIAN_FB_INFO_SC = 5437,						// 天宫试练 副本信息
		MT_QINGYUAN_BLESS_INFO_SC = 5438,								// 情缘祝福信息
		MT_QINGYUAN_HUNYAN_INVITE_SC = 5439,							// 情缘邀请信息
		MT_HUNYAN_ROLE_INFO_SC = 5440,									// 婚宴场景个人信息
		MT_HUNJIE_EQUIP_UPLEVEL_SEND_LOVER_INFO_SC = 5441,				// 升级情缘装备同步给伴侣
		MT_QINGYUAN_FB_REWARD_RECORD_INFO_SC = 5442,					// 情缘副本奖励结算信息
		MT_FB_PICK_ITEM_INFO_SC = 5443,									// 副本掉落拾取信息

		MT_FB_SCENE_GUWU_CS = 5498,										// 副本鼓舞
		MT_FB_SCENE_LGIC_INFO_SC = 5499,								// 副本通用信息
		////////////////////////////////////////////////////////////////
		// 5400段的协议id比较乱，新加协议不再用这个
		////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_WUSHUANG_EQUIP_INFO_SC = 5500,								// 无双装备信息
		MT_WUSHUANG_FB_KICKOUT_NOTIFY_SC = 5501,						// 无双副本踢人原因
		MT_WUSHUANG_FB_INFO_SC = 5502,									// 无双副本信息
		MT_WUSHUANG_BAG_INFO_SC = 5503,									// 无双背包信息
		MT_HUNSHOU_OPERA_STATUS_SC = 5504,								// 魂兽操作状态
		MT_ZHUANZHI_FB_NOTIFY_SC = 5505,								// 转职副本通知
		MT_ENTER_BOSS_FAMILY_OPERATE_CS = 5506,							// boss之家操作请求

		MT_GET_SCENE_GATHER_GENERATE_INFO_CS = 5525,					// 请求 场景刷新 采集物信息
		MT_GET_SCENE_GATHER_GENERATE_INFO_SC = 5526,					// 返回 场景刷新 采集物信息
		MT_FB_DROP_INFO_SC = 5527,										// 副本怪物掉落统计
		MT_FB_FINISH_SC = 5528,											// 副本通用通关通知
		MT_FB_TOWERDEFEND_REFRESH_REQ_CS = 5529,						// 塔防副本 立刻刷新

		MT_WUSHUANG_OPERA_REQ_CS = 5550,								// 无双装备请求
		MT_WUSHUANG_FB_INFO_CS = 5551,									// 无双副本信息请求
		MT_HUNSHOU_OPERA_REQ_CS = 5552,									// 魂兽操作请求

		MT_EXP_FB_FETCH_FIRST_REWARD_REQ_CS = 5554,						// 日常副本:经验本领取首通奖励
		MT_EXP_FB_INFO = 5555,											// 经验副本信息
		MT_EXP_FB_PAY_BUFF = 5556,										// 经验副本购买鼓舞effect

		MT_TEAM_EQUIP_FB_INFO_SC = 5570,								// 组队装备副本信息
		MT_TEAM_EQUIP_FB_DROP_TIMES_INFO_SC = 5571,						// 组队装备副本掉落次数信息
		MT_TEAM_EQUIP_FB_BUY_DROP_TIMES_CS = 5572,						// 组队装备副本购买掉落次数

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CARDZU_BASE_INFO_SC = 5600,									// 卡组基本信息
		MT_CARDZU_LEVEL_LIST_INFO_SC = 5601,							// 指定卡组所有组合等级信息
		MT_CARDZU_CARD_LIST_INFO_SC = 5602,								// 指定卡组所有卡牌数量信息
		MT_JINGLING_EXPLORE_OPER_REQ_CS = 5603,							// 精灵探险操作请求
		MT_JINGLING_EXPLORE_INFO_SC = 5604,								// 精灵探险信息
		MT_JINGLING_HOME_ROB_ACK_SC = 5605,								// 精灵家园掠夺结果
		MT_JINGLING_ADVANTAGE_INFO_SC = 5606,							// 精灵奇遇信息
		MT_JINGLING_ADVANTAGE_COUNT_SC = 5607,							// 精灵奇遇蓝色精灵数量
		MT_JINGLING_ADVANTAGE_BOSS_POS_SC = 5608,						// 精灵奇遇boss信息
		MT_JINGLING_ADVANTAGE_BOSS_ENTER_CS = 5609,						// 飞至奇遇boss
		MT_JINGLING_EGG_POS_INFO = 5610,                                // 精灵蛋位置

		MT_CARDZU_OPERA_REQ_CS = 5650,									// 卡组相关操作请求
		MT_LIEMING_HUNSHOU_OPERA_REQ_CS = 5655,							// 猎魂请求
		MT_LIEMING_SLOT_INFO_SC = 5656,									// 无双猎命槽信息
		MT_LIEMING_BAG_INFO_SC = 5657,									// 无双背包信息
		MT_LIEMING_HUNSHOU_EXCHANGE_CS = 5658,							// 命魂交换请求
		MT_JINGLING_OPER_REQ_CS = 5670,									// 精灵操作请求
		MT_JINGLING_INFO_SC = 5671,										// 精灵信息
		MT_JINGLING_VIEW_CHANGE_SC = 5672,								// 精灵形象改变广播
		MT_SHENGWANG_OPERA_CS = 5675,									// 声望操作请求
		MT_SHENGWANG_INFO_SC = 5676,									// 声望信息
		MT_XIANJIE_VIEW_CHANGE_SC = 5677,								// 仙阶变化
		MT_CHENGJIU_OPERA_CS = 5680,									// 成就操作请求
		MT_CHENGJIU_INFO_SC = 5681,										// 成就信息
		MT_CHENGJIU_TITLE_VIEW_CHANGESC = 5682,							// 成就称号变化
		MT_CHENGJIU_REWARD_CHANGESC = 5683,								// 成就称号变化
		MT_SHENZHOU_WEAPON_OPERA_REQ_CS = 5684,							// 神州六器相关操作请求
		MT_PASTURE_SPIRIT_OPERA_REQ_CS = 5685,							// 牧场精灵相关操作请求
		MT_MULTI_MOUNT_OPERA_REQ_CS = 5686,								// 双人坐骑操作请求
		MT_PERSONALIZE_OPERA_REQ_CS = 5687,								// 个性聊天框请求
		MT_MAGIC_CARD_OPERA_REQ_CS = 5688,								// 魔卡请求
		MT_FAIRY_TREE_OPERA_REQ_CS = 5689,								// 仙树操作请求
		MT_MAGIC_EQUIPMENT_OPERA_REQ_CS = 5690,							// 魔器操作请求
		MT_CHENGJIU_REWARD_INFO_SC = 5691,								// 宝具成就信息
		MT_SHEN_ZHOU_BOX_INFO_SC = 5692,								// 神州六器宝箱信息
		MT_SHEN_ZHOU_ONE_KEY_IDENTIFY_CS = 5693,						// 神州六器一键鉴定
		MT_SHEN_ZHOU_HUNYIN_RESOLVE_CS = 5694,							// 神州六器魂印分解
		MT_JINGLING_HOME_OPER_REQ_CS = 5695,							// 精灵操作请求
		MT_JINGLING_HOME_INFO_SC = 5696,								// 精灵家园信息
		MT_JINGLING_HOME_LINT_INFO_SC = 5697,							// 精灵家园列表信息
		MT_JINGLING_HOME_ROB_RECORD_SC = 5698,							// 精灵家园掠夺记录

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CROSS_START_REQ_CS = 5750,									// 请求开始跨服
		MT_CROSS_XIULUO_TOWER_JOIN_REQ_CS = 5751,						// 跨服修罗塔报名
		MT_CROSS_XIULUO_TOWER_BUY_BUFF_CS = 5752,						// 跨服修罗塔购买buff
		MT_CROSS_1V1_MATCH_REQ_CS = 5753,								// 跨服1v1请求匹配
		MT_CROSS_1V1_FIGHT_READY_CS = 5754,								// 跨服1v1准备战斗
		MT_CROSS_1V1_FETCH_REWARD_REQ_CS = 5755,						// 跨服1v1领取奖励
		MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_REQ_CS = 5756,			// 跨服3V3请求匹配
		MT_CROSS_MULTIUSER_CHALLENGE_GET_BASE_SELF_SIDE_INFO_CS = 5757,	// 跨服3V3请求初始队伍信息
		MT_CROSS_MULTIUSER_CHALLENGE_FETCH_DAYCOUNT_REWARD_CS = 5758,	// 跨服3V3领取每日次数奖励
		MT_CROSS_MULTIUSER_CHALLENGE_CANCLE_MATCHING_CS = 5759,			// 跨服3V3取消匹配
		MT_CROSS_MULTIUSER_CHALLENGE_REQ_SIDE_MEMBER_POS_CS = 5760,		// 跨服3v3，获取队友位置信息
		MT_CROSS_TUANZHAN_FETCH_SCOR_REWARD_CS = 5761,					// 跨服团战领取积分奖励
		MT_CROSS_1V1_BUY_TIME_REQ_CS = 5762,							// 跨服1v1请求购买次数
		MT_CROSS_1V1_RING_OPER_CS = 5763,								// 跨服1v1请求戒指操作  
		MT_CROSS_1V1_MATCH_RESULT_REQ_CS = 5764,					    // 跨服1v1匹配结果请求

		MT_CROSS_ENTER_SERVER_SC = 5700,								// 通知客户端进入隐藏服
		MT_CROSS_XIULUO_TOWER_SELF_ACTIVITY_INFO_SC = 5701,				// 跨服修罗塔个人活动信息
		MT_CROSS_XIULUO_TOWER_RANK_INFO_SC = 5702,						// 跨服修罗塔排行榜信息
		MT_CROSS_XIULUO_TOWER_CHANGE_LAYER_NOTICE_SC = 5703,			// 跨服修罗塔改变层提示
		MT_CROSS_XIULUO_TOWER_USER_RESULT_SC = 5704,					// 跨服修罗塔结果
		MT_CROSS_XIULUO_TOWER_INFO_SC = 5705,							// 跨服修罗塔信息
		MT_CROSS_HONOR_CHANGE_SC = 5706,								// 跨服荣誉变更
		MT_CROSS_1V1_SELF_INFO_SC = 5707,								// 跨服1V1个人信息
		MT_CROSS_1V1_FIGHT_START_SC = 5708,								// 跨服1V1战斗开始
		MT_CROSS_MULTIUSER_CHALLENGE_SELF_INFO_REFRESH_SC = 5709,		// 跨服3V3个人信息更新
		MT_CROSS_MULTIUSER_CHALLENGE_MATCH_INFO_REFRESH_SC = 5710,		// 跨服3V3比赛信息更新
		MT_CROSS_MULTIUSER_CHALLENGE_MATCH_STATE_SC = 5711,				// 跨服3V3状态通知
		MT_CROSS_MULTIUSER_CHALLENGE_BASE_SELF_SIDE_INFO_SC = 5712,		// 跨服3V3初始队伍信息
		MT_CROSS_MULTIUSER_CHALLENGE_SELF_ACTIVITY_INFO_SC = 5713,		// 跨服3V3个人活动信息
		MT_CROSS_MULTIUSER_CHALLENGE_REQ_SIDE_MEMBER_POS_SC = 5714,		// 跨服3v3，获取队友位置信息
		MT_CROSS_TUANZHAN_STATE_NOTIFY_SC = 5715,						// 跨服团战，比赛状态通知
		MT_CROSS_TUANZHAN_PLAYER_INFO_SC = 5716,						// 跨服团战，玩家信息
		MT_CROSS_TUANZHAN_RANK_INFO_SC = 5717,							// 跨服团战，排名信息
		MT_CROSS_TUANZHAN_SIDE_INFO_SC = 5718,							// 跨服团战，阵营积分信息
		MT_CROSS_TUANZHAN_PILLA_INFO_SC = 5719,							// 跨服团战，通天柱子信息
		MT_CROSS_TUANZHAN_DUR_KILL_INFO_SC = 5720,						// 跨服团战，连杀信息变更
		MT_CROSS_TUANZHAN_RESULT_INFO_SC = 5721,						// 跨服团战，比赛结果
		MT_CROSS_PASTURE_PLAYER_INFO_SC = 5722,							// 跨服牧场，玩家信息
		MT_CROSS_BOSS_BUY_RELIVE_TIMES_CS = 5723,						// 跨服BOSS，请求购买复活次数
		MT_CROSS_BOSS_PLAYER_INFO_SC = 5724,							// 跨服BOSS，玩家信息
		MT_CROSS_BOSS_SCENE_PLAYER_INFO_SC = 5725,						// 跨服BOSS，场景里面的玩家信息
		MT_SERVER_CLOSE_NOTIFY_SC = 5726,								// 服务器即将关闭通知
		MT_CROSS_XIULUO_TOWER_BUFF_INFO_SC = 5727,						// 跨服修罗塔BUFF信息
		MT_CROSS_XIULUO_TOWER_SCORE_REWARD_REQ_CS = 5728,				// 跨服修罗塔，请求积分奖励
		MT_CROSS_XIULUO_TOWER_GATHER_INFO_SC = 5729,					// 跨服修罗塔，采集物信息
		MT_CROSS_BOSS_BOSS_INFO_REQ_CS = 5730,							// 跨服BOSS，BOSS信息请求
		MT_CROSS_BOSS_BOSS_INFO_ACK_SC = 5731,							// 跨服BOSS，BOSS信息
		MT_CROSS_PASTURE_ATTACH_ANIMAL_NOTIC = 5732,					// 跨服牧场吸引怪物通知
		MT_CROSS_CROSS_GUILD_BATTLE_SPECIAL_TIME_NOTICE_SC = 5733,		// 跨服帮派战三倍积分提醒
		MT_CROSS_PASTURE_RANK_INFO_SC = 5734,							// 跨服牧场，玩家排行信息
		MT_CROSS_XIULUO_TOWER_BUY_BUFF_REQ_CS = 5735,					// 跨服修罗塔购买采集buff
		MT_CROSS_MULTIUSER_CHALLENGE_CARD_OPER_REQ = 5736,				// 跨服3v3令牌请求
		MT_CROSS_MULTIUSER_CHALLENGE_FETCH_GONGXUN_REWARD_SC = 5737,	// 跨服3v3领取功勋奖励
		MT_CROSS_BOSS_KILLER_RECORD_SC = 5738,                          // 跨服BOSS,击杀信息
		MT_CROSS_BOSS_DROP_RECORD_SC = 5739,                            // 跨服BOSS,掉落信息
		MT_RELIVE_TIRE_INFO_SC = 5740,                                  // 复活疲劳
		MT_CROSS_XIULUO_RNAK_TITLE_INFO_SC = 5741,						// 跨服修罗塔前三名进入10层的人
		MT_CROSS_XIULUO_TOWER_BOSS_INFO = 5742,							// 修罗塔boss信息

		MT_CROSS_DARK_NIGHT_USER_INFO_SC = 5770,						// 月黑风高玩家信息
		MT_CROSS_DARK_NIGHT_RANK_INFO_SC = 5771,						// 月黑风高排行信息
		MT_CROSS_DARK_NIGHT_BOSS_INFO_SC = 5772,						// 月黑风高boss信息
		MT_CROSS_DARK_NIGHT_PLAYER_INFO_BROADCAST_SC = 5773,			// 月黑风高玩家信息广播
		MT_CROSS_DARK_NIGHT_REWARD_TIMESTAMP_INFO_SC = 5774,			// 月黑风高发奖励时间戳
		MT_CROSS_DARK_NIGHT_TOP_PLAYERPOS_INFO_SC = 5775,				// 月黑风高排行榜第一玩家坐标信息
		MT_CROSS_DARK_NIGHT_PLAYER_REQ_CS = 5776,						// 月黑风高玩家请求

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_XUNZHANG_ALL_INFO_SC = 5800,									// 勋章所有信息
		MT_XUNZHANG_UPLEVEL_REQ_CS = 5801,								// 勋章升级请求
		MT_ZHIBAO_ALL_INFO_SC = 5805,									// 至宝所有信息
		MT_ZHIBAO_UPLEVEL_CS = 5806,									// 至宝升级请求
		MT_ZHIBAO_USEIMAGE_CS = 5807,									// 至宝形象请求
		MT_ZHIBAO_HUANHUA_CS = 5808,									// 至宝幻化请求
		MT_ZHIBAO_ATTACK_SC = 5852,										// 至宝攻击信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_TW_RELIVE_SHOP_BUY_CS = 5900,								// 领土战 复活商店购买商品
		MT_TW_FIGHT_SHOP_BUY_CS = 5901,									// 领土战 战斗商店购买商品
		MT_TW_QUALIFICATION_CS = 5902,									// 领土战 获取参赛资格信息
		MT_TW_SET_LANDMINE_REQ_CS = 5903,								// 领土战 设置地雷请求

		MT_TW_ROLE_IMAGE_INFO_SC = 5950,								// 领土战 广播变身形象
		MT_TW_GLOBE_INFO_SC = 5951,										// 领土战 全局信息
		MT_TW_ROLE_INFO_SC = 5952,										// 领土战 个人信息
		MT_TW_QUALIFICATION_SC = 5953,									// 领土战 发送参赛资格信息
		MT_TW_LUCK_REWARD_SC = 5954,									// 领土战 幸运奖励

		MT_FITHTING_MINING_REQ_CS = 5955,								// 决斗场系列 - 挖矿请求
		MT_FIGHTING_MINING_BASE_INFO_SC = 5956,							// 决斗场系列 - 挖矿基础信息
		MT_FIGHTING_MINING_BEEN_ROB_LIST_SC = 5957,						// 决斗场系列 - 挖矿被抢列表
		MT_FIGHTING_MINING_LIST_SC = 5958,								// 决斗场系列 - 挖矿列表
		MT_FIGHTING_CHALLENGE_BASE_INFO_SC = 5959,						// 决斗场系列 - 挑衅基础信息
		MT_FIGHTING_MINING_FIGHTING_RESULT = 5960,						// 决斗场系列 - 战斗结果
		MT_FIGHTING_CHALLENGE_LIST_SC = 5961,							// 决斗场系列 - 挑衅对手列表
		MT_FIGHTING_SAILING_BASE_INFO_SC = 5962,						// 决斗场系列 - 航海基础信息
		MT_FIGHTING_SAILING_BEEN_ROB_LIST_SC = 5963,					// 决斗场系列 - 航海被抢列表
		MT_FIGHTING_SAILING_LIST_SC = 5964,								// 决斗场系列 - 航海列表
		MT_FIGHTING_COUNT_DOWN_NOTIFY_SC = 5965,						// 决斗场系列 - 战斗开始时间通知
		MT_FIGHTING_BEEN_ROB_NOTIFY_SC = 5966,							// 决斗场系列 - 被抢夺通知（个人）
		MT_FIGHTING_ROBING_NOTIFY_SC = 5967,							// 决斗场系列 - 抢夺广播（发所有人）

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CUR_WDDING_INFO_SC = 6000,									// 当前婚宴场景信息请求返回
		MT_MAZE_INFO_SC = 6001,											// 迷宫信息
		MT_MAZE_MOVE_RET_SC = 6002,										// 迷宫移动结果
		MT_MARRY_SPECIAL_EFFECT_SC = 6003,								// 结婚特效
		MT_MARRY_REQ_ROUTE_SC = 6004,									// 求婚信息转发给对方
		MT_DIVORCE_REQ_ROUTE_SC = 6005,									// 离婚信息转发给对方
		MT_MARRY_XUNYOU_FIREWORK_SC = 6006,								// 结婚巡游烟花
		MT_HUNYAN_YUEYUE_INFO_SC = 6007,								// 结婚婚宴预约信息返回
		MT_MARRY_HUNYAN_OPERA_SC = 6008,								// 结婚婚宴操作
		MT_MARRY_HUNYAN_STATE_SC = 6009,								// 婚宴场景状态信息
		MT_HUNYAN_BLESS_RECORD_SC = 6010,								// 婚宴祝福记录

		MT_ACTIVITY_FIND_INFO_SC = 6011,								// 活动找回信息
		MT_WEDDINGRING_SKILL_INFO_SC = 6012,							// 婚戒技能信息

		MT_MARRY_NOTICE_SC = 6013,										// 结婚通知
		MT_MARRY_ZHUHE_SEND_CS = 6014,									// 结婚祝贺发送
		MT_MARRY_ZHUHE_SHOU_SC = 6015,									// 结婚祝贺接收
		MT_HUNYAN_APPLY_LIST_SC = 6016,									// 结婚婚宴申请列表信息

		MT_QING_YUAN_SHENG_DI_OPERA_REQ_CS = 6020,						// 情缘圣地操作请求
		MT_QING_YUAN_SHENG_DI_TASK_INFO_SC = 6021,						// 情缘圣地任务信息
		MT_QING_YUAN_SHENG_DI_BOSS_INFO_SC = 6022,						// 情缘圣地boss信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MAZE_QUERY_CS = 6100,										// 请求迷宫信息
		MT_MAZE_MOVE_CS = 6101,											// 请求迷宫移动
		MT_MAZE_BUY_CS = 6102,											// 请求迷宫购买次数
		MT_MAZE_FETCH_JIFEN_REWARD_CS = 6103,							// 领取迷宫积分奖励
		MT_MOLONG_INFO_SC = 6104,										// 魔龙系统
		MT_PET_RENAME_NOTIFY_SC = 6105,									// 宠物改名广播
		MT_PET_BACKPACK_INFO_SC = 6106,									// 宠物背包信息
		MT_PET_INFO_SC = 6107,											// 宠物基础信息
		MT_PET_CHOU_RESULT_SC = 6108,									// 宠物抽奖结果
		MT_PET_FIGHT_OUT_INFO_SC = 6109,								// 宠物出战信息
		MT_PET_OPERA_REQ_CS = 6110,										// 宠物相关操作请求
		MT_PET_UPGRADE_REQ_CS = 6111,									// 宠物升阶请求
		MT_PET_USE_SPECIAL_IMG_CS = 6112,				     			// 使用特殊形象
		MT_RA_SPECIAL_APPEARACE_PASSIVE_RANK_INFO_SC = 6113,			// 被动变身榜
		MT_RA_SPECIAL_APPEARACE_RANK_INFO_SC = 6114,					// 特殊形象榜
		MT_HUASHEN_ALL_INFO_SC = 6115,									// 化神信息
		MT_HUASHEN_OPERA_REQ_CS = 6116,									// 化神请求
		MT_PET_QINMI_ALL_INFO_SC = 6117,								// 宠物亲密所有信息
		MT_PET_QINMI_SINGLE_INFO_SC = 6118,								// 宠物亲密单个信息
		MT_PET_FOOD_MARKET_CHOU_RESULT_SC = 6119,						// 宠物吃货市场抽将结果
		MT_PASTURE_SPIRIT_ALL_INFO_SC = 6120,							// 牧场精灵所有信息
		MT_PASTURE_SPIRIT_SINGLE_INFO_SC = 6121,						// 牧场精灵单个信息
		MT_PASTURE_SPIRIT_LUCKY_DRAW_RESULT_SC = 6122,					// 牧场精灵抽奖结果
		MT_MULTI_MOUNT_ALL_INFO_SC = 6123,
		MT_MULTI_MOUNT_CHANGE_NOTIFY_SC = 6124,							// 双人坐骑增量更新通知
		MT_MULTI_MOUNT_NOTIFY_AREA_SC = 6125,							// 双人坐骑信息变更区域广播
		MT_PERSONALIZE_WINDOW_ALL_INFO_SC = 6126,						// 个性化窗口所有信息
		MT_PERSONALIZE_AVATAR_WINDOW_ALL_INFO_SC = 6127,				// 个性化窗口-头像框所有信息
		MT_HUASHEN_SPIRIT_INFO_SC = 6129,								// 化神守护精灵
		MT_MAGIC_CARD_ALL_INFO_SC = 6130,								// 魔卡所有信息
		MT_MAGIC_CARD_CHOU_RESULT_SC = 6131,							// 魔卡抽奖信息
		MT_WUSHANG_EQUIP_REQ_CS = 6132,									// 无上神兵请求
		MT_WUSHANG_EQUIP_INFO_SC = 6133,								// 无上神兵信息
		MT_MITAMA_OPERA_REQ_CS = 6134,									// 御魂请求
		MT_MITAMA_ALL_MITAMA_INFO_SC = 6135,							// 御魂信息
		MT_MITAMA_SINGLE_MITAMA_INFO_SC = 6136,							// 御魂单个信息
		MT_MITAMA_HOT_SPRING_SCORE_SC = 6137,							// 温泉积分
		MT_FAIRY_TREE_ALL_INFO_SC = 6139,								// 仙树所有信息
		MT_FAIRY_TREE_DRAW_REWARD_INFO_SC = 6140,						// 仙树抽奖信息
		MT_MAGIC_EQUIPMENT_ALL_INFO_SC = 6141,							// 魔器所有信息
		MT_MAGIC_EQUIPMENT_CHANGE_NOTIFY_SC = 6142,						// 魔器增量变更信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_GB_ROLE_ADD_HUDUN_CS = 6200,									// 公会争霸  主动释放护盾
		MT_GB_ROLE_SUBMIT_GOODS_CS = 6201,								// 公会争霸  提交护送物品请求
		MT_GB_WINNER_INFO_CS = 6202,									// 公会争霸  请求上一届霸主信息

		MT_GB_ROLE_BIANSHEN_IMAGE_SC = 6250,							// 公会争霸广播变身形象
		MT_GB_GLOBAL_INFO_SC = 6251,									// 公会争霸 全局信息（广播）
		MT_GB_ROLE_INFO_SC = 6252,										// 公会争霸 个人信息
		MT_GB_WINER_INFO_SC = 6253,										// 公会争霸 返回上一届霸主信息
		MT_GB_LUCK_REWARD_SC = 6254,									// 公会争霸 幸运奖励
		MT_GB_GLOBAL_INFO_NEW_SC = 6255,								// 新公会争霸 全局信息（广播）
		MT_GB_ROLE_INFO_NEW_SC = 6256,									// 新公会争霸 个人信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_HALO_INFO_SC = 6400,											// 光环信息
		MT_HALO_UPGRADE_REQ_CS = 6402,									// 请求光环进阶
		MT_HALO_USE_IMAGE_CS = 6403,									// 光环请求使用形象
		MT_HALO_GET_INFO_CS = 6404,										// 请求光环信息
		MT_HALO_UPLEVEL_EQUIP_CS  = 6405,								// 光环升级装备请求
		MT_HALO_SKILL_UPLEVEL_REQ_CS = 6406,							// 光环技能升级请求
		MT_HALO_SPECIAL_IMG_UPGRADE_CS =6407,							// 光环特殊形象进阶
		MT_HALO_UP_STAR_LEVEL_CS = 6408,								// 光环升星级请求

		MT_SHENGONG_INFO_SC = 6450,										// 神弓信息
		MT_SHENGONG_UPGRADE_REQ_CS = 6452,								// 神弓进阶
		MT_SHENGONG_USE_IMAGE_CS = 6453,								// 神弓请求使用形象
		MT_SHENGONG_GET_INFO_CS = 6454,									// 请求神弓信息
		MT_SHENGONG_UPLEVEL_EQUIP_CS  = 6455,							// 神弓升级装备请求
		MT_SHENGONG_SKILL_UPLEVEL_REQ_CS = 6456,						// 神弓技能升级请求
		MT_SHENGONG_SPECIAL_IMG_UPGRADE_CS =6457,						// 神弓特殊形象进阶
		MT_SHENGONG_UP_STAR_LEVEL_CS = 6458,							// 神弓升星级请求
		MT_SHENGONG_UNUSE_IMAGE_CS = 6459,								// 神弓请求取消使用形象

		MT_SHENYI_INFO_SC = 6500,										// 神翼信息
		MT_SHENYI_UPGRADE_REQ_CS = 6502,								// 请求神翼进阶
		MT_SHENYI_USE_IMAGE_CS = 6503,									// 神翼请求使用形象
		MT_SHENYI_GET_INFO_CS = 6504,									// 请求神翼信息
		MT_SHENYI_UPLEVEL_EQUIP_CS  = 6505,								// 神翼升级装备请求
		MT_SHENYI_SKILL_UPLEVEL_REQ_CS = 6506,							// 神翼技能升级请求
		MT_SHENYI_SPECIAL_IMG_UPGRADE_CS =6507,							// 神翼特殊形象进阶
		MT_SHENYI_UP_STAR_LEVEL_CS = 6508,								// 神翼升星级请求
		MT_SHENYI_UNUSE_IMAGE_CS = 6509,								// 神翼取消请求使用形象

		MT_FOOTPRINT_INFO_SC = 6520,									// 足迹信息
		MT_FOOTPRINT_OPERATE_CS = 6521,									// 足迹操作请求

		MT_CLOAK_INFO_SC = 6525,										// 披风信息
		MT_CLOAK_OPERATE_CS = 6526,										// 披风操作请求

		MT_ENTER_BOSS_DABAO_INFO_SC = 6551,								// 打宝boss信息
		MT_ENTER_BOSS_FAMILY_INFO_SC = 6552,							// boss之家boss信息
		MT_ENTER_BOSS_MIKU_INFO_SC = 6553,								// 秘窟boss信息
		MT_ENTER_BOSS_ROLE_INFO_SC = 6554,								// boss相关个人信息
		MT_DABAO_BOSS_NEXT_FLUSH_TIME_SC = 6555,						// 打宝boss刷新时间信息
		MT_BOSS_INFO_TO_ALL_SC = 6556,									// boss广播信息
		MT_BOSS_KILLER_LIST_SC = 6557,									// boss击杀列表信息
		MT_FOLLOW_BOSS_INFO_SC = 6558,									// 关注boss列表信息
		MT_ACTIVE_BOSS_NEXT_FLUSH_TIME_SC = 6559,						// 活跃boss刷新时间信息
		MT_ENTER_BOSS_ACTIVE_INFO_SC = 6560,							// 活跃boss信息
		MT_ENTER_BOSS_MIKU_MONSTER_INFO_SC = 6561,						// 秘窟怪物信息
		MT_PRECIOUS_BOSS_TASK_INFO_SC = 6562,							// 秘藏boss任务信息
		MT_PRECIOUS_BOSS_INFO_SC = 6563,								// 秘藏boss信息
		MT_PRECIOUS_BOSS_POS_REQ_CS = 6564,								// 秘藏boss位置请求
		MT_PRECIOUS_BOSS_POS_INFO_CS = 6565,							// 秘藏boss位置信息
		MT_ACTIVE_BOSS_HURT_RANK_INFO_SC = 6566,						// 活跃boss伤害排行信息
		MT_ACTIVE_BOSS_LEAVE_INFO_SC  = 6567,							// 离开活跃boss伤害信息区域
		MT_MIKU_BOSS_HURT_RANK_INFO_SC = 6568,							// 精英boss伤害排行信息
		MT_MIKU_BOSS_LEAVE_INFO_SC = 6569,								// 离开精英boss伤害信息区域

		MT_FOLLOW_BOSS_REQ_CS = 6570,									// boss关注请求
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_UPLEVEL_WEDDINGRING_CS = 6600,								// 升级婚戒
		MT_PERFORM_WEDDINGRING_SKILL_CS =6601,							// 释放婚戒技能
		MT_WELFARE_ONLINE_REWARD_CS = 6602,								// 福利在线奖励
		MT_DIVORCE_RET_CS = 6603,										// 离婚回复
		MT_MARRY_INFOMATION_SC = 6604,									// 结婚信息
		MT_WELFARE_HAPPYTREE_REWARD_CS = 6605,							// 欢乐果树奖励
		MT_WANGLINGEXPLORE_USER_INFO_SC = 6606,							// 王陵探险角色信息
		MT_CROSS_TEAM_FB_OPTION_CS = 6607,								// 跨服组队本操作
		MT_RA_OPEN_SERVER_INFO_SC = 6608,								// 随机活动开服活动信息
		MT_RA_OPEN_SERVER_UPGRADE_INFO_SC = 6609,						// 随机活动开服活动全服进阶信息
		MT_RA_OPEN_SERVER_RANK_INFO_SC = 6610,							// 随机活动开服活动排行信息
		MT_BAI_BEI_FAN_LI_INFO_SC = 6611,								// 百倍返利信息
		MT_BAI_BEI_FAN_LI_BUY_CS = 6612,								// 百倍返利购买物品
		MT_SHUIJING_TASK_INFO_SC = 6613,								// 水晶幻境，任务信息通知
		MT_CROSS_TEAM_FB_SELF_INFO_SC = 6614,							// 跨服组队本自己信息
		MT_FETCH_VIP_WEEK_REWARD = 6615,								// 领取vip周奖励
		MT_IS_ACCEPT_MARRY_SC = 6616,									// 是否同意结婚
		MT_CROSS_SHUIJING_PLAYER_INFO_SC = 6617,						// 跨服水晶幻境，玩家信息通知
		MT_CROSS_SHUIJING_BUY_BUFF_CS = 6618,							// 跨服水晶幻境，购买buff
		MT_RA_OPEN_SERVER_BOSS_INFO_SC = 6619,							// 随机活动开服活动Boss猎手信息
		MT_RA_OPEN_SERVER_BATTEL_INFO_SC = 6620,						// 随机活动开服活动开服争霸信息
		MT_SHUIJING_PLAYER_INFO_SC = 6621,								// 水晶幻境，玩家信息通知
		MT_SHUIJING_BUY_BUFF_CS = 6622,									// 水晶幻境，购买buff
		MT_WELFARE_CHONGJIHAOLI_REWARD_CS = 6623,                       // 领取冲级豪礼
		MT_MAGICAL_PRECIOUS_FETCH_REWARD_CS = 6624,                     // 领取魔龙秘宝请求
		MT_MAGICAL_PRECIOUS_ALL_INFO_REQ_SC = 6625,						// 魔龙秘宝所有信息
		MT_RA_OPEN_GAME_GIFT_SHOP_INFO_SC = 6626,						// 礼包限购活动信息
		MT_RA_OPEN_BUY_GAME_GIFT_SHOP_INFO_CS = 6627,					// 购买限购礼包请求
		MT_RA_OPEN_GAME_GIFT_SHOP_INFO_CS = 6628,						// 获取礼包限购活动信息请求
		MT_RA_EXP_REFIN_REQ_CS = 6629,									// 经验炼制请求
		MT_RA_EXP_REFINE_INFO_SC = 6630,								// 经验炼制信息
		MT_FETCH_TIME_LIMIT_VIP_CS = 6631,								// 获取限时vip
		MT_SHUIJING_FETCH_TASK_REWARD_CS = 6632,						// 水晶幻境，领取任务奖励
		MT_SHUIJING_GATHER_WUDI_INFO = 6633,							// 水晶幻境，采集不中断
		MT_MAGICAL_PRECIOUS_CUR_CHAPTER_INFO_REQ_SC = 6634,				// 魔龙秘宝当前章节信息
		MT_SHUIJING_SCENE_INFO_SC = 6635,								// 水晶幻境，场景信息通知
		MT_MAGICAL_PRECIOUS_CONDITION_CHANGE_SC = 6636,					// 魔龙秘宝条件变化信息
		MT_WANGLINGEXPLORE_BOSS_INFO_SC = 6637,							// 王陵探险BOSS信息
		MT_WANGLINGEXPLORE_BUY_BUFF_CS = 6638,							// 王陵探险买buff

		MT_CROSS_BIANJINGZHIDI_USER_INFO_SC = 6640,						// 跨服边境之地角色信息
		MT_CROSS_BIANJINGZHIDI_BOSS_INFO_SC = 6641,						// 跨服边境之地BOSS信息
		MT_CROSS_BIANJINGZHIDI_BOSS_HURT_INFO_SC = 6642,				// 跨服边境之地BOSS伤害排行信息
		MT_CROSS_BIANJINGZHIDI_BUY_BUFF_CS = 6643,						// 跨服边境之地买buff

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_LOVE_TREE_WATER_REQ_CS = 6650,								// 相思树-请求浇水
		MT_LOVE_TREE_INFO_REQ_CS = 6651,								// 相思树-请求相思树信息
		MT_LOVE_TREE_SEND_INFO_SC = 6675,							    // 相思树-发送相思树信息
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_GUILD_INFO_STATISTIC_REQ_CS = 6690,							// 仙盟输出统计信息请求
		MT_GUILD_INFO_STATISTIC_SC = 6691,								// 仙盟输出统计信息 
		MT_GUILD_MVP_INFO_SC = 6692,									// 仙盟MVP信息推送

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_GCZ_GLOBAL_INFO_SC = 6701,									// 攻城战 全局信息（广播）
		MT_GCZ_ROLE_INFO_SC = 6702,										// 攻城战 个人信息
		MT_GCZ_REWARD_INFO_SC = 6703,							        // 攻城战 奖励结算
		MT_GONGCHENGZHAN_OWNER_INFO_SC = 6704,							// 攻城战 城主信息
		MT_GONGCHENGZHAN_GET_OWNER_INFO_REQ_CS = 6705,					// 攻城战 获取城主信息
		MT_GCZ_FLAG_INFO_SC = 6706,										// 攻城战 雕像信息
		MT_GCZ_CHANGE_PLACE_CS = 6751,									// 攻城战 改变角色的在地图上的位置
		MT_GCZ_FETCH_DAILY_REWARD_CS = 6752,							// 攻城战 领取每日奖励
		MT_GCZ_LUCK_REWARD_SC = 6753,									// 攻城战 幸运奖励

		MT_GCZ_WORSHIP_INFO_SC = 6754,									// 攻城战膜拜信息
		MT_GCZ_WORSHIP_ACTIVITY_INFO_SC = 6755,							// 攻城战膜拜活动信息
		MT_GCZ_WORSHIP_CS = 6756,										// 攻城战膜拜点击

		MT_WORSHIP_COMMON_INFO_SC = 6757,								// 下发膜拜信息(几个活动公用)
		MT_WORSHIP_COMMON_START_SC = 6758,								// 广播角色开始膜拜(几个活动公用)
		MT_WORSHIP_COMMON_CS = 6759,									// 膜拜点击(几个活动公用)

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_XN_SHOUHU_UPSTAR_CS = 6800,									// 仙女守护请求升星
		MT_XN_SHOUHU_USE_IMAGE_CS = 6801,								// 仙女守护请求使用形象
		MT_XN_SHOUHU_INFO_REQ_CS = 6802,								// 仙女守护请求守护信息

		MT_XN_SHOUHU_INFO_SC = 6825,									// 仙女守护信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_JL_GUANGHUAN_UPSTAR_CS = 6850,								// 精灵光环请求升星
		MT_JL_GUANGHUAN_USE_IMAGE_CS = 6851,							// 精灵光环请求使用形象
		MT_JL_GUANGHUAN_INFO_REQ_CS = 6852,								// 精灵光环请求信息
		MT_JL_GUANGHUAN_SPECIAL_IMG_UPGRADE_CS =6853,					// 精灵光环特殊形象进阶

		MT_JL_GUANGHUAN_INFO_SC = 6875,									// 精灵光环信息

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FAIRY_BUDDHA_CARD_REQ_CS = 6855,                  // 仙尊卡激活请求
		MT_FAIRY_BUDDHA_CARD_INFO_SC = 6856,                 // 仙尊卡信息
		MT_FAIRY_BUDDHA_CARD_GOLD_REQ_CS = 6857,             // 领取绑元请求

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_GUILD_QUESTION_ENTER_REQ_CS = 6860,					// 帮派答题 进入请求
		MT_GUILD_QUESTION_PLAYER_INFO_SC = 6861,				// 帮派答题 玩家答题信息
		MT_GUILD_QUESTION_GUILD_RANK_INFO_SC = 6862,			// 帮派答题 帮派积分排行
		MT_GUILD_QUESTION_QUESTION_INFO_SC = 6863,				// 帮派答题 题目信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_JL_FAZHEN_UPSTAR_CS = 6900,									// 精灵法阵请求升星
		MT_JL_FAZHEN_USE_IMAGE_CS = 6901,								// 精灵法阵请求使用形象
		MT_JL_FAZHEN_INFO_REQ_CS = 6902,								// 精灵法阵请求信息
		MT_JL_FAZHEN_SPECIAL_IMG_UPGRADE_CS =6903,						// 精灵法阵特殊形象进阶

		MT_JL_FAZHEN_INFO_SC = 6925,									// 精灵法阵信息
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ZHUANSHENG_OPERA_REQ_CS = 6950,								// 转生操作请求

		MT_ZHUANSHENG_ALL_INFO_SC = 6975,								// 转生所有信息
		MT_ZHUANSHENG_BACKPACK_INFO_SC = 6976,							// 转生背包信息
		MT_ZHUANSHENG_OTHER_INFO_SC = 6977,								// 转生其它信息
		MT_ZHUANSHENG_CHOU_RESULT_SC = 6978,							// 转生抽奖结果
		MT_ZHUANSHENG_COMBINE_RESULT_SC = 6979,							// 转生合成结果
		MT_ZHUANSHENG_XIUWEI_NOTICE_SC = 6980,							// 转生修为提示信息

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RUNE_SYSTEM_REQ_CS = 6981,									// 符文系统 操作请求
		MT_RUNE_SYSTEM_BAG_INFO_SC = 6982,								// 符文系统 背包信息
		MT_RUNE_SYSTEM_RUNE_GRID_INFO_SC = 6983,						// 符文系统 符文格子信息
		MT_RUNE_SYSTEM_RUNE_OTHER_INFO_SC = 6984,						// 符文系统 其他信息
		MT_RUNE_TOWER_FETCH_TIME_CS = 6985,								// 符文塔领取离线时间
		MT_RUNE_TOWER_INFO_SC = 6986,									// 符文塔 信息
		MT_RUNE_TOWER_FB_INFO_SC = 6987,								// 符文塔 副本信息
		MT_RUNE_TOWER_OFFLINE_INFO_SC = 6988,							// 符文塔离线挂机信息
		MT_RUNE_SYSTEM_DISPOSE_ONE_KEY_CS = 6989,						// 符文系统 一键分解
		MT_RUNE_SYSTEM_COMPOSE_INFO_SC = 6990,							// 符文系统 符文合成结果
		MT_RUNE_SYSTEM_AWAKEN_INFO_SC = 6991,							// 符文系统 符文觉醒结果
		MT_RUNE_SYSTEM_AUTO_FB_CS = 6992,								// 符文塔 扫荡
		MT_RUNE_SYSTEM_AUTO_REWARD_SC = 6993,							// 符文塔 扫荡奖励

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_TALENT_SKILL_TARGET_LIST = 7252,						// 天赋技能伤害列表
		MT_TALENT_OPERATE_CS = 7260,							// 天赋操作
		MT_TALENT_INFO_SC = 7261,								// 天赋信息

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_LITTLE_PET_OPERA_REQ_CS = 8001,								// 小宠物操作请求
		MT_LITTLE_PET_RENAME_CS = 8002,									// 小宠物更改名字cs

		MT_LITTLE_PET_ALL_INFO_SC = 8050,								// 小宠物全量信息 
		MT_LITTLE_PET_SINGLE_INFO_SC = 8051,							// 小宠物单个信息
		MT_LITTLE_PET_PEI_CHANGE_INFO_SC = 8052,						// 小宠物改变信息
		MT_LITTLE_PET_PEI_CHOUJIANG_INFO_SC = 8053,						// 小宠物抽奖信息
		MT_LITTLE_PET_NOTIFY_INFO_SC = 8054,							// 小宠物增量信息
		MT_LITTLE_PET_FRIEND_INFO_SC = 8055,							// 小宠物宠友信息
		MT_LITTLE_PET_USING_IMG_SC = 8056,								// 使用的小宠物形象
		MT_LITTLE_PET_FRIEND_PET_LIST_SC = 8057,						// 好友宠物列表
		MT_LITTLE_PET_INTERACT_LOG_SC = 8058,							// 互动记录
		MT_LITTLE_PET_RENAME_SC = 8059,									// 小宠物更改名字sc
		MT_LITTLE_PET_WALK_SC = 8060,									// 溜小宠物
		
		MT_BABY_TYPE_WALK_SC = 8061,									// 溜宝宝

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CHINESE_ZODIAC_ALL_INFO_SC = 8200,							// 星座系统所有信息
		MT_CHINESE_ZODIAC_EQUIP_INFO_SC = 8201,							// 星座系统装备信息
		MT_CHINESEZODIAC_PROMOTE_XINGHUN_LEVEL_CS = 8202,				// 提升星座星魂等级
		MT_CHINESEZODIAC_PROMOTE_EQUIP_LEVEL_CS = 8203,					// 提升星座装备等级
		MT_CHINESEZODIAC_PROMOTE_LEVEL_CS = 8204,						// 提升星座等级
		MT_CHINESEZODIAC_TIANXIANG_REQ_CS = 8205,						// 天象请求
		MT_CHINESEZODIAC_TIANXIANG_INFO_SC = 8206,						// 天象总信息
		MT_CHINESEZODIAC_TIANXIANG_BEAD_SC = 8207,						// 天象单个珠信息
		MT_CHINESEZODIAC_TIANXIANG_COMBIND_SC = 8208,					// 天象组合信息
		MT_CHINESEZODIAC_GUNGUN_LE_SC = 8209,							// 滚滚乐信息
		MT_CHINESEZODIAC_SINGLE_CHANGE_SC = 8210,						// 秘籍单个改变信息
		MT_CHINESEZODIAC_COMBINE_SUCC_SC = 8211,						// 秘籍合成成功
		MT_CHINESEZODIAC_XINGLING_INFO_SC = 8212,						// 星灵信息
		MT_CHINESEZODIAC_TAKE_OFF_EQUIP_CS = 8213,						// 卸下装备
		MT_CHINESEZODIAC_RECYCLE_CS = 8214,								// 生肖装备回收

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MO_JIE_ALL_INFO_SC = 8250,									// 魔戒所有信息

		MT_BIG_CHATFACE_UP_LEVEL_REQ_CS = 8275,							// 大表情升级请求
		MT_TUHAOJIN_UP_LEVEL_REQ_CS = 8276,								// 土豪金升级请求

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FUN_OPEN_MOUNT_INFO_SC = 8300,								// 功能开启副本-坐骑
		MT_FUN_OPEN_WING_INFO_SC = 8301,								// 功能开启副本-羽翼
		MT_FUN_OPEN_XIANNV_INFO_SC = 8302,								// 功能开启副本-女神

		MT_FUN_OPEN_STORY_STEP_CS = 8325,								// 功能开启副本-请求刷怪
		MT_FUN_OPEN_SET_OBJ_POS_CS = 8326,								// 功能开启副本-设置OBJ位置
		MT_FUN_OPEN_CUR_STEP_END = 8327,								// 功能开启副本-当前步骤结束

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FRIEND_GIFT_INFO_SC = 8350,									// 好友送礼-总信息
		MT_FRIEND_GIFT_SONG_GIFT_CS = 8351,								// 好友送礼-送礼
		MT_FRIEND_GIFT_ALL_INFO_GIFT_CS = 8352,							// 好友送礼-总信息请求
		MT_FRIEND_GIFT_SHOU_NOTICE_CS = 8353,							// 好友送礼-收到礼物的提示
		MT_FRIEND_HELI_NOTICE_SC = 8354,								// 好友贺礼-可以送贺礼给好友的通知
		MT_FRIEND_HELI_SEND_REQ_CS = 8355,								// 好友贺礼-请求发贺礼
		MT_FRIEND_HELI_SEND_SC = 8356,									// 好友贺礼-发贺礼
		MT_GIVE_ITEM_OPERA_CS = 8357,									// 赠送物品操作类型
		MT_GIVE_ITEM_REQ_CS = 8358,										// 请求赠送物品
		MT_GIVE_ITEM_RECORD_SC = 8359,									// 赠送物品记录

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FRIEND_TUODAN_ALL_INFO_CS = 8371,							// 我要脱单-请求所有信息
		MT_FRIEND_TUODAN_ALL_INFO_SC = 8372,							// 我要脱单-全部信息
		MT_FRIEND_TUODAN_OPERA_REQ_CS = 8373,							// 我要脱单-操作请求
		MT_FRIEND_TUODAN_SINGLE_INFO_SC = 8374,							// 我要脱单-单个信息

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_QINGYUAN_EQUIP_OPERATE_CS = 8392,							// 情缘升级等级
		MT_QINGYUAN_EQUIP_INFO_SC = 8394,								// 情缘装备信息
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FB_GUIDE_FINISH = 8400,										// 副本引导-完成
		MT_FB_GUIDE_CREATE_GATHER = 8401,								// 副本引导-创建采集物
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_RED_ENVELOPE_INFO = 8410,									// 红包好礼-发送信息
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_MARRY_ME_COUPLE_INFO_SC = 8416,							// 我们结婚吧CP列表信息

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_OPEN_SERVER_INVEST_INFO_SC = 8417,						// 开服投资信息

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CHONGZHI_7DAY_FETCH_REWARD_CS = 8420,						// 充值-七天返利领取奖励

		MT_SHENGE_SYSTEM_REQ_CS = 8421,									// 神格系统请求
		MT_SHENGE_SYSTEM_INFO_SC = 8422,								// 神格信息下发
		MT_SHENGE_SYSTEM_ZHANGKONG_INFO_SC = 8423,						// 神格掌控信息
		MT_SHENGE_SYSTEM_ZHANGKONG_SIGN_INFO_SC = 8424,					// 神格掌控单个格子信息

		MT_GOLD_TURNTABLE_OPERATE_REQ_CS = 8425,						// 元宝转盘 操作请求
		MT_GOLD_TURNTABLE_REWARD_SC = 8426,								// 元宝转盘 奖品信息
		MT_GOLD_TURNTABLE_INFO_SC = 8427,								// 元宝转盘 信息

		MT_GOLDEN_PIG_OPERATE_REQ_CS = 8428,							// 金猪召唤操作请求
		MT_GOLDEN_PIG_SENG_INFO_SC = 8429,								// 金猪召唤信息
		MT_GOLDEN_PIG_BOSS_STATE_SC = 8435,								// 金猪召唤boss状态

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_TUITU_FB_OPERA_REQ_CS = 8430,								// 推图副本请求
		MT_TUITU_FB_INFO_SC = 8431,										// 推图副本所有信息
		MT_TUITU_FB_RESULT_SC = 8432,									// 推图副本结果信息
		MT_TUITU_FB_SINGLE_INFO_SC = 8433,								// 推图副本单个关卡信息
		MT_TUITUI_FB_FETCH_RESULT_SC = 8434,							// 推图副本拿取星级奖励结果

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SHENGE_SYSTEM_SHENQU_XILIAN_ALL_INFO_SC = 8440,				// 神格神躯洗炼信息返回
		MT_SHENGE_SYSTEM_SHENQU_XILIAN_INFO_SC = 8441,					// 单个神格神躯洗炼信息返回

		MT_RUNE_SYSTEM_ZHULING_NOTIFY_INFO_SC = 8442,					// 符文注灵通知信息
		MT_RUNE_SYSTEM_ZHULING_ALL_INFO_SC = 8443,						// 符文注灵所有信息返回

		MT_SHEN_YIN_SCORE_INFO_SC = 8444,									// 神印积分信息
		MT_SHEN_YIN_BAG_INFO_SC = 8445,										// 神印虚拟背包信息
		MT_SHEN_YIN_SHOP_INFO_SC = 8446,									// 神印位信息
		MT_SHEN_YIN_LIEHUN_POOL_INFO_SC = 8447,								// 神印猎魂池信息

		MT_RA_PLANTING_TREE_RANK_INFO = 8448,							// 随机活动-趣味植树排行榜信息
		MT_RA_PLANTING_TREE_TREE_INFO = 8449,							// 随机活动-趣味植树一棵树信息
		MT_RA_PLANTING_TREE_MINI_MAP_INFO = 8450,						// 随机活动-趣味植树小地图信息
		MT_RA_FISHING_ALL_INFO_SC = 8451,								// 随机活动-钓鱼信息
		MT_RA_FISHING_STATUS_SC = 8452,									// 随机活动-钓鱼信息广播
		MT_RA_CRYSTAL_DALARAN_INFO_SC = 8453,							// 随机活动-达拉然水晶信息

		MT_ELEMENT_HEART_REQ_CS = 8454,									// 请求操作
		MT_ELEMENT_HEART_INFO_SC = 8455,								// 元素信息
		MT_ELEMENT_HEART_CHARM_INFO_SC = 8456,							// 元素之纹列表信息
		MT_ELEMENT_HEART_SINGLE_CHARM_INFO_SC = 8457,					// 单个元素之纹信息
		MT_ELEMENT_HEART_REWARD_INFO_SC = 8458,							// 抽奖信息
		MT_ELEMENT_HEART_PRODUCT_INFO_SC = 8459,						// 产出物品信息
		MT_ELEMENT_SHOP_INFO_SC = 8460,									// 元素之心商店信息
		MT_ELEMENT_XILIAN_SINGLE_INFO_SC = 8461,						// 元素之心洗练单个信息
		MT_ELEMENT_XILIAN_ALL_INFO_SC = 8462,							// 元素之心洗练全部信息
		MT_RA_CRAZY_REBATE_CHONGZHI_INFO = 8463,						// 狂返元宝充值信息
		MT_RA_LOVE_DAILY_CHONGZHI_INFO = 8464,							// 每日一爱充值信息

		MT_SHEN_YIN_ONE_KEY_RECYLE_CS = 8465,							// 神印系统请求批量回收操作

		MT_UPGRADE_RUSH_INFO_SC = 8466,									// 快速进阶

		MT_RA_CONSUM_GIFT_INFO_SC = 8467,                               // 随机活动--消费领奖
		MT_RA_CONSUM_GIFT_ROLL_REWARD_SC = 8469,                        // 随机活动--消费领奖摇奖奖励
		MT_RA_CRITICAL_STRIKE_INFO_SC = 8468,                           // 随机活动--暴击日
		MT_RA_LIMIT_BUY_INFO_SC = 8470,                                 // 随机活动--每日限购
		MT_RA_COLLECT_TREASURE_INFO_SC = 8471,                          // 随机活动--聚宝盆新
		MT_RA_COLLECT_TREASURE_RESULT_SC = 8472,                        // 随机活动--聚宝盆新  摇奖结果
		MT_RA_HAPPY_CUMUL_CHONGZHI_INFO_SC = 8473,                      // 随机活动--欢乐累充  
		MT_RA_JINJIE_RETURN_INFO_SC = 8474,                             // 随机活动--进阶返还
		MT_RA_LIMIT_TIME_REBATE_INFO_SC = 8475,                         // 随机活动--限时回馈
		MT_RA_TIME_LIMIT_GIFT_INFO_SC = 8476,                           // 随机活动--限时礼包
		MT_RA_CIRCUL_CHONGZHI_INFO_SC = 8477,                           // 随机活动--循环充值
		MT_RA_SHAKE_MONEY_INFO_SC = 8478,                               // 随机活动--疯狂摇钱树
		MT_RA_TIME_LIMIT_LUXURY_GIFT_INFO_SC = 8479,                    // 随机活动--限时豪礼 
		MT_RA_DOUBLE_CHONGZHI_INFO_SC = 8480,                           // 随机活动--普天同庆
		MT_RA_RMB_CHEST_SHOP_INFO_SC = 8481,                            // 随机活动--神秘商店
		MT_RA_CONSUME_GOLD_INFO_SC = 8482,                              // 随机活动--消费返礼 
		MT_RA_BUY_ONE_GET_ONE_FREE_INFO_SC = 8483,						// 随机活动--买一送一
		

		MT_RA_VERSION_TOTAL_CHARGE_INFO_SC = 8484,                      // 版本活动--累计充值
		MT_RA_VERSION_CONTINUE_CHARGE_INFO_SC = 8485,                   // 版本活动--连续充值
		MT_RA_HUANLE_YAOJIANG_TWO_INFO_SC = 8486,                       // 版本活动--欢乐摇奖2
		MT_RA_HUANLE_YAOJIANG_TWO_TAO_SC = 8487,                        // 版本活动--欢乐摇奖2
		MT_RA_OFFLINE_SINGLE_CHARGE_INFO_SC = 8488,                     // 版本活动--线下单笔充值
		MT_RA_EXTREME_CHALLENGE_INFO_SC = 8489,                         // 版本活动--极限挑战
		MT_RA_COLLECT_SECOND_INFO_SC = 8490,                            // 随机活动--集月饼活动

		MT_RA_TOTAL_CHARGE_5_INFO_SC = 8491,							// 版本活动--累计充值5 信息

		MT_RA_CHONGZHI_GIFT_INFO_CS = 8492,                             // 随机活动--你充我送

		MT_CSA_TOUZI_JIHUA_INFO_CS = 8493,                              // 合服活动--投资计划
		MT_CSA_FOUNDATION_INFO_CS = 8494,                               // 合服活动--成长基金 
		MT_CSA_EXPREFINE_INFO_CS = 8495,                                // 合服活动--经验炼制
		MT_CSA_GONGCHENGZHAN_INFO_CS = 8496,                            // 合服活动--攻城战
		MT_RA_TOTAL_CHARGE_5_INFO_CS = 8497,							// 版本活动--累计充值5 请求信息

		MT_RA_JINJIE_RETURN_INFO2_SC = 8498,                            // 随机活动--进阶返还2
		MT_RA_CRITICAL_STRIKE2_INFO_SC = 8499,                          // 随机活动--暴击日2

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FB_NEQ_INFO_SC = 8501,										// 新装备本基本信息
		MT_FB_NEQ_PASS_INFO_SC = 8502,									// 新装备本通关信息
		MT_FB_NEQ_ROLL_POOL_SC = 8503,									// 新装备本奖池
		MT_FB_NEQ_ROLL_INFO_SC = 8504,									// 新装备本翻牌信息

		MT_FB_NEQ_TAKEREWARD_CS = 8511,									// 新装备本请求领奖
		MT_FB_NEQ_EXCHANGE_CS = 8512,									// 新装备本请求兑换
		MT_FB_NEQ_BUY_TIMES_CS = 8513,									// 新装备本请求购买次数
		MT_FB_NEQ_AUTO_CS = 8514,										// 新装备本请求扫荡
		MT_FB_NEQ_INFO_CS = 8515,										// 新装备本请求关卡信息
		MT_FB_NEQ_ROLL_CS = 8516,										// 新装备本请求翻牌

		MT_GET_DROP_LOG_CS = 8521,										// 掉落日志请求
		MT_DROP_LOG_RET_SC = 8522,										// 掉落日志返回

		MT_BABY_BOSS_OPERATE_CS = 8525,									// 宝宝boss请求
		MT_BABY_BOSS_ROLE_INFO_SC = 8526,								// 宝宝boss角色信息
		MT_ALL_BABY_BOSS_INFO_SC = 8527,								// 宝宝boss所有信息
		MT_SINGLE_BABY_BOSS_INFO_SC = 8528,								// 宝宝boss信息

		MT_FB_ARMORDEFEND_ROLE_REQ_CS = 8535,							// 防具材料副本请求
		MT_FB_ARMORDEFEND_ROLE_INFO_SC = 8536,							// 防具材料副本角色信息
		MT_FB_ARMORDEFEND_RESULT_SC = 8537,								// 防具材料副本结果
		MT_FB_ARMORDEFEND_INFO_SC = 8538,								// 防具材料副本信息
		MT_FB_ARMORDEFEND_WARNING_SC = 8539,							// 防具材料副本警示
		MT_FB_ARMORDEFEND_PERFORM_SKILL_SC = 8540,						// 防具材料副本释放技能

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SHANGGU_BOSS_OPERATE_CS = 8545,                              // 上古boss请求
		MT_SHANGGU_BOSS_ALL_INFO_SC = 8546,							    // 上古boss所有信息
		MT_SHANGGU_BOSS_LAYER_INFO_SC = 8547,							// 上古boss单层信息
		MT_SHANGGU_BOSS_SCENE_INFO_SC = 8548,							// 上古boss场景内信息
		MT_SHANGGU_BOSS_SCENE_OTHER_INFO_SC = 8549,						// 上古boss场景内其它信息

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MESS_BATTLE_ROLE_INFO_SC = 8560,										// 乱斗战场人物信息
		MT_MESS_BATTLE_ENTER_REQ_CS = 8561,										// 乱斗战场进入请求
		MT_MESS_BATTLE_RANK_INFO_SC = 8562,										// 乱斗战场排行信息
		MT_MESS_BATTLE_REWARD_INFO_SC = 8563,									// 乱斗战场奖励信息
		MT_MESS_BATTLE_HURT_RANK_LIST_INFO_SC = 8564,							// 乱斗战场伤害排行
		MT_MESS_BATTLE_HURT_ALL_ROLE_SCORE_INFO_SC = 8565,						// 乱斗战场人员积分信息
		MT_MESS_BATTLE_TOTAL_RANK_SC = 8566,									// 乱斗战场总积分排行榜

		//////////////////////////////////////// 建塔魂器副本 //////////////////////////////////////////////////////////
		MT_BUILD_TOWER_OPERA_CS = 8570,									// 塔防本操作请求
		MT_BUILD_TOWER_SCENE_INFO_SC = 8571,							// 塔防本信息下发
		MT_BUILD_TOWER_BUY_TIMES_CS = 8572,								// 建塔本购买次数		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CROSS_LIEKUN_FB_SCENE_INFO_SC = 8575,								// 猎鲲地带 场景信息
		MT_CROSS_LIEKUN_FB_LIVE_FLAG_SC = 8576,									// 猎鲲地带 主boss存活标记信息
		MT_CROSS_LIEKUN_FB_BOSS_HURT_INFO = 8577,								// 猎鲲地带 boss伤害信息排行

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FB_TEAMTOWERDEFEND_INFO_SC = 8580,					// 组队守护副本：副本信息
		MT_FB_TEAMTOWERDEFEND_ATTR_TYPE_INFO_SC = 8581,			// 组队守护 属性信息
		MT_FB_TEAMTOWERDEFEND_SKILL_INFO_SC = 8582,				// 组队守护 技能信息
		MT_FB_TEAMTOWERDEFEND_OPREAT_REQ_CS = 8583,				// 组队守护 客户端请求
		MT_FB_TEAMTOWERDEFEND_ALL_ROLE_SC = 8584,				// 组队守护 队友信息
		MT_FB_TEAMTOWERDEFEND_RESULTS_SC = 8585,				// 组队守护 结束信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SHENQI_OPERA_REQ_CS = 8586,							// 神器请求
		MT_SHENQI_ALL_INFO_SC = 8587,							// 神器所有信息
		MT_SHENQI_SINGLE_INFO_SC = 8588,						// 神器单个信息
		MT_SHENQI_IMAGE_INFO_SC = 8589,							// 神器形象信息
		MT_SHENQI_DECOMPOSE_RESULT_SC = 8590,					// 神器分解结果

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_GREATE_SOLDIER_OPERA_CS = 8595,						// 名将操作请求
		MT_GREATE_SOLDIER_UPGREADE_EQUIP_CS = 8596,				// 名将升级装备

		MT_GREATE_SOLDIER_ITEM_INFO_SC = 8600,					// 名将信息
		MT_GREATE_SOLDIER_OTHER_SC = 8601,						// 名将其他信息
		MT_GREATE_SOLDIER_SLOT_SC = 8602,						// 名将将位信息
		MT_GREATE_SOLDIER_FETCH_REWARD_SC = 8603,				// 名将领取阶段奖励
		MT_GREATE_SOLDIER_GOAL_INFO_SC = 8604,					// 名将目标信息
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		MT_HUGUOZHILI_INFO_SC = 8610,							// 天神护体 - 信息
		MT_HUGUOZHILI_REQ_CS = 8611,							// 天神护体 - 请求
	
		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_WORLD_ACQUISITION_LOG_SC = 8612,						// 全服收购下发 - 信息
		MT_WORLD_ACQUISITION_LOG_REQ_CS = 8613,					// 全服收购添加 - 请求

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FIGHT_BACK_ROLE_LIST_SC = 8615,						// 下发反击列表玩家uid

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_LUCKY_LOG_ITEM_REQ_CS = 8620,						// 幸运儿数据请求
		MT_LUCKY_LOG_ITEM_RET_SC = 8621,						// 幸运儿数据返回

		MT_CROSS_GOAL_OPERA_REQ_CS = 8625,						// 跨服目标请求
		MT_CROSS_GOAL_INFO_SC = 8626,							// 跨服目标信息
		MT_CROSS_GOAL_GUILD_NOTIFY = 8627,						// 跨服目标仙盟信息提示
		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_TIANSHENHUTI_ALL_EQUIP_INFO_SC = 8628,				// 周末装备所有装备信息
		MT_TIANSHENHUTI_ROLL_RESULT_SC = 8629,					// 周末装备抽奖结果
		MT_TIANSHENHUTI_OPERA_REQ_RESULT_SC = 8630,				// 周末装备相关请求结果
		MT_TIANSHENHUTI_OPERA_REQ_CS = 8631,					// 周末装备相关操作请求
		MT_TIANSHENHUTI_SCORE_CHANGE_CS = 8632,					// 周末装备积分变动
		MT_TIANSHENHUTI_COMBINE_ONEKEY_RESULT_SC = 8633,		// 周末装备一键合成返回
		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_SHENGXINGZHULI_INFO_REQ_CS = 8700,							// 升星助力信息请求
		MT_SHENGXINGZHULI_INFO_ACK_SC = 8701,							// 升星助力信息回复

		MT_SHENGXINGZHULI_GET_REWARD_REQ_CS = 8702,						// 升星助力领奖请求
		MT_SHENGXINGZHULI_GET_REWARD_ACK_SC = 8703,						// 升星助力领奖回复

	    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SHENGQI_INFO_SC = 8704,												// 圣器信息
		MT_SHENGQI_REQ_CS = 8705,												// 圣器操作请求																		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SEAL_REQ_CS = 8706,													// 圣印系统 操作请求
		MT_SEAL_BACKPACK_INFO_SC = 8707,										// 圣印系统 背包信息
		MT_SEAL_SLOT_INFO_SC = 8708,											// 圣印系统 圣印槽信息
		MT_SEAL_BASE_INFO_SC = 8709,											// 圣印系统 圣印基础信息
		MT_SEAL_RECYCLE_REQ_CS = 8710,											// 圣印系统 分解

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_JINJIE_TALENT_INFO_REQ_CS = 8720,									// 请求天赋信息
		MT_JINJIE_TALENT_ALL_INFO_SC = 8721,									// 所有天赋信息
		MT_JINJIE_TALENT_UPDATE_GRID_SC = 8722,									// 更新天赋格
		MT_JINJIE_TALENT_CHOUJIANG_PAGE_INFO_SC = 8723,							// 天赋抽奖页面信息
		MT_TALENT_ATTENTION_SKILL_SC = 8724,									// 天神天赋关注技能ID

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_YAOSHI_INFO_SC = 8730,								// 腰饰信息
		MT_YAOSHI_APPE_CHANGE_SC = 8731,						// 腰饰外观改变
		MT_YAOSHI_OPERA_REQ_CS = 8732,							// 腰饰请求操作
		MT_UPGRADE_SYS_INFO_SC = 8733,							// 进阶系统信息
		MT_UPGRADE_SYS_APPE_CHANGE_SC = 8734,					// 进阶系统外观改变
		MT_UPGRADE_SYS_OPERA_REQ_CS = 8735,						// 进阶系统请求操作

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_TOUSHI_INFO_SC = 8740,								// 头饰信息
		MT_TOUSHI_APPE_CHANGE_SC = 8741,						// 头饰外观改变
		MT_TOUSHI_OPERA_REQ_CS = 8742,							// 头饰请求操作
		MT_RESERVE_8743 = 8743,									// 保留
		MT_RESERVE_8744 = 8744,									// 保留
		MT_RESERVE_8745 = 8745,									// 保留

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_QILINBI_INFO_SC = 8750,								// 麒麟臂信息
		MT_QILINBI_APPE_CHANGE_SC = 8751,						// 麒麟臂外观改变
		MT_QILINBI_OPERA_REQ_CS = 8752,							// 麒麟臂请求操作
		MT_RESERVE_8753 = 8753,									// 保留
		MT_RESERVE_8754 = 8754,									// 保留
		MT_RESERVE_8755 = 8755,									// 保留

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MASK_INFO_SC = 8760,									// 面具信息
		MT_MASK_APPE_CHANGE_SC = 8761,							// 面具外观改变
		MT_MASK_OPERA_REQ_CS = 8762,							// 面具请求操作
		MT_RESERVE_8763 = 8763,									// 保留
		MT_RESERVE_8764 = 8764,									// 保留
		MT_RESERVE_8765 = 8765,									// 保留

		MT_RED_COLLECT_INFO_SC = 8766,                          // 红装收集
		MT_RED_COLLECT_OTHER_INFO_SC = 8767,                    // 红装收集
		MT_ORANGE_COLLECT_INFO_SC = 8768,                       // 橙装收集
		MT_ORANGE_COLLECT_OTHER_INFO_SC = 8769,                 // 橙装收集

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ZHUANZHI_EQUIP_REQ_CS = 8770,							// 转职装备请求
		MT_ZHUANZHI_EQUIP_INFO_SC = 8771,							// 转职装备信息
		MT_ZHUANZHI_STONE_INFO_SC = 8772,							// 转职玉石信息
		MT_ZHUANZHI_SUIT_INFO_SC = 8773,							// 转职套装信息

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_BOSS_CARD_REQ_CS = 8775,							    // boss图鉴 请求
		MT_BOSS_CARD_INFO_SC = 8776,							// boss图鉴 信息返回

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FISHING_OPERA_REQ_CS = 8777,							// 钓鱼活动相关请求

		MT_FISHING_USER_INFO_SC = 8778,							// 钓鱼活动玩家信息
		MT_FISHING_CHECK_EVENT_RESULT_SC = 8779,				// 钓鱼获得检查事件结果
		MT_FISHING_GEAR_USE_RESULT_SC = 8780,					// 钓鱼活动法宝使用结果
		MT_FISHING_EVENT_BIG_FISH_SC = 8781,					// 钓鱼活动触发了事件传说中的大鱼
		MT_FISHING_TEAM_MEMBER_INFO_SC = 8782,					// 钓鱼活动队友信息
		MT_FISHING_FISH_INFO_SC = 8783,							// 钓鱼活动钓上的鱼信息
		MT_FISHING_RAND_USER_INFO_SC = 8784,					// 钓鱼活动随机角色信息
		MT_FISHING_SCORE_INFO_SC = 8785,						// 钓鱼活动积分信息
		MT_FISHING_STEAL_RESULT_SC = 8786,						// 钓鱼活动偷鱼结果
		MT_FISHING_GET_FISH_BROCAST = 8787,						// 钓鱼活动获得鱼广播
		MT_FISHING_SCORE_RANK = 8788,					    	// 钓鱼活动积分排行信息
		MT_FISHING_STAGE_SCORE_INFO = 8789,						// 钓鱼阶段积分信息
		MT_FISHING_NOTIFY_STATUS_CHANGE_SC = 8790,				// 钓鱼状态改变通知
		MT_FISHING_STEAL_INFO_SC = 8791,						// 钓鱼偷鱼信息
		MT_FISHING_CONFIRM_RESULT_SC = 8792,					// 钓鱼确认事件返回

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_NIGHT_FIGHT_ROLE_INFO_SC = 8795,						// 夜战王城人物信息
		MT_NIGHT_FIGHT_REQ_CS = 8796,							// 夜战王城操作请求
		MT_NIGHT_FIGHT_RANK_INFO_SC = 8797,						// 夜战王城排行信息
		MT_NIGHT_FIGHT_REWARD_INFO_SC = 8798,					// 夜战王城奖励信息
		MT_NIGHT_FIGHT_RED_SIDE_LIST_INFO_SC = 8799,			// 夜战王城红方obj_id列表信息
		MT_NIGHT_FIGHT_HURT_ALL_ROLE_SCORE_INFO_SC = 8800,		// 夜战王城人员积分信息
		MT_NIGHT_FIGHT_RANK_ROLE_POSI_INFO_SC = 8801,			// 夜战王城前几名坐标信息
		MT_NIGHT_FIGHT_HURT_BOSS_RANK_SC = 8802,				// 夜战王城boss排行信息
		MT_NIGHT_FIGHT_TOTAL_RANK_SC = 8803,					// 夜战王城总积分排行榜

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_IMP_GURAD_OPERA_REQ_CS = 8805,						// 请求小鬼守护操作
		MT_IMP_GUARD_INFO_SC = 8806,							// 小鬼守护信息返回
		MT_IMP_GUARD_APPE_CHANGE_SC = 8807,                     // 小鬼守护外观变换

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_HESHENLUOSHU_REQ_CS = 8808,											// 河神洛书请求
		MT_HESHENLUOSHU_ALL_INFO_SC = 8809,										// 河神洛书全部信息
		MT_HESHENLUOSHU_CHANGE_INFO_SC = 8810,									// 河神洛书改变信息
		
		//////////////////////////////////////////////////////////////////////////
		MT_IMP_GUARD_EXPIRE_TIME_SC = 8811,                     // 小鬼到期

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ROLE_ZHUANZHI_REQ_CS = 8812,                // 转职请求
		MT_ROLE_ZHUANZHI_INFO_SC = 8813,               // 转职信息返回
		MT_ROLE_ZHUANZHI_SKILL_TRIGGER_SC = 8814,      // 转职技能触发

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_PERFECT_LOVER_INFO_SC = 8815,                     // 随机活动-完美情人

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_QUANMIN_JINJIE_INFO_SC = 8816,                     // 全民进阶

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_UPGRADE_GROUP_INFO_SC = 8817,                     // 全民总动员

		MT_FUNE_TOWER_REWARD_INFO_SC = 8818,                    // 符文塔奖励信息

		MT_RA_LOGIN_GIFT_INFO_0_SC = 8819,						// 登录送礼信息

		MT_RA_EVERYDAY_NICE_GIFT_INFO_SC = 8820,				// 每日好礼
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_RA_ROLE_BIG_SMALL_GOAL_CS = 8821,                    // 角色大小目标请求
		MT_RA_ROLE_BIG_SMALL_GOAL_INFO_SC = 8822,               // 角色大小目标信息

		MT_BOSS_DEAD_SC = 8824,                                  // boss死亡通知同场景有输出的玩家
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FB_HOLIDAY_GUARD_ROLE_REQ_CS = 8825,							// 请求塔防操作
		MT_FB_HOLIDAY_GUARD_ROLE_INFO_SC = 8826,						// 塔防角色信息
		MT_FB_HOLIDAY_GUARD_INFO_SC = 8827,								// 日常副本：单个塔防副本信息
		MT_FB_HOLIDAY_GUARD_DROP_INFO_SC = 8828,						// 副本怪物掉落统计
		MT_FB_HOLIDAY_GUARD_RESULT_SC = 8829,							// 塔防副本通关结果
		MT_FB_HOLIDAY_GUARD_WARNING_SC = 8830,							// 塔防副本：生命塔挨打报警
		MT_RA_HOLIDAY_GUARD_KILL_RANK_INFO_SC = 8831,					// 击杀排行信息
		MT_RA_HOLIDAY_GUARD_INFO_CS = 8832,								// 节日守护请求协议下发
		MT_RA_HOLIDAY_GUARD_NPC_INFO = 8833,							// 节日守护npc信息下发

		MT_RA_CONSUM_GIFT_ROLL_REWARD_TEN_SC = 8834,                    // 随机活动--消费领奖摇奖奖励
		MT_RA_COMBINEBUY_BUCKET_INFO_SC = 8835,                         // 随机活动组合购买  
		MT_RA_COMBINEBUY_ITEM_INFO2_SC = 8836,                          // 随机活动组合购买  

		MT_RA_DAY_CHONGZHI_REQ_CS = 8837,                               // 天天返利
		MT_RA_DAY_CHONGZHI_INFO_SC = 8838,                              // 天天返利

		MT_XIANZUNKA_REQ_CS = 8839,                                     // 仙尊卡请求
		MT_XIANZUNKA_INFO_SC = 8840,                                    // 仙尊卡信息返回

		MT_ROLE_APPE_CHANGE_SC = 8841,                                  // 人物外观变化

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SPOUSE_HOME_OPERE_REQ_CS = 8842,							// 夫妻家园操作请求
		MT_SPOUSE_HOME_ALL_ROOM_INFO_SC = 8843,						// 所有房间信息
		MT_SPOUSE_HOME_FIREND_INFO_SC = 8844,								// 好友信息
		MT_SPOUSE_HOME_GUILD_MEMBER_INFO_SC = 8845,						// 盟友信息
		MT_SPOUSE_HOME_GUILD_SINGLE_ROOM_INFO_SC = 8846,				// 单个房间信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_EQUIP_BAPTIZE_REQ_CS = 8847,                               // 装备洗炼请求
		MT_EQUIP_BAPTIZE_INFO_SC = 8848,                               // 装备洗炼信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_SHENGE_SYSTEM_SUIT_INFO_SC = 8849,							//星辉套装信息
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MIZANG_BOSS_RELIVE_TIRE_SC = 8860,                          // 秘藏boss复活疲劳
		MT_MIZANG_BOSS_BOSS_INFO_CS = 8850,                            // 秘藏boss请求                      
		MT_MIZANG_BOSS_BOSS_INFO_SC = 8851,                            // 秘藏boss所有信息返回
		MT_MIZANG_BOSS_SCENE_INFO_SC = 8852,                           // 秘藏boss场景内
		MT_MIZANG_BOSS_PLAYER_INFO_SC = 8853,                          // 秘藏boss玩家信息

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_YOUMING_BOSS_RELIVE_TIRE_SC = 8855,                          // 幽冥boss复活疲劳
		MT_YOUMING_BOSS_BOSS_INFO_CS = 8856,                            // 幽冥boss请求                      
		MT_YOUMING_BOSS_BOSS_INFO_SC = 8857,                            // 幽冥boss所有信息返回
		MT_YOUMING_BOSS_SCENE_INFO_SC = 8858,                           // 幽冥boss场景内
		MT_YOUMING_BOSS_PLAYER_INFO_SC = 8859,                          // 幽冥boss玩家信息
			//8860  秘藏boss复活疲劳
		MT_RA_LUCKY_WISH_INFO_SC = 8861,								// 随机活动-幸运许愿所有信息返回
		MT_RA_LUCKY_WISH_OPERA_CS = 8862,								// 随机活动-幸运许愿请求
		MT_IMAGE_COMPETITION_INFO_SC = 8863,                            // 形象比拼活动
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_FETCH_DOUBLE_REWARD_CS = 8873,								// 请求购买双倍奖励 
		MT_FETCH_DOUBLE_REWARD_RESULT_SC = 8874,						// 购买双倍奖励结果返回 
		MT_RAND_GIFT_REWARD_INFO_SC = 8875,								// 随机礼包开出的物品信息

		MT_PERSON_BOSS_INFO_CS = 8876,                                  // 个人boss信息
		MT_PERSON_BOSS_INFO_SC = 8877,                                  // 个人boss信息

		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_UPGRADE_CARD_BUY_INFO_SC = 8878,						// 直升丹购买信息
		MT_UPGRADE_CARD_BUY_REQ_CS = 8879,						// 直升丹购买请求

		MT_PERSON_AREAMSG_INFO_CS = 8888,                               // 个人广播视野消息
		MT_PERSON_AREAMSG_INFO_SC = 8889,                               // 个人广播视野消息

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CROSS_RA_REQUEST_OPERA_CS = 8895,					// 跨服随机活动，操作请求
		MT_CROSS_RA_ACTIVITY_STATUS_SC = 8896,					// 跨服随机活动，活动状态通知
		MT_CROSS_RA_CHONGZHI_RANK_INFO_SC = 8897,				// 充值排行，玩家充值信息
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CLOUD_PURCHASE_TOTAL_INFO = 8900,					// 一元夺宝活动信息
		MT_CLOUD_PURCHASE_CONVERT_INFO = 8901,					// 一元夺宝兑换信息
		MT_CLOUD_PURCHASE_BUY_INFO = 8902,						// 一元夺宝购买记录
		MT_CLOUD_PURCHASE_SERVER_RECORE_INFO = 8903,			// 全服纪录
		MT_CLOUD_PURCHASE_USER_INFO_SC = 8904,					// 一元夺宝个人信息

		MT_CROSS_RA_CONSUME_RANK_INFO_SC = 8905,				// 消费排行，玩家消费信息

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_YUNYOU_BOSS_OPERA_CS = 8906,                          // 云游boss请求
		MT_YUNYOU_BOSS_INFO_SC = 8907,                           // 云游boss信息 
		MT_YUNYOU_BOSS_SCENE_INFO_SC = 8908,                     // 云游boss场景个数信息 

		MT_GODMAGIC_BOSS_INFO_REQ_CS = 8910,					//神魔boss	
		MT_GODMAGIC_BOSS_INFO_ACK_SC = 8911,					//
		MT_GODMAGIC_PLAYER_INFO_SC = 8912,						//
		MT_GODMAGIC_BOSS_SCENE_INFO_SC = 8913,					//
		MT_GODMAGIC_BOSS_KILL_RECORD = 8914,					//
		MT_GODMAGIC_BOSS_DROP_RECORD = 8915,					//

		MT_TODAY_THEME_REWARD_REQ_CS = 8916,						// 今日主题领奖请求
		MT_TODAY_THEME_REWARD_FLAG_INFO_SC = 8917,					// 今日主题标记信息

		MT_BAI_BEI_FAN_LI2_INFO_SC = 8918,								// 百倍返利2信息
		MT_BAI_BEI_FAN_LI2_BUY_CS = 8919,								// 百倍返利2购买物品

		MT_RA_OPEN_GAME_GIFT_SHOP2_INFO_SC = 8920,						// 礼包限购活动信息
		MT_RA_OPEN_BUY_GAME_GIFT_SHOP2_INFO_CS = 8921,					// 限购礼包请求

		MT_ZERO_GIFT_GOD_COSTUME_INFO_SC = 8922,					// 零元礼包四阶神装信息

		MT_RA_LOVE_DAILY2_CHONGZHI_INFO = 8923,						// 每日一爱2

		MT_EQUIP_GRADE_SUIT_INFO_SC = 8924,							// 普通装备收集
		MT_SCENE_ACTIVITY_REWARD_SC = 8925,							// 活动奖励展示

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_BAIZHAN_EQUIP_REQ_CS = 8926,								// 百战装备请求
		MT_BAIZHAN_EQUIP_INFO_SC = 8927,							// 百战装备信息

		MT_ZHUANZHI_EQUIP_AWAKENING_ALL_INFO = 8928,				// 所有转职装备觉醒信息
		MT_ZHUANZHI_EQUIP_AWAKENING_INFO = 8929,					// 一件转职装备觉醒信息

		MT_GUILD_RARE_LOG_RQE_CS = 8930,							// 仙盟珍稀日志
		MT_GUILD_RARE_LOG_INFO_SC = 8931,							// 仙盟珍稀日志

		MT_LITTLE_HELPER_REQ_CS = 8932,								// 小助手请求

		MT_FIRST_RECHARGE_BUFF_FLAG_SC = 8933,						// 首充buff标记

		MT_TIANSHUXZ_INFO_SC=8935,									// 天数寻主信息
		MT_TIANSHUXZ_FETCH_REWARD_CS = 8936,						// 天数寻主获取奖励

		MT_EQUIP_GRADE_SUIT_ACTIVE_CS = 8937,						// 普通装备收集激活

		MT_EXP_BUFF_INFO_SC = 8938,									// 经验buff信息

		MT_LITTLE_HELPER_DROP_ITEM_INFO_SC = 8939,						// 小助手掉落物品信息

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CROSS_CHALLENGE_FIELD_REQ_CS = 8940,						// 跨服竞技场请求
		MT_CROSS_CHALLENGE_FIELD_STATUS_INFO_SC = 8941,				// 跨服竞技场副本状态信息
		MT_CROSS_CHALLENGE_FIELD_USER_INFO_SC = 8942,				// 跨服竞技场用户信息
		MT_CROSS_CHALLENGE_FIELD_OPPONENT_INFO_SC = 8943,			// 跨服竞技场对手详细信息
		MT_CROSS_CHALLENGE_BE_DEFEAT_NOTICE_SC = 8944,				// 跨服竞技场被打败通知
		MT_CROSS_CHALLENGE_FIELD_WIN = 8945,						// 跨服竞技场直接胜利
		MT_CROSS_CHALLENGE_FIELD_REPORT_INFO_SC = 8946,				// 跨服竞技场战报信息
		MT_CROSS_CHALLENGE_FIELD_RANK_INFO_SC = 8947,				// 跨服竞技场排名信息
		MT_CROSS_CHALLENGE_FIELD_OPPONENT_RANK_POS_CHANGE = 8948,	// 对手的排名发生变化

		MT_DISCOUNTSHOP_BUY_CS = 8950,								// 特惠商城购买
		MT_DISCOUNTSHOP_INFO_SC = 8951,								// 特惠商城信息

		MT_ZHUANZHI_EQUIP_COMPOSE_REQ_CS = 8952,					// 合成转职装备请求

		MT_LITTLE_HELPER_INFO_SC = 8953,							// 小助手信息

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_RA_LUCKYCLOUDBUYINFO_SC = 8954,								// 幸运云购基本信息
		MT_RA_LUCKYCLOUDBUYLIST_SC = 8955,								// 幸运云购购买记录
		MT_RA_LUCKYCLOUDBUYOPENINFO_SC = 8956,							// 幸运云购活动开启通知
		MT_ZHUANZHI_EQUIP_COMPOSE_SUCCEED_SC = 8957,					// 转职装备合成成功
		MT_LITTLE_HELPER_REPEAT_REQ_CS = 8958,							// 小助手重复请求

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_CRACY_BUY_ALL_INFO = 8959,								// 狂欢大乐购面板信息
		MT_CRACY_BUY_LIMIT_INFO = 8960,								// 限购信息

		MT_CROSSEQUIP_REQ_CS = 8961,								    // 斗气装备请求操作
		MT_CROSSEQUIP_ALL_INFO_SC = 8962,							    // 斗气装备所有信息下发
		MT_CROSSEQUIP_ROLL_RESULT_SC = 8963,							// 斗气装备抽奖返回信息
		MT_CROSS_EQUIP_BOSS_ONE_EQUIP_SC = 8964,						// 斗气装备单个装备
		MT_CROSS_EQUIP_BOSS_ALL_EQUIP_SC = 8965,						// 斗气装备所有装备
		MT_CROSS_EQUIP_BOSS_CHUANSHI_FRAG_CHANGE_SC = 8966,				// 传世碎片改变
		MT_CROSS_EQUIP_DOUQI_EXP_CHANGE_SC = 8967,						// 传世积分改变

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_MYTH_OPERA_REQ_CS = 8968,									// 神话系统操作请求
		MT_MYTH_CHPATER_INFO_SC = 8969,									// 神话篇章信息
		MT_MYTH_KNAPSACK_INTFO_SC = 8970,								// 神话背包信息
		MT_MYTH_CHPATER_SINGLE_INFO_SC = 8971,							// 神话篇章单个信息

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MT_ZODIAC_INFO_SC = 8972,												// 十二生肖信息
		MT_ZODIAC_BACKPACK_INFO_SC = 8973,										// 十二生肖背包信息
		MT_ZODIAC_REQ_CS = 8974,												// 十二生肖操作请求
		MT_ZODIAC_DECOMPOSE_REQ_CS = 8975,										// 十二生肖分解请求
		MT_ZODIAC_BASE_INFO_SC = 8976,											// 十二生肖精华信息
		
		MT_CROSS_HUSONG_SHUIJING_REQ_CS = 8979,							// 跨服护送水晶请求
		MT_CROSS_HUSONG_SHUIJING_INFO_SC = 8980,						// 跨服护送水晶信息
		MT_CROSS_HUSONG_SHUIJING_GATHER_INFO_SC = 8981,					// 跨服护送水晶水晶信息

		MT_RA_CHONGZHI_RANK2_INFO_SC = 8982,							// 随机活动充值信息

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		MT_AUTO_PROTOCOL_RESERVE_MIN = 11000,							// 自动生成协议占用协议段，旧式协议不要占用
		MT_AUTO_PROTOCOL_RESERVE_MAX = 13999,							// 自动生成协议占用协议段，旧式协议不要占用
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	};
}

#endif

