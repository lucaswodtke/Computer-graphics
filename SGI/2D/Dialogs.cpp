#include "Dialogs.hpp"

// Implementação dos diálogos

int Dialog::run()
{
    if(_dialog == NULL) return 0;
    return gtk_dialog_run(GTK_DIALOG(_dialog));
}

void Dialog::destroy()
{
    if(_dialog != NULL)
    {
        gtk_widget_destroy(_dialog);
        _dialog = NULL;
    }
}

void PolygonDialog::destroy()
{
    if(_model != NULL)
    {
        gtk_list_store_clear(GTK_LIST_STORE(_model));
        _model = NULL;
    }
}

PointDialog::PointDialog(GtkBuilder* builder)
{
    GError* error = NULL;
    char* ids[] = {(char*)"dlog_add_pnt",(char*)"adj_x1",
        (char*)"adj_y1",NULL};

    if(!gtk_builder_add_objects_from_file (builder, UI_FILE, ids, &error))
    {
        g_warning( "%s", error->message );
        g_free( error );
        return;
    }

    _dialog = GTK_WIDGET( gtk_builder_get_object( builder, "dlog_add_pnt" ) );
    _name = GTK_WIDGET( gtk_builder_get_object( builder, "point_name" ) );
    _x = GTK_WIDGET( gtk_builder_get_object( builder, "point_x" ) );
    _y = GTK_WIDGET( gtk_builder_get_object( builder, "point_y" ) );

    gtk_builder_connect_signals(builder, this);
}

LineDialog::LineDialog(GtkBuilder* builder)
{
    GError* error = NULL;
    char* ids[] = {(char*)"dlog_add_line",(char*)"adj_x1",
        (char*)"adj_y1",(char*)"adj_x2",(char*)"adj_y2",NULL};

    if(!gtk_builder_add_objects_from_file (builder, UI_FILE, ids, &error))
    {
        g_warning( "%s", error->message );
        g_free( error );
        return;
    }

    _dialog = GTK_WIDGET( gtk_builder_get_object( builder, "dlog_add_line" ) );
    _name = GTK_WIDGET( gtk_builder_get_object( builder, "line_name" ) );
    _x1 = GTK_WIDGET( gtk_builder_get_object( builder, "line_x1" ) );
    _y1 = GTK_WIDGET( gtk_builder_get_object( builder, "line_y1" ) );
    _x2 = GTK_WIDGET( gtk_builder_get_object( builder, "line_x2" ) );
    _y2 = GTK_WIDGET( gtk_builder_get_object( builder, "line_y2" ) );

    gtk_builder_connect_signals(builder, this);
}

PolygonDialog::PolygonDialog(GtkBuilder* builder)
{
    GError* error = NULL;
    char* ids[] = {(char*)"poly_list_store",(char*)"adj_x1",(char*)"adj_y1",
        (char*)"dlog_add_poly",NULL};

    if(!gtk_builder_add_objects_from_file(builder, UI_FILE, ids, &error))
    {
        g_warning( "%s", error->message );
        g_free( error );
        return;
    }

    _dialog = GTK_WIDGET( gtk_builder_get_object( builder, "dlog_add_poly" ) );
    _name = GTK_WIDGET( gtk_builder_get_object( builder, "poly_name" ) );
    _x = GTK_WIDGET( gtk_builder_get_object( builder, "poly_x" ) );
    _y = GTK_WIDGET( gtk_builder_get_object( builder, "poly_y" ) );
    _checkFilled = GTK_WIDGET( gtk_builder_get_object( builder, "poly_filled" ) );

    GtkTreeView* tree = GTK_TREE_VIEW( gtk_builder_get_object( GTK_BUILDER(builder), "poly_treeview" ) );
    _model = gtk_tree_view_get_model(tree);

    gtk_builder_connect_signals(builder, this);
}

CurveDialog::CurveDialog(GtkBuilder* builder): PolygonDialog(builder) 
{
    gtk_widget_hide(GTK_WIDGET(_checkFilled));
    gtk_window_set_title(GTK_WINDOW(_dialog), "Adicionar Curva");
}

