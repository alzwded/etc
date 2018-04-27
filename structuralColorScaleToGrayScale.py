#!/usr/bin/env python
#
# -------------------------------------------------------------------------------------
#
# Copyright (c) 2013, Jose F. Maldonado
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification, 
# are permitted provided that the following conditions are met:
#
#    - Redistributions of source code must retain the above copyright notice, this 
#    list of conditions and the following disclaimer.
#    - Redistributions in binary form must reproduce the above copyright notice, 
#    this list of conditions and the following disclaimer in the documentation and/or 
#    other materials provided with the distribution.
#    - Neither the name of the author nor the names of its contributors may be used 
#    to endorse or promote products derived from this software without specific prior 
#    written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
# SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR 
# BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
# DAMAGE.
#
# -------------------------------------------------------------------------------------
#
# This file is a basic example of a Python plug-in for GIMP.
#
# It can be executed by selecting the menu option: 'Filters/Test/Discolour layer v1'
# or by writing the following lines in the Python console (that can be opened with the
# menu option 'Filters/Python-Fu/Console'):
# >>> image = gimp.image_list()[0]
# >>> layer = image.layers[0]
# >>> gimp.pdb.python_fu_test_discolour_layer_v1(image, layer)


# original taken from https://github.com/jfmdev/PythonFuSamples/blob/master/test-discolour-layer-v1.py
# put it in %USERPROFILE%\.gimp-2.8\plug-ins
# should show up in Filters / test / colormapToGrayscale

from gimpfu import *
import colorsys

def colormapToGrayscale(img, layer) :
    #img = gimp.image_list()[0]
    #layer = image.layers[0]
    ''' Converts a layer to gray scale, without modifying his type (RGB or RGBA).
    Note that this implementation is very inefficient, since it do not make use 
    of tiles or pixel regions. Also, it has a bug which prevents to undo the 
    changes made by this function.
    
    Parameters:
    img : image The current image.
    layer : layer The layer of the image that is selected.
    '''
    # Indicates that the process has started.
    gimp.progress_init("Discolouring " + layer.name + "...")

    # Set up an undo group, so the operation will be undone in one step.
    pdb.gimp_image_undo_group_start(img)

    # Iterate over all the pixels and convert them to gray.
    try:
        for x in range(layer.width):
            # Update the progress bar.
            gimp.progress_update(float(x) / float(layer.width))

            for y in range(layer.height):
                # Get the pixel and verify that is an RGB value.
                pixel = layer.get_pixel(x,y)
            
                if(len(pixel) >= 3):
                    # convert gimp's int RGB to floats
                    r = float(pixel[0]) / 255.0
                    g = float(pixel[1]) / 255.0
                    b = float(pixel[2]) / 255.0
                    # convert to HLS
                    (h, l, s) = colorsys.rgb_to_hls(r, g, b)
                    # process non-dark (almost non-black) pixels only
                    if l > 0.2:
                        # map hue from 0..1 to 0..1.4 and clamp it
                        l = min(h*1.4, 1)
                        # saturation is 0 because we're doing grayscale
                        s = 0
                        # hue doesn't matter, but set it to 0
                        h = 0
                        # convert back to RGB
                        # ...but the colorsys.hls_to_rgb function seems to accept 
                        # parameters in a different order than it should; (at least for HSV it did)
                        # not that it matters since everything but Luminosity is 0
                        (r, g, b) = colorsys.hls_to_rgb(s, l, h)
                        # make array and append any existing ALPHA channel
                        newColor = (int(r * 255), int(g * 255), int(b * 255)) + pixel[3:]
                        # set the new pixel
                        layer.set_pixel(x,y, newColor)
        
        # Update the layer.
        layer.update(0, 0, layer.width, layer.height)

    except Exception as err:
        gimp.message("Unexpected error: " + str(err))
    
    # Close the undo group.
    pdb.gimp_image_undo_group_end(img)
    
    # End progress.
    pdb.gimp_progress_end()

register(
    "colormapToGrayscale",
    "colormapToGrayscale",
    "-",
    "JFM",
    "Open source (BSD 3-clause license)",
    "2013",
    "<Image>/Filters/Test/colormapToGrayscale",
    "RGB, RGB*",
    [],
    [],
    colormapToGrayscale)

main()
