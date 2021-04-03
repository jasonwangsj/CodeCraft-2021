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
	int cpumemsum;
	float restratio;
	int energy_cost;   // ���������ܺ� 
	int realID;        // ������ID(���˳��) 
};
// ���������״̬�ṹ�� 
struct VMState{
 	int vmID;       // �����ID
 	string vmName;     // ��������� 
	int hostID;     // ������ID  
 	int nodeFlag;      // 0��˫�ڵ㣬1��A�ڵ㣬2��B�ڵ� 
 	int change;       // 0�����id��1������id 
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
// Ǩ����Ϣ�ṹ��
struct VM_migration{
	string vmName;
	int vmID;
	int hostid_source;
	int hostid_target;
	int node_source;
	int node_target;
}; 
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
�������ܣ���������Ϣ�ṹ���������򣬽�����������CPU+MEM�������� 
input:
	a: ������a�Ľṹ��
	b: ������b�Ľṹ��
output:
	�������ṹ���������� 
*/
bool compare(Host a, Host b){
	return a.price < b.price;
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
// ������ʵid�������򣬷����޸���Ϣ 
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
�������ܣ�Ǩ�ƺ��� 
*/
int migration(vector<HostState> &Host_list, unordered_map<string, VM> VMs, vector<VMState> &VM_list, vector<VM_migration> &migration_result){
	// ������ز���
	int mignum_max = floor(VM_list.size()*5/1000);
	int mignum = 0;                              // Ǩ��������ƴ������Ѿ�Ǩ�ƴ���
	float treshhold = 0.3;                       // ѡ��Ǩ�Ʒ���������ֵ 
	int i,j,k;
	migration_result.clear();
	sort(Host_list.begin(),Host_list.end(), compare3); 
	// ����ѡ��Ǩ�Ƶ�Դ��������Ŀ�������
	vector<HostState> Host_source, Host_target, Host_temp;
	for(i=0;i<Host_list.size();i++){
		if(Host_list[i].restratio>=treshhold){
			Host_temp.push_back(Host_list[i]);
		}
		else{
			Host_target.push_back(Host_list[i]);
		} 
	}
	for(i=0;i<floor(Host_temp.size()/2);i++){
		Host_source.push_back(Host_temp[i]);
	}
	for(i=floor(Host_temp.size()/2);i<Host_temp.size();i++){
		Host_target.push_back(Host_temp[i]); 
	}
	sort(Host_list.begin(),Host_list.end(),compare1); 
//	cout << "the num of source hosts is:" << Host_source.size() << endl;
//	cout << "the num of target hosts is:" << Host_target.size() << endl;
//	cout <<"---------------------------------------------"<<endl;
	// ����Դ�������е�ÿ̨�����
	for(i=0;i<Host_source.size();i++){
		// Դ������������Ϊi������ʵidΪrealid
		int hostid_source = Host_source[i].realID;
		for(j=0;j<VM_list.size();j++){
			// ��j̨���������ʵIDΪ hostid_source�ķ������� 
			if(VM_list[j].hostID == hostid_source){
				// ���Ǹ�������Ƿ����Ǩ��
				// ����ȡ��������Ļ�����Ϣ
				string vmName = VM_list[j].vmName;
				int vmID = VM_list[j].vmID; 
				int node_source = VM_list[j].nodeFlag;
				int cpu = VMs[vmName].cpu;
				int mem = VMs[vmName].mem;
				// ��ʼ����Ǩ�Ƹ��������������Ŀ����������Һ���λ�� 
				// ��Ϊ˫�ڵ㲿������� 
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
					// �������ܴ��λ�������Ǩ�ƣ�Ŀ���������Host_target�ĵ�k�У���û��λ����ִ���κβ��� 
					if(pos!=-1){
						if(mignum >= mignum_max){
							return 1;
						} 
//						cout<<"-----------before migration-------------"<<endl;
//						for(int q=0;q<Host_list.size();q++){
//							cout << Host_list[q].HostName << "|||" << Host_list[q].hostID << "|||" << Host_list[q].realID <<"|||"<<Host_list[q].cpu_A<<"|||"<<Host_list[q].mem_A <<"|||"<<Host_list[q].cpu_B<<"|||"<<Host_list[q].mem_B<<endl;
//						}
						// ����Ǩ����Ϣ
						VM_migration vm_migration;
						vm_migration.hostid_source = hostid_source;
						vm_migration.hostid_target = Host_target[pos].realID;
						vm_migration.node_source = 0;
						vm_migration.node_target = 0;
						vm_migration.vmID = vmID;
						vm_migration.vmName = vmName;
						migration_result.push_back(vm_migration);
						// �޸�Ŀ����������ݴ���Ϣ����һ��Ǩ�������Ѱ�ҿռ�ʱ��Ҫʹ��
						Host_target[pos].cpu_A -= cpu/2;
						Host_target[pos].mem_A -= mem/2;
						Host_target[pos].cpu_B -= cpu/2;
						Host_target[pos].mem_B -= mem/2;
						// �޸���ʵ��������Ϣ�б���������Ϣ
						// �޸�Դ�����������Ϣ
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
//						cout<<"-----------after migration-------------"<<endl;
//						for(int q=0;q<Host_list.size();q++){
//							cout << Host_list[q].HostName << "|||" << Host_list[q].hostID << "|||" << Host_list[q].realID <<"|||"<<Host_list[q].cpu_A<<"|||"<<Host_list[q].mem_A <<"|||"<<Host_list[q].cpu_B<<"|||"<<Host_list[q].mem_B<<endl;
//						}
						// �޸������������Ϣ
						VM_list[j].hostID = Host_target[pos].realID;
						VM_list[j].nodeFlag = 0;
						// �����ж�
						mignum++;

  					}
				}	
				// ��Ϊ���ڵ㲿������� 
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
						// ����Ǩ����Ϣ
//						cout<<"-----------before migration-------------"<<endl;
//						for(int q=0;q<Host_list.size();q++){
//							cout << Host_list[q].HostName << "|||" << Host_list[q].hostID << "|||" << Host_list[q].realID <<"|||"<<Host_list[q].cpu_A<<"|||"<<Host_list[q].mem_A <<"|||"<<Host_list[q].cpu_B<<"|||"<<Host_list[q].mem_B<<endl;
//						}
						VM_migration vm_migration;
						vm_migration.hostid_source = hostid_source;
						vm_migration.hostid_target = Host_target[pos].realID;
						vm_migration.node_source = node_source;
						vm_migration.node_target = node_target;
						vm_migration.vmID = vmID;
						vm_migration.vmName = vmName;
						migration_result.push_back(vm_migration);
						// �޸�Ŀ����������ݴ���Ϣ
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
//						cout<<"-----------after migration-------------"<<endl;
//						for(int q=0;q<Host_list.size();q++){
//							cout << Host_list[q].HostName << "|||" << Host_list[q].hostID << "|||" << Host_list[q].realID <<"|||"<<Host_list[q].cpu_A<<"|||"<<Host_list[q].mem_A <<"|||"<<Host_list[q].cpu_B<<"|||"<<Host_list[q].mem_B<<endl;
//						}
						// �޸������������Ϣ
						VM_list[j].hostID = Host_target[pos].realID;
						VM_list[j].nodeFlag = node_target;
						// �����ж� 
						mignum++;
						
					}
				}	
			}
		}
	} 
}





