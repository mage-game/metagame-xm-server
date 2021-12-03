#ifndef __NOTICENUM_H__
#define __NOTICENUM_H__

namespace noticenum
{
	//成功提示码 从10000开始 不要超过100000

	enum NOTICE_NUM_DEF
	{
		NT_GUILD_UNION_APPLYFOR_SUCC = 10000,						//申请仙盟联盟成功
		NT_GUILD_RESET_NAME_SUCC,									//仙盟改名成功
		NT_GUILD_APPLYFOR_SETUP_SAVE_SUCC,							//申请设置保存成功
		NT_GUILD_GET_FULI_GIFT_SUCC,								//领取成功
		NT_GUILD_SET_AUTO_KICKOUT_SETUP_SUCC,						//设置成功
		NT_GUILD_INVITE_SUCC,										//仙盟邀请成功
		NT_GUILD_GET_BUFF_SUCC,										//仙盟BUFF领取成功
		NT_GUILD_BOSS_CALL_SUCC,									//仙盟BOSS召唤成功

		NT_EQUIP_COMPOSE_SUCC = 10500,								//合成成功
		NT_EQUIP_EXCHANGE_SUCC,										//兑换成功

		NT_EQUIP_INLAY_GEMSTONE_SUCC = 10600,						//装备镶嵌成功
		NT_EQUIP_UNINLAY_GEMSTONE_SUCC,								//宝石拆除成功
		NT_EQUIP_UP_STAR_SUCC,										//装备升星成功
		NT_EQUIP_FLUSH_SUCC,										//装备洗练成功
		NT_EQUIP_FLUSH_REPLACE_SUCC,								//装备洗练属性替换成功
		NT_EQUIP_UPQUALITY_SUCC,									//装备提升品质成功
		NT_EQUIP_UPLEVEL_SUCC,										//装备升级成功
		NT_EQUIP_DECOMPOSE_SUCC,									//装备分解成功
		NT_EQUIP_DRESS_APPE_SELECT_SUCC,							//时装装扮保存成功
		NT_EQUIP_REFINE_SUCC,										//装备精炼成功 
		NT_EQUIP_EXTEND_SUCC,										//装备继承成功
		NT_SZ_EXPIRED,												//时装已经到期
		NT_SZ_STRENGTH_SUCC,										//时装升级成功
		NT_SZ_FOREVER,												//恭喜你，时装获得永久时效
		NT_EQUIP_REPAIR_SUCC,										//装备修理成功
		NT_WEDDINGRING_LIGHTEN_SUCC,								//点亮成功
		NT_WEDDINGRING_UP_SUCC,										//婚戒升级成功
		NT_WEDDINGRING_LIGHTEN_FAIL,								//点亮失败
		NT_INLAY_GEMSTONE_SUCC,										//宝石镶嵌成功
		NT_UNINLAY_GEMSTONE_SUCC,									//宝石摘除成功
		NT_EQUIP_SHENZHU_SUCC,										//装备神铸成功
		NT_EQUIP_FULING_SUCC,										//装备附灵成功
		NT_STONE_UP_SUCC,											//宝石升级成功

		NT_NO_UES_10700 = 10700,									//暂无用
		NT_MOUNT_STRENGTHEN_SUCC,									//坐骑强化成功
		NT_MOUNT_STRENGTHEN_FAIL,									//坐骑强化失败
		NT_MOUNT_ACTIVE_IMG_FAIL,									//激活形象失败
		NT_MOUNT_LIFT_CRIT_1,										//坐骑进阶小暴击！
		NT_MOUNT_LIFT_CRIT_2,										//坐骑进阶大暴击！
		NT_MOUNT_FEED_SUCC,											//坐骑喂养成功
		NT_MOUNT_FEED_SUCC_AND_LEVELUP,								//坐骑喂养成功，喂养等级上升！
		NT_MOUNT_FEED_CRIT_1,										//坐骑喂养小暴击！
		NT_MOUNT_FEED_CRIT_2,										//坐骑喂养大暴击！
		NT_MOUNT_FLUSH_SUCC,										//坐骑驯化成功
		NT_MOUNT_FUSION_SUCC,										//坐骑融合成功
		NT_MOUNT_ACTIVE_IMG_SUCC,									//激活形象成功
		NT_MOUNT_IMG_EXPIRED,										//幻化形象已经到期
		NT_MOUNT_UP_VAL_ADD_1_SUCC,									//获得1点突破值
		NT_QIBING_LIFT_SUCC,										//骑兵升级成功
		NT_SELECT_APPERANCE_SUCC,									//选择形象成功

		NT_JINGLING_GUANGHUAN_LIFT_SUCC,							//精灵光环进阶成功
		NT_JINGLING_FAZHEN_LIFT_SUCC,								//精灵法阵进阶成功

