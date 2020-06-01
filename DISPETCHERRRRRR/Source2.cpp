#include <windows.h>
#include <stdlib.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <new>

using namespace std;

int ProcessPriorityTake(HANDLE* hproc, string* str) {
	if (!str) {
		cout << "Error ProcessPriorityTake 1" << endl;
		return 1;
	}
	if(!(*hproc)){
		*str = "System Process";
		return 2;
		}
	switch (GetPriorityClass(*hproc))
	{
	case HIGH_PRIORITY_CLASS:
		*str = "HIGHT PRIORITY";
		break;
	case NORMAL_PRIORITY_CLASS:
		*str = "NORMAL PRIORITY";
		break;
	case IDLE_PRIORITY_CLASS:
		*str = "LOW PRIORITY";
		break;
	case REALTIME_PRIORITY_CLASS:
		*str = "RILETIME PRIORITY(HIGHTEST POSSIBL)";
		break;
	case 0:
		cout << "Error ProcessPriorityTake 3" << endl;
		return 3;
	default:
		*str = "STANDART";
		break;
	}
	return 0;
}

int PidTake(DWORD* pid) {

	if (!pid) {
		cout << "Error PidTake 1" << endl;
		return 1;
	}

	cout << "ENTER PID" << endl;
	if (cout.fail()) {
		cout.clear();
		cout << "Error PidTake 2" << endl;
		return 2;
	}

	string* intermedStr = new(nothrow) string("a");
	if (intermedStr == nullptr) {
		cout << "Error PidTake 3" << endl;
		return 3;
	}
	
	char* end = new(nothrow) char('a');
	if (end == nullptr) {
		cout << "Error PidTake 4" << endl;
		return 4;
	}
	if (!getline(cin, *intermedStr, '\n')) {
		cin.clear();
		cout << "Error PidTake 5" << endl;
		delete end, intermedStr;
		return 5;
	}
	*pid = strtol((*intermedStr).c_str(), &end, 10);
	if (*pid == 0 || *pid == LONG_MAX) {
		cout << "Error PidTake 6" << endl;
		delete intermedStr, end;
		return 6;
	}
	delete intermedStr, end;
	intermedStr, end = nullptr;

	return 0;
}

int KillProc() {

	DWORD* pid = new(nothrow) DWORD('A');
	if (pid == nullptr) {
		cout << "Error KillProc 1" << endl;
		delete pid;
		return 1;
	}
	
	if (PidTake(pid) != 0) {
		cout << "Error KillProc PidTake 2" << endl;
		delete pid;
		return 2;
	}

	HANDLE deletproc = OpenProcess(
		PROCESS_ALL_ACCESS,
		FALSE,
		*pid
	);

	if (deletproc == INVALID_HANDLE_VALUE) {
		cout << "Error KillProc INCORRECT PID";
		delete pid;
		return 3;
	}

	if (TerminateProcess(deletproc, 0)) { 
		cout << "PROCESS SUCCESSFULLY DELETED" << endl;
		if (cout.fail()) {
			cout.clear();
			cout << "Error KillProc 4" << endl;
			delete pid;
			return 4;
		}
	}
	else { 
		cout << "Error KillProc 5";
		delete pid;
		if (!CloseHandle(deletproc)) {
			cout << "Error KillProc 6" << endl;
			return 6;
		}
		return 5;
	}
	if (!CloseHandle(deletproc)) {
		cout << "Error KillProc 7" << endl;
		delete pid;
		return 7;
	}

	delete pid;
	return 0;
}

int ProcInfo() {
	DWORD pdwHandleCount = (DWORD)('A');
	int k;

	DWORD* pid = new(nothrow) DWORD('A');
	if (pid == nullptr) {
		cout << "Error ProcInfo 1" << endl;
		delete pid;
		return 1;
	}

	if (PidTake(pid) != 0) {
		cout << "Eroror ProcInfo PidTake 2" << endl;
		delete pid;
		return 2;
	}

	HANDLE procinfo = OpenProcess(
		PROCESS_ALL_ACCESS,
		FALSE,
		*pid
	);

	if (procinfo == INVALID_HANDLE_VALUE) {
		cout << "INCORRECT PID ProcInfo" << endl;
		delete pid;
		return 3;
	}


	string* str = new(nothrow) string("a");
	if (str == nullptr) {
		cout << "Error ProcInfo 4" << endl;
		delete str, pid;
		return 4;
	}

	k = ProcessPriorityTake(&procinfo, str);
	if (k == 3 || k == 1) {
		cout << "Error ProcInfo 5" << endl;
		delete str, pid;
		if (!CloseHandle(procinfo)) {
			cout << "Error ProcInfo 6" << endl;
			return 6;
		}
		return 5;
	}
	else if (k == 2) {
		cout << "Couldn't take proc info because " << *str << endl;
		delete str, pid;
		if (!CloseHandle(procinfo)) {
			cout << "Error ProcInfo 8" << endl;
			return 8;
		}
		return 7;
	}
	else {
		if (!GetProcessHandleCount(procinfo, &pdwHandleCount)) {
			cout << "Error ProcInfo 9" << endl;
			delete str, pid;
			return 9;
		}
		cout << "PID - " << *pid << "  HandleCount - " << pdwHandleCount << "  Priority - " << *str << endl;
		if (cout.fail()) {
			cout.clear();
			cout << "Error ProcInfo 10" << endl;
			delete str, pid;
			if (!CloseHandle(procinfo)) {
				cout << "Error ProcInfo 11" << endl;
				return 11;
			}
			return 10;
		}
	}

	delete str, pid;
	str, pid = nullptr;
	if (!CloseHandle(procinfo)) {
		cout << "Error ProcInfo 12" << endl;
		return 12;
	}
	return 0;
}

