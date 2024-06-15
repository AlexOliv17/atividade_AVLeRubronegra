#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Estrutura do nó da árvore AVL
typedef struct No {
    int chave;
    struct No* esq;
    struct No* dir;
    int altura;
} No;

int rotacoes = 0; // Contador de rotações

int altura(No* N) {
    if (N == NULL)
        return 0;
    return N->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

No* novoNo(int chave) {
    No* no = (No*)malloc(sizeof(No));
    no->chave = chave;
    no->esq = NULL;
    no->dir = NULL;
    no->altura = 1;
    return(no);
}

No* rotacaoDireita(No* y) {
    rotacoes++; // Incrementa a contagem de rotacoes
    No* x = y->esq;
    No* T2 = x->dir;
    x->dir = y;
    y->esq = T2;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    return x;
}

No* rotacaoEsquerda(No* x) {
    rotacoes++; // Incrementa a contagem de rotacoes
    No* y = x->dir;
    No* T2 = y->esq;
    y->esq = x;
    x->dir = T2;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    return y;
}

int obterBalanco(No* N) {
    if (N == NULL)
        return 0;
    return altura(N->esq) - altura(N->dir);
}

No* inserir(No* no, int chave) {
    if (no == NULL)
        return(novoNo(chave));
    if (chave < no->chave)
        no->esq = inserir(no->esq, chave);
    else if (chave > no->chave)
        no->dir = inserir(no->dir, chave);
    else
        return no;
    no->altura = 1 + max(altura(no->esq), altura(no->dir));
    int balanco = obterBalanco(no);
    if (balanco > 1 && chave < no->esq->chave)
        return rotacaoDireita(no);
    if (balanco < -1 && chave > no->dir->chave)
        return rotacaoEsquerda(no);
    if (balanco > 1 && chave > no->esq->chave) {
        no->esq = rotacaoEsquerda(no->esq);
        return rotacaoDireita(no);
    }
    if (balanco < -1 && chave < no->dir->chave) {
        no->dir = rotacaoDireita(no->dir);
        return rotacaoEsquerda(no);
    }
    return no;
}

No* noValorMinimo(No* no) {
    No* atual = no;
    while (atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

No* deletarNo(No* raiz, int chave) {
    if (raiz == NULL)
        return raiz;
    if (chave < raiz->chave)
        raiz->esq = deletarNo(raiz->esq, chave);
    else if (chave > raiz->chave)
        raiz->dir = deletarNo(raiz->dir, chave);
    else {
        if ((raiz->esq == NULL) || (raiz->dir == NULL)) {
            No* temp = raiz->esq ? raiz->esq : raiz->dir;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else
                *raiz = *temp;
            free(temp);
        } else {
            No* temp = noValorMinimo(raiz->dir);
            raiz->chave = temp->chave;
            raiz->dir = deletarNo(raiz->dir, temp->chave);
        }
    }
    if (raiz == NULL)
        return raiz;
    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));
    int balanco = obterBalanco(raiz);
    if (balanco > 1 && obterBalanco(raiz->esq) >= 0)
        return rotacaoDireita(raiz);
    if (balanco > 1 && obterBalanco(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }
    if (balanco < -1 && obterBalanco(raiz->dir) <= 0)
        return rotacaoEsquerda(raiz);
    if (balanco < -1 && obterBalanco(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }
    return raiz;
}

No* buscar(No* raiz, int chave) {
    if (raiz == NULL || raiz->chave == chave)
        return raiz;
    if (raiz->chave < chave)
        return buscar(raiz->dir, chave);
    return buscar(raiz->esq, chave);
}

int main() {
    No* raiz = NULL;
    int i;
    clock_t inicio, fim;
    double tempo;

    // Inserção de 10.000 valores ordenados
    rotacoes = 0;
    inicio = clock();
    for (i = 1; i <= 10000; i++) {
        raiz = inserir(raiz, i);
    }
    fim = clock();
    tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo para inserir 10.000 valores ordenados: %f segundos\n", tempo);
    printf("Numero de rotacoes para inserir 10.000 valores: %d\n", rotacoes);

    // Remoção de 10.000 valores ordenados
    rotacoes = 0;
    inicio = clock();
    for (i = 1; i <= 10000; i++) {
        raiz = deletarNo(raiz, i);
    }
    fim = clock();
    tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo para remover 10.000 valores ordenados: %f segundos\n", tempo);
    printf("Numero de rotacoes para remover 10.000 valores: %d\n", rotacoes);

    // Reinserir os valores para a busca
    for (i = 1; i <= 10000; i++) {
        raiz = inserir(raiz, i);
    }

    // Busca de 1.000 elementos aleatórios
    int valoresAleatorios[1000];
    for (i = 0; i < 1000; i++) {
        valoresAleatorios[i] = rand() % 10000 + 1;
    }

    inicio = clock();
    for (i = 0; i < 1000; i++) {
        buscar(raiz, valoresAleatorios[i]);
    }
    fim = clock();
    tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo para buscar 1.000 elementos aleatorios: %f segundos\n", tempo);

    return 0;
}
