#include <gtk/gtk.h>
#include <c_figure_maker.h>

static
void draw_figures(cairo_t *cr, guint w, guint h, const CFigureMaker *fig){
	int i, j;
	double x, y, dw, dh;
	VgaCharMatrix *mat;
	GdkRGBA color;

	dw = w/40.0;
	dh = h/30.0;

	g_assert(gdk_rgba_parse(&color, "blue"));
	gdk_cairo_set_source_rgba(cr, &color);
	
	mat = c_figure_maker_get_matrix(fig);

	for(i = 0; i < 40; i++){
		for(j = 0; j < 30; j++){
			if((*mat)[i][j][0] != '\0'){
				x = i*dw;
				y = (29-j)*dh;
				cairo_rectangle(cr, x, y, dw, dh);
				cairo_fill(cr);
			}
		}
	}

	c_figure_maker_destroy_matrix(mat);
}

gboolean
draw_vga_char_matrix(GtkWidget *wid, cairo_t *cr, gpointer fig){
	guint width, height;

	if(*(CFigureMaker **)fig == NULL ) return FALSE;

	width = gtk_widget_get_allocated_width(wid);
	height = gtk_widget_get_allocated_height(wid);

	draw_figures(cr, width, height, *(CFigureMaker **) fig);

	/*

	cairo_arc(cr, width/2.0, height/2.0,
			MIN(width, height) / 2.5,
			0, 2 * G_PI);
	*/

	/*
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_rectangle(cr, 0, 0, width, height);
	cairo_fill(cr);

	cairo_set_line_width(cr, 10);
	cairo_rectangle(cr, 0, 0, width, height);
	cairo_stroke(cr);
	*/

	/*
	cairo_pattern_t *pat;
	pat = cairo_pattern_create_radial(20, 20, 30, width-20, height-20, 30);
	cairo_pattern_add_color_stop_rgb(pat, 0, 0, 0, 1);
	cairo_pattern_add_color_stop_rgb(pat, 1, 1, 0, 0);

	cairo_rectangle(cr, 0, 0, width, height);
	cairo_set_source(cr, pat);
	cairo_fill(cr);
	*/

	return FALSE;
}
