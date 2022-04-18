import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.ticker import LinearLocator

envi = np.loadtxt('plotTrack\\env.txt')
pos1 = np.loadtxt('plotTrack\\1track.txt')
pos2 = np.loadtxt('plotTrack\\2real.txt')

pathErr_2 = (pos2-pos1)**2
pathErr_2 = pathErr_2.sum(axis=1)
print(np.sqrt(np.mean(pathErr_2)))
print(np.mean(np.sqrt(pathErr_2)))

fig=plt.figure(figsize=(14,5))
ax=fig.add_axes([0,0.05,1,0.9],projection='3d')
ax.set_xlim(envi[0,0],envi[0,1])
ax.set_ylim(envi[1,0],envi[1,1])
ax.set_zlim(envi[2,0],envi[2,1])
ax.xaxis.set_major_locator(LinearLocator(int((envi[0,1]-envi[0,0])/3+1)))
ax.yaxis.set_major_locator(LinearLocator(int(envi[1,1]-envi[1,0]+1)))
ax.zaxis.set_major_locator(LinearLocator(int(envi[2,1]-envi[2,0]+1)))
ax.invert_xaxis()
ax.invert_zaxis()

ax.scatter(pos1[:,0],pos1[:,1],pos1[:,2],s=30,marker='>',color='cyan',label='designed path',alpha=1)
ax.scatter(pos2[:,0],pos2[:,1],pos2[:,2],s=30,marker='o',color='darkorange',label='real path',alpha=0.6)

for i in range(len(pos2)):
    ax.plot([pos2[i,0],pos1[i,0]],[pos2[i,1],pos1[i,1]],[pos2[i,2],pos1[i,2]],
        color='slategray',linestyle='-',linewidth=2)

plt.legend(loc='best',fontsize=12)
plt.show()