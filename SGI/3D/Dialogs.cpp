#include "Dialogs.hpp"
#include <sstream>

FileDialog::FileDialog(GtkBuilder* builder, bool toSave)
{
    GError* error = nullptr;
    char* ids[] = {(char*)"dlog_file",(char*)"obj_filter",nullptr};
    if(!gtk_builder_add_objects_from_file (builder, UI_FILE, ids, &error))
    {
        g_warning( "%s", error->message );
        g_free( error );
        return;
    }
    _dialog = GTK_WIDGET( gtk_builder_get_object(builder, "dlog_file"));
    if(toSave)
    {
        gtk_window_set_title (GTK_WINDOW(_dialog), "Salve o arquivo...");
        gtk_file_chooser_set_action(GTK_FILE_CHOOSER(_dialog), GTK_FILE_CHOOSER_ACTION_SAVE);
        gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(_dialog), true);
        gtk_file_chooser_set_create_folders(GTK_FILE_CHOOSER(_dialog), true);
        gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER(_dialog), "untitled.obj");
    }
}

int Dialog::run()
{
    if(_dialog == NULL) return -1;
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

void Object3dDialog::destroy()
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
    char* ids[] = {(char*)"dlog_add_pnt",(char*)"adj_x1", (char*)"adj_y1",(char*)"adj_z1",NULL};
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
    _z = GTK_WIDGET( gtk_builder_get_object( builder, "point_z" ) );
    gtk_builder_connect_signals(builder, this);
}

LineDialog::LineDialog(GtkBuilder* builder)
{
    GError* error = NULL;
    char* ids[] = {(char*)"dlog_add_line",(char*)"adj_x1", (char*)"adj_y1", (char*)"adj_z1", (char*)"adj_x2", (char*)"adj_y2", (char*)"adj_z2", NULL};
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
    _z1 = GTK_WIDGET( gtk_builder_get_object( builder, "line_z1" ) );
    _x2 = GTK_WIDGET( gtk_builder_get_object( builder, "line_x2" ) );
    _y2 = GTK_WIDGET( gtk_builder_get_object( builder, "line_y2" ) );
    _z2 = GTK_WIDGET( gtk_builder_get_object( builder, "line_z2" ) );
    gtk_builder_connect_signals(builder, this);
}

PolygonDialog::PolygonDialog(GtkBuilder* builder, bool isFace)
{
    GError* error = NULL;
    char* ids[] = {(char*)"poly_list_store",(char*)"adj_x1",(char*)"adj_y1", (char*)"adj_z1", (char*)"dlog_add_poly",NULL};
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
    _z = GTK_WIDGET( gtk_builder_get_object( builder, "poly_z" ) );
    _checkFilled = GTK_WIDGET( gtk_builder_get_object( builder, "poly_filled" ) );
    GtkTreeView* tree = GTK_TREE_VIEW( gtk_builder_get_object( GTK_BUILDER(builder), "poly_treeview" ) );
    _model = gtk_tree_view_get_model(tree);
    gtk_widget_hide(_checkFilled);
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
    char* ids[] = {(char*)"dlog_translate",(char*)"adj_x1",
        (char*)"adj_y1", (char*)"adj_z1", NULL};
    if(!gtk_builder_add_objects_from_file (builder, UI_FILE, ids, &error))
    {
        g_warning( "%s", error->message );
        g_free( error );
        return;
    }
    _dialog = GTK_WIDGET( gtk_builder_get_object( builder, "dlog_translate"));
    _dx = GTK_WIDGET( gtk_builder_get_object( builder, "dx" ) );
    _dy = GTK_WIDGET( gtk_builder_get_object( builder, "dy" ) );
    _dz = GTK_WIDGET( gtk_builder_get_object( builder, "dz" ) );
}

ScaDialog::ScaDialog(GtkBuilder* builder)
{
    GError* error = NULL;
    char* ids[] = {(char*)"dlog_scale",(char*)"adj_x1",
        (char*)"adj_y1", (char*)"adj_z1", NULL};
    if(!gtk_builder_add_objects_from_file (builder, UI_FILE, ids, &error))
    {
        g_warning( "%s", error->message );
        g_free( error );
        return;
    }
    _dialog = GTK_WIDGET( gtk_builder_get_object( builder, "dlog_scale"));
    _sx = GTK_WIDGET( gtk_builder_get_object( builder, "sx" ) );
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(_sx), 1.0);
    _sy = GTK_WIDGET( gtk_builder_get_object( builder, "sy" ) );
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(_sy), 1.0);
    _sz = GTK_WIDGET( gtk_builder_get_object( builder, "sz" ) );
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(_sz), 1.0);
}

