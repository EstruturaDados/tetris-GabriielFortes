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

int quantidadePilha(Pilha *pilha) {
    return pilha->topo + 1;
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

// ================= AÇÕES =================

void jogarPeca(Fila *fila) {
    Peca pecaJogada = retirarFila(fila);

    if (pecaJogada.id == -1) {
        printf("\nNao ha peca para jogar.\n");
        return;
    }

    printf("\nPeca jogada: [%c %d]\n", pecaJogada.nome, pecaJogada.id);

    // A fila deve permanecer cheia sempre que possível
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

    printf("\nPeca enviada para a reserva: [%c %d]\n", pecaReservada.nome, pecaReservada.id);

    // Gera uma nova peça para manter a fila cheia
    inserirFila(fila, gerarPeca());
}

void usarPecaReservada(Pilha *pilha) {
    Peca pecaUsada = desempilhar(pilha);

    if (pecaUsada.id == -1) {
        return;
    }

    printf("\nPeca usada da reserva: [%c %d]\n", pecaUsada.nome, pecaUsada.id);
}

void trocarPecaAtual(Fila *fila, Pilha *pilha) {
    if (filaVazia(fila)) {
        printf("\nNao ha peca na fila para trocar.\n");
        return;
    }

    if (pilhaVazia(pilha)) {
        printf("\nNao ha peca na pilha para trocar.\n");
        return;
    }

    Peca temporaria = fila->itens[fila->inicio];

    fila->itens[fila->inicio] = pilha->itens[pilha->topo];
    pilha->itens[pilha->topo] = temporaria;

    printf("\nTroca realizada entre a peca da frente da fila e o topo da pilha.\n");
}

void trocaMultipla(Fila *fila, Pilha *pilha) {
    if (fila->total < 3 || quantidadePilha(pilha) < 3) {
        printf("\nTroca multipla nao realizada.\n");
        printf("E necessario ter pelo menos 3 pecas na fila e 3 pecas na pilha.\n");
        return;
    }

    /*
        Troca os 3 primeiros da fila com as 3 peças da pilha.

        Fila:
        posicao 0 logica = frente da fila
        posicao 1 logica = segunda peça
        posicao 2 logica = terceira peça

        Pilha:
        topo, topo - 1, topo - 2
    */

    for (int i = 0; i < 3; i++) {
        int posicaoFila = (fila->inicio + i) % MAX_FILA;
        int posicaoPilha = pilha->topo - i;

        Peca temporaria = fila->itens[posicaoFila];

        fila->itens[posicaoFila] = pilha->itens[posicaoPilha];
        pilha->itens[posicaoPilha] = temporaria;
    }

    printf("\nTroca realizada entre os 3 primeiros da fila e as 3 pecas da pilha.\n");
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

        printf("\n========== TETRIS STACK - NIVEL AVANCADO ==========\n");
        printf("1 - Jogar peca da frente da fila\n");
        printf("2 - Enviar peca da fila para a pilha de reserva\n");
        printf("3 - Usar peca da pilha de reserva\n");
        printf("4 - Trocar peca da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf("0 - Sair\n");
        printf("Opcao escolhida: ");

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

            case 4:
                trocarPecaAtual(&filaPecas, &pilhaReserva);
                break;

            case 5:
                trocaMultipla(&filaPecas, &pilhaReserva);
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