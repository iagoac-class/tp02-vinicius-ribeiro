#ifndef _H_ARVORES
#define _H_ARVORES

// Implementar qualquer struct que for necessária aqui

// Struct árvore não balanceada 

typedef struct sBT{

    int data; // o valor que iremos guardar

    //Ponteiros para os nós filhos
    struct sBT *left; 
    struct sBT *right;

}nodeTreeBT;

// Funções árvore não balanceada

nodeTreeBT *insert(nodeTreeBT *root, int data);
nodeTreeBT *search(nodeTreeBT *root, int data);
nodeTreeBT *buscaPai(nodeTreeBT* root, nodeTreeBT* node);
nodeTreeBT *removeRaiz(nodeTreeBT* root);
nodeTreeBT *removeNo(nodeTreeBT *root, int data);
void processaEntradas(nodeTreeBT **root, const char *filename);

double arvore_binaria(int instancia_num);
// double arvore_balanceada(int instancia_num); //

#endif // _H_ARVORES