#ifndef CNNETWORK_H
#define CNNETWORK_H

#include "clayer.h"
#include "DEFINES.h"
#include "luainclude.h"
#include <fstream>
#include <iostream>

class CNNetwork
{
public:
    CNNetwork();
    ~CNNetwork();
    Vector<double> emulate();
    void setInput(Vector<double> input);
    void setOutput(Vector<double> output);
    CLayer *addLayer(int neuronCount, StdString sum, int sumSize, char *activation, int actSize);
    Vector<CLayer *> addLayers(int layerCount, int neuronCount, StdString sum, int sumSize, char *activation, int actSize);
    void delLayer(int id);
    void study();
    void setStudyFunc(StdString sf, int studySize);
    void saveWeights(StdString filename);
    int setWeight(lua_State* state);
    void setFirstLayer(CLayer *layer);
    CLayer *getFirstLayer();
    void setLastLayer(CLayer *layer);
    CLayer *getLastLayer();
    void setConnectionWeight(int id_layer_to, int id_neuron_from, int id_neuron_to, double weight);
    int registerFunc(const char* funcName);
    int callFunction(lua_State *state, int methodIndex);
    lua_State *luaState()
    {
        return lua;
    }
    int methods()
    {
        return m_nMethods;
    }
protected:
    Vector<CLayer *> layers;
    StdString studyFunc;
    Vector<double> input;
    Vector<double> studyOutput;
    CLayer* firstLayer;
    CLayer* lastLayer;
    lua_State* lua;
    int studySize;
    int m_nMethods;
    int m_iThisRef;
    int m_nArgs;
    int methodBase;
    const char *m_strFunctionName;
};

#endif // CNNETWORK_H
