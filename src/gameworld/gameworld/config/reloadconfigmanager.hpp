#ifndef __RELOAD_CONFIG_MANAGER_HPP__
#define __RELOAD_CONFIG_MANAGER_HPP__

enum RELOAD_CONFIG_TYPE
{
	RELOAD_CONFIG_TYPE_INVALID = 0,
	RELOAD_CONFIG_TYPE_GLOBAL_CONFIG,								// 全局
	RELOAD_CONFIG_TYPE_SKILL,										// 技能
	RELOAD_CONFIG_TYPE_TASK,										// 任务
	RELOAD_CONFIG_TYPE_MONSTER,										// 怪物
	RELOAD_CONFIG_TYPE_ITEM,										// 物品
	RELOAD_CONFIG_TYPE_LOGIC_CONFIG,								// 逻辑
	RELOAD_CONFIG_TYPE_DROP,										// 掉落
	RELOAD_CONFIG_TYPE_SHIZHUANG,									// 时装
	RELOAD_CONFIG_TYPE_SCENE,										// 场景
	RELOAD_CONFIG_TYPE_SHARED,										// 共享
	RELOAD_CONFIG_TYPE_ONE_ITEM,									// 单个物品
	RELOAD_CONFIG_TYPE_GSTR,										// 字符串资源
	RELOAD_CONFIG_TYPE_MAX,
};

class ReloadConfigManager
{
public:
	static ReloadConfigManager & Instance();

	bool ReloadConfig(int reload_cfg_type, int param1, int param2);

private:
	ReloadConfigManager();
	~ReloadConfigManager();
};

#endif //__RELOAD_CONFIG_MANAGER_HPP__



