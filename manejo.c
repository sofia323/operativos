// hace el manejo como tal de todas las operaciones que puede realizar la memoria
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

char main[32];
char pageTable[32];
char virtual[  ];
int sizeofRam = sizeof(pageTable);
int sizeofVirtual = sizeof(virtual);
char indicartorTable[52] ={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u',
            'v','x','y','w','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','Y','W','Z'};
int freeSpaceMain = 32;
int freeSpaceVirtual = ;

//reserve ram memory with a descriptor
char reserve(int space){
    char indicator = findIndicator();
    for (int i = 0; i < sizeofMain; ++i){
    	if (pageTable[i] == 0 && space > 0){
            pageTable[i] = indicator;
            space--;
    	}
    }
    return indicator;
}
//read the memory of the descriptor
void read(char descriptor){
	for (int i = 0; i < sizeofMain; ++i){
		if(pageTable[i] == descriptor){
			printf("%c",main[i]);
		}
	}
}

//write on ram a word and in the page table saves the descriptor
void write(char descriptor, char *palabra){
	int len= strlen(palabra);
    freeSpaceMain -= len;
	int wordCounter = 0;  
	for (int i = 0; i < sizeofMain; i++){
		if(pageTable[i] == 0 && wordCounter < len){
			main[i] = palabra[wordCounter];
			pageTable[i] = descriptor;
			wordCounter++;
		}
	}
}
