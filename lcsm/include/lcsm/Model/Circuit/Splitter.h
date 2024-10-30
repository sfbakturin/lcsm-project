#ifndef LCSM_MODEL_CIRCUIT_SPLITTER_H
#define LCSM_MODEL_CIRCUIT_SPLITTER_H

#include <lcsm/Component/CircuitComponent.h>
#include <lcsm/Component/Identifier.h>
#include <lcsm/Model/Width.h>
#include <lcsm/Model/Wiring/Wire.h>

#include <array>
#include <cstddef>
#include <limits>

namespace lcsm
{
	namespace model
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

			void swap(Splitter &other) noexcept;

			Width widthIn() const noexcept;
			std::size_t widthOut() const noexcept;

			void setWidthIn(Width newWidthIn) noexcept;
			void setWidthOut(std::size_t newWidthOut) noexcept;

			virtual Identifier ID() const noexcept override final;
			virtual Identifier identify(Identifier ID) noexcept override final;

			virtual void connectIn(wire_t &wire, std::size_t i) override final;
			virtual void connectOut(wire_t &wire, std::size_t i) override final;

			void connectBits(wire_t &wire);
			void connectBitN(wire_t &wire, std::size_t i);

			virtual CircuitComponentType circuitComponentType() const noexcept override final;

		  private:
			static constexpr std::size_t CONNECTORS_SIZE = Width::MaxWidth;
			static constexpr std::size_t NO_CONNECT = std::numeric_limits< std::size_t >::max();

			Identifier m_id;

			Width m_widthIn;
			std::size_t m_widthOut;
			std::array< std::size_t, CONNECTORS_SIZE > m_connectors;
			Wire m_wireIn;
			std::array< Wire, CONNECTORS_SIZE > m_wireOut;

			void resetConnectors() noexcept;
		};
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_CIRCUIT_SPLITTER_H */
