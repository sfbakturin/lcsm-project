#ifndef LCSM_TESTING_ASSERTS_H
#define LCSM_TESTING_ASSERTS_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Physical/DataBits.h>

#include <cstddef>
#include <string>

namespace lcsm
{
	namespace testing
	{
		void assertError(const char *general, const std::string &additional);

		void assertNotNullptr(const void *ptr, const char *additional = "<no information>");

		void assertEquals(const DataBits &actual, const DataBits &expected);
		void assertEquals(const DataBits &actual, DataBits &&expected);
		void assertEquals(std::size_t actual, std::size_t expected);

		void assertType(const Circuit *circuit, CircuitType type);

		void assertTrue(bool actual);
		void assertFalse(bool actual);
	}	 // namespace testing
}	 // namespace lcsm

#endif /* LCSM_TESTING_ASSERTS_H */
