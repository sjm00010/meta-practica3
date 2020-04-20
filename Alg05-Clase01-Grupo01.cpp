/* 
 * File:   Memetico.cpp
 * Author: sjm00010
 * 
 * Created on 18 de noviembre de 2019, 16:18
 */

#include "Alg05-Clase01-Grupo01.h"
#include "Parametros.h"
#include "FuncionesComunes.h"
#include "random.h"

#include <limits>
#include <iostream>

using namespace std;

Individuo Memetico::memetico(string rutaLog, vector<vector<int>>& flu, 
        vector<vector<int>>& dis, bool sim){
    
    // Variables
    vector<int> padre;
    vector<int> madre;
    
    // Inicio la generacion
    int generacion = 0;
    mejor.setCoste(numeric_limits<int>::max());

    // Redimensiono el vector
    poblacion.resize(stoi(parametros[TAM_POBLACION]));

    for(int i = 0; i < poblacion.size(); i++){
        vector<int> sol = creaSolucion(flu.size());
        int coste = calculaCoste(sol ,flu,dis,sim);
        poblacion[i].creaIndividuo(sol , coste, generacion);
    }

    // Registro en el log
    if(parametros[LOG] == "SI"){
        creaLog(rutaLog);
        registraLogGeneracion(rutaLog, generacion);
        registraLogPoblacion(rutaLog, poblacion);
    }
    
    generacion++;

    while(generacion < stoi(parametros[MAX_GENERACIONES])){
        
        // Selecciono los individuos mediante torneo
        seleccionados.first = torneo();
        int contador = 0;
        do{
            seleccionados.second = torneo();
            contador++;
        }while(seleccionados.first == seleccionados.second || 
                poblacion[seleccionados.first].GetSolucion() == poblacion[seleccionados.second].GetSolucion()
                && contador < poblacion.size());
        
        // Registro en el log los individuos seleccionados
        if(parametros[LOG] == "SI"){
            if(generacion < 30 || generacion % 10 == 0){
                registraLogGeneracion(rutaLog, generacion);
                registraCadena(rutaLog, "Individuos seleccionados");
                registraLogIndividuos(rutaLog,poblacion,seleccionados);
            }
        }
        
        // Cruzo los individuos seleccionados
        if(parametros[TIPO_CRUCE] == "OX2"){
            if(Randfloat(0,1) <= stof(parametros[PROB_CRUCE])){
                vector<int> marcados;
                padre = poblacion[seleccionados.first].GetSolucion();
                for(int j= 0; j < padre.size(); j++){
                    if(Randfloat(0,flu.size()) < stoi(parametros[PROB_OX2])){
                        marcados.push_back(padre[j]);
                    }
                }

                padre = OX2(poblacion[seleccionados.first].GetSolucion(), 
                        poblacion[seleccionados.second].GetSolucion(), marcados);
                madre = OX2(poblacion[seleccionados.second].GetSolucion(), 
                        poblacion[seleccionados.first].GetSolucion(), marcados);
            }    
        }else if(parametros[TIPO_CRUCE] == "MOC"){
                if(Randfloat(0,1) <= stof(parametros[PROB_CRUCE])){
                    padre = poblacion[seleccionados.first].GetSolucion();
                    int marca = Randint(1,padre.size()-2);
                    
                    padre = MOC(poblacion[seleccionados.first].GetSolucion(), 
                            poblacion[seleccionados.second].GetSolucion(), marca);
                    madre = MOC(poblacion[seleccionados.second].GetSolucion(), 
                            poblacion[seleccionados.first].GetSolucion(), marca);
                }
        }else{
            cerr << "Error en la seleccion del operador de cruce" << endl;
        }
        
        // Registro en el log las soluciones
        if(parametros[LOG] == "SI"){
            if(generacion < 30 || generacion % 10 == 0){ 
                registraCadena(rutaLog, "Individuos despues del cruce");
                registraLogVector(rutaLog,padre, to_string(seleccionados.first+1));
                registraLogVector(rutaLog,madre, to_string(seleccionados.second+1));
            }
        }

        
        // Muto las nuevas soluciones
        for(int j = 0; j < padre.size(); j++){
            // Para el primer Individuo 
            if(Randfloat(0,1) < stof(parametros[PROB_MUTA])){
                // Mutación
                padre = mutacion(padre, j);
            }
            
            // Para la segundo Individuo
            if(Randfloat(0,1) < stof(parametros[PROB_MUTA])){
                // Mutación
                madre = mutacion(madre, j);
            }
        }
        
        // Registro en el log las nuevas soluciones
        if(parametros[LOG] == "SI"){
            if(generacion < 30 || generacion % 10 == 0){
                // Registro en el log la poblacion
                registraCadena(rutaLog, "Individuos despues de la mutacion");
                registraLogVector(rutaLog,padre, to_string(seleccionados.first+1));
                registraLogVector(rutaLog,madre, to_string(seleccionados.second+1));
            }
        }
        
        // Calculo los costes de los nuevos genes
        int costeP = calculaCoste(padre, flu, dis, sim);
        int costeM = calculaCoste(madre, flu, dis, sim);

        // Se aplica la Tabu
        if(generacion % stoi(parametros[GEN_TABU]) == 0){
            
            // Registro en el log las nuevas soluciones
            if(parametros[LOG] == "SI"){
                if(generacion < 30 || generacion % 10 == 0){
                    // Registro en el log la poblacion
                    registraCadena(rutaLog, "Individuos antes de la Tabu");
                    registraLogTabu(rutaLog, padre, costeP, to_string(seleccionados.first+1));
                    registraLogTabu(rutaLog, madre, costeM, to_string(seleccionados.second+1));
                }
            }
            
            // Tabú para el padre
            padre = tabu(rutaLog, to_string(seleccionados.first+1), generacion, padre, costeP, flu, dis, sim);
            // Tabú para la madre
            madre = tabu(rutaLog, to_string(seleccionados.second+1), generacion, madre, costeM, flu, dis, sim);
            
            // Registro en el log las nuevas soluciones
            if(parametros[LOG] == "SI"){
                if(generacion < 30 || generacion % 10 == 0){
                    // Registro en el log la poblacion
                    registraCadena(rutaLog, "Individuos despues de la Tabu");
                    registraLogTabu(rutaLog, padre, costeP, to_string(seleccionados.first+1));
                    registraLogTabu(rutaLog, madre, costeM, to_string(seleccionados.second+1));
                }
            }
        }
        
        
        // Sustituyo las nuevas soluciones
        buscaPeores();
        poblacion[peores.first].SetSolucion(padre,generacion);
        poblacion[peores.second].SetSolucion(madre,generacion);
        
        // Asigno los costes de los nuevos genes
        poblacion[peores.first].setCoste(costeP);
        poblacion[peores.second].setCoste(costeM);
        
        // Registro en el log los individuos seleccionados
        if(parametros[LOG] == "SI"){
            if(generacion < 30 || generacion % 10 == 0){
                string t = "Se sustituyen los individuos por "+ to_string(peores.first+1) +
                        " y " + to_string(peores.second+1);
                registraCadena(rutaLog, t);
                registraCadena(rutaLog, "Individuos finales");
                registraLogIndividuos(rutaLog,poblacion,peores);
            }
        }
        
        // Finalmente cambio la generación
        generacion++;
        
        // Busco el mejor de la generación
        buscaMejor();
    }
    
    if(parametros[LOG] == "SI"){
        // Registro en el log la poblacion final
        registraLogGeneracion(rutaLog, generacion);
        registraCadena(rutaLog, "Poblacion de final");
        registraLogPoblacion(rutaLog,poblacion);
    }
    

            
    return mejor;
}