RotDialog::RotDialog(GtkBuilder* builder)
{
    GError* error = NULL;
    char* ids[] = {(char*)"dlog_rotate",(char*)"adj_x1", (char*)"adj_y1",(char*)"adj_z1",(char*)"adj_ang_x", (char*)"adj_ang_y",(char*)"adj_ang_z",(char*)"adj_angulo",NULL};
    if(!gtk_builder_add_objects_from_file (builder, UI_FILE, ids, &error))
    {
        g_warning( "%s", error->message );
        g_free( error );
        return;
    }
    _dialog = GTK_WIDGET( gtk_builder_get_object( builder, "dlog_rotate" ) );
    _cx = GTK_WIDGET( gtk_builder_get_object( builder, "cx" ) );
    _cy = GTK_WIDGET( gtk_builder_get_object( builder, "cy" ) );
    _cz = GTK_WIDGET( gtk_builder_get_object( builder, "cz" ) );
    _anguloX = GTK_WIDGET( gtk_builder_get_object( builder, "angX" ) );
    _anguloY = GTK_WIDGET( gtk_builder_get_object( builder, "angY" ) );
    _anguloZ = GTK_WIDGET( gtk_builder_get_object( builder, "angZ" ) );
    _angulo = GTK_WIDGET( gtk_builder_get_object( builder, "ang" ) );
    _centroObjeto = GTK_WIDGET( gtk_builder_get_object( builder, "t1" ) );
    _ponto = GTK_WIDGET( gtk_builder_get_object( builder, "t3" ) );
}

rotacao RotDialog::getTipo()
{
    if(isActive(_centroObjeto))
        return rotacao::OBJETO;
    else
        return rotacao::PONTO;
}

void PolygonDialog::onClickEvent()
{
    GtkListStore *liststore = GTK_LIST_STORE(_model);
    GtkTreeIter iter;
    double x = gtk_spin_button_get_value(GTK_SPIN_BUTTON(_x));
    double y = gtk_spin_button_get_value(GTK_SPIN_BUTTON(_y));
    double z = gtk_spin_button_get_value(GTK_SPIN_BUTTON(_z));
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter, 0, x, 1, y, 2, z, -1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(_x), 0.0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(_y), 0.0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(_z), 0.0);
    gtk_widget_grab_focus(_x);
}

Object3dDialog::Object3dDialog(GtkBuilder* builder)
{
    GError* error = nullptr;
    char* ids[] = {(char*)"obj3d_list_store",(char*)"dlog_add_obj3d",nullptr};
    if(!gtk_builder_add_objects_from_file(builder, UI_FILE, ids, &error))
    {
        g_warning( "%s", error->message );
        g_free( error );
        return;
    }
    _dialog = GTK_WIDGET( gtk_builder_get_object( builder, "dlog_add_obj3d" ) );
    _name = GTK_WIDGET( gtk_builder_get_object( builder, "obj3d_name" ) );
    GtkTreeView* tree = GTK_TREE_VIEW( gtk_builder_get_object( GTK_BUILDER(builder), "obj3d_treeview" ) );
    _model = gtk_tree_view_get_model(tree);
    gtk_builder_connect_signals(builder, this);
}

void Object3dDialog::onClickEvent(GtkBuilder* builder)
{
    GtkListStore *liststore = GTK_LIST_STORE(_model);
    GtkTreeIter iter;
    PolygonDialog dlogPoly(GTK_BUILDER(builder), true);
    if(dlogPoly.run() != 1)
        return;
    Coordinates c;
    dlogPoly.getCoords(c);
    std::string name = "face"+std::to_string(_faces.size()+1);
    _faces.emplace_back(name, c);
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter, 0, name.c_str(), -1);
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
    double x=0.0, y=0.0, z=0.0;
    valid = gtk_tree_model_get_iter_first (_model, &iter);
    if(!valid)
        throw "Nenhuma coordenada encontrada!";
    while(valid)
    {
        gtk_tree_model_get (_model, &iter, 0, &x, 1, &y, 2, &z, -1);
        coords.emplace_back(x, y, z);
        valid = gtk_tree_model_iter_next (_model, &iter);
    }
}

