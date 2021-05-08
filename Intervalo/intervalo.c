#include "intervalo.h"
#include <stdlib.h>
#include <stdio.h>

Intervalo *intervalo_crear(int extIzq, int extDer) {
  Intervalo *nuevo = malloc(sizeof(Intervalo));
  nuevo->extIzq = extIzq;
  nuevo->extDer = extDer;

  return nuevo;
}

Intervalo *intervalo_deep_copiar(Intervalo *dato) {
  Intervalo *nuevo = malloc(sizeof(Intervalo));
  nuevo->extIzq = dato->extIzq;
  nuevo->extDer = dato->extDer;;

  return nuevo;
}

void intervalo_destruir(void * intervalo) {
  free(intervalo);
}

int intervalo_validar(Intervalo * intervalo) {
  return intervalo->extIzq <= intervalo->extDer;
}

int intervalo_interseca(Intervalo * intervaloA, Intervalo * intervaloB) {
  return !(intervaloA->extDer < intervaloB->extIzq
           || intervaloA->extIzq > intervaloB->extDer);
}

void intervalo_imprimir(void * intervalo) {
  int izq = ((Intervalo*)intervalo)->extIzq, der = ((Intervalo*)intervalo)->extDer;
  if(izq == der) printf("%d", izq);
  else printf("[%d:%d]", izq,der);
}

int intervalo_coinciden(Intervalo * intervaloA, Intervalo * intervaloB) {
  return intervaloA->extIzq == intervaloB->extIzq
      && intervaloA->extDer == intervaloB->extDer;
}

int intervalo_pseudo_comp(void *intervaloA, void *intervaloB){
  if (((Intervalo*)intervaloA)->extIzq < ((Intervalo*)intervaloB)->extIzq) return -1;
  if (((Intervalo*)intervaloA)->extIzq > ((Intervalo*)intervaloB)->extIzq) return 1;
  return 0;
}