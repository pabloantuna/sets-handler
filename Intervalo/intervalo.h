#ifndef __INTERVALO_H__
#define __INTERVALO_H__

typedef struct {
  int extIzq;
  int extDer;
} Intervalo;

/**
 * Devuelve un intervalo formado por los extremos dados.
 */
Intervalo *intervalo_crear(int extIzq, int extDer);

/**
 * Devuelve una copia de un intervalo pasado como argumento.
 */
Intervalo *intervalo_deep_copiar(Intervalo *dato);

/**
 * Destruye el intervalo dado
 */
void intervalo_destruir(void * intervalo);

/**
 * Indica si el intervalo dado es valido
 */
int intervalo_validar(Intervalo * intervalo);

/**
 * Indica si los dos intervalos dados se intersecan
 */
int intervalo_interseca(Intervalo * intervaloA, Intervalo * intervaloB);

/**
 * Imprime en pantalla el intervalo dado
 */
void intervalo_imprimir(void * intervalo);

/**
 * Indica si dos intervalos dados son iguales
 */
int intervalo_coinciden(Intervalo * intervaloA, Intervalo * intervaloB);

/**
 * Compara dos intervalos en base a su extremo izquierdo
 */
int intervalo_pseudo_comp(void *intervaloA, void *intervaloB);

#endif                          /* __INTERVALO_H__ */