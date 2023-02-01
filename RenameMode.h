#pragma once

#include "Mode.h"

class RenameMode : public Mode //cada modo vai ter sua propria classe que vai herdar Mode
{
private:
	std::string m_Prefix;
	int m_StartNumber;

protected:
	void RunImpl() override;

public:
	//Construtor
	RenameMode(const std::string& filter, const std::string& folder, const std::string& prefix, int startNumber);

	const std::string& GetModeName() const override; //esse metodo é uma sobrecarga, "override" nos ajuda a nao errar o nome da funcao, perceba que é mais uma funcao virtual

};