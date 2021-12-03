#ifndef __ERRORNUM_H__
#define __ERRORNUM_H__

namespace errornum
{

	enum ERROR_NUM_DEF
	{
		//////////////////////////////////////  背包类错误码    ////////////////////////////////////
		EN_KNAPSACK_USE_NOT_DIRECT = 100,				//该物品不能直接使用
		EN_KNAPSACK_IS_LOCK,							//背包已经锁住
		EN_KNAPSACK_USE_ITEM_ERROR,						//使用物品错误
		EN_KNAPSACK_USE_ITEM_NUM_NOT_ENOUGH,			//使用物品数量错误
		EN_KNAPSACK_LOAD_ERROR,							//读取物品错误
		EN_KNAPSACK_CANNOT_DISCARD,						//物品不能丢弃
		EN_KNAPSACK_MOVE_ERROR,							//物品移动错误
		EN_KNAPSACK_SPLIT_CANNOT,						//物品不能拆分
		EN_KNAPSACK_SPLIT_NUM_ERROR,	                //物品拆分数量错误
		EN_KNAPSACK_PUT_ERROR,							//物品放入错误
		EN_KNAPSACK_NO_SAPCE,							//背包空间不足
		EN_KNAPSACK_EQUIP_TAKEOFF_ERROR,				//装备脱下错误
		EN_KNAPSACK_USE_XUANYUN_STATE,					//眩晕状态不允许使用物品
		EN_KNAPSACK_STORAGE_EXTEND_MAX,					//背包仓库扩展已达最大值
		EN_ITEM_PROF_LIMIT,								//职业不符合
		EN_ITEM_LEVEL_LIMIT,							//当前等级不足
		EN_ITEM_CANNOT_MULTI_USE,						//该物品不能批量使用
		EN_CHESTSHOP_NOT_ENOUGH_TEMP_SPACE,				//仓库空间不足
		EN_ITEM_COLDDOWN,								//物品冷却中
		EN_KNAPSACK_USE_EVIEL_DECCARD,					//当前罪恶值为0，无需使用
		EN_ITEM_USE_DAY_TEIMS_LIMIT,					//当日使用次数已用完
		EN_KNAPSACK_USE_SCENE_LIMIT,					//当前场景不允许使用该物品
		EN_ITEM_SEX_LIMIT,								//性别不符合
		EN_KNAPSACK_CANNOT_DISCARD_INLAY,				//有镶嵌宝石不能丢弃
		EN_KNAPSACK_USE_DEAD,							//已死亡无法使用物品
		EN_SUBLOCK_IS_LOCKED,							//二级锁未解开
		EN_SUBLOCK_ERROR_TIMES_LIMIT,					//已到达二级锁每天允许尝试密码次数
		EN_SUBLOCK_PASSWORD_ILLEGAL,					//二级锁密码非法
		EN_ITEM_TIME_NO_IN_STORAGE,						//时效性物品不可放入仓库
		EN_STOP_FLY_AREA_LIMIT,							//不可走区域不能停止飞行
		EN_KNAPSACK_STORAGE_NO_SPACE,					//背包仓库空间不足

		//////////////////////////////////////  掉落相关    ////////////////////////////////////
		EN_FALLING_NOT_YOURS = 200,						//该物品不是您的
		EN_FALLING_TOO_FAR,								//拾取物品的距离过远
		EN_FALLING_ROLE_DEAD,							//死亡状态不允许拾取物品

		//////////////////////////////////////  任务 && npc 相关    ////////////////////////////////////
		EN_TASK_MAX_NUM = 400,							//已接任务达到最大数量
		EN_TOO_FAR_FROM_NPC,							//距离npc太远
		EN_TASK_ACCEPT_PRE_TASK,						//必须先完成前置任务
		EN_TASK_ACCEPT_LEVEL,							//等级不足，请努力升级！
		EN_TASK_ACCEPT_MAX_LEVEL,						//等级过高无法接受该任务
		EN_TASK_ACCEPT_FLY_MODE_LIMIT,					//飞行状态下无法接此任务
		EN_TASK_ACCEPT_JINGHUA_LIMIT,					//已在精华护送中不能再接此任务
		EN_TUMO_TASK_ACCEPT_NO_COUNT,					//今天日常任务接取次数已满
		EN_TUMO_TASK_NO_TASK,							//您没有可提交的日常任务
		EN_TUMO_TASK_NO_COMPLETE,						//日常任务未完成
		EN_HUSONG_TASK_COLOR_TYPE_SATISFY,				//护送任务的颜色已经满足要求
		EN_HUSONG_TASK_COLOR_IS_RED,					//护送任务已经是红色
		EN_HUSONG_TASK_ACCEPT_NO_COUNT,					//已经完成全部护送任务
		EN_HUSONG_TASK_ACCEPT_FLY_LIMIT,				//飞行时不能接护送任务
		EN_HUSONG_TASK_VIP_BUY_LIMIT,					//你当前VIP等级，可购买的次数已达上限！
		EN_HUSONG_TASK_REFRESH_COLOR_FAIL,				//刷新失败，品质不变
		EN_HUSONG_TASK_SCENE_KEY_LIMIT,					//护送任务只能在1线接取
		EN_GUILD_TERRITORY_EXTRA_LIMIT,					//您不是会长，不能领取额外奖励
		EN_GUILD_TERRITORY_REWARD_LIMIT,				//您已领取过该奖励
		EN_GUILD_TERRITORY_REWARD_DAY_BANGGONG_LIMIT,	//每日帮贡不足
		EN_TUMO_PRETASK_UNCOMPLETE,						//日常任务前置任务未完成
		EN_TUMO_ALREADY_HAS_TASK,						//您已经接去过日常任务
		EN_HUSONG_TASK_CAN_BUY_LIMIT,					//购买额外护送次数，可获得丰厚奖励
		EN_GUILD_JOIN_GUILD_LIMIT,						//您不满足加入仙盟的条件
		EN_PAOHUAN_TASK_ACCEPT_NO_COUNT,				//今天跑环任务接取次数已满
		EN_PAOHUAN_ALREADY_HAS_TASK,					//你身上已经有跑环任务
		EN_PAOHUAN_TASK_NO_TASK,						//您没有可提交的跑环任务
		EN_TASK_APPERAN_CHANGED_GATHER,					//请先完成采集任务
		EN_TASK_APPERAN_CHANGED_IMAGE,					//请先完成变身任务
		EN_TASK_APPERAN_CHANGED_FLY,					//请先完成飞行任务

		//////////////////////////////////////  物品/装备功能 相关    ////////////////////////////////////
		EN_FUNC_ITEM_LOAD = 500,						//物品读取错误
		EN_FUNC_STUFF_NOT_ENOUGH,						//材料物品不足

		EN_ITEM_FUNC_ITEM_COMPOSE_CANNOT,				//物品不可合成
		EN_ITEM_FUNC_ITEM_COMPOSE_NOT_ENOUGH,			//所需材料不足

		EN_EQUIP_FUNC_CONFIG_ERROR,						//装备配置错误
		EN_EQUIP_FUNC_INLAY_HOLE_NOT_EMPTY,				//该孔已经镶嵌宝石了
		EN_EQUIP_FUNC_INLAY_EQUIP_LEVEL,				//装备等级不够
		EN_EQUIP_FUNC_QUALITY_NOT_ENOUGH,				//装备阶数不够等级
		EN_EQUIP_FUNC_INLAY_HAVE_STONE_IN_SLOT,			//该槽已装备宝石
		EN_EQUIP_FUNC_CANNOT_STRENGTHEN,				//该装备不能强化
		EN_EQUIP_FUNC_STRENGTHEN_REACH_LEVELBOUND,		//已经强化到最高等级了
		EN_ITEM_FUNC_ITEM_UPLEVEL_NOT_ENOUGH,			//装备升级所需材料不足
		EN_EQUIP_FUNC_CANNOT_UPLEVEL,					//该装备不能升级
		EN_ITEM_FUNC_ITEM_INHERIT_NEED_SAME_TYPE,		//装备传承需要同种类型的装备
		EN_EQUIP_FUNC_CANNOT_SHENZHU,					//装备无法神铸
		EN_EQUIP_NOT_EXIST,								//装备不存在
		EN_EQUIP_CANNOT_FULING,							//装备无法龙噬
		EN_EFFECT_NOT_ACTIVE_TO_SAVE,					//特效未激活，保存失败
		EN_STRENGTH_STUFF_NOT_ENOUGH,					//强化石不足
		EN_EQUIP_FUNC_CANNOT_UPSTAR,					//装备无法加星
		EN_EQUIP_SUIT_CANNOT_UP_TO_SUIT,				//该装备不能升级为套装装备
		EN_EQUIP_SUIT_UPTO_MAX_SUIT_LEVEL,				//该装备已经升级到最高属性套装
		EN_EQUIP_BAY_STONE_FAIL,						//升级失败，此类宝石不能在商店购买
		EN_EQUIP_FUNC_ETERNITY_EQUIP_LEVEL_LIMIT,		//装备等级不足
		EN_EQUIP_FUNC_ETERNITY_LEVEL_MAX,				//已达到最大等级
		EN_EQUIP_FUNC_CANNOT_UPQUALITY,					//装备无法提升品质
		EN_EQUIP_FUNC_LEVELUP_IS_MAX_LEVEL,				//已经生到最高等级
		EN_EQUIP_FUNC_LEVELUP_ODDS_FAILD,				//升级概率失败
		EN_EQUIP_FUNC_LEVELUP_SUCC,						//升级成功


		//////////////////////////////////////  好友、黑名单 相关    ////////////////////////////////////
		EN_FRIEND_NUM_MAX = 600,						//好友人数已达到上限
		EN_FRIEND_EXIST,								//对方已是您的好友
		EN_ROLE_NOT_ONLINE,								//对方不在线
		EN_FRIEND_OTHER_REJECT_REQ,						//对方拒绝了您的好友请求
		EN_FRIEND_OTHER_FULL,							//对方好友已满
		EN_FRIEND_BLESS_NOT_FRIEND,						//对方不是您的好友
		EN_FRIEND_BLESS_THANK_BLESS_TIMES_ZERO,			//回礼次数为零，不能继续回礼了
		EN_FRIEND_REMOVE_LOVER_LIMIT,					//不能和夫妻解除好友关系
		EN_ADD_BLACK_LOVER_LIMIT,						//不能将爱人加入黑名单
		EN_IN_YOUR_BLACKLISTS,							//对方在您的黑名单中
		EN_BLACKLISTS_NUM_MAX,							//黑名单已达上限
		EN_YOU_ARE_IN_BLACKLISTS,						//您在对方的黑名单中
		EN_MAIL_FRIEND_LIMIT,							//只能发邮件给好友

		//////////////////////////////////////   邮件系统 相关    ////////////////////////////////////
		EN_HAS_NO_SPACING = 650,						//背包空间不足
		EN_ITEM_IS_NOT_EXIST,							//附件物品不存在

		//////////////////////////////////////   拍卖出售 相关    ////////////////////////////////////
		EN_PUBLICSALE_ADD_ITEM_FAIL = 700,				//出售物品出错
		EN_PUBLICSALE_ADD_ITEM_FULL,					//出售的物品数量已达上限
		EN_PUBLICSALE_REMOVE_ITEM_NOT_EXIST,			//该物品已售出
		EN_PUBLICSALE_NOT_HAVE_ITEM,					//没有出售的物品
		EN_PUBLICSALE_NOT_ENOUGH_ITEM,					//出售的物品数量不足
		EN_PUBLICSALE_BUY_ITEM_NOT_EXIST,				//物品不存在
		EN_PUBLICSALE_ITEM_CANNOT_SALE,					//物品不能出售
		EN_PUBLICSALE_EQUIPMENT_CANNOT_SALE,			//装备不能出售
		EN_PUBLICSALE_SYSTEM_ERROR,						//出售系统繁忙，请稍后重试
		EN_PUBLICSALE_ROLE_LEVEL_LIMIT,					//角色等级40级才可进行出售

		//////////////////////////////////////   场景副本 相关    ////////////////////////////////////
		EN_FB_REST_MONSTER_NUM_LIMIT = 800,				//场景还有剩余怪物
		EN_FB_RANDOM_TRANSMIT_FAIL,						//场景随机传送失败
		EN_FB_IN_CD,									//副本正在冷却中
		EN_SCENE_TRANSPORT_LEVEL_LIMIT,					//您的等级不足，不能进入该场景
		EN_SCENE_TRANSPORT_WEARY_LIMIT,					//疲劳值不足
		EN_SCENE_TRANSPORT_ANGRY_LIMIT,					//愤怒值已满
		EN_FB_ENTER_HOSONG_LIMIT,						//正在执行护送任务
		EN_FB_ENTER_FB_ERROR,							//进入副本错误
		EN_SCENE_TRANSPORT_TOO_FAR,						//距离传送点太远
		EN_GATHER_TOO_FAR,								//采集点太远
		EN_STATUS_CAN_NOT_GATHER,						//当前状态不能采集
		EN_SCENE_FORBID_PK,								//该场景不允许战斗
		EN_SAVEAREA_FIGHT,								//安全区内不能攻击
		EN_SAVEAREA_SET_LANDMINE,						//安全区内不能放置地雷
		EN_SAVEAREA_FIGHT_TARGET,						//不能攻击安全区内的玩家
		EN_FB_DAY_COUNT_LIMIT,							//今日进入次数已经用完
		EN_TRANSMIT_LIMIT,								//不允许传送
		EN_FB_NOT_TEAM_LEADER,							//必须由队长申请进入副本
		EN_FB_TEAM_MEMBER_TOO_FEW,						//队伍人数不够
		EN_FB_TEAM_MEMBER_TOO_FAR,						//有队员不在附近 不能进入组队副本
		EN_FB_TEAM_LEVEL_REQUIRE,						//队伍所有成员等级必须达到要求
		EN_FB_TEAMMATE_CANOT_TRAINSMIT,					//队伍中存在队员处于不能传送状态
		EN_FB_TEAM_DAY_COUNT_LIMIT,						//队伍所有成员必须不超过副本每日次数限制
		EN_FB_TEAMMATE_CANOT_ENTER_FB,					//队伍中存在队员处于不能进入副本状态
		EN_FB_NOT_IN_TEAM,								//没有组队不能进入组队副本
		EN_FB_PERSONAL_DENY_TEAM,						//单人副本不能组队进入
		EN_CANNOT_INVITE_HAS_PARTNER_ROLE,				//对方正在双修，无法被邀请
		EN_REST_OTHER_NOT_ONLINE,						//对方已经不在线
		EN_REST_OTHER_CAN_NOT_REST,						//对方不能双修
		EN_REST_ALREADY_HAS_PARNTER,					//对方已经找到双修仙女了
		EN_ROLE_ALREADY_HAS_REST_PARNTER,				//您已经有双修的仙女了
		EN_ROLE_SHUANGXIU_LEVEL_NOT_ENOUGH,				//等级不足 不能双修
		EN_ROLE_OTHER_SHUANGXIU_LEVEL_NOT_ENOUGH,		//对方等级不足 不能双修
		EN_NPC_FUNC_TRASMIT_RED_NAME,					//红名状态不能传送
		EN_CLIENT_POS_NOT_MATCH,						//客户端角色位置存在偏差
		EN_FB_ENTER_CAN_NOT_IN_FB,						//副本中不能进入其它副本
		EN_SPECIAL_SCENE_OPERATE_LIMIT,					//您在特殊场景中，不能执行此操作
		EN_ENTER_ACTIVITY_NOT_START,					//活动还未开始
		EN_ENTER_ACTIVITY_FB_LEVEL_LIMIT,				//等级不足,不能进入活动场景
		EN_QUESTION_ACTIVITY_QUESTION_DAY_COUNT_LIMIT,	//今日答题已经完成 
		EN_QUESTION_CAN_NOT_USE_BIANSHEN_CARD,			//该物品只能在答题副本使用
		EN_QUESTION_HAS_ANSER,							//你已经回答过了
		EN_QUESTION_USE_DATI_CARD_LIMIT,				//准备阶段不能使用免答卡
		EN_NOT_QUESTION_RANK,							//暂未有排名
		EN_QUESTION_TARGE_NOT_IN_CUR_SCENE,				//目标不在当前场景
		EN_QUESTION_OTHER_USE_BIANSHEN_CARD_ON_YOU,		//其他玩家对你使用了变身卡
		EN_FETCH_ZHUXIE_TASK_REWARD_FAIL,				//领取任务奖励失败
		EN_FETCH_TIANJIANGCAIBAO_REWARD_FAIL,			//天降财宝任务奖励失败
		EN_TIANJIANGCAIBAO_ENTER_LEVEL_LIMIT,			//等级不足不能进入
		EN_ZHUXIE_TASK_UNFINISH,						//诛邪任务未完成
		EN_ZHUXIE_NO_SUCH_TASK,							//您没有这个诛邪任务
		EN_NONEXIST_TO_BUY,								//您购买的物品不存在
		EN_FB_YOU_CANOT_ENTER_FB,						//您处于不能进入副本状态	
		EN_FB_NEQ_NOT_MAX_STAR,							//没有达到满星
		EN_FB_NEQ_BUY_TIMES_NOT_VIP,					//vip才能购买次数
		EN_FB_NEQ_BUY_TIMES_UP,							//你当前VIP等级，可购买的次数已达上限！
		EN_CANNOT_CHG_PEACE_MODE,						//当前场景不能切换和平模式
		EN_TOWER_DEFEND_NEXT_WAVE_REQ_NOT_TEAM_LEADER,	//只有队长才能请求下一波
		EN_TOWER_DEFEND_CURR_WAVE_NOT_DONE,				//刷新太过频繁，请稍后再试
		EN_TOWER_DEFEND_ONLY_LEADER_CHOOSE_TYPE,		//只有队长可以设置类型
		EN_TOWER_DEFEND_MEMBER_TOO_FAR_TO_CHOOSE_TYPE,	//队友不在附近，不能设置类型
		EN_TOWER_DEFEND_CHOOSE_TYPE_DUP,				//不能设置相同类型
		EN_TOWER_DEFEND_ATTRTYPE_ERR,					//加成类型错误
		EN_TOWER_DEFEND_BUILDING_ERR,					//建筑类型错误
		EN_TOWER_DEFEND_BUILDING_PRELEVEL_REQUIRED,		//需要先建造低等级建筑
		EN_TOWER_DEFEND_BUILDING_SCORE_LESS,			//所需要的积分不足
		EN_TOWER_DEFEND_BUILDING_CLEARWAVE_LESS,		//累计消灭的波数不足
		EN_TOWER_DEFEND_BUILDING_KILL_SUPERBOSS_LESS,	//击杀的超级boss数量不足
		EN_FB_PRE_LEVEL_NOT_FINISH,						//通关神魔之井后才能挑战仙魔之井！
		EN_ACTIVITY_NOT_ENROLLED,						//请先报名再参加活动
		EN_LEVEL_LESS_THAN_50_TO_JOIN,					//您的等级不足50级，请继续努力
		EN_TOO_FAR_FROM_SHENSHI,						//距离神石太远
		EN_FB_DAY_COUNT_NOT_ENOUGH,						//今日副本剩余次数不足
		EN_XIANMENGZHAN_NO_GUILD,						//您还没有加入仙盟
		EN_XIANMENGZHAN_ROLE_LEVEL_LIMIT,				//角色等级不足
		EN_XIANMENGZHAN_GUILD_LEVEL_LIMIT,				//仙盟等级不足
		EN_XIANMENGZHAN_GUILD_SIGNUP_COUNT_LIMIT,		//您仙盟报名的人数不足
		EN_GUILD_RELIVE_ALL_NOT_TIMES,					//仙盟总复活次数不足
		EN_GUILD_RELIVE_SELF_NOT_TIMES,					//仙盟个人复活次数不足
		EN_XIANMENGZHAN_NOT_PRIVILEGE,					//没有召唤权限
		EN_XIANMENGZHAN_CALL_COUNT_LIMIT,				//免费召唤次数已用完
		EN_XIANMENGZHAN_CALL_CD_LIMIT,					//召唤CD冷却中
		EN_XIANMENGZHAN_LEADER_NOT_EXIST,				//盟主已离开
		EN_TRANSMIT_LIMIT_IN_DUOQUZHENBAO,				//夺取灵石中不允许传送
		EN_XIANMENGZHAN_DANYAO_BOSS_EXIST,				//仙丹灵兽已经召唤出来了
		EN_XIANMENGZHAN_XIANLI_NOT_ENOUGH,				//仙力不足100，提交丹药可以获得仙力
		EN_XIANMENGZHAN_NO_DEFEND_AREA,					//需要占领五行据点后才能召唤龙兽
		EN_NATIONALBOSS_BUY_TIMES_UP,					//攻击力提升已达最大值
		EN_TRANSMIT_LIMIT_IN_MARRY,						//婚礼中不允许传送
		EN_TOWER_DEFEND_FROZEN_IN_CD,					//冰冻正在CD时间内
		EN_TOWER_DEFEND_FROZEN_COUNT_LIMIT,				//冰冻的使用次数已到上限
		EN_YAOSHOUGUANGCHANG_JOIN_LIMIT,				//当日参与妖兽广场次数已达上限
		EN_BUY_YAOSHOUGUANGCHANG_TIME_LIMIT,			//你当前VIP等级，可购买的次数已达上限！
		EN_YAOSHOUGUANGCHANG_NOT_STANDBY,				//妖兽广场活动当前不是准备状态，不允许进入！
		EN_SUOYAOTA_JOIN_LIMIT,							//当日参与锁妖塔次数已达上限
		EN_BUY_SUOYAOTA_TIME_LIMIT,						//你当前VIP等级，可购买的次数已达上限！
		EN_BUY_EXP_FB_TIME_LIMIT,						//已达到今日购买次数上限
		EN_BUY_EXP_FB_NO_TIME,							//已没有重置次数
		EN_BUY_STROY_FB_TIME_LIMIT,						//你当前VIP等级，可购买的次数已达上限！
		EN_BUY_PAHSE_FB_TIME_LIMIT,						//你当前VIP等级，可购买的次数已达上限！
		EN_SUOYAOTA_NOT_STANDBY,						//锁妖塔活动当前不是准备状态，不允许进入！
		EN_SUOYAOTA_THIS_TIME_ALREADY_JOIN,				//你已经放弃此次锁妖塔活动！
		EN_YAOSHOUGUANGCHANG_THIS_TIME_ALREADY_JOIN,	//你已经放弃此次妖兽广场活动！
		EN_MARRY_BLESS_DAYS_NOT_ENOUGH,					//蜜月祝福天数不足
		EN_MARRY_BLESS_ALREADY_FETCH,					//今天已经领取过了
		EN_MARRY_BLESS_DAYS_EQUAL,						//双方蜜月祝福天数相等不能购卖
		EN_GOLD_VIP_AUTH_LIMIT,							//您还不是黄金会员
		EN_CHARGE4_REWARD_LIMIT,                        //随机活动三重奏500奖励已发完
		EN_CONSUM_GIFT_ROLL_NO_TIMES,                   //没有摇奖次数
		EN_CONSUM_GIFT_ROOL_INSUFFICIENT_NUMBER,		//摇奖次数不足

		//////////////////////////////////////   角色 相关    ////////////////////////////////////
		EN_ROLE_NOT_EXIST = 1000,						//角色不在线
		EN_ROLE_NOT_IN_CACHE,							//角色不存在哦
		EN_ROLE_HAS_CHOOSE_PROF,						//您已经有职业了
		EN_ROLE_XINSHOU_PROTECT,						//新手保护 不允许发起攻击
		EN_ROLE_PROF_NOT_SUITABLE,						//职业不满足
		EN_VIP_LEVEL_NOT_ENOUGH,						//VIP等级不足
		EN_STATUS_CAN_NOT_REST,							//处于不允许打坐休息的状态
		EN_ROLE_REST_LEVEL_NOT_ENOUGH,					//打坐等级不满足
		EN_CAN_NOT_GATHER_IN_FLY,						//飞行时不能采集
		EN_ROLE_NAME_REPEAT,							//角色名已被使用
		EN_ROLE_IS_MARRY_CAN_NOT_CHANGE_SEX,			//您已结婚，不能改变性别
		EN_ROLE_REST_FORBID_IN_FIGHT,					//战斗状态不能打坐
		EN_ROLE_FIGHT_TARGET_NOT_ENEMY,					//攻击对象不是敌人
		EN_ROLE_FIGHT_CHENMO_STATUS,					//您正被沉默中
		EN_ROLE_FIGHT_DINGSHEN_STATUS,					//您正被定身中
		EN_ROLE_FIGHT_DISTANCE_NOT_ENOUGH,				//攻击距离不够
		EN_ROLE_FIGHT_CD_NOT_ENOUGH,					//冷却中
		EN_ROLE_FIGHT_GLOBALCD_NOT_ENOUGH,				//全局冷却中
		EN_ROLE_LEVEL_NOT_ENOUGH,						//角色等级不够
		EN_ROLE_TRANSMIT_FORBID_IN_FIGHT,				//战斗状态不能传送
		EN_ROLE_NUQI_NOT_ENOUGH,						//怒气不满
		EN_ROLE_ZHANYI_NOT_ENOUGH,						//战意不足
		EN_ROLE_HUNZHI_NOT_ENOUGH,						//魂值不足
		EN_SPEAKER_LEVEL_NOT_ENOUGH,					//角色200级之后开启喇叭功能
		EN_FIGHT_BIANXING_FOOL,							//您被变身了，不能攻击
		EN_LOVER_SCENE_CANNOT_TRANSMIT,					//伴侣所在场景不能传送
		EN_QUESTION_LEVEL_LIMIT,						//答题功能未开放
		EN_QUESTION_HAS_ANSWER_LIMIT,					//您已经回答过该题目了
		EN_QUESTION_TIME_LIMIT,							//回答时间未到，不允许作答
		EN_ROLE_STATE_CANNT_RECOVER,					//当前处于禁疗状态
		EN_TATGET_ROLE_MASSAGE_CD_NOT_ENOUGH,			//该玩家正在按摩中
		EN_ROLE_MASSAGE_CD_NOT_ENOUGH,					//正在按摩中
		EN_ROLE_FLY_PROTECT,							//飞行中 不允许发起攻击

