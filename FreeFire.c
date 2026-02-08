#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ITENS 10

typedef struct  {
char nome[30];
char tipo[20];
int quantidade;
int prioridade; //campo adicional para demonstrar ordenacao por prioridade
}item;

typedef struct {
    int comparacoes;
    double tempo_ms;
    char algoritmo[30];
}desempenho;

typedef struct {
    item itens [MAX_ITENS];
    int quantidade;//contador para acompanhar quantos itens estão atualmente na mochila
    desempenho ultimo_desempenho; //armazena as metricas de desempenho da ultima operacao de ordenacao
}mochila;


void iniciarMochila(mochila *itemMochila);//funcao para inicializar a mochila, definindo a quantidade de itens como 0

void adicionarItem(mochila *itemMochila, const char* nome, const char* tipo, int qnt, int prioridade);//funcao para adicionar um item a mochila, recebe o nome, tipo e quantidade do item a ser adicionado

void removerItem(mochila *itemMochila, const char* nome, const char* tipo);//funcao para remover um item da mochila, 
//recebe o nome e tipo do item a ser removido

void listarItens(const mochila *itemMochila);//funcao para listar os itens atuais da mochila

void statusMochila(const mochila *itemMochila);//funcao para exibir o status atual da mochila (quantidade de itens)

//void buscarItemNome(const mochila *itemMochila);//funcao para buscar um item pelo nome usando busca sequencial

void organizarMochilaNome(mochila *itemMochila);//funcao para organizar os itens da mochila em ordem alfabetica usando bubble sort

void organizarMochilaTipo(mochila *itemMochila);//funcao para organizar os itens da mochila por tipo usando insertion sort

void organizarMochilaPrioridade(mochila *itemMochila);//funcao para organizar os itens da mochila por prioridade usando selection sort

void buscaBinariaNome(const mochila *itemMochila); //const porque a busca nao modifica a mochila, apenas consulta os dados

void exibirMetricasDesempenho(const desempenho *d);//funcao para exibir as metricas de desempenho

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

void adicionarItem(mochila *itemMochila, const char* nome, const char* tipo, int qnt, int prioridade){
    if (itemMochila->quantidade >= MAX_ITENS) {
        printf("Inventario cheio! Nao e possivel adicionar mais itens.\n");
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

    //organizar a exibição dos itens em formato de tabela para melhor visualização
    printf("====Itens na Mochila====\n");
    printf("-------------------------------------------------------------\n");
    printf("Nome|\t\t|Tipo\t\t|Quantidade\t|Prioridade\n");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < itemMochila->quantidade; i ++) {
        printf("%-10s\t|%-10s\t|%-10d\t|%-10d\n",
       itemMochila->itens[i].nome,
       itemMochila->itens[i].tipo,
       itemMochila->itens[i].quantidade,
       itemMochila->itens[i].prioridade);

    }
}


//busca binaria por nome, exige que a mochila esteja organizada por nome (alfabetica)
void buscaBinariaNome(const mochila *itemMochila) {
    //mensagem de erro se nao estiver organizado por nome (alfabetica)
    if (itemMochila->quantidade == 0) {
        printf("A mochila esta vazia. Organize a mochila por nome (ordem alfabetica) antes de usar a busca binaria.\n");
        return;
    }
    //clock_t inicio = clock(); serve para medir o tempo de execucao da busca binaria, armazenando o tempo de inicio da busca. 
    //depois da busca, o tempo de fim e calculado e a diferenca convertida
    //variavel para armazenar o tempo de inicio da busca
    clock_t inicio = clock();
    int comparacoes = 0;
    
    char nomeBuscar[30];
    printf("Digite o nome do item a buscar: ");
    scanf(" %29[^\n]", nomeBuscar);//%29[^\n] para ler uma string com espacos e evitar overflow
    int esquerda = 0;//
    int direita = itemMochila->quantidade - 1;//direita e o indice do ultimo item na mochila (quantidade - 1)
    int encontrado = 0;//variavel para indicar se o item foi encontrado
    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;//calculo do indice do meio para evitar overflow
        int comparacao = strcmp(itemMochila->itens[meio].nome, nomeBuscar);//compara o nome do item do meio com o nome buscado
        comparacoes++; //incrementa contador de comparacoes
        if (comparacao == 0) {
            //item encontrado
            printf("\n====ITEM ENCONTRADO!====\n");
            printf("Nome: %s\n", itemMochila->itens[meio].nome);
            printf("Tipo: %s\n", itemMochila->itens[meio].tipo);
            printf("Quantidade: %d\n", itemMochila->itens[meio].quantidade);
            printf("Prioridade: %d\n", itemMochila->itens[meio].prioridade);
            printf("Posicao na mochila: %d (COMPONENTE-CHAVE CONFIRMADO!)\n", meio + 1);
            encontrado = 1;
            break;
        } else if (comparacao < 0) {
            //o nome do item do meio e menor que o nome buscado, entao o item buscado esta na metade direita
            esquerda = meio + 1;
        } else {
            //o nome do item do meio e maior que o nome buscado, entao o item buscado esta na metade esquerda
            direita = meio - 1;
        }
    }
    if (encontrado == 0) {
        printf("\nItem \"%s\" nao foi encontrado na mochila.\n", nomeBuscar);
    }
    
    clock_t fim = clock();//variavel para armazenar o tempo de fim da busca
    double tempo_ms = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
    
    printf("\n========== METRICAS DE BUSCA BINARIA ==========\n");
    printf("Item procurado: %s\n", nomeBuscar);
    printf("Numero de comparacoes: %d\n", comparacoes);
    printf("Tempo de execucao: %.6f ms\n", tempo_ms);
    if(encontrado) {
        printf("Status: COMPONENTE-CHAVE LOCALIZADO COM SUCESSO!\n");
    } else {
        printf("Status: Componente nao encontrado - impossivel iniciar montagem.\n");
    }
    printf("==============================================\n\n");
}