TransDialog::TransDialog(GtkBuilder* builder)
{
    GError* error = NULL;
    char* ids[] = {(char*)"dlog_translate",(char*)"adj_x1", (char*)"adj_y1",NULL};

    if(!gtk_builder_add_objects_from_file (builder, UI_FILE, ids, &error))
    {
        g_warning( "%s", error->message );
        g_free( error );
        return;
    }

    _dialog = GTK_WIDGET( gtk_builder_get_object( builder, "dlog_translate" ) );
    _dx = GTK_WIDGET( gtk_builder_get_object( builder, "dx" ) );
    _dy = GTK_WIDGET( gtk_builder_get_object( builder, "dy" ) );
}

ScaDialog::ScaDialog(GtkBuilder* builder)
{
    GError* error = NULL;
    char* ids[] = {(char*)"dlog_scale",(char*)"adj_x1", (char*)"adj_y1",NULL};

    if(!gtk_builder_add_objects_from_file (builder, UI_FILE, ids, &error))
    {
        g_warning( "%s", error->message );
        g_free( error );
        return;
    }

    _dialog = GTK_WIDGET( gtk_builder_get_object( builder, "dlog_scale" ) );
    _sx = GTK_WIDGET( gtk_builder_get_object( builder, "sx" ) );
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(_sx), 1.0);
    _sy = GTK_WIDGET( gtk_builder_get_object( builder, "sy" ) );
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(_sy), 1.0);
}

RotDialog::RotDialog(GtkBuilder* builder)
{
    GError* error = NULL;
    char* ids[] = {(char*)"dlog_rotate",(char*)"adj_x1",
        (char*)"adj_y1",(char*)"adj_angulo",NULL};
    if(!gtk_builder_add_objects_from_file (builder, UI_FILE, ids, &error))
    {
        g_warning( "%s", error->message );
        g_free( error );
        return;
    }

    _dialog = GTK_WIDGET( gtk_builder_get_object( builder, "dlog_rotate" ) );
    _cx = GTK_WIDGET( gtk_builder_get_object( builder, "cx" ) );
    _cy = GTK_WIDGET( gtk_builder_get_object( builder, "cy" ) );
    _angulo = GTK_WIDGET( gtk_builder_get_object( builder, "ang" ) );
    _centroObjeto = GTK_WIDGET( gtk_builder_get_object( builder, "t1" ) );
    _centroMundo = GTK_WIDGET( gtk_builder_get_object( builder, "t2" ) );
    _ponto = GTK_WIDGET( gtk_builder_get_object( builder, "t3" ) );
}

rotacao RotDialog::getTipo()
{
    if(isActive(_centroObjeto))
        return rotacao::OBJETO;
    else if(isActive(_centroMundo))
        return rotacao::MUNDO;
    else
        return rotacao::PONTO;
}

void PolygonDialog::onClickEvent()
{
    GtkListStore *liststore = GTK_LIST_STORE(_model);
    GtkTreeIter iter;
    double x = gtk_spin_button_get_value(GTK_SPIN_BUTTON(_x));
    double y = gtk_spin_button_get_value(GTK_SPIN_BUTTON(_y));
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter, 0, x, 1, y, -1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(_x), 0.0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(_y), 0.0);
    gtk_widget_grab_focus(_x);
}

void PolygonDialog::onEditCelEvent(GtkCellRendererText *cell, const gchar *path_string, const gchar *new_text, int column)
{
    GtkTreePath *path = gtk_tree_path_new_from_string (path_string);
    GtkTreeIter iter;
    gtk_tree_model_get_iter (_model, &iter, path);
    gtk_list_store_set (GTK_LIST_STORE (_model), &iter, column, atof(new_text), -1);
}

void PolygonDialog::getCoords(Coordinates& coords)
{
    GtkTreeIter iter;
    gboolean valid;
    double x, y;

    valid = gtk_tree_model_get_iter_first (_model, &iter);

    if(!valid)
        throw "Nenhuma coordenada encontrada!";

    while(valid)
    {
        gtk_tree_model_get (_model, &iter, 0, &x, 1, &y, -1);
        coords.emplace_back(x, y);
        valid = gtk_tree_model_iter_next (_model, &iter);
    }
}
