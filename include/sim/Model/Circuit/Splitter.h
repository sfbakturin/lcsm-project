#ifndef SIM_MODEL_CIRCUIT_SPLITTER_H
#define SIM_MODEL_CIRCUIT_SPLITTER_H

#include <sim/Component/CircuitComponent.h>
#include <sim/Component/Identifier.h>
#include <sim/IR/Width.h>
#include <sim/Model/Wiring/Wire.h>

#include <array>
#include <cstddef>
#include <limits>

namespace sim
{
	class Pin;

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

		virtual Identifier ID() const noexcept override final;
		virtual Identifier identify(Identifier ID) noexcept override final;

		virtual void ConnectIn(wire_t &wire, std::size_t i) override;
		virtual void ConnectOut(wire_t &wire, std::size_t i) override;

		void ConnectBits(wire_t &wire);
		void ConnectBitN(wire_t &wire, std::size_t i);

		virtual CircuitComponentType circuitComponentType() const noexcept override;

	  private:
		static constexpr std::size_t CONNECTORS_SIZE = Width::W64;
		static constexpr std::size_t NO_CONNECT =
			std::numeric_limits< std::size_t >::max();

		Identifier m_id;

		Width m_widthIn;
		std::size_t m_widthOut;
		std::array< std::size_t, CONNECTORS_SIZE > m_connectors;
		Wire m_wireIn;
		std::array< Wire, CONNECTORS_SIZE > m_wireOut;

		void ResetConnectors() noexcept;
	};
}	 // namespace sim

#endif /* SIM_MODEL_CIRCUIT_SPLITTER_H */
