#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <gtk/gtk.h>
#include <string>
#include "Viewport.hpp"
#include "Dialogs.hpp"
#include "Objetos.hpp"
#include "World.hpp"

#define UI_FILE "ui.glade"

enum class Buttons { ZOOM_OUT, ZOOM_IN, UP, RIGHT, DOWN, LEFT, ROTATE_L, ROTATE_R };

// Classe MainWindow
// Nesta classe é estruturada a janela principal com seus containers e componentes
// +interface com Glade

class MainWindow
{
    public:
        MainWindow(GtkBuilder* builder);
        virtual ~MainWindow() { delete _world; delete _viewport; }
        void addPonto(GtkBuilder* builder);
        void addReta(GtkBuilder* builder);
        void addPoligono(GtkBuilder* builder);
	    void addCurva2(GtkBuilder* builder);
	    void addCurvaBSpline(GtkBuilder* builder);
        void zoom(Buttons id);
        void move(Buttons id);
        void rotateW(Buttons id);
        void onDraw(cairo_t* cr);
        void showPopUp(GdkEvent *event);
        void showHelpDialog();
        void removeObj();
        void translacaoObj(GtkBuilder* builder);
        void escalonarObj(GtkBuilder* builder);
        void rotacaoObj(GtkBuilder* builder);
	    void clipAlg(AlgoritmoClipping alg);

    protected:
    private:
        GtkWidget* _mainWindow, *_step, *_log, *_popUp, *_logScroll;
        GtkWidget *_helpDialog;
        GtkTreeModel* _mainModel;
        GtkTreeSelection* _treeSelection;
        Viewport *_viewport;
        World *_world;
        bool getNameObj(std::string &name, GtkTreeIter *iter);
        void showErrorDialog(const char* msg);
        void addObjOnListStore(const std::string name, const char* type);
};

MainWindow::MainWindow(GtkBuilder* builder) 
{
    GError* error = NULL;
    char* ids[] = {(char*)"main_list_store",(char*)"adj_step",
        (char*)"main_window",(char*)"main_pop_up_menu", NULL};
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

    gtk_widget_show( _mainWindow );
}

void MainWindow::showErrorDialog(const char* msg)
{
    GtkWidget *dialog = gtk_message_dialog_new (GTK_WINDOW(_mainWindow), 
    GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", msg);
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

void MainWindow::showPopUp(GdkEvent *event){
    // TODO fix this
    //gtk_menu_popup(GTK_MENU(_popUp), NULL, NULL, NULL, NULL, event->button.button, event->button.time);
    // https://github.com/csete/gpredict/issues/144
    //gtk_menu_popup_at_pointer(GTK_MENU(menu), NULL);
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
    catch(const char* msg){
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
    switch(id)
    {
        case Buttons::ROTATE_L:
            _viewport->rotateW(value);
            break;
        case Buttons::ROTATE_R:
            _viewport->rotateW(-value);
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
                Coordinate c(dialog.getX(),dialog.getY());
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
                c.emplace_back(dialog.getX1(), dialog.getY1());
                c.emplace_back(dialog.getX2(), dialog.getY2());
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

void MainWindow::addCurva2(GtkBuilder* builder)
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

                Objeto* obj = _world->addCurva2(dialog.getName(), c);
                _viewport->update(obj);
                addObjOnListStore(dialog.getName(), "Curva2");

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
    while(!finish){
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
                Objeto* obj = _world->translacaoObj(name, dialog.getDx(), dialog.getDy());
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
                Objeto* obj = _world->escalonarObj(name, dialog.getSx(), dialog.getSy());
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
                Objeto* obj = _world->rotacaoObj(name, dialog.getAngulo(),
                Coordinate(dialog.getCx(), dialog.getCy()), dialog.getTipo());
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

#endif // MAINWINDOW_HPP
