module;

#include <string>
#include <format>
#include <chrono>
#include <memory>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

export module Game;

import AppInfo;
import Windowing.Exceptions;
import Windowing.WindowBuilder;

export class Game
{
	protected:
		const std::string title = AppInfo::name;

		bool running = false;
		bool initialized = false;

		uint_fast8_t fps = 60;
		Uint32 frameTime = 1000 / this->fps;

		Uint64 frameCount = 0;
		Uint64 lastFrame = 0;
		float currentFps = 0.0f;

		std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window
		{
			nullptr,
			[] (SDL_Window* window) {
				if (window) {
					SDL_DestroyWindow(window);
				}
			},
		};

		std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer
		{
			nullptr,
			[] (SDL_Renderer* renderer) {
				if (renderer) {
					SDL_DestroyRenderer(renderer);
				}
			},
		};

		std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> fpsFont{
			nullptr,
			[](TTF_Font* font) { if(font) TTF_CloseFont(font); }
		};

		std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> fpsTexture{
			nullptr,
			[](SDL_Texture* tex) { if(tex) SDL_DestroyTexture(tex); }
		};

	public:
		Game(Game&&) = default;
		Game& operator=(Game&&) = delete;
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;

		Game() = default;
		~Game() = default;

		[[nodiscard]]
		bool isRunning() const
		{
			return this->running;
		}

		[[nodiscard]]
		bool isInitialized() const
		{
			return this->initialized;
		}

		void setFps(const uint_fast8_t fps)
		{
			this->fps = fps;
			this->frameTime = 1000 / this->fps;
		}

		void run()
		{
			if (!SDL_Init(SDL_INIT_VIDEO)) {
				throw SdlInitializationException();
			}

			this->initialized = true;

			this->createWindow();
			this->createRenderer();

			this->running = true;

			Uint64 lastFrameTime = SDL_GetTicks();
			Uint64 accumulator = 0;

			while (this->running) {
				const Uint64 currentTime = SDL_GetTicks();
				const Uint64 deltaTime = currentTime - lastFrameTime;
				lastFrameTime = currentTime;

				this->frameCount++;

				if (currentTime - this->lastFrame >= 1000) {
					this->currentFps = static_cast<float>(this->frameCount) / ((static_cast<float>(currentTime) - static_cast<float>(this->lastFrame)) / 1000.0f);
					this->frameCount = 0;
					this->lastFrame = currentTime;
					// this->updateFpsCounter();
				}

				this->handleInput();

				accumulator += deltaTime;

				while (accumulator >= this->frameTime) {
					this->update();
					accumulator -= this->frameTime;
				}

				this->render();

				if (const Uint64 frameDuration = SDL_GetTicks() - currentTime; frameDuration >= this->frameTime) {
					SDL_Delay(this->frameTime - frameDuration);
				}
			}

			this->renderer.reset();
			this->window.reset();

			SDL_Quit();

			this->initialized = false;
		}

	protected:
		void createWindow()
		{
			SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

			WindowBuilder builder;

			builder.setTitle(this->title);
			builder.setResizable(false);
			builder.setHighDpi(true);
			builder.setWindowSize({1280, 720});
			builder.setDisplay(0);

			this->window = builder.build();
		}

		void createRenderer()
		{
			this->renderer.reset(SDL_CreateRenderer(
				this->window.get(),
				nullptr
			));

			if (!this->renderer) {
				throw SdlRendererException("Renderer creation failed.");
			}
		}

		void handleInput()
		{
			SDL_Event event;

			while (SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_EVENT_QUIT:
						this->running = false;
						break;

					case SDL_EVENT_KEY_DOWN:
						if (event.key.key == SDLK_ESCAPE) {
							this->running = false;
						}
					break;
				}
			}
		}

		void update()
		{
			//
		}

		void render()
		{
			SDL_SetRenderDrawColor(this->renderer.get(), 0, 0, 0, 255);
			SDL_RenderClear(this->renderer.get());

			//

			SDL_RenderPresent(this->renderer.get());
		}
};
