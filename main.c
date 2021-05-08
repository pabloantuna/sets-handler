#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "./Conjunto/conjunto.h"
#include "./TablaHash/tablahash.h"
#define MAX_CHAR 1000

void imprimir_menu_principal() {
  printf("\t\t\tComandos\n\n");
  printf("A = {A1,A2,...,An}\tcrea un conjunto por extension\n");
  printf("A = {x : a <= x <= b}\tcrea un conjunto por comprension\n");
  printf("C = A | B\t\trealiza la union entre A y B y la guarda en C\n");
  printf("C = A & B\t\trealiza la interseccion entre A y B y la guarda en C\n");
  printf("C = A - B\t\trealiza la resta entre A y B y la guarda en C\n");
  printf("C = ~A\t\t\trealiza el complemento de A y lo guarda en C\n");
  printf("imprimir A\t\timprime el conjunto de alias A\n");
  printf("salir\t\t\tsale del programa\n\n");
}

void agregar_o_reemplazar(TablaHash tabla, char* alias, Conjunto conjunto){
    Conjunto conjuntoT = tabla_buscar(tabla, alias);
    if (conjunto && conjuntoT){
        glist_destruir(conjuntoT->datos, &intervalo_destruir);
        conjuntoT->datos = conjunto->datos;
        free(conjunto->alias);
        free(conjunto);
    } else if (conjunto) {
        tabla_insertar(tabla, conjunto);
    }
}

Conjunto leer_conjunto_extension(char* comando, char* alias) {
    Conjunto conjunto = conjunto_inicializar(alias);   
    int indice = 1;
    while (comando[indice] != '}') {
        if(isdigit(comando[indice]) || (comando[indice] == '-' && isdigit(comando[indice+1]))) {
            int extremo = strtol(comando + indice, &comando, 10);
            Intervalo *intervalo = intervalo_crear(extremo, extremo);
            if(!conjunto_agregar(conjunto, intervalo)) intervalo_destruir(intervalo);
            indice = 0;
        } else {
            if (comando[indice] == ',' || comando[indice] == ' ') {
                indice++;
            } else {
                printf("Formato no valido\n");
                conjunto_destruir(conjunto);
                return NULL;
            }
        }
    }
    conjunto->datos = glist_mergeSort(conjunto->datos, &intervalo_pseudo_comp);
    return conjunto;

}

Conjunto leer_conjunto_comprension(char* comando, char* alias) {
    Conjunto conjunto = conjunto_inicializar(alias);
    int extIzq, extDer;
    char var, var2;
    if (sscanf(comando, "{%c : %d <= %c <= %d}", &var, &extIzq, &var2, &extDer) == 4) {
        if (var == var2) {
            Intervalo* intervalo = intervalo_crear(extIzq, extDer);
            if (intervalo_validar(intervalo)){
                conjunto_agregar(conjunto, intervalo);
                return conjunto;
            }
            intervalo_destruir(intervalo);
            printf("La variable no se encuentra en un rango valido\n");
        } else
            printf("Las variables no coinciden\n");
    }
    printf("Formato no valido\n");
    conjunto_destruir(conjunto);

    return NULL;
}

int copiar_alias_operacion_obtener_operador(char *comando, char** alias1, char** alias2){
    int operador = -1;
    *alias1 = malloc(sizeof(char) * MAX_CHAR);
    *alias2 = malloc(sizeof(char) * MAX_CHAR);

    unsigned tamano_actual = MAX_CHAR;
    unsigned int i = 0;
    while (comando[i] != ' ' && comando[i] != '\0')
    {
        (*alias1)[i]=comando[i];
        i++;
        if(i == tamano_actual)
        {
            tamano_actual = i+MAX_CHAR;
            *alias1 = realloc(*alias1, tamano_actual);
        }
    }

    (*alias1)[i] = '\0';
    if (comando[i] != '\0' && comando[i+1] != '\0' && comando[i+2] != '\0' && comando[i+3] != '\0'){
        operador = comando[i+1];
        i += 3;
        unsigned int j = 0;
        tamano_actual = MAX_CHAR;
        while (comando[i] != '\0') {
            (*alias2)[j]=comando[i];
            i++;
            j++;
            if(j == tamano_actual)
            {
                tamano_actual = j+MAX_CHAR;
                *alias2 = realloc(*alias2, tamano_actual);
            }
        }
        (*alias2)[j] = '\0';
    }
    return operador;

}

