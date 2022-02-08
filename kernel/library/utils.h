#pragma once

UNICODE_STRING ansi_to_unicode(const char* str)
{
	UNICODE_STRING unicode;
	ANSI_STRING ansi_str;

	RtlInitAnsiString(&ansi_str, str);
	RtlAnsiStringToUnicodeString(&unicode, &ansi_str, TRUE);

	return unicode;
}

PVOID get_kernel_proc_address(const char* system_routine_name)
{
	UNICODE_STRING name;
	ANSI_STRING ansi_str;

	RtlInitAnsiString(&ansi_str, system_routine_name);
	RtlAnsiStringToUnicodeString(&name, &ansi_str, TRUE);

	return MmGetSystemRoutineAddress(&name);
}

PVOID get_module_base(const char* module_name, ULONG* size_module)
{
	PLIST_ENTRY ps_loaded_module_list = PsLoadedModuleList;
	if (!ps_loaded_module_list)
		return (PVOID)NULL;

	UNICODE_STRING name = ansi_to_unicode(module_name);
	for (PLIST_ENTRY link = ps_loaded_module_list; link != ps_loaded_module_list->Blink; link = link->Flink)
	{
		PLDR_DATA_TABLE_ENTRY entry = CONTAINING_RECORD(link, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList);

		if (RtlEqualUnicodeString((PCUNICODE_STRING)&entry->BaseDllName, (PCUNICODE_STRING)&name, TRUE))
		{
			if (size_module != 0)
				*size_module = entry->SizeOfImage;

			return (PVOID)entry->DllBase;
		}
	}

	return (PVOID)NULL;
}

PVOID get_system_base_export(const char* module_name, LPCSTR routine_name)
{
	PVOID image_base = get_module_base(module_name, 0);
	if (!image_base)
		return 0;

	return RtlFindExportedRoutineByName(image_base, routine_name);
}
