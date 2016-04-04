#ifndef GOGTRON_ENGINE_SYSTEM_WINDOW_H
#define GOGTRON_ENGINE_SYSTEM_WINDOW_H

#include <cstdint>
#include <string>
#include <memory>

namespace gogtron
{
	namespace system
	{

		template<typename TWindowHandle, typename TRenderContext>
		class Window
		{
		public:

			Window(const std::string& _title, std::uint32_t _width, std::uint32_t _height)
				: title(_title)
				, width(_width)
				, height(_height)
			{
			}

			virtual bool Init() = 0;
			virtual bool Release() = 0;

			std::uint32_t GetWidth() const
			{
				return width;
			}

			std::uint32_t GetHeight() const
			{
				return height;
			}

			const TWindowHandle GetHandle() const
			{
				return handle;
			}

			TWindowHandle GetHandle()
			{
				return handle;
			}

			const TRenderContext GetRenderContext() const
			{
				return renderContext;
			}

			TRenderContext GetRenderContext()
			{
				return renderContext;
			}

		protected:

			TWindowHandle handle;
			TRenderContext renderContext;
			const std::string title;
			std::uint32_t width;
			std::uint32_t height;
		};

		template<typename TWindowHandle, typename TRenderContext>
		using WindowPtr = std::shared_ptr<Window<TWindowHandle, TRenderContext>>;

	}
}

#endif