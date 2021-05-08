#include "conjunto.h"
#include <stdlib.h>

Conjunto conjunto_inicializar(char *alias){
    Conjunto conjunto = malloc(sizeof(PreConjunto));
    conjunto->alias = malloc(sizeof(char)*(strlen(alias) + 1));
    strcpy(conjunto->alias, alias);
    conjunto->datos = glist_inicializar();

    return conjunto;
}

void conjunto_destruir(void* conjunto) {
    if (conjunto){
        glist_destruir(((Conjunto)conjunto)->datos, &intervalo_destruir);
        free(((Conjunto)conjunto)->alias);
        free(conjunto);
    }
}

int conjunto_contiene(Conjunto conjunto, Intervalo *intervalo) {
    int esta = 0;
    GNodo *nodoAux = conjunto->datos->primero;
    for (; nodoAux != NULL && esta == 0; nodoAux = nodoAux->sig) {
        esta = intervalo_coinciden((Intervalo*)nodoAux->dato, intervalo);
    }
    return esta;
}

int conjunto_agregar(Conjunto conjunto, Intervalo *intervalo){
    if (!conjunto_contiene(conjunto, intervalo)){
        conjunto->datos = glist_agregar_final(conjunto->datos, (void*)intervalo);
        return 1;
    }

    return 0;
}

Conjunto conjunto_unir(Conjunto conjunto1, Conjunto conjunto2, char* alias){
    if (!conjunto1 && !conjunto2) return NULL;
    Conjunto conjuntoUnion = conjunto_inicializar(alias);
    GNodo *auxConj1 = conjunto1->datos->primero, *auxConj2 = conjunto2->datos->primero;
    for (; auxConj1 != NULL && auxConj2 != NULL;) {
        Intervalo *datoConj1 = (Intervalo*)auxConj1->dato;
        Intervalo *datoConj2 = (Intervalo*)auxConj2->dato;
        // si los intervalos a analizar no se intersecan directamente debo agregar el menor
        if (!intervalo_interseca(datoConj1, datoConj2)) {
            if (intervalo_pseudo_comp((void*)datoConj1, (void*)datoConj2) < 0) {
                conjunto_agregar(conjuntoUnion, intervalo_deep_copiar(datoConj1));
                auxConj1 = auxConj1->sig;
            } else {
                conjunto_agregar(conjuntoUnion, intervalo_deep_copiar(datoConj2));
                auxConj2 = auxConj2->sig;
            }
        } else {
            // si son iguales elijo cualquiera
            if (intervalo_coinciden(datoConj1, datoConj2))
                conjunto_agregar(conjuntoUnion, (Intervalo*)intervalo_deep_copiar((void*)datoConj1));
            else {
                // si intersecan creo uno nuevo con el menor de los extremos izquierdos
                // y el mayor de los derechos y comparo en caso de ser necesario con el ultimo
                // elemento que fue agregado al conjunto resultado
                Intervalo *nuevoInt = intervalo_crear(MIN(datoConj1->extIzq, datoConj2->extIzq), MAX(datoConj1->extDer, datoConj2->extDer));
                if (conjuntoUnion->datos->primero == NULL)
                    conjunto_agregar(conjuntoUnion, nuevoInt);
                else {
                    Intervalo *datoConj3 = (Intervalo*)conjuntoUnion->datos->ultimo->dato;
                    if (intervalo_interseca(datoConj3, nuevoInt) || datoConj3->extDer + 1 == nuevoInt->extIzq){
                        datoConj3->extDer = MAX(datoConj3->extDer, nuevoInt->extDer);
                        intervalo_destruir(nuevoInt);
                    } else
                        conjunto_agregar(conjuntoUnion, nuevoInt);
                }
            }
            // me desplazo en los conjuntos
            auxConj1 = auxConj1->sig;
            auxConj2 = auxConj2->sig;
        }
    }

    // en caso de que termine un conjunto pero no el otro debo
    // copiar lo restante del que aun no termina en el conjunto resultado
    GNodo* aCopiar = auxConj1 == NULL ? auxConj2 : auxConj1;
    for (;aCopiar != NULL; aCopiar = aCopiar->sig){
        Intervalo* actual = (Intervalo*)aCopiar->dato;
        Intervalo *datoConj3 = (Intervalo*)conjuntoUnion->datos->ultimo->dato;
        if (intervalo_interseca(datoConj3, actual) || datoConj3->extDer + 1 == actual->extIzq){
            datoConj3->extDer = MAX(datoConj3->extDer, actual->extDer);
        } else{
            conjunto_agregar(conjuntoUnion, (Intervalo*)intervalo_deep_copiar((void*)actual));
        }
    }
    return conjuntoUnion;
}

