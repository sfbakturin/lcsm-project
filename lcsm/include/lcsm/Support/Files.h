#ifndef LCSM_SUPPORT_FILES_H
#define LCSM_SUPPORT_FILES_H

#include <initializer_list>
#include <lcsm/lcsmconfig.h>

#include <string>
#include <vector>

namespace lcsm
{
	namespace support
	{
		LCSM_API bool removeFile(const char *filename);
		LCSM_API bool removeFile(const std::string &filename);
		LCSM_API bool removeFile(std::initializer_list< const char * > filenames);
		LCSM_API bool removeFile(const std::vector< std::string > &filenames);
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_FILES_H */
