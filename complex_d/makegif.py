# -*- coding: utf-8 -*-
"""
Created on Sat Feb 18 21:28:37 2017

@author: alessandro
"""

import imageio

path = "/home/alessandro/Documenti/Fractals/complex_d/imgs/"
pathout = "/home/alessandro/Documenti/Fractals/complex_d/"

#%%
filenames = [path+"Complex"+str(i)+".png" for i in range(1,201)]
filenames = filenames + filenames[::-1]
#%%

with imageio.get_writer(pathout+'complex1.gif', mode='I',duration=0.02) as writer:
    for filename in filenames:
        image = imageio.imread(filename)
        writer.append_data(image[::-1])
#%%
import moviepy.editor as mp
clip = mp.VideoFileClip(pathout+"complex1.gif",audio=False)
clip.write_videofile(pathout+"complex1.webm",audio=False)