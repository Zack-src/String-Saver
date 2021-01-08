#include "..\Link.h"

int ProcessPid;

std::string result;
void ProcessSave(std::string processName, bool is_svchost) {

	if (!is_svchost) ProcessPid = Process_Pid(processName + ".exe");
	else			 ProcessPid = Svchost_Pid(processName.c_str());

	if (ProcessPid != 0) {
		Save_Mem(ProcessPid, processName+".txt");
		result = "  -Save " + processName + " ended";
	}
	else {
		result = "  -" +processName + " Not Found";
	}
	puts(result.c_str());
	return;
}
