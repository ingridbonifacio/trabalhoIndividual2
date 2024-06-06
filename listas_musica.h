#ifndef PLAYLIST_MUSICAS_H
#define PLAYLIST_MUSICAS_H

typedef struct Musica {
    char artista[100];
    char titulo[100];
    int id;
    struct Musica *anterior;
    struct Musica *proxima;
} Musica;

void limparBuffer();
void carregarPlaylist(Musica playlist[], int *posicaoAtual);
void limparTexto(char *str);
void salvarPlaylist(Musica playlist[], int totalMusicas);
void mostrarPlaylist(Musica playlist[], int totalMusicas);
void adicionarMusica(Musica playlist[], int *posicaoAtual);
void excluirMusica(Musica playlist[], int *posicaoAtual, char titulo[100]);
void procurarMusica(Musica playlist[], int totalMusicas, char titulo[200]);
int compararTitulo(const void *a, const void *b);
void mostrarPlaylistOrdenada(Musica playlist[], int totalMusicas);
void tocarProxima(Musica **musicaAtual);
void tocarAnterior(Musica **musicaAtual);
int main();

#endif
