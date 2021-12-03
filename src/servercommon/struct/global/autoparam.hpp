#pragma once

#include "common/tlvprotocol.h"

class AutoParamBase
{
public:
	virtual ~AutoParamBase() = default;
	virtual int Size() const = 0;
	virtual void Reset() = 0;
	virtual bool Serialize(TLVSerializer &serializer) = 0;
	virtual bool Unserialize(TLVUnserializer &unserializer) = 0;
};

template<typename StructParam>
class AutoParamTemplate : public AutoParamBase
{
public:
	int Size() const override { return sizeof(m_data); }
	void Reset() override { m_data.Reset(); }
	bool Serialize(TLVSerializer &serializer) override
	{
		TLVSerializer data;
		data.Reset(static_cast<void*>(&m_data), sizeof(m_data));
		data.MoveCurPos(sizeof(m_data));
		return serializer.Push(data);
	}

	bool Unserialize(TLVUnserializer &unserializer) override
	{
		m_data.Reset();
		if (unserializer.IsAllPoped()) return true;
		TLVUnserializer data;
		if (!unserializer.Pop(&data))
		{
			printf("UNSERIALIZE_USER_DATA %s Pop fail!\n", __FUNCTION__); return false;
		}
		if (sizeof(m_data) >= data.Size())
		{
			memcpy(&m_data, data.Ptr(), data.Size());
		}
		else
		{
			printf("UNSERIALIZE_USER_DATA %s size check fail!\n", __FUNCTION__); return false;
		}

		return true;
	}

	StructParam & Param() { return m_data; }
	const StructParam & Param() const { return m_data; }

private:
	StructParam m_data;
};
