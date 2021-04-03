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
 	float performance_cost;  // �������Լ۱� 
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
	int flag;          // ����������ָ�� 
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
�������ܣ���ȡ��׼����
output:
	 input_data:string���͵�vector��vector��ÿһ�����ļ��е�һ������(string����) 
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
	return a.performance_cost>b.performance_cost;
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
		host.performance_cost = host.cpu*host.mem*1.0/host.price;
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
		host.performance_cost = host.cpu*host.mem*1.0/host.price;
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
	int cpu_request = 0;
	int mem_request = 0;
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
	// ����ϵͳ��ʣ���CPU���ڴ�����
	int cpu_rest=0;
	int mem_rest=0;
	
	for(int i=0;i<Host_list.size();i++){
		cpu_rest += Host_list[i].cpu_A + Host_list[i].cpu_B;
		mem_rest += Host_list[i].mem_A + Host_list[i].mem_B;
	} 
	
	// ���㹺��ķ�������������ǰ�����н������Լ۱���ߵķ�����
	int cpu_demand = 2*cpu_request - cpu_rest;
	int mem_demand = 2*mem_request - mem_rest;
	// cpu���ڴ������Ҫ 
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
			///// ע�⣺�˴���hostID�㷨�Ǵӿ��б�ʼ��� 
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
// �����Ǩ��
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
	int totalnum = 0;
	// ������������ 
	for(int i=0;i<Requests.size();i++){
		string name = Requests[i].vmName;
		VM thevm = VMs[name];
		// ����i������Ϊ������� 
		if(!Requests[i].type.compare("add")){
			int flag = 0;                       // flag=0��ʾ��i������û�з����ȥ 
			// �����������Ϣ�������������
			// ���ǵ��ڵ㲿��
			if(VMs[Requests[i].vmName].node_flag==0){
				// ���ζԸ������������ж� 
				for(int k=0;k<Host_list.size();k++){
					
					// ����k̨��������A�ڵ�ɴ��¸������,��ֱ�Ӵ洢 
					if(Host_list[k].cpu_A>=thevm.cpu && Host_list[k].mem_A>=thevm.mem){
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
						// ����ɹ�
						flag = 1;
						// ���������
						
						cout<< "(";
						cout<< temp.hostID;
						cout<< ", A)"<<endl;
						
						totalnum++;
						break; 
						
					}
					else{
						// ��k̨��������A�ڵ�治�¸������,��B�ڵ�ɴ���,��ֱ�Ӵ洢 
						if(Host_list[k].cpu_B>=thevm.cpu && Host_list[k].mem_B>=thevm.mem){
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
							
							cout<< "(";
							cout<< temp.hostID;
							cout<< ", B)"<<endl;
							
							totalnum++;
							
							break; 
							
						}
					}
				}
			} 
			//����˫�ڵ㲿�� 
			else{
				// ���ζԸ������������ж� 
				for(int k=0;k<Host_list.size();k++){
					// A�ڵ��B�ڵ�����㣬����з��� 
					if(Host_list[k].cpu_A>=thevm.cpu/2 && Host_list[k].mem_A>=thevm.mem/2 && Host_list[k].cpu_B>=thevm.cpu/2 && Host_list[k].mem_B>=thevm.mem/2){
						// �����޸ķ�������Ϣ 
						Host_list[k].cpu_A -= thevm.cpu/2;
						Host_list[k].mem_A -= thevm.mem/2;
						Host_list[k].cpu_B -= thevm.cpu/2;
						Host_list[k].mem_B -= thevm.mem/2;
						Host_list[k].vm_num += 1;
						// �����޸��������Ϣ
						VMState temp;
						temp.hostID = Host_list[k].hostID;
						temp.vmID = Requests[i].vmID;
						temp.vmName = Requests[i].vmName;
						temp.nodeFlag = 0;
						VM_list.push_back(temp); 
						// ����ɹ�
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
//			VM_list[k].vmID = 0;
//			VM_list[k].vmName = "empty";
//			VM_list[k].hostID = 0;
//			VM_list[k].nodeFlag = -1;
		}
	}
//	cout << totalnum << endl;
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
//		vector<Host_purchase> purchase_result = Purchase(Day_request, Host_list, VMs, Hosts);
//		int mignum = 0; 
//		cout << "(migration, " << mignum << ")" <<endl;
//		allocate(Day_request, Host_list, VM_list, VMs);	
//	}
	

	//   ���ز��Դ��� 
	// ���ļ��ж�ȡ���� 
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
	for(i=0;i<10;i++){
		// ȡ����i����������� 
		vector<string> request_list(first, second);
		vector<Request> Requests = Request_process(request_list, VM_list);
		// ���� + ���� 
		vector<Host_purchase> purchase_result = Purchase(Requests, Host_list, VMs, Hosts);
		// ������ز��� 
//		for(int j=0;j<Host_list.size();j++){
//			cout << Host_list[j].hostID << endl;
//		} 
		cout << "(migration, ";
		printf("%d)\n",0);
		
		allocate(Requests, Host_list, VM_list, VMs); 
//		for(int j=0;j<Host_list.size();j++){
//			cout << Host_list[j].hostID << endl;
//		} 
		// ������ز��� 
		pos = pos + request_num + 1;
		request_num = atoi(input_data[pos].c_str());
		first = second + 1;
		second = first + request_num;
	}	
//	for(i=0;i<VM_list.size();i++){
//		cout << VM_list[i].vmID << "|||" <<VM_list[i].vmName << "|||" << VM_list[i].hostID << "|||" << VM_list[i].nodeFlag << endl;
//	}
} 
