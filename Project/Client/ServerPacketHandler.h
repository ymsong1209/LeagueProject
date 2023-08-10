#pragma once
#include "BufferWriter.h"
#include "BufferReader.h"
#include "CoreMacro.h"

#include "ServerFunc.h"
enum
{
	S_TEST = 0,

	C_LOGIN = 1,
	S_LOGIN = 2,
	C_PICK_FACTION = 3,
	S_PICK_FACTION = 4,
	C_PICK_CHAMPION = 5,
	S_PICK_CHAMPION = 6,

	S_GAME_START = 7,

	C_PLAYER_MOVE = 8,
	S_PLAYER_MOVE = 9,

	C_OBJECT_ANIM = 10,
	S_OBJECT_ANIM = 11,

	S_SPAWN_OBJECT = 12,

	C_OBJECT_MOVE = 13,
	S_OBJECT_MOVE = 14,

	C_SKILL_PROJECTILE = 15,
	S_SKILL_PROJECTILE = 16,

	C_SKILL_HIT = 17,
	S_SKILL_HIT = 18,

	C_DESPAWN_OBJECT = 19,
	S_DESPAWN_OBJECT = 20,

	C_KDA_CS = 21,
	S_KDA_CS = 22,

	C_SOUND = 23,
	S_SOUND = 24,

	C_TIME = 25,
	S_TIME = 26,

	C_OBJECT_MTRL = 27,
	S_OBJECT_MTRL = 28,
};

class ServerPacketHandler
{
public:
	static void HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len);
	static void Handle_S_TEST(PacketSessionRef& session, BYTE* buffer, int32 len);
	static void Handle_S_LOGIN(PacketSessionRef& session, BYTE* buffer, int32 len);
	static void Handle_S_PICK_FACTION(PacketSessionRef& session, BYTE* buffer, int32 len);
	static void Handle_S_PICK_CHAMPION(PacketSessionRef& session, BYTE* buffer, int32 len);
	static void Handle_S_GAME_START(PacketSessionRef& session, BYTE* buffer, int32 len);

	static void Handle_S_PLAYER_MOVE(PacketSessionRef& session, BYTE* buffer, int32 len);
	static void Handle_S_OBJECT_ANIM(PacketSessionRef& session, BYTE* buffer, int32 len);
	static void Handle_S_SPAWN_OBJECT(PacketSessionRef& session, BYTE* buffer, int32 len);
	static void Handle_S_OBJECT_MOVE(PacketSessionRef& session, BYTE* buffer, int32 len);

	static void Handle_S_SKILL_PROJECTILE(PacketSessionRef& session, BYTE* buffer, int32 len);
	static void Handle_S_SKILL_HIT(PacketSessionRef& session, BYTE* buffer, int32 len);
	static void Handle_S_DESPAWN_OBJECT(PacketSessionRef& session, BYTE* buffer, int32 len);

	static void Handle_S_KDA_CS(PacketSessionRef& session, BYTE* buffer, int32 len);
	static void Handle_S_SOUND(PacketSessionRef& session, BYTE* buffer, int32 len);
	static void Handle_S_TIME(PacketSessionRef& session, BYTE* buffer, int32 len);
	static void Handle_S_OBJECT_MTRL(PacketSessionRef& session, BYTE* buffer, int32 len);


private:
	USE_LOCK;
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
	struct PlayerListItem
	{
		uint64  playerId;
		Faction playerFaction;
		bool    host;

		uint16 nickNameOffset;
		uint16 nickNameCount;

		bool Validate(BYTE* packetStart, uint16 packetSize, OUT uint32& size)
		{
			if (nickNameOffset + nickNameCount * sizeof(BYTE) * 2 > packetSize)
				return false;

			size += nickNameCount * sizeof(BYTE) * 2;
			return true;
		}
	};

	struct NickNameStruct {
		wchar_t nickname;
	};

	uint16 packetSize;
	uint16 packetId;
	bool success;
	uint64 playerId;
	uint16 playerListoffset;
	uint16 playerListcount;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_LOGIN);
		if (packetSize < size)
			return false;

		if (playerListoffset + playerListcount * sizeof(PlayerListItem) > packetSize)
			return false;

		size += playerListcount * sizeof(PlayerListItem);

		PlayerList playerList = GetPlayerList();
		for (int32 i = 0; i < playerList.Count(); i++)
		{
			if (playerList[i].Validate((BYTE*)this, packetSize, OUT size) == false)
				return false;
		}

		if (size != packetSize)
			return false;

		return true;
	}

	using PlayerList = PacketList<PKT_S_LOGIN::PlayerListItem>;
	using NickNameList = PacketList<PKT_S_LOGIN::NickNameStruct>;

	PlayerList GetPlayerList()
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += playerListoffset;
		return PlayerList(reinterpret_cast<PKT_S_LOGIN::PlayerListItem*>(data), playerListcount);
	}

	NickNameList  GetNickNameList(PlayerListItem* playerList)
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += playerList->nickNameOffset;
		return NickNameList(reinterpret_cast<PKT_S_LOGIN::NickNameStruct*>(data), playerList->nickNameCount);
	}

};
#pragma pack()

