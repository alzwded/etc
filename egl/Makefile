ifeq (,$(wildcard /usr/include/gbm.h))
	DEFINES = -DGLEW_EGL
else
	DEFINES = -DGLEW_EGL -DHAVE_GBM
endif
all: egles egl
egles: main_es2.cpp
	scl enable devtoolset-7 'g++ $(DEFINES) -o egles -g main_es2.cpp -lGLESv2 -lEGL -lgbm -lpng'
egl: main.cpp
	scl enable devtoolset-7 'g++ $(DEFINES) -o egl -g main.cpp -lEGL -lGLEW -lgbm -lpng -lGL -lOpenGL'

clean:
	rm -rf egl egles
