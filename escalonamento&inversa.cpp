#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
#include <time.h>
#include <windows.h>
struct rgAgora{
 int  dia, mes, ano;
 int  h, m, s; 
 };
// https://www.math.tecnico.ulisboa.pt/~calves/cursos/SisLin-Dir.htm
// https://www.mathwords.com/g/gauss-jordan_elimination.htm
// https://www.youtube.com/watch?v=dG7akik1wOU
// http://ganuff.weebly.com/uploads/1/9/2/5/19255685/matrizes_inversas.pdf
// https://www.youtube.com/watch?v=0cZUyOQqfW8
float **criarMatriz(int, int);
float **identidade(int);
void exibirMatriz(float **, int, int);
float **escalonarMatriz(float **, int, int);
float **reduzirMatrizEscalonada(float **, int, int);
float **inversa(float **, int);

int main() {
    struct tm tempo;
    time_t tempoSeg;
    
    time(&tempoSeg);
    tempo = *localtime(&tempoSeg);
    
    struct rgAgora agora;
    
    agora.dia = tempo.tm_mday;
    agora.mes = tempo.tm_mon + 1;
    agora.ano = tempo.tm_year +1900;
    agora.h   = tempo.tm_hour;
    agora.m   = tempo.tm_min;
    agora.s   = tempo.tm_sec;
    
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5 /*COR*/);
    printf("Hoje eh %02d/%02d/%d/ as %02d:%02d:%02d horas.\n",agora.dia, agora.mes, agora.ano, agora.h, agora.m, agora.s);
    
	int n, m, i, j;
	float **matriz;
	float **matrizOriginal;
	float **matrizEscalonada;
	float **matrizReduzida;
	float **matrizInversa;
	
	setlocale(LC_ALL, "Portuguese");
	
	printf("-------- Geometria Analitica e Algebra Linear-----------\n");
	printf("-------- Prof Wagner Monteiro-----------\n");
	
	// Recebe os dados
	printf("Digite as duas dimensoes da matriz (separados por espaco): ");
	scanf("%d %d", &n, &m);
	fflush (stdin);
	matriz = criarMatriz(n, m);
	matrizOriginal = criarMatriz(n, m);
	
	// Inicializa a matriz
	printf("Digite as linhas da matriz, (separando os valores por espaco): \n");
	for (i=0; i<n; i++) {
		for (j=0; j<m; j++) {
			scanf("%f", &matriz[i][j]);
			fflush (stdin);
			matrizOriginal[i][j] = matriz[i][j];
		}
	}
	
	// Exibe a matriz original
	printf("\nMatriz Original\n");
	exibirMatriz(matrizOriginal, n, m);
	
	// Escalonando a matriz...
	printf("Escalonando a matriz... (eliminacao de Gauss)\n\n");
	matrizEscalonada = escalonarMatriz(matriz, n, m);
	printf("Matriz Escalonada\n");
	exibirMatriz(matrizEscalonada, n, m);
	
	// Aplica o método de Gauss_Jordan se possivel
	if (n == m-1) {
		printf("Reduzindo a matriz escalonada... (método de Gauss-Jordan)\n");
		matrizReduzida = reduzirMatrizEscalonada(matrizEscalonada, n, m);
		printf("Matriz Reduzida\n");
		exibirMatriz(matrizReduzida, n, m);
	}
	
	// Tenta encontrar a inversa quando possivel
	else if (n == m) {
		printf("Invertendo Matriz\n");
		matrizInversa = inversa(matrizOriginal, n);
		if (matrizInversa) {
			printf("Matriz Inversa\n");
			exibirMatriz(matrizInversa, n, m);
		} else {
			printf("A matriz e singular (nao possui inversa)...");
		}
	}
	
	return 0;
}

// Aloca uma matriz dinamicamente na memoria
float **criarMatriz(int nLinhas, int nColunas) {
	float **matriz;
    int i;
    
    // alocando as linha da matriz
    matriz = (float **) malloc(nLinhas*sizeof(float *));
    if (matriz==NULL) return NULL;
    
    // alocando as colunas da matriz
    for(i=0; i<nLinhas; i++) {
    	matriz[i] = (float *) malloc(nColunas*sizeof(float));
    	if (matriz[i]==NULL) return NULL;
    }
    return matriz;
}

