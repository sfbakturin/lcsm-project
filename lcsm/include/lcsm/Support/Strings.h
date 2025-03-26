#ifndef LCSM_SUPPORT_STRINGS_H
#define LCSM_SUPPORT_STRINGS_H

#include <lcsm/lcsmconfig.h>

#include <string>

namespace lcsm
{
	namespace support
	{
		LCSM_API std::string stringsEscape(const std::string &str);
		LCSM_API std::string stringsUnescape(const std::string &str);
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_STRINGS_H */
