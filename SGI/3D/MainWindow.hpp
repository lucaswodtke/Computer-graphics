#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <gtk/gtk.h>
#include <string>
#include "Viewport.hpp"
#include "Dialogs.hpp"
#include "Objetos.hpp"
#include "World.hpp"
#include "FileHandlers.hpp"

#define UI_FILE "ui.glade"

enum class Buttons { ZOOM_OUT, ZOOM_IN, UP, RIGHT, DOWN, LEFT, ROTATE_L, ROTATE_R };
enum class Eixos { X, Y, Z };

class MainWindow
{
    public:
        MainWindow(GtkBuilder* builder);
        virtual ~MainWindow() { delete _world; delete _viewport; }
        void addPonto(GtkBuilder* builder);
        void addReta(GtkBuilder* builder);
        void addPoligono(GtkBuilder* builder);
	    void addCurvaBezier(GtkBuilder* builder);
	    void addCurvaBSpline(GtkBuilder* builder);
	    void addObj3D(GtkBuilder* builder);
	    void addSuperficie(GtkBuilder* builder);
        void removeObj();
        void zoom(Buttons id);
        void move(Buttons id);
        void rotateW(Buttons id);
	    void setEixo(Eixos eixo);
        void onDraw(cairo_t* cr);
        void showPopUp(GdkEvent *event);
        void translacaoObj(GtkBuilder* builder);
        void escalonarObj(GtkBuilder* builder);
        void rotacaoObj(GtkBuilder* builder);
	    void clipAlg(AlgoritmoClipping alg);
        void openFile(GtkBuilder* builder);
        void saveFile(GtkBuilder* builder);
	    //void projecaoTipo(); TODO
    private:
        bool getNameObj(std::string &name, GtkTreeIter *iter);
        void showErrorDialog(const char* msg);
        void addObjOnListStore(const std::string name, const char* type);
        GtkWidget* _mainWindow, *_step, *_log, *_popUp, *_logScroll, *_eixos;
        GtkWidget *_helpDialog;
        GtkTreeModel* _mainModel;
        GtkTreeSelection* _treeSelection;
        Viewport *_viewport;
        World *_world;

};

void MainWindow::openFile(GtkBuilder* builder)
{
    FileDialog dialog(GTK_BUILDER(builder));
    if(dialog.run() == 1)
    {
        char* filename = dialog.getFileName();
        if(filename == nullptr)
            return;
        std::string file(filename);
        delete filename;
        try
        {
            ObjReader r(file);
            for(auto obj : r.getObjs())
            {
                try
                {
                    _world->addObj(obj);
                    _viewport->update(obj);
                    addObjOnListStore(obj->getName(), obj->getTypeName().c_str());
                    gtk_widget_queue_draw(_mainWindow);
                }
                catch(const char* msg)
                {
                    showErrorDialog(msg);
                        delete obj;
                }
            }
        }
        catch(const char* msg)
        {
            showErrorDialog(msg);
        }
    }
}

void MainWindow::saveFile(GtkBuilder* builder)
{
    FileDialog dialog(GTK_BUILDER(builder), true);
    if(dialog.run() == 1)
    {
        char* filename = dialog.getFileName();
        if(filename == nullptr)
            return;
        std::string file(filename);
        delete filename;
        try
        {
            ObjWriter w(file);
            w.writeObjs(_world);

        }
        catch(const char* msg)
        {
            showErrorDialog(msg);
        }
    }
}

