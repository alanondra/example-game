module;

#include <optional>
#include <string>
#include <string_view>

export module ExampleGame.Common.Support.Env;

/**
 * Provides utility functions for getting information from the Environment.
 */
export class Env
{
	public:
		/**
		 * Get the value of an environment variable.
		 *
		 * @param name Name of the environment variable.
		 *
		 * @return Value of the environment variable, if it exists.
		 *
		 * @warning This method is not thread-safe due to using global shared state.
		 */
		[[nodiscard]]
		static std::optional<std::string> get(const std::string_view name)
		{
			if (const auto value = std::getenv(name.data())) {
				return std::string(value);
			}
			return std::nullopt;
		}
};
