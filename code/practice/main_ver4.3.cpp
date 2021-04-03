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
// ��������Ϣ�ṹ�� 
struct Host{
	string HostName;   // �������� 
 	int cpu;           // ������CPU��С 
 	int mem;           // �������ڴ��С 
 	int price;         // ������Ӳ���ɱ� 
 	int energy_cost;   // ���������ܺ� 
 	float cpu_mem;     // ������cpu�ڴ�� 
};
// �������Ϣ�ṹ�� 
struct VM{
 	string vmName;     // ���������  
 	int cpu;           // CPU���� 
 	int mem;           // �ڴ����� 
 	int node_flag;     // ���ڵ�or˫�ڵ�洢 
};
// ����������״̬�ṹ�� 
struct HostState{
 	int hostID;        // ������ID 
 	int cpu_A;         // A�ڵ�ʣ��cpu 
 	int mem_A;         // A�ڵ�ʣ���ڴ� 
	int cpu_B;         // B�ڵ�ʣ��CPU 
	int mem_B;         // B�ڵ�ʣ���ڴ� 
	int cpumemsum;     // �÷�����ԭʼCPU�ڴ�ĺ�
	float restratio;   // �÷�����ʣ��cpu�ڴ�ռ�� 
	int vm_num;        // �������ϵ���������� 
	int energy_cost;   // ���������ܺ� 
};
// ���������״̬�ṹ�� 
struct VMState{
 	int vmID;       // �����ID
 	string vmName;  // ��������� 
	int hostID;     // ������ID  
 	int nodeFlag;   // 0��˫�ڵ㣬1��A�ڵ㣬2��B�ڵ� 
};
// ������ṹ�� 
struct Request{
	string type;     // ���orɾ�� 
	int vmID;     // �����ID 
	string vmName;   // ��������� 
};
// ������������Ϣ�ṹ�� 
struct Host_purchase{
	string HostName; // ����ķ������� 
	int Hostnum;     // ����ķ��������� 
};
// �����Ǩ����Ϣ�ṹ��
struct VM_migration{
	int vmID;
	string vmName;
	int hostID_source;
	int hostID_target;
	int node_source;
	int node_target;
}; 

