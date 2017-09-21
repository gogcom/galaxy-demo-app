#ifndef GOGTRON_GOGTRON_H
#define GOGTRON_GOGTRON_H

#include "IGame.h"
#include <engine/renderer/OGLRenderer.h>
#include <galaxy/GalaxyApi.h>
#include <vector>

namespace gogtron
{

	class GogTron
		: public IGame
		, public std::enable_shared_from_this<GogTron>
	{
	public:

		GogTron(const renderer::OGLRendererPtr& renderEngine);

		virtual bool Init(int argc, char** argv) override;
		virtual bool Release() override;
		virtual bool Update() override;

		virtual void OnMouseDown(std::uint32_t x, std::uint32_t y) override;
		virtual void OnMouseMotion(std::uint32_t x, std::uint32_t y) override;
		virtual void OnKeyDown(SDL_Keysym key) override;
		virtual void OnLobbyEvent(const networking::LobbyEvent& lobbyEvent) override;

		virtual bool SetGameState(const scene::GameState::State& state) override;

	private:

		class AuthListener : public galaxy::api::GlobalAuthListener
		{
		public:

			AuthListener(const std::shared_ptr<GogTron>& game);

			virtual void OnAuthSuccess() override;
			virtual void OnAuthFailure(FailureReason reason) override;
			virtual void OnAuthLost() override;

		private:

			std::shared_ptr<GogTron> game;
		};

		class GameJoinRequestedListener : public galaxy::api::GlobalGameJoinRequestedListener
		{
		public:

			GameJoinRequestedListener(const std::shared_ptr<GogTron>& game);

			virtual void OnGameJoinRequested(galaxy::api::GalaxyID userID, const char* connectionString) override;

		private:

			std::shared_ptr<GogTron> game;
		};

		class UserStatsAndAchievementsRetrieveListener : public galaxy::api::GlobalUserStatsAndAchievementsRetrieveListener
		{
		public:

			UserStatsAndAchievementsRetrieveListener(const std::shared_ptr<GogTron>& game);

			virtual void OnUserStatsAndAchievementsRetrieveSuccess(galaxy::api::GalaxyID userID) override;

			virtual void OnUserStatsAndAchievementsRetrieveFailure(galaxy::api::GalaxyID userID, FailureReason failureReason) override;

		private:

			std::shared_ptr<GogTron> game;
		};

		class AchievementChangeListener : public galaxy::api::GlobalAchievementChangeListener
		{
		public:

			AchievementChangeListener(const std::shared_ptr<GogTron>& game);

			virtual void OnAchievementUnlocked(const char* name) override;

		private:

			std::shared_ptr<GogTron> game;
		};

		class StatsAndAchievementsStoreListener : public galaxy::api::GlobalStatsAndAchievementsStoreListener
		{
		public:

			StatsAndAchievementsStoreListener(const std::shared_ptr<GogTron>& game);

			virtual void OnUserStatsAndAchievementsStoreSuccess() override;

			virtual void OnUserStatsAndAchievementsStoreFailure(FailureReason failureReason) override;

		private:

			std::shared_ptr<GogTron> game;
		};

		class LeaderboardsRetrieveListener : public galaxy::api::GlobalLeaderboardsRetrieveListener
		{
		public:

			LeaderboardsRetrieveListener(const std::shared_ptr<GogTron>& game);

			virtual void OnLeaderboardsRetrieveSuccess() override;

			virtual void OnLeaderboardsRetrieveFailure(FailureReason failureReason) override;

		private:

			std::shared_ptr<GogTron> game;
		};

		class LeaderboardEntriesRetrieveListener : public galaxy::api::GlobalLeaderboardEntriesRetrieveListener
		{
		public:

			LeaderboardEntriesRetrieveListener(const std::shared_ptr<GogTron>& game);

			virtual void OnLeaderboardEntriesRetrieveSuccess(const char* name, uint32_t entryCount) override;

			virtual void OnLeaderboardEntriesRetrieveFailure(const char* name, FailureReason failureReason) override;

		private:

			std::shared_ptr<GogTron> game;
		};

		class LeaderboardScoreUpdateListener : public galaxy::api::GlobalLeaderboardScoreUpdateListener
		{
		public:

			LeaderboardScoreUpdateListener(const std::shared_ptr<GogTron>& game);

			virtual void OnLeaderboardScoreUpdateSuccess(const char* name, int32_t score, uint32_t oldRank, uint32_t newRank) override;

			virtual void OnLeaderboardScoreUpdateFailure(const char* name, int32_t score, FailureReason failureReason) override;

		private:

			std::shared_ptr<GogTron> game;
		};

		class FileShareListener: public galaxy::api::GlobalFileShareListener {
		public:

			FileShareListener(const std::shared_ptr<GogTron>& game);

			virtual void OnFileShareSuccess(const char* fileName, galaxy::api::SharedFileID sharedFileID) override;

			virtual void OnFileShareFailure(const char* fileName, FailureReason failureReason) override;

		private:

			std::shared_ptr<GogTron> game;
		};

		class SharedFileDownloadListener : public galaxy::api::GlobalSharedFileDownloadListener
		{
		public:

			SharedFileDownloadListener(const std::shared_ptr<GogTron>& game);

			virtual void OnSharedFileDownloadSuccess(galaxy::api::SharedFileID sharedFileID, const char* fileName) override;

			virtual void OnSharedFileDownloadFailure(galaxy::api::SharedFileID sharedFileID, FailureReason failureReason) override;

		private:

			std::shared_ptr<GogTron> game;
		};

		bool InitFontTextures();

		bool InitGalaxy();
		bool ReleaseGalaxy();

		bool AnimateNextFrame(std::uint32_t fps, float& globalDeltaTime);
		void CalculateFrameRate(float& globalDeltaTime);

		renderer::OGLRendererPtr renderEngine;
		std::vector<std::unique_ptr<galaxy::api::IGalaxyListener>> listeners;
	};

}

#endif