SurfaceDialog::SurfaceDialog(GtkBuilder* builder)
{
    GError* error = nullptr;
    char* ids[] = {(char*)"surface_comboBox",(char*)"adj_x1",(char*)"adj_y1", (char*)"adj_z1",(char*)"dlog_add_surface",nullptr};
    if(!gtk_builder_add_objects_from_file(builder, UI_FILE, ids, &error))
    {
        g_warning( "%s", error->message );
        g_free( error );
        return;
    }
    _dialog = GTK_WIDGET( gtk_builder_get_object( builder, "dlog_add_surface" ) );
    _name = GTK_WIDGET( gtk_builder_get_object( builder, "surface_name" ) );
    _x = GTK_WIDGET( gtk_builder_get_object( builder, "surface_x" ) );
    _y = GTK_WIDGET( gtk_builder_get_object( builder, "surface_y" ) );
    _z = GTK_WIDGET( gtk_builder_get_object( builder, "surface_z" ) );
    _grid = GTK_WIDGET( gtk_builder_get_object( builder, "surface_grid" ) );
    _comboBox = GTK_WIDGET( gtk_builder_get_object( builder, "surface_side" ) );
    gtk_combo_box_set_active(GTK_COMBO_BOX(_comboBox), 0);
    _btnAddSurface = GTK_WIDGET( gtk_builder_get_object( builder, "btn_add_surface" ) );
    GObject* tmpBezier = G_OBJECT(gtk_builder_get_object( builder, "bezier_surface" ));
    _rbBezier = GTK_WIDGET( tmpBezier );
    g_object_set_data(tmpBezier, "ID", GINT_TO_POINTER(ObjType::SUPERFICIE_BEZIER));
    GObject* tmpBSpline = G_OBJECT(gtk_builder_get_object( builder, "bspline_surface" ));
    _rbBSpline = GTK_WIDGET( tmpBSpline );
    g_object_set_data(tmpBSpline, "ID", GINT_TO_POINTER(ObjType::SUPERFICIE_BSPLINE));
    gtk_builder_connect_signals(builder, this);
}

void SurfaceDialog::setTipoSuperficie(ObjType tipo)
{
    if(tipo != ObjType::SUPERFICIE_BEZIER && tipo != ObjType::SUPERFICIE_BSPLINE)
        return;
    _tipo = tipo;
}

const Coordinates& SurfaceDialog::getCoords() const
{
    int size = _coords.size();
    if(size < (m_max_i*m_max_j))
        throw "Preencha todas as coordenadas.";
    return _coords;
}

void SurfaceDialog::addSurfaceEvent()
{
    gtk_widget_set_sensitive(GTK_WIDGET(_rbBezier), false);
    gtk_widget_set_sensitive(GTK_WIDGET(_rbBSpline), false);
    int num = 0;
    if(_tipo == ObjType::SUPERFICIE_BEZIER)
    {
        num = 3;
    }
    else if(_tipo == ObjType::SUPERFICIE_BSPLINE)
    {
        num = 1;
    }
    char *tmpSide = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(_comboBox));
    std::string side(tmpSide);
    delete tmpSide;
    if(side == "Ao lado")
    {
        for(int col = 0; col < num; col++)
        {
            gtk_grid_insert_column(GTK_GRID(_grid), m_max_j);
            for(int line = 0; line < m_max_i; line++)
            {
                gtk_grid_attach (GTK_GRID(_grid), gtk_label_new(NULL), m_max_j, line, 1, 1);
                gtk_widget_show(gtk_grid_get_child_at(GTK_GRID(_grid), m_max_j, line));
            }
            ++m_max_j;
        }
    }
    else if(side == "Abaixo")
    {
        for(int line = 0; line < num; line++)
        {
            gtk_grid_insert_row(GTK_GRID(_grid), m_max_i);
            for(int col = 0; col < m_max_j; col++)
            {
                gtk_grid_attach (GTK_GRID(_grid), gtk_label_new(NULL), col, m_max_i, 1, 1);
                gtk_widget_show(gtk_grid_get_child_at(GTK_GRID(_grid), col, m_max_i));
            }
            ++m_max_i;
        }
    }
}

void SurfaceDialog::addCoordEvent()
{
    gtk_widget_set_sensitive(GTK_WIDGET(_rbBezier), false);
    gtk_widget_set_sensitive(GTK_WIDGET(_rbBSpline), false);
    gtk_widget_set_sensitive(GTK_WIDGET(_comboBox), false);
    gtk_widget_set_sensitive(GTK_WIDGET(_btnAddSurface), false);
    if(m_i == m_max_i)
        return;
    std::stringstream fmt;
    fmt << "(" << getX() << ", " << getY() << ", " << getZ() << ")";
    std::string coord = fmt.str();
    _coords.emplace_back(getX(), getY(), getZ());
    GtkLabel* label = GTK_LABEL(gtk_grid_get_child_at(GTK_GRID(_grid), m_j++, m_i));
    gtk_label_set_text(label, coord.c_str());
    if(m_j == m_max_j)
    {
        m_j = 0;
        m_i++;
    }
    gtk_widget_grab_focus(_x);
}
