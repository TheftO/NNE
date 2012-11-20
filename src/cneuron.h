#ifndef CNEURON_H
#define CNEURON_H
#include "DEFINES.h"
#include "clayer.h"
#include "luainclude.h"
#include <fstream>

class CLayer;

class CNeuron
{
public:
    CNeuron();
    CNeuron(StdString sum, int sumSize, char *activ, int activeSize);
    ~CNeuron();
    int getId();
    void activate();
    void input(int id_sender, double signal);
    void input(double signal);
    StdMap<int, double> & getInputs();
    void setWeight(int id_sender, double value);
    double getWeight(int id_sender);
    Vector<double> getDenamedWeights();
    Vector<int> getConnections();
    void send();
    void connectWithNeuron(CNeuron *neuron);
    void disconnectNeuron(int id);
    void disconnectNeuron(CNeuron* neuron);
    void setSum(StdString sum, int sumSize);
    StdString getSum() const
    {
        return summFunc;
    }
    void setActivation(StdString activ, int actSize);
    StdString getActivation() const
    {
        return activationFunc;
    }
    double getOutput();
    void saveWeights(StdString file);
    void setOutput(double out);
    void setId(int id);
private:
    int id;
    double output;
    StdMap<int, double> inputs;
    Vector<CNeuron*> connections;
    StdMap<int, double> weights;
    StdString activationFunc;
    StdString summFunc;
    static int lastId;
    static Vector<CNeuron*> neurons;
    double sum();
    double activation(double sum);
    lua_State *lua;
};

#endif // CNEURON_H
