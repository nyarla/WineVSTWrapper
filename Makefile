CXX = x86_64-w64-mingw32-gcc

BUILD_FLAGS  = -Wall -Wextra -fpermissive -fPIC -DPIC -pipe
BUILD_FLAGS += -O3 -ffast-math -mtune=generic -msse -msse2 -mfpmath=sse
BUILD_FLAGS += -std=gnu++0x -shared $(CXXFLAGS) $(LDFLAGS)

all: vst.dll

vst.dll: src/WineVSTWrapper.cpp
	$(CXX) $^ $(BUILD_FLAGS) -o vst.dll 

clean:
	rm -f *.dll

.PHONY: aeffectx.hpp compile_commands.json

aeffectx.hpp:
	curl -L https://raw.githubusercontent.com/robbert-vdh/yabridge/master/src/include/vestige/aeffectx.h -o src/aeffectx.hpp

compile_commands.json:
	bear --output compile_commands.json -- make
