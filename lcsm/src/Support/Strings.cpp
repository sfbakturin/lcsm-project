#include <lcsm/Support/Strings.h>

#include <string>

std::string lcsm::support::stringsEscape(const std::string &str)
{
	std::string builder;
	builder.reserve(str.size());
	for (char c : str)
	{
		switch (c)
		{
		case '\n':
		{
			builder.append("\\n");
			break;
		}
		case '\t':
		{
			builder.append("\\t");
			break;
		}
		case '\r':
		{
			builder.append("\\r");
			break;
		}
		case '\\':
		{
			builder.append("\\\\");
			break;
		}
		default:
		{
			builder.push_back(c);
			break;
		}
		}
	}
	return builder;
}

std::string lcsm::support::stringsUnescape(const std::string &str)
{
	std::string builder;
	builder.reserve(str.size());
	bool backSlashStart = false;
	for (char c : str)
	{
		if (backSlashStart)
		{
			switch (c)
			{
			case 'n':
			{
				builder.push_back('\n');
				break;
			}
			case 't':
			{
				builder.push_back('\t');
				break;
			}
			case 'r':
			{
				builder.push_back('\r');
				break;
			}
			case '\\':
			{
				builder.push_back('\\');
				break;
			}
			default:
			{
				builder.push_back('\\');
				builder.push_back(c);
				break;
			}
			}
			backSlashStart = false;
		}
		else
		{
			if (c == '\\')
			{
				backSlashStart = true;
			}
			else
			{
				builder.push_back(c);
			}
		}
	}
	return builder;
}
