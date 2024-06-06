#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

typedef struct Musica {
    char artista[100];
    char titulo[100];
    int id;
    struct Musica *anterior;
    struct Musica *proxima;
} Musica;

void limparEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void carregarLista(Musica lista[], int *posicao) {
    FILE *arquivo = fopen("musicas.txt", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.");
        return;
    }

    char linha[200];
    while (fgets(linha, sizeof(linha), arquivo)) {
        lista[*posicao].id = *posicao;
        char *token = strtok(linha, ";");
        if (token != NULL) {
            strcpy(lista[*posicao].artista, token);
            token = strtok(NULL, "\n");
            if (token != NULL) {
                strcpy(lista[*posicao].titulo, token);
                (*posicao)++;
            }
        }
    }

    fclose(arquivo);
}

void limparTexto(char *str) {
    char *end;

    while (isspace((unsigned char)*str)) {
        str++;
    }
    if (*str == 0) {
        return;
    }

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
}

void salvarLista(Musica lista[], int quantidade) {
    FILE *arquivo = fopen("musicas.txt", "w+");

    if(arquivo == NULL) {
        printf("Erro ao abrir o arquivo.");
        exit(1);
    }

    for(int i = 0; i < quantidade; i++) {
        limparTexto(lista[i].artista);
        limparTexto(lista[i].titulo);

        if(i == 0) {
            lista[i].anterior = &lista[quantidade-1]; 
        } else { 
            lista[i].anterior = &lista[i-1];
        }

        if(i == quantidade-1) {
            lista[i].proxima = &lista[0]; 
        } else { 
            lista[i].proxima = &lista[i+1];
        }

        fprintf(arquivo, "%s;%s\n", lista[i].artista, lista[i].titulo);
    } 

    fclose(arquivo);
} 

void exibirPlaylist(Musica lista[], int quantidade) {
    for(int i = 0; i < quantidade; i++) {
        printf("\n%d. %s, %s", i+1, lista[i].artista, lista[i].titulo);
    }
}

void adicionarMusica(Musica lista[], int *posicao) {
    printf("\nDigite o nome do artista: ");
    fgets(lista[*posicao].artista, sizeof(lista[*posicao].artista), stdin);
    lista[*posicao].artista[strcspn(lista[*posicao].artista, "\n")] = '\0';

    printf("\nDigite o nome da música: ");
    fgets(lista[*posicao].titulo, sizeof(lista[*posicao].titulo), stdin);
    lista[*posicao].titulo[strcspn(lista[*posicao].titulo, "\n")] = '\0';

    printf("\nMúsica adicionada com sucesso.");
    (*posicao)++;
}

void removerMusica(Musica lista[], int *posicao, char titulo[100]) {
    int indice = -1;

    for(int i = 0; i < *posicao; i++) {
        if(strcmp(titulo, lista[i].titulo) == 0) {
            indice = i;
            break;
        }
    }

    if (indice != -1) {
        for (int i = indice; i < *posicao - 1; i++) {
            lista[i] = lista[i + 1];
        }
        (*posicao)--;

        salvarLista(lista, *posicao);
        printf("Música removida com sucesso.\n");
    } else {
        printf("Música não encontrada.\n");
    }
}

void buscarMusica(Musica lista[], int quantidade, char titulo[200]) {
    int encontrada = 0;

    for(int i = 0; i < quantidade; i++) {
        if(strcmp(lista[i].titulo, titulo) == 0) { 
            printf("\nMúsica encontrada...");
            printf("\nArtista: %s", lista[i].artista);
            printf("\nMúsica: %s", lista[i].titulo);
            encontrada = 1;
            break;
        }
    }

    if(!encontrada) {
        printf("\nNenhuma música encontrada.");
    }
}

int compararMusicas(const void *a, const void *b) {
    Musica *musicaA = (Musica *)a;
    Musica *musicaB = (Musica *)b;
    return strcasecmp(musicaA->titulo, musicaB->titulo);
}

void exibirPlaylistOrdenada(Musica lista[], int quantidade) {
    Musica listaCopia[500];

    for(int i = 0; i < quantidade; i++) {
        strcpy(listaCopia[i].artista, lista[i].artista);
        strcpy(listaCopia[i].titulo, lista[i].titulo);
        listaCopia[i].id = lista[i].id;
        listaCopia[i].anterior = lista[i].anterior;
        listaCopia[i].proxima = lista[i].proxima;
    }

    qsort(listaCopia, quantidade, sizeof(Musica), compararMusicas);

    printf("\nPlaylist ordenada pelo nome das músicas:");
    for (int i = 0; i < quantidade; i++) {
        printf("\n%d. %s, %s", i+1, listaCopia[i].artista, listaCopia[i].titulo);
    }
}

void proxMusica(Musica **musicaAtual) {
    *musicaAtual = (*musicaAtual)->proxima;
    printf("\nPróxima música: %s - %s\n", (*musicaAtual)->artista, (*musicaAtual)->titulo);
}

void musicaAnterior(Musica **musicaAtual) {
    *musicaAtual = (*musicaAtual)->anterior;
    printf("\nMúsica anterior: %s - %s\n", (*musicaAtual)->artista, (*musicaAtual)->titulo);
}

int main() {
    Musica lista[500];
    int posicao = 0;
    int escolha;
    char titulo[100];
    int continuar = 0;
    Musica *musicaAtual = NULL;

    carregarLista(lista, &posicao);
    salvarLista(lista, posicao);

    if (posicao > 0) {
        musicaAtual = &lista[0];
    }

    do {
        system("clear");

        if (musicaAtual != NULL) {
            printf("\n==================== MÚSICA ATUAL ====================");
            printf("\n%s de %s", musicaAtual->titulo, musicaAtual->artista);
        }

        printf("\n==================== MENU ====================");
        printf("\n[1] Exibir playlist por ordem de cadastro\n");
        printf("[2] Exibir playlist ordenada pelo nome das músicas\n");
        printf("[3] Inserir nova música\n");
        printf("[4] Remover música\n");
        printf("[5] Buscar música\n");
        printf("[6] Próxima música\n");
        printf("[7] Música anterior\n");
        printf("[8] Encerrar\n");
        printf("==================== **** ====================");
        printf("\nO que deseja fazer? ");
        scanf("%d", &escolha);

        switch(escolha) {
            case 1:
                exibirPlaylist(lista, posicao);
                break;
            case 2:
                exibirPlaylistOrdenada(lista, posicao);
                break;
            case 3:
                limparEntrada();
                adicionarMusica(lista, &posicao);
                salvarLista(lista, posicao);
                break;
            case 4:
                limparEntrada();
                printf("Digite o nome da música que você deseja remover: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0';
                removerMusica(lista, &posicao, titulo);
                break;
            case 5:
                limparEntrada();
                printf("Digite o nome da música: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0';
                buscarMusica(lista, posicao, titulo);
                break;
            case 6:
                proxMusica(&musicaAtual);
                break;
            case 7:
                musicaAnterior(&musicaAtual);
                break;
            case 8:
                printf("Obrigado por utilizar nosso programa.");
                break;
            default:
                printf("Comando não reconhecido...");
                break;
        }

        if(escolha != 8) {
            printf("\n\nDeseja realizar mais uma interação? SIM [0] / NÃO [1] ");
            scanf("%d", &continuar);
        } else {
            continuar = 1;
        }
    } while (continuar != 1);

    return 0;
}
