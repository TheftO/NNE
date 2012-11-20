#include "clayer.h"

int CLayer::last_layer_id = 0;

CLayer::CLayer()
{
    id_layer = last_layer_id++;
    nextLayer = NULL;
    setSummFunc(NULL, 0);
    setActivFunc(NULL, 0);
}

CLayer::CLayer(int neuron_count, StdString sum, int sumSize, StdString activation, int actSize)
{
    id_layer = CLayer::last_layer_id++;
    setSummFunc(sum, sumSize);
    setActivFunc(activation, actSize);
    nextLayer = NULL;
    addNeuron(neuron_count);
}

void CLayer::addNeuron(int count = 1)
{
    if(count > 0)
        for(int i = 0; i < count; ++i)
            vectorNeurons.push_back(new CNeuron(summFunc, sumSize, activFunc, activSize));
    else
        for(int i = 0; i < (- count); ++i)
            vectorNeurons.erase(vectorNeurons.end());
}

void CLayer::delNeuron(int id)
{
    for(unsigned int i = 0; i < vectorNeurons.size(); ++i)
        if(vectorNeurons.at(i)->getId() == id)
        {
            vectorNeurons.at(i) = (vectorNeurons.back());
            vectorNeurons.pop_back();
        }
}

Vector<CNeuron*> CLayer::getNeurons()
{
    return vectorNeurons;
}

void CLayer::connectWithLayer(CLayer *layer)
{
    if(layer != NULL)
        for(unsigned int i = 0; i < layer->getNeurons().size(); ++i)
            connectWithNeuron(layer->getNeurons()[i]);
}

void CLayer::connectWithNeuron(CNeuron *neuron)
{
    if(neuron != NULL)
        for(unsigned int i = 0; i < vectorNeurons.size(); i++)
            vectorNeurons[i]->connectWithNeuron(neuron);
}

void CLayer::setNextLayer(CLayer *nextLayer)
{
	this->nextLayer = nextLayer;
}

void CLayer::setWeight(int id_neuron_from, int id_neuron_to, double weight)
{
    for(unsigned int i = 0; i < vectorNeurons.size(); i++)
    {
        if(vectorNeurons[i]->getId() == id_neuron_to)
            vectorNeurons[i]->setWeight(id_neuron_from, weight);
    }
}

Vector<double> CLayer::getWeights()
{
    Vector<double> res;
    for(unsigned int i = 0; i < vectorNeurons.size(); i++)
        for(unsigned int j = 0; j < vectorNeurons[i]->getDenamedWeights().size(); j++)
            res.push_back(vectorNeurons[i]->getDenamedWeights()[j]);
    return res;
}

void CLayer::input(Vector<double> input)
{
    for(unsigned int i = 0; i < vectorNeurons.size(); ++i)
        if(i < input.size())
        {
            vectorNeurons.at(i)->input(input.at(i));
        }
        else
        {
            vectorNeurons.at(i)->input(0);
        }
}

void CLayer::activate()
{
    for(unsigned int i = 0; i < vectorNeurons.size(); i++)
        vectorNeurons.at(i)->activate();
    if(nextLayer != NULL)
        nextLayer->activate();
}

void CLayer::saveWeights(StdString file)
{
    if(file == NULL)
        return;
    std::ofstream out;
    out.open(file, std::ios_base::out | std::ios_base::binary | std::ios_base::app);
    if(!out.is_open())
        return;         //сохраняем id слоя и количество нейронов в слое
    std::cerr<<id_layer<<": "<<vectorNeurons.size();
    out.write((char *)&id_layer, sizeof(int));
    unsigned int si = vectorNeurons.size();
    out.write((char *)&si, sizeof(unsigned int));
    out.close();
    for(unsigned int i = 0; i < vectorNeurons.size(); i++)
        vectorNeurons[i]->saveWeights(file);
}

int CLayer::getId()
{
    return id_layer;
}

void CLayer::setId(int id)
{
    if(CLayer::last_layer_id < id)
    {
        CLayer::last_layer_id = id;
    }
    id_layer = id;
}

double CLayer::getOutput(int id)
{
    for(unsigned int i = 0; i < vectorNeurons.size(); ++i)
        if(vectorNeurons[i]->getId() == id)
            return vectorNeurons[i]->getOutput();
    return 0;
}

Vector<double> CLayer::getOutput()
{
    Vector<double> res;
    for(unsigned int i = 0; i < vectorNeurons.size(); i++)
        res.push_back(vectorNeurons[i]->getOutput());
    return res;
}

void CLayer::setActivFunc(StdString af, int size)
{
    delete [] activFunc;
    if(af != NULL)
    {
        activSize = size + 1;
        activFunc = new char[activSize];
        for(int i = 0; i < activSize; i++)
            activFunc[i] = af[i];
    }else
    {
        activSize = 1;
        activFunc = new char[activSize];
        activFunc[0] = '\0';
    }
    setActivFunc();
}

StdString CLayer::getActivFunc()
{
    return activFunc;
}

void CLayer::setSummFunc(StdString sf, int size)
{
    delete [] summFunc;
    if(sf != NULL)
    {
        sumSize = size + 1;
        summFunc = new char[sumSize];
        for(int i = 0; i < sumSize; i++)
            summFunc[i] = sf[i];
    }else
    {
        sumSize = 1;
        summFunc = new char[sumSize];
        summFunc[0] = '\0';
    }
    setSummFunc();
}

StdString CLayer::getSummFunc()
{
    return vectorNeurons.front()->getSum();
}

void CLayer::setActivFunc()
{
    for(unsigned int i = 0; i < vectorNeurons.size(); i++)
        vectorNeurons[i]->setActivation(activFunc, activSize);
}

void CLayer::setSummFunc()
{
    for(unsigned int i = 0; i < vectorNeurons.size(); i++)
        vectorNeurons[i]->setSum(summFunc, sumSize);
}

CLayer::~CLayer()
{
    vectorNeurons.clear();
}
