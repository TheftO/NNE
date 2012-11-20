#include "cnnetwork.h"

static int LuaCallback(lua_State *state)
{
    int iNumberIdx = lua_upvalueindex (1);
    int iThisRef = lua_upvalueindex(2);
    int nRetsOnStack = 0;
    if (lua_islightuserdata (state, iThisRef))
    {
        CNNetwork *pThis = (CNNetwork *) lua_touserdata (state, iThisRef);
        int iMethodIdx = (int) lua_tonumber (state, iNumberIdx);
        nRetsOnStack = pThis->callFunction(pThis->luaState(), iMethodIdx);
        lua_pushstring (state, "LuaCallback -> Failed to call the class function");
        lua_error (state);

    }
    return nRetsOnStack;
}

CNNetwork::CNNetwork()
{
    firstLayer = NULL;
    lastLayer = NULL;
    setStudyFunc(NULL, 0);
    lua = lua_open();
    m_nMethods = 0;
    m_nArgs = 0;
    luaopen_base(lua);
    luaopen_table(lua);
    luaopen_math(lua);
    lua_newtable(lua);
    methodBase = registerFunc("setWeight");
}

Vector<double> CNNetwork::emulate()
{
    if(firstLayer != NULL)
    {
        firstLayer->input(input);
        firstLayer->activate();
        if(lastLayer != NULL)
            return lastLayer->getOutput();
    }
    Vector<double> res;
    return res;
}

void CNNetwork::setInput(Vector<double> input)
{
    this->input = input;
}

void CNNetwork::setOutput(Vector<double> output)
{
    studyOutput = output;
}

void CNNetwork::setStudyFunc(StdString sf, int size)
{
    delete [] studyFunc;
    if(sf != NULL)
    {
        studySize = size + 1;
        studyFunc = new char[studySize];
        for(int i = 0; i < studySize; i++)
            studyFunc[i] = sf[i];
    }else
    {
        studySize = 1;
        studyFunc = new char[studySize];
        studyFunc[0] = '\0';
    }
}

