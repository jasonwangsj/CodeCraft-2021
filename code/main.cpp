#include <iostream>
#include <fstream>
#include <unordered_map>  
#include<algorithm>  
#include <string>  
#include<vector>
#include<fstream>
#include <iostream>  
#include<bits/stdc++.h>
#include <math.h>
#include <functional>
#include <iterator>
using namespace std;
// 服务器信息结构体 
struct Host{
	string HostName;   // 服务器名 
 	int cpu;           // 服务器CPU大小 
 	int mem;           // 服务器内存大小 
 	int price;         // 服务器硬件成本 
 	int energy_cost;   // 服务器日能耗 
 	float cpu_mem;     // 服务器性价比 
};
// 虚拟机信息结构体 
struct VM{
 	string vmName;     // 虚拟机名称  
 	int cpu;           // CPU需求 
 	int mem;           // 内存需求 
 	int node_flag;     // 单节点or双节点存储 
};
// 服务器工作状态结构体 
struct HostState{
 	int hostID;        // 服务器ID(购买顺序) 
 	string HostName;
 	int cpu_A;         // A节点剩余cpu 
 	int mem_A;         // A节点剩余内存 
	int cpu_B;         // B节点剩余CPU 
	int mem_B;         // B节点剩余内存 
	int vm_num;        // 服务器上的虚拟机个数 
	int cpumemsum;
	float restratio;
	int energy_cost;   // 服务器日能耗 
	int realID;        // 服务器ID(类别顺序) 
};
// 虚拟机工作状态结构体 
struct VMState{
 	int vmID;       // 虚拟机ID
 	string vmName;     // 虚拟机名称 
	int hostID;     // 服务器ID  
 	int nodeFlag;      // 0：双节点，1：A节点，2：B节点 
 	int change;       // 0代表假id，1代表真id 
};
// 日请求结构体 
struct Request{
	string type;     // 添加or删除 
	int vmID;     // 虚拟机ID 
	string vmName;   // 虚拟机名称 
};
// 服务器购买信息结构体 
struct Host_purchase{
	string HostName; // 购买的服务器名 
	int Hostnum;     // 购买的服务器数量 
};
// 虚拟机部署信息结构体
struct VM_allocate{
	int vmID;
	int hostID;
	int nodeflag;
}; 
// 迁移信息结构体
struct VM_migration{
	string vmName;
	int vmID;
	int hostid_source;
	int hostid_target;
	int node_source;
	int node_target;
}; 
/*
函数功能：服务器信息结构体的排序规则，将服务器按照CPU+MEM升序排列 
input:
	a: 服务器a的结构体
	b: 服务器b的结构体
output:
	服务器结构体的排序规则 
*/
bool compare(Host a, Host b){
	return (a.cpu+a.mem) < (b.cpu+b.mem);
} 
bool compare1(HostState a, HostState b){
	return a.realID < b.realID;
}
bool compare2(HostState a, HostState b){
	return a.hostID < b.hostID;
}
bool compare3(HostState a, HostState b){
	return a.energy_cost < b.energy_cost;
} 
/*
函数功能：字符串分割，将字符串按照指定字符分割为多项
input：
	s: 待分割的字符串 
	delim: 用于字符串分割的字符
output:
	v: string类型的vector，vector的每一项是分割后的一个子串 
*/
vector<string> split(string s, char delim){
    vector<string> v;
    stringstream stringstream1(s);
    string tmp;
    while(getline(stringstream1, tmp, delim)){
        v.push_back(tmp);
    }
    return v;
}
/*
函数功能:读标准输入并同步进行数据处理，功能等于Readinput +  GetHosts + GetVMs + Request_process
input:
	Hosts: 服务器按照性价比排列的结果，vector的每一项是一台服务器的结构体；
	VMs: 虚拟机的map信息，[虚拟机名称：虚拟机(名称，cpu，mem，node_flag)]  
 	
*/
void Input_Process(vector<Host> &Hosts,unordered_map<string, VM> &VMs, vector<vector<Request>> &Requests, vector<VMState> VM_list){	
	// 读服务器信息 
	string str;
	getline(cin, str);
	int hostnum = atoi(str.c_str());
	vector<string> str_split;
	int i,j;
	for(i=0;i<hostnum;i++){
		getline(cin, str);
		str_split = split(str,',');
		Host host;
		host.HostName = str_split[0].substr(1);
		host.cpu = atoi(str_split[1].c_str());
		host.mem = atoi(str_split[2].c_str());
		host.price = atoi(str_split[3].c_str());
		host.energy_cost = atoi(str_split[4].c_str());
		host.cpu_mem = host.cpu*1.0/host.mem;
		Hosts.push_back(host);
	}
	sort(Hosts.begin(), Hosts.end(), compare);
	// 读虚拟机信息
	getline(cin, str);
	int vmnum = atoi(str.c_str());
	for(i=0;i<vmnum;i++){
		getline(cin, str);
		str_split = split(str,',');
		VM vm;
		vm.vmName = str_split[0].substr(1);
		vm.cpu = atoi(str_split[1].c_str());
		vm.mem = atoi(str_split[2].c_str());
		vm.node_flag = atoi(str_split[3].c_str());
		VMs.insert(make_pair(vm.vmName, vm));
	}
	// 读每日请求 
	getline(cin, str);
	int days = atoi(str.c_str());
	for(i=0;i<days;i++){
		vector<Request> Day_request;
		Day_request.clear();
		Request request;
		getline(cin, str);
		int request_num = atoi(str.c_str());
		for(j=0;j<request_num;j++){
			getline(cin, str);
			str_split = split(str,',');
			request.type = str_split[0].substr(1);
			if(request.type.compare("add")){
				request.vmID = atoi(str_split[1].c_str());
				request.vmName = "empty";
			}
			else{
				request.vmName = str_split[1].substr(1);
				request.vmID = atoi(str_split[2].c_str());
			}
			Day_request.push_back(request);
		}
		Requests.push_back(Day_request);
	}
} 
/*
函数功能：迁移函数 
*/
int migration(vector<HostState> &Host_list, unordered_map<string, VM> VMs, vector<VMState> &VM_list, vector<VM_migration> &migration_result){
	// 设置相关参数
	int mignum_max = floor(VM_list.size()*5/1000);
	int mignum = 0;                              // 迁移最大限制次数和已经迁移次数
	float treshhold = 0.5;                       // 选择迁移服务器的阈值 
	int i,j,k;
	migration_result.clear();
	sort(Host_list.begin(),Host_list.end(),compare3); 
	// 首先选出迁移的源服务器和目标服务器
	vector<HostState> Host_source, Host_target, Host_temp;
	for(i=0;i<Host_list.size();i++){
		if(Host_list[i].restratio>=treshhold){
			Host_temp.push_back(Host_list[i]);
		}
	}
	for(i=0;i<floor(Host_temp.size()/2);i++){
		Host_source.push_back(Host_temp[i]);
	}
	for(i=floor(Host_temp.size()/2);i<Host_temp.size();i++){
		Host_target.push_back(Host_temp[i]); 
	}
	sort(Host_list.begin(),Host_list.end(),compare1); 
	// 遍历源服务器中的每台虚拟机
	for(i=0;i<Host_source.size();i++){
		// 源服务器的索引为i，其真实id为realid
		int hostid_source = Host_source[i].realID;
		for(j=0;j<VM_list.size();j++){
			// 第j台虚拟机在真实ID为 hostid_source的服务器上 
			if(VM_list[j].hostID == hostid_source){
				// 考虑该虚拟机是否可以迁移
				// 首先取出虚拟机的基本信息
				string vmName = VM_list[j].vmName;
				int vmID = VM_list[j].vmID; 
				int node_source = VM_list[j].nodeFlag;
				int cpu = VMs[vmName].cpu;
				int mem = VMs[vmName].mem;
				// 开始考虑迁移该虚拟机，依次在目标服务器中找合适位置 
				// 若为双节点部署虚拟机 
				if(node_source==0){
					int score_min = 99999;
					int pos = -1;
					int score;
					for(k=0;k<Host_target.size();k++){
						if(Host_target[k].cpu_A >= cpu/2 && Host_target[k].cpu_B >= cpu/2 && Host_target[k].mem_A >= mem/2 && Host_target[k].mem_B >= mem/2){
							score = Host_target[k].cpu_A + Host_target[k].cpu_B + Host_target[k].mem_A + Host_target[k].mem_B;
							if(score < score_min){
								score_min = score;
								pos = k;
							}
						}
					} 
					// 若存在能存放位置则进行迁移，目标服务器在Host_target的第k行；若没有位置则不执行任何操作 
					if(pos!=-1){
						if(mignum >= mignum_max){
							return 1;
						} 
						// 生成迁移信息
						VM_migration vm_migration;
						vm_migration.hostid_source = hostid_source;
						vm_migration.hostid_target = Host_target[pos].realID;
						vm_migration.node_source = 0;
						vm_migration.node_target = 0;
						vm_migration.vmID = vmID;
						vm_migration.vmName = vmName;
						migration_result.push_back(vm_migration);
						// 修改目标服务器的暂存信息，下一个迁移虚拟机寻找空间时需要使用
						Host_target[pos].cpu_A -= cpu/2;
						Host_target[pos].mem_A -= mem/2;
						Host_target[pos].cpu_B -= cpu/2;
						Host_target[pos].mem_B -= mem/2;
						// 修改真实服务器信息列表里的相关信息
						// 修改源服务器相关信息
						Host_list[hostid_source].cpu_A += cpu/2;
						Host_list[hostid_source].cpu_B += cpu/2;
						Host_list[hostid_source].mem_A += mem/2;
						Host_list[hostid_source].mem_B += mem/2;
						Host_list[hostid_source].vm_num -= 1;
						Host_list[hostid_source].restratio = (Host_list[hostid_source].cpu_A+Host_list[hostid_source].cpu_B+Host_list[hostid_source].mem_A+Host_list[hostid_source].mem_B)*1.0/Host_list[hostid_source].cpumemsum;
						Host_list[Host_target[pos].realID].cpu_A -= cpu/2;
						Host_list[Host_target[pos].realID].cpu_B -= cpu/2;
						Host_list[Host_target[pos].realID].mem_A -= mem/2;
						Host_list[Host_target[pos].realID].mem_B -= mem/2;
						Host_list[Host_target[pos].realID].vm_num += 1;
						Host_list[Host_target[pos].realID].restratio = (Host_list[Host_target[pos].realID].cpu_A+Host_list[Host_target[pos].realID].cpu_B+Host_list[Host_target[pos].realID].mem_A+Host_list[Host_target[pos].realID].mem_B)*1.0/Host_list[Host_target[pos].realID].cpumemsum;
						// 修改虚拟机工作信息
						VM_list[j].hostID = Host_target[pos].realID;
						VM_list[j].nodeFlag = 0;
						// 条件判断
						mignum++;

  					}
				}	
				// 若为单节点部署虚拟机 
				else{
					int score_min = 99999;
					int pos = -1;
					int node_target = -1;
					int score;
					for(k=0;k<Host_target.size();k++){
						if(Host_target[k].cpu_A >= cpu && Host_target[k].mem_A >= mem){
							score = Host_target[k].cpu_A + Host_target[k].mem_A;
							if(score < score_min){
								score_min = score;
								pos = k;
								node_target = 1;
							}
						}
						if(Host_target[k].cpu_B >= cpu && Host_target[k].mem_B >= mem){
							score = Host_target[k].cpu_B + Host_target[k].mem_B;
							if(score < score_min){
								score_min = score;
								pos = k;
								node_target = 2;
							}
						}
					}
					if(pos!=-1){
						if(mignum >= mignum_max){
							return 1;
						}
						// 生成迁移信息
						VM_migration vm_migration;
						vm_migration.hostid_source = hostid_source;
						vm_migration.hostid_target = Host_target[pos].realID;
						vm_migration.node_source = node_source;
						vm_migration.node_target = node_target;
						vm_migration.vmID = vmID;
						vm_migration.vmName = vmName;
						migration_result.push_back(vm_migration);
						// 修改目标服务器的暂存信息
						if(node_target == 1){
							Host_target[pos].cpu_A -= cpu;
							Host_target[pos].mem_A -= mem;
						}
						else{
							Host_target[pos].cpu_B -= cpu;
							Host_target[pos].mem_B -= mem;
						}
						int p;
						p = hostid_source;
						if(node_source==1){
							Host_list[p].cpu_A += cpu;
							Host_list[p].mem_A += mem;
							Host_list[p].vm_num -= 1;
						}
						else{
							Host_list[p].cpu_B += cpu;
							Host_list[p].mem_B += mem;
							Host_list[p].vm_num -= 1;
						}
						Host_list[p].restratio = (Host_list[p].cpu_A + Host_list[p].cpu_B + Host_list[p].mem_A + Host_list[p].mem_B)*1.0 / Host_list[p].cpumemsum;
						p = Host_target[pos].realID;
						if(node_target==1){
							Host_list[p].cpu_A -= cpu;
							Host_list[p].mem_A -= mem;
							Host_list[p].vm_num += 1;
						}
						else{
							Host_list[p].cpu_B -= cpu;
							Host_list[p].mem_B -= mem;
							Host_list[p].vm_num += 1;
						}
						Host_list[p].restratio = (Host_list[p].cpu_A + Host_list[p].cpu_B + Host_list[p].mem_A + Host_list[p].mem_B)*1.0 / Host_list[p].cpumemsum;
						// 修改虚拟机工作信息
						VM_list[j].hostID = Host_target[pos].realID;
						VM_list[j].nodeFlag = node_target;
						// 条件判断 
						mignum++;
						
					}
				}	
			}
		}
	} 
}
/*
函数功能：对一天的请求序列，执行购买和部署操作，输出相应结果 
*/
void Main_process(vector<Host> Hosts, vector<Request> Requests, vector<HostState> &Host_list, unordered_map<string, VM> VMs, vector<VMState> &VM_list, vector<Host_purchase> &purchase_result){
	vector<VM_migration> migration_result;
	
	migration(Host_list, VMs, VM_list, migration_result);
	int	id = Host_list.size();
	Host_purchase host_purchase;
	purchase_result.clear();
	vector<VM_allocate> allocate_result;
	VM_allocate vmallocate;
	HostState hoststate;
	VMState vmstate;
	VM thevm;
	int i,j,k;
	// 逐个处理当天的请求，若添加操作无空间存放，则购买服务器
	for(i=0;i<Requests.size();i++){
		// 执行删除操作 
		if(Requests[i].type.compare("add")){
			string vmName;
			int vmID = Requests[i].vmID;
			int cpu;
			int mem;
			// 更新服务器工作信息
			for(k=0;k<VM_list.size();k++){
				// 若列表中第K台虚拟机的ID与当前任务的ID匹配 
				if(VM_list[k].vmID==vmID){
					vmName = VM_list[k].vmName;
					break; 
				} 
			} 
			if(k==VM_list.size()){
				cout << Requests[i].vmID << endl;
				cout << "Here Here Here Here Here Here Here Here Here Here Here Here!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			}
			else{
				VM thevm = VMs[vmName];          
				cpu = thevm.cpu;
				mem = thevm.mem; 
			}
			int hostid = VM_list[k].hostID;
			if(VM_list[k].nodeFlag==0){
				Host_list[hostid].cpu_A += cpu/2;
				Host_list[hostid].mem_A += mem/2;
				Host_list[hostid].cpu_B += cpu/2;
				Host_list[hostid].mem_B += mem/2;
				Host_list[hostid].vm_num -= 1;
			}
			else{
				if(VM_list[k].nodeFlag==1){
					Host_list[hostid].cpu_A += cpu;
					Host_list[hostid].mem_A += mem;
					Host_list[hostid].vm_num -= 1;
				}
				else{
					Host_list[hostid].cpu_B += cpu;
					Host_list[hostid].mem_B += mem;
					Host_list[hostid].vm_num -= 1;
				}
			}
			Host_list[hostid].restratio = (Host_list[hostid].cpu_A + Host_list[hostid].cpu_B + Host_list[hostid].mem_A + Host_list[hostid].mem_B)*1.0/Host_list[hostid].cpumemsum;
			VM_list.erase(VM_list.begin()+k); 
		}
		else{
			// 先取出虚拟机的全部信息 
			string vmName = Requests[i].vmName;
			int vmID = Requests[i].vmID;
			VM thevm = VMs[vmName];          
			int cpu = thevm.cpu;
			int mem = thevm.mem; 
			int node_flag = VMs[Requests[i].vmName].node_flag;
			//cout << node_flag << endl;
			// 若是双节点部署 
			if(node_flag==1){
				//cout << "here2" <<endl; 
				// 首先尝试部署
				int score_min = 99999;
				int position = -1;
				for(j=0;j<Host_list.size();j++){
					if(Host_list[j].cpu_A>=cpu/2 && Host_list[j].cpu_B>=cpu/2 && Host_list[j].mem_A>=mem/2 && Host_list[j].mem_B>=mem/2){
						int score = Host_list[j].cpu_A+Host_list[j].cpu_B+Host_list[j].mem_A+Host_list[j].mem_B;
						if(score < score_min){
							score_min = score;
							position = j;
						} 
					}
				}
				// 若没有找到可部署位置，则购买服务器, 保存相应的购买信息 
				// 更新服务器工作信息并直接进行部署 
				if(position==-1){
					// 先寻找合适的服务器,其位置为j 
					for(j=0;j<Hosts.size();j++){
						if(Hosts[j].cpu > 1.5*cpu && Hosts[j].mem > 1.5*mem){
							break;
						}
					}
					// 将其插入到购买结果中 
					string hostname = Hosts[j].HostName;      // 想购买的服务器名称
					// 在购买列表中寻找该服务器
					host_purchase.HostName = hostname;
					for(k=0; k<purchase_result.size();k++){
						// 若找到了该服务器 
						if(!purchase_result[k].HostName.compare(hostname)){
							purchase_result[k].Hostnum++;
							break;
						}
					}
					// 若没有找到该服务器，则新建购买信息 
					if(k==purchase_result.size()){
						host_purchase.HostName = hostname;
						host_purchase.Hostnum = 1;
						purchase_result.push_back(host_purchase);
					}
					// 更新服务器工作信息
					hoststate.cpu_A = Hosts[j].cpu/2 - cpu/2;
					hoststate.cpu_B = Hosts[j].cpu/2 - cpu/2;
					hoststate.mem_A = Hosts[j].mem/2 - mem/2;
					hoststate.mem_B = Hosts[j].mem/2 - mem/2;
					hoststate.hostID = Host_list.size();
					hoststate.HostName = hostname;
					hoststate.vm_num = 1;
					hoststate.energy_cost = Hosts[j].energy_cost;
					hoststate.cpumemsum = Hosts[j].cpu+Hosts[j].mem;
					hoststate.restratio = (hoststate.cpu_A + hoststate.cpu_B + hoststate.mem_A + hoststate.mem_B)*1.0/hoststate.cpumemsum;
					Host_list.push_back(hoststate);
					vmstate.hostID = hoststate.hostID;
					vmstate.nodeFlag = 0;
					vmstate.vmID = Requests[i].vmID;
					vmstate.vmName = Requests[i].vmName;
					vmstate.change = 0; 
					VM_list.push_back(vmstate); 
					vmallocate.hostID = vmstate.hostID;
					vmallocate.nodeflag = 0;
					vmallocate.vmID = vmstate.vmID;
					allocate_result.push_back(vmallocate);
				}
				// 若找到了部署位置，位置为position，则直接进行部署
				else{
					// 修改服务器工作信息
					Host_list[position].cpu_A -= cpu/2;
					Host_list[position].cpu_B -= cpu/2;
					Host_list[position].mem_A -= mem/2;
					Host_list[position].mem_B -= mem/2;
					Host_list[position].vm_num += 1;
					Host_list[position].restratio = (Host_list[position].cpu_A + Host_list[position].cpu_B + Host_list[position].mem_A +Host_list[position].mem_B)*1.0/Host_list[position].cpumemsum;
					// 修改虚拟机工作信息
					vmstate.hostID = Host_list[position].hostID;
					vmstate.nodeFlag = 0;
					vmstate.vmID = Requests[i].vmID;
					vmstate.vmName = Requests[i].vmName;
					vmstate.change = 0;
					VM_list.push_back(vmstate);
					// 保存部署信息 
					vmallocate.hostID = Host_list[position].hostID;
					vmallocate.nodeflag = 0;
					vmallocate.vmID = Requests[i].vmID;
					allocate_result.push_back(vmallocate);
				} 
			} 
			// 若是单节点部署 
			else{
				//cout << "here3" <<endl; 
				// 首先尝试部署 
				int score_min = 99999;
				int position = -1;
				int node = -1;
				int score;
				for(j=0;j<Host_list.size();j++){
					if(Host_list[j].cpu_A >= cpu && Host_list[j].mem_A >= mem){
						score = Host_list[j].cpu_A + Host_list[j].mem_A;
						if(score < score_min){
							score_min = score;
							position = j;
							node = 1;
						}
					}
					if(Host_list[j].cpu_B >= cpu && Host_list[j].mem_B >= mem){
						score = Host_list[j].cpu_B + Host_list[j].mem_B;
						if(score < score_min){
							score_min = score;
							position = j;
							node = 2;
						}
					}
				} 
				// 若没有找到能存放的位置，则购买服务器 
				if(position==-1){
					for(j=0;j<Hosts.size();j++){
						if(Hosts[j].cpu > 3*cpu && Hosts[j].mem > 3*mem){
							break;
						}
					} 
					// 将其插入到购买结果中 
					string hostname = Hosts[j].HostName;      // 想购买的服务器名称
					// 在购买列表中寻找该服务器
					for(k=0; k<purchase_result.size();k++){
						// 若找到了该服务器 
						if(!purchase_result[k].HostName.compare(hostname)){
							purchase_result[k].Hostnum++;
							break;
						}
					}
					// 若没有找到该服务器，则新建购买信息 
					if(k==purchase_result.size()){
						host_purchase.HostName = hostname;
						host_purchase.Hostnum = 1;
						purchase_result.push_back(host_purchase);
					}
					// 更新服务器工作信息
					hoststate.cpu_A = Hosts[j].cpu/2 - cpu;
					hoststate.cpu_B = Hosts[j].cpu/2;
					hoststate.mem_A = Hosts[j].mem/2 - mem;
					hoststate.mem_B = Hosts[j].mem/2;
					hoststate.hostID = Host_list.size();
					hoststate.vm_num = 1;
					hoststate.HostName = hostname;
					hoststate.energy_cost = Hosts[j].energy_cost;
					hoststate.cpumemsum = Hosts[j].cpu+Hosts[j].mem;
					hoststate.restratio = (hoststate.cpu_A + hoststate.cpu_B + hoststate.mem_A + hoststate.mem_B)*1.0/hoststate.cpumemsum;
					Host_list.push_back(hoststate);
					vmstate.hostID = hoststate.hostID;
					vmstate.nodeFlag = 1;
					vmstate.vmID = Requests[i].vmID;
					vmstate.vmName = Requests[i].vmName;
					vmstate.change = 0;
					VM_list.push_back(vmstate); 
					vmallocate.hostID = vmstate.hostID;
					vmallocate.nodeflag = 1;
					vmallocate.vmID = vmstate.vmID;
					allocate_result.push_back(vmallocate);
				}
				// 若找到了位置，则进行部署 
				else{
					// 修改服务器工作信息
					// 修改虚拟机工作信息
					if(node==1){
						Host_list[position].cpu_A -= cpu;
						Host_list[position].mem_A -= mem;
						Host_list[position].vm_num += 1;
						vmstate.hostID = Host_list[position].hostID;
						vmstate.nodeFlag = 1;
						vmstate.vmID = Requests[i].vmID;
						vmstate.vmName = Requests[i].vmName;
						vmstate.change = 0;
						VM_list.push_back(vmstate);
					}
					else{
						Host_list[position].cpu_B -= cpu;
						Host_list[position].mem_B -= mem;
						Host_list[position].vm_num += 1;
						vmstate.hostID = Host_list[position].hostID;
						vmstate.nodeFlag = 2;
						vmstate.vmID = Requests[i].vmID;
						vmstate.vmName = Requests[i].vmName;
						vmstate.change = 0;
						VM_list.push_back(vmstate);
					}
					Host_list[position].restratio = (Host_list[position].cpu_A + Host_list[position].cpu_B + Host_list[position].mem_A +Host_list[position].mem_B)*1.0/Host_list[position].cpumemsum;
					// 保存部署信息 
					vmallocate.hostID = Host_list[position].hostID;
					vmallocate.nodeflag = node;
					vmallocate.vmID = Requests[i].vmID;
					allocate_result.push_back(vmallocate);
				}
			}
		} 
	}
	// 购买顺序id和服务器真实编码id的转换 
	int id_temp=id;
	for(j=0;j<purchase_result.size();j++){
		string name = purchase_result[j].HostName;
		for(k=id_temp;k<Host_list.size();k++){
			if(!Host_list[k].HostName.compare(name)){
				Host_list[k].realID = id;
				id++;
			}
		}
	}
	sort(Host_list.begin(), Host_list.end(), compare2);
	// 虚拟机工作信息的ID更改 
	for(j=0;j<VM_list.size();j++){
		if(VM_list[j].change==0){
			int orid = VM_list[j].hostID;
			VM_list[j].hostID = Host_list[orid].realID;
			VM_list[j].change = 1;
		}		
	}
	// 打印购买信息 
	cout << "(purchase, " << purchase_result.size() << ")" <<endl; 
	if(purchase_result.size()>0){
		for(j=0;j<purchase_result.size();j++){
			cout << "(";
			cout << purchase_result[j].HostName;
			cout << ", ";
			cout << purchase_result[j].Hostnum;
			cout << ")" << endl;
		}
	}	
	int mignum = migration_result.size();
	cout << "(migration, "<< mignum << ")" << endl;
	for(j=0;j<migration_result.size();j++){
		if(migration_result[j].node_target==0){
			cout << "(" << migration_result[j].vmID << ", " << migration_result[j].hostid_target << ")" << endl;
		}
		if(migration_result[j].node_target==1){
			cout << "(" << migration_result[j].vmID << ", " << migration_result[j].hostid_target << ", A)" << endl;
		}
		if(migration_result[j].node_target==2){
			cout << "(" << migration_result[j].vmID << ", " << migration_result[j].hostid_target << ", B)" << endl;
		}
	}
	// 打印部署信息
	for(j=0;j<allocate_result.size();j++){
		id = allocate_result[j].hostID;
		allocate_result[j].hostID = Host_list[id].realID;
		if(allocate_result[j].nodeflag==0){
			cout << "(" << allocate_result[j].hostID << ")" << endl; 
		}
		if(allocate_result[j].nodeflag==1){
			cout << "(" << allocate_result[j].hostID << ", A)" << endl; 
		}
		if(allocate_result[j].nodeflag==2){
			cout << "(" << allocate_result[j].hostID << ", B)" << endl; 
		}
	} 
}
/*
主函数 
*/
int main(){	
	//服务器提交代码 
	vector<Host> Hosts;
	unordered_map<string, VM> VMs;
	vector<VMState> VM_list;
	vector<Host_purchase> purchase_result;
	vector<HostState> Host_list; 
	vector<vector<Request>> Requests;
	Input_Process(Hosts, VMs, Requests, VM_list);
	int i,j;
	for(i=0;i<Requests.size();i++){
		vector<Request> Day_request = Requests[i];
		Main_process(Hosts, Day_request, Host_list, VMs, VM_list, purchase_result); 
	}
} 