		NT_SHOP_BUY_SUCC = 10800,									//购买成功
		NT_NPCSHOP_SELL_SUCC,										//出售成功
		NT_CONVERTSHOP_CONVERT_SUCC,								//兑换成功
		NT_READE_REQ_SUCC,											//发起交易成功，等待对方确认
		NT_PASS_TOWER_GET_REWARD_SUCC,								//领取奖励成功
		NT_EXPDITION_GET_REWARD_SUCC,								//领取奖励成功
		NT_SELECT_PROF_1_SUCC,										//恭喜你成功选择剑灵职业
		NT_SELECT_PROF_2_SUCC,										//恭喜你成功选择影刺职业
		NT_SELECT_PROF_3_SUCC,										//恭喜你成功选择冰姬职业
		NT_SELECT_PROF_4_SUCC,										//恭喜你成功选择炎尊职业
		NT_SELECT_CAMP_1_SUCC,										//恭喜你成功选择天山师门
		NT_SELECT_CAMP_2_SUCC,										//恭喜你成功选择精武师门
		NT_SELECT_CAMP_3_SUCC,										//恭喜你成功选择百花师门
		NT_TRADE_OTHER_IS_TRAING,									//此玩家正在交易中

		NT_GUILD_SIGNIN_SUCC = 10900,								//仙盟签到成功
		NT_GUILD_GET_SHENLU_BIND_COIN_SUCC,							//领取仙盟神炉铜币成功
		EN_GUILD_BLESSING_SUCC,										//祝福成功
		EN_GUILD_INVITE_BLESSING_SUCC,								//邀请祝福成功
		EN_GUILD_CALL_IN_SUCC,										//发布招募成功
		EN_GUILD_FETCH_REDBAG_SUCC,									//领取红包成功

		NT_NO_USE_11000 = 11000,									//暂无用
		NT_NO_USE_11001,											//暂无用
		NT_PET_LIFT_WUXING_SUCC,									//宠物提升悟性成功
		NT_PET_LIFT_LINGXING_CRIT,									//宠物提升灵性暴击！
		NT_PET_LIFT_LINGXING_SUCC,									//宠物提升灵性成功
		NT_PET_PROMOTE_SUCC,										//宠物提升成长成功
		NT_PET_CHEAT_ROLLING,										//摇骰子作弊被抓住
		NT_PET_SKILL_LEARNT_SUCC,									//技能学习成功
		NT_PET_SKILL_UPGRADE_SUCC,									//技能升级成功
		NT_PET_SKILL_FORGET_SUCC,									//技能遗忘成功
		NT_PET_MERGE_SUCC,											//宠物融合成功
		NT_PET_FEED_SUCC,											//宠物喂养成功
		NT_PET_LINGXUE_PROMOTE_SUCC,								//宠物灵穴提升成功
		NT_PET_IMAGE_ACTIVATE_SUCC,									//宠物图鉴激活成功
		NT_PET_IMAGE_PROMOTE_SUCC,									//宠物图鉴提升成功
		NT_PET_IMAGE_TRANSFORM_SUCC,								//宠物图鉴幻化成功
		NT_PET_IMAGE_RESTORE_SUCC,									//宠物图鉴还原成功
		NT_PET_CONVERT_SUCC,										//宠物转生成功
		NT_PET_CONVERT_ITEM_TAKE_SUCC,								//成功领取宠物转生丹
		
		NT_MENTALITY_SPEED_SUCC = 11100,							//永久清除冷却时间成功
		NT_NO_USE_11101,											//暂无用
		NT_MENTALITY_VIP_FREE_SPEED_SUCC,							//VIP免费加速成功

		NT_FRIEND_ADD_SUCCESS = 11200,								//添加好友成功
		NT_FRIEND_BLESS_FETCH_TOTAL_EXP_SUCCESS,					//获取葫芦经验成功
		NT_OFFICIAL_GET_SALARY_SUCCESS,								//官职俸禄领取成功
		NT_WELFARE_GET_ACCOUNT_LOGIN_SUCCESS,						//累计登陆福利领取成功
		EN_WELFARE_GET_WEEKLY_ACCLOGIN_SUCCESS,						//周累计福利领取成功
		NT_VIP_GET_WELFARE_SUCCESS,									//领取VIP福利成功
		NT_WELFARE_GET_DAILYFIND_SUCCESS,							//找回领取成功
		NT_WELFARE_GET_DAILY_ONLINE_SUCCESS,						//领取在线奖励成功
		NT_GET_OFFLINE_EXP_SUCCESS,									//领取离线经验成功
		NT_WELFARE_GET_PLAT_VIP_GIFT_SUCCESS,						//领取礼包成功
		NT_WELFARE_HAPPY_TREE_UPLEVEL,								//欢乐果树升级成功
		NT_WELFARE_EXCHANG_SUCCESS,									//兑换成功
		NT_WELFARE_SIGN_SUCCESS,									//签到成功
		NT_WELFARE_BEFORE_SIGN_SUCCESS,								//补签成功
		NT_WELFARE_QUICK_SIGN_SUCCESS,								//一键签到成功

		NT_RUNE_SYSTEM_SET_RUNE_SUCC,								//战魂装备成功
		NT_RUNE_SYSTEM_COMPOSE_SUCC,								//战魂合成成功
		NT_RUNE_SYSTEM_DECOMPOSE_SUCC,								//战魂分解成功
		NT_RUNE_SYSTEM_RUNE_UP_LEVEL_SUCC,							//战魂升级成功
		NT_RUNE_SYSTEM_RUNE_UNLOAD_RUNE_SUCC,						//战魂拆卸成功
		NT_RUNE_SYSTEM_CONVERT_SUCC,								//兑换战魂成功

