#include <iostream>
#include <gtk/gtk.h>
#include "MainWindow.hpp"

GtkBuilder * builder = NULL;

// Chamadas principais
extern "C"
{
    // Abrir arquivo .obj
    void open_file_event(GtkMenuItem *menuitem, MainWindow* window)
    {
        window->openFile(builder);
    }

    // Salvar arquivo .obj
    void save_file_event(GtkMenuItem *menuitem, MainWindow* window)
    {
        window->saveFile(builder);
    }

    // Adicionar superfície
    void add_surface_event(GtkMenuItem *menuitem, MainWindow* window)
    {
        window->addSuperficie(builder);
    }

    // Adicionar coordenadas da superfície
    void add_surface_coord_event(GtkWidget *button, SurfaceDialog* dialog)
    {
        dialog->addCoordEvent();
    }

    // Adicionar grid da superfície
    void add_grid_surface_event(GtkWidget *button, SurfaceDialog* dialog)
    {
        dialog->addSurfaceEvent();
    }

    // Mudar superfície
    void change_surface_type_event(GtkToggleButton *button, SurfaceDialog* dialog)
    {
        int btnId = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "ID"));
        ObjType type = (ObjType) btnId;
        dialog->setTipoSuperficie(type);
    }

    // Mudar algoritmo de clipping
    void change_alg_event(GtkToggleButton *button, MainWindow* window)
    {
        int btnId = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "ID"));
        AlgoritmoClipping alg = (AlgoritmoClipping) btnId;
        window->clipAlg(alg);
    }

    // Adicionar ponto
    void add_pnt_event(GtkMenuItem *menuitem, MainWindow* window)
    {
        window->addPonto(builder);
    }

    // Adicionar reta
    void add_line_event(GtkMenuItem *menuitem, MainWindow* window)
    {
        window->addReta(builder);
    }

    // Adicionar polígono
    void add_poly_event(GtkMenuItem *menuitem, MainWindow* window)
    {
        window->addPoligono(builder);
    }

    // Adicionar bezier
    void add_curve_event(GtkMenuItem *menuitem, MainWindow* window)
    {
        window->addCurvaBezier(builder);
    }

    // Adicionar spline
    void add_bspline_curve_event(GtkMenuItem *menuitem, MainWindow* window)
    {
        window->addCurvaBSpline(builder);
    }
    
    // Adicionar objeto tridimensional
    void add_obj3d_event(GtkMenuItem *menuitem, MainWindow* window)
    {
        window->addObj3D(builder);
    }

    // Adicionar face de objeto
    void add_face_event(GtkWidget *button, Object3dDialog* dialog)
    {
        dialog->onClickEvent(builder);
    }

    // Adicionar coordenadas no polígono
    void add_poly_coord_event(GtkWidget *button, PolygonDialog* dialog)
    {
        dialog->onClickEvent();
    }
    
    // Update on draw cairo
    bool on_draw_event(GtkWidget *widget, cairo_t *cr, MainWindow* window)
    {
        window->onDraw(cr);
        return true;
    }

    // Mostrar diálogo
    bool show_pop_up_event(GtkWidget *widget, GdkEvent *event, MainWindow *window)
    {
        if(event->type == GDK_BUTTON_PRESS && event->button.button == 3)
            window->showPopUp(event);
        return false;
    }

    // Deletar objeto
    void remove_obj_event(GtkMenuItem *menuitem, MainWindow* window)
    {
        window->removeObj();
    }

    // Transladar objeto
    void translate_obj_event(GtkMenuItem *menuitem, MainWindow* window)
    {
        window->translacaoObj(builder);
    }

    // Escalar objeto
    void scale_obj_event(GtkMenuItem *menuitem, MainWindow* window)
    {
        window->escalonarObj(builder);
    }

    // Roatacionar objeto
    void rotate_obj_event(GtkMenuItem *menuitem, MainWindow* window)
    {
        window->rotacaoObj(builder);
    }

    // Edição do X
    void add_poly_x_cell_edited (GtkCellRendererText *cell, const gchar *path_string,
                                 const gchar *new_text, PolygonDialog* dialog)
    {
        dialog->onEditCelEvent(cell, path_string, new_text, 0);
    }

    // Edição do Y
    void add_poly_y_cell_edited (GtkCellRendererText *cell, const gchar *path_string,
                                 const gchar *new_text, PolygonDialog* dialog)
    {
        dialog->onEditCelEvent(cell, path_string, new_text, 1);
    }

    // Edição do Z
    void add_poly_z_cell_edited(GtkCellRendererText *cell, const gchar *path_string,
                                const gchar *new_text, PolygonDialog* dialog)
    {
        dialog->onEditCelEvent(cell, path_string, new_text, 2);
    }

    // Mudar projeção
    void change_proj_event(GtkToggleButton *button, MainWindow* window)
    {
        // TODO FIX
    }

    // BOTÕES DO MENU
    void main_btns_event(GtkWidget *button, MainWindow* window)
    {
        int btnId = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "ID"));
        Buttons b = (Buttons)btnId;

        switch(b)
        {
            case Buttons::ZOOM_OUT:
            case Buttons::ZOOM_IN:
                window->zoom(b);
                break;
            case Buttons::UP:
            case Buttons::RIGHT:
            case Buttons::DOWN:
            case Buttons::LEFT:
                window->move(b);
                break;
            case Buttons::ROTATE_L:
            case Buttons::ROTATE_R:
                window->rotateW(b);
                break;
        }
    }

    // ATALHOS DE TECLADO
    bool main_key_press_event(GtkWidget *widget, GdkEventKey *event, MainWindow *window)
    {
        switch(event->keyval)
        {
            case GDK_KEY_Up:
                window->move(Buttons::UP);
                return true;
            case GDK_KEY_Right:
                if (event->state & GDK_CONTROL_MASK)
                    window->rotateW(Buttons::ROTATE_R);
                else
                    window->move(Buttons::RIGHT);
                return true;
            case GDK_KEY_Down:
                window->move(Buttons::DOWN);
                return true;
            case GDK_KEY_Left:
                if (event->state & GDK_CONTROL_MASK)
                    window->rotateW(Buttons::ROTATE_L);
                else
                    window->move(Buttons::LEFT);
                return true;
            case GDK_KEY_plus:
            case GDK_KEY_KP_Add:
                window->zoom(Buttons::ZOOM_IN);
                return true;
            case GDK_KEY_minus:
            case GDK_KEY_KP_Subtract:
                window->zoom(Buttons::ZOOM_OUT);
                return true;
            case GDK_KEY_1:
            case GDK_KEY_x:
            case GDK_KEY_X:
                window->setEixo(Eixos::X);
                return true;
            case GDK_KEY_2:    
            case GDK_KEY_y:
            case GDK_KEY_Y:
                window->setEixo(Eixos::Y);
                return true;
            case GDK_KEY_3:  
            case GDK_KEY_z:
            case GDK_KEY_Z:
                window->setEixo(Eixos::Z);
                return true;
            default:
                return false;
        }
    }

} // Fim extern

// Método principal
int main(int argc, char ** argv)
{
    gtk_init( &argc, &argv );
    builder = gtk_builder_new();
    MainWindow* window = new MainWindow(GTK_BUILDER(builder));
    gtk_builder_connect_signals( GTK_BUILDER(builder), window );
    gtk_main();
    return 0;
}
