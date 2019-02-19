#include "header.h"

int main()
{
	LPNETRESOURCE lpnr = NULL;
    if (!enumerateResources(lpnr, NULL, 0)) {
        printf("Call to enumerateResources function failed");
    }

    printf("\n");
    system("pause");
    return 0;
}