#pragma pack(1)
struct PKT_C_PICK_FACTION
{
	uint16 packetSize;
	uint16 packetId;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_C_PICK_FACTION);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_S_PICK_FACTION
{
	uint16 packetSize;
	uint16 packetId;
	bool success;
	WaitingStatus waiting;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_PICK_FACTION);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_C_PICK_CHAMPION
{
	uint16 packetSize;
	uint16 packetId;
	ChampionType champion;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_C_PICK_CHAMPION);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_S_PICK_CHAMPION
{
	uint16 packetSize;
	uint16 packetId;
	bool   success;
	uint16 PlayerID;
	ChampionType champion;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_C_PICK_CHAMPION);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_S_GAME_START {
	uint16 packetSize;
	uint16 packetId;
	bool   success;
	uint16 playerInfoOffset;
	uint16 playerInfoCount;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_GAME_START);
		if (packetSize < size)
			return false;

		if (playerInfoOffset + playerInfoCount * sizeof(PlayerInfoPacket) > packetSize)
			return false;

		size += playerInfoCount * sizeof(PlayerInfoPacket);

		PlayerInfoList playerInfoList = GetPlayerInfoList();
		for (int32 i = 0; i < playerInfoList.Count(); i++)
		{
			if (playerInfoList[i].Validate((BYTE*)this, packetSize, OUT size) == false)
				return false;
		}

		if (size != packetSize)
			return false;

		return true;
	}

	using PlayerInfoList = PacketList<PlayerInfoPacket>;
	using NickNameList = PacketList<PlayerInfoPacket::NickNameItem>;

	PlayerInfoList GetPlayerInfoList()
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += playerInfoOffset;
		return PlayerInfoList(reinterpret_cast<PlayerInfoPacket*>(data), playerInfoCount);
	}

	NickNameList GetNickNameList(PlayerInfoPacket* playerInfoPacket)
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += playerInfoPacket->nickNameOffset;
		return NickNameList(reinterpret_cast<PlayerInfoPacket::NickNameItem*>(data), playerInfoPacket->nickNameCount);
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_C_PLAYER_MOVE
{
	uint16 packetSize;
	uint16 packetId;
	ObjectMove playerMove;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_C_PLAYER_MOVE);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_S_PLAYER_MOVE
{
	uint16 packetSize;
	uint16 packetId;
	uint64 playerId;
	ObjectMove playerMove;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_PLAYER_MOVE);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_C_OBJECT_ANIM {
	uint16 packetSize;
	uint16 packetId;
	uint64 sendId;
	AnimInfoPacket animInfo;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_C_OBJECT_ANIM);
		if (packetSize < size)
			return false;

		if (animInfo.Validate((BYTE*)this, packetSize, OUT size) == false)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}

	using AnimNameList = PacketList<AnimInfoPacket::animNameItem>;

	AnimNameList GetAnimNameList()
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += animInfo.animNameOffset;
		return AnimNameList(reinterpret_cast<AnimInfoPacket::animNameItem*>(data), animInfo.animNameCount);
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_S_OBJECT_ANIM {
	uint16 packetSize;
	uint16 packetId;
	uint64 sendId;
	AnimInfoPacket animInfo;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_OBJECT_ANIM);
		if (packetSize < size)
			return false;

		if (animInfo.Validate((BYTE*)this, packetSize, OUT size) == false)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}

	using AnimNameList = PacketList<AnimInfoPacket::animNameItem>;

	AnimNameList GetAnimNameList()
	{
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += animInfo.animNameOffset;
		return AnimNameList(reinterpret_cast<AnimInfoPacket::animNameItem*>(data), animInfo.animNameCount);
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_S_SPAWN_OBJECT {
	uint16 packetSize;
	uint16 packetId;
	ObjectInfo objectInfo;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_SPAWN_OBJECT);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_C_OBJECT_MOVE {
	uint16 packetSize;
	uint16 packetId;
	uint64 objectId;
	ObjectMove objectMove;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_C_OBJECT_MOVE);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_S_OBJECT_MOVE {
	uint16 packetSize;
	uint16 packetId;
	uint64 objectId;
	ObjectMove objectMove;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_OBJECT_MOVE);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_C_SKILL_PROJECTILE {
	uint16 packetSize;
	uint16 packetId;
	SkillInfo skillInfo;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_C_SKILL_PROJECTILE);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_S_SKILL_PROJECTILE {
	uint16 packetSize;
	uint16 packetId;
	uint64 projectileId;
	SkillInfo skillInfo;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_SKILL_PROJECTILE);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_C_SKILL_HIT {
	uint16 packetSize;
	uint16 packetId;
	uint64 objecId;
	SkillInfo skillInfo;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_C_SKILL_HIT);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_S_SKILL_HIT {
	uint16 packetSize;
	uint16 packetId;
	uint64 objecId;
	SkillInfo skillInfo;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_SKILL_HIT);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_C_DESPAWN_OBJECT {
	uint16		packetSize;
	uint16		packetId;
	uint64		objId;
	float		time;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_C_DESPAWN_OBJECT);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_S_DESPAWN_OBJECT {
	uint16		packetSize;
	uint16		packetId;
	uint64		objId;
	float		time;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_DESPAWN_OBJECT);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_C_KDA_CS {
	uint16      packetSize;
	uint16      packetId;
	KDACSInfo kdacsInfo;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_C_KDA_CS);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_S_KDA_CS {
	uint16      packetSize;
	uint16      packetId;
	KDACSInfo kdacsInfo;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_KDA_CS);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_C_SOUND {
	uint16 packetSize;
	uint16 packetId;
	SoundInfoPacket soundInfo;

	bool Validate() {
		{
			uint32 size = 0;
			size += sizeof(PKT_C_SOUND);
			if (packetSize < size)
				return false;

			if (soundInfo.Validate((BYTE*)this, packetSize, OUT size) == false)
				return false;

			if (size != packetSize)
				return false;

			return true;
		}
	}

	using SoundNameList = PacketList<SoundInfoPacket::soundNameItem>;

	SoundNameList GetSoundNameList() {
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += soundInfo.soundNameOffset;
		return SoundNameList(reinterpret_cast<SoundInfoPacket::soundNameItem*>(data), soundInfo.soundNameCount);
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_S_SOUND {
	uint16 packetSize;
	uint16 packetId;
	SoundInfoPacket soundInfo;

	bool Validate() {
		{
			uint32 size = 0;
			size += sizeof(PKT_S_SOUND);
			if (packetSize < size)
				return false;

			if (soundInfo.Validate((BYTE*)this, packetSize, OUT size) == false)
				return false;

			if (size != packetSize)
				return false;

			return true;
		}
	}

	using SoundNameList = PacketList<SoundInfoPacket::soundNameItem>;

	SoundNameList GetSoundNameList() {
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += soundInfo.soundNameOffset;
		return SoundNameList(reinterpret_cast<SoundInfoPacket::soundNameItem*>(data), soundInfo.soundNameCount);
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_C_TIME {
	uint16      packetSize;
	uint16      packetId;
	float         second;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_C_TIME);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_S_TIME {
	uint16      packetSize;
	uint16      packetId;
	float         second;

	bool Validate()
	{
		uint32 size = 0;
		size += sizeof(PKT_S_TIME);
		if (packetSize < size)
			return false;

		if (size != packetSize)
			return false;

		return true;
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_C_OBJECT_MTRL {
	uint16		packetSize;
	uint16		packetId;
	MtrlInfoPacket mtrlInfo;

	bool Validate() {
		{
			uint32 size = 0;
			size += sizeof(PKT_C_OBJECT_MTRL);
			if (packetSize < size)
				return false;

			if (mtrlInfo.Validate((BYTE*)this, packetSize, OUT size) == false)
				return false;

			if (size != packetSize)
				return false;

			return true;
		}
	}

	using TexNameList = PacketList<MtrlInfoPacket::texNameItem>;

	TexNameList GetTexNameList() {
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += mtrlInfo.texNameOffset;
		return TexNameList(reinterpret_cast<MtrlInfoPacket::texNameItem*>(data), mtrlInfo.texNameCount);
	}
};
#pragma pack()

#pragma pack(1)
struct PKT_S_OBJECT_MTRL {
	uint16		packetSize;
	uint16		packetId;
	MtrlInfoPacket mtrlInfo;

	bool Validate() {
		{
			uint32 size = 0;
			size += sizeof(PKT_S_OBJECT_MTRL);
			if (packetSize < size)
				return false;

			if (mtrlInfo.Validate((BYTE*)this, packetSize, OUT size) == false)
				return false;

			if (size != packetSize)
				return false;

			return true;
		}
	}

	using TexNameList = PacketList<MtrlInfoPacket::texNameItem>;

	TexNameList GetMtrlNameList() {
		BYTE* data = reinterpret_cast<BYTE*>(this);
		data += mtrlInfo.texNameOffset;
		return TexNameList(reinterpret_cast<MtrlInfoPacket::texNameItem*>(data), mtrlInfo.texNameCount);
	}
};
#pragma pack()

//=====================================
// 이 밑은 패킷 Write 클래스 모음입니다. |
//=====================================

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

#pragma pack(1)
class PKT_C_PICK_FACTION_WRITE
{
public:
	PKT_C_PICK_FACTION_WRITE() {
		_sendBuffer = GSendBufferManager->Open(4096);
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());

		_pkt = _bw.Reserve<PKT_C_PICK_FACTION>();
		_pkt->packetSize = 0;
		_pkt->packetId = C_PICK_FACTION;
	}
	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈 계산
		_pkt->packetSize = _bw.WriteSize();

		_sendBuffer->Close(_bw.WriteSize());
		return _sendBuffer;
	}

private:
	PKT_C_PICK_FACTION* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};
#pragma pack()

#pragma pack(1)
class PKT_C_PICK_CHAMPION_WRITE
{
public:
	PKT_C_PICK_CHAMPION_WRITE(ChampionType _champion) {
		_sendBuffer = GSendBufferManager->Open(4096);
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());

		_pkt = _bw.Reserve<PKT_C_PICK_CHAMPION>();
		_pkt->packetSize = 0;
		_pkt->packetId = C_PICK_CHAMPION;
		_pkt->champion = _champion;
	}

	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈 계산
		_pkt->packetSize = _bw.WriteSize();

		_sendBuffer->Close(_bw.WriteSize());
		return _sendBuffer;
	}

private:
	PKT_C_PICK_CHAMPION* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};
#pragma pack()

#pragma pack(1)
class PKT_C_PLAYER_MOVE_WRITE
{
public:
	PKT_C_PLAYER_MOVE_WRITE(ObjectMove _playerMove) {
		_sendBuffer = GSendBufferManager->Open(4096);
		// 초기화
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());

		_pkt = _bw.Reserve<PKT_C_PLAYER_MOVE>();
		_pkt->packetSize = 0; // To Fill
		_pkt->packetId = C_PLAYER_MOVE;
		_pkt->playerMove = _playerMove;
	}

	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈 계산
		_pkt->packetSize = _bw.WriteSize();

		_sendBuffer->Close(_bw.WriteSize());
		return _sendBuffer;
	}

private:
	PKT_C_PLAYER_MOVE* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};
#pragma pack()

#pragma pack(1)
class PKT_C_OBJECT_ANIM_WRITE {
public:
	using AnimNameList = PacketList<AnimInfoPacket::animNameItem>;
	using AnimNameItem = AnimInfoPacket::animNameItem;

	PKT_C_OBJECT_ANIM_WRITE(uint64 _sendId, /*animName은 가변 배열임으로 넣어주지 말것*/ AnimInfoPacket _animInfo)
	{
		_sendBuffer = GSendBufferManager->Open(4096);
		// 초기화
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());

		_pkt = _bw.Reserve<PKT_C_OBJECT_ANIM>();
		_pkt->packetSize = 0; // To Fill
		_pkt->packetId = C_OBJECT_ANIM;
		_pkt->sendId = _sendId;
		_pkt->animInfo = _animInfo;
	}

	AnimNameList ReserveAnimNameList(uint16 animCount) {
		AnimNameItem* firstBuffsListItem = _bw.Reserve<AnimNameItem>(animCount);
		_pkt->animInfo.animNameOffset = (uint64)firstBuffsListItem - (uint64)_pkt;
		_pkt->animInfo.animNameCount = animCount;
		return AnimNameList(firstBuffsListItem, animCount);
	}

	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈 계산
		_pkt->packetSize = _bw.WriteSize();

		_sendBuffer->Close(_bw.WriteSize());
		return _sendBuffer;
	}

