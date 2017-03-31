# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

from PIL import Image, ImageDraw

im = Image.new(mode='RGB',size=(128,128))

draw = ImageDraw.Draw(im)
draw.line((0, 0) + im.size, fill=128)
draw.line((0, im.size[1], im.size[0], 0), fill=128)
#del draw

# write to stdout
#im.save(sys.stdout, "PNG")
#%%
import numpy as np

def collatz_even(n):
    return n*2
    
def collatz_odd(n):
    temp = int((n*2-1)/3)
    if( temp % 2 and not (n*2-1)% 3 ):# and temp>1):
        return temp
    else: 
        return 0

def sumvect(r1,f1,r2,f2):
    r = np.sqrt(r1*r1 + r2*r2 + 2*r1*r2*np.cos(f2 - f1))
    f = f1 + np.arctan2(r2*np.sin(f2 - f1), r1 + r2*np.cos(f2 - f1))
    return r,f


class Tree(object):
    
    def makenlist(self,head):
        head.nlist.append(self.n)#,self.r,self.f])
        if(self.right):
            self.right.makenlist(head)
        if(self.left):
            self.left.makenlist(head)
            
    def makelinelist(self,head):
        x = self.r*np.cos(self.f)
        y = self.r*np.sin(self.f)
        if(self.right):
            head.linelist.append([x,y,self.right.r*np.cos(self.right.f),self.right.r*np.sin(self.right.f)])
            self.right.makelinelist(head)
        if(self.left):
            head.linelist.append([x,y,self.left.r *np.cos(self.left.f), self.left.r *np.sin(self.left.f)])
            self.left.makelinelist(head)
            
        
    def __init__(self, n=2,r=1,f=np.pi/2, stop=10):
        self.n = n
        self.nlist = []
        self.linelist = []
        self.stop = stop
        self.r = r
        self.f = f
        #self.nlist.append(n)

        if(stop > 0):
            a = collatz_even(n)
            ra,fa = sumvect(self.r,self.f,1,self.f-30/180*np.pi)
            self.right = Tree(a,ra,fa,stop-1)
            
            #print("a=",a)
            b = collatz_odd(n)
            rb,fb = sumvect(self.r,self.f,1,self.f+30/180*np.pi)
            if(b>1):# and (b not in self.nlist)):
                self.left = Tree(b,rb,fb,stop-1)
                #print("b=",b)
            else:
                self.left = None
        else:
            self.right = None
            self.left  = None
    
    def __repr__(self):
        return str(self.nlist)
#%%
t = Tree(2,stop=7)
t.makenlist(t)
#print(t.nlist)
t.makelinelist(t)
#%%
coord = np.array(t.linelist)
coord = 100*coord

for j in range(len(coord)):
    coord[j] = [np.int(c) for c in coord[j]]

temp = np.transpose(coord)
#temp = np.array([c-np.min(c)+50 for c in temp],dtype=int) #offset
xoff = np.min([np.min(temp[0]),np.min(temp[2])])
yoff = np.min([np.min(temp[1]),np.min(temp[3])])
temp[0] = temp[0] - xoff
temp[2] = temp[2] - xoff
temp[1] = temp[1] - yoff
temp[3] = temp[3] - yoff
temp = temp + 50 
w = int(np.max([np.max(temp[0]),np.max(temp[2])]))
h = int(np.max([np.max(temp[1]),np.max(temp[3])])) 
coord = np.transpose(temp)
#del temp
#%%
from PIL import Image, ImageDraw
im = Image.new('RGBA', (w+50, h+50), (255, 255, 255, 0)) 
draw = ImageDraw.Draw(im)
for p in coord:
    draw.line(list(p), fill=(255,0,0,0),width=6)
#draw.line((100,200, 150,300), fill=128)
#im.show()
#%%
im.save('collatz.jpg')    