/*
�������ܣ����ļ��ж�ȡ����
output:
	 input_data:string���͵�vector��vector��ÿһ�����ļ��е�һ������(string����) 
*/
vector<string> ReadInput(){
	ifstream in("training-2.txt"); 
	string filename;
	string line;
	vector<string>  input_data;
	// ���ж������ݲ�����input_data 
	if(in){
		while (getline(in, line)){ 
			input_data.push_back(line); 
		}
	}
	//�������ļ� 
	else{
		cout << "no such file" << endl;
	}
	return input_data;
}
/*
�������ܣ��ַ����ָ���ַ�������ָ���ַ��ָ�Ϊ����
input��
	s: ���ָ���ַ��� 
	delim: �����ַ����ָ���ַ�
output:
	v: string���͵�vector��vector��ÿһ���Ƿָ���һ���Ӵ� 
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
�������ܣ���������Ϣ�ṹ���������򣬽�����������CPU/MEM�������� 
input:
	a: ������a�Ľṹ��
	b: ������b�Ľṹ��
output:
	�������ṹ���������� 
*/
bool compare(Host a, Host b){
	return (a.cpu*1.0/a.mem) < (b.cpu*1.0/b.mem);
}
/*
��������: ������������cpu+mem�������� 
input: 
	input_data: �������������ݣ�Readinput���������
output:
	Hosts: �����������Լ۱����еĽ����vector��ÿһ����һ̨�������Ľṹ�� 
*/
vector<Host> GetHosts(vector<string> input_data){
	vector<Host> Hosts;
	vector<string> input_split;
	int hostnum = atoi(input_data[0].c_str());   // ���������� 
	// ������������Vector������Ϊ�������Ϣ 
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
��������: ����������������Ϣ
input:
	input_data: �������������ݣ�Readinput���������
output:
	VMs: �������map��Ϣ��[��������ƣ������(���ƣ�cpu��mem��node_flag)] 
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
�������ܣ���һ�����������(string���͵�vector)ת��ΪRequest�ṹ�����͵�vector���ṹ�����add/del, �������, �����ID 
input:
	request_list: һ�����������, string���͵�vector, vector��ÿһ����һ������
	VM_list: ����������Ĺ���״̬��VMState���͵�vector��vector��ÿһ����һ̨�����
output:
	Requests: һ����������У�Request���͵�vector
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
��������:����׼���벢ͬ���������ݴ������ܵ���Readinput +  GetHosts + GetVMs + Request_process
input:
	Hosts: �����������Լ۱����еĽ����vector��ÿһ����һ̨�������Ľṹ�壻
	VMs: �������map��Ϣ��[��������ƣ������(���ƣ�cpu��mem��node_flag)]  
*/
void Input_Process(vector<Host> &Hosts,unordered_map<string, VM> &VMs, vector<vector<Request>> &Requests, vector<VMState> VM_list){	
	// ����������Ϣ 
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
	// ���������Ϣ
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
	// ��ÿ������ 
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
�������ܣ�����һ����������й��������
input:
	Requests: һ����������У�Request���͵�vector
	Host_list: ���з������Ĺ���״̬��Hoststate���͵�vector
	VMs: �������������Ϣ, VM���͵�vector
	Hosts: ���з�������Ϣ, Host���͵�vector
output:
	purchase_result: �������Ĺ�����vector��vector��ÿһ����һ���ṹ�壬�ṹ����[���������ƣ�����������]�������� 
ע�������ֱ�Ӹ��·������Ĺ���״̬ 
*/
vector<Host_purchase> Purchase(vector<Request> Requests, vector<HostState> &Host_list, unordered_map<string, VM> VMs, vector<Host> Hosts, int flag){
	vector<Host_purchase> purchase_result;
	purchase_result.clear();
	// �������������� 
	// ���㵱�������cpu���ڴ�����
	long long cpu_request = 0;
	long long mem_request = 0;
	for(int i=0;i<Requests.size();i++){
		// ����������Ϊ��� 
		if(!Requests[i].type.compare("add")){
			// ��������� 
			string name = Requests[i].vmName;
			// �������������ȷ����cpu���ڴ� 
			cpu_request += VMs[name].cpu;
			mem_request += VMs[name].mem;
		}
	}
	// ����ϵͳ��ʣ���CPU���ڴ�����
	long long cpu_rest=0;
	long long mem_rest=0;
	int k;
	for(k=0;k<Host_list.size();k++){
		cpu_rest += Host_list[k].cpu_A + Host_list[k].cpu_B;
		mem_rest += Host_list[k].mem_A + Host_list[k].mem_B;
	} 
	// ������Ҫ��CPU���ڴ����� 
	long long cpu_demand = 11*cpu_request-cpu_rest;
	long long mem_demand = 11*mem_request-mem_rest;
	// �ҵ��ռ䲻��ôС�ĵ�һ̨������ 
	int pos;
	for(pos=0;pos<Hosts.size();pos++){
		if(Hosts[pos].cpu_mem >= cpu_request*1.0/mem_request){
			break;
		}
	}
	// ���ݼ��������й��� 
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
				///// ע�⣺�˴���hostID�㷨�Ǵӿ��б�ʼ��� 
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
				///// ע�⣺�˴���hostID�㷨�Ǵӿ��б�ʼ��� 
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
	// ���������Ϣ 
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
�������ܣ������Ǩ�� 
*/
void migration(vector<HostState> &Host_list, vector<VMState> &VM_list, unordered_map<string, VM> &VMs){
	// ���ȱ������з�����������ռ�ʣ����� 
	vector<HostState> Host_list_source;
	vector<HostState> Host_list_target;
	vector<HostState> Host_list_temp;
	vector<VM_migration> migration_result; 
	int mignum_max = VM_list.size()*5/1000;
	float treshhold = 0.1;                // ʣ��ռ���ֵ������Ϊ80%������ཱུ����50% 
	int i,j;
	// ������ֵ����������Ϊ�����֣�һ����ʣ��ռ��һ����ʣ��ռ�С����ǰ��Ǩ���������У��Ӷ����ĳЩ�������� 
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
	// ��ÿ̨Դ���������������е�ÿһ̨�����
	for(j=0;j<Host_list_source.size();j++){
		// �������ķ�����ID 
		int hostid = Host_list_source[j].hostID;
		//cout << "the hostid is:" << hostid << endl;
		// Ѱ�Ҹ÷������е�������������������Ƿ��Ǩ��
		int k;
		for(k=0;k<VM_list.size();k++){
			//cout << "the kth vm is in host" << VM_list[k].hostID << endl;
			// ����k̨�������IDΪhostid�ķ������У����Զ������Ǩ�� 
			if(VM_list[k].hostID == hostid){
				// ��k̨����������л�����Ϣ 
				int vmID = VM_list[k].vmID;
				string vmName = VM_list[k].vmName;
				int nodeflag = VM_list[k].nodeFlag;
				int cpu = VMs[vmName].cpu;
				int mem = VMs[vmName].mem;
				//cout << cpu << "|||" << mem << endl;
				// �����������в���
				// ����˫�ڵ㲿�� 
				if(nodeflag==0){
					int score_min = 999999;
					int position = -1; 
					int p;
					// ������Ŀ��������н���Ѱ�� 
					for(p=0;p<Host_list_target.size();p++){
						if(Host_list_target[p].cpu_A >= cpu/2 && Host_list_target[p].cpu_B >= cpu/2 && Host_list_target[p].mem_A >= mem/2 && Host_list_target[p].mem_B >= mem/2){
							int score = Host_list_target[p].cpu_A+Host_list_target[p].cpu_B+Host_list_target[p].mem_A+Host_list_target[p].mem_B;
							if(score < score_min){
								score_min = score;
								position = p;
							}
						}
					}
					// �������ܴ�ŵķ����������Ǩ�ƣ����������򲻽���Ǩ�� 
					if(position!=-1){
						// ����Ǩ����Ϣ 
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
						// ����Դ��������Ŀ���������������Ĺ�����Ϣ
						// ԭ��������Ϣ����Ҫ���£���Ϊ����ʱֻȡԴ��������ID 
						// Ŀ���������Ϣ��Ҫ���£���Ϊÿ�ζ�Ҫѡ��cpu�ڴ���С���ܴ�ŵķ�����
						Host_list_target[position].cpu_A -= cpu/2;
						Host_list_target[position].cpu_B -= cpu/2; 
						Host_list_target[position].mem_A -= mem/2;
						Host_list_target[position].mem_B -= mem/2;
						Host_list_target[position].vm_num += 1; 
						// Host_list�޸�
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
						// VM_list�޸�
						VM_list[k].hostID = Host_list_target[position].hostID;
						VM_list[k].nodeFlag = 0;
					} 
				} 
				// ���ǵ��ڵ㲿�� 
				else{
					int score_min = 999999;
					int position = -1; 
					int node_source = nodeflag;         // 1-A,2-B
					int node_target; 
					int p;
					for(p=0;p<Host_list_target.size();p++){
						// ��A�ڵ���д�� 
						if(Host_list_target[p].cpu_A>=cpu && Host_list_target[p].mem_A>=mem){
							int score = Host_list_target[p].cpu_A+Host_list_target[p].mem_A;
							if(score < score_min){
								score_min = score;
								position = p;
								node_target = 1;
							}
						}
						// ��B�ڵ���д��
						if(Host_list_target[p].cpu_B>=cpu && Host_list_target[p].mem_B>=mem){
							int score = Host_list_target[p].cpu_B + Host_list_target[p].mem_B;
							if(score<score_min){
								score_min = score;
								position = p;
								node_target = 2;
							}
						} 
					}
					// ���ҵ��˿ɴ�ŵķ������������Ǩ�ƣ����򲻽���Ǩ��
					if(position!=-1){
						if(migration_result.size()>=mignum_max){
							break;
						}
						// ����Ǩ����Ϣ 
						VM_migration mig;
						mig.hostID_source = hostid;
						mig.hostID_target = Host_list_target[position].hostID;
						mig.node_source = node_source;
						mig.node_target = node_target;
						mig.vmID = vmID;
						mig.vmName = vmName;
						migration_result.push_back(mig);
						// ����Դ��������Ŀ���������������Ĺ�����Ϣ
						// ԭ��������Ϣ����Ҫ���£���Ϊ����ʱֻȡԴ��������ID 
						// Ŀ���������Ϣ��Ҫ���£���Ϊÿ�ζ�Ҫѡ��cpu�ڴ���С���ܴ�ŵķ�����
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
						// Host_list�޸�
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
						// VM_list�޸�
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
//	// ��ӡǨ����Ϣ
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
�������ܣ���������� 
*/
int vm_allocate(int cpu, int mem, int vmID, string vmName, int node_flag, vector<HostState> &Host_list, vector<VMState> &VM_list){
//	cout << cpu <<"|||"<<mem << endl; 
	// ���ڵ㲿��, �����нڵ���ѡ��cpu+mem��С�� 
	if(node_flag==0){
		int score_min = 99999;         // ��ʼ����С��cpu�ڴ�� 
		int position = -1;             // ��ʼ����ŷ������ı�� 
		int node = 0;                  // 0��ʾA��1��ʾB 
		int i;
		for(i=0;i<Host_list.size();i++){
			// �ȿ���A�ڵ�
			// ���ܴ���ٿ����Ƿ���� 
			if(Host_list[i].cpu_A >= cpu && Host_list[i].mem_A >= mem){
				int score = Host_list[i].cpu_A + Host_list[i].mem_A;
				if(score < score_min){
					score_min = score;
					position = i;
					node = 0;
				}
			}
			// �ٿ���B�ڵ� 
			if(Host_list[i].cpu_B >= cpu && Host_list[i].mem_B >= mem){
				int score = Host_list[i].cpu_B + Host_list[i].mem_B;
				if(score < score_min){
					score_min = score;
					position = i;
					node = 1;
				}
			}
		} 
		// û���ҵ��ܴ�ŵ�λ�� 
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
			// A�ڵ㲿�� 
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
				// ����ɹ�							
				cout<< "(" << temp.hostID << ", A)" << endl;
			}
			// B�ڵ㲿�� 
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
				// ����ɹ�							
				cout<< "(" << temp.hostID << ", B)" << endl;
			}
		} 
	}
	// ˫�ڵ㲿�� 
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
			// ����ɹ�							
			cout<< "(" << temp.hostID << ")" << endl;
		}
	} 
	return 0;
}
/*
��������: ����������
input: 
	Requests: һ����������У�Request���͵�vector;
	Host_list: ���з������Ĺ���״̬��Hoststate���͵�vector;
	VM_list: ����������Ĺ���״̬��VMState���͵�vector��vector��ÿһ����һ̨�����;
	VMs: �������������Ϣ, VM���͵�vector
ע���������·�������������Ĺ���״̬ 
*/
int allocate(vector<Request> Requests, vector<HostState> &Host_list, vector<VMState> &VM_list, unordered_map<string, VM> VMs){
	// ������������ 
	for(int i=0;i<Requests.size();i++){  
		// ����i������Ϊ������� 
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
		// ����ɾ������ 
		else{
			string vmName;
			int vmID = Requests[i].vmID;
			int cpu;
			int mem;
			// ���·�����������Ϣ
			int k=0;
			for(k=0;k<VM_list.size();k++){
				// ���б��е�K̨�������ID�뵱ǰ�����IDƥ�� 
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
�������ܣ������ܳɱ�������ÿ��Ĺ������ͷ����������������ܳɱ� 
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
	//�������ύ���� 
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
	

	//   ���ز��Դ��� 
	// ���ļ��ж�ȡ���� 
	long long cost = 0; 
	vector<string> input_data;
	input_data = ReadInput();     //input_data:string���͵�vector��vector��ÿһ�����ļ��е�һ������(string����)   
	// �����������ݣ��õ���������Ϣ(�����Լ۱�����)���������Ϣ
	vector<Host> Hosts;
	unordered_map<string, VM> VMs; 
	Hosts = GetHosts(input_data); // ��ȡ�����������Լ۱ȵ����н�� 
	VMs = GetVMs(input_data);     // ��ȡ�����������Ϣ
	// ������������������ȡ�����󣬲����д���
	int hostnum = atoi(input_data[0].c_str()); 
	int vmnum = atoi(input_data[hostnum+1].c_str());
	int days = atoi(input_data[hostnum+vmnum+2].c_str());       // ���������� 
	int i;
	// ��������������б��ʼ�� 
	vector<HostState> Host_list;
	vector<VMState> VM_list; 
	// ���δ���ÿ������� 
	int pos = hostnum + vmnum + 3;
	int request_num = atoi(input_data[pos].c_str());	
	vector<string>::const_iterator first = input_data.begin()+pos+1;
	vector<string>::const_iterator second = first + request_num;
	for(i=0;i<days;i++){	
		// ȡ����i����������� 
		vector<string> request_list(first, second);
		vector<Request> Requests = Request_process(request_list, VM_list);
		// ���� + Ǩ�� + ���� + ������� 
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
		// ������ز��� 
		pos = pos + request_num + 1;
		request_num = atoi(input_data[pos].c_str());
		first = second + 1;
		second = first + request_num;
	}	
	cout << cost << endl;
} 