		NT_CHANGE_TITLE_SUCC = 11300,								//选择称号成功
		NT_GET_TITLE_SUCC,											//恭喜获得称号
		NT_TITLE_UPGRADE_SUCC,										//称号进阶成功
		NT_NEWHAND_GUILD_SUCC,										//恭喜您被选拔为新手指导员

		NT_ADD_BLACK_SUCC = 11400,									//添加黑名单成功
		NT_REMOVE_BLACK_SUCC,										//删除黑名单成功
		NT_REMOVE_FRIEND_SUCC,										//删除好友成功
		NT_REMOVE_ENEMY_SUCC,										//删除仇人成功
		NT_GIVE_FLOWER_SUCCESS,										//赠送鲜花成功
		NT_THANK_BLESS_SUCCESS,										//回礼成功
		NT_SEND_ADD_FRIEND_REQ_SUCC,								//好友申请发送成功

		NT_REST_INVITE_SUCC = 11500,								//双修邀请已发送
		NT_TEAM_CALL_TOGETHER_SUCC,									//召集发送成功
		NT_TEAM_JOIN_SUCC,											//成功加入队伍
		NT_TEAM_SET_TOWERDEFEND_ATTRTYPE_SUCC,						//设置类型成功
		NT_TEAM_INVITE_REQ_SUCC,									//组队邀请发送成功
		NT_TEAM_JOIN_REQ_SUCC,										//已发送入队申请

		NT_LEARN_SKILL_SUCC = 11600,								//技能学习成功
		NT_SKILL_UP_SUCC,											//技能升级成功

		NT_CARD_TAKEOFF_SUCC = 11650,								//卡牌成功卸下
		NT_CARD_PUTON_SUCC,											//卡牌成功装配
		NT_CARD_STRENGTH_SUCC,										//卡牌强化成功
		NT_CARD_TAKE_REWARD_SUCC,									//卡牌奖励领取成功

		NT_DUOBAOXIANBING_CONVERT_SUCC = 11700,						//兑换礼包成功

		NT_LINGMAI_GATHER_SUCC = 11800,								//灵脉采集成功
		NT_LINGZHUPLANT_SUCC,										//地脉灵珠放置成功
		NT_ZHADAN_PUT_SUCC,											//崩天玉放置成功
		NT_SOULING_FIND_LINGZHU,									//成功搜到一个灵珠	

		NT_LINGSHANXIANQI_CAPTURE_SUCC,								//恭喜您捕获成功
		NT_XIANBANG_TASK_REFRESH_SUCC,								//刷新成功，消耗1个揭榜令
		NT_XIANBANG_TASK_FREE_REFRESH_SUCC,							//免费刷新成功
		NT_TUMO_TASK_ITEM_REFRESH_COLOR_CHANGE_SUCC,				//消耗屠魔令，任务刷新成功
		NT_TUMO_TASK_COIN_REFRESH_COLOR_CHANGE_SUCC,				//消耗铜币，任务刷新成功
		NT_HUSONG_REFRESH_COLOR_SUCC,								//仙女刷新成功
		NT_TUMO_TASK_REFRESH_SUCC,									//屠魔任务刷新成功
		NT_TUMO_TASK_ITEM_REFRESH_SUCC,								//消耗屠魔令，任务品质不变
		NT_TUMO_TASK_COIN_REFRESH_SUCC,								//消耗元宝，任务品质不变

		NT_NINGJU_SUCC,												//恭喜您凝聚成功
		NT_ITEM_USE_SUCC,											//物品使用成功

		NT_GET_REWARD_SUCC,											//领取奖励成功
		NT_GET_FIRST_CHONGZHI_GIFT_SUCC,							//领取礼包成功
		NT_ADD_SHENLONG_BAOZANG,									//获得神龙宝藏
		NT_ADD_YUANGU_BAOZANG,										//获得远古宝藏
		NT_GET_CLOSED_BETA_REWARD_SUCC,								//领取封测奖励成功
		NT_BUY_GONGJI_ADD_SUCC,										//购买成功，获得100连斩

		NT_BUILDING_SUCC,											//守卫升级成功，获得更强大的战斗力！
		NT_TOUZIJIHUA_REWARD_SUCC,									//领取投资计划奖励成功
		NT_XIANMENGZHAN_USE_DANYAO_SUCC,							//提交丹药成功
		NT_TRAIN_LONGHUI_SUCC,										//龙魂注入成功
		NT_TIANCI_TONGBI_GATHER_SUCC,								//采集成功
		NT_TIANCI_TONGBI_USE_GATHER_SUCC,							//提交成功
		
		NT_GET_COMMON_ACTIVITY_LOGIN_REWARD_SUCCESS,				//领取登录奖励成功
		NT_GET_COMMON_ACTIVITY_DUR_LOGIN_REWARD_SUCCESS,			//领取连续登录奖励成功
		NT_GET_COMMON_ACTIVITY_CONSUME_GOLD_REWARD_SUCCESS,			//领取今日消费奖励成功
		NT_COMMON_ACTIVITY_PICK_UP_HONGBAO_SUCCESS,					//拾取成功
		NT_GET_COMMON_ACTIVITY_MEIGUI_DINGQIN_REWARD_SUCCESS,		//领取玫瑰定情奖励成功
		NT_GET_COMMON_ACTIVITY_ACTIVE_REWARD_SUCCESS,				//领取活跃奖励成功
		NT_GET_COMMON_ACTIVITY_SHIZHUANG_REWARD_SUCCESS,			//领取时装比拼奖励成功

