clc;clear all

tau=0.1/5;
k=83.776/12;

s=tf('s');
MT=k/(tau*s+1)
kpote=0.924