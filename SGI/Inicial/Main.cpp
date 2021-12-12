
#include <string>
#include <iostream>
#include <gtk/gtk.h>
#include "ListaEnc.hpp"
#include "Testes.hpp"

using namespace std;

// Links
// GTK:
// https://developer.gnome.org/gtk3/3.0/gtk-getting-started.html
// C++ Reference:
// https://www.tutorialspoint.com/cplusplus/cpp_quick_guide.htm

static void print_hello (GtkWidget *widget, gpointer data)
{
  g_print ("Hello World\n");
}
static void print_hello2 (GtkWidget *widget, gpointer data)
{
  g_print ("Hello World2\n");
}
static void print_hello3 (GtkWidget *widget, gpointer data)
{
  g_print ("Hello World3\n");
}
static void print_hello4 (GtkWidget *widget, gpointer data)
{
  g_print ("Hello World4\n");
}

int main(int argc, char *argv[])
{
  GtkBuilder *builder;
  GObject *window;
  GObject *button;

  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "ui.glade", NULL);

  /* Connect signal handlers to the constructed widgets. */
  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  button = gtk_builder_get_object (builder, "button1");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);

  button = gtk_builder_get_object (builder, "button2");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello2), NULL);
  
  button = gtk_builder_get_object (builder, "button3");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello3), NULL);

  button = gtk_builder_get_object (builder, "button4");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello4), NULL);

  button = gtk_builder_get_object (builder, "quit");
  g_signal_connect (button, "clicked", G_CALLBACK (gtk_main_quit), NULL);

  gtk_main ();

  return 0;
}
