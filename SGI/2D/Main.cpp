#include <iostream>
#include <gtk/gtk.h>
#include "MainWindow.hpp"

GtkBuilder * builder = NULL;

// chamadas 
extern "C"
{

	void change_alg_event(GtkToggleButton *button, MainWindow* janela)
    {
        int btnId = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "ID"));
        AlgoritmoClipping alg = (AlgoritmoClipping) btnId;
        janela->clipAlg(alg);
    }

    void add_pnt_event(GtkMenuItem *menuitem, MainWindow* janela)
    {
        janela->addPonto(builder);
    }

    void add_line_event(GtkMenuItem *menuitem, MainWindow* janela)
    {
        janela->addReta(builder);
    }

    void add_poly_event(GtkMenuItem *menuitem, MainWindow* janela)
    {
        janela->addPoligono(builder);
    }

    void add_curve_event(GtkMenuItem *menuitem, MainWindow* janela)
    {
        janela->addCurva2(builder);
    }
    void add_bspline_curve_event(GtkMenuItem *menuitem, MainWindow* janela)
    {
        janela->addCurvaBSpline(builder);
    }

    void add_poly_coord_event(GtkWidget *button, PolygonDialog* dialog)
    {
        dialog->onClickEvent();
    }

    bool on_draw_event(GtkWidget *widget, cairo_t *cr, MainWindow* janela)
    {
        janela->onDraw(cr);
        return true;
    }

    void main_btns_event(GtkWidget *button, MainWindow* janela)
    {
        int btnId = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "ID"));
        Buttons b = (Buttons)btnId;

        switch(b)
        {
            case Buttons::ZOOM_OUT:
            case Buttons::ZOOM_IN:
                janela->zoom(b);
                break;
            case Buttons::UP:
            case Buttons::RIGHT:
            case Buttons::DOWN:
            case Buttons::LEFT:
                janela->move(b);
                break;
            case Buttons::ROTATE_L:
            case Buttons::ROTATE_R:
                janela->rotateW(b);
                break;
        }
    }

    bool main_key_press_event(GtkWidget *widget, GdkEventKey *event, MainWindow *janela)
    {
        switch(event->keyval)
        {
            case GDK_KEY_Up:
                janela->move(Buttons::UP);
                return true;
            case GDK_KEY_Right:
                if (event->state & GDK_CONTROL_MASK)
                    janela->rotateW(Buttons::ROTATE_R);
                else
                    janela->move(Buttons::RIGHT);
                return true;
            case GDK_KEY_Down:
                janela->move(Buttons::DOWN);
                return true;
            case GDK_KEY_Left:
                if (event->state & GDK_CONTROL_MASK)
                    janela->rotateW(Buttons::ROTATE_L);
                else
                    janela->move(Buttons::LEFT);
                return true;
            case GDK_KEY_plus:
            case GDK_KEY_KP_Add:
                janela->zoom(Buttons::ZOOM_IN);
                return true;
            case GDK_KEY_minus:
            case GDK_KEY_KP_Subtract:
                janela->zoom(Buttons::ZOOM_OUT);
                return true;
            default:
                return false;
        }
    }

    bool show_pop_up_event(GtkWidget *widget, GdkEvent *event, MainWindow *janela)
    {
        if(event->type == GDK_BUTTON_PRESS && event->button.button == 3)
            janela->showPopUp(event);

        return false;
    }

    void remove_obj_event(GtkMenuItem *menuitem, MainWindow* janela)
    {
        janela->removeObj();
    }

    void translate_obj_event(GtkMenuItem *menuitem, MainWindow* janela)
    {
        janela->translacaoObj(builder);
    }

    void scale_obj_event(GtkMenuItem *menuitem, MainWindow* janela)
    {
        janela->escalonarObj(builder);
    }

    void rotate_obj_event(GtkMenuItem *menuitem, MainWindow* janela)
    {
        janela->rotacaoObj(builder);
    }

    void add_poly_x_cell_edited (GtkCellRendererText *cell, const gchar *path_string, 
                                 const gchar *new_text, PolygonDialog* dialog)
    {
        dialog->onEditCelEvent(cell, path_string, new_text, 0);
    }

    void add_poly_y_cell_edited (GtkCellRendererText *cell, const gchar *path_string,
                                 const gchar *new_text, PolygonDialog* dialog)
    {
        dialog->onEditCelEvent(cell, path_string, new_text, 1);
    }
}

// Método principal
// Status atual: Implementar spline adequadamente
int main(int argc, char ** argv)
{
    // Iniciar o GTK
    gtk_init( &argc, &argv );

    builder = gtk_builder_new();
    MainWindow* janela = new MainWindow(GTK_BUILDER(builder));
    gtk_builder_connect_signals( GTK_BUILDER(builder), janela );

    gtk_main();
    return 0;
}