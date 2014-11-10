#ifdef WIN32
	#define CLEAR (system("cls"))
	#define PAUSE (system("pause"))
	#define INC_STR 0
#else
	#define CLEAR (system("clear"))
	#define PAUSE (system("read -p \"\nPressione enter para continuar\" saindo"))
	#define INC_STR 1
#endif

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;

void imprime_dicionario();
void inicializa_jogo();
void imprime_possiveis_palavras(vector<string>);
void imprime_letras(vector<char>);
vector<string> possiveis_palavras(int);
void atualiza_palavras(string, vector<string> &, int);
char adivinha_letra(vector<string>, vector<char>, int);

void imprime_profundidade(char[], int);
char busca_profundidade(char[], int, vector<char>);
char busca_largura(char[], vector<char>);
void imprime_largura(char[]);
void salva_dicionario(string);

bool letra_selecionada(char, vector<char>);

bool letra_selecionada(char letra, vector<char> letras) {
    for(int i = 0; i < int(letras.size()); i++) {
        if(letra == letras[i]) {
            return true;
        }
    }

    return false;
}

int main ()
{
    int opcao_inicio = 0;

    while(opcao_inicio != 2) {
        // INICIO DO JOGO
        cout << "\n\n***** JOGO DA FORCA *****" << endl;
        cout << "\nDESEJA JOGAR?" << "\n1) SIM\n2) NAO" << endl;
        cout << "\nOPCAO: ";
        cin >> opcao_inicio;

        switch(opcao_inicio) {
            case 1: inicializa_jogo();
                    break;
            case 2: break;
            default: cout << "\nOPCAO INVALIDA!!\n\n";
        }
    }

	return 0;
}

void imprime_profundidade(char* letras, int i)
{
    if(i > 26) return;
    cout << letras[i-1] << " ";
    imprime_profundidade(letras, i*2);
    imprime_profundidade(letras, i*2 + 1);
}

char busca_profundidade(char* letras, int i, vector<char> letras_selecionadas)
{
    if(i > 26) return '\0';
    if(letra_selecionada(letras[i-1], letras_selecionadas) == false) {
        return letras[i-1];
    }
    char c = busca_profundidade(letras, i*2, letras_selecionadas);
    if(c != '\0') {
        return c;
    }
    return busca_profundidade(letras, i*2 + 1, letras_selecionadas);
}

char busca_largura(char* letras, vector<char> letras_selecionadas)
{
    int i = 0;
    for(i = 0; i < 26; i++) {
        if(letra_selecionada(letras[i], letras_selecionadas) == false) {
            break;
        }
    }

    return letras[i];
}

void imprime_largura(char* letras)
{
    cout << "\nBUSCA EM LARGURA:\n";
    for(int i = 0; i < 26; i++) {
        cout << letras[i] << " ";
    }
}

