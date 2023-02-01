#pragma once //~mesma funcao do ifndef

#include <string>
#include <map>

/* 
* Classe usada para ler os argumentos da linha de comando
* 
* Exemplo:
* ArgumentParser argParser;
* argParser.RegisterFlag("rename");
* argParser.Parser(argc, argv);
* argParser.GetFlag("rename");
*/
class ArgumentParser
{
private:
	//todo membro de classe possui "m_" no inicio do nome
	std::map<std::string, bool> m_Flags; //map<tipo chave, tipo valor> nome;
	std::map<std::string, std::string> m_Options;

	//obtem o valor de uma opcao lida durante o parse
	const std::string& GetOption(const std::string& option) const;

	//obtem o valor de uma opcao lida durante o parse como float
	float GetOptionAsFloat(const std::string& option) const;

	//obtem o valor de uma opcao lida durante o parse como int
	int GetOptionAsInt(const std::string& option) const;

public:
	//Registra uma flag usada durante o parse
	void RegisterFlag(const std::string& flag); 

	//Registra uma opcao usada durante o parse
	void RegisterOption(const std::string& option);

	//Retorna se a flag está ou não registrada 
	bool IsFlagRegistered(const std::string& flag) const;

	//Retorna se a opcao está ou não registrada
	bool IsOptionRegistered(const std::string& option) const;

	//obtem o valor de uma flag lida durante o parse
	bool GetFlag(const std::string& flag) const; //a declaracao de const instrui o compilador de que o método nao pode alterar o estado do objeto, isto é, não pode alterar os atributos do objeto.

	//uma funcao template para retornar o tipo que eu quiser
	template<typename T>
	T GetOptionAs(const std::string& option) const; //funcao generica

	//tratamentos diferentes para os tipos que vou poder usar no template:
	template<>
	std::string GetOptionAs(const std::string& option) const { return GetOption(option); }

	template<>
	float GetOptionAs(const std::string& option) const { return GetOptionAsFloat(option); } //vou fazer o corpo dela aqui mesmo pra ficar mais claro

	template<>
	int GetOptionAs(const std::string& option) const { return GetOptionAsInt(option); }

	//Faz o parse da lista de argumentos em argv
	void Parse(int argc, char* argv[]); //adicione um const em char* argv[] para usar o googleTest!

};