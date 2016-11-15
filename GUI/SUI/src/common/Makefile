#sources := $(wildcard *.c)
sources := goods.cpp invoice.cpp mpm78m_printer.cpp config_query.cpp
objects := $(sources:%.cpp=%.o)
CPPFLAGS =  -g -fPIC  -I .. -I ../gui -I .
CFLAGS = -g -fPIC  -I .. -I ../gui -I .
LDFLAGS =  
SFLAGS = -fPIC -shared
static_target = ../libdatas.a
share_target = ../libdatas.so.0

.PHONY:all
all:depend $(static_target) $(share_target) 

depend:
	@$(CXX)  $(CFLAGS) -MM $(sources) > .depend

-include .depend


$(static_target):$(objects)
	$(AR) -rcs  $@ $^ 
	@rm -f .depend

$(share_target):$(objects)
	$(CXX) $(SFLAGS) -o $@ $^


.PHONY:clean
clean:
	rm -f $(static_target) $(share_target) $(objects)
