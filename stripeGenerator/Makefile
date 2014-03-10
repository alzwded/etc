CFLAGS = -g
LDFLAGS = -lpng

WIDTH = 800
HEIGHT = 800
LINE1WIDTH = 110
LINE2WIDTH = 65
LINE1COLOUR = 0xFF410D
LINE2COLOUR = 0xD6D25A

SHOW = 1

stripes.png: stripeGenerator
	./stripeGenerator stripes.png $(WIDTH) $(HEIGHT) $(LINE1WIDTH) $(LINE2WIDTH) $(LINE1COLOUR) $(LINE2COLOUR)
	[ $(SHOW) ] && display stripes.png

stripeGenerator: stripeGenerator.c
	gcc -o stripeGenerator $(CFLAGS) stripeGenerator.c $(LDFLAGS)

clean:
	rm -f stripeGenerator stripes.png
