close all;
clear;
clc;

mat=load('3pso.txt');
ITER=mean(mat(:,1))  %平均迭代次数
figure(1);
plot3(mat(:,5),mat(:,6),mat(:,7),'rx');
grid on;
hold on;
plot3(mat(:,2),mat(:,3),mat(:,4),'bo');

dx=mat(:,2)-mat(:,5);
dy=mat(:,3)-mat(:,6);
dz=mat(:,4)-mat(:,7);

RMSE=sqrt(mean(dx.^2+dy.^2+dz.^2))

sum=size(mat,1)
cnt=0;
for i=1:sum
    if(mat(i,1)==200)
        cnt=cnt+1;
    end
end
cnt