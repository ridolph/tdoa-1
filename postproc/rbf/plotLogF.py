import numpy as np
import matplotlib.pyplot as plt

log1=np.loadtxt('plotRBF\\logG.txt')
log2=np.loadtxt('plotRBF\\logE.txt')
log3=np.loadtxt('plotRBF\\logL.txt')
log4=np.loadtxt('plotRBF\\logI.txt')

fig=plt.figure(figsize=(12,6))
ax=fig.add_axes([0.06,0.08,0.9,0.9])
ax.set_xlabel('times', fontsize=15)
ax.set_ylabel('RMSE', fontsize=15)
ax.set_xlim(0,50000)
ax.set_ylim(0,1)
ax.grid()
ax.plot(log1[:,0],log1[:,1],linestyle='-',linewidth=4,alpha=0.8,color='deepskyblue',
        label='Gaussian Kernel, at '+str(int(log1[np.size(log1,0)-1,0]+1))+' time achieve RMSE='+str(log1[np.size(log1,0)-1,1]))
ax.plot(log2[:,0],log2[:,1],linestyle='-',linewidth=4,alpha=0.8,color='slategrey',
        label='Exponential Kernel, at '+str(int(log2[np.size(log2,0)-1,0]+1))+' time achieve RMSE='+str(log2[np.size(log2,0)-1,1]))
ax.plot(log3[:,0],log3[:,1],linestyle='-',linewidth=4,alpha=0.8,color='tomato',
        label='Laplacian Kernel, at '+str(int(log3[np.size(log3,0)-1,0]+1))+' time achieve RMSE='+str(log3[np.size(log3,0)-1,1]))
ax.plot(log4[:,0],log4[:,1],linestyle='-',linewidth=4,alpha=0.8,color='seagreen',
        label='Inverse Multiquadric, at '+str(int(log4[np.size(log4,0)-1,0]+1))+' time achieve RMSE='+str(log4[np.size(log4,0)-1,1]))

ax.hlines(0.15,0,50000,linestyle=':',linewidth=2,alpha=0.8)
ax.text(500,0.16,'RMSE=0.15',fontsize=13)
plt.legend(loc='upper right',fontsize=15)
plt.show()