#include "Python.h"

#include <stdlib.h> 
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>
#include <iostream>
#include "../configFiles/CpmgConfig.h"
#include "../configFiles/RwnmrConfig.h"
#include "../configFiles/UctConfig.h"
#include "./BitBlock.h"
#include "./rwnmrApp.h"
static PyObject* metodo1(PyObject* self, PyObject* args){

    PyArrayObject *np_array;


    if (!PyArg_ParseTuple(args, "O", &np_array)) {
        return NULL; 
    }else{
    uint8_t* printar = (uint8_t*)PyArray_DATA(np_array);
    npy_intp size = PyArray_SIZE(np_array);
    uint8_t total=0;
    for (npy_intp i = 0; i < size; i++) {
        total += printar[i];
    }
    Py_DECREF(np_array);
    return PyLong_FromUnsignedLongLong((unsigned long long)total);
    }
};

static PyObject* create_and_return_array(PyObject* self, PyObject* args) {
    std::vector<double> signalTimes = {0.1, 0.2, 0.3, 0.4};
    std::vector<double> signalAmps = {1.0, 0.8, 0.6, 0.4};

    // Define the size of the arrays
    npy_intp size = signalTimes.size();

    // Create a new NumPy array for signalTimes
    PyObject* np_signalTimes = PyArray_SimpleNew(1, &size, NPY_DOUBLE);
    if (!np_signalTimes) {
        return NULL;  // Return NULL if array creation fails
    }

    // Copy data from signalTimes to the NumPy array
    double* signalTimes_data = static_cast<double*>(PyArray_DATA((PyArrayObject*)np_signalTimes));
    std::copy(signalTimes.begin(), signalTimes.end(), signalTimes_data);

    // Create a new NumPy array for signalAmps
    PyObject* np_signalAmps = PyArray_SimpleNew(1, &size, NPY_DOUBLE);
    if (!np_signalAmps) {
        Py_DECREF(np_signalTimes);  // Clean up if second array creation fails
        return NULL;
    }

    // Copy data from signalAmps to the NumPy array
    double* signalAmps_data = static_cast<double*>(PyArray_DATA((PyArrayObject*)np_signalAmps));
    std::copy(signalAmps.begin(), signalAmps.end(), signalAmps_data);

    // Return both arrays as a tuple
    return PyTuple_Pack(2, np_signalTimes, np_signalAmps);
}