void inicializa_jogo()
{
    // VARIAVEIS
    string PALAVRA; // PALAVRA A SER ADIVINHADA
    string TENTATIVA; // ARMAZENA O PROGRESSO DAS LETRAS SELECIONADAS
    vector<string> palavras; // LISTA DE POSSIVEIS PALAVRAS DO DICIONARIO
    char letra_selecionada; // LETRA DA RODADA
    vector<char> letras; // LETRAS QUE JA FORAM SELECIONADAS
    int tamanho = 0;
    int numero_tentativas = 6;
    int opcao_metodo = 0;

    // ORDEM PADRAO DO ALFABETO, APROPRIADA PARA ESSE TIPO DE BUSCA
    char letras_profundidade[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                        'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    // ORDEM MODIFICADA DO ALFABETO, APROPRIADA PARA ESSE TIPO DE BUSCA
    // CRITERIO: INCIDENCIA DAS LETRAS NO ALFABETO
    // QUANTO MAIOR A INCIDENCIA, MAIS NO TOPO ESTA A LETRA
    char letras_largura[] = {'A', 'O', 'E', 'I', 'R', 'T', 'N', 'C', 'S', 'D', 'L', 'M', 'U', 'P', 'G', 'B', 'V', 'F', 'H',
                            'Z', 'Q', 'X', 'J', 'Y', 'K', 'W'};

    cout << "\nDigite uma palavra, por favor: ";
    cin >> PALAVRA;
    tamanho = int(PALAVRA.length());

    for(int i = 0; i < tamanho; i++) {
        PALAVRA[i] = toupper(PALAVRA[i]);
    }

    TENTATIVA.replace(0, tamanho, tamanho, '_');

    //imprime_dicionario();

    palavras = possiveis_palavras(tamanho);

	int teste = int(palavras.size());

	cout << "\n\nPALAVRAS SIZE: " << teste << endl;

    while(numero_tentativas > 0 && PALAVRA.compare(TENTATIVA) != 0) {

        if(int(palavras.size()) > 0) {
            int opcao_possiveis_palavras = 0;
            while(opcao_possiveis_palavras != 1 && opcao_possiveis_palavras != 2) {
                CLEAR;
                cout << "\n\nGostaria de ver as possiveis palavras do dicionario?\n\n";
                cout << "1) Sim\n2) Nao\n\nOPCAO: ";
                cin >> opcao_possiveis_palavras;
                switch(opcao_possiveis_palavras) {
                    case 1: imprime_possiveis_palavras(palavras);
                            PAUSE;
                        break;
                    case 2:
                        break;
                    default: cout << "\n\nOpcao invalida!!\n\n";
                             PAUSE;
                }
            }
        }

        while(opcao_metodo != 1 && opcao_metodo != 2 && opcao_metodo != 3) {
            cout << "\n\nQUAL METODO DEVE SER USADO?\n\n1) Dicionario\n2) Busca em Profundidade\n3) Busca em Largura\n\nOPCAO: ";
            cin >> opcao_metodo;
            switch(opcao_metodo) {
                case 1: letra_selecionada = adivinha_letra(palavras, letras, tamanho);
                    break;
                case 2: letra_selecionada = busca_profundidade(letras_profundidade, 1, letras);
                    break;
                case 3: letra_selecionada = busca_largura(letras_largura, letras);
                    break;
                default: cout << "\n\nOPCAO INVALIDA!!\n\n";
            }
        }

        opcao_metodo = 0;

        cout << "\nLETRA SELECIONADA: " << letra_selecionada << endl;
        letras.push_back(letra_selecionada);

        PAUSE;

        // PREENCHE A ALTERNATIVA COM A LETRA SELECIONADA, CASO ELA EXISTA NA PALAVRA INICIAL
        for(int i = 0; i < tamanho; i++) {
            if(letra_selecionada == PALAVRA[i]) {
                TENTATIVA[i] = letra_selecionada;
            }
        }

        if(PALAVRA.find(letra_selecionada) == string::npos) {
            numero_tentativas--;
        }

        cout << "\nRESULTADO ATUAL: ";
        for(int i = 0; i < tamanho; i++) {
            cout << TENTATIVA[i] << " ";
        }
        cout << endl;

        switch(numero_tentativas) {
            case 6: cout << " ______\n|\n|\n|\n|\n|\n|\n\n * POR ENQUANTO, ESTA TUDO BEM! *" << endl;
                break;
            case 5: cout << " ______\n|      0\n|      |\n|      |\n|      |\n|\n|\n\n* UUUHHH, COMECOU A FICAR PERIGOSO! *\n\n * (SERIA LEGAL APRENDER ARTE EM ASCII...) *" << endl;
                break;
            case 4: cout << " ______\n|      0\n|      |\n|     /|\n|    / |\n|\n|\n\n* OH, PEGARAM O SEU BRACO! *" << endl;
                break;
            case 3: cout << " ______\n|      0\n|      |\n|     /|\\\n|    / | \\\n|\n|\n\n* CONCENTRE-SE MAIS! *" << endl;
                break;
            case 2: cout << " ______\n|      0\n|      |\n|     /|\\\n|    / | \\\n|     /\n|    / \n\n* PEGARAM SUA PERNA DIREITA! *\n\n* (SUPONDO QUE VOCE ESTEJA DE FRENTE!) *" << endl;
                break;
            case 1: cout << " ______\n|      0\n|      |\n|     /|\\\n|    / | \\\n|     / \\\n|    /   \\\n\n* MAIS UM ERRO E VOCE MORRE! *" << endl;
        }

        atualiza_palavras(TENTATIVA, palavras, tamanho);

        PAUSE;
	}

    if(numero_tentativas == 0) {
        int opcao_dic = 0;
        cout << " ______\n|      0\n|      _\n|\n|     /|\\\n|    / | \\\n|     / \\\n|    /   \\\n\n* MORREU ENFORCADO! *" << endl;
        cout << "\n\n ** MAIS SORTE NA PROXIMA. **\n\n ** ALIAS, VA LER ALGUNS LIVROS E **\n\n** AUMENTAR O SEU VOCABULARIO! **" << endl;

        while(opcao_dic != 1 && opcao_dic != 2) {
            cout << "\n\nDESEJA APRENDER ESSA PALAVRA? (sera acoplada no dicionario)\n1) Sim\n2) Nao\n\nOPCAO: ";
            cin >> opcao_dic;
            switch(opcao_dic) {
                case 1: salva_dicionario(PALAVRA);
                    break;
                case 2:
                    break;
                default: cout << "\n\nOPCAO INVALIDA!\n";
            }
        }
    } else {
        cout << "\n\n** VOCE VENCEU, PARABENS! **\n\n";
        cout << "\n\nNUMERO DE TENTATIVAS SOBRANDO: " << numero_tentativas << endl;
    }

    PAUSE;
}

void salva_dicionario(string palavra) {
    std::ofstream outfile;
    outfile.open("dicionario.txt", std::ios_base::app);
    outfile << palavra;
    outfile.close();
    cout << "\nPALAVRA ANEXADA COM SUCESSO!\n";
}


void atualiza_palavras(string TENTATIVA, vector<string> &palavras, int tamanho)
{
    vector<string> novasPalavras;

    bool remove = false;

    char current_character;

    for(int i = 0; i < int(palavras.size()); i++) {
        for(int j = 0; j < tamanho; j++) {
            if(TENTATIVA[j] != '_') {
                if(palavras[i][j] != TENTATIVA[j]) {
                    remove = true;
                    break;
                } else {
                    current_character = TENTATIVA[j];
                    for(int k = 0; k < tamanho; k++) {
                        if(current_character == palavras[i][k] && current_character != TENTATIVA[k]) {
                            remove = true;
                            break;
                        }
                    }
                }
            }
            if(remove == true) break;
        }

        if(remove == false) {
            novasPalavras.push_back(palavras[i]);
        } else {
            remove = false;
        }
    }

    palavras.clear();

    for(int i = 0; i < int(novasPalavras.size()); i++) {
        palavras.push_back(novasPalavras[i]);
    }

	// IMPRIMIR PALAVRAS AQUI
}

void imprime_letras(vector<char> letras)
{
    int tamanho = int(letras.size());
    for(int i = 0; i < tamanho; i++) {
        cout << letras[i] << endl;
    }
}

char adivinha_letra(vector<string> palavras, vector<char> letras_selecionadas, int tamanho_palavra)
{
    int opcao_adivinhacao = 0;

    char letras[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                        'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    int probabilidades[26] = {0};
    int maior_prob = 0;
    int pos_prob = 0;

    char LETRA;

    fill(probabilidades, probabilidades + 26, 0);

    // CONVERTE AS LETRAS SELECIONADAS PARA A FORMA MAIUSCULA
    for(int i = 0; i < int(letras_selecionadas.size()); i++) {
        letras_selecionadas[i] = toupper(letras_selecionadas[i]);
    }

    // MARCA AS LETRAS SELECIONADAS NO VETOR DE PROBABILIDADES COM - 1
    for(int i = 0; i < int(letras_selecionadas.size()); i++) {
        probabilidades[int(letras_selecionadas[i]) - 65] = -1;
    }

    int quantidade_palavras = int(palavras.size());

    // BUSCA PELO DICIONARIO
    if(quantidade_palavras > 0) {
        vector<char> alfabeto(letras, letras + sizeof(letras) / sizeof(char));

        for(int i = 0; i < quantidade_palavras; i++) {
            for(int j = 0; j < palavras[i][j]; j++) {
                //cout << palavras[i][j] << endl;
                int prob = int(palavras[i][j]) - 65;
                if(probabilidades[prob] != -1) {
                    probabilidades[prob]++;
                }
            }
        }

        while(opcao_adivinhacao != 2 && opcao_adivinhacao != 1) {
            cout << "\n\nDeseja ver a incidencia das letras (com uso do dicionario)?\n\n1)Sim\n2)Nao" << endl;
            cout << "\nOPCAO: ";
            cin >> opcao_adivinhacao;
            switch(opcao_adivinhacao) {
                case 1: cout << "INCIDENCIA DAS LETRAS (probabilidades):" << endl;
                        for(int i = 0; i < 26; i++) {
                            cout << char(i+65) << " = " << probabilidades[i] << endl;
                        }
                    break;
                case 2:
                    break;
                default: cout << "\n\nOpcao invalida!\n\n";
            }
        }

	opcao_adivinhacao = 0;

        for(int i = 0; i < 26; i++) {
            if(probabilidades[i] > maior_prob) {
                maior_prob = probabilidades[i];
                pos_prob = i;
            }
        }

        LETRA = char(pos_prob + 65);
    } else {
        // BUSCA CEGA
        // SELECIONA UMA LETRA RANDOMICA, QUE AINDA NAO FOI SELECIONADA, CASO NAO EXISTAM MAIS
        // PALAVRAS POSSIVEIS NO DICIONARIO
        for(int i = 0; i < 26; i++) {
            if(probabilidades[i] != -1) {
                LETRA = letras[i];
            }
        }
    }

    PAUSE;

    return LETRA;
}

void imprime_possiveis_palavras(vector<string> palavras)
{
    cout << "\nPOSSIVEIS PALAVRAS (dicionario):\n" << endl;
    int tamanho = int(palavras.size());
    for(int i = 0; i < tamanho; i++) {
        cout << palavras[i] << endl;
    }
    cout << endl;
}

vector<string> possiveis_palavras(int tamanho)
{
	string STRING;
	vector<string> palavras;
	ifstream infile;
	infile.open ("dicionario.txt");
	while(!infile.eof()) // To get you all the lines.
	{
		getline(infile,STRING); // Saves the line in STRING.
		if(int(STRING.length()) == tamanho+INC_STR) {
		    palavras.push_back(STRING);
		}
	}
	infile.close();

	return palavras;
}

void imprime_dicionario()
{
    string STRING;
	ifstream infile;
	infile.open ("dicionario.txt");
    while(!infile.eof()) // To get you all the lines.
    {
        getline(infile,STRING); // Saves the line in STRING.
        cout<<STRING << endl; // Prints our STRING.
    }
	infile.close();
	PAUSE;
}
