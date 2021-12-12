#include <iostream>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>  

//Objetos da main window
GtkBuilder *builder;
Viewport* viewport;

GObject *window_principal;

GtkListStore  *store;
GtkTreeIter iter;
GtkTreeView* tree_view_objetos;
GtkWidget *view;
GtkTreeSelection* tree_view_selection_objetos;

GtkWidget* draw_viewport;

GtkButton* cima_button;
GtkButton* baixo_button;
GtkButton* esquerda_button;
GtkButton* direita_button;

GtkButton* tras_button;
GtkButton* frente_button;

GtkButton* rotacionar_direita_button;
GtkButton* rotacionar_esquerda_button;

GtkButton* adicionar_objeto_button;
GtkButton* modificar_objeto_button;


GtkEntry* quantidade_entry;
GtkEntry* angulo_entry;

GtkToggleButton *cohen_check_button,*liang_check_button;
GtkToggleButton *paralela_check_button,*perspectiva_check_button;
GtkToggleButton *x_check_button,*y_check_button,*z_check_button;

GtkMenuItem* menuitem1;
GtkMenuItem* menuitem2;
GtkMenuItem* menuitem3;
GtkMenuItem* menuitem4;

GtkAdjustment* escala_scale;

    
int main (int argc, char *argv[]) {
	
	gtk_init (&argc, &argv);

	/* Construct a GtkBuilder instance and load our UI description */
	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "interface.glade", NULL);

	/* Connect signal handlers to the constructed widgets. */
	window_principal = gtk_builder_get_object (builder, "window_principal");
	g_signal_connect (window_principal, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	gtk_window_set_resizable (GTK_WINDOW(window_principal),  false);

	/* Tamanho da viewport */
	viewport = new Viewport(510,515);
	/* Connect Treeview*/
	objects_tree = GTK_TREE_VIEW(gtk_builder_get_object(builder, "object_tree"));
	create_treeview();


	viewport = GTK_WIDGET(gtk_builder_get_object(builder, "viewport"));

	
	escala_scale = GTK_ADJUSTMENT(gtk_builder_get_object(builder, "adjustment1"));


	/* Buttons */
	

	cima = GTK_BUTTON(gtk_builder_get_object(builder, "cima_button"));
	

	baixo = GTK_BUTTON(gtk_builder_get_object(builder, "baixo_button"));


	esquerda = GTK_BUTTON(gtk_builder_get_object(builder, "esquerda_button"));
	

	direira = GTK_BUTTON(gtk_builder_get_object(builder, "direita_button"));


	tras = GTK_BUTTON(gtk_builder_get_object(builder, "tras_button"));
	

	frente = GTK_BUTTON(gtk_builder_get_object(builder, "frente_button"));


	adicionar_objeto = GTK_BUTTON(gtk_builder_get_object(builder, "adicionar_objeto_button"));
	

	modificar_objeto = GTK_BUTTON(gtk_builder_get_object(builder, "modficar_objeto_button"));


	rotacionar_direita = GTK_BUTTON(gtk_builder_get_object(builder, "rotacionar_direita_button"));

	rotacionar_esquerda = GTK_BUTTON(gtk_builder_get_object(builder, "rotacionar_esquerda_button"));
	
	/* Buttons add_geometric_shape_w, just open a new window*/
	

	/* Delete action windows*/
	
    
   
	/* Buttons Change object */
	

	
	


	gtk_widget_show(GTK_WIDGET(window_principal));

	gtk_main ();

	return 0;
}	 	  	 	  	 	     	  		  	  	    	      	 	
