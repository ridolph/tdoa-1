import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.ticker import LinearLocator

pos11=np.loadtxt('plotTrack\\1trackLine10-0.txt')
pos21=np.loadtxt('plotTrack\\1trackSine10-0.txt')
pos31=np.loadtxt('plotTrack\\1trackHelix10-0.txt')
pos12=np.loadtxt('plotTrack\\2realLine10-0.txt')
pos22=np.loadtxt('plotTrack\\2realSine10-0.txt')
pos32=np.loadtxt('plotTrack\\2realHelix10-0.txt')

pos41=np.loadtxt('plotTrack\\1trackLine20-5.txt')
pos51=np.loadtxt('plotTrack\\1trackSine20-5.txt')
pos61=np.loadtxt('plotTrack\\1trackHelix20-5.txt')
pos42=np.loadtxt('plotTrack\\2realLine20-5.txt')
pos52=np.loadtxt('plotTrack\\2realSine20-5.txt')
pos62=np.loadtxt('plotTrack\\2realHelix20-5.txt')

envi=np.loadtxt('plotTrack\\env.txt')
fig=plt.figure(figsize=(10,8))

def scatter3D(stL=0, stD=0, W=1, H=1, Lim=envi, rZ=-70, rY=30, TITLE='Line Track with e=0 and s=10cm', 
              X1=pos11, MK1='>', sMK1=30, CL1='cyan', AP1=1.0, LB1='designed path', sF=12,
              X2=pos12, MK2='o', sMK2=20, CL2='darkorange', AP2=0.8, LB2='real path'):
    ax = fig.add_axes([stL, stD, W, H], projection='3d')
    ax.set_title(TITLE, fontsize = sF)
    ax.set_xlim(Lim[0,0], Lim[0,1])
    ax.set_ylim(Lim[1,0], Lim[1,1])
    ax.set_zlim(Lim[2,0], Lim[2,1])
    ax.xaxis.set_major_locator(LinearLocator(int((Lim[0,1]-Lim[0,0])/3+1)))
    ax.yaxis.set_major_locator(LinearLocator(int((Lim[1,1]-Lim[1,0])/1+1)))
    ax.zaxis.set_major_locator(LinearLocator(int((Lim[2,1]-Lim[2,0])/1+1)))
    ax.invert_xaxis()
    ax.invert_zaxis()
    ax.view_init(azim = rZ, elev = rY)
    ax.scatter(X1[:,0], X1[:,1], X1[:,2], marker = MK1, s = sMK1, color = CL1, alpha = AP1, label = LB1)
    ax.scatter(X2[:,0], X2[:,1], X2[:,2], marker = MK2, s = sMK2, color = CL2, alpha = AP2, label = LB2)

scatter3D(stL=0,   W=0.5, stD=0.675, H=0.3, TITLE='Line Track with e=0 and s=10cm',    X1=pos11, X2=pos12)
scatter3D(stL=0,   W=0.5, stD=0.35,  H=0.3, TITLE='Sine Track with e=0 and s=10cm',    X1=pos21, X2=pos22)
scatter3D(stL=0,   W=0.5, stD=0.025, H=0.3, TITLE='Helix Track with e=0 and s=10cm',   X1=pos31, X2=pos32)
scatter3D(stL=0.5, W=0.5, stD=0.675, H=0.3, TITLE='Line Track with e=5cm and s=20cm',  X1=pos41, X2=pos42)
scatter3D(stL=0.5, W=0.5, stD=0.35,  H=0.3, TITLE='Sine Track with e=5cm and s=20cm',  X1=pos51, X2=pos52)
scatter3D(stL=0.5, W=0.5, stD=0.025, H=0.3, TITLE='Helix Track with e=5cm and s=20cm', X1=pos61, X2=pos62)

plt.legend(loc='lower left', fontsize=10)
plt.show()