# Simulador Memoria Caché
# Integrantes

 José López C.I.: 30.077.008 - [Github](https://github.com/JoseLopez3)

 Edgar Gutiérrez C.I.: 28.505.513 - [Github](https://github.com/zayans13)

# ¿Qué es la memoria caché?

La memoria caché es una memoria de acceso rápido que se utiliza para almacenar temporalmente los datos que se utilizan con más frecuencia en un procesador.
La memoria caché se divide en tres tipos de correspondencia: directa, asociativa y asociativa por conjuntos.


La correspondencia directa es la técnica más simple y consiste en hacer corresponder cada bloque de memoria principal a solo una línea posible de caché.
La correspondencia asociativa por conjuntos permite que cada bloque de memoria principal pueda cargarse en cualquier línea de la caché.
Por último, la correspondencia asociativa es la técnica más compleja y permite que cada bloque de memoria principal pueda cargarse en cualquier línea de la caché.


En este proyecto se busca simular su funcionamiento, mediante la introducción de datos como: (direcciones de la memoria principal, capacidad total de la caché reflejada en bytes, tipo de correspondencia, así como también el número de palabras por bloque). Toda esta información será utilizada para generar un archivo de salida el cual mostrará el desempeño de la memoria caché según los criterios elegidos.

# Ejecución del programa

Para ejecutar el programa, puede escribir los siguientes comandos:

Para compilar puede hacer uso de
``` 
make 
```
o 
```
g++ -std=c++11 ProyectoArquitectura.cpp -o cache.out
```
Y para ejecutar
```
./cache.out
```

Una vez ejecutado el programa, se le pedirá ingresar 1 o cualquier otro número según la opción que desee tomar para las primeras 2 preguntas, las cuales hacen referencia para generar un archivo genérico de lectura de direcciones. Así como también, la capacidad de imprimir un archivo salida.out que tendrá la representación gráfica de la memoria caché. Tras haber ingresado con éxito estos datos, se refleja un menú al cual el usuario podrá seleccionar que archivo desea utilizar. Una vez seleccionado se le pedirá cuanta capacidad de almacenamiento tendrá la memoria caché. Lo cual dará lugar nuevamente a un menú con 3 opciones, las cuales serán el tipo de correspondencia que se desea utilizar, si la opción elegida fue asociativa por conjuntos el programa le solicitará el número de palabras por bloque y ¿cuantas vías tendrá el conjunto? . Caso contrario, solo le pedirá
el número de palabras por bloque. Finalmente, el programa le dará la posibilidad de usar el prefetching en la memoria caché si lo desea. Después de haber ingresado todos los datos podrá ver por la terminal las características de la memoria caché, así como también la representación gráfica de la memoria caché en el archivo *salida.out* siempre y cuando se le haya solicitado al programa crear dicho archivo.

# Información a tener en cuenta

- El programa solo trabaja con direcciones de 32 bits.  
- Las palabras solo serán de 4 bytes.  
- Los datos a ingresar, como lo es la capacidad total de la caché, su tipo de correspondencia, al igual que el número de palabras por bloques, serán ingresadas por teclado.

