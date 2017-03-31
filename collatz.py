# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

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
        if(self.right):
            head.linelist.append([self.x,self.y,self.right.x,self.right.y])
            self.right.makelinelist(head)
        if(self.left):
            head.linelist.append([self.x,self.y,self.left.x ,self.left.y ])
            self.left.makelinelist(head)
            
        
    def __init__(self, n=2,x=0,y=0,f=15/180*np.pi, stop=10):
        self.n = n
        self.nlist = []
        self.linelist = []
        self.stop = stop
        self.x = x
        self.y = y
        self.f = f
        #self.nlist.append(n)

        if(stop > 0):
            a = collatz_even(n)
            fa = self.f + 15/180*np.pi
            xa = self.x + np.cos(fa)
            ya = self.y + np.sin(fa)
            self.right = Tree(a,xa,ya,fa,stop-1)
            
            #print("a=",a)
            b = collatz_odd(n)
            fb = self.f - 15/180*np.pi
            xb = self.x + np.cos(fb)
            yb = self.y + np.sin(fb)
            if(b>1):# and (b not in self.nlist)):
                self.left = Tree(b,xb,yb,fb,stop-1)
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
print("\n len=",len(t.nlist),
      "\n max=",np.max(t.nlist))
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
draw.rectangle([coord[0][0]-15,coord[1][0]-15,coord[0][0]+15,coord[1][0]+15],fill=(255,255,255,0))

#draw.line((100,200, 150,300), fill=128)
#im.show()
#%%
im.rotate(180).save('collatz.jpg')    
#%%
prova = [ [t.nlist[j+1]] + list(coord[j]) for j in range(len(coord))]