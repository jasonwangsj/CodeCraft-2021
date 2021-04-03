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
 	int hostID;        // ������ID 
 	int cpu_A;         // A�ڵ�ʣ��cpu 
 	int mem_A;         // A�ڵ�ʣ���ڴ� 
	int cpu_B;         // B�ڵ�ʣ��CPU 
	int mem_B;         // B�ڵ�ʣ���ڴ� 
	int vm_num;        // �������ϵ���������� 
	int energy_cost;   // ���������ܺ� 
	float cpu_mem_A;
	float cpu_mem_B;
	float cpu_mem;
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
�������ܣ���������Ϣ�ṹ���������򣬽������������Լ۱Ƚ������� 
input:
	a: ������a�Ľṹ��
	b: ������b�Ľṹ��
output:
	�������ṹ���������� 
*/
bool compare(Host a, Host b){
	return a.cpu_mem<b.cpu_mem;
}
/*
��������: �������������Լ۱Ƚ�������
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
	// ���ȶ����������� 
	int hostnum;
	scanf("%d",&hostnum);
	int i,j;
	char s[100];
	string temp;
	// ���ζ����������Ϣ 
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
		host.cpu_mem = host.cpu*1.0/host.mem;
		Hosts.push_back(host);
	}
	//sort(Hosts.begin(), Hosts.end(), compare);
	// �������������	
	int vmnum;
	scanf("%d",&vmnum);
	// ���ζ����������Ϣ 
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
	// ����������Ĵ�������
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
vector<Host_purchase> Purchase(vector<Request> Requests, vector<HostState> &Host_list, unordered_map<string, VM> VMs, vector<Host> Hosts){
	vector<Host_purchase> purchase_result;
	purchase_result.clear();
	// ���㵱�������cpu���ڴ�����
	long long cpu_request = 0;
	long long mem_request = 0;
	// ����������� 
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
	float day_cpu_mem = cpu_request*1.0/mem_request;        // ���������CPU���ڴ�� 
	// ����ϵͳ��ʣ���CPU���ڴ�����
	long long cpu_rest=0;
	long long mem_rest=0;
	int k;
	for(k=0;k<Host_list.size();k++){
		cpu_rest += Host_list[k].cpu_A + Host_list[k].cpu_B;
		mem_rest += Host_list[k].mem_A + Host_list[k].mem_B;
	} 
	// ���㹺��ķ�������������ǰ������CPU�ڴ����ӽ��ķ�����
	int i;
	for(i=0;i<Hosts.size();i++){
		// �ҵ�cpu�ڴ���Դ�ķ�����--��į������ 
		if(Hosts[i].cpu_mem>=day_cpu_mem){
			break;
		}
	}
//	cout << cpu_request << "|||" << mem_request << endl;
	long long cpu_demand = 1*cpu_request;
	long long mem_demand = 1*mem_request;
	// cpu���ڴ������Ҫ 
	if(cpu_rest >= 100*cpu_request && mem_rest >= 100*mem_request){
		
	}
	else{
		int num1 = ceil(cpu_demand*1.0 / Hosts[i].cpu);
		int num2 = ceil(mem_demand*1.0 / Hosts[i].mem);
		int num = max(num1,num2)+1;
		Host_purchase host_purchase;
		host_purchase.HostName = Hosts[i].HostName;
		host_purchase.Hostnum = num;
		purchase_result.push_back(host_purchase);
		for(int j=0;j<num;j++){
			HostState temp;
			///// ע�⣺�˴���hostID�㷨�Ǵӿ��б�ʼ��� 
			temp.hostID = Host_list.size();
			temp.cpu_A = Hosts[i].cpu/2;
			temp.cpu_B = Hosts[i].cpu/2;
			temp.mem_A = Hosts[i].mem/2;
			temp.mem_B = Hosts[i].mem/2;
			temp.cpu_mem_A = temp.cpu_A*1.0/temp.mem_A;
			temp.cpu_mem_B = temp.cpu_mem_A;
			temp.cpu_mem = temp.cpu_mem_A;
			temp.vm_num = 0;
			temp.energy_cost = Hosts[i].energy_cost;
			Host_list.push_back(temp);
		}
	}
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
����������״̬�б������ڵ�CPU�ڴ�ȵľ�ֵ�ɴ�С���� 
*/
bool host_compare(HostState a, HostState b){
	return a.cpu_mem > b.cpu_mem;
} 
/*
����������״̬�б�ֱ���AB�ڵ�CPU�ڴ���ɴ�С���� 
*/
bool host_compare1(HostState a, HostState b){
	if(a.cpu_mem_A!=b.cpu_mem_A){
		return a.cpu_mem_A > b.cpu_mem_A;
	}
	else{
		return abs(a.cpu_mem_A-a.cpu_mem_B)>abs(b.cpu_mem_A-b.cpu_mem_B);
	}
}
bool host_compare2(HostState a, HostState b){
	if(a.cpu_mem_B!=b.cpu_mem_B){
		return a.cpu_mem_B > b.cpu_mem_B;
	}
	else{
		return abs(a.cpu_mem_A-a.cpu_mem_B)>abs(b.cpu_mem_A-b.cpu_mem_B);
	}
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
void allocate(vector<Request> Requests, vector<HostState> &Host_list, vector<VMState> &VM_list, unordered_map<string, VM> VMs){
	// ������������ 
	for(int i=0;i<Requests.size();i++){
		string name = Requests[i].vmName;
		VM thevm = VMs[name];             
		// ����i������Ϊ������� 
		if(!Requests[i].type.compare("add")){
			float cpu_mem_vm = thevm.cpu*1.0/thevm.mem;   // ��ǰ�������cpu�ڴ�� 			
			// ���ǵ��ڵ㲿�𣬷������б���ʱ����������˳����Һ��ʵķ�����
			// ���ǵ��ڵ㲿��
			if(VMs[Requests[i].vmName].node_flag==0){
				
			//if(0){
				//sort(Host_list.begin(), Host_list.end(), host_compare); 
				int flag=0;
				// ���ζԸ������������ж� 
				for(int k=0;k<Host_list.size();k++){					
					if((Host_list[k].cpu_A>=thevm.cpu)&&(Host_list[k].mem_A>=thevm.mem)){
						// ��A�ڵ��ܴ��£�B�ڵ㲻��
						if(!((Host_list[k].cpu_B>=thevm.cpu)&&(Host_list[k].mem_B>=thevm.mem))){
							// �����޸ķ�������Ϣ 
							Host_list[k].cpu_A -= thevm.cpu;
							Host_list[k].mem_A -= thevm.mem;
							Host_list[k].vm_num += 1;
							// �����޸��������Ϣ
							VMState temp;
							temp.hostID = Host_list[k].hostID;
							temp.vmID = Requests[i].vmID;
							temp.vmName = Requests[i].vmName;
							temp.nodeFlag = 1;
							VM_list.push_back(temp); 
							if(Host_list[k].mem_A==0){
								Host_list[k].cpu_mem_A = 0;
								Host_list[k].cpu_mem = Host_list[k].cpu_mem_B/2;
							}
							else{
								Host_list[k].cpu_mem_A = Host_list[k].cpu_A*1.0/Host_list[k].mem_A;
								Host_list[k].cpu_mem = (Host_list[k].cpu_mem_A + Host_list[k].cpu_mem_B)/2;
							}
							// ����ɹ�
							flag = 1;
							// ���������
							cout<< "(" << temp.hostID << ", A)" << endl;						
							break; 
						}
						// ��A�ڵ��ܴ��£�B�ڵ��ܴ���
						else{
							if(Host_list[k].cpu_A>=Host_list[k].cpu_B){
								// �����޸ķ�������Ϣ 
								Host_list[k].cpu_A -= thevm.cpu;
								Host_list[k].mem_A -= thevm.mem;
								Host_list[k].vm_num += 1;
								// �����޸��������Ϣ
								VMState temp;
								temp.hostID = Host_list[k].hostID;
								temp.vmID = Requests[i].vmID;
								temp.vmName = Requests[i].vmName;
								if(Host_list[k].mem_A==0){
									Host_list[k].cpu_mem_A = 0;
									Host_list[k].cpu_mem = Host_list[k].cpu_mem_B/2;
								}
								else{
									Host_list[k].cpu_mem_A = Host_list[k].cpu_A*1.0/Host_list[k].mem_A;
									Host_list[k].cpu_mem = (Host_list[k].cpu_mem_A + Host_list[k].cpu_mem_B)/2;
								}
								temp.nodeFlag = 1;
								VM_list.push_back(temp); 
								// ����ɹ�
								flag = 1;
								// ���������
								cout<< "(" << temp.hostID << ", A)" << endl;						
								break; 
							}
							else{
								// �����޸ķ�������Ϣ 
								Host_list[k].cpu_B -= thevm.cpu;
								Host_list[k].mem_B -= thevm.mem;
								Host_list[k].vm_num += 1;
								// �����޸��������Ϣ
								VMState temp;
								temp.hostID = Host_list[k].hostID;
								temp.vmID = Requests[i].vmID;
								temp.vmName = Requests[i].vmName;
								temp.nodeFlag = 2;
								VM_list.push_back(temp); 
								// ����ɹ�
								flag = 1;
								if(Host_list[k].mem_B==0){
									Host_list[k].cpu_mem_B = 0;
									Host_list[k].cpu_mem = Host_list[k].cpu_mem_A/2;
								}
								else{
									Host_list[k].cpu_mem_B = Host_list[k].cpu_B*1.0/Host_list[k].mem_B;
									Host_list[k].cpu_mem = (Host_list[k].cpu_mem_A + Host_list[k].cpu_mem_B)/2;
								}
								cout << "(" << temp.hostID << ", B)" << endl;			
								break; 
							}
						}
					}
					else{
						// ��A�ڵ㲻�ܴ��£�B�ڵ��ܴ��� 
						if(((Host_list[k].cpu_B>=thevm.cpu)&&(Host_list[k].mem_B>=thevm.mem))){
							// �����޸ķ�������Ϣ 
							Host_list[k].cpu_B -= thevm.cpu;
							Host_list[k].mem_B -= thevm.mem;
							Host_list[k].vm_num += 1;
							// �����޸��������Ϣ
							VMState temp;
							temp.hostID = Host_list[k].hostID;
							temp.vmID = Requests[i].vmID;
							temp.vmName = Requests[i].vmName;
							if(Host_list[k].mem_B==0){
								Host_list[k].cpu_mem_B = 0;
								Host_list[k].cpu_mem = Host_list[k].cpu_mem_A/2;
							}
							else{
								Host_list[k].cpu_mem_B = Host_list[k].cpu_B*1.0/Host_list[k].mem_B;
								Host_list[k].cpu_mem = (Host_list[k].cpu_mem_A + Host_list[k].cpu_mem_B)/2;
							}
							temp.nodeFlag = 2;
							VM_list.push_back(temp); 
							// ����ɹ�
							flag = 1;
							cout << "(" << temp.hostID << ", B)" << endl;			
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
			// ����˫�ڵ㲿������cpu�ڴ�ȶԷ������������� 
			else{
				
				int flag = 0;                     // flag=0��ʾ��i������û�з����ȥ
				// ���Ƚ��������б������ڵ�CPU�ڴ�ȵľ�ֵ�ɴ�С���� 
				sort(Host_list.begin(), Host_list.end(), host_compare); 
				// ��CPU�ڴ�ȴ��ڵ���1����CPU�����ڴ棬����б�ײ���ʼ���� 
				if(cpu_mem_vm>=1){
					// ���б�ײ���ʼ������Ѱ�ҵ�һ��CPU�ڴ���Դ�ķ����� 
					int j;
					for(j=Host_list.size()-1;j>=0;j--){
						if(Host_list[j].cpu_mem>=cpu_mem_vm){
							break;
						}
					}
					if(j==-1){
						j=0;
					}
					// �ӵ�j��λ�ÿ�ʼѰ�����Ž����ܴ�ŵķ�����
					int k;
					for(k=j;k>=0;k--){
						if(Host_list[k].cpu_A>=thevm.cpu/2 && Host_list[k].cpu_B>=thevm.cpu/2 && Host_list[k].mem_A>=thevm.mem/2 && Host_list[k].mem_B>=thevm.mem/2){
							flag = 1;
							break;
						}
					} 
					if(flag==0){
						// ��û���ҵ��ܴ�ŵķ�������������б�����λ��
						for(k=j;k<Host_list.size();k++){
							if(Host_list[k].cpu_A>=thevm.cpu/2 && Host_list[k].cpu_B>=thevm.cpu/2 && Host_list[k].mem_A>=thevm.mem/2 && Host_list[k].mem_B>=thevm.mem/2){
								flag = 1;
								break;
							}
						}
					}
					// ���ҵ��˺���λ��, ���޸���Ӧ������Ϣ 
					if(flag==1){
						// �����޸ķ�������Ϣ 
						Host_list[k].cpu_A -= thevm.cpu/2;
						Host_list[k].mem_A -= thevm.mem/2;
						Host_list[k].cpu_B -= thevm.cpu/2;
						Host_list[k].mem_B -= thevm.mem/2;
						Host_list[k].vm_num += 1;
						// ��AB�ڵ���ڴ����Ϊ0����������������ֵ 
						if(!(Host_list[k].mem_A==0||Host_list[k].mem_B==0)){
							Host_list[k].cpu_mem_A = Host_list[k].cpu_A*1.0/Host_list[k].mem_A;
							Host_list[k].cpu_mem_B = Host_list[k].cpu_B*1.0/Host_list[k].mem_B;
							Host_list[k].cpu_mem = (Host_list[k].cpu_mem_A + Host_list[k].cpu_mem_B)/2;
						}
						else{
							if(Host_list[k].mem_A==0 && Host_list[k].cpu_mem_A){
								Host_list[k].cpu_mem_A = 0;
								Host_list[k].cpu_mem_B = 0;
								Host_list[k].cpu_mem = 0;
							}
							else{
								if(Host_list[k].mem_A==0){
									Host_list[k].cpu_mem_A = 0;
									Host_list[k].cpu_mem_B = Host_list[k].cpu_B*1.0/Host_list[k].mem_B;
									Host_list[k].cpu_mem = Host_list[k].cpu_mem_B/2;
								}
								else{
									Host_list[k].cpu_mem_A = Host_list[k].cpu_A*1.0/Host_list[k].mem_A;
									Host_list[k].cpu_mem_B = 0;
									Host_list[k].cpu_mem = Host_list[k].cpu_mem_A/2;
								}
							}
						}
						// �����޸��������Ϣ
						VMState temp;
						temp.hostID = Host_list[k].hostID;
						temp.vmID = Requests[i].vmID;
						temp.vmName = Requests[i].vmName;
						temp.nodeFlag = 0;
						VM_list.push_back(temp); 
						// ����ɹ�
						flag = 1;										
						cout<< "(" << temp.hostID << ")" << endl;						
					} 
					else{
						cout << thevm.vmName << endl; 
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
				// ��CPU�ڴ��С��1 
				else{
					int j;
					for(j=0;j<Host_list.size();j++){
						if(Host_list[j].cpu_mem<=cpu_mem_vm){
							break;
						}
					}
					if(j==Host_list.size()){
						j--;
					}
					int k;
					for(k=j;k<Host_list.size();k++){
						if(Host_list[k].cpu_A>=thevm.cpu/2 && Host_list[k].cpu_B>=thevm.cpu/2 && Host_list[k].mem_A>=thevm.mem/2 && Host_list[k].mem_B>=thevm.mem/2){
							flag = 1;
							break;
						}
					} 
					if(flag==0){
						// ��û���ҵ��ܴ�ŵķ�������������б�����λ��
						for(k=j;k>=0;k--){
							if(Host_list[k].cpu_A>=thevm.cpu/2 && Host_list[k].cpu_B>=thevm.cpu/2 && Host_list[k].mem_A>=thevm.mem/2 && Host_list[k].mem_B>=thevm.mem/2){
								flag = 1;
								break;
							}
						}
					}
					// ���ҵ��˺���λ��, ���޸���Ӧ������Ϣ 
					if(flag==1){
						// �����޸ķ�������Ϣ 
						Host_list[k].cpu_A -= thevm.cpu/2;
						Host_list[k].mem_A -= thevm.mem/2;
						Host_list[k].cpu_B -= thevm.cpu/2;
						Host_list[k].mem_B -= thevm.mem/2;
						Host_list[k].vm_num += 1;
						// ��AB�ڵ���ڴ����Ϊ0����������������ֵ 
						if(!(Host_list[k].mem_A==0||Host_list[k].mem_B==0)){
							Host_list[k].cpu_mem_A = Host_list[k].cpu_A*1.0/Host_list[k].mem_A;
							Host_list[k].cpu_mem_B = Host_list[k].cpu_B*1.0/Host_list[k].mem_B;
							Host_list[k].cpu_mem = (Host_list[k].cpu_mem_A + Host_list[k].cpu_mem_B)/2;
						}
						else{
							if(Host_list[k].mem_A==0 && Host_list[k].cpu_mem_A){
								Host_list[k].cpu_mem_A = 0;
								Host_list[k].cpu_mem_B = 0;
								Host_list[k].cpu_mem = 0;
							}
							else{
								if(Host_list[k].mem_A==0){
									Host_list[k].cpu_mem_A = 0;
									Host_list[k].cpu_mem_B = Host_list[k].cpu_B*1.0/Host_list[k].mem_B;
									Host_list[k].cpu_mem = Host_list[k].cpu_mem_B/2;
								}
								else{
									Host_list[k].cpu_mem_A = Host_list[k].cpu_A*1.0/Host_list[k].mem_A;
									Host_list[k].cpu_mem_B = 0;
									Host_list[k].cpu_mem = Host_list[k].cpu_mem_A/2;
								}
							}
						}
						// �����޸��������Ϣ
						VMState temp;
						temp.hostID = Host_list[k].hostID;
						temp.vmID = Requests[i].vmID;
						temp.vmName = Requests[i].vmName;
						temp.nodeFlag = 0;
						VM_list.push_back(temp); 
						// ����ɹ�
						flag = 1;										
						cout<< "(" << temp.hostID << ")" << endl;						
					} 
					else{
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
			
			}
			
			
		}
		// ����ɾ������ 
		else{
			// ���·�����������Ϣ
			int k=0;
			for(k=0;k<VM_list.size();k++){
				// ���б��е�K̨�������ID�뵱ǰ�����IDƥ�� 
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
		}
	}
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
	vector<Host> Hosts;
	unordered_map<string, VM> VMs;
	vector<VMState> VM_list;
	vector<HostState> Host_list; 
	vector<vector<Request>> Requests;
	Input_Process(Hosts, VMs, Requests, VM_list);
	int i,j;
	for(i=0;i<Requests.size();i++){
		vector<Request> Day_request = Requests[i];
		vector<Host_purchase> purchase_result = Purchase(Day_request, Host_list, VMs, Hosts);
		int mignum = 0; 
		cout << "(migration, " << mignum << ")" <<endl;
		allocate(Day_request, Host_list, VM_list, VMs);	
	}
	

	//   ���ز��Դ��� 
	// ���ļ��ж�ȡ���� 
//	long long cost = 0; 
//	vector<string> input_data;
//	input_data = ReadInput();     //input_data:string���͵�vector��vector��ÿһ�����ļ��е�һ������(string����)   
//	// �����������ݣ��õ���������Ϣ(�����Լ۱�����)���������Ϣ
//	vector<Host> Hosts;
//	unordered_map<string, VM> VMs; 
//	Hosts = GetHosts(input_data); // ��ȡ�����������Լ۱ȵ����н�� 
//	VMs = GetVMs(input_data);     // ��ȡ�����������Ϣ
//	// ������������������ȡ�����󣬲����д���
//	int hostnum = atoi(input_data[0].c_str()); 
//	int vmnum = atoi(input_data[hostnum+1].c_str());
//	int days = atoi(input_data[hostnum+vmnum+2].c_str());       // ���������� 
//	int i;
//	// ��������������б��ʼ�� 
//	vector<HostState> Host_list;
//	vector<VMState> VM_list; 
//	// ���δ���ÿ������� 
//	int pos = hostnum + vmnum + 3;
//	int request_num = atoi(input_data[pos].c_str());	
//	vector<string>::const_iterator first = input_data.begin()+pos+1;
//	vector<string>::const_iterator second = first + request_num;
//	for(i=0;i<1;i++){
//		
////		// ȡ����i����������� 
//		vector<string> request_list(first, second);
//		vector<Request> Requests = Request_process(request_list, VM_list);
////		// ���� + ���� 
//		vector<Host_purchase> purchase_result = Purchase(Requests, Host_list, VMs, Hosts);
//////		printf("-----------------befor allocation------------------\n"); 
//////		for(int j=0;j<Host_list.size();j++){
//////			cout<< Host_list[j].hostID << "|||" << Host_list[j].cpu_A << "|||" << Host_list[j].mem_A << "|||" << Host_list[j].cpu_mem << endl;
//////		}
////		// ������ز��� 
//		cout << "(migration, 0)" << endl;		
//		allocate(Requests, Host_list, VM_list, VMs);
//		printf("-----------------host list------------------\n"); 
//		for(int j=0;j<Host_list.size();j++){
//			cout<< Host_list[j].hostID << "|||" << Host_list[j].cpu_A << "|||" << Host_list[j].mem_A << "|||" << Host_list[j].cpu_mem_A << "|||" << Host_list[j].cpu_B << "|||" << Host_list[j].mem_B << "|||" << Host_list[j].cpu_mem_B << endl;
//		}
//		cost += Totalcost(purchase_result, Host_list, Hosts); 
////		// ������ز��� 
//		pos = pos + request_num + 1;
//		request_num = atoi(input_data[pos].c_str());
//		first = second + 1;
//		second = first + request_num;
//	}	
//	cout << cost << endl;
} 
