#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__amigaos__) && defined(__USE_INLINE__)
#include <proto/expat.h>
#endif

#ifdef XML_LARGE_SIZE
#if defined(XML_USE_MSC_EXTENSIONS) && _MSC_VER < 1400
#define XML_FMT_INT_MOD "I64"
#else
#define XML_FMT_INT_MOD "ll"
#endif
#else
#define XML_FMT_INT_MOD "l"
#endif

#include "expat.h"
#include "strstack.h"

static void XMLCALL
startElement(void *userdata, const char *tag, const char **atts)
{
    StrStack *stack = (StrStack*) userdata;
    strstack_push(stack, tag);
    
    for (int I = 0;; I += 2) {
        for (int i = 0; i < stack->size; i++) {
            printf("/%s", stack->data[i]);
        }
        const char *name = atts[I];
        const char *cont = atts[I+1];
        if (name) {
            printf("/@%s\t%s\n", name, cont);
        } else {
            puts("");
            break;
        }
    }
}

static void XMLCALL
endElement(void *userdata, const char *tag)
{
    StrStack *stack = (StrStack*) userdata;
    strstack_popd(stack);
}

int
main(int argc, char *argv[])
{
    StrStack _stack;
    StrStack *stack = &_stack;

    int err = strstack_init(stack, 5);
    if (err) {
        fprintf(stderr, "Problem initializing stack\n");
        return -1;
    }

    char buf[BUFSIZ];
    XML_Parser parser = XML_ParserCreate(NULL);
    int done;

    XML_SetUserData(parser, stack);
    XML_SetElementHandler(parser, startElement, endElement);
  
    do {
        int len = (int)fread(buf, 1, sizeof(buf), stdin);
        done = len < sizeof(buf);
        if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR) {
            fprintf(stderr,
                "%s at line %" XML_FMT_INT_MOD "u\n",
                XML_ErrorString(XML_GetErrorCode(parser)),
                XML_GetCurrentLineNumber(parser));
            return 1;
        }
    } while (!done);

    XML_ParserFree(parser);

    return 0;
}
