ifeq (,$(wildcard /usr/include/gbm.h))
	DEFINES = 
else
	DEFINES = -DHAVE_GBM
endif
egl: main.cpp
	scl enable devtoolset-7 'g++ $(DEFINES) -o egl -g main.cpp -lEGL -lGL -lOpenGL -lGLEW -lgbm -lpng'

clean:
	rm -rf egl
