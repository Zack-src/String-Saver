#include "Link.h"

BOOL GetPrivilege(std::string perm)
{
	const char* permchar = perm.c_str();
	HANDLE tokenhandle;
	LUID permissionidentifier;
	TOKEN_PRIVILEGES tokenpriv{};
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tokenhandle))
	{
		if (LookupPrivilegeValue(NULL, permchar, &permissionidentifier))
		{
			tokenpriv.PrivilegeCount = 1;
			tokenpriv.Privileges[0].Luid = permissionidentifier;
			tokenpriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (AdjustTokenPrivileges(tokenhandle, false, &tokenpriv, sizeof(tokenpriv), NULL, NULL)) { return true; }
			else { return false; }
		}
		else { return false; }
	}
	else { return false; }
	CloseHandle(tokenhandle);
}

int main() {

	GetPrivilege(SE_SECURITY_NAME);
	GetPrivilege(SE_DEBUG_NAME);

	ShowWindow(GetConsoleWindow(), 0);

	ConfigGUI();

	return 0;
}