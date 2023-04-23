# Simulador Memoria Cache
# Integrantes

> José López C.I.: 30.077.008 - [Github](https://github.com/JoseLopez3)

> Edgar Gutiérrez C.I.: 28.505.513 - [Github](https://github.com/zayans13)

# ¿Que es la memoria cache?

La memoria caché es una memoria de acceso rápido que se utiliza para almacenar temporalmente los datos que se utilizan con más frecuencia en un procesador.  
La memoria caché se divide en tres tipos de correspondencia: directa, asociativa y asociativa por conjuntos.


La correspondencia directa es la técnica más simple y consiste en hacer corresponder cada bloque de memoria principal a solo una línea posible de caché.   
La correspondencia asociativa por conjuntos permite que cada bloque de memoria principal pueda cargarse en cualquier línea de la caché.   
Por último, la correspondencia asociativa es la técnica más compleja y permite que cada bloque de memoria principal pueda cargarse en cualquier línea de la caché.

En este proyecto se busca simular su funcionamiento, mediante la introduccion de datos como: (direcciones de la memoria principal, capacidad total de la cache reflejada en bytes, tipo de correspondencia,
asi como tambien numero de palabras por bloque). Toda esta informacion sera utilizada para generar un archivo de salida el cual mostrara el desempeño de la memoria cache segun los criterios elegidos.

# Ejecucion del programa

Para ejecutar el programa, primero se debera escribir los siguientes comandos:

```  
make 
./cache.out
```

Una vez ejecutado el programa, se le pedira ingresar cuanta capacidad de almacenamiento tiene la memoria cache. Lo cual dara lugar a un menu con 3 opciones, las cuales seran el tipo de correspondencia
que se desea utilizar, si la opcion elegida fue asociativa por conjuntos el programa le solicitara el numero de palabras por bloque y ¿cuantas vias tendra el conjunto? . Caso contrario, solo le pedira
el numero de palabras por bloque. Tras haber ingresado todos los datos podra ver por la terminal las caracteristicas de la memoria cache, asi como tambien la representacion grafica de la memoria cache en el archivo *salida.out*

# Informacion a tener en cuenta

- El programa solo trabaja con direcciones de 32 bits  
- Las palabras solo seran de 4 bytes  
- Los direcciones que se desean ingresar al simulador, deberan ser colocadas en el archivo *"entrada.in"*, dichas direcciones deberan ser escritas de manera hexadecimal.  
- El resto de datos a ingresar, como lo es la capacidad total de la cache, su tipo de correspondencia, al igual que el numero de palabras por bloques, seran ingresadas por teclado

