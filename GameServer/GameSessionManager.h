#pragma once

class GameSession;

using GameSessionRef = shared_ptr<GameSession>;

class GameSessionManager
{
public:
	void Add(GameSessionRef session);
	void Remove(GameSessionRef session);
	void Broadcast(SendBufferRef sendBuffer);

public:
	Set<GameSessionRef> GetGameSession() { return _sessions; }

private:
	USE_LOCK;
	Set<GameSessionRef> _sessions;
};

extern GameSessionManager GSessionManager;