void CNNetwork::saveWeights(StdString filename)
{
    if(filename == NULL)
        return;
    std::ofstream out;
    out.open(filename, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if(!out.is_open())
        return;
    unsigned int si = layers.size();
    out.write((char *)&si, sizeof(unsigned int)); //сохраняем количество слоев
    for(unsigned int i = 0; i < layers.size(); i++)
        layers[i]->saveWeights(filename);
    out.close();
}

void CNNetwork::study()
{
    emulate();
    luaL_loadfile(lua, studyFunc);
    lua_newtable(lua);
    for(unsigned int i = 1; i < studyOutput.size(); i++)
    {
        lua_pushnumber(lua, i);
        lua_pushnumber(lua, studyOutput[i]);
        lua_rawset(lua, -3);
    }
    lua_setglobal(lua, "studyOutput");
    int i = 1;
    CLayer *layer = firstLayer;
    lua_newtable(lua);
    while(layer != NULL)
    {
        lua_pushinteger(lua, i);
        lua_newtable(lua);
        lua_pushstring(lua, "id");
        lua_pushinteger(lua, layer->getId());
        lua_rawset(lua, -3);
        for(unsigned int j = 0; j < layer->getNeurons().size(); j++)
        {
            lua_pushinteger(lua, j + 1);
            lua_newtable(lua);
            lua_pushstring(lua, "id");
            lua_pushinteger(lua, layer->getNeurons()[j]->getId());
            lua_rawset(lua, -3);
            lua_pushstring(lua, "weights");
            lua_newtable(lua);
            for(unsigned int k = 0; k < layer->getNeurons()[j]->getConnections().size(); k++)
            {
                int neuronFromId = layer->getNeurons()[j]->getConnections()[k];
                lua_pushinteger(lua, neuronFromId);
                lua_pushnumber(lua, layer->getNeurons()[j]->getWeight(neuronFromId));
                lua_rawset(lua, -3);
            }
            lua_rawset(lua, -3);
            lua_pushstring(lua, "inputs");
            lua_newtable(lua);
            StdMap<int, double>::iterator it;
            for(it = layer->getNeurons()[j]->getInputs().begin(); it != layer->getNeurons()[j]->getInputs().end(); it++)
            {
                lua_pushinteger(lua, it->first);
                lua_pushnumber(lua, it->second);
                lua_rawset(lua, -3);
            }
            lua_rawset(lua, -3);
            lua_pushstring(lua, "output");
            lua_pushnumber(lua, layer->getNeurons()[j]->getOutput());
            lua_rawset(lua, -3);
            lua_rawset(lua, -3);
        }
        lua_rawset(lua, -3);
        layer = layer->getNextLayer();
        i++;
    }
    lua_setglobal(lua, "layers");
    int err = lua_pcall(lua, 0, 0, 0);
    if(err)
        return;
}

int CNNetwork::setWeight(lua_State *state)
{
    if(lua_gettop(state) > 3)
    {
        int id_layer_to = lua_tointeger(state, 1);
        int id_neuron_from = lua_tointeger(state, 2);
        int id_neuron_to = lua_tointeger(state, 3);
        double weight = lua_tonumber(state, 4);
        setConnectionWeight(id_layer_to, id_neuron_from, id_neuron_to, weight);
        return 1;
    }
    return 0;
}

void CNNetwork::setFirstLayer(CLayer *layer)
{
    if(layer != NULL)
        firstLayer = layer;
}

CLayer *CNNetwork::getFirstLayer()
{
    return firstLayer;
}

void CNNetwork::setLastLayer(CLayer *layer)
{
    if(layer != NULL)
        lastLayer = layer;
}

CLayer *CNNetwork::getLastLayer()
{
    return lastLayer;
}

void CNNetwork::setConnectionWeight(int id_layer_to, int id_neuron_from, int id_neuron_to, double weight)
{
    for(unsigned int i = 0; i < layers.size(); i++)
    {
        if(layers[i]->getId() == id_layer_to)
        {
            layers[i]->setWeight(id_neuron_from, id_neuron_to, weight);
        }
    }
}

int CNNetwork::registerFunc(const char *funcName)
{
    int iMethodIdx = -1;
    iMethodIdx = ++m_nMethods;
    lua_pushnumber (lua, (lua_Number) iMethodIdx);
    lua_pushlightuserdata (lua, (void *) this);
    lua_pushcclosure (lua, LuaCallback, 2);
    lua_setglobal(lua, funcName);
    return iMethodIdx;
}

int CNNetwork::callFunction(lua_State *state, int methodIndex)
{
    switch(methodIndex - methodBase)
    {
        case 0:
        {
            return setWeight(state);
        }
        default:
        {
            break;
        }
    }
    return 0;
}

CLayer *CNNetwork::addLayer(int neuronCount, StdString sum, int sumSize, StdString activation ,int actSize)
{
    if(neuronCount > 0)
    {
        CLayer *newLayer = new CLayer(neuronCount, sum, sumSize, activation, actSize);
        layers.push_back(newLayer);
        return newLayer;
    }
    return NULL;
}

Vector<CLayer *> CNNetwork::addLayers(int layerCount, int neuronCount, StdString sum, int sumSize, StdString activation, int actSize)
{
    Vector<CLayer *> newLayersList;
    for(int i = 0; i < layerCount; ++i)
    {
        newLayersList.push_back(addLayer(neuronCount, sum, sumSize, activation ,actSize));
    }
    return newLayersList;
}

void CNNetwork::delLayer(int id)
{
    for(unsigned int i = 0; i < layers.size(); ++i)
        if(layers[i]->getId() == id)
        {
            layers[i] = (layers.back());
            layers.pop_back();
        }
}

CNNetwork::~CNNetwork()
{
    lua_close(lua);
    layers.clear();
    input.clear();
    studyOutput.clear();
}
