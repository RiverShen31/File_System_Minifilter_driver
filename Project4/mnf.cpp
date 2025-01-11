#include "common.h"
#include "mnf.h"
#include "Utils.h"

UNICODE_STRING BootVolumePrefix = RTL_CONSTANT_STRING(BOOT_VOLUME_NT_PATH);

FLT_PREOP_CALLBACK_STATUS CreatePreoperationCallback(
	PFLT_CALLBACK_DATA data,
	PCFLT_RELATED_OBJECTS fltObjects,
	PVOID* completionContext
) {
	UNREFERENCED_PARAMETER(completionContext);
	//UNREFERENCED_PARAMETER(fltObjects);

	ULONG createDisposition = (data->Iopb->Parameters.Create.Options & 0xFF);

	PFLT_FILE_NAME_INFORMATION fileNameInfo = NULL;
	NTSTATUS status;
	status = FltGetFileNameInformation(data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_ALWAYS_ALLOW_CACHE_LOOKUP, &fileNameInfo);
	if (NT_SUCCESS(status)) {
		if (fileNameInfo != NULL) {
			FltParseFileNameInformation(fileNameInfo);
			TRACKED_ITEM<TRACKED_ACTION>* item;
			switch (createDisposition) {

			case FILE_SUPERSEDE:
			case FILE_CREATE: {
				GenerateLog(&MinifilterData, fileNameInfo, ACTION::Create, &item);
				//DbgPrint("[LOG][CREATE] ++++++++++++++++++++++ A file is being created %ws\n", fileNameInfo->Name.Buffer);
				break;
			}
							
			case FILE_OPEN:
			case FILE_OVERWRITE: {
				GenerateLog(&MinifilterData, fileNameInfo, ACTION::Open, &item);
				//DbgPrint("[LOG][OPEN] ++++++++++++++++++++++ A file is being opened %ws\n", fileNameInfo->Name.Buffer);
				break;
			}

			case FILE_OPEN_IF:
			case FILE_OVERWRITE_IF: {
				// check if file has already existed
				// if yes, its an open operation
				NTSTATUS fileStatus = FltQueryInformationFile(
					fltObjects->Instance,
					fltObjects->FileObject,
					&fileNameInfo,
					sizeof(FILE_BASIC_INFORMATION),
					FileBasicInformation,
					NULL
				);

				if (NT_SUCCESS(fileStatus)) {
					// this is an open action
					GenerateLog(&MinifilterData, fileNameInfo, ACTION::Open, &item);
					//DbgPrint("[LOG][OPEN] ++++++++++++++++++++++ A file is being opened %ws\n", fileNameInfo->Name.Buffer);

				}
				else {
					// this is a create action
					GenerateLog(&MinifilterData, fileNameInfo, ACTION::Create, &item);
					//DbgPrint("[LOG][CREATE] ++++++++++++++++++++++ A file is being created %ws\n", fileNameInfo->Name.Buffer);

				}
				break;
			}

			default: {
				item = NULL;
				//DbgPrint("=============== UNKNOWN ACTION\n");
				break;
			}
			}

			if (item != NULL) {
				AddTrackedLog(MinifilterData.LoggingItems, item);
			}
		}
	}

	if (fileNameInfo != NULL) {
		FltReleaseFileNameInformation(fileNameInfo);
		//DbgPrint("Test");
	}

	return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_PREOP_CALLBACK_STATUS ReadPreoperationCallback(
	PFLT_CALLBACK_DATA data,
	PCFLT_RELATED_OBJECTS fltObjects,
	PVOID* completionContext
) {
	UNREFERENCED_PARAMETER(completionContext);
	UNREFERENCED_PARAMETER(fltObjects);

	PFLT_FILE_NAME_INFORMATION fileNameInfo;
	NTSTATUS status;
	status = FltGetFileNameInformation(data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_ALWAYS_ALLOW_CACHE_LOOKUP, &fileNameInfo);
	if (NT_SUCCESS(status)) {
		FltParseFileNameInformation(fileNameInfo);
		DbgPrint("=======[LOG][Read] A file is being created: %wZ%wZ\n", &fileNameInfo->Extension, &fileNameInfo->FinalComponent);
	}
	else {
		FltReleaseFileNameInformation(fileNameInfo);
		DbgPrint("=======Cannot FltGetFileNameInformation at ReadPreoperationCallback\n");
	}

	return FLT_PREOP_SUCCESS_NO_CALLBACK;

}

FLT_PREOP_CALLBACK_STATUS WritePreoperationCallback(
	PFLT_CALLBACK_DATA data,
	PCFLT_RELATED_OBJECTS fltObjects,
	PVOID* completionContext
) {
	UNREFERENCED_PARAMETER(completionContext);
	UNREFERENCED_PARAMETER(fltObjects);

	PFLT_FILE_NAME_INFORMATION fileNameInfo;
	NTSTATUS status;
	status = FltGetFileNameInformation(data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_ALWAYS_ALLOW_CACHE_LOOKUP, &fileNameInfo);
	if (NT_SUCCESS(status)) {
		FltParseFileNameInformation(fileNameInfo);
		DbgPrint("=======[LOG][Write] A file is being created: %wZ%wZ\n", &fileNameInfo->Extension, &fileNameInfo->FinalComponent);
	}
	else {
		FltReleaseFileNameInformation(fileNameInfo);
		DbgPrint("=======Cannot FltGetFileNameInformation at WritePreoperationCallback\n");
	}

	return FLT_PREOP_SUCCESS_NO_CALLBACK;

}

FLT_PREOP_CALLBACK_STATUS SetInformationPreoperationCallback(
	PFLT_CALLBACK_DATA data,
	PCFLT_RELATED_OBJECTS fltObjects,
	PVOID* completionContext
) {
	UNREFERENCED_PARAMETER(completionContext);
	UNREFERENCED_PARAMETER(fltObjects);

	PFLT_FILE_NAME_INFORMATION fileNameInfo;
	NTSTATUS status;
	status = FltGetFileNameInformation(data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_ALWAYS_ALLOW_CACHE_LOOKUP, &fileNameInfo);
	if (NT_SUCCESS(status)) {
		FltParseFileNameInformation(fileNameInfo);
		DbgPrint("=======[LOG][Delete] A file is being created: %wZ%wZ\n", &fileNameInfo->Extension, &fileNameInfo->FinalComponent);
	}
	else {
		FltReleaseFileNameInformation(fileNameInfo);
		DbgPrint("=======Cannot FltGetFileNameInformation at SetInformationPreoperationCallback\n");
	}

	return FLT_PREOP_SUCCESS_NO_CALLBACK;

}

NTSTATUS InstanceSetupCallback(
	PCFLT_RELATED_OBJECTS fltObjects,
	FLT_INSTANCE_SETUP_FLAGS flags,
	DEVICE_TYPE volumeDeviceType,
	FLT_FILESYSTEM_TYPE fileSystemType
) {
	UNREFERENCED_PARAMETER(fltObjects);
	UNREFERENCED_PARAMETER(flags);
	UNREFERENCED_PARAMETER(volumeDeviceType);
	UNREFERENCED_PARAMETER(fileSystemType);

	return STATUS_SUCCESS;
}

NTSTATUS FilterUnload(
	FLT_FILTER_UNLOAD_FLAGS flags
) {
	UNREFERENCED_PARAMETER(flags);

	return STATUS_SUCCESS;
}

MINIFILTER_DATA MinifilterData;

const FLT_OPERATION_REGISTRATION Callbacks[] = {			//khởi tạo callback 
	{
		IRP_MJ_CREATE,
		0,
		CreatePreoperationCallback,
		NULL
	},
	//{
	//	IRP_MJ_READ,
	//	0,
	//	ReadPreoperationCallback,
	//	NULL
	//},
	//{
	//	IRP_MJ_WRITE,
	//	0,
	//	WritePreoperationCallback,
	//	NULL
	//},
	//{
	//	IRP_MJ_SET_INFORMATION,				//chính là DELETE
	//	0,
	//	SetInformationPreoperationCallback,
	//	NULL
	//},
	//{
	//	IRP_MJ_VOLUME_MOUNT,					//IRP của usb
	//	0,
	//	VolumeMountPreoperationCallback,
	//	NULL
	//},
	{IRP_MJ_OPERATION_END }
};

const FLT_REGISTRATION FilterRegistration = {
	sizeof(FLT_REGISTRATION),			
	FLT_REGISTRATION_VERSION,
	0,
	NULL,
	Callbacks,					// Mảng callback cho hoạt động file system
	FilterUnload,						// Callback gỡ bỏ filter
	InstanceSetupCallback,		// Callback khi gắn filter vào volume
	NULL,
	NULL,
	NULL
};