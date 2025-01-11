#include "Utils.h"

VOID GenerateLog(
	_In_ MINIFILTER_DATA *MinifilterData,
	_In_ PFLT_FILE_NAME_INFORMATION fileNameInfo,
	_In_ ACTION action,
	_Out_ TRACKED_ITEM<TRACKED_ACTION>** item
) {
	UNREFERENCED_PARAMETER(MinifilterData);

	PLARGE_INTEGER time = nullptr;
	KeQuerySystemTime(&time);

	ULONG size = sizeof(TRACKED_ITEM<TRACKED_ACTION>) + fileNameInfo->Name.Length;
	do {
		*item = (TRACKED_ITEM<TRACKED_ACTION>*)ExAllocatePool2(POOL_FLAG_NON_PAGED, size, '4P4M');
	} while (*item == NULL);

	ULONG offset = 0;
	auto& actionCreate = (*item)->Data;
	actionCreate.CanonicalFilePathLength = fileNameInfo->Name.Length;
	actionCreate.CanonicalFilePathOffset = sizeof(TRACKED_ITEM<TRACKED_ACTION>) + offset;
	RtlCopyMemory((PCHAR)&actionCreate + actionCreate.CanonicalFilePathOffset, fileNameInfo->Name.Buffer, fileNameInfo->Name.Length);

	actionCreate.Time = time;
	actionCreate.Action = action;
	actionCreate.size = size;
}

VOID AddTrackedLog(
	PLIST_TRACKED_ITEM list,
	TRACKED_ITEM<TRACKED_ACTION>* action
) {
	AutoLock<FastMutex> lock(list->mtx);
	if (list->ItemsCount > MAX_ITEMS_COUNT) {
		auto head = RemoveHeadList(&list->ItemsHead);
		list->ItemsCount -= 1;
		auto item = CONTAINING_RECORD(head, TRACKED_ITEM<TRACKED_ACTION>, Entry);
		ExFreePool(item);
	}
	InsertTailList(&list->ItemsHead, &action->Entry);
	list->ItemsCount += 1;
}