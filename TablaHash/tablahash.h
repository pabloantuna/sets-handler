#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__
#include "../GTree/gtree.h"
#include "../Conjunto/conjunto.h"
#define TAMANO 1000

typedef GTree* TablaHash;

/**
 * Crea una tabla hash
 */
TablaHash tabla_crear();

/**
 * Toma una cadena de caracteres y devuelve su clave de hasheo
 */
int crear_clave(char* alias);

/**
 * Toma un alias de conjunto y devuelve el conjunto si existe en la tabla, NULL en caso contrario
 */
Conjunto tabla_buscar(TablaHash tabla, char* alias);

/**
 * Dada la tabla y un conjunto inserta el mismo en dicha tabla
 */
void tabla_insertar(TablaHash tabla, Conjunto conjunto);

/**
 * Dada una tabla la destruye por completo
 */
void tabla_destruir(TablaHash tabla);

#endif