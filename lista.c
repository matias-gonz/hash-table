#include "lista.h"
#include <stdlib.h>


#define TEMP 0 

const int ERROR = -1;
const int EXITO = 0;

typedef struct nodo{
	void* elemento;
	struct nodo* siguiente;
}nodo_t;

struct lista{
	nodo_t* primer_nodo;
	nodo_t* ultimo_nodo;
	size_t cantidad;
};

struct lista_iterador{
	nodo_t* nodo_it;
	lista_t* lista;
};

lista_t* lista_crear(){
	return calloc(1,sizeof(lista_t));
}


/*Pre: El puntero al nodo debe ser valido (que no sea null)
 *Post: Inicializara los campos del nodo con el elemento recibido y la direccion del nodo siguiente en la lista
*/
void inicializar_nodo(nodo_t* nodo, void* elemento, nodo_t* siguiente){
	if(!nodo) return;
	nodo->elemento = elemento;
	nodo->siguiente = siguiente;
}

int lista_insertar(lista_t* lista, void* elemento){
	if(!lista) return ERROR;

	nodo_t* nodo = calloc(1,sizeof(nodo_t));
	if(!nodo) return ERROR;

	inicializar_nodo(nodo,elemento,NULL);
	if(lista_vacia(lista)) lista->primer_nodo = nodo;
	else lista->ultimo_nodo->siguiente = nodo;
	
	lista->ultimo_nodo = nodo;
	lista->cantidad++;
	return EXITO;
}

/*Funcion recursiva
 *Pre:El nodo recibido en la primer llamada debe ser el primero de la lista
 *El indice recibido en la primer llamada debe ser 0
 *Pos es la posicion del nodo que se quiere en la lista
 *Post: Devuelve el nodo que se encuentra en determinada posicion.
 *Si la posicion es mas grande que la cantidad de nodos devolvera null
 *Si no hay nodos, devolvera null
*/
void* nodo_en_pos(nodo_t* nodo, int indice, size_t pos){
	if(!nodo || indice >= pos) return nodo;
	return nodo_en_pos(nodo->siguiente, indice+1, pos);
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
	if(!lista || (int)posicion < 0) return ERROR;
	if(posicion >= lista->cantidad) return lista_insertar(lista,elemento);
	
	nodo_t* nodo = calloc(1,sizeof(nodo_t));
	if(!nodo) return ERROR;	
		
	if(posicion == 0){
		inicializar_nodo(nodo,elemento,lista->primer_nodo);
		lista->primer_nodo = nodo;
	}else{
		nodo_t* nodo_anterior = nodo_en_pos(lista->primer_nodo,0,posicion-1);
		if(!nodo_anterior) return ERROR;
		inicializar_nodo(nodo,elemento,nodo_anterior->siguiente);
		nodo_anterior->siguiente = nodo;
	}

	lista->cantidad++;	
	return EXITO;
}

int lista_borrar(lista_t* lista){
	if(lista_vacia(lista)) return ERROR;
	if(!lista->ultimo_nodo) return ERROR;
	free(lista->ultimo_nodo);
	lista->cantidad--;
	if(lista->cantidad == 0){
		lista->primer_nodo = NULL;
		lista->ultimo_nodo = NULL;
	}else{
		nodo_t* nuevo_ultimo = nodo_en_pos(lista->primer_nodo,0,lista->cantidad-1);
		nuevo_ultimo->siguiente = NULL;
		lista->ultimo_nodo = nuevo_ultimo;	
	}

	return EXITO;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
	if(lista_vacia(lista)) return ERROR;
	if(posicion >= lista->cantidad-1) return lista_borrar(lista);
	if(posicion == 0){
		nodo_t* nodo_aux = lista->primer_nodo->siguiente;
		free(lista->primer_nodo);
		lista->primer_nodo = nodo_aux;
	}else{
		nodo_t* nodo_anterior = nodo_en_pos(lista->primer_nodo,0,posicion-1);
		nodo_t* nodo_aux = nodo_anterior->siguiente;
		nodo_anterior->siguiente = nodo_aux->siguiente;
		free(nodo_aux);
	}
	lista->cantidad--;	
	return EXITO;
}



void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
	if(!lista) return NULL;
	if(posicion >= lista->cantidad) return NULL;
	if(posicion == lista->cantidad-1) return lista_ultimo(lista);
	nodo_t* nodo = nodo_en_pos(lista->primer_nodo,0,posicion);
	if(!nodo) return nodo;
	return nodo->elemento;
}

void* lista_ultimo(lista_t* lista){
	if(lista_vacia(lista)) return NULL;
	if(!lista->ultimo_nodo) return NULL;
	return lista->ultimo_nodo->elemento;
}

bool lista_vacia(lista_t* lista){
	if (!lista) return true;
	return (lista_elementos(lista) == 0);
}

size_t lista_elementos(lista_t* lista){
	if(!lista) return (size_t)NULL;
	return lista->cantidad;
}

int lista_apilar(lista_t* lista, void* elemento){
	return lista_insertar(lista,elemento);
}

int lista_desapilar(lista_t* lista){
	return lista_borrar(lista);
}

void* lista_tope(lista_t* lista){
	return lista_ultimo(lista);
}

int lista_encolar(lista_t* lista, void* elemento){
	return lista_insertar(lista,elemento);
}

int lista_desencolar(lista_t* lista){
	return lista_borrar_de_posicion(lista,0);
}

void* lista_primero(lista_t* lista){
	return lista_elemento_en_posicion(lista,0);
}

/*Funcion Recursiva
 *Pre: El nodo recibido en la primer llamada debe ser el primero de la lista
 *Post: Le hara free() a cada nodo de la lista
*/
void destruir_recursivo(nodo_t* nodo){
	if(!nodo) return;
	destruir_recursivo(nodo->siguiente);
	free(nodo);
}

void lista_destruir(lista_t* lista){
	if(!lista) return;
	destruir_recursivo(lista->primer_nodo);
	free(lista);
}

lista_iterador_t* lista_iterador_crear(lista_t* lista){
	if(!lista) return NULL;
	lista_iterador_t* it = calloc(1,sizeof(lista_iterador_t));
	if(!it) return NULL;
	it->nodo_it = lista->primer_nodo;
	it->lista = lista;
	return it;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* iterador){
	if(!iterador) return false;

	return (iterador->nodo_it != NULL);
}

void* lista_iterador_siguiente(lista_iterador_t* iterador){
	if(!iterador || !lista_iterador_tiene_siguiente(iterador)) return NULL;	
	
	void* elemento = iterador->nodo_it->elemento;
	iterador->nodo_it = iterador->nodo_it->siguiente;

	return elemento;
}

void lista_iterador_destruir(lista_iterador_t* iterador){
	if(!iterador) return;
	free(iterador);
}

/*Funcion Recursiva
 *Pre: El nodo recibido en la primer llamada debe ser el primero de la lista
 *El puntero a funcion debera ser distinto a null
 *Post: Llamara a la funcion recibida por cada elemento de la lista
 *La funcion recibira el puntero al elemento y el puntero al contexto(dependera de cada funcion)
*/
void iterar_cada_elemento(nodo_t* nodo, void (*funcion)(void*, void*), void *contexto){
	if(!nodo || !funcion) return;
	funcion(nodo->elemento,contexto);
	iterar_cada_elemento(nodo->siguiente,funcion,contexto);	
}

void lista_con_cada_elemento(lista_t* lista, void (*funcion)(void*, void*), void *contexto){
	if(!lista || !funcion) return;

	iterar_cada_elemento(lista->primer_nodo,funcion,contexto);
}






