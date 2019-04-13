#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_aceptar_in_clicked();

    void on_cancelar_in_clicked();

    void on_aceptar_z_clicked();

    void on_cancelar_z_clicked();

    void on_leer_ar_com_clicked();

    void on_leer_ar_ven_clicked();

    void on_aceptar_pp_clicked();

    void on_cancelar_pp_clicked();

    void on_aceptar_np_clicked();

    void on_cancelar_np_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
