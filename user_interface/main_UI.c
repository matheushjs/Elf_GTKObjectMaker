#include <gtk/gtk.h>
#include <c_figure_maker.h>
#include <draw.h>
#include <stdio.h>
#include <cmath>

CFigureMaker *global_fig = NULL;
struct {
	int init_addr;
	gchar shape[100];
	gchar func_type[100];
	gchar fill[100];
	int x, y;
	int width, height;
	double stretch_x, stretch_f;
	int xi, xe; //interval
	gchar vga_char[5];
} global_info = { 0, "dot", "x", "None", 0, 0, 0, 0, 1.0, 1.0, 0, 0, "" };

static
void destroy_global_fig(){
	if(global_fig)
		c_figure_maker_destroy(global_fig);
	global_fig = NULL;
}

static
void init_address_activate(GtkEntry *entry, gpointer data){
	const gchar *text;
	gchar *new_text;
	int addr;

	text = gtk_entry_get_text(entry);

	if(sscanf(text, "%d", &addr) != 0){
		global_info.init_addr = addr;
		global_fig = c_figure_maker_new(addr);
		gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
		new_text = g_strdup_printf("%d", addr);
		gtk_entry_set_text(entry, new_text);
		g_free(new_text);
		gtk_widget_set_can_focus(GTK_WIDGET(entry), FALSE);
		gtk_widget_show(GTK_WIDGET(data));
	}
}

static
void show_hide_foreach(GtkWidget *wid, gpointer data){
	static const gchar geometric_activate[][100] =
	//names of widgets to activate when a geometric shape is selected.
	{
		"figure_param_box"
	};
	static const gchar function_activate[][100] =
	//names of widgets to deactivate when a function shape is selected.
	{
		"func_box",
		"filling_box",
		"func_param_box"
	};
	static const gchar anyway_activate[][100] =
	//names of widgets to activate when either are selected.
	{
		"start_point_box",
		"vga_char_box",
		"action_box"
	};

	const gchar *name = gtk_widget_get_name(wid);
	guint i;

	for(i = 0; i < sizeof(geometric_activate)/sizeof(geometric_activate[0]); i++){
		if(g_strcmp0(name, geometric_activate[i]) == 0){
			if(GPOINTER_TO_INT(data) == 1)
				gtk_widget_show(wid);
			else gtk_widget_hide(wid);
			return;
		}
	}

	for(i = 0; i < sizeof(function_activate)/sizeof(function_activate[0]); i++){
		if(g_strcmp0(name, function_activate[i]) == 0){
			if(GPOINTER_TO_INT(data) == 2)
				gtk_widget_show(wid);
			else gtk_widget_hide(wid);
			return;
		}
	}

	for(i = 0; i < sizeof(anyway_activate)/sizeof(anyway_activate[0]); i++){
		if(g_strcmp0(name, anyway_activate[i]) == 0){
			gtk_widget_show(wid);
			return;
		}
	}

	if(GTK_IS_CONTAINER(wid))
		gtk_container_foreach(GTK_CONTAINER(wid), GtkCallback(show_hide_foreach), data);
}

//Demultiplexes the selected shapes to hide/show the due other boxes.
static
void shape_box_demux(GtkRadioButton *but){
	//When you switch buttons, the one that became innactive also
	//emits the 'toggled' signal. This conditional prevents double
	//execution of this function.
	if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(but))) return;

	GtkWidget *win = gtk_widget_get_toplevel(GTK_WIDGET(but));
	const gchar *name = gtk_widget_get_name(GTK_WIDGET(but));
	g_stpcpy(global_info.shape, name);

	if(g_strcmp0(name, "rectangle") == 0
	|| g_strcmp0(name, "utriangle") == 0
	|| g_strcmp0(name, "btriangle") == 0)
		gtk_container_foreach(GTK_CONTAINER(win), GtkCallback(show_hide_foreach), GINT_TO_POINTER(1));
	else if(g_strcmp0(name, "function") == 0)
		gtk_container_foreach(GTK_CONTAINER(win), GtkCallback(show_hide_foreach), GINT_TO_POINTER(2));
	else if(g_strcmp0(name, "dot") == 0)
		gtk_container_foreach(GTK_CONTAINER(win), GtkCallback(show_hide_foreach), GINT_TO_POINTER(3));
	else g_assert(FALSE);

	printf("%d\n", global_info.init_addr);
	printf("%s\n", global_info.shape);
	printf("%s\n", global_info.func_type);
	printf("%s\n", global_info.fill);
	printf("x,y: %d %d\n", global_info.x, global_info.y);
	printf("w,h: %d %d\n", global_info.width, global_info.height);
	printf("w,h: %lf %lf\n", global_info.stretch_x, global_info.stretch_f);
	printf("xi,xe: %d %d\n", global_info.xi, global_info.xe);
	printf("%s\n", global_info.vga_char);
}

