module;

#include <cctype>
#include <string>
#include <string_view>
#include <vector>
#include <locale>

export module ExampleGame.Common.Support.Str;

/**
 * Collection of stateless string utility helpers.
 */
export class Str
{
	public:
#ifdef OS_WINDOWS
		/**
		 * Platform-specific end-of-line sequence.
		 */
		static constexpr std::string_view EOL = "\r\n";
#else
		static constexpr std::string_view EOL = "\n";
#endif

		/**
		 * Concatenate two string views into a new `std::string`.
		 * No additional separator is inserted between the arguments.
		 *
		 * @param a The first part of the result.
		 * @param b The second part of the result.
		 *
		 * @return A new string equal to `a` followed by `b`.
		 */
		[[nodiscard]]
		static std::string concat(const std::string_view& a, const std::string_view& b)
		{
			std::string result;
			result.reserve(a.size() + b.size());

			result += a;
			result += b;

			return result;
		}

		/**
		 * Join a sequence of strings with a separator.
		 *
		 * Example:
		 *  - `join({"a", "b", "c"}, ", ")` -> `"a, b, c"`
		 *  - `join({}, ", ")`              -> `""`
		 *
		 * @param strings   The elements to join.
		 * @param separator The string inserted between elements.
		 *     Not added at the beginning or end of the result.
		 *
		 * @return A new string containing all elements separated by `separator`.
		 */
		[[nodiscard]]
		static std::string join(const std::vector<std::string>& strings, const std::string& separator)
		{
			std::string result;

			if (strings.empty()) {
				return result;
			}

			const std::size_t totalSizeEstimate = (strings.size() * separator.size()) +
				[&strings]() {
					std::size_t len = 0;
					for (const auto& s : strings) {
						len += s.size();
					}
					return len;
				}();

			result.reserve(totalSizeEstimate);

			for (std::size_t i = 0; i < strings.size(); ++i) {
				if (i > 0) {
					result += separator;
				}
				result += strings[i];
			}

			return result;
		}

		/**
		 * Repeat a string a fixed number of times.
		 *
		 * Example:
		 *  - `repeat("ab", 3)` -> `"ababab"`
		 *  - `repeat("x", 0)`  -> `""`
		 *
		 * @param str   The substring to repeat.
		 * @param count The number of repetitions. If `count <= 0`, the result
		 *     is an empty string.
		 *
		 * @return A new string consisting of `str` repeated `count` times.
		 */
		[[nodiscard]]
		static std::string repeat(const std::string_view& str, const int count)
		{
			if (count <= 0) {
				return {};
			}

			std::string result;
			result.reserve(str.size() * static_cast<std::size_t>(count));

			for (int i = 0; i < count; ++i) {
				result += str;
			}

			return result;
		}

		/**
		 * Convert text to `snake_case`.
		 *
		 * Behavior:
		 *  - Any run of whitespace or punctuation is treated as a separator.
		 *  - Separators are collapsed into a single underscore.
		 *  - Leading and trailing separators are removed.
		 *  - Alphabetic characters are converted to lowercase.
		 *
		 * Examples:
		 *  - `"Hello World"`     -> `"hello_world"`
		 *  - `"  Foo---Bar  "`   -> `"foo_bar"`
		 *  - `"Already_snake"`   -> `"already_snake"`
		 *
		 * @param input The input string.
		 * @return A new string in snake_case.
		 */
		[[nodiscard]]
		static std::string toSnakeCase(const std::string_view input)
		{
			std::string result;
			result.reserve(input.size());
			bool previous_separator = true;

			for (const char c : input) {
				if (std::ispunct(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c))) {
					if (!previous_separator) {
						result += '_';
						previous_separator = true;
					}
				} else {
					result += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
					previous_separator = false;
				}
			}

			if (!result.empty() && result.back() == '_') {
				result.pop_back();
			}

