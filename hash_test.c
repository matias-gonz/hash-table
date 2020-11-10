#include "hash.h"
#include "hash_iterador.h"
#include "test.h"
#include<stdlib.h>

static const int EXITO = 0;
static const int ERROR = -1;


/*Pre: Hash debe tener todos los campos correctamente cargados
 *Los elementos del hash deben ser ints
 *Suma debe apuntar a un int
 *Post: Suma el int que tiene la clave recibida al int suma
 *Devuelve true en caso de error, false sino
*/
bool obtener_suma(hash_t* hash, const char* clave,void* suma){
	if(!hash || !clave || !suma){
		return true;
	}
	int* elemento = hash_obtener(hash,clave);
	if(!elemento) return true;
	*(int*)suma += *elemento;
	return false;	
}

//Prueba la creacion de un hash con la capacidad y destructor recibidos
void probar_crear_capacidad(stats_t* stats, size_t capacidad, hash_destruir_dato_t destructor){
	hash_t* hash = hash_crear(destructor,capacidad);
	test_check(stats,"Se crea hash correctamente",hash);
	test_check(stats,"Hash se crea vacio",(hash_cantidad(hash) == 0));
	test_check(stats,"Hash no contiene clave aleatoria",(!hash_contiene(hash,"Vanukas")));
	hash_destruir(hash);
}

void funcion_que_no_hace_nada(const void* algo){

}

/*||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
 *||													||
 *||				   FUNCIONES ESPECIFICAS DE INSERTAR:					||
 *||													||
 *||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
*/

void probar_insertar_un_elemento_destructor_null(stats_t* stats){
	test_anunciar_prueba("Prueba un solo elemento, destructor NULL");
	hash_t* hash = hash_crear(NULL,3);
	int a = 1;
	char* clave_a = "Vanukas";
	char* clave_b = "Pirtelo";
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_a,&a) == EXITO));
	test_check(stats,"Hash contiene un elemento",(hash_cantidad(hash) == 1));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_a)));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_a) == &a));
	test_check(stats,"Hash NO obtiene elemento incorrecto",(hash_obtener(hash,clave_b) == NULL));
	test_check(stats,"Hash NO contiene elemento incorrecto",(!hash_contiene(hash,clave_b)));
	hash_destruir(hash);
}
void probar_insertar_elementos_destructor_null(stats_t* stats){
	test_anunciar_prueba("Prueba varios elementos, destructor NULL");
	hash_t* hash = hash_crear(NULL,3);
	int a = 1,b = 2,c = 3,d = 4, e = 5;
	char* clave_a = "Vanukas";
	char* clave_b = "Pirtelo";
	char* clave_c = "Maito";
	char* clave_d = "Piima";
	char* clave_e = "Jugurtti";
	char* clave_f = "Kaakao";
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_a,&a) == EXITO));
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_b,&b) == EXITO));
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_c,&c) == EXITO));
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_d,&d) == EXITO));
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_e,&e) == EXITO));
	test_check(stats,"Hash contiene un elemento",(hash_cantidad(hash) == 5));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_a)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_b)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_c)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_d)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_e)));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_a) == &a));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_b) == &b));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_c) == &c));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_d) == &d));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_e) == &e));
	test_check(stats,"Hash NO obtiene elemento incorrecto",(hash_obtener(hash,clave_f) == NULL));
	test_check(stats,"Hash NO contiene elemento incorrecto",(!hash_contiene(hash,clave_f)));
	hash_destruir(hash);
}

