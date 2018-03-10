/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * viewer.h
 * Copyright (C) 2016 Wil C <wilz04@gmail.com>
 */

#ifndef VIEWER
#define VIEWER

#include <gtk/gtk.h>

GApplication* viewer_new();
GtkWidget* viewer_get_window();
int viewer_show();

#endif
