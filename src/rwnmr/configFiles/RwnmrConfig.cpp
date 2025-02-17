#include "RwnmrConfig.h"

using namespace std;

// default constructors
RwnmrConfig::RwnmrConfig() : 
    MAP_TIME(0.0),
    MAP_STEPS(0),
    MAP_FILTER(0),
    MAP_TOL(0.01),
    MAP_ITERATIONS(1),
    SAVE_IMG_INFO(false),
    SAVE_BINIMG(false),
    SAVE_WALKERS(false),
    OPENMP_USAGE(true),
    OPENMP_THREADS(4),
    GPU_USAGE(true),
    REDUCE_IN_GPU(true),
    WALKER_SAMPLES(1)
{
}
bool checkItem(bool condition, const string item, vector<string> &mp)
{
    if(!condition) mp.push_back(item);
    return condition;
}   
    
vector<string> RwnmrConfig::checkConfig()
{
    vector<string> missingParameters;
    bool validState = true;

    validState &= checkItem((*this).getWalkers() > 0, (string) "WALKERS", missingParameters);
    validState &= checkItem((*this).getWalkerSamples() > 0, (string) "WALKER_SAMPLES", missingParameters);

    vector<string> placements = {"random", "center", "cubic"};
    validState &= checkItem(std::find(placements.begin(), placements.end(), (*this).getWalkersPlacement()) != placements.end(),
                                    (string) "WALKER_PLACEMENT", missingParameters); 

    vector<string> rhoTypes = {"uniform", "sigmoid"};
    validState &= checkItem(std::find(rhoTypes.begin(), rhoTypes.end(), (*this).getRhoType()) != rhoTypes.end(),
                                    (string) "RHO_TYPE", missingParameters);
    if ((*this).getRhoType() == "uniform")
    {
        validState &= checkItem((*this).getRho().size() == 1, (string) "RHO_VALUE", missingParameters);
    }
    else if ((*this).getRhoType() == "sigmoid")
    {
        validState &= checkItem(((*this).getRho().size() > 0 && (*this).getRho().size() % 4 == 0), (string) "RHO_VALUE", missingParameters);
    }

    validState &= checkItem((*this).getGiromagneticRatio() > 0.0, (string) "GYROMAGNETIC_RATIO", missingParameters);
    vector<string> gUnits = {"mhertz", "rad"};
    validState &= checkItem(std::find(gUnits.begin(), gUnits.end(), (*this).getGiromagneticUnit()) != gUnits.end(),
                                    (string) "GYROMAGNETIC_UNIT", missingParameters);

    validState &= checkItem((*this).getD0() > 0.0, (string) "D0", missingParameters);
    validState &= checkItem((*this).getBulkTime() > 0.0, (string) "BULK_TIME", missingParameters);
    validState &= checkItem((*this).getStepsPerEcho() > 0, (string) "STEPS_PER_ECHO", missingParameters);

    vector<string> bcs = {"periodic", "mirror"};
    validState &= checkItem(std::find(bcs.begin(), bcs.end(), (*this).getBC()) != bcs.end(),
                                    (string) "BC", missingParameters);

    validState &= checkItem((*this).getHistograms() > 0, (string) "HISTOGRAMS", missingParameters);
    validState &= checkItem((*this).getHistogramSize() > 0, (string) "HISTOGRAM_SIZE", missingParameters);
    vector<string> hScales = {"linear", "log"};
    validState &= checkItem(std::find(hScales.begin(), hScales.end(), (*this).getHistogramScale()) != hScales.end(),
                                    (string) "HISTOGRAM_SCALE", missingParameters);

    validState &= checkItem(((*this).getOpenMPUsage() == false || ((*this).getOpenMPUsage() == true && (*this).getOpenMPThreads() > 0)),
                                    (string) "OPENMP", missingParameters);
    validState &= checkItem((*this).getBlocks() > 0, (string) "CUDA_BLOCKS", missingParameters);
    validState &= checkItem((*this).getThreadsPerBlock() > 0, (string) "CUDA_THREADSPERBLOCK", missingParameters);
    validState &= checkItem((*this).getEchoesPerKernel() > 0, (string) "CUDA_ECHOESPERKERNEL", missingParameters);
    validState &= checkItem((*this).getMaxRWSteps() > 0, (string) "CUDA_MAXRWSTEPSPERKERNEL", missingParameters);

    validState &= checkItem((*this).getMapTime() >= 0.0, (string) "MAP_TIME", missingParameters);
    validState &= checkItem((*this).getMapSteps() >= 0, (string) "MAP_STEPS", missingParameters);
    validState &= checkItem((*this).getMapFilter() >= 0.0, (string) "MAP_FILTER", missingParameters);
    validState &= checkItem((*this).getMapTol() >= 0.0, (string) "MAP_TOL", missingParameters);

    (*this).setReady(validState);
    return missingParameters;
}
// read config file
void RwnmrConfig::setConfig(const string &nameContent, const string &walkersContent, const string &walkerSamplesContent,
                                 const string &walkersPlacementContent, const string &placementDeviationContent,
                                 const string &rhoTypeContent, const string &rhoContent, const string &stepsPerEchoContent,
                                 const string &giromagneticRatioContent, const string &giromagneticUnitContent,
                                 const string &d0Content, const string &bulkTimeContent, const string &seedContent,
                                 const string &bcContent, const string &saveImgInfoContent, const string &saveBinImgContent,
                                 const string &saveWalkersContent, const string &histogramsContent,
                                 const string &histogramSizeContent, const string &histogramScaleContent,
                                 const string &mapTimeContent, const string &mapStepsContent, const string &mapFilterContent,
                                 const string &mapTolContent, const string &mapIterationsContent,
                                 const string &openMPUsageContent, const string &openMPThreadsContent,
                                 const string &gpuUsageContent, const string &blocksContent,
                                 const string &threadsPerBlockContent, const string &echoesPerKernelContent,
                                 const string &reduceInGPUContent, const string &maxRWStepsContent)
{
    printf("Reading config file\n");
    (*this).readName(nameContent);
    (*this).readWalkers(walkersContent);
    if (!walkerSamplesContent.empty()) (*this).readWalkerSamples(walkerSamplesContent);
    (*this).readWalkersPlacement(walkersPlacementContent);
    (*this).readPlacementDeviation(placementDeviationContent);
    (*this).readRhoType(rhoTypeContent);
    (*this).readRho(rhoContent);
    (*this).readStepsPerEcho(stepsPerEchoContent);
    (*this).readGiromagneticRatio(giromagneticRatioContent);
    (*this).readGiromagneticUnit(giromagneticUnitContent);
    (*this).readD0(d0Content);
    (*this).readBulkTime(bulkTimeContent);
    (*this).readSeed(seedContent);
    (*this).readBC(bcContent);
    if (!saveImgInfoContent.empty()) (*this).readSaveImgInfo(saveImgInfoContent);
    if (!saveBinImgContent.empty()) (*this).readSaveBinImg(saveBinImgContent);
    if (!saveWalkersContent.empty()) (*this).readSaveWalkers(saveWalkersContent);
    (*this).readHistograms(histogramsContent);
    (*this).readHistogramSize(histogramSizeContent);
    (*this).readHistogramScale(histogramScaleContent);
    if (!mapTimeContent.empty()) (*this).readMapTime(mapTimeContent);
    if (!mapStepsContent.empty()) (*this).readMapSteps(mapStepsContent);
    if (!mapFilterContent.empty()) (*this).readMapFilter(mapFilterContent);
    if (!mapTolContent.empty()) (*this).readMapTol(mapTolContent);
    if (!mapIterationsContent.empty()) (*this).readMapIterations(mapIterationsContent);
    if (!openMPUsageContent.empty()) (*this).readOpenMPUsage(openMPUsageContent);
    if (!openMPThreadsContent.empty()) (*this).readOpenMPThreads(openMPThreadsContent);
    if (!gpuUsageContent.empty()) (*this).readGPUUsage(gpuUsageContent);
    (*this).readBlocks(blocksContent);
    (*this).readThreadsPerBlock(threadsPerBlockContent);
    (*this).readEchoesPerKernel(echoesPerKernelContent);
    if (!reduceInGPUContent.empty()) (*this).readReduceInGPU(reduceInGPUContent);
    (*this).readMaxRWSteps(maxRWStepsContent);
}

