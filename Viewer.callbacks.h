#include <gtk/gtk.h>

#ifndef CALLBACKS
#define CALLBACKS

void window_init(GtkWindow* window, GtkBuilder* sender);
void file_loader(GtkBuilder *sender);
void btStart_clicked(GtkWidget* btStart, gpointer user_data);
void btAbout_clicked(GtkWidget* btStart, gpointer user_data);

#endif
