#ifndef CLAYER_H
#define CLAYER_H
#include "DEFINES.h"
#include "luainclude.h"
#include "cneuron.h"
#include <fstream>
#include <iostream>

class CNeuron;

class CLayer
{
public:
    CLayer();
    CLayer(int neuron_count, StdString sum, int sumSize, char *activation, int actSize);
    ~CLayer();
    int getId();
    void setId(int id);
    void activate();
    void saveWeights(StdString file);
    void setWeight(int id_neuron_from, int id_neuron_to, double weight);
    Vector<double> getWeights();
    void connectWithLayer(CLayer* layer);
    void connectWithNeuron(CNeuron* neuron);
    void setNextLayer(CLayer* nextLayer);
    CLayer *getNextLayer()
    {
        return nextLayer;
    }
    Vector<CNeuron*> getNeurons();
    void addNeuron(int count);
    void delNeuron(int id);
    void input(Vector<double> input);
    double getOutput(int id);
    //int getOutput(lua_State* state);
    Vector<double> getOutput();
    void setSummFunc(StdString sf, int size);
    StdString getSummFunc();
    void setActivFunc(StdString af, int size);
    StdString getActivFunc();
private:
    Vector<CNeuron*> vectorNeurons;
    CLayer* nextLayer;
    int id_layer;
    StdString summFunc;
    int sumSize;
    StdString activFunc;
    int activSize;
    static int last_layer_id;
    void setSummFunc();
    void setActivFunc();
    //int setOutput(lua_State* state);
    //lua_State* lua;
};

#endif // CLAYER_H
