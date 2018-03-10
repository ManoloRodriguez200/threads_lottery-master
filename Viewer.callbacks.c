#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib/gi18n.h>
#include "Thread_Callbacks.h"
#include "LoteryScheduler.h"
#include "ProgressbarList.h"
#include "FileLoader.h"
#include "SharedState.h"
#include "Timer.h"

/* ---------------------------------------------------------------- */
void* startBackgroundTask(void* parameters) {
    if (Scheduler->preemptive) {
        setup_scheduler_timer(Loader->quantum);
		}
    int retVal = sigsetjmp(Scheduler->context, 1);
    if (retVal == 1) {
        pthread_exit((void*) 0);
		}
    LoteryScheduler_Schedule(Scheduler);
    pthread_exit((void*) 0);
}
/* ---------------------------------------------------------------- */
gboolean update_function(gpointer data) {
    int i;
    ThreadEntry* entry = NULL;
    gboolean allFinished = true;
    for (i = 0; i < Scheduler->numThreads; i++) {
        entry = &SharedState[i];
        if (entry->percentage != 1) {
            allFinished = false;
				}
        progressbarlist_item_update(i, entry->accuResult, entry->percentage, (int) Scheduler->threads[i]->tickets, allFinished);
    }
    if (allFinished) {
        free(SharedState);
        SharedState = NULL;
        GtkWidget* button = GTK_WIDGET(gtk_builder_get_object(Builder, "btStart"));
        gtk_widget_set_sensitive(button, true);
        return false;
    } else {
        return true;
		}
}
/* ---------------------------------------------------------------- */
void file_loader(GtkBuilder* sender) {
    GtkWidget* dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new(
		    "Open File",
		    GTK_WINDOW(gtk_builder_get_object(sender, "window")),
		    action,
		    _("_Cancel"),
		    GTK_RESPONSE_CANCEL,
		    _("_Open"),
		    GTK_RESPONSE_ACCEPT,
		    NULL
		);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        FileLoader_Init(filename);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}
/* ---------------------------------------------------------------- */
void window_init(GtkWindow* window, GtkBuilder* sender) {
    gtk_window_set_icon_from_file(window, "src/icon_96.jpg", NULL);
    gtk_window_set_default_icon_from_file("src/icon_96.jpg", NULL);
	
    file_loader(sender);
    progressbarlist_init(sender, (int) Loader->numThreads);
}
/* ---------------------------------------------------------------- */
void btStart_clicked(GtkWidget* btStart, gpointer user_data) {
    if (SharedState != NULL) {
        free(SharedState);
        SharedState = NULL;
    }
    SharedState = (ThreadEntry*) malloc(sizeof(ThreadEntry) * Loader->numThreads);
    int i;
    for (i = 0; i < Loader->numThreads; i++) {
        SharedState[i].percentage = 0.0;
        SharedState[i].accuResult = 0.0;
        SharedState[i].iteration = 0;
    }

    if (Scheduler != NULL) {
        LoteryScheduler_Free(Scheduler);
        Scheduler = NULL;
    }
    LoteryScheduler_Init(
		    Loader->numThreads,
		    runThread,
		    Loader->preemptive,
        Loader->yieldPercentage,
		    Loader->tickets,
		    Loader->work
		);

    GtkWidget* button = GTK_WIDGET(gtk_builder_get_object(Builder, "btStart"));
    gtk_widget_set_sensitive(button, false);

    g_timeout_add(100, update_function, NULL);

    pthread_t backgroundThread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    if (pthread_create(&backgroundThread, &attr, startBackgroundTask, NULL)) {
        puts("ERROR from pthread_create()");
		}
}
/* ---------------------------------------------------------------- */
void btAbout_clicked(GtkWidget* btStart, gpointer user_data) {
    const char* authors[6] = {
        "Olger Calder&#xF3;n Ach&#xED;o",
        "Wilberth Castro Fuentes",
        "Irene Gamboa Padilla",
        "Andr&#xE9;s Morales Esquivel",
        "Diego P&#xE9;rez Arroyo",
        NULL
    };
    
    GtkAboutDialog *dialog = GTK_ABOUT_DIALOG(gtk_about_dialog_new());
    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "Threads Lottery");
    gtk_about_dialog_set_comments(
        GTK_ABOUT_DIALOG(dialog), 
        "Advanced Operating Systems, Project 1."
    );
    
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("src/icon.png", NULL);
    gtk_about_dialog_set_logo(dialog, pixbuf);
    g_object_unref(pixbuf);
    pixbuf = NULL; 
    
    gtk_about_dialog_set_authors(dialog, authors);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(GTK_WIDGET(dialog));
}
/* ---------------------------------------------------------------- */
