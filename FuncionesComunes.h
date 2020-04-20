/* 
 * File:   FuncionesComunes.h
 * Author: PORTATIL
 *
 * Created on 1 de noviembre de 2019, 21:39
 */

#ifndef FUNCIONESCOMUNES_H
#define FUNCIONESCOMUNES_H

#include "Individuo.h"

using namespace std;
    // Carga Datos
    void cargaParametros();
    void cargaDatos(string &archivo, vector<vector<int>> &flu,
            vector<vector<int>> &dis, bool &sim);
    
    // Registros .log
    void creaLog(string nombreAr);
    void registraCadena(string log, string elec);
    void registraTiempo(string log, double t, int semilla);
    void registraLogGeneracion(string log, int generacion);
    void registraLogPoblacion(string log, vector<Individuo> poblacion);
    void registraLogIndividuos(string log, vector<Individuo> poblacion, pair<int, int> seleccionados);
    void registraLogSolucion(string log, Individuo i);
    void registraLogVector(string log, vector<int> v, string id);
    void registraLogTabu(string log, vector<int> v, int coste, string id);
    
    // Otros
    vector<int> creaSolucion(int tam);
    int calculaCoste (vector<int> sol, vector<vector<int>>& flu, 
            vector<vector<int>>& dis, bool sim);
    int calculaCoste2(int costeViejo, int pos1, int pos2 ,vector<int> sol,
            vector<vector<int>>& flu, vector<vector<int>>& dis);
    int calculaSemilla(int prueba);
    void mostrarResultado( vector<int> v, const int coste, double t);

#endif /* FUNCIONESCOMUNES_H */

