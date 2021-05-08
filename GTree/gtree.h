#ifndef __GTREE_H__
#define __GTREE_H__

#include "../Conjunto/conjunto.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct _GTNodo {
  void* dato;
  int altura;
  struct _GTNodo *izq;
  struct _GTNodo *der;
} GTNodo;

typedef GTNodo *GTree;

typedef void (*FuncionVisitante) (void* dato);
typedef void (*FuncionDestruir) (void* dato);
typedef int (*FuncionComparacion) (void* dato1, void* dato2);

/**
 * Devuelve un arbol vacío.
 */
GTree gtree_crear();

/**
 * Devuelve un nuevo nodo con el valor de intervalo dado
 */
GTree gtree_nuevo_nodo(void* dato);

/**
 * Devuelve la altura asignada al arbol dado
 */
int gtree_altura(GTree tree);

/**
 * Calcula y devuelve la altura que debe tener el arbol dado
 */
int gtree_nueva_altura(GTree tree);

/**
 * Destruye el arbol dado
 */
void gtree_destruir(GTree tree, FuncionDestruir destruir_dato);

/**
 * Destruye unicamente el nodo dado
 */
void gtree_destruir_nodo(GTNodo* tree, FuncionDestruir destruir_dato);

/**
 * Inserta un dato dado en el arbol indicado
 */
GTree gtree_insertar(GTree tree, void* dato, FuncionComparacion comparacion);

/**
 * Devuelve un dato buscado en base a la funcion comparacion o NULL si no existe
 */
void* gtree_buscar_dato(GTree tree, void* dato, FuncionComparacion comparacion);

/**
 * Balancea el arbol dado
 */
GTree gtree_balancear(GTree tree);

/**
 * Aplica la rotacion a derecha al arbol dado
 */
GTree gtree_rotar_derecha(GTree tree);

/**
 * Aplica la rotacion a derecha al arbol dado
 */
GTree gtree_rotar_izquierda(GTree tree);

/**
 * Devuelve el factor de balance del arbol dado
 */
int gtree_balance_factor(GTree tree);

#endif                          /* __GTREE_H__ */