// Funciones locales complementarias

/**
 * Funcion que busca el mejor de la poblacion, devolviendolo y borrandolo 
 * del vector
 * @return Una copia del mejor individuo
 */
void Memetico::buscaMejor(){
    int coste = mejor.GetCoste();
    int sol = -1;

    if(poblacion[peores.first].GetCoste() < coste){
        coste = poblacion[peores.first].GetCoste();
        sol = peores.first;
    }
    
    if(poblacion[peores.second].GetCoste() < coste){
        coste = poblacion[peores.second].GetCoste();
        sol = peores.second;
    }

    if (sol != -1){
        mejor = poblacion[sol];
    }
}

/**
 * Funcion que busca los peores de la poblacion
 */
void Memetico::buscaPeores(){
        peores.first = 0;
        peores.second = 1;

    
    for(int i = 2; i < poblacion.size(); i++){
        if(poblacion[i].GetCoste() > poblacion[peores.first].GetCoste()){
            if (poblacion[peores.first].GetCoste() > poblacion[peores.second].GetCoste())
                peores.second = peores.first;
            
            peores.first = i;
        }
    }
}

/**
 * Funcion que realiza la seleccion del mejor de 2 individuos
 * @return El mejor de los individuos
 */
int Memetico::torneo(){
    int ind1 = Randint(0, poblacion.size()-1);
    int ind2 = Randint(0, poblacion.size()-1);
    while (ind1 == ind2){ind2 = Randint(0, poblacion.size()-1);}

    if(poblacion[ind1].GetCoste() < poblacion[ind2].GetCoste()){
        return ind1;
    }

    return ind2;
}