private:
	PKT_C_OBJECT_ANIM* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};
#pragma pack()

#pragma pack(1)
class PKT_C_OBJECT_MOVE_WRITE {
public:
	PKT_C_OBJECT_MOVE_WRITE(uint64 _objectId, ObjectMove _objectMove) {
		_sendBuffer = GSendBufferManager->Open(4096);
		// 초기화
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());

		_pkt = _bw.Reserve<PKT_C_OBJECT_MOVE>();
		_pkt->packetSize = 0; // To Fill
		_pkt->packetId = C_OBJECT_MOVE;
		_pkt->objectId = _objectId;
		_pkt->objectMove = _objectMove;
	}

	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈 계산
		_pkt->packetSize = _bw.WriteSize();

		_sendBuffer->Close(_bw.WriteSize());
		return _sendBuffer;
	}

private:
	PKT_C_OBJECT_MOVE* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};
#pragma pack()

#pragma pack(1)
class PKT_C_SKILL_PROJECTILE_WRITE {
public:
	PKT_C_SKILL_PROJECTILE_WRITE(SkillInfo _skillInfo) {
		_sendBuffer = GSendBufferManager->Open(4096);
		// 초기화
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());

		_pkt = _bw.Reserve<PKT_C_SKILL_PROJECTILE>();
		_pkt->packetSize = 0; // To Fill
		_pkt->packetId = C_SKILL_PROJECTILE;
		_pkt->skillInfo = _skillInfo;
	}

	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈 계산
		_pkt->packetSize = _bw.WriteSize();

		_sendBuffer->Close(_bw.WriteSize());
		return _sendBuffer;
	}

