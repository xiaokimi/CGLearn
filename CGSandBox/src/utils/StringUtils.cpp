#include "StringUtils.h"

void StringUtils::split(const std::string &in, std::vector<std::string> &out, std::string token)
{
	out.clear();

	int tokenSize = token.size();
	std::string temp;

	for (int i = 0; i < int(in.size()); i++)
	{
		std::string test = in.substr(i, tokenSize);
		if (test == token)
		{
			if (!temp.empty())
			{
				out.push_back(temp);
				temp.clear();
				i += tokenSize - 1;
			}
			else
			{
				out.push_back("");
			}
		}
		else if (i + token.size() >= in.size())
		{
			temp += in.substr(i, tokenSize);
			out.push_back(temp);
			break;
		}
		else
		{
			temp += in[i];
		}
	}
}

std::string StringUtils::firstToken(const std::string& in)
{
	if (in.empty())
	{
		return "";
	}

	size_t tokenStart = in.find_first_not_of(" \t");
	size_t tokenEnd = in.find_first_of(" \t", tokenStart);

	if (tokenStart != std::string::npos && tokenEnd != std::string::npos)
	{
		return in.substr(tokenStart, tokenEnd - tokenStart);
	}
	else if (tokenStart != std::string::npos)
	{
		return in.substr(tokenStart);
	}

	return "";
}

std::string StringUtils::tail(const std::string& in)
{
	if (in.empty())
	{
		return "";
	}

	size_t tokenStart = in.find_first_not_of(" \t");
	size_t spaceStart = in.find_first_of(" \t", tokenStart);
	size_t tailStart = in.find_first_not_of(" \t", spaceStart);
	size_t tailEnd = in.find_last_not_of(" \t");

	if (tailStart != std::string::npos && tailEnd != std::string::npos)
	{
		return in.substr(tailStart, tailEnd - tailStart + 1);
	}
	else if (tailStart != std::string::npos)
	{
		return in.substr(tailStart);
	}

	return "";
}