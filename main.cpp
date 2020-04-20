/* 
 * File:   main.cpp
 * Author: sjm00010
 *
 * Created on 25 de octubre de 2019, 15:35
 */

#include <cstdlib>
#include <iostream>
#include <vector>

#include "Timer.h"
#include "Parametros.h"
#include "FuncionesComunes.h"
#include "Alg05-Clase01-Grupo01.h"
#include "random.h"

using namespace std;

string rutaParam = "./parametros.txt";
int numParam = 17;
vector<string> parametros;

/*
 * 
 */
int main(int argc, char** argv) {
    // Variables que almacenan los datos del problema
    vector<vector<int>> flujo;
    vector<vector<int>> distancia;
    bool simetrica;
    
    // Variables locales
    Timer crono;
    Individuo sol;
    int coste;
    double tiempo;
    
    // Cargar de datos
    cargaParametros();
    string ruta = parametros[CARPETA_DATOS] + parametros[NOMBRE_ARCHIVO];
    cout << "\n-------------------------------------------------\n";
    cout << "       CARGA DE : "+ parametros[NOMBRE_ARCHIVO] +"\n";
    cout << "-------------------------------------------------\n";
    cargaDatos(ruta, flujo, distancia, simetrica);
    cout << " Carga completada con exito.\n";
    
    // Prueba Memetico
    for(int i = 1; i <= stoi(parametros[NUM_PRUEBAS], nullptr, 10); i++){
        Memetico alg;
        cout << "-------------------------------------------------\n";
        cout << "  MEJOR SOLUCION " + to_string(i) +" MEMETICO(IT. "+ parametros[NUM_ITERACIONES]
                +")-"+ parametros[TIPO_CRUCE] +" PARA : "+ parametros[NOMBRE_ARCHIVO] +"\n";
        cout << "-------------------------------------------------\n";

        // Establezco la semilla para cada prueba
        Set_random(calculaSemilla(i));

        // Inicio del contador
        crono.start();

        string rutaLog = parametros[CARPETA_LOG] + parametros[NOMBRE_ARCHIVO] +
                "-MEMETICO("+ parametros[NUM_ITERACIONES] +")_" + parametros[TIPO_CRUCE] + "-" + to_string(i) + ".log";
        sol = alg.memetico(rutaLog, flujo, distancia, simetrica);

        // Fin del contador
        crono.stop();
        tiempo = crono.getElapsedTimeInMilliSec();

        // Escribir soluciones en fichero .log
        if(parametros[LOG] == "SI"){
            registraLogSolucion(rutaLog, sol);
            registraTiempo(rutaLog, tiempo, calculaSemilla(i));
        }

        //Mostrar datos
        mostrarResultado(sol.GetSolucion(), sol.GetCoste(), tiempo);
    }
    
    return 0;
}

