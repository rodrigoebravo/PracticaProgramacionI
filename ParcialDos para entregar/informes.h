#ifndef INFORMES_H_INCLUDED
#define INFORMES_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Venta.h"
#include "utn.h"
void inf_printInforme(int cantidadUnidadesTotales, int cantidadVentasMontoMayor10mil, int cantidadVentasMontoMayor20mil, int cantidadTvsVendidas);
int inf_contarTotalUnidadesVendidas(void* pElement);
int inf_contarPorCriterioMontoMayor10mil(void* pElement);
int inf_contarPorCriterioMontoMayor20mil(void* pElement);
int inf_contarTotalUnidadesLCDVendidas(void* pElement);
int inf_contarCantidadVentasPorCriterio(void* pElement, void* valorCriterio);

#endif // INFORMES_H_INCLUDED
