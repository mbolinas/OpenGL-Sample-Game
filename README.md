Marc Bolinas
CISC440

I started by creating my object in blender, in particular my hydroflask water bottle. I followed one of the tutorials linked in the homework description that had me create an apple from scratch. I started texturing with a blank canvas, which was relatively easy because my water bottle consists of solid colors. I exported both the .obj and .jpg from blender, and used an online converter to convert my texture.jpg to a .DDS to make it usable in OpenGL.

My Catmull-Rom spline was implemented using two nested for loops, where the first for loop iterated over each keyframe, and the inner for loop iterated on the amount of steps for the interpolation. The interpolation was implemented as an equation rather than a matrix, as it was easier to do so. I used http://web.eecs.utk.edu/~smarz1/splines.html to double check my equation derivation, which should be correct.

My play-pause is a standard key event handler.