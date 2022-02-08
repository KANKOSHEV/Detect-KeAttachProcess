#include "library/globals.h"

VOID NTAPI entry_thread(PVOID)
{
	while (1)
	{
		ke_attach_detect();
	}
}

NTSTATUS entry_point(PVOID, PVOID)
{
	HANDLE h_thread = 0;
	NTSTATUS status = PsCreateSystemThread(&h_thread, 0x10000000L, 0, 0, 0, entry_thread, 0);

	if (!NT_SUCCESS(status))
		return status;

	ZwClose(h_thread);

	return STATUS_SUCCESS;	
}
