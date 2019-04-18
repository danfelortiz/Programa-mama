#ifndef ARCHIVO_H_INCLUDED
#define ARCHIVO_H_INCLUDED

#include <fstream>
#include <stdio.h>
#include <wchar.h>
#include <tuple>
#include <locale>
#include <typeinfo>
#include "clases.h"

//Esta función lee y organiza el archivo cuenta.
std::vector<std::vector<Factura> > leer_cuenta(const char* name);
//Esta función imprime el archivo cuenta.
void imprimir_cuenta(const char* name, std::vector<std::vector<Factura> > &y);
//Esta función me agrega la factura transacción al archivo de cuentas
void cuenta_factura(const char* name, Factura transaccion);
//Esta función lee el archivo name.
std::vector<std::vector<std::wstring> > leer_archivo(const char* name);
//Esta función imprime la compra en la terminal.
void imprmir_archivo(std::vector<std::vector<std::wstring> > &y);
//Esta función organiza la fecha.
std::vector<std::wstring> fecha(std::wstring &y);
//Esta función me dice si es de contado o no.
bool contado_fun(std::wstring &y);
//Está función organiza la transacción.
Factura organizar_archivo(std::vector<std::vector<std::wstring> > &y);
//Esta funcion imprime hey.
void imprimir_r(const char* name, Factura cambio);



void cuenta_factura(const char* name, Factura transaccion)
{
    std::vector<std::vector<Factura> > cuenta_empresa = leer_cuenta(name);
    std::wcout << L"8\t" << transaccion.costo_total << std::endl;
    int contador = 0;
    for(int i=0; i<cuenta_empresa.size(); i++){
        std::wcout << L"9\t" << cuenta_empresa[i][0].costo_total << std::endl;
        if(cuenta_empresa[i][0].empresa == transaccion.empresa){
            cuenta_empresa[i].push_back(transaccion);
            contador++;
            break;
        }
    }

    if(contador == 0){
        cuenta_empresa.push_back(std::vector<Factura>());
        cuenta_empresa[cuenta_empresa.size()-1].push_back(transaccion);
    }

    imprimir_cuenta(name, cuenta_empresa);
}


std::vector<std::vector<Factura> > leer_cuenta(const char* name)
{
    std::vector<std::vector<std::wstring> > archivo = leer_archivo(name);
    std::vector<std::vector<Factura> > cuenta_empresa;
    int k=0;
    for(int i=0; i<archivo.size(); i++){
        if(archivo[i].size() == 2){
            cuenta_empresa.push_back(std::vector<Factura>());
            int j=2;
            while(archivo[i+j].size() == 4){
                Factura cuenta;
                cuenta.empresa = archivo[i][1];
                cuenta.numero = archivo[i+j][0];
                cuenta.f_entrega = fecha(archivo[i+j][1]);
                cuenta.f_pago = fecha(archivo[i+j][2]);
                std::wcout << L"6\t" << archivo[i+j][3] << std::endl;
                cuenta.costo_total = std::stoi(archivo[i+j][3]);
                std::wcout << L"7\t" << cuenta.costo_total << std::endl;
                cuenta_empresa[k].push_back(cuenta);
                j++;
            }
            k++;
            i += j;
        }
    }

    return cuenta_empresa;
}


void imprimir_cuenta(const char* name, std::vector<std::vector<Factura> > &y)
{
    std::wstring fecha_entrega, fecha_pago;
    std::wofstream fout(name);
    fout.imbue(std::locale(""));

    for(int i=0; i<y.size(); i++){
        fout << L"Empresa:\t" << y[i][0].empresa << L"\n\n";
        for(int j=0; j<y[i].size(); j++){
            fout << y[i][j].numero << L"\t";
            for(int k=0; k<y[i][j].f_entrega.size(); k++){
                fecha_entrega.append(y[i][j].f_entrega[k]);
                fecha_entrega.append(L" ");
            }
            fecha_entrega.pop_back();
            fout << fecha_entrega << L"\t";
            fecha_entrega.clear();
            for(int k=0; k<y[i][j].f_pago.size(); k++){
                fecha_pago.append(y[i][j].f_pago[k]);
                fecha_pago.append(L" ");
            }
            fecha_pago.pop_back();
            fout << fecha_pago << L"\t";
            fecha_pago.clear();
            std::wcout << L"10\t" << y[i][j].costo_total << std::endl;
            std::wcout << typeid(y[i][j].costo_total).name() << std::endl;
            fout << (int)y[i][j].costo_total << L"\n";
            std::wcout << L"11\t" << y[i][j].costo_total << std::endl;
        }
        fout << L"\n\n";
    }
    fout.close();
}