		///////////////////////////////////  个人交易   //////////////////////////////////////////////////
		EN_ITEM_CANNOT_TRADE = 1100,					//该物品不能交易
		EN_ITEM_NUM_NOT_ENOUGH,							//物品数量不足

		///////////////////////////////////  魔戒   //////////////////////////////////////////////////
		EN_MOJIE_LEVEL_LIMIT = 1150,					//该魔戒等级已满
		EN_MOJIE_ROLE_LEVEL_LIMIT,						//角色等级不足
		EN_MOJIE_UP_LEVEL_STUFF_NOT_ENOUGH,				//升级材料不足

		//////////////////////////////////////   队伍 相关    ////////////////////////////////////
		EN_ALREADY_IN_TEAM = 1200,						//您已有队伍
		EN_TEAM_IS_FULL,								//队伍人数已达上限
		EN_USER_IS_NOT_EXIST,							//用户不存在
		EN_USER_IS_NOT_IS_YOUR_SCENE,					//被邀请者和您不在同个场景
		EN_HE_IS_IN_TEAM,								//对方已经有队伍了
		EN_HE_IS_REJUCT,								//对方拒绝您的邀请
		EN_TEAM_LEADER_IS_REJUCT,						//队伍拒绝您加入
		EN_TEAM_IS_NOT_EXIST,							//队伍不存在
		EN_YOU_ARE_NOT_IN_TEAM,							//您不在队伍中
		EN_YOU_HAVE_NO_POWER,							//您没有权限进行该操作
		EN_HE_IS_NOT_IN_YOUR_TEAM,						//对方不在您的队伍中
		EN_USER_IS_NOT_ONLINE,							//对方不在线
		EN_TEAM_JOIN_LIMIT,								//您的条件还未达标，再接再厉吧！
		EN_TEAM_CALL_TOGETHER_LIMIT_IN_FB,				//副本中不能召集队员
		EN_TEAM_CALL_IN_LIMIT_IN_FB,					//副本中不能招募队员
		EN_TEAM_SPECIAL_TEAM_JOIN_LIMIT,				//该队伍已经开始副本了
		EN_TEAM_TEAM_FB_OPERA_LIMIT,					//当前副本下不能离队，踢人或者解散队伍
		EN_TEAM_JOIN_OPERA_LIMIT_IN_TEAM_MATCHING,		//队伍正在匹配中，不可加入此队伍
		EN_TEAM_JOIN_OPERA_LIMIT_IN_ROLE_MATCHING,		//正在匹配中，不可加入队伍
		EN_USER_IS_NOT_IN_SAME_SERVER,					//对方与你不在同个服务器
		EN_USER_IS_NOT_IN_SAME_SERVER_SCENE,			//跨服中只能与同场景本服玩家组队
		EN_TEAM_TOO_MANY_MEMBERS,						//队伍人数过多，无法组队
		EN_TEAM_LEADER_NOT_FOUND,						//队长不存在
		EN_HE_IS_IN_SELF_TEAM,							//对方已经在您的队伍中
		EN_YOU_ARE_IN_HIS_TEAM,							//您已经在对方的队伍中
		EN_TEAM_JOIN_LIMIT_OF_MERGE,					//队伍中有成员不满足组队条件，不能合并队伍
		EN_ROLE_IS_IN_THIS_TEAM,						//已在该队伍中
		EN_YOU_HAVE_NO_POWER_MERGE,						//您没有权限进行队伍合并

		//////////////////////////////////////   仙盟 相关    ////////////////////////////////////
		EN_GUILD_SYSTEM_ERROR = 1300,					//仙盟系统错误
		EN_GUILD_CREATE_NO_ITEM,						//没有建团令
		EN_GUILD_NO_ATUO_JOIN_GUILD,					//没有加入符合的仙盟
		EN_GUILD_ATUO_JOIN_TIME_LIMIT,					//不能频繁申请，请稍后再尝试
		EN_GUILD_YOU_ALREADY_IN_GUILD,					//您已经加入仙盟
		EN_GUILD_NOT_ENOUGH_LEVEL,						//等级不足，请努力升级！
		EN_GUILD_INVILD_NAME,							//仙盟名称已存在
		EN_GUILD_YOU_NOT_IN_GUILD,						//您不在仙盟中
		EN_GUILD_YOU_NOT_HAVE_POWER,					//您没权力进行此操作
		EN_GUILD_NO_EXIST,								//仙盟不存在
		EN_GUILD_REPEAT_APPLY_FOR,						//您已经申请过了
		EN_GUILD_APPLY_FOR_FULL,						//该仙盟的申请列表已满
		EN_GUILD_NOT_IN_SAME_CAMP,						//没有在同一师门中
		EN_GUILD_ROLE_COUNT_LIMIT,						//仙盟人数已满
		EN_GUILD_OTHER_ALREADY_IN_GUILD,				//对方已加入仙盟
		EN_GUILD_APPLY_FOR_NOT_EXIST,					//申请已失效
		EN_GUILD_CANNOT_QUIT,							//不能退出仙盟
		EN_GUILD_INVITE_INVALID,						//邀请已失效
		EN_GUILD_OTHER_NOT_IN_YOUR_GUILD,				//对方不在您仙盟中
		EN_GUILD_CANNOT_LEAVE_POST,						//不允许卸任
		EN_GUILD_POST_IS_SAME,							//请选择不同职位进行任免
		EN_GUILD_POST_FULL,								//职位人数已满
		EN_GUILD_NOT_JOIN_GUILD,						//请先加入一个仙盟
		EN_GUILD_DELATE_FAIL,							//盟主健在，不可胡来，弹劾失败
		EN_GUILD_DELATE_NO_ITEM,						//没有弹劾令
		EN_GUILD_IS_REJUCT,								//仙盟拒绝您加入
		EN_GUILD_STORAGE_LOCKED,						//仙盟仓库已锁定
		EN_GUILD_STORAGE_NOT_HAVE_ITEM,					//没有放入仙盟仓库的物品
		EN_GUILD_STORAGE_PUTON_ITEM_NOT_ENOUGH,			//放入仙盟仓库的物品不足
		EN_GUILD_STORAGE_ITEM_CANNOT_SALE,				//绑定物品不能放入仙盟仓库
		EN_GUILD_STORAGE_PUT_ERROR,						//放入物品出错
		EN_GUILD_STORAGE_TAKE_ERROR,					//取出物品出错
		EN_GUILD_STORAGE_FULL,							//仙盟仓库已满
		EN_GUILD_STORAGE_NO_ITEM,						//仙盟仓库格子没物品
		EN_GUILD_STORAGE_ITEM_BE_TAKEN,					//该物品已被取走
		EN_GUILD_STORAGE_PUTOFF_ITEM_NOT_ENOUGH,		//取出仙盟仓库的物品不足
		EN_GUILD_STORAGE_DISCARD_NOPOWER,				//您没有权限销毁仓库物品
		EN_GUILD_STORAGE_ITEM_NOT_PUT,					//仙盟仓库不能放入该物品
		EN_GUILD_STORAGE_TAKE_SCORE_NOT_ENOUGH,			//仓库积分不足，无法取出物品
		EN_GUILD_YAOJIANG_DAYLIMIT,						//今日已无摇奖次数
		EN_GUILD_ALREADY_IN_GUILD_STATION,				//已经在仙盟驻地
		EN_GUILD_CAN_NOT_INVITE_FOR_CAMP_NOT_SAME,		//不同师门不可被邀请
		EN_GUILD_CAN_NOT_INVITE_TARGET_LEVEL_LIMIT,		//对方等级不足
		EN_GUILD_CAN_NOT_INVITE_FOR_MEMBER_FULL,		//仙盟人数已满 不可邀请
		EN_GUILD_YOU_NOT_JOIN_GUILD,					//尚未加入仙盟
		EN_GUILD_RESET_NAME_ERROR,						//仙盟名不可修改
		EN_GUILD_FORBIT_APPLYFOR,						//该仙盟禁止申请
		EN_GUILD_APPLYFOR_SETUP_ERROR,					//申请设置保存失败
		EN_GUILD_JIANZHULEVEL_MUST_LESSTHAN_GUILDLEVEL, //建筑等级不允许超过大厅等级
		EN_GUILD_NO_FULI_GIFT,							//当前没有可领取的福袋
		EN_GUILD_BLESSING_CD_LIMIT,						//请过一会再对对方进行祝福
		EN_GUILD_NO_BLESSING_COUNT,						//您已经没有祝福次数了
		EN_GUILD_TARGET_NO_BLESSING_COUNT,				//对方没有祝福次数了

		EN_GUILD_GCZ_LIMIT,								//攻城之战期间禁止此操作
		EN_GUILD_XMZ_LIMIT,								//仙盟战期间禁止此操作
		EN_GUILD_TW_LIMIT,								//领土战期间禁止此操作
		EN_GUILD_BATTLE_LIMIT,							//仙盟抢旗期间禁止此操作
		EN_GUILD_BOSS_LIMIT,							//仙盟Boss期间禁止此操作
		EN_GUILD_LIEKUN_ZHUMO_LIMIT,					//灵鲲之战或诛魔期间禁止此操作
		EN_CROSS_GUILD_BATTLE_LIMIT,					//连服夺城期间禁止此操作						
		EN_CROSS_LIEKUN_FB_GATHER_TOO_MUCH,				//这个宝箱您已经采集过了

		EN_GUILD_GUILD_BOSS_CALL_TOWER_DEFEND_LIMIT,	//仙盟守护中不能召唤神兽
		EN_GUILD_TOWER_DEFEND_SCENE_LIMIT,				//只能在仙盟驻地开启
		EN_GUILD_TOWER_DEFEND_GUILD_BOSS_LIMIT,			//召唤神兽过程中不能开启仙盟守护
		EN_GUILD_TOWER_DEFEND_DAYCOUNT_LIMIT,			//今日仙盟守护已开启
		EN_GUILD_TOWER_DEFEND_GUILD_LEVEL_LIMIT,		//仙盟要达到3级才能开启仙盟守护
		EN_GUILD_TOWER_DEFEND_MONSTER_NOT_FINISH,		//当前波次怪物出完才能提前
		EN_GUILD_UNION_BACK_STATION_FB,					//副本中不能进入驻地
		EN_GUILD_OPERATOR_TOO_FAST,						//操作过于频繁
		EN_GUILD_GET_REWARD_FAILED,						//领取奖励失败
		EN_GUILD_ALREADY_IN_GUILD_FB,					//已经在副本中
		EN_GUILD_ALREADY_JOIN_GUILD_FB,					//你今天已经参与过仙盟试炼
		EN_GUILD_FB_OUT_OF_TIME,						//尚未到活动开启时间
		EN_GUILD_UPLELVEL_TO_MAX,						//已达到最高等级，无法升级
		EN_GUILD_UPLELVEL_NOT_ENOUGH_EXP,				//仙盟财富不足，无法升级
		EN_GUILD_UPLELVEL_NOT_ENOUGH_STUFF,				//仙盟升级材料不足，无法升级
		EN_GUILD_UPLELVEL_NEED_UPLEVEL_OTHER,			//需要先升级其他建筑
		EN_GUILD_UPLELVEL_NEED_UPLEVEL_MAIN_HALL,		//需要先升级仙盟旗帜
		EN_GUILD_GET_BUFF_NOT_EXISTS,					//领取的仙盟buff不存在
		EN_GUILD_GET_BUFF_NOT_ENOUGH_GONG_XIAN,			//仙盟战功不足，无法领取
		EN_GUILD_EXCHANGE_NOT_ENOUGH_GONG_XIAN,			//仙盟战功不足，无法兑换
		EN_GUILD_LEVEL_LIMIT,							//仙盟等级不足
		EN_GUILD_OVER_EXCHANGE_TIMES,					//超过最大兑换次数，请明天再兑换
		EN_GUILD_FETCH_REWARD_BEFORE,					//您已领取过该奖励，无法再次领取
		EN_GUILD_BOSS_NOT_BEKILLED,						//仙盟boss未被击杀，不能领取
		EN_GUILD_NOT_KILL_PERE_BOSS,					//请先击杀上一个boss
		EN_GUILD_FB_LV_LIMIT,							//您的仙盟等级不足，无法开启试炼副本
		EN_GUILD_FB_OPEN_EXP_LIMIT,						//您的仙盟资金不足，无法开启试炼副本
		EN_GUILD_BONFIRE_OPEN_EXP_LIMIT,				//您的仙盟资金不足，无法开启仙女祝福活动
		EN_GUILD_BONFIRE_MOVE_MODE_LIMIT,				//飞行中不能开启仙女祝福活动
		EN_GUILD_CALL_IN_LEVEL_LIMIT,					//等级不足199无法进行招募
		EN_GUILD_BOX_NOT_START,							//尚未开始铸剑
		EN_GUILD_BOX_UPLEVEL_FAIL,						//剑炉升级失败
		EN_GUILD_BOX_UPLEVEL_VIP_COUNT_LIMIT,			//VIP等级不足，不能升级该剑炉
		EN_GUILD_BOX_UPLEVEL_TIME_LIMIT,				//离开仙盟24小时内不能升级剑炉
		EN_GUILD_BOX_OPENING,							//铸剑中
		EN_GUILD_BOX_REWARDED,							//剑炉已开启
		EN_GUILD_BOX_PRE_BOX_OPEN_LIMIT,				//上一个剑炉还没开启
		EN_GUILD_BOX_ASSIST_COUNT_LIMIT,				//剑炉协助开启已达最大次数
		EN_GUILD_BOX_OPEN_VIP_COUNT_LIMIT,				//VIP等级不足，不能铸剑
		EN_GUILD_BOX_OPEN_TIME_LIMIT,					//离开仙盟24小时内不能铸剑
		EN_GUILD_BOX_ASSIST_TIME_LIMIT,					//铸剑协助正在冷却中
		EN_GUILD_BOX_HAVE_ASSIST_LIMIT,					//成功协助该剑炉开启
		EN_GUILD_BOX_BE_ASSIST_TIMES_LIMIT,				//该剑炉协助次数已达上限
		EN_GUILD_BOX_CAN_OPEN,							//剑炉已经到时间可以打开
		EN_GUILD_BOX_ASSIST_NOT_HAVE,					//没有剑炉可协助
		EN_GUILD_BOSS_UPLEVEL_MAX,						//仙盟BOSS已升到最高级
		EN_GUILD_BOSS_NORMAL_CALL_COUNT_LIMIT,			//普通BOSS召唤已达最大次数
		EN_GUILD_BOSS_SUPER_CALL_LIMIT,					//必须先召唤普通BOSS才能召唤超级BOSS
		EN_GUILD_BOSS_SUPER_CALL_COUNT_LIMIT,			//超级BOSS召唤已达最大次数
		EN_GUILD_REWARD_TIME_LIMIT,						//离开仙盟24小时内不能领取奖励
		EN_GUILD_REWARD_FETCHED,						//您已领过该奖励
		EN_GUILD_BOSS_ACTIVITY_NOT_OPEN,				//仙盟BOSS时间未到
		EN_GUILD_BOSS_NOT_IN_STATION,					//召唤仙盟BOSS需要在仙盟驻地
		EN_GUILD_BOSS_CALL_AUTORITY_LIMIT,				//您的权限不足以召唤BOSS
		EN_GUILD_BOSS_BE_BEING,							//普通BOSS击杀后，才能召唤超级BOSS
		EN_GUILD_BOSS_CALLED,							//已召唤过仙盟BOSS，不可喂养
		EN_GUILD_UNION_BACK_STATION_BOSS,				//BOSS地图中不能进入驻地
		EN_GUILD_EXTEND_MEMBER_MAX,						//仙盟成员数量扩展已达最大值
		EN_GUILD_RED_PAPER_FETCH_LIMIT,					//红包数量不能超过仙盟人数上限
		EN_GUILD_RED_PAPER_CREATE_TIME_LIMIT,			//红包已过期，不能派发
		EN_COMMAND_SPEAKER_RED_PAPER_TIME_LIMIT,		//红包已过期，领取失败

		EN_WELF_SIGNIN_DAYLIMIT,						//签到达到每天次数限制
		EN_WELF_FILL_SIGNIN_LIMIT,						//没有可补签天数
		EN_GUILD_TOTAY_IS_SIGNIN,						//仙盟已签到
		EN_GUILD_CREATE_CD_TIME,						//创建仙盟需要间隔5分钟

		/////////////////////////////////////	 元神	///////////////////////////////////////////////
		EN_MENTALITY_PROTECT_ITEM_NOT_EXIST = 1500,		//根骨保护符不足
		EN_MENTALITY_UPGRADE_ITEM_NOT_ENOUGH,			//根骨丹不足
		EN_MENTALITY_UPGRADE_GENGU_FAIL,				//提升失败
		EN_MENTALITY_MENTALITY_LEVEL_ZERO,				//经脉对应的属性为0，不能提升根骨
		EN_MENTYALITY_LEVEL_LIMIT,						//提高角色等级才能继续提升经脉
		EN_MENTALITY_NO_NEED_CLEAR_CD,					//无需清理经脉CD
		EN_MENTALITY_WUXING_LEVEL_LIMIT,				//五行等级已经达到最大值
		EN_MENTALITY_WUXING_JINGMAI_LIMIT,				//经脉总等级不足
		EN_MENTALITY_NOT_NEED_PROTECT_ITEM,				//不需要保护符

		/////////////////////////////////////	 坐骑	///////////////////////////////////////////////
		EN_MOUNT_NOT_EXIST = 1550,						//坐骑未激活
		EN_MOUNT_CANNOT_UPGRADE,						//坐骑已达最大阶
		EN_MOUNT_UP_ONE_GRADE_LIMIT,					//坐骑阶级不满足
		EN_MOUNT_CANNOT_MOUNTON,						//您现在不能上坐骑
		EN_MOUNT_CANNOT_GOON_IN_FLY,					//您在飞行中不能上下坐骑
		EN_MOUNT_IMG_NOT_ACTIVE,						//形象未激活
		EN_MOUNT_TEMPORARY_IMAGE_TIME_LIMIT,			//您的体验坐骑已到期，通过坐骑进阶可获得更多形象！
		EN_MOUNT_PROF_NOT_ENOUGH,						//未达到转职要求

		/////////////////////////////////////	 战骑	///////////////////////////////////////////////
		EN_FIGHT_MOUNT_NOT_EXIST = 1570,				//战骑未激活
		EN_FIGHT_MOUNT_CANNOT_UPGRADE,					//战骑已达最大阶
		EN_FIGHT_MOUNT_UP_ONE_GRADE_LIMIT,				//战骑阶级不满足
		EN_FIGHT_MOUNT_CANNOT_GOON_IN_FLY,				//您在飞行中不能上下战骑
		EN_FIGHT_MOUNT_IMG_NOT_ACTIVE,					//形象未激活
		EN_FIGHT_MOUNT_PROF_NOT_ENOUGH,					//未达到转职要求

		/////////////////////////////////////	 羽翼	///////////////////////////////////////////////
		EN_WING_NOT_EXIST = 1590,						//羽翼未激活
		EN_WING_CANNOT_UPGRADE,							//羽翼已达最大阶
		EN_WING_UP_ONE_GRADE_LIMIT,						//羽翼阶级不满足
		EN_WING_IMG_NOT_ACTIVE,							//形象未激活
		EN_WING_TEMPORARY_IMAGE_TIME_LIMIT,				//您的体验羽翼已到期，通过羽翼进阶可获得更多形象！
		EN_WING_PROF_NOT_ENOUGH,						//未达到转职要求

		/////////////////////////////////////	 光环	///////////////////////////////////////////////
		EN_HALO_NOT_EXIST = 1600,						//光环未激活
		EN_HALO_CANNOT_UPGRADE,							//光环已达最大阶
		EN_HALO_UP_ONE_GRADE_LIMIT,						//光环阶级不满足
		EN_HALO_IMG_NOT_ACTIVE,							//形象未激活
		EN_HALO_SPECIAL_IMG_GRADE_LIMIT,				//特殊光环形象已满阶

		/////////////////////////////////////	 仙女光环	///////////////////////////////////////////////
		EN_SHENGONG_NOT_EXIST = 1610,					//仙女光环未激活
		EN_SHENGONG_CANNOT_UPGRADE,						//仙女光环已达最大阶
		EN_SHENGONG_UP_ONE_GRADE_LIMIT,					//仙女光环阶级不满足
		EN_SHENGONG_IMG_NOT_ACTIVE,						//形象未激活
		EN_SHENGONG_SPECIAL_IMG_GRADE_LIMIT,			//特殊仙女光环形象已满阶

		/////////////////////////////////////	 仙女法阵	///////////////////////////////////////////////
		EN_SHENYI_NOT_EXIST = 1620,						//仙女法阵未激活
		EN_SHENYI_CANNOT_UPGRADE,						//仙女法阵已达最大阶
		EN_SHENYI_UP_ONE_GRADE_LIMIT,					//仙女法阵阶级不满足
		EN_SHENYI_IMG_NOT_ACTIVE,						//形象未激活
		EN_SHENYI_SPECIAL_IMG_GRADE_LIMIT,				//特殊仙女法阵形象已满阶

		/////////////////////////////////////	 足迹	///////////////////////////////////////////////
		EN_FOOTPRINT_NOT_EXIST = 1630,					//足迹未激活
		EN_FOOTPRINT_CANNOT_UPGRADE,					//足迹已达最大阶
		EN_FOOTPRINT_UP_ONE_GRADE_LIMIT,				//足迹阶级不满足
		EN_FOOTPRINT_IMG_NOT_ACTIVE,					//形象未激活
		EN_FOOTPRINT_SPECIAL_IMG_GRADE_LIMIT,			//特殊足迹形象已满阶

		/////////////////////////////////////	 披风	///////////////////////////////////////////////
		EN_CLOAK_NOT_EXIST = 1640,						//披风未激活
		EN_CLOAK_IMG_NOT_ACTIVE,						//形象未激活
		EN_CLOAK_SPECIAL_IMG_GRADE_LIMIT,				//特殊披风形象已满阶
		
		/////////////////////////////////////	宠物	///////////////////////////////////////////////
		EN_JINGLING_NOT_EMPTY_GRID = 1730,				//宠物格子已满，请先取下其他宠物
		EN_JINGLING_SAME_TYPE,							//不能装备相同类型的宠物
		EN_JINGLING_MAX_LEVEL,							//已达到最高等级，无法升级
		EN_LINGJING_NOT_ENOUGH,							//灵晶不足，无法升级
		EN_JINGLING_CARD_MAX_LEVEL,						//已达到最大等级
		EN_JINGLING_ALREADY_ACTIVE,						//已经激活该形象
		EN_JINGLING_WUXING_LEVEL_LIMIT,					//宠物悟性已达最大等级
		EN_JINGLING_XIANZHEN_LEVEL_LIMIT,				//宠物仙阵已达最大等级
		EN_JINGLING_HUNYU_LEVEL_LIMIT,					//宠物魂玉已达最大等级
		EN_JINGLING_SKILL_PRE,							//请先学习前置技能
		EN_JINGLING_SKILL_STORAGE_FULL,					//宠物技能背包已满
		EN_JINGLING_SKILL_GRID_NOT_OPEN,				//该技能槽未开启
		EN_JINGLING_WUXING_UP_FAIL,						//提升悟性失败
		EN_JINGLING_SKILL_TYPE_LIMIT,					//该宠物已拥有同类技能
		EN_JINGLING_HOME_ROB_TIMES_NOT_ENOUGH,			//宠物家园掠夺次数不足
		EN_JINGLING_HOME_ROB_REPEAT_TOO_MORE,			//您今天已抢他太多次了，换个人抢吧
		EN_JINGLING_HOME_GET_TIME_LIMIT,				//领取时间未到
		EN_JINGLING_EXPLORE_TIMES_NOT_ENOUGH,			//挑战次数不足
		EN_JINGLING_EXPLORE_NOT_PASS_LEVEL,				//请先通关该关卡
		EN_JINGLING_ADVANTAGE_BOSS_LIMIT,				//今日奇遇BOSS奖励次数已满，击杀将不会获得奖励

