import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.ticker import LinearLocator

envi = np.loadtxt('plotTrack\\env.txt')
anch = np.loadtxt('plotTrack\\anc.txt')
fig = plt.figure(figsize = (15,6))

def scatter3D(stL=0, stD=0, W=1, H=1, rZ=-70, rY=30, LB='anchor pos',
              Lim=envi, X=anch, MK='*', sMK=160, CL='dodgerblue', AP=0.8):
    ax = fig.add_axes([stL, stD, W, H], projection='3d')
    ax.set_xlim(Lim[0,0], Lim[0,1])
    ax.set_ylim(Lim[1,0], Lim[1,1])
    ax.set_zlim(Lim[2,0], Lim[2,1])
    ax.xaxis.set_major_locator(LinearLocator(int((Lim[0,1]-Lim[0,0])/5+1)))
    ax.yaxis.set_major_locator(LinearLocator(int((Lim[1,1]-Lim[1,0])/1+1)))
    ax.zaxis.set_major_locator(LinearLocator(int((Lim[2,1]-Lim[2,0])/1+1)))
    ax.invert_xaxis()
    ax.invert_zaxis()
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.view_init(azim = rZ, elev = rY)
    ax.scatter(X[:,0], X[:,1], X[:,2], marker=MK, s=sMK, color=CL, alpha=AP, label=LB)

scatter3D(stL=0,   W=0.5, stD=0.5, H=0.5, rZ=90, rY=2)
scatter3D(stL=0,   W=0.5, stD=0,   H=0.5, rZ=15, rY=1)
scatter3D(stL=0.5, W=0.5, stD=0.5, H=0.5, rZ=90, rY=95)
scatter3D(stL=0.5, W=0.5, stD=0,   H=0.5, rZ=-72, rY=15)

plt.legend(loc='upper left', fontsize=10)
plt.show()