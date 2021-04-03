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
bool Compare(Host a, Host b){
	return a.cpu*a.mem*1.0/a.price > b.cpu*b.mem*1.0/b.price;
}

vector<Host_purchase> Purchase(vector<Request> Requests, vector<HostState> &Host_list, unordered_map<string, VM> VMs, vector<Host> Hosts, int flag){
	vector<Host_purchase> purchase_result;
	purchase_result.clear();
	Host_purchase host_purchase;
	// ������Ԥ����
	if(flag==0){
		/*
		// �����������Լ۱� 
		sort(Hosts.begin(),Hosts.end(),Compare);
		host_purchase.HostName = Hosts[0].HostName;
		host_purchase.Hostnum = 3000;
		purchase_result.push_back(host_purchase);
		for(int j=0;j<3000;j++){
			HostState temp;
			///// ע�⣺�˴���hostID�㷨�Ǵӿ��б�ʼ��� 
			temp.hostID = Host_list.size();
			temp.cpu_A = Hosts[0].cpu/2;
			temp.cpu_B = Hosts[0].cpu/2;
			temp.mem_A = Hosts[0].mem/2;
			temp.mem_B = Hosts[0].mem/2;
			temp.cpu_mem_A = temp.cpu_A*1.0/temp.mem_A;
			temp.cpu_mem_B = temp.cpu_mem_A;
			temp.cpu_mem = temp.cpu_mem_A;
			temp.vm_num = 0;
			temp.energy_cost = Hosts[2].energy_cost;
			Host_list.push_back(temp);
		}
		*/
	}
	// �������������� 
	// ����������cpu�ڴ������ 
	sort(Hosts.begin(), Hosts.end(), compare);
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
	long long cpu_demand = 1.0*cpu_request;
	long long mem_demand = 1.0*mem_request;
	// cpu���ڴ������Ҫ 
	if(cpu_rest>=165*cpu_request && mem_rest>=165*mem_request){
	}
	// �������41̨ 
	else{
		int num1 = ceil(cpu_demand*1.0 / Hosts[i].cpu);
		int num2 = ceil(mem_demand*1.0 / Hosts[i].mem);
		int num = max(num1,num2)+4;
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
		/*
		sort(Hosts.begin(),Hosts.end(),Compare);
		
		*/
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
�������ܣ���˫�ڵ��������ѡ��÷���ߵķ������������в��� 
*/
void vm_allocate(int cpu, int mem, int vmID, string vmName, vector<HostState> &Host_list, vector<VMState> &VM_list){
	float cpu_mem = cpu*1.0/mem;          // ��ǰ�������CPU�ڴ��
	// ���������CPU�ڴ�ȴ��ڵ���1 
	if(cpu_mem>=1){
		float score_max = -99999; 
		int position = -1; 
		int i;
		for(i=0;i<Host_list.size();i++){
			// ���÷������ܴ�����������ټ�����÷� 
			if(Host_list[i].cpu_A>=cpu/2 && Host_list[i].cpu_B>=cpu/2 && Host_list[i].mem_A>=mem/2 && Host_list[i].mem_B>=mem/2){
				float score1,score2;
				if(Host_list[i].cpu_mem_A>=1){
					score1 = 1/(0.0001+abs(cpu_mem - Host_list[i].cpu_mem_A));
					score1 += 1/Host_list[i].mem_A;
				} 
				else{
					score1 = -abs(cpu_mem - Host_list[i].cpu_mem_A);
				}
				if(Host_list[i].cpu_mem_B>=1){
					score2 = 1/(0.0001+abs(cpu_mem - Host_list[i].cpu_mem_B));
					score2 += 1/Host_list[i].mem_B;
				}
				else{
					score2 = -abs(cpu_mem - Host_list[i].cpu_mem_B);
				}
				float score = (score1 + score2)/2;
				if(score >= score_max){
					score_max = score;
					position = i;
				}
			}
		}
		// �������б���û�пɴ�ŵķ����� 
		if(position==-1){
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
		}
		// �ڵ÷���ߵķ������Ͻ��д�� 
		else{
			// �����޸ķ�������Ϣ 
			Host_list[position].cpu_A -= cpu/2;
			Host_list[position].mem_A -= mem/2;
			Host_list[position].cpu_B -= cpu/2;
			Host_list[position].mem_B -= mem/2;
			Host_list[position].vm_num += 1;
			if(Host_list[position].mem_A==0){
				Host_list[position].cpu_mem_A = 0;
			}
			else{
				Host_list[position].cpu_mem_A = Host_list[position].cpu_A*1.0/Host_list[position].mem_A;
			}
			if(Host_list[position].mem_B==0){
				Host_list[position].cpu_mem_B = 0;
			}
			else{
				Host_list[position].cpu_mem_B = Host_list[position].cpu_B*1.0/Host_list[position].mem_B;
			}
			//Host_list[position].cpu_mem = (Host_list[position].cpu_mem_A + Host_list[position].cpu_mem_B)/2;
			// �޸��������Ϣ
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
	else{
		float score_max = -99999; 
		int position = -1; 
		int i;
		for(i=0;i<Host_list.size();i++){
			// ���÷������ܴ�����������ټ�����÷� 
			if(Host_list[i].cpu_A>=cpu/2 && Host_list[i].cpu_B>=cpu/2 && Host_list[i].mem_A>=mem/2 && Host_list[i].mem_B>=mem/2){
				float score1,score2;
				if(Host_list[i].cpu_mem_A<1){
					score1 = 1/(0.0001+abs(cpu_mem -  Host_list[i].cpu_mem_A));
					score1 += 1/Host_list[i].mem_A;
				} 
				else{
					score1 = -abs(cpu_mem -  Host_list[i].cpu_mem_A);
				}
				if(Host_list[i].cpu_mem_B<1){
					score2 = 1/(0.0001+abs(cpu_mem -  Host_list[i].cpu_mem_B));
					score2 += 1/Host_list[i].mem_B;
				}
				else{
					score2 = -abs(cpu_mem -  Host_list[i].cpu_mem_B);
				}
				float score = (score1 + score2)/2;
				if(score >= score_max){
					score_max = score;
					position = i;
				}
			}
		}
		if(position==-1){
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
		}
		// �ڵ÷���ߵķ������Ͻ��д�� 
		else{
			// �����޸ķ�������Ϣ 
			Host_list[position].cpu_A -= cpu/2;
			Host_list[position].mem_A -= mem/2;
			Host_list[position].cpu_B -= cpu/2;
			Host_list[position].mem_B -= mem/2;
			Host_list[position].vm_num += 1;
			if(Host_list[position].mem_A==0){
				Host_list[position].cpu_mem_A = 0;
			}
			else{
				Host_list[position].cpu_mem_A = Host_list[position].cpu_A*1.0/Host_list[position].mem_A;
			}
			if(Host_list[position].mem_B==0){
				Host_list[position].cpu_mem_B = 0;
			}
			else{
				Host_list[position].cpu_mem_B = Host_list[position].cpu_B*1.0/Host_list[position].mem_B;
			}
			//Host_list[position].cpu_mem = (Host_list[position].cpu_mem_A + Host_list[position].cpu_mem_B)/2;
			// �޸��������Ϣ
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
}
void vm_allocate1(int cpu, int mem, int vmID, string vmName, vector<HostState> &Host_list, vector<VMState> &VM_list){
	float cpu_mem = cpu*1.0/mem;
	// �����еĽڵ���д�� 
	if(cpu_mem>=1){
		float score_max = -99999; 
		int position = -1; 
		float score;
		int flag;          // 1��ʾA�ڵ㣬2��ʾB�ڵ� 
		int i;
		for(i=0;i<Host_list.size();i++){
			// ��A�ڵ�ɴ������������д�� 
			if(Host_list[i].cpu_A>=cpu && Host_list[i].mem_A>=mem){
				 if(Host_list[i].cpu_mem_A>=1){
				 	score = 1/(0.0001+abs(cpu_mem - Host_list[i].cpu_mem_A));
					score += 1/Host_list[i].mem_A;
					score += abs(Host_list[i].cpu_A + Host_list[i].mem_A - Host_list[i].cpu_B - Host_list[i].mem_B); 
				 }
				 else{
				 	score = -abs(cpu_mem - Host_list[i].cpu_mem_A);
				 	score += abs(Host_list[i].cpu_A + Host_list[i].mem_A - Host_list[i].cpu_B - Host_list[i].mem_B); 
				 }
				 if(score >= score_max){
					score_max = score;
					flag = 1; 
					position = i;
				 }
			}
			// ��B�ڵ�ɴ�����Ҳ��B�ڵ���д�� 
			if(Host_list[i].cpu_B>=cpu && Host_list[i].mem_B>=mem){
				if(Host_list[i].cpu_mem_B>=1){
				 	score = 1/(0.0001+abs(cpu_mem - Host_list[i].cpu_mem_B));
					score += 1/Host_list[i].mem_B;
					score += abs(Host_list[i].cpu_A + Host_list[i].mem_A - Host_list[i].cpu_B - Host_list[i].mem_B); 
					if(Host_list[i].cpu_mem_A<1){
						score += 1;
					} 
				 }
				 else{
				 	score = -abs(cpu_mem - Host_list[i].cpu_mem_B);
				 	score += abs(Host_list[i].cpu_A + Host_list[i].mem_A - Host_list[i].cpu_B - Host_list[i].mem_B); 
				 	if(Host_list[i].cpu_mem_A>=1){
						score += 1;
					} 
				 } 
				 if(score >= score_max){
					score_max = score;
					flag = 2; 
					position = i;
				 }
			}
		} 
		if(position==-1){
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
		}
		else{
			if(flag==1){
				Host_list[position].cpu_A -= cpu;
				Host_list[position].mem_A -= mem;
				Host_list[position].vm_num += 1;
				if(Host_list[position].mem_A==0){
					Host_list[position].cpu_mem_A = 0;
				}
				else{
					Host_list[position].cpu_mem_A = Host_list[position].cpu_A*1.0/Host_list[position].mem_A;
				}
				VMState temp;
				temp.hostID = Host_list[position].hostID;
				temp.vmID = vmID;
				temp.vmName = vmName;
				temp.nodeFlag = 1;
				VM_list.push_back(temp); 
				// ����ɹ�							
				cout<< "(" << temp.hostID << ", A)" << endl;
			} 
			else{
				Host_list[position].cpu_B -= cpu;
				Host_list[position].mem_B -= mem;
				Host_list[position].vm_num += 1;
				if(Host_list[position].mem_B==0){
					Host_list[position].cpu_mem_B = 0;
				}
				else{
					Host_list[position].cpu_mem_B = Host_list[position].cpu_B*1.0/Host_list[position].mem_B;
				}
				VMState temp;
				temp.hostID = Host_list[position].hostID;
				temp.vmID = vmID;
				temp.vmName = vmName;
				temp.nodeFlag = 0;
				VM_list.push_back(temp); 
				// ����ɹ�							
				cout<< "(" << temp.hostID << ", B)" << endl;
			}
			
		}
	}
	else{
		float score_max = -99999; 
		int position = -1; 
		float score;
		int flag;          // 1��ʾA�ڵ㣬2��ʾB�ڵ� 
		int i;
		for(i=0;i<Host_list.size();i++){
			// ��A�ڵ�ɴ������������д�� 
			if(Host_list[i].cpu_A>=cpu && Host_list[i].mem_A>=mem){
				 if(Host_list[i].cpu_mem_A<1){
				 	score = 1/(0.0001+abs(cpu_mem - Host_list[i].cpu_mem_A));
					score += 1/Host_list[i].mem_A;
					score += abs(Host_list[i].cpu_A + Host_list[i].mem_A - Host_list[i].cpu_B - Host_list[i].mem_B); 
					if(Host_list[i].cpu_mem_B>=1){
						score += 1;
					}
				 }
				 else{
				 	score = -abs(cpu_mem - Host_list[i].cpu_mem_A);
				 	score += abs(Host_list[i].cpu_A + Host_list[i].mem_A - Host_list[i].cpu_B - Host_list[i].mem_B); 
				 	if(Host_list[i].cpu_mem_B<1){
						score += 1;
					}
				 }
				 if(score >= score_max){
					score_max = score;
					flag = 1; 
					position = i;
				 }
			}
			// ��B�ڵ�ɴ�����Ҳ��B�ڵ���д�� 
			if(Host_list[i].cpu_B>=cpu && Host_list[i].mem_B>=mem){
				if(Host_list[i].cpu_mem_B<1){
				 	score = 1/(0.0001+abs(cpu_mem - Host_list[i].cpu_mem_B));
					score += 1/Host_list[i].mem_B;
					score += abs(Host_list[i].cpu_A + Host_list[i].mem_A - Host_list[i].cpu_B - Host_list[i].mem_B); 
				 }
				 else{
				 	score = -abs(cpu_mem - Host_list[i].cpu_mem_B);
				 	score += abs(Host_list[i].cpu_A + Host_list[i].mem_A - Host_list[i].cpu_B - Host_list[i].mem_B); 
				 } 
				 if(score >= score_max){
					score_max = score;
					flag = 2; 
					position = i;
				 }
			}
		} 
		if(position==-1){
			cout << cpu << "|||" <<mem << endl; 
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
			cout<< "A VM Has Not Been Allocated!!!!" << endl;
		}
		else{
			if(flag==1){
				Host_list[position].cpu_A -= cpu;
				Host_list[position].mem_A -= mem;
				Host_list[position].vm_num += 1;
				if(Host_list[position].mem_A==0){
					Host_list[position].cpu_mem_A = 0;
				}
				else{
					Host_list[position].cpu_mem_A = Host_list[position].cpu_A*1.0/Host_list[position].mem_A;
				}
				VMState temp;
				temp.hostID = Host_list[position].hostID;
				temp.vmID = vmID;
				temp.vmName = vmName;
				temp.nodeFlag = 1;
				VM_list.push_back(temp); 
				// ����ɹ�							
				cout<< "(" << temp.hostID << ", A)" << endl;
			} 
			else{
				Host_list[position].cpu_B -= cpu;
				Host_list[position].mem_B -= mem;
				Host_list[position].vm_num += 1;
				if(Host_list[position].mem_B==0){
					Host_list[position].cpu_mem_B = 0;
				}
				else{
					Host_list[position].cpu_mem_B = Host_list[position].cpu_B*1.0/Host_list[position].mem_B;
				}
				VMState temp;
				temp.hostID = Host_list[position].hostID;
				temp.vmID = vmID;
				temp.vmName = vmName;
				temp.nodeFlag = 0;
				VM_list.push_back(temp); 
				// ����ɹ�							
				cout<< "(" << temp.hostID << ", B)" << endl;
			}
			
		}
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
		string vmName = Requests[i].vmName;
		int vmID = Requests[i].vmID;
		VM thevm = VMs[vmName];          
		int cpu = thevm.cpu;
		int mem = thevm.mem;   
		// ����i������Ϊ������� 
		if(!Requests[i].type.compare("add")){
			// ���ǵ��ڵ㲿��
			if(VMs[Requests[i].vmName].node_flag==0){
				vm_allocate1(cpu, mem, vmID, vmName, Host_list, VM_list);
			}
			// ����˫�ڵ㲿������cpu�ڴ�ȶԷ������������� 
			else{
				vm_allocate(cpu, mem, vmID, vmName, Host_list, VM_list);
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
				Host_list[hostid].cpu_mem_A = Host_list[hostid].cpu_A*1.0/Host_list[hostid].mem_A;
				Host_list[hostid].cpu_mem_B = Host_list[hostid].cpu_B*1.0/Host_list[hostid].mem_B;
				Host_list[hostid].cpu_mem = (Host_list[hostid].cpu_mem_A + Host_list[hostid].cpu_mem_B)/2;
				Host_list[hostid].vm_num -= 1;
			}
			else{
				if(VM_list[k].nodeFlag==1){
					Host_list[hostid].cpu_A += thevm.cpu/2;
					Host_list[hostid].mem_A += thevm.mem/2;
					Host_list[hostid].cpu_mem_A = Host_list[hostid].cpu_A*1.0/Host_list[hostid].mem_A;
					Host_list[hostid].cpu_mem = (Host_list[hostid].cpu_mem_A + Host_list[hostid].cpu_mem_B)/2;
					Host_list[hostid].vm_num -= 1;
				}
				else{
					Host_list[hostid].cpu_B += thevm.cpu/2;
					Host_list[hostid].mem_B += thevm.mem/2;
					Host_list[hostid].cpu_mem_B = Host_list[hostid].cpu_B*1.0/Host_list[hostid].mem_B;
					Host_list[hostid].cpu_mem = (Host_list[hostid].cpu_mem_A + Host_list[hostid].cpu_mem_B)/2;
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
		vector<Host_purchase> purchase_result = Purchase(Day_request, Host_list, VMs, Hosts,i);
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
////	for(int i=0;i<Hosts.size();i++){
////		cout << i << "|||" << Hosts[i].HostName << endl;
////	} 
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
//	for(i=0;i<days;i++){
//		
////		// ȡ����i����������� 
//		vector<string> request_list(first, second);
//		vector<Request> Requests = Request_process(request_list, VM_list);
////		// ���� + ���� 
//		vector<Host_purchase> purchase_result = Purchase(Requests, Host_list, VMs, Hosts, i);
////		printf("-----------------befor allocation------------------\n"); 
////		for(int j=0;j<Host_list.size();j++){
////			cout<< Host_list[j].hostID << "|||" << Host_list[j].cpu_A << "|||" << Host_list[j].mem_A << "|||" << Host_list[j].cpu_mem << endl;
////		}
////		// ������ز��� 
//		cout << "(migration, 0)" << endl;		
//		allocate(Requests, Host_list, VM_list, VMs);
////		printf("-----------------host list------------------\n"); 
////		for(int j=0;j<Host_list.size();j++){
////			cout<< Host_list[j].hostID << "|||" << Host_list[j].cpu_A << "|||" << Host_list[j].mem_A << "|||" << Host_list[j].cpu_mem_A << "|||" << Host_list[j].cpu_B << "|||" << Host_list[j].mem_B << "|||" << Host_list[j].cpu_mem_B << endl;
////		}
//		cost += Totalcost(purchase_result, Host_list, Hosts); 
////		// ������ز��� 
//		pos = pos + request_num + 1;
//		request_num = atoi(input_data[pos].c_str());
//		first = second + 1;
//		second = first + request_num;
//	}	
//	cout << cost << endl;
}