void RwnmrConfig::readName(string s)
{
    this->NAME = s;
}

void RwnmrConfig::readWalkers(string s)
{
    this->WALKERS = std::stoi(s);
}

void RwnmrConfig::readWalkerSamples(string s)
{
    this->WALKER_SAMPLES = std::stoi(s);
}

void RwnmrConfig::readWalkersPlacement(string s)
{
    if (s == "point" || s == "cubic")
        this->WALKERS_PLACEMENT = s;
    else
        this->WALKERS_PLACEMENT = "random";
}

void RwnmrConfig::readPlacementDeviation(string s)
{
    this->PLACEMENT_DEVIATION = std::stoi(s);
}

void RwnmrConfig::readRhoType(string s)
{
    if (s == "uniform")
        this->RHO_TYPE = "uniform";
    else if (s == "sigmoid")
        this->RHO_TYPE = "sigmoid";
    else
        this->RHO_TYPE = "undefined";
}

void RwnmrConfig::readRho(string s) // vector?
{
    // parse vector
    if (s.compare(0, 1, "{") == 0 && s.compare(s.length() - 1, 1, "}") == 0)
    {
        string strvec = s.substr(1, s.length() - 2);
        string delimiter = ",";
        size_t pos = 0;
        string token, content;
        while ((pos = strvec.find(delimiter)) != std::string::npos)
        {
            token = strvec.substr(0, pos);
            content = strvec.substr(pos + delimiter.length(), strvec.length());
            strvec.erase(0, pos + delimiter.length());

            // add value to RHO attribute
            this->RHO.push_back(std::stod(token));
        }
        // add value to RHO attribute
        this->RHO.push_back(std::stod(strvec));
    }
    else
    {
        this->RHO.push_back(std::stod(s));
    }
}

