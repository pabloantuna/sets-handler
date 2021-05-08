#include "glist.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

GList glist_inicializar() {
  GList lista = malloc(sizeof(Extremos));
  lista->primero = NULL;
  lista->ultimo = NULL;
  lista->cantidad = 0;
  return lista;
}

GList glist_copiar(GList lista) {
  GList nuevaLista = glist_inicializar();
  GNodo *nodoAux = lista->primero;
  for (; nodoAux != NULL; nodoAux = nodoAux->sig)
    nuevaLista = glist_agregar_final(nuevaLista, nodoAux->dato);

  return nuevaLista;
}

GList glist_deep_cpy(GList lista, FuncionNombrePendiente copiarDato) {
  GList nuevaLista = glist_inicializar();
  GNodo *nodoAux = lista->primero;
  for (; nodoAux != NULL; nodoAux = nodoAux->sig)
    nuevaLista = glist_agregar_final(nuevaLista, copiarDato(nodoAux->dato));

  return nuevaLista;
}

GList glist_deep_cpy_ft(GList listaTgt, GNodo* listaSrc, FuncionNombrePendiente copiarDato) {
  GNodo *nodoAux = listaSrc;
  for (; nodoAux != NULL; nodoAux = nodoAux->sig)
    listaTgt = glist_agregar_final(listaTgt, copiarDato(nodoAux->dato));

  return listaTgt;
}

void glist_recorrer(GList lista, FuncionVisitante visit) {
  GNodo *nodoAux = lista->primero;
  for (; nodoAux != NULL; nodoAux = nodoAux->sig)
    visit(nodoAux->dato);
}

GList glist_agregar_final(GList lista, void *dato) {
  GNodo *nuevoNodo = malloc(sizeof(GNodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = NULL;
  nuevoNodo->ant = lista->ultimo;
  if (lista->ultimo != NULL)
    lista->ultimo->sig = nuevoNodo;
  if (lista->primero == NULL)
    lista->primero = nuevoNodo;
  lista->ultimo = nuevoNodo;
  lista->cantidad++;
  return lista;
}

void glist_destruir(GList lista, Destruir d) {
  GNodo *nodoAux = lista->primero;
  while (nodoAux != NULL) {
    d(nodoAux->dato);
    lista->primero = nodoAux->sig;
    free(nodoAux);
    nodoAux = lista->primero;
  }
  free(lista);
}

void glist_destruir_copia(GList lista) {
  GNodo *nodoAux = lista->primero;
  while (nodoAux != NULL) {
    lista->primero = nodoAux->sig;
    free(nodoAux);
    nodoAux = lista->primero;
  }
  free(lista);
}

void node_swap(GNodo * nodo1, GNodo * nodo2) {
  void *aux = nodo1->dato;
  nodo1->dato = nodo2->dato;
  nodo2->dato = aux;
}

GList glist_append(GNodo * nodo, GList lista) {
  nodo->ant = NULL;
  nodo->sig = lista->primero;
  lista->primero->ant = nodo;
  lista->primero = nodo;
  lista->cantidad++;

  return lista;
}

GList glist_merge(GList lista1, GList lista2, Compara c) {
  if (lista1->cantidad == 0)
    return lista2;
  if (lista2->cantidad == 0)
    return lista1;
  if (c(lista1->primero->dato, lista2->primero->dato) <= 0) {
    if (lista1->cantidad == 1) {
      lista1 = glist_append(lista1->primero, lista2);
    } else {
      GNodo *nodoAux = lista1->primero;
      lista1->primero = lista1->primero->sig;
      lista1->cantidad--;
      lista1 = glist_merge(lista1, lista2, c);
      lista1 = glist_append(nodoAux, lista1);
    }

    return lista1;
  } else {
    if (lista2->cantidad == 1) {
      lista2 = glist_append(lista2->primero, lista1);
    } else {
      GNodo *nodoAux = lista2->primero;
      lista2->primero = lista2->primero->sig;
      lista2->cantidad--;
      lista2 = glist_merge(lista1, lista2, c);
      lista2 = glist_append(nodoAux, lista2);
    }

    return lista2;
  }
}

GList glist_split(GList lista) {
  int medio = lista->cantidad / 2, indice = 0;
  GNodo *nodoAux = lista->primero;
  GList lista2 = malloc(sizeof(Extremos));
  for (; indice < medio; indice++, nodoAux = nodoAux->sig);
  lista2->primero = nodoAux;
  lista2->ultimo = lista->ultimo;
  lista->ultimo = nodoAux->ant;
  lista2->cantidad = lista->cantidad - medio;
  lista->cantidad = medio;

  lista2->primero->ant = NULL;
  lista->ultimo->sig = NULL;

  return lista2;
}

GList glist_mergeSort(GList lista, Compara c) {
  if (lista->cantidad <= 1)
    return lista;
  GList aux = lista;
  GList derecha = glist_split(lista);
  derecha = glist_mergeSort(derecha, c);
  lista = glist_mergeSort(lista, c);

  lista = glist_merge(lista, derecha, c);
  aux->primero = lista->primero;
  aux->ultimo = lista->ultimo;
  aux->cantidad = lista->cantidad;
  lista = aux;
  free(derecha);

  return lista;
}
