#ifndef CPMG_CONFIG_H_
#define CPMG_CONFIG_H_

#include "configFiles_defs.h"
#include "BaseConfig.h"

using namespace std;

class CpmgConfig 
{
private:
    bool APPLY_BULK;
    double OBS_TIME;
    string METHOD;
    bool TIME_VERBOSE;
    string RESIDUAL_FIELD;
    double GRADIENT_VALUE;
    int GRADIENT_DIRECTION;
    string PATH_TO_FIELD;
    bool INTERPOLATE_FIELD;
    double MIN_T2;
    double MAX_T2;
    bool USE_T2_LOGSPACE;
    int NUM_T2_BINS;
    double MIN_LAMBDA;
    double MAX_LAMBDA;
    int NUM_LAMBDAS;
    int PRUNE_NUM;
    double NOISE_AMP;
    bool SAVE_MODE;
    bool SAVE_T2;
    bool SAVE_WALKERS;
    bool SAVE_DECAY;
    bool SAVE_HISTOGRAM;
    bool SAVE_HISTOGRAM_LIST;
    bool ready;

public:
    // Constructors
    CpmgConfig();
    CpmgConfig(const CpmgConfig &otherConfig);

    // Methods
    vector<string> checkConfig();
    void setConfig(const string &applyBulkContent, const string &obsTimeContent, 
                        const string &methodContent, const string &timeVerboseContent, const string &residualFieldContent, 
                        const string &gradientValueContent, const string &gradientDirectionContent, 
                        const string &interpolateFieldContent, const string &minT2Content, const string &maxT2Content, 
                        const string &useT2LogspaceContent, const string &numT2BinsContent, const string &minLambdaContent, 
                        const string &maxLambdaContent, const string &numLambdasContent, const string &pruneNumContent, 
                        const string &noiseAmpContent, const string &saveModeContent, const string &saveT2Content, 
                        const string &saveWalkersContent, const string &saveDecayContent, const string &saveHistogramContent, 
                        const string &saveHistogramListContent);

    void readApplyBulk(string s);
    void readTimeVerbose(string s);
    void readObservationTime(string s);
    void readMethod(string s);
    void readResidualField(string s);
    void readGradientValue(string s);
    void readGradientDirection(string s);
    void readPathToField(string s);
    void readInterpolateField(string s);
    void readMinT2(string s);
    void readMaxT2(string s);
    void readUseT2Logspace(string s);
    void readNumT2Bins(string s);
    void readMinLambda(string s);
    void readMaxLambda(string s);
    void readNumLambdas(string s);
    void readPruneNum(string s);
    void readNoiseAmp(string s);
    void readSaveMode(string s);
    void readSaveWalkers(string s);
    void readSaveDecay(string s);
    void readSaveHistogram(string s);
    void readSaveHistogramList(string s);
    void readSaveT2(string s);

    // Set methods
    void setReady(bool _b) { this->ready = _b; }
    bool getReady() { return this->ready; }
    void setApplyBulk(bool s) { this->APPLY_BULK = s; }
    void setObservationTime(double s) { this->OBS_TIME = s; }
    void setMethod(string s) { this->METHOD = s; }
    void setTimeVerbose(bool s) { this->TIME_VERBOSE = s; }
    void setResidualField(string s) { this->RESIDUAL_FIELD = s; }
    void setGradientValue(double s) { this->GRADIENT_VALUE = s; }
    void setGradientDirection(int s) { this->GRADIENT_DIRECTION = s; }
    void setPathToField(string s) { this->PATH_TO_FIELD = s; }
    void setInterpolateField(bool s) { this->INTERPOLATE_FIELD = s; }
    void setMinT2(double s) { this->MIN_T2 = s; }
    void setMaxT2(double s) { this->MAX_T2 = s; }
    void setUseT2Logspace(bool s) { this->USE_T2_LOGSPACE = s; }
    void setNumT2Bins(int s) { this->NUM_T2_BINS = s; }
    void setMinLambda(double s) { this->MIN_LAMBDA = s; }
    void setMaxLambda(double s) { this->MAX_LAMBDA = s; }
    void setNumLambdas(int s) { this->NUM_LAMBDAS = s; }
    void setPruneNum(int s) { this->PRUNE_NUM = s; }
    void setNoiseAmp(double s) { this->NOISE_AMP = s; }
    void setSaveMode(bool s) { this->SAVE_MODE = s; }
    void setSaveT2(bool s) { this->SAVE_T2 = s; }
    void setSaveWalkers(bool s) { this->SAVE_WALKERS = s; }
    void setSaveDecay(bool s) { this->SAVE_DECAY = s; }
    void setSaveHistogram(bool s) { this->SAVE_HISTOGRAM = s; }
    void setSaveHistogramList(bool s) { this->SAVE_HISTOGRAM_LIST = s; }

    // Get methods
    bool getApplyBulk() { return this->APPLY_BULK; }
    double getObservationTime() { return this->OBS_TIME; }
    string getMethod() { return this->METHOD; }
    bool getTimeVerbose() { return this->TIME_VERBOSE; }
    string getResidualField() { return this->RESIDUAL_FIELD; }
    double getGradientValue() { return this->GRADIENT_VALUE; }
    int getGradientDirection() { return this->GRADIENT_DIRECTION; }
    string getPathToField() { return this->PATH_TO_FIELD; }
    bool getInterpolateField() { return this->INTERPOLATE_FIELD; }
    double getMinT2() { return this->MIN_T2; }
    double getMaxT2() { return this->MAX_T2; }
    bool getUseT2Logspace() { return this->USE_T2_LOGSPACE; }
    int getNumT2Bins() { return this->NUM_T2_BINS; }
    double getMinLambda() { return this->MIN_LAMBDA; }
    double getMaxLambda() { return this->MAX_LAMBDA; }
    int getNumLambdas() { return this->NUM_LAMBDAS; }
    int getPruneNum() { return this->PRUNE_NUM; }
    double getNoiseAmp() { return this->NOISE_AMP; }
    bool getSaveMode() { return this->SAVE_MODE; }
    bool getSaveT2() { return this->SAVE_T2; }
    bool getSaveWalkers() { return this->SAVE_WALKERS; }
    bool getSaveDecay() { return this->SAVE_DECAY; }
    bool getSaveHistogram() { return this->SAVE_HISTOGRAM; }
    bool getSaveHistogramList() { return this->SAVE_HISTOGRAM_LIST; }
};

#endif // CPMG_CONFIG_H_