static
void function_radio_button_toggled(GtkRadioButton *button, gpointer data){
	if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button))) return;
	g_stpcpy(global_info.func_type, gtk_button_get_label(GTK_BUTTON(button)));
}

static
void filling_radio_button_toggled(GtkRadioButton *button, gpointer data){
	if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button))) return;
	g_stpcpy(global_info.fill, gtk_button_get_label(GTK_BUTTON(button)));
}

static
void spin_buttons_changed(GtkSpinButton *but, gpointer data){
	const gchar *name = gtk_widget_get_name(GTK_WIDGET(but));

	if(g_strcmp0(name, "x") == 0){
		global_info.x = gtk_spin_button_get_value_as_int(but);
	} else if(g_strcmp0(name, "y") == 0){
		global_info.y = gtk_spin_button_get_value_as_int(but);
	} else if(g_strcmp0(name, "width") == 0){
		global_info.width = gtk_spin_button_get_value_as_int(but);
	} else if(g_strcmp0(name, "height") == 0){
		global_info.height = gtk_spin_button_get_value_as_int(but);
	} else if(g_strcmp0(name, "stretch_x") == 0){
		global_info.stretch_x = gtk_spin_button_get_value(but);
	} else if(g_strcmp0(name, "stretch_f") == 0){
		global_info.stretch_f = gtk_spin_button_get_value(but);
	} else if(g_strcmp0(name, "xi") == 0){
		global_info.xi = gtk_spin_button_get_value_as_int(but);
	} else if(g_strcmp0(name, "xe") == 0){
		global_info.xe = gtk_spin_button_get_value_as_int(but);
	} else g_assert(FALSE);
}

static
void vga_char_buffer_store(GtkEntry *ent){
	const gchar *text;
	text = gtk_entry_get_text(ent);
	g_stpcpy(global_info.vga_char, text);
}

double f1(double x){ return x; }
double f2(double x){ return std::fabs(x); }
double f3(double x){ return x*x; }
double f4(double x){ return x*x*x; }
double f5(double x){ return std::fabs(x*x*x); }
double f6(double x){ return std::cos(x); }
double f7(double x){ return std::fabs(std::cos(x)); }
double f8(double x){ return std::sin(x); }
double f9(double x){ return std::fabs(std::sin(x)); }
double f10(double x){ return std::pow(2,x); }
double (*choose_shape_function())(double){
	if(g_ascii_strcasecmp(global_info.func_type, "x") == 0) return f1;
	else if(g_ascii_strcasecmp(global_info.func_type, "|x|") == 0) return f2;
	else if(g_ascii_strcasecmp(global_info.func_type, "x**2") == 0) return f3;
	else if(g_ascii_strcasecmp(global_info.func_type, "x**3") == 0) return f4;
	else if(g_ascii_strcasecmp(global_info.func_type, "|x**3|") == 0) return f5;
	else if(g_ascii_strcasecmp(global_info.func_type, "cos(x)") == 0) return f6;
	else if(g_ascii_strcasecmp(global_info.func_type, "|cos(x)|") == 0) return f7;
	else if(g_ascii_strcasecmp(global_info.func_type, "sin(x)") == 0) return f8;
	else if(g_ascii_strcasecmp(global_info.func_type, "|sin(x)|") == 0) return f9;
	else if(g_ascii_strcasecmp(global_info.func_type, "2**x") == 0) return f10;
	else return f1;
}

enum fill_t choose_filling(){
	if(g_ascii_strcasecmp(global_info.fill, "up") == 0) return UP;
	else if(g_ascii_strcasecmp(global_info.fill, "down") == 0) return DOWN;
	else if(g_ascii_strcasecmp(global_info.fill, "left") == 0) return LEFT;
	else if(g_ascii_strcasecmp(global_info.fill, "right") == 0) return RIGHT;
	else return NONE;
}