void probar_insertar_elementos_repetidos_destructor_null(stats_t* stats){
	test_anunciar_prueba("Prueba varios elementos repetidos, destructor NULL");
	hash_t* hash = hash_crear(NULL,3);
	int a = 1,b = 2,c = 3,d = 4,e = 5;
	char* clave_a = "Vanukas";
	char* clave_b = "Pirtelo";
	char* clave_c = "Maito";
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_a,&a) == EXITO));
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_b,&b) == EXITO));
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_c,&c) == EXITO));
	test_check(stats,"Hash contiene tres elementos",(hash_cantidad(hash) == 3));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_a)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_b)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_c)));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_a) == &a));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_b) == &b));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_c) == &c));
	test_check(stats,"Se inserta un elemento clave repetida",(hash_insertar(hash,clave_b,&d) == EXITO));
	test_check(stats,"Se inserta un elemento clave repetida",(hash_insertar(hash,clave_c,&e) == EXITO));
	test_check(stats,"Hash contiene tres elementos",(hash_cantidad(hash) == 3));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_a)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_b)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_c)));
	test_check(stats,"Hash sobrescribe elemento correcto",(hash_obtener(hash,clave_b) == &d));
	test_check(stats,"Hash sobrescribe elemento correcto",(hash_obtener(hash,clave_c) == &e));
	hash_destruir(hash);
}

void probar_insertar_un_elemento_con_destructor(stats_t* stats){
	test_anunciar_prueba("Prueba un solo elemento con destructor (free)");
	hash_t* hash = hash_crear(&free,3);
	int* a = malloc(sizeof(int));
	*a = 1;
	char* clave_a = "Vanukas";
	char* clave_b = "Pirtelo";
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_a,a) == EXITO));
	test_check(stats,"Hash contiene un elemento",(hash_cantidad(hash) == 1));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_a)));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_a) == a));
	test_check(stats,"Hash NO obtiene elemento incorrecto",(hash_obtener(hash,clave_b) == NULL));
	test_check(stats,"Hash NO contiene elemento incorrecto",(!hash_contiene(hash,clave_b)));
	hash_destruir(hash);
}
void probar_insertar_elementos_con_destructor(stats_t* stats){
	test_anunciar_prueba("Prueba varios elementos con destructor (free)");
	hash_t* hash = hash_crear(&free,3);
	int* a = malloc(sizeof(int));
	int* b = malloc(sizeof(int));
	int* c = malloc(sizeof(int));
	int* d = malloc(sizeof(int));
	int* e = malloc(sizeof(int));
	*a = 1;
	*b = 2;
	*c = 3;
	*d = 4;
	*e = 5;
	char* clave_a = "Vanukas";
	char* clave_b = "Pirtelo";
	char* clave_c = "Maito";
	char* clave_d = "Piima";
	char* clave_e = "Jugurtti";
	char* clave_f = "Kaakao";
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_a,a) == EXITO));
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_b,b) == EXITO));
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_c,c) == EXITO));
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_d,d) == EXITO));
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_e,e) == EXITO));
	test_check(stats,"Hash contiene un elemento",(hash_cantidad(hash) == 5));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_a)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_b)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_c)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_d)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_e)));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_a) == a));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_b) == b));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_c) == c));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_d) == d));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_e) == e));
	test_check(stats,"Hash NO obtiene elemento incorrecto",(hash_obtener(hash,clave_f) == NULL));
	test_check(stats,"Hash NO contiene elemento incorrecto",(!hash_contiene(hash,clave_f)));
	hash_destruir(hash);
}

void probar_insertar_elementos_repetidos_con_destructor(stats_t* stats){
	test_anunciar_prueba("Prueba varios elementos repetidos con destructor (free)");
	hash_t* hash = hash_crear(&free,3);
	int* a = malloc(sizeof(int));
	int* b = malloc(sizeof(int));
	int* c = malloc(sizeof(int));
	int* d = malloc(sizeof(int));
	int* e = malloc(sizeof(int));
	*a = 1;
	*b = 2;
	*c = 3;
	*d = 4;
	*e = 5;
	char* clave_a = "Vanukas";
	char* clave_b = "Pirtelo";
	char* clave_c = "Maito";
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_a,a) == EXITO));
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_b,b) == EXITO));
	test_check(stats,"Se inserta un elemento",(hash_insertar(hash,clave_c,c) == EXITO));
	test_check(stats,"Hash contiene tres elementos",(hash_cantidad(hash) == 3));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_a)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_b)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_c)));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_a) == a));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_b) == b));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_c) == c));
	test_check(stats,"Se inserta un elemento clave repetida",(hash_insertar(hash,clave_b,d) == EXITO));
	test_check(stats,"Se inserta un elemento clave repetida",(hash_insertar(hash,clave_c,e) == EXITO));
	test_check(stats,"Hash contiene tres elementos",(hash_cantidad(hash) == 3));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_a)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_b)));
	test_check(stats,"Hash contiene elemento correcto",(hash_contiene(hash,clave_c)));
	test_check(stats,"Hash sobrescribe elemento correcto",(hash_obtener(hash,clave_b) == d));
	test_check(stats,"Hash sobrescribe elemento correcto",(hash_obtener(hash,clave_c) == e));
	hash_destruir(hash);
}

