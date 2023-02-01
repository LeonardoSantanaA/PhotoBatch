#include "ConvertMode.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> //esta é uma biblioteca do github para conseguir editar as imagens, ela foi definida para ser baixada automaticamente no cmake
#define STB_IMAGE_WRITE_IMPLEMENTATION //este define é porque ele permite que voce inclua junto com um cabecalho que possui uma assinatura, nos estamos definindo aqui para conseguir compilar
#include <stb_image_write.h>

//funcao que converte um formato do enum para string
std::string toString(ConvertMode::Format format)
{
	switch (format)
	{
	case ConvertMode::Format::JPG:
		return "jpg";

	case ConvertMode::Format::PNG:
		return "png";

	case ConvertMode::Format::JPEG:
		return "jpeg";

	default:
		break;
	}
	return "";
}

//sobrecarga do operador de fluxo (<<) para ele aceitar um tipo enum na hora de printar
std::ostream& operator<< (std::ostream& out, ConvertMode::Format format) 
{
	out << toString(format);
		return out;
}

ConvertMode::ConvertMode(const std::string & filter, const std::string & folder, Format fromFormat, Format toFormat)
	:Mode{ filter, folder },
	m_FromFormat{ fromFormat },
	m_ToFormat { toFormat }
{
}

const std::string& ConvertMode::GetModeName() const
{
	static const std::string ConvertModeName = "[Converter]: ";
	return ConvertModeName;
}

void ConvertMode::RunImpl()
{
	std::cout << GetModeName() << "Modo   : Converter" << std::endl;
	std::cout << GetModeName() << "Pasta  : " << GetFolder()<< std::endl;
	std::cout << GetModeName() << "Filtro : " << GetFilter()<< std::endl;
	std::cout << GetModeName() << "Origem : " << m_FromFormat << std::endl; //a sobrecarga do operador esta sendo usada aqui
	std::cout << GetModeName() << "Destino: " << m_ToFormat << std::endl;
	
	const std::filesystem::path fromExtension = "." + toString(m_FromFormat); //eu preciso adicionar o ponto (.) pois o filesystem nao coloca o ponto pra gente
	//adquirir a lista de arquivos que precisamos converter
	for (const std::filesystem::path& filepath : GetFiles(fromExtension))
	{
		std::cout << GetModeName() << "Convertendo " << filepath << std::endl;

		//para usar a biblioteca sdb, precisamos das seguintes variaveis:
		int width = 0;
		int height = 0;
		int numComp = 0; //numero de componentes (rgb, rgba etc)
		const int numReqComp = 3; //numero de componentes que eu quero, caso é 3 para nao ter a transparecia, apenas o RGB

		if (unsigned char* data = stbi_load(filepath.string().c_str(), &width, &height, &numComp, numReqComp)) //vai ser nulo se ele nao conseguir abrir, e um valor valido se conseguir,
																									//por isso ele ta num if
		{
			std::filesystem::path destFilepath = filepath; //esse vai ser o arquivo de destino para a conversao
			destFilepath.replace_extension(toString(m_ToFormat)); //aqui passamos a extensao que queremos que ele troca

			int writeResult = 0;
			switch (m_ToFormat)
			{
			case Format::PNG:
				//aqui realmente ocorre a conversao
				writeResult = stbi_write_png(destFilepath.string().c_str(), width, height, numReqComp, data, 0); //essa operacao retorna um int, se for 0 é porque deu erro, se der certo é um valor != 0
				break;

			case Format::JPG:
				writeResult = stbi_write_jpg(destFilepath.string().c_str(), width, height, numReqComp, data, 50);
				break;

			case Format::JPEG:
				writeResult = stbi_write_jpg(destFilepath.string().c_str(), width, height, numReqComp, data, 50);
				break;

			default:
				break;
			}

			if (writeResult == 0) 
			{
				std::cout << GetModeName() << "Erro ao converter " << filepath << std::endl;
			}

			stbi_image_free(data); //preciso liberar os dados que nos lemos, se nao ele fica na memoria
		}
		else {
			std::cout << GetModeName() << "Erro ao carregar " << filepath << std::endl;
		}
	}
}