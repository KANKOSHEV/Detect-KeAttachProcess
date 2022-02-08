#pragma once

/*

The offsets are given below, and the "ETHREAD_META" structure works from win 10 19041 to win 11.

*/

#define ImageFileName 0x5A8 // EPROCESS::ImageFileName
#define ActiveThreads 0x5F0 // EPROCESS::ActiveThreads
#define ThreadListHead 0x5E0 // EPROCESS::ThreadListHead
#define ActiveProcessLinks 0x448 // EPROCESS::ActiveProcessLinks

void ke_attach_detect()
{
	PEPROCESS sys_process = PsInitialSystemProcess;
	PEPROCESS cur_entry = sys_process;

	do
	{
		CHAR image_name[32];
		DWORD active_threads;

		RtlCopyMemory((PVOID)&image_name, (PVOID)((uintptr_t)cur_entry + ImageFileName), sizeof(image_name));
		RtlCopyMemory((PVOID)&active_threads, (PVOID)((uintptr_t)cur_entry + ActiveThreads), sizeof(active_threads));

		if (active_threads && cur_entry != sys_process)
		{
			LIST_ENTRY* thread_list_head = (LIST_ENTRY*)make_ptr(cur_entry, ThreadListHead);
			PLIST_ENTRY list = thread_list_head;

			while ((list = list->Flink) != thread_list_head)
			{
				PETHREAD_META entry = CONTAINING_RECORD(list, ETHREAD_META, ThreadListEntry);
				PEPROCESS attached_pe_proc = *(PEPROCESS*)((uintptr_t)entry + 0xB8);

				if (attached_pe_proc != cur_entry)
				{
					CHAR target_image_name[512];
					RtlCopyMemory((PVOID)(&target_image_name), (PVOID)((uintptr_t)attached_pe_proc + ImageFileName), sizeof(image_name));

					DbgPrintEx(0, 0, "%s attached -> %s\n", image_name, target_image_name);
				}
			}
		}

		PLIST_ENTRY list = (PLIST_ENTRY)((uintptr_t)(cur_entry) + ActiveProcessLinks);
		cur_entry = (PEPROCESS)((uintptr_t)list->Flink - ActiveProcessLinks);

	} while (cur_entry != sys_process);
}