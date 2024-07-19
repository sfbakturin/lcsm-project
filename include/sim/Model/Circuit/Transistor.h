#ifndef SIM_MODEL_CIRCUIT_TRANSISTOR_H
#define SIM_MODEL_CIRCUIT_TRANSISTOR_H

namespace sim
{
	enum TransistorType
	{
		P, N
	};

	class Transistor
	{
		public:
			Transistor() noexcept;
			Transistor(TransistorType type) noexcept;

			Transistor(const Transistor &other) noexcept;
			Transistor(Transistor &&other) noexcept;

			Transistor &operator=(const Transistor &other) noexcept;
			Transistor &operator=(Transistor &&other);

			void Swap(Transistor &other) noexcept;

		private:
			TransistorType m_type;
			/* <wire_type> base; */
			/* <wire_type> src1; */
			/* <wire_type> src2; */
	};
}

#endif /* SIM_MODEL_CIRCUIT_TRANSISTOR_H */