/*||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
 *||													||
 *||				      FUNCIONES ESPECIFICAS DE BORRAR:					||
 *||													||
 *||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
*/
void probar_borrar_un_elemento_destructor_null(stats_t* stats){
	test_anunciar_prueba("Prueba un solo elemento, destructor NULL");
	hash_t* hash = hash_crear(NULL,3);
	int a = 1;
	char* clave_a = "Vanukas";
	hash_insertar(hash,clave_a,&a);
	test_check(stats,"Se borra elemento del hash",(hash_quitar(hash,clave_a) == EXITO));
	test_check(stats,"Hash esta vacio",(hash_cantidad(hash) == 0));
	test_check(stats,"Hash NO obtiene elemento borrado",(hash_obtener(hash,clave_a) == NULL));
	hash_destruir(hash);
}


void probar_borrar_elementos_destructor_null(stats_t* stats){
	test_anunciar_prueba("Prueba varios elementos, destructor NULL");
	hash_t* hash = hash_crear(NULL,3);
	int a = 1,b = 2,c = 3,d = 4, e = 5;
	char* clave_a = "Vanukas";
	char* clave_b = "Pirtelo";
	char* clave_c = "Maito";
	char* clave_d = "Piima";
	char* clave_e = "Jugurtti";
	hash_insertar(hash,clave_a,&a);
	hash_insertar(hash,clave_b,&b);
	hash_insertar(hash,clave_c,&c);
	hash_insertar(hash,clave_d,&d);
	hash_insertar(hash,clave_e,&e);
	test_check(stats,"Se borra elemento del hash",(hash_quitar(hash,clave_a) == EXITO));
	test_check(stats,"Se borra elemento del hash",(hash_quitar(hash,clave_b) == EXITO));
	test_check(stats,"Se borra elemento del hash",(hash_quitar(hash,clave_c) == EXITO));
	test_check(stats,"Hash NO obtiene elemento borrado",(hash_obtener(hash,clave_a) == NULL));
	test_check(stats,"Hash NO obtiene elemento borrado",(hash_obtener(hash,clave_b) == NULL));
	test_check(stats,"Hash NO obtiene elemento borrado",(hash_obtener(hash,clave_c) == NULL));
	test_check(stats,"No se borra elemento inexistente del hash",(hash_quitar(hash,clave_a) == ERROR));
	test_check(stats,"No se borra elemento inexistente del hash",(hash_quitar(hash,clave_b) == ERROR));
	test_check(stats,"No se borra elemento inexistente del hash",(hash_quitar(hash,clave_c) == ERROR));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_d) == &d));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_e) == &e));
	test_check(stats,"Hash contiene dos elementos",(hash_cantidad(hash) == 2));
	test_check(stats,"Se borra elemento del hash",(hash_quitar(hash,clave_d) == EXITO));
	test_check(stats,"Se borra elemento del hash",(hash_quitar(hash,clave_e) == EXITO));
	test_check(stats,"Hash NO obtiene elemento borrado",(hash_obtener(hash,clave_d) == NULL));
	test_check(stats,"Hash NO obtiene elemento borrado",(hash_obtener(hash,clave_e) == NULL));
	test_check(stats,"Hash esta vacio",(hash_cantidad(hash) == 0));
	hash_destruir(hash);
}

