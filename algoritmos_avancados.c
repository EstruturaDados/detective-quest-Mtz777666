#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

typedef struct No
{
    char nome[50];
    struct No *esquerdo;
    struct No *direito;
} No;

typedef struct NoFila
{
    No *sala;
    struct NoFila *proximo;
} NoFila;

typedef struct Fila
{
    NoFila *front;
    NoFila *rear;
} Fila;

typedef struct Pista
{
    char texto[100];
    struct Pista *esquerda;
    struct Pista *direita;
} Pista;

typedef struct Suspeito
{
    char nome[50];
    char pistasAssociadas[5][100]; 
    int contadorPistas;
    struct Suspeito* proximo;
} Suspeito;


#define HASH_SIZE 10
Suspeito* tabelaHash[HASH_SIZE];

Fila *criarFila()
{
    Fila *novaFila = (Fila *)malloc(sizeof(Fila));
    if (novaFila != NULL)
    {
        novaFila->front = NULL;
        novaFila->rear = NULL;
    }
    return novaFila;
}

int hashFunction(char *chave)
{
    int hashIndex = 0;
    for (int i = 0; chave[i] != '\0'; i++)
    {
        hashIndex += chave[i];
    }
    return hashIndex % HASH_SIZE;
}

void inicializarHash()
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        tabelaHash[i] = NULL;
    }
}

int buscarSuspeito(char *nome)
{
    int index = hashFunction(nome);
    Suspeito *atual = tabelaHash[index];
    while (atual != NULL)
    {
        if (strcmp(atual->nome, nome) == 0)
        {
            return index; 
        }
        atual = atual->proximo;
    }
    return -1; 
}

void inserirHash(Pista *pista, char *suspeitoNome)
{
    if (buscarSuspeito(suspeitoNome) == -1)
    {
        int index = hashFunction(suspeitoNome);
        Suspeito *novoSuspeito = (Suspeito *)malloc(sizeof(Suspeito));
        strcpy(novoSuspeito->nome, suspeitoNome);
        strcpy(novoSuspeito->pistasAssociadas[0], pista->texto);
        novoSuspeito->contadorPistas = 1;
        novoSuspeito->proximo = tabelaHash[index];
        tabelaHash[index] = novoSuspeito;
    }
    else
    {
        int index = buscarSuspeito(suspeitoNome);
        Suspeito *atual = tabelaHash[index];
        while (atual != NULL)
        {
            if (strcmp(atual->nome, suspeitoNome) == 0)
            {
                if (atual->contadorPistas < 5)
                {
                    strcpy(atual->pistasAssociadas[atual->contadorPistas], pista->texto);
                    atual->contadorPistas++;
                }
                break;
            }
            atual = atual->proximo;
        }
    }
}

void mostrarSuspeitos()
{
    printf("\n---Suspeitos e suas pistas associadas---\n");
    for (int i = 0; i < HASH_SIZE; i++)
    {
        Suspeito *atual = tabelaHash[i];
        while (atual != NULL)
        {
            printf("Suspeito: %s\n", atual->nome);
            printf("Pistas associadas:\n");
            for (int j = 0; j < atual->contadorPistas; j++)
            {
                printf("- %s\n", atual->pistasAssociadas[j]);
            }
            printf("\n");
            atual = atual->proximo;
        }
    }
}

int filaVazia(Fila *f)
{
    return (f->front == NULL);
}

void enqueue(Fila *f, No *salaAdd)
{
    NoFila *novaSala = (NoFila *)malloc(sizeof(NoFila));
    if (novaSala == NULL)
        return;
    novaSala->sala = salaAdd;
    novaSala->proximo = NULL;

    if (filaVazia(f))
    {
        f->front = novaSala;
    }
    else
    {
        f->rear->proximo = novaSala;
    }

    f->rear = novaSala;
}

