
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TRUE 1;
#define FALSE 0;

struct matriz {
    int numero_elementos;
    int** elementos;
};

struct nodo {
    int indice;
    int valor;
};

//Funções

int calcular_custo(struct matriz, int*);
void ler_arquivo(struct matriz*, char*);
void construir_caminho(struct matriz, int*);
void construir_caminho_aleatorio(struct matriz, int*);
void realizar_troca(struct matriz, int*,int*);
void copiar_caminho(struct matriz, int*,int*);
void imprimir_caminho(int,int*);
void imprimir_matriz(struct matriz);
void linha();

int main(int argc, char *argv[]){
    struct matriz m;
    ler_arquivo(&m,"C:/Users/CaioZero/Documents/FUCAPI/2020.1/Caxeiro-Viajante/txt/4_01.txt");
    imprimir_matriz(m);

    int *solucao_inicial = malloc((m.numero_elementos +1)*sizeof(int));
    construir_caminho(m,solucao_inicial);
    printf("Solucao inicial de estrategia gulosa pela heuristica do vizinho mais proximo: ");
    imprimir_caminho(m.numero_elementos +1 ,solucao_inicial);
    int custo_solucao_inicial = calcular_custo(m,solucao_inicial);
    printf("Custo solucao inicial de estrategia gulosa pela heuristica do vizinho mais proximo: %d",custo_solucao_inicial);

    /*linha();

    int *melhor_vizinho = malloc((m.numero_elementos+1)*sizeof(int));
    realizar_troca(m,solucao_inicial,melhor_vizinho);
    printf("Aplicando solucao do melhor vizinho a SOLUCAO INICIAL de estrategia gulosa pela heuristica do vizinho mais proximo: "); 
    imprimir_caminho(m.numero_elementos +1 ,melhor_vizinho);
    int custo_solucao_melhor_vizinho = calcular_custo(m,melhor_vizinho);
    printf("Custo SOLUCAO INICIAL de estrategia gulosa pela heuristica do vizinho mais proximo: %d",custo_solucao_melhor_vizinho);
    */

    int *melhor_vizinho = malloc((m.numero_elementos+1)*sizeof(int));
    srand(1);
    linha();
    
    /*Gerando um loop para solucoes aleatorias*/
    int *solucao_aleatoria = malloc((m.numero_elementos + 1) * sizeof(int));
    for(int i = 0; i < 3; i++) {
        construir_caminho_aleatorio(m, solucao_aleatoria);
        int custo_solucao_aleatoria = calcular_custo(m, solucao_aleatoria);
        printf("Solucao aleatória: ");
        imprimir_caminho(m.numero_elementos + 1, solucao_aleatoria);
        printf("Custo solução aleatória: %d\n", custo_solucao_aleatoria);
        int *melhor_vizinho_aleatorio = malloc((m.numero_elementos+1)*sizeof(int));
        realizar_troca(m,solucao_aleatoria,melhor_vizinho_aleatorio);
        printf("Aplicando solucao do melhor vizinho a Solucao aleatoria: "); 
        imprimir_caminho(m.numero_elementos +1 ,melhor_vizinho_aleatorio);
        int custo_solucao_melhor_vizinho_aleatorio = calcular_custo(m,melhor_vizinho_aleatorio);
        printf("Custo solucao do melhor vizinho a Solucao aleatoria: %d",custo_solucao_melhor_vizinho_aleatorio);
        linha();
    }
}

/*Lendo um arquivo onde contem 10 elementos com resultado "otimo" de 25 unidades de tempo*/
void ler_arquivo(struct matriz* m, char* arquivo) {
    FILE* fp = fopen(arquivo, "r");/*Lendo o Arquivo*/
    fscanf(fp, "%d\n", &m->numero_elementos);/*Colocando os elementos do txt na struct*/
    m->elementos = malloc(m->numero_elementos * sizeof(int*));/*Inicializando matriz*/
    for(int i = 0; i < m->numero_elementos; i++) {
        m->elementos[i] = malloc(m->numero_elementos * sizeof(int));/*Representando uma linha da matriz*/
        for(int j = 0; j < m->numero_elementos; j++) {
            fscanf(fp, "%d ", &m->elementos[i][j]);
        }
    }
    fclose(fp);
}

/*Calcular o custo, ou seja, soma das arestas do caminho*/
int calcular_custo(struct matriz m, int* caminho){
    int custo = 0;
    for (int i = 0; i < m.numero_elementos; i++)
    {
        custo = custo + m.elementos[caminho[i]][caminho[i+1]];
    }
    return custo;
    
}

//imprime a matriz inicial encontrada no txt
void imprimir_matriz(struct matriz m){
    linha();
    printf("Matriz\n\n");
    for(int i = 0; i < m.numero_elementos; i++) {
        for(int j = 0; j < m.numero_elementos; j++) {
            printf("%d ", m.elementos[i][j]);
        }
        printf("\n");
    }
    linha();
}

//Imprime o caminho percorrido
void imprimir_caminho(int n, int* caminho){
    int i;

    for (i = 0; i < n; i++)
    {
        printf("%d ",caminho[i]);
    }
    printf("\n");
}

//imprime uma linha -------------------------------
void linha() {
    int i;
    printf("\n");
    for(i = 0; i < 125; i++) printf("_");
    printf("\n");
}

