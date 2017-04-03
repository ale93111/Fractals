# -*- coding: utf-8 -*-
"""
Created on Sat Feb 18 21:28:37 2017
@author: alessandro
"""

import imageio

path = "/home/alessandro/Documenti/Fractals/collatz/gif3d/"
pathout = "/home/alessandro/Documenti/Fractals/collatz/"
#%%
n = 71
filenames = [path+"collatz"+str(i)+".jpg" for i in range(n)]
end = [path+"collatz"+str(n-1)+".jpg" for i in range(6)]
filenames = filenames + end
#%%

with imageio.get_writer(pathout+'collatz3d.gif', mode='I',duration=0.15) as writer:
    for filename in filenames:
        image = imageio.imread(filename)
        writer.append_data(image)
