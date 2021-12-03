#include "packetstream.hpp"
#include <string.h>

char g_struct_code_buffer[MAX_STRUCT_CODE_BUFFER_LEN];

CPacketEncoder::CPacketEncoder()
{
	m_aucCodeBuf = NULL;
	m_pCurCodePos = NULL;
	m_bError = false;
}

CPacketEncoder::~CPacketEncoder()
{
	m_aucCodeBuf = NULL;
	m_pCurCodePos = NULL;
	m_iMaxCodeLen = 0;
	m_bError = false;
}

int CPacketEncoder::Initialize(char* pucCodeBuf, int iMaxCodeLen)
{
	if(pucCodeBuf == NULL)
	{
		return -1;
	}

	m_aucCodeBuf = pucCodeBuf;
	m_pCurCodePos = pucCodeBuf;
	m_iMaxCodeLen = iMaxCodeLen;
	m_bError = false;

	return 0;
}


CPacketEncoder& CPacketEncoder::operator <<(char cChar)
{
	return (*this << (unsigned char)cChar);
}

CPacketEncoder& CPacketEncoder::operator <<(unsigned char ucChar)
{
	if(m_pCurCodePos == NULL)
	{
		return *this;
	}
	if(!CanEncodeLen((int)sizeof(unsigned char)))
	{
		return *this;
	}

	*m_pCurCodePos++ = ucChar;

	return *this;
}

CPacketEncoder& CPacketEncoder::operator <<(short shShort16)
{
	return (*this << (unsigned short)shShort16);
}

CPacketEncoder& CPacketEncoder::operator <<(unsigned short ushShort16)
{
	if(m_pCurCodePos == NULL)
	{
		return *this;
	}
	if(!CanEncodeLen((int)sizeof(unsigned short)))
	{
		return *this;
	}

	*(unsigned short *)m_pCurCodePos = ushShort16;
	m_pCurCodePos += sizeof(unsigned short);

	return *this;
}

CPacketEncoder& CPacketEncoder::operator <<(int iInt32)
{
	return (*this << (unsigned int)iInt32);
}

CPacketEncoder& CPacketEncoder::operator <<(unsigned int uiInt32)
{
	if(m_pCurCodePos == NULL)
	{
		return *this;
	}
	if(!CanEncodeLen((int)sizeof(unsigned int)))
	{
		return *this;
	}

	*(unsigned int *)m_pCurCodePos = uiInt32;
	m_pCurCodePos += sizeof(unsigned int);

	return *this;
}

CPacketEncoder& CPacketEncoder::operator <<(long long iInt64)
{
	return (*this << (unsigned long long)iInt64);
}

CPacketEncoder& CPacketEncoder::operator <<(unsigned long long uiInt64)
{
	if(m_pCurCodePos == NULL)
	{
		return *this;
	}
	if(!CanEncodeLen((int)sizeof(unsigned long long)))
	{
		return *this;
	}

	*(unsigned long long *)m_pCurCodePos = uiInt64;
	m_pCurCodePos += sizeof(unsigned long long);

	return *this;
}

//编码Mem
int CPacketEncoder::EncodeMem(char *pcSrc, int iMemSize)
{
	if(m_pCurCodePos == NULL || pcSrc == NULL || iMemSize <= 0)
	{
		return -1;
	}

	if(m_pCurCodePos + (unsigned int)iMemSize > m_aucCodeBuf + m_iMaxCodeLen)
	{
		return -2;
	}

	if(!CanEncodeLen(iMemSize))
	{
		return -100;
	}

	memcpy(m_pCurCodePos, pcSrc, iMemSize);
	m_pCurCodePos += (unsigned int)iMemSize;

	return 0;
}

int CPacketEncoder::EncodeString(char *pcSrc, short shMaxStrLength)
{
	unsigned short ushTempLength = (unsigned short)strlen(pcSrc);

	if(ushTempLength > (unsigned short)shMaxStrLength)
	{
		ushTempLength = (unsigned short)shMaxStrLength;
	}

	if(m_pCurCodePos + (unsigned int)ushTempLength > m_aucCodeBuf + m_iMaxCodeLen)
	{
		return -2;
	}

	if(!CanEncodeLen((int)sizeof(unsigned short)))
	{
		return -100;
	}

	*(unsigned short *)m_pCurCodePos = ushTempLength;
	m_pCurCodePos += sizeof(unsigned short);

	if(!CanEncodeLen(ushTempLength))
	{
		return -200;
	}

	strncpy((char*)m_pCurCodePos, pcSrc, ushTempLength);

	if(ushTempLength == shMaxStrLength)
	{
		m_pCurCodePos[ushTempLength-1] = '\0';
	}

	m_pCurCodePos += (unsigned int)ushTempLength;

	return 0;
}

