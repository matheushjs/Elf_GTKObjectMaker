#include <gtk/gtk.h>
#include <c_figure_maker.h>

static
void decide_color(cairo_t *cr, GdkRGBA *color, char col){
	switch(g_ascii_tolower(col)){
		case '0':
			g_assert(gdk_rgba_parse(color, "#000000"));
			break;
		case '1':
			g_assert(gdk_rgba_parse(color, "dark red"));
			break;
		case '2':
			g_assert(gdk_rgba_parse(color, "dark green"));
			break;
		case '3':
			g_assert(gdk_rgba_parse(color, "#888800"));
			break;
		case '4':
			g_assert(gdk_rgba_parse(color, "dark blue"));
			break;
		case '5':
			g_assert(gdk_rgba_parse(color, "#880088"));
			break;
		case '6':
			g_assert(gdk_rgba_parse(color, "dark cyan"));
			break;
		case '7':
			g_assert(gdk_rgba_parse(color, "#c0c0c0"));
			break;
		case '8':
			g_assert(gdk_rgba_parse(color, "gray"));
			break;
		case '9':
			g_assert(gdk_rgba_parse(color, "red"));
			break;
		case 'a':
			g_assert(gdk_rgba_parse(color, "#00ff00"));
			break;
		case 'b':
			g_assert(gdk_rgba_parse(color, "yellow"));
			break;
		case 'c':
			g_assert(gdk_rgba_parse(color, "blue"));
			break;
		case 'd':
			g_assert(gdk_rgba_parse(color, "magenta"));
			break;
		case 'e':
			g_assert(gdk_rgba_parse(color, "cyan"));
			break;
		case 'f':
			g_assert(gdk_rgba_parse(color, "white"));
			break;
	}
	
	gdk_cairo_set_source_rgba(cr, color);
}

static
void draw_figures(cairo_t *cr, guint w, guint h, const CFigureMaker *fig){
	int i, j;
	double x, y, dw, dh;
	VgaCharMatrix *mat;
	GdkRGBA color;

	dw = w/40.0;
	dh = h/30.0;

	mat = c_figure_maker_get_matrix(fig);

	for(i = 0; i < 40; i++){
		char control_char = 'x'; //efficiency.
		for(j = 0; j < 30; j++){
			if((*mat)[i][j][0] != '\0'){
				if((*mat)[i][j][1] != control_char){
					decide_color(cr, &color, (*mat)[i][j][1]);
					control_char = (*mat)[i][j][1];
				}
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
