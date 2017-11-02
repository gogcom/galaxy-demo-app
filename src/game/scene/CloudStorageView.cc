#include "CloudStorageView.h"
#include <game/IGame.h>
#include <engine/system/Button.h>

namespace gogtron
{
	namespace scene
	{

		namespace
		{

			const char* SAMPLE_FILE_NAME = "sample.txt";
			const uint32_t SAMPLE_FILE_MAX_SIZE = 32;

		}

		CloudStorageView::CloudStorageView(const gogtron::IGamePtr& _game)
			: GameState(_game)
		{
		}

		bool CloudStorageView::Init()
		{
			guiElements.emplace_back(std::make_shared<system::Button>(
				"SAVE", 1280 / 2 - 150, 450, 300, 100,
				[&]()
			{
				galaxy::api::Storage()->FileWrite(SAMPLE_FILE_NAME, newFileContent.c_str(), newFileContent.size());
				game->SetGameState(GameState::State::START_MENU);
				Reset();
			}));

			guiElements.emplace_back(std::make_shared<system::Button>(
				"CANCEL", 1280 / 2 - 150, 575, 300, 100,
				[&]()
			{
				game->SetGameState(GameState::State::START_MENU);
				Reset();
			}));

			Reset();

			return true;
		}

		bool CloudStorageView::Release()
		{
			return true;
		}

		void CloudStorageView::OnMouseDown(std::uint32_t x, std::uint32_t y)
		{
			for (const auto& element : guiElements)
			{
				element->OnMouseDown(x, y);
			}
		}

		void CloudStorageView::OnMouseMotion(std::uint32_t x, std::uint32_t y)
		{
			for (const auto& element : guiElements)
			{
				element->OnMouseMotion(x, y);
			}
		}

		void CloudStorageView::OnKeyDown(SDL_Keysym key)
		{
			// TODO Use SDL_StartTextInput
			if (key.sym == SDLK_BACKSPACE && !newFileContent.empty())
			{
				newFileContent.pop_back();
			}
			if (
				key.sym >= std::numeric_limits<char>::min() &&
				key.sym <= std::numeric_limits<char>::max() &&
				isalnum(key.sym) &&
				newFileContent.size() < SAMPLE_FILE_MAX_SIZE
				)
			{
				newFileContent.push_back(key.sym);
			}
		}

		void CloudStorageView::OnLobbyEvent(const networking::LobbyEvent& lobbyEvent)
		{
		}

		bool CloudStorageView::Update()
		{
			switch (game->GetStorageSynchronizationStatus())
			{
				case IGame::FileSharingStatus::DOWNLOADED:
				{
					if (!galaxy::api::Storage()->FileExists(SAMPLE_FILE_NAME))
						currentFileContent.clear();
					else
					{
						currentFileContent.resize(SAMPLE_FILE_MAX_SIZE);
						uint32_t bytesRead = galaxy::api::Storage()->FileRead(SAMPLE_FILE_NAME, &currentFileContent[0], SAMPLE_FILE_MAX_SIZE);
						currentFileContent.resize(bytesRead);
					}
					break;
				}
				case IGame::FileSharingStatus::SHARED:
					break;
				case IGame::FileSharingStatus::FAILED:
					errorMessage = "Failed to download shared files";
					break;
			}

			return true;
		}

		bool CloudStorageView::Display(const renderer::OGLRendererPtr& renderEngine)
		{
			for (const auto& element : guiElements)
			{
				element->Display(renderEngine);
			}

			if (!errorMessage.empty())
			{
				renderEngine->DisplayText(errorMessage.c_str(), renderer::Sprite(50, 100, 300, 100), "CloudStorageViewErrorMessage", SDL_Color{255, 0, 0, 255});
			}
			else
			{
				// TODO Do not generate new texture for each message
				renderEngine->DisplayText(
					"Number of files in remote storage: " + std::to_string(galaxy::api::Storage()->GetFileCount()),
					renderer::Sprite(1280 / 2 - 200, 25, 400, 50),
					"FreeSans_FilesNumberInStorage",
					SDL_Color{255, 0, 0, 255});

				renderEngine->DisplayText(
					"Current message:",
					renderer::Sprite(1280 / 2 - 200, 100, 400, 50),
					"FreeSans_CurrentMessageCaption",
					SDL_Color{255, 0, 0, 255});

				renderEngine->DisplayText(
					currentFileContent,
					renderer::Sprite(1280 / 2 - 200, 175, 400, 50),
					"FreeSans_CurrentMessage" + currentFileContent,
					SDL_Color{255, 0, 0, 255});

				renderEngine->DisplayText(
					"Type new message below:",
					renderer::Sprite(1280 / 2 - 200, 250, 400, 50),
					"FreeSans_NewMessageCaption",
					SDL_Color{255, 0, 0, 255});

				renderEngine->DisplayText(
					newFileContent,
					renderer::Sprite(1280 / 2 - 200, 325, 400, 100),
					"FreeSans_MessageCaption" + newFileContent,
					SDL_Color{255, 0, 0, 255});
			}
			return true;
		}

		void gogtron::scene::CloudStorageView::Reset()
		{
			currentFileContent.clear();
			newFileContent.clear();
		}

	}
}