No *dequeue(Fila *f)
{
    if (filaVazia(f))
    {
        return NULL;
    }

    NoFila *removido = f->front;
    No *salaRetornada = removido->sala;

    f->front = f->front->proximo;

    if (f->front == NULL)
    {
        f->rear = NULL;
    }

    free(removido);
    return salaRetornada;
}

No *criarSala(char *nome)
{
    No *novo = (No *)malloc(sizeof(No));
    strcpy(novo->nome, nome);
    novo->esquerdo = NULL;
    novo->direito = NULL;
    return novo;
}

void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

Pista* inserirBST(Pista **raiz, char *texto)
{
    if (*raiz == NULL)
    {
        Pista *novaPista = (Pista *)malloc(sizeof(Pista));
        strcpy(novaPista->texto, texto);
        novaPista->esquerda = NULL;
        novaPista->direita = NULL;
        *raiz = novaPista;
        return novaPista;
    }
    else
    {
        if (strcmp(texto, (*raiz)->texto) < 0)
        {
            return inserirBST(&(*raiz)->esquerda, texto);
        }
        else if (strcmp(texto, (*raiz)->texto) > 0)
        {
            return inserirBST(&(*raiz)->direita, texto);
        }
        return NULL;
    }
}

void emOrdem(Pista *raiz)
{
    if (raiz != NULL)
    {
        emOrdem(raiz->esquerda);
        printf("%s\n", raiz->texto);
        emOrdem(raiz->direita);
    }
}

void mostrarMaisCitado()
{
    int maxPistas = 0;
    char suspeitoMaisProvavel[50] = "";

    for (int i = 0; i < HASH_SIZE; i++)
    {
        Suspeito *atual = tabelaHash[i];
        while (atual != NULL)
        {
            if (atual->contadorPistas > maxPistas)
            {
                maxPistas = atual->contadorPistas;
                strcpy(suspeitoMaisProvavel, atual->nome);
            }
            atual = atual->proximo;
        }
    }

    if (maxPistas > 0)
    {
        printf("Suspeito mais provavel: %s com %d pistas associadas.\n", suspeitoMaisProvavel, maxPistas);
    }
    else
    {
        printf("Nenhum suspeito encontrado com pistas associadas.\n");
    }
}

void escolherSuspeito()
{
    int escolhido = 0;
    char nome[50];
    do
    {
    printf("Digite o nome do suspeito que deseja escolher como principal:\n");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0'; 

    int index = buscarSuspeito(nome);
    if (index != -1)
    {
        Suspeito *atual = tabelaHash[index];
        while (atual != NULL)
        {
            if (strcmp(atual->nome, nome) == 0)
            {
                printf("Voce escolheu '%s' como suspeito principal.\n", atual->nome);
                printf("Pistas associadas a este suspeito:\n");
                for (int j = 0; j < atual->contadorPistas; j++)
                {
                    printf("- %s\n", atual->pistasAssociadas[j]);
                }
                escolhido = 1;
                return;
            }
            atual = atual->proximo;
        }
        escolhido = -1;
    }
        printf("Suspeito '%s' nao encontrado na tabela hash. Tente novamente.\n", nome);
        limparBuffer();
    
} while (escolhido != 1);
    
}

void conectarSalas(No *raiz, No *filho)
{
    if (raiz == NULL)
        return;

    Fila *fila = criarFila();

    enqueue(fila, raiz);

    while (!filaVazia(fila))
    {
        No *atual = dequeue(fila);

        if (atual->esquerdo == NULL)
        {
            atual->esquerdo = filho;
            printf("Sala '%s' conectada a esquerda de '%s'\n", filho->nome, atual->nome);
            break;
        }
        else
        {
            enqueue(fila, atual->esquerdo);
        }

        if (atual->direito == NULL)
        {
            atual->direito = filho;
            printf("Sala '%s' conectada a direita de '%s'\n", filho->nome, atual->nome);
            break;
        }
        else
        {
            enqueue(fila, atual->direito);
        }
    }

    while (!filaVazia(fila))
    {
        dequeue(fila);
    }
    free(fila);
}

