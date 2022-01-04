CXX = x86_64-w64-mingw32-gcc

BUILD_FLAGS  = -Wall -Wextra -fpermissive -fPIC -DPIC -pipe
BUILD_FLAGS += -O3 -ffast-math -mtune=generic -msse -msse2 -mfpmath=sse
BUILD_FLAGS += -std=gnu++0x -shared $(CXXFLAGS) $(LDFLAGS)

all: win64

win64: vst.dll

vst.dll: src/WineVSTWrapper.cpp
	$(CXX) $(BUILD_FLAGS) -o vst.dll

clean:
	rm -f *.dll


.PHONY: aeffectx.hpp compile_flags.txt

aeffectx.hpp:
	curl -L https://raw.githubusercontent.com/robbert-vdh/yabridge/master/src/include/vestige/aeffectx.h -o src/aeffectx.hpp

compile_flags.txt:
	test -e compile_flags.txt || echo '-I' >compile_flags.txt
	sed -i 's|-I.*$$|-I$(MINGW_INCLUDE)|' compile_flags.txt
