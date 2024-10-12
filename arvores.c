#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>       
#include "arvores.h"

// Funções para a implementação de um árvore não balanceada //

// Inserindo um nó na árvore

nodeTreeBT *insert(nodeTreeBT *root, int data /*Recebe um ponteiro para a Raiz e o valor que queremos inserir*/){
    nodeTreeBT *temp = (nodeTreeBT*)malloc(sizeof(nodeTreeBT));

    temp->data = data;
    temp->left = NULL;
    temp->right = NULL;

    //Se a raiz for nula, como é o caso da 1ª inserção, ele passa apontar para a variável temp //
    if(root == NULL){
        root = temp;
    } else{ // Caso já tenha feito inserções anteriores:

        nodeTreeBT *current = root; // aponta para o end. da raiz
        nodeTreeBT *parent = NULL; // o "pai" começa nulo

        while(1){

            parent = current; // na 1ª passada do loop, é o primeiro node que já está inserido // 

            if(data < parent->data){ // se o nº que queremos inserir for menor que o número onde "parent" aponta
                current = current->left;

                if(current == NULL){
                    parent->left = temp;
                    return root;
                }

                /*Se não for nulo, entra de novo no loop e agora parent será os próximos nodes abaixo da raiz,
                até encontrar um node que esteja apontando para NULL, onde será feito a inserção*/ 
                
            } else { // se o nº que queremos inserir for maior ou igual onde "parent" aponta

                current = current->right; // aponta a direita, pois é um número maior

                if(current == NULL){ // se não tiver nenhum número ainda inserido a direita
                    parent->right = temp;
                    return root;
                }
            }
        }
    }

    return root;
}

// Buscando um elemento na árvore

nodeTreeBT *search(nodeTreeBT *root, int data){
    nodeTreeBT *current = root;

    while(current){

        if(data < current->data){
            current = current->left;
        }else if(data > current->data){
            current = current->right;
        }else{
            return current;
        }

    }
    return current;
}

// Removendo um nó

nodeTreeBT *buscaPai(nodeTreeBT* root, nodeTreeBT* node) {
    // Se o nó que estamos procurando for a raiz, ele não tem pai
    if (root == NULL || root == node) {
        return NULL;
    }

    nodeTreeBT* current = root;

    while (current != NULL) {
        if ((current->left == node) || (current->right == node)) {
            return current; // Se o nó atual for o pai do nó procurado
        } else if (node->data < current->data) {
            current = current->left; // Vai para a esquerda
        } else {
            current = current->right; // Vai para a direita
        }
    }

    return NULL; // Se não encontrar o pai
}

nodeTreeBT *removeRaiz(nodeTreeBT* root) {
    // Caso 1: Nó folha (sem filhos) - Se o nó não tem filhos, ele é simplesmente removido e NULL é retornado. //
    if (root->left == NULL && root->right == NULL) {
        free(root);
        return NULL;
    }
    // Caso 2: Apenas um filho - Se o nó tem apenas um filho (esquerda ou direita), ele é removido e o filho é retornado para substituir o nó removido. //
    else if (root->left == NULL) {
        nodeTreeBT* temp = root->right;
        free(root);
        return temp;
    }
    else if (root->right == NULL) {
        nodeTreeBT* temp = root->left;
        free(root);
        return temp;
    }
    // Caso 3: Dois filhos - Se o nó tem dois filhos, o menor valor da subárvore à direita (sucessor) é encontrado para substituir o nó a ser removido. O sucessor é, então, removido. //
    else {
        nodeTreeBT* succParent = root;
        nodeTreeBT* succ = root->right;

        // Encontra o sucessor (menor valor da subárvore direita)
        while (succ->left != NULL) {
            succParent = succ;
            succ = succ->left;
        }

        // O sucessor precisa ser movido para substituir a raiz
        if (succParent != root) {
            succParent->left = succ->right;
        } else {
            succParent->right = succ->right;
        }

        // Atualiza o valor da raiz com o valor do sucessor
        root->data = succ->data;

        free(succ); // Remove o sucessor
        return root;
    }
}

