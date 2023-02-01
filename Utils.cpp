#include "Utils.h"

#include <algorithm>  //para fazer a conversao de strings para tudo minusculo

namespace Utils
{
	std::string ToLower(std::string str)
	{
		std::transform(std::begin(str), std::end(str), std::begin(str),
			[](unsigned char c) {return std::tolower(c); }); //vou usar uma funcao lambda, veja na documentacao do algorithm para ver o transform()

		return str;
	}

	bool HasWhitespace(const std::string std)
	{
		return std.find_first_of(' ') != std::string::npos;
	}
} 