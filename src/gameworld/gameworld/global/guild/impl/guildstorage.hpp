#ifndef __GUILD_STORAGE_HPP__
#define __GUILD_STORAGE_HPP__

#include "servercommon/struct/global/guildparam.hpp"

#include "global/guild/impl/guildmember.hpp"

#include "protocal/msgscguild.hpp"
#include "protocal/msgguild.hpp"

class Guild;
class Role;
struct ItemDataWrapper;

class GuildStorage
{
public:
	GuildStorage();
	~GuildStorage();

	void SetGuild(Guild *guild) { m_guild = guild;}
	Guild * GetGuild() { return m_guild; }

	void Init(const GuildBaseParam &param);
	void GetStorgeItemList(GuildBaseParam *param);
	void ClearDirtyMark();

	void OnPutItem(Role *role, int bag_index, int putitem_num);
	void OnPutItemOneKey(Role *role, Protocol::CSGuildStorgeOneKeyOperate *cmd);
	void OnTakeItem(Role *role, int storge_index, int takeitem_num, ItemID item_id);
	void OnDiscardItem(Role *role, int storge_index, ItemID item_id);
	void OnDiscardItemOneKey(Role *role, Protocol::CSGuildStorgeOneKeyOperate *cmd);
	void SyncStorgeChangeToAllMember(int storge_index);
	void SyncStorgeInfo(Role *role);
	void SyncStorgeLog(Role *role);

private:

	void CheckAddConstantItem();    // 添加固定物品ID
	void SetDirty(int item_index);

	Guild *m_guild;

	int m_count;

	ItemDataWrapper m_item_list[GUILD_STORAGE_MAX_GRID_NUM];		// 仙盟仓库列表
};

#endif  // __GUILD_STORAGE_HPP__
