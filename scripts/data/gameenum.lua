CHARINTATTR_TYPE = 
{	
	-- auto by tools
	CHARINTATTR_TYPE_INVALID = 0,
	FIGHT_CHARINTATTR_TYPE_BEGIN = 0,
	FIGHT_CHARINTATTR_TYPE_GLOBAL_COOLDOWN = 1,									-- 全局cooldown时间
	FIGHT_CHARINTATTR_TYPE_HP = 2,												-- 血量
	FIGHT_CHARINTATTR_TYPE_MP = 3,												-- 魔法
	FIGHT_CHARINTATTR_TYPE_MAXHP = 4,											-- 最大血量
	FIGHT_CHARINTATTR_TYPE_MAXMP = 5,											-- 最大魔法
	FIGHT_CHARINTATTR_TYPE_GONGJI = 6,											-- 攻击
	FIGHT_CHARINTATTR_TYPE_FANGYU = 7,											-- 防御
	FIGHT_CHARINTATTR_TYPE_MINGZHONG = 8,										-- 命中
	FIGHT_CHARINTATTR_TYPE_SHANBI = 9,											-- 闪避
	FIGHT_CHARINTATTR_TYPE_BAOJI = 10,											-- 暴击
	FIGHT_CHARINTATTR_TYPE_JIANREN = 11,										-- 坚韧
	FIGHT_CHARINTATTR_TYPE_MOVE_SPEED = 12,										-- 移动速度
	FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI = 13,									-- 附加伤害
	FIGHT_CHARINTATTR_TYPE_DIKANG_SHANGHAI = 14,								-- 抵抗伤害
	FIGHT_CHARINTATTR_TYPE_PER_JINGZHUN = 15,									-- 精准
	FIGHT_CHARINTATTR_TYPE_PER_BAOJI = 16,										-- 暴击（幸运一击）
	FIGHT_CHARINTATTR_TYPE_PER_KANGBAO = 17,									-- 抗暴（抵抗幸运一击）
	FIGHT_CHARINTATTR_TYPE_PER_POFANG = 18,										-- 破防百分比
	FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG = 19,									-- 免伤百分比
	FIGHT_CHARINTATTR_TYPE_MAX= 20,												-- 
	-- lua define
	BASE_CHARINTATTR_TYPE_BEGIN = 32,
	BASE_CHARINTATTR_TYPE_MAXHP = 33,											-- 基础最大血量
	BASE_CHARINTATTR_TYPE_MAXMP = 34,											-- 基础最大魔法
	BASE_CHARINTATTR_TYPE_GONGJI = 35,											-- 基础攻击
	BASE_CHARINTATTR_TYPE_FANGYU = 36,											-- 基础防御
	BASE_CHARINTATTR_TYPE_MINGZHONG = 37,										-- 基础命中
	BASE_CHARINTATTR_TYPE_SHANBI = 38,											-- 基础闪避
	BASE_CHARINTATTR_TYPE_BAOJI = 39,											-- 基础暴击
	BASE_CHARINTATTR_TYPE_JIANREN = 40,											-- 基础坚韧
	BASE_CHARINTATTR_TYPE_MOVE_SPEED = 41,										-- 基础移动速度
	BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI = 42,									-- 附加伤害
	BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI = 43,									-- 抵抗伤害
	BASE_CHARINTATTR_TYPE_PER_JINGZHUN = 44,										-- 精准
	BASE_CHARINTATTR_TYPE_PER_BAOJI = 45,										-- 暴击（幸运一击）
	BASE_CHARINTATTR_TYPE_PER_KANGBAO = 46,										-- 抗暴（抵抗幸运一击）
	BASE_CHARINTATTR_TYPE_PER_POFANG = 47,										-- 破防百分比
	BASE_CHARINTATTR_TYPE_PER_MIANSHANG = 38,									-- 免伤百分比
	BASE_CHARINTATTR_TYPE_MAX = 49,												-- 
}

PUT_REASON_TYPE = 
{
	PUT_REASON_INVALID = 0,								-- 无效
	PUT_REASON_NO_NOTICE = 1,							-- 不通知

	PUT_REASON_BEGIN = 150,
}


-- 角色属性重算原因
C_CharIntAttrs = C_CharIntAttrs or {}
C_CharIntAttrs.RECALC_REASON_LUABEGIN = 30
C_CharIntAttrs.RECALC_REASON_SHENZHUANG2 = 31

-- 请求操作 返回结果类型
PROTOCOL_OPERATE_RESULT_TYPE = 
{
	OP_SHENZHUANG_JINJIE = 18,		--神装进阶
}

