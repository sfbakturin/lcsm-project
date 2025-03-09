#include <lcsm/Support/Files.h>

#include <windows.h>

bool lcsm::support::removeFile(const char *filename)
{
	return DeleteFile(filename);
}
