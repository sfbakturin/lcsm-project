#ifndef SIM_MODEL_CIRCUIT_SPLITTER_H
#define SIM_MODEL_CIRCUIT_SPLITTER_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Model/Width.h>
#include <sim/Model/Wiring/Wire.h>

#include <array>
#include <cstddef>
#include <limits>

namespace sim
{
	class Splitter : public CircuitComponent
	{
	  public:
		Splitter(Width widthIn, std::size_t widthOut);

		Splitter(const Splitter &other);
		Splitter(Splitter &&other) noexcept;

		Splitter &operator=(const Splitter &other);
		Splitter &operator=(Splitter &&other) noexcept;

		void Swap(Splitter &other) noexcept;

		Width GetWidthIn() const noexcept;
		std::size_t GetWidthOut() const noexcept;

		void SetWidthIn(Width newWidthIn) noexcept;
		void SetWidthOut(std::size_t newWidthOut) noexcept;

		virtual void ConnectIn(const wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(const wire_t &wire, std::size_t i) override;

		void ConnectBits(const wire_t &wire);
		void ConnectBitN(const wire_t &wire, std::size_t i);

	  private:
		static constexpr std::size_t CONNECTORS_SIZE = Width::W64;
		static constexpr std::size_t NO_CONNECT = std::numeric_limits< std::size_t >::max();

		Width m_widthIn;
		std::size_t m_widthOut;
		std::array< std::size_t, CONNECTORS_SIZE > m_connectors;
		sim::Wire m_wireIn;
		std::array< sim::Wire, CONNECTORS_SIZE > m_wireOut;

		void ResetConnectors() noexcept;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_SPLITTER_H */
