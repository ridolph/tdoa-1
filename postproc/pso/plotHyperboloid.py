import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

L=np.arange(0,6.0,0.2)
D=np.arange(0,np.pi*2.01,0.1)
X1=np.zeros((len(L),len(D)))
X2=np.zeros((len(L),len(D)))
Y=np.zeros((len(L),len(D)))
Z=np.zeros((len(L),len(D)))
for i in range(len(L)):
    X1[i,:]= 10/2*np.sqrt(1+L[i]**2*4/(12**2-10**2))+6
    X2[i,:]=-10/2*np.sqrt(1+L[i]**2*4/(12**2-10**2))+6
    for j in range(len(D)):
        Y[i,j]=L[i]*np.cos(D[j])
        Z[i,j]=L[i]*np.sin(D[j])

ax=Axes3D(plt.figure(figsize=(11,7)))
ax.invert_xaxis()
ax.invert_zaxis()

ax.plot_surface(X1,Y,Z,cmap=plt.cm.summer,alpha=0.8)
ax.plot_surface(X2,Y,Z,cmap=plt.cm.winter,alpha=0.8)

ax.scatter( 0,0,0,s=120,marker='*',color='orange')
ax.scatter(12,0,0,s=120,marker='*',color='orange')
ax.text( 0,0,-0.05,'anchor0')
ax.text(12,0,-0.05,'anchor1')

plt.show()