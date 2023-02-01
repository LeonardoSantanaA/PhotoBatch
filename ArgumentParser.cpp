#include "ArgumentParser.h"
#include "Utils.h"

#include <cctype> //isspace()



void ArgumentParser::RegisterFlag(const std::string& flag) //vou passar a referencia da string pra nao ficar copiando ela toda hora
{
	if (!flag.empty() && !Utils::HasWhitespace(flag))
	{
		m_Flags[flag] = false; //toda flag que eu registrar vai receber false, e vou registrar ela no meu map
	}
}

void ArgumentParser::RegisterOption(const std::string& option)
{
	if (!option.empty() && !Utils::HasWhitespace(option))
	{
		m_Options[option] = ""; //valor padrao, uma string vazia
	}
}

bool ArgumentParser::IsFlagRegistered(const std::string& flag) const
{
	if (!flag.empty() &&!Utils::HasWhitespace(flag))
	{
		return m_Flags.count(flag) == 1;
	}
	return false;
}

bool ArgumentParser::IsOptionRegistered(const std::string& option) const
{
	if (!option.empty() && !Utils::HasWhitespace(option))
	{
		return m_Options.count(option) == 1;
	}
	return false;
}

bool ArgumentParser::GetFlag(const std::string& flag) const //a declaracao de const instrui o compilador de que o método nao pode alterar o estado do objeto, isto é, não pode alterar os atributos do objeto.
{
	if (!flag.empty()) //se nao for vazia
	{
		auto flagIt = m_Flags.find(flag); //para pesquisar no meu map se está registrado a flag que eu quero pegar (o tipo auto me retorna um iterator)
		if (flagIt != std::end(m_Flags)) //verifica se o iterator que me retornou é valido
		{
			return flagIt->second; //o iterador é um par, o flagIt->first é a chave, enquanto o flagIt->second é o valor
		}
	}
	return false;
}

const std::string& ArgumentParser::GetOption(const std::string& option) const
{
	if (!option.empty())
	{
		auto optionIt = m_Options.find(option);
		if (optionIt != std::end(m_Options))
		{
			return optionIt->second;
		}
	}
	static std::string emptyOption = ""; 
	return emptyOption;
	/*
	* perceba que o meu metodo retorna uma referencia(&), entao eu preciso que esse valor esteja armazenado em algum lugar,
	* se eu fizer um return "" com o valor direto, como eu retorno um endereco temporario ele vai sumir,
	* e isso pode crashar meu programa, entao eu declaro uma variavel estatica, que só vai sumir quando o programa fechar, entao posso retornar ela quantas vezes eu quiser sem problema
	*/
}

float ArgumentParser::GetOptionAsFloat(const std::string& option) const
{
	const std::string& optionValue = GetOption(option); //eu posso chamar o GetOption pra pegar o valor da opcao

	if(!optionValue.empty() && !Utils::HasWhitespace(option))
	{
		return std::stof(optionValue); //StringToFloat, vou fazer a conversao
	}
	return -1;
}

int ArgumentParser::GetOptionAsInt(const std::string& option) const
{
	const std::string& optionValue = GetOption(option); //eu posso chamar o GetOption pra pegar o valor da opcao
	char charOption;
	charOption = option[0];

	if (!optionValue.empty() && !isspace(charOption))
	{
		return std::stoi(optionValue); //StringToInt, vou fazer a conversao
	}
	return -1;
}

void ArgumentParser::Parse(int argc, char* argv[])
{
	if (argc > 1 && argv != nullptr)
	{
		for (int i = 0; i < argc; i++)
		{
			std::string arg = Utils::ToLower(argv[i]); //vamos converter tudo pra caracteres minusculos

			if (arg.length() >= 3) //definicao de uma flag válida, tem que ter mais de 2 caracteres
			{
				if (arg[0] == '-' && arg[1] == '-') //verifica se os 2 primeiros caracteres é --, assim temos o prefixo da flag
				{
					//agora precisamos remover o "--" iniciais para pegar apenas a palavra
					arg = arg.substr(2); //o substr() vai remover os 2 primeiros caracteres
					//agora precisamos verificar se é uma opcao
					if (arg.find_first_of('=') != std::string::npos) //find_first_of é "encontrar a primeira ocorrencia de: ", e "npos" é um valor estatico com o maior valor possivel para um elemento do tipo (nesse caso string) e é definido como -1
					{
						//é uma opcao
						const size_t equalSignPos = arg.find('='); //acha a posicao do "=" na string
						if (equalSignPos != std::string::npos) //verifica se achamos o "="
						{
							//dividir a opcao em chave e valor
							std::string optionName = arg.substr(0, equalSignPos); //pego a string da posicao 0 até o simbolo de "="
							std::string optionValue = arg.substr(equalSignPos + 1); //pego a string que vem depois do simbolo de "="

							auto optionIt = m_Options.find(optionName);
							if (optionIt != std::end(m_Options))
							{
								//achamos uma opcao registrada
								optionIt->second = optionValue; //vou passar o valor
							}
						}
					}
					else
					{
						//é uma flag
						auto flagIt = m_Flags.find(arg); //verificar se a flag foi registrada
						if (flagIt != std::end(m_Flags))
						{
							//achamos uma flag registrada
							flagIt->second = true; //vou setar o valor como true
							/*Voce pode fazer m_Flags[arg] = true; para setar o valor como true tambem, porem fazendo isso ele vai fazer uma busca,
							entao para dar uma minima otimizada vamos usar o iterator para mudar o valor mesmo*/
						}
					}

				}
			}
		}
	}
}

