#include "hash.h"
#include "hash_iterador.h"
#include "lista.h"
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

#define MAX_CLAVE 100

static const int EXITO = 0;
static const int ERROR = -1;

const size_t CAPACIDAD_MINIMA = 3;
const size_t FACTOR_HASH = 53;
const double UMBRAL = 0.5;

typedef struct node{
	char clave[MAX_CLAVE];
	void* elemento;
}node_t;

typedef struct tabla{
	bool contiene;
	lista_t* elementos;
}tabla_t;

struct hash{
	hash_destruir_dato_t destructor;
	size_t capacidad;
	size_t ocupados;
	tabla_t* tabla;
};

struct hash_iter{
	tabla_t* tabla;
	size_t capacidad;
	size_t cantidad_elementos;
	size_t posicion;
	lista_iterador_t* iterador_lista;	
};


//Devuelve true si el numero recibido es primo
bool es_primo(size_t num){
	for(size_t i = 2; i < num/2; i++){
		if(num%i == 0) return false;
	}
	return true;
}

//Dado un numero, devuelve el numero primo mayor mas cercano a este
size_t proximo_primo(size_t num){
	if(es_primo(num)){
		return num;
	}
	return proximo_primo(num+1);
}

//Devuelve una capacidad valida mayor o igual al numero recibido (numero primo mayor o igual a 3)
size_t obtener_capacidad(size_t size){
	if(size < CAPACIDAD_MINIMA){
		return CAPACIDAD_MINIMA;
	}
	return proximo_primo(size);
}

/*Pre: la lista recibida debe contener estructuras de node_t
 *Post: Si existe un elemento con la clave recibida en la lista recibida, devuelve su posicion en ella
 *Devuelve -1 si no no encuentra
*/
int obtener_posicion(lista_t* lista,char* clave){
	if(!lista) return ERROR;
	lista_iterador_t* it = lista_iterador_crear(lista);
	if(!it){
		return ERROR;
	}
	node_t* node;
	int posicion = 0;
	while(lista_iterador_tiene_siguiente(it)){
		node = (node_t*)lista_iterador_siguiente(it);
		if(strcmp(node->clave,clave) == 0){
			lista_iterador_destruir(it);
			return posicion;
		}
		posicion++;	
	}
	lista_iterador_destruir(it);
	return ERROR;
}

/*Pre: La capacidad debe ser la longitud del vector tabla
 *En cada posicion de tabla, el campo 'contiene' debe ser true solo si existe una lista en el campo 'elementos'
 *Post: Libera la memoria reservada para la lista 'elementos'
 *No llama al destructor para cada elemento contenido
*/
void destruir_listas(tabla_t* tabla,size_t capacidad){
	for(size_t i = 0; i < capacidad; i++){
		if(tabla[i].contiene){
			lista_destruir(tabla[i].elementos);
		}
	}
}

hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad){
	hash_t* hash = calloc(1,sizeof(hash_t));
	if(!hash) return NULL;
	hash->destructor = destruir_elemento;
	size_t size = obtener_capacidad(capacidad);
	hash->capacidad = size;
	hash->tabla = calloc(1,size*sizeof(tabla_t));
	if(!hash->tabla){
		free(hash);
		return NULL;
	}
	return hash;
}

//Devuelve el resultado de elevar base a la potencia recibida
size_t power(size_t base,size_t potencia){
	size_t resultado = 1;
	for(size_t i = 0; i < potencia;i++){
		resultado *= base;
	}
	return resultado;
}

/*Pre: la capacidad debe corresponder con la cantidad de posiciones en la tabla del hash
 *Post: Dada una clave y capacidad devuelve una posicion en la tabla del hash
*/
size_t funcion_hash(char* clave, size_t capacidad){
	size_t clave_longitud = strlen(clave);
	size_t valor_hash = 0;
	for(size_t i = 0; i < clave_longitud; i++){
		valor_hash += (size_t)clave[i]*power(FACTOR_HASH,i);
	}
	return valor_hash%capacidad;
}


/*Pre: La capacidad del hash no puede ser 0
 *Post: Devuelve true si la cantidad de elementos en el hash supera cierto umbral
 *(constante UMBRAL)
*/
bool hash_umbral(hash_t* hash){
	return ((double)hash->ocupados/((double)hash->capacidad) > UMBRAL);
}



