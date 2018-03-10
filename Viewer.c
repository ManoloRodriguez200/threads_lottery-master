/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * viewer.c
 * Copyright (C) 2016 Wil C <wilz04@gmail.com>
 */
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include "Viewer.h"
#include "Viewer.callbacks.h"

#define UI_FILE "ui/viewer.ui"
#define TOP_WINDOW "window"

GtkWidget* viewer_get_window(GtkBuilder* builder) {
	return GTK_WIDGET(gtk_builder_get_object(builder, TOP_WINDOW));
}

GApplication* viewer_new() {
	return g_application_new("org.gnome.viewer", G_APPLICATION_HANDLES_OPEN);
}

int viewer_show() {
	GApplication* viewer = viewer_new();
	GtkWidget *window = NULL;
	GtkBuilder* builder;
	GError* error = NULL;

	/* Load UI from file */
	builder = gtk_builder_new();
	if (!gtk_builder_add_from_file(builder, UI_FILE, &error)) {
		g_critical("Couldn't load builder file: %s", error->message);
		g_error_free(error);
		return 1;
	}

	/* Connect signal handlers */
	gtk_builder_connect_signals(builder, viewer);

	/* Get the window object from the ui file */
	window = viewer_get_window(builder);

	if (!window) {
		g_critical("Widget \"%s\" is missing in file %s.", TOP_WINDOW, UI_FILE);
		return 1;
	}

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	/* Widgets initialization for viewer.ui - DO NOT REMOVE */
	window_init(GTK_WINDOW(window), builder);
	g_object_unref(builder);

	/* Enter the main loop */
	gtk_widget_show_all(window);

	return 0;
}