		/////////////////////////////////////	声望	///////////////////////////////////////////////
		EN_SHENGWANG_XIANJIE_MAX_LEVEL = 1750,			//仙阶已达到最高等级，无法提升
		EN_SHENGWANG_XIANDAN_MAX_LEVEL,					//仙丹已达到最高等级，无法提升
		EN_SHENGWANG_XIANJIE_LIMIT,						//仙阶等级不足，无法提升

		/////////////////////////////////////	 成就	///////////////////////////////////////////////
		EN_CHENGJIU_NOT_OPENED = 1800,					//成就未触发
		EN_CHENGJIU_ALREADY_REWARD,						//该奖励已领取
		EN_CHENGJIU_NOT_COMPLETE,						//成就未完成
		EN_CHENGJIU_TITLE_MAX_LEVEL,					//称号已达到最高等级，无法提升
		EN_CHENGJIU_FUWEN_MAX_LEVEL,					//战魂已达到最高等级，无法提升
		EN_CHENGJIU_TITLE_LIMIT,						//成就称号等级不足，无法提升
		EN_CHENGJIU_CHENGJIU_NOT_ENOUGH,				//成就不足，无法提升

		/////////////////////////////////////	 其他	///////////////////////////////////////////////
		EN_OTHER_CANNOT_EVALUATE_SELF = 1900,			//不能评价自己
		EN_OTHER_NO_EVALUATE_NUM,						//您当日评价次数已用完
		EN_OTHER_ROLE_EVALUATEED,						//今日已点赞
		EN_TITLE_NO_EXIST,								//该称号未激活
		EN_NOT_TANSMIT_XUANYUN,							//眩晕状态不可传送
		EN_NOT_TANSMIT_DINGSHEN,						//定身状态不可传送
		EN_NOT_FIGHT_IN_FLY,							//飞行时不能发起攻击
		EN_NOT_TRANSMIT_FLY_MODE,						//飞行状态不可传送
		EN_NOT_FLY_BY_SHOE_SCENE_LIMIT,					//该场景无法传送
		EN_NOT_FLY_BY_SHOE_IN_FB,						//当前地图不能传送
		EN_FORBID_TALK,									//您已被禁言
		EN_TITLE_ALREADY_HAVE,							//您已经拥有了这个称号
		EN_TITLE_ALREADY_FULL,							//称号数量已满
		EN_GIVE_FLOWER_CAN_NOT_SELF,					//不能送花给自己
		EN_TARGET_NOT_ONLINE,							//对方不在线
		EN_FLOWER_ITEM_TYPE_ERROR,						//鲜花类型错误
		EN_BUY_FLOWER_ERROR,							//购买鲜花失败
		EN_ATTACK_MODE_NOT_ALLOW_IN,					//当前攻击模式不能进入
		EN_HANGUP_SCENE_VIP_LIMIT,						//只有VIP用户才能进入挂机地图
		EN_HANGUP_SCENE_LEVEL_LIMIT,					//您还没达到进入等级
		EN_CAN_NOT_ENTER_HANGUP_IN_FB_SCENE,			//在副本内不能进入挂机地图
		EN_OTHER_EVALUATE_LEVEL_LIMIT,					//50级以上可评价玩家
		EN_HAS_FETCH_THIS_REWARD,						//已经领取过该奖励
		EN_LOGIN_DAY_COUNT_DISSATISFY,					//登录天数不满足要求
		EN_REWARD_NOT_SATISFY,							//还没达成领取条件
		EN_MOUNT_BIANSHEN_TIME_LIMIT,					//变身已到期，升级后可体验更炫酷坐骑形象！
		EN_SPECIAL_IMG_ALREADY_ACTIVE,					//特殊形象已经激活
		EN_SHENBING_ALREADY_ACTIVE,						//神兵已经激活
		EN_SHENBING_STUFF_ERR,							//神兵激活材料错误
		EN_SHENBING_NOT_ACTIVE_TO_OP,					//需要先锻造出一件神兵后才能附体
		EN_NO_YAOJIANG_ITEM,							//你的暗黑轮盘不足，可通过副本获得！
		EN_ACTIVITY_FIND_REWARED,						//您今天已获得过该活动的奖励
		EN_NEQ_PRELEVEL_NOT_PASS,						//请先通过前一关
		EN_NEQ_TIMES_LIMIT,								//今日挑战次数已经用完
		EN_ACTIVITY_HAS_ENTER,							//今天您已经参与过该活动，不能立即领奖
		EN_ACTIVITY_ACTIVITY_NOT_START,					//活动尚未开启，不能委托
		EN_ENERGY_IS_FULL,								//您当前体力已满
		EN_BUY_ENERGY_TIMES_LIMIT,						//购买体力次数达到上限
		EN_NEQ_LEVEL_TIMES_LIMIT,						//荒野草原每天仅允许挑战一次
		EN_MOUNT_UPLEVEL_EQUIP_LEVEL_LIMIT,				//等级不足-坐骑
		EN_EQUIP_LEVEL_MAX,								//已达到最大等级
		EN_WING_UPLEVEL_EQUIP_LEVEL_LIMIT,				//等级不足-羽翼
		EN_BOSS_NOT_IN_BOSS_FAMILY,						//当前boss不在boss之家的
		EN_GATHAR_SELF_BONFIRE_LIMIT,					//一天只能向自己仙盟的仙女祈福5次！
		EN_GATHAR_BONFIRE_LIMIT,						//一天最多向仙女祈福10次！
		EN_GATHAR_GUILD_LIMIT,							//加入仙盟后，可得到仙女祝福！
		EN_GATHAR_ADD_MUCAI_LIMIT,						//加木材次数已达上限
		EN_EXPBOTTLE_TRANSFORM_LIMIT,					//你今天凝聚经验瓶已达上限！
		EN_EXPBOTTLE_USE_LIMIT,							//你今天使用经验瓶已达上限！
		EN_EXPBOTTLE_EXP_LIMIT,							//凝聚经验瓶所需的经验不足！
		EN_BOSS_DONGKU_WEARY_LIMIT,						//疲劳值为0，无法对boss造成伤害
		EN_NOT_FLY_BY_SHOE_BOSS_FAMILY_LIMIT,			//boss之家不允许直接传送！
		EN_NOT_FLY_BY_SHOE_DABAO_LIMIT,					//打宝地图不允许直接传送！
		EN_NOT_FLY_BY_SHOE_ACTIVE_LIMIT,				//活跃boss地图不允许直接传送！
		EN_NOT_FLY_BY_SHOE_BOSS_DONGKU_LIMIT,			//秘窟boss地图不允许直接传送！
		EN_NOT_FLY_BY_SHOE_CAMPDUOBAO_LIMIT,			//师门夺宝地图不允许直接传送！
		EN_ADD_MUCAI_SUCC,								//守护仙女已经收到你的心意，赐予你一份小礼物！
		EN_NOT_FAMILY_BOSS_BUY_WEARY_TIME_LIMIT,		//疲劳值购买次数已达上限
		EN_ADD_MUCAI_NOT_YOUR_GUILD,					//您不能对其他仙盟的篝火添加木材！
		EN_EQUIPMENT_FUMO_FAIL,							//请穿上对应装备再进行附魔
		EN_MILLIONAIRE_GATHER_LIMIT,					//采集次数已达上限
		EN_MILLIONAIRE_GATHER_LEVEL_LIMIT,				//等级不足不能采集宝箱
		EN_MILLIONAIRE_SKILL_TIME_LIMIT,				//技能使用次数不足
		EN_TITLE_HAS_ACTIVE,							//该称号已激活
		EN_TITLE_GRADE_LIMIT,							//称号进阶已达最大等级
		EN_CAN_NOT_ENTER_BOSS,							//该场景不能直接进入boss地图
		EN_MAGICAL_PRECIOUS_FETCHT_REWARD_LIMIT,		//已经领取过该奖励
		EN_MAGICAL_PRECIOUS_CAN_NOT_FETCHT_REWARD,		//未达到领取条件
		EN_MAGICAL_PRECIOUS_PRE_CHAPTER_NOT_FINISH,		//未完成上一章节所有任务，不能领取
		EN_SHANGGU_BOSS_ENTER_TIMES_FULL,               //今日进入次数已用完
		EN_BOSS_SHANGGU_WEARY_LIMIT,					//疲劳值过高，无法造成伤害
		EN_EXPENSE_NICE_GIFT_DAILY_LIMIT,	            //已达今日抽奖最大次数
		EN_NOT_FIGHT_IN_JUMP,							//飞行时不能发起攻击
		EN_NOT_FIGHT_IN_JUMP2,							//飞行时不能发起攻击

		/////////////////////////////////////	 商店 相关	///////////////////////////////////////////////
		EN_SHOP_HAS_BOUGHT_LIMIT_ITEM = 2000,			//您在本次限购已经购买过该物品 
		EN_SHOP_ITEM_NOT_EXIST,							//商店中不存在该物品
		NT_CONVERTSHOP_CONVERT_COUNT_LIMIT,				//今日兑换次数已达上限
		NT_CONVERTSHOP_LIFETIME_CONVERT_COUNT_LIMIT,	//兑换次数已达上限
		NT_CONVERTSHOP_LEVEL_LIMIT,						//等级不符，不能兑换
		EN_CONVERTSHOP_LINGZHI_NOT_ENOUGH,				//灵值不足
		EN_CONVERTSHOP_LIMIT_TIME_INVALID,				//该物品已过期，不可兑换
		NT_MYSTERIOUS_SHOP_BUY_LIMIT,					//购买次数已达上限
		EN_MYSTERIOUS_SHOP_CREDIT_NOT_ENOUGH,			//积分商城积分不足
		EN_PRECIOUS_BOSS_SCORE_NOT_ENOUGH,				//秘宝积分不足
		EN_BUY_TOO_MUCH_ONCE,                           //超过了最大购买量
		EN_SHENZHOUWENPON_SCORE_NOT_ENOUGH,				//异火积分不足

		/////////////////////////////////////	 活动	///////////////////////////////////////////////
		EN_LINGMAI_LINGMAI_DAYCOUNT_LIMIT = 2100,		//今日使用次数已满
		EN_LINGMAI_NOT_IN_WANGCHENG,					//只能在鼎盛王城区域使用王城灵脉相关物品
		EN_LINGZHU_ITEM_NOT_ENOUGH,						//没有地脉灵珠
		EN_ZHADAN_ITEM_NOT_ENOUGH,						//没有崩天玉
		EN_SOULINGFU_ITEM_NOT_ENOUGH,					//搜灵符不足，可通过活跃度奖励获得
		EN_LINGMAI_NOTHING_FOUND,						//这里没有喔，到别的地方碰碰运气吧。
		EN_QUNXIANLUANDOU_CONTROLLING_SHENSHI,			//您已经拥有神石
		EN_GONGCHENGZHAN_IN_CARRIER,					//您已经有木甲兽了
		EN_GONGCHENGZHAN_TOWER_TIME_LIMIT,				//请过一会再进入仙术之塔
		EN_GONGCHENGZHAN_TOWER_SIDE,					//攻城方不可使用仙术之塔
		EN_GONGCHENGZHAN_CARRIER_STAGE,					//攻击龙柱阶段不可召唤木甲兽
		EN_GONGCHENGZHAN_CARRIER_TOO_FAR,				//距离木甲兽太远
		EN_GONGCHENGZHAN_CHANGE_CAMP,					//不可使用其它师门木甲兽
		EN_GONGCHENGZHAN_REWARD_ERROR,					//您不能领取该奖励
		EN_GONGCHENGZHAN_REWARD_COUNT_LIMIT,			//您今日已经领过了该奖励
		EN_GONGCHENGZHAN_ITEM_BUY_MULTI,				//该器械不能批次购买
		EN_GONGCHENGZHAN_ITEM_BUY_LIMIT,				//当日剩余购买次数不足
		EN_GONGCHENGZHAN_NOT_EMPTYSLOT_INDEX,			//技能槽已满
		EN_BATTLE_FIELD_KILL_USER_INVALID,				//短时间内击杀相同敌人无奖励
		EN_LINGMAI_PLANT_OUT_OF_RANGE,					//不在放置区域内不可放置
		EN_ENTER_NOT_GUILD,								//没有加入仙盟
		EN_GONGCHENGZHAN_TOWER_TOO_FAR,					//距离防御塔太远
		EN_1VN_ALREADY_SIGN,							//您已经报名了
		EN_XIANMENGZHAN_INVALID_GUILD,					//没有加入仙盟不能参加仙盟战
		EN_RA_SERVER_NIUEGG_TIMES_LIMIT,				//全服扭蛋次数未达到要求
 		EN_RA_SERVER_NIUEGG_HAS_FATCHED,				//已经领取过该奖励
 		EN_RA_SERVER_NIUEGG_VIP_LIMIT,					//你的VIP等级不足，无法领取该奖励
 		EN_RA_SERVER_NIUEGG_LEFT_CHARGE_LIMIT,			//你的剩余总充值额度不足
		EN_RA_ZHENBAOGE_VIP_LIMIT,						//您的VIP等级不足，无法领取该奖励
		EN_RA_ZHENBAOGE_TIMES_NOT_ENOUGH,				//当前次数不足领取该礼包
		EN_RA_ZHENBAOGE_HAVE_FETCH,						//该奖励已经领取过了
		EN_RA_ZHENBAOGE_HAVE_ALL_BUY,					//已经全部购买了
		EN_RA_SERVER_MONEYTREE_TIMES_LIMIT,				//当前抽奖积分未达到要求
		EN_RA_SERVER_MONEYTREE_HAS_FATCHED,				//已经领取过该奖励
		EN_RA_SERVER_MONEYTREE_VIP_LIMIT,				//您的VIP等级不足，无法领取该奖励
		EN_RA_TIAN_MING_DIVINATION_MAX_ADD_LOT,			//该占卜瓶加注已满
		NT_WELFARE_HAPPY_TREE_VALUE_TOO_LARGE,			//今日此方式获得成长值已满
		NT_WELFARE_GROWTH_VAL_NOT_ENOUGTH,				//欢乐果树长成值不足
		NT_WELFARE_ALREADY_FETCH_HAPPYTREE_REWARD,		//已经领取了
		EN_ENTER_GUILD_NOT_HAV_QUALIFICATION,			//没有获得领土战资格
		EN_GUILD_BATTLE_HAS_USE_SHILD,					//你已经使用过护盾
		EN_TERRITORYWAR_BUY_CAR_LIMIT,					//当前已购买载具
		EN_TERRITORYWAR_BUY_FIRE_LANDMINE_LIMIT,		//爆裂地雷携带数量已达上限
		EN_TERRITORYWAR_BUY_ICE_LANDMINE_LIMIT,			//冰霜地雷携带数量已达上限
		EN_RA_TIAN_MING_DIVINATION_ALL_MAX_ADD_LOT,		//占卜瓶加注次数已用完
		EN_ACTIVITY_ENTER_TIME_OUT,                     //活动进入时间已过
		EN_RA_LIMIT_TIME_REBATE_CHONGZHI_DAY_LIMIT,		//充值天数不足

		/////////////////////////////////////	 福利	///////////////////////////////////////////////
		EN_WELFARE_ACCLOGIN_DAYS_NOT_ENOUGH = 2300,		//累计登陆天数不足
		EN_WELFARE_CHONGJIHAOLI_NOT_ENOUGH,				//奖励已经被领取完毕

		////////////////////////////////////  五谷丰收 //////////////////////////////////////////////
		EN_DAILYBUYCOIN_LEVEL_LIMIT = 2400,				//角色等级不足不能招财
		EN_DAILYBUYCOIN_LEFT_COUNT_NOT_ENOUGH,			//剩余招财次数不足

		////////////////////////////////////    保留    //////////////////////////////////////////////
		EN_RESERVE_2600 = 2600,							//保留2600

		/////////////////////////////////////	 结婚	///////////////////////////////////////////////	
		EN_MARRY_THIS_TIME_IS_ALREADY_RESERVE = 2700,	//该时间已经被预约
		EN_MARRY_HUNYAN_NOT_BE_INVITED,					//你沒有喜帖，可向新郎新娘索取喜帖。
		EN_MARRY_HUNYAN_CAN_NOT_OPERA_TIANMI_WUSHUANG,	//您的伴侣不在附近
		EN_MARRY_HUNYAN_ALREADY_BLESS,					//祝福次数已经用完
		EN_MARRY_NO_IN_TEAM,							//当前状态未组队
		EN_MARRY_TEAM_MEMBER_MUNBER_ERROR,				//当前组队人数必须为2人
		EN_MARRY_TARGET_MUST_IN_TEAM,					//队员不是正确
		EN_MARRY_HUNYAN_GATHER_THIS_FOOD,				//您已经吃过这桌食物了

		/////////////////////////////////////	 副本	///////////////////////////////////////////////	
		EN_TASK_NOT_COMPLETED = 2720,					//进入副本所需任务未完成
		EN_PRVE_FB_NOT_COMPLETED,						//请先完成上一个副本
		EN_FB_DAYTIMES_NOTENOUGH,						//进入副本次数已用完
		EN_FB_DAILYTIMES_BUY_TIMES_MAX,					//当日购买次数已用完
		EN_FB_ATUO_NOT_PASS_FB,							//需要通过此副本才能进行扫荡
		EN_FB_ROLE_LEVEL_LIMIT,							//等级不足，请努力升级！
		EN_FB_AUTO_PASS_TIME_LIMIT,						//通关时间未达到扫荡要求
		EN_FB_EXP_NOT_REWARD_TO_FETCH,					//没有可领取的奖励
		EN_FB_EXP_NOT_SWEEPING_CHECKPOINT,				//当前已无可扫荡关卡
		EN_FB_OPERATE_LIMIT,							//副本中不能执行此操作
		EN_FB_ATUO_NOT_IN_STATIC_SCENE,					//当前场景不能进行副本扫荡

		/////////////////////////////////////    生肖   ///////////////////////////////////////////////
		EN_CHINESE_ZODIAC_LEVEL_LIMIT,					//该生肖等级已满
		EN_CHINESE_ZODIAC_LEVEL_ZERO,					//生肖等级不足，不能提升装备
		EN_CHINESE_ZODIAC_XINGHUN_LEVEL_LIMIT,			//该生肖星魂等级已满
		EN_CHINESE_ZODIAC_XINGHUN_LEVEL_UPGRADE_FAIL,	//生肖星魂等级提升失败
		EN_CHINESE_ZODIAC_EQUIP_MAX_LEVEL,				//生肖装备已达最高级
		EN_CHINESE_ZODIAC_UP_LEVEL_LIMIT,				//上一个生肖等级不足，不能升级
		EN_CHINESE_ZODIAC_MIJI_PUT_FAIL,				//原来秘籍等级较大，放置失败
		EN_CHINESE_ZODIAC_LIMIT_LEVEL_LIMIT,			//该生肖未激活，不能穿戴装备
		EN_CHINESE_ZODIAC_MIJI_COMBINE_LEVEL_NOT_SAME,	//秘籍的等级不一致
		EN_CHINESE_ZODIAC_MIJI_NOT_SAME_TYPE,			//此类型的秘籍已经存在
		EN_CHINESE_ZODIAC_ITEM_NOT_MIJI,				//物品不是秘籍
		EN_CHINESE_LAST_ZODIAC_MIJI_NUM_LIMIT,			//上一个生肖秘籍数量不足
		EN_CHINESE_XINGLING_LEVEL_LIMIT,				//龙魂已经是最高等级
		EN_CHINESE_XINGLING_CHAPTER_LIMIT,				//对应的星图章节没有开启
		EN_CHINESE_XINGLING_UPLEVEL_FAIL,               //龙魂升级失败，祝福值增加
		EN_CHINESE_XINGTU_ACTIVITE_FAIL,				//没有空位
		EN_CHINESE_ZODIAC_EQUIP_ERROR,					//生肖装备错误
		EN_CHINESE_ZOIDAC_NOT_EQUIP,					//不是生肖装备

		/////////////////////////////////////	 其他2	///////////////////////////////////////////////	
		EN_XIANNV_ACTIVE_FIRST = 3100,					//请先激活一个仙女
		EN_JOIN_CAMP_FIRST,								//请先加入一个师门
		EN_QIFU_BUYCOIN_NO_TIMES_LEFT,					//铜币祈福次数已经用完
		EN_QIFU_BUYYUANLI_NO_TIMES_LEFT,				//真气祈福次数已经用完
		EN_QIFU_BUYXIANHUN_NO_TIMES_LEFT,				//今日仙魂祈福次数已经用完
		EN_QIFU_TIMES_NOT_ENOUGH,						//祈福次数没有达到，不能领取该奖励
		EN_ACTIVITY_CANNOT_FIND,						//该活动不能找回
		EN_NATIONAL_BOSS_CAMP_FINISH,					//您所在师门的魔神禁域活动已经结束
		EN_OPEN_GAME_ACTIVITY_NOT_OPEN,					//开服活动没有开启
		EN_OPEN_GAME_ACTIVITY_TOTAL_CHONGZHI_NOT_ENOUGH,//累计充值金额不足
		EN_OPEN_GAME_ACTIVITY_CAPABILITY_LIMIT,			//战斗力不足，不能领取
		EN_OPEN_GAME_ACTIVITY_ROLELEVEL_LIMIT,			//人物等级不足，不能领取
		EN_OPEN_GAME_ACTIVITY_PUTON_EQUIPMENT_LIMIT,	//装备未收集，不能领取
		EN_OPEN_GAME_ACTIVITY_ALREADY_FETCH_REWARD,		//您已经领取过该奖励
		EN_OPEN_GAME_ACTIVITY_GLOBAL_FETCH_TIMES_LIMIT,	//可领取次数已不足
		EN_OPEN_GAME_ACTIVITY_ALREADY_BUY,				//您已经购买过该物品
		EN_OPEN_GAME_ACTIVITY_ALREADY_BUY_ONE,				//您已经购买过物品
		EN_OPEN_GAME_ACTIIVTY_XIANNV_CHANMIAN_LEVEL_LIMIT,	//仙女缠绵等级不足
		EN_OPEN_GAME_ACTIVITY_MOUNT_GRADE_LIMIT,		//坐骑阶数不足
		EN_OPEN_GAME_ACTIVITY_STONE_LEVEL_LIMIT,		//宝石总等级不足
		EN_OPEN_GAME_ACTIVITY_XUNBAO_LIMIT_TIME,		//全服寻宝次数不足
		EN_OPEN_GAME_ACTIVITY_MONTHCARD_BUY_NUM_LIMIT,	//全服购买至尊会员次数不足
		EN_OPEN_GAME_ACTIVITY_VIPLEVEL_NUM_LIMIT,		//该等级的Vip人数不足
		EN_OPEN_GAME_ACTIVITY_SHEN_FANGJU_NUM_LIMIT,	//神装防具数量不足
		EN_MAZE_CANNOT_BUY_MOVE_TIMES,					//今日购买行动力次数已经用完 
		EN_ACTIVE_DEGREE_ALREADY_FETCH_REWARD,			//你已经领过该奖励
		EN_EXP_FB_TODAY_FREE_TIMES_LIMIT,				//今天的免费次数已经用完
		EN_CHALLENGE_FB_JOIN_TIMES_LIMIT,				//挑战次数已经用完
		EN_CHALLENGE_FB_BUY_TIMES_LIMIT,				//你当前VIP等级，可购买的次数已达上限！
		EN_CHALLENGE_FB_RESET_LAYER_LIMIT,				//当前副本已重置
		EN_CHALLENGE_FB_CURR_LEVEL_LOCK,				//当前关卡尚未解锁
		EN_CHALLENGE_FB_LEVEL_LIMIT,					//等级不足，请努力升级！
		EN_DISCOUNT_BUY_CLOSE_TIME_LIMIT,				//当前阶段的一折抢购已经过期
		EN_DISCOUNT_BUY_BUY_COUNT_LIMIT,				//该物品的购买次数已达上限
		EN_ZERO_GIFT_BUY_TIME_LIMIT,					//礼包已过期
		EN_ZERO_GIFT_BUY_LEVEL_LIMIT,					//购买等级不足
		EN_ZERO_GIFT_BUY_DAY_LIMIT,						//活动不在开启日期内
		EN_ZERO_GIFT_GOD_COSTUME_FETCH_LEVEL_LIMIT,		//领取等级不足
		EN_ZERO_GIFT_FETCH_TIME_LIMIT,					//领取时间还没到
		EN_QUESTION_ROLE_LEVEL_LIMIT,					//等级不足，不能参与答题
		EN_QUESTION_FETCH_QUESTION_TIMES_LIMIT,			//答题次数已经用完
		EN_TOWERDEFEND_FB_BUY_LIMIT_TIMES,				//你当前VIP等级，可购买的次数已达上限！
		EN_WING_NOT_ACTIVE,								//羽翼还没激活
		EN_MAZE_MOVE_TIMES_FULL,						//当前行动力已经满，不能购买
		EN_CHALLENGE_FIELD_OPPONENT_NOT_EXIST,			//对手不存在
		EN_CHALLENGE_FIELD_JOIN_TIMES_LIMIT,			//挑战次数不足，请购买额外挑战次数
		EN_CHALLENGE_FIELD_JIFEN_LIMIT,					//积分不足，挑战其他玩家可获得积分
		EN_CHALLENGE_FIELD_FETCH_EMPTY,					//当前奖励已领取，请等待下一次排名结算
		EN_PHASE_FB_FUN_OPEN_LEVEL_LIMIT,				//未达到副本开启等级
		EN_PHASE_FB_FUN_OPEN_PRE_TASK_LIMIT,			//未完成副本开启任务
		EN_PHASE_FB_TIMES_LIMIT,						//挑战次数已经用完
		EN_PHASE_FB_NO_PASS,							//请先通关该关卡
		EN_PHASE_FB_AUTO_LEVEL_LIMIT,					//未达到扫荡开启等级
		EN_PHASE_FB_PASS_ALL_LIMIT,						//您已经通关所有关卡
		EN_SKILL_MAX_LEVEL_LIMIT,						//技能已经升到最高级
		EN_MAZE_HIT_BOMB,								//遭遇炸弹，扣除一点行动力
		EN_REALIVE_HERE_IN_CD,							//原地复活正处于CD中
		EN_STORY_FB_FUN_OPEN_LEVEL_LIMIT,				//未达到副本开启等级
		EN_STORY_FB_TIMES_LIMIT,						//挑战次数已经用完
		EN_STORY_FB_NO_PASS,							//请先通关该关卡
		EN_STORY_FB_NOT_LEVEL,							//当前无可扫荡关卡

