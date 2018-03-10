#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "ProgressbarList.h"

/* Global variables ----------------------------------------------- */
GtkBuilder* Builder = NULL;
int progressbarlist_length = 0;

/* ---------------------------------------------------------------- */
void progressbarlist_init(GtkBuilder *sender, int length) {
	Builder = g_object_ref(sender);
	
	if (length > 10) {
		g_critical("The max capacity for visible threads is 10 (requested: %d)", length);
		progressbarlist_length = 10;
	} else {
		progressbarlist_length = length;
	}
	int i;
	for (i=0; i<progressbarlist_length; i++) {
		gtk_widget_set_visible(GTK_WIDGET(progressbarlist_get_label(i)), 1);
		gtk_widget_set_visible(GTK_WIDGET(progressbarlist_get_progressbar(i)), 1);
		gtk_widget_set_visible(GTK_WIDGET(progressbarlist_get_status(i)), 1);
	}
}
/* ---------------------------------------------------------------- */
void progressbarlist_item_update(int id, double result, double percentage, int tickets, gboolean finished) {
	if (id >= progressbarlist_length) {
		g_critical("Couldn't update item, the thread is not visible (threadid: %d)", id);
		return;
	}
	
	char* label = (char*) malloc(sizeof(char)*64);
	snprintf(label, 63, "Process %i (%d tickets): %3.1f%%, Pi = %lf ", id, tickets, percentage * 100, result);
	
	char* ptext = (char*) malloc(sizeof(char)*32);
	snprintf(ptext, 31, "%1.15f", result);

	gtk_label_set_text(
		GTK_LABEL(progressbarlist_get_status(id)),
		percentage != gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(progressbarlist_get_progressbar(id))) && !finished?
		"[running]":
		""
	);
	
	gtk_label_set_text(GTK_LABEL(progressbarlist_get_label(id)), label);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressbarlist_get_progressbar(id)), percentage);

	free(label);
	free(ptext);
}
/* ---------------------------------------------------------------- */
GObject* progressbarlist_get_label(int id) {
	if (id >= progressbarlist_length) {
		g_critical("Couldn't get label, the thread is not visible (threadid: %d)", id);
		return NULL;
	}
	
	char* name = (char*) malloc(sizeof(char)*8);
	sprintf(name, "label_%d", id);
	return gtk_builder_get_object(Builder, name);
}
/* ---------------------------------------------------------------- */
GObject* progressbarlist_get_status(int id) {
	if (id >= progressbarlist_length) {
		g_critical("Couldn't get status, the thread is not visible (threadid: %d)", id);
		return NULL;
	}
	
	char* name = (char*) malloc(sizeof(char)*8);
	sprintf(name, "status_%d", id);
	return gtk_builder_get_object(Builder, name);
}
/* ---------------------------------------------------------------- */
GObject* progressbarlist_get_progressbar(int id) {
	if (id >= progressbarlist_length) {
		g_critical("Couldn't get progressbar, the thread is not visible (threadid: %d)", id);
		return NULL;
	}
	
	char* name = (char*) malloc(sizeof(char)*16);
	sprintf(name, "progressbar_%d", id);
	return gtk_builder_get_object(Builder, name);
}
/* ---------------------------------------------------------------- */
