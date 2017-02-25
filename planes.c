#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

typedef struct TReceiver
{
    int ID; 
    int flightDataMemory;
    int flightDataSum;
    int flightDataLength;
    int * flightData;
} Receiver;

typedef enum 
{   RED, 
    BLACK
} Color;

typedef struct TNode
{
    Receiver receiver;
    Color color;
    struct TNode * right;
    struct TNode * left;
    struct TNode * parent;
} Node;

typedef struct TRedBlackTree
{
    Node * root;
} RedBlackTree;


/*---------------------------------------Input part----------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
/* A function reads the number (ID) from the stdin.
 * 
 * Returns:
 *      EOF - The end of reading data
 *      0   - An incorrect character
 *      1   - OK
 */
int getID(int * ID)
{
    int res;
    if ((res = scanf(" %d", ID)) != 1 || *ID < 0)
    {
        if (res == EOF)
            return EOF;
        else
            return 0;
    }
    return 1;
}

/*-----------------------------------------------------------------------------------------*/
/* A function reallocs the memory of an array (int), given as first parameter.
 * Function returns:
 *      a pointer to newly allocated array of integers
 */
int * reallocMemory(int * flightData, int ** maxMemory)
{   
    //Increase memory limit
    **maxMemory += ( **maxMemory < 100 ) ? 10 : **maxMemory / 2;
    
    flightData = (int*) realloc(flightData, **maxMemory * sizeof(int));
    return flightData;
}

/*-----------------------------------------------------------------------------------------*/
/* A function reads the flight data from the stdin.
 * Example: [ 10,6,1,-3 , 9, 10]
 * 
 * Params:
 *      flightDataSum    - the sum of identification messages
 *      flightDataLength - a length of an array (flight data)
 *      flightDataMemory - an own memory limit of dynamically allocated array
 * 
 * Returns:
 *      a pointer to array of integers
 */
int * getFlightData(int * flightDataSum, int * flightDataLength, int * flightDataMemory)
{
    int * flightData = NULL;
    int message;
    char c;
    
    if (scanf(" %c", &c) != 1 || c != '[')
    {
        return NULL;
    }
    
    while (true)
    {
        if (scanf(" %d %c", &message, &c) != 2 || (c != ',' && c != ']'))
        {
            if (*flightDataLength != 0)
                free(flightData);
            return NULL;
        }
        
        if (*flightDataLength >= *flightDataMemory)
            flightData = reallocMemory(flightData, &flightDataMemory);
        
        flightData[*flightDataLength] = message;
        *flightDataSum += message;
        (*flightDataLength)++;
        
        if (c == ']')
            break;
    }
    
    return flightData;
}

/*-----------------------------------------------------------------------------------------*/
/* A function reads the receiver's data from stdin.
 * Example: 1689 : [50, 60, -10, 93   ,  84 , 0, -2 ]
 * 
 * Params:
 *      receiver - the data will be assigned to this receiver
 * Returns:
 *      EOF
 *      0   - An incorrect character
 *      1   - OK
 */
int getReceiverData(Receiver * receiver)
{
    int ID, res, flightDataSum = 0, flightDataLength = 0, flightDataMemory = 0;
    int * flightData = NULL;
    char c;
    
    //Read the receiver's ID
    if ((res = getID(&ID)) == EOF)
        return EOF;
    else if (res == 0)
        return 0;
      
    //Read the separator ':'
    if (scanf(" %c", &c) != 1 || c != ':')
        return 0;
    
    //Read the flight data
    if ((flightData = getFlightData(&flightDataSum, &flightDataLength, &flightDataMemory)) == NULL)
        return 0;
    
    
    receiver->ID = ID;
    receiver->flightData = flightData;
    receiver->flightDataLength = flightDataLength;
    receiver->flightDataSum = flightDataSum;
    receiver->flightDataMemory = flightDataMemory;
    return 1;
}



/*-----------------------------------Red Black Tree part-----------------------------------*/
/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
/* Initialize the red black tree
 */
void initTree(RedBlackTree * tree)
{
    tree->root = NULL;
}
/*-----------------------------------------------------------------------------------------*/
/* A function recursively searches for the receiver by the ID in the red black tree.
 * 
 * Params:
 *      node - the root of tree
 *      ID   - the receiver's ID
 * 
 * Returns:
 *      NULL - a receiver with given ID does not exist
 *      node - a node which contains the receiver with given ID 
 */
Node * searchForReceiver(Node * node, int ID)
{
    if (node == NULL)
        return NULL;
    
    if (ID > node->receiver.ID)
        return searchForReceiver(node->right, ID);
    else if (ID < node->receiver.ID)
        return searchForReceiver(node->left, ID);
    else
        return node;
}

/*--------------------------------------Insertion part-------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
Node * createRedNode(Node * parent, Receiver receiver)
{
    Node * node = (Node*) malloc(sizeof(Node));
    node->receiver = receiver;
    node->color = RED;
    node->parent = parent;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/*-----------------------------------------------------------------------------------------*/
