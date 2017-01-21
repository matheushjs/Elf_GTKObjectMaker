#ifndef _DRAW_H
#define _DRAW_H

#include <gtk/gtk.h>

gboolean
draw_vga_char_matrix(GtkWidget *wid, cairo_t *cr, gpointer data);

#endif
