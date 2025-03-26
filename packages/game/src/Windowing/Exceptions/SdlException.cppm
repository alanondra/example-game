module;

#include <optional>
#include <string>
#include <stdexcept>
#include <SDL3/SDL.h>

export module Windowing.Exceptions.SdlException;

export class SdlException:
	public std::runtime_error
{
	protected:
		std::optional<std::string> sdlError;

		void fetchSdlError()
		{
			this->sdlError = std::nullopt;

			if (const char* error = SDL_GetError(); error && *error != '\0') {
				sdlError = std::string(error);
			}

			SDL_ClearError();
		}

	public:
		explicit SdlException(const char* what):
			std::runtime_error(what)
		{
			this->fetchSdlError();
		}

		explicit SdlException(const std::string& what):
			std::runtime_error(what)
		{
			this->fetchSdlError();
		}

		[[nodiscard]]
		const std::optional<std::string>& getSdlError() const noexcept
		{
			return this->sdlError;
		}
};
