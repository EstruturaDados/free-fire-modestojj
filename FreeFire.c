#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 10

typedef struct  {
char nome[30];
char tipo[20];
int quantidade;
}item;

typedef struct {
    item itens [MAX_ITENS];
    int quantidade;
}mochila;


void iniciarMochila(mochila *itemMochila);

void adicionarItem(mochila *itemMochila, const char* nome, const char* tipo, int qnt);

void removerItem(mochila *itemMochila, const char* nome, const char* tipo);

void listarItens(const mochila *itemMochila);

void statusMochila(const mochila *itemMochila);

void buscarItemNome(const mochila *itemMochila);

void menuItem(mochila *itemMochila);

//--------------------main-----------------------------------
int main() {
    mochila itemMochila;
    iniciarMochila(&itemMochila);

    int opcao;
    do {
        printf("\n==============================================\n");
        printf("\n---MOCHILA DE SOBREVIVENCIA--CODIGO DA ILHA---\n");
        printf("\n==============================================\n");
        printf("1. Gerenciar Itens\n");
        printf("0. Sair do programa\n");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            menuItem(&itemMochila);
            break;
        case 0:
            printf("Saindo...\n");
            break;
        default:
            printf("Opção Invalida!");
            break;
        }
    } while (opcao != 0);
    return 0;

}

void iniciarMochila(mochila *itemMochila) {
    itemMochila->quantidade = 0;
}

void adicionarItem(mochila *itemMochila, const char* nome, const char* tipo, int qnt){
    if (itemMochila->quantidade >= MAX_ITENS) {
        printf("A mochila esta cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }

    strcpy(itemMochila->itens[itemMochila->quantidade].nome, nome);
    strcpy(itemMochila->itens[itemMochila->quantidade].tipo, tipo);
    itemMochila->itens[itemMochila->quantidade].quantidade = qnt;
    //apos a copia itemMochila->quantidade é incremento pois agora temos mais um item na lista 
    itemMochila->quantidade++;
    printf("Item \"%s\" (%d unidades) inserido com sucesso.\n", nome, qnt);

    statusMochila(itemMochila);
}

void removerItem(mochila *itemMochila, const char* nome, const char* tipo) {
    int i = 0, pos = -1;
    //para compara string usamos strcmp(). retorna 0 se forem iguais 
    //a funcao percorre os itens em uso na lista (de 0 ate quantidade)
    //para compara string usamos strcmp(string1, string2)******importante********
    //essa funcao retorna 0 se as strings forem identicas. se encontrar guarda a posicao (pos) e para o loop
    for (i = 0; i < itemMochila->quantidade; i++) {
        if(strcmp(itemMochila->itens[i].nome, nome) == 0 && strcmp(itemMochila->itens[i].tipo, tipo) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
       printf("Item \"%s\" do tipo \"%s\" nao encontrado na mochila.\n", nome, tipo);

        return;
    }

        //fecha a lacuna: se removemo um item do meio da lista criamos um buraco
    /* para preenchelo movemos todos os itens subsequuentes uma posicao para a esquerda
        o loop for comecar na posicao do item removido e copia cada item i+1 para a posicao i
        deslocar os elementos posteriores para a esquerda usando strcpy */
        for (i = pos; i < itemMochila->quantidade -1; i++) {
            strcpy(itemMochila->itens[i].nome, itemMochila->itens[i + 1].nome);
            strcpy(itemMochila->itens[i].tipo, itemMochila->itens[i + 1].tipo);
            itemMochila->itens[i].quantidade = itemMochila->itens[i + 1].quantidade;
        }

         //atualizacao do contador finalmente quantidade e decrementado para refletir que a lista tem um item a menos 
        itemMochila->quantidade--;
        printf("Item \"%s\" do tipo \"%s\" removido com sucesso.\n", nome, tipo);

            statusMochila(itemMochila);

}

void listarItens(const mochila *itemMochila) {
    if (itemMochila->quantidade == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }

    printf("====Itens na Mochila====\n");
    printf("--------------------------------------------\n");
    printf("Nome|\t\t|Tipo\t\t|Quantidade\n");
    printf("--------------------------------------------\n");
    for (int i = 0; i < itemMochila->quantidade; i ++) {
        printf("%-10s\t%-10s\t%d\n",
       itemMochila->itens[i].nome,
       itemMochila->itens[i].tipo,
       itemMochila->itens[i].quantidade);

    }
}

void buscarItemNome(const mochila *itemMochila) {
    char nomeBuscar[30];
    printf("Digite o nome do item a buscar: ");
    scanf(" %29[^\n]", nomeBuscar);
    int encontrado = 0;

    // Busca Sequencial: percorre item por item até encontrar
    for (int i = 0; i < itemMochila->quantidade; i++) {
        // Compara o nome do item atual com o nome procurado
        if (strcmp(itemMochila->itens[i].nome, nomeBuscar) == 0) {
            printf("\n====Item Encontrado!====\n");
            printf("Nome: %s\n", itemMochila->itens[i].nome);
            printf("Tipo: %s\n", itemMochila->itens[i].tipo);
            printf("Quantidade: %d\n", itemMochila->itens[i].quantidade);
            printf("Posicao na mochila: %d\n", i + 1);
            encontrado = 1;
            break;  // Para a busca após encontrar
        }
    }
    
    // Se saiu do loop sem encontrar
    if (encontrado == 0) {
        printf("\nItem \"%s\" nao foi encontrado na mochila.\n", nomeBuscar);
    }
}

void statusMochila(const mochila *itemMochila) {
    printf("\n[ Mochila: %d/%d itens ]\n",
           itemMochila->quantidade,
           MAX_ITENS);
}


void menuItem(mochila *itemMochila) {
    int opcao;
    char nome[30];
    char tipo[20];
    int quantidade;
    do {
        //statusMochila(itemMochila);

        printf("\n--- Menu Itens da Mochila ---\n");
        printf("1. Adicionar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Item Nome\n");
        printf("0. Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                printf("Digite o nome do item (loot): ");
                scanf(" %29[^\n]", nome);
                printf("Digite o tipo do item a inserir(ex: cura, munição, etc): ");
                scanf(" %19[^\n]", tipo);
                printf("Digite a quantidade de munições/unidades: ");
                scanf("%d", &quantidade);
                adicionarItem(itemMochila, nome, tipo, quantidade);
                break;
            case 2:
                printf("Digite o nome do item (loot) a remover: ");
                scanf(" %29[^\n]", nome);
                printf("Digite o tipo do item a remover(ex: cura, munição, etc): ");
                scanf(" %19[^\n]", tipo);
                removerItem(itemMochila, nome, tipo);
                break;
            case 3:
                listarItens(itemMochila);
                break;
            case 4:
            buscarItemNome(itemMochila);
                break;
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);
}





