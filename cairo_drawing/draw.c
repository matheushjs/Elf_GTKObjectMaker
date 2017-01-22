#include <gtk/gtk.h>
#include <c_figure_maker.h>
#include <string.h>
#include <math.h>

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
void draw_figures(cairo_t *cr, guint w, guint h, const CFigureMaker *fig, guint transw, guint transh){
	int i, j;
	double x, y, dw, dh;
	VgaCharMatrix *mat;
	GdkRGBA color;

	cairo_save(cr);
	cairo_translate(cr, transw, transh);
	w -= transw*2;
	h -= transh*2;

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
	cairo_restore(cr);
}

static
void draw_ruler(cairo_t *cr, guint w, guint h){
	double hor_square_size, ver_square_size; //size of each square.
	hor_square_size = w/42.0;
	ver_square_size = h/32.0;

	double hor_pos, ver_pos; //Place within each square where text should be printed.

	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 12);

	int i, j;
	gchar *text;
	for(j = 0; j < 2; j++){
		ver_pos = (j == 0 ? 0.75 : 31.75) * ver_square_size;
		for(i = 0; i < 40; i++){
			text = g_strdup_printf("%d", i);
			hor_pos = hor_square_size/(pow(3.0, strlen(text))); //To attempt to center multi-digit numbers
			
			cairo_move_to(cr, hor_pos + (hor_square_size * (i+1)), ver_pos);
			cairo_show_text(cr, text);
			g_free(text);
		}
	}

	for(j = 0; j < 2; j++){
		for(i = 0; i < 30; i++){
			text = g_strdup_printf("%d", (29-i));
			hor_pos = hor_square_size/(pow(3.0, strlen(text))); //To attempt to center multi-digit numbers
			hor_pos += hor_square_size * (j == 1 ? 41.0 : 0);
			ver_pos = ver_square_size/1.5;

			cairo_move_to(cr, hor_pos, ver_pos + (ver_square_size * (i+1)));
			cairo_show_text(cr, text);
			g_free(text);
		}
	}
}

gboolean
draw_vga_char_matrix(GtkWidget *wid, cairo_t *cr, gpointer fig){
	guint width, height;

	if(*(CFigureMaker **)fig == NULL ) return FALSE;

	width = gtk_widget_get_allocated_width(wid);
	height = gtk_widget_get_allocated_height(wid);

	draw_ruler(cr, width, height);
	draw_figures(cr, width, height, *(CFigureMaker **) fig, width/42.0, height/32.0);

	return FALSE;
}
