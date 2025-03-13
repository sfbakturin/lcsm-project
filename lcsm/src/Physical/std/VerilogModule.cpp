#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Physical/Context.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Physical/Evaluator.h>
#include <lcsm/Physical/Event.h>
#include <lcsm/Physical/Instruction.h>
#include <lcsm/Physical/Timestamp.h>
#include <lcsm/Physical/std/Verilog.h>
#include <lcsm/Support/PointerView.hpp>
#include <lcsm/Verilog/Module.h>
#include <lcsm/Verilog/Port.h>
#include <unordered_map>
#include <unordered_set>

#include <deque>
#include <stdexcept>
#include <utility>
#include <vector>

lcsm::physical::VerilogModule::VerilogModule(const lcsm::verilog::Module *module, lcsm::object_type_t objectType) :
	lcsm::EvaluatorNode(objectType), m_module(module)
{
}

lcsm::NodeType lcsm::physical::VerilogModule::nodeType() const noexcept
{
	return lcsm::NodeType::Dynamic;
}

std::size_t lcsm::physical::VerilogModule::contextSize() const noexcept
{
	return m_inputs.size() + m_inouts.size() + m_outputs.size();
}

std::size_t lcsm::physical::VerilogModule::privateContextSize() const noexcept
{
	return 0;
}

void lcsm::physical::VerilogModule::setContext(const lcsm::support::PointerView< lcsm::Context > &context)
{
	// If context is already exists, then reset it.
	if (m_context)
	{
		resetContext();
	}

	// Set and verify.
	m_context = context;
	verifyContext();
}

void lcsm::physical::VerilogModule::resetContext() noexcept
{
	m_context.reset();
}

void lcsm::physical::VerilogModule::verifyContext()
{
	// Check sizes.
	if (contextSize() != m_context->size() || privateContextSize() != m_context->privateSize())
	{
		resetContext();
		throw std::logic_error("Bad context size");
	}
}

static inline lcsm::portid_t
	Find(const std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > > &wires, const lcsm::EvaluatorNode *toFind)
{
	for (lcsm::portid_t i = 0; i < static_cast< lcsm::portid_t >(wires.size()); i++)
	{
		if (wires[i] == toFind)
		{
			return i;
		}
	}
	return -1;
}

void lcsm::physical::VerilogModule::addInstant(const Instruction &instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::InstructionType type = instruction.type();

	// Check, if target is this circuit.
	if (target != this)
	{
		throw std::logic_error("Bad target!");
	}

	// Check instruction.
	if (type != lcsm::InstructionType::WriteValue)
	{
		throw std::logic_error("Bad instruction!");
	}

	// Check, if caller is one of outputs, then ignore this instant.
	if (Find(m_outputs, caller) >= 0)
	{
		return;
	}

	// Check, if caller is input.
	const lcsm::portid_t foundInput = Find(m_inputs, caller);
	if (foundInput >= 0)
	{
		m_inputsInstants.emplace_back(foundInput, instruction);
		return;
	}

	// Check, if caller is inout.
	const lcsm::portid_t foundInout = Find(m_inputs, caller);
	if (foundInput >= 0)
	{
		m_inoutsInstants.emplace_back(foundInout, instruction);
		return;
	}

	// Otherwise, error.
	throw std::logic_error("Bad caller!");
}

void lcsm::physical::VerilogModule::addInstant(Instruction &&instruction)
{
	const lcsm::EvaluatorNode *caller = instruction.caller();
	const lcsm::EvaluatorNode *target = instruction.target();
	const lcsm::InstructionType type = instruction.type();

	// Check, if target is this circuit.
	if (target != this)
	{
		throw std::logic_error("Bad target!");
	}

	// Check instruction.
	if (type != lcsm::InstructionType::WriteValue)
	{
		throw std::logic_error("Bad instruction!");
	}

	// Check, if caller is one of outputs, then ignore this instant.
	if (Find(m_outputs, caller) >= 0)
	{
		return;
	}

	// Check, if caller is input.
	const lcsm::portid_t foundInput = Find(m_inputs, caller);
	if (foundInput >= 0)
	{
		m_inputsInstants.emplace_back(foundInput, std::move(instruction));
		return;
	}

	// Check, if caller is inout.
	const lcsm::portid_t foundInout = Find(m_inputs, caller);
	if (foundInput >= 0)
	{
		m_inoutsInstants.emplace_back(foundInout, std::move(instruction));
		return;
	}

	// Otherwise, error.
	throw std::logic_error("Bad caller!");
}

static void GenerateEvents(
	lcsm::EvaluatorNode *caller,
	std::vector< lcsm::support::PointerView< lcsm::EvaluatorNode > > &wires,
	std::unordered_map< lcsm::verilog::PortDirectionType, std::vector< lcsm::DataBits > > &data,
	std::vector< lcsm::Event > &events,
	lcsm::verilog::PortDirectionType portDirectionType)
{
	// Create write value events.
	if (!wires.empty())
	{
		// Verilog module ensures, that outputData[ioType] is exists and its size == wires.size().
		const std::size_t size = wires.size();
		std::vector< lcsm::DataBits > &output = data[portDirectionType];
		for (std::size_t i = 0; i < size; i++)
		{
			lcsm::Instruction I{ lcsm::InstructionType::WriteValue, caller, wires[i].get(), std::move(output[i]) };
			events.emplace_back(I);
		}
	}
}

