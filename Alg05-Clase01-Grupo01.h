/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Memetico.h
 * Author: PC
 *
 * Created on 18 de noviembre de 2019, 16:18
 */

#ifndef MEMETICO_H
#define MEMETICO_H
#include <vector>
#include <list>
#include <string>
#include "Individuo.h"

class Memetico {
public:
    Individuo memetico(string rutaLog, vector<vector<int>>& flu, 
            vector<vector<int>>& dis, bool sim);
private:
    // Variables
    // Genético
    vector<Individuo> poblacion;
    pair<int, int> seleccionados;
    pair<int, int> peores;
    Individuo mejor;
    // Tabú
    list<vector<int>> solRecientes;
    list<pair<int,int>> movRecientes;
    int posListaSol = 0;
    int posListaMov = 0;
    
    // Funciones privadas
    // Genético
    void buscaMejor();
    void buscaPeores();
    int torneo();
    vector<int> OX2(vector<int> padre, vector<int> madre, vector<int> marcados);
    vector<int> MOC(vector<int> padre, vector<int> madre, int marca);
    vector<int> mutacion(vector<int> sol, int pos);
    // Tabú
    vector<int> tabu(string log, string id, int generacion, vector<int> partida, int& coste, vector<vector<int>>& flu, 
                        vector<vector<int>>& dis, bool sim);
    void generaCambios(string log, int& costeActual, vector<int>& sol,
        vector<vector<int>>& flu, vector<vector<int>>& dis, bool sim);
    void guardaMov(int pos1, int pos2);
    void guardaSol(vector<int> sol);
    bool compruebaSol(vector<int> sol, int pos1, int pos2);
};

#endif /* MEMETICO_H */

