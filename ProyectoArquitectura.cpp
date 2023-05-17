#include <iostream>
#include <cmath>
#include <bitset>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <random>



using namespace std;

class Cache{

    private:
     
    int  esValido,etiqueta,datos,lru,lfu;

    public:
    Cache(){
        this->esValido = 0;
        this->etiqueta = 0;
        this->lru = 0;
        this->datos = 0x0000000;
        this->lfu = 0;
    }

    Cache(int esValido, int etiqueta, int datos, int lru, int lfu){
        this->esValido = esValido;
        this->etiqueta = etiqueta;
        this->datos = datos;
        this->lru = lru;
        this->lfu= lfu;

    }
    //Getters y setters
    int getEsValido(){
        return esValido;
    }
    int getEtiqueta(){
        return etiqueta;
    }
    int getDatos(){
        return datos;
    }
    int getLRU(){
        return lru;
    }
    int getLFU(){
        return lfu;
    }    
    void setEsValido(int esValido){
        this->esValido = esValido;
    }
    void setEtiqueta(int etiqueta){
        this->etiqueta = etiqueta;
    }
    void setDatos(int datos){
        this->datos = datos;
    }
    void setLRU(int lru){
        this->lru = lru;
    }
    void setLFU(int lfu){
        this->lfu = lfu;
    }    
};

//Utilizado para generar un espacio suficiente para direcciones de 32 bits
string espaciar(int tamanio, int valor){
    int espacio = 0,i;
    string texto = "";

    espacio = valor  - tamanio;

    for(i=0;i<espacio;i++){
        texto = texto + " ";
    }
    return texto;
}


void generate_random_direcciones(uint32_t minimo, uint32_t maximo, double probabilidadTemporal, double probabilidadEspacial) {
    random_device aleatorio;
    mt19937 generador(aleatorio());
    uniform_int_distribution<uint32_t> rango(minimo, maximo);//Genera valores aleatorios de forma uniforme entre el minimo y el maximo
    ofstream archivo("casoAux.in");
    int i,aux;
    vector<uint32_t> direcciones;
    uint32_t direccionAntigua = 0,direccion;
    long address;
    for ( i = 0; i < 5000; i++) {
         direccion = rango(generador);
        if (i > 0) {
            if (probabilidadTemporal >= generate_canonical<double, numeric_limits<double>::digits>(generador)) {
                // Repetir un valor utilizado con anterioridad, esto para generar casos donde se aproveche la localidad temporal
                 aux = uniform_int_distribution<int>(0, i - 1)(generador);
                direccion = direcciones[aux];
            } else if (probabilidadEspacial >= generate_canonical<double, numeric_limits<double>::digits>(generador) ) {
                // Generar un valor adyacente al valor anterior donde la direccion sera un salto de 20, prediciendo con la localidad espacial
                direccion = direccionAntigua + 20;
            }
        }
        direcciones.push_back(direccion);
        direccionAntigua = direccion;
    }
    //Se crea el archivo casoAux.in
    while(direcciones.size()!=0){    

        archivo << "0x" << hex << direcciones.front() << endl;
        direcciones.erase(direcciones.begin());
    }
    archivo.close();
}


