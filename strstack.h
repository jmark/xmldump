#include <stdlib.h>
#include <string.h>

typedef struct
{
    int size;   // size of actual filled elements
    int SIZE;   // size of available storage
    char **data;
} StrStack;

inline int
strstack_resize(StrStack *const stack, const int N)
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
extern inline int strstack_resize(StrStack *const stack, const int N);

int
strstack_init(StrStack *const stack)
{
    stack->data = 0;
    stack->size = 0;
    stack->SIZE = 0;
    
    return strstack_resize(stack, 10);
}
extern inline int strstack_init(StrStack *const stack);

char*
strstack_push(StrStack *const stack, const char *val)
{
    if (stack->size + 1 > stack->SIZE)
        if (strstack_resize(stack, 10 + stack->size / 10))
            return NULL;    

    const int len = strlen(val);
    char *const tmp = malloc(sizeof(char) * len);
    strcpy(tmp, val);

    stack->size++; 
    stack->data[stack->size-1] = tmp;
    return tmp;
}
extern inline char* strstack_push(StrStack *const stack, const char *val);

char*
strstack_pop(StrStack *const stack)
{
    if (stack->size - 1 < 0)
        return NULL;
    stack->size--;
    return stack->data[stack->size];
}
extern inline char* strstack_pop(StrStack *stack);

char*
strstack_popd(StrStack *const stack)
{
    free(strstack_pop(stack));
    return NULL;
}
extern inline char* strstack_popd(StrStack *stack);
