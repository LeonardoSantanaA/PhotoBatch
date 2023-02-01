
/*
#include <gtest/gtest.h>

#include <ArgumentParser.h>
#include <array>

//testar registro de flag comum
TEST(TestArgumentParser, RegisterFlag) //TEST(nomeConjunto, nomeTeste)
{
	//Criar um objeto ArgumentParser
	ArgumentParser argParser;

	//Verificar o estado antes da operacao
	EXPECT_FALSE(argParser.IsFlagRegistered("MyFlag"));

	//Registrar a flag
	//vamos chamar a funcao que esta sendo testada
	argParser.RegisterFlag("MyFlag");

	//Verificar o estado depois da operacao
	EXPECT_TRUE(argParser.IsFlagRegistered("MyFlag"));
}

//testar registro de flag com espaco no nome
TEST(TestArgumentParser, RegisterFlag_Whitespace)
{
	ArgumentParser argParser;

	argParser.RegisterFlag("My Flag");

	EXPECT_FALSE(argParser.IsFlagRegistered("My Flag"));
}

//testar registro de opcao comum
TEST(TestArgumentParser, RegisterOption)
{
	ArgumentParser argParser;

	EXPECT_FALSE(argParser.IsOptionRegistered("MyOption"));

	argParser.RegisterOption("MyOption");

	EXPECT_TRUE(argParser.IsOptionRegistered("MyOption"));
}

//testar registro de opcao com espaco no nome
TEST(TestArgumentParser, RegisterOption_Whitespace)
{
	ArgumentParser argParser;

	argParser.RegisterOption("My Option");

	EXPECT_FALSE(argParser.IsOptionRegistered("My Option"));
}

//testar o metodo parse()
TEST(TestArgumentParser, Parse)
{
	ArgumentParser argParser;

	//o metodo parse recebe o argv e o argc[], entao vamos criar:
	std::array<const char*, 5> args =
	{
		"TestArgumentParser", //essa é sem dois tracinhos para testar!
		"--flag",
		"--string=AaBbCcDd",
		"--number=42",
		"--float=4.2"
	};

	//agora os mesmos argumentos que eu coloquei nesse array, vou registrar:
	argParser.RegisterFlag("flag");
	argParser.RegisterOption("string");
	argParser.RegisterOption("number");
	argParser.RegisterOption("float");

	//chamar a funcao Parse() com o tamanho do array e um pontei que aponta pro primeiro elemento do array (data())
	argParser.Parse(args.size(), args.data());

	//vamos verificar os resultados dessa aplicao
	EXPECT_FALSE(argParser.GetFlag("TestArgumentParser"));
	EXPECT_TRUE(argParser.GetFlag("flag"));
	EXPECT_EQ(argParser.GetOptionAs<std::string>("string"), "aabbccdd"); //EQ = EQUAL, eu vou comparar duas strings, é minusculo pois eu usei o ToLower na funcao()
	EXPECT_EQ(argParser.GetOptionAs<int>("number"), 42); //perceba que eu uso o EQ para comparar ints tambem
	EXPECT_EQ(argParser.GetOptionAs<float>("float"), 4.2f); //perceba que eu uso o EQ para comparar floats tambem
}

//testar o parse() com valores inválidos
TEST(TestArgumentParser, Parse_InvalidArguments)
{
	ArgumentParser argParser;

	argParser.Parse(0, nullptr);
	argParser.Parse(42, nullptr);
	argParser.Parse(-1, nullptr);
}

*/