std::vector<std::vector<std::wstring> > leer_archivo(const char* name)
{
  char *locale = setlocale(LC_ALL, "");
  FILE *pFile = fopen(name, "r");

  wint_t c;
  std::vector<std::vector<std::wstring> > y;
  std::wstring ws;
  int i=0;
  y.push_back( std::vector<std::wstring>() );

  while((c = fgetwc(pFile)) != WEOF){
    if(c == L'\t' || c == L'\n'){
      if(ws.length()!=0){
        y[i].push_back(ws);
      }
      ws.clear();
    }
    else{
      ws.push_back(c);
    }
    if(c == L'\n'){
      y.push_back( std::vector<std::wstring>() );
      i++;
    }
  }

  fclose(pFile);
  y.pop_back();

  return y;
}

void imprmir_archivo(std::vector<std::vector<std::wstring> > &y)
{
    for(unsigned int i=0; i<y.size(); i++){
        for(unsigned int j=0; j<y[i].size(); j++){
            std::wcout << y[i][j] << L" , ";
        }
        std::wcout << std::endl;
    }
}


Factura organizar_archivo(std::vector<std::vector<std::wstring> > &y)
{
    Factura transaccion;
    transaccion.empresa = y[0][1];
    transaccion.numero = y[1][1];
    transaccion.f_entrega = fecha(y[2][1]);
    transaccion.contado = contado_fun(y[3][1]);

    if(transaccion.contado == true){
        transaccion.f_pago = transaccion.f_entrega;
        transaccion.credito = 0;
    }
    else{
        transaccion.credito = std::stoi(y[4][1]);
        transaccion.f_pago = fecha(y[5][1]);
    }

    for(unsigned int i=7; i<(y.size()-1); i++){
        Producto producto;
        producto.nombre = y[i][0];
        producto.codigo = std::stoi(y[i][1]);
        producto.cantidad = std::stoi(y[i][2]);
        transaccion.productos.push_back(producto);
    }

    std::wcout << L"1\t" << y[y.size()-1][1] << std::endl;
    transaccion.costo_total = std::stoi(y[y.size()-1][1]);
    std::wcout << L"2\t" << transaccion.costo_total << std::endl;

    return transaccion;
}


std::vector<std::wstring> fecha(std::wstring &y)
{
    std::vector<std::wstring> vect;
    std::wstring ws;

    for(unsigned int i=0; i<y.length(); i++){
        if(iswdigit(y[i])){
            ws.push_back(y[i]);
        }
        else{
            vect.push_back(ws);
            ws.clear();
        }
    }
    vect.push_back(ws);

    return vect;
}


bool contado_fun(std::wstring &y)
{
    for(unsigned int i=0; i<y.length(); i++){
        if(iswupper(y[i])){
            y[i] = towlower(y[i]);
        }
    }

    if(y==L"sí" || y==L"si"){
        return true;
    }
    else{
        return false;
    }
}


void imprimir_r(const char* name, Factura cambio)
{
    std::wofstream fout;
    fout.open(name, std::ios_base::app);
    fout.imbue(std::locale(""));

    std::wstring doble_tab = L"\t\t";
    std::wstring doble_enter = L"\n\n";
    std::wstring tab = L"\t";
    std::wstring enter = L"\n";
    std::wstring slash = L"/";

    fout << L"Factura:" << doble_tab << L"Fecha recibido:" << doble_tab << L"Fecha de pago:" << enter;
    fout << cambio.numero << doble_tab;
    for(unsigned int i=0; i<cambio.f_entrega.size(); i++){
        fout << cambio.f_entrega[i] << slash;
    }
    fout << doble_tab;
    for(unsigned int i=0; i<cambio.f_pago.size(); i++){
        fout << cambio.f_pago[i] << slash;
    }
    fout << doble_enter;

    if(cambio.contado == true){
        fout << L"Contado:" << tab << L"Sí" << enter << L"Crédito:" << tab << cambio.credito << tab << L"días" << doble_enter;
    }
    else {
        fout << L"Contado:" << tab << L"No" << enter << L"Crédito:" << tab << cambio.credito << tab << L"días" << doble_enter;
    }

    fout << tab << L"Nombre" << tab << L"Código" << tab << L"Cantidad" << enter;
    for(unsigned int i=0; i<cambio.productos.size(); i++){
        std::wstring codigo = std::to_wstring(cambio.productos[i].codigo);
        std::wstring cantidad = std::to_wstring(cambio.productos[i].cantidad);
        fout << tab << cambio.productos[i].nombre << tab << codigo << tab << cantidad << enter;
    }
    fout << enter;
    std::wcout << L"4\t" << cambio.costo_total << std::endl;
    fout << L"Costo total:" << tab << cambio.costo_total << doble_enter << enter;
    std::wcout << L"5\t" << cambio.costo_total << std::endl;
    fout.close();
}




#endif
