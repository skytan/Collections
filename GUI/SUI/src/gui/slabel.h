#ifndef  SLABEL_H
#define  SLABEL_H

#include "scontrol.h"

class sLabel:public sControl
{
public:
      sLabel(const char *str = 0);
      ~sLabel();
      
      virtual void update();
};

#endif