/*Construção de uma solução inicial gulosa
Utilizando a heurística do vizinho mais próximo esse bloco de código gera 
uma solução inicial gulosa para o problema. É importante ressaltar a forma 
como a lista de vizinhos inseridos no caminho é gerenciada e também a decisão 
de seleção do vizinho, que é baseada no custo de utilização da aresta.*/
void construir_caminho(struct matriz m, int* caminho) {
    int *inseridos = malloc(m.numero_elementos * sizeof(int)); /*Inicializando Vetor que armazenara itens ja inseridos**/
    for(int i = 0; i < m.numero_elementos; i++) {
        inseridos[i] = FALSE;
    }

    caminho[0] = 0; /*Nodo inicial sendo 0*/
    inseridos[0] = TRUE; /*Dizendo que o nodo inicial ja foi inserido*/

    for(int i = 0; i < m.numero_elementos; i++) {/*Navegando Item a item*/
        int valor_referencia = INT_MAX; /*Nodo de referencia*/
        int vizinho_selecionado = 0;

        for(int j = 0; j < m.numero_elementos; j++) {
            /*Verificar se o nodo foi inserido e se ele eh maior que o nodo de referencia*/
            if(!inseridos[j] && valor_referencia > m.elementos[i][j]) {
                vizinho_selecionado = j;
                valor_referencia = m.elementos[i][j];
            }
        }

        caminho[i+1] = vizinho_selecionado; /*Proximo nodo a ser visitado*/
        inseridos[vizinho_selecionado] = TRUE; /*inserir o nodo que acabei de ir*/
    }

    caminho[m.numero_elementos] = 0;

    free(inseridos);/*Liberar memoria para o vetor de alocacao*/
}

/*Construção de uma solução aleatória
Esse bloco de código gera uma solução inicial aleatória para o problema. 
É importante ressaltar a forma como a lista de vizinhos inseridos no caminho é 
gerenciada e também a decisão de seleção do vizinho, que é baseada no sorteio de um 
elemento disponível na lista de vizinhos possíveis de serem visitados a partir do no*/

void construir_caminho_aleatorio(struct matriz m, int* caminho) {
    int *inseridos = malloc(m.numero_elementos * sizeof(int)); /*Inicializando Vetor que armazenara itens ja inseridos**/
    struct nodo *vizinhos = malloc(m.numero_elementos * sizeof(struct nodo));//*Inicializando estrutura que armazenara os vizinhos**/  
    for(int i = 0; i < m.numero_elementos; i++) {
        inseridos[i] = FALSE;
    }

    caminho[0] = 0; /*Nodo inicial sendo 0*/
    inseridos[0] = TRUE; /*Dizendo que o nodo inicial ja foi inserido*/
    
    for (int i = 0; i < m.numero_elementos; i++){
        int iv = 0; /*Indice dos Vizinhos*/
        for (int j = 0; j < m.numero_elementos; j++){
            /*Verificar se o nodo J ja foi inserido e,
            caso nao tenha sido, colocar em um vetor de possiveis vizinhos
            a serem inseridos*/
            if(!inseridos[j]){
                vizinhos[iv].indice =j;
                vizinhos[iv].valor = m.elementos[i][j];
                iv++;
            }
        }
        /*Verificar se ha vizinhos a serem inseridos.
        Se nao ha, retorna*/
        if (iv==0){
           caminho[i+1]=0;
        }else{
            /*Se ha vizinhos, escolher um valor aleatorio
            entre os indices dos vizinhos e depois escolher um item*/
            int vizinho_selecionado = rand()%iv;
            caminho[i+1]=vizinhos[vizinho_selecionado].indice;
            inseridos[vizinhos[vizinho_selecionado].indice]=TRUE;
        }
    }
    /*Liberando memoria dos vetores alocados*/
    free(vizinhos);
    free(inseridos);
}

/*Copia os dados de um vetor para outro*/
void copiar_caminho(struct matriz m, int* origem, int*destino){
    int i;
    /*Percorrer de 0 ate o numero total de elementos*/
    for (i = 0; i <=m.numero_elementos; i++){
    destino[i]=origem[i];
    }
}

/*Realizar a troca dos vizinhos*/
void realizar_troca(struct matriz m, int* solucao_inicial,int* melhor_vizinho){
    /*Este vetor com a solucao temporaria armazenara a situacao dos nodos atuais,
    para poder fazer a verificacao na hora de trocar*/
    int* solucao_tmp = malloc((m.numero_elementos + 1) * sizeof(int));
    int custo_referencia_inicial = calcular_custo(m,solucao_inicial);
    int custo_referencia = calcular_custo(m,solucao_inicial);

    /*Para garantir que, caso nao haja nenhuma solucao melhor, o meu melhor vizinho eh a propria solucao inicial*/
    copiar_caminho(m, solucao_inicial, melhor_vizinho);

    /*Partindo do primeiro elemento a frente*/
    for (int i = 1; i < m.numero_elementos; i++){
        copiar_caminho(m,solucao_inicial,solucao_tmp); /*Copiando a solucao atual para a temporaria*/

        /*Sempre partindo da proxima posicao que i estiver*/
       for (int j = i+1; j < m.numero_elementos; j++){
          
            int tmp = solucao_tmp[i]; /*inicia uma variavel auxiliar para verificar o custo dela e comparar com a referencia*/
            solucao_tmp[i] = solucao_tmp[j];
            solucao_tmp[j] = tmp;

           int custo_solucao_tmp = calcular_custo(m,solucao_tmp);
           /*Se esse custo da solucao temporaria for menor que a atual, logo essa solucao temporaria eh melhor
           e assim, substitui a atual pela temporaria*/
           if(custo_solucao_tmp  <custo_referencia){
               custo_referencia = custo_solucao_tmp;
               /*Copio a solucao temporaria para o melhor vizinho*/
               copiar_caminho(m,solucao_tmp,melhor_vizinho);
           }
     /*    if(custo_referencia<custo_referencia_inicial){
            custo_referencia_inicial = custo_referencia;
            copiar_caminho(m,solucao_tmp,melhor_vizinho);
        } */
       }
    }
}