		EN_GUILD_TASK_GIVEUP_TASK_SUCC,								//仙盟任务放弃成功
		EN_TASK_GIVEUP_SUCC,										//任务放弃成功

		NT_GET_COMMON_ACTIVITY_CYCLE_CONSUME_REWARD_SUCCESS,		//领取消费有礼奖励成功
		NT_MENTALITY_COMBINE_UPGRADE_SUCCESS,						//提升成功
		NT_MENTALITY_COMBINE_SUCCESS,								//合体成功，女神已出战！
		NT_1VN_BET_SUCC,											//感谢您的支持
		NT_MENTALITY_CANCLE_COMBINE_SUCCESS,						//解除合体成功

		NT_BUY_XIUWEI_SUCCESS,										//膜拜成功
		NT_GET_COMMON_ACTIVITY_CHEST_SHOP_BUY_REWARD_SUCCESS,		//领取团购寻宝奖励成功

		NT_YUJIANQIUMINGSHAN_PICK_SKILL_SUCCESS,					//拾取技能成功
		NT_DATING_INVITE_SUCCESS,									//约会邀请发送成功
		NT_DATING_INVITE_CANCELED,									//约会邀请已取消

		NT_TAKE_REWARD_SUCC,										//领取奖励成功
		NT_GET_COMMON_ACTIVITY_DAY_FIRST_CHONGZHI_REWARD_SUCCESS,	//领取每日首充奖励成功
		NT_GET_COMMON_ACTIVITY_MOUNT_UPGRADE_REWARD_SUCCESS,		//领取坐骑进阶奖励成功
		NT_USE_YANHUA_SUCCESS,										//成功点燃，获得5000经验
		NT_USE_MARRY_YANHUA_SUCCESS,								//成功点燃
		NT_NEQ_AUTO_SUCC,											//扫荡完成
		NT_AUTO_ACTIIVTY_SUCC,										//领取成功
		NT_ACTIVITY_FIND_SUCCESS,									//领取成功

		NT_XIANNV_ONEKEY_PROMOTE_LEVEL_SUCC,						//女神一键升级成功
		NT_PRAISE_SUCC,												//点赞成功
		NT_TOWERDEFEND_NEXT_WAVE_SUCC,								//刷新成功，请注意防守
		NT_XIANNV_ACTIVE_SUCC,										//仙女激活成功
		NT_XIANNV_ADD_ZIZHI_SUCC,									//资质升级成功
		NT_MAZE_FETCH_REWARD_SUCC,									//领取成功
		NT_MAZE_BUY_MOVE_TIMES_SUCC,								//购买行动力成功

		NT_QIFU_ROLL_COIN_SUCC,										//祈福成功,获得大量铜币
		NT_QIFU_ROLL_XIANHUN,										//祈福成功,获得大量仙魂
		NT_QIFU_ROLL_YUANLI,										//祈福成功,获得大量真气

		NT_SPEAK_TALK_SUCC,											//传音发送成功
		NT_SETATTACKMODE_PEACE,										//当前处于和平模式，不可攻击其他玩家，但可能被其他玩家攻击
		NT_SETATTACKMODE_GUILD,										//当前处于仙盟模式，只可攻击其他仙盟的玩家
		NT_SETATTACKMODE_TEAM,										//当前处于组队模式，只可攻击除队友以外的玩家
		NT_SETATTACKMODE_ALL,										//当前处于全体模式，可攻击所有玩家
		NT_SETATTACKMODE_NAMECOLOR,									//当前处于善恶模式，只可以攻击红名的玩家
		NT_SETATTACKMODE_CAMP,										//当前处于门派模式，只可攻击其他门派的玩家

