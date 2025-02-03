// include C++ standard libraries
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <stdint.h>

#include "CpmgConfig.h"

using namespace std;

// default constructors
CpmgConfig::CpmgConfig() : 
    APPLY_BULK(false), 
    TIME_VERBOSE(false), 
    INTERPOLATE_FIELD(false),
    USE_T2_LOGSPACE(true), 
    SAVE_MODE(true)
{
    (*this).readConfigFile("false", "3000.0", "image-based", "false", "uniform", "10.0", "2", "${path-to-bin}", "false", "0.1", "100000.0", "true", "256", "-4", "2", "512", "0", "0.00", "true", "false", "false", "false", "false", "false");
}

// copy constructors
CpmgConfig::CpmgConfig(const CpmgConfig &otherConfig) 
{
    // --- Physical attributes
    this->APPLY_BULK = otherConfig.APPLY_BULK;
    this->OBS_TIME = otherConfig.OBS_TIME;
    this->METHOD = otherConfig.METHOD;
    this->TIME_VERBOSE = otherConfig.TIME_VERBOSE;

    this->RESIDUAL_FIELD = otherConfig.RESIDUAL_FIELD;
    this->GRADIENT_VALUE = otherConfig.GRADIENT_VALUE;
    this->GRADIENT_DIRECTION = otherConfig.GRADIENT_DIRECTION;
    this->PATH_TO_FIELD = otherConfig.PATH_TO_FIELD;
    this->INTERPOLATE_FIELD = otherConfig.INTERPOLATE_FIELD;

    this->MIN_T2 = otherConfig.MIN_T2;
    this->MAX_T2 = otherConfig.MAX_T2;
    this->USE_T2_LOGSPACE = otherConfig.USE_T2_LOGSPACE;
    this->NUM_T2_BINS = otherConfig.NUM_T2_BINS;
    this->MIN_LAMBDA = otherConfig.MIN_LAMBDA;
    this->MAX_LAMBDA = otherConfig.MAX_LAMBDA;
    this->NUM_LAMBDAS = otherConfig.NUM_LAMBDAS;
    this->PRUNE_NUM = otherConfig.PRUNE_NUM;
    this->NOISE_AMP = otherConfig.NOISE_AMP;

    // --- cpmg SAVE. 
    this->SAVE_MODE = otherConfig.SAVE_MODE;
    this->SAVE_T2 = otherConfig.SAVE_T2;
    this->SAVE_WALKERS = otherConfig.SAVE_WALKERS;
    this->SAVE_DECAY = otherConfig.SAVE_DECAY;
    this->SAVE_HISTOGRAM = otherConfig.SAVE_HISTOGRAM;
    this->SAVE_HISTOGRAM_LIST = otherConfig.SAVE_HISTOGRAM_LIST;
}


// read config file
void CpmgConfig::readConfigFile(const string &applyBulkContent, const string &obsTimeContent, 
                                const string &methodContent, const string &timeVerboseContent, const string &residualFieldContent, 
                                const string &gradientValueContent, const string &gradientDirectionContent, const string &pathToFieldContent, 
                                const string &interpolateFieldContent, const string &minT2Content, const string &maxT2Content, 
                                const string &useT2LogspaceContent, const string &numT2BinsContent, const string &minLambdaContent, 
                                const string &maxLambdaContent, const string &numLambdasContent, const string &pruneNumContent, 
                                const string &noiseAmpContent, const string &saveModeContent, const string &saveT2Content, 
                                const string &saveWalkersContent, const string &saveDecayContent, const string &saveHistogramContent, 
                                const string &saveHistogramListContent)
{
    (*this).readApplyBulk(applyBulkContent);
    (*this).readObservationTime(obsTimeContent);
    (*this).readMethod(methodContent);
    (*this).readTimeVerbose(timeVerboseContent);
    (*this).readResidualField(residualFieldContent);
    (*this).readGradientValue(gradientValueContent);
    (*this).readGradientDirection(gradientDirectionContent);
    (*this).readInterpolateField(interpolateFieldContent);
    (*this).readMinT2(minT2Content);
    (*this).readMaxT2(maxT2Content);
    (*this).readUseT2Logspace(useT2LogspaceContent);
    (*this).readNumT2Bins(numT2BinsContent);
    (*this).readMinLambda(minLambdaContent);
    (*this).readMaxLambda(maxLambdaContent);
    (*this).readNumLambdas(numLambdasContent);
    (*this).readPruneNum(pruneNumContent);
    (*this).readNoiseAmp(noiseAmpContent);
    (*this).readSaveMode(saveModeContent);
    (*this).readSaveT2(saveT2Content);
    (*this).readSaveWalkers(saveWalkersContent);
    (*this).readSaveDecay(saveDecayContent);
    (*this).readSaveHistogram(saveHistogramContent);
    (*this).readSaveHistogramList(saveHistogramListContent);
}

