// Cause a Blue Screen of Death from Usermode using the undocumented "NtRaiseHardError" function.
// http://undocumented.ntinternals.net/index.html?page=UserMode%2FUndocumented%20Functions%2FError%2FNtRaiseHardError.html
// This program WILL cause your Windows system to BSOD. Run with caution (or in a VM)

#include <iostream>
#include <Windows.h>
#include <winternl.h>

typedef NTSTATUS(NTAPI *pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);
typedef NTSTATUS(NTAPI *pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);

int main()
{
    BOOLEAN pB;
    ULONG eR;

    std::cout << "https://github.com/rico-vz/bsod-um" << std::endl;
    std::cout << "BSOD from Usermode using the undocumented \"NtRaiseHardError\" function." << std::endl;
    std::cout << "This program WILL cause your Windows system to BSOD. Run with caution (or in a VM)." << std::endl;
    std::cout << "Press any key to continue..." << std::endl;
    std::cin.get();
    std::cout << "Trying to raise a BSOD..." << std::endl;

    std::cout << "Attempting to locate RtlAdjustPrivilege" << std::endl;

    pdef_RtlAdjustPrivilege RtlAdjustPrivilege = (pdef_RtlAdjustPrivilege)GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlAdjustPrivilege");
    if (RtlAdjustPrivilege == NULL)
    {
        std::cout << "GetProcAddress for RtlAdjustPrivilege failed" << std::endl;
        return 1;
    }

    std::cout << "Attempting to locate NtRaiseHardError" << std::endl;

    pdef_NtRaiseHardError NtRaiseHardError = (pdef_NtRaiseHardError)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtRaiseHardError");
    if (NtRaiseHardError == NULL)
    {
        std::cout << "GetProcAddress for NtRaiseHardError failed" << std::endl;
        return 1;
    }

    std::cout << "Attempting to set privilege" << std::endl;

    if (RtlAdjustPrivilege(19, TRUE, FALSE, &pB) != 0)
    {
        std::cout << "Setting privilege using RtlAdjustPrivilege failed" << std::endl;
        return 1;
    }

    std::cout << "Goodbye" << std::endl;

    NtRaiseHardError(0xDECAFBAD, 0, 0, NULL, 6, &eR);
    return 0;
}  