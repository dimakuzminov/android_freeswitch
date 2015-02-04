#include <stddef.h>
#include <errno.h>

#define ICONV_CONST

#define _LIBICONV_VERSION_MINOR 14
#define _LIBICONV_VERSION_MAJOR 1
#define _LIBICONV_VERSION ((_LIBICONV_VERSION_MAJOR<<8)+_LIBICONV_VERSION_MINOR)    /* version number: (major<<8) + minor */

typedef void* iconv_t;

iconv_t iconv_open(const char *tocode, const char *fromcode);
int     iconv_close(iconv_t cd);
size_t  iconv (iconv_t icd,
              ICONV_CONST char* * inbuf, size_t *inbytesleft,
              char* * outbuf, size_t *outbytesleft);
