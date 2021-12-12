#ifndef DIALOGS_HPP
#define DIALOGS_HPP

#include <gtk/gtk.h>
#include <string>
#include "Objetos.hpp"

#define UI_FILE "ui.glade"

enum class rotacao { OBJETO, PONTO };

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

class FileDialog : public Dialog
{
    public:
        FileDialog(GtkBuilder* builder, bool toSave=false);
        char* const getFileName() const { return gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(_dialog)); }
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
	    double getZ(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_z)); }
    private:
        GtkWidget *_x, *_y, *_z;
};

class LineDialog : public ObjectDialog
{
    public:
        LineDialog(GtkBuilder* builder);
        double getX1(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_x1)); }
        double getY1(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_y1)); }
	    double getZ1(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_z1)); }
        double getX2(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_x2)); }
        double getY2(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_y2)); }
	    double getZ2(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_z2)); }
    private:
        GtkWidget *_x1, *_y1, *_z1, *_x2, *_y2, *_z2;
};

class PolygonDialog : public ObjectDialog
{
    public:
        PolygonDialog(GtkBuilder* builder, bool isFace=false);
        ~PolygonDialog(){ destroy(); }
        void destroy();
        void getCoords(Coordinates& coords);
        bool shouldBeFilled(){ return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(_checkFilled)); }
        void onClickEvent();
        void onEditCelEvent(GtkCellRendererText *cell, const gchar *path_string, const gchar *new_text, int column);
    protected:
        GtkTreeModel *_model;
        GtkWidget *_x, *_y, *_z, *_checkFilled;
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
	    double getDz(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_dz)); }
    private:
        GtkWidget *_dx, *_dy, *_dz;
};

class ScaDialog : public Dialog
{
    public:
        ScaDialog(GtkBuilder* builder);
        double getSx(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_sx)); }
        double getSy(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_sy)); }
	    double getSz(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_sz)); }
    private:
        GtkWidget *_sx, *_sy, *_sz;
};

class RotDialog : public Dialog
{
    public:
        RotDialog(GtkBuilder* builder);
	    double getAnguloX(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_anguloX)); }
        double getAnguloY(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_anguloY)); }
        double getAnguloZ(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_anguloZ)); }
        double getAngulo(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_angulo)); }
        double getCx(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_cx)); }
        double getCy(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_cy)); }
	    double getCz(){ return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_cz)); }
        rotacao getTipo();
    private:
        GtkWidget *_anguloX, *_anguloY, *_anguloZ, *_angulo, *_cx, *_cy, *_cz, *_centroObjeto, *_centroMundo, *_ponto;
        bool isActive(GtkWidget *widget){ return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)); }
};

class Object3dDialog : public ObjectDialog
{
    public:
        Object3dDialog(GtkBuilder* builder);
        ~Object3dDialog(){ destroy(); }
        void destroy();
        ListaFaces& getFaces(){ return _faces; }
        void onClickEvent(GtkBuilder* builder);
    protected:
        GtkTreeModel *_model = nullptr;
        ListaFaces _faces;
};

class SurfaceDialog : public ObjectDialog
{
    public:
        SurfaceDialog(GtkBuilder* builder);
        ~SurfaceDialog(){ destroy(); }
   	    double getX() { return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_x)); }
    	double getY() { return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_y)); }
    	double getZ() { return gtk_spin_button_get_value(GTK_SPIN_BUTTON(_z)); }
    	ObjType getTipoSuperficie(){ return _tipo; }
    	int getMaxLin(){ return m_max_i; }
    	int getMaxCols(){ return m_max_j; }
    	void setTipoSuperficie(ObjType tipo);
        const Coordinates& getCoords() const;
        void addCoordEvent();
        void addSurfaceEvent();
    protected:
        GtkWidget *_x, *_y, *_z, *_comboBox, *_grid;
        GtkWidget *_btnAddSurface, *_rbBezier, *_rbBSpline;
        ObjType _tipo = ObjType::SUPERFICIE_BEZIER;
	    int m_i = 0, m_j = 0, m_max_i = 4, m_max_j = 4;
        Coordinates _coords;
};

#endif // DIALOGS_HPP
