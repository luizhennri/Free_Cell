// Inclusão das bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>
#include <conio.h>

// Definição do tamanho de cada espaço
#define TAM_MESA 8
#define TAM_NAIPE 4
#define TAM_TEMP 4

// Definição das cores
#define RED 4
#define GREEN 2
#define WHITE 15
#define BLACK 0

// Definição da estrutura das cartas
typedef struct no
{
    short num;
    char naipe;
    struct no *prox;
} tCarta;

// Inicialização das pilhas
tCarta *primMonte = NULL;
tCarta *primMesa[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
tCarta *primNaipe[] = {NULL, NULL, NULL, NULL};
tCarta *primTemp[] = {NULL, NULL, NULL, NULL};

// Função para mudar a cor do texto do terminal
void textcolor(int iColor)
{
    HANDLE hl = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    bool b = GetConsoleScreenBufferInfo(hl, &bufferInfo);
    bufferInfo.wAttributes &= 0x00F0;
    SetConsoleTextAttribute(hl, bufferInfo.wAttributes |= iColor);

} // end textcolor()

// Função para mudar a cor de fundo do terminal
void backcolor(int iColor)
{
    HANDLE hl = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    bool b = GetConsoleScreenBufferInfo(hl, &bufferInfo);
    bufferInfo.wAttributes &= 0x000F;
    SetConsoleTextAttribute(hl, bufferInfo.wAttributes |= (iColor << 4));

} // end backcolor()

// Função para exibir uma mensagem de erro
void mensagem(char *textIn)
{
    system("cls");
    printf("%s\n\n", textIn);
    system("pause");

} // end mensagem()

// Função para verificar se uma carta é vermelha
bool cartaVermelha(tCarta *carta)
{
    return ((carta->naipe >= 3) && (carta->naipe <= 4));
} // end cartaVermelha()

// Função para verificar se uma carta é preta
bool cartaPreta(tCarta *carta)
{
    return ((carta->naipe >= 5) && (carta->naipe <= 6));
} // end cartaPreta()

// Função para verificar se as cores de duas cartas são diferentes
bool coresDiferentes(tCarta *carta1, tCarta *carta2)
{
    return ((cartaVermelha(carta1) && cartaPreta(carta2)) || (cartaPreta(carta1) && cartaVermelha(carta2)));
} // end coresDiferentes()

// Função para verificar se uma sequência de cartas está em ordem
bool sequenciaCorreta(tCarta *carta)
{
    while (carta->prox != NULL)
    {
        // Percorre a pilha a partir da carta passada comparando se são de cores diferentes e estão em ordem decrescente
        if ((coresDiferentes(carta, carta->prox)) && (carta->num - 1 == carta->prox->num))
        {
            carta = carta->prox;
        } // end if((coresDiferentes(carta, carta->prox)) && (carta->num - 1 == carta->prox->num))
        else
        {
            return false;
        } // end else
    }     // end while(carta->prox != NULL)

    return true;

} // end sequenciaCorreta()

// Função para gerar o monte
void gerarBaralho()
{
    tCarta *novo;
    int i, j;

    // Criação dos 4 naipes com 13 cartas cada
    for (i = 0; i < 4; i++)
    {
        for (j = 1; j <= 13; j++)
        {
            // Alocação de memória para a nova carta
            novo = (tCarta *)malloc(sizeof(tCarta));

            // A nova carta recebe um valor (0 a 13) e um naipe (copas, ouros, paus e espadas)
            novo->num = j;
            novo->naipe = i + 3;

            // A nova carta aponta para a que está no topo da pilha
            novo->prox = primMonte;

            // O primeiro elemento da pilha passa a ser a nova carta
            primMonte = novo;
        } // end for(j = 1; j <= 13; j++)
    }     // end for(i = 0; i < 4; i++)

} // end gerarBaralho()

// Função para embaralhar o monte de cartas
void embaralhaBaralho()
{
    tCarta *atual, *ant;
    int i, j, sort;

    // Realiza uma randomização quatro vezes maior que o número de cartas
    for (i = 0; i < 208; i++)
    {
        // Sorteia uma posição do monte
        sort = rand() % 52;
        ant = NULL;

        atual = primMonte;

        // Encontra a carta nessa posição
        for (j = 0; j < sort; j++)
        {
            ant = atual;
            atual = atual->prox;
        } // end for(j = 0; j < sort; j++)

        // Troca a carta de lugar com a primeira do monte
        if (sort > 0)
        {
            ant->prox = atual->prox;
            atual->prox = primMonte;
            primMonte = atual;
        } // end if(sort > 0)
    }     // end for(i = 0; i < 208; i++)

} // end embaralhaBaralho()

// Função para distribuir as cartas na mesa
void distribuirMesa()
{
    tCarta *auxMonte, *auxMesa;
    int i = 0;

    while (primMonte != NULL)
    {
        auxMonte = primMonte;
        primMonte = primMonte->prox;

        // Se for a primeira carta do monte da mesa
        if (primMesa[i] == NULL)
        {
            primMesa[i] = auxMonte;
            primMesa[i]->prox = NULL;
        } // end if(primMesa[i] == NULL)
        else
        {
            auxMesa = primMesa[i];
            primMesa[i] = auxMonte;
            primMesa[i]->prox = auxMesa;
        } // end else

        if (primMonte == NULL)
        {
            break;
        } // end if(primMonte == NULL)

        // Passa para o próximo monte da mesa, se chegar ao final volta ao primeiro e continua assim até acabar as cartas
        i = (i + 1) % 8;
    } // end while(primMonte != NULL)

} // end distribuirMesa()

// Função para imprimir o TEMP, o NAIPE e a MESA
void imprime()
{
    tCarta *atual;
    int i;

    textcolor(WHITE); // Muda a cor do texto para branca

    // Limpa o terminal
    system("cls");

    printf("::::::::::::::::::::::FREE CELL::::::::::::::::::::::\n");

    printf("[TEMP] = ");
    // Percorre as posições de TEMP (0 a 3)
    for (i = 0; i < TAM_TEMP; i++)
    {
        if (primTemp[i] != NULL)
        {
            printf("%d-[%02d/", i, primTemp[i]->num);
            // Imprime o texto do naipe com a cor correspondente
            if (cartaVermelha(primTemp[i]))
            {
                textcolor(RED);
            }
            else if (cartaPreta(primTemp[i]))
            {
                textcolor(BLACK);
            }
            printf("%c", primTemp[i]->naipe);
            textcolor(WHITE);
            printf("] ");
        } // end if(primTemp[i] != NULL)
        else
        {
            printf("%d-[    ] ", i);
        } // end else
    }     // end for(i = 0; i < TAM_TEMP; i++)
    printf("\n\n");

    // Percorre as posições de NAIPE (0 a 3)
    for (i = 0; i < TAM_NAIPE; i++)
    {
        atual = primNaipe[i];
        printf("[NAIPE %d] = ", i);
        while (atual != NULL)
        {
            printf("[%02d/", atual->num);
            if (cartaVermelha(atual))
            {
                textcolor(RED);
            }
            else if (cartaPreta(atual))
            {
                textcolor(BLACK);
            }
            printf("%c", atual->naipe);
            textcolor(WHITE);
            printf("]");

            atual = atual->prox;
        } // end while(atual != NULL)
        printf("\n");
    } // end for(i = 0; i < TAM_NAIPE; i++)
    printf("\n");

    // Percorre as posições de MESA (0 a 7)
    for (i = 0; i < TAM_MESA; i++)
    {
        atual = primMesa[i];
        printf("[MESA %d] = ", i);
        while (atual != NULL)
        {
            printf("[%02d/", atual->num);
            if (cartaVermelha(atual))
            {
                textcolor(RED);
            }
            else if (cartaPreta(atual))
            {
                textcolor(BLACK);
            }
            printf("%c", atual->naipe);
            textcolor(WHITE);
            printf("]");

            atual = atual->prox;
        } // end while(atual != NULL)
        printf("\n");
    } // end for(i = 0; i < TAM_MESA; i++)
    printf("\n");

} // end imprime()

// Função para mover da MESA para o TEMP
void moveMesaTemp()
{
    tCarta *ant, *atual;
    int posMesa, posTemp;

    // Inserção da posição da mesa pelo usuário
    printf("Digite a posicao da MESA na qual esta a carta que deseja mover (0 - 7): ");
    scanf("%d", &posMesa);

    // Verifica se a posição da mesa é válida
    if ((posMesa >= 0) && (posMesa <= 7))
    {
        // Verifica se há carta para mover
        if (primMesa[posMesa] != NULL)
        {
            // Inserção da posição de temp pelo usuário
            printf("Digite a posicao de TEMP para qual deseja mover a carta (0 - 3): ");
            scanf("%d", &posTemp);

            // Percorre a mesa até o último elemento
            ant = NULL;
            atual = primMesa[posMesa];

            while (atual->prox != NULL)
            {
                ant = atual;
                atual = atual->prox;
            } // end while(atual->prox != NULL)

            // Verifica se a posição de temp é válida
            if ((posTemp >= 0) && (posTemp <= 3))
            {
                // Verifica se a posição de temp está vazia
                if (primTemp[posTemp] == NULL)
                {
                    primTemp[posTemp] = atual;

                    if (ant == NULL)
                    {
                        primMesa[posMesa] = NULL;
                    } // end if(ant == NULL)
                    else
                    {
                        ant->prox = NULL;
                    } // end else
                }     // end if(primTemp[posTemp] == NULL)
                else
                {
                    system("cls");
                    printf(":::Posicao %d de TEMP nao disponivel!:::\n\n", posTemp);
                    system("pause");
                } // end else
            }     // end if((posTemp >= 0) && (posTemp <= 3))
            else
            {
                mensagem(":::Posicao de TEMP invalida!:::");
            } // end else
        }     // end if(primMesa[posMesa] != NULL)
        else
        {
            system("cls");
            printf(":::Sem carta na pilha %d de MESA:::\n\n", posMesa);
            system("pause");
        } // end else
    }     // end if((posMesa >= 0) && (posMesa <= 7))
    else
    {
        mensagem(":::Posicao da MESA invalida!:::");
    } // end else

} // end moveMesaTemp()

// Função para mover de TEMP para a MESA
void moveTempMesa()
{
    tCarta *atual;
    int posMesa, posTemp;

    // Inserção da posição de temp pelo usuário
    printf("Digite a posicao de TEMP na qual esta a carta que deseja mover (0 - 3): ");
    scanf("%d", &posTemp);

    // Verifica se a posição de temp é válida
    if ((posTemp >= 0) && (posTemp <= 3))
    {
        // Verifica se há uma carta para mover
        if (primTemp[posTemp] != NULL)
        {
            // Inserção da posição da mesa pelo usuário
            printf("Digite a posicao da MESA para qual deseja mover a carta (0 - 7): ");
            scanf("%d", &posMesa);

            // Verifica se a posição da mesa é válida
            if ((posMesa >= 0) && (posMesa <= 7))
            {
                // Se o destino da carta na mesa é uma pilha vazia
                if (primMesa[posMesa] == NULL)
                {
                    // Verifica se a carta que está sendo movida é a 13 (rei)
                    if (primTemp[posTemp]->num == 13)
                    {
                        // Atualiza a pilha de temp e coloca a carta na pilha da mesa
                        primMesa[posMesa] = primTemp[posTemp];
                        primTemp[posTemp] = NULL;
                    } // end if(primTemp[posTemp]->num == 13)
                    else
                    {
                        mensagem(":::Mesas vazias so aceitam a carta 13 (rei)!:::");
                    } // end else
                }     // end if(primMesa[posMesa] == NULL)
                else
                {
                    // Percorre a mesa até achar o último elemento
                    atual = primMesa[posMesa];

                    while (atual->prox != NULL)
                    {
                        atual = atual->prox;
                    } // end while(atual->prox != NULL)

                    // Verifica se a carta que se deseja mover está na sequência da mesa
                    if ((coresDiferentes(primTemp[posTemp], atual)) && (atual->num - 1 == primTemp[posTemp]->num))
                    {
                        atual->prox = primTemp[posTemp];
                        primTemp[posTemp] = NULL;
                    } // end if()
                    else
                    {
                        mensagem(":::A carta a ser movida deve ser de uma cor diferente e na sequencia da que esta no topo do destino!:::");
                    } // end else
                }     // end else
            }         // end if((posMesa >= 0) && (posMesa <= 7))
            else
            {
                mensagem(":::Posicao invalida da MESA!:::");
            } // end else
        }     // end if(primTemp[posTemp] != NULL)
        else
        {
            system("cls");
            printf(":::Sem carta na posicao %d de TEMP!:::\n\n", posTemp);
            system("pause");
        } // end else
    }     // end if((posTemp >= 0) && (posTemp <= 3))
    else
    {
        mensagem(":::Posicao invalida de TEMP!:::");
    } // end else

} // end moveTempMesa()

// Função para mover da MESA para o NAIPE
void moveMesaNaipe()
{
    tCarta *ant, *atual;
    int posMesa, posNaipe;

    // Inserção da posição da mesa pelo usuário
    printf("Digite a posicao da MESA na qual esta a carta que deseja mover (0 - 7): ");
    scanf("%d", &posMesa);

    // Verifica se a posição da mesa é válida
    if ((posMesa >= 0) && (posMesa <= 7))
    {
        // Verifica se há uma carta para mover
        if (primMesa[posMesa] != NULL)
        {
            // Inserção da posição de naipe pelo usuário
            printf("Digite a posicao de NAIPE para qual deseja mover a carta (0 - 3): ");
            scanf("%d", &posNaipe);

            // Percorre a mesa até achar o último elemento
            ant = NULL;
            atual = primMesa[posMesa];

            while (atual->prox != NULL)
            {
                ant = atual;
                atual = atual->prox;
            } // end while(atual->prox != NULL)

            // Verifica se a posição do naipe é válida
            if ((posNaipe >= 0) && (posNaipe <= 3))
            {
                // Verifica se a carta que se deseja mover está na sequência do naipe
                if (((atual->num == 1) && (primNaipe[posNaipe] == NULL)) ||                                                                            // Se a pilha estiver vazia a primeira carta deve ser o ás
                    (((primNaipe[posNaipe] != NULL) && (atual->naipe == primNaipe[posNaipe]->naipe) && (atual->num - 1 == primNaipe[posNaipe]->num)))) // Caso contrário o naipe deve ser o mesmo e o número o próximo da sequência
                {
                    // Se a mesa tiver somente um elemento
                    if (ant == NULL)
                    {
                        primMesa[posMesa] = NULL;
                    } // end if(ant == NULL)
                    else
                    {
                        ant->prox = NULL;
                    } // end else

                    // Se o naipe estiver vazio
                    if (primNaipe[posNaipe] == NULL)
                    {
                        // A carta passa a ser a primeira do naipe
                        primNaipe[posNaipe] = atual;
                    } // end if(primNaipe[posNaipe] == NULL)
                    else
                    {
                        // Move a carta
                        atual->prox = primNaipe[posNaipe];
                        primNaipe[posNaipe] = atual;
                    } // end else()
                }     // end if()
                else
                {
                    mensagem(":::A carta a ser movida deve ser do mesmo naipe e a proxima da sequencia!:::");
                } // end else
            }     // end if((posNaipe >= 0) && (posNaipe <= 3))
            else
            {
                mensagem(":::Posicao de NAIPE invalida!:::");
            } // end else
        }     // end if(primMesa[posMesa] != NULL)
        else
        {
            system("cls");
            printf(":::Sem carta na pilha %d de MESA!:::\n\n", posMesa);
            system("pause");
        } // end else
    }     // end if((posMesa >= 0) && (posMesa <= 7))
    else
    {
        mensagem(":::Posicao da MESA invalida!:::");
    } // end else

} // end moveMesaNaipe()

// Função para mover de NAIPE para o TEMP
void moveNaipeTemp()
{
    int posNaipe, posTemp;

    // Inserção da posição do naipe pelo usuário
    printf("Digite a posicao de NAIPE na qual esta a carta que deseja mover (0 - 3): ");
    scanf("%d", &posNaipe);

    // Verifica se a posição do naipe é válida
    if ((posNaipe >= 0) && (posNaipe <= 3))
    {
        // Verifica se há carta para mover
        if (primNaipe[posNaipe] != NULL)
        {
            // Inserção da posição de temp pelo usuário
            printf("Digite a posicao de TEMP para qual deseja mover a carta (0 - 3): ");
            scanf("%d", &posTemp);

            // Verifica se a posição de temp é válida
            if ((posTemp >= 0) && (posTemp <= 3))
            {
                // Verifica se a posição de temp está vazia
                if (primTemp[posTemp] == NULL)
                {
                    // Move a carta
                    primTemp[posTemp] = primNaipe[posNaipe];
                    primNaipe[posNaipe] = primNaipe[posNaipe]->prox;
                    primTemp[posTemp]->prox = NULL;
                } // end if(primTemp[posTemp] == NULL)
                else
                {
                    system("cls");
                    printf(":::Posicao %d de TEMP nao disponivel!:::\n\n", posTemp);
                    system("pause");
                } // end else
            }     // end if((posTemp >= 0) && (posTemp <= 3))
            else
            {
                mensagem(":::Posicao de TEMP invalida!:::");
            } // end else
        }     // end if(primNaipe[posNaipe] != NULL)
        else
        {
            system("cls");
            printf(":::Sem carta na pilha %d de NAIPE!:::\n\n", posNaipe);
            system("pause");
        } // end else
    }     // end if((posNaipe >= 0) && (posNaipe <= 3))
    else
    {
        mensagem(":::Posicao de NAIPE invalida!:::");
    } // end else

} // end moveNaipeTemp()

// Função para mover do TEMP para o NAIPE
void moveTempNaipe()
{
    int posTemp, posNaipe;

    // Inserção da posição de temp pelo usuário
    printf("Digite a posicao da TEMP na qual esta a carta que deseja mover (0 - 3): ");
    scanf("%d", &posTemp);

    // Verifica se a posição de temp é válida
    if ((posTemp >= 0) && (posTemp <= 3))
    {
        // Verifica se há carta para mover
        if (primTemp[posTemp] != NULL)
        {
            // Inserção da posição de naipe pelo usuário
            printf("Digite a posicao de NAIPE para qual deseja mover a carta (0 - 3): ");
            scanf("%d", &posNaipe);

            // Verifica se a posição de naipe é válida
            if ((posNaipe >= 0) && (posNaipe <= 3))
            {
                // Verifica se a carta que se deseja mover está na sequência do naipe
                if (((primTemp[posTemp]->num == 1) && (primNaipe[posNaipe] == NULL)) ||                                                                                        // Se a pilha estiver vazia a primeira carta deve ser o ás
                    (((primNaipe[posNaipe] != NULL) && (primTemp[posTemp]->naipe == primNaipe[posNaipe]->naipe) && (primTemp[posTemp]->num - 1 == primNaipe[posNaipe]->num)))) // Caso contrário o naipe deve ser o mesmo e o número o próximo da sequência
                {
                    // Move a carta
                    primTemp[posTemp]->prox = primNaipe[posNaipe];
                    primNaipe[posNaipe] = primTemp[posTemp];
                    primTemp[posTemp] = NULL;
                } // end if()
                else
                {
                    mensagem(":::A carta a ser movida deve ser do mesmo naipe e a proxima da sequencia!:::");
                } // end else
            }     // end if((posNaipe >= 0) && (posNaipe <= 3))
            else
            {
                mensagem(":::Posicao de NAIPE invalida!:::");
            } // end else
        }     // end if(primTemp[posTemp] != NULL)
        else
        {
            system("cls");
            printf(":::Sem carta na pilha %d de TEMP!:::\n\n", posTemp);
            system("pause");
        } // end else
    }     // end if((posTemp >= 0) && (posTemp <= 3))
    else
    {
        mensagem(":::Posicao de TEMP invalida!:::");
    } // end else

} // end moveTempNaipe()

// Função para mover do NAIPE para a MESA
void moveNaipeMesa()
{
    tCarta *atual;
    int posMesa, posNaipe;

    // Inserção da posição do naipe pelo usuário
    printf("Digite a posicao do NAIPE na qual esta a carta que deseja mover (0 - 3): ");
    scanf("%d", &posNaipe);

    // Verifica se a posição do naipe é válida
    if ((posNaipe >= 0) && (posNaipe <= 3))
    {
        // Verifica se há carta para mover
        if (primNaipe[posNaipe] != NULL)
        {
            // Inserção da posição da mesa pelo usuário
            printf("Digite a posicao da MESA para qual deseja mover a carta (0 - 7): ");
            scanf("%d", &posMesa);

            // Verifica se a posição da mesa é válida
            if ((posMesa >= 0) && (posMesa <= 7))
            {
                // Percorre a mesa até achar o último elemento
                atual = primMesa[posMesa];

                // Se a mesa não está vazia
                if (atual != NULL)
                {
                    while (atual->prox != NULL)
                    {
                        atual = atual->prox;
                    } // end while(atual->prox != NULL)
                }     // end if(atual != NULL)

                // Se a mesa estiver vazia
                if (primMesa[posMesa] == NULL)
                {
                    // Verifica se a carta que está sendo movida é a 13 (rei)
                    if (primNaipe[posNaipe]->num == 13)
                    {
                        // A carta passa a ser a primeira da mesa
                        primMesa[posMesa] = primNaipe[posNaipe];
                        primNaipe[posNaipe] = primNaipe[posNaipe]->prox;
                        primMesa[posMesa]->prox = NULL;
                    } // end if(primNaipe[posNaipe]->num == 13)
                    else
                    {
                        mensagem(":::Mesas vazias so aceitam a carta 13 (rei)!:::");
                    } // end else
                }     // end if(primMesa[posMesa] == NULL)
                // Verifica se a carta que se deseja mover está na sequência da mesa
                else if ((coresDiferentes(primNaipe[posNaipe], atual)) && (atual->num - 1 == primNaipe[posNaipe]->num))
                {
                    // Move a carta
                    atual->prox = primNaipe[posNaipe];
                    primNaipe[posNaipe] = primNaipe[posNaipe]->prox;
                    atual->prox->prox = NULL;
                } // end else if()
                else
                {
                    mensagem(":::A carta a ser movida deve ser de uma cor diferente e na sequencia da que esta no destino!:::");
                } // end else
            }     // end if((posMesa >= 0) && (posMesa <= 7))
            else
            {
                mensagem(":::Posicao da MESA invalida!:::");
            } // end else
        }     // end if(primNaipe[posNaipe] != NULL)
        else
        {
            system("cls");
            printf(":::Sem carta na pilha %d de NAIPE!:::\n\n", posNaipe);
            system("pause");
        } // end else
    }     // end if((posNaipe >= 0) && (posNaipe <= 3))
    else
    {
        mensagem(":::Posicao de NAIPE invalida!:::");
    } // end else

} // end moveNaipeMesa()

// Função para mover de uma MESA para outra
void moveMesaMesa()
{
    tCarta *atualOrigem, *antOrigem, *atualDestino;
    int posOrigemMesa, posDestinoMesa, movimento, quant = 0, indice, i = 0;

    // Inserção da posição da mesa de origem pelo usuário
    printf("Digite a posicao da MESA de origem (0 - 7): ");
    scanf("%d", &posOrigemMesa);

    // Verifica se a posição da mesa de origem é válida
    if ((posOrigemMesa >= 0) && (posOrigemMesa <= 7))
    {
        // Verifica se há carta para mover
        if (primMesa[posOrigemMesa] != NULL)
        {
            // Inserção da posição da mesa de destino pelo usuário
            printf("Digite a posicao da MESA de destino (0 - 7): ");
            scanf("%d", &posDestinoMesa);

            // Verifica se a posição da mesa de destino é válida
            if ((posDestinoMesa >= 0) && (posDestinoMesa <= 7))
            {
                // Percorre a mesa de destino até achar o último elemento
                atualDestino = primMesa[posDestinoMesa];

                // Se a mesa de destino não está vazia
                if (primMesa[posDestinoMesa] != NULL)
                {
                    while (atualDestino->prox != NULL)
                    {
                        atualDestino = atualDestino->prox;
                    } // end while(atualDestino->prox != NULL)
                }     // end if(primMesa[posDestinoMesa] != NULL)

                // Inserção do tipo de movimento pelo usuário
                printf("(0) - Mover uma carta\n(1) - Mover um bloco\nEscolha uma opcao: ");
                scanf("%d", &movimento);

                // Verifica se a opção é válida
                if (movimento == 0)
                {
                    // Percorre a mesa de origem até achar o último elemento
                    antOrigem = NULL;
                    atualOrigem = primMesa[posOrigemMesa];

                    while (atualOrigem->prox != NULL)
                    {
                        antOrigem = atualOrigem;
                        atualOrigem = atualOrigem->prox;
                    } // end while(atualOrigem->prox != NULL)

                    // Verifica a mesa de destino está vazia
                    if (primMesa[posDestinoMesa] == NULL)
                    {
                        // Verifica se a carta que está sendo movida é a 13 (rei)
                        if (atualOrigem->num == 13)
                        {
                            // A carta passa a ser a primeira da mesa
                            primMesa[posDestinoMesa] = atualOrigem;
                            antOrigem->prox = NULL;
                        } // end if(atualOrigem->num == 13)
                        else
                        {
                            mensagem(":::Mesas vazias so aceitam a carta 13 (rei)!:::");
                        } // end else
                    }     // end if(primMesa[posDestinoMesa] == NULL)
                    // Verifica se a carta que se deseja mover está na sequência da mesa de destino
                    else if ((coresDiferentes(atualOrigem, atualDestino)) && (atualDestino->num - 1 == atualOrigem->num))
                    {
                        // Se há apenas uma carta na origem
                        if (antOrigem == NULL)
                        {
                            // Move a carta
                            atualDestino->prox = atualOrigem;
                            primMesa[posOrigemMesa] = NULL;
                        } // end if(antOrigem == NULL)
                        else
                        {
                            // Move a carta
                            atualDestino->prox = atualOrigem;
                            antOrigem->prox = NULL;
                        } // end else
                    }     // end else if()
                    else
                    {
                        mensagem(":::A carta a ser movida deve ser de uma cor diferente e na sequencia da que esta no destino!:::");
                    } // end else
                }     // end if(movimento == 0)
                else if (movimento == 1)
                {
                    // Percorre a origem verificando a quantidade de elementos disponíveis
                    atualOrigem = primMesa[posOrigemMesa];

                    while (atualOrigem->prox != NULL)
                    {
                        quant++;
                        atualOrigem = atualOrigem->prox;
                    } // end while(atualOrigem->prox != NULL)

                    // Inserção da posição que inicia o bloco que o usuário quer mover
                    printf("Digite o indice da carta que inicia o bloco que deseja mover (0 - %d): ", quant);
                    scanf("%d", &indice);

                    // Verifica se o índice é válido
                    if ((indice >= 0) && (indice <= quant))
                    {
                        // Percorre a mesa até o índice passado pelo usuário
                        antOrigem = NULL;
                        atualOrigem = primMesa[posOrigemMesa];

                        while (i != indice)
                        {
                            antOrigem = atualOrigem;
                            atualOrigem = atualOrigem->prox;
                            i++;
                        } // end while(i != indice)

                        // Verifica se as cartas do bloco estão na sequência correta
                        if (sequenciaCorreta(atualOrigem))
                        {
                            // Se a mesa de destino está vazia
                            if (primMesa[posDestinoMesa] == NULL)
                            {
                                // Verifica se a carta do topo que está sendo movida é a 13 (rei)
                                if (atualOrigem->num == 13)
                                {
                                    // Se a mesa de origem possuía somente um elemento
                                    if (antOrigem == NULL)
                                    {
                                        // Move o bloco
                                        primMesa[posDestinoMesa] = atualOrigem;
                                        primMesa[posOrigemMesa] = NULL;
                                    } // end if(antOrigem == NULL)
                                    else
                                    {
                                        // Move o bloco
                                        primMesa[posDestinoMesa] = atualOrigem;
                                        antOrigem->prox = NULL;
                                    } // end else
                                }     // end if(atualOrigem->num == 13)
                                else
                                {
                                    mensagem(":::Mesas vazias so aceitam a carta 13 (rei)!:::");
                                } // end else
                            }     // end if(primMesa[posDestinoMesa] == NULL)
                            // Verifica se a carta do início do bloco que se deseja mover está na sequência da mesa de destino
                            else if ((coresDiferentes(atualOrigem, atualDestino)) && (atualDestino->num - 1 == atualOrigem->num))
                            {
                                // Se há apenas uma carta na origem
                                if (antOrigem == NULL)
                                {
                                    // Move a carta
                                    atualDestino->prox = atualOrigem;
                                    primMesa[posOrigemMesa] = NULL;
                                } // end if(antOrigem == NULL)
                                else
                                {
                                    // Move a carta
                                    atualDestino->prox = atualOrigem;
                                    antOrigem->prox = NULL;
                                } // end else
                            }     // end else if()
                            else
                            {
                                mensagem(":::A carta do topo do bloco a ser movido deve ser de uma cor diferente e na sequencia da que esta no destino!:::");
                            } // end else
                        }     // end if(sequenciaCorreta(atualOrigem))
                        else
                        {
                            mensagem(":::As cartas devem estar na sequencia correta para serem movidas!:::");
                        } // end else
                    }     // end else if((indice >= 0) && (indice <= quant))
                    else
                    {
                        mensagem(":::Indice invalido!:::");
                    } // end else
                }     // end else if(movimento == 1)
                else
                {
                    mensagem(":::Insira uma opcao valida!:::");
                } // end else
            }     // end if((posDestinoMesa >= 0) && (posDestinoMesa <= 7))
            else
            {
                mensagem(":::Posicao da MESA de destino invalida!:::");
            } // end else
        }     // end if(primMesa[posOrigemMesa] != NULL)
        else
        {
            system("cls");
            printf(":::Sem carta na pilha %d da MESA de origem!:::\n\n", posOrigemMesa);
            system("pause");
        } // end else
    }     // end if((posOrigemMesa >= 0) && (posOrigemMesa <= 7))
    else
    {
        mensagem(":::Posicao da MESA de origem invalida!:::");
    } // end else

} // end moveMesaMesa()

// Função para destruir as pilhas
void destroiTudo()
{
    tCarta *atual;
    tCarta *apaga;
    int i;

    // Percorre as posições de MESA (0 a 7)
    for (i = 0; i < TAM_MESA; i++)
    {
        // Começa pelo topo da pilha
        atual = primMesa[i];
        while (atual != NULL)
        {
            // Apaga recebe o ponteiro que terá a memória liberada
            apaga = atual;
            // Passa para a próxima carta da pilha
            atual = atual->prox;
            // Libera a memória
            free(apaga);
        } // end while(atual != NULL)
    }     // end for(i = 0; i < TAM_MESA; i++)

    // Percorre as posições de NAIPE (0 a 3)
    for (i = 0; i < TAM_NAIPE; i++)
    {
        atual = primNaipe[i];
        while (atual != NULL)
        {
            apaga = atual;
            atual = atual->prox;
            free(apaga);
        } // end while(atual != NULL)
    }     // end for(i = 0; i < TAM_NAIPE; i++)

    // Percorre as posições de TEMP (0 a 3)
    for (i = 0; i < TAM_TEMP; i++)
    {
        atual = primTemp[i];
        while (atual != NULL)
        {
            apaga = atual;
            atual = atual->prox;
            free(apaga);
        } // end while(atual != NULL)
    }     // end for(i = 0; i < TAM_TEMP; i++)

} // end destroiTudo()

// Função para reiniciar o jogo
void reiniciar()
{
    int i;

    // Libera a memória das cartas criadas
    destroiTudo();

    // Inicializa os montes
    primMonte = NULL;

    // Percorre as posições de MESA (0 a 7)
    for (i = 0; i < TAM_MESA; i++)
    {
        primMesa[i] = NULL;
    } // end for(i = 0; i < TAM_MESA; i++)

    // Percorre as posições de NAIPE (0 a 3)
    for (i = 0; i < TAM_NAIPE; i++)
    {
        primNaipe[i] = NULL;
    } // end for(i = 0; i < TAM_NAIPE; i++)

    // Percorre as posições de TEMP (0 a 3)
    for (i = 0; i < TAM_TEMP; i++)
    {
        primTemp[i] = NULL;
    } // end for(i = 0; i < TAM_TEMP; i++)

    // Gera o baralho, embaralha ele e distribui nas mesas
    gerarBaralho();
    embaralhaBaralho();
    distribuirMesa();

} // end reiniciar()

// Função principal
int main(int argc, char **argv)
{
    int op = 0;

    backcolor(GREEN); // Cor de fundo do terminal muda para verde

    // Inicia uma semente aleatória para a função rand()
    srand(time(NULL));

    // Gera o baralho
    gerarBaralho();

    // Embaralha o baralho
    embaralhaBaralho();

    // Distribui as cartas na mesa
    distribuirMesa();

    while (op != 9)
    {
        // Imprime TEMP, NAIPE e MESA
        imprime();

        textcolor(WHITE); // Cor do texto branca

        // Menu
        printf("(1) - Mover: MESA --> TEMP\n");
        printf("(2) - Mover: MESA --> NAIPE\n");
        printf("(3) - Mover: TEMP --> MESA\n");
        printf("(4) - Mover: NAIPE --> TEMP\n");
        printf("(5) - Mover: TEMP --> NAIPE\n");
        printf("(6) - Mover: NAIPE --> MESA\n");
        printf("(7) - Mover: MESA --> MESA\n");
        printf("(8) - Reiniciar\n");
        printf("(9) - Sair\n");

        // Inserção da opção pelo usuário
        printf("Opcao: ");
        scanf("%d", &op);

        getchar();

        // Dependendo da opção realiza uma ação
        switch (op)
        {
        case 1:
            // Mover de MESA para TEMP
            moveMesaTemp();
            break;
        case 2:
            // Mover de MESA para NAIPE
            moveMesaNaipe();
            break;
        case 3:
            // Mover de TEMP para MESA
            moveTempMesa();
            break;
        case 4:
            // Mover de NAIPE para TEMP
            moveNaipeTemp();
            break;
        case 5:
            // Mover de TEMP para NAIPE
            moveTempNaipe();
            break;
        case 6:
            // Mover de NAIPE para MESA
            moveNaipeMesa();
            break;
        case 7:
            // Mover de Mesa para MESA
            moveMesaMesa();
            break;
        case 8:
            // Reiniciar
            reiniciar();
            break;
        case 9:
            // Sair
            printf("\nSaindo...");
            break;
        default:
            // Opção fora do menu
            printf("\n:::Opcao Invalida!:::\n");
            break;
        } // end switch(op)
    }     // end while(op != 4)

    // Liberação da memória
    destroiTudo();

    return 0;

} // end main()
