#include "ResizeMode.h"

#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_write.h>

#include <stb_image_resize.h>

#include <iostream>
#include <vector>

ResizeMode::ResizeMode(const std::string& filter, const std::string& folder, int width, int height)
	: Mode{ filter, folder },
	m_Width{ width },
	m_Height{ height }
{
}

const std::string& ResizeMode::GetModeName() const
{
	static const std::string ResizeModeName = "[Redimensionar]: ";
	return ResizeModeName;
}

void ResizeMode::ResizeImage(const std::filesystem::path& filepath, int newWidth, int newHeight) const
{
	//Ler a imagem
	//Redimensionar em memoria
	//escrever a imagem

	int inputWidth = 0;
	int inputHeight = 0;
	int inputNumComp = 0;
	const int numCompReq = 4; //agora pode ter transparencia (RGBA), por isso tem 4

	if (unsigned char* inputData = stbi_load(filepath.string().c_str(), &inputWidth, &inputHeight, &inputNumComp, numCompReq))
	{
		//precisamos calcular quantos pixels de saida a gent precisa
		const int numOuputPixels = newWidth * newHeight * numCompReq;
		//alocar memoria para guardar a nova imagem, em pixels
		std::vector<uint8_t> outputData(numOuputPixels, 0);
		
		const int resizeResult = stbir_resize_uint8(inputData, inputWidth, inputHeight, 0,	//se for igual a 1, deu tudo certo
													outputData.data(), newWidth, newHeight, 0,
													numCompReq);
		if (resizeResult == 1)
		{
			int writeResult = 1;

			//agora vamos escrever os bytes na forma de arquivo
			const std::filesystem::path extension = filepath.extension();
			if (extension == ".jpg")
			{
				writeResult = stbi_write_jpg(filepath.string().c_str(), newWidth, newHeight, numCompReq, outputData.data(), 50);
			}
			else if (extension == ".jpeg")
			{
				writeResult = stbi_write_jpg(filepath.string().c_str(), newWidth, newHeight, numCompReq, outputData.data(), 50);
			}
			else if (extension == ".png")
			{
				writeResult = stbi_write_png(filepath.string().c_str(), newWidth, newHeight, numCompReq, outputData.data(), 0);
			}
			else 
			{
				std::cout << GetModeName() << "Formato não suportado " << filepath << std::endl;
			}

			if (writeResult == 0) //quer dizer que deu erro
			{
				std::cout << GetModeName() << "Erro ao escrever " << filepath << std::endl;
			}
		}
		else 
		{
			std::cout << GetModeName() << "Erro ao redimensionar " << filepath << std::endl;
		}

		stbi_image_free(inputData);
	}
	else {
		std::cout << GetModeName() << "Erro ao carregar " << filepath << std::endl; //eu nao estou usando cerr pois nao quero que pare o processamento
	}

	
}

void ResizeMode::RunImpl()
{
	std::cout << GetModeName() << "Modo        : Redimensionar" << std::endl;
	std::cout << GetModeName() << "Pasta       : " << GetFolder() << std::endl;
	std::cout << GetModeName() << "Filtro      : " << GetFilter() << std::endl;
	std::cout << GetModeName() << "Comprimento : " << m_Width << std::endl;
	std::cout << GetModeName() << "Altura      : " << m_Height << std::endl;

	if (m_Width > 0 && m_Height > 0)
	{
		for (const std::filesystem::path& filepath : GetFiles())
		{
			std::cout << GetModeName() << "Redimensionando " << filepath << std::endl;
			ResizeImage(filepath, m_Width, m_Height);
		}
	}
}