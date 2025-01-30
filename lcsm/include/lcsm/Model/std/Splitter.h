#ifndef LCSM_MODEL_STD_SPLITTER_H
#define LCSM_MODEL_STD_SPLITTER_H

namespace lcsm
{
	namespace model
	{
		// class Splitter : public Circuit
		// {
		//   public:
		// 	enum Port : portid_t
		// 	{
		// 		Wiring = 65,
		// 		Out0 = 0,
		// 		Out63 = 63
		// 	};

		//   public:
		// 	Splitter(Width widthIn, width_t widthOut);

		// 	Splitter(const Splitter &other);
		// 	Splitter(Splitter &&other) noexcept;

		// 	Splitter &operator=(const Splitter &other);
		// 	Splitter &operator=(Splitter &&other) noexcept;

		// 	void swap(Splitter &other) noexcept;

		// 	Width widthIn() const noexcept;
		// 	void setWidthIn(Width newWidthIn) noexcept;

		// 	width_t widthOut() const noexcept;
		// 	void setWidthOut(width_t newWidthOut) noexcept;

		// 	virtual Identifier id() const noexcept override final;
		// 	virtual Identifier identify(Identifier id) noexcept override final;

		// 	virtual void connect(portid_t portId, const support::PointerView< Circuit > &circuit) override final;
		// 	void connectIn(const support::PointerView< Circuit > &circuit);

		//   private:
		// 	static constexpr std::size_t CONNECTORS_SIZE = Width::LastWidth;
		// 	static constexpr std::size_t NO_CONNECT = std::numeric_limits< std::size_t >::max();

		// 	Identifier m_id;

		// 	Width m_widthIn;
		// 	width_t m_widthOut;

		// 	std::array< std::size_t, CONNECTORS_SIZE > m_connectInsToOuts;

		// 	Wire m_wireIn;
		// 	std::array< Wire, CONNECTORS_SIZE > m_wireOut;

		//   private:
		// 	void resetConnectors() noexcept;
		// };
		// class Pin;

		// class Splitter : public CircuitComponent
		// {
		//   public:
		// 	Splitter(Width widthIn, std::size_t widthOut);

		// 	Splitter(const Splitter &other);
		// 	Splitter(Splitter &&other) noexcept;

		// 	Splitter &operator=(const Splitter &other);
		// 	Splitter &operator=(Splitter &&other) noexcept;

		// 	void swap(Splitter &other) noexcept;

		// 	Width widthIn() const noexcept;
		// 	std::size_t widthOut() const noexcept;

		// 	void setWidthIn(Width newWidthIn) noexcept;
		// 	void setWidthOut(std::size_t newWidthOut) noexcept;

		// 	virtual Identifier ID() const noexcept override final;
		// 	virtual Identifier identify(Identifier ID) noexcept override final;

		// 	virtual void connectIn(wire_t &wire, std::size_t i) override final;
		// 	virtual void connectOut(wire_t &wire, std::size_t i) override final;

		// 	void connectBits(wire_t &wire);
		// 	void connectBitN(wire_t &wire, std::size_t i);

		// 	virtual CircuitComponentType circuitComponentType() const noexcept override final;

		// 	virtual Splitter *asSplitter() noexcept override final;
		// 	virtual const Splitter *asSplitter() const noexcept override final;

		//   private:
		// 	static constexpr std::size_t CONNECTORS_SIZE = Width::LastWidth;
		// 	static constexpr std::size_t NO_CONNECT = std::numeric_limits< std::size_t >::max();

		// 	Identifier m_id;

		// 	Width m_widthIn;
		// 	std::size_t m_widthOut;
		// 	std::array< std::size_t, CONNECTORS_SIZE > m_connectors;
		// 	Wire m_wireIn;
		// 	std::array< Wire, CONNECTORS_SIZE > m_wireOut;

		// 	void resetConnectors() noexcept;
		// };
	}	 // namespace model
}	 // namespace lcsm

#endif /* LCSM_MODEL_STD_SPLITTER_H */
