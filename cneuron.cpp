#include "cneuron.h"

int CNeuron::lastId = 1;
Vector<CNeuron*> CNeuron::neurons;

CNeuron::CNeuron()
{
    neurons.push_back(this);
    id = CNeuron::lastId++;
    output = 0;
    summFunc = NULL;
    activationFunc = NULL;
    lua = lua_open();
    luaopen_base(lua);
    luaopen_math(lua);
}

CNeuron::CNeuron(StdString sum, int sumSize, StdString activ, int activeSize)
{
    id = CNeuron::lastId++;
    output = 0;
    setSum(sum, sumSize);
    setActivation(activ, activeSize);
    lua = lua_open();
    luaopen_base(lua);
    luaopen_table(lua);
    luaopen_math(lua);
}

int CNeuron::getId()
{
    return id;
}

double CNeuron::getOutput()
{
    return output;
}

void CNeuron::input(int id_sender, double signal)
{
    inputs[id_sender] = signal;
    if(weights.find(id_sender) == weights.end())
        weights.insert(std::make_pair<int, double>(id_sender, 1.0));
}

void CNeuron::input(double signal)
{
    inputs[0] = signal;
    if(weights.find(0) == weights.end())
        weights.insert(std::make_pair<int, double>((0), 1.0));
}

StdMap<int, double> &CNeuron::getInputs()
{
    return inputs;
}

void CNeuron::setWeight(int id_sender, double value)
{
    weights[id_sender] = value;
}

double CNeuron::getWeight(int id_sender)
{
    if(weights.find(id_sender) != weights.end())
        return weights[id_sender];
    return 0;
}

Vector<double> CNeuron::getDenamedWeights()
{
    Vector<double> res;
    StdMap<int, double>::iterator pos;
    for(pos = weights.begin(); pos != weights.end(); ++pos)
        res.push_back(pos->second);
    return res;
}

Vector<int> CNeuron::getConnections()
{
    Vector<int> res;
    StdMap<int, double>::iterator pos;
    for(pos = weights.begin(); pos != weights.end(); ++pos)
        res.push_back(pos->first);
    return res;
}

void CNeuron::saveWeights(StdString file)
{
    if(file == NULL)
        return;
    std::ofstream out;
    out.open(file, std::ios_base::out | std::ios_base::binary | std::ios_base::app);
    if(!out.is_open())
        return;
    out.write((char *)&id, sizeof(int));  //сохраняем id нейрона и количество весов
    unsigned int si = weights.size();
    out.write((char *)&si, sizeof(unsigned int));
    std::cerr<<id<<": "<<si;
    StdMap<int, double>::iterator pos;
    for(pos = weights.begin(); pos != weights.end(); ++pos)
    {
        int i = pos->first;
        out.write((char *)&i, sizeof(int)); //сохраняем id соединенного нейрона и вес
        double w = pos->second;
        out.write((char *)&w, sizeof(double));
    }
    out.close();
}

void CNeuron::connectWithNeuron(CNeuron *neuron)
{
    if(neuron != NULL)
    {
        for(unsigned int i = 0; i < connections.size(); ++i)
            if(neuron == connections[i])
            {
                return;
            }
	    connections.push_back(neuron);
    }
}

void CNeuron::disconnectNeuron(int id)
{
    weights.erase(id);
    inputs.erase(id);
}

void CNeuron::disconnectNeuron(CNeuron *neuron)
{
    neuron->disconnectNeuron(id);
    for(unsigned int i = 0; i < connections.size(); i++)
        if(connections.at(i) == neuron)
        {
            connections.at(i) = connections.back();
            connections.pop_back();
        }
}

void CNeuron::send()
{
    for(int i = 0; i < connections.size(); i++)
        connections.at(i)->input(id, output);
}

void CNeuron::setActivation(StdString activ, int actSize)
{
    delete [] activationFunc;
    activationFunc = new char[actSize];
    for(int i = 0; i < actSize; i++)
        activationFunc[i] = activ[i];
}

void CNeuron::setSum(StdString sum, int sumSize)
{
    delete [] summFunc;
    summFunc = new char[sumSize];
    for(int i = 0; i < sumSize; i++)
        summFunc[i] = sum[i];
}

void CNeuron::activate()
{
    setOutput(activation(sum()));
    send();
}

double CNeuron::sum()
{
    double res = 0;
    if(summFunc == NULL || summFunc[0] == '\0')
        return res;
    luaL_loadfile(lua, summFunc);
    lua_newtable(lua);
    StdMap<int, double>::iterator it;
    int i = 0;
    for(it = inputs.begin(); it != inputs.end(); it++)
    {
        lua_pushnumber(lua, i);
        lua_pushnumber(lua, it->second);
        lua_rawset(lua, -3);
        i++;
    }
    lua_setglobal(lua, "inputs");
    lua_newtable(lua);
    i = 0;
    for(it = inputs.begin(); it != inputs.end(); it++)
    {
        StdMap<int, double>::iterator it1 = weights.find(it->first);
        if(it1 != weights.end())
        {
            lua_pushnumber(lua, i);
            lua_pushnumber(lua, it1->second);
            lua_rawset(lua, -3);
            i++;
        }
    }
    lua_setglobal(lua, "weights");
    int err = lua_pcall(lua, 0, 1, 0);
    if(err)
        return 0;
    if(lua_isnumber(lua, -1))
    {
        res = lua_tonumber(lua, -1);
        lua_pop(lua, 0);
        return res;
    }
    return 0;
}

double CNeuron::activation(double sum)
{
    double res = 0;
    if(activationFunc == NULL || activationFunc[0] == '\0')
        return res;
    luaL_loadfile(lua, activationFunc);
    lua_pushnumber(lua, sum);
    lua_setglobal(lua, "sum");
    int err = lua_pcall(lua, 0, 1, 0);
    if(err)
        return 0;
    if(lua_isnumber(lua, -1))
    {
        res = lua_tonumber(lua, -1);
        lua_pop(lua, 0);
        return res;
    }
    return 0;
}

void CNeuron::setOutput(double out)
{
    output = out;
}

void CNeuron::setId(int id)
{
    CNeuron::lastId = id;
    this->id = CNeuron::lastId++;
}

CNeuron::~CNeuron()
{
    for(unsigned int i = 0; i < connections.size(); i++)
        connections.at(i)->disconnectNeuron(id);
    for(unsigned int i = 0; i < neurons.size(); i++)
        neurons.at(i)->disconnectNeuron(this);
    for(unsigned int i = 0; neurons.size(); i++)
        if(neurons.at(i) == this)
        {
            neurons.at(i) = (neurons.back());
            neurons.pop_back();
        }
    inputs.clear();
    connections.clear();
    lua_close(lua);
}
