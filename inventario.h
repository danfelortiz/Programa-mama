#ifndef INVENTARIO_H_INCLUDED
#define INVENTARIO_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <wchar.h>
#include <string>
#include <typeinfo>
#include <tuple>
#include <locale>
#include "clases.h"

//Esta función lee el archivo donde están los productos con sus precios.
std::vector<std::vector<std::wstring> > leer_inventario(const char* name);
//Esta función ordena los datos y en cada tupla va el código, el nombre y valor del producto.
//std::vector<std::tuple<int, std::wstring, int, int, int> > inventario_organizado(std::vector<std::vector<std::wstring> > &y);
std::vector<Producto> inventario_organizado(std::vector<std::vector<std::wstring> > &y);
//Esta función imprime el inventario.
//void inventario_imprimir(const char* name, std::vector<std::tuple<int, std::wstring, int, int, int> > &y);
void inventario_imprimir(const char* name, std::vector<Producto> &y);

std::vector<std::vector<std::wstring> > leer_inventario(const char* name)
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
  y.erase(y.begin() + 0);
  y.pop_back();

  return y;
}


std::vector<Producto> inventario_organizado(std::vector<std::vector<std::wstring> > &y)
{
  //std::vector<std::tuple<int, std::wstring, int, int, int> > v;
  std::vector<Producto> v;
  for(unsigned int i=0; i<y.size(); i++){
    Producto elemento;
    elemento.codigo = std::stoi(y[i][0]);
    elemento.nombre = y[i][1];
    elemento.apodo = y[i][2];
    elemento.cantidad = std::stoi(y[i][3]);
    elemento.costo = std::stoi(y[i][4]);
    elemento.precio = std::stoi(y[i][5]);
    v.push_back(elemento);
  }

  return v;
}


void inventario_imprimir(const char* name, std::vector<Producto> &y)
{
    std::wofstream fout1(name);
    fout1.imbue(std::locale(""));
    fout1 << L"CÓDIGO\tNOMBRE\tAPODO\tCANTIDAD\tCOSTO\tPRECIO\tCOSTO TOTAL\tPRECIO TOTAL\n";
    for(unsigned int i=0; i<y.size(); i++){
       fout1 << y[i].codigo << '\t' << y[i].nombre << '\t' << y[i].apodo << '\t';
       fout1 << y[i].cantidad << '\t' << y[i].costo << '\t' << y[i].precio << '\t';
       int costo_total = y[i].cantidad * y[i].costo;
       int precio_total = y[i].cantidad * y[i].precio;
       fout1 << costo_total << '\t' << precio_total << '\n';
    }
    fout1.close();
}

#endif