		NT_XIANMENGZHAN_CALL_MEMBER_SUCC,							//召集发送成功
		NT_FOOTPRINT_ACTIVE_SUCC,									//成功激活足迹
		NT_MARRY_REQ_SUCC,											//求婚发起成功，等待对方回复
		NT_DIVORCE_REQ_SUCC,										//离婚发起成功
		NT_MARRY_HONEYMOON_FETCH_REWARD_SUCC,						//蜜月祝福物品领取成功
		NT_MARRY_HONEYMOON_ADD_DAYS_SUCC,							//蜜月祝福天数增加成功
		NT_TRADE_ANOTHER_HAS_LOCK,									//对方已锁定物品
		NT_TRADE_ANOTHER_HAS_AFFIRM,								//对方已确认交易
		NT_TRADE_HAS_AFFIRM,										//已确认交易，等待对方回复
		NT_TRADE_SUCC,												//交易成功
		NT_BUY_ENERGY_SUCC,											//购买体力成功
		NT_BUY_ROLE_HP_SUCC,										//购买补给成功
		NT_FRIEND_BLESS_SUCC,										//祝福成功！
		NT_FRIEND_FETCH_BLESS_REWARD,								//领取成功！
		NT_FRIEND_BLESS_INVATE_BLESS_SUCC,							//成功邀请好友祝福
		NT_HUNYAN_INVATE_SUCC,										//邀请成功
		NT_HUNYAN_GATHAR_SUCC,										//采集成功
		NT_HUNYAN_FA_HONGBAO_SUCC,									//发红包成功
		NT_HUNYAN_PAO_HUAQIU_LEN_QIU,								//抛花球冷却中
		NT_HUNYAN_PAO_HUAQIU_SUCC,									//抛花球成功
		NT_HUNYAN_PAO_HUAQIU_FAIL,									//抛花球次数已经用完
		NT_HUNYAN_SAHUA_SUCC,										//撒花成功
		NT_FISHPOOL_RAISE_SUCC,										//放养成功
		NT_FISHPOOL_HARVEST_SUCC,									//捕获成功
		NT_HUNYAN_ZHUFU_SUCC,										//祝福已发送
		NT_TITLE_USE_SUCC,											//保存成功
		NT_RAND_ACTIVITY_FETCH_REWARD_SUCC,			                //奖励领取成功
		NT_YIZHANDAODI_GUWU_SUCC,									//鼓舞成功
		NT_MENTALITY_WUXING_UPLEVEL_SUCC,							//提升成功
		NT_MENTALITY_WUXING_UPLEVEL_FAIL,							//提升失败
		NT_WUXINGGUAJI_GET_REWARD_SUCC,								//获取奖励成功！
		NT_INVITE_LUCKY_ZHUFU_SUCC,									//邀请成功！
		NT_ATTACK_MODE_FANJI,										//你已经切换到全体模式进行反击
		NT_MONTH_CARD_REWARD_FETCH_SUCCESS,							//领取成功
		NT_MOUNT_FLY_UP_SUCC,										//坐骑飞升成功
		NT_CSA_FETCH_REWARD_SUCC,									//奖励领取成功
		NT_XIANNV_HALO_UPGRADE_SUCC,								//女神光环升级成功
		NT_ZHUANZHI_SUCC,											//成功觉醒
		NT_WUSHUANG_EQUIP_DAZAO_SUCC,								//打造成功
		NT_FBGUWU_SUCC,												//鼓舞成功，攻击提升50%
		NT_FISH_POOL_UP_FISH_QUALITY_SUCC,							//升级成功

		NT_CARDZU_HUALING_SUCC,										//化灵成功
		NT_CARDZU_LINGZHU_SUCC,										//灵铸成功
		NT_CARDZU_ACTIVE_ZUHE_SUCC,									//组合激活成功
		NT_CARDZU_UPGRADE_ZUHE_SUCC,								//组合升级成功
		NT_CARDZU_BUY_LINGLI_SUCC,									//灵力购买成功，赠送卡牌

		NT_QINGYUAN_CARD_UPGRADE_SUCC,								//情缘卡牌升级成功
		NT_QINGYUAN_BUY_FB_TIMES_SUCC,								//情缘副本次数购买成功

		NT_CROSS_XIULUOTA_GUWU_SUCC,								//鼓舞成功
		NT_CROSS_BUY_REALIVE_GUWU_SUCC,								//购买复活成功

		NT_FETCH_CROSS_MULTIUSER_CHALLENGE_DAYCOUNT_REWARD_SUCCESS,	//领取跨服3V3每天参与奖励成功

		NT_LIFESKILL_MAKE_SUCC,										//制作成功
		NT_OPERATE_SUCC,											//操作成功
		NT_SPACE_CAI_SUCC,											//成功在对方空间留下脚印
		NT_SAVE_SUCC,												//保存成功
		NT_PRESENT_SUCC,											//赠送成功
		NT_COMMIT_SUCC,												//提交成功
		NT_STRENGTH_SUCC,											//强化成功
		NT_SELECT_SUCC,												//选择成功
		NT_STRENGTH_TAOZHUANG_SUCC,									//成功激活套装属性

		NT_SHENZHOU_WEAPON_UPGRADE_IDENTIFY_SUCC,					//恭喜你，升级成功
		NT_SHENZHOU_WEAPON_RECYCLE_SUCC,							//恭喜你，进阶成功
		NT_CAMP_BEAST_LEVEL_LIMIT,									//师门神兽已达最高等级
		NT_SHEN_ZHOU_HELP_BOX_SUCCESS,								//协助成功
		NT_SHEN_ZHOU_OPEN_BOX_SUCCESS,								//开宝箱成功

		NT_BABY_RENAME_SUCC,										//宝宝改名成功
		NT_BABY_UPGRADE_SUCC,										//宝宝进阶成功
		NT_BABY_BORN_SUCC,											//抱娃成功，喜获宝宝
		NT_BABY_CHAOSHENG_SUCC,										//超生名额获取成功
		NT_BABY_SPIRIT_UPGRADE_SUCC,								//守护精灵升级

		NT_PET_FIGHT_OUT_SUCC,										//灵器出战成功
		NT_PET_ACTIVE_SUCC,											//灵器激活成功
		NT_PET_UPGRADE_SUCC,										//灵器进阶成功
		NT_PET_RENAME_SUCC,											//灵器改名成功
		NT_PET_CALL_BACK_SUCC,										//灵器取消出战成功

		NT_MINE_GATHER_SUCC,										//捕猎成功

		NT_HUASHEN_CHANGE_IMAGE,									//切换化神形象成功
		NT_HUASHEN_PUTOFF_IMAGE,									//取消化神形象

