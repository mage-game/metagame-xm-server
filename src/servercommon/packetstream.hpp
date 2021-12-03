#ifndef _PACKETSTREAM_HPP_
#define _PACKETSTREAM_HPP_

const int MAX_STRUCT_CODE_BUFFER_LEN = 1024 * 1024;

class CPacketEncoder
{
public:
	CPacketEncoder();
	~CPacketEncoder();

public:
	int Initialize(char* pucCodeBuf, int iMaxCodeLen);

	int GetCurOffset() const
	{
		if(m_bError)
		{
			return 0x0fffffff;
		}
		return (int)(m_pCurCodePos - m_aucCodeBuf);
	}

	void SetCurOffset(int iOffset) { m_pCurCodePos = m_aucCodeBuf + iOffset; }

	CPacketEncoder& operator <<(char cChar);
	CPacketEncoder& operator <<(unsigned char ucChar);

	CPacketEncoder& operator <<(short shShort16);
	CPacketEncoder& operator <<(unsigned short ushShort16);

	CPacketEncoder& operator <<(int iInt32);
	CPacketEncoder& operator <<(unsigned int uiInt32);

	CPacketEncoder& operator <<(unsigned long long uiInt64);
	CPacketEncoder& operator <<(long long iInt64);

	int EncodeMem(char *pcSrc, int iMemSize);

	//�൱��PutUTF
	int EncodeString(char *pcSrc, short shMaxStrLength);

	bool IsError()
	{
		return m_bError;
	}
private:
	bool CanEncodeLen(int iLen);

	char* GetCodeBuf() const { return m_aucCodeBuf; }

private:
	char* m_aucCodeBuf;				//!<��ű��������������
	char* m_pCurCodePos;			//!<��ǰ����λ��
	int m_iMaxCodeLen; 
	bool m_bError;
};

//////////////////////////////////////////////////////////////////////////

class CPacketDecoder
{
public:
	CPacketDecoder();
	~CPacketDecoder();

public:
	int Initialize(char* pucCodeBuf, int iCodeLen);

	int GetCurOffset() const
	{
		if(m_bError)
		{
			return 0x0fffffff;
		}
		return (int)(m_pCurCodePos - m_aucCodeBuf);
	}

	void SetCurOffset(int iOffset) { m_pCurCodePos = m_aucCodeBuf + iOffset; }

	CPacketDecoder& operator >>(char& rcChar);
	CPacketDecoder& operator >>(unsigned char& rucChar);

	CPacketDecoder& operator >>(short& rshShort16);
	CPacketDecoder& operator >>(unsigned short& rushShort16);

	CPacketDecoder& operator >>(int& riInt32);
	CPacketDecoder& operator >>(unsigned int& ruiInt32);

	CPacketDecoder& operator >>(long long& riInt64);
	CPacketDecoder& operator >>(unsigned long long& ruiInt64);

	int DecodeMem(char *pcDest, int iMemSize);

	bool IsError()
	{
		return m_bError;
	}
	//�൱��GetUTF
	int DecodeString(char *strDest, short shMaxStrLength);
private:
	//�ж��Ƿ��ܽ����ô��������
	bool CanDecodeLen(int iLen);

	char* GetCodeBuf() const { return m_aucCodeBuf; }

private:
	int m_iCodeLen; //�յ���Buffer����
	char* m_aucCodeBuf; 
	char* m_pCurCodePos; 
	bool m_bError;
};


#endif

