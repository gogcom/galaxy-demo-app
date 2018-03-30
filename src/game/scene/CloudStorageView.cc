#include "CloudStorageView.h"
#include <game/IGame.h>
#include <engine/system/Button.h>

#include <galaxy/GalaxyExceptionHelper.h>

#include <algorithm>

namespace gogtron::scene
{

	namespace
	{

		constexpr uint32_t MAX_FILES_ON_SCREEN = 5;

		constexpr auto SAMPLE_FILE_CONTENT = "This file is automatically synchronized by the Galaxy Client";

		constexpr auto SAMPLE_FILE_MAX_NAME_LENGTH = 32;
		constexpr auto FILENAME_CHAR_WIDTH = 10;
		constexpr auto MAX_FILENAME_WIDTH = FILENAME_CHAR_WIDTH * SAMPLE_FILE_MAX_NAME_LENGTH;

	}

	CloudStorageView::CloudStorageView(const gogtron::IGamePtr& _game)
		: GameState(_game)
	{
	}

	bool CloudStorageView::Init()
	{
		guiElements.emplace_back(std::make_shared<system::Button>(
			"SAVE NEW FILE", (1280 + MAX_FILENAME_WIDTH) / 2 + 25, 25, 300, 50,
			[&]()
		{
			SaveNewFile();
			newFileName.clear();
			ReadSavedFiles();
		}));

		guiElements.emplace_back(std::make_shared<system::Button>(
			"CANCEL", 1280 / 2 - 150, 565, 300, 100,
			[&]()
		{
			game->SetGameState(GameState::State::START_MENU);
		}));

		ReadSavedFiles();

		return true;
	}

	bool CloudStorageView::Release()
	{
		return true;
	}

	void CloudStorageView::OnMouseDown(std::uint32_t x, std::uint32_t y)
	{
		for (const auto& element : guiElements)
			element->OnMouseDown(x, y);

		for (const auto& btn : fileRemoveButtons)
			btn->OnMouseDown(x, y);
	}

	void CloudStorageView::OnMouseMotion(std::uint32_t x, std::uint32_t y)
	{
		for (const auto& element : guiElements)
			element->OnMouseMotion(x, y);

		for (const auto& btn : fileRemoveButtons)
			btn->OnMouseMotion(x, y);
	}

	void CloudStorageView::OnKeyDown(SDL_Keysym key)
	{
		// TODO Use SDL_StartTextInput
		if (key.sym == SDLK_BACKSPACE && !newFileName.empty())
		{
			newFileName.pop_back();
		}
		if (key.sym >= std::numeric_limits<char>::min()
			&& key.sym <= std::numeric_limits<char>::max()
			&& (isalnum(key.sym) || key.sym == '.')
			&& newFileName.size() < SAMPLE_FILE_MAX_NAME_LENGTH)
		{
			newFileName.push_back(key.sym);
		}
	}

	void CloudStorageView::OnLobbyEvent(const networking::LobbyEvent&  /*lobbyEvent*/)
	{
	}

	bool CloudStorageView::Update()
	{
		auto buttnosNum = std::min(savedFileNames.size(), static_cast<size_t>(MAX_FILES_ON_SCREEN));
		for (size_t idx = 0; idx < buttnosNum; ++idx)
		{
			fileRemoveButtons.emplace(std::begin(fileRemoveButtons) + idx, std::make_shared<system::Button>(
				"X",
				(1280 + MAX_FILENAME_WIDTH) / 2 + 25, 175 + 75 * idx, 50, 50,
				[&, idx = idx]()
			{
				galaxy::api::Storage()->FileDelete(savedFileNames[idx].c_str());
				savedFileNames.erase(savedFileNames.begin() + idx);
			}));
		}
		fileRemoveButtons.resize(buttnosNum);
		return true;
	}

	bool CloudStorageView::Display(const renderer::OGLRendererPtr& renderEngine)
	{
		constexpr auto newFileHintWidth = 200;
		renderEngine->DisplayText(
			"New file: ",
			renderer::Sprite((1280 - MAX_FILENAME_WIDTH) / 2 - newFileHintWidth, 25, newFileHintWidth, 50),
			"FreeSans_NewFileHint",
			SDL_Color{255, 0, 0, 255});

		if (newFileName.empty())
		{
			constexpr auto newFilePlaceholderWidth = 300;
			renderEngine->DisplayText(
				"<type new filename>",
				renderer::Sprite((1280 - newFilePlaceholderWidth) / 2, 25, newFilePlaceholderWidth, 50),
				"FreeSans_NoFilenameHint",
				SDL_Color{0, 255, 255, 255});
		}
		else
		{
			renderEngine->DisplayText(
				newFileName,
				renderer::Sprite((1280 - MAX_FILENAME_WIDTH) / 2, 25, FILENAME_CHAR_WIDTH * newFileName.size(), 50),
				"FreeSans_" + newFileName,
				SDL_Color{255, 0, 0, 255});
		}

		constexpr auto storedFilesNumHintWidth = 400;
		renderEngine->DisplayText(
			"Number of files in remote storage: " + std::to_string(savedFileNames.size()),
			renderer::Sprite((1280 - storedFilesNumHintWidth) / 2, 100, storedFilesNumHintWidth, 50),
			"FreeSans_FilesNumberInStorage" + std::to_string(savedFileNames.size()),
			SDL_Color{255, 0, 0, 255});

		for (size_t idx{0}; idx < std::min(savedFileNames.size(), static_cast<size_t>(MAX_FILES_ON_SCREEN)); ++idx)
		{
			renderEngine->DisplayText(
				savedFileNames[idx],
				renderer::Sprite((1280 - MAX_FILENAME_WIDTH) / 2, 175 + 75 * idx, MAX_FILENAME_WIDTH, 50),
				"FreeSans_FileNameCaption" + savedFileNames[idx],
				SDL_Color{255, 0, 0, 255});
		}

		for (const auto& btn : fileRemoveButtons)
			btn->Display(renderEngine);

		for (const auto& element : guiElements)
			element->Display(renderEngine);

		renderEngine->DisplayText(
			"Game files are automatically synchronized by the Galaxy Client",
			renderer::Sprite((1280 - 500) / 2, 675, 500, 50),
			"FreeSans_SyncFilesHint",
			SDL_Color{255, 255, 225, 255});

		return true;
	}

	void CloudStorageView::SaveNewFile() const
	{
		if (newFileName.empty())
			return;

		galaxy::api::Storage()->FileWrite(newFileName.c_str(), SAMPLE_FILE_CONTENT, std::strlen(SAMPLE_FILE_CONTENT));
		galaxy::api::ThrowIfGalaxyError();
	}

	uint32_t CloudStorageView::ReadSavedFiles()
	{
		auto saveFilesCount = galaxy::api::Storage()->GetFileCount();
		for (uint32_t idx{0}; idx < saveFilesCount; ++idx)
		{
			savedFileNames.emplace(std::begin(savedFileNames) + idx, galaxy::api::Storage()->GetFileNameByIndex(idx));
		}
		savedFileNames.resize(saveFilesCount);

		return savedFileNames.size();
	}

}