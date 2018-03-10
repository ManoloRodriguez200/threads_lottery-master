#include <gtk/gtk.h>

#ifndef PROGRESSBARLIST
#define PROGRESSBARLIST

void progressbarlist_init(GtkBuilder* builder, int length);
void progressbarlist_item_update(int id, double result, double percentage, int tickets, gboolean finished);

extern GtkBuilder*Builder;
GObject* progressbarlist_get_label(int id);
GObject* progressbarlist_get_progressbar(int id);
GObject* progressbarlist_get_status(int id);

#endif
