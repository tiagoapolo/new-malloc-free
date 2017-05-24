//
//  main.cpp
//  myAlloc
//
//  Created by Tiago Lopes de Paiva Dionysio da Fonseca on 23/05/17.
//  Copyright © 2017 Apolox. All rights reserved.
//

#include <iostream>
#include<stdio.h>
#include <unistd.h>
#include <string.h>

using namespace std;

#define BLOCK_SIZE 30

typedef struct s_block *t_block;

void *base = NULL;

struct s_block {
    size_t size;
    t_block next;
    int free;
    void *ptr;
    char data[1];
};

t_block findBlock(t_block *last , size_t size ){
    
    t_block temp = (s_block *)base;
    
    while (temp && !(temp->free && temp->size >= size)) {
        *last = temp;
        temp = temp->next;
    }
    return (temp);
}

t_block growHeap(t_block last, size_t size){
    t_block temp;
    temp = (s_block *)sbrk(0);
    
    if (sbrk(BLOCK_SIZE + size) < 0){
        printf("\n\nNot possible!\n\n");
        return NULL;
    }
    
    temp->size = size;
    temp->next = NULL;
    temp->ptr = temp->data;
    
    if(last) last->next = temp;
    
    temp->free = 0;
    
    return temp;
}

t_block getBlock (void *p){
    char *tmp;
    tmp = (char *) p;
    t_block temp = (t_block)base;
    
    while (temp != NULL) {
        if (temp->ptr == p) {
            return temp;
        }
        temp = temp->next;
    }
    return (t_block)(p = tmp -= BLOCK_SIZE);
}


void *meu_malloc(unsigned short nbytes)
{
    printf("\n\nmeu_malloc() -- NOVA MEMÓRIA: PROGRAM-BREAK DESLOCADO\n");
    printf("tam = %hu\n", nbytes);
    
    t_block temp, last;
    size_t  s = nbytes;
    
    if(base){
        last = (t_block)base;
        temp = findBlock(&last, s);
        if(temp){
            temp->free = 0;
        } else {
            temp = growHeap(last, s);
            if(!temp) return NULL;
        }
    } else {
        temp = growHeap(NULL, s);
        if(!temp) return NULL;
        base = temp;
    }
    printf("início = 0x%0x\n", (t_block)temp->data);
    return temp->data;
}



void * meu_free(void *p)
{
    t_block temp;

    printf("\n\nmeu_free()\n");
    
    temp = getBlock(p);
    printf("tam = %lu\n", strlen((char *)p)+1);
    printf("início = 0x%0x\n", temp->data);
    temp->free = 1;

    
    return 0;
}

void preenche_x(char *p, unsigned short tam)
{
    int i;
    for (i=0; i<tam-1; ++i)
        *(p+i) = 'X';
    *(p+tam-1) = '\0';
    printf("Mem: %s\n\n",p);
}

int main(int argc, char* argv[])
{
    char *p = (char*) meu_malloc(30);
    preenche_x(p,30);
    
    char *q = (char*) meu_malloc(20);
    preenche_x(q,20);
    
    meu_free(p);
    
    char *r = (char*) meu_malloc(40);
    preenche_x(r,40);
    
    char *s = (char*) meu_malloc(20);
    preenche_x(s,20);
    
    meu_free(q);
    meu_free(r);
    meu_free(s);
    
    return 0;
}
