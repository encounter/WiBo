#include "common.h"

namespace advapi32 {
	unsigned int WIN_FUNC RegOpenKeyExA(void *hKey, const char *lpSubKey, unsigned int ulOptions, void *samDesired, void **phkResult) {
		printf("RegOpenKeyExA(key=%p, subkey=%s, ...)\n", hKey, lpSubKey);
		return 1; // screw them for now
	}
}

void *wibo::resolveAdvApi32(const char *name) {
	if (strcmp(name, "RegOpenKeyExA") == 0) return (void *) advapi32::RegOpenKeyExA;
	return 0;
}

