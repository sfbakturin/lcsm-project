#include <Support/Strings.h>

#include <QString>
#include <string>

std::string QtStringToAsciiStd(const QString &s)
{
	std::string builder;
	for (const QChar &c : s)
	{
		builder.push_back(c.toLatin1());
	}
	return builder;
}
