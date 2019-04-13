#ifndef PRECIOS_H_INCLUDED
#define PRECIOS_H_INCLUDED

#include <fstream>
#include <stdio.h>
#include <wchar.h>
#include <tuple>
#include <locale>
#include "clases.h"

//Esta función lee el archivo de la lista de precios
std::vector<std::vector<std::wstring> > leer_precios(const char* name);
//Esta organiza la lectura del archivo de lista de precios
std::vector<Producto> organizar_precios(std::vector<std::vector<std::wstring> > &y);
//Esta función imprime la lista de precios
void imprimir_precios(const char* name, std::vector<Producto> &y);

std::vector<std::vector<std::wstring> > leer_precios(const char* name)
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


std::vector<Producto> organizar_precios(std::vector<std::vector<std::wstring> > &y)
{
    std::vector<Producto> lista;

    for(unsigned int i=0; i<y.size(); i++){
        Producto mercancia;
        mercancia.codigo = std::stoi(y[i][0]);
        mercancia.nombre = y[i][1];
        mercancia.precio = std::stoi(y[i][2]);
        lista.push_back(mercancia);
    }

    return lista;
}

void imprimir_precios(const char* name, std::vector<Producto> &y)
{
    std::wofstream fout1(name);
    fout1.imbue(std::locale(""));
    for(unsigned int i=0; i<y.size(); i++){
       fout1 << y[i].codigo;
       fout1 << '\t';
       fout1 << y[i].nombre;
       fout1 << '\t';
       fout1 << y[i].precio;
       fout1 << '\n';
    }
    fout1.close();
}

#endif
