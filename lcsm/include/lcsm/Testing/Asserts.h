#ifndef LCSM_TESTING_ASSERTS_H
#define LCSM_TESTING_ASSERTS_H

#include <lcsm/LCSM.h>
#include <lcsm/Model/Circuit.h>
#include <lcsm/Physical/DataBits.h>
#include <lcsm/lcsmconfig.h>

#include <cstddef>
#include <string>

namespace lcsm
{
	namespace testing
	{
		LCSM_API void assertError(const char *general, const std::string &additional);

		LCSM_API void assertNotNullptr(const void *ptr, const char *additional = "<no information>");

		LCSM_API void assertEquals(const DataBits &actual, const DataBits &expected);
		LCSM_API void assertEquals(const DataBits &actual, DataBits &&expected);
		LCSM_API void assertEquals(std::size_t actual, std::size_t expected);

		LCSM_API void assertType(const Circuit *circuit, CircuitType type);

		LCSM_API void assertTrue(bool actual);
		LCSM_API void assertFalse(bool actual);
	}	 // namespace testing
}	 // namespace lcsm

#endif /* LCSM_TESTING_ASSERTS_H */
