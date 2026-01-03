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
    snprintf(APP.base, sizeof(APP.base), ".\\tools");
    // ...

    const char *sep = "\\";
#else
    snprintf(APP.base, sizeof(APP.base), "./tools");
    const char *sep = "/";
#endif

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat-truncation"
    snprintf(APP.data, sizeof(APP.data), "%s%sdata", APP.base, sep);
    snprintf(APP.forms, sizeof(APP.forms), "%s%sdata%sforms", APP.base, sep, sep);
    snprintf(APP.records, sizeof(APP.records), "%s%sdata%srecords", APP.base, sep, sep);
    snprintf(APP.exports, sizeof(APP.exports), "%s%sexports", APP.base, sep);
    snprintf(APP.backups, sizeof(APP.backups), "%s%sbackups", APP.base, sep);
    snprintf(APP.templates, sizeof(APP.templates), "%s%stemplates", APP.base, sep);
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