		NT_QINGYUAN_COUPLE_HALO_ICON_ACTIVATE_SUCC,					//夫妻光环图标激活成功
		NT_QINGYUAN_COUPLE_HALO_EQUIPED_ON,							//已装备夫妻光环
		NT_QINGYUAN_COUPLE_HALO_EQUIPED_OFF,						//已卸下夫妻光环

		NT_PET_QINMI_UPSTAR_SUCC,									//恭喜你，成功提升契合度
		NT_PET_QINMI_UPLEVEL_SUCC,									//恭喜你，成功提升契合等级

		NT_PASTURE_SPIRIT_UPGRADE_SUCC,								//牧场精灵升级成功
		NT_PASTURE_SPIRIT_PROMOTE_QUALITY_SUCC,						//牧场精灵提示品质成功
		NT_PASTURE_FIRST_ENTER_GIVE_SCORE,							//恭喜获得系统免费赠送的500积分！

		NT_PERSONALIZE_WINDOW_BUBBLE_RIM_SUIT_ACTIVE_SUCC,			//套件部位激活成功

		NT_HUASHEN_SPIRIT_UPGRADE_SUCC,								//恭喜，升级化神精灵成功

		NT_QUESTION_USE_MIANDA_CAR_SUCC,							//使用免答卡成功
		NT_QUESTION_USE_BIANSHEN_CAR_SUCC,							//使用变身卡成功

		NT_MAGICCARD_SKILL_SUCC,									//魔卡技能使用成功
		NT_MAGICCARD_CARD_USE_SUCC,									//魔卡激活成功
		NT_MAGICCARD_CARD_EXCHANGE_SUCC,							//兑换成功

		NT_JINGLING_HALO_UPGRADE_SUCC,								//精灵光环升级成功

		NT_CROSS_BOSS_BUY_RELIVE_TIME_SUCC,							//购买成功
		NT_WING_FUMO_SUCC,											//羽翼附魔成功

		NT_MITAMA_UPGRADE_SUCC,										//恭喜你，成功升级了神侍
		NT_MITAMA_TASK_FIGHTING_SUCC,								//神侍出征！
		NT_MITAMA_TASK_FETCH_REWARD_SUCC,							//成功领取神侍战利品

		NT_FAIRY_TREE_FETCH_MONEY_SUCC,								//恭喜，领取仙树在线奖励成功
		NT_FAIRY_TREE_FETCH_GIFT_SUCC,								//恭喜，领取仙树礼包成功
		NT_FAIRY_TREE_UPLEVEL_SUCC,									//恭喜，仙树升级成功
		NT_FAIRY_TREE_UPGRADE_SUCC,									//恭喜，仙树进阶成功

		NT_RA_BLACK_MARKET_OFFER_ITEM_SUCC,							//恭喜你，竞价成功了

		NT_LIEMING_UP_LEVEL_SUCC,									//命魂升级成功

		NT_XIANNV_ACTIVEHUANHUA_SUCC,								//仙女幻化成功

		NT_ZHIBAO_HUANHUA_GARDE_UPLEVEL_SUCC,						//圣物形象升阶成功
		NT_ZHIBAO_HUANHUA_JIHUO_SUCC,								//圣物形象激活成功
		NT_YANHUI_START_SUCC,										//宴会开启成功
		NT_TIANJIANGCAIBAO_GET_REWARD_ITEM,							//获得天降财宝物品奖励
		NT_TIANJIANGCAIBAO_GATHER_SUCC,								//采集成功
		NT_YANHUI_BLESS_GET_BIND_GOLD,								//祝福获得绑钻
		NT_WANGLINGEXPLORE_GATHER_SUCC,								//采集成功
		NT_WANGLINGEXPLORE_REWARD_ITEM,								//获得奖励
		NT_TIANJIANGCAIBAO_GET_HONOR,								//获得天降财宝荣誉奖励
		NT_CROSS_HS_ADD_PARTNER_SUCCESS,							//幽会成功
		NT_CROSS_HS_GIVE_PRESENT_SUCCESS,							//赠送礼物成功
		NT_CROSS_HS_CANCEL_PARTNER_SUCCESS,							//取消幽会成功

		NT_CHAT_ACTIVE_BUBBLE_SUCCESS,								//激活气泡框成功
		NT_CHAT_USE_BUBBLE_SUCCESS,									//使用气泡框成功
		NT_CHAT_UN_USE_BUBBLE_SUCCESS,								//取消使用气泡框成功

		NT_ZHUANSHENG_AUTO_CONBIME_SUCC,							//恭喜，一键合并成功

		NT_LITTLE_PET_FEED_SUCC,									//小宠物喂养成功
		NT_LITTLE_PET_INTERACT_SUCC,								//小宠物互动成功
		NT_LITTLE_PET_EXCHANGE_SUCC,								//兑换成功
		NT_LITTLE_PET_RENAME_SUCC,									//小宠物改名成功


		NT_LOVE_TREE_RENAME_SUCC,									//相思树改名成功

		NT_ZHUANSHEN_SUCC,											//角色转生成功

		NT_MOJIE_ACTIVE_SUCC,										//特戒激活成功
		NT_MOJIE_UP_LEVEL_SUCC,										//特戒升级成功

		NT_TEAM_CHANGE_CAPTAIN_SUCC,								//你已经成为了队长