static void ExtractValues(
	std::size_t size,
	bool takeFirst,
	std::deque< std::pair< lcsm::portid_t, lcsm::Instruction > > &instants,
	lcsm::support::PointerView< lcsm::Context > &context,
	std::size_t offset,
	std::unordered_map< lcsm::verilog::PortDirectionType, std::vector< lcsm::DataBits > > &testBenchData,
	lcsm::verilog::PortDirectionType portDirectionType)
{
	if (size != 0)
	{
		std::vector< lcsm::DataBits > databits(size);
		std::unordered_set< lcsm::portid_t > taken;
		std::vector< std::pair< lcsm::portid_t, lcsm::Instruction > > cleaned;

		// If NOW is later, then THEN, then we should take first value as not-dirty.
		if (takeFirst)
		{
			for (std::pair< lcsm::portid_t, lcsm::Instruction > &inputInstant : instants)
			{
				if (taken.find(inputInstant.first) != taken.end())
				{
					cleaned.push_back(std::move(inputInstant));
				}
				else
				{
					const lcsm::portid_t idx = inputInstant.first;
					taken.emplace(idx);
					databits[idx] = inputInstant.second.value();
				}
			}
		}

		// Extract all other values from context.
		for (lcsm::portid_t i = 0; i < static_cast< lcsm::portid_t >(size); i++)
		{
			if (taken.find(i) == taken.end())
			{
				databits[i] = context->getValue(i + offset);
			}
		}

		// Invoke instructions.
		for (const std::pair< lcsm::portid_t, lcsm::Instruction > &inputInstant : cleaned)
		{
			const lcsm::portid_t idx = inputInstant.first;
			const lcsm::Instruction &i = inputInstant.second;
			databits[idx] |= i.value();
		}

		// Transfer values to test bench data.
		testBenchData[portDirectionType] = std::move(databits);
	}
}

std::vector< lcsm::Event > lcsm::physical::VerilogModule::invokeInstants(const lcsm::Timestamp &now)
{
	// Generated events.
	std::vector< lcsm::Event > events;

	// Test bench data.
	std::unordered_map< lcsm::verilog::PortDirectionType, std::vector< lcsm::DataBits > > testBenchData;

	// Extract timestamp.
	const lcsm::Timestamp &then = m_context->lastUpdate();
	const bool takeFirst = now > then;

	// Extract values and invoke instructions on inputs.
	ExtractValues(m_inputs.size(), takeFirst, m_inputsInstants, m_context, 0, testBenchData, lcsm::verilog::PortDirectionType::InputPortDirection);

	// Extract values and invoke instructions on inouts.
	ExtractValues(m_inouts.size(), takeFirst, m_inoutsInstants, m_context, m_inputs.size(), testBenchData, lcsm::verilog::PortDirectionType::InoutPortDirection);

	// Invoke the Verilog module.
	std::unordered_map< lcsm::verilog::PortDirectionType, std::vector< lcsm::DataBits > > outputData = m_module->invoke(testBenchData);

	// Create for inouts.
	GenerateEvents(this, m_inouts, outputData, events, lcsm::verilog::PortDirectionType::InoutPortDirection);

	// Create for outputs.
	GenerateEvents(this, m_outputs, outputData, events, lcsm::verilog::PortDirectionType::OutputPortDirection);

	// Save inputs values to context.
	m_context->beginUpdate(now);
	std::size_t i = 0;
	if (!m_inputs.empty())
	{
		// Algorithm above make guarantees, that m_inputs would be used to generate testBenchData.
		for (lcsm::DataBits &databits : testBenchData[lcsm::verilog::PortDirectionType::InputPortDirection])
		{
			m_context->updateValue(i++, std::move(databits));
		}
	}
	if (!m_inouts.empty())
	{
		// Algorithm above make guarantees, that m_inouts would be used to generate testBenchData.
		for (lcsm::DataBits &databits : testBenchData[lcsm::verilog::PortDirectionType::InoutPortDirection])
		{
			m_context->updateValue(i++, std::move(databits));
		}
	}
	if (!m_outputs.empty())
	{
		// Algorithm above make guarantees, that m_outputs would be used to generate outputData.
		for (lcsm::DataBits &databits : outputData[lcsm::verilog::PortDirectionType::OutputPortDirection])
		{
			m_context->updateValue(i++, std::move(databits));
		}
	}
	m_context->endUpdate();

	// Clear instants.
	m_inputsInstants.clear();
	m_inoutsInstants.clear();

	return events;
}

void lcsm::physical::VerilogModule::connectInput(const lcsm::support::PointerView< lcsm::EvaluatorNode > &input)
{
	m_inputs.push_back(input);
}

void lcsm::physical::VerilogModule::connectInout(const lcsm::support::PointerView< lcsm::EvaluatorNode > &inout)
{
	m_inouts.push_back(inout);
}

void lcsm::physical::VerilogModule::connectOutput(const lcsm::support::PointerView< lcsm::EvaluatorNode > &output)
{
	m_outputs.push_back(output);
}
