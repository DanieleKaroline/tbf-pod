#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT 10

 /*
 |
 |  Lista de structs
 |
*/
typedef struct{
    int day;
    int month;
    int year;
}Date;
//**********************************************
struct tree{
    char name[30];
    Date birth;
    char email[40];
    char phone[15];
    struct tree *left;
    struct tree *right;
};
typedef struct tree Tree;
//**********************************************

int menu(){
    int op=0;
    scanf("%d",&op);
       
    return op;    
}

Tree* busca(Tree *root, char *name){
    if(root==NULL){
        return NULL;
    }
    int diferente = strcmp(root->name, name);
    if(!diferente)
        return root;
    return busca((diferente>0)?root->left:root->right, name);
}
void readContactData(Tree *contact){
    //system("cls");
    printf("nome: %s\nnascimento: %d/%d/%d\nemail: %s\ntelefone: %s\n", contact->name, contact->birth.day, contact->birth.month, contact->birth.year, contact->email, contact->phone);
}

void queryContact(Tree *root){
    char name[30];
    scanf("%s", name);
    Tree *node = busca(root, name);
    if(node==NULL){
        printf("contato nao achado\n");
        return;
    }
    readContactData(node);
}

void upContact(Tree *root){
    char name[30];
    scanf("%s", name);
    Tree *node = busca(root, name);
    if(node==NULL){
        printf("contato nao achado\n");
        return;
    }
    Date birth;
    char email[45];
    char phone[15];
    scanf("%d/%d/%d", &birth.day, &birth.month, &birth.year);
    scanf("%s", email);
    scanf("%s", phone);
    node->birth=birth;
    strcpy(node->email, email);
    strcpy(node->phone, phone);
}

 /*
 |
 |  Funçôes relacionadas a AVL
 |
*/
//altura da arvore
int deapthSubTree(Tree *root){
    int deapthLeft, deapthRight;
    if(root==NULL)
        return 0;
    deapthLeft = deapthSubTree(root->left);
    deapthRight = deapthSubTree(root->right);
    return 1 + (deapthRight>deapthLeft?deapthRight: deapthLeft);
}

//Rotacoes da avl
Tree* ll(Tree *root){
    Tree *newRoot = root->left;
	root->left = newRoot->right;
	newRoot->right = root;
	return newRoot;
}
Tree* rr(Tree *root){
	Tree *newRoot = root->right;
	root->right = newRoot->left;
	newRoot->left = root;
	return newRoot;
}
Tree* lr(Tree* root){
	root->left = rr(root->left);
	return ll(root);
}
Tree* rl(Tree* root){
	root->right = ll(root->right);
	return rr(root);
}
//checando e balanceando a arvore
Tree* checkBalance(Tree *root){
    int balanceFactor = deapthSubTree(root->left) - deapthSubTree(root->right);
    int sonBalance;
    Tree *son;
    if (balanceFactor > 1){
		son = root->left;
		sonBalance = deapthSubTree(son->left) - deapthSubTree(son->right);
		if (sonBalance > 0 ){
			// printf("%s unbalance LL!\n", root->name);
			root = ll(root);
		}else{
			// printf("%s unbalance LR!\n", root->name);
			root = lr(root);
		}
		
	}else if (balanceFactor < -1){
		son = root->right;
		sonBalance = deapthSubTree(son->left) - deapthSubTree(son->right);
		if (sonBalance < 0 ){
			// printf("%s unbalance RR!\n", root->name);
			root = rr(root);
		}else{
			// printf("%s unbalance RL!\n", root->name);
			root = rl(root);
		}
	}
	return root;
}

 /*
 |
 |  Funçôes relacionadas a criaçao e inserção de um contato na agenda
 |
*/
//le os imputs do usuario e retorna o endereço de uma struct Tree
Tree* newContact(){
    Tree *contact = malloc(sizeof(Tree));
    // printf("nome: ");
    // gets(contact->name);
    // fflush(stdin);
    scanf("%s", contact->name);
    // fflush(stdin);
    // printf("data: ");
    scanf("%d/%d/%d", &contact->birth.day, &contact->birth.month, &contact->birth.year);
    // contact->birth.day=10;
    // contact->birth.month=10;
    // contact->birth.year=2000;
    // fflush(stdin);
    // printf("email: ");
    scanf("%s", contact->email);
    // strcpy(contact->email, "example@gmail.com");
    // fflush(stdin);
    // printf("telefone: ");
    scanf("%s", contact->phone);
    // strcpy(contact->phone, "22222222");
    // fflush(stdin);
    contact->right=NULL;
    contact->left=NULL;
    return contact;
}
//**********************************************
//adiciona o contato na agenda
Tree* insertion(Tree* node, Tree* new){
    if(node == NULL)
        return new;
    for(int i=0;i<30;i++){
        if(new->name[i]<node->name[i]){
            node->left = insertion(node->left, new);
            break;
        }
        if(new->name[i]>node->name[i]){
            node->right = insertion(node->right, new);
            break;
        }
    }
    node = checkBalance(node);
    return node;
}