/*Pre: La lista recibida debe contener estructoras de tipo node_t
 *Post: Libera la memoria reservada para cada elemento de la lista
 *Si se le pasa un destructor valido, tambien libera la memoria de los elementos insertados en el hash
*/
void destructor_nodes(lista_t* nodes,hash_destruir_dato_t destructor){
	lista_iterador_t* it = lista_iterador_crear(nodes);
	if(!it){
		return;
	}
	node_t* node;
	while(lista_iterador_tiene_siguiente(it)){
		node = (node_t*)lista_iterador_siguiente(it);
		if(destructor){
			destructor(node->elemento);
		}
		free(node);
	}
	lista_iterador_destruir(it);
}

/*Pre: La capacidad debe ser la longitud del vector tabla
 *En cada posicion de tabla, el campo 'contiene' debe ser true solo si existe una lista en el campo 'elementos'
 *Post: Destruye todos los node_t creados e insertados en las listas de la tabla, si se le pasa un destructor valido,
 *tambien destruye a los elementos insertados.
*/
void destruir_elementos(tabla_t* tabla, size_t size,hash_destruir_dato_t destructor){
	if(!tabla) return;
	for(size_t i = 0; i < size; i++){
		destructor_nodes(tabla[i].elementos,destructor);
	}
}

/*Pre: Posicion debe estar dentro de los limites de la tabla
 *En cada posicion de tabla, el campo 'contiene' debe ser true solo si existe una lista en el campo 'elementos'
 *La clave recibida no puede contener mas de 99 caracteres
 *Post: Inserta un elemento en la posicion dada en la tabla,
 *Si no hay una lista en esa posicion reserva la memoria para ella, tambien para el contenedor del elemento y clave
 *Devuelve true si pudo insertar
 *False en caso de error
*/
bool tabla_insertar(tabla_t* tabla,size_t posicion,char* clave,void* elemento){
	if(!tabla) return false;
	if(!tabla[posicion].contiene){
		tabla[posicion].elementos = lista_crear();
		if(!tabla[posicion].elementos){
			return false;
		}
		tabla[posicion].contiene = true;
	}
	node_t* node = calloc(1,sizeof(node_t));
	if(!node){
		return false;
	}
	strcpy(node->clave,clave);
	node->elemento = elemento;
	if(lista_insertar(tabla[posicion].elementos,node) == ERROR){
		free(node);
		return false;
	}
	return true;
}

/*Pre: La lista recibida debe contener estructoras de tipo node_t
 *En cada posicion de tabla, el campo 'contiene' debe ser true solo si existe una lista en el campo 'elementos'
 *Post: Inserta los elementos de una lista en una tabla hash
 *Devuelve true si puede insertar todos los elementos, false en caso de error
*/
bool reinsertar_lista(lista_t* lista,tabla_t* tabla,size_t capacidad){
	if(!lista || !tabla) return false;
	lista_iterador_t* it = lista_iterador_crear(lista);
	if(!it){
		return false;
	}
	node_t* node;
	size_t posicion;	
	while(lista_iterador_tiene_siguiente(it)){
		node = (node_t*)lista_iterador_siguiente(it);
		posicion = funcion_hash(node->clave,capacidad);	
		if(!tabla_insertar(tabla,posicion,node->clave,node->elemento)){
			lista_iterador_destruir(it);
			return false;
		}
	}
	lista_iterador_destruir(it);
	return true;
}

