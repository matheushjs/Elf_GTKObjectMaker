#ifndef _DRAW_H
#define _DRAW_H

#include <gtk/gtk.h>

void cairo_set_filename(const gchar *);
void cairo_free_filename();
void cairo_set_presentation();
void cairo_unset_presentation();

gboolean
draw_vga_char_matrix(GtkWidget *wid, cairo_t *cr, gpointer data);

#endif
