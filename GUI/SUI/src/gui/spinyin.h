#ifndef SPINYIN_H
#define SPINYIN_H

struct t9_py
{
	int t;
	char py[8];
	char *str;
};

const char * py_ime(char* py);

const struct t9_py* t9_py_ime(char *t9);
const struct t9_py* t9_py_next(const struct t9_py *t9);


#endif
