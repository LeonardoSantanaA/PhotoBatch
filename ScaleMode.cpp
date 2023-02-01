#include "ScaleMode.h"

#include <stb_image.h>

#include <iostream>

ScaleMode::ScaleMode(const std::string& filter, const std::string& folder, float scaleFactor)
	: ResizeMode{ filter, folder },
	m_ScaleFactor{ scaleFactor }
{
}

const std::string& ScaleMode::GetModeName() const
{
	static const std::string ScaleModeName = "[Escalar]: ";
	return ScaleModeName;
}

void ScaleMode::RunImpl()
{
	std::cout << GetModeName() << "Modo        : Escalar" << std::endl;
	std::cout << GetModeName() << "Pasta       : " << GetFolder() << std::endl;
	std::cout << GetModeName() << "Filtro      : " << GetFilter() << std::endl;
	std::cout << GetModeName() << "Fator       : " << m_ScaleFactor << std::endl;

	for (const std::filesystem::path& filepath : GetFiles())
	{
		std::cout << GetModeName() << "Escalando " << filepath << std::endl;

		//pegar o tamanho original da imagem:
		int width = 0;
		int height = 0;
		int numComp = 0; //numero componentes
		stbi_info(filepath.string().c_str(), &width, &height, &numComp);

		const int newWidth = static_cast<int>(width * m_ScaleFactor); //usei um static_cast para o compilador ter certeza que quero converter de float para int
		const int newHeight = static_cast<int>(height * m_ScaleFactor);

		ResizeImage(filepath, newWidth, newHeight);
	}
}