			return result;
		}

		/**
		 * Convert text to `kebab-case`.
		 *
		 * Behavior:
		 *  - Any run of whitespace or punctuation is treated as a separator.
		 *  - Separators are collapsed into a single hyphen.
		 *  - Leading and trailing separators are removed.
		 *  - Alphabetic characters are converted to lowercase.
		 *
		 * Examples:
		 *  - `"Hello World"`     -> `"hello-world"`
		 *  - `"  Foo---Bar  "`   -> `"foo-bar"`
		 *
		 * @param input The input string.
		 * @return A new string in kebab-case.
		 */
		[[nodiscard]]
		static std::string toKebabCase(const std::string_view input)
		{
			std::string result;
			result.reserve(input.size());
			bool previous_separator = true;

			for (const char c : input) {
				if (std::ispunct(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c))) {
					if (!previous_separator) {
						result += '-';
						previous_separator = true;
					}
				} else {
					result += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
					previous_separator = false;
				}
			}

			if (!result.empty() && result.back() == '-') {
				result.pop_back();
			}

			return result;
		}

		/**
		 * Convert text to `camelCase`.
		 *
		 * Behavior:
		 *  - The first non-separator character is lowercased.
		 *  - Any run of whitespace or punctuation starts a "new word".
		 *  - The first character of each subsequent "word" is uppercased.
		 *  - All other characters are lowercased.
		 *
		 * Examples:
		 *  - `"Hello world"`     -> `"helloWorld"`
		 *  - `"  foo---bar  "`   -> `"fooBar"`
		 *  - `"alreadyCamel"`    -> `"alreadycamel"` (no special casing retained)
		 *
		 * @param input The input string.
		 * @return A new string in camelCase.
		 */
		[[nodiscard]]
		static std::string toCamelCase(const std::string_view input)
		{
			std::string result;
			result.reserve(input.size());
			bool new_word = false;
			bool first_char = true;

			for (const char c : input) {
				if (std::ispunct(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c))) {
					new_word = true;
				} else {
					if (first_char) {
						result += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
						first_char = false;
					} else if (new_word) {
						result += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
						new_word = false;
					} else {
						result += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
					}
				}
			}

			return result;
		}

		/**
		 * Convert text to "Train-Case".
		 *
		 * In this implementation, "Train-Case" is similar to "Title-Case"
		 * but uses underscores between words, and each word starts with
		 * an uppercase letter.
		 *
		 * Behavior:
		 *  - Any run of whitespace or punctuation is treated as a separator.
		 *  - Separators are collapsed into single underscores.
		 *  - Leading underscores are removed.
		 *  - The first character of each word is uppercased, the rest
		 *    are lowercased.
		 *
		 * Examples:
		 *  - `"hello world"`     -> `"Hello_World"`
		 *  - `"  foo---BAR  "`   -> `"Foo_Bar"`
		 *
		 * @param input The input string.
		 * @return A new string in Train-Case.
		 */
		[[nodiscard]]
		static std::string toTrainCase(const std::string_view input)
		{
			std::string result;
			result.reserve(input.size());
			bool previous_separator = true;

			for (const char c : input) {
				if (std::ispunct(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c))) {
					if (!previous_separator) {
						result += '_';
						previous_separator = true;
					}
				} else {
					if (previous_separator) {
						result += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
					} else {
						result += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
					}
					previous_separator = false;
				}
			}

			if (!result.empty() && result.front() == '_') {
				result.erase(0, 1);
			}

			return result;
		}

		/**
		 * Convert text to "Title Case".
		 *
		 * Behavior:
		 *  - Any run of whitespace or punctuation is treated as a separator.
		 *  - Separators are collapsed into single spaces.
		 *  - Leading and trailing separators are removed.
		 *  - The first character of each word is uppercased, the rest
		 *    are lowercased.
		 *
		 * Examples:
		 *  - `"hello world"`     -> `"Hello World"`
		 *  - `"  foo---BAR  "`   -> `"Foo Bar"`
		 *
		 * @param input The input string.
		 * @return A new string in Title Case.
		 */
		[[nodiscard]]
		static std::string toTitleCase(const std::string_view input)
		{
			std::string result;
			result.reserve(input.size());
			bool in_separator = true;

			for (const char c : input) {
				if (std::ispunct(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c))) {
					if (!in_separator) {
						result += ' ';
						in_separator = true;
					}
				} else {
					if (in_separator) {
						result += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
						in_separator = false;
					} else {
						result += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
					}
				}
			}

			if (!result.empty() && result.back() == ' ') {
				result.pop_back();
			}

			return result;
		}

		/**
		 * Convert all alphabetic characters to lowercase.
		 *
		 * Non-alphabetic characters (digits, punctuation, whitespace, etc.)
		 * are preserved as-is.
		 *
		 * @param input The input string.
		 * @return A new, lowercased string.
		 */
		[[nodiscard]]
		static std::string toLowerCase(const std::string_view input)
		{
			std::string result;
			result.reserve(input.size());

			for (const char c : input) {
				result += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
			}

			return result;
		}

		/**
		 * Convert all alphabetic characters to UPPERCASE.
		 *
		 * Non-alphabetic characters (digits, punctuation, whitespace, etc.)
		 * are preserved as-is.
		 *
		 * @param input The input string.
		 * @return A new, uppercased string.
		 */
		[[nodiscard]]
		static std::string toUpperCase(const std::string_view input)
		{
			std::string result;
			result.reserve(input.size());

			for (const char c : input) {
				result += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
			}

			return result;
		}

		/**
		 * Remove leading and trailing whitespace characters.
		 *
		 * Whitespace is determined using `std::isspace` with the default C locale.
		 *
		 * Examples:
		 *  - `"  hello  "`    -> `"hello"`
		 *  - `" \t\n "`       -> `""`
		 *
		 * @param input The input string.
		 * @return A new string with outer whitespace removed.
		 */
		[[nodiscard]]
		static std::string trim(const std::string_view input)
		{
			auto is_space = [](const unsigned char ch) {
				return static_cast<bool>(std::isspace(ch));
			};

			// get starting bounds
			std::size_t start = 0;

			while (start < input.size() && is_space(static_cast<unsigned char>(input[start]))) {
				++start;
			}

			if (start == input.size()) {
				return std::string{};
			}

			// get ending bounds
			std::size_t end = input.size();

			while (is_space(static_cast<unsigned char>(input[end - 1]))) {
				--end;
			}

			return std::string(input.substr(start, end - start));
		}
};
