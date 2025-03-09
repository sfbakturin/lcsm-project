#include <initializer_list>
#include <lcsm/Support/Files.h>

#include <string>

bool lcsm::support::removeFile(const std::string &filename)
{
	return lcsm::support::removeFile(filename.c_str());
}

bool lcsm::support::removeFile(std::initializer_list< const char * > filenames)
{
	bool retval = true;
	for (const char *filename : filenames)
	{
		retval &= lcsm::support::removeFile(filename);
	}
	return retval;
}

bool lcsm::support::removeFile(const std::vector< std::string > &filenames)
{
	bool retval = true;
	for (const std::string &filename : filenames)
	{
		retval &= lcsm::support::removeFile(filename);
	}
	return retval;
}