int ListOfProcesses() {

	HANDLE* procSnap = new(nothrow) HANDLE;
	if (procSnap == nullptr) {
		cout << "Error 1" << endl;
		return 1;
	}
	*procSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (procSnap == INVALID_HANDLE_VALUE) {
		delete procSnap;
		cout << "Error 2" << endl;
		return 2;
	}

	PROCESSENTRY32* pe = new(nothrow) PROCESSENTRY32;
	if (pe == nullptr) {
		cout << "Error 3" << endl;
		return 3;
	}
	(*pe).dwSize = sizeof(PROCESSENTRY32);


	int i;
	if (Process32First(*procSnap, pe)) {
		cout << "PROC ID   PROC NAME   PROC PRIORITY" << endl;
		if (cout.fail()) {
			cout.clear();
			delete procSnap, pe;
			cout << "Error ListofProcesses 4" << endl;
			return 4;
		}

		string* strProcPrior = new(nothrow) string("a");
		if (strProcPrior == nullptr) {
			cout << "Error ListofProcesses 5" << endl;
			delete strProcPrior, procSnap, pe;
			return 5;
		}

		do {
			if (!pe->th32ProcessID) continue;
			HANDLE hproc = OpenProcess(
				PROCESS_ALL_ACCESS,
				FALSE,
				pe->th32ProcessID
			);
			if (hproc == INVALID_HANDLE_VALUE) {
				cout << "Error ListofProcesses 6" << endl;
				delete strProcPrior, procSnap, pe;
				return 6;
			}

			i = ProcessPriorityTake(&hproc, strProcPrior);
			if (i != 2 && i != 0) {
				cout << "Error ListofProcesses 7" << endl;
				delete strProcPrior, procSnap, pe;
				if (!CloseHandle(hproc)) {
					cout << "Error ListofProcesses 8" << endl;
					return 8;
				}
				return 7;
			}

			cout << pe->th32ProcessID << "  " << pe->szExeFile << "  " << *strProcPrior << endl;
			if (cout.fail()) {
				cout.clear();
				cout << "Error ListofProcesses 9" << endl;
				delete strProcPrior, procSnap, pe;
				if (!CloseHandle(hproc)) {
					cout << "Error ListofProcesses 10" << endl;
					return 10;
				}
				return 9;
			}

		} while (Process32Next(*procSnap, pe));

		if (!CloseHandle(*procSnap)) {
			cout << "Error ListofProcesses 11" << endl;
			delete strProcPrior, procSnap, pe;
			return 11;
		}

		delete strProcPrior, procSnap, pe;
		strProcPrior, procSnap, pe = nullptr;
		return 0;
	}
	else {
		delete pe, procSnap;
		if (!CloseHandle(*procSnap)) {
			cout << "Error ListofProcesses 12" << endl;
			return 12;
		}
		cout << "COULDN'T TAKE PROCESS LIST"; 
		return 13; 
	}
}

int Menu() {

	string* ASK = new(nothrow) string("a");
	if (ASK == nullptr) {
		cout << "Error Menu 1" << endl;
		return 1;
	}

	cout << "Menu:\n1-Kill process by PID\n2-Process Info by PID\n3-List of processes\n4-Exit\nInput number you choice" << endl;
	if (cout.fail()) {
		cout.clear();
		cout << "Error Menu 2" << endl;
		delete ASK;
		return 2;
	}

	if (!getline(cin, *ASK, '\n')) {
		cin.clear();
		delete ASK;
		cout << "Error Menu 3" << endl;
		return 3;
	}
	if (*ASK == "1") {
		delete ASK;
		if (KillProc() != 0) {
			cout << "Error Menu KillProc 4" << endl;
			return 4;
		}
		if (Menu() != 0) {
			cout << "Error Menu 5" << endl;
			return 5;
		}
		return 0;
	}
	else if (*ASK == "2") {
		delete ASK;
		if (ProcInfo() != 0) {
			cout << "Error Menu ProcInfo 6" << endl;
			return 6;
		}
		if (Menu() != 0) {
			cout << "Error Menu 7" << endl;
			return 7;
		}
		return 0;
	}
	else if (*ASK == "3") {
		delete ASK;
		if (ListOfProcesses() != 0) {
			cout << "Error Menu ProcInfo 8" << endl;
			return 8;
		}
		if (Menu() != 0) {
			cout << "Error Menu 9" << endl;
			return 9;
		}
		return 0;
	}
	else if (*ASK == "4") {
		delete ASK;
		return 0;
	}
	else {
		delete ASK;
		cout << "Error 10" << endl;
		return 10;
	}
}


int main() {

	if (ListOfProcesses() != 0) {
		cout << "Error 1" << endl;
		return 1;
	}

	if (Menu() != 0) {
		cout << "Error 2" << endl;
		return 2;
	}


	if(system("pause") != -1){
		cout << "Error 3" << endl;
		return 3;
	}

	return 0;
}



