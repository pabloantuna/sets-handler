#include "gtree.h"

GTree gtree_crear() {
  return NULL;
}

GTree gtree_nuevo_nodo(void* dato) {
  GTree nodo = malloc(sizeof(GTNodo));
  nodo->izq = NULL;
  nodo->der = NULL;
  nodo->dato = dato;
  nodo->altura = 0;

  return nodo;
}

void gtree_destruir(GTree tree, FuncionDestruir destruir_dato) {
  if (tree != NULL) {
    gtree_destruir(tree->izq, destruir_dato);
    gtree_destruir(tree->der, destruir_dato);
    gtree_destruir_nodo(tree, destruir_dato);
  }
}

void gtree_destruir_nodo(GTNodo * tree, FuncionDestruir destruir_dato) {
  if (tree != NULL) {
    destruir_dato(tree->dato);
    free(tree);
  }
}

GTree gtree_insertar(GTree tree, void* dato, FuncionComparacion comparacion) {
  if (tree == NULL)
    tree = gtree_nuevo_nodo(dato);
  else {
      if(comparacion(dato, tree->dato)){
        if (comparacion(dato, tree->dato) < 0)
          tree->izq = gtree_insertar(tree->izq, dato, comparacion);
        else
          tree->der = gtree_insertar(tree->der, dato, comparacion);

        tree->altura = gtree_nueva_altura(tree);

        tree = gtree_balancear(tree);
      } else 
          printf("El dato ingresado ya se encuentra en el arbol\n");

  }

  return tree;
}

void* gtree_buscar_dato(GTree tree, void* dato, FuncionComparacion comparacion) {
  void *datoBuscado = NULL;
  if (tree != NULL) {
    if(comparacion(dato, tree->dato) != 0){
      if (comparacion(dato, tree->dato) < 0)
        datoBuscado = gtree_buscar_dato(tree->izq, dato, comparacion);
      else
        datoBuscado = gtree_buscar_dato(tree->der, dato, comparacion);
    } else
      datoBuscado = tree->dato;
  }

  return datoBuscado;
}

int gtree_altura(GTree tree) {
  if (!tree)
    return -1;
  return tree->altura;
}

int gtree_nueva_altura(GTree tree) {
  if (!tree)
    return -1;
  if (gtree_altura(tree->izq) > gtree_altura(tree->der))
    return gtree_altura(tree->izq) + 1;
  else
    return gtree_altura(tree->der) + 1;
}

int gtree_balance_factor(GTree tree) {
  return gtree_altura(tree->izq) - gtree_altura(tree->der);
}

GTree gtree_rotar_derecha(GTree tree) {
  GTree aux = tree->izq;
  tree->izq = aux->der;
  aux->der = tree;

  //Altura
  tree->altura = gtree_nueva_altura(tree);
  aux->altura = gtree_nueva_altura(aux);

  return aux;
}

GTree gtree_rotar_izquierda(GTree tree) {
  GTree aux = tree->der;
  tree->der = aux->izq;
  aux->izq = tree;

  //Altura
  tree->altura = gtree_nueva_altura(tree);
  aux->altura = gtree_nueva_altura(aux);

  return aux;
}

GTree gtree_balancear(GTree tree) {
  if (tree) {
    int balanceFactor = gtree_balance_factor(tree);
    // Caso izquierda
    if (balanceFactor == 2) {
      // Caso izquierda-derecha
      if (gtree_balance_factor(tree->izq) < 0)
        tree->izq = gtree_rotar_izquierda(tree->izq);
      // Rotacion en ambos casos
      tree = gtree_rotar_derecha(tree);

    }
    // Caso derecha
    if (balanceFactor == -2) {
      // Caso derecha-izquierda
      if (gtree_balance_factor(tree->der) > 0)
        tree->der = gtree_rotar_derecha(tree->der);
      // Rotacion en ambos casos
      tree = gtree_rotar_izquierda(tree);
    }
  }

  return tree;
}

GTree gtree_obtener_minimo(GTree tree) {
  GTree aux = tree;
  if (aux) {
    if (!aux->izq) {
      return aux;
    }
  }
  return gtree_obtener_minimo(aux->izq);
}