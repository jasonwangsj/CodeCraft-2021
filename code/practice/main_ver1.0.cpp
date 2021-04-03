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
	VMs:  
*/
vector<VM> GetVMs(vector<string> input_data){
	int hostnum = atoi(input_data[0].c_str());
	int vmnum = atoi(input_data[hostnum+1].c_str());
	vector<string> input_split;
	vector<VM> VMs;
	for(int i=hostnum+2;i<=hostnum+2+vmnum-1;i++){
		input_split = split(input_data[i],',');
		VM vm;
		vm.vmName = input_split[0].substr(1);
		vm.cpu = atoi(input_split[1].c_str());
		vm.mem = atoi(input_split[2].c_str());
		vm.node_flag = atoi(input_split[3].c_str());
		VMs.push_back(vm);
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
vector<Host_purchase> Purchase(vector<Request> Requests, vector<HostState> &Host_list, vector<VM> VMs, vector<Host> Hosts){
	vector<Host_purchase> purchase_result;
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
			for(int j=0;j<VMs.size();j++){
				if(!VMs[j].vmName.compare(name)){
					cpu_request += VMs[j].cpu;
					mem_request += VMs[j].mem;
					break;
				}	
			}
		}
	}
	// ����ϵͳ��ʣ���CPU���ڴ�����
	int cpu_rest=0;
	int mem_rest=0;
	/*
	for(int i=0;i<Host_list.size();i++){
		cpu_rest += Host_list[i].cpu_A + Host_list[i].cpu_B;
		mem_rest += Host_list[i].mem_A + Host_list[i].mem_B;
	} 
	*/
	// ���㹺��ķ�������������ǰ�����н������Լ۱���ߵķ�����
	int cpu_demand = 2*cpu_request - cpu_rest;
	int mem_demand = 2*mem_request - mem_rest;
	// cpu���ڴ������Ҫ 
	if(cpu_demand<0 && mem_demand<0){
		Host_purchase host_purchase;
		host_purchase.HostName = "0";
		host_purchase.Hostnum = 0;
		purchase_result.push_back(host_purchase);
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
			temp.hostID = Host_list.size()+i;
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
	int num=0;
	for(int i=0;i<purchase_result.size();i++){
		num += purchase_result[i].Hostnum;
	}
	cout << "(purchase, ";
	cout << num;
	cout << ")" <<endl; 
	if(num>0){
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
void allocate(vector<Request> Requests, vector<HostState> &Host_list, vector<VMState> &VM_list, vector<VM> VMs){
	// ������������ 
	for(int i=0;i<Requests.size();i++){
		// ����i������Ϊ������� 
		if(!Requests[i].type.compare("add")){
			int flag = 0;                       // flag=0��ʾ��i������û�з����ȥ 
			// ������������Ʋ����������Ϣ
			int j=0;
			for(j=0;j<VMs.size();j++){ 
				if(!Requests[i].vmName.compare(VMs[j].vmName)){
					break; 
				}
			} 
			// �����������Ϣ�������������
			// ���ǵ��ڵ㲿��
			if(VMs[j].node_flag==0){
				// ���ζԸ������������ж� 
				for(int k=0;k<Host_list.size();k++){
					// ����k̨��������A�ڵ�ɴ��¸������,��ֱ�Ӵ洢 
					if(Host_list[k].cpu_A>=VMs[j].cpu && Host_list[k].mem_A>=VMs[j].mem){
						// �����޸ķ�������Ϣ 
						Host_list[k].cpu_A -= VMs[j].cpu;
						Host_list[k].mem_A -= VMs[j].mem;
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
						break; 
					}
					else{
						// ��k̨��������A�ڵ�治�¸������,��B�ڵ�ɴ���,��ֱ�Ӵ洢 
						if(Host_list[k].cpu_B>=VMs[j].cpu && Host_list[k].mem_B>=VMs[j].mem){
							// �����޸ķ�������Ϣ 
							Host_list[k].cpu_B -= VMs[j].cpu;
							Host_list[k].mem_B -= VMs[j].mem;
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
					if(Host_list[k].cpu_A>=VMs[j].cpu/2 && Host_list[k].mem_A>=VMs[j].mem/2 && Host_list[k].cpu_B>=VMs[j].cpu/2 && Host_list[k].mem_B>=VMs[j].mem/2){
						// �����޸ķ�������Ϣ 
						Host_list[k].cpu_A -= VMs[j].cpu/2;
						Host_list[k].mem_A -= VMs[j].mem/2;
						Host_list[k].cpu_B -= VMs[j].cpu/2;
						Host_list[k].mem_B -= VMs[j].mem/2;
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
						break; 
					}
				}
			}
			if(flag==0){
				cout<<"ERROR!!!!A Request Has Not Been Allocated!!!"<<endl;
			}
			
		}
		// ����ɾ������ 
		else{
			// ������������Ʋ����������Ϣ
			int j=0;
			for(j=0;j<VMs.size();j++){ 
				if(!Requests[i].vmName.compare(VMs[j].vmName)){
					break; 
				}
			} 
			// ���·�����������Ϣ
			int k=0;
			for(int k=0;k<VM_list.size();k++){
				// ���б��е�K̨�������ID�뵱ǰ�����IDƥ�� 
				if(VM_list[k].vmID==Requests[i].vmID){
					break; 
				} 
			} 
			int hostid = VM_list[k].hostID;
			if(VM_list[k].nodeFlag==0){
				Host_list[hostid].cpu_A += VMs[j].cpu/2;
				Host_list[hostid].mem_A += VMs[j].mem/2;
				Host_list[hostid].cpu_B += VMs[j].cpu/2;
				Host_list[hostid].mem_B += VMs[j].mem/2;
				Host_list[hostid].vm_num -= 1;
			}
			else{
				if(VM_list[k].nodeFlag==1){
					Host_list[hostid].cpu_A += VMs[j].cpu/2;
					Host_list[hostid].mem_A += VMs[j].mem/2;
					Host_list[hostid].vm_num -= 1;
				}
				else{
					Host_list[hostid].cpu_B += VMs[j].cpu/2;
					Host_list[hostid].mem_B += VMs[j].mem/2;
					Host_list[hostid].vm_num -= 1;
				}
			}
			VM_list.erase(VM_list.begin()+k+1);	
		}
	}
}


int main(){
	// ���ļ��ж�ȡ���� 
	vector<string> input_data;
	input_data = ReadInput();     //input_data:string���͵�vector��vector��ÿһ�����ļ��е�һ������(string����)   
	// �����������ݣ��õ���������Ϣ(�����Լ۱�����)���������Ϣ
	vector<Host> Hosts;
	vector<VM> VMs; 
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
		// ���� + ���� 
		vector<Host_purchase> purchase_result = Purchase(Requests, Host_list, VMs, Hosts);
		cout << "(migration, 0)"<<endl;
		allocate(Requests, Host_list, VM_list, VMs);
		// ������ز��� 
		pos = pos + request_num + 1;
		request_num = atoi(input_data[pos].c_str());
		first = second + 1;
		second = first + request_num;
	}
} 
