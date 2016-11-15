#sources := $(wildcard *.c)
sources := main.cpp window_sale.cpp window_main.cpp common.cpp window_login.cpp window_goods.cpp
objects := $(sources:%.cpp=%.o)
CPPFLAGS = -I gui  -I common
CXXFLAGS = -g -I gui -I common
CFLAGS = -g -I gui -I common 
LDFLAGS = -g
lib_object = libsui.a libdatas.a
target = bank

.PHONY:all
all:depend $(target)
	

depend:
	$(MAKE) -C gui all
	$(MAKE) -C common all
	@$(CXX) $(CPPFLAGS) -MM $(sources) > .depend

-include .depend

$(target):$(objects) $(lib_object)
	$(CXX) $(LDFLAGS) -o $@ $^
	@rm -f .depend


.PHONY:clean
clean:
	$(MAKE) -C gui clean
	$(MAKE) -C common clean
	rm -f $(target) $(objects)
