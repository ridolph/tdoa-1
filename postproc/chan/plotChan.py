import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

envi=np.loadtxt('env.txt')
anch=np.loadtxt('anc.txt')
pos2=np.loadtxt('plotChan\\3chan.txt')

print(len(pos2))
print(np.sqrt(np.mean(pos2[:,0]**2)))
print(np.mean(pos2[:,0]))

ax=Axes3D(plt.figure())
ax.set_xlim(envi[0,0],envi[0,1])
ax.set_ylim(envi[1,0],envi[1,1])
ax.set_zlim(envi[2,0],envi[2,1])
ax.invert_xaxis()
ax.invert_zaxis()
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

ax.scatter(anch[:,0],anch[:,1],anch[:,2],s=160,marker='*',color='dodgerblue',alpha=0.8)
for i in range(len(anch)):
  ax.text(anch[i,0],anch[i,1],anch[i,2]-0.05,'anchor'+str(i))

ax.scatter(pos2[:,1],pos2[:,2],pos2[:,3],s=40,marker='^',color='seagreen',label='predicted position')
ax.scatter(pos2[:,4],pos2[:,5],pos2[:,6],s=40,marker='o',color='darkorange',label='real position')
for i in range(len(pos2)):
  ax.plot([pos2[i,1],pos2[i,4]],[pos2[i,2],pos2[i,5]],[pos2[i,3],pos2[i,6]],
          color='slategray',linestyle='-',linewidth=2)
plt.legend(loc='best')
plt.show()