void probar_borrar_un_elemento_con_destructor(stats_t* stats){
	test_anunciar_prueba("Prueba un solo elemento con destructor (free)");
	hash_t* hash = hash_crear(&free,3);
	int* a = malloc(sizeof(int));
	*a = 1;
	char* clave_a = "Vanukas";
	hash_insertar(hash,clave_a,a);
	test_check(stats,"Se borra elemento del hash",(hash_quitar(hash,clave_a) == EXITO));
	test_check(stats,"Hash esta vacio",(hash_cantidad(hash) == 0));
	test_check(stats,"Hash NO obtiene elemento borrado",(hash_obtener(hash,clave_a) == NULL));
	test_check(stats,"Hash NO contiene elemento borrado",(!hash_contiene(hash,clave_a)));
	hash_destruir(hash);
}

void probar_borrar_elementos_con_destructor(stats_t* stats){
	test_anunciar_prueba("Prueba varios elementos con destructor (free)");
	hash_t* hash = hash_crear(&free,3);
	int* a = malloc(sizeof(int));
	int* b = malloc(sizeof(int));
	int* c = malloc(sizeof(int));
	int* d = malloc(sizeof(int));
	int* e = malloc(sizeof(int));
	*a = 1;
	*b = 2;
	*c = 3;
	*d = 4;
	*e = 5;
	char* clave_a = "Vanukas";
	char* clave_b = "Pirtelo";
	char* clave_c = "Maito";
	char* clave_d = "Piima";
	char* clave_e = "Jugurtti";
	hash_insertar(hash,clave_a,a);
	hash_insertar(hash,clave_b,b);
	hash_insertar(hash,clave_c,c);
	hash_insertar(hash,clave_d,d);
	hash_insertar(hash,clave_e,e);
	test_check(stats,"Se borra elemento del hash",(hash_quitar(hash,clave_a) == EXITO));
	test_check(stats,"Se borra elemento del hash",(hash_quitar(hash,clave_b) == EXITO));
	test_check(stats,"Se borra elemento del hash",(hash_quitar(hash,clave_c) == EXITO));
	test_check(stats,"Hash NO obtiene elemento borrado",(hash_obtener(hash,clave_a) == NULL));
	test_check(stats,"Hash NO obtiene elemento borrado",(hash_obtener(hash,clave_b) == NULL));
	test_check(stats,"Hash NO obtiene elemento borrado",(hash_obtener(hash,clave_c) == NULL));
	test_check(stats,"Hash NO contiene elemento borrado",(!hash_contiene(hash,clave_a)));
	test_check(stats,"Hash NO contiene elemento borrado",(!hash_contiene(hash,clave_b)));
	test_check(stats,"Hash NO contiene elemento borrado",(!hash_contiene(hash,clave_c)));
	test_check(stats,"No se borra elemento inexistente del hash",(hash_quitar(hash,clave_a) == ERROR));
	test_check(stats,"No se borra elemento inexistente del hash",(hash_quitar(hash,clave_b) == ERROR));
	test_check(stats,"No se borra elemento inexistente del hash",(hash_quitar(hash,clave_c) == ERROR));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_d) == d));
	test_check(stats,"Hash obtiene elemento correcto",(hash_obtener(hash,clave_e) == e));
	test_check(stats,"Hash contiene dos elementos",(hash_cantidad(hash) == 2));
	test_check(stats,"Se borra elemento del hash",(hash_quitar(hash,clave_d) == EXITO));
	test_check(stats,"Se borra elemento del hash",(hash_quitar(hash,clave_e) == EXITO));
	test_check(stats,"Hash NO obtiene elemento borrado",(hash_obtener(hash,clave_d) == NULL));
	test_check(stats,"Hash NO obtiene elemento borrado",(hash_obtener(hash,clave_e) == NULL));
	test_check(stats,"Hash NO contiene elemento borrado",(!hash_contiene(hash,clave_d)));
	test_check(stats,"Hash NO contiene elemento borrado",(!hash_contiene(hash,clave_e)));
	test_check(stats,"Hash esta vacio",(hash_cantidad(hash) == 0));
	hash_destruir(hash);
}

