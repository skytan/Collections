#ifndef SENCODED_H
#define SENCODED_H


int unicode_to_utf8(const wchar_t *in, char *out);
int unicode_to_utf8(const wchar_t *in, char *out, int outsize);


int utf8_to_unicode(const char *in, wchar_t *out);
int utf8_to_unicode(const char *in, wchar_t *out, int size);


int unicode_to_gbk2312(const wchar_t *in, char *out);
int unicode_to_gbk2312(const wchar_t *in, char *out, int outsize);


int utf8_to_gbk2312(const char *in, char *out);
int utf8_to_gbk2312(const char *in, char *out, int outsize);

#endif
