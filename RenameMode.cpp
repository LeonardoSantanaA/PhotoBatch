#include "RenameMode.h"

#include <iostream>
#include <filesystem>
#include <vector>

//Classe Rename
RenameMode::RenameMode(const std::string& filter, const std::string& folder, const std::string& prefix, int startNumber)
	: Mode{ filter, folder }// com isso eu chamo o construtor da classe mae, que guarda os valores de filter e folder
	, m_Prefix{ prefix }, m_StartNumber{ startNumber }
{
}

const std::string& RenameMode::GetModeName() const
{
	static const std::string RenameModeName = "[Renomear]: "; //é estatico para retornar sempre a mesma instancia
	return RenameModeName;
}

void RenameMode::RunImpl()
{
	std::cout << GetModeName() << "Modo		  : Renomear" << std::endl;
	std::cout << GetModeName() << "Pasta		  : " << GetFolder() << std::endl;
	std::cout << GetModeName() << "Filtro		  : " << GetFilter() << std::endl;
	std::cout << GetModeName() << "Prefixo		  : " << m_Prefix << std::endl;
	std::cout << GetModeName() << "Número inicial	  : " << m_StartNumber << std::endl;

	//Iniciar o processo de renomeamento
	//"ExemploFoto.png" -> "PrefixoN.png"
	int currentNumber = m_StartNumber;

	for (const std::filesystem::path& filepath : GetFiles()) //vai percorrer por todos os arquivos que 
	{
		const std::filesystem::path extension = filepath.extension(); //retorna o formato da imagem
		const std::string newFileName = m_Prefix + std::to_string(currentNumber) + extension.string(); //novo nome do arquivo
		std::cout << GetModeName() << filepath << std::endl;
		std::filesystem::path newFilePath = filepath; //atualizar o caminho com os nomes
		newFilePath.replace_filename(newFileName); //troca o nome do arquivo do diretorio de entrada

		try
		{
			std::filesystem::rename(filepath, newFilePath); //tente renomear

			std::cout << GetModeName() << filepath << " -> " << newFilePath << std::endl; //imprime como será o caminho dos novos arquivos

		}
		catch(const std::exception& exception)
		{
			std::cout << GetModeName() << "Erro ao renomear \"" << filepath << "\": " << exception.what() << std::endl;
		}

		currentNumber++;
	}
}