void RwnmrConfig::readGiromagneticRatio(string s)
{
    this->GIROMAGNETIC_RATIO = std::stod(s);
}

void RwnmrConfig::readGiromagneticUnit(string s)
{
    if (s == "MHertz" || s == "mhertz")
        this->GIROMAGNETIC_UNIT = "mhertz";
    else
        this->GIROMAGNETIC_UNIT = "rad";
}

void RwnmrConfig::readD0(string s)
{
    this->D0 = std::stod(s);
}

void RwnmrConfig::readBulkTime(string s)
{
    this->BULK_TIME = std::stod(s);
}

void RwnmrConfig::readStepsPerEcho(string s)
{
    this->STEPS_PER_ECHO = std::stoi(s);
}

void RwnmrConfig::readSeed(string s)
{
    this->SEED = std::stol(s);
}

void RwnmrConfig::readBC(string s)
{
    if (s == "mirror")
    {
        this->BC = "mirror";
    }
    else if (s == "debug" || s == "test")
    {
        this->BC = "test";
    }
    else
        this->BC = "periodic";
}

// -- Saving
void RwnmrConfig::readSaveImgInfo(string s)
{
    if (s == "true")
        this->SAVE_IMG_INFO = true;
    else
        this->SAVE_IMG_INFO = false;
}

void RwnmrConfig::readSaveBinImg(string s)
{
    if (s == "true")
        this->SAVE_BINIMG = true;
    else
        this->SAVE_BINIMG = false;
}

void RwnmrConfig::readSaveWalkers(string s)
{
    if (s == "true")
        this->SAVE_WALKERS = true;
    else
        this->SAVE_WALKERS = false;
}

// Histograms
void RwnmrConfig::readHistograms(string s)
{
    this->HISTOGRAMS = std::stoi(s);
}

void RwnmrConfig::readHistogramSize(string s)
{
    this->HISTOGRAM_SIZE = std::stoi(s);
}

void RwnmrConfig::readHistogramScale(string s)
{
    if (s == "log")
        this->HISTOGRAM_SCALE = "log";
    else
        this->HISTOGRAM_SCALE = "linear";
}

void RwnmrConfig::readMapTime(string s)
{
    this->MAP_TIME = std::stod(s);
}

void RwnmrConfig::readMapSteps(string s)
{
    this->MAP_STEPS = std::stoi(s);
}

void RwnmrConfig::readMapFilter(string s)
{
    this->MAP_FILTER = std::stod(s);
}

void RwnmrConfig::readMapTol(string s)
{
    this->MAP_TOL = std::stod(s);
}

void RwnmrConfig::readMapIterations(string s)
{
    this->MAP_ITERATIONS = std::stoi(s);
}

// -- OpenMP
void RwnmrConfig::readOpenMPUsage(string s)
{
    if (s == "true")
        this->OPENMP_USAGE = true;
    else
        this->OPENMP_USAGE = false;
}

void RwnmrConfig::readOpenMPThreads(string s)
{
    this->OPENMP_THREADS = std::stoi(s);
}

// -- CUDA/GPU Params
void RwnmrConfig::readGPUUsage(string s)
{
    if (s == "true")
        this->GPU_USAGE = true;
    else
        this->GPU_USAGE = false;
}

void RwnmrConfig::readBlocks(string s)
{
    this->BLOCKS = std::stoi(s);
}

void RwnmrConfig::readThreadsPerBlock(string s)
{
    this->THREADSPERBLOCK = std::stoi(s);
}

void RwnmrConfig::readEchoesPerKernel(string s)
{
    this->ECHOESPERKERNEL = std::stoi(s);
}

void RwnmrConfig::readMaxRWSteps(string s)
{
    this->MAX_RWSTEPS = std::stoi(s);
}

void RwnmrConfig::readReduceInGPU(string s)
{
    if (s == "true")
        this->REDUCE_IN_GPU = true;
    else
        this->REDUCE_IN_GPU = false;
}
