#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "arvores.h"

// Criação da estrutura e funções da arvore não balanceada:

// Cria o nó raiz da árvore
no_NB *raiz_no(int valor)
{
    no_NB *temp = (no_NB *)malloc(sizeof(no_NB));
    temp->valor = valor;
    temp->esquerda = temp->direita = NULL;
    return temp;
}

// Função para inserir um novo valor na árvore
no_NB *inserirNB(no_NB *raiz, int valor)
{
    if (raiz == NULL)
    {
        return raiz_no(valor);
    }
    if (valor < (raiz->valor))
    {
        raiz->esquerda = inserirNB(raiz->esquerda, valor);
    }
    else if (valor > (raiz->valor))
    {
        raiz->direita = inserirNB(raiz->direita, valor);
    }

    return raiz;
}

// Função para remover um nó da árvore
no_NB *removerNB(no_NB *raiz, int valor)
{
    if (raiz == NULL)
        return raiz;

    if (valor < (raiz->valor))
    {
        raiz->esquerda = removerNB(raiz->esquerda, valor);
    }
    else if (valor > (raiz->valor))
    {
        raiz->direita = removerNB(raiz->direita, valor);
    }
    else
    {
        if (raiz->esquerda == NULL)
        {
            no_NB *no_temporario = raiz->direita;
            free(raiz);
            return no_temporario;
        }
        else if (raiz->direita == NULL)
        {
            no_NB *no_temporario = raiz->esquerda;
            free(raiz);
            return no_temporario;
        }

        no_NB *no_substituto = raiz->direita;
        while (no_substituto->esquerda != NULL)
        {
            no_substituto = no_substituto->esquerda;
        }

        raiz->valor = no_substituto->valor;
        raiz->direita = removerNB(raiz->direita, no_substituto->valor);
    }
    return raiz;
}

no_NB *root_NB = NULL;

double arvore_binaria(int instancia_num)
{
    double tempo = 0;
    clock_t beginNB = clock();
    char textNumber[12];

    sprintf(textNumber, "%d", instancia_num);
    char fileName[15] = "instancias/";
    strcat(fileName, textNumber);

    FILE *readFile = fopen(fileName, "r");
    char leitor;
    int valor;

    while (fscanf(readFile, "%c %d", &leitor, &valor) != EOF)
    {

        switch (leitor)
        {
        case 'I':
            root_NB = inserirNB(root_NB, valor);
            break;
        case 'R':
            root_NB = removerNB(root_NB, valor);
            break;
        }
    }
    clock_t endNB = clock();
    tempo += (double)(endNB - beginNB) / CLOCKS_PER_SEC;
    fclose(readFile);
    return (tempo);
}

// Criação da estrutura e funções da arvore balanceada:

/*
    Função que cria um novo nó
    x -> valor a ser inserido no nó
    Retorna: o endereço do nó criado
*/
no_balanceada *novoNoBalanceada(int x)
{
    no_balanceada *novo = malloc(sizeof(no_balanceada));

    if (novo)
    {
        novo->valor = x;
        novo->esquerdo = NULL;
        novo->direito = NULL;
        novo->altura = 0;
    }
    else
        printf("\nERRO ao alocar nó em novoNo!\n");
    return novo;
}

/*
    Retorna o maior dentre dois valores
    a, b -> altura de dois nós da árvore
*/
int maior(int a, int b)
{
    return (a > b) ? a : b;
}

//  Retorna a altura de um nó ou -1 caso ele seja null
int alturaDoNo(no_balanceada *no)
{
    if (no == NULL)
        return -1;
    else
        return no->altura;
}

//   Calcula e retorna o fator de balanceamento de um nó
int fatorDeBalanceamento(no_balanceada *no)
{
    if (no)
        return (alturaDoNo(no->esquerdo) - alturaDoNo(no->direito));
    else
        return 0;
}

// --------- ROTAÇÕES ---------------------------

// função para a rotação à esquerda
no_balanceada *rotacaoEsquerda(no_balanceada *r)
{
    no_balanceada *y, *f;

    y = r->direito;
    f = y->esquerdo;

    y->esquerdo = r;
    r->direito = f;

    r->altura = maior(alturaDoNo(r->esquerdo), alturaDoNo(r->direito)) + 1;
    y->altura = maior(alturaDoNo(y->esquerdo), alturaDoNo(y->direito)) + 1;

    return y;
}

// função para a rotação à direita
no_balanceada *rotacaoDireita(no_balanceada *r)
{
    no_balanceada *y, *f;

    y = r->esquerdo;
    f = y->direito;

    y->direito = r;
    r->esquerdo = f;

    r->altura = maior(alturaDoNo(r->esquerdo), alturaDoNo(r->direito)) + 1;
    y->altura = maior(alturaDoNo(y->esquerdo), alturaDoNo(y->direito)) + 1;

    return y;
}

no_balanceada *rotacaoEsquerdaDireita(no_balanceada *r)
{
    r->esquerdo = rotacaoEsquerda(r->esquerdo);
    return rotacaoDireita(r);
}

no_balanceada *rotacaoDireitaEsquerda(no_balanceada *r)
{
    r->direito = rotacaoDireita(r->direito);
    return rotacaoEsquerda(r);
}