private:
	PKT_C_SKILL_PROJECTILE* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};
#pragma pack()

#pragma pack(1)
class PKT_C_SKILL_HIT_WRITE {
public:
	PKT_C_SKILL_HIT_WRITE(uint64 _objectId, SkillInfo _skillInfo) {
		_sendBuffer = GSendBufferManager->Open(4096);
		// 초기화
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());

		_pkt = _bw.Reserve<PKT_C_SKILL_HIT>();
		_pkt->packetSize = 0; // To Fill
		_pkt->packetId = C_SKILL_HIT;
		_pkt->objecId = _objectId;
		_pkt->skillInfo = _skillInfo;
	}

	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈 계산
		_pkt->packetSize = _bw.WriteSize();

		_sendBuffer->Close(_bw.WriteSize());
		return _sendBuffer;
	}

private:
	PKT_C_SKILL_HIT* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};
#pragma pack()

#pragma pack(1)
class PKT_C_DESPAWN_OBJECT_WRITE {
public:
	PKT_C_DESPAWN_OBJECT_WRITE(uint64 _objId, float _time) {
		_sendBuffer = GSendBufferManager->Open(4096);
		// 초기화
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());

		_pkt = _bw.Reserve<PKT_C_DESPAWN_OBJECT>();
		_pkt->packetSize = 0; // To Fill
		_pkt->packetId = C_DESPAWN_OBJECT;
		_pkt->objId = _objId;
		_pkt->time = _time;
	}

	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈 계산
		_pkt->packetSize = _bw.WriteSize();

		_sendBuffer->Close(_bw.WriteSize());
		return _sendBuffer;
	}

