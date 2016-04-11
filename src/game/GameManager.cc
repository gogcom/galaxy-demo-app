#include "GameManager.h"
#include <cassert>
#include <algorithm>

using namespace gogtron;

bool GameManager::Init()
{
    SetClientState(GameManager::ClientState::INIT);
    SetServerState(GameManager::ServerState::INIT);
    players.clear();
    return true;
}

bool GameManager::Release()
{
    return true;
}

const PlayerPtr& GameManager::GetLocalPlayer() const
{
    const auto& localPlayer = std::find_if(std::begin(players), std::end(players),
        [](const PlayerPtr& player) { return player->GetGalaxyID() == galaxy::api::User()->GetGalaxyID(); });
    assert(localPlayer != std::end(players));
    return *localPlayer;
}

const std::vector<PlayerPtr>& GameManager::GetPlayers() const
{
    return players;
}

std::vector<PlayerPtr>& GameManager::GetPlayers()
{
    return players;
}

void GameManager::AddPlayer(const PlayerPtr& player)
{
    players.push_back(player);
}

void GameManager::SetGameTime(int _gameTime)
{
    gameTime = _gameTime;
}

int GameManager::GetGameTime() const
{
    return gameTime;
}

void GameManager::SetServerState(const GameManager::ServerState& state)
{
    serverState = state;
}

const GameManager::ServerState& GameManager::GetServerState() const
{
    return serverState;
}

void GameManager::SetClientState(const GameManager::ClientState& state)
{
    clientState = state;
}

const GameManager::ClientState& GameManager::GetClientState() const
{
    return clientState;
}