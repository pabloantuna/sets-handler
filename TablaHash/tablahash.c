#include "tablahash.h"
#include <stdlib.h>

TablaHash tabla_crear(){
    TablaHash tabla = malloc(sizeof(GTree) * TAMANO);
    for (int i = 0; i < TAMANO; i++)
        tabla[i] = NULL;
    
    return tabla;
}

int crear_clave(char* alias){
    int clave = 0, base = 13, pot = 1;
    for (int i = 0; alias[i]; i++) {
        clave = (clave + alias[i] * pot) % TAMANO;
        pot = (pot * base) % TAMANO; 
    }
    return clave;
}

Conjunto tabla_buscar(TablaHash tabla, char* alias){
    GTree arbol = tabla[crear_clave(alias)];
    void* dato = gtree_buscar_dato(arbol, alias, &conjunto_comparar_alias);
    return (Conjunto)dato;
}

void tabla_insertar(TablaHash tabla, Conjunto conjunto){
    int clave = crear_clave(conjunto->alias);
    if (conjunto) {
        tabla[clave] = gtree_insertar(tabla[clave],(void*)conjunto, &conjunto_comparar_alias_v2);
    }
}

void tabla_destruir(TablaHash tabla) {
    for (int i = 0; i < TAMANO; i++)
        gtree_destruir(tabla[i], &conjunto_destruir);
    free(tabla);
}