//funcao para organizar os itens da mochila em ordem alfabetica usando bubble sort
void organizarMochila(mochila *itemMochila) {
    clock_t inicio = clock();
    int comparacoes = 0;
    
    // bubble sort para organizar os itens por nome em ordem alfabetica
    for (int i = 0; i < itemMochila->quantidade - 1; i++) {
        int trocou = 0; //variavel para otimizar o algoritmo, se apos uma passagem completa nenhuma troca for feita a lista ja esta ordenada
        for (int j = 0; j < itemMochila->quantidade - i - 1; j++) {
            comparacoes++; //incrementa contador de comparacoes
            if (strcmp(itemMochila->itens[j].nome, itemMochila->itens[j + 1].nome) > 0) {
                // Troca os itens usando uma variável temporária
                item temp = itemMochila->itens[j];
                itemMochila->itens[j] = itemMochila->itens[j + 1];
                itemMochila->itens[j + 1] = temp;
                trocou = 1; //indica que houve uma troca
            }
        }
        //otimizacao: se apos uma passagem completar nenhuma troca foi feita
        //a lista ja esta ordenada e podemos para o algoritimo
        if(trocou == 0) {
            break;
        }
    }
    
    clock_t fim = clock();
    double tempo_ms = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
    
    //armazena as metricas de desempenho
    itemMochila->ultimo_desempenho.comparacoes = comparacoes;
    itemMochila->ultimo_desempenho.tempo_ms = tempo_ms;
    strcpy(itemMochila->ultimo_desempenho.algoritmo, "Bubble Sort (Nome)");
}

//funcao para organizar os itens da mochila por tipo usando insertion sort
void organizarMochilaTipo(mochila *itemMochila) {
    clock_t inicio = clock();
    int comparacoes = 0;
    
    int i, j;
    item chave;

    //start do segundo elemento (indice 1), pois o primeiro ja e uma sub-lista
    for (i = 1; i < itemMochila->quantidade; i++) {
        //'chave' é o elemento que queremos inserir na parte ordenada
        chave = itemMochila->itens[i];
        //'j' aponta para o ultimo elemento da parte ordenada
        j = i - 1;

        //move os elementos da parte ordenada que sao maiores q a chave
        //uma posicao para a diretia, para abrir espaco para a insercao
        while (j >= 0 && strcmp(itemMochila->itens[j].tipo, chave.tipo) > 0) {
            comparacoes++; //incrementa contador de comparacoes
            itemMochila->itens[j + 1] = itemMochila->itens[j];
            j = j - 1;
        }
        //incrementa comparacoes mesmo quando sai do while (ultima comparacao falsa)
        if(j >= 0) comparacoes++;

        //insere a chave na posicao correta (o buraco aberto)
        itemMochila->itens[j + 1] = chave;
    }
    
    clock_t fim = clock();
    double tempo_ms = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
    
    //armazena as metricas de desempenho
    itemMochila->ultimo_desempenho.comparacoes = comparacoes;
    itemMochila->ultimo_desempenho.tempo_ms = tempo_ms;
    strcpy(itemMochila->ultimo_desempenho.algoritmo, "Insertion Sort (Tipo)");
}

