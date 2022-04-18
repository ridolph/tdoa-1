import numpy as np
import matplotlib.pyplot as plt

X=np.arange(-3,3,0.1)
Y1=1/(1+np.exp(-X))
Y2=1-2/(1+np.exp(X*2))
Y3=np.zeros(len(X))
for i in range(len(X)):
    if X[i]>0:
        Y3[i]=X[i]
    else:
        Y3[i]=np.exp(X[i])-1
Y4=np.log(1+np.exp(X))


fig=plt.figure(figsize=(10,6))
ax=fig.add_axes([0.05,0.08,0.9,0.9])
ax.grid()
ax.plot(X,Y1,linestyle='-',linewidth=3,alpha=0.8,color='slategrey',label='Sigmoid')
ax.plot(X,Y2,linestyle='-',linewidth=3,alpha=0.8,color='tomato',label='Tanh')
ax.plot(X,Y3,linestyle='-',linewidth=3,alpha=0.8,color='seagreen',label='ELU')
ax.plot(X,Y4,linestyle='-',linewidth=3,alpha=0.8,color='deepskyblue',label='Softplus')

plt.legend(loc='upper left',fontsize=15)
plt.show()