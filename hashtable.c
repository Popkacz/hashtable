#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_TABELA 2000
#define MAX_CHAVES 10000

typedef struct No {
    int chave;
    struct No* proximo;
} No;

No* tab_hash[TAM_TABELA];
No Nos[MAX_CHAVES];
int NoCount = 0;

unsigned int funcHash(int chave) {
    return (chave * 2654435761) % TAM_TABELA;
}

void inserir(int chave) {
    unsigned int hashIndex = funcHash(chave);
    No* novoNo = &Nos[NoCount++];
    novoNo->chave = chave;
    novoNo->proximo = tab_hash[hashIndex];
    tab_hash[hashIndex] = novoNo;
}

int busca(int chave, int* passos, int* colisoes) {
    unsigned int hashIndex = funcHash(chave);
    No* atual = tab_hash[hashIndex];
    *passos = 0;
    *colisoes = 0;

    while (atual) {
        (*passos)++;
        if (atual->chave == chave) {
            return 1;
        }
        (*colisoes)++;
        atual = atual->proximo;
    }
    return 0;
}

void carregararquivo(const char* nomeArquivo) {
    FILE* file = fopen(nomeArquivo, "r");
    if (!file) {
        printf("Erro ao abrir arquivo %s\n", nomeArquivo);
        exit(1);
    }

    int chave;
    while (fscanf(file, "%d", &chave) != EOF) {
        inserir(chave);
    }

    fclose(file);
}

void checarchaves(const char* nomeArquivo) {
    FILE* file = fopen(nomeArquivo, "r");
    if (!file) {
        printf("Erro ao abrir arquivo %s\n", nomeArquivo);
        exit(1);
    }

    int chave, passos, colisoes;
    while (fscanf(file, "%d", &chave) != EOF) {
        clock_t start_time = clock();
        int found = busca(chave, &passos, &colisoes);
        clock_t end_time = clock();
        double tempoGasto = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

        printf("Chave: %d, Passos: %d, Colisoes: %d, Tempo: %f segundos\n", chave, passos, colisoes, tempoGasto);
    }

    fclose(file);
}

void iniciarHashTable() {
    memset(tab_hash, 0, sizeof(tab_hash));
    NoCount = 0;
}

int main() {
    iniciarHashTable();

    carregararquivo("numeros_aleatorios.txt");
    checarchaves("checkin.txt");

    return 0;
}