		EN_WABAO_JOIN_TIME_LIMIT,						//宝图次数已用完
		EN_WABAO_BAOZANG_NOT_IN_SAME_SCENE,				//宝藏不在这个场景内
		EN_DIG_BAOZANG_LONG_LONG_AWAY,					//宝藏还在很远的地方
		EN_DIG_BAOZANG_LONG_AWAY,						//宝藏似乎就在不远处！
		EN_DIG_BAOZANG_MID_LONG_AWAY,					//宝藏就在附近！
		EN_DIG_BAOZANG_SMALL_LONG_AWAY,					//越来越接近宝藏了！
		EN_DIG_BAOZANG_NEARBY,							//宝藏就在旁边了！
		EN_QIFU_FREE_TIMES_LIMIT,						//免费祈福次数已经用完
		EN_QIFU_FREE_CHANCE_CD,							//免费祈福机会还在冷却中
		EN_TUMO_COMPLETE_ALL_TASK_FIRST,				//请先完成所有日常任务
		EN_TUMO_HAS_FETCH_COMPLETE_ALL_REWARD,			//已经领取过该奖励
		EN_ROLE_TILI_LIMIT,								//体力不足
		EN_EXP_FB_LEVEL_UNLOCK,							//需先三星通关前一关卡
		EN_MAZE_GAME_FINISH,							//迷宫游戏已经结束，不能再购买移动次数
		EN_RENAME_NAME_INVALID,							//含非法字符，请重新输入
		EN_TEAM_FB_OPEN_TIMES_LIMIT,					//您今天开启副本次数已用完
		EN_TEAM_FB_PARTNER_NOT_ONLINE,					//有队友不在线
		EN_TEAM_FB_JOIN_LEVEL_LIMIT,					//有队友等级不足以进入副本
		EN_TEAM_FB_ASSIST_TIMES_LIMIT,					//有队友的协助次数不足
		EN_TEAM_FB_ENTER_FB_LIMIT,						//有队友当前不能进入副本场景
		EN_SHENGWANG_LESS_LIMIT,						//声望不足
		EN_XIANMENGZHAN_GATHER_LIMIT,					//据点已经被占领
		EN_FB_TEAM_TYPE_YAOSHOUJITAN_LIMIT,				//必须在妖兽祭坛组队模式下才能进入副本
		EN_FB_TEAM_TYPE_TOWERDEFEND_LIMIT,				//必须在神城守卫组队模式下才能进入副本
		EN_FB_TEAM_TYPE_MIGONGXIANFU_LIMIT,				//必须在迷宫仙府组队模式下才能进入副本
		EN_TEAM_FB_OPEN_FB_LIMIT,						//只有队长才能开启
		EN_CHONGZHI_HAS_FETCH_REWARD,					//您已经领取过该奖励 
		EN_CHONGZHI_CAN_NOT_FETCH_REWARD,				//您还不能领取该奖励
		EN_CHONGZHI_TOTAL_RECHARGE_LIMIT,				//充值金额不足，不能领取该奖励
		EN_CHONGZHI_NOT_RECHARGE_TODAY,					//今天还没有充值
		EN_CHONGZHI_FIRST_RECHARGE_NOT_OPEN,			//首充活动尚未开始
		EN_CISHA_TASK_ALREAD_OWN_CISHA_TASK,			//你已经接取了一个刺杀任务
		EN_CISHA_TASK_ACCEPT_TIMES_LIMIT,				//今天的刺杀任务已经完成
		EN_JILIAN_OUT_OF_RANGE,							//超出可祭炼仙丹范围
		EN_JILIAN_JOIN_LIMIT_TIMES,						//今天的祭炼次数已经用完
		EN_JILIAN_START_JILIAN_FIRST,					//请先开始祭炼
		EN_JILIAN_FINISH_JILIAN_FIRST,					//请先完成本次祭炼
		EN_CHALLENGE_FB_RESET_LEVEL_FIRST,				//请先重置副本
		EN_WING_SPECIAL_IMG_ALREAD_ACTIVE,				//特殊羽翼形象已经激活
		EN_FOOTPRINT_ALREAD_ACTIVE,						//该足迹已经激活
		EN_FOOTPRINT_NOT_ACTIVE,						//该足迹尚未激活
		EN_MARRY_TEAM_MEMBER_MUST_IN_SAME_MAP,			//对方必须在同一地图
		EN_MARRY_MUST_DIFFERENT_SEX_IN_TEAM,			//同性别不可以结婚
		EN_MARRY_RESERVE_MUST_BE_TEAM_LEADER,			//只有队长才能申请开始
		EN_TEAM_MUST_HAVE_MEMEBER,						//抓鬼至少两个人才能开始哦！
		EN_MARRY_HAS_BEEN_MARRY,						//双方有人已经结婚了
		EN_MARRY_LEVEL_NOT_ENOUGH,						//等级不足，不允许结婚
		EN_MARRY_INTIMACY_NOT_ENOUGH,					//所需亲密度不足
		EN_MARRY_RET_NOT_ACCEPT,						//对方拒绝了您的求婚
		EN_DIVORCE_RET_NOT_ACCEPT,						//对方拒绝了您的离婚
		EN_MARRY_TEAM_MEMBER_NUM_INVALID,				//请单独与伴侣组队进入副本
		EN_QINGYUAN_EQUIP_STUFF_ERROR,					//婚戒升星材料使用错误
		EN_QINGYUAN_EQUIP_NOT_ACTIVE,					//情缘装备还未激活
		EN_QINGYUAN_EQUIP_NOT_GOOD_AND_LEVEL_MAX,		//该物品的不能提升品质并且装备等级已经达到最高
		EN_QINGYUAN_EQUIP_QUALITY_ALREADY_MAX,			//品质已经提升到最高
		EN_QINGYUAN_EQUIP_SLOT_OPEN_LIMIT,				//结婚后开放该装备
		EN_QINGYUAN_EQUIP_LEVEL_MAX,					//情缘装备已达到最高等级
		EN_QINGYUAN_EQUIP_ALREADY_HAVE,					//已有情缘装备
		EN_QINGYUAN_MAX_BUY_COUNT,						//情缘次数已达最大数量
		EN_QINGYUAN_TEAM_MEMBER_ERROR,					//情缘队伍人数必须为2人

		EN_QINGYUAN_EQUIP_NO_MARRY,						//您还未有伴侣
		EN_QINGYUAN_MARRY_LEVEL_NOT_ENOUGH,				//结婚等级不足，无法穿戴
		EN_QINGYUAN_EQUIP_SUIT_ACTIVE_LIMIT,			//该套装槽位已激活
		EN_QINGYUAN_EQUIP_SUIT_EQUIP_ATTR_LIMIT,		//结婚装备属性不满满足激活条件

		EN_QINGYUAN_TUODAN_TIMES_MAX,					//每天只能发布两次脱单宣言
		EN_QINGYUAN_TUODAN_SUCCESS,						//脱单宣言发布成功

		EN_QINGYUAN_TREE_LEVEL_MAX,						//相思树已经达到最高等级
		
		EN_MARRY_HUNYAN_OPEN_LIMIT,						//今天开启婚宴次数已经用完
		EN_MARRY_HUNYAN_JOIN_LIMIT,						//今天参与婚宴次数已经用完
		EN_MARRY_NOT_MARRIED,							//还没有结婚
		EN_MARRY_HUNYAN_WITH_OTHER,						//与伴侣组队后才能进入婚宴
		EN_TRADE_ITEM_IS_CHANGE,						//交易物品已经改变
		EN_NOT_AT_TRADING,								//不在适当的交易状态
		EN_REJECT_TRADE,								//对方拒绝了您的交易请求
		EN_QINGYUAN_TARGET_NOT_ON_LINE,					//伴侣不在线
		EN_QINGYUAN_TARGET_IN_TEAM,						//伴侣不在队伍中
		EN_QINGYUAN_FB_CAN_NOT_SKIP,					//波数不足,不能跳过
		EN_TRADE_CHECK_TRADE_FAIL,						//检查交易失败
		EN_QINGYUAN_DIVORCE_DAYS_LIMIT,					//结婚1天后，才能离婚
		EN_QINGYUAN_REMARRY_DAYS_LIMIT,					//离婚当天不能结婚
		EN_QINGYUAN_ANOTHER_REMARRY_LIMIT_DAYS,			//对方刚离婚
		EN_TRADE_LOCK_STATE_CAN_NOT_CHANGE,				//当前交易已锁定，无法更改物品
		EN_TRADE_FAILED_OWN_BAG_HAS_FULL,				//背包不足，交易失败
		EN_TRADE_FAILED_ANOTHER_BAG_HAS_FULL,			//对方背包不足，交易失败
		EN_TRADE_FAILED_CANCLE_SELF,					//交易已取消
		EN_TRADE_FAILED_CANCLE_ANOTHER,					//对方取消交易，交易失败
		EN_TRADE_PUTITEM_MAX_COUNT,						//当前交易物品数量达到最大值
		EN_TRADE_WAIT_FOR_ANOTHER_LOCK,					//等待对方锁定
		EN_TRADE_LOCK_FIRST,							//请先锁定交易物品
		EN_TRADE_LEVEL_LIMIT,							//交易双方等级必须达到31级
		EN_ZHANSHENDIAN_LEVEL_LIMIT,					//未达到参加镇魔塔所需等级
		EN_ZHANSHENDIAN_MAX_LEVEL,						//您已通关镇魔塔最高层
		EN_LUCKYROLL_TIMES_NOT_ENOUGH,					//累计转动次数不足，无法领取额外额励
		EN_CLOSE_BETA_ACTIVITY_NOT_OPEN,				//封测活动没有开启
		EN_CLOSE_BETA_HAS_FETCH_REWARD,					//已经领取过该奖励
		EN_CLOSE_BETA_FETCH_REWARD_LIMIT,				//还没达成领取奖励条件
		EN_CLOSE_BETA_HAS_NOT_MARRY,					//您还没结过婚
		EN_CLOSE_BETA_JOIN_GUILD_FIRST,					//加入一个仙盟才能领取该奖励
		EN_CLOSE_BETA_ONLINE_TIMES_LIMIT,				//在线时间不足，请稍后再试
		EN_CLOSE_LUCKYROLL_NOT_OPEN,					//幸运转盘活动未开启
		EN_GUILD_TASK_ALL_COMPLETE,						//仙盟任务已经全部完成
		EN_GUILD_TASK_ONE_KEY_VIP_LEVEL_LIMIT,			//VIP等级太低不能一键完成
		EN_FRIEND_BLESS_FRIEND_LIMIT,					//对方不是你的好友，不能祝福
		EN_FRIEND_BLESS_HAS_BLESS,						//已祝福该好友，请明天再来
		EN_FRIEND_BLESS_NOT_BLESS,						//对方没有祝福你
		EN_FRIEND_BLESS_HAS_FETCH_REWARD,				//已领取过该祝福奖励
		EN_FRIEND_BLESS_FETCH_REWARD_TIMES_LIMIT,		//今天的祝福奖励次数已经领完
		EN_FRIEND_BLESS_BLESS_TIMES_LIMIT,				//祝福次数已经达到上限
		EN_FRIEND_BLESS_ENERGY_IS_FULL,					//体力已满
		EN_TOUZIJIHUA_ACTIVITY_NOT_OPEN,				//活动未开启不能激活
		EN_TOUZIJIHUA_LEVEL_LIMIT,						//超出投资等级
		EN_TOUZIJIHUA_PLAN_NOT_ACTIVE,					//计划还没激活
		EN_TOUZIJIHUA_PLAN_HAS_ACTIVE,					//已经激活过投资计划
		EN_TOUZIJIHUA_HAS_FETCH_REWARD,					//你已经领取过该奖励
		EN_TOUZIJIHUA_FETCH_REWARD_LIMIT,				//等级不足，无法领取该奖励
		EN_TOUZIJIHUA_FOUNDATION_BUY_LEVEL_LIMIT,		//当前等级不满足购买条件，无法购买
		EN_TOUZIJIHUA_FOUNDATION_HAD_BUY,				//你已经购买过了
		EN_TOUZIJIHUA_FOUNDATION_FETCH_LEVEL_LIMIT,		//等级不足，无法领取该奖励
		EN_TOUZIJIHUA_FOUNDATION_FETCH_NOT_BUY,			//你还没有购买，无法领取该奖励
		EN_TOUZIJIHUA_TIME_LIMIT,						//天数不足，无法领取该奖励
		EN_ROLL_MONEY_ROLL_TIMES_LIMIT,					//摇奖次数已用完
		EN_ROLL_MONEY_DAILY_ROLL_TIMES_LIMIT,			//当日摇奖次数已用完
		EN_ROLL_MONEY_HAS_FETCH_ALL_REWARD,				//已经用完了所有奖励次数
		EN_ROLL_MONEY_ACTIVE_DEDREE_LIMIT,				//活跃度未达到要求
		EN_ROLL_MONEY_ALREADY_ROLL_TODAY,				//明天可继续摇奖
		EN_ROLL_MONEY_LOGIN_DAYS_NOT_ENOUGH,			//登录天数不足
		EN_FISH_POOL_DAY_RAISE_COUNT_LIMIT,				//此类鱼每日放养数量已满
		EN_FISH_POOL_POOL_RAISE_COUNT_LIMIT,			//鱼池不能再放养这类鱼
		EN_FISH_POOL_POOL_LEVEL_LIMIT,					//鱼池等级不足
		EN_FISH_POOL_CAPACITY_LIMIT,					//鱼池容量不足
		EN_FISH_POOL_GUARD_FISH_CAPACITY_LIMIT,			//不能再养更多的守卫鱼了
		EN_FISH_POOL_EXTEND_CAPACITY_MAX_LIMIT,			//拓展容量达到上限
		EN_FISH_POOL_HARVEST_GRADE_LIMIT,				//鱼儿还不能收获
		EN_FISH_POOL_STEAL_GRADE_LIMIT,					//鱼儿还不能被偷	
		EN_FISH_POOL_LACK_BULLET,						//缺少子弹
		EN_FISH_POOL_BUY_BULLET_LIMIT_TIMES,			//购买子弹次数限制
		EN_FISH_POOL_CANNOT_STEAL_SELF,					//不能偷自己的鱼
		EN_FISH_POOL_FISH_TOO_LITTLE,					//鱼儿太小，无法捕获
		EN_FISH_POOL_STEAL_ERROR,						//偷鱼失败，鱼塘不是想偷的
		EN_FISH_POOL_IS_CHANGE,							//池塘已变化，请重新操作
		EN_FISH_POOL_STEAL_FAIL,						//鱼儿在你的眼底下溜走了
		EN_FISH_POOL_TEALFISH_TIME_LIMIT,				//此鱼塘不能偷鱼

		EN_FISH_POOL_FANG_FISH_TIMES_LIMIT,			    //养鱼次数已经用完
		EN_FISH_POOL_UP_FISH_LIMIT,						//池里还有鱼，不能升级
		EN_FISH_POOL_UP_FISH_FAIL,						//升级失败
		EN_FISH_POOL_BUY_FANG_FISH_TIMES_LIMIT,			//今天不能再购买养鱼次数

		EN_ZHANSHENDIAN_ALREADY_FETCH_DAY_REWARD,		//今天已领取过奖励
		EN_HUNYAN_HONGBAO_BUY_TOOMUCH,					//购买红包已达到上限
		EN_HUNYAN_NOT_START,							//婚宴还没开始，请稍候
		EN_HUNYAN_INVITE_FAIL,							//邀请失败
		EN_HUNYAN_GATHAR_TOO_MUCH,						//采集已达到上限
		EN_HUNYAN_GATHAR_FOOD_TOO_MUCH,					//吃食物已达到上限
		EN_HUNYAN_SAXIANHUA_MAX_COUNT,					//撒鲜花次数已用完
		EN_HUNYAN_ALREADY_START,						//婚宴只能举行一次
		EN_HUNYAN_ALREADY_STOP,							//婚宴已经结束
		EN_HUNYAN_IS_RUNNING,							//婚宴已经开始过
		EN_WORLD_EVENT_HAS_FIND_NPC,					//你已经找到过这个NPC
		EN_QINGYUAN_DIVORCE_LIMIT,						//婚宴开启期间不可以离婚
		EN_QINGYUAN_HUNYAN_INVITE_TOO_MUCH,				//对方婚宴邀请已满
		EN_FISHPOOL_HARVEST_LIMIT,						//当前阶段不能捕捞
		EN_FISHPOOL_STEAL_LIMIT,						//鱼苗无法捕捞,捕捞失败
		EN_WORLD_EVENT_NOT_FIT_NPC,						//这不是您当前所需要寻找的NPC
		EN_WORLD_EVENT_LIGHT_WORD_STUFF_LIMIT,			//所需材料不足
		EN_WORLD_EVENT_NOT_REWARD_TO_FETCH,				//没有可领取的奖励
		EN_WORLD_EVENT_DISTANCE_LIMIT,					//距离过远
		EN_WORLD_EVENT_USE_FOOL_CARD_LIMIT,				//附近没有可以被变身的玩家
		EN_WORLD_EVENT_SHENMEZHIXI_FB_LEVEL_LIMIT,		//30级以上才能进入神魔之隙
		EN_RAND_ACTIVITY_NOT_OPEN,						//活动尚未开启
		EN_RAND_ACTIVITY_PERSONAL_BUY_NUM_LIMIT,		//该物品购买次数已经达到上限
		EN_RAND_ACTIVITY_SERVER_BUY_NUM_LIMIT,			//悲剧啊，该道具已经被抢光了
		EN_FBMANAGER_JOIN_FB_LIMIT,						//现在还不能参与此副本
		EN_RAND_ACTIVITY_HAS_FETCH_REWARD,				//已经领取过该奖励
		EN_RAND_ACTIVITY_CAN_NOT_FETCH_REWARD,			//条件未达成，无法领取
		EN_ROLE_GOAL_HAS_FETCH_REWARD,					//没有可领取的奖励
		EN_ROLE_GOAL_CAN_NOT_FETCH_REWARD,				//还不能领取该奖励
		EN_ROLE_GOAL_SKILL_LEVEL_MAX,					//技能已达最大级
		EN_RAND_ACTIVITY_MENTALITY_CAN_NOT_FETCH_REWARD,//还不能领取该奖励
		EN_RAND_ACTIVITY_MENTALITY_HAS_FETCH_REWARD,	//已领取
		EN_RAND_ACTIVITY_NOT_GIVE_FLOWER,				//请先参与送花活动
		EN_RAND_ACTIVITY_CORNUCOPIA_VALUE_LIMIT,		//聚宝值不足
		EN_RED_PAPER_CAN_FETCH_TIMES_LIMIT,				//每日红包领取次数不能超过100

		EN_RAND_ACTIVITY_LOTTERY_TOTAL_TIMES_LIMIT,		//累计购买次数不足
		EN_RAND_ACTIVITY_LOTTERY_HAS_FETCH_LIMIT,		//您已经领取过该奖励
		EN_RAND_ACTIVITY_LOTTERY_VIP_LIMIT,				//vip等级不足，不能领取

		EN_RED_PAPER_ONCE_GOLD_NUM_LIMIT,				//单个红包金额不能超过200
		EN_HUNYAN_FA_BLESS_GIFT_COUNT_LIMIT,			//每日婚宴发贺礼次数限制