Conjunto conjunto_intersecar(Conjunto conjunto1, Conjunto conjunto2, char* alias){
    if (!conjunto1 && !conjunto2) return NULL;
    Conjunto conjuntoInterseccion = conjunto_inicializar(alias);
    GNodo *auxConj1 = conjunto1->datos->primero, *auxConj2 = conjunto2->datos->primero;
    for (; auxConj1 != NULL && auxConj2 != NULL;) {
        Intervalo *datoConj1 = (Intervalo*)auxConj1->dato;
        Intervalo *datoConj2 = (Intervalo*)auxConj2->dato;
        // si no hay interseccion entre los intervalos procedo a desplazarme
        // en el conjunto que tenga el intervalo mas "chico"
        if (!intervalo_interseca(datoConj1, datoConj2)) {
            if (intervalo_pseudo_comp((void*)datoConj1, (void*)datoConj2) < 0)
                auxConj1 = auxConj1->sig;
            else 
                auxConj2 = auxConj2->sig;
        } else {
            // si son iguales meto cualquiero
            if (intervalo_coinciden(datoConj1, datoConj2))
                conjunto_agregar(conjuntoInterseccion, (Intervalo*)intervalo_deep_copiar((void*)datoConj1));
            else {
                // si hay interseccion creo el intervalo tentativo a insertar
                // y comparo con el ultimo elemento agregado al conjunto solucion
                // si es necesario
                Intervalo *nuevoInt = intervalo_crear(MAX(datoConj1->extIzq, datoConj2->extIzq), MIN(datoConj1->extDer, datoConj2->extDer));
                if (conjuntoInterseccion->datos->primero == NULL)
                    conjunto_agregar(conjuntoInterseccion, nuevoInt);
                else {
                    Intervalo *datoConj3 = (Intervalo*)conjuntoInterseccion->datos->ultimo->dato;
                    if (intervalo_interseca(datoConj3, nuevoInt) || datoConj3->extDer + 1 == nuevoInt->extIzq){
                        datoConj3->extDer = MAX(datoConj3->extDer, nuevoInt->extDer);
                        intervalo_destruir(nuevoInt);
                    } else
                        conjunto_agregar(conjuntoInterseccion, nuevoInt);
                }
            }

            // me muevo al siguiente de cada conjunto solo si coincide el extremo derecho de mi intervalo
            // con el extremo derecho del ultimo elemento de mi interseccion hasta el momento
            auxConj1 = ((Intervalo*)conjuntoInterseccion->datos->ultimo->dato)->extDer == ((Intervalo*)auxConj1->dato)->extDer ? auxConj1->sig : auxConj1;
            auxConj2 = ((Intervalo*)conjuntoInterseccion->datos->ultimo->dato)->extDer == ((Intervalo*)auxConj2->dato)->extDer ? auxConj2->sig : auxConj2;
        }
    }

    return conjuntoInterseccion;
}

Conjunto conjunto_complemento(Conjunto conjunto, char* alias){
    if (!conjunto) return NULL;
    Conjunto conjuntoComplemento = conjunto_inicializar(alias);
    // conjunto vacio ya conozco el complemento
    if(conjunto->datos->primero == NULL) {
        Intervalo *intervalo = intervalo_crear(INT_MIN, INT_MAX);
        conjunto_agregar(conjuntoComplemento, intervalo);
        return conjuntoComplemento;
    }
    GNodo *nodoAux = conjunto->datos->primero;
    // conjunto universo ya conozco el complemento
    if(((Intervalo*)nodoAux->dato)->extIzq == INT_MIN && ((Intervalo*)nodoAux->dato)->extDer == INT_MAX)
        return conjuntoComplemento;
    int izq = INT_MIN, der;
    // recorro el conjunto y voy guardando los complementos correspondientes
    for(;nodoAux != NULL;nodoAux = nodoAux->sig) {
        // si el extremo izquierdo del intervalo actual
        // no coincide con el "minimo" determinado entonces
        // ya conozco parte del complemento
        if(((Intervalo*)nodoAux->dato)->extIzq != izq){
            der = ((Intervalo*)nodoAux->dato)->extIzq - 1;
            Intervalo* intervalo = intervalo_crear(izq,der);
            conjunto_agregar(conjuntoComplemento, intervalo);
        }
        // luego de ese analisis debo actualizar el "minimo"
        izq = ((Intervalo*)nodoAux->dato)->extDer + 1;
    }
    // analizo el ultimo intervalo del conjunto para calcular el ultimo complemento
    // ya que el bucle anterior analiza el complemento que estaria a la izquierda
    // de cada intervalo por lo tanto puede ser necesario 
    // (si el ultimo intervalo no tiene como extremo derecho INT_MAX) analizar la derecha del ultimo intervalo

    if(((Intervalo*)conjunto->datos->ultimo->dato)->extIzq != INT_MAX){
        izq = ((Intervalo*)conjunto->datos->ultimo->dato)->extDer + 1;
        Intervalo* intervalo = intervalo_crear(izq,INT_MAX);
        conjunto_agregar(conjuntoComplemento, intervalo);
    }

    return conjuntoComplemento;
}

Conjunto conjunto_resta(Conjunto conjunto1, Conjunto conjunto2, char* alias){
    //la resta es la interseccion del primero con el complemento del segundo
    Conjunto complemento = conjunto_complemento(conjunto2, "aliasAux");
    Conjunto conjuntoResta = conjunto_intersecar(conjunto1, complemento, alias);
    // destruyo el conjunto auxiliar
    conjunto_destruir(complemento);
    return conjuntoResta;
}


void conjunto_imprimir(Conjunto conjunto){
    if (conjunto){
        if(conjunto->datos->primero == NULL) printf("{}\n");
        else {
            GNodo *nodoAux = conjunto->datos->primero;
            for (; nodoAux != NULL; nodoAux = nodoAux->sig){
                intervalo_imprimir(nodoAux->dato);
                if (nodoAux->sig != NULL){
                    printf(", ");
                }
            }
            printf("\n");    
        }
    }
}

int conjunto_comparar_alias(void* alias, void* conjunto){
    return strcmp((char*)alias, ((Conjunto)conjunto)->alias);
}

int conjunto_comparar_alias_v2(void* conjunto1, void* conjunto2){
    return strcmp(((Conjunto)conjunto1)->alias, ((Conjunto)conjunto2)->alias);
}