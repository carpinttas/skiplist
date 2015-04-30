
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <map>
#include <fstream>
#include <string>

using namespace std;

#define MAXIMUMLEVEL 16 
#define LEVELPROBABILITY 0.5

    typedef struct node {
        int key;
        int value;
        node * forward[MAXIMUMLEVEL+1];
    }node;

    struct skiplist {
        int minimumKey;
        int maximumKey;
        int Maxlevel;
        node * header;
        node * tail;
    };


// Global, who needs classes anyway?
skiplist * list;
int numberofnodes;


void insertNode(int newKey, int newValue){
    node * update[MAXIMUMLEVEL];
    node * auxNode = list->header;
    for(int i=list->Maxlevel; i >=1; i--) {
        while ( auxNode->forward[i]->key < newKey ) {
            auxNode = auxNode->forward[i];
        }
        update[i] = auxNode;
    }
    auxNode = auxNode->forward[1];
    if ( auxNode->key == newKey ) {
        auxNode->value = newValue;
    } else {
        int randomLevel = 1;
        while ( (rand() / double(RAND_MAX)) < LEVELPROBABILITY && randomLevel < MAXIMUMLEVEL ) {
            randomLevel++;
        }

        if ( randomLevel > list->Maxlevel ) {
            for ( int i = list->Maxlevel+1; i <= randomLevel; i++ ) {
                update[i] = list->header;
            }
         	list->Maxlevel = randomLevel;
        }
        node * newNode = new node();
        newNode->key = newKey;
        newNode->value = newValue;
        for ( int i=1; i<=MAXIMUMLEVEL; i++ ) {
            newNode->forward[i] = NULL;
        }

        for ( int i=1; i<=list->Maxlevel; i++ ) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }
}

void deleteNode(int nodeKey){
    node * update[MAXIMUMLEVEL];
    node* auxNode = list->header;
    for(int i=list->Maxlevel; i >=1; i--) {
        while ( auxNode->forward[i]->key < nodeKey ) {
            auxNode = auxNode->forward[i];
        }
        update[i] = auxNode;
    }
    auxNode = auxNode->forward[1];
    if ( auxNode->key == nodeKey ) {
        for ( int i = 1; i <=list->Maxlevel; i++ ) {
            if ( update[i]->forward[i] != auxNode ) {
                break;
            }
            update[i]->forward[i] = auxNode->forward[i];
        }
        free(auxNode);
        
        while (list->Maxlevel > 1 && list->header->forward[list->Maxlevel] == NULL ) {
           list->Maxlevel--;
        }
    }
}

const node* searchNode(int nodeKey)
{
    node* auxNode = list->header;
    for(int i=list->Maxlevel; i >=1; i--) {
        while ( auxNode->forward[i]->key < nodeKey ) {
            auxNode = auxNode->forward[i];
        }
    }
    auxNode = auxNode->forward[1];
    if ( auxNode->key == nodeKey ) {
        return auxNode;
    }
    else {
        return NULL;
    }
}



int main(int argc, char** argv) {
    string filename = "";

    if( argc == 2 )
      filename = argv[1];
  	else
  		return 0;

	list = new skiplist();

	fstream inputFile(filename.c_str(), ios_base::in);

	inputFile >> numberofnodes;
	inputFile >> list->minimumKey;
	inputFile >> list->maximumKey;

	printf("%d\n", numberofnodes);
	printf("%d\n", list->minimumKey);
	printf("%d\n", list->maximumKey);
    
    list->Maxlevel = 1;

    list->header = new node();
    list->tail = new node();
    list->header->key = list->minimumKey;
    list->tail->key = list->maximumKey;


    for ( int i=1; i<=MAXIMUMLEVEL; i++ ) {
       list->header->forward[i] = list->tail;
       list->tail->forward[i] = NULL;
    }
	

    // insert nodes
    int inputKey;
    int inputValue = 0;
    int * keys = new int[numberofnodes+1];
    while (inputFile >> inputKey)
    {
    	inputValue++;
       	keys[inputValue] = inputKey;
        insertNode(inputKey, inputValue);
        
    }
    printf("\n\nNodes inserted: %d\n\n",inputValue);

	
    /* search for all nodes
    const node * resultNode;
    for (int i=1; i<=numberofnodes; i++)	{
		resultNode = searchNode(keys[i]);
		if(resultNode != NULL)
			printf("%d:%d  ", resultNode->key, resultNode->value);
	}
	printf("\n");

    /*print everything on the list
    auxNode = list->header->forward[1];
   
    while ( auxNode !=list->tail ) {
        cout << "(" << auxNode->key << "," << auxNode->value << ")" << endl;
        auxNode = auxNode->forward[1];
    }*/

    //free memory

    node * auxNode = list->header->forward[1];
    while(auxNode != list->tail){
        node * freeNode = auxNode;    
        auxNode = auxNode->forward[1];
        free(freeNode);
    }
    delete list->tail;
    delete list->header;
    delete list;

    cout << "end";
}