int main(){
    int capacidadCache, numPalabras,numBloques, desplazamiento, bloque, etiqueta, conjunto,contadorConjunto=0,creacionArchivo;
    int bitsDesplazamiento, bitsBloque, bitsEtiqueta,j,k,i,maximo=-1,ultimo=9999,posUltimoAccedido,tipo,contadorAciertos=0,remplazamiento;
    int bitsBloqueBuffer=-1,bitsEtiquetaBuffer=-1,bitsDesplazamientoBuffer=-1,prefecSelec;//se inicializan a -1, para evitar que entren la primera vez
    long direccion,prefetchingSalto;
    float frecuenciaNumerador,frecuenciaDenominador;
    string nombreArchivo;
    bitset<32> aux;
    bool bandera,banderaBuffer;
    ofstream salida("salida.out");
    ofstream csv("archivo.csv");
    Cache **memCache;
    srand(time(NULL));
    




        csv<<"FrecuenciaDeaciertos,NumeroDeAccesos\n";

        direccion=32;   //Cantidad de bits que tiene la direccion

        //Entradas y validacion
        cout<<"¿Desea generar un archivo aleatorio de direcciones?\n";
        cout<<"Ingrese 1 para generarlo, caso contrario ingrese cualquier otro numero\n";
        if(!(cin>>creacionArchivo)){
            cout<<"Ha ingresado un valor incorrecto, el valor a ingresar debe ser un numero. A continuacion el programa se cerrara.\n";
            return 0;
        }
        if(creacionArchivo==1){
                        //como parametros de entrada tiene la cantidad permitida de direcciones, asi como tambien el porcentaje respectivo a la localidad temporal y espacial
                        generate_random_direcciones(0, 0xFFFFFFFF, 0.6, 0.6);
        }

        //Seleccion archivo
        cout << "Seleccione un archivo de entrada:\n"<<"1. casoAlto.in\n"<<"2. casoMedio.in\n"<<"3. casoBajo.in\n"<<"4. casoAux.in\n";
        if(!(cin>>tipo)){
            cout<<"Ha ingresado un valor incorrecto, el valor a ingresar debe ser un numero. A continuacion el programa se cerrara.\n";
            return 0;
        }

        switch (tipo) {
            case 1:
                nombreArchivo = "casoAlto.in";
            break;
            case 2:
                nombreArchivo = "casoMedio.in";
            break;
            case 3:
                nombreArchivo = "casoBajo.in";
            break;
            case 4:
                nombreArchivo ="casoAux.in";
            break;
            default:
                cout<<"Debe ingresar (1) o (2) o (3) o (4 ) para escoger el archivo a utilizar, el programa se cerrara.\n";
                return 0;
            break;
        }
        //Abre el archivo a utilizar
        ifstream entrada(nombreArchivo);

        entrada.seekg(0, ios::end);
        if (entrada.tellg() == 0) {
            cout << "El archivo está vacío. El programa se cerrara\n";
            return 0;
        }
        entrada.seekg(0, ios::beg);


        cout<<"Ingrese la capacidad de la memoria cache en bytes\n";
        //Memoria cache recibida en bytes
        if(!(cin>>capacidadCache)){
            cout<<"Ha ingresado un valor incorrecto, el valor a ingresar debe ser un numero. A continuacion el programa se cerrara.\n";
            return 0;
        }

        cout<<"Ingrese cual tipo de remplazamiento desea utilizar\n"<<"1. LFU\n"<<"2. LRU\n"<<"3. Remplazo aleatorio\n";
        if(!(cin>>remplazamiento)){
            cout<<"Ha ingresado un valor incorrecto, el valor a ingresar debe ser un numero. A continuacion el programa se cerrara.\n";
            return 0;
        }
        cout<<"Ingrese que tipo de correspondencia desea\n"<<"1. Correspondencia directa\n"<<"2. Asociativa por conjuntos\n"<<"3. Completamente asociativa\n";

        if(!(cin>>tipo)){
            cout<<"Ha ingresado un valor incorrecto, el valor a ingresar debe ser un numero. A continuacion el programa se cerrara.\n";
            return 0;
        }
        cout<<"Ingrese cuantas palabras tendra cada bloque\n";

        if(!(cin>>numPalabras)){
            cout<<"Ha ingresado un valor incorrecto, el valor a ingresar debe ser un numero. A continuacion el programa se cerrara.\n";
            return 0;
        }

        
        if(capacidadCache < (numPalabras*4)){
            cout<<"La capacidad de la memoria cache es menor que el total de bytes en el numero de bloques\n";
            cout<<"El programa se cerrara.\n";
            return 0;
        }


        switch (tipo){
            case 1:
                conjunto = 1;//Al seleccionar correspondencia directa, es un conjunto de 1 via
                numBloques=capacidadCache/((numPalabras*4)*conjunto);//El total de bloques que tendra la cache
            break;
        
            case 2:
                cout<<"Ingrese cuantas vias tendra el conjunto\n";
                if(!(cin>>conjunto)){
                    cout<<"Ha ingresado un valor incorrecto, el valor a ingresar debe ser un numero. A continuacion el programa se cerrara.\n";
                    return 0;
                }

                numBloques=capacidadCache/((numPalabras*4)*conjunto);//El total de bloques que tendra la cache

                if(conjunto > numBloques) {
                    cout << "La cantidad de vías del conjunto es mayor al número de bloques. El programa se cerrará.\n";
                    return 0;
                }
            break;
            case 3:
                numBloques=1;
                conjunto=capacidadCache/(4*numPalabras);//El total de vias 
            break;   
            default:
                cout<<"Debe ingresar (1) o (2) o (3) en el tipo de correspondencia, el programa se cerrara.\n";
                return 0;
            break;
        }

        if((capacidadCache % 2 != 0) || (numBloques % 2 != 0 && numBloques > 1)){
            cout<<"La capacidad de la memoria cache y el total de bloques debe ser potencia de 2\n";
            cout<<"El programa se cerrara.\n";
            return 0;
        }

        cout<<"Ingrese 1 si desea utilizar prefetching, caso contrario ingrese cualquier otro numero\n";
        cin>>prefecSelec;

        // Asignar memoria para el numero total de conjuntos
        memCache = new Cache*[numBloques];

        // Asignar memoria para el total de vias
        for ( i = 0; i < numBloques; i++) {
        memCache[i] = new Cache[conjunto];
        }


        //Se calcula el numero total de bits para el espacio del desplazamiento mediante el log2(tamaño del bloque en bytes)
        desplazamiento=log(4*numPalabras)/log(2);   

        //Se calcula el numero total de bits para el espacio del bloque mediante el log2(número total de bloques en la cache) 
        bloque = log(numBloques) / log(2);  

        etiqueta=direccion-bloque-desplazamiento;    //Se calcula el numero total de bits para el espacio de la etiqueta

        cout<<"El tamaño de la direccion es: "<<direccion<<" bits"<<endl;
        cout<<"El total de conjuntos de la cache es: "<<numBloques<<endl;
        cout<<"El total de vias del conjunto es: "<<conjunto<<endl;
        cout<<"El total de bits para la etiqueta es: "<<etiqueta<<endl;
        cout<<"El total de bits para el bloque es: "<<bloque<<endl;   
        cout<<"El total de bits para el desplazamiento es: "<<desplazamiento<<endl;


        i=0;//Contador de accesos
        while(entrada>> hex >> direccion){
            //prefetching de salto Se toma en consideracion que el programa aprovechara saltos de 1 palabra para acceder a arreglos/matrices
            prefetchingSalto =  20 + direccion;//aprovechando asi la localidad espacial
            i++;
            salida<< dec << i<<endl;
            salida<<"Direccion leida 0x"<< hex << direccion<<endl;
            bandera=false;
            banderaBuffer=false;
    
            //Se separa el bit recibido en la direccion, obteniendo los valores correspondientes a desplazamiento,bloque y etiqueta (cache)
            bitsDesplazamiento= direccion & ((1 << desplazamiento) - 1);  

            bitsBloque = direccion >> desplazamiento;  
            bitsBloque = bitsBloque & ((1 << bloque) - 1);

            bitsEtiqueta = direccion >> (bloque+desplazamiento); 

            do{
                //Verifica el estado del bit de validez
                if(memCache[bitsBloque][contadorConjunto].getEsValido()==1){
                    //Se verifica si la etiqueta concuealeatorioa
                    if(bitsEtiqueta==memCache[bitsBloque][contadorConjunto].getEtiqueta()){
                        memCache[bitsBloque][contadorConjunto].setDatos(direccion);//Se muestra en datos la direccion actual
                        bandera=true;//Como la etiqueta fue igual, a la buscada es un acierto
                        //Le suma 1 al LRU, por que acaba ingresar en el
                        memCache[bitsBloque][contadorConjunto].setLRU(memCache[bitsBloque][contadorConjunto].getLRU()+1);
                        //Se le suma 1 al LFU, por que acaba de entrar a el
                        memCache[bitsBloque][contadorConjunto].setLFU(memCache[bitsBloque][contadorConjunto].getLFU()+1);                        
                    }
                }
                //Aumenta el contador, para ver si la informacion solicitada esta en otro bloque del conjunto
                contadorConjunto++;

            }while(contadorConjunto<conjunto);
            contadorConjunto=0;

            //Si era falso, pero la direccion que se trata de acceder ya esta en el buffer, entonces sera true
            if (!bandera && (bitsBloqueBuffer==bitsBloque)&&(bitsEtiquetaBuffer==bitsEtiqueta) && (prefecSelec==1)){
                banderaBuffer = true;
                //No obstante, debera entrar a insertar el nuevo dato del buffer a la memoria cache, es por ello que la variable "bandera" sigue en falso
                
            }   

            //Caso donde se utiliza LFU
            if( (!bandera) && (remplazamiento==1)){//Hubo un fallo asique hay que agregar el nuevo dato
                do{
                    //LFU, se busca cual fue el menos reciente usado
                    if(ultimo>memCache[bitsBloque][contadorConjunto].getLFU()){
                        ultimo = memCache[bitsBloque][contadorConjunto].getLFU();
                        posUltimoAccedido = contadorConjunto;
                    }

                
                    contadorConjunto++;
                }while(contadorConjunto<conjunto);
                //reseteo
                ultimo=9999;
                contadorConjunto=0;

                //Se almacena en el ultimo dato accedido
                memCache[bitsBloque][posUltimoAccedido].setEsValido(1);
                memCache[bitsBloque][posUltimoAccedido].setEtiqueta(bitsEtiqueta);
                memCache[bitsBloque][posUltimoAccedido].setDatos(direccion);        
                //Se le suma 1 a su LFU, por que se acaba de ingresar en el

                memCache[bitsBloque][posUltimoAccedido].setLFU(memCache[bitsBloque][posUltimoAccedido].getLFU()+1);        
            
            }
        
            if( (!bandera) && (remplazamiento==2)){//Hubo un fallo asique hay que agregar el nuevo dato
                do{
                    //LRU, se busca cual es el menos usado
                    if(ultimo>memCache[bitsBloque][contadorConjunto].getLRU()){
                        ultimo = memCache[bitsBloque][contadorConjunto].getLRU();
                        posUltimoAccedido = contadorConjunto;
                    }

                
                    contadorConjunto++;
                }while(contadorConjunto<conjunto);
                //reseteo
                ultimo=9999;
                contadorConjunto=0;

                //Se almacena en el ultimo dato accedido
                memCache[bitsBloque][posUltimoAccedido].setEsValido(1);
                memCache[bitsBloque][posUltimoAccedido].setEtiqueta(bitsEtiqueta);
                memCache[bitsBloque][posUltimoAccedido].setDatos(direccion);        

                
                do{//Busca el valor mas alto para asi asignar el futuro LRU con el "valor mas alto + 1"
                    if(maximo<memCache[bitsBloque][contadorConjunto].getLRU()){
                        maximo = memCache[bitsBloque][contadorConjunto].getLRU();
                        posUltimoAccedido = contadorConjunto;
                    }

                
                    contadorConjunto++;
                }while(contadorConjunto<conjunto);
                contadorConjunto=0;

                //Se le suma 1 al LRU total, por que se acaba de ingresar en uno nuevo
                memCache[bitsBloque][posUltimoAccedido].setLRU(maximo+1);        
                maximo=-1;

            }

            if( (!bandera) && (remplazamiento==3)){//Hubo un fallo asique hay que agregar el nuevo dato
                posUltimoAccedido=-1;
                do{
                    //random, verifica primero si hay sitios libres antes de usar random
                    if(memCache[bitsBloque][contadorConjunto].getDatos()==0){
                        posUltimoAccedido = contadorConjunto;
                    }

                
                    contadorConjunto++;
                }while(contadorConjunto<conjunto);
                //reseteo
                contadorConjunto=0;

                //Se obtiene el random si ya estan todas las vias ocupadas
                if (posUltimoAccedido==-1){
                    posUltimoAccedido = rand() % conjunto;
                }
                //Se almacena en el ultimo dato accedido
                memCache[bitsBloque][posUltimoAccedido].setEsValido(1);
                memCache[bitsBloque][posUltimoAccedido].setEtiqueta(bitsEtiqueta);
                memCache[bitsBloque][posUltimoAccedido].setDatos(direccion);        


            }
            //Procede a imprimir todo

            if(bandera || banderaBuffer){
                contadorAciertos++;
                salida<<"Acierto"<<endl;
            }
            else{
                salida<<"Fallo"<<endl;
            }

            contadorConjunto=0;
            if(bloque!=0){//Si no es completamente asociativa imprime "Indice"
                 salida<<"Índice"<<espaciar(6,10);
            }
            for(j=0;j<conjunto;j++){
            salida<<"V"<<espaciar(1,5)<<"Etiqueta"<<espaciar(8,32)<<"Datos"<<espaciar(5,40);
            }
            salida<<endl;
            
            for(j=0;j<numBloques;j++){
                aux=(j);
                //Indice
                if(bloque!=0){//Si no es completamente asociativa imprime el valor del indice
                    salida<<aux.to_string().substr(32 - bloque)<<espaciar(bloque,10);
                }
                for(k=conjunto-1;k>=0;k--){
                    //bit validez
                    salida<<memCache[j][k].getEsValido();
                    //Imprime la etiqueta y los datos
                    aux=memCache[j][k].getEtiqueta();
                    salida<<espaciar(1,5)<<aux.to_string().substr(32 - etiqueta);
                    aux=memCache[j][k].getDatos();
                    salida<<espaciar(etiqueta,32)<<aux<<espaciar(32,40);
                }
                salida<<"\n";
            }
            salida<<"\n";
            //Finalmente se prepara el prefetching de buffer para la siguiente llamada, almacenandole asi cada uno de sus respectivos datos de la direccion pronosticada
            bitsDesplazamientoBuffer= prefetchingSalto & ((1 << desplazamiento) - 1);  

            bitsBloqueBuffer = prefetchingSalto >> desplazamiento;  
            bitsBloqueBuffer = bitsBloqueBuffer & ((1 << bloque) - 1);

            bitsEtiquetaBuffer = prefetchingSalto >> (bloque+desplazamiento); 


            //Imprime para generar .csv
            frecuenciaNumerador = (float)(contadorAciertos);
            frecuenciaDenominador = (float)(i);
            csv.precision(2);

            csv<<(frecuenciaNumerador/ frecuenciaDenominador)*100<<",";
            csv<<i<<endl;
        }
        

        salida.precision(2);
        salida<<"Frecuencia de aciertos: "<<(frecuenciaNumerador/ frecuenciaDenominador)*100<<"%"<<endl;
        salida<<"Frecuencia de fallos: "<<(1-(frecuenciaNumerador/frecuenciaDenominador))*100<<"%"<<endl;

        cout<<"\nFrecuencia de aciertos: "<<(frecuenciaNumerador/ frecuenciaDenominador)*100<<"%"<<endl;
        cout<<"Frecuencia de fallos: "<<(1-(frecuenciaNumerador/frecuenciaDenominador))*100<<"%"<<endl;
        cout<<"\nEl archivo de salida se ha creado con exito."<<endl;





    
    //V30.077.008 - V28.505.513
    //José López  - Edgar Gutiérrez
    return 0;
}