/*||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
 *||													||
 *||				   FUNCIONES ESPECIFICAS DE ITERADORES:					||
 *||													||
 *||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
*/

void probar_iterador_interno(stats_t* stats){
	test_anunciar_prueba("Prueba de obtener suma con iterador interno");	
	hash_t* hash = hash_crear(NULL,3);
	int a = 1,b = 2,c = 3,d = 4, e = 5, suma = 0;
	char* clave_a = "Vanukas";
	char* clave_b = "Pirtelo";
	char* clave_c = "Maito";
	char* clave_d = "Piima";
	char* clave_e = "Jugurtti";
	hash_insertar(hash,clave_a,&a);
	hash_insertar(hash,clave_b,&b);
	hash_insertar(hash,clave_c,&c);
	hash_insertar(hash,clave_d,&d);
	hash_insertar(hash,clave_e,&e);
	test_check(stats,"Iterador itera 5 veces",(hash_con_cada_clave(hash,&obtener_suma,&suma) == 5));
	test_check(stats,"Suma obtenida es correcta",(suma == 15));
	hash_destruir(hash);
}

void probar_iterador_externo_crear(stats_t* stats){
	test_anunciar_prueba("Prueba crear iterador externo");
	hash_iterador_t* it = hash_iterador_crear(NULL);
	test_check(stats,"No se crea iterador con hash NULL",(it == NULL));	
	hash_t* hash = hash_crear(NULL,3);
	it = hash_iterador_crear(hash);
	test_check(stats,"Se puede crear iterador",(it != NULL));

	hash_iterador_destruir(it);
	hash_destruir(hash);
}

void probar_iterador_externo_hash_vacio(stats_t* stats){
	test_anunciar_prueba("Prueba iterador externo con hash vacio");
	hash_t* hash = hash_crear(NULL,3);
	hash_iterador_t* it = hash_iterador_crear(hash);
	test_check(stats,"Iterador de hash vacio no tiene siguiente",(!hash_iterador_tiene_siguiente(it)));
	hash_iterador_destruir(it);
	hash_destruir(hash);
}

void probar_iterador_externo_cantidad_correcta_iteraciones(stats_t* stats){
	test_anunciar_prueba("Prueba de cantidad correcta de iteraciones iterador externo");
	hash_t* hash = hash_crear(NULL,3);
	int a = 1,b = 2,c = 3;
	char* clave_a = "Vanukas";
	char* clave_b = "Pirtelo";
	char* clave_c = "Maito";
	const char* aux;
	hash_insertar(hash,clave_a,&a);
	hash_insertar(hash,clave_b,&b);
	hash_insertar(hash,clave_c,&c);
	hash_iterador_t* it = hash_iterador_crear(hash);
	test_check(stats,"Iterador (0) de hash con 3 elementos tiene siguiente",(hash_iterador_tiene_siguiente(it)));
	aux = hash_iterador_siguiente(it);
	test_check(stats,"Iterador (1) de hash con 3 elementos tiene siguiente",(hash_iterador_tiene_siguiente(it)));
	aux = hash_iterador_siguiente(it);
	test_check(stats,"Iterador (2) de hash con 3 elementos tiene siguiente",(hash_iterador_tiene_siguiente(it)));
	aux = hash_iterador_siguiente(it);
	test_check(stats,"Iterador (3) de hash con 3 elementos NO tiene siguiente",(!hash_iterador_tiene_siguiente(it)));
	hash_iterador_destruir(it);
	hash_destruir(hash);
	funcion_que_no_hace_nada(aux);
}

