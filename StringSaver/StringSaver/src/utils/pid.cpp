#include "..\Link.h"

DWORD Svchost_Pid(const char* serviceName)
{
	const auto hScm = OpenSCManager(nullptr, nullptr, NULL);
	const auto hSc = OpenService(hScm, serviceName, SERVICE_QUERY_STATUS);
	SERVICE_STATUS_PROCESS ssp = {};
	DWORD bytesNeeded = 0;
	if (QueryServiceStatusEx(hSc, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&ssp), sizeof(ssp), &bytesNeeded) != 0) CloseServiceHandle(hSc);
	CloseServiceHandle(hScm);
	return ssp.dwProcessId;
}

DWORD Process_Pid(std::string name)
{
	PROCESSENTRY32 entry{};
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(hSnapshot, &entry);
	if (name.compare(entry.szExeFile) == 0)
		return entry.th32ProcessID;

	while (Process32Next(hSnapshot, &entry))
		if (name.compare(entry.szExeFile) == 0)
		{
			CloseHandle(hSnapshot);
			return entry.th32ProcessID;
		}


	CloseHandle(hSnapshot);
	return 0;
}