		EN_YIZHANDAODI_GUWU_MAX,						//鼓舞已达上限
		EN_YIZHANDAODI_DEAD_TIMES_LIMIT,				//该次一战封神活动中，你已经死亡超过8次，请下次再战
		EN_TEAM_TOWERDEFEND_MUST_BE_TEAM_LEADER,		//只有队长才能申请
		EN_TEAM_TOWERDEFEND_MEMEBER_COUNT_LIMIT,		//队伍不足3人
		EN_TEAM_TOWERDEFEND_JOIN_TIMES_LIMIT,			//你已经参与过该副本
		EN_TEAM_TOWERDEFEND_PARTNER_NOT_ONLINE,			//有队友不在线
		EN_SKILL_NOT_LEARNT,							//没有这个技能
		EN_SCORE_NOT_ENOUGH,							//积分不足
		EN_TEAM_TOWERDEFEND_REPEAT_ATTR_TYPE,			//请提醒队员正确选择类型
		EN_CISHA_TASK_ACTIVITY_OPEN_LIMIT,				//刺杀活动还未开启，不能接受刺杀任务
		EN_MARRIAGE_SEEKING_SUCCESS,					//宣言成功
		EN_MARRIAGE_SEEKING_MARRIED,					//您已经结婚，不能进行宣言
		EN_LINGYUSHOUSHUI_HAS_FETCH_REWARD,				//您已经领取了该奖励
		EN_LINGYUSHOUSHUI_GCZ_WIN_REWARD_CAMP_LIMIT,	//您和城主不在同一个师门
		EN_LINGYUSHOUSHUI_XMZ_REWARD_RANK_LIMIT,		//您的仙盟当前没有占领任何领域
		EN_GUILD_FB_START_TUANZHANG_LIMIT,				//你没有权限开启仙盟试炼活动
		EN_GUILD_FB_IS_OPEN,							//仙盟试炼正在进行中
		EN_GUILD_FB_ALREADY_START_ONCE,					//仙盟试炼已经开启过了
		EN_GUILD_BONFIRE_START_TUANZHANG_LIMIT,			//你没有权限开启仙女祝福活动
		EN_GUILD_BONFIRE_ALREADY_START_ONCE,			//仙女祝福活动已经开启过了
		EN_GUILD_BONFIRE_SCENE_LIMIT,					//当前场景不允许开启仙女祝福活动
		EN_GUILD_BONFIRE_SAVE_AREA_LIMIT,				//安全区不允许开启仙女祝福活动
		EN_GUILD_BONFIRE_IS_OPEN,						//仙女祝福活动正在进行中
		EN_GUILD_ZHUFU_LUCKY_NOT_IN_YOURGUILD,			//对方不在你的仙盟里面
		EN_GUILD_ZHUFU_LUCKY_FLUSH_ISSAME,				//非常遗憾，运势不变
		EN_GUILD_ZHUFU_LUCKY_FLUSH_UPGRADE,				//鸿运当头，运势提升
		EN_GUILD_ZHUFU_LUCKY_IS_FULL,					//对方运势已最高
		EN_GUILD_ZHUFU_LUCKY_TIMES_IS_OUT,				//您今天祝福次数已用完
		EN_GUILD_FB_IS_OPEN_CANNOT_DISMISS,				//仙盟试炼活动正在进行，不允许解散仙盟
		EN_GUILD_BONFIRE_IS_OPEN_CANNOT_DISMISS,		//仙女祝福活动正在进行，不允许解散仙盟
		EN_QIBING_MOUNT_NOT_ACHIVE,						//需要完成主线任务开启坐骑功能
		EN_HUNYAN_GATHER_XIANHUAN_LIMIT,				//今日可获取婚宴奖励已达最大值
		EN_HUNYAN_GET_NUM_MAX,							//今天宴会得到该奖励已上限
		EN_HUNYAN_GET_NUM_MAX_EXP,						//今天宴会得到经验奖励已上限
		EN_HUNYAN_GET_NUM_MAX_BIND_GOLD,				//今天宴会得到绑钻奖励已上限
		EN_HUNYAN_GET_NUM_MAX_HUNJIE_STUFF,				//今天宴会得到婚戒奖励已上限
		EN_HUNYAN_GET_NUM_MAX_ROSES,					//今天宴会得到玫瑰花奖励已上限
		EN_YANHUI_INVITE_NUMBER_MAX,					//宴会邀请人数已达到最大值
		EN_YANHUI_TEAM_MEMBER_ERROR,					//队伍的条件不符合，必须是个人或和伴侣组队才能进入！
		EN_YANHUI_NOT_RUNNING,							//宴会已经结束
		EN_YANHUI_PART_BE_INVITED,						//因邀请人数已达最大，只有一部分人被邀请！
		EN_SHIZHUANG_NOT_ACTIVE,						//时装未激活
		EN_SHIZHUANG_HAS_ACTIVE,						//时装已经激活
		EN_CHANGE_CAMP_NOCAMP_LIMIT,					//请先加入师门
		EN_CHANGE_CAMP_LEVEL_LIMIT,						//等级不足，不能转师门
		EN_CHANGE_CAMP_OPEN_DAY_LIMIT,					//开服第一天不能转师门
		EN_CHANGE_CAMP_LIMIT_CHENGZHU,					//王城城主不可转师门
		EN_CHANGE_CAMP_LIMIT_HAS_TASK,					//护送和刺杀任务期间不可转师门
		EN_CHANGE_CAMP_LIMIT_HAS_WABAO,					//仙女掠夺期间不能转师门
		EN_CHANGE_CAMP_LIMIT_CAMPDEFEND,				//雕像守卫期间不能转师门
		EN_ZHUANZHI_MARRY_LIMIT,						//结婚角色不能转性别
		EN_ZHUANZHI_LEVEL_LIMIT,						//等级不足，不能转职
		EN_ALL_REWARD_HAS_FETCHED,						//所有奖励都已领完
		EN_CHONGZHI_WANT_MONEY_HAS_NOT_ENOUGH_CHARGE,	//未达到目标充值额度
		EN_CHONGZHI_WANT_MONEY_HAS_NOT_ENOUGH,			//剩余元宝不足
		EN_ZAI_CHONGZHI_FETCH_LIMIT,					//您还没满足领取条件
		EN_ZAI_CHONGZHI_HAS_FETCH,						//您已领取过再充值奖励
		EN_WING_CAN_NOT_UPGRADE,						//羽翼不能进阶
		EN_WING_GRADE_NOT_ACTIVE,						//羽翼形象未激活
		EN_MONTH_CARD_HAS_NOT_ACTIVITY_FETCH,			//您还没有购买或激活月卡
		EN_MONTH_CARD_REWARD_HAS_FETCH,					//已领取过元宝
		EN_CHARGE_REPAYMENT_NO_CFG,						//该奖励不存在
		EN_CHARGE_REPAYMENT_NOT_ACTIVE,					//未达到充值额度
		EN_CHARGE_REPAYMENT_HAS_FETCHED,				//该奖励已领取
		EN_SHUXINGDAN_USE_LIMIT_NUM,					//属性丹使用数量已达上限
		EN_FIGHTMOUNT_SHUXINGDAN_USE_LIMIT_NUM,			//战骑资质丹使用数量已达上限
		EN_FIGHTMOUNT_SHUXINGDAN_USE_ORDER_LIMIT,		//战骑阶数不足
		EN_MOUNT_SHUXINGDAN_USE_LIMIT_NUM,				//坐骑资质丹使用数量已达上限
		EN_MOUNT_SHUXINGDAN_USE_ORDER_LIMIT,			//坐骑阶数不足
		EN_WING_SHUXINGDAN_USE_LIMIT_NUM,				//羽翼资质丹使用数量已达上限
		EN_WING_SHUXINGDAN_USE_ORDER_LIMIT,				//羽翼阶数不足
		EN_HALO_SHUXINGDAN_USE_LIMIT_NUM,				//光环资质丹使用数量已达上限
		EN_HALO_SHUXINGDAN_USE_ORDER_LIMIT,				//光环阶数不足
		EN_SHENGONG_SHUXINGDAN_USE_LIMIT_NUM,			//仙女光环资质丹使用数量已达上限
		EN_SHENGONG_SHUXINGDAN_USE_ORDER_LIMIT,			//仙女光环阶数不足
		EN_SHENYI_SHUXINGDAN_USE_LIMIT_NUM,				//神翼资质丹使用数量已达上限
		EN_SHENYI_SHUXINGDAN_USE_ORDER_LIMIT,			//神翼阶数不足
		EN_FOOTPRINT_SHUXINGDAN_USE_LIMIT_NUM,			//足迹资质丹使用数量已达上限
		EN_FOOTPRINT_SHUXINGDAN_USE_ORDER_LIMIT,		//足迹阶数不足
		EN_CLOAK_SHUXINGDAN_USE_LIMIT_NUM,				//披风资质丹使用数量已达上限
		EN_CLOAK_SHUXINGDAN_USE_ORDER_LIMIT,			//披风阶数不足
		EN_CSA_QIANGGOU_NUM_LIMIT,						//该商品已经被抢购一空
		EN_CSA_ROLL_CHONGZHI_LIMIT,						//充值金额不足，不能摇奖
		EN_CSA_CAN_NOT_FETCH_REWARD,					//条件未达成，无法领取
		EN_CSA_HAS_FETCH_REWARD,						//已经领取过该奖励
		EN_SUPPLY_MAX_LIMIT,							//已经购买太多补给
		EN_CSA_PERSONAL_BUY_NUM_LIMIT,					//该物品购买次数已经达到上限
		EN_CSA_SERVER_BUY_NUM_LIMIT,					//悲剧啊，该道具已经被抢光了
		EN_DANBI_CHONGZHI_REWARD_HAS_NOT_ACTIVE,		//奖励未激活
		EN_DANBI_CHONGZHI_REWARD_HAS_FETCH,				//奖励已领取过
		EN_ZHUANZHI_COMPLETE_TASK_FIRST,				//请先完成转职任务
		EN_RA_TOTAL_CHARGE_DAY_CAN_NOT_FETCH_REWARD,	//奖励无法领取
		EN_RA_TOTAL_CHARGE_DAY_REWARD_NOT_ACTIVE,		//奖励未激活
		EN_RA_TOTAL_CHARGE_DAY_REWARD_HAS_FETCH,		//奖励已领取过
		EN_ONLY_TEAM_LEADER_CAN_REQUEST_START,			//只有队长才能申请开始
		EN_SOME_TEAMMATE_NOT_ONLINE,					//有队友不在线
		EN_CHENGZHANGDAN_USE_LIMIT_NUM,					//成长丹使用数量已达上限
		EN_CSA_TOUZIJIHUA_NOT_BUY,						//还没有购买
		EN_CSA_TOUZIJIHUA_HAD_BUY,						//已经购买过了
		EN_CSA_TOUZIJIHUA_HAD_FETCH,					//已经领取过该奖励
		EN_CSA_TOUZIJIHUA_FETCH_LIMIT,					//还不能领取该奖励
		EN_CSA_LEVEL_LIMIT,								//您的等级不符合购买条件

		EN_WUSHUANG_LIEHUN_POOL_FULL,					//猎魂池已满
		EN_WUSHUANG_ALREADY_MAX_CHOUHUN_COLOR,			//请直接抽魂
		EN_WUSHUANG_BAG_ALREAD_FULL,					//背包已满
		EN_WUSHUANG_EQUIP_FULING_SLOT_USED,				//该槽位已经被使用
		EN_WUSHUANG_EQUIP_FULING_SAME_HUNSHOU_LIMIT,	//同一类命魂只能装备一个
		EN_WUSHUANG_FUHUN_ADD_EXP_ALREADY_REACHED_LEVEL,//已经到达指定等级
		EN_WUSHUANG_FUHUN_EXP_LIMIT,					//命魂经验不足
		EN_WUSHUANG_FB_HAS_PASSED_MAX_LEVEL,			//您已通关无双副本最高层
		EN_WUSHUANG_FB_NOT_ENOUGH_LEVEL,				//等级不足，无法挑战
		EN_WUSHUANG_FB_TODAY_HAS_FETCH_REWARD,			//您已领过今天的奖励
		EN_WUSHUANG_FB_HAS_NOT_PASS,					//您还没有通关过该副本
		EN_WUSHUANG_QUALITY_IS_MAX,						//该装备当前品质已是最高
		EN_WUSHUANG_DAZAO_LEVEL_IS_MAX,					//该装备打造等级已是最高

		EN_LIEMING_LIEHUN_POOL_FULL,					//猎魂池已满
		EN_LIEMING_ALREADY_MAX_CHOUHUN_COLOR,			//请直接抽魂
		EN_LIEMING_BAG_ALREAD_FULL,						//背包已满
		EN_LIEMING_EQUIP_FULING_SLOT_USED,				//该槽位已经被使用
		EN_LIEMING_EQUIP_FULING_SAME_HUNSHOU_LIMIT,		//同一类灵魂只能装备一个
		EN_LIEMING_FUHUN_ADD_EXP_ALREADY_REACHED_LEVEL, //已经到达最大等级
		EN_LIEMING_FUHUN_EXP_LIMIT,						//灵魂经验不足
		EN_LIEMING_SOLT_NOT_ACTIVTY,					//该槽位未激活
		EN_LIEMING_SOLT_NO_SPACE,						//没有可用槽位

		EN_CARDZU_HUALING_CARD_NUM_LIMIT,				//卡牌数量不足
		EN_CARDZU_ZHULING_LINGLI_LIMIT,					//当前灵力不足
		EN_CARDZU_HAS_NOT_ACTIVED,						//该卡牌组合未激活
		EN_CARDZU_HAS_ACTIVED,							//该卡牌组合已激活过
		EN_CARDZU_ACTIVED_ZUHE_CARD_NUM_LIMIT,			//卡牌数量不足
		EN_CARDZU_UPGRADE_ZUHE_LINGLI_LIMIT,			//当前灵力不足
		EN_CARDZU_CHOUKA_TIME_LIMIT,					//抽卡次数已达上限
		EN_FB_TEAM_TYPE_ZHUANZHI_LIMIT,					//必须在转职副本组队模式下才能进入副本
		EN_CHAT_WORD_LEVEL_LIMIT,						//80级开启世界聊天
		EN_SINGLE_CHAT_OTHER_LEVEL_LIMIT,				//对方私聊等级不足
		EN_NOT_START_CROSS_IN_FB,						//副本中不能进入跨服
		EN_CAN_NOT_START_CROSS,							//您无法参加当前跨服活动
		EN_CROSS_HONOR_LESS_LIMIT,						//跨服荣誉不足
		EN_CAN_NOT_USE_ITEM_IN_CROSS,					//在跨服中不允许使用道具
		EN_GUILD_CURRENT_GUILD_STATION_BUSY,			//仙盟领地已有其他活动
		EN_GUILD_STATION_BUSY_CANNOT_DISMISS,			//仙盟活动进行中，不能解散仙盟
		EN_GUILD_ACTIVE_DEGREE_LIMIT,					//仙盟活跃度不足
		EN_GUILD_TOTEM_EXP_NOT_ENOGHT,					//图腾经验不足
		EN_RA_TOMORROW_REWARD_COUNT_LIMIT,				//剩余抽奖次数不足
		EN_QINGYUAN_CARD_ID_INVALID,					//该卡牌不存在
		EN_QINGYUAN_CARD_HAS_FULL_LEVEL,				//该卡牌已是最高等级
		EN_QINGYUAN_CARD_UPGRADE_ITEM_LIMIT,			//合成材料不足
		EN_EQUIP_CROSS_EQUIP_CANNOT_PUTON,				//该物品不能装备
		EN_EQUIP_CROSS_EQUIP_ALREAD_EQUIP,				//已经装备了装备
		EN_CROSS_XIULUOTA_BUY_REALIVE_LIMIT_TIMES,		//当前鼓舞已达最大值
		EN_CROSS_XIULUOTA_GUWU_LIMIT_TIMES,				//可购买复活次数已达最大值
		EN_CROSS_XIULUOTA_LEVEL_LIMIT,					//等级不足301，不能进入无尽之塔
		EN_CROSS_XIULUOTOWER_HAS_BUY_BUFF,				//你已经购买过该buff
		EN_QINGYUAN_CARD_UPGRADE_LEVEL_LIMIT,			//角色等级不足
		EN_CROSS_NOT_CONNECTED_TO_CROSS_SERVER,			//没有连接到跨服服务器
		EN_CROSS_1V1_CAPABILITY_LIMIT,					//三万以上战力才可参与活动
		EN_CROSS_1V1_REWARD_SCORE_LIMIT,				//积分不足
		EN_CROSS_1V1_REWARD_FETCHED,					//您已领取过该奖励
		EN_1V1_NOT_MATCH,								//还没有匹配对手
		EN_1V1_MATCHING_CANNOT_ENTER_FB,				//活动正在匹配中，不能进入副本
		EN_1V1_CANNOT_MATCH_STATUS,						//特殊场景，特殊、战斗状态下不能进行匹配
		EN_1V1_MATCHING_CANNOT_HUSONG,					//活动正在匹配中，不能护送仙女
		EN_1V1_MATCHING_CANNOT_DAOQULINGSHI,			//活动正在匹配中，不能夺取灵石
		EN_1V1_DAILYTIMES_BUY_TIMES_MAX,				//当日购买次数已用完
		EN_1VN_NOT_SIGN_PHASE,							//现在不是报名阶段
		EN_1VN_ALREADY_BET,								//每轮只能支持一次选手
		EN_1VN_BET_TOO_MUCH,							//您支持的数额太大了
		EN_1VN_BET_TOO_LATE,							//您错过了支持的时间
		EN_1VN_NOT_SIGN,								//您没有报名本场比赛
		EN_WUSHUANG_FUMO_ITEM_INVALID,					//这个物品不能用于附魔
		EN_WUSHUANG_FUMO_HAS_FUMO,						//这个位置已经附魔
		EN_WUSHUANG_FUMO_HAS_NOT_FUMO,					//这个位置没有附魔
		EN_WUSHUANG_FUMO_HAS_SAME_TYPE_FUMO,			//已经有同类型的附魔
		EN_MULTIUSER_CHALLENGE_MEMBER_NOT_IN_SAME_SCENE,//有成员不在同一场景内
		EN_MULTIUSER_CHALLENGE_MEMBER_CAN_NOT_START_CROSS, //有成员不能进行跨服
		EN_MULTIUSER_CHALLENGE_MEMBER_LEVEL_LIMIT,		//有成员等级不足
		EN_MULTIUSER_CHALLENGE_MEMBER_CAPABILITY_LIMIT, //有成员战斗力不足
		EN_MULTIUSER_CHALLENGE_MEMBER_IS_NOT_TEAM_LEADER, //组队状态只有队长才能请求匹配
		EN_MULTIUSER_CHALLENGE_MEMBER_NO_DAYCOUNT,		//有成员无参赛次数
		EN_MULTIUSER_CHALLENGE_MEMBER_IN_MATCH,			//本方有成员尚在匹配冷却时间
		EN_MULTIUSER_CHALLENGE_MEMBER_IN_MATCHING,		//有成员正在匹配中
		EN_MULTIUSER_CHALLENGE_MEMBER_END_NOT_GATHER,	//本场比赛已结束，不能采集
		EN_MULTIUSER_CHALLENGE_WAER_CARD_LIMIT,			//最多只能佩戴四个令牌
		EN_MULTIUSER_CHALLENGE_MEMBER_IN_FB,			//有成员在副本中，不能进行匹配
		EN_MULTIUSER_CHALLENGE_SELF_IN_MATCH,			//上次参加的战斗尚未结束，匹配冷却中
		EN_TIANGONG_SHILIAN_MAX_LEVEL,					//您已通关天宫试练最高层
		EN_WUSHUANG_NOT_ACTIVE,							//该装备还未激活
		EN_NV_WA_SHI_NOT_ENOUGH,						//女娲石不足
		EN_MOJING_NOT_ENOUGH,							//魔晶不足
		EN_GONGXUN_NOT_ENOUGH,							//功勋不足
		EN_WEIWANG_NOT_ENOUGH,							//威望不足
		EN_TREASURE_HUNT_CREDIT_NOT_ENOUGH,				//寻宝积分不足
		EN_JINGLING_CREDIT_NOT_ENOUGH,					//宠物寻宝积分不足
		EN_HAPPYTREE_GROW_NOT_ENOUGH,					//欢乐果树成长值不足
		EN_CROSS_HONOR_NOT_ENOUGH,						//跨服荣誉不足
		EN_USE_ITEM_SCENE_LIMIT,						//该场景不能使用该物品
		EN_HAS_BUFF_MORE_LEVEL,							//身上存在更高级的BUFF
		EN_TASK_MAX_STAR,								//当前已是最高星星等级
		EN_TEAM_ROOM_LEADER_LIMIT,						//您不是队长不能开启副本
		EN_MGXF_NOT_NEAR_DOOR,							//不在传送点附近
		EN_MGXF_DOOR_NOT_EXIST,							//传送点不存在
		EN_MGXF_LAYER_NOT_EXIST,						//传送点连接的场景不存在
		EN_SPACE_REMOVE_RECORD_SUCC,					//消息删除成功！
		EN_TODAY_U_CAI_TARGET_LIMIT,					//您今天已踩过对方啦！
		EN_ACTIVEDEGREE_NOT_ENOUGH,						//活跃度不足，不能领取奖励！
		EN_ACTIVEDEGREE_TARGET_LIMIT,					//未达到领取条件
		EN_ACTIVEDEGREE_HAVE_FETCH_LIMIT,				//您已经领取过该活跃奖励
		EN_ENTER_DABAO_MAP_COUNT_LIMIT,					//套装Boss地图进入次数不足啦！
		EN_LIFESKILL_MAX_SKLL_LEVEL,					//已达到最大技能等级
		EN_LIFESKILL_HUOLI_NOT_ENOUGH,					//活力不足
		EN_LIFESKILL_LEVEL_LIMIT,						//技能等级不足，无法制作
		EN_LIFESKILL_MAKE_FAIL,							//很遗憾，此次制作失败，继续加油
		EN_UPDATE_SPACE_XINQING_SUCC,					//修改签名成功！
		EN_CARD_NOT_ACTIVE,								//卡牌还未激活，不能升级
		EN_CARD_NOT_SAME_STUFF,							//不是对应的卡牌碎片，不能回收
		EN_CARD_MAX_LEVEL,								//卡牌已升到最高级
		EN_CARD_UPLEVEL_STUFF_NOT_ENOUGH,				//卡牌升级材料不足
		EN_JINGHUA_GATHER_DAYCOUNT_NOT_ENOUGH,			//当日次数已用完
		EN_JINGHUA_GATHER_NONE,							//精华已经采完，请等下次刷新
		EN_JINGHUA_NOT_IN_ROLE,							//请先去采集精华
		EN_JINGHUA_HUSONG_TASK_LIMIT,					//已有护送任务不能护送精华

