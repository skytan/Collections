/*****************************************************************
 * outline.c
 *
 * Copyright 1999, Clark Cooper
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the license contained in the
 * COPYING file that comes with the expat distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Read an XML document from standard input and print an element
 * outline on standard output.
 * Must be used with Expat compiled for UTF-8 output.
 */

#include <stdio.h>
#include <stdlib.h>
#include <expat.h>


int Eventcnt = 0;
char Buff[512];

void
default_hndl(void *data, const char *s, int len) {
  fwrite(s, len, sizeof(char), stdout);
}  /* End default_hndl */

/****************************************************************
 ** Call from within a handler to print the currently recognized
 ** document fragment. Temporarily set the default handler and then
 ** invoke it via the the XML_DefaultCurrent call.
 */
void
printcurrent(XML_Parser p) {
  XML_SetDefaultHandler(p, default_hndl);
  XML_DefaultCurrent(p);
  XML_SetDefaultHandler(p, (XML_DefaultHandler) 0);
}  /* End printcurrent */

void
start_hndl(void *data, const char *el, const char **attr) {
  printf("\n%4d: Start tag %s - ", Eventcnt++, el);

  printcurrent((XML_Parser) data);
}  /* End of start_hndl */


void
end_hndl(void *data, const char *el) {
  printf("\n%4d: End tag %s -\n", Eventcnt++, el);
}  /* End of end_hndl */

void
char_hndl(void *data, const char *txt, int txtlen) {
  printf("\n%4d: Text - ", Eventcnt++);
  fwrite(txt, txtlen, sizeof(char), stdout);
}  /* End char_hndl */

void
proc_hndl(void *data, const char *target, const char *pidata) {
  printf("\n%4d: Processing Instruction - ", Eventcnt++);
  printcurrent((XML_Parser) data);
}  /* End proc_hndl */

void
main(int argc, char **argv) {
  XML_Parser p = XML_ParserCreate(NULL);
  if (! p) {
    fprintf(stderr, "Couldn't allocate memory for parser\n");
    exit(-1);
  }

  XML_UseParserAsHandlerArg(p);
  XML_SetElementHandler(p, start_hndl, end_hndl);
  XML_SetCharacterDataHandler(p, char_hndl);
  XML_SetProcessingInstructionHandler(p, proc_hndl);

  /* Notice that the default handler is not set at this point */

  for (;;) {
    int done;
    int len;
    fgets(Buff, sizeof(Buff), stdin);
    len = strlen(Buff);
    if (ferror(stdin)) {
      fprintf(stderr, "Read error\n");
      exit(-1);
    }
    done = feof(stdin);
    if (! XML_Parse(p, Buff, len, done)) {
      fprintf(stderr, "Parse error at line %d:\n%s\n",
	      XML_GetCurrentLineNumber(p),
	      XML_ErrorString(XML_GetErrorCode(p)));
      exit(-1);
    }

    if (done)
      break;
  }
  printf("\n");
}  /* End main */
#if 0
void startElement (void *userData, const char *name, const char **atts)
{
    int i;
    int *depthPtr = userData;

    for (i = 0; i < *depthPtr; i++)
    {
		printf("\t");
	}
    puts(name);

    for (i = 0; atts[i]; i += 2)
    {
        printf("\t%s='%s'", atts[i], atts[i + 1]);
    }

    printf("\n");
    *depthPtr += 1;
}

void endElement (void *userData, const char *name)
{
    int *depthPtr = userData;
    *depthPtr -= 1;
}

int main (int argc, char *argv[])
{
    char buf[BUFSIZ];
    XML_Parser parser = XML_ParserCreate (NULL);
    int done;
    int depth = 0;
    FILE *fp;

    if ((fp = fopen("test.xml", "r")) == NULL)
    {
        printf("Can't open %s\n", argv[1]);
        exit(1);

    }

    XML_SetUserData (parser, &depth);
    XML_SetElementHandler (parser, startElement, endElement);
    do
    {
        size_t len = fread (buf, 1, sizeof (buf), fp);
        // done = len < sizeof(buf);
        done = feof (fp);
        if (!XML_Parse (parser, buf, len, done))
        {
            fprintf (stderr, "%s at line %d\n", XML_ErrorString (XML_GetErrorCode (parser)), XML_GetCurrentLineNumber (parser));
            return 1;
        }
    }
    while (!done);
    XML_ParserFree (parser);
    return 0;
}
#endif


#if 0
#include <stdio.h>
#include <expat.h>

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

#define BUFFSIZE        8192

char Buff[BUFFSIZE];

int Depth;

static void XMLCALL
start(void *data, const char *el, const char **attr)
{
  int i;

  for (i = 0; i < Depth; i++)
    printf("  ");

  printf("%s", el);

  for (i = 0; attr[i]; i += 2) {
    printf(" %s='%s'", attr[i], attr[i + 1]);
  }

  printf("\n");
  Depth++;
}

static void XMLCALL
end(void *data, const char *el)
{
  Depth--;
}

int
main(int argc, char *argv[])
{
  XML_Parser p = XML_ParserCreate(NULL);
  if (! p) {
    fprintf(stderr, "Couldn't allocate memory for parser\n");
    exit(-1);
  }

  XML_SetElementHandler(p, start, end);

  for (;;) {
    int done;
    int len;

    len = (int)fread(Buff, 1, BUFFSIZE, stdin);
    if (ferror(stdin)) {
      fprintf(stderr, "Read error\n");
      exit(-1);
    }
    done = feof(stdin);

    if (XML_Parse(p, Buff, len, done) == XML_STATUS_ERROR) {
      fprintf(stderr, "Parse error at line %" XML_FMT_INT_MOD "u:\n%s\n",
              XML_GetCurrentLineNumber(p),
              XML_ErrorString(XML_GetErrorCode(p)));
      exit(-1);
    }

    if (done)
      break;
  }
  XML_ParserFree(p);
  return 0;
}
#endif