void probar_iterador_externo_obtener_suma(stats_t* stats){
	test_anunciar_prueba("Prueba de obtener suma con iterador externo");
	hash_t* hash = hash_crear(NULL,3);
	int a = 1,b = 2,c = 3,suma = 0;
	char* clave_a = "Vanukas";
	char* clave_b = "Pirtelo";
	char* clave_c = "Maito";
	hash_insertar(hash,clave_a,&a);
	hash_insertar(hash,clave_b,&b);
	hash_insertar(hash,clave_c,&c);
	hash_iterador_t* it = hash_iterador_crear(hash);
	while(hash_iterador_tiene_siguiente(it)){
		const char* clave = hash_iterador_siguiente(it);
		if(clave)
			suma += *(int*)hash_obtener(hash,clave);
	}
	test_check(stats,"Se obtiene la suma correcta",(suma == 6));
	hash_iterador_destruir(it);
	hash_destruir(hash);
}

/*||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
 *||FUNCIONES PRINCIPALES DE PRUEBAS:									||
 *||Las funciones a continuacion ejecutaran una serie de pruebas sobre el TDA.				||
 *||Cada funcion principal se encarga de testear diferentes funcionalidades del TDA.			||
 *||Las funciones especificas estan aisladas y testean funcionalidades minimas y casos bordes.		||
 *||En cada funcion especifica hay diferentes mini-tests, cada uno con un nombre descriptivo		||
 *||PRE: Todas las funciones reciben un puntero a estadisticas, este debe ser valido/distinto de null	||
 *||POST: Modificara las estadisticas, agregando la cantidad de pruebas ejecutadas y exitosas		||
 *||AVISO: Las pruebas estan ordenadas jerarquicamente, es decir, si el TDA no pasa algun test, 	||
 *||creara un grado de incertidumbre sobre las pruebas que estan debajo ya que estas utilizaran,	||
 *||en general, las funciones testeadas anteriormente.							||
 *||=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||
*/
void probar_crear(stats_t* stats){
	test_anunciar_prueba("Prueba con destructor NULL y distintas capacidades");
	probar_crear_capacidad(stats,1,NULL);
	probar_crear_capacidad(stats,4,NULL);
	probar_crear_capacidad(stats,11,NULL);
	test_anunciar_prueba("Prueba con destructor (free) y distintas capacidades");
	probar_crear_capacidad(stats,1,&free);
	probar_crear_capacidad(stats,4,&free);
	probar_crear_capacidad(stats,11,&free);
}

void probar_insertar(stats_t* stats){
	probar_insertar_un_elemento_destructor_null(stats);
	probar_insertar_elementos_destructor_null(stats);
	probar_insertar_elementos_repetidos_destructor_null(stats);
	probar_insertar_un_elemento_con_destructor(stats);
	probar_insertar_elementos_con_destructor(stats);
	probar_insertar_elementos_repetidos_con_destructor(stats);
}

void probar_borrar(stats_t* stats){
	probar_borrar_un_elemento_destructor_null(stats);
	probar_borrar_elementos_destructor_null(stats);
	probar_borrar_un_elemento_con_destructor(stats);
	probar_borrar_elementos_con_destructor(stats);
}

void probar_iteradores(stats_t* stats){
	probar_iterador_interno(stats);
	probar_iterador_externo_crear(stats);
	probar_iterador_externo_hash_vacio(stats);
	probar_iterador_externo_cantidad_correcta_iteraciones(stats);
	probar_iterador_externo_obtener_suma(stats);
}

int main(){
	stats_t* stats = test_crear_stats();
	if(!stats) return ERROR;
	test_anunciar("Iniciando pruebas de creacion");
	probar_crear(stats);
	test_anunciar("Iniciando pruebas de insertar");
	probar_insertar(stats);
	test_anunciar("Iniciando pruebas de borrar");
	probar_borrar(stats);
	test_anunciar("Iniciando pruebas de iteradores");
	probar_iteradores(stats);
	test_anunciar("Pruebas finalizadas");
	test_imprimir_stats(stats);
	test_destruir_stats(stats);
	return 0;
}