Tree* insContact(Tree *root){
    Tree *new = newContact();
    return insertion(root, new);
}

Tree* maxChild(Tree *root){
    Tree *promoted;
    if(root->right->right==NULL){
        promoted=root->right;
        root->right=promoted->left;
        promoted->left=NULL;
        return promoted;
    }
    return maxChild(root->right);
}


Tree* delet(Tree *root, char *name){
    int stringsDiferentes = strcmp(name, root->name);
    Tree *newroot;
    if(!stringsDiferentes){
        if(root->left==NULL && root->right==NULL){
            free(root);
            return NULL;
        }
        if(root->left==NULL || root->right==NULL){
            newroot = (root->left==NULL)?root->right:root->left;
            free(root);
            return newroot;
        }
        if(root->left->right==NULL){
            newroot = root->left;
            newroot->right = root->right;
            free(root);
            return newroot;
        }else{
            newroot = maxChild(root->left);
            newroot->left=root->left;
            newroot->right=root->right;
            free(root);
            newroot = checkBalance(newroot);
            return newroot;
        }
    }
    else{
        if(stringsDiferentes>0){
            //vai pra right
            root->right = delet(root->right, name);

        }else{
            //vai pra left
            root->left = delet(root->left, name);
        }
    }
    return checkBalance(root);
}

Tree* delContact(Tree *root){
    char apagar[30];
    scanf("%s", apagar);
    root = delet(root, apagar);
    return root;
}

Tree* carregaArq(Tree *root){
    FILE *arq = fopen("arquivo.dat", "rb+");
    if(!arq)
        return root;
    Tree *node = malloc(sizeof(Tree));
    fread(node, sizeof(Tree), 1, arq);
    node->left=NULL;
    node->right=NULL;   
    
    Tree *aux = malloc(sizeof(Tree));
    while(fread(aux, sizeof(Tree), 1, arq)){
        aux->left=NULL;
        aux->right=NULL;
        node = insertion(node, aux);
        aux = malloc(sizeof(Tree));
    }
    fclose(arq);
    return node;
}

void save(Tree *root, int n, FILE *arq){
    if(root==NULL)
        return;
    save(root->right, n+1, arq);
    fwrite(root, sizeof(Tree), 1, arq);
    save(root->left, n+1, arq);
}

void salvarAgenda(Tree *root){
    if(root==NULL)
        return;
    FILE *arq = fopen("arquivo.dat", "wb+");
    save(root, 1, arq);
    fclose(arq);
}

/*
 |
 |  Função que le os dados de um contato
 |
*/
void listContacts(Tree *root){
    if(root != NULL){
        listContacts(root->left);
        printf("%s\n", root->name);
        listContacts(root->right);
    }
}

void printTree(Tree *root, int level){
    if(root==NULL) return;
    printTree(root->right, level+1);
    for(int i=0;i<level;i++)
        printf("\t");
    printf("%s\n", root->name);
    printTree(root->left, level+1);
}

void limpaTree(Tree *node){
	if (node==NULL) return;
	limpaTree(node->left);
	limpaTree(node->right);
    free(node);
}

int main(){

    int op=0;
    Tree* MContact=NULL;
    MContact = carregaArq(MContact);
    while (op!=EXIT)
    {
          op=menu();
          switch(op)
          {
              case 1 : MContact = insContact(MContact); break;
              case 2 : MContact = delContact(MContact); break;
              case 3 : upContact(MContact); break;
              case 4 : queryContact(MContact); break;
              case 5 : listContacts(MContact); break;
          }
    }
    salvarAgenda(MContact);
    limpaTree(MContact);
    return 0;
}
