#ifndef CLASES_H_INCLUDED
#define CLASES_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

class Producto
{

 public:
  std::wstring nombre;
  std::wstring apodo;
  int cantidad;
  int precio;
  int costo;
  int codigo;

};


class Factura
{

 public:
  std::wstring empresa;
  std::wstring numero;
  std::vector<std::wstring> f_entrega;
  bool contado;
  int credito;
  std::vector<std::wstring> f_pago;
  std::vector<Producto> productos;
  int costo_total;

};


#endif