EFFECT_PRODUCT_ID = 
{
	PRODUCT_ID_GATHER_SPEED = 10000,				--采集速度上升
	PRODUCT_ID_GATHER_NOT_STOP = 10001,				--采集不打断
	PRODUCT_ID_TO_BUILDINF_INC_HURT = 10002,		--对建筑物提升伤害
	PRODUCT_ID_HP_CONTINUE = 10003,					--气血持续回复
	PRODUCT_ID_MP_CONTINUE = 10004,					--mp持续回复
	PRODUCT_ID_INC_PUTON_LEVEL = 10005,				--增加穿戴等级
	PRODUCT_ID_INC_EXTRAL_EXP_PER = 10006,			--经验额外加成百分比
	PRODUCT_ID_INC_INHURT_ON_ATK = 10007,			--攻击时提高别人伤害
	PRODUCT_ID_HP_DAN = 10008,						--气血丹回复气血
	PRODUCT_ID_SZ_PROTECT = 10009,					--神装保护
	PRODUCT_ID_JINGHUAHUSONG = 10010,				--精华护送
	PRODUCT_ID_NAMECOLOR = 10011,					--红名

	PRODUCT_ID_GCZ_ZHIMINGYIJI = 10100,				--攻城战：致命一击
	PRODUCT_ID_GCZ_YOUDIANYIJI = 10101,				--攻城战：弱点一击
	PRODUCT_ID_GCZ_POZHENGYIJI = 10102,				--攻城战：破阵一击
	PRODUCT_ID_GCZ_NEXT_CHENGMO = 10103,			--攻城战：下一次攻击沉默
	PRODUCT_ID_GCZ_NEXT_DINGSHENG = 10104,			--攻城战：下一次攻击定身
	PRODUCT_ID_GCZ_NEXT_XUANYUN = 10105,			--攻城战：下一次攻击眩晕

	PRODUCT_ID_BASE_ATTR_BEGIN = 10200,
	PRODUCT_ID_BASE_ATTR_ADD_JINGZHUN = 10201,		--增加精准固定值
	PRODUCT_ID_BASE_ATTR_ADD_BAOJI = 10202,			--增加暴击固定值
	PRODUCT_ID_BASE_ATTR_END = 10203,
}

-- 活动状态
ACTIVITY_STATUS = {
	CLOSE = 0,								-- 活动关闭状态
	STANDY = 1,								-- 活动准备状态
	OPEN = 2,								-- 活动开始中 
	MAX = 3,
}

-- 属性重算原因类型
RECALC_REASON_TYPE = {
	LINGYU = 28,							-- 灵玉
	TIANXIANGEFB = 29,						-- 天仙阁副本

	MAX = 50,								-- 最大值（改动需改c++)
}

-- 战斗力类型
CAPABILITY_TYPE = {
	LINGYU = 28,							-- 灵玉
	TIANXIANGEFB = 29,						-- 天仙阁副本

	MAX = 50,								-- 最大值（改动需改c++)
}

-- 放物品进入背包的原因
PUT_REASON_TYPE = {
	CHALLENGEFB_AUTO = 108					-- 挑战副本扫荡
}

-- 复活方式
REALIVE_TYPE = {
	BACK_HOME = 0,							-- 回城复活
	HERE_ICON = 1,							-- 使用铜钱原地复活
}

-- 每日计数id
DAYCOUNT_ID = {
	SHUIJING_GATHER = 64,					-- 水晶采集 次数
	
	MAX = 256,								-- 最大值（改动需改c++)
}

-- vip权限类型
VIP_AUTH_TYPE = {
	CHALLENGEFB_BUYTIMES = 19,				-- 挑战副本购买次数
	CHALLENGEFB_FREE_AUTOTIMES = 20,		-- 挑战副本免费扫荡次数

	MAX = 30								-- 最大值（改动需改c++)
}

-- 副本类型
FB_TYPE = {
	TIANXIANGE = 20,						-- 天仙阁
	CHALLENGE = 21,							-- 挑战副本（灵玉副本）
}

-- 场景类型
SCENE_TYPE = {
	SHUIJING = 35,							-- 水晶境
	TIANXIANGE = 40,						-- 天仙阁
	CHALLENGE = 41,							-- 挑战副本（灵玉副本）

	MAX = 50,
}

-- 活动玩法类型
ACTIVITY_TYPE = {
	SHUIJING = 14,							-- 水晶活动
	MIN = 19,								-- 最小值（改动需改c++)	
}

-- 各个模块的消息操作类型
MSG_OPERATE_TYPE = {
	['LINGYU_REQINFO'] = 0,					-- 灵玉信息
	['LINGYU_UPSTAR'] = 1,					-- 灵玉升星

	['CHALLENGEFB_REQINFO'] = 0,			-- 挑战副本请求信息
	['CHALLENGEFB_BUYJOINTIMES'] = 1,		-- 挑战副本购买次数
	['CHALLGNGEFB_AUTO'] = 2,				-- 挑战副本扫荡
	['CHALLENGEFB_RESETLEVEL'] = 3,			-- 挑战副本重置关卡
}