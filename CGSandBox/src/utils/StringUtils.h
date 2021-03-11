#pragma once

#include "global.h"

class StringUtils
{
public:
	static void split(const std::string &in, std::vector<std::string> &out, std::string token);
	static std::string firstToken(const std::string& in);
	static std::string tail(const std::string& in);
	
	template<typename T>
	static const T& getElement(const std::vector<T>& elements, std::string& index)
	{
		int idx = std::stoi(index);
		if (idx < 0)
		{
			idx = int(elements.size()) + idx;
		}
		else
		{
			idx--;
		}

		return elements[idx];
	}
};