void manejar_operaciones(TablaHash tabla, char *comando, char *alias) {
    if (comando[0] == '~') {
        Conjunto conjunto = tabla_buscar(tabla, comando+1);
        if (conjunto) {
            Conjunto complemento = conjunto_complemento(conjunto, alias);
            agregar_o_reemplazar(tabla, alias, complemento);
        } else 
            printf("El conjunto de alias %s no existe\n", comando+1);
    } else {
        char *alias1 = NULL, *alias2 = NULL;
        int operador;
        operador = copiar_alias_operacion_obtener_operador(comando, &alias1, &alias2);
        if (operador != -1){
            Conjunto conjunto1 = tabla_buscar(tabla, alias1), conjunto2 = tabla_buscar(tabla, alias2);
            Conjunto resultado = NULL;
            if (conjunto1 && conjunto2){
                switch (operador)
                {
                case '-':
                    resultado = conjunto_resta(conjunto1, conjunto2, alias);
                    break;
                
                case '|':
                    resultado = conjunto_unir(conjunto1, conjunto2, alias);
                    break;

                case '&':
                    resultado = conjunto_intersecar(conjunto1, conjunto2, alias);
                    break;

                default:
                    printf("La operacion ingresada no es valida\n");
                    break;
                }

                if (resultado)
                    agregar_o_reemplazar(tabla, alias, resultado);
            } else
                printf("El conjunto de alias %s no existe\n", conjunto1 ? alias2 : alias1);
        } else
            printf("Formato no valido\n");
        free(alias1);     
        free(alias2);     
    }
}

char* leer_comando(){
    char *comando = malloc(sizeof(char)*MAX_CHAR);
    unsigned tamano_actual = MAX_CHAR;
    int c;
    unsigned int i = 0;
    while (( c = getchar() ) != '\n')
    {
        comando[i]=(char)c;
        i++;
        if(i == tamano_actual)
        {
            tamano_actual = i+MAX_CHAR;
            comando = realloc(comando, tamano_actual);
        }
    }

    comando[i] = '\0';

    return comando;
}

int copiar_alias1_obtener_comando(char **alias, char *comando){
    int posicion = -1;
    *alias = malloc(sizeof(char)* MAX_CHAR);
    unsigned tamano_actual = MAX_CHAR;
    unsigned int i = 0;
    while (comando[i] != ' ' && comando[i] != '\0') {
        (*alias)[i]=comando[i];
        i++;
        if(i == tamano_actual) {
            tamano_actual = i+MAX_CHAR;
            *alias = realloc(*alias, tamano_actual);
        }
    }

    (*alias)[i] = '\0';
    if (comando[i] != '\0' && comando[i+1] != '\0'){
        if (comando[i+1] == '=' && comando[i+2] == ' ') posicion = i+3;
    }
    return posicion;
}

void crear_conjunto_vacio(TablaHash tabla, char *alias){
    Conjunto conjunto = conjunto_inicializar(alias);
    agregar_o_reemplazar(tabla, alias, conjunto);
}

int main() {
    imprimir_menu_principal();
    char *comando, *alias = NULL;
    int loop = 1, posicion;
    Conjunto conjunto = NULL;
    TablaHash tabla = tabla_crear();
    while (loop) {
        printf("Ingrese opcion\n");
        comando = leer_comando();
        if (!strcmp(comando,"salir")) {
            loop = 0;
            printf("Adios!\n");
        }
        else {
            if (strncmp(comando, "imprimir ", 9) == 0) {
                Conjunto aImprimir = tabla_buscar(tabla, comando+9);
                if (aImprimir)
                    conjunto_imprimir(aImprimir);
                else
                    printf("El conjunto de alias %s no existe\n", comando+9);
            } else {
                posicion = copiar_alias1_obtener_comando(&alias, comando);
                if(posicion != -1){
                    // si se ingresa un conjunto
                    if (comando[posicion] == '{') {
                        if(comando[strlen(comando) - 1] == '}'){
                            if (posicion + 1 == (int)strlen(comando) - 1)
                                crear_conjunto_vacio(tabla, alias);
                            else {
                                if (isdigit(comando[posicion+1]) || comando[posicion+1] == '-')
                                    conjunto = leer_conjunto_extension(comando+posicion, alias);
                                else
                                    conjunto = leer_conjunto_comprension(comando+posicion, alias);
                                agregar_o_reemplazar(tabla, alias, conjunto);
                            }
                        }
                    } else 
                        manejar_operaciones(tabla, comando+posicion, alias);
                }
                else
                    printf("Formato no valido\n");
                if (alias) free(alias);
            }
        }        
        if (comando) free(comando);
    }

    tabla_destruir(tabla);

    return 0;
}