private:
	PKT_C_DESPAWN_OBJECT* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};
#pragma pack()

#pragma pack(1)
class PKT_C_KDA_CS_WRITE {
public:
	PKT_C_KDA_CS_WRITE(KDACSInfo _kdacsInfo) {
		_sendBuffer = GSendBufferManager->Open(4096);
		// 초기화
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());

		_pkt = _bw.Reserve<PKT_C_KDA_CS>();
		_pkt->packetSize = 0; // To Fill
		_pkt->packetId = C_KDA_CS;
		_pkt->kdacsInfo = _kdacsInfo;
	}

	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈 계산
		_pkt->packetSize = _bw.WriteSize();

		_sendBuffer->Close(_bw.WriteSize());
		return _sendBuffer;
	}

private:
	PKT_C_KDA_CS* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};
#pragma pack()

#pragma pack(1)
class PKT_C_SOUND_WRITE {
public:
	using SoundNameList = PacketList<SoundInfoPacket::soundNameItem>;
	using SoundNameItem = SoundInfoPacket::soundNameItem;

	PKT_C_SOUND_WRITE(SoundInfoPacket _soundInfo)
	{
		_sendBuffer = GSendBufferManager->Open(4096);
		// 초기화
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());

		_pkt = _bw.Reserve<PKT_C_SOUND>();
		_pkt->packetSize = 0; // To Fill
		_pkt->packetId = C_SOUND;
		_pkt->soundInfo = _soundInfo;
	}

	SoundNameList ReserveAnimNameList(uint16 _soundNameCount) {
		SoundNameItem* firstBuffsListItem = _bw.Reserve<SoundNameItem>(_soundNameCount);
		_pkt->soundInfo.soundNameOffset = (uint64)firstBuffsListItem - (uint64)_pkt;
		_pkt->soundInfo.soundNameCount = _soundNameCount;
		return SoundNameList(firstBuffsListItem, _soundNameCount);
	}

	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈 계산
		_pkt->packetSize = _bw.WriteSize();

		_sendBuffer->Close(_bw.WriteSize());
		return _sendBuffer;
	}

