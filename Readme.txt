Matias Ignacio Gonzalez
TDA HASH

Se presentan:
    >> Implementacion de TDA Hash
    >> Programa de testeo para el TDA Hash presentado
    >> Biblioteca de testeo

Implementacion de TDA Hash - Descripcion:
	El Hash se implementa como hash abierto. En cada posicion de la tabla se crea una lista que contiene los elementos y claves insertados,
tambien almacena las colisiones.
	El TDA incluye dos tipos de iteradores: interno y externo.

Programa de testeo para el TDA Hash:
    En el archivo .zip tambien se incluye un programa para testear la implementacion del TDA. Al compilar y ejecutar el programa se ejecutaran una serie de pruebas
sobre el TDA, testeando cada funcion por separado y testeando el funcionamiento en casos bordes.
    Los resultados de las pruebas seran impresos por pantalla con un formato vistoso y facil de leer.
    Codigo de colores:
        Verde = Test exitoso
        Rojo = Test fallido
        Amarillo = Advertencia

POSIBLES FALLAS DEL TESTEO:
Si el usuario no tiene permisos para asignar memoria en el heap el programa se comportara de forma indeterminada.
Si la memoria disponible no es suficiente el programa se comportara de forma indeterminada.

Compilacion - Ejecucion Testeo:
    Para compilar el programa de testeo debe escribirse en la terminal:
           gcc *.c -o hash

Uso como biblioteca:
    Incluir al comienzo del programa:
        #include "hash.h"