/*
�������ܣ���һ����������У�ִ�й���Ͳ�������������Ӧ��� 
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
			Host_list[hostid].restratio = (Host_list[hostid].cpu_A + Host_list[hostid].cpu_B + Host_list[hostid].mem_A + Host_list[hostid].mem_B)*1.0/Host_list[hostid].cpumemsum;
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
				// ���ҵ��˲���λ�ã�λ��Ϊposition����ֱ�ӽ��в���
				else{
					// �޸ķ�����������Ϣ
					Host_list[position].cpu_A -= cpu/2;
					Host_list[position].cpu_B -= cpu/2;
					Host_list[position].mem_A -= mem/2;
					Host_list[position].mem_B -= mem/2;
					Host_list[position].vm_num += 1;
					Host_list[position].restratio = (Host_list[position].cpu_A + Host_list[position].cpu_B + Host_list[position].mem_A +Host_list[position].mem_B)*1.0/Host_list[position].cpumemsum;
					// �޸������������Ϣ
					vmstate.hostID = Host_list[position].hostID;
					vmstate.nodeFlag = 0;
					vmstate.vmID = Requests[i].vmID;
					vmstate.vmName = Requests[i].vmName;
					vmstate.change = 0;
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
	sort(Host_list.begin(), Host_list.end(), compare2);
	// �����������Ϣ��ID���� 
	for(j=0;j<VM_list.size();j++){
		if(VM_list[j].change==0){
			int orid = VM_list[j].hostID;
			VM_list[j].hostID = Host_list[orid].realID;
			VM_list[j].change = 1;
		}		
//		for(k=0;k<Host_list.size();k++){
//			if(Host_list[k].hostID == orid){
//				VM_list[j].hostID = Host_list[k].realID;
//			}
//		} 
	}
//	cout << "------------------VM_list--------------------" << endl;
//	for(int q=0;q<VM_list.size();q++){
//		cout << VM_list[q].vmName << "|||" << VM_list[q].vmID << "|||" << VM_list[q].change << VM_list[q].hostID <<"|||" << VM_list[q].nodeFlag <<endl; 
//	}
//	for(j=0;j<Host_list.size();j++){
//		cout << Host_list[j].HostName <<"|||"<< Host_list[j].hostID << "|||" << Host_list[j].realID << endl;
//	} 
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
	// ��ӡ������Ϣ
	for(j=0;j<allocate_result.size();j++){
		id = allocate_result[j].hostID;
		allocate_result[j].hostID = Host_list[id].realID;
//		for(k=0;k<Host_list.size();k++){
//			if(Host_list[k].hostID==id){
//				allocate_result[j].hostID = Host_list[k].realID;
//			}
//		}
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
/*
������ 
*/
int main(){	
	//�������ύ���� 
//	vector<Host> Hosts;
//	unordered_map<string, VM> VMs;
//	vector<VMState> VM_list;
//	vector<Host_purchase> purchase_result;
//	vector<HostState> Host_list; 
//	vector<vector<Request>> Requests;
//	Input_Process(Hosts, VMs, Requests, VM_list);
//	int i,j;
//	for(i=0;i<Requests.size();i++){
//		vector<Request> Day_request = Requests[i];
//		Main_process(Hosts, Day_request, Host_list, VMs, VM_list, purchase_result); 
//	}

	long long cost = 0; 
	vector<string> input_data;
	vector<Host_purchase> purchase_result;
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
		Main_process(Hosts, Requests, Host_list, VMs, VM_list, purchase_result);
		cost += Totalcost(purchase_result, Host_list, Hosts); 
//		// ������ز��� 
		pos = pos + request_num + 1;
		request_num = atoi(input_data[pos].c_str());
		first = second + 1;
		second = first + request_num;
	}	
	cout << cost << endl;
} 
