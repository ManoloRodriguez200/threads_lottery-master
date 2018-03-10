#include <gtk/gtk.h>
#include "SharedState.h"
#include "Viewer.h"
#include "LoteryScheduler.h"
#include "FileLoader.h"

int main (int argc, char *argv[])
{
    //Initialize GTK+
    g_log_set_handler("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
    gtk_init(&argc, &argv);
    g_log_set_handler("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

    viewer_show();

    gtk_main();

    if (SharedState != NULL) {
        free(SharedState);
        SharedState = NULL;
    }
    if (Scheduler != NULL) {
        LoteryScheduler_Free(Scheduler);
        Scheduler = NULL;
    }
    if (Loader !=  NULL) {
        FileLoader_Free(Loader);
        Loader = NULL;
    }

    return 0;
}