void CpmgConfig::readApplyBulk(string s)
{
    if(s == "true") (*this).setApplyBulk(true);
    else (*this).setApplyBulk(false);
}

void CpmgConfig::readTimeVerbose(string s)
{
    if(s == "true") (*this).setTimeVerbose(true);
    else (*this).setTimeVerbose(false);
}

void CpmgConfig::readObservationTime(string s)
{
    (*this).setObservationTime(std::stod(s));
}

void CpmgConfig::readMethod(string s)
{
    if(s == "histogram") (*this).setMethod(s);
    else (*this).setMethod("image-based");
}

void CpmgConfig::readResidualField(string s)
{
    if(s == "uniform" || s == "import") (*this).setResidualField(s);
    else (*this).setResidualField("none");
}

void CpmgConfig::readGradientValue(string s)
{
    (*this).setGradientValue(std::stod(s));
}

void CpmgConfig::readGradientDirection(string s)
{
    if(s == "0" || s == "x")  (*this).setGradientDirection(0);
    else if(s == "1" || s == "y") (*this).setGradientDirection(1);
    else (*this).setGradientDirection(2);
}


void CpmgConfig::readInterpolateField(string s)
{
    if(s == "true") (*this).setInterpolateField(true);
    else (*this).setInterpolateField(false);
}

void CpmgConfig::readMinT2(string s)
{
    (*this).setMinT2(std::stod(s));
}

void CpmgConfig::readMaxT2(string s)
{
    (*this).setMaxT2(std::stod(s));
}

void CpmgConfig::readUseT2Logspace(string s)
{
    if(s == "true") (*this).setUseT2Logspace(true);
    else (*this).setUseT2Logspace(false);
}

void CpmgConfig::readNumT2Bins(string s)
{
    (*this).setNumT2Bins(std::stoi(s));
}

void CpmgConfig::readMinLambda(string s)
{
    (*this).setMinLambda(std::stod(s));
}

void CpmgConfig::readMaxLambda(string s)
{
    (*this).setMaxLambda(std::stod(s));
}

void CpmgConfig::readNumLambdas(string s)
{   
    (*this).setNumLambdas(std::stoi(s));
}

void CpmgConfig::readPruneNum(string s)
{
    (*this).setPruneNum(std::stoi(s));
}

void CpmgConfig::readNoiseAmp(string s)
{
    (*this).setNoiseAmp(std::stod(s));
}

void CpmgConfig::readSaveMode(string s)
{
    if(s == "true") (*this).setSaveMode(true);
    else (*this).setSaveMode(false);
}

void CpmgConfig::readSaveWalkers(string s)
{
    if(s == "true") (*this).setSaveWalkers(true);
    else (*this).setSaveWalkers(false);
}

void CpmgConfig::readSaveDecay(string s)
{
    if(s == "true") (*this).setSaveDecay(true);
    else (*this).setSaveDecay(false);
}

void CpmgConfig::readSaveHistogram(string s)
{
    if(s == "true") (*this).setSaveHistogram(true);
    else (*this).setSaveHistogram(false);
}

void CpmgConfig::readSaveHistogramList(string s)
{
    if(s == "true") (*this).setSaveHistogramList(true);
    else (*this).setSaveHistogramList(false);
}

void CpmgConfig::readSaveT2(string s)
{
    if(s == "true") (*this).setSaveT2(true);
    else (*this).setSaveT2(false);
}