/**
 * Funcion que implementa el operador de cruce OX2
 * @param padre Primer gen a cruzar
 * @param madre Segundo gen a cruzar
 * @param marcados Vector de los elementos marcados para cruzar
 * @return Devuelve el hijo
 */
vector<int> Memetico::OX2(vector<int> padre, vector<int> madre, vector<int> marcados){

    vector<int> posiciones;
    vector<int> datos;
    int total = 1;
    for(int i = 0; i < padre.size() && total < (marcados.size()*2) ; i++){
        for(int j = 0; j < marcados.size(); j++){
            if(padre[i] == marcados[j]){
                posiciones.push_back(i);
                total++;
            }

            if(madre[i] == marcados[j]){
                datos.push_back(madre[i]);
                total++;
            }
        }
    }

    for (int i = 0; i < posiciones.size(); i++){
        padre[posiciones[i]] = datos[i];
    }
    return padre;
}

/**
 * Funcion que implementa el operador de cruce MOC
 * @param padre Primer gen a cruzar
 * @param madre Segundo gen a cruzar
 * @param marca Marca a partir de la que se produce el cruce
 * @return Devuelve el hijo
 */
vector<int> Memetico::MOC(vector<int> padre, vector<int> madre, int marca){
    int pos = marca;
    for(int i = 0; i < padre.size(); i++){
        // Busco si el elemento debe conservarse
        bool marcado = false;
        for(int j = 0; j < marca; j++){
            if(padre[i] == madre[j])
                marcado = true;
        }

        // Lo sustituyo en caso de que no deba conservarse
        if(!marcado){
            padre[i] = madre[pos];
            pos++;
        }
    }
    return padre;
}


/**
 * Funcion que realiza la mutacion de los genes
 * @param sol Vector que hay que mutar
 * @param pos Posicion de la mutacion
 * @return Devuelve el vector mutado
 */
vector<int> Memetico::mutacion(vector<int> sol, int pos){
    int pos2, pos3;
    do{
        pos2 = Randint(0, sol.size()-1);
        pos3 = Randint(0, sol.size()-1);
    }while(pos != pos2 && pos2 != pos3 && pos != pos3 && pos2 < sol.size() 
            && pos3 < sol.size());

    swap(sol[pos],sol[pos3]);
    swap(sol[pos2],sol[pos3]);

    return sol;
}

vector<int> Memetico::tabu(string log, string id, int generacion, vector<int> partida, int& coste, 
        vector<vector<int> >& flu, vector<vector<int> >& dis, bool sim) {

    // Inicializo las memorias
    solRecientes.clear();
    movRecientes.clear();
    posListaSol = 0;
    posListaMov = 0;
    
    // Creo la los vectores que van a ir conteniendo las soluciones
    vector<int> mejorParcial(flu.size()); // Mejor del momento
    vector<int> mejorPeor(flu.size()); // Mejor solucion de todas las peores
    vector<int> solParcial(flu.size()); // Solucion actual

    // Asigno la solución de partida
    mejorParcial = partida;
    mejorPeor = partida;
    solParcial = partida;

    // Calculo el coste de la solución generada
    int costeMejorParcial = coste;
    int costeMejorPeor = std::numeric_limits<int>::max(); // Para asi irme a la mejor solucion, aunque ninguna de las generadas sea mejor que la de partida
    int costeSol = costeMejorParcial;

    int k = 0; // Limite de iteraciones 100 o 500
    int intentos = 0; // Limite de 100 intentos
    while( k <= stoi(parametros[NUM_ITERACIONES])){
        
        // Calcúlo 10 vecinos
        generaCambios(log, costeSol, solParcial, flu, dis, sim);
        
        if(costeMejorParcial > costeSol){
            k++;
            intentos = 0;
            guardaSol(solParcial);
            mejorParcial = solParcial;
            costeMejorParcial = costeSol;
            costeMejorPeor = std::numeric_limits<int>::max();
            
            if (coste > costeMejorParcial){
                partida = solParcial;
                coste = costeMejorParcial;
                
                // Registro la iteracion en la que se produce el movimiento de mejora con respecto a la solucipon de partida
                if(parametros[LOG] == "SI"){
                    if(generacion < 30 || generacion % 10 == 0){
                        // Registro en el log la poblacion
                        registraCadena(log, "Individuos mejorado de la Tabu. Iteracion : " + to_string(k));
                        registraLogTabu(log,partida, coste, id);
                    }
                }
            }
                     
        }else{
            intentos++;
            if( costeMejorPeor > costeSol){
                mejorPeor = solParcial;
                costeMejorPeor = costeSol;
            }
            solParcial = mejorParcial;
            costeSol = costeMejorParcial;
        }
        
        if(intentos == stoi(parametros[MAX_INTENTOS_TABU])){              
            k++;
            intentos = 0;
            
            // Cambio la solucion actual por la mejor de los 100 entornos
            solParcial = mejorPeor;
            costeSol = costeMejorPeor;
            mejorParcial = solParcial;
            costeMejorParcial = costeSol;
            
            
            // Para asi irme a la mejor solucion, aunque ninguna de las generadas sea mejor que la de partida
            costeMejorPeor = std::numeric_limits<int>::max();
        }
    }
    
    return partida;
}

