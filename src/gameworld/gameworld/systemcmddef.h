#ifndef __SYSTEM_CMD_DEF_HPP__
#define __SYSTEM_CMD_DEF_HPP__

namespace systemcmd
{
	enum config_type				// 配置表类型的定义
	{
		CONF_NONE = 0,
		CONF_TASK = 1,				// 任务配置
		CONF_ITEM_GIFT = 2,			// 物品礼包配置
		CONF_COMMONACTIVITY = 3,	// 通用活动配置
	};

	enum EffectType					// 对角色操作的类型定义
	{
		EFFECT_AUTHORITY = 0,		// 改变管理权限
		EFFECT_USE_GOLD,			// 消耗元宝
		EFFECT_USE_GOLD_BIND,		// 消耗绑定元宝
		EFFECT_USE_COIN_BIND,		// 消耗铜币
		EFFECT_ADD_HISTORY_CHONGZHI,// 增加累计充值数
		EFFECT_SET_CHARM,			// 设置魅力值
		EFEECT_ADD_UNIQUE_TITLE,	// 增加唯一称号
		EFFECT_COMPLETE_TASK,		// 完成任务
		EFFECT_SET_LEVEL,			// 设置等级
		EFFECT_CONSUME_ITEM,		// 消耗物品
	};

	enum RoleCountType
	{
		RCT_INVALID = 0,
		RCT_TUMO_TASK,				// 屠魔任务
		RCT_HUSONG_TASK,			// 护送任务

		RCT_MAX,
	};
}

#endif