		NT_CHINESE_ZODIAC_UP_LEVEL_SUCC,							//生肖等级提升成功
		NT_CHINESE_XINGLING_UP_LEVEL_SUCC,							//龙魂等级提升成功

		NT_USE_TUHAOJIN_SCUCC,										//使用土豪金成功
		NT_NOT_USE_TUHAOJIN_SCUCC,									//已恢复默认文字颜色

		NT_GOLD_VIP_ACTIVE_SUCC,									//恭喜您成功激活黄金会员

		NT_KICK_OUT_FROM_DABAO,										//您因愤怒值超出被踢出
		NT_BUY_LOVE_CONTRACT_SUCCESS,								//爱情契约购买成功
		NT_SHENWU_HUANHUA_SUCCESS,									//特殊形象已经激活

		NT_EQUI_EXP_FB_GUWU_SUCC,									//鼓舞成功
		NT_EQUI_EXP_FB_AUTO_GUWU_SUCC,								//自动鼓舞成功

		NT_SHENGE_SYSTEM_SET_SHENGE_SUCC,							//星辉装备成功
		NT_SHENGE_SYSTEM_COMPOSE_SUCC,								//星辉合成成功
		NT_SHENGE_SYSTEM_DECOMPOSE_SUCC,							//星辉分解成功
		NT_SHENGE_SYSTEM_SHENGE_UP_LEVEL_SUCC,						//星辉升级成功
		NT_SHENGE_SYSTEM_SHENGE_UNLOAD_SHENGE_SUCC,					//星辉拆卸成功
		NT_SHENGE_SYSTEM_SHENGE_PAGE_CLEAR_SUCC,					//星辉页清除成功
		NT_SHENGE_SYSTEM_BAOJI_SUCC,								//星辉暴击增伤成功发动
		NT_SHENGE_SYSTEM_ZHANGKONG_UPLEVEL_SUCC,					//聚星进阶成功

		NT_TIAN_XIANG_ACTIVE_BEAD_SUCC,								//成功激活龙珠
		NT_TIAN_XIANG_CHANGE_BEAD_TYPE_SUCC,						//改变龙珠类型成功
		NT_TIAN_XIANG_MOVE_BEAD_SUCC,								//龙珠移动成功

		NT_ZODIAC_MIJI_PUT_SUCC,									//秘籍放置成功
		NT_ZODIAC_MIJI_CHANGE_SUCC,									//秘籍替换成功
		NT_ZODIAC_MIJI_COMPOUND_SUCC,								//秘籍合成成功

		NT_RED_PAPER_NOTICE_SUCC,									//提醒发红包成功

		NT_YIZHANDAODI_ADD_SPEED,									//您获得了加速效果
		NT_YIZHANDAODI_DEC_SPEED,									//您获得了减速效果
		NT_YIZHANDAODI_ADD_GONGJI,									//您获得了攻击提升效果
		NT_YIZHANDAODI_DEC_GONGJI,									//您获得了攻击下降效果
		NT_YIZHANDAODI_ADD_FANGYU,									//您获得了防御提升效果
		NT_YIZHANDAODI_DEC_FANGYU,									//您获得了防御下降效果
		NT_YIZHANDAODI_WUDI,										//您获得了无敌效果
		NT_YIZHANDAODI_RECOVER,										//您获得了回复效果
		NT_YIZHANDAODI_BIANSHEN,									//您获得了变身效果
		NT_YIZHANDAODI_DEADLY_ATTACK,								//您获得了致命一击效果

		NT_MARRY_UP_LEVEL_SUCC,										//结婚等级提升

		NT_RUSH_BUYING_BUY_ITEM_SUCC,								//恭喜，抢购物品成功
		NT_MAP_HUNT_GET_RARE_CITY,									//恭喜你发现宝藏古城
		NT_JUHUN_PUTON_SUCC,										//龙器装备成功
		NT_JUHUN_TAKE_OFF_SUCC,										//龙器装备脱下成功

		NT_SHEN_YIN_ACTIVE_BEAD_SUCC,								//成功激活源珠
		NT_SHEN_YIN_XILIAN_SUCC,									//精炼成功
		NT_SHEN_YIN_CHANGE_BEAD_TYPE_SUCC,							//改变源珠类型成功
		NT_SHEN_YIN_MOVE_BEAD_SUCC,									//源珠移动成功

		NT_FISHING_TIME_LIMIT,										//今日钓鱼次数已用完
		NT_ROLE_LOGOUT,												//玩家已下线，无法偷取
		NT_STEAL_FISH_TIEM_LIMIT,									//您今日的偷鱼次数已用完
		NT_BE_STEAL_FISH_TIME_LIMIT,								//当前玩家的被偷次数已达上限
		NT_HAVE_NO_FISH,											//当前玩家还没钓到鱼
		NT_STEAL_FISH_FAIL,											//手太滑，鱼儿逃走了
		NT_FISH_NOT_ENOUGH,											//鱼儿不够
		NT_IS_NOT_FISHING,											//玩家没在钓鱼

		NT_PLANTING_TREE_PLANTING_SUCC,								//恭喜你，捕捉玉兔成功
		NT_PLANTING_TREE_WATERING_SUCC,								//召唤玉兔成功