// Exibe qualquer matriz, dadas suas dimensoes
void exibirMatriz(float **matriz, int n, int m) {
	int i, j;
	for (i=0; i<n; i++) {
		for (j=0; j<m; j++) {
			printf("%.3f ", matriz[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

// Retorna uma matriz identidade, dada sua ordem
float **identidade(int ordem) {
	float **matriz = criarMatriz(ordem, ordem);
	int i, j;
	for (i=0; i<ordem; i++) {
		for (j=0; j<ordem; j++) {
			if (i == j) matriz[i][j] = 1;
			else matriz[i][j] = 0;
			
		}
	}
	return matriz;
}

// Eliminação de Gauss
float **escalonarMatriz(float **matriz, int n, int m) {
	int i, j, pivo; // pivo <=> diagonal
	//int nLinhas = n, nColunas = m;
	float aux, multiplicador;
	
	// Defines os pivos, trocando linhas conforme necessario
	for (pivo=0; pivo<n; pivo++) {
		for (i=pivo; i<n-1; i++) {
			// Se o coeficiente for 0 e o proximo também...
			if (i != 0 && matriz[i][pivo] == 0 && matriz[i-1][pivo]) {
				printf("Invertendo linhas L%d e L%d...\n", i+1, i+2);
				// Troca os valores das linhas
				for (j=pivo; j<m; j++) {
					aux = matriz[i][j];
					matriz[i][j] = matriz[i+1][j];
					matriz[i+1][j] = aux;
				}
				exibirMatriz(matriz, n, m);
			}
		}
	}

	// Escalonamento da matriz
	for (pivo=0; pivo<n-1; pivo++) {
		for (i=pivo; i<n-1; i++) {
			// Se o elemento ja for zero, pula a iteracao, pois nao e esta sendo necessario zera-lo
			if (matriz[i+1][pivo] == 0) continue;
			else if (matriz[pivo][pivo] == 0) {
				printf("Invertendo linhas L%d e L%d...\n", pivo+1, i+2);
				for (j=pivo; j<m; j++) {
					aux = matriz[i][j];
					matriz[i][j] = matriz[i+1][j];
					matriz[i+1][j] = aux;
				}
				exibirMatriz(matriz, n, m);
			}
			
			// Multiplicador do pivo para zerar os os coeficientes de baixo
			multiplicador = -matriz[i+1][pivo]/matriz[pivo][pivo];
			
			// Calcula o valor de cada elemento da linha
			printf("L%d + (L%d * %.3f)\n", i+1, pivo+1, multiplicador);
			for (j=pivo; j<m; j++)
				matriz[i+1][j] = matriz[i+1][j] + matriz[pivo][j] * multiplicador;
				
			exibirMatriz(matriz, n, m);
		}
	}
	
	return matriz;
}

// Metodo de Gauss_Jordan
float **reduzirMatrizEscalonada(float **matriz, int n, int m) {
	int i, j, pivo;
	float multiplicador;
	
	// Zerando os elementos das colunas dos pivos
	printf("\nZerando os elementos das colunas dos pivos...\n");
	for (pivo=1; pivo<n; pivo++) {
		for (i=0; i<pivo; i++) {
			// Se o elemento ja for 0, nao tem por que zerar
			if (matriz[i][pivo] == 0) continue;
			
			// Multiplicador do pivo para zerar os os coeficientes dos elementos da coluna
			multiplicador = -matriz[i][pivo]/matriz[pivo][pivo];
			
			// Em alguns casos o multiplicador é infinito e isso não pode acontecer
			// Caso aconteça, avisa sobre o erro e imediatamente sai da função
			if (isinf(multiplicador)) {
				printf("Erro ao aplicar o método de Gauss-Jordan: multiplicador infinito!\n\n");
				return 0;
			}
			
			// Calcula o valor de cada elemento da linha
			printf("L%d + (L%d * %.3f)\n", i+1, pivo+1, multiplicador);
			for (j=pivo; j<m; j++)
				matriz[i][j] = matriz[i][j] + matriz[pivo][j] * multiplicador;
				
			exibirMatriz(matriz, n, m);
		}
	}
	
	// Divide os elementos das linhas pelo valor do pivo
	printf("Dividindo os elementos das linhas pelo valor do pivo...\n");
	for (pivo=0; pivo<n; pivo++) {
		while (matriz[pivo][pivo] == 0);
		for (j=m-1; j>=pivo; j--)
			matriz[pivo][j] /= matriz[pivo][pivo];
	}
	exibirMatriz(matriz, n, m);
	
	return matriz;
}

// Calcula a inversa da Matriz
float **inversa(float **matriz, int n) {
	float **matrizIdentidade = identidade(n);
	float **novaMatriz = criarMatriz(n, n*2); // junção das duas matrizes
	float **matrizInversa = criarMatriz(n, n);
	int i, j;
	
	// "Junta" a matriz e uma identidade de mesma ordem
	// formando uma matriz com o mesmo numero de linhas e o dobro do numero de colunas
	printf("Aplicar gauss-jordan na seguinte matriz para obter a inversa: \n");
	for (i=0; i<n; i++) {
		for (j=0; j<n*2; j++) {
			if (j<n) novaMatriz[i][j] = matriz[i][j];
			else novaMatriz[i][j] = matrizIdentidade[i][j-n];
		}
	}
	exibirMatriz(novaMatriz, n, n*2);
	
	// Escalona a matriz
	printf("Escalonando a matriz...");
	novaMatriz = escalonarMatriz(novaMatriz, n, n*2);
	exibirMatriz(novaMatriz, n, n*2);
	
	// Tenta aplicar o metodo de Gauss_Jordan
	printf("Aplicando Gauss-Jordan na matriz...");
	novaMatriz = reduzirMatrizEscalonada(novaMatriz, n, n*2);
	
	// Verifica se foi possivel aplicar o metodo de Gauss_Jordan
	if (novaMatriz) exibirMatriz(novaMatriz, n, n*2);
	else return 0;
	
	// Pega somente a parte da direita (da matriz expandida "nova_Matriz")
	// Essa parte corresponde a inversa da matrizOriginal
	for (i=0; i<n; i++) {
		for (j=n; j<n*2; j++) {
			matrizInversa[i][j-n] = novaMatriz[i][j];
		}
	}
		
	return matrizInversa;
	return 0;
}

