/******************************************************************************************
FileName: system.hpp
Author: 
Description: ϵͳ���
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreanwang    2011/08/17/ 09:04:05     1.0           ����
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
		NEWNOTICE_TYPE_QUANFU_MAIL,				// ȫ���ʼ�				
		NEWNOTICE_TYPE_PERSON_MAIL,				// ϵͳ���������ʼ�
		NEWNOTICE_TYPE_CHONGZHI,				// ��ֵ

		NEWNOTICE_TYPE_MAX,
	};

	void OnCmd(int cmd_id, const std::string &creator, const std::string &cmd);
	void NewNoticeCmdHelper(int notice_type, int role_id);

	void StartPublicNoticeCheck();				// �����ݿ����Ƿ��й�����Ϣ
	void StartCmdCheck();						// �����ݿ����Ƿ�������

	time_t m_next_get_public_notice;			// �´����󹫸�ʱ��
	time_t m_next_get_cmd;						// �´���������ʱ��

	int m_curr_normal_notice_index;				// ��ǰ�Ĺ�������
	time_t m_next_normal_notice_time;			// �´β��Ź���ʱ��
	int m_normal_notice_count;					// �������������
	PublicNoticeList::NoticeAttr m_normal_noticelist[PublicNoticeList::MAX_PUBLICNOTICE_NUM];	// ��������һ������
};

#endif  // __SYSTEM_HPP__