/*
    Função para realizar o balanceamento da árvore após uma inserção ou remoção
    Recebe o nó que está desbalanceado e retorna a nova raiz após o balanceamento
*/
no_balanceada *balancear(no_balanceada *raiz)
{
    int fb = fatorDeBalanceamento(raiz);

    // Rotação à esquerda
    if (fb < -1 && fatorDeBalanceamento(raiz->direito) <= 0)
        raiz = rotacaoEsquerda(raiz);

    // Rotação à direita
    else if (fb > 1 && fatorDeBalanceamento(raiz->esquerdo) >= 0)
        raiz = rotacaoDireita(raiz);

    // Rotação dupla à esquerda
    else if (fb > 1 && fatorDeBalanceamento(raiz->esquerdo) < 0)
        raiz = rotacaoEsquerdaDireita(raiz);

    // Rotação dupla à direita
    else if (fb < -1 && fatorDeBalanceamento(raiz->direito) > 0)
        raiz = rotacaoDireitaEsquerda(raiz);

    return raiz;
}

/*
    Insere um novo nó na árvore
    raiz -> raiz da árvore
    x -> valor a ser inserido
    Retorno: endereço do novo nó ou nova raiz após o balanceamento
*/
no_balanceada *inserirBalanceada(no_balanceada *raiz, int x)
{
    if (raiz == NULL) // árvore vazia
        return novoNoBalanceada(x);
    else
    { // inserção será à esquerda ou à direita
        if (x < raiz->valor)
            raiz->esquerdo = inserirBalanceada(raiz->esquerdo, x);
        else if (x > raiz->valor)
            raiz->direito = inserirBalanceada(raiz->direito, x);
    }

    // Recalcula a altura de todos os nós entre a raiz e o novo nó inserido
    raiz->altura = maior(alturaDoNo(raiz->esquerdo), alturaDoNo(raiz->direito)) + 1;

    // verifica a necessidade de rebalancear a árvore
    raiz = balancear(raiz);

    return raiz;
}

/*
    Função para remover um nó da Árvore binária balanceada
    Pode ser necessário rebalancear a árvore e a raiz pode ser alterada
    por isso precisamos retornar a raiz
*/
no_balanceada *removerBalanceada(no_balanceada *raiz, int chave)
{
    if (raiz == NULL)
    {
        printf("Valor nao encontrado!\n");
        return NULL;
    }
    else
    { // procura o nó a remover
        if (raiz->valor == chave)
        {
            // remove nós folhas (nós sem filhos)
            if (raiz->esquerdo == NULL && raiz->direito == NULL)
            {
                free(raiz);
                return NULL;
            }
            else
            {
                // remover nós que possuem 2 filhos
                if (raiz->esquerdo != NULL && raiz->direito != NULL)
                {
                    no_balanceada *aux = raiz->esquerdo;
                    while (aux->direito != NULL)
                        aux = aux->direito;
                    raiz->valor = aux->valor;
                    aux->valor = chave;
                    raiz->esquerdo = removerBalanceada(raiz->esquerdo, chave);
                    return raiz;
                }
                else
                {
                    // remover nós que possuem apenas 1 filho
                    no_balanceada *aux;
                    if (raiz->esquerdo != NULL)
                        aux = raiz->esquerdo;
                    else
                        aux = raiz->direito;
                    free(raiz);
                    return aux;
                }
            }
        }
        else
        {
            if (chave < raiz->valor)
                raiz->esquerdo = removerBalanceada(raiz->esquerdo, chave);
            else
                raiz->direito = removerBalanceada(raiz->direito, chave);
        }

        // Recalcula a altura de todos os nós entre a raiz e o novo nó inserido
        raiz->altura = maior(alturaDoNo(raiz->esquerdo), alturaDoNo(raiz->direito)) + 1;

        // verifica a necessidade de rebalancear a árvore
        raiz = balancear(raiz);

        return raiz;
    }
}

no_balanceada *root_avl = NULL;

double arvore_balanceada(int instancia_num)
{
    double tempoB = 0;
    clock_t beginB = clock();
    char textNumber[12];

    sprintf(textNumber, "%d", instancia_num);
    char fileName[15] = "instancias/";
    strcat(fileName, textNumber);

    FILE *readFile = fopen(fileName, "r");
    char leitor;
    int valor;

    while (fscanf(readFile, "%c %d", &leitor, &valor) != EOF)
    {

        switch (leitor)
        {
        case 'I':
            root_avl = inserirBalanceada(root_avl, valor);
            break;
        case 'R':
            root_avl = removerBalanceada(root_avl, valor);
            break;
        }

        
    }
    clock_t endB = clock();
    tempoB += (double)(endB - beginB) / CLOCKS_PER_SEC;
    fclose(readFile);
    return (tempoB);
}

int main(int argc, char *argv[])
{
    ///////////////////////////////////////////////////////////
    /////////////////// Leitor de instâncias //////////////////
    ///////////////// Não deve ser modificado /////////////////
    ///////////////////////////////////////////////////////////
    int instancia_num = -1;
    instancia_num = atoi(argv[1]);
    if (instancia_num <= 0 || instancia_num > 10)
    {
        printf("Para executar o código, digite ./arvores x\nonde x é um número entre 1 e 10 que simboliza a instância utilizada\n");
        exit(0);
    }

    double tempo_n_balanceada = arvore_binaria(instancia_num);
    double tempo_balanceada = arvore_balanceada(instancia_num);

    printf("%f\n%f\n", tempo_n_balanceada, tempo_balanceada);

    return (1);
}