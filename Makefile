src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)
dep = $(obj:.o=.d)  # one dependency file for each source

LDFLAGS = `PKG_CONFIG_PATH=/usr/local/Cellar/gtk+3/3.22.30/lib/pkgconfig pkg-config --libs gtkmm-3.0`
CFLAGS = `PKG_CONFIG_PATH=/usr/local/Cellar/gtk+3/3.22.30/lib/pkgconfig pkg-config --cflags gtkmm-3.0` -g
CXXFLAGS = $(CFLAGS) -std=c++17

tbgb: $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

-include $(dep)   # include all dep files in the makefile

# rule to generate a dep file by using the C preprocessor
# (see man cpp for details on the -MM and -MT options)
%.d: %.cpp
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(obj) tbgb

.PHONY: cleandep
cleandep:
	rm -f $(dep)
