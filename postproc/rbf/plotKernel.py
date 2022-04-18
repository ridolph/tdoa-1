import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

X=np.arange(-5,5,0.2)
Y=np.arange(-5,5,0.2)
X,Y = np.meshgrid(X,Y)
Z=np.exp(-(X**2+Y**2)/2)
ax=Axes3D(plt.figure(figsize=(12,6)))
ax.plot_surface(X,Y,Z,cmap=plt.cm.summer,alpha=1.0)
plt.show()