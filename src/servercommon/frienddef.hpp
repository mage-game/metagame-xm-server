/******************************************************************************************
FileName: frienddef.hpp
Author: shell
Description: ���ѡ����ˡ���������صĶ���
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
shell       2011/08/01/ 16:03:08     1.0           ����
*****************************************************************************************/

#ifndef __FRIENDDEF_H__
#define __FRIENDDEF_H__

#include "servercommon/serverdef.h"
#include "servercommon/servercommon.h"

static const int MAX_FRIEND_NUM = 200;			// ����������
static const int MAX_ENEMY_NUM = 100;			// ����������
static const int MAX_BLACK_NUM = 100;			// ������������

static const int EXPAND_SIZE = 4;				// �����ڴ�ÿ��Ԥ����С
static const int FRIENDNUM_FOR_LEVEL_COUNT = 4; // �������������ͬ����θ���

static const int FRIENDNUM_HELI_COND_TYPE_MAX_COUNT = 30;	// ���Ѻ���ף�������������ֵ
static const int DEC_FRIEND_OFFLINE_TIME = 3;				// ��������x�첢�����ܶ�����xʱɾ������(�죩
static const int DEC_FRIEND_OFFLINE_INTIMACY = 99;			// ��������x�첢�����ܶ�����xʱɾ������(���ܶȣ�

#pragma pack(push, 4)

enum AddFriendStatus
{
	ADDFRIEND_REJECT = 0,						// �ܾ�
	ADDFRIEND_AGREE = 1,						// ͬ��
	ADDFRIEND_OTHERFULL = 2,					// �Է�����
};

#pragma pack(pop)

#endif

