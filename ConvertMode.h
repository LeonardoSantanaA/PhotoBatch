#pragma once

#include "Mode.h"

class ConvertMode final : public Mode //ela é final pois nao vai ter mais herdeiros a partir dela, declarar como final ajuda o compilador a otimizar algumas coisas
{

protected:
	void RunImpl() override;

public:
	enum class Format //esse enum possui os formatos que são suportados
	{
		JPG,
		PNG,
		JPEG
	};

	ConvertMode(const std::string& filter, const std::string& folder, Format fromFormat, Format toFormat);

	const std::string& GetModeName() const override;

private: //o private fica embaixo porque eu estou declarando o enum format no public que precisa ficar acima
	Format m_FromFormat;
	Format m_ToFormat;

};