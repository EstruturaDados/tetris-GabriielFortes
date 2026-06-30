#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int total;
} Fila;

typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

int proximoId = 0;

// Gera uma peça aleatória
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    int indice = rand() % 4;

    Peca peca;
    peca.nome = tipos[indice];
    peca.id = proximoId++;

    return peca;
}

// ================= FILA =================

void inicializarFila(Fila *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->total = 0;
}

int filaVazia(Fila *fila) {
    return fila->total == 0;
}

int filaCheia(Fila *fila) {
    return fila->total == MAX_FILA;
}

void inserirFila(Fila *fila, Peca peca) {
    if (filaCheia(fila)) {
        return;
    }

    fila->itens[fila->fim] = peca;
    fila->fim = (fila->fim + 1) % MAX_FILA;
    fila->total++;
}

Peca retirarFila(Fila *fila) {
    Peca vazia = {'-', -1};

    if (filaVazia(fila)) {
        return vazia;
    }

    Peca peca = fila->itens[fila->inicio];
    fila->inicio = (fila->inicio + 1) % MAX_FILA;
    fila->total--;

    return peca;
}

void carregarFilaInicial(Fila *fila) {
    for (int i = 0; i < MAX_FILA; i++) {
        inserirFila(fila, gerarPeca());
    }
}

// ================= PILHA =================

void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

int pilhaCheia(Pilha *pilha) {
    return pilha->topo == MAX_PILHA - 1;
}

void empilhar(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        printf("\nA pilha de reserva esta cheia!\n");
        return;
    }

    pilha->topo++;
    pilha->itens[pilha->topo] = peca;
}

Peca desempilhar(Pilha *pilha) {
    Peca vazia = {'-', -1};

    if (pilhaVazia(pilha)) {
        printf("\nA pilha de reserva esta vazia!\n");
        return vazia;
    }

    Peca peca = pilha->itens[pilha->topo];
    pilha->topo--;

    return peca;
}

// ================= AÇÕES DO JOGO =================

void jogarPeca(Fila *fila) {
    Peca pecaJogada = retirarFila(fila);

    if (pecaJogada.id == -1) {
        printf("\nNao ha peca para jogar.\n");
        return;
    }

    printf("\nPeca jogada: [%c %d]\n", pecaJogada.nome, pecaJogada.id);

    // Mantém a fila sempre cheia
    inserirFila(fila, gerarPeca());
}

void reservarPeca(Fila *fila, Pilha *pilha) {
    if (pilhaCheia(pilha)) {
        printf("\nNao foi possivel reservar. A pilha esta cheia.\n");
        return;
    }

    Peca pecaReservada = retirarFila(fila);

    if (pecaReservada.id == -1) {
        printf("\nNao ha peca para reservar.\n");
        return;
    }

    empilhar(pilha, pecaReservada);

    printf("\nPeca reservada: [%c %d]\n", pecaReservada.nome, pecaReservada.id);

    // Mantém a fila sempre cheia
    inserirFila(fila, gerarPeca());
}

void usarPecaReservada(Pilha *pilha) {
    Peca pecaUsada = desempilhar(pilha);

    if (pecaUsada.id == -1) {
        return;
    }

    printf("\nPeca reservada usada: [%c %d]\n", pecaUsada.nome, pecaUsada.id);
}

// ================= EXIBIÇÃO =================

void exibirFila(Fila *fila) {
    printf("Fila de pecas: ");

    if (filaVazia(fila)) {
        printf("vazia");
    }

    for (int i = 0; i < fila->total; i++) {
        int posicao = (fila->inicio + i) % MAX_FILA;
        printf("[%c %d] ", fila->itens[posicao].nome, fila->itens[posicao].id);
    }

    printf("\n");
}

void exibirPilha(Pilha *pilha) {
    printf("Pilha de reserva (Topo -> Base): ");

    if (pilhaVazia(pilha)) {
        printf("vazia");
    }

    for (int i = pilha->topo; i >= 0; i--) {
        printf("[%c %d] ", pilha->itens[i].nome, pilha->itens[i].id);
    }

    printf("\n");
}

void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("\n========== ESTADO ATUAL ==========\n");
    exibirFila(fila);
    exibirPilha(pilha);
}

// ================= MAIN =================

int main() {
    Fila filaPecas;
    Pilha pilhaReserva;
    int opcao;

    srand(time(NULL));

    inicializarFila(&filaPecas);
    inicializarPilha(&pilhaReserva);

    carregarFilaInicial(&filaPecas);

    do {
        exibirEstado(&filaPecas, &pilhaReserva);

        printf("\n========== TETRIS STACK ==========\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
        printf("0 - Sair\n");
        printf("Opcao: ");

        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca(&filaPecas);
                break;

            case 2:
                reservarPeca(&filaPecas, &pilhaReserva);
                break;

            case 3:
                usarPecaReservada(&pilhaReserva);
                break;

            case 0:
                printf("\nEncerrando Tetris Stack...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}