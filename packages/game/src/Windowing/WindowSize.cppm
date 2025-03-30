module;

#include <cstdint>
#include <numeric>
#include <string>
#include <iostream>
#include <algorithm>
#include <format>

export module Windowing.WindowSize;

export struct WindowSize
{
	public:
		/**
		 * Minimum screen width.
		 */
		static constexpr uint16_t MinWidth = 640;

		/**
		 * Maximum screen width.
		 */
		static constexpr uint16_t MaxWidth = 3840;

		/**
		 * Minimum screen height.
		 */
		static constexpr uint16_t MinHeight = 400;

		/**
		 * Maximum screen height.
		 */
		static constexpr uint16_t MaxHeight = 2160;

	protected:
		static uint16_t filterWidth(const uint16_t width)
		{
			return std::clamp(width, WindowSize::MinWidth, WindowSize::MaxWidth);
		}

		static uint16_t filterHeight(const uint16_t height)
		{
			return std::clamp(height, WindowSize::MinHeight, WindowSize::MaxHeight);
		}

		static std::string calculateLabel(const uint16_t width, const uint16_t height)
		{
			return std::format("{}x{}", width, height);
		}

		static double calculateAspectRatio(const uint16_t width, const uint16_t height)
		{
			return static_cast<double>(width) / static_cast<double>(height);
		}

		static std::string calculateAspectRatioString(const uint16_t width, const uint16_t height)
		{
			const int divisor = std::gcd(width, height);
			const int simplifiedWidth = width / divisor;
			const int simplifiedHeight = height / divisor;

			return std::format("{}:{}", simplifiedWidth, simplifiedHeight);
		}

		uint16_t width;
		uint16_t height;
		uint32_t area;
		std::string label;
		double aspectRatio;
		std::string aspectRatioString;

	public:
		WindowSize(const uint16_t width, const uint16_t height)
		{
			this->width = WindowSize::filterWidth(width);
			this->height = WindowSize::filterHeight(height);
			this->area = this->width * this->height;
			this->label = WindowSize::calculateLabel(this->width, this->height);
			this->aspectRatio = WindowSize::calculateAspectRatio(this->width, this->height);
			this->aspectRatioString = WindowSize::calculateAspectRatioString(this->width, this->height);
		}

		[[nodiscard]]
		uint16_t getWidth() const
		{
			return this->width;
		}

		[[nodiscard]]
		uint16_t getHeight() const
		{
			return this->height;
		}

		[[nodiscard]]
		uint32_t getArea() const
		{
			return this->area;
		}

		[[nodiscard]]
		inline std::string toString() const
		{
			return this->label;
		}

		bool operator==(const WindowSize& other) const
		{
			return this->width == other.getWidth() && this->height == other.getHeight();
		}

		bool operator>(const WindowSize& other) const
		{
			return this->getArea() > other.getArea();
		}

		bool operator>=(const WindowSize& other) const
		{
			return this->getArea() >= other.getArea();
		}

		bool operator<(const WindowSize& other) const
		{
			return this->getArea() < other.getArea();
		}

		bool operator<=(const WindowSize& other) const
		{
			return this->getArea() <= other.getArea();
		}

		friend std::ostream& operator<<(std::ostream& os, const WindowSize& WindowSize)
		{
			os << WindowSize.label;
			return os;
		}
};
