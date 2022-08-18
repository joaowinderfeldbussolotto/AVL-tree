
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>



typedef struct {
	int day;
	int month;
	int year;
} Date;

// Estrutura que contém os campos dos registros da agenda
struct MREC {
       char name[30];
       Date  birth; 
       char email[40];
       char phone[15];
};

// Tipo criado para instanciar variaveis do tipo agenda
typedef struct MREC Contact;

// Nó da árvore
struct fNode
{
  Contact *info;
	int height;
	struct fNode *left;
	struct fNode *right;
};
typedef struct fNode Node;


void printInfo(Contact *info){
  printf("%s %d/%d/%d %s %s\n", info->name, info->birth.day, info->birth.month, info->birth.year, info->email, info->phone);
}


// Função que retorna a altura da árvore, utilizada quando é preciso usar a altura em outra parte do código
int height(Node *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

// Função que retorna o máximo entre dois números inteiros
int max(int a, int b)
{
    return (a > b)? a : b;
}

/* Função auxiliar pra adicionar um novo nó na árvore
   com ponteiros nulos tanto left quanto right*/
Node* newNode(Contact *info)
{

	Node* node = (Node *) malloc(sizeof(Node));
	node->info = info;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return node;
}

/*Função que rotaciona a subarvore para direita e retorna a nova raíz*/
Node *rightRotate(Node *y)
{
	Node *x = y->left;
	Node *T2 = x->right;

	//Executa a rotação
	x->right = y;
	y->left = T2;

	//Atualiza a altura
	y->height = max(height(y->left), height(y->right))+1;
	x->height = max(height(x->left), height(x->right))+1;

	//Retorna a nova raíz
	return x;
}

//Função que rotaciona a subarvore para a esquerda e retorna a nova raíz
Node *leftRotate(Node *x)
{
	Node *y = x->right;
	Node *T2 = y->left;

	//Executa a rotação
	y->left = x;
	x->right = T2;

	//Atualiza a altura
	x->height = max(height(x->left), height(x->right))+1;
	y->height = max(height(y->left), height(y->right))+1;

	//Retorna nova raíz
	return y;
}

//Função que acha o fator balanceamento da árvore
int getBalance(Node *N)
{
	if (N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}

/*Função que transforma a string em uma string feita totalmente 
  em letras maiusculas, usada depois para as comparações entre os nomes*/
char* toUpperCase(char* string){
	int i = 0;
	  while(string[i]) {
      string[i] =  (toupper(string[i]));
      i++;
   }
	 return string;
}

Node* insert(Node* node, Contact *info){
	strcpy(info->name, toUpperCase(info->name));
	if (node == NULL)
		return(newNode(info));

	if (strcmp(info->name,node->info->name) < 0)
		node->left = insert(node->left, info);
	else if (strcmp(info->name,node->info->name) > 0)
		node->right = insert(node->right, info);
	else 
		printf("Nome ja existente\n");
		return node;

	/*Atualiza a altura do nó anterior*/
	node->height = 1 + max(height(node->left),
						height(node->right));

	/*Pega a altura do nó anterior para verificar se 
	  a arvore está balanceada ou não*/
	int balance = getBalance(node);

	//Se ela não estiver balanceada há 4 casos possíveis

	//1 -> Caso LL(Left Left)
	if (balance > 1 && strcmp(info->name,node->left->info->name) < 0)
		return rightRotate(node);

	//2 -> Caso RR(Right Right)
	if (balance < -1 && strcmp(info->name,node->right->info->name) > 0)
		return leftRotate(node);

	//3 -> Caso LR(Left Right)
	if (balance > 1 && strcmp(info->name,node->left->info->name) > 0)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	//4 -> Caso RL(Right Left)
	if (balance < -1 && strcmp(info->name,node->right->info->name) < 0)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}
	//retorna o ponteiro
	return node;
}

/*Percorre a lista a esquerda atrás do ponteiro com menor valor,
  no caso do trabalho procura o nome com menor valor em ordem alfabetica
  */
Node * minValueNode(Node* node)
{
	Node* current = node;

	/*um laço para percorrer a arvore para a esquerda, assim achando o nó
	  que tem o menor valor*/
	while (current->left != NULL)
		current = current->left;
	//retorna o node
	return current;
}

Node* queryNode(Node* root, char *key)
{

	Node *current = root;
	strcpy(key, toUpperCase(key));

	while(current && (strcmp(key,current->info->name)) != 0){
		if( strcmp(key,current->info->name) > 0 ){
			current = current->right;
		}
		else if( strcmp(key,current->info->name) < 0 ){
			current = current->left;
		}

		}
	return current;
}


// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
Node* deleteNode(Node* root, char *key)
{
	// STEP 1: PERFORM STANDARD BST DELETE
	//recebe o nome e transforma no mesmo em caixa alta
	strcpy(key, toUpperCase(key));

	if (root == NULL)
		return root;

	/*Verifica se o valor é menor que o do nó atual (ordem alfabética), caso sim,
	  vai para a esquerda*/
	if ( strcmp(key,root->info->name) < 0)
		root->left = deleteNode(root->left, key);

	/*Verifica se o valor é maior que o do nó atual (ordem alfabética), caso sim,
	  vai para a direita*/
	else if( strcmp(key,root->info->name) > 0)
		root->right = deleteNode(root->right, key);

	//Se o valor(nome) for igual ao valor(nome) presente no nó, então é o nó a ser deletado
	else
	{
		//nós com apenas um filho ou nenhum filho
		if( (root->left == NULL) || (root->right == NULL) )
		{
			Node *temp = root->left ? root->left :
											root->right;

			//caso 1: nenhum filho
			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else //caso 2: um filho apenas
			*root = *temp; //copia o conteúdo do filho
			free(temp);
		}
		else
		{
			// node with two children: Get the inorder
			// successor (smallest in the right subtree)
			Node* temp = minValueNode(root->right);

			// Copy the inorder successor's data to this node
			root->info = temp->info;

			// Delete the inorder successor
			root->right = deleteNode(root->right, temp->info->name);
		}
	}

	// If the tree had only one node then return
	if (root == NULL)
	return root;

	// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
	root->height = 1 + max(height(root->left),
						height(root->right));

	// STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
	// check whether this node became unbalanced)
	int balance = getBalance(root);

	// If this node becomes unbalanced, then there are 4 cases

	// Left Left Case
	if (balance > 1 && getBalance(root->left) >= 0)
		return rightRotate(root);

	// Left Right Case
	if (balance > 1 && getBalance(root->left) < 0)
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	// Right Right Case
	if (balance < -1 && getBalance(root->right) <= 0)
		return leftRotate(root);

	// Right Left Case
	if (balance < -1 && getBalance(root->right) > 0)
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}

//Função pra printar a árvore em Pre Order
void preOrder(Node *root)
{
	if(root != NULL)
	{
		printf("%s ", root->info->name);
		preOrder(root->left);
		preOrder(root->right);
	}
}

void inOrder(Node *n)
{
	if (n==NULL) return;
	inOrder(n->left);
	printInfo(n->info);
	inOrder(n->right);
}

void printPreOrderInFile(Node *n, FILE *file)
{
	if (n==NULL) return;
	fwrite(n->info, sizeof(Contact), 1, file);
	printPreOrderInFile(n->left, file);
	printPreOrderInFile(n->right, file);

}

// valor fixo para a opção que finaliza a aplicação
#define EXIT 10

// Apresenta o menu da aplicação e retorna a opção selecionada
int menu()
{
    int op=0;
         
        printf("1 - Inserir\n2 - Deletar\n3 - Alterar\n4 - Consultar\n5 - Listar\n");
        printf("%d Finaliza\n",EXIT);
        
        scanf(" %d",&op);
       
    
    return op;
}

// Permite o cadastro de um contato
Node* insContact(Node *root)
{
     int d,m,y;
     Contact *new = (Contact*)malloc(sizeof(Contact));
     printf("Informe o nome:\n ");
     scanf(" %s", new->name);
	 if(queryNode(root, new->name)){
	 	printf("Nome já existente\n");
		return root; 
	 }
     printf("\nInforme a data de nascimento: \n");
     scanf(" %d/%d/%d",&d,&m,&y);
     printf("Informe o email: \n");
     scanf(" %s",new->email);     
     printf("Informe o telefone: \n");
     scanf(" %s",new->phone);

     Date birth;
     birth.day = d;
     birth.month = m;
     birth.year = y;

     new->birth = birth;
     //printInfo(new);
	 
     return insert(root, new);

}

// Permite excluir um contato da agenda
Node* delContact (Node *root)
{    
     char key[40];
     printf("Informe o nome: \n");
     scanf(" %s", key);

     root = deleteNode(root, key);
     return root;

}

// Lista o conteudo da agenda (todos os campos)
void listContacts (Node *root)
{
     inOrder(root);
     printf("\n");
}

// Permite consultar um contato da agenda por nome
void queryContact (Node *root)
{
    char key[40];
     printf("Informe o nome que deseja buscar:\n");
     scanf(" %s", key);

     Node *found = queryNode(root, key);
     found!=NULL? printInfo(found->info) : printf("Nao foi encontrado!\n");
}

// Permite a atualização dos dados de um contato da agenda
Node* upContact (Node *root)
{
     char key[40];
     printf("Informe o nome que deseja atualizar:\n");
     scanf(" %s", key);

     Node *found = queryNode(root, key);
     if(found){
      Contact *updated = malloc(sizeof(Contact));
      int d,m,y;
      printf("Entre as informações atualizadas:\nInforme o nome:\n ");
      scanf(" %s", updated->name);
      printf("\nInforme a data de nascimento: \n");
      scanf("%d/%d/%d",&d,&m,&y);
      printf("Informe o email: \n");
      scanf(" %s",updated->email);     
      printf("Informe o telefone: \n");
      scanf(" %s",updated->phone);

      Date birth;
      birth.day = d;
      birth.month = m;
      birth.year = y;

      updated->birth = birth;
      if(strcmp(found->info->name, updated->name) != 0){
        root = deleteNode(root, found->info->name);
        root = insert(root, updated);
      }
     }  
     else{
         printf("Nao foi encontrado\n");
     }
     return root;
}


Node* insertFromFile(Node* root){
	Contact *c = (Contact*)malloc(sizeof(Contact));
	FILE *file = fopen("agenda.dat", "rb+");
	if(file == NULL){
		//printf("Erro ao abrir arquivo"); 
		FILE *file = fopen("agenda.dat", "wb+");

	} 

	while(fread(c, sizeof(Contact),1,file)){
		root = insert(root, c);
		c = (Contact*)malloc(sizeof(Contact));
	}
	
	return root;

}

void saveToFile(Node *root){

	FILE *file = fopen("agenda.dat", "wb");
	if(file == NULL){
		printf("Erro ao abrir arquivo"); return;
	} 

	printPreOrderInFile(root, file);
}



// Programa principal
int main()
{
	Node *root = NULL;
	setlocale(LC_ALL, "Portuguese");

    int op=0;
    Contact MContact;
	root = insertFromFile(root);
     while (op!=EXIT)
     {
          op=menu();
          switch(op)
          {
              case 1 : root = insContact(root); break;
              case 2 : root = delContact(root); break;
              case 3 : root = upContact(root); break;
              case 4 : queryContact(root); break;
              case 5 : listContacts(root); 
              break;
          }
    }

	
	saveToFile(root);

  
}
