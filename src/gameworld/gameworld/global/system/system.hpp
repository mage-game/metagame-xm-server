/******************************************************************************************
FileName: system.hpp
Author: 
Description: 系统相关
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreanwang    2011/08/17/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __SYSTEM_HPP__
#define __SYSTEM_HPP__

#include <string>

#include "iclockmodule.h"
#include "servercommon/struct/system/publicnoticelist.h"


class System
{
	friend class RMIGetCmdBackObjectImpl;

public:
	static System &Instance();

	void Init();
	void Update(unsigned long interval, time_t now_second);
	
	bool NormalNoticeIsEmpty() { return m_normal_notice_count <= 0; }
	void AddNormalNotice(const PublicNoticeList::NoticeAttr &notice_attr);
	void UpdateNormalNotice(time_t now_second);

	void ResetNotice();

private:
	System();
	~System();

	enum NEWNOTICE_TYPE
	{
		NEWNOTICE_TYPE_INVALID = 0,
		NEWNOTICE_TYPE_QUANFU_MAIL,				// 全服邮件				
		NEWNOTICE_TYPE_PERSON_MAIL,				// 系统发给个人邮件
		NEWNOTICE_TYPE_CHONGZHI,				// 充值

		NEWNOTICE_TYPE_MAX,
	};

	void OnCmd(int cmd_id, const std::string &creator, const std::string &cmd);
	void NewNoticeCmdHelper(int notice_type, int role_id);

	void StartPublicNoticeCheck();				// 到数据库检查是否有公告信息
	void StartCmdCheck();						// 到数据库检查是否有命令

	time_t m_next_get_public_notice;			// 下次请求公告时间
	time_t m_next_get_cmd;						// 下次请求命令时间

	int m_curr_normal_notice_index;				// 当前的公告索引
	time_t m_next_normal_notice_time;			// 下次播放公告时间
	int m_normal_notice_count;					// 正常公告的条数
	PublicNoticeList::NoticeAttr m_normal_noticelist[PublicNoticeList::MAX_PUBLICNOTICE_NUM];	// 正常公告一轮数据
};

#endif  // __SYSTEM_HPP__

