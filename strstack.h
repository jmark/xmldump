typedef struct
{
    int size;   // size of actual filled elements
    int SIZE;   // size of available storage
    char **data;
} StrStack;

int strstrstack_resize(StrStack *const stack, int N);
int strstack_init(StrStack *const stack, const int SIZE);
char* strstack_push(StrStack *const stack, const char *val);
char* strstack_pop(StrStack *stack);
char* strstack_popd(StrStack *stack);
