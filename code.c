#include "header.h"

void throwError(FILE *errFile, CONST CHAR *msg, DWORD dwErrcode);
void showData(LPNETRESOURCE lpnrLocal, DWORD dwNesting);

BOOL WINAPI enumerateResources(LPNETRESOURCE lpnr, FILE *errFile, DWORD dwNesting)
{
	
	DWORD dwErrLog, dwEntries = -1, dwBuffer = 16384, i;
    HANDLE hEnum;
    LPNETRESOURCE lpnrLocal;
	//begin enumeration	   //all network resources //all recources //enum all //null //handle to resource
	dwErrLog = WNetOpenEnum(RESOURCE_GLOBALNET, RESOURCETYPE_ANY, 0, lpnr, &hEnum);
    if (dwErrLog != NO_ERROR) {
		throwError(errFile, "WNetOpenEnum failed with error %d\n", dwErrLog);
        return FALSE;
    }
	//allocate memory for resource
    lpnrLocal = (LPNETRESOURCE) GlobalAlloc(GPTR, dwBuffer);
    if (lpnrLocal == NULL) {
        throwError(errFile, "WNetOpenEnum failed with error %d\n", dwErrLog);
        return FALSE;
    }
	// if success, continue enumerarion
    do {
        ZeroMemory(lpnrLocal, dwBuffer);
		dwErrLog = WNetEnumResource(hEnum, &dwEntries, lpnrLocal, &dwBuffer);
        if (dwErrLog == NO_ERROR) {
            for (i = 0; i < dwEntries; i++) {
				//show current resource
				showData(&lpnrLocal[i], dwNesting);
                if (RESOURCEUSAGE_CONTAINER == (lpnrLocal[i].dwUsage & RESOURCEUSAGE_CONTAINER)) {
					//if the netresource structure is a container resource, call recursively.
                    if (!enumerateResources(&lpnrLocal[i], errFile, dwNesting + 1)) {
						throwError(errFile, "enumerateResources function returned FALSE\n", 0);
                    }
                }
            }
        }
        else if (dwErrLog != ERROR_NO_MORE_ITEMS) {
			throwError(errFile, "WNetEnumResource failed with error %d\n", dwErrLog);
            break;
        }
    } while (dwErrLog != ERROR_NO_MORE_ITEMS);
	//free memory and call CloseEnum to finish enumeration
    GlobalFree((HGLOBAL)lpnrLocal);
	dwErrLog = WNetCloseEnum(hEnum);

    if (dwErrLog != NO_ERROR) {
		throwError(errFile, "WNetCloseEnum failed with error %d\n", dwErrLog);
        return FALSE;
    }

    return TRUE;
}

void throwError(FILE *errFile, CONST CHAR *msg, DWORD dwErrcode)
{
    if (errFile) {
		if(!dwErrcode)
			fprintf(errFile, msg);
		else
			fprintf(errFile, msg, dwErrcode);
    }
}

void showData(LPNETRESOURCE lpnrLocal, DWORD dwNesting)
{
	printf("\n==============================\n\n");
	printf("Nesting %d\n", dwNesting);
    DWORD i;

	printf("%S ", lpnrLocal->lpRemoteName);
    if (lpnrLocal->lpComment) {
        printf("  Comment: (%S) ", lpnrLocal->lpComment);
    }

    printf("  Resource type:   ");
    switch (lpnrLocal->dwType) {
    case (RESOURCETYPE_ANY):
        printf("other resource");
        break;
    case (RESOURCETYPE_DISK):
        printf("shared disk volume");
        break;
    case (RESOURCETYPE_PRINT):
        printf("shared printer");
        break;
    default:
        printf("unknown %X", lpnrLocal->dwDisplayType);
        break;
    }
	printf("\n");

    printf("  Display type:    ");
    switch (lpnrLocal->dwDisplayType) {
    case (RESOURCEDISPLAYTYPE_GENERIC):
        printf("generic");
        break;
    case (RESOURCEDISPLAYTYPE_DOMAIN):
        printf("domain");
        break;
    case (RESOURCEDISPLAYTYPE_SERVER):
        printf("server");
        break;
    case (RESOURCEDISPLAYTYPE_SHARE):
        printf("share");
        break;
    case (RESOURCEDISPLAYTYPE_FILE):
        printf("file");
        break;
    case (RESOURCEDISPLAYTYPE_GROUP):
        printf("group");
        break;
    case (RESOURCEDISPLAYTYPE_NETWORK):
        printf("network");
        break;
	case(RESOURCEDISPLAYTYPE_ROOT):
		printf("logical root for the entire network");
		break;
	case(RESOURCEDISPLAYTYPE_SHAREADMIN):
		printf("administrative share");
		break;
	case(RESOURCEDISPLAYTYPE_DIRECTORY):
		printf("directory");
		break;
	case(RESOURCEDISPLAYTYPE_TREE):
		printf("tree");
		break;
	case(RESOURCEDISPLAYTYPE_NDSCONTAINER):
		printf("Netware Directory Service container");
		break;
    default:
        printf("unknown %X", lpnrLocal->dwDisplayType);
        break;
    }
    printf("\n");

	printf("  Usage:           ");
	if (RESOURCEUSAGE_CONNECTABLE & lpnrLocal->dwUsage)
		printf("connectable resource");
	else if (RESOURCEUSAGE_CONTAINER & lpnrLocal->dwUsage)
		printf("container resource");
	else if (RESOURCEUSAGE_NOLOCALDEVICE & lpnrLocal->dwUsage)
		printf("not a local device");
	else if (RESOURCEUSAGE_SIBLING & lpnrLocal->dwUsage)
		printf("sibling");
	else if (RESOURCEUSAGE_ATTACHED & lpnrLocal->dwUsage)
		printf("attach-specific");
	else
		printf("undefined");
	printf("\n");
	printf("  Provider:        %s\n", lpnrLocal->lpProvider);
}