static PyObject* helloworld(PyObject* self, PyObject* args){
    PyObject* texto;
    if(!PyArg_ParseTuple(args, "U", &texto)){
        Py_DecRef(texto);
        return NULL;
    }
    PyObject* ret = PyUnicode_FromFormat("Hello from %U!", texto);
    Py_DecRef(texto);
    return ret;
};
CpmgConfig CPMG(PyObject* CPMG_object){

    CpmgConfig cpmg_config;

    PyObject* applyBulk = PyObject_GetAttrString(CPMG_object, "apply_bulk");
    PyObject* obsTime = PyObject_GetAttrString(CPMG_object, "obs_time");
    PyObject* method = PyObject_GetAttrString(CPMG_object, "method");
    PyObject* timeVerbose = PyObject_GetAttrString(CPMG_object, "time_verbose");
    PyObject* residualField = PyObject_GetAttrString(CPMG_object, "residual_field");
    PyObject* gradientValue = PyObject_GetAttrString(CPMG_object, "gradient_value");
    PyObject* gradientDirection = PyObject_GetAttrString(CPMG_object, "gradient_direction");
    PyObject* interpolateField = PyObject_GetAttrString(CPMG_object, "interpolate_field");
    PyObject* minT2 = PyObject_GetAttrString(CPMG_object, "min_t2");
    PyObject* maxT2 = PyObject_GetAttrString(CPMG_object, "max_t2");
    PyObject* useT2Logspace = PyObject_GetAttrString(CPMG_object, "use_t2_logspace");
    PyObject* numT2Bins = PyObject_GetAttrString(CPMG_object, "num_t2_bins");
    PyObject* minLambda = PyObject_GetAttrString(CPMG_object, "min_lambda");
    PyObject* maxLambda = PyObject_GetAttrString(CPMG_object, "max_lambda");
    PyObject* numLambdas = PyObject_GetAttrString(CPMG_object, "num_lambdas");
    PyObject* pruneNum = PyObject_GetAttrString(CPMG_object, "prune_num");
    PyObject* noiseAmp = PyObject_GetAttrString(CPMG_object, "noise_amp");
    PyObject* saveMode = PyObject_GetAttrString(CPMG_object, "save_mode");
    PyObject* saveT2 = PyObject_GetAttrString(CPMG_object, "save_t2");
    PyObject* saveWalkers = PyObject_GetAttrString(CPMG_object, "save_walkers");
    PyObject* saveDecay = PyObject_GetAttrString(CPMG_object, "save_decay");
    PyObject* saveHistogram = PyObject_GetAttrString(CPMG_object, "save_histogram");
    PyObject* saveHistogramList = PyObject_GetAttrString(CPMG_object, "save_histogram_list");

    if (applyBulk == NULL || obsTime == NULL || method == NULL || timeVerbose == NULL || residualField == NULL ||
        gradientValue == NULL || gradientDirection == NULL || interpolateField == NULL ||
        minT2 == NULL || maxT2 == NULL || useT2Logspace == NULL || numT2Bins == NULL || minLambda == NULL ||
        maxLambda == NULL || numLambdas == NULL || pruneNum == NULL || noiseAmp == NULL || saveMode == NULL ||
        saveT2 == NULL || saveWalkers == NULL || saveDecay == NULL || saveHistogram == NULL || saveHistogramList == NULL) {
        Py_XDECREF(applyBulk);
        Py_XDECREF(obsTime);
        Py_XDECREF(method);
        Py_XDECREF(timeVerbose);
        Py_XDECREF(residualField);
        Py_XDECREF(gradientValue);
        Py_XDECREF(gradientDirection);
        Py_XDECREF(interpolateField);
        Py_XDECREF(minT2);
        Py_XDECREF(maxT2);
        Py_XDECREF(useT2Logspace);
        Py_XDECREF(numT2Bins);
        Py_XDECREF(minLambda);
        Py_XDECREF(maxLambda);
        Py_XDECREF(numLambdas);
        Py_XDECREF(pruneNum);
        Py_XDECREF(noiseAmp);
        Py_XDECREF(saveMode);
        Py_XDECREF(saveT2);
        Py_XDECREF(saveWalkers);
        Py_XDECREF(saveDecay);
        Py_XDECREF(saveHistogram);
        Py_XDECREF(saveHistogramList);
        return CpmgConfig();
    }

    cpmg_config.setConfig(
        PyUnicode_AsUTF8(applyBulk),
        PyUnicode_AsUTF8(obsTime),
        PyUnicode_AsUTF8(method),
        PyUnicode_AsUTF8(timeVerbose),
        PyUnicode_AsUTF8(residualField),
        PyUnicode_AsUTF8(gradientValue),
        PyUnicode_AsUTF8(gradientDirection),
        PyUnicode_AsUTF8(interpolateField),
        PyUnicode_AsUTF8(minT2),
        PyUnicode_AsUTF8(maxT2),
        PyUnicode_AsUTF8(useT2Logspace),
        PyUnicode_AsUTF8(numT2Bins),
        PyUnicode_AsUTF8(minLambda),
        PyUnicode_AsUTF8(maxLambda),
        PyUnicode_AsUTF8(numLambdas),
        PyUnicode_AsUTF8(pruneNum),
        PyUnicode_AsUTF8(noiseAmp),
        PyUnicode_AsUTF8(saveMode),
        PyUnicode_AsUTF8(saveT2),
        PyUnicode_AsUTF8(saveWalkers),
        PyUnicode_AsUTF8(saveDecay),
        PyUnicode_AsUTF8(saveHistogram),
        PyUnicode_AsUTF8(saveHistogramList)
    );

    return cpmg_config;
    
}
static PyObject *recebe_objeto_classe(PyObject *self, PyObject *args) {
    PyObject *obj_python;
    PyObject *classe_python;
    if (!PyArg_ParseTuple(args, "OO", &obj_python, &classe_python)) {
        return NULL; // Erro: argumentos incorretos
    }

    // Verificando se obj_python é uma instância de classe_python
    if (PyObject_IsInstance(obj_python, classe_python)) {
        // Exemplo: acessando um atributo do objeto
        PyObject *nome_attr = PyObject_GetAttrString(obj_python, "apply_bulk");
        if (nome_attr != NULL) {
            if (PyUnicode_Check(nome_attr)) {
                const char *nome = PyUnicode_AsUTF8(nome_attr);
                PyObject *result = PyUnicode_FromFormat("Nome do objeto: %s", nome);
                Py_DECREF(nome_attr);
                return result;
            }
            Py_DECREF(nome_attr);
        }
        Py_RETURN_NONE;
    } else {
        return PyUnicode_FromString("Objeto Python recebido NÃO é uma instância da classe esperada.");
    }

    Py_RETURN_NONE;
}
RwnmrConfig RWNMR(PyObject* RWNMR_object) {
    
    printf("Entrou no RWNMR\n");
    RwnmrConfig rwnmr_config;
    PyObject* walkersContent = PyObject_GetAttrString(RWNMR_object, "walkers");
    PyObject* nameContent = PyObject_GetAttrString(RWNMR_object, "name");
    PyObject* walkerSamplesContent = PyObject_GetAttrString(RWNMR_object, "walker_samples");
    PyObject* walkersPlacementContent = PyObject_GetAttrString(RWNMR_object, "walkers_placement");
    PyObject* placementDeviationContent = PyObject_GetAttrString(RWNMR_object, "placement_deviation");
    PyObject* rhoTypeContent = PyObject_GetAttrString(RWNMR_object, "rho_type");
    PyObject* rhoContent = PyObject_GetAttrString(RWNMR_object, "rho");
    PyObject* stepsPerEchoContent = PyObject_GetAttrString(RWNMR_object, "steps_per_echo");
    PyObject* giromagneticRatioContent = PyObject_GetAttrString(RWNMR_object, "giromagnetic_ratio");
    PyObject* giromagneticUnitContent = PyObject_GetAttrString(RWNMR_object, "giromagnetic_unit");
    PyObject* d0Content = PyObject_GetAttrString(RWNMR_object, "d0");
    PyObject* bulkTimeContent = PyObject_GetAttrString(RWNMR_object, "bulk_time");
    PyObject* seedContent = PyObject_GetAttrString(RWNMR_object, "seed");
    PyObject* bcContent = PyObject_GetAttrString(RWNMR_object, "bc");
    PyObject* mapStepsContent = PyObject_GetAttrString(RWNMR_object, "map_steps");
    PyObject* saveImgInfoContent = PyObject_GetAttrString(RWNMR_object, "save_img_info");
    PyObject* saveBinImgContent = PyObject_GetAttrString(RWNMR_object, "save_binimg");
    PyObject* saveWalkersContent = PyObject_GetAttrString(RWNMR_object, "save_walkers");
    PyObject* histogramsContent = PyObject_GetAttrString(RWNMR_object, "histograms");
    PyObject* histogramSizeContent = PyObject_GetAttrString(RWNMR_object, "histogram_size");
    PyObject* histogramScaleContent = PyObject_GetAttrString(RWNMR_object, "histogram_scale");
    PyObject* mapTimeContent = PyObject_GetAttrString(RWNMR_object, "map_time");
    PyObject* mapFilterContent = PyObject_GetAttrString(RWNMR_object, "map_filter");
    PyObject* mapTolContent = PyObject_GetAttrString(RWNMR_object, "map_tol");
    PyObject* mapIterationsContent = PyObject_GetAttrString(RWNMR_object, "map_iterations");
    PyObject* openMPUsageContent = PyObject_GetAttrString(RWNMR_object, "openmp_usage");
    PyObject* openMPThreadsContent = PyObject_GetAttrString(RWNMR_object, "openmp_threads");
    PyObject* gpuUsageContent = PyObject_GetAttrString(RWNMR_object, "gpu_usage");
    PyObject* blocksContent = PyObject_GetAttrString(RWNMR_object, "blocks");
    PyObject* threadsPerBlockContent = PyObject_GetAttrString(RWNMR_object, "threads_per_block");
    PyObject* echoesPerKernelContent = PyObject_GetAttrString(RWNMR_object, "echoes_per_kernel");
    PyObject* reduceInGPUContent = PyObject_GetAttrString(RWNMR_object, "reduce_in_gpu");
    PyObject* maxRWStepsContent = PyObject_GetAttrString(RWNMR_object, "max_rwsteps");

    printf("passou os paarametros\n");
if (nameContent == NULL || walkersContent == NULL || walkersPlacementContent == NULL ||
        placementDeviationContent == NULL || rhoTypeContent == NULL || rhoContent == NULL || stepsPerEchoContent == NULL ||
        giromagneticRatioContent == NULL || giromagneticUnitContent == NULL || d0Content == NULL || bulkTimeContent == NULL ||
        seedContent == NULL || bcContent == NULL || histogramsContent == NULL || histogramSizeContent == NULL || histogramScaleContent == NULL ||
        blocksContent == NULL || threadsPerBlockContent == NULL || echoesPerKernelContent == NULL || maxRWStepsContent == NULL) {
    // Handle error
    printf("Erro ao pegar os parametros\n");
        if (nameContent == NULL) printf("nameContent is NULL\n");
        if (walkersContent == NULL) printf("walkersContent is NULL\n");
        if (walkerSamplesContent == NULL) printf("walkerSamplesContent is NULL\n");
        if (walkersPlacementContent == NULL) printf("walkersPlacementContent is NULL\n");
        if (placementDeviationContent == NULL) printf("placementDeviationContent is NULL\n");
        if (rhoTypeContent == NULL) printf("rhoTypeContent is NULL\n");
        if (rhoContent == NULL) printf("rhoContent is NULL\n");
        if (stepsPerEchoContent == NULL) printf("stepsPerEchoContent is NULL\n");
        if (giromagneticRatioContent == NULL) printf("giromagneticRatioContent is NULL\n");
        if (giromagneticUnitContent == NULL) printf("giromagneticUnitContent is NULL\n");
        if (d0Content == NULL) printf("d0Content is NULL\n");
        if (bulkTimeContent == NULL) printf("bulkTimeContent is NULL\n");
        if (seedContent == NULL) printf("seedContent is NULL\n");
        if (bcContent == NULL) printf("bcContent is NULL\n");
        if (mapStepsContent == NULL) printf("mapStepsContent is NULL\n");
        if (saveImgInfoContent == NULL) printf("saveImgInfoContent is NULL\n");
        if (saveBinImgContent == NULL) printf("saveBinImgContent is NULL\n");
        if (saveWalkersContent == NULL) printf("saveWalkersContent is NULL\n");
        if (histogramsContent == NULL) printf("histogramsContent is NULL\n");
        if (histogramSizeContent == NULL) printf("histogramSizeContent is NULL\n");
        if (histogramScaleContent == NULL) printf("histogramScaleContent is NULL\n");
        if (mapTimeContent == NULL) printf("mapTimeContent is NULL\n");
        if (mapFilterContent == NULL) printf("mapFilterContent is NULL\n");
        if (mapTolContent == NULL) printf("mapTolContent is NULL\n");
        if (mapIterationsContent == NULL) printf("mapIterationsContent is NULL\n");
        if (openMPUsageContent == NULL) printf("openMPUsageContent is NULL\n");
        if (openMPThreadsContent == NULL) printf("openMPThreadsContent is NULL\n");
        if (gpuUsageContent == NULL) printf("gpuUsageContent is NULL\n");
        if (blocksContent == NULL) printf("blocksContent is NULL\n");
        if (threadsPerBlockContent == NULL) printf("threadsPerBlockContent is NULL\n");
        if (echoesPerKernelContent == NULL) printf("echoesPerKernelContent is NULL\n");
        if (reduceInGPUContent == NULL) printf("reduceInGPUContent is NULL\n");
        if (maxRWStepsContent == NULL) printf("maxRWStepsContent is NULL\n");
        
        return RwnmrConfig(); 
    }
    rwnmr_config.readName(PyUnicode_AsUTF8(nameContent));
    rwnmr_config.readWalkers(PyUnicode_AsUTF8(walkersContent));
    rwnmr_config.readWalkerSamples(PyUnicode_AsUTF8(walkerSamplesContent));
    rwnmr_config.readWalkersPlacement(PyUnicode_AsUTF8(walkersPlacementContent));
    rwnmr_config.readPlacementDeviation(PyUnicode_AsUTF8(placementDeviationContent));
    rwnmr_config.readRhoType(PyUnicode_AsUTF8(rhoTypeContent));
    rwnmr_config.readRho(PyUnicode_AsUTF8(rhoContent));
    rwnmr_config.readStepsPerEcho(PyUnicode_AsUTF8(stepsPerEchoContent));
    rwnmr_config.readGiromagneticRatio(PyUnicode_AsUTF8(giromagneticRatioContent));
    rwnmr_config.readGiromagneticUnit(PyUnicode_AsUTF8(giromagneticUnitContent));
    rwnmr_config.readD0(PyUnicode_AsUTF8(d0Content));
    rwnmr_config.readBulkTime(PyUnicode_AsUTF8(bulkTimeContent));
    rwnmr_config.readSeed(PyUnicode_AsUTF8(seedContent));
    rwnmr_config.readBC(PyUnicode_AsUTF8(bcContent));
    rwnmr_config.readMapSteps(PyUnicode_AsUTF8(mapStepsContent));
    rwnmr_config.readSaveImgInfo(PyUnicode_AsUTF8(saveImgInfoContent));
    rwnmr_config.readSaveBinImg(PyUnicode_AsUTF8(saveBinImgContent));
    rwnmr_config.readSaveWalkers(PyUnicode_AsUTF8(saveWalkersContent));
    rwnmr_config.readHistograms(PyUnicode_AsUTF8(histogramsContent));
    rwnmr_config.readHistogramSize(PyUnicode_AsUTF8(histogramSizeContent));
    rwnmr_config.readHistogramScale(PyUnicode_AsUTF8(histogramScaleContent));
    rwnmr_config.readMapTime(PyUnicode_AsUTF8(mapTimeContent));
    rwnmr_config.readMapFilter(PyUnicode_AsUTF8(mapFilterContent));
    rwnmr_config.readMapTol(PyUnicode_AsUTF8(mapTolContent));
    rwnmr_config.readMapIterations(PyUnicode_AsUTF8(mapIterationsContent));
    rwnmr_config.readOpenMPUsage(PyUnicode_AsUTF8(openMPUsageContent));
    rwnmr_config.readOpenMPThreads(PyUnicode_AsUTF8(openMPThreadsContent));
    rwnmr_config.readGPUUsage(PyUnicode_AsUTF8(gpuUsageContent));
    rwnmr_config.readBlocks(PyUnicode_AsUTF8(blocksContent));
    rwnmr_config.readThreadsPerBlock(PyUnicode_AsUTF8(threadsPerBlockContent));
    rwnmr_config.readEchoesPerKernel(PyUnicode_AsUTF8(echoesPerKernelContent));
    rwnmr_config.readReduceInGPU(PyUnicode_AsUTF8(reduceInGPUContent));
    rwnmr_config.readMaxRWSteps(PyUnicode_AsUTF8(maxRWStepsContent));
  
    printf("leu os parametros\n");
    Py_DECREF(nameContent);
    Py_DECREF(walkersContent);
    Py_DECREF(walkerSamplesContent);
    Py_DECREF(walkersPlacementContent);
    Py_DECREF(placementDeviationContent);
    Py_DECREF(rhoTypeContent);
    Py_DECREF(rhoContent);
    Py_DECREF(stepsPerEchoContent);
    Py_DECREF(giromagneticRatioContent);
    Py_DECREF(giromagneticUnitContent);
    Py_DECREF(d0Content);
    Py_DECREF(bulkTimeContent);
    Py_DECREF(seedContent);
    Py_DECREF(bcContent);
    Py_DECREF(mapStepsContent);
    Py_DECREF(saveImgInfoContent);
    Py_DECREF(saveBinImgContent);
    Py_DECREF(saveWalkersContent);
    Py_DECREF(histogramsContent);
    Py_DECREF(histogramSizeContent);
    Py_DECREF(histogramScaleContent);
    Py_DECREF(mapTimeContent);
    Py_DECREF(mapFilterContent);
    Py_DECREF(mapTolContent);
    Py_DECREF(mapIterationsContent);
    Py_DECREF(openMPUsageContent);
    Py_DECREF(openMPThreadsContent);
    Py_DECREF(gpuUsageContent);
    Py_DECREF(blocksContent);
    Py_DECREF(threadsPerBlockContent);
    Py_DECREF(echoesPerKernelContent);
    Py_DECREF(reduceInGPUContent);
    Py_DECREF(maxRWStepsContent);
    
    return rwnmr_config;
}

