#include <QDate>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clases.h"
#include "inventario.h"
#include "z.h"
#include "precios.h"
#include "archivo.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString sdate = QDate::currentDate().toString();
    ui->label_fecha->setText(sdate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_aceptar_in_clicked()
{
    const QString &palabra = ui->line_ar_in->text();
    std::string archivo = palabra.toUtf8().constData();
    std::string dir("/home/daniel/Yo/Mamá/Widget_program/");
    std::string new_dir = dir + archivo;
    const char* nuevo = new_dir.c_str();

    std::vector<std::vector<std::wstring> > leyo = leer_inventario(nuevo);

    std::vector<Producto> para = inventario_organizado(leyo);

    const char* direction = "/home/daniel/Yo/Mamá/Widget_program/inventario.csv";
    inventario_imprimir(direction, para);
}

void MainWindow::on_cancelar_in_clicked()
{
    ui->line_ar_in->clear();
}

void MainWindow::on_aceptar_z_clicked()
{
    const QString &palabra = ui->line_ar_z->text();
    std::string archivo = palabra.toUtf8().constData();
    std::string dir("/home/daniel/Yo/Mamá/Widget_program/");
    std::string new_dir = dir + archivo;
    const char* nuevo = new_dir.c_str();

    std::vector<std::vector<std::wstring> > z_ar = z_leer(nuevo);

    std::vector<std::vector<std::vector<std::wstring> > > z_par = z_partes(z_ar);

    std::vector<std::tuple<std::wstring, std::wstring, std::wstring, std::vector<Producto> > > z_r = z_REG(z_par);

    const char* name = "/home/daniel/Yo/Mamá/Widget_program/registro_diario.csv";
    informe_z(name, archivo, z_r);

    const char* name1 = "/home/daniel/Yo/Mamá/Widget_program/registro.csv";
    imprimir_z(name1, z_r);

    const char* direction = "/home/daniel/Yo/Mamá/Widget_program/inventario.csv";
    inventario_z(direction, z_r);
    //const char* name = "/home/daniel/Yo/Mamá/Widget_program/z_REG.csv";
    //imprimir_z(name,z_r);

}

void MainWindow::on_cancelar_z_clicked()
{
    ui->line_ar_z->clear();
}

void MainWindow::on_leer_ar_com_clicked()
{
    const char* name = "/home/daniel/Yo/Mamá/Widget_program/compra.csv";
    std::wcout << "Error" << std::endl;
    std::vector<std::vector<std::wstring> > archivo = leer_archivo(name);
    std::wcout << "Error" << std::endl;
    Factura mio = organizar_archivo(archivo);
    std::wcout << "Error" << std::endl;
    std::wstring ws_empresa = mio.empresa;
    std::string s_empresa(ws_empresa.begin(), ws_empresa.end());
    std::string dir("/home/daniel/Yo/Mamá/Widget_program/");
    std::string ext(".csv");
    std::string name2 = dir + s_empresa + ext;
    std::wcout << "Error" << std::endl;
    const char* name3 = name2.c_str();
    std::wcout << "Error" << std::endl;
    imprimir_r(name3, mio);
    cuenta_factura(mio);
    /*
    const char* name4 = "/home/daniel/Yo/Mamá/Widget_program/cuentas_pagar.csv";
    imprimir_cuenta(name4,);
    */
}

void MainWindow::on_leer_ar_ven_clicked()
{
    const char* name = "/home/daniel/Yo/Mamá/Widget_program/venta.csv";
    std::vector<std::vector<std::wstring> > archivo = leer_archivo(name);
    imprmir_archivo(archivo);
    Factura mio = organizar_archivo(archivo);
    std::wstring ws_empresa = mio.empresa;
    std::string s_empresa(ws_empresa.begin(), ws_empresa.end());
    std::string dir("/home/daniel/Yo/Mamá/Widget_program/");
    std::string ext(".csv");
    std::string name2 = dir + s_empresa + ext;
    const char* name3 = name2.c_str();
    imprimir_r(name3, mio);
}
//Esta función esta incompleta porque no hay lista de precios, por lo tanto toca hacer una lista de precios y su función para leer e imprimir.
void MainWindow::on_aceptar_pp_clicked()
{
    const QString &lcodigo = ui->line_co_pp->text();
    std::string scodigo = lcodigo.toUtf8().constData();
    int icodigo = std::stoi(scodigo);

    const QString &lprecio = ui->line_pr_pp->text();
    std::string sprecio = lprecio.toUtf8().constData();
    int iprecio = std::stoi(sprecio);

    const char* nuevo = "/home/daniel/Yo/Mamá/Widget_program/inventario1.csv";
    std::vector<std::vector<std::wstring> > leyo = leer_precios(nuevo);
    std::vector<Producto> para = organizar_precios(leyo);

    for(unsigned int i=0; i<para.size(); i++){
        if(para[i].codigo == icodigo){
            para[i].precio = iprecio;
            break;
        }
    }

    imprimir_precios(nuevo, para);
}

void MainWindow::on_cancelar_pp_clicked()
{
    ui->line_co_pp->clear();
    ui->line_pr_pp->clear();
}

void MainWindow::on_aceptar_np_clicked()
{
    Producto mercancia;

    const QString &lcodigo = ui->line_cod_np->text();
    std::string scodigo = lcodigo.toUtf8().constData();
    mercancia.codigo = std::stoi(scodigo);

    const QString &lcosto = ui->line_cos_np->text();
    std::string scosto = lcosto.toUtf8().constData();
    mercancia.costo = std::stoi(scosto);

    const QString &lprecio = ui->line_pr_np->text();
    std::string sprecio = lprecio.toUtf8().constData();
    mercancia.precio = std::stoi(sprecio);

    const QString &lcantidad = ui->line_ca_np->text();
    std::string scantidad = lcantidad.toUtf8().constData();
    mercancia.cantidad = std::stoi(scantidad);

    const QString &lnombre = ui->line_nom_np->text();
    std::string snombre = lnombre.toUtf8().constData();
    std::wstring wsnombre(snombre.begin(), snombre.end());
    mercancia.nombre = wsnombre;

    const char* inuevo = "/home/daniel/Yo/Mamá/Widget_program/inventario.csv";
    std::vector<std::vector<std::wstring> > ileyo = leer_inventario(inuevo);
    std::vector<Producto> ipara = inventario_organizado(ileyo);
    ipara.push_back(mercancia);
    inventario_imprimir(inuevo, ipara);

    const char* lnuevo = "/home/daniel/Yo/Mamá/Widget_program/lista.csv";
    std::vector<std::vector<std::wstring> > lleyo = leer_precios(lnuevo);
    std::vector<Producto> lpara = organizar_precios(lleyo); lpara.push_back(mercancia);
    imprimir_precios(lnuevo, lpara);

}

void MainWindow::on_cancelar_np_clicked()
{
    ui->line_ca_np->clear();
    ui->line_cod_np->clear();
    ui->line_cos_np->clear();
    ui->line_nom_np->clear();
    ui->line_pr_np->clear();
}