		EN_JINGHUA_ALREADY_HAVE,						//你已经有精华可以护送，不必再采集
		EN_JINGHUA_GATHER_FLY_LIMIT,					//飞行时不能采集
		EN_FORBID_MAILGOLD,								//元宝不能寄送
		EN_MIAL_LOGIN_TIME_LIMIT,						//在线时长不足10分钟不能发邮件
		EN_TRADE_LOGIN_TIME_LIMIT,						//在线时长不足10分钟不能进行交易
		EN_BETRADE_LOGIN_TIME_LIMIT,					//对方在线时长不足10分钟,不能进行交易
		EN_PUBLICSALE_LOGIN_TIME_LIMIT,					//在线时长不足10分钟,不能进行市场交易
		EN_PUBLICSALE_HIGGHEST_PRICE,					//最大可寄售10万元宝
		EN_SHENZHUANG_MAX_LEVEL,						//神装已达到最大等级
		EN_MYSTERIOUSSHOP_BUY_NUM_LIMIT,				//可剩余购买数量不足
		EN_MYSTERIOUSSHOP_NPC_DISSAPPEAR,				//神秘商人已消失，请等下次刷新
		EN_MYSTERIOUSSHOP_ITEM_NOTEXIST,				//您要购买的物品不存在
		EN_CHANGE_CAMP_LIMIT_TIME,						//距上次转师门24小时内不能再转
		EN_CHANGE_CAMP_LIMIT_DUOBAOSCENE,				//在夺宝地图中不能转师门
		EN_CAMPEQUIP_LIMIT_CAMP,						//需要加入师门才能穿戴师门装备
		EN_CAMPNORMALDUOBAO_LIMIT_CAMP,					//加入师门后才能进入夺宝
		EN_CAMPDUOBAO_LIMIT_LEVEL,						//等级不足，不能进入
		EN_CAMPDUOBAO_COUNT_LIMIT,						//今日次数已用完
		EN_DIFF_WEEKDAY_TOTAL_CHONGZHI_HAS_FETCH,		//你已经领取过该奖励了
		EN_DIFF_WEEKDAY_TOTAL_CHONGZHI_NOT_ENOUGH,		//每日累充金额未达到领取条件
		EN_JINGLING_MAX_GRADE,							//宠物已提升最高阶
		EN_JINGLING_IMG_NOT_ACTIVE,						//形象未激活
		EN_JINGLING_EQUIP_MAX_STRENGTH,					//已达到最高等级
		EN_SHENZHUANG_JINJIE_MAX_LEVEL,					//神装进阶已达到最高等级
		EN_SHENZHUANG_SHENZHU_MAX_LEVEL,				//神铸已达到最高等级
		EN_ROLE_UPLEVEL_FAIL,							//人物升级失败
		EN_SHIMEN_HUNLIAN_MAX_LEVEL,					//魂炼已达到最高等级
		EN_PHANTOM_MAX_LEVEL,							//幻化已达最大等级
		EN_PHANTOM_IMG_NOT_ACTIVE,						//幻化形象未激活
		EN_PHANTOM_CARD_NOT_ENOUGH,						//材料不足
		EN_JINGLING_SOUL_MAX_LEVEL,						//聚灵已达最高等级
		EN_JINGLING_SOUL_NOT_ENOUGH,					//灵魄材料不足
		EN_LEVEL_NOT_ENOUGH_TO_SEND_EMAIL,				//等级不足199,不能发邮件!
		EN_LEVEL_NOT_ENOUGH_TO_SIGNAL_CHAT,				//等级不足199,不能进行私聊!
		EN_TUHAOJIN_LEVEL_LIMIT,						//土豪金已达最高级
		EN_TUHAOJIN_COLOR_NOT_ACTIVE,					//土豪金颜色未激活
		EN_BIG_CHATFACE_LEVEL_LIMIT,					//大表情已达最高级
		EN_SHENZHOU_WEAPON_SLOT_LEVEL_MAX,				//当前异火碎片已达到最大等级
		EN_SHENZHOU_WEAPON_ELEMENT_LEVEL_MAX,			//当前异火之魂已达到最大等级	
		EN_SHENZHOU_WEAPON_LINGSHU_LEVEL_MAX,			//当前魂槽已达到最大等级
		EN_SHENZHOU_WEAPON_WOLF_NOT_INLAY_LIMIT,		//该狼魂槽未镶嵌魂石，不能升级
		EN_SHENZHOU_WEAPON_LEOPARD_NOT_INLAY_LIMIT,		//该豹魂槽未镶嵌魂石，不能升级
		EN_SHENZHOU_WEAPON_BEAR_NOT_INLAY_LIMIT,		//该熊魂槽未镶嵌魂石，不能升级
		EN_SHENZHOU_WEAPON_LION_NOT_INLAY_LIMIT,		//该狮魂槽未镶嵌魂石，不能升级
		EN_SHENZHOU_WEAPON_TIGER_NOT_INLAY_LIMIT,		//该虎魂槽未镶嵌魂石，不能升级
		EN_SHENZHOU_WEAPON_KYLIN_NOT_INLAY_LIMIT,		//该麒麟魂槽未镶嵌魂石，不能升级
		EN_SHENZHOU_WEAPON_PHOENIX_NOT_INLAY_LIMIT,		//该凤凰魂槽未镶嵌魂石，不能升级
		EN_SHENZHOU_WEAPON_DRAGON_NOT_INLAY_LIMIT,		//该龙魂槽未镶嵌魂石，不能升级
		EN_SHENZHOU_WEAPON_WOLF_COLOR_LIMIT,			//狼魂槽强化已达瓶颈，镶嵌更高品质魂石方可突破
		EN_SHENZHOU_WEAPON_LEOPARD_COLOR_LIMIT,			//豹魂槽强化已达瓶颈，镶嵌更高品质魂石方可突破
		EN_SHENZHOU_WEAPON_BEAR_COLOR_LIMIT,			//熊魂槽强化已达瓶颈，镶嵌更高品质魂石方可突破
		EN_SHENZHOU_WEAPON_LION_COLOR_LIMIT,			//狮魂槽强化已达瓶颈，镶嵌更高品质魂石方可突破
		EN_SHENZHOU_WEAPON_TIGER_COLOR_LIMIT,			//虎魂槽强化已达瓶颈，镶嵌更高品质魂石方可突破
		EN_SHENZHOU_WEAPON_KYLIN_COLOR_LIMIT,			//麒麟魂槽强化已达瓶颈，镶嵌更高品质魂石方可突破
		EN_SHENZHOU_WEAPON_PHOENIX_COLOR_LIMIT,			//凤凰魂槽强化已达瓶颈，镶嵌更高品质魂石方可突破
		EN_SHENZHOU_WEAPON_DRAGON_COLOR_LIMIT,			//龙魂槽强化已达瓶颈，镶嵌更高品质魂石方可突破
		EN_SHENZHOU_WEAPON_HUNYIN_TYPE_LIMIT,			//该魂石无法镶嵌在当前魂槽	
		EN_SHENZHOU_WEAPON_LINGSHU_EXP_NOT_ENOUGH,		//灵性值不足		
		EN_SHENZHOU_WEAPON_HUNQI_LEVEL_LIMIT,			//异火等级不足	
		EN_SHENZHOU_WEAPON_JINGHUA_NOT_ENOUGH,			//异火精华不足	
		EN_SHENZHOU_WEAPON_IDENTIFY_LEVEL_ALEADY_MAX,	//已达到最大窥火鉴定等级			
		EN_SHENZHOU_WEAPON_EXCHANGE_TIME_MAX,			//今天兑换已经用完	
		EN_SHENZHOU_WEAPON_TODAY_GATHER_TIME_MAX,		//今天采集次数已达上限		
		EN_SHENZHOU_WEAPON_BUY_TODAY_GATHER_TIME_LIMIT,	//今天购买次数已达上限			
		EN_SHENZHOU_WEAPON_CANT_HELP_SELF_BOX,			//不能协助自己宝箱	
		EN_SHENZHOU_WEAPON_CANT_BOUBLE_HELP_SELF_BOX,	//不能重复协助			
		EN_SHENZHOU_WEAPON_OTHER_BOX_YET_OPEN,			//对方宝箱已经打开	
		EN_SHENZHOU_WEAPON_HELP_BOX_LIMIT,				//协助次数已经到达上限
		EN_SHENZHOU_WEAPON_XILIAN_OPEN_SLOT_LIMIT,		//当前属性槽已开启		
		EN_SHENZHOU_WEAPON_XILIAN_FREE_TIMES_LIMIT,		//每日免费炼火次数不足		
		EN_SHENZHOU_WEAPON_XILIAN_NOT_OPEN,				//当前异火淬炼槽未开启
		EN_SHENZHOU_WEAPON_HUNYIN_OPEN_LIMIT_1,			//前一个异火未激活，不能开启此魂槽
		EN_SHENZHOU_WEAPON_HUNYIN_OPEN_LIMIT_2,			//请先开启前一个魂槽
		EN_BABY_BIRTH_REQ_SED_SUCCESS,					//生宝宝请求发送成功
		EN_JINJIE_EQUIP_LEVEL_LIMIT,					//进阶等级不足
		EN_BABY_HAVE_TO_IN_SAME_TEAM,					//生宝宝需要夫妻组队
		EN_BABY_TEAM_MEMBER_NUM_INVALID,				//只能夫妻两人组队生宝宝
		EN_BABY_CAN_NOT_LITTLE_THREE,					//只能夫妻两人组队生宝宝
		EN_BABY_UGRADE_STUFF_NOT_ENOUGHT,				//进阶材料不足
		EN_BABY_RENAME_STUFF_NOT_ENOUGHT,				//改名卡不足
		EN_BABY_NOT_ENOUGH_LEVEL,						//您有宝宝未达到4阶，还不能继续生育哦！
		EN_BABY_COUNT_FULL,								//宝宝数已满
		EN_BABY_OTHER_COUNT_FULL,						//您的伴侣的宝宝数已满，不能再生宝宝
		EN_BABY_GRADE_LIMIT,							//宝宝学识已满
		EN_BABY_LEVEL_LIMIT,							//宝宝资质等级已满
		EN_BABY_LEVEL_NOT_ENOUGH_OTHER,					//您的伴侣等级还未达55级
		EN_BABY_LEVEL_NOT_ENOUGH,						//您的等级还未达到55级
		EN_BABY_UN_ACCEPT,								//您的伴侣取消了这次祈福
		EN_BABY_IS_OVERDUE,								//离婚后的宝宝不能培养
		EN_BABY_LOVER_NOT_ONLINE,						//伴侣不在线
		EN_BABY_COUNT_NOT_FULL,							//宝宝数未满，不需要超生
		EN_BABY_CHAOSHENG_LIMIT,						//宝宝超生名额已满
		EN_BABY_SPIRIT_LEVEL_FULL,						//宝宝守护宠物满级
		EN_BABY_SPIRIT_CONSUME_NOT_ENOUGH,				//宝宝守护宠物消耗物品不足
		EN_BABY_CAN_NOT_REMOVE_BABY_WHICH_AND_YOUR_LOVER,//不能丢弃与当前伴侣生的宝宝
		EN_SHIZHUANG_GRADE_LIMIT,						//时装进阶已达最大等级
		EN_PET_HAS_NOT_ACTIVE,							//灵器未激活
		EN_PET_HAS_ACTIVE,								//灵器已经激活
		EN_PET_EGG_NOT_ENOUGH,							//你的灵器召唤卡不足
		EN_PET_BACKPACK_FULL,							//你的灵器背包已经不够用了
		EN_PET_LEVEL_LIMIT,								//灵器已达最大等级
		EN_PET_GRADE_LIMIT,								//灵器已经满阶了
		EN_PET_NULL_REWARD,								//什么都没有抽到
		EN_PET_SKILL_ALEADY_LEARN,						//该技能已学习过
		EN_PET_SKILL_HAS_NOT_LEARN,						//该技能未激活
		EN_PET_CUR_PET_CANNOT_LEARN_THIS_SKILL,			//该技能不专属于当前出战灵器
		EN_PET_CUR_PET_THIS_SKILL_HAS_MAX_LEVEL,		//该技能已经是最高级
		EN_PET_CUR_PET_THIS_SKILL_NOT_MATCH_SLOT,		//该技能类型跟技能槽不匹配
		EN_PET_CUR_PET_THIS_SKILL_ARE_IN_USED,			//该技能已经装备过
		EN_PET_CUR_PET_HAS_NO_THIS_SKILL_SLOT,			//该宠物没有这个技能槽
		EN_PET_SPECIAL_IMG_ALREADY_ACTIVE,			    //灵器特殊形象已经激活
		EN_PET_SPECIAL_IMG_NOT_ACTIVE,				    //灵器特殊形象还未激活
		EN_PET_SPECIAL_IMG_UN_USE_SUCC,					//取消幻化成功
		EN_RA_MONSTER_DROP_ITEM_USE_TIMES_LIMIT,		//今日该物品使用次数已到达上限
		EN_RA_MONSTER_DROP_ITEM_USE_LIMIT,				//该物品只能在活动期间使用
		EN_MOUNT_SPECIAL_IMG_GRADE_LIMIT,				//坐骑特殊形象已达最高级
		EN_RA_MINE_EXCHANGE_REWARD_SEND_BY_MAIL,		//背包不足，挖矿兑换奖励已通过邮件发送
		EN_RA_MINE_FETCH_SERVER_REWARD_VIP_LIMIT,		//领取全服挖矿奖励VIP等级不足
		EN_RA_MINE_EXCHANGE_COUNT_NOT_ENOUGH,			//猎物不足，无法兑换
		EN_RA_MINE_FETCH_SERVER_REWARD_LEVEL_LIMIT,		//领取全服奖励等级不足
		EN_HUASHEN_ID_HAVE_ACTIVE,						//化神形象已激活
		EN_HUASHEN_ID_NOT_ACTIVE,						//化神形象未激活
		EN_HUASHEN_LEVEL_FULL,							//化神等级已满
		EN_HUASHEN_LEVEL_STUFF_NOT_ENOUGH,				//化神升级材料不足
		EN_HUASHEN_SPIRIT_LEVEL_FULL,					//化神宠物已满级
		EN_HUASHEN_GRADE_FULL,							//化神阶数已达最大
		EN_RA_COUPLE_HALO_ACTIVATE_STUFF_NOT_ENOUGH,	//夫妻光环激活材料不足
		EN_WING_SPECIAL_IMG_GRADE_LIMIT,				//特殊羽翼形象已满阶
		EN_PET_QINMI_STUFF_ID_NOT_ENOUGH,				//亲，没有足够的粉尘蕴养灵器了呢
		EN_PET_QINMI_LEVEL_FULL,						//宠物契合度已经满级
		EN_RA_FANFAN_WORD_REWARD_FULL,					//请先兑换字组奖励
		EN_RA_FANFAN_CARD_LIST_FULL,					//请先重置牌组
		EN_RA_FANFAN_WORD_NOT_ACTIVE,					//字组还没有被激活
		EN_CONTINUE_CHONGZHI_REWARD_NOT_ACTIVE,			//奖励未激活，不可以领取
		EN_CONTINUE_CHONGZHI_REWARD_HAS_FETCH,			//奖励已领取过
		EN_CONTINUE_CONSUME_REWARD_NOT_ACTIVE,			//奖励未激活，不可以领取
		EN_CONTINUE_CONSUME_NOT_REACH_NEED_CONSUME_GOLD,//您目前消费元宝不足，不可以领取该奖励
		EN_CONTINUE_CONSUME_NOT_CONSUME_ANY_GOLD,		//您今天没有消耗任何元宝，不可以领取该奖励
		EN_ARMY_DAY_EXCHANGE_LIMIT,						//已经该军旗兑换过了
		EN_ARMY_DAY_ACTIVITY_DEGREE_NOT_ENOUGH,			//活跃度不足
		EN_ARMY_DAY_FLAG_NOT_ENOUGH,					//军旗不足
		EN_PASTURE_SPIRIT_LEVEL_FULL,					//牧场宠物等级已满
		EN_PASTURE_SPIRIT_QUALITY_FULL,					//牧场宠物品质已满
		EN_PASTURE_SPIRIT_NOT_ACTIVE,					//牧场宠物还未激活
		EN_PASTURE_SPIRIT_POINT_NOT_ENOUGH,				//五行积分不足
		EN_CROSS_PASTURE_CD_NOT_ENOUGH,					//冷却时间未到
		EN_CROSS_PASTURE_DISTANCE_NOT_ENOUGH,			//距离太远
		EN_CROSS_PASTURE_GET_SCORE_TIMES_FULL,			//次数已满，无法获得积分
		EN_OTHER_XUNZHANG_STUFF_NOT_ENOUGH,				//勋章升级绑定材料不足
		EN_SHEN_ZHOU_HELP_BOX_FAIL,						//对方的宝箱协助次数已满
		EN_SHEN_ZHOU_INVITE_HELP_BOX_FAIL,				//没有可以协助你的玩家
		EN_SHEN_ZHOU_INVITE_HELP_BOX_SUCC,				//邀请协助成功

		EN_MULTI_MOUNT_NOT_ACTIVE,						//该坐骑未激活
		EN_MULTI_MOUNT_IMAGE_NOT_ACTIVE,				//该坐骑形象未激活
		EN_MULTI_MOUNT_ALEADY_MAX_GRADE,				//该坐骑已达到最高阶
		EN_MULTI_MOUNT_IS_NOT_RIDING,					//您还未上双人坐骑
		EN_MULTI_MOUNT_ALEADY_TOW_RIDE,					//您已经处于双人骑乘状态
		EN_MULTI_MOUNT_OWNER_CHANGE_INVITE,				//对方已经改邀请了其他人
		EN_MULTI_MOUNT_OWNER_NOT_RIDING,				//对方已下坐骑
		EN_MULTI_MOUNT_OWNER_NOT_INVITE,				//对方没有发出邀请
		EN_MULTI_MOUNT_OWNER_MOUNT_IS_FULL,				//对方已有骑乘伴侣
		EN_MULTI_MOUNT_PARTNER_TOO_FARAWAY,				//对方跟你相隔太远
		EN_MULTI_MOUNT_NORMAL_SCENE_CAN_INVITE,			//普通场景才可以邀请双人骑乘
		EN_MULTI_MOUNT_CANNOT_FLY,						//双人坐骑不能飞行哦
		EN_MULTI_MOUNT_MY_MOUNT_IS_FULL,				//您已有骑乘伴侣
		EN_MULTI_MOUNT_GRADE_IS_FULL,					//该坐骑已经是最高阶
		EN_MULTI_MOUNT_ID_INVALID,						//坐骑ID非法
		EN_MULTI_MOUNT_OTHER_CANNOT_MOUNT_ON,			//对方不能上坐骑
		EN_MULTI_MOUNT_SPECIAL_IMG_ALREADY_ACTIVE,	    //双人坐骑特殊形象已经激活
		EN_MULTI_MOUNT_SPECIAL_IMG_NOT_ACTIVE,		    //双人坐骑特殊形象还未激活
		EN_MULTI_MOUNT_EQUIP_ALEADY_MAX_LEVEL,			//该坐骑装备已达到最高等级
		EN_CIRCULATION_CHONGZHI_REWARD_NOT_ACTIVE,		//奖励未激活，不可以领取
		EN_CIRCULATION_CHONGZHI_NOT_CHONGZHI_ANY_GOLD,	//您今天没有充值任何元宝，不可以领取该奖励
		EN_CIRCULATION_CHONGZHI_NOT_REACH_NEED_CHONGZHI_GOLD,//您目前充值元宝不足，不可以领取该奖励
		EN_PERSONALIZE_BUBBLE_WINDOW_LEVEL_LIMIT,		//气泡框已达最大等级
		EN_PERSONALIZE_BUBBLE_WINDOW_NOT_ACTIVE,		//气泡框未激活
		EN_GUILD_DAY_INVITE_COUNT_LIMIT,				//每日仙盟邀请次数已用完
		EN_MAGIC_CARD_SLOT_ALEADY_HAS_CARD,				//你已经激活该卡牌
		EN_MAGIC_CARD_SPRITE_NOT_ENOUGH,				//魔魂不足
		EN_MAGIC_CARD_NOT_ENOUGH,						//魔卡不足
		EN_MAGIC_CARD_STRENGTH_LEVEL_FULL,				//该魔卡已满级
		EN_MAIGC_CARD_STRENGTH_LEVEL_NO_ENOUGH,			//有魔卡不够等级激活技能
		EN_CHESTSHOP_NOT_ENOUGH_SPACE,					//寻宝仓库已满
		EN_WUSHANG_EQUIP_NOT_ENOUGH,					//物品不足
		EN_WUSHANG_EQUIP_NOT_HAVE_EQUIP,				//该位置没有装备
		EN_WUSHANG_EQUIP_ALREADY_PUTON,					//已装备该物品
		EN_WUSHANG_EQUIP_JIFEN_NOT_ENOUGH,				//积分不足
		EN_WUSHANG_EQUIP_NOT_ACTIVE,					//无上神兵未激活
		EN_WUSHANG_EQUIP_STRONG_LEVEL_LIMIT,			//无上神兵强化等级已满
		EN_WUSHANG_EQUIP_STRONG_STUFF_NOT_ENOUGH,		//无上神兵强化材料不足
		EN_WUSHANG_EQUIP_STRONG_FAILD,					//无上神兵强化失败
		EN_WUSHANG_EQUIP_STAR_LEVEL_LIMIT,				//无上神兵星级已满
		EN_WUSHANG_EQUIP_STAR_STUFF_NOT_ENOUGH,			//无上神兵升星材料不足
		EN_WUSHANG_EQUIP_GLORY_NOT_ENOUGH,				//无上神兵荣耀不足
		EN_WUSHANG_EQUIP_JYGLORY_NOT_ENOUGH,			//无上神兵精英荣耀不足
		EN_JINGLING_HALO_LEVEL_LIMIT,					//宠物光环等级已满
		EN_CROSS_ACTIVITY_LEVEL_LIMIT,					//等级不足，不能参与该跨服
		EN_CROSS_BOSS_GATHER_TIME_LIMIT,				//今天已经没有采集次数
		EN_CROSSBOSS_GATHER_REDUCE_HP_NOTICE,			//采集时会百分比扣除当前生命值
		EN_SOMEONE_GATHER_ING,							//该采集物正在被其他人采集中
		EN_CROSS_HS_LEVEL_LIMIT,						//等级不足，不参加温泉活动
		EN_CROSS_HS_CANCEL_PARTNER,						//对方取消了幽会
		EN_CROSS_HS_FREE_PRESETN_TIME_LIMIT,			//免费送礼次数已用完
		EN_CROSS_HS_HAS_PARTNER,						//正在幽会中
		EN_CROSS_HS_NOT_IN_SCENES,						//对方不在当前场景
		EN_CROSS_HS_TARGET_HAS_PARTNER,					//该玩家正在幽会中
		EN_CROSS_HS_REJECT_PARTNER_INVITE,				//对方拒绝了你的邀请
		EN_CROSS_HS_SKILL_TIME_LIMIT,					//技能使用次数已用完
		EN_CROSS_HS_SHUANGXIU_CANNOT_MASSAGE,			//幽会中不能使用按摩技能
		EN_CROSS_HS_TARGET_SHUANGXIU_CANNOT_MASSAGE,	//该玩家正在幽会中
		EN_WING_FUMO_LEVEL_FULL,						//附魔等级已满
		EN_WING_FUMO_FAIL,								//羽翼附魔失败
		EN_MITAMA_LEVEL_FULL,							//该神侍等级已满
		EN_MITAMA_JINGHUN_LEVEL_FULL,					//该精魂等级已满
		EN_MITAMA_TASK_LEVEL_LIMIT,						//当前神侍等级不足，不能执行任务
		EN_MITAMA_EXCHANGE_SCORE_NOT_ENOUGH,			//当前温泉积分不足，不能兑换物品
		EN_MITAMA_TASK_CANT_FIGHTING,					//现在还不能出征神侍
		EN_HOT_SPRING_GAME_TIME_MAX,					//这次活动内玩游戏次数已到上限
		EN_HOT_SPRING_GAME_ALEADY_START,				//游戏已经正在进行
		EN_HOT_SPRING_GAME_ALEADY_STOP,					//游戏已经结束
		EN_FAIRY_TREE_MONEY_CANT_FETCH,					//抱歉，在线时间不足以领取仙树奖励
		EN_FAIRY_TREE_GIFT_CANT_FETCH,					//抱歉，在线时间不足以领取仙树礼包
		EN_FAIRY_TREE_FUNNY_FARM_SCORE_NOT_ENOUGH,		//抱歉，春耕农场积分不足
		EN_FAIRY_TREE_LEVEL_FULL,						//仙树等级已满
		EN_FAIRY_TREE_GRADE_FULL,						//仙树阶数已满
		EN_MAGIC_EQUIPMENT_HAS_ACTIVE,					//该魔器已经激活过
		EN_MAGIC_EQUIPMENT_HAS_NOT_ACTIVE,				//该魔器未激活
		EN_MAGIC_EQUIPMENT_ALEADY_MAX_LEVEL,			//该魔器已经达到最大等级
		EN_MAGIC_EQUIPMENT_ALEADY_HAS_STONE,			//该孔位已有魔石
		EN_CROSS_TEAM_FB_NOT_READY,						//有队员还未准备
		EN_CROSS_TEAM_FB_HAS_START,						//该房间已开始副本
		EN_MAGIC_EQUIPMENT_SLOT_HAS_NO_STONE,			//该孔位是空的
		EN_RA_BLACK_MARKET_CANT_OFFER_ITEM,				//抱歉，竞价失败了
		EN_RA_BLACK_MARKET_OFFER_GOLD_PRICE_LIMIT,		//您的出价低于当前竞拍价
		EN_QINGYUAN_HALO_LEVEL_LIMIT,					//光环已达最大等级
		EN_QINGYUAN_PRE_HALO_LEVEL_LIMIT,				//前置光环等级不足
		EN_RA_ZHENBAOSHANGCHENG_SCORE_NOT_ENOUGH,		//珍宝商城积分不足
		EN_XIANNV_NOT_ACTIVE,							//该仙女未激活
		EN_XIANNV_LEVEL_MAX,							//仙女等级已达最大
		EN_XIANNV_ZIZHI_MAX,							//仙女资质已达最大
		EN_XIANNV_HUANHUAN_LEVEL_MAX,					//仙女幻化等级已达最大
		EN_XIANNV_ALREADY_ACTIVE,						//已经激活该形象
		EN_ZHIBAO_HUANHUA_GARDE_MAX,					//幻化已达最大阶数
		EN_ZHIBAO_ALREADY_ACTIVE,						//已经激活该形象
		EN_WANGLINGEXPLORE_ENTER_LEVEL_LIMIT,			//等级不足不能进入
		EN_ZHIBAO_LEVEL_TOO_LARGE,						//圣物等级已满
		EN_MILLIONAIRE_HAD_TURN,						//转动次数已用完
		EN_OPEN_GAME_ACTIVITY_FETCHED,					//您已领取过该奖励
		EN_OPEN_GAME_ACTIVITY_COND_LIMIT,				//未达到领取条件
		EN_CHONGZHI_COND_LIMIT,							//未达到领取条件
		EN_CHONGZHI_HAS_FETCH,							//你已经领取过该奖励了
		EN_CHAT_LEVEL_LIMIT,							//物品使用等级不足
		EN_TW_WAR_IS_FINISH,							//征战已结束
		EN_TW_WAR_CREDIT_NO_ENOUGHT,					//积分不足
		EN_EXP_REFINE_TODAY_TIME_MAX,					//今日炼制已达最大次数

		EN_CROSS_TEAM_LEVEL_LIMIT,						//等级不足，不能参与该跨服
		EN_CROSS_TEAM_COUNT_LIMIT,						//今日跨服组队次数已达最大
		EN_CROSS_TEAM_HUSONG_LIMIT,						//护送中不能进行跨服组队操作

		EN_TW_WAR_TOWER_WUDI_CLOD_LIMIT,				//物品冷却中，不能购买
		EN_TW_WAR_TRANSPORT_NOT_TARGET,					//无可传送目标
		EN_TW_WAR_TRANSPORT_CENTER_LIMIT,				//未占领中央复活点，不能传送

		EN_MOVE_CHESS_ARRIVE_END_POINT,					//已到达终点
		EN_MOVE_CHESS_RESET_TIME_LIMIT,					//已达最大重置次数

		EN_CROSS_GUILDBATTLE_REWARD_HAD_FETCHED,		//该奖励已领取
		EN_CROSS_GUILDBATTLE_REWARD_FETCH_LIMIT,		//领取条件不足
		EN_CROSS_GUILDBATTLE_GUILD_LIMIT,				//必须有仙盟才能参与该活动
		EN_CROSS_GUILDBATTLE_GATHER_LIMIT,				//这个宝箱您已经采集过了
		EN_EXP_JADE_FETCH_TIME_LIMIT,					//您今天领取次数已满

		EN_WORLD_BOSS_REWARD_IS_GIVE,					//奖励已被取走
		EN_CROSS_XIULUO_TOWER_SCORE_NOT_ENOUGH,			//积分不足
		EN_CROSS_XIULUO_TOWER_SCORE_FETCHED,			//您已领取过该奖励
		EN_CROSS_SHUIJING_GATHER_TIMES_LIMIT,			//今天的采集次数已用完
		EN_CROSS_XIULUO_TOWER_REALIVE_TIME_LIMIT,		//复活时间未到
		EN_SHUIJING_GATHER_TIMES_LIMIT,					//今天的采集次数已用完

		EN_CHONGZHI_7DAY_TODAY_FETCH,					//今天已领取奖励
		EN_CHALLENGE_FIELD_BUFF_TIMES_LIMIT,			//已达到最大购买次数
		EN_FETCH_CONDITION_NOENOUGH,                    //不满足领取条件       

