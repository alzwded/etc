DEFINES = -DGLEW_EGL -DHAVE_GBM
INCLUDES = -I/usr/X11R6/include -I/usr/local/include/libpng16 -L/usr/X11R6/lib -L/usr/local/lib -I/usr/local/include
all: egles egl
egles: main_es2.cpp
	clang++ $(DEFINES) $(INCLUDES) -o egles -g main_es2.cpp -lGLESv2 -lEGL -lpng -lgbm
egl: main.cpp
	clang++ $(DEFINES) $(INCLUDES) -o egl -g main.cpp -lGL -lEGL -lGLEW -lpng -lgbm
	#c++ $(DEFINES) $(INCLUDES) -o egl -g main.cpp -lEGL -lGLEW -lpng -lGL -lOpenGL

clean:
	rm -rf egl egles
