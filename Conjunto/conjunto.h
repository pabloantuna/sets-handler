#ifndef __CONJUNTO_H__
#define __CONJUNTO_H__

#include "../GList/glist.h"
#include "../Intervalo/intervalo.h"
#include <limits.h>
#include <string.h>
#define MAX(a,b) a > b ? a : b
#define MIN(a,b) a < b ? a : b

typedef struct {
    GList datos;
    char *alias;
} PreConjunto;

typedef PreConjunto* Conjunto;

/**
 * Inicializa un conjunto con un alias dado
 */
Conjunto conjunto_inicializar(char *alias);

/**
 * Agrega un intervalo a un conjunto y devuelve 1 o 0 en caso de tener exito o fallar respectivamente
 */
int conjunto_agregar(Conjunto conjunto, Intervalo *intervalo);

/**
 * Destruye un conjunto dado
 */
void conjunto_destruir(void* conjunto);

/**
 * Dado dos conjuntos y un alias devuelve un tercer conjunto (con dicho alias) resultado de la union de los conjuntos dados
 */
Conjunto conjunto_unir(Conjunto conjunto1, Conjunto conjunto2, char* alias);

/**
 * Dado dos conjuntos y un alias devuelve un tercer conjunto (con dicho alias) resultado de la interseccion de los conjuntos dados
 */
Conjunto conjunto_intersecar(Conjunto conjunto1, Conjunto conjunto2, char* alias);

/**
 * Dado dos conjuntos y un alias devuelve un tercer conjunto (con dicho alias) resultado de la resta de los conjuntos dados (primero - segundo)
 */
Conjunto conjunto_resta(Conjunto conjunto1, Conjunto conjunto2, char* alias);

/**
 * Dado un conjunto y un alias devuelve un tercer conjunto (con dicho alias) resultado de calcular el complemento del conjunto dado
 */
Conjunto conjunto_complemento(Conjunto conjunto, char* alias);

/**
 * Dado un conjunto imprime sus elementos
 */
void conjunto_imprimir(Conjunto conjunto);

/**
 * Toma un alias y un conjunto y devuelve la comparacion entre el alias dado y el alias del conjunto dado
 */
int conjunto_comparar_alias(void* alias1, void* conjunto);

int conjunto_comparar_alias_v2(void* conjunto1, void* conjunto2);

#endif