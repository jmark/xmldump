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

typedef struct {
    int level;
    int error;
    char *cdata;    
    StrStack tags;
    long lastokbyte;
    long lastokline;
    XML_Parser parser;
} UserData;

static void XMLCALL
startElement(void *userdata, const char *tag, const char **atts)
{
    UserData *ud = (UserData*) userdata;
    StrStack *tags = &(ud->tags);

    strstack_push(tags, tag);
    
    for (int I = 0;; I += 2) {
        for (int i = 0; i < tags->size; i++) {
            printf("/%s", tags->data[i]);
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
    ud->lastokbyte = XML_GetCurrentByteIndex(ud->parser);
    ud->lastokline = XML_GetCurrentLineNumber(ud->parser);
}

static void XMLCALL
endElement(void *userdata, const char *tag)
{
    UserData *ud = (UserData*) userdata;
    strstack_popd(&(ud->tags));
    ud->lastokbyte = XML_GetCurrentByteIndex(ud->parser);
    ud->lastokline = XML_GetCurrentLineNumber(ud->parser);
}

static void XMLCALL
hdlCharacterData(void *userdata, const XML_Char *data, int len)
{
    UserData *ud = (UserData*) userdata;
    ud->lastokbyte = XML_GetCurrentByteIndex(ud->parser);
    ud->lastokline = XML_GetCurrentLineNumber(ud->parser);
}

int
main(int argc, char *argv[])
{
    // --------------------------------------------------------------------- //
    XML_Parser parser = XML_ParserCreate(NULL);

    // --------------------------------------------------------------------- //
    // setup user data
    UserData userdata;

    int err = strstack_init(&(userdata.tags));
    if (err) {
        fprintf(stderr, "Problem initializing stack\n");
        return -1;
    }

    userdata.parser = parser;
    userdata.lastokbyte = 0;
    userdata.lastokline = 1;

    // --------------------------------------------------------------------- //
    // register handlers
    XML_SetUserData(parser, &userdata);
    XML_SetElementHandler(parser, startElement, endElement);
    XML_SetCharacterDataHandler(parser, hdlCharacterData);
 
    // --------------------------------------------------------------------- //
    // enter data pull oriented loop 
    int done = 0;
    while (!done) {
        char buf[BUFSIZ];
        const int len = (int)fread(buf, 1, sizeof(buf), stdin);
        done = len < sizeof(buf);
        if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR) {
            const int errcode   = XML_GetErrorCode(parser);
            const char *errstr  = XML_ErrorString(errcode);
            const int lineno    = XML_GetCurrentLineNumber(parser);
            const int colno     = XML_GetCurrentColumnNumber(parser);

            // ------------------------------------------------------------- //
            // print helpfull error message + context where error was found
            fprintf(stderr,
                "Error %d: %s at line %d, column %d\n",
                errcode, errstr, lineno, colno);

            char *eptr = buf + userdata.lastokbyte;             
            for (int i = userdata.lastokline; i <= lineno; i++) {
                char* const newline = strchr(eptr, '\n');
                fprintf(stderr, "%2d: %.*s\n", i, newline - eptr, eptr);
                eptr = newline + 1;
            }
            //for (int i = -2; i < colno; i++)
            //    fprintf(stderr, "-");
            //fprintf(stderr, "^^^\n");
            return -1;
        }
    }

    XML_ParserFree(parser);

    return 0;
}
