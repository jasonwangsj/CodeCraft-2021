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
 	float performance_cost;  // 服务器性价比 
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
 	int hostID;     // 服务器ID 
 	int cpu_A;         // A节点剩余cpu 
 	int mem_A;         // A节点剩余内存 
	int cpu_B;         // B节点剩余CPU 
	int mem_B;         // B节点剩余内存 
	int vm_num;        // 服务器上的虚拟机个数 
	int energy_cost;   // 服务器日能耗 
	int flag;          // 服务器评价指标 
};
// 虚拟机工作状态结构体 
struct VMState{
 	int vmID;       // 虚拟机ID
 	string vmName;     // 虚拟机名称 
	int hostID;     // 服务器ID  
 	int nodeFlag;      // 0：双节点，1：A节点，2：B节点 
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

/*
函数功能：从文件中读取输入
output:
	 input_data:string类型的vector，vector的每一项是文件中的一行数据(string类型) 
*/
vector<string> ReadInput(){
	ifstream in("training-1.txt"); 
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
函数功能：读取标准输入
output:
	 input_data:string类型的vector，vector的每一项是文件中的一行数据(string类型) 
*/
vector<string> ReadInput1(){
	string str;
	vector<string> input_data;
	while(getline(cin,str)){
		input_data.push_back(str);
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
函数功能：服务器信息结构体的排序规则，将服务器按照性价比降序排列 
input:
	a: 服务器a的结构体
	b: 服务器b的结构体
output:
	服务器结构体的排序规则 
*/
bool compare(Host a, Host b){
	return a.performance_cost>b.performance_cost;
}
/*
函数功能: 将服务器按照性价比降序排列
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
		host.performance_cost = host.cpu*host.mem*1.0/host.price;
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
	// 首先读服务器个数 
	int hostnum;
	scanf("%d",&hostnum);
	int i,j;
	char s[100];
	string temp;
	// 依次读入服务器信息 
	for(i=0;i<hostnum;i++){
		Host host;
		for(j=0;j<5;j++){
			scanf("%s",s);
			temp = s;
			switch(j){
				case 0:
					host.HostName = temp.substr(1,temp.size()-2);
					break;
				case 1:
					host.cpu = atoi(temp.c_str());
					break;
				case 2:
					host.mem = atoi(temp.c_str());
					break;
				case 3:
					host.price = atoi(temp.c_str());
					break;
				case 4:
					host.energy_cost = atoi(temp.c_str());
					break;
			}		
		}
		host.performance_cost = host.cpu*host.mem*1.0/host.price;
		Hosts.push_back(host);
	}
	//sort(Hosts.begin(), Hosts.end(), compare);
	// 读入虚拟机个数	
	int vmnum;
	scanf("%d",&vmnum);
	// 依次读入虚拟机信息 
	for(i=0;i<vmnum;i++){
		VM vm;
		for(j=0;j<4;j++){
			scanf("%s",s);
			temp = s;
			switch(j){
				case 0:
					vm.vmName = temp.substr(1,temp.size()-2);
					break;
				case 1:
					vm.cpu = atoi(temp.c_str());
					break;
				case 2:
					vm.mem = atoi(temp.c_str());
					break;
				case 3:
					vm.node_flag = atoi(temp.c_str());
					break;
			}
		}
		VMs.insert(make_pair(vm.vmName, vm));
	}
	// 读入若干天的处理请求
	vector<Request> Request_day;
	int days;
	scanf("%d",&days);
	for(i=0;i<days;i++){
		int requestnum;
		scanf("%d",&requestnum);
		Request_day.clear();
		for(j=0;j<requestnum;j++){
			int k;
			Request request;
			for(k=0;k<3;k++){
				if(k==0){
					scanf("%s",s);
					temp = s;
					request.type = temp.substr(1,temp.size()-2);
				} 
				else{
					if(request.type.compare("del")){
						if(k==1){
							scanf("%s",s);
							temp = s;
							request.vmName = temp.substr(0,temp.size()-1);
						}
						if(k==2){
							scanf("%s",s);
							temp = s;
							request.vmID = atoi(temp.c_str());
						}
					}
					else{
						//int temp1;
						//scanf("%d",&temp1);
						//request.vmID = temp1;
						if(k==2){
							scanf("%s",s);
							temp = s;
							request.vmID = atoi(temp.c_str());
							request.vmName = "empty";
						}
					}
				}
			}
			Request_day.push_back(request);
		}
		Requests.push_back(Request_day);
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
vector<Host_purchase> Purchase(vector<Request> Requests, vector<HostState> &Host_list, unordered_map<string, VM> VMs, vector<Host> Hosts){
	vector<Host_purchase> purchase_result;
	purchase_result.clear();
	// 计算当天请求的cpu和内存总量
	int cpu_request = 0;
	int mem_request = 0;
	// 逐个处理请求 
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
	int cpu_rest=0;
	int mem_rest=0;
	
	for(int i=0;i<Host_list.size();i++){
		cpu_rest += Host_list[i].cpu_A + Host_list[i].cpu_B;
		mem_rest += Host_list[i].mem_A + Host_list[i].mem_B;
	} 
	
	// 计算购买的服务器个数，当前方案中仅购买性价比最高的服务器
	int cpu_demand = 2*cpu_request - cpu_rest;
	int mem_demand = 2*mem_request - mem_rest;
	// cpu和内存均不需要 
	if(cpu_demand<0 && mem_demand<0){
	}
	else{
		int num1 = ceil(cpu_demand*1.0 / Hosts[0].cpu);
		int num2 = ceil(mem_demand*1.0 / Hosts[0].mem);
		int num = max(num1,num2);
		Host_purchase host_purchase;
		host_purchase.HostName = Hosts[0].HostName;
		host_purchase.Hostnum = num;
		purchase_result.push_back(host_purchase);
		for(int i=0;i<num;i++){
			HostState temp;
			///// 注意：此处的hostID算法是从空列表开始算的 
			temp.hostID = Host_list.size();
			temp.cpu_A = Hosts[0].cpu/2;
			temp.cpu_B = Hosts[0].cpu/2;
			temp.mem_A = Hosts[0].mem/2;
			temp.mem_B = Hosts[0].mem/2;
			temp.vm_num = 0;
			temp.flag = 0;
			temp.energy_cost = Hosts[0].energy_cost;
			Host_list.push_back(temp);
		}
	}
	cout << "(purchase, " << purchase_result.size() << ")" <<endl; 
	if(purchase_result.size()>0){
		for(int i=0;i<purchase_result.size();i++){
			cout << "(";
			cout << purchase_result[i].HostName;
			cout << ", ";
			cout << purchase_result[i].Hostnum;
			cout << ")" << endl;
		}
	}
	
	return purchase_result;
} 
// 虚拟机迁移
/*
函数功能: 服务器部署
input: 
	Requests: 一天的请求序列，Request类型的vector;
	Host_list: 所有服务器的工作状态，Hoststate类型的vector;
	VM_list: 所有虚拟机的工作状态，VMState类型的vector，vector的每一项是一台虚拟机;
	VMs: 所有虚拟机的信息, VM类型的vector
注：部署后更新服务器和虚拟机的工作状态 
*/
void allocate(vector<Request> Requests, vector<HostState> &Host_list, vector<VMState> &VM_list, unordered_map<string, VM> VMs){
	int totalnum = 0;
	// 逐条处理请求 
	for(int i=0;i<Requests.size();i++){
		string name = Requests[i].vmName;
		VM thevm = VMs[name];
		// 若第i条请求为添加请求 
		if(!Requests[i].type.compare("add")){
			int flag = 0;                       // flag=0表示第i条请求没有分配出去 
			// 根据其基本信息进行虚拟机部署
			// 若是单节点部署
			if(VMs[Requests[i].vmName].node_flag==0){
				// 依次对各服务器进行判断 
				for(int k=0;k<Host_list.size();k++){
					
					// 若第k台服务器的A节点可存下该虚拟机,则直接存储 
					if(Host_list[k].cpu_A>=thevm.cpu && Host_list[k].mem_A>=thevm.mem){
						// 首先修改服务器信息 
						Host_list[k].cpu_A -= thevm.cpu;
						Host_list[k].mem_A -= thevm.mem;
						Host_list[k].vm_num += 1;
						// 接着修改虚拟机信息
						VMState temp;
						temp.hostID = Host_list[k].hostID;
						temp.vmID = Requests[i].vmID;
						temp.vmName = Requests[i].vmName;
						temp.nodeFlag = 1;
						VM_list.push_back(temp); 
						// 分配成功
						flag = 1;
						// 输出分配结果
						
						cout<< "(";
						cout<< temp.hostID;
						cout<< ", A)"<<endl;
						
						totalnum++;
						break; 
						
					}
					else{
						// 第k台服务器的A节点存不下该虚拟机,但B节点可存下,则直接存储 
						if(Host_list[k].cpu_B>=thevm.cpu && Host_list[k].mem_B>=thevm.mem){
							// 首先修改服务器信息 
							Host_list[k].cpu_B -= thevm.cpu;
							Host_list[k].mem_B -= thevm.mem;
							Host_list[k].vm_num += 1;
							// 接着修改虚拟机信息
							VMState temp;
							temp.hostID = Host_list[k].hostID;
							temp.vmID = Requests[i].vmID;
							temp.vmName = Requests[i].vmName;
							temp.nodeFlag = 2;
							VM_list.push_back(temp); 
							// 分配成功
							flag = 1;
							
							cout<< "(";
							cout<< temp.hostID;
							cout<< ", B)"<<endl;
							
							totalnum++;
							
							break; 
							
						}
					}
				}
			} 
			//若是双节点部署 
			else{
				// 依次对各服务器进行判断 
				for(int k=0;k<Host_list.size();k++){
					// A节点和B节点均满足，则进行分配 
					if(Host_list[k].cpu_A>=thevm.cpu/2 && Host_list[k].mem_A>=thevm.mem/2 && Host_list[k].cpu_B>=thevm.cpu/2 && Host_list[k].mem_B>=thevm.mem/2){
						// 首先修改服务器信息 
						Host_list[k].cpu_A -= thevm.cpu/2;
						Host_list[k].mem_A -= thevm.mem/2;
						Host_list[k].cpu_B -= thevm.cpu/2;
						Host_list[k].mem_B -= thevm.mem/2;
						Host_list[k].vm_num += 1;
						// 接着修改虚拟机信息
						VMState temp;
						temp.hostID = Host_list[k].hostID;
						temp.vmID = Requests[i].vmID;
						temp.vmName = Requests[i].vmName;
						temp.nodeFlag = 0;
						VM_list.push_back(temp); 
						// 分配成功
						flag = 1;		
								
						cout<< "(";
						cout<< temp.hostID; 
						cout<< ")"<<endl;
						
						totalnum++;
						break; 
					}
				}
			}
			if(flag==0){
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
			}
			
		}
		// 若是删除操作 
		else{
			// 更新服务器工作信息
			int k=0;
			for(k=0;k<VM_list.size();k++){
				// 若列表中第K台虚拟机的ID与当前任务的ID匹配 
				if(VM_list[k].vmID==Requests[i].vmID){
					break; 
				} 
			} 
			int hostid = VM_list[k].hostID;
			if(VM_list[k].nodeFlag==0){
				Host_list[hostid].cpu_A += thevm.cpu/2;
				Host_list[hostid].mem_A += thevm.mem/2;
				Host_list[hostid].cpu_B += thevm.cpu/2;
				Host_list[hostid].mem_B += thevm.mem/2;
				Host_list[hostid].vm_num -= 1;
			}
			else{
				if(VM_list[k].nodeFlag==1){
					Host_list[hostid].cpu_A += thevm.cpu/2;
					Host_list[hostid].mem_A += thevm.mem/2;
					Host_list[hostid].vm_num -= 1;
				}
				else{
					Host_list[hostid].cpu_B += thevm.cpu/2;
					Host_list[hostid].mem_B += thevm.mem/2;
					Host_list[hostid].vm_num -= 1;
				}
			}
			VM_list.erase(VM_list.begin()+k); 
//			VM_list[k].vmID = 0;
//			VM_list[k].vmName = "empty";
//			VM_list[k].hostID = 0;
//			VM_list[k].nodeFlag = -1;
		}
	}
//	cout << totalnum << endl;
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
//		vector<Host_purchase> purchase_result = Purchase(Day_request, Host_list, VMs, Hosts);
//		int mignum = 0; 
//		cout << "(migration, " << mignum << ")" <<endl;
//		allocate(Day_request, Host_list, VM_list, VMs);	
//	}
	

	//   本地测试代码 
	// 从文件中读取输入 
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
	for(i=0;i<10;i++){
		// 取出第i天的请求序列 
		vector<string> request_list(first, second);
		vector<Request> Requests = Request_process(request_list, VM_list);
		// 购买 + 部署 
		vector<Host_purchase> purchase_result = Purchase(Requests, Host_list, VMs, Hosts);
		// 更新相关参数 
//		for(int j=0;j<Host_list.size();j++){
//			cout << Host_list[j].hostID << endl;
//		} 
		cout << "(migration, ";
		printf("%d)\n",0);
		
		allocate(Requests, Host_list, VM_list, VMs); 
//		for(int j=0;j<Host_list.size();j++){
//			cout << Host_list[j].hostID << endl;
//		} 
		// 更新相关参数 
		pos = pos + request_num + 1;
		request_num = atoi(input_data[pos].c_str());
		first = second + 1;
		second = first + request_num;
	}	
//	for(i=0;i<VM_list.size();i++){
//		cout << VM_list[i].vmID << "|||" <<VM_list[i].vmName << "|||" << VM_list[i].hostID << "|||" << VM_list[i].nodeFlag << endl;
//	}
} 
