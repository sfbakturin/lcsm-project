#include <lcsm/Support/Files.h>

#include <stdio.h>
#include <unistd.h>

bool lcsm::support::removeFile(const char *filename)
{
	return remove(filename) == 0;
}
