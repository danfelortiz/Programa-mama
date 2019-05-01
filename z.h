#ifndef Z_H_INCLUDED
#define Z_H_INCLUDED

#include <fstream>
#include <stdio.h>
#include <wchar.h>
#include <tuple>
#include <locale>
#include <algorithm>
#include <vector>
#include "clases.h"
#include "inventario.h"

//Esta función lee un archivo tipo Z y separa sus caracteres que no son de tipo espacio en wstring. Además que también los separa en líneas.
std::vector<std::vector<std::wstring> > z_leer(const char* name);
//Esta función separa el archivo Z ya leído en sus partes de REG, PMG, etc.
std::vector<std::vector<std::vector<std::wstring> > > z_partes(std::vector<std::vector<std::wstring> > &y);
//Esta función obtiene la hora y el minuto de la compra.
std::vector<std::wstring> hora_minuto(std::wstring &ws);
//Esta función me dice si el wstring ws contien el caracter signo.
bool deteccion(std::wstring ws, wchar_t signo);
//Esta función me genera una tuple con el nombre y el valor del producto.
std::tuple<std::wstring, int> nombre_valor(std::vector<std::wstring> &y);
//Esta función me guarda en una tupla la cantidad y el precio del producto.
std::tuple<int, int> cantidad_precio(std::vector<std::wstring> &y);
//Esta función guarda (en este orden) el año, el mes, el día, la hora, el minuto, el número de comprador y los productos vendidos (estos en un vector) en una tupla, la cual esta dentro de un vector para tener todo organizado. Dentro del vector que esta en la tupla habitan tuplas que contienen la información de cada venta
std::vector<std::tuple<std::wstring, std::wstring, std::wstring, std::vector<Producto> > > z_REG(std::vector<std::vector<std::vector<std::wstring> > > &y);
//Esta función imprime en el archivo name los registros de Z.
void imprimir_z(const char* name, std::vector<std::tuple<std::wstring, std::wstring, std::wstring, std::vector<Producto> > > &y);
//Esta función hace algo.
void informe_z(const char* name, std::string sarchivo, std::vector<std::tuple<std::wstring, std::wstring, std::wstring, std::vector<Producto> > > &y);
//Esta función me descuenta del inventario los productos vendidos
void inventario_z(const char* name_in, std::vector<std::tuple<std::wstring, std::wstring, std::wstring, std::vector<Producto> > > &y);

std::vector<std::vector<std::wstring> > z_leer(const char* name)
{
  char *locale = setlocale(LC_ALL, "");
  FILE *pFile = fopen(name, "r");
  
  wint_t c;
  std::vector<std::vector<std::wstring> > y;
  std::wstring ws;
  int i=0;
  y.push_back( std::vector<std::wstring>() );

  while((c = fgetwc(pFile)) != WEOF){
    if(c == L'\n'){
      y.push_back( std::vector<std::wstring>() );
      i++;
    }
    if(iswspace(c)){
      if(ws.length()!=0){
        y[i].push_back(ws);
      }
      ws.clear();
    }
    else{
      ws.push_back(c);
    }
  }

  fclose(pFile);

  return y;
}


std::vector<std::vector<std::vector<std::wstring> > > z_partes(std::vector<std::vector<std::wstring> > &y)
{
  std::vector<std::vector<std::vector<std::wstring> > > v;
  v.push_back( std::vector<std::vector<std::wstring> >() );
  int k=0;
  int l=0;
  for(int i=0; i<y.size(); i++){
    v[k].push_back( std::vector<std::wstring>() );
    for(int j=0; j<y[i].size(); j++){
      if(y[i][j] == L"REG" || y[i][j] == L"PGM" || y[i][j] == L"X" || y[i][j] == L"Z" || y[i][j] == L"_PSD" || y[i][j] == L"OFF" || y[i][j] == L"------------------------"){
	v.push_back( std::vector<std::vector<std::wstring> >() );
	k++;
	v[k].push_back( std::vector<std::wstring>() );
	l = 0;
      }
      v[k][l].push_back(y[i][j]);
    }
    l++;
  }
    
  v.erase(v.begin() + 0);

  for(int i=0; i<v.size(); i++){
    v[i].pop_back();
  }

  return v;
}


