import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.ticker import LinearLocator

pos1=np.loadtxt('plotChan\\3chanRandom10-0.txt')
pos2=np.loadtxt('plotChan\\3chanSine10-0.txt')
pos3=np.loadtxt('plotChan\\3chanHelix10-0.txt')
pos4=np.loadtxt('plotChan\\3chanRandom20-5.txt')
pos5=np.loadtxt('plotChan\\3chanSine20-5.txt')
pos6=np.loadtxt('plotChan\\3chanHelix20-5.txt')

envi=np.loadtxt('env.txt')
anch=np.loadtxt('anc.txt')
fig=plt.figure(figsize=(10,8))

def scatter3D(stL=0, stD=0, W=1, H=1, Lim=envi, rZ=-70, rY=30, TITLE='', X1=pos1,
              MK1='^', sMK1=40, AP1=0.8, CL1='seagreen', LB1='chan pos', X2=anch,
              MK2='o', sMK2=40, AP2=0.8, CL2='darkorange', LB2='real pos', sF=12,
              MK3='*', sMK3=80, AP3=0.8, CL3='dodgerblue', LB3='anchor pos'):
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
    ax.scatter(X1[:,1], X1[:,2], X1[:,3], marker = MK1, s = sMK1, color = CL1, alpha = AP1, label = LB1)
    ax.scatter(X1[:,4], X1[:,5], X1[:,6], marker = MK2, s = sMK2, color = CL2, alpha = AP2, label = LB2)
    ax.scatter(X2[:,0], X2[:,1], X2[:,2], marker = MK3, s = sMK3, color = CL3, alpha = AP3, label = LB3)

scatter3D(stL=0,   W=0.5, stD=0.675, H=0.3, X1=pos1, TITLE='Random Positioning with e=0 and s=10cm')
scatter3D(stL=0,   W=0.5, stD=0.35,  H=0.3, X1=pos2, TITLE='2D Trajectory Positioning with e=0 and s=10cm')
scatter3D(stL=0,   W=0.5, stD=0.025, H=0.3, X1=pos3, TITLE='3D Trajectory Positioning with e=0 and s=10cm')
scatter3D(stL=0.5, W=0.5, stD=0.675, H=0.3, X1=pos4, TITLE='Random Positioning with e=5cm and s=20cm')
scatter3D(stL=0.5, W=0.5, stD=0.35,  H=0.3, X1=pos5, TITLE='2D Trajectory Positioning with e=5cm and s=20cm')
scatter3D(stL=0.5, W=0.5, stD=0.025, H=0.3, X1=pos6, TITLE='3D Trajectory Positioning with e=5cm and s=20cm')

plt.legend(loc='lower left', fontsize=12)
plt.show()