void explorarSalas(No *raiz, Pista **arvorePistas)
{
    char direcao;
    No *atual = raiz;

    if (atual == NULL)
    {
        printf("Arvore vazia...\n");
        return;
    }

    do
    {
        printf("\n---Exploracao das salas---\n");
        printf("Area atual: %s\n", atual->nome);
        printf("Pistas coletadas:\n");
        emOrdem(*arvorePistas);
        printf("\na(esquerda)\n"
               "d(direita)\n"
               "s(sair da exploracao)\n");
        printf("Insira a direcao em que deseja explorar:\n");
        scanf(" %c", &direcao);
        limparBuffer();
        switch (direcao)
        {
        case 'a':
            if (atual->esquerdo != NULL)
            {
                atual = atual->esquerdo;
                printf("Voce se moveu para a esquerda, entrando na sala: %s\n", atual->nome);
                if (strcmp(atual->nome, "Garagem") == 0)
            {
                Pista *novaPista = inserirBST(arvorePistas, "Pista: Sangue encontrado na garagem.");
                if (novaPista != NULL){
                printf("Voce encontrou uma pista importante na garagem! Adicionando a pista na arvore de busca...\n");
                inserirHash(novaPista, "Motorista da mansao");
                }
            } else if (strcmp(atual->nome, "Biblioteca") == 0)
            {
                Pista *novaPista = inserirBST(arvorePistas, "Pista: Livro de registros da mansao encontrado na biblioteca.");
                if (novaPista != NULL){
                printf("Voce encontrou uma pista importante na biblioteca! Adicionando a pista na arvore de busca...\n");
                inserirHash(novaPista, "Bibliotecario da mansao");
                }
            }
            }
            if (atual->esquerdo == NULL && atual->direito == NULL)
            {
                printf("Voce chegou a uma sala sem saidas. Retornando para a primeira sala...\n");
                atual = raiz;
            }
            
            break;

        case 'd':
            if (atual->direito != NULL)
            {
                atual = atual->direito;
                printf("Voce se moveu para a direita, entrando na sala: %s\n", atual->nome);
                if (strcmp(atual->nome, "Cozinha") == 0)
            {
                Pista *novaPista = inserirBST(arvorePistas, "Pista: Pegadas de barro encontradas na cozinha.");
                if (novaPista != NULL){
                    printf("Voce encontrou uma pista importante na cozinha! Adicionando a pista na arvore de busca...\n");
                inserirHash(novaPista, "Motorista da mansao");
                }
                
            }
            }
            if (atual->esquerdo == NULL && atual->direito == NULL)
            {
                printf("Voce chegou a uma sala sem saidas. Retornando para a primeira sala...\n");
                atual = raiz;
            }
            
            break;

        case 's':
            break;

        default:
            printf("Valor invalido. Tente novamente...\n");
            limparBuffer();
            break;
        }
    } while (direcao != 's');
    printf("Saindo da exploracao...\n");
    limparBuffer();
}

int main()
{
    inicializarHash();
    No *Sala = criarSala("Hall de Entrada");
    Sala->esquerdo = criarSala("Sala de estar");
    Sala->direito = criarSala("Cozinha");
    Sala->esquerdo->esquerdo = criarSala("Biblioteca");
    Sala->esquerdo->direito = criarSala("Sotao");
    Sala->direito->esquerdo = criarSala("Garagem");
    Sala->direito->direito = criarSala("Jardim");
    No *Quarto = criarSala("Quarto");
    conectarSalas(Sala, Quarto);
    Pista *arvorePistas = NULL;
    explorarSalas(Sala, &arvorePistas);
    mostrarSuspeitos();
    printf("Continuando...\n");
            limparBuffer();
    mostrarMaisCitado();
    printf("Continuando...\n");
            limparBuffer();
    escolherSuspeito();

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

    return 0;
}
