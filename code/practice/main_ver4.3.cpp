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
 	float cpu_mem;     // 服务器cpu内存比 
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
 	int hostID;        // 服务器ID 
 	int cpu_A;         // A节点剩余cpu 
 	int mem_A;         // A节点剩余内存 
	int cpu_B;         // B节点剩余CPU 
	int mem_B;         // B节点剩余内存 
	int cpumemsum;     // 该服务器原始CPU内存的和
	float restratio;   // 该服务器剩余cpu内存占比 
	int vm_num;        // 服务器上的虚拟机个数 
	int energy_cost;   // 服务器日能耗 
};
// 虚拟机工作状态结构体 
struct VMState{
 	int vmID;       // 虚拟机ID
 	string vmName;  // 虚拟机名称 
	int hostID;     // 服务器ID  
 	int nodeFlag;   // 0：双节点，1：A节点，2：B节点 
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
// 虚拟机迁移信息结构体
struct VM_migration{
	int vmID;
	string vmName;
	int hostID_source;
	int hostID_target;
	int node_source;
	int node_target;
}; 

/*
函数功能：从文件中读取输入
output:
	 input_data:string类型的vector，vector的每一项是文件中的一行数据(string类型) 
*/
vector<string> ReadInput(){
	ifstream in("training-2.txt"); 
	string filename;
	string line;
	vector<string>  input_data;
	// 逐行读入数据并存入input_data 
	if(in){
		while (getline(in, line)){ 
			input_data.push_back(line); 
		}
	}
	//无输入文件 
	else{
		cout << "no such file" << endl;
	}
	return input_data;
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
函数功能：服务器信息结构体的排序规则，将服务器按照CPU/MEM升序排列 
input:
	a: 服务器a的结构体
	b: 服务器b的结构体
output:
	服务器结构体的排序规则 
*/
bool compare(Host a, Host b){
	return (a.cpu*1.0/a.mem) < (b.cpu*1.0/b.mem);
}
/*
函数功能: 将服务器按照cpu+mem升序排列 
input: 
	input_data: 完整的输入数据，Readinput函数的输出
output:
	Hosts: 服务器按照性价比排列的结果，vector的每一项是一台服务器的结构体 
*/
vector<Host> GetHosts(vector<string> input_data){
	vector<Host> Hosts;
	vector<string> input_split;
	int hostnum = atoi(input_data[0].c_str());   // 服务器数量 
	// 建立服务器的Vector，属性为其基本信息 
	for(int i=1;i<=hostnum;i++){
		input_split = split(input_data[i],',');
		Host host;
		host.HostName = input_split[0].substr(1);
		host.cpu = atoi(input_split[1].c_str());
		host.mem = atoi(input_split[2].c_str());
		host.price = atoi(input_split[3].c_str());
		host.energy_cost = atoi(input_split[4].c_str());
		host.cpu_mem = host.cpu*1.0/host.mem;
		Hosts.push_back(host);
	}
	sort(Hosts.begin(), Hosts.end(), compare); 
	return Hosts;
}
/*
函数功能: 获得所有虚拟机的信息
input:
	input_data: 完整的输入数据，Readinput函数的输出
output:
	VMs: 虚拟机的map信息，[虚拟机名称：虚拟机(名称，cpu，mem，node_flag)] 
*/
unordered_map<string, VM> GetVMs(vector<string> input_data){
		int hostnum = atoi(input_data[0].c_str());
	int vmnum = atoi(input_data[hostnum+1].c_str());
	vector<string> input_split;
	unordered_map<string, VM> VMs;	
	for(int i=hostnum+2;i<=hostnum+2+vmnum-1;i++){
		input_split = split(input_data[i],',');
		VM vm;
		vm.vmName = input_split[0].substr(1);
		vm.cpu = atoi(input_split[1].c_str());
		vm.mem = atoi(input_split[2].c_str());
		vm.node_flag = atoi(input_split[3].c_str());
		VMs.insert(make_pair(vm.vmName, vm));
	}
	return VMs; 
}
/*
函数功能：将一天的请求序列(string类型的vector)转换为Request结构体类型的vector，结构体包括add/del, 虚拟机名, 虚拟机ID 
input:
	request_list: 一天的请求序列, string类型的vector, vector的每一行是一个请求
	VM_list: 所有虚拟机的工作状态，VMState类型的vector，vector的每一项是一台虚拟机
output:
	Requests: 一天的请求序列，Request类型的vector
*/
vector<Request> Request_process(vector<string> request_list, vector<VMState> VM_list){
	int i;
	vector<Request> Requests;
	vector<string> request_split;
	for(i=0;i<request_list.size();i++){
		request_split = split(request_list[i],',');
		Request request;
		request.type = request_split[0].substr(1);
		if(request.type.compare("add")){
			request.vmID = atoi(request_split[1].substr(1,request_split[1].length()-2).c_str());
			for(int j=0;j<VM_list.size();j++){
				if(VM_list[j].vmID==request.vmID){
					request.vmName = VM_list[j].vmName;
					break;
				}
			}
		}
		else{
			request.vmName = request_split[1].substr(1);
			request.vmID = atoi(request_split[2].substr(1,request_split[2].length()-2).c_str());
		}
		Requests.push_back(request);
	} 
	return Requests;
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
//	cout << hostnum << endl;
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
函数功能：根据一天的请求序列购买服务器
input:
	Requests: 一天的请求序列，Request类型的vector
	Host_list: 所有服务器的工作状态，Hoststate类型的vector
	VMs: 所有虚拟机的信息, VM类型的vector
	Hosts: 所有服务器信息, Host类型的vector
output:
	purchase_result: 服务器的购买结果vector，vector的每一项是一个结构体，结构体有[服务器名称，服务器数量]两个属性 
注：购买后直接更新服务器的工作状态 
*/
vector<Host_purchase> Purchase(vector<Request> Requests, vector<HostState> &Host_list, unordered_map<string, VM> VMs, vector<Host> Hosts, int flag){
	vector<Host_purchase> purchase_result;
	purchase_result.clear();
	// 服务器正常购买 
	// 计算当天请求的cpu和内存总量
	long long cpu_request = 0;
	long long mem_request = 0;
	for(int i=0;i<Requests.size();i++){
		// 若请求命令为添加 
		if(!Requests[i].type.compare("add")){
			// 虚拟机名称 
			string name = Requests[i].vmName;
			// 根据虚拟机名称确定其cpu和内存 
			cpu_request += VMs[name].cpu;
			mem_request += VMs[name].mem;
		}
	}
	// 计算系统中剩余的CPU和内存总数
	long long cpu_rest=0;
	long long mem_rest=0;
	int k;
	for(k=0;k<Host_list.size();k++){
		cpu_rest += Host_list[k].cpu_A + Host_list[k].cpu_B;
		mem_rest += Host_list[k].mem_A + Host_list[k].mem_B;
	} 
	// 计算需要的CPU和内存总数 
	long long cpu_demand = 11*cpu_request-cpu_rest;
	long long mem_demand = 11*mem_request-mem_rest;
	// 找到空间不那么小的第一台服务器 
	int pos;
	for(pos=0;pos<Hosts.size();pos++){
		if(Hosts[pos].cpu_mem >= cpu_request*1.0/mem_request){
			break;
		}
	}
	// 根据计算结果进行购买 
	//  
	if(cpu_rest>=11*cpu_request && mem_rest>=11*mem_request){
		
	}
	else{
		if(cpu_rest>=11*cpu_request && mem_rest>=11*mem_request){
			int num1 = ceil(cpu_request*1.0 / Hosts[pos].cpu);
			int num2 = ceil(mem_request*1.0 / Hosts[pos].mem);
			int num_1 = max(num1,num2);
			int num3 = ceil(cpu_demand*1.0 / Hosts[pos].cpu);
			int num4 = ceil(mem_demand*1.0 / Hosts[pos].mem);
			int num_2 = max(num3,num4);
			int num = min(num_1, num_2);
			if(flag==0){
				num += 3;
			}
			else{
				num += 0;
			}
			Host_purchase host_purchase;
			host_purchase.HostName = Hosts[pos].HostName;
			host_purchase.Hostnum = num;
			purchase_result.push_back(host_purchase);
			for(int j=0;j<num;j++){
				HostState temp;
				///// 注意：此处的hostID算法是从空列表开始算的 
				temp.hostID = Host_list.size();
				temp.cpu_A = Hosts[pos].cpu/2;
				temp.cpu_B = Hosts[pos].cpu/2;
				temp.mem_A = Hosts[pos].mem/2;
				temp.mem_B = Hosts[pos].mem/2;
				temp.cpumemsum = Hosts[pos].cpu + Hosts[pos].mem;
				temp.restratio = 1; 
				temp.vm_num = 0;
				temp.energy_cost = Hosts[pos].energy_cost;
				Host_list.push_back(temp);
			}
		}
		else{
			int num1 = ceil(cpu_request*1.0 / Hosts[pos].cpu);
			int num2 = ceil(mem_request*1.0 / Hosts[pos].mem);
			int num_1 = max(num1,num2);
			int num3 = ceil(cpu_demand*1.0 / Hosts[pos].cpu);
			int num4 = ceil(mem_demand*1.0 / Hosts[pos].mem);
			int num_2 = max(num3,num4);
			int num = min(num_1, num_2);
			if(flag==0){
				num += 3;
			}
			else{
				num += 2;
			}
			Host_purchase host_purchase;
			host_purchase.HostName = Hosts[pos].HostName;
			host_purchase.Hostnum = num;
			purchase_result.push_back(host_purchase);
			for(int j=0;j<num;j++){
				HostState temp;
				///// 注意：此处的hostID算法是从空列表开始算的 
				temp.hostID = Host_list.size();
				temp.cpu_A = Hosts[pos].cpu/2;
				temp.cpu_B = Hosts[pos].cpu/2;
				temp.mem_A = Hosts[pos].mem/2;
				temp.mem_B = Hosts[pos].mem/2;
				temp.cpumemsum = Hosts[pos].cpu + Hosts[pos].mem;
				temp.restratio = 1; 
				temp.vm_num = 0;
				temp.energy_cost = Hosts[pos].energy_cost;
				Host_list.push_back(temp);
			}
		}
	
	}
	// 输出购买信息 
	cout << "(purchase, " << purchase_result.size() << ")" <<endl; 
	if(purchase_result.size()>0){
		for(int j=0;j<purchase_result.size();j++){
			cout << "(";
			cout << purchase_result[j].HostName;
			cout << ", ";
			cout << purchase_result[j].Hostnum;
			cout << ")" << endl;
		}
	}	
	return purchase_result;
} 
/*
函数功能：虚拟机迁移 
*/
void migration(vector<HostState> &Host_list, vector<VMState> &VM_list, unordered_map<string, VM> &VMs){
	// 首先遍历所有服务器，计算空间剩余比例 
	vector<HostState> Host_list_source;
	vector<HostState> Host_list_target;
	vector<HostState> Host_list_temp;
	vector<VM_migration> migration_result; 
	int mignum_max = VM_list.size()*5/1000;
	float treshhold = 0.1;                // 剩余空间阈值，设置为80%，可最多降低至50% 
	int i,j;
	// 按照阈值将服务器分为两部分，一部分剩余空间大，一部分剩余空间小。将前者迁移至后者中，从而清空某些服务器。 
	for(i=0;i<Host_list.size();i++){
//		if(Host_list[i].restratio>=treshhold){
			Host_list_temp.push_back(Host_list[i]);
//		}
//		else{
//			Host_list_target.push_back(Host_list[i]);
//		}
	} 
	for(i=0;i<floor(Host_list_temp.size()/1.2);i++){
		Host_list_source.push_back(Host_list_temp[i]);
		
	}
	for(i=floor(Host_list_temp.size()/1.2);i<Host_list_temp.size();i++){
		Host_list_target.push_back(Host_list_temp[i]);
	}
	// 对每台源服务器，考察其中的每一台虚拟机
	for(j=0;j<Host_list_source.size();j++){
		// 服务器的服务器ID 
		int hostid = Host_list_source[j].hostID;
		//cout << "the hostid is:" << hostid << endl;
		// 寻找该服务器中的所有虚拟机，并考察是否可迁移
		int k;
		for(k=0;k<VM_list.size();k++){
			//cout << "the kth vm is in host" << VM_list[k].hostID << endl;
			// 若第k台虚拟机在ID为hostid的服务器中，则尝试对其进行迁移 
			if(VM_list[k].hostID == hostid){
				// 第k台虚拟机的所有基本信息 
				int vmID = VM_list[k].vmID;
				string vmName = VM_list[k].vmName;
				int nodeflag = VM_list[k].nodeFlag;
				int cpu = VMs[vmName].cpu;
				int mem = VMs[vmName].mem;
				//cout << cpu << "|||" << mem << endl;
				// 分情况对其进行部署
				// 若是双节点部署 
				if(nodeflag==0){
					int score_min = 999999;
					int position = -1; 
					int p;
					// 依次在目标服务器中进行寻找 
					for(p=0;p<Host_list_target.size();p++){
						if(Host_list_target[p].cpu_A >= cpu/2 && Host_list_target[p].cpu_B >= cpu/2 && Host_list_target[p].mem_A >= mem/2 && Host_list_target[p].mem_B >= mem/2){
							int score = Host_list_target[p].cpu_A+Host_list_target[p].cpu_B+Host_list_target[p].mem_A+Host_list_target[p].mem_B;
							if(score < score_min){
								score_min = score;
								position = p;
							}
						}
					}
					// 若存在能存放的服务器则进行迁移，若不存在则不进行迁移 
					if(position!=-1){
						// 保存迁移信息 
						//cout << "YESYESYESYSEYS" << endl; 
						if(migration_result.size()>=mignum_max){
							break;
						}
						VM_migration mig;
						mig.hostID_source = hostid;
						mig.hostID_target = Host_list_target[position].hostID;
						mig.node_source = 0;
						mig.node_target = 0;
						mig.vmID = vmID;
						mig.vmName = vmName;
						migration_result.push_back(mig);
						// 更新源服务器，目标服务器和虚拟机的工作信息
						// 原服务器信息不需要更新，因为遍历时只取源服务器的ID 
						// 目标服务器信息需要更新，因为每次都要选择cpu内存最小且能存放的服务器
						Host_list_target[position].cpu_A -= cpu/2;
						Host_list_target[position].cpu_B -= cpu/2; 
						Host_list_target[position].mem_A -= mem/2;
						Host_list_target[position].mem_B -= mem/2;
						Host_list_target[position].vm_num += 1; 
						// Host_list修改
						Host_list[hostid].cpu_A += cpu/2;
						Host_list[hostid].cpu_B += cpu/2;
						Host_list[hostid].mem_A += mem/2;
						Host_list[hostid].mem_B += mem/2;
						Host_list[hostid].vm_num -= 1;
						Host_list[hostid].restratio = (Host_list[hostid].cpu_A+Host_list[hostid].cpu_B+Host_list[hostid].mem_A+Host_list[hostid].mem_B)*1.0/Host_list[hostid].cpumemsum;
						Host_list[Host_list_target[position].hostID].cpu_A -= cpu/2;
						Host_list[Host_list_target[position].hostID].cpu_B -= cpu/2;
						Host_list[Host_list_target[position].hostID].mem_A -= mem/2;
						Host_list[Host_list_target[position].hostID].mem_B -= mem/2;
						Host_list[Host_list_target[position].hostID].vm_num += 1;
						Host_list[Host_list_target[position].hostID].restratio = (Host_list[Host_list_target[position].hostID].cpu_A+Host_list[Host_list_target[position].hostID].cpu_B+Host_list[Host_list_target[position].hostID].mem_A+Host_list[Host_list_target[position].hostID].mem_B)*1.0/Host_list[Host_list_target[position].hostID].cpumemsum;
						// VM_list修改
						VM_list[k].hostID = Host_list_target[position].hostID;
						VM_list[k].nodeFlag = 0;
					} 
				} 
				// 若是单节点部署 
				else{
					int score_min = 999999;
					int position = -1; 
					int node_source = nodeflag;         // 1-A,2-B
					int node_target; 
					int p;
					for(p=0;p<Host_list_target.size();p++){
						// 对A节点进行打分 
						if(Host_list_target[p].cpu_A>=cpu && Host_list_target[p].mem_A>=mem){
							int score = Host_list_target[p].cpu_A+Host_list_target[p].mem_A;
							if(score < score_min){
								score_min = score;
								position = p;
								node_target = 1;
							}
						}
						// 对B节点进行打分
						if(Host_list_target[p].cpu_B>=cpu && Host_list_target[p].mem_B>=mem){
							int score = Host_list_target[p].cpu_B + Host_list_target[p].mem_B;
							if(score<score_min){
								score_min = score;
								position = p;
								node_target = 2;
							}
						} 
					}
					// 若找到了可存放的服务器，则进行迁移，否则不进行迁移
					if(position!=-1){
						if(migration_result.size()>=mignum_max){
							break;
						}
						// 保存迁移信息 
						VM_migration mig;
						mig.hostID_source = hostid;
						mig.hostID_target = Host_list_target[position].hostID;
						mig.node_source = node_source;
						mig.node_target = node_target;
						mig.vmID = vmID;
						mig.vmName = vmName;
						migration_result.push_back(mig);
						// 更新源服务器，目标服务器和虚拟机的工作信息
						// 原服务器信息不需要更新，因为遍历时只取源服务器的ID 
						// 目标服务器信息需要更新，因为每次都要选择cpu内存最小且能存放的服务器
						if(node_target==1){
							Host_list_target[position].cpu_A -= cpu;
							Host_list_target[position].mem_A -= mem;
							Host_list_target[position].vm_num += 1;
						}
						else{
							Host_list_target[position].cpu_B -= cpu;
							Host_list_target[position].mem_B -= mem;
							Host_list_target[position].vm_num += 1;
						}
						// Host_list修改
						if(node_source==1){
							Host_list[hostid].cpu_A += cpu;
							Host_list[hostid].mem_A += mem;
							Host_list[hostid].vm_num -= 1;
						}
						else{
							Host_list[hostid].cpu_B += cpu;
							Host_list[hostid].mem_B += mem;
							Host_list[hostid].vm_num -= 1;
						}
						Host_list[hostid].restratio = (Host_list[hostid].cpu_A+Host_list[hostid].cpu_B+Host_list[hostid].mem_A+Host_list[hostid].mem_B)*1.0/Host_list[hostid].cpumemsum;
						if(node_target==1){
							Host_list[Host_list_target[position].hostID].cpu_A -= cpu;
							Host_list[Host_list_target[position].hostID].mem_A -= mem;
							Host_list[Host_list_target[position].hostID].vm_num += 1;
						}
						else{
							Host_list[Host_list_target[position].hostID].cpu_B -= cpu;
							Host_list[Host_list_target[position].hostID].mem_B -= mem;
							Host_list[Host_list_target[position].hostID].vm_num += 1;
						}
						Host_list[Host_list_target[position].hostID].restratio = (Host_list[Host_list_target[position].hostID].cpu_A+Host_list[Host_list_target[position].hostID].cpu_B+Host_list[Host_list_target[position].hostID].mem_A+Host_list[Host_list_target[position].hostID].mem_B)*1.0/Host_list[Host_list_target[position].hostID].cpumemsum;
						// VM_list修改
						VM_list[k].hostID = Host_list_target[position].hostID;
						VM_list[k].nodeFlag = node_target;
					} 
				} 
			}
		} 
		if(migration_result.size()>=mignum_max){
			break;
		}
	} 
//	cout << migration_result.size() << endl;
//	// 打印迁移信息
	int mignum = migration_result.size();
	cout << "(migration, " << mignum << ")" << endl;
	for(j=0;j<mignum;j++){
		if(migration_result[j].node_target==0){
			cout << "(" << migration_result[j].vmID << ", " <<migration_result[j].hostID_target << ")" <<endl;
		}
		if(migration_result[j].node_target==1){
			cout << "(" << migration_result[j].vmID << ", " <<migration_result[j].hostID_target << ", A)" <<endl;
		}
		if(migration_result[j].node_target==2){
			cout << "(" << migration_result[j].vmID << ", " <<migration_result[j].hostID_target << ", B)" <<endl;
		}
	} 
	
}
/*
函数功能：虚拟机部署 
*/
int vm_allocate(int cpu, int mem, int vmID, string vmName, int node_flag, vector<HostState> &Host_list, vector<VMState> &VM_list){
//	cout << cpu <<"|||"<<mem << endl; 
	// 单节点部署, 在所有节点中选择cpu+mem最小的 
	if(node_flag==0){
		int score_min = 99999;         // 初始化最小的cpu内存和 
		int position = -1;             // 初始化存放服务器的编号 
		int node = 0;                  // 0表示A，1表示B 
		int i;
		for(i=0;i<Host_list.size();i++){
			// 先考察A节点
			// 若能存放再看其是否合适 
			if(Host_list[i].cpu_A >= cpu && Host_list[i].mem_A >= mem){
				int score = Host_list[i].cpu_A + Host_list[i].mem_A;
				if(score < score_min){
					score_min = score;
					position = i;
					node = 0;
				}
			}
			// 再考察B节点 
			if(Host_list[i].cpu_B >= cpu && Host_list[i].mem_B >= mem){
				int score = Host_list[i].cpu_B + Host_list[i].mem_B;
				if(score < score_min){
					score_min = score;
					position = i;
					node = 1;
				}
			}
		} 
		// 没有找到能存放的位置 
		if(position==-1){
			cout << "1" << endl; 
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			return 1;
		}
		else{
			// A节点部署 
			if(node==0){
				Host_list[position].cpu_A -= cpu;
				Host_list[position].mem_A -= mem;
				Host_list[position].vm_num += 1;
				Host_list[position].restratio = (Host_list[position].cpu_A + Host_list[position].cpu_B + Host_list[position].mem_A + Host_list[position].mem_B)*1.0/Host_list[position].cpumemsum;
				VMState temp;
				temp.hostID = Host_list[position].hostID;
				temp.vmID = vmID;
				temp.vmName = vmName;
				temp.nodeFlag = 1;
				VM_list.push_back(temp); 
				// 分配成功							
				cout<< "(" << temp.hostID << ", A)" << endl;
			}
			// B节点部署 
			else{
				Host_list[position].cpu_B -= cpu;
				Host_list[position].mem_B -= mem;
				Host_list[position].vm_num += 1;
				Host_list[position].restratio = (Host_list[position].cpu_A + Host_list[position].cpu_B + Host_list[position].mem_A + Host_list[position].mem_B)*1.0/Host_list[position].cpumemsum;
				VMState temp;
				temp.hostID = Host_list[position].hostID;
				temp.vmID = vmID;
				temp.vmName = vmName;
				temp.nodeFlag = 2;
				VM_list.push_back(temp); 
				// 分配成功							
				cout<< "(" << temp.hostID << ", B)" << endl;
			}
		} 
	}
	// 双节点部署 
	else{
		int score_min = 99999;
		int position = -1;
		int i;
		for(i=0;i<Host_list.size();i++){
			if(Host_list[i].cpu_A >= cpu/2 && Host_list[i].cpu_B >= cpu/2 && Host_list[i].mem_A >= mem/2 && Host_list[i].mem_B >= mem/2){
				int score = Host_list[i].cpu_A + Host_list[i].cpu_B + Host_list[i].mem_A + Host_list[i].mem_B;
				if(score < score_min){
					score_min = score;
					position = i;
				}
			}
		}
		if(position == -1){
			cout << "2" << endl; 
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			return 1;
		}
		else{
			Host_list[position].cpu_A -= cpu/2;
			Host_list[position].mem_A -= mem/2;
			Host_list[position].cpu_B -= cpu/2;
			Host_list[position].mem_B -= mem/2;
			Host_list[position].vm_num += 1;
			Host_list[position].restratio = (Host_list[position].cpu_A + Host_list[position].cpu_B + Host_list[position].mem_A + Host_list[position].mem_B)*1.0/Host_list[position].cpumemsum;
			VMState temp;
			temp.hostID = Host_list[position].hostID;
			temp.vmID = vmID;
			temp.vmName = vmName;
			temp.nodeFlag = 0;
			VM_list.push_back(temp); 
			// 分配成功							
			cout<< "(" << temp.hostID << ")" << endl;
		}
	} 
	return 0;
}
/*
函数功能: 服务器部署
input: 
	Requests: 一天的请求序列，Request类型的vector;
	Host_list: 所有服务器的工作状态，Hoststate类型的vector;
	VM_list: 所有虚拟机的工作状态，VMState类型的vector，vector的每一项是一台虚拟机;
	VMs: 所有虚拟机的信息, VM类型的vector
注：部署后更新服务器和虚拟机的工作状态 
*/
int allocate(vector<Request> Requests, vector<HostState> &Host_list, vector<VMState> &VM_list, unordered_map<string, VM> VMs){
	// 逐条处理请求 
	for(int i=0;i<Requests.size();i++){  
		// 若第i条请求为添加请求 
		if(!Requests[i].type.compare("add")){
			string vmName = Requests[i].vmName;
			int vmID = Requests[i].vmID;
			VM thevm = VMs[vmName];          
			int cpu = thevm.cpu;
			int mem = thevm.mem; 
			int node_flag = VMs[Requests[i].vmName].node_flag;
			int value = vm_allocate(cpu, mem, vmID, vmName, node_flag, Host_list, VM_list);
			if(value==1){
				return 1;
			}
		}
		// 若是删除操作 
		else{
			string vmName;
			int vmID = Requests[i].vmID;
			int cpu;
			int mem;
			// 更新服务器工作信息
			int k=0;
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
				Host_list[hostid].restratio = (Host_list[hostid].cpu_A + Host_list[hostid].cpu_B + Host_list[hostid].mem_A + Host_list[hostid].mem_B)*1.0/Host_list[hostid].cpumemsum;
				Host_list[hostid].vm_num -= 1;
			}
			else{
				if(VM_list[k].nodeFlag==1){
					Host_list[hostid].cpu_A += cpu;
					Host_list[hostid].mem_A += mem;
					Host_list[hostid].restratio = (Host_list[hostid].cpu_A + Host_list[hostid].cpu_B + Host_list[hostid].mem_A + Host_list[hostid].mem_B)*1.0/Host_list[hostid].cpumemsum;
					Host_list[hostid].vm_num -= 1;
				}
				else{
					Host_list[hostid].cpu_B += cpu;
					Host_list[hostid].mem_B += mem;
					Host_list[hostid].restratio = (Host_list[hostid].cpu_A + Host_list[hostid].cpu_B + Host_list[hostid].mem_A + Host_list[hostid].mem_B)*1.0/Host_list[hostid].cpumemsum;
					Host_list[hostid].vm_num -= 1;
				}
			}
			VM_list.erase(VM_list.begin()+k); 
		}
	}
	return 0;
}
/*
函数功能：计算总成本，根据每天的购买结果和服务器部署结果计算总成本 
*/
int Totalcost(vector<Host_purchase> purchase_result, vector<HostState> Host_list, vector<Host> Hosts){
	long long int cost = 0;
	int i,j,k;
	for(i=0;i<purchase_result.size();i++){
		string name = purchase_result[i].HostName;
		int num = purchase_result[i].Hostnum;
		for(j=0;j<Hosts.size();j++){
			if(!name.compare(Hosts[j].HostName)){
				break;
			}
		} 
		int price = Hosts[j].price;
		cost += price * num;
	}
	for(i=0;i<Host_list.size();i++){
		if(Host_list[i].vm_num!=0){
			cost += Host_list[i].energy_cost;
		}
	}
	return cost;
}

int main(){	
	//服务器提交代码 
//	vector<Host> Hosts;
//	unordered_map<string, VM> VMs;
//	vector<VMState> VM_list;
//	vector<HostState> Host_list; 
//	vector<vector<Request>> Requests;
//	Input_Process(Hosts, VMs, Requests, VM_list);
//	int i,j;
//	for(i=0;i<Requests.size();i++){
//		vector<Request> Day_request = Requests[i];
//		vector<Host_purchase> purchase_result = Purchase(Day_request, Host_list, VMs, Hosts,i);
//		migration(Host_list, VM_list, VMs);	
//		allocate(Day_request, Host_list, VM_list, VMs);	
//	}
	

	//   本地测试代码 
	// 从文件中读取输入 
	long long cost = 0; 
	vector<string> input_data;
	input_data = ReadInput();     //input_data:string类型的vector，vector的每一项是文件中的一行数据(string类型)   
	// 处理输入数据，得到服务器信息(按照性价比排列)和虚拟机信息
	vector<Host> Hosts;
	unordered_map<string, VM> VMs; 
	Hosts = GetHosts(input_data); // 获取服务器按照性价比的排列结果 
	VMs = GetVMs(input_data);     // 获取所有虚拟机信息
	// 从输入数据中依次提取日请求，并进行处理
	int hostnum = atoi(input_data[0].c_str()); 
	int vmnum = atoi(input_data[hostnum+1].c_str());
	int days = atoi(input_data[hostnum+vmnum+2].c_str());       // 请求总天数 
	int i;
	// 服务器和虚拟机列表初始化 
	vector<HostState> Host_list;
	vector<VMState> VM_list; 
	// 依次处理每天的请求 
	int pos = hostnum + vmnum + 3;
	int request_num = atoi(input_data[pos].c_str());	
	vector<string>::const_iterator first = input_data.begin()+pos+1;
	vector<string>::const_iterator second = first + request_num;
	for(i=0;i<days;i++){	
		// 取出第i天的请求序列 
		vector<string> request_list(first, second);
		vector<Request> Requests = Request_process(request_list, VM_list);
		// 购买 + 迁移 + 部署 + 计算代价 
		vector<Host_purchase> purchase_result = Purchase(Requests, Host_list, VMs, Hosts, i);
//		cout << "------------before allocation:----------------" << endl;
//		for(int j=0;j<Host_list.size();j++){
//			cout<<Host_list[j].hostID<<" ||| "<<Host_list[j].cpu_A << " ||| " << Host_list[j].mem_A <<" ||| " <<Host_list[j].cpu_B<<" ||| "<<Host_list[j].mem_B << endl;
//		}
		migration(Host_list, VM_list, VMs);	
//		cout << "------------after allocation:----------------"	 << endl;
//		for(int j=0;j<Host_list.size();j++){
//			cout<<Host_list[j].hostID<<" ||| "<<Host_list[j].cpu_A << " ||| " << Host_list[j].mem_A <<" ||| " <<Host_list[j].cpu_B<<" ||| "<<Host_list[j].mem_B << endl;
//		}
		int value = allocate(Requests, Host_list, VM_list, VMs);
		if(value == 1){
			break;
		}
		cost += Totalcost(purchase_result, Host_list, Hosts); 
		// 更新相关参数 
		pos = pos + request_num + 1;
		request_num = atoi(input_data[pos].c_str());
		first = second + 1;
		second = first + request_num;
	}	
	cout << cost << endl;
} 