static PyObject* BitBlockMethod(PyObject* self, PyObject* args){
    PyArrayObject *np_array;


    int rows;
    int cols;
    int depth;

    if (!PyArg_ParseTuple(args, "Oiii", &np_array, &depth, &rows, &cols)) {
        return NULL; 
    }else{

    uint8_t* dados = (uint8_t*)PyArray_DATA(np_array);
    for (npy_intp i = 0; i < PyArray_SIZE(np_array); ++i) {
        printf("%i ", dados[i]);
    }
    std::vector<CustomMat> binaryMap;

    for (int d = 0; d < depth; ++d) {
        uint8_t* slice_data = dados + (d * rows * cols);
        std::vector<uint8_t> vec_data(slice_data, slice_data + (rows * cols));
        CustomMat data = CustomMat(rows, cols, vec_data);
        binaryMap.push_back(data);
    }
    for (int d = 0; d < depth; ++d) {
        CustomMat mat = binaryMap[d];
        for (int i = 0; i < mat.getRows(); ++i) {
            for (int j = 0; j < mat.getCols(); ++j) {
                std::cout << static_cast<int>(mat.getData()[i * mat.getCols() + j]) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // std::vector<uint8_t> vec_data(dados, dados + (rows * cols));
    // CustomMat data(rows, cols, vec_data);
    // std::vector<CustomMat> binaryMap = {data};

    CustomMat data = CustomMat(rows, cols, std::vector<uint8_t>(dados, dados + (rows * cols)));
    std::vector<CustomMat> binaryMap2 = {data};

    BitBlock bitBlock;
    BitBlock bitBlock2;
    // bitBlock.createBlockMap(binaryMap, 0);
    // bitBlock2.createBlockMap(binaryMap2, 0);
    printf("rows: %i\n cols: %i \n depth: %i \n", rows,cols, depth);
    bitBlock2.saveBitBlockArray_2D("bitblock2D.txt");
    bitBlock.saveBitBlockArray_3D("bitblock3D.txt");
    Py_DECREF(np_array);
    }
    return PyUnicode_FromString("BitBlockMethod");

}

UctConfig UCT(PyObject* UCT_object){
    UctConfig uct_config;
    
    PyObject* firstIdxContent = PyObject_GetAttrString(UCT_object, "first_idx");
    PyObject* digitsContent = PyObject_GetAttrString(UCT_object, "digits");
    PyObject* extensionContent = PyObject_GetAttrString(UCT_object, "extension");
    PyObject* slicesContent = PyObject_GetAttrString(UCT_object, "slices");
    PyObject* resolutionContent = PyObject_GetAttrString(UCT_object, "resolution");
    PyObject* voxelDivisionContent = PyObject_GetAttrString(UCT_object, "voxel_division");
    PyObject* poreColorContent = PyObject_GetAttrString(UCT_object, "pore_color");

    if (firstIdxContent == NULL || digitsContent == NULL || extensionContent == NULL || slicesContent == NULL ||
        resolutionContent == NULL || voxelDivisionContent == NULL || poreColorContent == NULL) {
        Py_XDECREF(firstIdxContent);
        Py_XDECREF(digitsContent);
        Py_XDECREF(extensionContent);
        Py_XDECREF(slicesContent);
        Py_XDECREF(resolutionContent);
        Py_XDECREF(voxelDivisionContent);
        Py_XDECREF(poreColorContent);
        return UctConfig();
    }
    uct_config.readDigits(PyUnicode_AsUTF8(digitsContent));
    uct_config.readExtension(PyUnicode_AsUTF8(extensionContent));
    uct_config.readSlices(PyUnicode_AsUTF8(slicesContent));
    uct_config.readResolution(PyUnicode_AsUTF8(resolutionContent));
    uct_config.readVoxelDivision(PyUnicode_AsUTF8(voxelDivisionContent));
    uct_config.readPoreColor(PyUnicode_AsUTF8(poreColorContent));
    uct_config.readFirstIdx(PyUnicode_AsUTF8(firstIdxContent));

    return uct_config;
};

PyObject* vector_to_pylist(const std::vector<double>& vec) {
    PyObject* list = PyList_New(vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
        PyList_SetItem(list, i, PyFloat_FromDouble(vec[i]));
        // Note: PyList_SetItem steals reference, so no need to DECREF
    }
    return list;
}


// (ponteiro binmap, pontei vetor blockmap, int depth, int rows, int cols, int* blockSize, int* blockSize2, int* blockSize3)
// uint8_t* binmap;
// uint64_t* blockMap;
// uint32_t* depth;
// uint32_t* rows; uint32_t* cols,  uint32_t* bDepth, uint32_t* blockRows, uint32_t* blockCols;

uint64_t array[8 * 8 * 8] = {0};




uint32_t blockMap(uint8_t* binmap, uint64_t* blockMap, uint32_t depth, uint32_t rows, uint32_t cols,  uint32_t bDepth, uint32_t bRow, uint32_t bCols){
    printf("entrouuu Block Map:\n");
    int totalBlocks = bDepth * bRow * bCols; 
    int count = 0;
    printf("totalBlocks: %d\n", totalBlocks);
    for(int blockId = 0; blockId < totalBlocks; blockId++){
        // printf("Block ID: %d\n", blockId);

        int blockMapz = blockId / (bCols*bRow);
        int blockMapy = (blockId / bCols) % bRow;
        int blockMapx = blockId % bCols;
        for(int z = 0; z < 4; z++){
            for(int y = 0; y < 4; y++){
                for(int x = 0; x < 4; x++){
                    int imx = blockMapx*4 + x;
                    int imy = blockMapy*4 + y;
                    int imz = blockMapz*4 + z;

                    int img_id = imx + (imy* cols) + (imz * cols * rows);

                    int color = binmap[img_id];
                    // printf("Block ID: %d, Color: %d, img_id: %d\n", blockId, color, img_id);
                    blockMap[blockId] = (blockMap[blockId] << 1) + color;
                    if (color == 0) count++;

                }
            }
        }
            
    }
    return count;

}

static PyObject* CPMG_EXECUTE(PyObject* self, PyObject* args){
    PyObject* CPMG_object;
    PyObject* RWNMR_object;
    PyObject* UCT_object;
    PyObject* image_input_obj;
    int rows;
    int cols;
    int depth;
    if(!PyArg_ParseTuple(args, "OOOOiii", &CPMG_object, &RWNMR_object, &UCT_object, &image_input_obj, &depth, &rows, &cols)){
        return NULL;
    }
    //Image retrieval
    PyArrayObject* image_array = (PyArrayObject*) PyArray_FROM_OTF(image_input_obj, NPY_UINT8, NPY_ARRAY_IN_ARRAY);
    if (image_array == NULL) {
        PyErr_SetString(PyExc_TypeError, "Failed to convert image to a NumPy uint8 array.");
        return NULL;
    }

    uint8_t* image_data = (uint8_t*)PyArray_DATA(image_array);
    
    for (int i = 0; i < 9; ++i) {
        printf("%i ", image_data[i]);
    }
    uint32_t bDepth = depth / 4;
    uint32_t bRow = rows / 4;
    uint32_t bCols = cols / 4;
    cout << "bDepth: " << bDepth << " bRow: " << bRow << " bCols: " << bCols << endl;
    uint64_t* blockMapArray = (uint64_t*)malloc(bDepth * bRow * bCols * sizeof(uint64_t));
    uint32_t count = blockMap(image_data,blockMapArray, depth, rows, cols, bDepth, bRow, bCols);

    RwnmrConfig rwnmr_config =  RWNMR(RWNMR_object);
    UctConfig uCT_Config = UCT(UCT_object);
    CpmgConfig cpmg_config = CPMG(CPMG_object);
    rwnmrApp app;

    //build the simulation
    app.buildEssentials(rwnmr_config, uCT_Config, blockMapArray, rows,cols,depth, count);

    //run the simulation and return the results
    NMR_cpmg cpmg = app.CPMG(cpmg_config);
    PyObject* amps = vector_to_pylist(cpmg.getSignalAmps());
    PyObject* times = vector_to_pylist(cpmg.getSignalTimes());
    PyObject* result = PyTuple_Pack(2, times, amps);
    return result;
};

static struct PyMethodDef methods[] = {
    {"metodo1", (PyCFunction) metodo1,METH_VARARGS, "Testando metodo simples"},
    {"metodo2", (PyCFunction) helloworld,METH_VARARGS, "Testando print simples"},
    // {"CPMG", (PyCFunction) CPMG,METH_VARARGS, "Testando CPMG"},
    {"create_and_return_array", (PyCFunction)create_and_return_array, METH_VARARGS, "Create and return a NumPy array"},
    {"recebe_objeto_classe", (PyCFunction) recebe_objeto_classe, METH_VARARGS, "Recebe um objeto Python e uma classe Python"},
    {"BitBlockMethod", (PyCFunction) BitBlockMethod,METH_VARARGS, "Testando BitBlock"},
    {"CPMG_EXECUTE", (PyCFunction) CPMG_EXECUTE,METH_VARARGS, "Testando CPMG_EXECUTE"},
    
    {NULL, NULL}
};

static struct PyModuleDef rwnmr ={
    PyModuleDef_HEAD_INIT,
    "wrapper",
    NULL,
    -1,
    methods
};

PyMODINIT_FUNC PyInit_wrapper(void) {
    import_array();  // Necessary for NumPy initialization
    return PyModule_Create(&rwnmr);
}