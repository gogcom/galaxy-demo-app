#ifndef GOGTRON_GAME_MANAGER_H
#define GOGTRON_GAME_MANAGER_H

#include "Player.h"
#include <memory>
#include <vector>

namespace gogtron
{

    class GameManager
    {
    public:

        enum class ServerState
        {
            INIT,
            GAME,
            RESULTS
        };

        enum class ClientState
        {
            INIT,
            START_GAME,
            GAME,
            RESULT
        };

        bool Init();
        bool Release();

        const PlayerPtr& GetLocalPlayer() const;
        const std::vector<PlayerPtr>& GetPlayers() const;
        std::vector<PlayerPtr>& GetPlayers();
        void AddPlayer(const PlayerPtr& player);

        void SetGameTime(uint64_t gameTime);
		uint64_t GetGameTime() const;

        void SetServerState(const ServerState& state);
        const ServerState& GetServerState() const;

        void SetClientState(const ClientState& state);
        const ClientState& GetClientState() const;

    private:

        std::vector<PlayerPtr> players;
		uint64_t gameTime;
        ServerState serverState;
        ClientState clientState;
    };

    using GameManagerPtr = std::shared_ptr<GameManager>;

}

#endif