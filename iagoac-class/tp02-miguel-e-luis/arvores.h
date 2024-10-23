#ifndef _H_ARVORES
#define _H_ARVORES

// Implementar qualquer struct que for necessária aqui

typedef struct no_B{
    int valor;
    struct no_B *esquerdo, *direito;
    int altura;
} no_balanceada;
// Estrutura da arvore balanceada

typedef struct no_NotB{
    int valor;
    struct no_NotB *esquerda, *direita;
} no_NB;
// Estrutura da arvore não balanceada

double arvore_binaria(int instancia_num);
double arvore_balanceada(int instancia_num);

// Funções da arvore não balanceada: 
no_NB* raiz_no(int valor);
no_NB* inserirNB(no_NB* raiz, int valor) ;
no_NB* removerNB(no_NB* raiz, int valor);

// Funções da arvore balanceada: 
no_balanceada* removerBalanceada(no_balanceada *raiz, int chave);
no_balanceada* inserirBalanceada(no_balanceada *raiz, int x);
no_balanceada* balancear(no_balanceada *raiz);
no_balanceada* rotacaoDireitaEsquerda(no_balanceada *r);
no_balanceada* rotacaoEsquerdaDireita(no_balanceada *r);
no_balanceada* rotacaoDireita(no_balanceada *r);
no_balanceada* rotacaoEsquerda(no_balanceada *r);
int fatorDeBalanceamento(no_balanceada *no);
int alturaDoNo(no_balanceada *no);
int maior(int a, int b);
no_balanceada* novoNoBalanceada(int x);

#endif // _H_ARVORES