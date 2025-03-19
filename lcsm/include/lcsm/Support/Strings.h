#ifndef LCSM_SUPPORT_STRINGS_H
#define LCSM_SUPPORT_STRINGS_H

#include <string>

namespace lcsm
{
	namespace support
	{
		std::string stringsEscape(const std::string &str);
		std::string stringsUnescape(const std::string &str);
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_STRINGS_H */