MainWindow::MainWindow(GtkBuilder* builder) 
{
    GError* error = NULL;
    char* ids[] = {(char*)"main_list_store",(char*)"adj_step", (char*)"main_window",(char*)"main_pop_up_menu", NULL};
    if(!gtk_builder_add_objects_from_file (GTK_BUILDER(builder), UI_FILE, ids, &error))
    {
        g_warning( "%s", error->message );
        g_free( error );
        return;
    }
    _mainWindow = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "main_window" ) );
    GtkWidget* area = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "drawing_area" ) );
    GtkRequisition min_size;
    gtk_widget_get_preferred_size(area, &min_size, NULL);
    _world = new World();
    _viewport = new Viewport(min_size.width, min_size.height, _world);
    _step = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "entry_step" ) );
    GtkTreeView* tree = GTK_TREE_VIEW( gtk_builder_get_object( GTK_BUILDER(builder), "main_treeview" ) );
    _mainModel = gtk_tree_view_get_model(tree);
    _treeSelection = gtk_tree_view_get_selection(tree);
    _popUp = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "main_pop_up_menu" ) );
    _eixos = GTK_WIDGET( gtk_builder_get_object( GTK_BUILDER(builder), "comboBox_eixos" ) );
    gtk_combo_box_set_active(GTK_COMBO_BOX(_eixos), 2);
    g_object_set_data(G_OBJECT(gtk_builder_get_object(GTK_BUILDER(builder), "btn_zoom_out")), "ID", GINT_TO_POINTER(Buttons::ZOOM_OUT));
    g_object_set_data(G_OBJECT(gtk_builder_get_object(GTK_BUILDER(builder), "btn_zoom_in")), "ID", GINT_TO_POINTER(Buttons::ZOOM_IN));
    g_object_set_data(G_OBJECT(gtk_builder_get_object(GTK_BUILDER(builder), "btn_up")), "ID", GINT_TO_POINTER(Buttons::UP));
    g_object_set_data(G_OBJECT(gtk_builder_get_object(GTK_BUILDER(builder), "btn_right")), "ID", GINT_TO_POINTER(Buttons::RIGHT));
    g_object_set_data(G_OBJECT(gtk_builder_get_object(GTK_BUILDER(builder), "btn_down")), "ID", GINT_TO_POINTER(Buttons::DOWN));
    g_object_set_data(G_OBJECT(gtk_builder_get_object(GTK_BUILDER(builder), "btn_left")), "ID", GINT_TO_POINTER(Buttons::LEFT));
    g_object_set_data(G_OBJECT(gtk_builder_get_object(GTK_BUILDER(builder), "btn_rotate_l")), "ID", GINT_TO_POINTER(Buttons::ROTATE_L));
    g_object_set_data(G_OBJECT(gtk_builder_get_object(GTK_BUILDER(builder), "btn_rotate_r")), "ID", GINT_TO_POINTER(Buttons::ROTATE_R));
    g_object_set_data(G_OBJECT(gtk_builder_get_object(GTK_BUILDER(builder), "clip_CS")), "ID", GINT_TO_POINTER(AlgoritmoClipping::CS));
    g_object_set_data(G_OBJECT(gtk_builder_get_object(GTK_BUILDER(builder), "clip_LB")), "ID", GINT_TO_POINTER(AlgoritmoClipping::LB));

/*
    g_object_set_data(G_OBJECT(gtk_builder_get_object(GTK_BUILDER(builder), "paralela")),
                       "ID", GINT_TO_POINTER(Projecao::PARALELA));
    g_object_set_data(G_OBJECT(gtk_builder_get_object(GTK_BUILDER(builder), "perspectiva")),
                       "ID", GINT_TO_POINTER(Projecao::PERSPECTIVA));
*/
    gtk_widget_show( _mainWindow );
}

/*
void MainWindow::projecaoTipo(Projecao p)
{
    //_viewport->setProjecao(p);
    //gtk_widget_queue_draw(_mainWindow);
}
*/

void MainWindow::setEixo(Eixos eixo)
{
    gtk_combo_box_set_active(GTK_COMBO_BOX(_eixos), (int)eixo);
}

void MainWindow::showErrorDialog(const char* msg)
{
    GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(_mainWindow), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", msg);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

void MainWindow::addObjOnListStore(const std::string name, const char* type)
{
    GtkListStore *liststore = GTK_LIST_STORE(_mainModel);
    GtkTreeIter iter;
    gtk_list_store_append(liststore, &iter);
    gtk_list_store_set(liststore, &iter, 0, name.c_str(), 1, type, -1);
}

void MainWindow::showPopUp(GdkEvent *event)
{
    gtk_menu_popup(GTK_MENU(_popUp), NULL, NULL, NULL, NULL, event->button.button, event->button.time);
}

bool MainWindow::getNameObj(std::string &name, GtkTreeIter *iter)
{
    if(!gtk_tree_selection_get_selected(GTK_TREE_SELECTION(_treeSelection), NULL, iter))
        return false;
    char *_name;
    gtk_tree_model_get(_mainModel, iter, 0, &_name, -1);
    name = _name;
    delete _name;
    return true;
}

void MainWindow::onDraw(cairo_t* cr)
{
    _viewport->desenhaObjs(cr);
}

void MainWindow::removeObj()
{
    GtkTreeIter iter;
    std::string name;
    if(!getNameObj(name, &iter))
        return;
    try
    {
        _world->removeObj(name);
        gtk_list_store_remove(GTK_LIST_STORE(_mainModel), &iter);
        gtk_widget_queue_draw(_mainWindow);
    }
    catch(const char* msg)
    {
        showErrorDialog(msg);
    }
}

void MainWindow::zoom(Buttons id)
{
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(_step));
    try
    {
        switch(id)
        {
            case Buttons::ZOOM_OUT:
                _viewport->zoomW(value);
                break;
            case Buttons::ZOOM_IN:
                _viewport->zoomW(-value);
                break;
            default:
                break;
        }
        gtk_widget_queue_draw(_mainWindow);
    }
    catch(const char* msg)
    {
        showErrorDialog(msg);
    }
}

