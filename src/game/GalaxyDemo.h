#ifndef GALAXY_DEMO_GALAXY_DEMO_H
#define GALAXY_DEMO_GALAXY_DEMO_H

#include "IGame.h"
#include <engine/renderer/OGLRenderer.h>
#include <galaxy/GalaxyApi.h>
#include <vector>

namespace galaxy
{
	namespace api
	{
		extern bool IsFullyInitialized;
	}
}

namespace galaxy::demo
{

	class GalaxyDemo
		: public IGame
		, public std::enable_shared_from_this<GalaxyDemo>
	{
	public:

		GalaxyDemo(const renderer::OGLRendererPtr& renderEngine);

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

			AuthListener(const std::shared_ptr<GalaxyDemo>& game);

			virtual void OnAuthSuccess() override;
			virtual void OnAuthFailure(FailureReason reason) override;
			virtual void OnAuthLost() override;

		private:

			std::shared_ptr<GalaxyDemo> game;
		};

		class GameJoinRequestedListener : public galaxy::api::GlobalGameJoinRequestedListener
		{
		public:

			GameJoinRequestedListener(const std::shared_ptr<GalaxyDemo>& game);

			virtual void OnGameJoinRequested(galaxy::api::GalaxyID userID, const char* connectionString) override;

		private:

			std::shared_ptr<GalaxyDemo> game;
		};

		bool InitFontTextures();

		void InitGalaxy();

		void InitListeners();

		bool ReleaseGalaxy();

		bool AnimateNextFrame(std::uint32_t fps, float& globalDeltaTime);
		void CalculateFrameRate(float& globalDeltaTime);

		renderer::OGLRendererPtr renderEngine;
		std::vector<std::unique_ptr<galaxy::api::IGalaxyListener>> listeners;
	};

}

#endif