		//////////////////////////////////////  符文系统    ////////////////////////////////////
		EN_RUNE_SYSTEM_BAG_NO_ENOUGH_SPACE,				//战魂背包不足
		EN_RUNE_SYSTEM_MAX_LEVEL,						//战魂最高等级
		EN_RUNE_SYSTEM_NOR_ENOUGH_SCORE,				//战魂精华不足
		EN_RUNE_SYSTEM_AWAKEN_LIMIT,					//战魂格已达觉醒上限
		EN_RUNE_SYSTEM_COMPOSE_FAILE,					//合成失败
		EN_RUNE_SYSTEM_MAX_COMPOSE_QUALITY,				//该战魂不能合成
		EN_RUNE_SYSTEM_MAX_QUALITY,						//战魂最高品质
		EN_RUNE_SYSTEM_CAN_NOR_COMPOSE_DIFF_QUALITY,	//不同品质战魂不能合成
		EN_RUNE_SYSTEM_CAN_NOR_COMPOSE_DIFF_KIND,		//不同种类战魂不能合成
		EN_RUNE_SYSTEM_CAN_NOR_DECOMPOSE,				//该战魂不能分解
		EN_RUNE_SYSTEM_KIND_MAX_COUNT,					//该种类战魂已达最大装备数量
		EN_RUNE_SYSTEM_SLOT_NOT_OPEN,					//该战魂槽没有开启
		EN_RUNE_SYSTEM_ATTR_IS_SAME,					//不能装备属性相同的战魂
		EN_RUNE_SYSTEM_TYPE_IS_SAME,					//不能装备类型相同的战魂
		EN_RUNE_SYSTEM_SUIPIAN_NOT_ENOUGH,				//战魂碎片不足
		EN_RUNE_SYSTEM_ITEM_NOT_ENOUGH,					//物品不足
		EN_RUNE_SYSTEM_MAGIC_CRYSTAL_ENOUGH,			//战魂水晶数量不足
		EN_RUNE_SYSTEM_RUNE_NOT_OPEN,					//该战魂还未开启
		EN_RUNE_SYSTEM_RUNE_JINGHUA_NOT_PUTON,			//战魂精华不能装备
		EN_RUNE_SYSTEM_RUNE_COMPOSE_NUM_LESS,			//必须要有两个战魂
		EN_RUNE_TOWER_OFFLINE_TIME_MAX,					//离线时间已达最大
		EN_RUNE_TOWER_OFFLINE_TIME_FETCHED,				//今日已领取离线时间
		EN_RUNE_TOWER_LAYER_LIMIT,						//战魂塔已通关最高层

		EN_XINGZUOYIJI_GATHER_LIMIT,					//已达到最大采集数
		EN_NO_JOIN_TIMES,                               //没有参与次数


		//////////////////////////////////////  仙女守护、宠物法阵、宠物光环    ////////////////////////////////////

		EN_XIANNV_SHOUHU_NOT_EXIST,						//仙女守护不存在

		EN_JINGLING_FAZHEN_NOT_EXIST,					//宠物法阵不存在
		EN_JINGLING_FAZHEN_CANNOT_UPGRADE,				//宠物法阵满级，不能升阶

		EN_JINGLING_GUANGHUAN_NOT_EXIST,				//宠物光环不存在
		EN_JINGLING_GUANGHUAN_CANNOT_UPGRADE,			//宠物光环满级，不能升阶
		EN_JINGLING_ADVANTAGE_TIMES_LIMIT,				//今日可孵化宠物蛋次数已达上限
		EN_JINGLING_ADVANTAGE_LEVEL_LIMIT,				//等级不足，120级后再来孵化宠物蛋吧

		//////////////////////////////////////  转生系统    ////////////////////////////////////
		EN_ZHUANSHENG_BACKPACK_FULL,					//转生背包空间不足
		EN_ZHUANSHENG_LEVEL_FULL,						//转生等级已经满了
		EN_ZHUANSHENG_XIUWEI_NOT_ENOUGH,				//修为不足
		EN_ZHUANSHENG_CHANGE_TIMES_FULL,				//今天的兑换次数已满
		EN_ZHUANSHENG_NOT_HAVE_EQUIP,					//该位置没有装备
		EN_ZHUANSHENG_LEVEL_NOT_ENOUGH,					//转生等级不够
		EN_ZHUANSHENG_EQUIP_IS_BIND,					//转生装备已经绑定
		EN_ZHUANSHENG_IS_XIUWEI_DAN,					//这个是转生修为丹
		EN_ZHUANSHENG_KNAPSACK_NOT_ENOUGH, 				//人物背包空间不足
		EN_ZHUANSHENG_AUTO_CONBIME_EQUIP_NOT_ENOUGTH,	//物品不足

		//////////////////////////////////////  小宠物    ////////////////////////////////////
		EN_LITTLE_PET_NO_PET_INTERACT_TIMES,			//小宠物互动次数已满
		EN_LITTLE_PET_NO_PERSONAL_INTERACT_TIMES,		//个人互动次数已满
		EN_LITTLE_PET_NO_ENOUGH_QIANGHUA_DAN,			//小宠物强化丹不足
		EN_LITTLE_PET_MAX_FEED_DEGREE,					//小宠物饱食度已满
		EN_LITTLE_PET_MAX_NUM,							//小宠物数量已满
		EN_LITTLE_PET_NO_ENOUGH_SCORE,					//小宠物友善值不足
		EN_LITTLE_PET_CAN_NOT_INTERACT_SEFL_PET,		//不能和自己的小宠物互动


		//////////////////////////////////////  魔戒    ////////////////////////////////////
		EN_MOJIE_NOT_FINISH_PRETASK,					//没有完成前置任务
		EN_MOJIE_TASK_BEEN_FINISH,						//本副本已经通过

		//////////////////////////////////////  神兵    ////////////////////////////////////////
		EN_SHENBING_LEVEL_TOO_LARGE,					//神兵等级已满
		EN_SHENBING_SHUXINGDAN_USE_LIMIT_NUM,			//神兵属性丹已达上限

		//////////////////////////////////////  好友回礼    ////////////////////////////////////
		EN_FRIEND_GIFT_NOT_TIMES,						//今天对此好友的送礼次数已经用完
		EN_FRIEND_GIFT_SONG_SUCCESS,					//送礼成功
		EN_FRIEND_GIFT_RETURN_SUCCESS,					//回礼成功

		//////////////////////////////////////  装备合成    ////////////////////////////////////
		EN_EQUI_COMPOUND_LEVEL_LIMIT,					//等级不足，不能合成
		EN_EQUI_COMPOUND_STUFF_ERROR,					//该物品不能合成

		//////////////////////////////////////  副本引导   ////////////////////////////////////
		EN_TACK_FINISH_YET_ERROR,						//已经完成此任务
		EN_GOAL_FINISH_YET_ERROR,						//已经完成此目标

		//////////////////////////////////////  限时vip   ////////////////////////////////////
		EN_TIME_LIMIT_NOT_FETCH_REWARD,					//限时vip不可以领取奖励

		//////////////////////////////////////  经验副本  ////////////////////////////////////
		EN_EXP_FB_GUWU_LIMIT,							//鼓舞已经到了上限
		EN_EXP_FB_GUWU_GOLD_NOT_ENOUGH,					//自动鼓舞元宝不足
		EN_FB_ENTER_FB_CONDITMONS_ERROR,				//没有满足进入副本的条件
		EN_FB_ENTER_FB_IN_TEAM,							//当前在队伍中
		EN_FB_ENTER_FB_NOT_LEADER,						//你不是队长
		EN_FB_ENTER_FB_PLEAR_TEAM,						//请和你的好友一起组队进入
		EN_FB_ENTER_FB_NOT_TIMES,						//次数不足，无法进入经验副本
		EN_FB_ENTER_FB_TEAM_NUM_INSU,					//队伍人数不足
		EN_FB_EXP_TIME_LIMIT,							//副本冷却时间未到，无法进入经验副本
		EN_FB_EXP_PAY_TIME_LIMIT,						//你当前VIP等级，可购买的次数已达上限！
		EN_FB_EXP_PAY_TIME_VIP_LIMIT,					//你当前VIP等级，可购买的次数已达上限！
		EN_FB_ROLE_LEVEL_LIMIT_EXP_FB,					//等级不足，无法进入经验副本
		EN_TASK_NOT_COMPLETED_EXP_FB,					//前置任务未完成，无法进入经验副本

		//////////////////////////////////////  组队装备副本  ////////////////////////////////////
		EN_ENTER_TEAM_EQUIP_FB_TEAM_LIMIT,				//不在队伍中，不能进入副本
		EN_ENTER_TEAM_EQUIP_FB_LEVEL_LIMIT,				//等级不足，不能进入副本
		EN_ENTER_TEAM_EQUIP_FB_LEADER_LIMIT,			//您不是队长，不能创建副本
		EN_TEAM_EQUIP_FB_BUY_TIME_VIP_LIMIT,			//你当前VIP等级，可购买的次数已达上限！

		//////////////////////////////////////  组队装备副本  ////////////////////////////////////
		EN_SHENGE_SYSTEM_BAG_NO_ENOUGH_SPACE,			//星辉背包不足
		EN_SHENGE_SYSTEM_MAX_LEVEL,						//星辉最高等级
		EN_SHENGE_SYSTEM_NOR_ENOUGH_SCORE,				//星辉精华不足
		EN_SHENGE_SYSTEM_COMPOSE_FAILE,					//合成失败
		EN_SHENGE_SYSTEM_MAX_COMPOSE_QUALITY,			//该星辉不能合成
		EN_SHENGE_SYSTEM_MAX_QUALITY,					//星辉最高品质
		EN_SHENGE_SYSTEM_CAN_NOR_COMPOSE_DIFF_QUALITY,	//不同品质星辉不能合成
		EN_SHENGE_SYSTEM_CAN_NOR_COMPOSE_DIFF_KIND,		//不同种类星辉不能合成
		EN_SHENGE_SYSTEM_CAN_NOR_DECOMPOSE,				//该星辉不能分解
		EN_SHENGE_SYSTEM_KIND_MAX_COUNT,				//该种类星辉已达最大装备数量

		EN_TIAN_XIANG_BEAN_NUM_LIMIT,					//天象星魂数量达到上限
		EN_XIANNV_SHENGWU_EXP_HAS_NOT_FETCH,			//请先领取了上次的奖励
		EN_XIANNV_SHENGWU_LEVEL_HAS_FULL,				//已达到最高级
		EN_XIANNV_COUNT_NOT_ENOUGH,						//仙女数量不足
		EN_XIANNV_SHENGWU_GRID_LINGYE_NOT_ENOUGH,		//灵液不足以升级
		EN_XIANNV_SHENGWU_GRID_DEPEND_LEVEL_LIMIT,		//请先升级前置节点的等级
		EN_XIANNV_SHENGWU_ALL_FULL_LEVEL,				//所有圣物都已达到最高级
		EN_XIANNV_SHENGWU_ESSENCE_NOT_ENOUGH,			//仙器精华不足
		EN_XIANNV_SHENGWU_DAY_FETCH_LING_TIMES_LIMIT,	//今日引灵次数已经到达上限
		EN_SHENGE_SYSTEM_ZHANGKONG_FULL_LEVEL,			//星辉已经满级


		////////////////////////////////////// 好友经验瓶 ////////////////////////////////////
		EN_EXP_BOTTLE_NOT_ACTIVE,						//经验瓶未激活
		EN_EXP_BOTTLE_NOT_FULL,							//经验瓶未满
		EN_EXP_BOTTLE_FRIEND_NOT_ENOUGH,				//好友数量不足

		//////////////////////////////////////  推图副本  ////////////////////////////////////
		EN_TUITU_FB_UNLOCK_LAST_LEVEL_LIMIT,			//请先通关上一个关卡
		EN_TUITU_FB_FULL_STAR_PASS_FIRST,				//请先三星通过上一关卡
		EN_TUITU_FB_ENTER_TIMES_LIMIT,					//进入副本次数不足
		EN_TUITU_FB_BUY_TIMES_NOT_ENOUGH,				//副本剩余购买次数不足
		EN_TUITU_FB_CHAPTER_STAR_NUM_NOT_ENOUGH,		//副本章节总星数不足，无法领取
		EN_TUITU_FB_CANT_SAODANG,						//副本扫荡条件未满足
		EN_TUITU_FB_CANT_SAODANG_ENTER_TIMES_LIMIT,		//进入副本次数不足无法扫荡
		EN_TUITU_FB_ALL_COMPLETE,						//已通关所有章节

		EN_FIGHTING_MINING_QUALITY_FULL,				//已经是最高品质了
		EN_FIGHTING_MINING_ALEADY,						//您的工人当前还在挖矿中
		EN_FIGHTING_MINING_TIMES_LIMIT,					//您今天挖矿次数已达上限
		EN_FIGHTING_MINING_BEEN_ROB_LIMIT,				//对方很惨了，别再抢他了
		EN_FIGHTING_MINING_ROB_LIMIT,					//您今天的掠夺次数已经达到上限了
		EN_FIGHTING_MINING_FETCH_TIME_LIMIT,			//时间未到，无法领取奖励
		EN_FIGHTING_MINING_REFLUSH_FAIL,				//刷新失败
		EN_FIGHTING_MINING_ALEADY_ROB,					//已经掠夺过对方了
		EN_FIGHTING_MINING_TIME_END,					//对方的矿已经开采完毕

		EN_FIGHTING_SAILING_QUALITY_FULL,				//已经是最高品质了
		EN_FIGHTING_SAILING_ALEADY,						//您的工人当前还在航海中
		EN_FIGHTING_SAILING_TIMES_LIMIT,				//您今天航海次数已达上限
		EN_FIGHTING_SAILING_BEEN_ROB_LIMIT,				//对方很惨了，别再抢他了
		EN_FIGHTING_SAILING_ROB_LIMIT,					//您今天的掠夺次数已经达到上限了
		EN_FIGHTING_SAILING_FETCH_TIME_LIMIT,			//时间未到，无法领取奖励
		EN_FIGHTING_SAILING_REFLUSH_FAIL,				//刷新失败
		EN_FIGHTING_SAILING_ALEADY_ROB,					//已经掠夺过对方了
		EN_FIGHTING_SAILING_TIME_END,					//对方的船已经靠岸

		EN_FIGHTING_CHALLENGE_ALEADY_WIN,				//已经战胜过此对手
		EN_FIGHTING_CHALLENGE_LEFT_TIMES_LIMIT,			//已经没有挑战次数了
		EN_FIGHTING_VIP_BUY_LIMIT,						//你当前VIP等级，可购买的次数已达上限！

		//////////////////////////////////////  珍宝阁    ///////////////////////////////////
		EN_ZHENBAOGE_HAD_REFRESH,						//宝牌刷新成功
		EN_ZHENBAOGE_NOT_REFRESH,						//该宝牌尚未刷新

		//////////////////////////////////////	金猪召唤	  ///////////////////////////////////
		EN_GOLDEN_PIG_EXIST,							//该boss已被召唤

		//////////////////////////////////////  开服投资   //////////////////////////////////
		EN_OPEN_SERVER_INVEST_REWARD,					//该奖励已被领取
		EN_OPEN_SERVER_INVEST_LAST_REWARD,				//请先领取上一层奖励
		EN_OPEN_SERVER_INVEST_UNFINISH,					//尚未完成投资目标
		EN_OPEN_SERVER_INVEST_HAD_INVEST,				//已进行该投资
		EN_OPEN_SERVER_INVEST_NOT_INVEST,				//尚未进行投资

		//////////////////////////////////////  聚划算   //////////////////////////////////
		EN_RA_XIANYUAN_TREAS_ALREADY_BUY,				//该礼包已经购买过了
		EN_RA_XIANYUAN_TREAS_PASS_BUY_DAY,				//已经超过了可购买天数
		EN_RA_XIANYUAN_TREAS_HAVE_ALL_BUY,				//礼包已经全部购买了
		EN_RA_XIANYUAN_TREAS_NO_BUY,					//礼包还没有购买
		EN_RA_XIANYUAN_TREAS_ALREADY_FETCH,				//今天已经领取过该奖励了
		EN_RA_XIANYUAN_TREAS_GIFT_FULL,					//礼包领取次数已满

		//////////////////////////////////////  限时秒杀   //////////////////////////////////
		EN_RA_RUSH_BUYING_NOT_IN_TIME,					//限时秒杀抢购时间未到
		EN_RA_RUSH_BUYING_SERVER_BUY_TIMES_FULL,		//该物品已经被抢购一空了
		EN_RA_RUSH_BUYING_ROLE_BUY_TIMES_FULL,			//你已经不能再抢购该物品了

		///////////////////////////////////// 消费有礼 ////////////////////////////////////
		EN_RA_CONSUME_FOR_GIFT_POINTS_NOT_ENOUGH,		//您的积分不足，尚不能兑换该物品


		//////////////////////////////////////  情缘圣地   //////////////////////////////////
		EN_QING_YUAN_SHENG_DI_TIME_LIMIT,				//情缘圣地未到开启时间
		EN_QING_YUAN_SHENG_DI_LEVEL_LIMIT,				//结婚等级不足无法进入
		EN_QING_YUAN_SHENG_DI_LAYER_LIMIT,				//您的结婚等级过高，无法进入场景
		EN_QING_YUAN_SHENG_DI_TASK_FETCHED,				//已领取过该奖励
		EN_QING_YUAN_SHENG_DI_TASK_NOT_COMPLETE,		//任务未完成

		//////////////////////////////////////  秘藏boss	  /////////////////////////////////
		EN_PRECIOUS_BOSS_IN_SAVE_AREA,					//中心安全区不允许PK

		//////////////////////////////////////  神兽	      /////////////////////////////////
		EN_SHENSHOU_BACKPACK_NOT_ENOUGH,				//龙器背包不足
		EN_SHENSHOU_ITEM_IS_NOT_EQUIP,					//该物品不是装备
		EN_SHENSHOU_ZHUZHAN_COUNT_LIMIT,				//助战位不足
		EN_SHENSHOU_NOT_ACTIVE,							//该龙器未激活
		EN_SHENSHOU_QUALITY_NOT_ENOUGH,					//不符合品质要求
		EN_ITEM_NOT_ENOUGH,								//物品不足
		EN_SHENSHOU_SCORE_NOT_ENOUGH,					//唤龙积分不足
		EN_SHENSHOU_TIMES_LIMIT,						//唤龙次数已达上限

		//////////////////////////////////////  圣器	      /////////////////////////////////
		EN_SHENGQI_OPERATION_FAIL,						//系统异常，请稍后重试
		EN_SHENGQI_SPIRIT_MAX,							//附灵已满级

		/////////////////////////////////////// 合服装备  ///////////////////////////////////
		EN_CSA_EQUIP_MAX_LEVEL,							//装备已达最大等级

		/////////////////////////////////////// 双人坐骑  ///////////////////////////////////
		EN_FOLLOWER_LEVEL_NOT_ENOUGH,					//你的跟随者等级不足，无法进入该场景

		EN_SHENQU_XILIAN_CAP_NOT_ENOUGH,				//前一个融星战力不足，无法修炼此融星

		/////////////////////////////////////// 天象系统 ////////////////////////////////////
		EN_SHEN_YIN_BEAN_NUM_LIMIT,						//铭纹源珠数量达到上限
		EN_SHEN_YIN_XILIAN_UNSUCCESS,					//精炼失败
		EN_ITEM_NOT_PUT_IN_BAG,							//该物品不能放入铭纹背包
		EN_SHEN_YIN_PROTECT_NOT_ENOUGH,					//保护符不足
		EN_SHEN_YIN_UPLEVEL_UNSUCCESS,					//注灵失败
		EN_SHEN_YIN_MAX_LEVEL,							//铭纹已达最高等级

		EN_RA_PLANTING_TREE_ACTIVITY_NOT_BEGIN,			//捕捉玉兔活动还没开始呢
		EN_RA_PLANTING_TREE_CANT_PLANTING,				//您不能在这里召唤玉兔
		EN_RA_PLANTING_TREE_PLANTING_COUNT_FULL,		//您捕捉了好多玉兔了，休息一会儿吧
		EN_RA_PLANTING_TREE_CANT_WATERING,				//您已经捕捉过这只玉兔了
		EN_RA_PLANTING_TREE_NOT_SUIT_SCENE,				//请前往祖龙神都召唤玉兔

		EN_ELEMENT_HEART_ALREADY_ACTIVE,				//该五灵已经激活
		EN_ELEMENT_HEART_NOT_ACTIVE,					//该五灵尚未激活
		EN_ELEMENT_HEART_LAST_ELEMENT_HEART_NOT_ACTIVE,	//需要激活前置五灵
		EN_ELEMENT_HEART_LAST_ELEMENT_HEART_LIMIT_GRADE,//五灵已经达到最大等级
		EN_ELEMENT_HEART_LAST_ELEMENT_HEART_LIMIT_FEED,	//该食物已经达到最大喂养度
		EN_ELEMENT_HEART_LAST_CAN_NOT_GET_PRODUCT_TIME,	//还没有到达采集时间
		EN_ELEMENT_HEART_KNAPSACK_NO_SPACE,				//背包空间不足
		EN_ELEMENT_HEART_NOT_ACTIVE_GHOST,				//请先激活五灵
		EN_ELEMENT_HEART_FUZHOU_MAX_GRADE,				//五灵之纹已达最大等级
		EN_ELEMENT_TEXTURE_RESTRAINT,					//五灵相克，不能进行升级
		EN_ELEMENT_HEART_RESTRAIN,						//五灵相克不能喂养
		EN_ELEMENT_HEART_MAX_WUXING_ZHI,				//已到达五灵值最大值
		EN_ELEMENT_HEART_NO_FOOD,						//没有可以喂养的食物
		EN_ELEMENT_HEART_ITEM_NOT_ENOGH,				//物品材料不足

		EN_ELEMENT_HEART_EQUIP_UPGRADE_ALEADY_PUT_SLOT,	//该格子已经激活
		EN_ELEMENT_HEART_EQUIP_ITEM_NOT_ENOUTH,			//所需物品不足
		EN_ELEMENT_HEART_EQUIP_UPGRADE_ALEADY_MAX_LEVEL,//已经达到最高
		EN_ELEMENT_EQUIP_CANNOT_UPGRADE,				//装备不能进阶
		EN_ELEMENT_EQUIP_UPGRADE_HAS_SLOT_NOT_ACTIVE,	//晶石未镶嵌满，不能提升品质
		EN_ELEMENT_NO_EMPTY_GRID,						//背包格子不足
		EN_ELEMENT_HEART_XILIAN_NOT_OPEN,				//洗练槽未开启
		EN_ELEMENT_HEART_FEED_LEVEL_NOT_ENOGH,			//喂养等级不足
		EN_ELEMENT_HEART_LAST_ELEMENT_GRADE_NOT_ENOUGH,	//前置进阶等级不足

		//////////////////////////////////////  升星助力    ////////////////////////////////////
		EN_SHENGXINGZHULI_FETCH_TOO_MUCH,				//当前无可提升星数
		EN_SHENGXINGZHULI_CHONGZHI_NOT_ENOUGH,			//今天的充值数额还不够!

		EN_SHEN_YIN_LIEMING_LIEHUN_POOL_FULL,			//凝纹池已满
		EN_FABAO_CANNOT_UPGRADE,                        //法宝已达最大阶数
		EN_FABAO_IMG_NOT_ACTIVE,                        //法宝形象未激活
		EN_FABAO_IMG_HAS_ACTIVE,                        //法宝形象已经激活
		EN_FABAO_SHUXINGDAN_USE_LIMIT_NUM,              //法宝属性丹(资质丹)已达上限
		EN_SHIZHUANF_SHUXINGDAN_USE_LIMIT_NUM,          //时装属性丹(资质丹)已达上限
		EN_SHIZHUANF_SHENBING_SHUXINGDAN_USE_LIMIT_NUM, //神兵属性丹(资质丹)已达上限
		EN_FABAO_EQUIP_LEVEL_MAX,                       //法宝装备等级达到最大
		EN_FABAO_UP_ONE_GRADE_LIMIT,					//法宝阶级不满足
	
		//////////////////////////////////////  飞仙装备  ////////////////////////////////////
		EN_FEIXIAN_EQUIP_NOEXIT,						//装备不存在
		EN_FEIXIAN_EQUIP_TYPEERR,						//装备类型错误
		EN_FEIXIAN_EQUIP_COMP_TWICE,					//不能重复合成
		EN_FEIXIAN_EQUIP_COMP_DIFF,						//两个装备不一样
		EN_FEIXIAN_EQUIP_COMP_COLORERR,					//颜色错误
		EN_FEIXIAN_EQUIP_COMP_STARERR,					//星星数不够
		EN_FEIXIAN_EQUIP_UP_ERR,						//升级添加装备异常
		EN_SHIZHUANG_IMG_GRADE_LIMIT,					//时装进阶已达最大等级

		//////////////////////////////////////  1v1 竞技场补充    /////////////////////////////////
		EN_CHALLENGE_FIELD_RANK_POS_LIMIT,				//竞技场排名超出奖励排名
		EN_CHALLENGE_FIELD_IS_CLOSED,					//论剑已结束
			

		//////////////////////////////////////  境界进阶系统  /////////////////////////////////
		EN_JINGJIE_LEVEL_MAX,							//已达最高境界
		EN_JINGJIE_CAP_LIMIT,							//未达到提升条件


		/////////////////////////////////////// 仙尊卡 ////////////////////////////////////
		EN_XIANZUNKA_HAS_ACTIVE,						//仙尊卡已激活
		EN_XIANZUNKA_NOT_ACTIVE,						//仙尊卡未激活
		EN_XIANZUNKA_DAILY_REWARD_HAS_FETCH,			//每日奖励已领取
		EN_XIANZUNKA_ACTIVE_ITEM_LACK,					//您身上没有仙尊卡