private:
	PKT_C_SOUND* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};
#pragma pack()


#pragma pack(1)
class PKT_C_TIME_WRITE {
public:
	PKT_C_TIME_WRITE(float _seconds) {
		_sendBuffer = GSendBufferManager->Open(4096);
		// 초기화
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());

		_pkt = _bw.Reserve<PKT_C_TIME>();
		_pkt->packetSize = 0; // To Fill
		_pkt->packetId = C_TIME;
		_pkt->second = _seconds;
	}

	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈 계산
		_pkt->packetSize = _bw.WriteSize();

		_sendBuffer->Close(_bw.WriteSize());
		return _sendBuffer;
	}

private:
	PKT_C_TIME* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};
#pragma pack()


#pragma pack(1)
class PKT_C_OBJECT_MTRL_WRITE {
public:
	using TexNameList = PacketList<MtrlInfoPacket::texNameItem>;
	using TexNameItem = MtrlInfoPacket::texNameItem;

	PKT_C_OBJECT_MTRL_WRITE(MtrlInfoPacket  _mtrlInfo)
	{
		_sendBuffer = GSendBufferManager->Open(4096);
		// 초기화
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());

		_pkt = _bw.Reserve<PKT_C_OBJECT_MTRL>();
		_pkt->packetSize = 0; // To Fill
		_pkt->packetId = C_OBJECT_MTRL;
		_pkt->mtrlInfo = _mtrlInfo;
	}

	TexNameList ReserveTexNameList(uint16 _texNameCount) {
		TexNameItem* firstBuffsListItem = _bw.Reserve<TexNameItem>(_texNameCount);
		_pkt->mtrlInfo.texNameOffset = (uint64)firstBuffsListItem - (uint64)_pkt;
		_pkt->mtrlInfo.texNameCount = _texNameCount;
		return TexNameList(firstBuffsListItem, _texNameCount);
	}

	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈 계산
		_pkt->packetSize = _bw.WriteSize();

		_sendBuffer->Close(_bw.WriteSize());
		return _sendBuffer;
	}

private:
	PKT_C_OBJECT_MTRL* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
};
#pragma pack()