bool CPacketEncoder::CanEncodeLen(int iLen)
{
	if(m_pCurCodePos + iLen > m_aucCodeBuf + m_iMaxCodeLen)
	{
		m_bError = true;

		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////


CPacketDecoder::CPacketDecoder()
{
	m_aucCodeBuf = NULL;
	m_pCurCodePos = NULL;
	m_bError = false;
}

CPacketDecoder::~CPacketDecoder()
{
	m_aucCodeBuf = NULL;
	m_pCurCodePos = NULL;
	m_iCodeLen = 0;
	m_bError = false;
}

int CPacketDecoder::Initialize(char* pucCodeBuf, int iCodeLen)
{
	if(pucCodeBuf == NULL)
	{
		return -1;
	}

	m_aucCodeBuf = pucCodeBuf;
	m_pCurCodePos = pucCodeBuf;
	m_iCodeLen = iCodeLen;
	m_bError = false;

	return 0;
}

//解码char
CPacketDecoder& CPacketDecoder::operator >>(char& rcChar)
{
	return (*this >> (unsigned char&)rcChar);
}

//解码char
CPacketDecoder& CPacketDecoder::operator >>(unsigned char& rucChar)
{
	if(m_pCurCodePos == NULL)
	{
		return *this;
	}
	if(!CanDecodeLen((int)sizeof(unsigned char)))
	{
		return *this;	//溢出了
	}

	rucChar = *m_pCurCodePos++;

	return *this;

}

CPacketDecoder& CPacketDecoder::operator >>(short& rshShort16)
{
	return (*this >> (unsigned short&)rshShort16);
}

//解码short16
CPacketDecoder& CPacketDecoder::operator >>(unsigned short& rushShort16)
{
	if(m_pCurCodePos == NULL)
	{
		return *this;
	}
	if(!CanDecodeLen((int)sizeof(unsigned short)))
	{
		return *this;	//溢出了
	}

	rushShort16 = *(unsigned short *)m_pCurCodePos;
	m_pCurCodePos += sizeof(unsigned short);

	return *this;

}

CPacketDecoder& CPacketDecoder::operator >>(int& riInt32)
{
	return (*this >> (unsigned int&)riInt32);
}

//解码Int32
CPacketDecoder& CPacketDecoder::operator >>(unsigned int& ruiInt32)
{
	if(m_pCurCodePos == NULL)
	{
		return *this;
	}
	if(!CanDecodeLen((int)sizeof(unsigned int)))
	{
		return *this;	//溢出了
	}

	ruiInt32 = *(unsigned int *)m_pCurCodePos;
	m_pCurCodePos += sizeof(unsigned int);

	return *this;
}

CPacketDecoder& CPacketDecoder::operator >>(long long& riInt64)
{
	return (*this >> (unsigned long long&)riInt64);
}

CPacketDecoder& CPacketDecoder::operator >>(unsigned long long& ruiInt64)
{
	if(m_pCurCodePos == NULL)
	{
		return *this;
	}
	if(!CanDecodeLen((int)sizeof(unsigned long long)))
	{
		return *this;	//溢出了
	}

	ruiInt64 = *(unsigned long long *)m_pCurCodePos;
	m_pCurCodePos += sizeof(unsigned long long);

	return *this;
}

int CPacketDecoder::DecodeMem(char *pcDest, int iMemSize)
{
	if(m_pCurCodePos == NULL || pcDest == NULL || iMemSize <= 0)
	{
		return -1;
	}
	if(!CanDecodeLen(iMemSize))
	{
		return -100;	//溢出了
	}

	memcpy(pcDest, m_pCurCodePos, iMemSize);
	m_pCurCodePos += (unsigned int)iMemSize;

	return 0;
}

int CPacketDecoder::DecodeString( char *strDest, short shMaxStrLength )
{
	if(m_pCurCodePos == NULL || strDest == NULL || shMaxStrLength <= 0)
	{
		return -1;
	}

	if(!CanDecodeLen((int)sizeof(unsigned short)))
	{
		return -100;	//溢出了
	}

	short shStrLen = *(unsigned short *)m_pCurCodePos;
	m_pCurCodePos += sizeof(unsigned short);

	if(m_pCurCodePos + (unsigned int)shStrLen > m_aucCodeBuf + m_iCodeLen)
	{
		return -2;
	}
	if(!CanDecodeLen(shStrLen))
	{
		return -200;	//溢出了
	}

	unsigned short ushRealLength = 0;
	--shMaxStrLength;

	if( shStrLen > shMaxStrLength )
	{
		ushRealLength = shMaxStrLength;
	}
	else
	{
		ushRealLength = shStrLen;
	}

	if(ushRealLength > 0)
	{
		strncpy(strDest, (char*)m_pCurCodePos, ushRealLength);
		strDest[ushRealLength] = '\0';
	}
	else
	{
		strDest[0] = '\0';
	}
	m_pCurCodePos += shStrLen;

	return 0;
}
bool CPacketDecoder::CanDecodeLen(int iLen)
{
	//当前加上欲读的字节数超过传进来 的字节数，就有问题
	if(m_pCurCodePos + iLen > m_aucCodeBuf + m_iCodeLen)
	{
		m_bError = true;
		
		return false;
	}

	return true;
}
