include ./colors.mk

CXX := clang++

CFLAGS := -Wall -std=c++17 -I/usr/local/include/ -Iinclude/ -O2 $(shell Magick++-config --cxxflags --cppflags)
LDFLAGS := -w $(shell Magick++-config --ldflags --libs)

SOURCES := $(shell find lib -name '*.cpp')
OBJ := $(patsubst lib/%.cpp,build/%.o,$(SOURCES))
DEPS := $(patsubst lib/%.cpp,deps/%.d,$(SOURCES))
HEADERS := $(shell find include -name '*.hpp')

TARGET := main

all:
	@#printf '$(RED)>$(BLK) Building target $(TARGET)$(RST)\n'
	@mkdir -p build
	@mkdir -p build/deps
	@make $(TARGET) --no-print-directory

$(TARGET): $(OBJ) ${HEADERS}
	@#printf '$(YLW)==>$(BLK) Linking target $(TARGET)$(RST)\n'
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)


$(OBJ): build/%.o : lib/%.cpp
	@#printf '$(GRN)==>$(BLK) Compiling $<$(RST)\n'
	$(CXX) -c $< $(CFLAGS) -o $@ -MD -MF $(patsubst build/%.o,build/deps/%.d,$@)

clean:
	@printf '$(RED)>$(BLK) Cleaning target $(TARGET)$(RST)\n'
	@find . -name '*.o' -exec rm -rf {} \;
	@rm -rf build/deps
	@rm -rf build
	@rm -rf main