/**
* Función para generar los posibles vecinos adaptada al tabú
* @param log Objeto para registrar los cambios que se van realizando
* @param costeActual Coste de la solución actual
* @param sol Vector solución actual, se modifica por el mejor vecino si se mejora el coste
* @param flu Matriz de flujo
* @param dis Matriz de distancia
* @return Devuelve True si se ha movido a algun vecino, False si no lo hace
*/
void Memetico::generaCambios(string log, int& costeActual, vector<int>& sol,
       vector<vector<int>>& flu, vector<vector<int>>& dis, bool sim){
   int mejorCoste = std::numeric_limits<int>::max();
   int pos = 0;
   vector<pair<int,int>> vecinos; // vector con las permutaciones creadas
   
   // Creo los diez posibles vecinos de forma aleatoria
   while( vecinos.size() < stoi(parametros[NUM_SOL_TABU])){
        int pos1 = Randint(0,sol.size()-1);
        int pos2;
        do{
            pos2 = Randint(0,sol.size()-1);
        }while(pos1 == pos2);
        
        // Compruebo que la solución no esta ya creada
        bool esta = false;
        for(int j = 0; j < vecinos.size(); j++){
            if(vecinos[j].first == pos1 && vecinos[j].second == pos2){
                esta = true;
            }else if(vecinos[j].first == pos2 && vecinos[j].second == pos1){
                esta = true;
            }
        }

        if(!esta && (pos1 < sol.size() && pos2 < sol.size())){ // No se porque pero me genera numeros fuera del rango
            vecinos.push_back(pair<int,int>(pos1, pos2));

            if(compruebaSol(sol, pos1, pos2)){ //Comprueba que no este en la lista de movimientos

                int coste = calculaCoste2(costeActual,pos1, pos2, sol, flu, dis);
                // Voy seleccionando la mejor solucion hasta el momento.
                // Y que no sea una solución tabú
                if(coste < mejorCoste){ 
                    mejorCoste = coste;
                    pos = vecinos.size()-1;
                }
            }
        }
    }

   if(costeActual > mejorCoste){
       guardaMov(vecinos[pos].first, vecinos[pos].second);
   }

   costeActual = mejorCoste;
   swap(sol[vecinos[pos].first], sol[vecinos[pos].second]);//Al estar pasado referencia se modifica el original
}

/**
 * Función que guarda el movimiento generado
 * @param sol Movimiento generado
 */
void Memetico::guardaMov(int pos1, int pos2){
    list<pair<int,int>>::iterator it = movRecientes.begin();
    for(int i = 0; i < posListaMov % stoi(parametros[TENENCIA_TABU]); i++)
        it++;
    posListaMov++;
    pair<int,int> sol(pos1,pos2);
    movRecientes.insert(it, sol);
}

/**
 * Función que guarda la solución generada
 * @param sol Movimiento generado
 */
void Memetico::guardaSol(vector<int> sol){
    list<vector<int>>::iterator it = solRecientes.begin();
    for(int i = 0; i < posListaSol % stoi(parametros[TENENCIA_TABU]); i++)
        it++;
    posListaSol++;
    solRecientes.insert(it, sol);
}

/**
 * Función que comprueba si la solución esta en la lista de movimientos tabú
 * @param sol Solución orignal
 * @param pos1 Posición 1
 * @param pos2 Posición 2
 * @return True si no esta, False en caso contrario
 */
bool Memetico::compruebaSol(vector<int> sol, int pos1, int pos2){
    swap(sol[pos1], sol[pos2]);//Al estar pasado por copia no se modifica el original
    for (list<vector<int>>::iterator it = solRecientes.begin(); it != solRecientes.end(); it++){
        if(*it == sol){
            return false;
        }
    }

    for(list<pair<int,int>>::iterator it = movRecientes.begin(); it != movRecientes.end(); it++){
        if(it->first == pos1 && it->second == pos2)
            return false;
    }

    return true;
}