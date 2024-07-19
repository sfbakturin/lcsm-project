#ifndef SIM_MODEL_CIRCUIT_SPLITTER_H
#define SIM_MODEL_CIRCUIT_SPLITTER_H

#include <sim/Model/Width.h>

#include <cstddef>
#include <limits>

namespace sim
{
	class Splitter
	{
		public:
			Splitter() noexcept;
			Splitter(Width widthIn) noexcept;
			Splitter(Width widthIn, std::size_t widthOut) noexcept;

			Splitter(const Splitter &other) noexcept;
			Splitter(Splitter &&other) noexcept;

			Splitter &operator=(const Splitter &other) noexcept;
			Splitter &operator=(Splitter &&other) noexcept;

			void Swap(Splitter &other) noexcept;

			Width GetWidthIn() const noexcept;
			std::size_t GetWidthOut() const noexcept;

			void SetWidthIn(Width newWidthIn) noexcept;
			void SetWidthOut(std::size_t newWidthOut) noexcept;

		private:
			static constexpr std::size_t CONNECTORS_SIZE = Width::W64;
			static constexpr std::size_t NO_CONNECT = std::numeric_limits<std::size_t>::max();

			static_assert(CONNECTORS_SIZE < NO_CONNECT);

			Width m_widthIn;
			std::size_t m_widthOut;
			std::size_t m_connectors[CONNECTORS_SIZE];

			void ResetConnectors() noexcept;
	};
}

#endif /* SIM_MODEL_CIRCUIT_SPLITTER_H */