static
void create_figure_activated(GtkWidget *wid){
	guint k;
	gchar *comm = NULL;
	
	if(sscanf(global_info.vga_char, "%1x%1x%1x%1x",&k,&k,&k,&k) != 4){
		fprintf(stderr, "vga_write is not a hexadecimal number\n");
		return;
	}

	sscanf(global_info.vga_char, "%4x", &k);
	if(k & 0x1000){
		fprintf(stderr, "The \"size\" bit cannot be set to '1'. Feel free to use the other 3 bits at will.\n");
		return;
	}

	if(g_ascii_strcasecmp(global_info.shape, "dot") == 0){
		comm = g_strdup_printf("Dot (%d, %d)", global_info.x, global_info.y);
		c_figure_maker_add_point(global_fig, global_info.x, global_info.y, global_info.vga_char, comm);
		c_figure_maker_merge(global_fig);
	} else if(g_ascii_strcasecmp(global_info.shape, "rectangle") == 0){
		comm = g_strdup_printf("Rectangle (%d, %d) w: %d, h: %d", global_info.x, global_info.y, global_info.width, global_info.height);
		c_figure_maker_add_rectangle(global_fig, global_info.x, global_info.y, global_info.vga_char, comm, global_info.width, global_info.height);
		c_figure_maker_merge(global_fig);
	} else if(g_ascii_strcasecmp(global_info.shape, "utriangle") == 0){
		comm = g_strdup_printf("Base-Up Triangle (%d, %d) w: %d, h: %d", global_info.x, global_info.y, global_info.width, global_info.height);
		c_figure_maker_add_up_triangle(global_fig, global_info.x, global_info.y, global_info.vga_char, comm, global_info.width, global_info.height);
		c_figure_maker_merge(global_fig);
	} else if(g_ascii_strcasecmp(global_info.shape, "btriangle") == 0){
		comm = g_strdup_printf("Base-Down Triangle (%d, %d) w: %d, h: %d", global_info.x, global_info.y, global_info.width, global_info.height);
		c_figure_maker_add_down_triangle(global_fig, global_info.x, global_info.y, global_info.vga_char, comm, global_info.width, global_info.height);
		c_figure_maker_merge(global_fig);
	} else if(g_ascii_strcasecmp(global_info.shape, "function") == 0){
		if(global_info.xi > global_info.xe){
			int aux = global_info.xi;
			global_info.xi = global_info.xe;
			global_info.xe = aux;
		}
		comm = g_strdup_printf("Function %s (%d, %d) interval: [%d,%d]", global_info.func_type, global_info.x, global_info.y, global_info.xi, global_info.xe);
		double (*func)(double) = choose_shape_function();
		enum fill_t fill = choose_filling();
		c_figure_maker_add_function(global_fig, global_info.x, global_info.y,
				global_info.vga_char, comm, func,
				global_info.xi, global_info.xe,
				global_info.stretch_x, global_info.stretch_f,
				fill);
		c_figure_maker_merge(global_fig);
	} else g_assert(FALSE);

	if(comm) g_free(comm);
	gtk_widget_queue_draw(wid);
}

static
void undo_activated(GtkWidget *wid){
	c_figure_maker_undo(global_fig);
	gtk_widget_queue_draw(wid);
}

static
void register_signal_handlers(GtkBuilder *build){
	gtk_builder_add_callback_symbol(build, "destroy_global_fig", G_CALLBACK(destroy_global_fig));
	gtk_builder_add_callback_symbol(build, "init_address_activate", G_CALLBACK(init_address_activate));
	gtk_builder_add_callback_symbol(build, "shape_box_demux", G_CALLBACK(shape_box_demux));
	gtk_builder_add_callback_symbol(build, "function_radio_button_toggled", G_CALLBACK(function_radio_button_toggled));
	gtk_builder_add_callback_symbol(build, "filling_radio_button_toggled", G_CALLBACK(filling_radio_button_toggled));
	gtk_builder_add_callback_symbol(build, "spin_buttons_changed", G_CALLBACK(spin_buttons_changed));
	gtk_builder_add_callback_symbol(build, "vga_char_buffer_store", G_CALLBACK(vga_char_buffer_store));
	gtk_builder_add_callback_symbol(build, "create_figure_activated", G_CALLBACK(create_figure_activated));
	gtk_builder_add_callback_symbol(build, "undo_activated", G_CALLBACK(undo_activated));
}

GtkWindow *main_UI_build_window(){
	GtkBuilder *build;
	GtkWindow *win;
	GtkDrawingArea *da;

	build = gtk_builder_new_from_file("/home/mathjs/ComputerS/"
			"projects/gtk_object_maker_sisdig/user_interface/main_UI.glade");
	
	
	da = GTK_DRAWING_AREA(gtk_builder_get_object(build, "drawing_area"));
	g_signal_connect(da, "draw", G_CALLBACK(draw_vga_char_matrix), &global_fig);

	win = GTK_WINDOW(gtk_builder_get_object(build, "main_window"));
	
	register_signal_handlers(build);
	gtk_builder_connect_signals(build, NULL);
	g_object_unref(build);
	
	gtk_widget_show(GTK_WIDGET(win));

	return win;
}
