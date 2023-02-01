#include <iostream>
#include <iomanip> //para usar o std::boolalpha, que converte 0 e 1 para false e true no terminal

#include "ArgumentParser.h"
#include "Mode.h"

int main(int argc, char* argv[]) 
{
	setlocale(LC_ALL, "pt_BR"); //acentuacao
	setlocale(LC_NUMERIC, "en_US"); //mudo a regiao do tipo numerico, para usar "." ao inves de "," nos numeros flutuantes

	ArgumentParser argParser;

	//Registra as flags
	argParser.RegisterFlag(Args::Flags::Help);
	argParser.RegisterFlag(Args::Flags::Rename);
	argParser.RegisterFlag(Args::Flags::Convert);
	argParser.RegisterFlag(Args::Flags::Resize);
	argParser.RegisterFlag(Args::Flags::Scale);

	//Registra as opcoes
	argParser.RegisterOption(Args::Opts::Folder);
	argParser.RegisterOption(Args::Opts::Filter);
	argParser.RegisterOption(Args::Opts::Width);
	argParser.RegisterOption(Args::Opts::Height);
	argParser.RegisterOption(Args::Opts::Amount);
	argParser.RegisterOption(Args::Opts::Prefix);
	argParser.RegisterOption(Args::Opts::StartNumber);
	argParser.RegisterOption(Args::Opts::From);
	argParser.RegisterOption(Args::Opts::To);

	argParser.Parse(argc, argv); //o Parse() vai ler todos os argumentos que nos registramos

	//try catch para verificar se existe mais de um modo ativo
	try 
	{
		if (std::unique_ptr<Mode> PhotoBatchMode = CreateMode(argParser)) //repare que eu retornei nullptr no final do CreateMode, entao se nao for nulo executa o run()
		{
			PhotoBatchMode->Run();
		}
	}
	catch (const std::exception& exception)
	{
		std::cerr << exception.what() << std::endl;
		std::cerr << "Para obter ajuda coloque a flag \"--help\"";
	}


	return 0;
}