#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 5

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila;

int proximoId = 0;

void inicializarFila(Fila *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->total = 0;
}

int filaCheia(Fila *fila) {
    return fila->total == MAX;
}

int filaVazia(Fila *fila) {
    return fila->total == 0;
}

Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    int indice = rand() % 4;

    Peca novaPeca;
    novaPeca.nome = tipos[indice];
    novaPeca.id = proximoId;
    proximoId++;

    return novaPeca;
}

void inserirPeca(Fila *fila, Peca peca) {
    if (filaCheia(fila)) {
        printf("\nA fila esta cheia. Nao foi possivel inserir nova peca.\n");
        return;
    }

    fila->itens[fila->fim] = peca;
    fila->fim = (fila->fim + 1) % MAX;
    fila->total++;

    printf("\nNova peca inserida: [%c %d]\n", peca.nome, peca.id);
}

Peca jogarPeca(Fila *fila) {
    Peca vazia = {'-', -1};

    if (filaVazia(fila)) {
        printf("\nA fila esta vazia. Nao ha peca para jogar.\n");
        return vazia;
    }

    Peca pecaRemovida = fila->itens[fila->inicio];
    fila->inicio = (fila->inicio + 1) % MAX;
    fila->total--;

    printf("\nPeca jogada: [%c %d]\n", pecaRemovida.nome, pecaRemovida.id);

    return pecaRemovida;
}

void exibirFila(Fila *fila) {
    printf("\nFila de pecas:\n");

    if (filaVazia(fila)) {
        printf("Fila vazia.\n");
        return;
    }

    for (int i = 0; i < fila->total; i++) {
        int posicao = (fila->inicio + i) % MAX;
        printf("[%c %d] ", fila->itens[posicao].nome, fila->itens[posicao].id);
    }

    printf("\n");
}

void carregarFilaInicial(Fila *fila) {
    for (int i = 0; i < MAX; i++) {
        inserirPeca(fila, gerarPeca());
    }
}

int main() {
    Fila filaPecas;
    int opcao;

    srand(time(NULL));

    inicializarFila(&filaPecas);
    carregarFilaInicial(&filaPecas);

    do {
        exibirFila(&filaPecas);

        printf("\n========== TETRIS STACK ==========\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
              
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca(&filaPecas);
                break;

            case 2:
                inserirPeca(&filaPecas, gerarPeca());
                break;

            case 0:
                printf("\nEncerrando Tetris Stack...\n");
                break;

            default:
                printf("\nOpcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}