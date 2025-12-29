#include "app_context.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define MKDIR(p) _mkdir(p)
#else
#include <sys/stat.h>
#define MKDIR(p) mkdir(p, 0700)
#endif

AppContext APP;

static void criar_dir(const char *path) {
    MKDIR(path); // ignora erro se já existir
}

int inicializar_app_context(void) {
#ifdef _WIN32
    DWORD len = GetEnvironmentVariableA("USERPROFILE", APP.base, sizeof(APP.base));
    if (len == 0 || len >= sizeof(APP.base)) {
        return 0;
    }
    snprintf(APP.base + len, sizeof(APP.base) - len, "\\FormDB");
#else
    snprintf(APP.base, sizeof(APP.base), "./FormDB");
#endif

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat-truncation"
    snprintf(APP.data, sizeof(APP.data), "%s/data", APP.base);
    snprintf(APP.forms, sizeof(APP.forms), "%s/data/forms", APP.base);
    snprintf(APP.records, sizeof(APP.records), "%s/data/records", APP.base);
    snprintf(APP.exports, sizeof(APP.exports), "%s/exports", APP.base);
    snprintf(APP.backups, sizeof(APP.backups), "%s/backups", APP.base);
    snprintf(APP.templates, sizeof(APP.templates), "%s/templates", APP.base);
    #pragma GCC diagnostic pop

    criar_dir(APP.base);
    criar_dir(APP.data);
    criar_dir(APP.forms);
    criar_dir(APP.records);
    criar_dir(APP.exports);
    criar_dir(APP.backups);
    criar_dir(APP.templates);

    return 1;
}
