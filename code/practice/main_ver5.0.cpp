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
 	int hostID;        // ������ID(����˳��) 
 	string HostName;
 	int cpu_A;         // A�ڵ�ʣ��cpu 
 	int mem_A;         // A�ڵ�ʣ���ڴ� 
	int cpu_B;         // B�ڵ�ʣ��CPU 
	int mem_B;         // B�ڵ�ʣ���ڴ� 
	int vm_num;        // �������ϵ���������� 
	int energy_cost;   // ���������ܺ� 
	int realID;        // ������ID(���˳��) 
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
// �����������Ϣ�ṹ��
struct VM_allocate{
	int vmID;
	int hostID;
	int nodeflag;
}; 
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
�������ܣ���������Ϣ�ṹ���������򣬽�����������CPU+MEM�������� 
input:
	a: ������a�Ľṹ��
	b: ������b�Ľṹ��
output:
	�������ṹ���������� 
*/
bool compare(Host a, Host b){
	return (a.cpu+a.mem) < (b.cpu+b.mem);
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
�������ܣ���һ����������У�ִ�й���Ͳ�������������Ӧ��� 
*/
void Main_process(vector<Host> Hosts, vector<Request> Requests, vector<HostState> &Host_list, unordered_map<string, VM> VMs, vector<VMState> &VM_list, vector<Host_purchase> &purchase_result){
	int	id = Host_list.size();
	Host_purchase host_purchase;
	purchase_result.clear();
	vector<VM_allocate> allocate_result;
	VM_allocate vmallocate;
	HostState hoststate;
	VMState vmstate;
	VM thevm;
	int i,j,k;
	// ������������������Ӳ����޿ռ��ţ����������
	for(i=0;i<Requests.size();i++){
		// ִ��ɾ������ 
		if(Requests[i].type.compare("add")){
			string vmName;
			int vmID = Requests[i].vmID;
			int cpu;
			int mem;
			// ���·�����������Ϣ
			for(k=0;k<VM_list.size();k++){
				// ���б��е�K̨�������ID�뵱ǰ�����IDƥ�� 
				if(VM_list[k].vmID==vmID){
					//cout<<VM_list[k].vmID << "|||" <<vmID<<endl;
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
		else{
			// ��ȡ���������ȫ����Ϣ 
			string vmName = Requests[i].vmName;
			int vmID = Requests[i].vmID;
			VM thevm = VMs[vmName];          
			int cpu = thevm.cpu;
			int mem = thevm.mem; 
			int node_flag = VMs[Requests[i].vmName].node_flag;
			//cout << node_flag << endl;
			// ����˫�ڵ㲿�� 
			if(node_flag==1){
				//cout << "here2" <<endl; 
				// ���ȳ��Բ���
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
				// ��û���ҵ��ɲ���λ�ã����������, ������Ӧ�Ĺ�����Ϣ 
				// ���·�����������Ϣ��ֱ�ӽ��в��� 
				if(position==-1){
					// ��Ѱ�Һ��ʵķ�����,��λ��Ϊj 
					for(j=0;j<Hosts.size();j++){
						if(Hosts[j].cpu > 1.5*cpu && Hosts[j].mem > 1.5*mem){
							break;
						}
					}
					// ������뵽�������� 
					string hostname = Hosts[j].HostName;      // �빺��ķ���������
					// �ڹ����б���Ѱ�Ҹ÷�����
					host_purchase.HostName = hostname;
					for(k=0; k<purchase_result.size();k++){
						// ���ҵ��˸÷����� 
						if(!purchase_result[k].HostName.compare(hostname)){
							purchase_result[k].Hostnum++;
							break;
						}
					}
					// ��û���ҵ��÷����������½�������Ϣ 
					if(k==purchase_result.size()){
						host_purchase.HostName = hostname;
						host_purchase.Hostnum = 1;
						purchase_result.push_back(host_purchase);
					}
					// ���·�����������Ϣ
					hoststate.cpu_A = Hosts[j].cpu/2 - cpu/2;
					hoststate.cpu_B = Hosts[j].cpu/2 - cpu/2;
					hoststate.mem_A = Hosts[j].mem/2 - mem/2;
					hoststate.mem_B = Hosts[j].mem/2 - mem/2;
					hoststate.hostID = Host_list.size();
					hoststate.HostName = hostname;
					hoststate.vm_num = 1;
					hoststate.energy_cost = Hosts[j].energy_cost;
					Host_list.push_back(hoststate);
					vmstate.hostID = hoststate.hostID;
					vmstate.nodeFlag = 0;
					vmstate.vmID = Requests[i].vmID;
					vmstate.vmName = Requests[i].vmName;
					VM_list.push_back(vmstate); 
					vmallocate.hostID = vmstate.hostID;
					vmallocate.nodeflag = 0;
					vmallocate.vmID = vmstate.vmID;
					allocate_result.push_back(vmallocate);
				}
				// ���ҵ��˲���λ�ã�λ��Ϊposition����ֱ�ӽ��в���
				else{
					// �޸ķ�����������Ϣ
					Host_list[position].cpu_A -= cpu/2;
					Host_list[position].cpu_B -= cpu/2;
					Host_list[position].mem_A -= mem/2;
					Host_list[position].mem_B -= mem/2;
					Host_list[position].vm_num += 1;
					// �޸������������Ϣ
					vmstate.hostID = Host_list[position].hostID;
					vmstate.nodeFlag = 0;
					vmstate.vmID = Requests[i].vmID;
					vmstate.vmName = Requests[i].vmName;
					VM_list.push_back(vmstate);
					// ���沿����Ϣ 
					vmallocate.hostID = Host_list[position].hostID;
					vmallocate.nodeflag = 0;
					vmallocate.vmID = Requests[i].vmID;
					allocate_result.push_back(vmallocate);
				} 
			} 
			// ���ǵ��ڵ㲿�� 
			else{
				//cout << "here3" <<endl; 
				// ���ȳ��Բ��� 
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
				// ��û���ҵ��ܴ�ŵ�λ�ã���������� 
				if(position==-1){
					for(j=0;j<Hosts.size();j++){
						if(Hosts[j].cpu > 3*cpu && Hosts[j].mem > 3*mem){
							break;
						}
					} 
					// ������뵽�������� 
					string hostname = Hosts[j].HostName;      // �빺��ķ���������
					// �ڹ����б���Ѱ�Ҹ÷�����
					for(k=0; k<purchase_result.size();k++){
						// ���ҵ��˸÷����� 
						if(!purchase_result[k].HostName.compare(hostname)){
							purchase_result[k].Hostnum++;
							break;
						}
					}
					// ��û���ҵ��÷����������½�������Ϣ 
					if(k==purchase_result.size()){
						host_purchase.HostName = hostname;
						host_purchase.Hostnum = 1;
						purchase_result.push_back(host_purchase);
					}
					// ���·�����������Ϣ
					hoststate.cpu_A = Hosts[j].cpu/2 - cpu;
					hoststate.cpu_B = Hosts[j].cpu/2;
					hoststate.mem_A = Hosts[j].mem/2 - mem;
					hoststate.mem_B = Hosts[j].mem/2;
					hoststate.hostID = Host_list.size();
					hoststate.vm_num = 1;
					hoststate.HostName = hostname;
					hoststate.energy_cost = Hosts[j].energy_cost;
					Host_list.push_back(hoststate);
					vmstate.hostID = hoststate.hostID;
					vmstate.nodeFlag = 1;
					vmstate.vmID = Requests[i].vmID;
					vmstate.vmName = Requests[i].vmName;
					VM_list.push_back(vmstate); 
					vmallocate.hostID = vmstate.hostID;
					vmallocate.nodeflag = 1;
					vmallocate.vmID = vmstate.vmID;
					allocate_result.push_back(vmallocate);
				}
				// ���ҵ���λ�ã�����в��� 
				else{
					// �޸ķ�����������Ϣ
					// �޸������������Ϣ
					if(node==1){
						Host_list[position].cpu_A -= cpu;
						Host_list[position].mem_A -= mem;
						Host_list[position].vm_num += 1;
						vmstate.hostID = Host_list[position].hostID;
						vmstate.nodeFlag = 1;
						vmstate.vmID = Requests[i].vmID;
						vmstate.vmName = Requests[i].vmName;
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
						VM_list.push_back(vmstate);
					}
					// ���沿����Ϣ 
					vmallocate.hostID = Host_list[position].hostID;
					vmallocate.nodeflag = node;
					vmallocate.vmID = Requests[i].vmID;
					allocate_result.push_back(vmallocate);
				}
			}
		} 
	}
	// ����˳��id�ͷ�������ʵ����id��ת�� 
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
	// ��ӡ������Ϣ 
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
	cout << "(migration, 0)" << endl;	
	// ��ӡ������Ϣ
	for(j=0;j<allocate_result.size();j++){
		id = allocate_result[j].hostID;
		for(k=0;k<Host_list.size();k++){
			if(Host_list[k].hostID==id){
				allocate_result[j].hostID = Host_list[k].realID;
			}
		}
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
������ 
*/
int main(){	
	//�������ύ���� 
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
		test(Hosts, Day_request, Host_list, VMs, VM_list, purchase_result); 
	}
} 
