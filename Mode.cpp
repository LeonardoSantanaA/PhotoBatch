#include "Mode.h"
#include "ArgumentParser.h"

#include "RenameMode.h"
#include "ConvertMode.h"
#include "ResizeMode.h"
#include "ScaleMode.h"

#include <iostream>
#include <array>
#include <vector>
#include <filesystem> //para trabalhar com o sistema de arquivos do computador
#include <chrono> //para conseguirmos ver o tempo de execucao

Mode::Mode(const std::string& filter, const std::string& folder)
	: m_Filter{ filter }, m_Folder{ folder } //é o mesmo que fazer "m_Filter = filter, m_Folder = folder, porem assim é mais eficiente
{
	
}

const std::string& Mode::GetFilter() const
{
	return m_Filter;
}

const std::string& Mode::GetFolder() const
{
	return m_Folder;
}

void Mode::Run()
{
	//Vamos medir quanto tempo a operacao levou
	using ClockT = std::chrono::high_resolution_clock; //vou usar Clock ao inves desse codigo gigante

	//a ideia é ler o tempo neste ponto:
	ClockT::time_point startTime = ClockT::now();

	RunImpl(); //essa é a funcao original que vai realmente rodar tudo
	//ler novamente o tempo neste ponto e calcular a diferenca
	ClockT::time_point endTime = ClockT::now();

	ClockT::duration elapsedTime = endTime - startTime; //tempo gasto (em nanossegundos), precisamos converter, vou converter pra milissegundos:
	std::chrono::milliseconds elapsedTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime);

	std::cout << GetModeName() << "Operacao finalizada em " << elapsedTimeMs.count() << "ms" << std::endl;
}

std::vector<std::filesystem::path> Mode::GetFiles(const std::filesystem::path& extension) const
{
	//vou guardar todos os arquivos que tenho que renomear em um vetor
	std::vector<std::filesystem::path> files; //é um vetor de caminhos (path)
	int numSkippedFiles = 0; //guarda o numero de arquivos que encontramos mas nao bate com o filtro

	//coletar todos os arquivos que correspondem ao filtro especificado
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(GetFolder())) //percorre todos os arquivos do folder
	{
		const bool bIsFile = std::filesystem::is_regular_file(entry.path()); //verifica se é um arquivo valido
		const bool bMatchFilter = GetFilter().empty() || (entry.path().string().find(GetFilter()) != std::string::npos); //verifica se o filtro é vazio ou se existe o filtro no arquivo
		const bool bMatchExtension = extension.empty() || (entry.path().extension() == extension);

		if (bIsFile && bMatchFilter && bMatchExtension) //se o arquivo é valido e o filtro existe no arquivo
		{
			files.push_back(entry.path()); //vou armazenar no vetor
		}
		else
		{
			numSkippedFiles++; //se nao, vou contar mais um aqui
		}
	}
	std::cout << GetModeName() << "Numero de arquivos encontrados: " << files.size() << std::endl;
	std::cout << GetModeName() << "Numero de arquivos ignorados: " << numSkippedFiles << std::endl;

	return files;
}

//pega os formatos suportados pelo programa
const std::string& GetFormats(std::vector<std::string> formats)
{
	std::string sFormats = "";

	for (int i = 0; i < formats.size(); i++)
	{
		sFormats += formats[i];
		if (i != formats.size() - 1)
		{
			sFormats += ", ";
		}
		else {
			sFormats += ".";
		}

	}
	static const std::string str = sFormats;
	return str;
}

//Pega os caracteres invalidos
const std::string& GetInvalidChars()
{
	static const std::string invalidCharacters = "\\/\"*?<>|:"; //vou criar uma string que possui todos os caracteres invalidos de um arquivo, ela é static pois quando eu chamar a funcao mais de uma vez e ela nao vai me retornar uma copia
	return invalidCharacters;
}

//Valida se existe caracteres invalidos numa string
bool HasInvalidChars(const std::string& str)
{
	const bool bHasInvalidChars = str.find_first_of(GetInvalidChars()) != std::string::npos; //procura e retorna a posicao do primeiro caractere que tem ocorrencia, se for diferente de npos eu achei um caractere invalido
	return bHasInvalidChars;
}

