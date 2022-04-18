import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.ticker import LinearLocator

envi=np.loadtxt('env.txt')
anch=np.loadtxt('anc.txt')
pos3=np.loadtxt('plotPSO\\3psoSine10-0.txt')
pos4=np.loadtxt('plotPSO\\3psoSine20-5.txt')
pos5=np.loadtxt('plotPSO\\3psoHelix10-0.txt')
pos6=np.loadtxt('plotPSO\\3psoHelix20-5.txt')

fig=plt.figure(figsize=(10,5.5))
ax3=fig.add_axes([0,0.515,0.5,0.455],projection='3d')
ax3.set_title('2D Trajectory Positioning with e=0 and s=10cm', fontsize=12)
ax3.set_xlim(envi[0,0],envi[0,1])
ax3.set_ylim(envi[1,0],envi[1,1])
ax3.set_zlim(envi[2,0],envi[2,1])
ax3.xaxis.set_major_locator(LinearLocator(int(envi[0,1]-envi[0,0]+1)))
ax3.yaxis.set_major_locator(LinearLocator(int(envi[1,1]-envi[1,0]+1)))
ax3.zaxis.set_major_locator(LinearLocator(int(envi[2,1]-envi[2,0]+1)))
ax3.invert_xaxis()
ax3.invert_zaxis()
ax4=fig.add_axes([0.5,0.515,0.5,0.455],projection='3d')
ax4.set_title('2D Trajectory Positioning with e=5cm and s=20cm', fontsize=12)
ax4.set_xlim(envi[0,0],envi[0,1])
ax4.set_ylim(envi[1,0],envi[1,1])
ax4.set_zlim(envi[2,0],envi[2,1])
ax4.xaxis.set_major_locator(LinearLocator(int(envi[0,1]-envi[0,0]+1)))
ax4.yaxis.set_major_locator(LinearLocator(int(envi[1,1]-envi[1,0]+1)))
ax4.zaxis.set_major_locator(LinearLocator(int(envi[2,1]-envi[2,0]+1)))
ax4.invert_xaxis()
ax4.invert_zaxis()
ax5=fig.add_axes([0,0.03,0.5,0.455],projection='3d')
ax5.set_title('3D Trajectory Positioning with e=0 and s=10cm', fontsize=12)
ax5.set_xlim(envi[0,0],envi[0,1])
ax5.set_ylim(envi[1,0],envi[1,1])
ax5.set_zlim(envi[2,0],envi[2,1])
ax5.xaxis.set_major_locator(LinearLocator(int(envi[0,1]-envi[0,0]+1)))
ax5.yaxis.set_major_locator(LinearLocator(int(envi[1,1]-envi[1,0]+1)))
ax5.zaxis.set_major_locator(LinearLocator(int(envi[2,1]-envi[2,0]+1)))
ax5.invert_xaxis()
ax5.invert_zaxis()
ax6=fig.add_axes([0.5,0.03,0.5,0.455],projection='3d')
ax6.set_title('3D Trajectory Positioning with e=5cm and s=20cm', fontsize=12)
ax6.set_xlim(envi[0,0],envi[0,1])
ax6.set_ylim(envi[1,0],envi[1,1])
ax6.set_zlim(envi[2,0],envi[2,1])
ax6.xaxis.set_major_locator(LinearLocator(int(envi[0,1]-envi[0,0]+1)))
ax6.yaxis.set_major_locator(LinearLocator(int(envi[1,1]-envi[1,0]+1)))
ax6.zaxis.set_major_locator(LinearLocator(int(envi[2,1]-envi[2,0]+1)))
ax6.invert_xaxis()
ax6.invert_zaxis()

ax3.scatter(pos3[:,4],pos3[:,5],pos3[:,6],
            s=40,marker='o',color='darkorange',label='real position',alpha=0.8)
ax3.scatter(pos3[:,1],pos3[:,2],pos3[:,3],
            s=40,marker='^',color='seagreen',label='pso position',alpha=0.8)   
for i in range(len(pos3)):
    ax3.plot([pos3[i,1],pos3[i,4]],[pos3[i,2],pos3[i,5]],[pos3[i,3],pos3[i,6]],
            color='slategray',linestyle='-',linewidth=2)
ax3.scatter(anch[:,0],anch[:,1],anch[:,2],
            s=160,marker='*',color='dodgerblue',label='anchor position',alpha=0.8)

ax4.scatter(pos4[:,4],pos4[:,5],pos4[:,6],
            s=40,marker='o',color='darkorange',label='real position',alpha=0.8)
ax4.scatter(pos4[:,1],pos4[:,2],pos4[:,3],
            s=40,marker='^',color='seagreen',label='pso position',alpha=0.8)   
for i in range(len(pos4)):
    ax4.plot([pos4[i,1],pos4[i,4]],[pos4[i,2],pos4[i,5]],[pos4[i,3],pos4[i,6]],
            color='slategray',linestyle='-',linewidth=2)
ax4.scatter(anch[:,0],anch[:,1],anch[:,2],
            s=160,marker='*',color='dodgerblue',label='anchor position',alpha=0.8)

ax5.scatter(pos5[:,4],pos5[:,5],pos5[:,6],
            s=40,marker='o',color='darkorange',label='real position',alpha=0.8)
ax5.scatter(pos5[:,1],pos5[:,2],pos5[:,3],
            s=40,marker='^',color='seagreen',label='pso position',alpha=0.8)   
for i in range(len(pos5)):
    ax5.plot([pos5[i,1],pos5[i,4]],[pos5[i,2],pos5[i,5]],[pos5[i,3],pos5[i,6]],
            color='slategray',linestyle='-',linewidth=2)
ax5.scatter(anch[:,0],anch[:,1],anch[:,2],
            s=160,marker='*',color='dodgerblue',label='anchor position',alpha=0.8)

ax6.scatter(pos6[:,4],pos6[:,5],pos6[:,6],
            s=40,marker='o',color='darkorange',label='real position',alpha=0.8)
ax6.scatter(pos6[:,1],pos6[:,2],pos6[:,3],
            s=40,marker='^',color='seagreen',label='pso position',alpha=0.8)   
for i in range(len(pos6)):
    ax6.plot([pos6[i,1],pos6[i,4]],[pos6[i,2],pos6[i,5]],[pos6[i,3],pos6[i,6]],
            color='slategray',linestyle='-',linewidth=2)
ax6.scatter(anch[:,0],anch[:,1],anch[:,2],
            s=160,marker='*',color='dodgerblue',label='anchor position',alpha=0.8)

plt.legend(loc='lower left', fontsize=10)
plt.show()