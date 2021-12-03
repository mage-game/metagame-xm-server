#ifndef __RELOAD_CONFIG_MANAGER_HPP__
#define __RELOAD_CONFIG_MANAGER_HPP__

enum RELOAD_CONFIG_TYPE
{
	RELOAD_CONFIG_TYPE_INVALID = 0,
	RELOAD_CONFIG_TYPE_GLOBAL_CONFIG,								// ȫ��
	RELOAD_CONFIG_TYPE_SKILL,										// ����
	RELOAD_CONFIG_TYPE_TASK,										// ����
	RELOAD_CONFIG_TYPE_MONSTER,										// ����
	RELOAD_CONFIG_TYPE_ITEM,										// ��Ʒ
	RELOAD_CONFIG_TYPE_LOGIC_CONFIG,								// �߼�
	RELOAD_CONFIG_TYPE_DROP,										// ����
	RELOAD_CONFIG_TYPE_SHIZHUANG,									// ʱװ
	RELOAD_CONFIG_TYPE_SCENE,										// ����
	RELOAD_CONFIG_TYPE_SHARED,										// ����
	RELOAD_CONFIG_TYPE_ONE_ITEM,									// ������Ʒ
	RELOAD_CONFIG_TYPE_GSTR,										// �ַ�����Դ
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



