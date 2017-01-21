#include <gtk/gtk.h>
#include <c_figure_maker.h>
#include <draw.h>
#include <stdio.h>

CFigureMaker *global_fig = NULL;

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
			if(GPOINTER_TO_INT(data) == 1)
				gtk_widget_hide(wid);
			else gtk_widget_show(wid);
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

static
void show_hide_boxes(GtkRadioButton *but){
	GtkWidget *win = gtk_widget_get_toplevel(GTK_WIDGET(but));
	
	const gchar *name;
	name = gtk_widget_get_name(GTK_WIDGET(but));
	
	if(g_strcmp0(name, "rectangle") == 0
	|| g_strcmp0(name, "utriangle") == 0
	|| g_strcmp0(name, "btriangle") == 0)
		gtk_container_foreach(GTK_CONTAINER(win), GtkCallback(show_hide_foreach), GINT_TO_POINTER(1));
	else if(g_strcmp0(name, "function") == 0)
		gtk_container_foreach(GTK_CONTAINER(win), GtkCallback(show_hide_foreach), GINT_TO_POINTER(2));
	else g_assert(FALSE);
}

//Demultiplexes the selected shapes to hide/show the due other boxes.
static
void shape_box_demux(GtkRadioButton *radio_but){
	GSList *buttons;

	//When you switch buttons, the one that became innactive also
	//emits the 'toggled' signal. This conditional prevents double
	//execution of this function.
	if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_but))) return;

	buttons = gtk_radio_button_get_group(GTK_RADIO_BUTTON(radio_but));

	printf("Run\n");

	while(buttons){
		if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(buttons->data))) break;
		buttons = g_slist_next(buttons);
	}
	
	g_assert(buttons); //If buttons is false, no buttons are activated, which is impossible.
	GtkRadioButton *but = GTK_RADIO_BUTTON(buttons->data);

	show_hide_boxes(but);
}

static
void register_signal_handlers(GtkBuilder *build){
	gtk_builder_add_callback_symbol(build, "destroy_global_fig", G_CALLBACK(destroy_global_fig));
	gtk_builder_add_callback_symbol(build, "init_address_activate", G_CALLBACK(init_address_activate));
	gtk_builder_add_callback_symbol(build, "shape_box_demux", G_CALLBACK(shape_box_demux));
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
