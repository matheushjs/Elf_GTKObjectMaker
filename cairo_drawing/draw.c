#include <gtk/gtk.h>
#include <c_figure_maker.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

static gchar *global_filename = NULL;
static gboolean present_charmap = FALSE;

typedef unsigned char PixMap[8];
static PixMap global_pixmap[256];

static inline
void uchar_set_bit(unsigned char *c, int bit){ (*c) |= 1<<bit; }

static inline
unsigned char uchar_get_bit(unsigned char c, int bit){ return c & (1<<bit); }

static inline
double double_round(double x, int prec){
	double power = pow(10, prec);
	return round(x*power)/power;
}

static
void add_pixmap(int addr, char *bits){
	int char_index = addr/8;
	int bits_index = addr%8;
	int i;

	unsigned char *main = &global_pixmap[char_index][bits_index];
	for(i = 0; i < 8; i++)
		if(bits[7-i] == '1')
			uchar_set_bit(main, i);
}

static
void add_pixmap_mult(int addr0, int addr1, char *bits){
	int char_index = addr0/8;
	int bits_index = addr0%8;
	int i;

	unsigned char *main = &global_pixmap[char_index][bits_index];
	unsigned char *target = main + (addr1 - addr0);
	for(; main <= target; main++){
		for(i = 0; i < 8; i++)
			if(bits[7-i] == '1')
				uchar_set_bit(main, i);
	}
}

static
void read_charmap(const gchar *filename){
	FILE *fp = fopen(filename, "r");
	g_assert(fp);

	char line[100];
	int addr0, addr1;
	char bits[9];
	while(fscanf(fp, "%[^\n]\n", line) != EOF){
		if(sscanf(line, " [ %d .. %d ] : %8[01]", &addr0, &addr1, bits) == 3){
			add_pixmap_mult(addr0, addr1, bits);
		} else if(sscanf(line, " %d : %8[01]", &addr0, bits) == 2){
			add_pixmap(addr0, bits);
		}
	}
}

void cairo_set_filename(const gchar *filename){
	if(global_filename) g_free(global_filename);
	global_filename = g_strdup(filename);
	read_charmap(global_filename);
}

void cairo_free_filename(){
	g_free(global_filename);
}

void cairo_set_presentation(){
	present_charmap = TRUE;
}

void cairo_unset_presentation(){
	present_charmap = FALSE;
}

static
void decide_color(GdkRGBA *color, char col){
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
}

static
void draw_char(cairo_t *cr, double x, double y, double width, double height, GdkRGBA *color, char obj[5]){
	guint object_id;

	width /= 8;
	height /= 8;

	sscanf(obj+2, "%x", &object_id); //Will read the last 2 hexadecimal numbers.
	unsigned char *char_pointer = global_pixmap[object_id];

	int i, j;
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			if(uchar_get_bit(*char_pointer, 7-j)){
				gdk_cairo_set_source_rgba(cr, color);
			} else cairo_set_source_rgb(cr, 0, 0, 0);
			cairo_rectangle(cr, x+(j*width), y+(i*height), width, height);
			cairo_fill(cr);
		}
		char_pointer++;
	}
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
					decide_color(&color, (*mat)[i][j][1]);
					control_char = (*mat)[i][j][1];
				}
				
				x = i*dw;
				y = (29-j)*dh;
				if(global_filename) draw_char(cr, x, y, dw, dh, &color, (*mat)[i][j]);
				else{
					gdk_cairo_set_source_rgba(cr, &color);
					cairo_rectangle(cr, x, y, dw, dh);
					cairo_fill(cr);
				}
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

static
void draw_charmap(cairo_t *cr, double w, double h){
	double dw, dh, iw, ih;
	int i, j;
	GdkRGBA color;
	char obj[5];
	char *p = obj+2;

	gdk_rgba_parse(&color, "white");

	dw = w/16.0;
	dh = h/16.0;
	iw = 0.25*dw;
	ih = 0.25*dh;

	unsigned int k = 0;
	for(j = 0; j < 16; j++){
		for(i = 0; i < 16; i++){
			sprintf(p, "%2x", k++);
			draw_char(cr, iw+(i*dw), ih+(j*dh), dw-iw, dh-ih, &color, obj);
		}
	}
}

gboolean
draw_vga_char_matrix(GtkWidget *wid, cairo_t *cr, gpointer fig){
	guint width, height;

	width = gtk_widget_get_allocated_width(wid);
	height = gtk_widget_get_allocated_height(wid);
	
	if(!present_charmap){
		if(*(CFigureMaker **)fig == NULL ) return FALSE;
		draw_ruler(cr, width, height);
		draw_figures(cr, width, height, *(CFigureMaker **) fig, width/42.0, height/32.0);
	} else {
		draw_charmap(cr, width, height);
	}

	return FALSE;
}