//funcao para organizar os itens da mochila por prioridade usando selection sort
void organizarMochilaPrioridade(mochila *itemMochila) {
    clock_t inicio = clock();
    int comparacoes = 0;
    
    int i, j, indice_min;
    item temp;

    //laco externo percorre a lista para definir a fronteira entre
    //a parte ordenada e a nao ordenada
    for (i = 0; i < itemMochila->quantidade - 1; i++) {
        //assume que o primeiro elemento da parte nao ordenada e a menor 
        indice_min = i;

        //laco interno percorre a parte nao ordenada para encontra o menor elemento
        for (j = i + 1; j < itemMochila->quantidade; j++) {
            comparacoes++; //incrementa contador de comparacoes
            //se encontrar um elemento menor que o minimo atual...
            if (itemMochila->itens[j].prioridade < itemMochila->itens[indice_min].prioridade) {
                //...atualiza o indice do minimo
                indice_min = j;
            }
        }

        //se o menor elemento nao for o primeiro da parte nao ordenada
        //troca-os de lugar
        if (indice_min != i) {
            temp = itemMochila->itens[i];
            itemMochila->itens[i] = itemMochila->itens[indice_min];
            itemMochila->itens[indice_min] = temp;
        }
    }
    
    clock_t fim = clock();
    double tempo_ms = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;
    
    //armazena as metricas de desempenho
    itemMochila->ultimo_desempenho.comparacoes = comparacoes;
    itemMochila->ultimo_desempenho.tempo_ms = tempo_ms;
    strcpy(itemMochila->ultimo_desempenho.algoritmo, "Selection Sort (Prioridade)");
}

void estrategiaOrganizacao(mochila *itemMochila) {
    printf("---------Estrategia de organizacao--------\n");
    printf("1. Organizar por Nome (ordem alfabetica)\n");
    printf("2. Organizar por Tipo\n");
    printf("3. Organizar por Prioridade de montagem\n");
    printf("0. Voltar\n");
    printf("Escolha uma opcao: ");
    int opcaoOrganizacao;
    scanf("%d", &opcaoOrganizacao);
    switch (opcaoOrganizacao) {
        case 1:
            organizarMochila(itemMochila);
            printf("Mochila organizada por nome (ordem alfabetica).\n");
            exibirMetricasDesempenho(&itemMochila->ultimo_desempenho);
            listarItens(itemMochila);
            break;
        case 2:
            organizarMochilaTipo(itemMochila);
            printf("Mochila organizada por tipo.\n");
            exibirMetricasDesempenho(&itemMochila->ultimo_desempenho);
            listarItens(itemMochila);
            break;
        case 3:
            organizarMochilaPrioridade(itemMochila);
            printf("Mochila organizada por prioridade de montagem.\n");
            exibirMetricasDesempenho(&itemMochila->ultimo_desempenho);
            listarItens(itemMochila);
            break;
        case 0:
            printf("Voltando ao menu anterior...\n");
            break;
        default:
            printf("Opcao invalida! Voltando ao menu anterior...\n");
            break;
    }
}

void exibirMetricasDesempenho(const desempenho *d) {
    printf("\n========== METRICAS DE DESEMPENHO ==========\n");
    printf("Algoritmo: %s\n", d->algoritmo);
    printf("Numero de comparacoes: %d\n", d->comparacoes);
    printf("Tempo de execucao: %.6f ms\n", d->tempo_ms);
    printf("===========================================\n\n");
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
    int prioridade;
    do {
        //statusMochila(itemMochila);

        printf("\n--- Menu Itens da Mochila ---\n");
        printf("1. Adicionar Componente\n");
        printf("2. Remover Componente\n");
        printf("3. Listar Componentes\n");
        printf("4. Organizar Mochila (ordenar componentes)\n");
        printf("5. Busca Binaria por componente-chave (por nome)\n");
        printf("0. Voltar\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1://para adicionar um item precisamos do nome, tipo, quantidade e prioridade para organizar depois por prioridade
                printf("Digite o nome do componente: ");
                scanf(" %29[^\n]", nome);
                printf("Digite o tipo do item a inserir(ex: controle, Suporte, Propulsão): ");
                scanf(" %19[^\n]", tipo);
                printf("Digite a quantidade do componente: ");
                scanf("%d", &quantidade);
                printf("Digite a prioridade do item (1-5, sendo 1 a mais alta): ");
                scanf("%d", &itemMochila->itens[itemMochila->quantidade].prioridade);
                adicionarItem(itemMochila, nome, tipo, quantidade, itemMochila->itens[itemMochila->quantidade].prioridade);
                break;
            case 2://para remover um item precisamos do nome e tipo para identificar unicamente o item a ser removido
                printf("Digite o nome do componente a remover: ");
                scanf(" %29[^\n]", nome);
                printf("Digite o tipo a remover(ex: controle, Suporte, Propulsão): ");
                scanf(" %19[^\n]", tipo);
                removerItem(itemMochila, nome, tipo);
                break;
            case 3:
                listarItens(itemMochila);//chama a funcao para listar os itens atuais da mochila
                break;
            case 4:
                estrategiaOrganizacao(itemMochila);//chama o menu de organizacao para escolher a estrategia de ordenacao
                break;
            case 5:
                buscaBinariaNome(itemMochila);//chama a funcao de busca binaria por nome, que exige que a mochila esteja organizada por nome (alfabetica)
                break;
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);
}





