close all;
clear;
clc;
mat1=load('4trainResult.txt');
RMSE1=sqrt(mean(mat1(:,1).^2))
MERR1=mean(sqrt(mat1(:,1).^2))
mat2=load('6testResult.txt');
RMSE2=sqrt(mean(mat2(:,1).^2))
MERR2=mean(sqrt(mat2(:,1).^2))
%{
figure(1);
plot3(mat1(:,5),mat1(:,6),mat1(:,7),'rx');
grid on;
hold on;
plot3(mat1(:,2),mat1(:,3),mat1(:,4),'bo');
for i=1:size(mat1,1)
    line([mat1(i,5) mat1(i,2)],[mat1(i,6) mat1(i,3)],[mat1(i,7) mat1(i,4)]);
end
%}
figure(2);
plot3(mat2(:,5),mat2(:,6),mat2(:,7),'rx');
grid on;
hold on;
plot3(mat2(:,2),mat2(:,3),mat2(:,4),'bo');

for i=1:size(mat2,1)
    line([mat2(i,5) mat2(i,2)],[mat2(i,6) mat2(i,3)],[mat2(i,7) mat2(i,4)]);
end