std::vector<std::tuple<std::wstring, std::wstring, std::wstring, std::vector<Producto> > > z_REG(std::vector<std::vector<std::vector<std::wstring> > > &y)
{
  std::vector<std::vector<std::vector<std::wstring> > > v;
  for(int i=0; i<y.size(); i++){
    if(y[i][0][0] == L"REG"){
      v.push_back(y[i]);
    }
  }

  std::vector<std::tuple<std::wstring, std::wstring, std::wstring, std::vector<Producto> > > venta;
  std::vector<std::wstring> tiempo;
  std::wstring comprador;
  std::tuple<std::wstring, int> producto;
  std::tuple<int, int> cantidad;

  for(int i=0; i<v.size(); i++){
    std::vector<Producto> vect;
    for(int j=0; j<v[i].size(); j++){
      if(j==0){
        tiempo = hora_minuto(v[i][j][2]);
      }
      else if(j==1){
        comprador = v[i][j][0];
      }
      else if(v[i][j][0] == L"ANULAR"){
        vect.pop_back();
        j += 2;
      }
      else if(v[i][j][0]!=L"EFECTIVO" && v[i][j][0]!=L"CAMBIO" && v[i][j][0].at(0)!='_' && v[i][j][0].at(0)!='#'){
        if(deteccion(v[i][j][v[i][j].size()-1],'$')){
            producto = nombre_valor(v[i][j]);
            cantidad = std::make_tuple(1,std::get<1>(producto));
        }
        else{
            producto = nombre_valor(v[i][j+1]);
            cantidad = cantidad_precio(v[i][j]);
            j++;
        }
        //Aquí genero la tupla donde (en orden) aparecen el nombre, cantidad, precio unitario y valor total del producto.
        //auto tuple = std::make_tuple(std::get<0>(producto), std::get<0>(cantidad), std::get<1>(cantidad), std::get<1>(producto));
        Producto elemento;
        elemento.apodo = std::get<0>(producto);
        elemento.cantidad = std::get<0>(cantidad);
        elemento.precio = std::get<1>(cantidad);
        vect.push_back(elemento);
      }
    }
    auto informacion = std::make_tuple(tiempo[0],tiempo[1],comprador,vect);
    venta.push_back(informacion);
  }
  
  return venta;
}


std::vector<std::wstring> hora_minuto(std::wstring &ws)
{
  std::wstring delimiter = L":";
  size_t pos = ws.find(delimiter);
  std::wstring hora = ws.substr(0, pos);
  std::vector<std::wstring> y;

  y.push_back(hora);
  ws.erase(0, pos + delimiter.length());
  y.push_back(ws);
  
  return y;
}


bool deteccion(std::wstring ws, wchar_t signo)
{
  for(int i=0; i<ws.length(); i++){
    if(ws.at(i)==signo){
      return true;
    }
  }

  return false;
}


std::tuple<std::wstring, int> nombre_valor(std::vector<std::wstring> &y)
{
  std::wstring nombre;
  std::wstring espacio (L" "); 
  for(int i=0; i<(y.size()-1); i++){
    nombre.append(y[i] + espacio);
  }
  //Para quitar el último espacio y tener compatibilidad con los nombres en el inventario, hacemos lo siguiente:
  nombre.pop_back();
 
  std::wstring numero = y[y.size()-1];
  for(int i=0; i<numero.length(); i++){
    if(numero.at(i)=='$' || numero.at(i)==','){
      numero.erase(numero.begin() + i);
      i--;
    }
  }
  int valor = std::stoi(numero);  
  
  return std::make_tuple(nombre,valor);
}


std::tuple<int, int> cantidad_precio(std::vector<std::wstring> &y)
{
  y[0].pop_back();
  
  for(int i=0; i<y[2].length(); i++){
    if(y[2].at(i)==','){
      y[2].erase(y[2].begin() + i);
      i--;
    }
  }
  
  return std::make_tuple(std::stoi(y[0]), std::stoi(y[2]));
}

//Toca terminar la función con la lista del inventario

