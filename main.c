
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


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
// C program to delete a node from AVL Tree


// An AVL tree node
struct Node
{
  Contact *info;
	int height;
	struct Node *left;
	struct Node *right;
};


void printInfo(Contact *info){
  printf("%s %d/%d/%d %s %s\n", info->name, info->birth.day, info->birth.month, info->birth.year, info->email, info->phone);
}


// A utility function to get height of the tree
int height(struct Node *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

// A utility function to get maximum of two strings
int max(int a, int b)
{
    return (a > b)? a : b;
}

/* Helper function that allocates a new node with the given key and
	NULL left and right pointers. */
struct Node* newNode(Contact *info)
{

	struct Node* node = (struct Node*)
						malloc(sizeof(struct Node));
	node->info = info;
	node->left = NULL;
	node->right = NULL;
	node->height = 1; // new node is initially added at leaf
	return node;
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct Node *rightRotate(struct Node *y)
{
	struct Node *x = y->left;
	struct Node *T2 = x->right;

	// Perform rotation
	x->right = y;
	y->left = T2;

	// Update heights
	y->height = max(height(y->left), height(y->right))+1;
	x->height = max(height(x->left), height(x->right))+1;

	// Return new root
	return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct Node *leftRotate(struct Node *x)
{
	struct Node *y = x->right;
	struct Node *T2 = y->left;

	// Perform rotation
	y->left = x;
	x->right = T2;

	// Update heights
	x->height = max(height(x->left), height(x->right))+1;
	y->height = max(height(y->left), height(y->right))+1;

	// Return new root
	return y;
}

// Get Balance factor of node N
int getBalance(struct Node *N)
{
	if (N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}

char* toUpperCase(char* string)
{
	int i = 0;
	  while(string[i]) {
      string[i] =  (toupper(string[i]));
      i++;
   }
	 return string;
}
struct Node* insert(struct Node* node, Contact *info)

{
  //printInfo(info);
	strcpy(info->name, toUpperCase(info->name));
	/* 1. Perform the normal BST rotation */
	if (node == NULL)
		return(newNode(info));

	if (strcmp(info->name,node->info->name) < 0)
		node->left = insert(node->left, info);
	else if (strcmp(info->name,node->info->name) > 0)
		node->right = insert(node->right, info);
	else // Equal keys not allowed
		return node;

	/* 2. Update height of this ancestor node */
	node->height = 1 + max(height(node->left),
						height(node->right));

	/* 3. Get the balance factor of this ancestor
		node to check whether this node became
		unbalanced */
	int balance = getBalance(node);

	// If this node becomes unbalanced, then there are 4 cases

	// Left Left Case
	if (balance > 1 && strcmp(info->name,node->left->info->name) < 0)
		return rightRotate(node);

	// Right Right Case
	if (balance < -1 && strcmp(info->name,node->right->info->name) > 0)
		return leftRotate(node);

	// Left Right Case
	if (balance > 1 && strcmp(info->name,node->left->info->name) > 0)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// Right Left Case
	if (balance < -1 && strcmp(info->name,node->right->info->name) < 0)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	/* return the (unchanged) node pointer */
	return node;
}

/* Given a non-empty binary search tree, return the
node with minimum key value found in that tree.
Note that the entire tree does not need to be
searched. */
struct Node * minValueNode(struct Node* node)
{
	struct Node* current = node;

	/* loop down to find the leftmost leaf */
	while (current->left != NULL)
		current = current->left;

	return current;

}

struct Node* queryNode(struct Node* root, char *key)
{

	struct Node *current = root;

  while(current && (strcmp(key,root->info->name)) != 0){
    if( strcmp(key,current->info->name) > 0 )
			current = current->right;
		else
			current = current->left;
	}
  return current;
}


// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
struct Node* deleteNode(struct Node* root, char *key)
{
	// STEP 1: PERFORM STANDARD BST DELETE

	if (root == NULL)
		return root;

	// If the key to be deleted is smaller than the
	// root's key, then it lies in left subtree
	if ( strcmp(key,root->info->name) < 0)
		root->left = deleteNode(root->left, key);

	// If the key to be deleted is greater than the
	// root's key, then it lies in right subtree
	else if( strcmp(key,root->info->name) > 0)
		root->right = deleteNode(root->right, key);

	// if key is same as root's key, then This is
	// the node to be deleted
	else
	{
		// node with only one child or no child
		if( (root->left == NULL) || (root->right == NULL) )
		{
			struct Node *temp = root->left ? root->left :
											root->right;

			// No child case
			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else // One child case
			*root = *temp; // Copy the contents of
							// the non-empty child
			free(temp);
		}
		else
		{
			// node with two children: Get the inorder
			// successor (smallest in the right subtree)
			struct Node* temp = minValueNode(root->right);

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

// A utility function to print preorder traversal of
// the tree.
// The function also prints height of every node
void preOrder(struct Node *root)
{
	if(root != NULL)
	{
		printf("%s ", root->info->name);
		preOrder(root->left);
		preOrder(root->right);
	}
}

void inOrder(struct Node *n)
{
	if (n==NULL) return;
	inOrder(n->left);
	printInfo(n->info);
	inOrder(n->right);
}

void printPreOrderInFile(struct Node *n, FILE *file)
{
	if (n==NULL) return;
	fwrite(n->info, sizeof(Contact), 1, file);
	printPreOrderInFile(n->left, file);
	printPreOrderInFile(n->right, file);

}


#define EXIT 10  // valor fixo para a opção que finaliza a aplicação

//Struct que representa a data.


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
struct Node* insContact(struct Node *root)
{
     int d,m,y;
     Contact *new = (Contact*)malloc(sizeof(Contact));
     printf("Informe o nome:\n ");
     scanf(" %s", new->name);
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
struct Node* delContact (struct Node *root)
{    
     char key[40];
     printf("Informe o nome: \n");
     scanf(" %s", key);

     root = deleteNode(root, key);
     return root;

}

// Lista o conteudo da agenda (todos os campos)
void listContacts (struct Node *root)
{
     inOrder(root);
     printf("\n");
}

// Permite consultar um contato da agenda por nome
void queryContact (struct Node *root)
{
    char key[40];
     printf("Informe o nome que deseja buscar:\n");
     scanf(" %s", key);

     struct Node *found = queryNode(root, key);
     found? printInfo(found->info) : printf("Nao foi encontrado\n");
}

// Permite a atualização dos dados de um contato da agenda
struct Node* upContact (struct Node *root)
{
     char key[40];
     printf("Informe o nome que deseja atualizar:\n");
     scanf(" %s", key);

     struct Node *found = queryNode(root, key);
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


struct Node* insertFromFile(struct Node* root){
	Contact *c = (Contact*)malloc(sizeof(Contact));
	FILE *file = fopen("agenda.dat", "rb+");
	if(file == NULL){
		printf("Erro ao abrir arquivo"); 
		return root;
	} 

	while(fread(c, sizeof(Contact),1,file)){
		root = insert(root, c);
		c = (Contact*)malloc(sizeof(Contact));
	}
	
	return root;

}

void saveToFile(struct Node *root){

	FILE *file = fopen("agenda.dat", "wb");
	if(file == NULL){
		printf("Erro ao abrir arquivo"); return;
	} 

	printPreOrderInFile(root, file);
}



// Programa principal
int main()
{
  struct Node *root = NULL;
	
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