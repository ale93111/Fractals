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
            head.linelist.append([self.x,self.y,self.right.x,self.right.y,self.stop,self.right.n]+list(self.right.color))
            self.right.makelinelist(head)
        if(self.left):
            head.linelist.append([self.x,self.y,self.left.x ,self.left.y ,self.stop,self.left.n ]+list(self.left.color))
            self.left.makelinelist(head)
            
        
    def __init__(self, n=2,x=0,y=0,f=15/180*np.pi, color=(255,0,0,0),stop=10):
        self.n = n
        self.nlist = []
        self.linelist = []
        self.stop = stop
        self.x = x
        self.y = y
        self.f = f
        self.color = color
        #self.nlist.append(n)

        if(stop > 0):
            a = collatz_even(n)
            fa = self.f + 15/180*np.pi
            xa = self.x + np.cos(fa)
            ya = self.y + np.sin(fa)
            self.right = Tree(a,xa,ya,fa,(255,0,0,0),stop-1)
            
            #print("a=",a)
            b = collatz_odd(n)
            fb = self.f - 15/180*np.pi
            xb = self.x + np.cos(fb)
            yb = self.y + np.sin(fb)
            if(b>1):# and (b not in self.nlist)):
                self.left = Tree(b,xb,yb,fb,(0,0,255,0),stop-1)
                #print("b=",b)
            else:
                self.left = None
        else:
            self.right = None
            self.left  = None
    
    def __repr__(self):
        return str(self.nlist)
#%%
t = Tree(2,stop=35)
t.makenlist(t)
#print(t.nlist)
t.makelinelist(t)
print("\n len=",len(t.nlist),
      "\n max=",np.max(t.nlist))
#%%
coord = np.array(t.linelist)
for j in range(len(coord)):
    coord[j][:4] = 100*coord[j][:4]

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
temp[:4] = temp[:4] + 50 
#for j in range(len(temp)-2):
#    temp[j] = temp[j] + 50 
w = int(np.max([np.max(temp[0]),np.max(temp[2])]) +50)
h = int(np.max([np.max(temp[1]),np.max(temp[3])]) +50) 
target = 1000
ratio = target/h
h_new = int(h*ratio)
w_new = int(w*ratio)
temp[:4] = ratio*temp[:4]

coord = np.transpose(temp)
coord = np.array(coord,dtype=int)

#del temp
#%%
from operator import itemgetter
coord2 = sorted(coord, key=itemgetter(4), reverse=True)
#%%
from PIL import Image, ImageDraw
im = Image.new('RGBA', (w_new, h_new), (0,0,0,0)) 
draw = ImageDraw.Draw(im)

count = 0
for j in range(coord2[0][4]):
    start = coord2[0][4]-j
    while True:
        #if coord2[count][5] < np.average(t.nlist):
        draw.line(list(coord2[count][:4]), fill=tuple(coord2[count][-4:]),width=3)
        count += 1
        if count==len(coord2):
            break
        new = coord2[count][4]
        if start != new:
            break
    #im = im.resize((int(w/2),int(h/2)))
    im.rotate(180).save("/home/alessandro/Documents/Fractals/collatz/gif/collatz"+str(j)+".jpg")    
#%%
from PIL import Image, ImageDraw
im = Image.new('RGBA', (w+50, h+50), (0,0,0,0)) 
draw = ImageDraw.Draw(im)
for p in coord2:
    draw.line(list(p[:4]), fill=tuple(p[-4:]),width=6)
#draw.rectangle([coord[0][0]-15,coord[1][0]-15,coord[0][0]+15,coord[1][0]+15],fill=(255,255,255,0))
#im.show()
#%%
im.rotate(180).save('/home/alessandro/Documents/Fractals/collatz/collatz.jpg')    
#%%
from operator import itemgetter
prova = sorted(coord, key=itemgetter(4), reverse=True)

#%%
prova = [ [t.nlist[j+1]] + list(coord[j]) for j in range(len(coord))]
#%%
pairs = list(zip(t.nlist[::2], t.nlist[1::2]))
