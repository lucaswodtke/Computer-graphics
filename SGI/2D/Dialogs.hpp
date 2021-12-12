#ifndef DIALOGS_HPP
#define DIALOGS_HPP

#include <gtk/gtk.h>
#include <string>
#include "Objetos.hpp"

#define UI_FILE "ui.glade"

enum class rotacao { OBJETO, MUNDO, PONTO };

// Classe Dialog
// Classe dos diálogos da interface

class Dialog
{
    public:
        Dialog(){}
        ~Dialog(){ destroy(); }
        int run();
        virtual void destroy();
    protected:
        GtkWidget *_dialog;
    private:
};

class ObjectDialog : public Dialog
{
    public:
        ObjectDialog(){}
        std::string const getName(){ return gtk_entry_get_text(GTK_ENTRY(_name)); }
    protected:
        GtkWidget* _name;
};

class PointDialog : public ObjectDialog
{
    public:
        PointDialog(GtkBuilder* builder);
        double getX(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_x)); }
        double getY(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_y)); }
    protected:
    private:
        GtkWidget *_x, *_y;
};

class LineDialog : public ObjectDialog
{
    public:
        LineDialog(GtkBuilder* builder);
        double getX1(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_x1)); }
        double getY1(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_y1)); }
        double getX2(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_x2)); }
        double getY2(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_y2)); }
    protected:
    private:
        GtkWidget *_x1, *_y1,
            *_x2, *_y2;
};

class PolygonDialog : public ObjectDialog
{
    public:
        PolygonDialog(GtkBuilder* builder);
        ~PolygonDialog(){ destroy(); }
        void destroy();
        void getCoords(Coordinates& coords);
        bool shouldBeFilled(){ return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(_checkFilled)); }
        void onClickEvent();
        void onEditCelEvent(GtkCellRendererText *cell, const gchar *path_string, const gchar *new_text, int column);
    protected:
        GtkTreeModel *_model;
        GtkWidget *_x, *_y, *_checkFilled;
};

class CurveDialog : public PolygonDialog
{
    public:
        CurveDialog(GtkBuilder* builder);
        ~CurveDialog(){ destroy(); }
};

class TransDialog : public Dialog
{
    public:
        TransDialog(GtkBuilder* builder);
        double getDx(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_dx)); }
        double getDy(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_dy)); }

    private:
        GtkWidget *_dx, *_dy;
};

class ScaDialog : public Dialog
{
    public:
        ScaDialog(GtkBuilder* builder);
        double getSx(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_sx)); }
        double getSy(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_sy)); }

    private:
        GtkWidget *_sx, *_sy;
};

class RotDialog : public Dialog
{
    public:
        RotDialog(GtkBuilder* builder);

        double getAngulo(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_angulo)); }
        double getCx(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_cx)); }
        double getCy(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_cy)); }
        rotacao getTipo();

    private:
        GtkWidget *_angulo, *_cx, *_cy,
            *_centroObjeto, *_centroMundo, *_ponto;

        bool isActive(GtkWidget *widget){ return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)); }
};

#endif // DIALOGS_HPP
