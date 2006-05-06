/*
 * viking -- GPS Data and Topo Analyzer, Explorer, and Manager
 *
 * Copyright (C) 2003-2005, Evan Battaglia <gtoevan@gmx.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include <string.h>
#include <glib/gprintf.h>

#include "viking.h"
#include "babel.h"
#include "gpx.h"
#include "acquire.h"

#define GOOGLE_DIRECTIONS_STRING "(wget -O - \"http://maps.google.com/maps?q=%s to %s&output=xml\" 2>/dev/null) | head -3 | tail -1 | sed 's/.*<page>\\(.*\\)<\\/page>.*/<page>\\1<\\/page>/'"

typedef struct {
  GtkWidget *from_entry, *to_entry;
} datasource_google_widgets_t;


gpointer datasource_google_add_widgets ( GtkWidget *dialog, VikViewport *vvp );
static void datasource_google_get_cmd_string ( datasource_google_widgets_t *widgets, gchar **cmd, gchar **input_type );	
static void datasource_google_first_cleanup ( gpointer data );

VikDataSourceInterface vik_datasource_google_interface = {
  "Google Directions",
  "Google Directions",
  VIK_DATASOURCE_SHELL_CMD,
  VIK_DATASOURCE_ADDTOLAYER,
  (VikDataSourceCheckExistenceFunc)	NULL,
  (VikDataSourceAddWidgetsFunc)		datasource_google_add_widgets,
  (VikDataSourceGetCmdStringFunc)	datasource_google_get_cmd_string,
  (VikDataSourceFirstCleanupFunc)	datasource_google_first_cleanup,
  (VikDataSourceProgressFunc)		NULL,
  (VikDataSourceAddProgressWidgetsFunc)	NULL,
  (VikDataSourceCleanupFunc)		NULL
};


gpointer datasource_google_add_widgets ( GtkWidget *dialog, VikViewport *vvp )
{
  datasource_google_widgets_t *widgets = g_malloc(sizeof(*widgets));
  GtkWidget *from_label, *to_label;
  from_label = gtk_label_new ("From:");
  widgets->from_entry = gtk_entry_new();
  to_label = gtk_label_new ("To:");
  widgets->to_entry = gtk_entry_new();
  gtk_box_pack_start ( GTK_BOX(GTK_DIALOG(dialog)->vbox), from_label, FALSE, FALSE, 5 );
  gtk_box_pack_start ( GTK_BOX(GTK_DIALOG(dialog)->vbox), widgets->from_entry, FALSE, FALSE, 5 );
  gtk_box_pack_start ( GTK_BOX(GTK_DIALOG(dialog)->vbox), to_label, FALSE, FALSE, 5 );
  gtk_box_pack_start ( GTK_BOX(GTK_DIALOG(dialog)->vbox), widgets->to_entry, FALSE, FALSE, 5 );
  gtk_widget_show_all(dialog);
  return widgets;
}

static void datasource_google_get_cmd_string ( datasource_google_widgets_t *widgets, gchar **cmd, gchar **input_type )
{
  /* TODO: special characters handling!!! */
  gchar *from_quoted, *to_quoted;
  from_quoted = g_shell_quote ( gtk_entry_get_text ( GTK_ENTRY(widgets->from_entry) ) );
  to_quoted = g_shell_quote ( gtk_entry_get_text ( GTK_ENTRY(widgets->to_entry) ) );

  *cmd = g_strdup_printf( GOOGLE_DIRECTIONS_STRING, from_quoted, to_quoted );
  *input_type = g_strdup("google");

  g_free(from_quoted);
  g_free(to_quoted);
}

static void datasource_google_first_cleanup ( gpointer data )
{
  g_free ( data );
}