nodeTreeBT *removeNo(nodeTreeBT *root, int data){
    nodeTreeBT *temp = search(root, data); // Busca se o valor existe na árvore

    if(temp){ // Se houver o valor na árvore:
        nodeTreeBT *parent = buscaPai(root, temp); // Vamos encontrar o nó "pai"
        if(parent){ // Se o nó a ser removido tem um pai (não é a raiz)
            if(parent->right == temp){
                parent->right = removeRaiz(temp); // Atualizamos o ponteiro do filho direito do pai para o novo nó retornado por removeRaiz(temp).
            } else {
                parent->left = removeRaiz(temp);
            }
        } else { // Se o nó a ser removido é a raiz da árvore (não tem pai)
            root = removeRaiz(temp);
        }

        return root;
    }

    return root; // Se o valor não for encontrado, a função termina sem alterar a árvore.

}

// Função para ler as entradas e processar a inserção e removeção na árvore não balanceada 

void processaEntradas(nodeTreeBT **root, const char *filename){   
    char operacao;           // Para armazenar a operação (I ou R)
    int valor;              // Para armazenar o valor a ser inserido ou removido

    // Abre o arquivo para leitura
    FILE *file = fopen(filename, "r"); // O arquivo será aberto apenas para leitura, pois o argumento 'r'
    if (file == NULL) {
        puts("Erro ao abrir o arquivo");
        return; // Retorna se houver erro ao abrir o arquivo
    }

    // Leitura das entradas até o fim 
    // Utilizando um loop para leitura das operações de entrada até o fim do arquivo (EOF, que é o End of File (Fim de Arquivo, em português) ). "
    while (fscanf(file, " %c %d", &operacao, &valor) != EOF) {
        if (operacao == 'I') {
            // Chama a função de inserção quando a operação for 'I'
            *root = insert(*root, valor);
            // printf("Valor %d inserido.\n", valor);  // Para fins de teste
        } else if (operacao == 'R') {
            // Chama a função de remoção quando a operação for 'R'
            *root = removeNo(*root, valor);
            // printf("Valor %d removido.\n", valor);  // Para fins de teste
        } else {
            // Se a operação não for válida
            printf("Operação inválida: %c\n", operacao);
        }

    }
    fclose(file);
}    

// Códigos disponibilizados //

double arvore_binaria(int instancia_num) {
    nodeTreeBT *root = NULL; // Inicializa a raiz da árvore como NULL
    double tempo = 0;

    // Estrutura para o nome do arquivo
    char filename[50];
    sprintf(filename, "instancias/%d", instancia_num); // Cria o nome do arquivo baseado na instância

    // Iniciando a medição do tempo
    clock_t begin = clock();

    processaEntradas(&root, filename); // Chamando a função para manipular a árvore

    // Finalizando a medição tempo
    clock_t end = clock();

    // calcula o tempo decorrido encontrando a diferença (end - begin) e
    // dividindo a diferença por CLOCKS_PER_SEC para converter em segundos
    tempo += (double)(end - begin) / CLOCKS_PER_SEC;
    return (tempo);
}

double arvore_balanceada(int instancia_num) {
    double tempo = 0;
    clock_t begin = clock();


    
    clock_t end = clock();
    // calcula o tempo decorrido encontrando a diferença (end - begin) e
    // dividindo a diferença por CLOCKS_PER_SEC para converter em segundos
    tempo += (double)(end - begin) / CLOCKS_PER_SEC;
    return (tempo);
}


int main(int argc, char* argv[]) {
    ///////////////////////////////////////////////////////////
    /////////////////// Leitor de instâncias //////////////////
    ///////////////// Não deve ser modificado /////////////////
    ///////////////////////////////////////////////////////////
    int instancia_num = -1;
    instancia_num = atoi(argv[1]);
    if (instancia_num <= 0 || instancia_num > 10) {
        printf("Para executar o código, digite ./arvores x\nonde x é um número entre 1 e 10 que simboliza a instância utilizada\n");
        exit(0);
    }
    
    double tempo_n_balanceada = arvore_binaria(instancia_num);
    double tempo_balanceada = arvore_balanceada(instancia_num);

    
    printf("%f\n", tempo_n_balanceada);

    return (1);
}