		//////////////////////////////////////  天赋 /////////////////////////////////
		EN_TALENT_ROLE_LEVEL_LIMIT,						//当前等级不足，请努力升级
		EN_TALENT_IS_MAX_LEVEL,							//当前天赋已达到最大等级
		EN_TALENT_NO_TALENT_POINT,						//天赋点数不足
		EN_TALENT_OTHER_TALENT_LEVEL_LIMIT,				//未达到升级要求
		EN_TALENT_OTHER_TALENT_PROFLEVEL_LIMIT,			//未达到五转要求

		////////////////////////////////////// 天赋进阶 //////////////////////////////////////
		EN_TALENT_NOT_FOUNT_TALENT,						//没有这个天赋
		EN_TALENT_TALENT_GRID_ERROR,					//天赋格子错误
		EN_TALENT_GRID_NOT_ACTIVATE,					//天赋格未激活
		EN_TALENT_GRID_SKILL_EXISTS,					//天赋格已存在技能
		EN_TALENT_GRID_SKILL_NOT_EXISTS,				//天赋格不存在技能
		EN_TALENT_SKILL_ITEM_NOT_ENOUTH,				//天赋技能物品不足
		EN_TALENT_NOT_SKILL_BOOK,						//没有这个技能书
		EN_TALENT_FLUSH_NOT_CONFIG,						//未配置扣除元宝
		EN_TALENT_KNAPSACK_GRID_NO_LESS,				//背包格子不足
		EN_TALENT_NOT_CHOUJIANG_GRID,					//没有这个抽奖格子
		EN_TALENT_NOT_CHOUJIANG_SKILL,					//抽奖格没有技能
		EN_TALENT_GRID_SKILL_TYPE_EXISTS,				//技能类型已存在
		EN_TALENT_FINAL_SKILL_TYPE_ERROR,				//终极技能类型错误
		EN_TALENT_FINAL_SKILL_GRID_ERROR,				//终极技能格子错误
		EN_TALENT_FINAL_NOT_CAN_UPGRADE,				//天赋技能不能升级
		EN_TALENT_ITEM_NOT_ENOUGH,						//材料不足
		EN_TALENT_NEED_LEARN_JUNIOR_SKILL,				//需要学习低等技能书
		EN_TALENT_SKILL_ALREADY_ATTENTION,				//已经关注过该技能
		EN_TALENT_SKILL_ATTENTION_TOO_MUCH,				//关注技能太多啦
		EN_TALENT_SKILL_NOT_ATTENTION,					//尚未关注过该技能
		///////////////////////////////////// 建塔副本 /////////////////////////////////////
		EN_BUILD_TOWER_TOO_FAR,							//距离太远
		EN_BUILD_TOWER_LOW_DOUHUN,						//塔魂不足
		EN_CUR_WAVE_HAS_CALL_EXTRA,						//当前不能召唤鬼蟾王

		/////////////////////////////////////	 腰饰	///////////////////////////////////////////////
		EN_YAOSHI_NOT_EXIST,							//腰饰未激活
		EN_YAOSHI_CANNOT_UPGRADE,						//腰饰已达最大阶
		EN_YAOSHI_UP_ONE_GRADE_LIMIT,					//腰饰阶级不满足
		EN_YAOSHI_IMG_NOT_ACTIVE,						//形象未激活
		EN_YAOSHI_SPECIAL_IMG_GRADE_LIMIT,				//腰饰特殊形象已达最高级
		EN_YAOSHI_SHUXINGDAN_USE_LIMIT_NUM,				//腰饰资质丹使用数量已达上限

		/////////////////////////////////////	 头饰	///////////////////////////////////////////////
		EN_TOUSHI_NOT_EXIST,							//头饰未激活
		EN_TOUSHI_CANNOT_UPGRADE,						//头饰已达最大阶
		EN_TOUSHI_UP_ONE_GRADE_LIMIT,					//头饰阶级不满足
		EN_TOUSHI_IMG_NOT_ACTIVE,						//形象未激活
		EN_TOUSHI_SPECIAL_IMG_GRADE_LIMIT,				//头饰特殊形象已达最高级
		EN_TOUSHI_SHUXINGDAN_USE_LIMIT_NUM,				//头饰资质丹使用数量已达上限

		/////////////////////////////////////	 麒麟臂	///////////////////////////////////////////////
		EN_QILINBI_NOT_EXIST,							//麒麟臂未激活
		EN_QILINBI_CANNOT_UPGRADE,						//麒麟臂已达最大阶
		EN_QILINBI_UP_ONE_GRADE_LIMIT,					//麒麟臂阶级不满足
		EN_QILINBI_IMG_NOT_ACTIVE,						//形象未激活
		EN_QILINBI_SPECIAL_IMG_GRADE_LIMIT,				//麒麟臂特殊形象已达最高级
		EN_QILINBI_SHUXINGDAN_USE_LIMIT_NUM,			//麒麟臂资质丹使用数量已达上限

		/////////////////////////////////////	 面具	///////////////////////////////////////////////
		EN_MASK_NOT_EXIST,								//面具未激活
		EN_MASK_CANNOT_UPGRADE,							//面具已达最大阶
		EN_MASK_UP_ONE_GRADE_LIMIT,						//面具阶级不满足
		EN_MASK_IMG_NOT_ACTIVE,							//形象未激活
		EN_MASK_SPECIAL_IMG_GRADE_LIMIT,				//面具特殊形象已达最高级
		EN_MASK_SHUXINGDAN_USE_LIMIT_NUM,				//面具资质丹使用数量已达上限

		////////////////////////////钓鱼//////////////////////////////////////////////
		EN_FISHING_KNAPSACK_SPACE_NOT_ENOUGH,			//请先清理一下背包再来钓鱼吧
		EN_FISHING_NOT_FISHING_AREA,					//抱歉，这里不能钓鱼
		EN_FISHING_FISH_BAIT_NOT_ENOUGH,				//鱼饵不足，快去补充一下吧
		EN_FISHING_CANT_CASTING_RODS,					//现在还不能抛竿呢
		EN_FISHING_CANT_PULL_RODS,						//现在还不能拉竿
		EN_FISHING_CANT_CONFIRM_RESULT,					//现在还不能确认
		EN_FISHING_GET_FISH_FAIL,						//很遗憾，鱼儿溜走了
		EN_FISHING_TREASURE_REWARD_NO_SPACE,			//背包空间不足，已经邮件发放宝箱奖励
		EN_FISHING_GEAR_NOT_ENOUGH,						//法宝不足，不能使用
		EN_FISHING_OIL_NOT_ENOUGH,						//香油不足，无法使用
		EN_FISHING_SPEAR_NOT_ENOUGH,					//鱼叉不足，无法使用
		EN_FISHING_NET_NOT_ENOUGH,						//渔网不足，无法使用
		EN_FISHING_BIG_FISH_ALREADY_HELPED,				//已经帮忙拉钩了
		EN_FISHING_BIG_FISH_PULL_FAIL,					//你猛地一拉竿，大鱼溜走了
		EN_FISHING_THE_HELPED_PULLED,					//队友已经拉杆，你没能帮上忙
		EN_FISHING_CONFIRM_GET_FISH_MEMBER_NOT_FISHING,	//队友没在钓鱼，给您收下了
		EN_FISHINT_STEAL_TARGET_NOT_IN_SCENE,			//对方已离开场景，无法偷鱼
		EN_FISHINT_STEAL_COUNT_LIMIT,					//您的偷鱼次数已满
		EN_FISHINT_BE_STEALED_COUNT_LIMIT,				//对方的被偷次数已满
		EN_FISHINT_STEAL_FISH_FAIL,						//偷鱼失败
		EN_FISHING_STEAL_TARGET_HAS_NO_FISH,			//对方已没有鱼可偷
		EN_FISHING_FISH_NOT_ENOUGH,						//鱼儿不足，无法兑换
		EN_FISHING_QUICK_FISHING_HAS_ACTIVED,			//你已经购买过了，无需再次购买
		EN_FISHING_FISHING_AREA_HAS_OTHER_ROLE,			//请稍微远一点钓鱼，不要打扰到其他玩家哦
		EN_FISHING_REWARD_SOCRE_NOT_ENOUGH,				//积分不足，无法领取奖励

		//////////////////////////////////////  符文系统补充    /////////////////////////////////
		EN_RUNE_SYSTEM_RUNE_WUSHIYIJI_EQUIP_FAIL,	//“无视一击”战魂只能装备在绝版槽位
		EN_RUNE_SYSTEM_SLOT_LIMIT,					//该战魂槽只能镶嵌无视一击战魂

		//////////////////////////////////////  神兽补充    /////////////////////////////////
		EN_SHENSHOU_EQUIP_STRENGTH_LEVEL_MAX,			//龙器装备强化达到最高等级

		//////////////////////////////////////河神洛书/////////////////////////////////
		EN_HESHENG_LUOSHU_YIJING_ALREADY_ACTIVATED,		//已经激活
		EN_HESHENG_LUOSHU_YIJING_NOT_ITEM,				//物品不足
		EN_HESHENG_LUOSHU_YIJING_NOT_ACTIVATED,			//没有激活
		EN_HESHENG_LUOSHU_YIJING_UPGRADE_MAX,			//已满级
		EN_HESHENG_LUOSHU_YIJING_NOT_MAX_LEVEL,			//没有满级
		EN_HESHENG_LUOSHU_YIJING_MAX_LEVEL,				//神化已满级

		////////////////////////////////////// 进阶系统奖励 //////////////////////////////////
		EN_JINJIESYS_REWARD_OWNED,						//您已经获得该形象
		EN_SPECIAL_JINGLING_REWARD_OWNED,				//您已经获得超级宠物激活卡，无法继续购买！


		//////////////////////////////////////  转职任务  /////////////////////////////////
		EN_DAILY_WORK_EXP_NOT_ENOUGH,					//经验值不足

		////////////////////////////////////// 小鬼守护  /////////////////////////////////
		EN_INVALID_IMP_GUARD,                            //小鬼已失效

		////////////////////////////////////// 帮派争霸  /////////////////////////////////
		EN_OPEN_GAME_ACTIVITY_GUILD_BATTLE_NOT_END,		//您没有参与帮派争霸活动

		////////////////////////////////////// 符文系统（补充）  /////////////////////////////////
		EN_RUNE_TOWER_LAYER_NOT_ENOUGH,					//战魂塔未达到层数

		//////////////////////////////// 衣柜   //////////////////////////////////////////////
		EN_DRESSING_ROOM_EXCHANGE_ITEM_NOT_ENOUGH,		//兑换物不足

		////////////////////////////神器//////////////////////////////////////////////
		EN_SHENQI_MAX_QUALITY_LIMIT,					//只能镶嵌更高品质的神器！
		EN_SHENQI_SHENBING_NOT_ACTIVE,					//神兵尚未激活不能进行升级
		EN_SHENQI_BAOJIA_NOT_ACTIVE,					//宝甲尚未激活不能进行升级
		EN_SHENQI_NOT_SUIT,								//穿戴齐当前神兵（宝甲）即可升级		
		EN_SHENQI_NOT_ACHIEVE_MAX_QUALITY,				//套装未达到最大品质
		EN_WUQI_HAS_ACTIVE,                             //神兵已经激活

		//////////////////////////// 神兵图录 //////////////////////////////////////////////
		EN_RED_EQUIP_COLLECT_TAKEON_LIMIT,				//该套装尚未激活，不能穿戴
		EN_RED_EQUIP_COLLECT_NOT_ACTIVE_FECTCH_REWARD,	//套装未激活,无法领取称号奖励
		EN_RED_EQUIP_COLLECT_HAS_FETCH_REWARD,			//该奖励称号已经领取过

		//////////////////////////////////////	组合购买	///////////////////////////////////
		EN_RA_COMBINE_BUY_TOO_MUCH_ITEM,				//购物车物品过多
		EN_RA_COMBINE_BUY_NO_ITEM,						//购物车是空的
		EN_RA_COMBINE_BUY_ITEM_TOO_MUCH,				//购买物品数量超过上限

		//////////////////////////////////////	天天返利	///////////////////////////////////
		EN_DAY_CHONGZHI_REWARD_HAD_FETCH_REWARD,		//已领取过该奖励
		EN_DAY_CHONGZHI_REWARD_DAY_COUNT_NOT_ENOUGH,	//累计充值天数未达到要求

		//////////////////////////////////////	个人boss ///////////////////////////////////
		EN_PERSONAL_BOSS_BUY_LIMIT_VIP,		           //你当前VIP等级，可购买的次数已达上限！

		//////////////////////////////////////	夫妻家园	///////////////////////////////////
		EN_SPOUSE_HOME_ITEM_NOT_PUT_IN_BAG,				//该物品不可被装备
		EN_SPOUSE_HOME_BEFORE_ROOM_NOT_OPEN,			//前面房间未打开
		EN_SPOUSE_HOME_ROOM_BUY_LIMIT,					//房间购买已达上限
		EN_SPOUSE_HOME_ONLY_CAN_BUY_FOR_LOVER,          //只能为自己的伴侣购买房间
		EN_SPOUSE_HOME_LOVER_ROOM_BUY_LIMIT,			//伴侣的房间数量已达上限
		EN_SPOUSE_HOME_ONLY_CAN_EQUIP_FOR_LOVER,        //只能为自己的伴侣装扮房间
		EN_SPOUSE_HOME_LOVER_ROOM_NO_OPEN,              //伴侣没有空房间可以装扮

		//////////////////////////////////////	装备洗炼	///////////////////////////////////
		EN_EQUIP_BAPTIZE_PART_NOT_OPEN,					//该部位还未开启附灵
		EN_EQUIP_BAPTIZE_SLOT_ALREADY_OPEN,				//该附灵槽已开启
		EN_EQUIP_BAPTIZE_SLOT_NOT_OPEN,					//该附灵槽未开启
		EN_EQUIP_BAPTIZE_CANT_LOCK_ALL_SLOT,			//不可锁定所有槽

		EN_REDUCE_ZERO_WEARY,                           //没有可减少的疲劳
		EN_CROSS_ACTIVITY_LEVEL_MAX_LIMIT,				//等级过高，不能参与该跨服
		EN_CROSS_ACTIVITY_OPEN_SERVER_DAY_LIMIT,		//活动尚未开启
		EN_ENTER_ACTIVITY_FB_LEVEL_MAX_LIMIT,			//等级过高，不能进入活动场景

		//////////////////////////////////////	多倍领取提示	///////////////////////////////////
		EN_BUY_MULTIPLE_REWARD_LIMIT,					//今天购买多倍奖励已达上限
		EN_VIP_LEVEL_NOT_ENOUGHT,						//vip等级不足，不能购买多倍奖励
		EN_PASS_LEVEL_NOT_ENOUGHT,						//至少通关一层才能购买多倍奖励
		EN_BUY_MUITIPLE_SCENE_LIMIT,					//当前场景不允许购买多倍奖励

		//////////////////////////////////////	时装	///////////////////////////////////
		EN_SHIZHUANG_BODY_UP_ONE_GRADE_LIMIT,			//时装阶级不满足
		EN_SHIZHUANG_WUQI_UP_ONE_GRADE_LIMIT,			//神兵阶级不满足


		//////////////////////////////////////	名将	///////////////////////////////////
		EN_GREATE_SOLDIER_HUANHUA_CARD_ALREADY_ACTIVED,	//您已激活过该神魔幻化卡
		EN_GREATESOLDIER_NOT_ALIVE,						//神魔未激活
		EN_GREATESOLDIER_POTENTIAL_UP_FAIL,				//潜能升级失败
		EN_BEAUTY_FETCH_SEQ_ERROR,						//领取下标错误
		EN_BEAUTY_FETCH_HAS_FETCHED,					//已领取过奖励
		EN_BIANSHEN_LIMIT,								//变身状态下无法进行该操作
		EN_GREATESOLDIER_QUALITY_NOT_ENOUGH,			//不符合品质要求
		EN_GREATESOLDIER_EQUIP_STRENGTH_LEVEL_MAX,		//神魔装备强化达到最高等级
		EN_GREATESOLDIER_NO_EQUIP,						//该槽位没有装备神魔装备

		/////////////////////////////////// 一元抢购 ////////////////////////////////////////
		EN_CLOUD_PURCHASE_BUY_TIME_LIMIT,				//未到开放购买时间
		EN_CLOUD_PURCHASE_BUY_COUNT_LIMIT,				//抱歉，你购买的份数超过上限
		EN_CLOUD_PURCHASE_CONVERT_COUNT_LIMIT,			//兑换次数超过限制
		EN_CLOUD_PURCHASE_CONVERT_SCORE_LIMIT,			//积分不足
		EN_CLOUD_PURCHASE_TICKET_LIMIT,					//抢购券不足

		////////////////////////////	直升丹购买	//////////////////////////////////////////////
		EN_UPGRADE_CARD_BUY_ALREADY_BUY,				//今天已经购买过了，明天再来吧

		//////////////////////////////////////   参与限制 相关    ////////////////////////////////////
		EN_JOIN_LIMIT_LEVEL_NOT_ENOUGH,					//等级不足
		EN_JOIN_LIMIT_LEVEL_EXCEEDS_MAXIMUM,			//等级过高
		EN_JOIN_LIMIT_CAP_NOT_ENOUGH,					//战力不足
		EN_JOIN_LIMIT_OPEN_DAY_LIMIT,					//今日不开放，无法参与

		//////////////////////////////////////  天神护体    ////////////////////////////////////
		EN_HUGUOZHILI_CANNOT_ACTIVE,					//死亡次数不足，无法激活天神护体
		EN_HUGUOZHILI_ALREADY_ACTIVE,					//你已拥有天神护体
		EN_HUGUOZHILI_IN_CD,							//天神护体正在冷却中
		EN_HUGUOZHILI_ALIVE,							//您已复活，不能激活天神护体

		//////////////////////////////////////  今日主题    ////////////////////////////////////
		EN_TODAY_THEME_FETCH_FAIL,						//已经领取过该奖励

		//////////////////////////////////////  零元购    ////////////////////////////////////
		EN_ZERO_BUY_RETURN_ALREADY_BUY,					//已经购买过该礼包
		EN_ZERO_BUY_RETURN_NOT_BUY,						//尚未购买该礼包

		////////////////////////////////////////////////////////////////////////////////////////
		EN_BAIBEIFANLI2_ALREADY_BUY,					//已经购买过该物品

		EN_GIFTSHOP_BUY2_NOT_START,						//礼包限购尚未开启
		EN_GIFTSHOP_BUY2_SEQ_ERROR,						//seq错误

		//////////////////////////////////////  转职装备觉醒    ////////////////////////////////////
		EN_ZHUANZHI_EQUIP_WAKE_ORDER_NOT_ENOUGH,		//转职装备阶数不足
		EN_ZHUANZHI_EQUIP_WAKE_COLOR_NOT_ENOUGH,		//转职装备品质不足
		EN_ZHUANZHI_EQUIP_WAKE_NOT_WEAR,				//穿戴转职装备后才能觉醒
		EN_ZHUANZHI_EQUIP_COMPOSE_FAIL,					//合成失败
		EN_ZHUANZHI_EQUIP_COMPOSE_REQ_FAIL,				//合成请求失败
		EN_ZHUANZHI_EQUIP_COMPOSE_XIANPIN_NUM_ERROR,	//卓越属性条数不足
		EN_ZHUANZHI_EQUIP_COMPOSE_INDEX_ERROR,			//合成材料出错

		//////////////////////////////////////	幸运云购	///////////////////////////////////
		EN_RA_LUCKY_CLOUD_BUY_ALL_SELL,					//本轮库存已售罄，请等待下一轮开启
		EN_RA_LUCKY_CLOUD_ALL_ROUND_FINISH,				//今日云购的全部轮次已刷完了，活动结束
		EN_RA_LUCKY_CLOUD_THIS_ROUND_NO_OPEN,			//下一轮尚未开始
		EN_RA_LUCKY_CLOUD_KNAPSACK_NO_SPACE,			//背包已满，云购奖励已通过邮件补发

		//////////////////////////////////////狂欢大乐购///////////////////////////////////
		EN_CRACY_BUY_LEVEL_LESS_THAN_ACTIVITY,			//已经被抢完了

		//////////////////////////////////////  周末装备  ////////////////////////////////////
		EN_TIANSHENHUTI_SLOT_EMPTY,						//该位置没有装备
		EN_TIANSHENHUTI_BACKPACK_FULL,					//周末装备背包已满
		EN_TIANSHENHUTI_EQUIP_LEVEL_DIFFERENT,			//装备阶数不相等
		EN_TIANSHENHUTI_BACKPACK_NO_SAPCE,				//背包空间不足
		EN_TIANSHENHUTI_ROLL_NOT_OPEN,					//该抽奖周末才开放
		EN_TIANSHENHUTI_EQUIP_ALEADY_MAX_LEVEL,			//装备已经是最高阶
		EN_TIANSHENHUTI_SCORE_NOT_ENOUGH,				//积分不足

		//////////////////////////////////////	神话系统	///////////////////////////////////
		EN_MYTH_BACKPACK_NOT_ENOUGH,					//神话背包不足
		EN_MYTH_HAD_RESONANCED_ALREAD,					//已经共鸣成功了
		EN_MYTH_STUFF_NOT_ENOUGH,						//材料不足
		EN_MYTH_SOUL_ESSENCE_NOT_ENOUGH,				//神魂精华不足
		EN_MYTH_DIGEATION_LEVEL_LIMIT,					//当前领悟等级已达上限

		//////////////////////////////////////  跨服护送水晶 ////////////////////////////////////
		EN_SHUIJING_GATHER_NONE,						//已经采完，请等下次刷新
		EN_SHUIJING_NOT_IN_ROLE,						//请先去采集
		EN_SHUIJING_HUSONG_TASK_LIMIT,					//已有护送任务不能护送
		EN_SHUIJING_ALREADY_HAVE,						//你已经有水晶可以护送，不必再采集

		//////////////////////////////////////  私聊失败原因 ////////////////////////////////////
		EN_SINGLE_CHAT_FAIL_TYPE_FORMAT,				//私聊格式错误
		EN_SINGLE_CHAT_FAIL_TYPE_LENGTH,				//私聊长度错误
		EN_SINGLE_CHAT_FAIL_TYPE_AUDIO,					//不允许发送语音私聊
		EN_SINGLE_CHAT_FAIL_TYPE_CHAT_USER,				//获取私聊信息失败
		EN_SINGLE_CHAT_FAIL_TYPE_FORBID_CROSS_CHAT,		//禁止跨服私聊
		EN_SINGLE_CHAT_FAIL_TYPE_NOT_FRIEND,			//对方与您不是好友关系
		EN_SINGLE_CHAT_FAIL_TYPE_FORBID_CHANNEL,		//被禁言中，无法发起私聊
		EN_SINGLE_CHAT_FAIL_TYPE_FORBID_CHANNEL_TIME,	//该时间无法私聊


		//////////////////////////////////////  固定值    ////////////////////////////////////
		EN_GET_ACCOUNT_GOLD_TOO_FAST = 100000,			//从账号提取元宝间隔时间不足
		EN_COIN_NOT_ENOUGH = 100001,					//铜币不足
		EN_GOLD_NOT_ENOUGH = 100002,					//您元宝不足，请前往充值！
		EN_BIND_GOLD_NOT_ENOUGH = 100003,				//绑定元宝不足
		EN_MONEY_IS_LOCK = 100004,						//金钱已经锁定
		EN_ROLE_ZHENQI_NOT_ENOUGH = 100005,				//仙魂不足
		EN_XIANNV_EXP_DAN_LIMIT = 100006,				//仙女经验丹不足
		EN_CONVERTSHOP_BATTLE_FIELD_HONOR_LESS = 100007,//战场荣誉不足
		EN_ROLE_NV_WA_SHI_NOT_ENOUGH = 100008,			//女蜗石不足
		EN_ROLE_HUNLI_NOT_ENOUGH = 100009,				//魂力不足
		EN_SHENGWANG_NOT_ENOUGH = 100010,				//声望不足
		EN_HUOLI_NOT_ENOUGH = 100011,					//活力不足
		EN_GUANGHUI_NOT_ENOUGH = 100012,				//光辉不足
		EN_REWARD_HAS_FETCH = 100013,					//奖励已领取
		EN_CONFIG_ERROR = 100014,						//配置读取错误
		EN_ILLEGAL_OPERA = 100015,						//非法的操作
		EN_PARAMETER_ERROR = 100016,					//请求参数错误
		EN_LEVEL_LIMIT = 100017,						//已满级
		EN_NO_TIMES = 100018,							//已经没有次数了
		EN_HIDDEN_SERVER_OPERA_ERROR = 100019,			//跨服中无法进行此操作
	};
}

#endif
