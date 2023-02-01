#pragma once

#include <string>
#include <memory>
#include <filesystem>
#include <vector>

class Mode
{
private:
	std::string m_Filter;
	std::string m_Folder;

protected:
	virtual void RunImpl() = 0;
	std::vector<std::filesystem::path> GetFiles(const std::filesystem::path& extension = "") const; //essa funcao vai percorrer todos os arquivos que batem com a extensao, perceba que o parametro
																										//recebe um valor, entao se eu nao passar nenhum parametro, esse vai ser o valor padrao

public:
	//construtor
	Mode(const std::string& filter, const std::string& folder);

	//retorna o filter
	const std::string& GetFilter() const;
	
	//retorna o folder
	const std::string& GetFolder() const;

	//retorna o nome do modo
	virtual const std::string& GetModeName() const = 0; //ele é virtual pois muda de classe pra classe, e eu igualei a 0 para obrigar ele a ser implementado na classe base (abstrato)

	//Run() vai ser responsavel por calcular o tempo de execucao do programa, dentro dele ele chama o RunImpl(), este que realmente valida e executa determinado modo, por isso ela é virtual
	void Run();
};



//como estamos trabalhando com strings, para evitar o erro de escrita vamos armazenar dentro de namespaces
namespace Args
{
	namespace Flags
	{
		static constexpr const char* Rename = "rename";
		static constexpr const char* Convert = "convert";
		static constexpr const char* Resize = "resize";
		static constexpr const char* Scale = "scale";
		static constexpr const char* Help = "help";
	}

	namespace Opts
	{
		static constexpr const char* Folder = "folder";
		static constexpr const char* Filter = "filter";
		static constexpr const char* Width = "width";
		static constexpr const char* Height = "height";
		static constexpr const char* Amount = "amount";
		static constexpr const char* Prefix = "prefix";
		static constexpr const char* StartNumber = "startnumber";
		static constexpr const char* From = "from";
		static constexpr const char* To = "to";
	}
}

class ArgumentParser; //vou fazer uma chamada adiantada aqui apenas pra usar na funcao CreateMode()

//funcao responsavel por validar todos os modos
std::unique_ptr<Mode> CreateMode(const ArgumentParser& argParser); //vou usar o unique_ptr pois ele funciona como um ponteiro, mas quando sai do seu escopo ele chama o destrutor, que deleta o ponteiro pra gente
