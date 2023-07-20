#pragma once
#include "BufferWriter.h"
#include "BufferReader.h"

enum
{
	S_TEST = 0,
	C_LOGIN = 1,
	S_LOGIN = 2,
};

class ServerPacketHandler
{
public:
	static void HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len);

	static void Handle_S_TEST(PacketSessionRef& session, BYTE* buffer, int32 len);
	static void Handle_S_LOGIN(PacketSessionRef& session, BYTE* buffer, int32 len);
};


template<typename T, typename C>
class PacketIterator
{
public:
	PacketIterator(C& container, uint16 index) : _container(container), _index(index) { }

	bool				operator!=(const PacketIterator& other) const { return _index != other._index; }
	const T&			operator*() const { return _container[_index]; }
	T&					operator*() { return _container[_index]; }
	T*					operator->() { return &_container[_index]; }
	PacketIterator&		operator++() { _index++; return *this; }
	PacketIterator		operator++(int32) { PacketIterator ret = *this; ++_index; return ret; }

private:
	C&				_container;
	uint16			_index;
};

template<typename T>
class PacketList
{
public:
	PacketList() : _data(nullptr), _count(0) { }
	PacketList(T* data, uint16 count) : _data(data), _count(count) { }

	T& operator[](uint16 index)
	{
		ASSERT_CRASH(index < _count);
		return _data[index];
	}

	uint16 Count() { return _count; }

	// ranged-base for 지원
	PacketIterator<T, PacketList<T>> begin() { return PacketIterator<T, PacketList<T>>(*this, 0); }
	PacketIterator<T, PacketList<T>> end() { return PacketIterator<T, PacketList<T>>(*this, _count); }

private:
	T*			_data;
	uint16		_count;
};

#pragma pack(1)
// [ PKT_S_TEST ][BuffsListItem BuffsListItem BuffsListItem][ victim victim ] [victim victim]
struct PKT_S_TEST
{
	struct BuffsListItem
	{
		uint64 buffId;
		float remainTime;

		uint16 victimsOffset;
		uint16 victimsCount;

		bool Validate(BYTE* packetStart, uint16 packetSize, OUT uint32& size)
		{
			if (victimsOffset + victimsCount * sizeof(uint64) > packetSize)
				return false;

			size += victimsCount * sizeof(uint64);
			return true;
		}
	};

	uint16 packetSize; // 공용 헤더
	uint16 packetId; // 공용 헤더
	uint64 id; // 8
	uint32 hp; // 4
	uint16 attack; // 2
	uint16 buffsOffset;
	uint16 buffsCount;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_TEST);
		if (packetSize < size)
			return false;

		if (buffsOffset + buffsCount * sizeof(BuffsListItem) > packetSize)
			return false;

		// Buffers 가변 데이터 크기 추가
		size += buffsCount * sizeof(BuffsListItem);

		BuffsList buffList = GetBuffsList();
		for (int32 i = 0; i < buffList.Count(); i++)
		{
			if (buffList[i].Validate((BYTE*)this, packetSize, OUT size) == false)
				return false;
		}

		// 최종 크기 비교
		if (size != packetSize)
			return false;

		return true;
	}

	using BuffsList = PacketList<PKT_S_TEST::BuffsListItem>;
	using BuffsVictimsList = PacketList<uint64>;

	BuffsList GetBuffsList()
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += buffsOffset;
		return BuffsList(reinterpret_cast<PKT_S_TEST::BuffsListItem*>(data), buffsCount);
	}

	BuffsVictimsList GetBuffsVictimList(BuffsListItem* buffsItem)
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += buffsItem->victimsOffset;
		return BuffsVictimsList(reinterpret_cast<uint64*>(data), buffsItem->victimsCount);
	}

	//vector<BuffData> buffs;
	//wstring name;
};
#pragma pack()

#pragma pack(1)
struct PKT_C_LOGIN
{	
	struct NickNameStruct {
		wchar_t nickname;
	};

	uint16 packetSize;
	uint16 packetId;
	uint16 nicknameOffset;
	uint16 nicknameCount;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_C_LOGIN);
		if (packetSize < size)
			return false;

		if (nicknameOffset + nicknameCount * sizeof(BYTE)*2 > packetSize)
			return false;

		size += nicknameCount * sizeof(BYTE)*2;

		NickName nickname = GetNickName();

		if (size != packetSize)
			return false;

		return true;
	}

	using NickName = PacketList<NickNameStruct>;

	NickName GetNickName()
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += nicknameOffset;
		return NickName(reinterpret_cast<PKT_C_LOGIN::NickNameStruct*>(data), nicknameCount);
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_S_LOGIN
{
	uint16 packetSize;
	uint16 packetId;
	bool success;
	uint64 playerId;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_LOGIN);
		if (packetSize < size)
			return false;


		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

//===============================
// 이 밑은 패킷 Write 클래스 모음입니다. |
// ==============================

#pragma pack(1)
class PKT_C_LOGIN_WRITE 
{
public:
	using NickNameStruct = PKT_C_LOGIN::NickNameStruct;
	using NickName = PacketList<PKT_C_LOGIN::NickNameStruct>;

	PKT_C_LOGIN_WRITE() {
		_sendBuffer = GSendBufferManager->Open(4096);
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());

		_pkt = _bw.Reserve<PKT_C_LOGIN>();
		_pkt->packetSize = 0;
		_pkt->packetId = C_LOGIN;
		_pkt->nicknameOffset = 0;
		_pkt->nicknameCount = 0;
	}

	NickName ReserveNickName(uint16 buffCount) {
		NickNameStruct* firstBuffsListItem = _bw.Reserve<NickNameStruct>(buffCount);
		_pkt->nicknameOffset = (uint64)firstBuffsListItem - (uint64)_pkt;
		_pkt->nicknameCount = buffCount;
		return NickName(firstBuffsListItem, buffCount);
	}

	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈 계산
		_pkt->packetSize = _bw.WriteSize();

		_sendBuffer->Close(_bw.WriteSize());
		return _sendBuffer;
	}

private:
	PKT_C_LOGIN* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};
#pragma pack()