# -*- coding: utf-8 -*-
"""
Created on Wed Mar 17 22:43:18 2021
@author: Lenovo
输入数据分析
"""
import numpy as np
import matplotlib.pyplot as plt
'''
读输入数据，保存服务器信息、虚拟机信息以及请求序列
'''
def readinput():
    with open('training-1.txt') as f:        # file input
        data = f.read()
    f.closed
    data = data.splitlines(keepends=False)
    length1 = int(data[0])
    length2 = int(data[length1+1])
    server = data[1:length1+1]
    vm = data[length1+2:length1+length2+2]
    request = data[length1+length2+2:int(len(data))]
    for i in range(len(server)):
        server[i] = str.split(server[i][1:len(server[i])-1], ', ')
    for i in range(len(vm)):
        vm[i] = str.split(vm[i][1:len(vm[i])-1], ', ')
    server, vm = np.array(server), np.array(vm)
    vm_dict = dict(zip(vm[:,0],vm[:,1:4]))
    return server, vm, request, vm_dict
'''
服务器按照CPU/内存进行排序并绘图
'''
def server_plot(server):
    server = np.append(server, np.zeros((server.shape[0],1)), axis = 1)
    for i in range(server.shape[0]):
        server[i,5] = int(server[i,1])*1.0/int(server[i,2])
    server = server[server[:,5].argsort()]
    server_cpu = server[:,1]
    server_ram = server[:,2]
    server_num = server_cpu.shape[0]
    server_name = np.arange(server_num)
    y1 = np.zeros((server_num,1))
    y2 = np.zeros((server_num,1))
    for i in range(server_num):
        y1[i] = int(server_cpu[i])
        y2[i] = int(server_ram[i])
    fig = plt.figure()
    plt.bar(server_name,y1.flatten(),0.5,color="green")
    plt.bar(server_name, -1*y2.flatten(), 0.5, color = "red")
    plt.legend(["cpu","ram"])
'''
虚拟机按CPU/内存进行排序并绘图
'''    
def vm_plot(vm):
    vm = np.append(vm, np.zeros((vm.shape[0],1)), axis=1)
    for i in range(vm.shape[0]):
        vm[i,4] = int(vm[i,1])*1.0/int(vm[i,2])
    vm = vm[vm[:,4].argsort()]
    vm_cpu = vm[:,1]
    vm_ram = vm[:,2]
    vm_num = vm_cpu.shape[0]
    vm_name = np.arange(vm_num)
    y1 = np.zeros((vm_num,1))
    y2 = np.zeros((vm_num,1))
    for i in range(vm_num):
        y1[i] = int(vm_cpu[i])
        y2[i] = int(vm_ram[i])
    fig = plt.figure()
    plt.bar(vm_name,y1.flatten(),0.5,color="green")
    plt.bar(vm_name, -1*y2.flatten(), 0.5, color = "red")
    plt.legend(["cpu","ram"])
def request_plot(request, vm_dict):
    days = int(request[0])
    pos = 1
    cpu_ram = np.zeros((days,2))
    for i in range(days):
        request_num = int(request[pos])
        request_list = request[pos+1:pos+request_num+1]
        for j in range(len(request_list)):
            request_list[j] = str.split(request_list[j][1:len(request_list[j])-1], ', ')
            if len(request_list[j])==2:
                request_list[j].extend('1')
        request_list = np.array(request_list)
        
        casor = np.where(request_list[:,0]=='add')
        add_list = request_list[casor]
        day_cpu_ram = np.zeros((add_list.shape[0],2))
        cpu = 0
        ram = 0
        for j in range(add_list.shape[0]):     
            cpu += int(vm_dict[add_list[j,1]][0])
            ram += int(vm_dict[add_list[j,1]][1])
            day_cpu_ram[j,0] = int(vm_dict[add_list[j,1]][0])
            day_cpu_ram[j,1] =int(vm_dict[add_list[j,1]][1])
        if i==799:
            day_cpu_ram = np.append(day_cpu_ram,np.zeros((add_list.shape[0],1)),axis=1)
            for k in range(add_list.shape[0]):
                day_cpu_ram[k,2] = day_cpu_ram[k,0]*1.0/day_cpu_ram[k,1]
            day_cpu_ram = day_cpu_ram[day_cpu_ram[:,2].argsort()]
            name = np.arange(add_list.shape[0])
            fig = plt.figure()
            plt.bar(name,day_cpu_ram[:,0].flatten(),0.5,color="green")
            plt.bar(name,-1*day_cpu_ram[:,1].flatten(), 0.5, color = "red")
            plt.legend(["cpu","ram"])
        cpu_ram[i,0] = cpu
        cpu_ram[i,1] = ram
        pos = pos + request_num + 1
    cpu_ram = np.append(cpu_ram,np.zeros((days,1)),axis=1)
    for i in range(days):
        cpu_ram[i,2] = cpu_ram[i,0]*1.0/cpu_ram[i,1]
    name = np.arange(days)
    fig = plt.figure()
    plt.bar(name,cpu_ram[:,0].flatten(),0.5,color="green")
    plt.bar(name,-1*cpu_ram[:,1].flatten(), 0.5, color = "red")
    plt.legend(["cpu","ram"])