/*Pre: 
 *En cada posicion de tabla, el campo 'contiene' debe ser true solo si existe una lista en el campo 'elementos'
 *Post: modifica la capacidad del hash con el cuadrado de su capacidad original
 *Reinserta los elementos del hash en la tabla nueva.
 *Devuelve true si puede insertar todos, false en caso de error
*/
bool ajustar_capacidad(hash_t* hash){
	if(!hash) return false;
	size_t nueva_capacidad = proximo_primo(power(hash->capacidad,2));
	tabla_t* nueva_tabla = calloc(1,nueva_capacidad*sizeof(tabla_t));
	if(!nueva_tabla) return false;
	for(size_t i = 0; i<hash->capacidad;i++){
		if(hash->tabla[i].contiene){
			if(!reinsertar_lista(hash->tabla[i].elementos,nueva_tabla,nueva_capacidad)){
				destruir_elementos(nueva_tabla,nueva_capacidad,NULL);
				free(nueva_tabla);
				return false;
			}
		}
	}
	destruir_elementos(hash->tabla,hash->capacidad,NULL);
	destruir_listas(hash->tabla,hash->capacidad);
	free(hash->tabla);
	hash->tabla = nueva_tabla;
	hash->capacidad = nueva_capacidad;
	return true;
}

/*Pre: Posicion debe estar dentro de los limites de la tabla
 *En cada posicion de tabla, el campo 'contiene' debe ser true solo si existe una lista en el campo 'elementos'
 *Post: Checkea si la clave recibida ya existe en el hash, si la encuentra, libera la memoria reservada para su elemento correspondiente
 *Devuelve true si encuentra la clave, false en caso de que no
*/
bool clave_repetida(tabla_t* tabla,size_t posicion,char* clave, hash_destruir_dato_t destructor){
	if(!tabla[posicion].contiene) return false;
	int posicion_elemento = obtener_posicion(tabla[posicion].elementos,clave);
	if(posicion_elemento == ERROR) return false;
	node_t* node = (node_t*)lista_elemento_en_posicion(tabla[posicion].elementos,(size_t)posicion_elemento);
	if(!node) return false;
	if(lista_borrar_de_posicion(tabla[posicion].elementos,(size_t)posicion_elemento) == ERROR){
		return false;
	}
	if(destructor){
		destructor(node->elemento);
	}
	free(node);
	return true;
}

int hash_insertar(hash_t* hash, const char* clave, void* elemento){
	if(!hash || !clave) return ERROR;
	if(hash_umbral(hash)){
		if(!ajustar_capacidad(hash)){
			//
			return ERROR;
		}
	}
	size_t posicion = funcion_hash((char*)clave,hash->capacidad);
	if(clave_repetida(hash->tabla,posicion,(char*)clave,hash->destructor)){
		hash->ocupados--;
	}
	if(!tabla_insertar(hash->tabla,posicion,(char*)clave,elemento)){
		return ERROR;
	}
	hash->ocupados++;
	return EXITO;
}

/*Pre: El campo casilla debe contener una lista que almacena los elementos
 *Post: Devuelve el elemento correspondiente a la clave recibida, devuelve NULL en caso de no encontrarlo
*/
void* obtener_de_posicion(tabla_t casilla,char* clave){
	int posicion = obtener_posicion(casilla.elementos,clave);
	if(posicion == ERROR) return NULL;
	node_t* node = (node_t*)lista_elemento_en_posicion(casilla.elementos,(size_t)posicion);	
	if(!node) return NULL;
	return node->elemento;
}

/*Pre: El campo casilla debe contener una lista que almacena los elementos
 *Post: Elimina la memoria reservada para el elemento almacenado en la casilla recibida correspondiente con la clave recibida
 *Si el destructor recibido es valido, tambien libera el elemento almacenado
*/
int eliminar_de_posicion(tabla_t casilla,char* clave,hash_destruir_dato_t destructor){
	if(!casilla.contiene) return false;
	int posicion = obtener_posicion(casilla.elementos,clave);
	if(posicion == ERROR) return false;
	node_t* node = (node_t*)lista_elemento_en_posicion(casilla.elementos,(size_t)posicion);	
	if(!node) return false;
	if(lista_borrar_de_posicion(casilla.elementos,(size_t)posicion) == ERROR){
		return false;
	}
	if(destructor){
		destructor(node->elemento);
	}
	free(node);
	return true;
}


int hash_quitar(hash_t* hash, const char* clave){
	if(!hash) return ERROR;
	size_t posicion = funcion_hash((char*)clave,hash->capacidad);
	if(!eliminar_de_posicion(hash->tabla[posicion],(char*)clave,hash->destructor)){
		return ERROR;
	}
	hash->ocupados--;
	return EXITO;
}

