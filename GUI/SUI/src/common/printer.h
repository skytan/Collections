#ifndef PRINTER_H
#define PRINTER_H

struct printer
{
	int 	fd;
	struct 	termios oldtio;
};

int printer_open(struct printer *ptrPri);
int printer_close(struct printer *ptrPri);

int printer_char(struct printer *ptrPri, char c, int count);
int printer_string(struct printer *ptrPri, char *str);
int printer_space_string(struct printer *ptrPri, int count, char *str);

#endif
