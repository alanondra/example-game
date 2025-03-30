module;

#include <utility>
#include <optional>
#include <string>
#include <algorithm>
#include <iostream>
#include <memory>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_video.h>

export module Windowing.WindowBuilder;

import Windowing.WindowState;
import Windowing.WindowSize;
import Windowing.Exceptions.SdlWindowException;

using std::optional;
using std::string;
using std::string_view;

export class WindowBuilder
{
	protected:
		string title = "SDL";

		bool fullscreen = false;
		bool borderless = false;
		bool resizable = true;
		bool focusable = true;
		bool alwaysOnTop = false;
		bool highDpi = true;
		bool mouseGrabbed = false;

		uint display = 0;
		WindowState state = Restored;
		WindowSize windowSize = {1280, 720};

	public:
		void setTitle(string title)
		{
			this->title = std::move(title);
		}

		const string& getTitle() const
		{
			return this->title;
		}

		void setFullscreen(bool fullscreen)
		{
			this->fullscreen = fullscreen;
		}

		bool getFullscreen() const
		{
			return this->fullscreen;
		}

		void setBorderless(bool borderless)
		{
			this->borderless = borderless;
		}

		bool getBorderless() const
		{
			return this->borderless;
		}

		void setResizable(bool resizable)
		{
			this->resizable = resizable;
		}

		bool getResizable() const
		{
			return this->resizable;
		}

		void setFocusable(bool focusable)
		{
			this->focusable = focusable;
		}

		bool getFocusable() const
		{
			return this->focusable;
		}

		void setAlwaysOnTop(bool alwaysOnTop)
		{
			this->alwaysOnTop = alwaysOnTop;
		}

		bool getAlwaysOnTop() const
		{
			return this->alwaysOnTop;
		}

		void setHighDpi(bool highDpi)
		{
			this->highDpi = highDpi;
		}

		bool getHighDpi() const
		{
			return this->highDpi;
		}

		void setMouseGrabbed(bool mouseGrabbed)
		{
			this->mouseGrabbed = mouseGrabbed;
		}

		bool getMouseGrabbed() const
		{
			return this->mouseGrabbed;
		}

		void setDisplay(uint display)
		{
			this->display = display;
		}

		uint getDisplay() const
		{
			return this->display;
		}

		void setWindowState(WindowState state)
		{
			this->state = state;
		}

		const WindowState& getWindowState() const
		{
			return this->state;
		}

		void setWindowSize(WindowSize size)
		{
			this->windowSize = std::move(size);
		}

		const WindowSize& getWindowSize() const
		{
			return this->windowSize;
		}

		std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> build()
		{
			auto properties = this->getProperties();

			SDL_Window* window = SDL_CreateWindowWithProperties(properties);
			SDL_DestroyProperties(properties);

			if (!window) {
				throw SdlWindowException(std::format("Failed to create window {}", this->title));
			}

			return {window, &SDL_DestroyWindow};
		}

		SDL_PropertiesID getProperties()
		{
			SDL_PropertiesID properties = SDL_CreateProperties();

			SDL_SetStringProperty(properties, SDL_PROP_WINDOW_CREATE_TITLE_STRING, this->title.c_str());
			SDL_SetBooleanProperty(properties, SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, this->fullscreen);
			SDL_SetBooleanProperty(properties, SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, this->borderless);
			SDL_SetBooleanProperty(properties, SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, this->resizable);
			SDL_SetBooleanProperty(properties, SDL_PROP_WINDOW_CREATE_FOCUSABLE_BOOLEAN, this->focusable);
			SDL_SetBooleanProperty(properties, SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN, this->alwaysOnTop);
			SDL_SetBooleanProperty(properties, SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN, this->highDpi);
			SDL_SetBooleanProperty(properties, SDL_PROP_WINDOW_CREATE_MOUSE_GRABBED_BOOLEAN, this->mouseGrabbed);

			int displayCount = 0;
			SDL_DisplayID* displayIds = SDL_GetDisplays(&displayCount);

			if (displayCount == 0) {
				SDL_free(displayIds);
				throw SdlWindowException("No displays found");
			}

			const uint clampedDisplay = std::clamp(this->display, 0u, static_cast<uint>(displayCount - 1));
			const SDL_DisplayID displayId = displayIds[clampedDisplay];

			SDL_free(displayIds);

			SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_X_NUMBER, SDL_WINDOWPOS_CENTERED_DISPLAY(displayId));
			SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_Y_NUMBER, SDL_WINDOWPOS_CENTERED_DISPLAY(displayId));

			if (this->state == Maximized) {
				SDL_SetBooleanProperty(properties, SDL_PROP_WINDOW_CREATE_MAXIMIZED_BOOLEAN, true);
			} else if (this->state == Minimized) {
				SDL_SetBooleanProperty(properties, SDL_PROP_WINDOW_CREATE_MINIMIZED_BOOLEAN, true);
			}

			if (this->fullscreen) {
				const SDL_DisplayMode* displayMode = SDL_GetCurrentDisplayMode(displayId);

				if (!displayMode) {
					throw SdlWindowException("Failed to get display mode");
				}

				SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, displayMode->w);
				SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, displayMode->h);
			} else {
				SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, this->windowSize.getWidth());
				SDL_SetNumberProperty(properties, SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, this->windowSize.getHeight());
			}

			return properties;
		}
};
