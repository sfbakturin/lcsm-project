#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/Testing/Asserts.h>

#include <cstdlib>
#include <iostream>
#include <string>

void lcsm::testing::assertError(const char *general, const std::string &additional)
{
	std::cerr << "Testing error! " << general << ": " << additional << ".\n";
	std::exit(EXIT_FAILURE);
}

void lcsm::testing::assertNotNullptr(const void *ptr, const char *additional)
{
	if (ptr == nullptr)
	{
		assertError("Nullptr", additional);
	}
}

void lcsm::testing::assertEquals(const lcsm::DataBits &actual, const lcsm::DataBits &expected)
{
	if (actual != expected)
	{
		const std::string additional = "actual DataBits != expected DataBits [" + actual.toString() + " vs. " + expected.toString();
		assertError("Assertion", additional);
	}
}

void lcsm::testing::assertEquals(const lcsm::DataBits &actual, lcsm::DataBits &&expected)
{
	if (actual != expected)
	{
		const std::string additional =
			"actual DataBits != expected DataBits [" + actual.toString() + " vs. " + expected.toString() + "]";
		assertError("Assertion", additional);
	}
}

void lcsm::testing::assertEquals(std::size_t actual, std::size_t expected)
{
	if (actual != expected)
	{
		const std::string additional = "actual != expected [" + std::to_string(actual) + " vs. " + std::to_string(expected) + "]";
		assertError("Assertion", additional);
	}
}

void lcsm::testing::assertType(const lcsm::Circuit *circuit, lcsm::CircuitType type)
{
	assertNotNullptr(circuit, "circuit doesn't exists");
	if (circuit->circuitType() != type)
	{
		const std::string additional =
			"circuit \"" + circuit->name() + "\" has wrong circuit's type [" + std::to_string(circuit->circuitType()) +
			" vs. " + std::to_string(type) + "]";
		assertError("Assertion", additional);
	}
}

void lcsm::testing::assertTrue(bool actual)
{
	if (!actual)
	{
		const std::string additional = "expected true, but found false";
		assertError("Assertion", additional);
	}
}

void lcsm::testing::assertFalse(bool actual)
{
	if (actual)
	{
		const std::string additional = "expected false, but found true";
		assertError("Assertion", additional);
	}
}