std::unique_ptr<Mode> CreateMode(const ArgumentParser& argParser)
{
	//Ler as flagas que o argumentParser identificou
	const bool bHelpMode = argParser.GetFlag(Args::Flags::Help);
	bool bRenameMode = argParser.GetFlag(Args::Flags::Rename);
	bool bConvertMode = argParser.GetFlag(Args::Flags::Convert);
	bool bResizeMode = argParser.GetFlag(Args::Flags::Resize);
	bool bScaleMode = argParser.GetFlag(Args::Flags::Scale);

	//Verificar se somente um modo do PhotoBatch está ativo 
	/* Para isso vamos usar o OU EXCLUSIVO, que só é verdadeiro se uma das opcoes for verdadeira:
	* 1 ^ 1 = 0
	* 1 ^ 0 = 1
	* 0 ^ 1 = 1
	* 0 ^ 0 = 0
	* if (!(bRenameMode ^ bConvertMode ^ bResizeMode ^ bScaleMode)) //se houver mais de um modo ativo, vamos lancar uma excecao
	*
	* Voce pode usar um algoritmo para issa tambem, o algoritmo count(), que conta quantas vezes um valor se repetiu em um determinado espaco
	* para isso, primeiro precisamos criar um array que vamos armazenar nossas constantes booleanas:
	*/

	const std::array<bool, 5> modes = { bHelpMode, bRenameMode, bConvertMode, bResizeMode, bScaleMode }; //array<tipo, tamanho>, vou deixar ele como const porque nao quero que ele mude
	const std::ptrdiff_t numActiveModes = std::count(std::begin(modes), std::end(modes), true); //vou criar uma constante do tipo ptrdiff_t(o tipo inteiro assinado do resultado da subtração de dois ponteiros)
	//esse é o tipo de retorno da funcao count(inicio, fim, valor que quero procurar e contar), entao estou
		//quantos valores true tenho no meu array

	if (numActiveModes != 1 && !bHelpMode) //se for diferente de 1, vou lancar uma excecao
	{
		throw std::invalid_argument("É possível selecionar apenas um modo de cada vez!");
	}

	//Validar a pasta passada como a opcao Folder
	const std::string folder = argParser.GetOptionAs<std::string>(Args::Opts::Folder); //vou definir como const porque nao vou alterar a variavel, é uma boa pratica marcar como const se vou souber que nao vai mudar
	if (folder.empty() && !bHelpMode) //se a string estiver vazia, vou jogar uma excecao
	{
		throw std::invalid_argument("A pasta não pode estar em branco!");
	}

	//Verificar se a pasta existe no computador, para isso vamos usar a biblioteca filesystem, esta é uma adicao relativamente nova, do C++17, por isso esta especificado a versao c++17 no CMakeLists.txt
	if (!std::filesystem::exists(folder) && !bHelpMode) //se a pasta nao existir
	{
		throw std::invalid_argument("O caminho do arquivo passado não existe!");
	}

	//Validar se o filtro é uma string válida
	const std::string filter = argParser.GetOptionAs<std::string>(Args::Opts::Filter);
	if (!filter.empty() && HasInvalidChars(filter) && !bHelpMode)
	{
		throw std::invalid_argument("O filtro nao pode conter " + GetInvalidChars());
	}

	//Validar o modo Help
	if (bHelpMode)
	{
		//Desabilitar todos os outros, pois se aparecer --help nos argumentos precisa mostrar essa mensagem
		bResizeMode = false;
		bRenameMode = false;
		bScaleMode = false;
		bConvertMode = false;

		std::cout << "[USO]: PhotoBatch --[Rename|Scale|Convert|Resize] [Opcoes]\n\n\n";



		std::cout << "Modos:			Opcoes:\n\n";

		std::cout << "----------------------------------------------\n";
		std::cout << "--Rename		--Folder=\"CaminhoDoArquivo\"\n";
		std::cout << "			--Filter=\".tipo\"\n";
		std::cout << "			--Prefix=\"NomePrefixo_\"\n";
		std::cout << "			--StartNumber=n\n";
		std::cout << "----------------------------------------------\n";
		std::cout << "--Resize		--Folder=\"CaminhoDoArquivo\"\n";
		std::cout << "			--Filter=\".tipo\"\n";
		std::cout << "			--Width=n\n";
		std::cout << "			--Height=n\n";
		std::cout << "----------------------------------------------\n";
		std::cout << "--Convert		--Folder=\"CaminhoDoArquivo\"\n";
		std::cout << "			--Filter=\".tipo\"\n";
		std::cout << "			--From=TipoArquivo\n";
		std::cout << "			--To=TipoArquivo\n";
		std::cout << "----------------------------------------------\n";
		std::cout << "--Scale			--Folder=\"CaminhoDoArquivo\"\n";
		std::cout << "			--Filter=\".tipo\"\n";
		std::cout << "			--Amount=n\n";
		std::cout << "----------------------------------------------\n\n\n";



		std::cout << "Modos:\n";
		std::cout << "----------------------------------------------\n";
		std::cout << "Rename: renomear arquivos\n";
		std::cout << "Resize: redimensionar arquivos\n";
		std::cout << "Convert: converter arquivos de um formato para outro\n";
		std::cout << "		OBS: os tipos suportados irão aparecer quando utilizar este modo\n";
		std::cout << "		OBS: jpg e jpeg é o mesmo tipo de arquivo\n";
		std::cout << "Scale: mudar escala de uma imagem\n";
		std::cout << "----------------------------------------------\n\n\n";



		std::cout << "Opcoes:\n";
		std::cout << "----------------------------------------------\n";
		std::cout << "Folder: especificar o caminho das fotos\n";
		std::cout << "Filter: filtrar o tipo de arquivo que vai modificar\n";
		std::cout << "Prefix: prefixo que vai ser usado para renomear as imagens\n";
		std::cout << "StartNumber: numero de partida para renomear as imagens\n";
		std::cout << "Width: nova largura da imagem\n";
		std::cout << "Height: nova altura da imagem\n";
		std::cout << "From: tipo inicial da imagem\n";
		std::cout << "To: tipo final da imagem\n";
		std::cout << "Amount: valor que sera multiplicado a escala original da imagem\n";
		std::cout << "----------------------------------------------\n\n\n";
	}

	//Validar o modo Resize
	if (bResizeMode)
	{
		int width = 0;
		int height = 0;
		//vou lancar um try catch para o caso do usuario lancar uma letra no width ou height
		try
		{
			width = argParser.GetOptionAs<int>(Args::Opts::Width);
			height = argParser.GetOptionAs<int>(Args::Opts::Height);
		}
		catch (const std::invalid_argument&) //perceba que eu nao coloquei nome, isso pois nao estou usando nenhuma funcao membro entao nao precisa
		{
			throw std::invalid_argument("O valor informado para Width ou Height não são números válidos!"); //eu vou jogar o throw dentro do catch para reescrever a mensagem de erro
		}


		//No modo resize o comprimento e a altura devem ser > 0
		if (width <= 0 || height <= 0)
		{
			throw std::invalid_argument("Width e Height devem ser maiores que zero!");
		}

		if (filter.empty())
		{
			throw std::invalid_argument("O filtro não pode estar em branco no modo Resize!");
		}

		return std::make_unique<ResizeMode>(filter, folder, width, height);
	}

	//Validar o modo Scale
	if (bScaleMode)
	{
		float amount = 0.1f;
		try {
			amount = argParser.GetOptionAs<float>(Args::Opts::Amount);
		}
		catch (const std::invalid_argument&)
		{
			throw std::invalid_argument("O valor informado para Amount não é um número válido!");
		}
		//No modo escala o amount deve ser maior do que zero
		if (amount <= 0.0f)
		{
			throw std::invalid_argument("Amount deve ser maior que zero!");
		}
		if (filter.empty())
		{
			throw std::invalid_argument("O filtro não pode estar em branco no modo Escale!");
		}

		return std::make_unique<ScaleMode>(filter, folder, amount);

	}

	//Validar o modo Rename
	if (bRenameMode)
	{
		int startNumber = 0;
		try
		{
			startNumber = argParser.GetOptionAs<int>(Args::Opts::StartNumber);
		}
		catch (const std::invalid_argument&)
		{
			throw std::invalid_argument("O valor informado para StartNumber não é um número válido!");
		}

		std::string prefix = argParser.GetOptionAs<std::string>(Args::Opts::Prefix);

		if (startNumber < 0)
		{
			throw std::invalid_argument("StartNumber deve ser maior ou igual a zero!");
		}

		if (prefix.empty() || HasInvalidChars(prefix))
		{
			throw std::invalid_argument("Prefix não pode estar em branco e não pode conter os caracteres " + GetInvalidChars());
		}

		return std::make_unique<RenameMode>(filter, folder, prefix, startNumber); //retorna uma unica instancia da classe RenameMode, e ela é a reponsavel por executar as funcoes
	}

	//Validar o modo Convert
	if (bConvertMode)
	{
		const std::string from = argParser.GetOptionAs<std::string>(Args::Opts::From);
		const std::string to = argParser.GetOptionAs<std::string>(Args::Opts::To);
		const std::vector<std::string> convertOptions = { "jpg", "png", "jpeg"}; //este array armazena os tipos de dados que eu posso converter

		const bool bIsFromValid = std::find(std::begin(convertOptions), std::end(convertOptions), from) != std::end(convertOptions); //o find vai procurar no array se o conteudo do from está la
		const bool bIsToValid = std::find(std::begin(convertOptions), std::end(convertOptions), to) != std::end(convertOptions); //o find vai procurar no array se o conteudo do to está la
		if (from.empty() || to.empty())
		{
			throw std::invalid_argument("Você precisa declarar From e To com algum dos seguintes formatos: " + GetFormats(convertOptions));
		}

		if (!bIsFromValid || !bIsToValid) //se algum deles nao for valido
		{
			throw std::invalid_argument("From e To devem ser algum dos seguintes formatos: " + GetFormats(convertOptions));
		}

		if (from == to)
		{
			throw std::invalid_argument("From e To devem ser de formatos diferentes!");
		}

		const std::map<std::string, ConvertMode::Format> convertOptionsMap = { //vou criar um map para guardar o nome dos arquivos que quero receber para conseguir converter para o enum
			{"jpg", ConvertMode::Format::JPG}, //{chave, valor}
			{"png", ConvertMode::Format::PNG},
			{"jpeg", ConvertMode::Format::JPEG}
		};

		return std::make_unique<ConvertMode>(filter, folder, convertOptionsMap.at(from), convertOptionsMap.at(to));
	}

	return nullptr;
}