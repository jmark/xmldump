#include <stdlib.h>
#include <string.h>
#include "strstack.h"

int
strstack_resize(StrStack *const stack, int N)
{
    //TODO: consider negative n
    if (N <= 0)
        return 0;

    // copy data from old buffer to new buffer
    int NEWSIZE = stack->SIZE + N;
    char **tmp = malloc(sizeof(char*) * NEWSIZE);
    if (!tmp)
        return -1;
    for (int i = 0; i < stack->size; i++)
        tmp[i] = stack->data[i];
   
    if (stack->data) 
        free(stack->data);

    stack->data = tmp;
    stack->SIZE = NEWSIZE;

    return 0; // everything went fine
}

int
strstack_init(StrStack *const stack, const int SIZE)
{
    stack->data = 0;
    stack->size = 0;
    stack->SIZE = 0;
    
    return strstack_resize(stack, SIZE);
}

char*
strstack_push(StrStack *const stack, const char *val)
{
    if (stack->size + 1 > stack->SIZE) {
        int err = strstack_resize(stack, 10);
        if (err)
            return NULL;    
    }

    char *tmp = malloc(sizeof(char) * 50);
    strncpy(tmp, val, 50);

    stack->size++; 
    stack->data[stack->size-1] = tmp;
    return tmp;
}

char*
strstack_pop(StrStack *stack)
{
    if (stack->size - 1 < 0)
        return NULL;
    stack->size--;
    return stack->data[stack->size];
}

char*
strstack_popd(StrStack *stack)
{
    free(strstack_pop(stack));
    return NULL;
}

char*
strstack_get_tail(StrStack *stack)
{
    if (!stack->SIZE)
        return NULL;
    return stack->data[stack->SIZE-1];
}

char*
strstack_set_tail(StrStack *stack, char *val)
{
    if (!stack->SIZE)
        return NULL;
    char *tmp = stack->data[stack->SIZE-1];
    stack->data[stack->SIZE-1] = val;
    return tmp;
}
