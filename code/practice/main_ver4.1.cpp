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
 	float cpu_mem;     // �������Լ۱� 
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
 	int hostID;     // ������ID 
 	int cpu_A;         // A�ڵ�ʣ��cpu 
 	int mem_A;         // A�ڵ�ʣ���ڴ� 
	int cpu_B;         // B�ڵ�ʣ��CPU 
	int mem_B;         // B�ڵ�ʣ���ڴ� 
	int vm_num;        // �������ϵ���������� 
	int energy_cost;   // ���������ܺ� 
};
// ���������״̬�ṹ�� 
struct VMState{
 	int vmID;       // �����ID
 	string vmName;     // ��������� 
	int hostID;     // ������ID  
 	int nodeFlag;      // 0��˫�ڵ㣬1��A�ڵ㣬2��B�ڵ� 
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

/*
�������ܣ����ļ��ж�ȡ����
output:
	 input_data:string���͵�vector��vector��ÿһ�����ļ��е�һ������(string����) 
*/
vector<string> ReadInput(){
	ifstream in("training-1.txt"); 
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
	long long cpu_demand = 15*cpu_request-cpu_rest;
	long long mem_demand = 15*mem_request-mem_rest;
	// �ҵ��ռ䲻��ôС�ĵ�һ̨������
	int pos;
	for(pos=0;pos<Hosts.size();pos++){
		if(Hosts[pos].cpu_mem >= cpu_request*1.0/mem_request){
			break;
		}
	}
	// ���ݼ��������й��� 
	//  
	if(cpu_rest>=15*cpu_request && mem_rest>=15*mem_request){
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
			temp.vm_num = 0;
			temp.energy_cost = Hosts[pos].energy_cost;
			Host_list.push_back(temp);
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
// �����Ǩ��
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
//		int mignum = 0; 
//		cout << "(migration, " << mignum << ")" <<endl;
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
//	for(int i=0;i<Hosts.size();i++){
//		cout << i << "|||" << Hosts[i].cpu << "|||" << Hosts[i].mem << endl;
//	} 
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
		
//		// ȡ����i����������� 
		vector<string> request_list(first, second);
		vector<Request> Requests = Request_process(request_list, VM_list);
//		// ���� + ���� 
		vector<Host_purchase> purchase_result = Purchase(Requests, Host_list, VMs, Hosts, i);
//		printf("-----------------befor allocation------------------\n"); 
//		for(int j=0;j<Host_list.size();j++){
//			cout<< Host_list[j].hostID << "|||" << Host_list[j].cpu_A << "|||" << Host_list[j].mem_A << "|||" << Host_list[j].cpu_mem << endl;
//		}
//		// ������ز��� 
		cout << "(migration, 0)" << endl;		
		int value = allocate(Requests, Host_list, VM_list, VMs);
		if(value == 1){
			break;
		}
//		printf("-----------------host list------------------\n"); 
//		for(int j=0;j<Host_list.size();j++){
//			cout<< Host_list[j].hostID << "|||" << Host_list[j].cpu_A << "|||" << Host_list[j].mem_A <<  "|||" << Host_list[j].cpu_B << "|||" << Host_list[j].mem_B <<endl;
//		}
		cost += Totalcost(purchase_result, Host_list, Hosts); 
//		// ������ز��� 
		pos = pos + request_num + 1;
		request_num = atoi(input_data[pos].c_str());
		first = second + 1;
		second = first + request_num;
	}	
	cout << cost << endl;
} 