void MainWindow::move(Buttons id)
{
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(_step));
    switch(id)
    {
        case Buttons::UP:
            _viewport->moveW(0,value);
            break;
        case Buttons::RIGHT:
            _viewport->moveW(value,0);
            break;
        case Buttons::DOWN:
            _viewport->moveW(0,-value);
            break;
        case Buttons::LEFT:
            _viewport->moveW(-value,0);
            break;
        default:
            break;
    }
    gtk_widget_queue_draw(_mainWindow);
}

void MainWindow::rotateW(Buttons id)
{
    double value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(_step));
    char *tmpEixo = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(_eixos));
    std::string eixo(tmpEixo);
    delete tmpEixo;
    switch(id)
    {
        case Buttons::ROTATE_L:
            _viewport->rotateW(value, eixo);
            break;
        case Buttons::ROTATE_R:
            _viewport->rotateW(-value, eixo);
            break;
        default:
            break;
    }
    gtk_widget_queue_draw(_mainWindow);
}

void MainWindow::addPonto(GtkBuilder* builder)
{
    PointDialog dialog(GTK_BUILDER(builder));
    bool finish = false;
    while(!finish)
    {
        if(dialog.run() == 1)
        {
            try
            {
                Coordinate c(dialog.getX(),dialog.getY(), dialog.getZ());

                Objeto* obj = _world->addPonto(dialog.getName(),c);
                _viewport->update(obj);
                addObjOnListStore(dialog.getName(), "Ponto");

                gtk_widget_queue_draw(_mainWindow);
                finish = true;
            }
            catch(const char* msg)
            {
                showErrorDialog(msg);
            }
        }
        else
            finish = true;
    }
}

void MainWindow::addReta(GtkBuilder* builder)
{
    LineDialog dialog(GTK_BUILDER(builder));
    bool finish = false;
    while(!finish)
    {
        if(dialog.run() == 1)
        {
            try
            {
                Coordinates c;
                c.emplace_back(dialog.getX1(), dialog.getY1(), dialog.getZ1());
                c.emplace_back(dialog.getX2(), dialog.getY2(), dialog.getZ2());
                Objeto* obj = _world->addReta(dialog.getName(), c);
                _viewport->update(obj);
                addObjOnListStore(dialog.getName(), "Reta");
                gtk_widget_queue_draw(_mainWindow);
                finish = true;
            }
            catch(const char* msg)
            {
                showErrorDialog(msg);
            }
        }
        else
            finish = true;
    }
}

void MainWindow::addPoligono(GtkBuilder* builder)
{
    PolygonDialog dialog(GTK_BUILDER(builder));
    bool finish = false;
    while(!finish)
    {
        if(dialog.run() == 1)
        {
            try
            {
                Coordinates c;
                dialog.getCoords(c);
                Objeto* obj = _world->addPoligono(dialog.getName(),dialog.shouldBeFilled(), c);
                _viewport->update(obj);
                addObjOnListStore(dialog.getName(), "Poligono");
                gtk_widget_queue_draw(_mainWindow);
                finish = true;
            }
            catch(const char* msg)
            {
                showErrorDialog(msg);
            }
        }
        else
            finish = true;
    }
}

void MainWindow::addObj3D(GtkBuilder* builder)
{
    Object3dDialog dialog(GTK_BUILDER(builder));
    bool finish = false;
    while(!finish)
    {
        if(dialog.run() == 1)
        {
            try
            {
                Objeto* obj = _world->addObj3D(dialog.getName(), dialog.getFaces());
                _viewport->update(obj);
                addObjOnListStore(dialog.getName(), "Objeto 3D");
                gtk_widget_queue_draw(_mainWindow);
                finish = true;
            }
            catch(const char* msg)
            {
                showErrorDialog(msg);
            }
        }
        else
        {
            finish = true;
        }
    }
}