void* hash_obtener(hash_t* hash, const char* clave){
	if(!hash) return NULL;
	size_t posicion = funcion_hash((char*)clave,hash->capacidad);	
	return obtener_de_posicion(hash->tabla[posicion],(char*)clave);
}

bool hash_contiene(hash_t* hash, const char* clave){
	if(!hash) return false;
	size_t posicion = funcion_hash((char*)clave,hash->capacidad);
	if(!hash->tabla[posicion].contiene) return false;
	int posicion_elemento = obtener_posicion(hash->tabla[posicion].elementos,(char*)clave);
	return (posicion_elemento != ERROR);
}

size_t hash_cantidad(hash_t* hash){
	if(!hash) return 0;
	return hash->ocupados;
}



void hash_destruir(hash_t* hash){
	if(!hash) return;
	destruir_elementos(hash->tabla,hash->capacidad,hash->destructor);
	destruir_listas(hash->tabla,hash->capacidad);
	free(hash->tabla);
	free(hash);
}

/*Pre: La lista recibida debe contener estructoras de tipo node_t
 *Post: Llama a la funcion recibida con cada elemento en la lista,
 *Aumenta el contador por cada elemento que itera
 *Si la funcion devuelve true, devuelve true, sino false
 *
*/
bool iterar_lista(hash_t* hash, lista_t* lista, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux, size_t* contador){
	if(!lista || !funcion || !contador) return false;
	lista_iterador_t* it = lista_iterador_crear(lista);
	if(!it){
		return false;
	}
	node_t* node;
	while(lista_iterador_tiene_siguiente(it)){
		node = (node_t*)lista_iterador_siguiente(it);
		(*contador)++;
		if(funcion(hash,node->clave,aux)){
			lista_iterador_destruir(it);
			return true;
		}
	}
	lista_iterador_destruir(it);
	return false;
}

size_t hash_con_cada_clave(hash_t* hash, bool (*funcion)(hash_t* hash, const char* clave, void* aux), void* aux){
	if(!hash || !funcion) return 0;
	size_t contador = 0;
	for(size_t i = 0; i< hash->capacidad;i++){
		if(hash->tabla[i].contiene){
			if(iterar_lista(hash,hash->tabla[i].elementos,funcion,aux,&contador)){
				return contador;
			}
		}	
	}
	return contador;
}




hash_iterador_t* hash_iterador_crear(hash_t* hash){
	if(!hash) return NULL;
	hash_iterador_t* it = calloc(1,sizeof(hash_iterador_t));
	if(!it) return NULL;
	it->tabla = hash->tabla;
	it->capacidad = hash->capacidad;
	it->cantidad_elementos = hash->ocupados;
	return it;
}

const char* hash_iterador_siguiente(hash_iterador_t* iterador){
	if(!iterador) return NULL;
	if(iterador->iterador_lista){
		if(lista_iterador_tiene_siguiente(iterador->iterador_lista)){
			node_t* node = (node_t*)lista_iterador_siguiente(iterador->iterador_lista);
			if(!node){
				return NULL;
			}
			iterador->cantidad_elementos--;
			return node->clave;
		}
		lista_iterador_destruir(iterador->iterador_lista);
		iterador->iterador_lista = NULL;
	}

	for(size_t i = iterador->posicion+1; i < iterador->capacidad;i++){
		if(iterador->tabla[i].contiene && !lista_vacia(iterador->tabla[i].elementos)){
			iterador->posicion = i;
			lista_iterador_t* it = lista_iterador_crear(iterador->tabla[i].elementos);
			if(!it) return NULL;
			iterador->iterador_lista = it;
			node_t* node = (node_t*)lista_iterador_siguiente(iterador->iterador_lista);
			if(!node){
				return NULL;
			}
			iterador->cantidad_elementos--;
			return node->clave;
		}
	}
	return NULL;
}

bool hash_iterador_tiene_siguiente(hash_iterador_t* iterador){
	if(!iterador) return false;
	return (iterador->cantidad_elementos > 0);
}

void hash_iterador_destruir(hash_iterador_t* iterador){
	if(!iterador) return;
	lista_iterador_destruir(iterador->iterador_lista);
	free(iterador);
}

