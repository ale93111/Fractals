# -*- coding: utf-8 -*-
"""
Created on Sat Feb 18 21:28:37 2017

@author: alessandro
"""

import imageio

path = "/home/alessandro/Documenti/Fractals/dragons/"

#%%
start = [path+"animationheightogolden+2PI/"+str(0)+".png" for i in range(10)]
filenames = [path+"animationheightogolden+2PI/"+str(i)+".png" for i in range(201)]
end   = [path+"animationheightogolden+2PI/"+str(200)+".png" for i in range(10)]
filenames = start + filenames + end
#%%

with imageio.get_writer(path+'h_to_g+2PI.gif', mode='I',duration=0.05) as writer:
    for filename in filenames:
        image = imageio.imread(filename)
        writer.append_data(image[::-1])