void MainWindow::addCurvaBezier(GtkBuilder* builder)
{
    CurveDialog dialog(GTK_BUILDER(builder));
    bool finish = false;
    while(!finish)
    {
        if(dialog.run() == 1)
        {
            try
            {
                Coordinates c;
                dialog.getCoords(c);
                Objeto* obj = _world->addCurvaBezier(dialog.getName(), c);
                _viewport->update(obj);
                addObjOnListStore(dialog.getName(), "Curva Bezier");
                gtk_widget_queue_draw(_mainWindow);
                finish = true;
            }
            catch(const char* msg)
            {
                showErrorDialog(msg);
            }
        }
        else
            finish = true;
    }
}

void MainWindow::addCurvaBSpline(GtkBuilder* builder)
{
    CurveDialog dialog(GTK_BUILDER(builder));
    bool finish = false;
    while(!finish)
    {
        if(dialog.run() == 1)
        {
            try
            {
                Coordinates c;
                dialog.getCoords(c);
                Objeto* obj = _world->addCurvaBSpline(dialog.getName(), c);
                _viewport->update(obj);
                addObjOnListStore(dialog.getName(), "CurvaBSpline");
                gtk_widget_queue_draw(_mainWindow);
                finish = true;
            }
            catch(const char* msg)
            {
                showErrorDialog(msg);
            }
        }
        else
            finish = true;
    }
}

void MainWindow::translacaoObj(GtkBuilder* builder)
{
    TransDialog dialog(GTK_BUILDER(builder));
    bool finish = false;
    std::string name;
    GtkTreeIter iter;
    if(!getNameObj(name, &iter))
        return;
    while(!finish)
    {
        if(dialog.run() == 1)
        {
            try
            {
                Objeto* obj = _world->translacaoObj(name, dialog.getDx(), dialog.getDy(), dialog.getDz());
                _viewport->update(obj);
                gtk_widget_queue_draw(_mainWindow);
                finish = true;
            }
            catch(const char* msg)
            {
                showErrorDialog(msg);
            }
        }
        else
            finish = true;
    }
}

void MainWindow::escalonarObj(GtkBuilder* builder)
{
    ScaDialog dialog(GTK_BUILDER(builder));
    bool finish = false;
    std::string name;
    GtkTreeIter iter;
    if(!getNameObj(name, &iter))
        return;
    while(!finish)
    {
        if(dialog.run() == 1)
        {
            try
            {
                Objeto* obj = _world->escalonarObj(name, dialog.getSx(), dialog.getSy(), dialog.getSz());
                _viewport->update(obj);
                gtk_widget_queue_draw(_mainWindow);
                finish = true;
            }
            catch(const char* msg)
            {
                showErrorDialog(msg);
            }
        }
        else
            finish = true;
    }
}

void MainWindow::rotacaoObj(GtkBuilder* builder)
{
    RotDialog dialog(GTK_BUILDER(builder));
    bool finish = false;
    std::string name;
    GtkTreeIter iter;
    if(!getNameObj(name, &iter))
        return;
    while(!finish)
    {
        if(dialog.run() == 1)
        {
            try
            {
		        Coordinate c(dialog.getCx(), dialog.getCy(), dialog.getCz());
                Objeto* obj = _world->rotacaoObj(name, dialog.getAnguloX(), dialog.getAnguloY(), dialog.getAnguloZ(), dialog.getAngulo(), Coordinate(dialog.getCx(), dialog.getCy()), dialog.getTipo());
                _viewport->update(obj);
                gtk_widget_queue_draw(_mainWindow);
                finish = true;
            }
            catch(const char* msg)
            {
                showErrorDialog(msg);
            }
        }
        else
            finish = true;
    }
}

void MainWindow::clipAlg(AlgoritmoClipping alg)
{
    _viewport->clipAlg(alg);
    gtk_widget_queue_draw(_mainWindow);
}


void MainWindow::addSuperficie(GtkBuilder* builder)
{
    SurfaceDialog dialog(GTK_BUILDER(builder));
    bool finish = false;
    while(!finish)
    {
        if(dialog.run() == 1)
        {
            try
            {
                auto& c = dialog.getCoords();
                Objeto* obj = _world->addSuperficie(dialog.getName(),dialog.getTipoSuperficie(), dialog.getMaxLin(), dialog.getMaxCols(), c); 
                _viewport->update(obj);
                addObjOnListStore(dialog.getName(), "Superficie");
                gtk_widget_queue_draw(_mainWindow);
                finish = true;
            }
            catch(const char* msg)
            {
                showErrorDialog(msg);
            }
        }
        else
            finish = true;
    }
}
#endif // MAINWINDOW_HPP
