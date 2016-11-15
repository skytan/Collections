#sources := $(wildcard *.cpp)
sources := main.cpp sbutton.cpp sentry.cpp sevent.cpp sfont16.cpp sgb2312.cpp \
	slabel.cpp sperr.cpp spyime.cpp swidget.cpp \
	sapplication.cpp  scontrol.cpp  sencoded.cpp  sfont12.cpp  \
	sframebuffer.cpp  skeyevent.cpp  slist.cpp   spinyin.cpp  \
	stable.cpp  swordgb2312.cpp sobject.cpp smessagebox.cpp \
	st9ime.cpp sscrolled.cpp
objects := $(sources:%.cpp=%.o)
CXX = g++
CPPFLAGS = -g
CFLAGS = -g
LDFLAGS = -g
SFLAGS = -fPIC -shared
target = sui
static_target = ../libsui.a
share_target = ../libsui.so.0 

.PHONY:all
all:depend $(target) $(static_target) $(share_target)

depend:
	@$(CXX) -MM $(sources) > .depend

-include .depend

$(target):$(objects)
	$(CXX) $(CFLAGS) -o $@ $^

$(static_target):$(objects)
	$(AR) -rcs  $@ $(filter-out main.o,$(objects))

$(share_target):$(objects)
	$(CXX) $(SFLAGS) -o $@ $(filter-out main.o,$(objects))
	@rm -f .depend


.PHONY:clean
clean:
	rm -f $(target) $(static_target) $(share_target) $(objects) 