		NT_STEAL_TIMES_LIMIT,										//今日偷取次数已用完
		NT_STEAL_COIN_LIMIT,										//该玩家没看黄历,已被偷取10次,为他默哀吧
		NT_STEAL_ONE_PEOPLE_COUNT_LIMIT,							//积点德吧,他已经被偷的所剩无几了
		NT_HAS_NOT_ENOUGH_CRYSTAL_DALARAN,							//他的钱袋已经空空如也了

		NT_ELEMENT_HEART_UP_SPEED_SUCC,								//五灵之纹产出时间加速成功
		NT_ELEMENT_HEART_UP_GRADE_GHOST_SUCC,						//五灵进阶成功
		NT_ELEMENT_HEART_FEED_SUCC,									//五灵喂养成功

		NT_ELEMENT_SHOP_BUY_SUCC,									//购买成功
		EN_ELEMENT_SHOP_HAS_ALEADY_BUY,								//已经购买过该物品

		NT_UPGRADE_RUSH_RETURN_REWARD,								//恭喜您获得进阶返还奖励
		NT_KICK_OUT_FROM_BABY_BOSS,									//您因愤怒值超出被踢出

		NT_BUILD_TOWER_BUILD_SUCC,									//建造成功
		NT_BUILD_TOWER_UPLEVEL_SUCC,								//升级成功
		NT_BUILD_TOWER_REMOVE_SUCC,									//拆除成功
		NT_BUILD_TOWER_CALL_SUCC,									//召唤成功
		
		NT_FISHING_WATCH_YOUR_FISH,									//有人正在紧紧的盯着你，小心你的鱼哟
		NT_FISHING_EVENT_GET_FISH,									//恭喜你钓上一条可爱的鱼
		NT_FISHING_CONFIRM_GET_FISH,								//嘿嘿嘿，鱼儿又多了
		NT_FISHING_CONFIRM_GET_FISH_GIVE_SUCC,						//你送给了队友一条小鱼儿
		NT_FISHING_CONFIRM_GET_FISH_BE_GIVIEN,						//你的队友送你了一条小鱼儿
		NT_FISHING_BIG_FISH_HELP_SUCC,								//大伙加把劲，大鱼就要上钩了
		NT_FISHING_BIG_FISH_BE_HELPED_SUCC,							//你的一个同伴狠狠地帮你拉了一把
		NT_FISHING_EVENT_GET_BIG_FISH_SUCC,							//太棒了，上钩一条大鱼
		NT_FISHING_CONFIRM_BIG_FISH_TO_HELPER,						//你的队伍齐心协力，钓上了一条大鱼
		NT_FISHING_STEAL_FISH_SUCC,									//恭喜你，摸到一条大鱼
		NT_FISHING_EXCHANGE_SUCC,									//兑换成功！

		NT_WORLD_BOSS_CONCERN_SUCC,                                 //关注设置成功 
		NT_RENEW_SUCC,												//续费成功

		NT_LIGHT_TIANMING_SUCC,										//点亮天命成功
		NT_LIGHT_CANGLONG_SUCC,										//点亮苍龙成功
		NT_LIGHT_SHENGXIAO_SUCC,									//点亮生肖成功
		NT_LIGHT_YUANQI_SUCC,									    //点亮神道成功
		NT_LIGHT_SHENQI_SUCC,									    //点亮圣器成功
		NT_LIGHT_XINGXIU_SUCC,                                      //点亮星宿成功

		NT_TICKET_NO_ENOUGH,                                        //门票数量不足

		NT_SHENGQI_STUFF_DECOMPOSE_SUCC,							//分解成功

		NT_BABY_SPIRIT_WEN_UPGRADE_SUCC,                            //智力值升级
		NT_BABY_SPIRIT_WU_UPGRADE_SUCC,                             //武力值升级

	    NT_RED_EQUIP_COLLECT_SUCC,									//红装收集成功！
		NT_ORANGE_EQUIP_COLLECT_SUCC,								//橙装收集成功！

		NT_EQUIP_BAPTIZE_OPEN_SUCC,									//开启成功

		NT_SPOUSE_HOME_LOVER_ROOM_BUY_SUCC,                         //成功为伴侣购置了一间房
		NT_SPOUSE_HOME_LOVER_ROOM_EQUIP_SUCC,                       //成功为伴侣装扮房间
		NT_SPOUSE_HOME_LOVER_ROOM_EQUIP_FAIL,                       //该位置已有变化

		NT_GREATESOLDIER_EXCHANGE_SUCC,								//兑换成功
		NT_GREATESOLDIER_PUTON_SUCC,								//神魔装备成功
		NT_GREATESOLDIER_TAKE_OFF_SUCC,								//神魔装备脱下成功

		NT_GUILD_BOX_OPEN,											//您的背包已满，奖励将通过邮件的形式发送

		NT_GUILD_BOX_UPLEVEL_SUCC,									//剑炉升级成功
		NT_CROSS_LIEKUN_FB_GATHER_SUCC,								//采集成功

		NT_SHENYIN_EQUIP_SUCC,										//铭纹装备成功
		NT_UPLEVEL_RETURN_RWARD_MAIL,								//有进阶返利未领，已通过邮件发放
	};


	enum reply_type
	{
		NT_REPLY_TYPE_SHENGE = 1,//神格系统
	}; //REPLY_TYPE
}

#endif