Node * createBlackNode(Receiver receiver)
{
    Node * node = (Node*) malloc(sizeof(Node));
    node->receiver = receiver;
    node->parent = NULL;
    node->color = BLACK;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/*-----------------------------------------------------------------------------------------*/
int isLeftChild(Node * root)
{
    if (root == root->parent->left)
        return true;
    else
        return false;
}

/*-----------------------------------------------------------------------------------------*/
Node * getSiblingNode(Node * root)
{
    if (isLeftChild(root))
        return root->parent->right;
    else
        return root->parent->left;
}

/*-----------------------------------------------------------------------------------------*/
void rightRotate(Node ** root, int changeColor)
{
    Node * parent = (*root)->parent;
    (*root)->parent = parent->parent;
    
    if (parent->parent != NULL)
    {
        if (parent->parent->right == parent)
            parent->parent->right = *root;
        else
            parent->parent->left = *root;
    }
    
    Node * right = (*root)->right;
    (*root)->right = parent;
    parent->parent = *root;
    parent->left = right;
    if (right != NULL)
        right->parent = parent;
    
    if (changeColor)
    {
        (*root)->color = BLACK;
        parent->color = RED;
    }
}

/*-----------------------------------------------------------------------------------------*/
void leftRotate(Node ** root, int changeColor)
{
    Node * parent = (*root)->parent;
    (*root)->parent = parent->parent;
    
    if (parent->parent != NULL)
    {
        if (parent->parent->right == parent)
            parent->parent->right = *root;
        else
            parent->parent->left = *root;
    }
    
    Node * left = (*root)->left;
    (*root)->left = parent;
    parent->parent = *root;
    parent->right = left;
    if (left != NULL)
        left->parent = parent;
    
    if (changeColor)
    {
        (*root)->color = BLACK;
        parent->color = RED;
    }
}

/*-----------------------------------------------------------------------------------------*/
/* A funtion recursively insterts the receiver to red black tree.
 */
Node * insert(Node * parent, Node * root, Receiver receiver)
{
    if (root == NULL)
    {   
        //If the parent exist the tree is not empty -> create a read leaf node
        if (parent != NULL)
            return createRedNode(parent, receiver);
        //Otherwise the tree is empty -> create a black root node
        else
            return createBlackNode(receiver);
    }
    
    //If we go to the left node -> isLeft will be true, else false
    int isLeft;
    
    if (receiver.ID < root->receiver.ID )
    {
        Node * left = insert(root, root->left, receiver);
        
        /* If true = A rotation has happened at lower level and "left" is now a parent of "root" so just return "left" node
         * so nodes at upper level can set their child correctly.
         */
        if (left == root->parent)
            return left;
        
        //Set left node to root's left node in tree
        root->left = left;
        isLeft = true;
    }
    else
    {   
        Node * right = insert(root, root->right, receiver);
        
        //Same case as above, just another direction 
        if (right == root->parent)
            return right;
        
        root->right = right;
        isLeft = false;
    }
    
    
    if (isLeft)
    {   
        //Chceck the Red-Red conflict on left side between root and its child
        if (root->color == RED && root->left->color == RED)
        {   
            //Get the sibling of the root
            Node * sibling = getSiblingNode(root);
            
            //A sibling does not exist or sibling's color is black
            if (sibling == NULL || sibling->color == BLACK)
            {   
                //Check if root is left child of its parent
                if (isLeftChild(root))
                    //Left-Left case -> one right rotation
                    rightRotate(&root, true);
                /* Root is not a left child: 
                 * Left-Right case -> one right rotation (without changing a color) and then left rotation.
                 */
                else
                {   
                    //Right rotation
                    rightRotate(&root->left, false);
                    
                    /* A root after the right rotation becomes right child of its left child
                     * so update this root.
                     */ 
                    root = root->parent;
                    
                    //Left rotation
                    leftRotate(&root, true);
                }
            }
            /* A sibling does exist and its color is red -> change color of the root and its
             * sibling to black and if their parent is not the root of the tree then change its
             * color to red.
             */
            else
            {
                root->color = BLACK;
                sibling->color = BLACK;
                if (root->parent->parent != NULL)
                    root->parent->color = RED;
            }
        }
    }
    //A mirror case of above.
    else
    {   
        if (root->color == RED && root->right->color == RED)
        {
            Node * sibling = getSiblingNode(root);
            if (sibling == NULL || sibling->color == BLACK)
            {
                if (!isLeftChild(root))
                    leftRotate(&root, true);
                else
                {
                    leftRotate(&root->right, false);
                    root = root->parent;
                    rightRotate(&root, true);
                }
            }
            else
            {
                root->color = BLACK;
                sibling->color = BLACK;
                if (root->parent->parent != NULL)
                    root->parent->color = RED;
            }
        }
    }
    return root;
}
void insertToTree(RedBlackTree ** tree, Receiver receiver)
{
    (*tree)->root = insert(NULL, (*tree)->root, receiver);
}



/*-----------------------------------------------------------------------------------------*/
/* A function copies the array (flight data) from the receiver to the end of the receiver's array in node. 
 */
void copyFlightData(Node * node, Receiver receiver)
{   
    //Check if the sum of array's length in node's receiver and in receiver is greater than the memory limit of an array in node's receiver
    if (node->receiver.flightDataMemory <= node->receiver.flightDataLength + receiver.flightDataLength)
    {   
        //Increase own memory limit
        node->receiver.flightDataMemory += receiver.flightDataLength + node->receiver.flightDataMemory / 2;
        //Ralloc
        node->receiver.flightData = (int*) realloc (node->receiver.flightData, node->receiver.flightDataMemory * sizeof(int));
    }
    
    memcpy(node->receiver.flightData + node->receiver.flightDataLength, receiver.flightData, receiver.flightDataLength * sizeof(int));
    node->receiver.flightDataLength += receiver.flightDataLength;
    node->receiver.flightDataSum += receiver.flightDataSum;
    free(receiver.flightData);
}

/*-----------------------------------------------------------------------------------------*/
/* A function inserts the receiver either to the new node in tree or copies receiver's flight data
 * to already existing receiver(node) with same ID.
 */
void insertReceiver(RedBlackTree * tree, Receiver receiver)
{   
    Node * node = searchForReceiver(tree->root, receiver.ID);
   
    //A receiver with its ID does not exist yet -> create new node (new receiver in tree)
    if (node == NULL)
        insertToTree(&tree, receiver);
    //A receiver with its ID already exist in tree -> copy its flight data to receiver in node 
    else
        copyFlightData(node, receiver);
}

/*-----------------------------------------------------------------------------------------*/
/* A function compares the integer arrays (flight data) with naive algorithm.
 * 
 * Returns:
 *       true  - the flight datas are identical
 *       false - the flight datas ate not identical 
 */
int areIdentical(Receiver * receiver1, Receiver * receiver2)
{
    if (receiver1->flightDataLength != receiver2->flightDataLength || receiver1->flightDataSum != receiver2->flightDataSum)
        return false;
      
    int j = 1;
    for(int i = 0; i < receiver1->flightDataLength; ++i)
    {  
        if (receiver1->flightData[0] == receiver2->flightData[i])
        {   
            if (receiver1->flightData[receiver1->flightDataLength-1] == receiver2->flightData[(i + receiver1->flightDataLength - 1) % receiver1->flightDataLength])
            {     
                j = 1;
                
                while (j < receiver1->flightDataLength && receiver1->flightData[j] == receiver2->flightData[(i + j) % receiver2->flightDataLength]) 
                    ++j;

                if (j == receiver1->flightDataLength)
                    return true;
            } 
        }        
    }
    return false;    
}

/*-----------------------------------------------------------------------------------------*/
/* A function recursively compares the node (receiver) passed from findDuplicate function with other
 * nodes (receivers) in tree.
 * 
 * Setting receiver's ID in function as "-1" means this receiver has been already compared and
 * its flight data has been identical with the compared receiver.
 */
void compareFlightData(Node * receiverNode, Node * node)
{
    if (node == NULL)
        return;
    
    compareFlightData(receiverNode, node->right);
    compareFlightData(receiverNode, node->left);
    
    //We want to avoid comparing the same nodes (receivers)
    if (node == receiverNode)
        return;
    
    if (node->receiver.ID != -1)
    {   
        //Check if the flight data in receivers are identical
        if (areIdentical(&receiverNode->receiver, &node->receiver))
        {   
            //Print the ID
            printf("%d, ", node->receiver.ID);
            
            node->receiver.ID = -1;
        }
    }
}

/*-----------------------------------------------------------------------------------------*/
/* A function recursively (bottom-up) passes the node (receiver) to "compareFlightData" function in which
 * this node is compared with other nodes (receivers) in tree.
 * After comparison the node will be freed. 
 */
void findDuplicates(Node * root, Node * node)
{
    if (node == NULL)
        return;
    
    
    findDuplicates(root, node->right);
    findDuplicates(root, node->left);
    if (node->receiver.ID != -1)
    {
        compareFlightData(node, root);
        printf("%d\n", node->receiver.ID);    
    }
    
    free(node->receiver.flightData);
    free(node);
}

/*-----------------------------------------------------------------------------------------*/
/* A function frees the tree from memory.
 */
void destroy(Node * root)
{
    if (root == NULL)
        return;
    
    destroy(root->right);
    destroy(root->left);
    free(root->receiver.flightData);
    free(root);
}

/*-----------------------------------------------------------------------------------------*/
int main() {
    
    Receiver receiver;
    RedBlackTree tree;
    int res;
    
    initTree(&tree);
    
    printf("Flight data:\n");
    //Reading the data (receivers) until EOF
    while (true)
    {   
        //Get the receiver
        res = getReceiverData(&receiver);
        
        if (res == EOF)
            break;
        else if (res == 0)
        {   
            //Error input -> free the entire tree
            destroy(tree.root);
            printf("Error input.\n");     
            return 0;
        }
        else
        {
            //OK - Insert the receiver
            insertReceiver(&tree, receiver);
        }
    }
    
    printf("Unique flight data:\n");
    findDuplicates(tree.root, tree.root);

    return 0;
}

