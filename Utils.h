#pragma once

#include <string>

namespace Utils
{
	//funcao para diminuir os caracteres de uma string
	std::string ToLower(std::string str);
	
	//funcao para verificar se uma string contem espaco
	bool HasWhitespace(const std::string str);
}