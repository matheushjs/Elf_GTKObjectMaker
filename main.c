#include <gtk/gtk.h>
#include <draw.h>
#include <main_UI.h>

/*
CFigureMaker *get_test_figure(){
	CFigureMaker *fig;

	fig = c_figure_maker_new(0);

	c_figure_maker_add_rectangle(fig, 35, 25, "271B",
			"Comment!", 4, 4);

	c_figure_maker_add_up_triangle(fig, 29, 24, "271B",
			"Comment!", 5, 5);

	c_figure_maker_add_rectangle(fig, 0, 0, "271B",
			"Comment!", 5, 5);

	c_figure_maker_display(fig);

	return fig;
}
*/

int main(int argc, char *argv[]){
	gtk_init(&argc, &argv);

	main_UI_build_window();
	gtk_main();

	return 0;
}
