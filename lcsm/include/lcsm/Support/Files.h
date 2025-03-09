#ifndef LCSM_SUPPORT_FILES_H
#define LCSM_SUPPORT_FILES_H

#include <initializer_list>

#include <string>
#include <vector>

namespace lcsm
{
	namespace support
	{
		bool removeFile(const char *filename);
		bool removeFile(const std::string &filename);
		bool removeFile(std::initializer_list< const char * > filenames);
		bool removeFile(const std::vector< std::string > &filenames);
	}	 // namespace support
}	 // namespace lcsm

#endif /* LCSM_SUPPORT_FILES_H */