void informe_z(const char* name, std::string sarchivo, std::vector<std::tuple<std::wstring, std::wstring, std::wstring, std::vector<Producto> > > &y)
{
    std::vector<Producto> repetidos;
    std::vector<std::wstring> pcopy;
    std::vector<Producto> unidades;
    for(int i=0; i<y.size(); i++){
        std::vector<Producto> cosa = std::get<3>(y[i]);
        for(int j=0; j<cosa.size(); j++){
           Producto elemento;
           elemento.apodo = cosa[j].apodo;
           elemento.cantidad = cosa[j].cantidad;
           elemento.precio = cosa[j].precio;
           if(cosa[j].apodo == L"UNIDADES "){
               unidades.push_back(elemento);
           }
           else{
               repetidos.push_back(elemento);
               pcopy.push_back(elemento.apodo);
           }
        }
    }

    auto end = pcopy.end();
    for(auto it=pcopy.begin(); it!=end; ++it){
        end = std::remove(it + 1, end, *it);
    }
    pcopy.erase(end, pcopy.end());

    std::vector<Producto> lista;
    for(int i=0; i<pcopy.size(); i++){
        Producto elemento;
        elemento.apodo = pcopy[i];
        elemento.cantidad = 0;
        elemento.precio = 0;
        lista.push_back(elemento);
    }

    for(int i=0; i<lista.size(); i++){
        for(int j=0; j<repetidos.size(); j++){
            if(lista[i].apodo == repetidos[j].apodo){
                lista[i].cantidad += repetidos[j].cantidad;
                lista[i].precio = repetidos[j].precio;
            }
        }
    }

    for(int i=0; i<unidades.size(); i++){
        lista.push_back(unidades[i]);
    }

    std::wstring wsarchivo(sarchivo.begin() + 2, sarchivo.end() - 4);
    std::wofstream fout;
    fout.open(name, std::ios_base::app);
    fout.imbue(std::locale(""));
    fout << L"\t" << wsarchivo << L"\n\n";
    int total_productos = 0, total_precio = 0;
    for(int i=0; i<lista.size(); i++){
        int precio_total = lista[i].precio * lista[i].cantidad;
        fout << lista[i].apodo << L"\t" << lista[i].cantidad << L"\t" << lista[i].precio << L"\t" << precio_total << L"\n";
        total_productos += lista[i].cantidad;
        total_precio += precio_total;
    }
    fout << L"\n";
    fout << L"Número total de clientes:\t" << y.size() << L"\n";
    fout << L"Número de productos vendidos:\t" << total_productos << L"\n";
    fout << L"Total ingresos:\t" << total_precio << L"\n";
    fout << L"Promedio de venta por producto:\t" << total_precio / total_productos << L"\n";
    fout << L"\n\n";
    fout.close();
}



void imprimir_z(const char* name, std::vector<std::tuple<std::wstring, std::wstring, std::wstring, std::vector<Producto> > > &y)
{
    std::wofstream fout1;
    fout1.open(name, std::ios_base::app);
    fout1.imbue(std::locale(""));
    for(unsigned int i=0; i<y.size(); i++){
       std::wstring hora = L"Hora: ";
       std::wstring minuto = L"Minuto: ";
       std::wstring comprador = L"Comprador: ";
       std::wstring enter = L"\n";
       fout1 << hora << std::get<0>(y[i]) << enter << minuto << std::get<1>(y[i]) << enter << comprador << std::get<2>(y[i]) << enter;

       std::vector<Producto> productos = std::get<3>(y[i]);
       for(unsigned int j=0; j<productos.size(); j++){
           fout1 << L'\t';
           std::wstring nombre = productos[j].nombre;
           fout1 << productos[j].nombre;
           fout1 << L'\t';
           fout1 << productos[j].cantidad;
           fout1 << L'\t';
           fout1 << productos[j].precio;
           fout1 << L'\t';
           int precio_total = productos[j].cantidad * productos[j].precio;
           fout1 << precio_total;
           fout1 << L'\n';
       }
       fout1 << L'\n';
    }
    fout1.close();
}


void inventario_z(const char* name_in, std::vector<std::tuple<std::wstring, std::wstring, std::wstring, std::vector<Producto> > > &y)
{
    std::vector<std::vector<std::wstring> > leyo = leer_inventario(name_in);
    std::vector<Producto> para = inventario_organizado(leyo);
    std::vector<Producto> product;
    std::wcout << para.size() << std::endl;

    for(unsigned int i=0; i<y.size(); i++){
        product = std::get<3>(y[i]);
        for(unsigned int j=0; j<product.size(); j++){
            for(unsigned int k=0; k<para.size(); k++){
                if(para[k].apodo == product[j].apodo){
                    para[k].cantidad = para[k].cantidad - product[j].cantidad;
                    std::wcout << para[k].codigo << L"\t" << para[k].cantidad << std::endl;
                    break;
                }
            }
        }
    }

    std::wcout << L"Salí" << std::endl;
    const char* direction = "..\\..\\MACC\\inventario1.csv";
    inventario_imprimir(direction, para);

}


#endif
