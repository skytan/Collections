#ifndef WINDOW_CATEGORY_H
#define WINDOW_CATEGORY_H

#include "perr.h"
#include "config.h"

int create_category_window();

struct category_gui
{
	GtkWidget *window;
	GtkWidget *name;
	GtkWidget *number;
	GtkWidget *taxName;
};
      
#endif