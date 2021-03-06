/* GAIL - The GNOME Accessibility Implementation Library
 * Copyright 2001 Sun Microsystems Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <gtk/gtk.h>
#include "gailmenushell.h"

static void         gail_menu_shell_class_init          (GailMenuShellClass *klass);
static void         gail_menu_shell_init                (GailMenuShell      *menu_shell);
static void         gail_menu_shell_initialize          (AtkObject          *accessible,
                                                         gpointer            data);
static void         atk_selection_interface_init        (AtkSelectionIface  *iface);
static gboolean     gail_menu_shell_add_selection       (AtkSelection   *selection,
                                                         gint           i);
static gboolean     gail_menu_shell_clear_selection     (AtkSelection   *selection);
static AtkObject*   gail_menu_shell_ref_selection       (AtkSelection   *selection,
                                                         gint           i);
static gint         gail_menu_shell_get_selection_count (AtkSelection   *selection);
static gboolean     gail_menu_shell_is_child_selected   (AtkSelection   *selection,
                                                         gint           i);
static gboolean     gail_menu_shell_remove_selection    (AtkSelection   *selection,
                                                         gint           i);

G_DEFINE_TYPE_WITH_CODE (GailMenuShell, gail_menu_shell, GAIL_TYPE_CONTAINER,
                         G_IMPLEMENT_INTERFACE (ATK_TYPE_SELECTION, atk_selection_interface_init))

static void
gail_menu_shell_class_init (GailMenuShellClass *klass)
{
  AtkObjectClass *atk_object_class = ATK_OBJECT_CLASS (klass);

  atk_object_class->initialize = gail_menu_shell_initialize;
}

static void
gail_menu_shell_init (GailMenuShell *menu_shell)
{
}

static void
gail_menu_shell_initialize (AtkObject *accessible,
                            gpointer  data)
{
  ATK_OBJECT_CLASS (gail_menu_shell_parent_class)->initialize (accessible, data);

  if (GTK_IS_MENU_BAR (data))
    accessible->role = ATK_ROLE_MENU_BAR;
  else
    /*
     * Accessible object for Menu is created in gailmenu.c
     */
    accessible->role = ATK_ROLE_UNKNOWN;
}

static void
atk_selection_interface_init (AtkSelectionIface *iface)
{
  iface->add_selection = gail_menu_shell_add_selection;
  iface->clear_selection = gail_menu_shell_clear_selection;
  iface->ref_selection = gail_menu_shell_ref_selection;
  iface->get_selection_count = gail_menu_shell_get_selection_count;
  iface->is_child_selected = gail_menu_shell_is_child_selected;
  iface->remove_selection = gail_menu_shell_remove_selection;
  /*
   * select_all_selection does not make sense for a menu_shell
   * so no implementation is provided.
   */
}

static gboolean
gail_menu_shell_add_selection (AtkSelection *selection,
                               gint          i)
{
  GList *kids;
  GtkWidget *item;
  guint length;
  GtkWidget *widget;

  widget =  gtk_accessible_get_widget (GTK_ACCESSIBLE (selection));
  if (widget == NULL)
  {
    /* State is defunct */
    return FALSE;
  }

  kids = gtk_container_get_children (GTK_CONTAINER (widget));
  length = g_list_length (kids);
  if (i < 0 || i > length)
    {
      g_list_free (kids);
      return FALSE;
    }

  item = g_list_nth_data (kids, i);
  g_list_free (kids);
  g_return_val_if_fail (GTK_IS_MENU_ITEM(item), FALSE);
  gtk_menu_shell_select_item (GTK_MENU_SHELL (widget), item);
  return TRUE;
}

static gboolean
gail_menu_shell_clear_selection (AtkSelection   *selection)
{
  GtkMenuShell *shell;
  GtkWidget *widget;

  widget =  gtk_accessible_get_widget (GTK_ACCESSIBLE (selection));
  if (widget == NULL)
  {
    /* State is defunct */
    return FALSE;
  }

  shell = GTK_MENU_SHELL (widget);

  gtk_menu_shell_deselect (shell);
  return TRUE;
}

static AtkObject*
gail_menu_shell_ref_selection (AtkSelection   *selection,
                               gint           i)
{
  GtkMenuShell *shell;
  AtkObject *obj;
  GtkWidget *widget;
  GtkWidget *item;

  if (i != 0)
    return NULL;

  widget =  gtk_accessible_get_widget (GTK_ACCESSIBLE (selection));
  if (widget == NULL)
  {
    /* State is defunct */
    return NULL;
  }

  shell = GTK_MENU_SHELL (widget);

  item = gtk_menu_shell_get_selected_item (shell);
  if (item != NULL)
  {
    obj = gtk_widget_get_accessible (item);
    g_object_ref (obj);
    return obj;
  }
  else
  {
    return NULL;
  }
}

static gint
gail_menu_shell_get_selection_count (AtkSelection   *selection)
{
  GtkMenuShell *shell;
  GtkWidget *widget;

  widget =  gtk_accessible_get_widget (GTK_ACCESSIBLE (selection));
  if (widget == NULL)
  {
    /* State is defunct */
    return 0;
  }

  shell = GTK_MENU_SHELL (widget);

  /*
   * Identifies the currently selected menu item
   */
  if (gtk_menu_shell_get_selected_item (shell) == NULL)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

static gboolean
gail_menu_shell_is_child_selected (AtkSelection   *selection,
                                   gint           i)
{
  GtkMenuShell *shell;
  GList *kids;
  gint j;
  GtkWidget *widget;
  GtkWidget *item;

  widget =  gtk_accessible_get_widget (GTK_ACCESSIBLE (selection));
  if (widget == NULL)
  {
    /* State is defunct */
    return FALSE;
  }

  shell = GTK_MENU_SHELL (widget);
  item = gtk_menu_shell_get_selected_item (shell);
  if (item == NULL)
    return FALSE;

  kids = gtk_container_get_children (GTK_CONTAINER (shell));
  j = g_list_index (kids, item);
  g_list_free (kids);

  return (j==i);
}

static gboolean
gail_menu_shell_remove_selection (AtkSelection   *selection,
                                  gint           i)
{
  GtkMenuShell *shell;
  GtkWidget *widget;
  GtkWidget *item;

  if (i != 0)
    return FALSE;

  widget =  gtk_accessible_get_widget (GTK_ACCESSIBLE (selection));
  if (widget == NULL)
  {
    /* State is defunct */
    return FALSE;
  }

  shell = GTK_MENU_SHELL (widget);

  item = gtk_menu_shell_get_selected_item (shell);
  if (item && gtk_menu_item_get_submenu (GTK_MENU_ITEM (item)))
  {
    /*
     * Menu item contains a menu and it is the selected menu item
     * so deselect it.
     */
    gtk_menu_shell_deselect (shell);
  }
  return TRUE;
}
