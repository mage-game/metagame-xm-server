#ifndef __OFFLINE_REGISTER_HPP__
#define __OFFLINE_REGISTER_HPP__

#include <stdlib.h>

#include "servercommon/serverdef.h"

#include "servercommon/struct/global/offlineregisterparam.hpp"
#include "obj/character/role.h"

class OfflineRegister
{
public:
	OfflineRegister();
	OfflineRegister(const UserID &user_id);
	~OfflineRegister();

	void Init(const OfflineRegisterParam::OfflineRegisterAttr &attr);
	void GetAttr(OfflineRegisterParam::OfflineRegisterAttr *attr);

	const UserID & GetUserID() const { return m_user_id; }

	int GetIncreaseCharm() const { return m_increase_charm; }
	void AddIncreaseCharm(int num = 1) { m_increase_charm += num; }
	void ClearIncreaseCharm() { m_increase_charm = 0; }

	void SetLastDivorceTime(unsigned int divorce_time) { m_last_divorce_time = divorce_time; }
	unsigned int GetLastDivorceTime() { return m_last_divorce_time; }

	void OnChongzhiCmd(int chongzhi_num);
	int GetChongzhiCmdFlag() { return m_chongzhi_cmd_flag; }
	void ClearChongzhiCmdFlag() { m_chongzhi_cmd_flag = 0; }

	void OnRedPaperConsumeGold(int gold_num) { m_red_paper_consume_gold += gold_num; }
	int GetRedPaperConsumeGold() { return m_red_paper_consume_gold; }
	void ClearRedPaperConsumeGold() { m_red_paper_consume_gold = 0; }

	void OnAddRedPaperLuckyValue(int add_value) { m_red_paper_lucky_value += add_value; }
	int GetRedPaperLuckyValue() { return m_red_paper_lucky_value; }
	void ClearRedPaperLuckyValue() { m_red_paper_lucky_value = 0; }

	void OnSpaceBeCai() { m_space_cai_count ++; }
	int GetSpaceCaiCount() { return m_space_cai_count; }
	void ClearSpaceCaiCount() { m_space_cai_count = 0; }

	void OnSpaceBeGift() { m_space_gift_count ++; }
	int GetSpaceGiftCount() { return m_space_gift_count; }
	void ClearSpaceGiftCount() { m_space_gift_count = 0; }

	void SetForbidTalkTimestamp(unsigned int forbid_end_timestamp) { m_common_param.forbid_talk_end_timestamp = forbid_end_timestamp; }
	unsigned int GetForbidTalkTimestamp() { return m_common_param.forbid_talk_end_timestamp; }
	void ClearForbidTalkTimestamp() { m_common_param.forbid_talk_end_timestamp = 0; }

	void SetQingyuanBlessDays(int bless_days) { m_qingyuan_bless_days = bless_days; }
	int *GetQingyuanBlessDays() { return &m_qingyuan_bless_days; }

	GuildBoxInfo *GetGuildBoxInfo() { return &m_guild_box_info; }

	void OnSetAuthorityType(int authority_type) { m_common_param.authority_type = static_cast<short>(authority_type); }
	const short GetAuthorityType() { return m_common_param.authority_type; }
	void ClearAuthortiyType() { m_common_param.authority_type = -1; }

	void OnSetConsumeItem(unsigned short item_id, unsigned short item_num);
	bool ConsumeItem(Role* role);


	void OnSetConsumeGold(int num);
	void OnSetConsumeBindGold(int num);
	bool ConsumeGold(Role* role);
// 	void OnProfessFrom(Role* profess_role, PersonProfessItem& item);
// 	int GetProfessCount() { return m_profess_param.profess_count; }
// 	PersonProfessItem* GetProfessByOther();
// 	void ClearProfess();

	bool CanRemove(unsigned int now_second);

	void * operator new(size_t c);
	void operator delete(void *m);


	long long GetOffLineRegisterID() { return m_id_offline_register; }
private:
	long long m_id_offline_register;

	UserID m_user_id;

	int m_increase_charm;									// 增加魅力

	unsigned int m_last_divorce_time;						// 上次离婚时间

	int m_chongzhi_cmd_flag;								// 充值命令标记

	int m_red_paper_consume_gold;							// 发红包消耗元宝
	int m_red_paper_lucky_value;							// 红包幸运值

	int m_space_cai_count;									// 离线期间 空间被踩次数
	int m_space_gift_count;									// 离线期间 空间被送礼次数

	int m_qingyuan_bless_days;								// 情缘祝福天数

	GuildBoxInfo m_guild_box_info;							// 仙盟宝箱信息

	OfflineRegisterCommonParam m_common_param;
};

#endif // __OFFLINE_REGISTER_HPP__

