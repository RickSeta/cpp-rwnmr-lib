#include "Model.h"

std::mt19937 Model::_rng;

Model::Model(rwnmr_config _rwNMR_config, 
                               uct_config _uCT_config,
                               string _project_root) :   rwNMR_config(_rwNMR_config),
                                                         uCT_config(_uCT_config),
                                                         simulationSteps(0),
                                                         numberOfEchoes(0),
                                                         numberOfImages(0),
                                                         bitBlock(NULL),
                                                         width(0),
                                                         height(0),
                                                         depth(0),
                                                         boundaryCondition("noflux"),
                                                         numberOfPores(0),
                                                         porosity(-1.0),
                                                         interfacePoreMatrix(0),
                                                         svpRatio(-1.0),
                                                         walkerOccupancy(-1.0),
                                                         voxelDivisionApplied(false),
                                                         histogram()
{
    // init vector objects
    vector<Mat> binaryMap();
    vector<Point3D> pores();
    vector<uint> walkersIdxList();
    vector<Walker> walkers();
    vector<CollisionHistogram> histogramList();

    // set simulation name and directory to save results
    (*this).setDbPath(_project_root + "./db/");
    (*this).setName(this->rwNMR_config.getName());
    (*this).setDir((*this).createDirectoryForResults((*this).getDbPath()));

    // assign attributes from rwnmr config files
    (*this).setNumberOfWalkers(this->rwNMR_config.getWalkers());
    (*this).setWalkerSamples(this->rwNMR_config.getWalkerSamples());
    (*this).setDiffusionCoefficient(this->rwNMR_config.getD0());
    (*this).initGiromagneticRatio(this->rwNMR_config.getGiromagneticRatio(), this->rwNMR_config.getGiromagneticUnit());
    (*this).setBulkRelaxationTime(this->rwNMR_config.getBulkTime());
    (*this).setStepsPerEcho(this->rwNMR_config.getStepsPerEcho());
    (*this).setGpuUsage(this->rwNMR_config.getGPUUsage());
    (*this).initSeed(true);
    
    // Initialize random state
    Model::_rng.seed((*this).getInitialSeed());
    Model::_rng.discard(4096);


    // assign attributes from uct config files
    (*this).setImageResolution(this->uCT_config.getResolution());

    (*this).initVoxelDivision(0);
    (*this).initImageVoxelResolution();
    if(this->uCT_config.getVoxelDivision() > 0) this->voxelDivisionApplied = true; 

    // set default time step measurement
    (*this).initTimeInterval(); 
    (*this).setBoundaryCondition(this->rwNMR_config.getBC());
}

void Model::initSeed(bool _flag)
{
    if(this->rwNMR_config.getSeed() == 0)
        (*this).setInitialSeed(mRNG::RNG_uint64());        
    else
        (*this).setInitialSeed(this->rwNMR_config.getSeed());
    (*this).setSeedFlag(_flag);
}

void Model::initGiromagneticRatio(double _gamma, string _unit)
{
    if(_unit == "mhertz") this->giromagneticRatio = _gamma * TWO_PI;
    else this->giromagneticRatio = _gamma;
}


void Model::initImageVoxelResolution()
{
    (*this).setImageVoxelResolution((*this).getImageResolution() / (double) (*this).getVoxelDivision());
}

void Model::initTimeInterval()
{
    (*this).setTimeInterval(((*this).getImageVoxelResolution() * 
                             (*this).getImageVoxelResolution()) / 
                             (6.0 * (*this).getDiffusionCoefficient()));
}

void Model::initVoxelDivision(uint _shifts)
{
    this->voxelDivision = pow(2,_shifts);
    if(this->voxelDivision > 1) this->voxelDivisionApplied = true;
    else this->voxelDivisionApplied = false;
}

void Model::applyVoxelDivision(uint _shifts)
{
    double time = omp_get_wtime();
    cout << "- applying voxel division:" << endl;

    // reset resolution scales
    uint previousDivision = (*this).getVoxelDivision();
    (*this).initVoxelDivision(_shifts);
    (*this).initImageVoxelResolution();

    // // trying to maintain steps per echo
    (*this).initTimeInterval();
    uint steps = (*this).getStepsPerEcho() * (*this).getNumberOfEchoes();
    (*this).buildTimeFramework(steps);

    // if walkers exists, fill intra-voxel sites //
    if(this->walkers.size() > 0)
    {
        double shiftFactor = (*this).getVoxelDivision() / (double) previousDivision;
        uint indexExpansion = (uint) shiftFactor - 1;
        if(indexExpansion < 0) indexExpansion = 0;
        
        int shiftX, shiftY, shiftZ;
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, indexExpansion);

        // Divide walkers in packs
        uint walkerPacks = 10;
        uint packSize = this->numberOfWalkers / walkerPacks;
        uint lastPackSize = this->numberOfWalkers % walkerPacks;

        // create progress bar object
        ProgressBar pBar(10);
        uint idx = 0;

        for (uint pack = 0; pack < (walkerPacks - 1); pack++)
        {
            for (uint i = 0; i < packSize; i++)
            {
                idx = pack * packSize + i;

                // randomly place walker in voxel sites
                shiftX = ((int) this->walkers[idx].getInitialPositionX() * shiftFactor) + dist(Model::_rng);
                shiftY = ((int) this->walkers[idx].getInitialPositionY() * shiftFactor) + dist(Model::_rng);
                shiftZ = ((int) this->walkers[idx].getInitialPositionZ() * shiftFactor) + dist(Model::_rng);
                this->walkers[idx].placeWalker(shiftX, shiftY, shiftZ);

                // update collision penalty
                this->walkers[idx].computeDecreaseFactor(this->imageVoxelResolution, this->diffusionCoefficient);
            }

            // Update progress bar
            pBar.update(1);
            pBar.print();
        }

        for (uint i = 0; i < (packSize + lastPackSize); i++)
        {
            idx = (walkerPacks - 1) * packSize + i;

            // randomly place walker in voxel sites
            shiftX = ((int) this->walkers[idx].getInitialPositionX() * shiftFactor) + dist(Model::_rng);
            shiftY = ((int) this->walkers[idx].getInitialPositionY() * shiftFactor) + dist(Model::_rng);
            shiftZ = ((int) this->walkers[idx].getInitialPositionZ() * shiftFactor) + dist(Model::_rng);
            this->walkers[idx].placeWalker(shiftX, shiftY, shiftZ);

            // update collision penalty
            this->walkers[idx].computeDecreaseFactor(this->imageVoxelResolution, this->diffusionCoefficient);
        }

        // Last update in progress bar
        pBar.update(1);
        pBar.print();    
    }

    time = omp_get_wtime() - time;
    cout << " in " << time << " seconds." << endl; 
}

// param @_time needs to be in miliseconds
void Model::initNumberOfStepsFromTime(double _time)
{
    // _time = _time;
    this->simulationSteps =  round( _time * (6 * this->diffusionCoefficient / (this->imageVoxelResolution * this->imageVoxelResolution))); 
    
    // correct steps < steps per echo case (simulation becomes inaccurate but at least don't crash)
    if(this->simulationSteps < this->stepsPerEcho) this->simulationSteps = this->stepsPerEcho;
}

void Model::buildTimeFramework(uint _steps)
{
    if(_steps < this->stepsPerEcho) _steps = this->stepsPerEcho;
    this->numberOfEchoes = (uint)ceil( _steps / (double)this->stepsPerEcho);
    this->simulationSteps = this->numberOfEchoes * this->stepsPerEcho;
}

// param @_time needs to be in miliseconds
void Model::buildTimeFramework(double _time)
{
    (*this).initNumberOfStepsFromTime(_time);
    this->numberOfEchoes = (uint)ceil((double)this->simulationSteps / (double)this->stepsPerEcho);
    this->simulationSteps = this->numberOfEchoes * this->stepsPerEcho;
}

void Model::readImage()
{
    if(this->uCT_config.getImgFiles().size() == this->uCT_config.getSlices())
    {
        this->numberOfImages = this->uCT_config.getSlices();
        this->depth = this->uCT_config.getSlices();
        (*this).assemblyImagePath();
        (*this).loadRockImageFromList();
    }
    else
    {
        (*this).assemblyImagePath();
        (*this).loadRockImage();
    } 

    (*this).createBitBlockMap();
    (*this).countPoresInBitBlock();
    (*this).countInterfacePoreMatrix();

}

void Model::initWalkers(void)
{
    if(this->bitBlock->getNumberOfBlocks() > 0) // only set walkers, if image was loaded
    {
        if(this->rwNMR_config.getWalkersPlacement() == "point")
        { 
            // define coords of central point
            int center_x = (int) (*this).getWidth()/2;
            int center_y = (int) (*this).getHeight()/2;
            int center_z = (int) (*this).getDepth()/2;
            Point3D center(center_x, center_y, center_z);

            // now set walkers
            (*this).initWalkers(center, this->rwNMR_config.getWalkers());
        } else 
        if (this->rwNMR_config.getWalkersPlacement() == "cubic")
        {   
            // define restriction points
            int center_x = (int) ((*this).getWidth() - 1)/2;
            int center_y = (int) ((*this).getHeight() - 1)/2;
            int center_z = (int) ((*this).getDepth() - 1)/2;
            int deviation = (int) this->rwNMR_config.getPlacementDeviation();
            Point3D point1(center_x - deviation, center_y - deviation, center_z - deviation);
            Point3D point2(center_x + deviation, center_y + deviation, center_z + deviation);

            // now set walkers 
            (*this).initWalkers(point1, point2, this->rwNMR_config.getWalkers());
        } else
        {
            (*this).initWalkers(this->rwNMR_config.getWalkers());
        }  
        
        // apply voxel division if needed
        if(this->voxelDivisionApplied) (*this).applyVoxelDivision(this->uCT_config.getVoxelDivision());
    } else
    {
        cout << "error: image was not loaded yet" << endl;
    }
    
}

void Model::initWalkers(uint _numberOfWalkers, bool _randomInsertion)
{    
    (*this).setNumberOfWalkers(_numberOfWalkers);
    (*this).updateWalkerOccupancy();
    (*this).createWalkers();

    if(_randomInsertion == false)
    {
        (*this).createPoreList();
        (*this).createWalkersIdxList();
        (*this).placeWalkersUniformly(); 
        (*this).freePoreList();
    } else
    {
        (*this).placeWalkersByChance();
    }

    // associate rw simulation methods
    (*this).associateMapSimulation(); 
}

void Model::initWalkers(Point3D _point, uint _numberOfWalkers)
{    
    (*this).setNumberOfWalkers(_numberOfWalkers);
    (*this).updateWalkerOccupancy();
    (*this).createWalkers();
    (*this).createPoreList();
    (*this).placeWalkersInSamePoint(_point.getX(), _point.getY(), _point.getZ());

    // associate rw simulation methods
    (*this).associateMapSimulation(); 
}

void Model::initWalkers(Point3D _point1, Point3D _point2, uint _numberOfWalkers)
{    
    (*this).setNumberOfWalkers(_numberOfWalkers);
    (*this).updateWalkerOccupancy();
    (*this).countPoresInCubicSpace(_point1, _point2);
    (*this).createPoreList(_point1, _point2);
    (*this).createWalkers();
    (*this).placeWalkersInCubicSpace(_point1, _point2);

    // associate rw simulation methods
    (*this).associateMapSimulation();  
}

// save results
void Model::save()
{
    double time = omp_get_wtime();
    cout << "saving results:" << endl;
    (*this).saveInfo(this->dir);

    ProgressBar pBar(3.0);

    if(this->rwNMR_config.getSaveImgInfo())
    {   
        (*this).saveImageInfo(this->dir);
    } 

    pBar.update(1);
    pBar.print();

    if(this->rwNMR_config.getSaveBinImg())
    {   
        (*this).saveBitBlock(this->dir);
    }

    pBar.update(1);
    pBar.print();

    if(this->rwNMR_config.getSaveWalkers())
    {   
        (*this).saveWalkers(this->dir);
    }

    pBar.update(1);
    pBar.print();

    time = omp_get_wtime() - time;
    cout << " in " << time << " seconds." << endl; 
}

// save results in other directory
void Model::save(string _otherDir)
{
    double time = omp_get_wtime();
    cout << "saving results...";

    ProgressBar pBar(3.0);
    
    if(this->rwNMR_config.getSaveImgInfo())
    {   
        (*this).saveImageInfo(_otherDir);
    }

    pBar.update(1);
    pBar.print();

    if(this->rwNMR_config.getSaveBinImg())
    {   
        (*this).saveBitBlock(_otherDir);
    }

    pBar.update(1);
    pBar.print();

    if(this->rwNMR_config.getSaveWalkers())
    {   
        (*this).saveWalkers(_otherDir);
    }

    pBar.update(1);
    pBar.print();

    time = omp_get_wtime() - time;
    cout << " in " << time << " seconds." << endl; 
}

void Model::loadRockImage()
{
    double time = omp_get_wtime();
    cout << "- loading rock image:" << endl;

    // reserve memory for binaryMap
    this->binaryMap.reserve(numberOfImages);

    // constant strings
    string currentDirectory = this->imagePath.getPath();
    string currentFileName = this->imagePath.getFilename();
    string currentExtension = this->imagePath.getExtension();

    // variable strings
    string currentFileID;
    string currentImagePath;

    uint firstImage = this->imagePath.getFileID();
    uint digits = this->imagePath.getDigits();

    // create progress bar object
    ProgressBar pBar((double) (this->numberOfImages));

    for (uint slice = 0; slice < this->numberOfImages; slice++)
    {
        // identifying next image to be read
        currentFileID = (*this).convertFileIDToString(firstImage + slice, digits);
        currentImagePath = currentDirectory + currentFileName + currentFileID + currentExtension;

        Mat rockImage = imread(currentImagePath, 1);

        if (!rockImage.data)
        {
            cout << "Error: No image data in file " << currentImagePath << endl;
            exit(1);
        }

        this->height = rockImage.rows;
        this->width = rockImage.cols * rockImage.channels();

        (*this).createBinaryMap(rockImage, slice);

        // Update progress bar
        pBar.update(1);
        pBar.print();
        
    }

    time = omp_get_wtime() - time;
    cout << " in " << time << " seconds." << endl;
}

void Model::loadRockImageFromList()
{
    double time = omp_get_wtime();
    cout << "- loading rock image from list:" << endl;

    // reserve memory for binaryMap
    this->binaryMap.reserve(this->numberOfImages);

    // variable strings
    string currentImagePath;

    // create progress bar object
    ProgressBar pBar((double) this->numberOfImages);

    for (uint slice = 0; slice < this->numberOfImages; slice++)
    {
        currentImagePath = this->uCT_config.getImgFile(slice);

        Mat rockImage = imread(currentImagePath, 1);

        if (!rockImage.data)
        {
            cout << "Error: No image data in file " << currentImagePath << endl;
            exit(1);
        }

        this->height = rockImage.rows;
        this->width = rockImage.cols * rockImage.channels();

        (*this).createBinaryMap(rockImage, slice);

        // Update progress bar
        pBar.update(1);
        pBar.print();
    }

    time = omp_get_wtime() - time;
    cout << " in " << time << " seconds." << endl;
}


void Model::createBinaryMap(Mat &_rockImage, uint slice)
{
    // create an "empty" image to be filled in binary map vector
    Mat emptyMap = Mat::zeros(_rockImage.rows, _rockImage.cols, CV_8UC1);
    binaryMap.push_back(emptyMap);

    int channels = _rockImage.channels();
    uchar *rockImagePixel;
    uchar *binaryMapPixel;

    for (int row = 0; row < this->height; ++row)
    {
        rockImagePixel = _rockImage.ptr<uchar>(row);
        binaryMapPixel = this->binaryMap[slice].ptr<uchar>(row);
        int mapColumn = 0;

        for (int column = 0; column < this->width; column += channels)
        {
            int currentChannel = 0;
            bool pixelIsPore = true;

            while (currentChannel < channels && pixelIsPore != false)
            {

                if (rockImagePixel[column + currentChannel] != 0)
                {
                    pixelIsPore = false;
                }

                currentChannel++;
            }

            if (pixelIsPore == false)
            {
                binaryMapPixel[mapColumn] = 255;
            }

            mapColumn++;
        }
    };
}

void Model::createBitBlockMap()
{
    double time = omp_get_wtime();
    cout << "- creating (bit)block map:" << endl;
    if(this->bitBlock == NULL)
        this->bitBlock = new BitBlock();
    this->bitBlock->createBlockMap(this->binaryMap);

    // update image info
    this->width = this->bitBlock->getImageColumns();
    this->height = this->bitBlock->getImageRows();
    this->depth = this->bitBlock->getImageDepth();
    this->binaryMap.clear();

    time = omp_get_wtime() - time;
    cout << " in " << time << " seconds." << endl;
}

// deprecated
void Model::countPoresInBinaryMap()
{
    double time = omp_get_wtime(); 
    cout << "counting ";

    for (int slice = 0; slice < this->numberOfImages; slice++)
    { // accept only char type matrices
        CV_Assert(binaryMap[slice].depth() == CV_8U);

        uint mapWidth = (uint)binaryMap[slice].cols;
        uchar *binaryMapPixel;

        for (int row = 0; row < height; ++row)
        {
            // psosition pointer at first element in current row
            binaryMapPixel = binaryMap[slice].ptr<uchar>(row);

            for (int column = 0; column < mapWidth; column++)
            {
                if (binaryMapPixel[column] == 0)
                {
                    // x coordinate corresponds to column in binary map Mat structure
                    // y coordinate corresponds to row in the binary map Mat structure
                    Point3D detectedPore = {column, row, slice};
                    pores.insert(pores.end(), detectedPore);
                    this->numberOfPores++;
                }
            }
        }
    }

    this->binaryMap.clear();
    (*this).updatePorosity();

    time = omp_get_wtime() - time;
    cout << this->numberOfPores << " pore voxels in rock image...Ok. (" << time << " seconds)." << endl; 
    cout << "porosity: " << this->porosity << endl;
}

void Model::countPoresInBitBlock()
{
    double time = omp_get_wtime(); 
    cout << "- counting pore voxels in rock image:" << endl;

    // consider 2 or 3 dimensions
    bool dim3 = false; 
    if(this->bitBlock->getImageDepth() > 1) 
        dim3 = true;

    // Create progress bar object
    ProgressBar pBar((double) (this->bitBlock->getImageDepth()));

    // first, count all pores in image
    this->numberOfPores = 0;
    for(uint z = 0; z < this->bitBlock->getImageDepth(); z++)
    {
        for(uint y = 0; y < this->bitBlock->getImageRows(); y++)
        {
            for(uint x = 0; x < this->bitBlock->getImageColumns(); x++)
            {
                int block, bit;
                if(dim3 == true)
                {
                    block = this->bitBlock->findBlock(x, y, z);
                    bit = this->bitBlock->findBitInBlock(x, y, z);
                } else
                {
                    block = this->bitBlock->findBlock(x, y);
                    bit = this->bitBlock->findBitInBlock(x, y);                    
                }

                // now check if bit is pore or wall
                if (!this->bitBlock->checkIfBitIsWall(block, bit))
                {
                    this->numberOfPores++;
                }
            }
        } 

        // Update progress bar
        pBar.update(1);
        pBar.print();      
    }

    (*this).updatePorosity();

    time = omp_get_wtime() - time;
    cout << " in " << time << " seconds." << endl; 
    cout << this->numberOfPores << " pore voxel(s) identified. " ;
    cout << "porosity: " << this->porosity << endl;
}

void Model::countPoresInCubicSpace(Point3D _vertex1, Point3D _vertex2)
{
    double time = omp_get_wtime(); 
    cout << "- counting pore voxels in cubic space:" << endl;

    // consider 2 or 3 dimensions
    bool dim3 = false; 
    if(this->bitBlock->getImageDepth() > 1) 
        dim3 = true;

    // set cube limits
    uint x0, y0, z0;
    uint xf, yf, zf;

    // coordinate x:
    if(_vertex1.getX() < _vertex2.getX()) 
    { 
        x0 = _vertex1.getX(); xf = _vertex2.getX();
    } else
    {
        x0 = _vertex2.getX();    xf = _vertex1.getX();
    }

    // coordinate y:
    if(_vertex1.getY() < _vertex2.getY())
    {
        y0 = _vertex1.getY();    yf = _vertex2.getY();
    } else
    {
        y0 = _vertex1.getY();    yf = _vertex2.getY();
    }

    // coordinate z:
    if(_vertex1.getZ() < _vertex2.getZ())
    {
        z0 = _vertex1.getZ();    zf = _vertex2.getZ();
    } else
    {
        z0 = _vertex1.getZ();    zf = _vertex2.getZ();
    }

    // apply image border restrictions
    if(x0 < 0) x0 = 0;
    if(y0 < 0) y0 = 0;
    if(z0 < 0) z0 = 0;
    if(xf > this->bitBlock->getImageColumns()) xf = this->bitBlock->getImageColumns();
    if(yf > this->bitBlock->getImageRows()) yf = this->bitBlock->getImageRows();
    if(zf > this->bitBlock->getImageDepth()) zf = this->bitBlock->getImageDepth();

    // Create progress bar object
    ProgressBar pBar((double) (zf - z0));  

    // first, count all pores in image
    this->numberOfPores = 0;
    for(uint z = z0; z < zf; z++)
    {
        for(uint y = y0; y < yf; y++)
        {
            for(uint x = x0; x < xf; x++)
            {
                int block, bit;
                if(dim3 == true)
                {
                    block = this->bitBlock->findBlock(x, y, z);
                    bit = this->bitBlock->findBitInBlock(x, y, z);
                } else
                {
                    block = this->bitBlock->findBlock(x, y);
                    bit = this->bitBlock->findBitInBlock(x, y);                    
                }

                // now check if bit is pore or wall
                if (!this->bitBlock->checkIfBitIsWall(block, bit))
                {
                    this->numberOfPores++;
                }
            }
        }  
        
        // Update progress bar
        pBar.update(1);
        pBar.print();       
    }

    time = omp_get_wtime() - time;
    cout << " in " << time << " seconds." << endl; 
    cout << this->numberOfPores << " pore voxel(s) in cubic space identified." << endl; 
    cout << "porosity: " << this->porosity << endl; 
}

void Model::countInterfacePoreMatrix()
{
    double time = omp_get_wtime(); 
    cout << "- counting pore-matrix interface voxels in rock image:" << endl;

    // consider 2 or 3 dimensions
    bool dim3 = false; 
    if(this->bitBlock->getImageDepth() > 1) 
        dim3 = true;

    // Create progress bar object
    ProgressBar pBar((double) (this->bitBlock->getImageDepth()));

    // first, count all pores in image
    this->interfacePoreMatrix = 0;
    for(int z = 0; z < this->bitBlock->getImageDepth(); z++)
    {
        // neighbor Z location
        int nextZ = (z + 1) % this->bitBlock->getImageDepth();

        for(int y = 0; y < this->bitBlock->getImageRows(); y++)
        {
            // neighbor Y location
            int nextY = (y + 1) % this->bitBlock->getImageRows();

            for(int x = 0; x < this->bitBlock->getImageColumns(); x++)
            {
                // neighbor X location
                int nextX = (x + 1) % this->bitBlock->getImageColumns();

                int currentBlock, currentBit;
                int xBlock, xBit;
                int yBlock, yBit;
                int zBlock, zBit;              

                if(dim3 == true)
                {
                    currentBlock = this->bitBlock->findBlock(x, y, z);
                    currentBit = this->bitBlock->findBitInBlock(x, y, z);

                    xBlock = this->bitBlock->findBlock(nextX, y, z);
                    xBit = this->bitBlock->findBitInBlock(nextX, y, z);
                    
                    yBlock = this->bitBlock->findBlock(x, nextY, z);
                    yBit = this->bitBlock->findBitInBlock(x, nextY, z);
                    
                    zBlock = this->bitBlock->findBlock(x, y, nextZ);
                    zBit = this->bitBlock->findBitInBlock(x, y, nextZ);


                } else
                {
                    currentBlock = this->bitBlock->findBlock(x, y);
                    currentBit = this->bitBlock->findBitInBlock(x, y);

                    xBlock = this->bitBlock->findBlock(nextX, y);
                    xBit = this->bitBlock->findBitInBlock(nextX, y);
                    
                    yBlock = this->bitBlock->findBlock(x, nextY);
                    yBit = this->bitBlock->findBitInBlock(x, nextY);
                                                            
                }

                // Check if neighbor bit values are different
                if (this->bitBlock->checkIfBitIsWall(currentBlock, currentBit) != this->bitBlock->checkIfBitIsWall(xBlock, xBit))
                {
                    this->interfacePoreMatrix++;
                }

                if (this->bitBlock->checkIfBitIsWall(currentBlock, currentBit) != this->bitBlock->checkIfBitIsWall(yBlock, yBit))
                {
                    this->interfacePoreMatrix++;
                }

                if (dim3 == true and this->bitBlock->checkIfBitIsWall(currentBlock, currentBit) != this->bitBlock->checkIfBitIsWall(zBlock, zBit))
                {
                    this->interfacePoreMatrix++;
                }
            }
        } 

        // Update progress bar
        pBar.update(1);
        pBar.print();      
    }

    (*this).updateSvpRatio();

    time = omp_get_wtime() - time;
    cout << " in " << time << " seconds." << endl; 
    cout << this->interfacePoreMatrix << " interface pore-matrix voxel(s) identified. " ;
    cout << "S/Vp: " << this->svpRatio << endl;
}

void Model::updateSvpRatio()
{
    double voxelFacialArea = (*this).getImageVoxelResolution() * (*this).getImageVoxelResolution();
    double voxelVolume = voxelFacialArea * (*this).getImageVoxelResolution();
    this->svpRatio = ((double) this->interfacePoreMatrix * voxelFacialArea) / ((double) this->numberOfPores * voxelVolume);
}

void Model::updatePorosity()
{
    this->porosity = (double) this->numberOfPores / 
                     (double) (this->bitBlock->getImageColumns() * this->bitBlock->getImageRows() * this->bitBlock->getImageDepth());
}

void Model::updateNumberOfPores()
{
    this->numberOfPores = (uint) (this->porosity * (double) (this->bitBlock->getImageColumns() * this->bitBlock->getImageRows() * this->bitBlock->getImageDepth()));
}

void Model::createPoreList()
{
    double time = omp_get_wtime(); 
    cout << "- creating complete pore list:" << endl;

    // consider 2 or 3 dimensions
    bool dim3 = false; 
    if(this->bitBlock->getImageDepth() > 1) 
        dim3 = true;

    // second, create pore list
    (*this).updateNumberOfPores();
    if(this->pores.size() > 0) this->pores.clear();
    this->pores.reserve(this->numberOfPores);

    // Create progress bar object
    ProgressBar pBar((double) (this->bitBlock->getImageDepth()));   

    for(int z = 0; z < this->bitBlock->getImageDepth(); z++)
    {
        for(int y = 0; y < this->bitBlock->getImageRows(); y++)
        {
            for(int x = 0; x < this->bitBlock->getImageColumns(); x++)
            {
                int block, bit;
                if(dim3 == true)
                {
                    block = this->bitBlock->findBlock(x, y, z);
                    bit = this->bitBlock->findBitInBlock(x, y, z);
                } else
                {
                    block = this->bitBlock->findBlock(x, y);
                    bit = this->bitBlock->findBitInBlock(x, y);                    
                }

                if (!this->bitBlock->checkIfBitIsWall(block, bit))
                {
                    Point3D detectedPore = {x, y, z};
                    this->pores.push_back(detectedPore);
                }
            }
        } 

        // Update progress bar
        pBar.update(1);
        pBar.print();         
    }

    time = omp_get_wtime() - time;
    cout << " in " << time << " seconds." << endl; 
}

void Model::createPoreList(Point3D _vertex1, Point3D _vertex2)
{
    double time = omp_get_wtime(); 
    cout << "- creating restricted pore list:" << endl;

    // consider 2 or 3 dimensions
    bool dim3 = false; 
    if(this->bitBlock->getImageDepth() > 1) 
        dim3 = true;

    // set cube limits
    uint x0, y0, z0;
    uint xf, yf, zf;

    // coordinate x:
    if(_vertex1.getX() < _vertex2.getX()) 
    { 
        x0 = _vertex1.getX(); xf = _vertex2.getX();
    } else
    {
        x0 = _vertex2.getX();    xf = _vertex1.getX();
    }

    // coordinate y:
    if(_vertex1.getY() < _vertex2.getY())
    {
        y0 = _vertex1.getY();    yf = _vertex2.getY();
    } else
    {
        y0 = _vertex1.getY();    yf = _vertex2.getY();
    }

    // coordinate z:
    if(_vertex1.getZ() < _vertex2.getZ())
    {
        z0 = _vertex1.getZ();    zf = _vertex2.getZ();
    } else
    {
        z0 = _vertex1.getZ();    zf = _vertex2.getZ();
    }

    // apply image border restrictions
    if(x0 < 0) x0 = 0;
    if(y0 < 0) y0 = 0;
    if(z0 < 0) z0 = 0;
    if(xf > this->bitBlock->getImageColumns()) xf = this->bitBlock->getImageColumns();
    if(yf > this->bitBlock->getImageRows()) yf = this->bitBlock->getImageRows();
    if(zf > this->bitBlock->getImageDepth()) zf = this->bitBlock->getImageDepth();

    // second, create pore list
    if(this->pores.size() > 0) this->pores.clear();
    this->pores.reserve(this->numberOfPores);

    // Create progress bar object
    ProgressBar pBar((double) (zf-z0));
    for(int z = z0; z < zf; z++)
    {
        for(int y = y0; y < yf; y++)
        {
            for(int x = x0; x < xf; x++)
            {
                int block, bit;
                if(dim3 == true)
                {
                    block = this->bitBlock->findBlock(x, y, z);
                    bit = this->bitBlock->findBitInBlock(x, y, z);
                } else
                {
                    block = this->bitBlock->findBlock(x, y);
                    bit = this->bitBlock->findBitInBlock(x, y);                    
                }

                if (!this->bitBlock->checkIfBitIsWall(block, bit))
                {
                    Point3D detectedPore = {x, y, z};
                    this->pores.push_back(detectedPore);
                }
            }
        }

        // Update progress bar
        pBar.update(1);
        pBar.print();          
    }

    time = omp_get_wtime() - time;
    cout << " in " << time << " seconds." << endl; 
}

void Model::updateWalkerOccupancy()
{   
    this->walkerOccupancy = (this->numberOfWalkers / ((double) this->numberOfPores)); 
}

void Model::createWalkersIdxList()
{
    double time = omp_get_wtime();
    cout << "- creating walkers ID list:" << endl;

    if(this->walkersIdxList.size() > 0) this->walkersIdxList.clear();
    this->walkersIdxList.reserve(this->numberOfWalkers);

    if(this->pores.size() == 0)
    {
        cout << endl;
        cout << "pores not listed." << endl;
        return;
    }

    // Divide walkers in packs
    uint walkerPacks = 10;
    uint packSize = this->numberOfWalkers / walkerPacks;
    uint lastPackSize = this->numberOfWalkers % walkerPacks;

    // create progress bar object
    ProgressBar pBar(10);
    uint idx = 0;
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, (*this).getNumberOfPores());                
    for (uint pack = 0; pack < (walkerPacks - 1); pack++)
    {
        for (uint i = 0; i < packSize; i++)
        {
            idx = pack * packSize + i;
            this->walkersIdxList.push_back(dist(Model::_rng));
        }

        // Update progress bar
        pBar.update(1);
        pBar.print();
    }

    for (uint i = 0; i < (packSize + lastPackSize); i++)
    {
        idx = (walkerPacks - 1) * packSize + i;
        this->walkersIdxList.push_back(dist(Model::_rng));
    }

    // Last update in progress bar
    pBar.update(1);
    pBar.print();

    
    time = omp_get_wtime() - time;
    cout << " in " << time << " seconds." << endl; 
}


void Model::createWalkers()
{
    double time = omp_get_wtime();
    cout << "- creating " << this->numberOfWalkers << " walkers:" << endl;

    // define the dimensionality that walkers will be trated
    bool dim3 = false;
    if (this->bitBlock->getImageDepth() > 1)
    {
        dim3 = true;
    }

    // alloc memory space for vector of walkers with size = numberOfWalkers
    if(this->walkers.size() > 0) this->walkers.clear();
    this->walkers.reserve(this->numberOfWalkers);
    uint64_t tempSeed = this->initialSeed + 1;

    // create walkers
    vector<double> rho;
    rho = this->rwNMR_config.getRho();

    uint walkerPacks = 10;
    uint packSize = this->numberOfWalkers / walkerPacks;
    uint lastPackSize = this->numberOfWalkers % walkerPacks;

    // create progress bar object
    ProgressBar pBar(10);
    uint idx = 0;
    std::uniform_int_distribution<uint64_t> uint64_dist;

    for (uint pack = 0; pack < (walkerPacks - 1); pack++)
    {
        for (uint i = 0; i < packSize; i++)
        {
            idx = pack * packSize + i;
            Walker temporaryWalker(dim3);
            this->walkers.push_back(temporaryWalker);
            if(this->rwNMR_config.getRhoType() == "uniform") this->walkers[idx].setSurfaceRelaxivity(rho[0]);
            else if(this->rwNMR_config.getRhoType() == "sigmoid") this->walkers[idx].setSurfaceRelaxivity(rho);
            this->walkers[idx].computeDecreaseFactor(this->imageVoxelResolution, this->diffusionCoefficient);
            this->walkers[idx].setRandomSeed(uint64_dist(Model::_rng));
        }

        // Update progress bar
        pBar.update(1);
        pBar.print();
    }

    for (uint i = 0; i < (packSize + lastPackSize); i++)
    {
        idx = (walkerPacks - 1) * packSize + i;
        Walker temporaryWalker(dim3);
        this->walkers.push_back(temporaryWalker);
        if(this->rwNMR_config.getRhoType() == "uniform") this->walkers[idx].setSurfaceRelaxivity(rho[0]);
        else if(this->rwNMR_config.getRhoType() == "sigmoid") this->walkers[idx].setSurfaceRelaxivity(rho);
        this->walkers[idx].computeDecreaseFactor(this->imageVoxelResolution, this->diffusionCoefficient);
        this->walkers[idx].setRandomSeed(uint64_dist(Model::_rng)); 
    }

    // Update progress bar
    pBar.update(1);
    pBar.print();
    
    time = omp_get_wtime() - time;
    cout << " in " << time << " seconds." << endl; 
}


void Model::placeWalkersByChance()
{

    double time = omp_get_wtime();
    cout << "- placing walkers by chance...";

    if(numberOfPores == 0)
    {
        cout << endl;
        cout << "pores not counted." << endl;
        return;
    }

    // define the dimensionality that walkers will be trated
    bool dim3 = false;
    if (this->bitBlock->getImageDepth() > 1)
    {
        dim3 = true;
    }    

    uint walkersInserted = 0;
    uint errorCount = 0;
    uint erroLimit = 1000;
    uint idx = 0;
    Point3D point; 
    bool validPoint = false;   

    std::uniform_int_distribution<std::mt19937::result_type> columnDist(0, this->bitBlock->getImageColumns());
    std::uniform_int_distribution<std::mt19937::result_type> rowDist(0, this->bitBlock->getImageRows());
    std::uniform_int_distribution<std::mt19937::result_type> depthDist(0, this->bitBlock->getImageDepth());
        
    while(walkersInserted < this->numberOfWalkers && errorCount < erroLimit)
    {
        // randomly choose a position
        point.setX(columnDist(Model::_rng));
        point.setY(rowDist(Model::_rng));
        point.setZ(depthDist(Model::_rng));
        if(dim3)
        {
            validPoint = walkers[idx].checkNextPosition_3D(point, (*this).getBitBlock());        
        } else
        {
            validPoint = walkers[idx].checkNextPosition_2D(point, (*this).getBitBlock());
        }

        if(validPoint)
        {
            this->walkers[idx].placeWalker(point.getX(), point.getY(), point.getZ());
            idx++;
            walkersInserted++;
            errorCount = 0;
        }
        else
        {
            errorCount++;
        }
    }
    
    time = omp_get_wtime() - time;
    cout << "Ok. (" << time << " seconds)." << endl; 
    if(errorCount == erroLimit) cout << "could only insert " << endl;
}


void Model::placeWalkersUniformly()
{
    double time = omp_get_wtime();
    cout << "- placing walkers uniformly ";

    if(this->pores.size() == 0)
    {
        cout << endl;
        cout << "pores not listed." << endl;
        return;
    }

    // set omp variables for parallel loop throughout walker list
    const int num_cpu_threads = omp_get_max_threads();
    const int loop_size = this->numberOfWalkers;
    int loop_start, loop_finish;
    cout << "using " << num_cpu_threads << " cpu threads:" << endl;

    ProgressBar pBar((double) num_cpu_threads);
    pBar.print();

    #pragma omp parallel shared(walkers, walkersIdxList, pores, pBar) private(loop_start, loop_finish) 
    {
        const int thread_id = omp_get_thread_num();
        OMPLoopEnabler looper(thread_id, num_cpu_threads, loop_size);
        loop_start = looper.getStart();
        loop_finish = looper.getFinish();   


        if (this->walkerOccupancy != 1.0) 
        {
            for (uint idx = loop_start; idx < loop_finish; idx++)
            {
                // select a pore location from a list randomly generated to place the walker
                uint randomIndex = this->walkersIdxList[idx];
                this->walkers[idx].placeWalker(this->pores[randomIndex].getX(), 
                                               this->pores[randomIndex].getY(), 
                                               this->pores[randomIndex].getZ());
            }
        }
        else
        {
            // If occupancy is 100%, just loop over pore list
            for (uint idx = loop_start; idx < loop_finish; idx++)
            {
                this->walkers[idx].placeWalker(this->pores[idx].getX(), 
                                               this->pores[idx].getY(), 
                                               this->pores[idx].getZ());
            }
        }

        #pragma omp critical
        {
            // update progress bar
            pBar.update(1);
            pBar.print();
        }

    }

    time = omp_get_wtime() - time;
    cout << " in " << time << " seconds." << endl; 
}

void Model::placeWalkersInSamePoint(uint _x, uint _y, uint _z)
{
    double time = omp_get_wtime();
    cout << "- placing walkers at point ";
    cout << "(" << _x << ", " << _y << ", " << _z << ")..."; 

    // check walkers array
    if(this->walkers.size() == 0)
    {
        cout << endl;
        cout << "no walkers to place" << endl;
        return;
    }

    // check image limits
    if(_x > this->bitBlock->getImageRows() || 
       _y > this->bitBlock->getImageColumns() || 
       _z > this->bitBlock->getImageDepth())
    {
        cout << endl;
        cout << "could not place walkers: point is outside image limits." << endl;
        return;
    }

    // set dimensionality that walkers will be trated
    bool dim3 = false;
    if (this->bitBlock->getImageDepth() > 1)
    {
        dim3 = true;
    }

    // checar se ponto é poro
    Point3D point(_x, _y, _z);
    bool validPoint;    
    if(dim3)
    {
        validPoint = walkers[0].checkNextPosition_3D(point, (*this).getBitBlock());        
    } else
    {
        validPoint = walkers[0].checkNextPosition_2D(point, (*this).getBitBlock());
    }

    if(validPoint == false)
    {
        cout << endl;
        cout << "could not place walkers: point is not pore." << endl;
        return;
    } else
    {

        for(uint id = 0; id < this->walkers.size(); id++)
        {
            this->walkers[id].placeWalker(_x, _y, _z);
        }
    
        time = omp_get_wtime() - time;
        cout << "Ok. (" << time << " seconds)." << endl; 
    }
}

void Model::placeWalkersInCubicSpace(Point3D _vertex1, Point3D _vertex2)
{
    double time = omp_get_wtime();
    cout << "- placing walkers at selected space: " << endl;
    cout << "vertex 1: "; _vertex1.printInfo();
    cout << "vertex 2: "; _vertex2.printInfo();


    // check walkers array
    if(this->walkers.size() == 0)
    {
        cout << endl;
        cout << "no walkers to place" << endl;
        return;
    }

    // create a list of pores in selected zone
    vector<uint> selectedPores;
    for(uint idx = 0; idx < this->pores.size(); idx++)
    {
        // check if pore is inside selected cube
        if(this->pores[idx].getX() >= _vertex1.getX() && this->pores[idx].getX() <= _vertex2.getX() &&
           this->pores[idx].getY() >= _vertex1.getY() && this->pores[idx].getY() <= _vertex2.getY() &&
           this->pores[idx].getZ() >= _vertex1.getZ() && this->pores[idx].getZ() <= _vertex2.getZ())
        {
            // add pore to list
            selectedPores.push_back(idx);
        }
    }

    if(selectedPores.size() == 0)
    {
        cout << endl;
        cout << "no pores in the selected cubic space." << endl;
        return;
    }
    else
    {    

        // Create Progress Bar object
        ProgressBar pBar((double) this->numberOfWalkers);
        pBar.print();

        std::uniform_int_distribution<std::mt19937::result_type> dist(0, selectedPores.size());
        for(uint id = 0; id < this->numberOfWalkers; id++)
        {   
    
            uint poreID = selectedPores[dist(Model::_rng)];
            this->walkers[id].placeWalker(this->pores[poreID].getX(), 
                                          this->pores[poreID].getY(), 
                                          this->pores[poreID].getZ());
            
            pBar.update(1);
            pBar.print();
        }
    }

    time = omp_get_wtime() - time;
    cout << " in " << time << " seconds." << endl; 
}

void Model::initHistogramList()
{   
    int numberOfHistograms = this->rwNMR_config.getHistograms();

    // check for really small simulations
    if(this->numberOfEchoes < numberOfHistograms) 
        numberOfHistograms = 1;

    int echosPerHistogram = (this->numberOfEchoes / numberOfHistograms);
    int echosInLastHistogram = this->numberOfEchoes - (numberOfHistograms * echosPerHistogram);
    if(this->histogramList.size() != 0) this->histogramList.clear();
    this->histogramList.reserve(numberOfHistograms);

    for(int idx = 0; idx < numberOfHistograms; idx++)
    {
        CollisionHistogram newHistogram;
        newHistogram.setFirstEcho(idx * echosPerHistogram);
        newHistogram.setLastEcho(newHistogram.getFirstEcho() + echosPerHistogram);
        this->histogramList.push_back(newHistogram);
    }

    // charge rebalance 
    this->histogramList.back().setLastEcho(this->histogramList.back().getLastEcho() + echosInLastHistogram);
}

void Model::createHistogram()
{
    this->histogram.createBlankHistogram(this->rwNMR_config.getHistogramSize(), this->rwNMR_config.getHistogramScale());
    int steps = this->histogramList.back().getLastEcho() * this->stepsPerEcho;
    this->histogram.fillHistogram(this->walkers, steps);       
}

void Model::createHistogram(uint histID, uint _steps)
{
    this->histogramList[histID].createBlankHistogram(this->rwNMR_config.getHistogramSize(), this->rwNMR_config.getHistogramScale());
    this->histogramList[histID].fillHistogram(this->walkers, _steps);       
}

void Model::updateHistogram()
{
    this->histogram.updateHistogram(this->walkers, this->simulationSteps);
}

// cost function methods
void Model::updateWalkersRelaxativity(vector<double> &sigmoid)
{
    for (uint id = 0; id < this->walkers.size(); id++)
    {
        this->walkers[id].updateXiRate(this->simulationSteps);
        this->walkers[id].setSurfaceRelaxivity(sigmoid);
        this->walkers[id].computeDecreaseFactor(this->imageVoxelResolution, this->diffusionCoefficient);
    }
}

void Model::updateWalkersRelaxativity(double rho)
{
    for (uint id = 0; id < this->walkers.size(); id++)
    {
        this->walkers[id].updateXiRate(this->simulationSteps);
        this->walkers[id].setSurfaceRelaxivity(rho);
        this->walkers[id].computeDecreaseFactor(this->imageVoxelResolution, this->diffusionCoefficient);
    }
}


// associate methods
void Model::associateMapSimulation()
{
    if ((*this).getGpuUsage() == true)
    {
        if (this->numberOfImages == 1)
        {
            mapSimulationPointer = &Model::mapSimulation_CUDA_2D_histograms;
        }
        else
        {
            mapSimulationPointer = &Model::mapSimulation_CUDA_3D_histograms;
        }
    }
    else
    {
        mapSimulationPointer = &Model::mapSimulation_OMP;
    }
}

uint Model::pickRandomIndex(uint _minValue, uint _maxValue)
{
    // int CPUfactor = 1;
    // if(this->rwNMR_config.getOpenMPUsage()) CPUfactor += omp_get_thread_num();
    // std::random_device dev;
    // std::mt19937 rng(dev()* CPUfactor * CPUfactor);
    std::uniform_int_distribution<std::mt19937::result_type> dist(_minValue, _maxValue-1); 
    return dist(Model::_rng); 
}

Point3D Model::removeRandomPore(vector<Point3D> &_pores, uint _randomIndex)
{
    Point3D randomPore = _pores[_randomIndex];
    std::swap(_pores[_randomIndex], _pores.back());
    _pores.pop_back();
    return randomPore;
}

uint Model::removeRandomIndexFromPool(vector<uint> &_pool, uint _randomIndex)
{
    uint element = _pool[_randomIndex];
    std::swap(_pool[_randomIndex], _pool.back());
    _pool.pop_back();
    return element;
}

void Model::assemblyImagePath()
{
    // User Input
     ImagePath input;
     input.setImages(this->uCT_config.getSlices());
     input.setPath(this->uCT_config.getDirPath());
     input.setFilename(this->uCT_config.getFilename());
     input.setFileID(this->uCT_config.getFirstIdx());
     input.setDigits(this->uCT_config.getDigits());
     input.setExtension(this->uCT_config.getExtension());
     input.updateCompletePath();

     (*this).setImagePath(input);
     (*this).setNumberOfImages(input.getImages());
     (*this).setDepth((*this).getNumberOfImages());
}

string Model::createDirectoryForResults(string _path)
{
    BaseFunctions::createDirectory(_path, this->name);
    return (_path + this->name);
}

void Model::saveInfo(string filedir)
{
    string filename = filedir + "/SimInfo.txt";

    // file object init
    ofstream fileObject;

    // open file
    fileObject.open(filename, ios::out);
    if (fileObject.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    // print input details
    fileObject << "------------------------------------------------------" << endl;
    fileObject << ">>> RWNMR SIMULATION PARAMETERS: " << (*this).getName() << endl;
    fileObject << "------------------------------------------------------" << endl;
    fileObject << "Data path: " << (*this).getDbPath() + (*this).getName() << endl;
    fileObject << "Image path: " << (*this).getImagePath().getCompletePath() << endl;
    fileObject << "Image resolution (um/voxel): " << (*this).getImageVoxelResolution() << endl;
    fileObject << "Diffusion coefficient (um^2/ms): " << (*this).getDiffusionCoefficient() << endl;
    fileObject << "Number of images: " << (*this).getNumberOfImages() << endl;
    fileObject << "Walkers pore occupancy in simulation: " << (*this).getWalkerOccupancy() * 100.0 << "%" << endl;
    

    fileObject << "Initial seed: ";
    if (this->seedFlag)
    {
        fileObject << this->initialSeed << endl;
    }
    else
    {
        fileObject << this->initialSeed << "\t(defined by user)" << endl;
    }

    fileObject << "GPU usage: ";
    if ((*this).getGpuUsage())
    {
        fileObject << "ON" << endl;
    }
    else
    {
        fileObject << "OFF" << endl;
    }

    fileObject << "BC: " << (*this).getBoundaryCondition() << endl;

    fileObject << "------------------------------------------------------" << endl;
    fileObject << endl;

    // close file
    fileObject.close();
}

void Model::saveImageInfo(string filedir)
{
    string filename = filedir + "/ImageInfo.txt";

    // file object init
    ofstream fileObject;

    // open file
    fileObject.open(filename, ios::out);
    if (fileObject.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    // write info 
    fileObject << "image path: " << this->imagePath.getCompletePath() << endl;
    fileObject << "width(x): " << this->bitBlock->getImageColumns() << endl;
    fileObject << "height(y): " << this->bitBlock->getImageRows() << endl;
    fileObject << "depth(z): " << this->bitBlock->getImageDepth() << endl;
    fileObject << "resolution: " << this->imageResolution << endl;
    fileObject << "voxel shift: " << this->voxelDivision << endl;
    fileObject << "step length: " << this->imageVoxelResolution << endl;
    fileObject << "porosity: " << this->porosity << endl;
    fileObject << "SVp: " << this->svpRatio << endl;

    // close file
    fileObject.close();
}

void Model::saveWalkers(string filedir)
{
    string filename = filedir + "/walkers.csv";
    ofstream file;
    file.open(filename, ios::out);
    if (file.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

    file << "PositionXi";
    file << ",PositionYi";
    file << ",PositionZi";
    file << ",PositionXf";
    file << ",PositionYf";
    file << ",PositionZf";
    file << ",Collisions";
    file << ",XIRate";
    file << ",Energy"; 
    file << ",RNGSeed" << endl;

    const int precision = 6;
    for (uint index = 0; index < this->walkers.size(); index++)
    {
        file << setprecision(precision) << this->walkers[index].getInitialPositionX()
        << "," << this->walkers[index].getInitialPositionY()
        << "," << this->walkers[index].getInitialPositionZ()
        << "," << this->walkers[index].getCurrentPositionX() 
        << "," << this->walkers[index].getCurrentPositionY() 
        << "," << this->walkers[index].getCurrentPositionZ() 
        << "," << this->walkers[index].getCollisions() 
        << "," << this->walkers[index].getXiRate() 
        << "," << this->walkers[index].getEnergy() 
        << "," << this->walkers[index].getInitialSeed() << endl;
    }

    file.close();
}

void Model::saveBitBlock(string filePath)
{

    string fileName = filePath + "/NMR_binImage.txt";
    this->bitBlock->saveBitBlockArray(fileName);
}

void Model::saveHistogram(string filePath)
{
    string filename = filePath + "/NMR_histogram.txt";
    ofstream in(filename, std::ios::out);
 
    const size_t num_points = this->histogram.getBins().size();
    if (in)
    {
        for (int i = 0; i < num_points; i++)
        {
            in << this->histogram.getBin(i) << "\t" << this->histogram.getAmp(i) << endl;
        }
        return;
    }
    throw(errno);

    in.close();
}

void Model::saveHistogramList(string filePath)
{
    string filename = filePath + "/NMR_histogramEvolution.txt";
    if(this->rwNMR_config.getHistograms() > 0)
    {
        for(int hst_ID = 0; hst_ID < this->histogramList.size(); hst_ID++)
        {
            ofstream in;
            if(hst_ID == 0) in.open(filename, std::ios::out);
            else in.open(filename, std::ios::app);

            in << "histogram [" << hst_ID << "]" << endl;
            const size_t num_points = this->histogramList[hst_ID].getBins().size();
            if (in)
            {
                // in << x_label << "\t" << y_label << endl;
                for (int i = 0; i < num_points; i++)
                {
                    in << this->histogramList[hst_ID].getBin(i) << "\t" << this->histogramList[hst_ID].getAmp(i) << endl;
                }
                in << endl;
                return;
            }
            throw(errno);

            in.close();
        }
    }
}

void Model::info()
{
    (*this).printDetails();
}

void Model::printDetails()
{ 
    // print input details
    cout << "------------------------------------------------------" << endl;
    cout << ">>> NMR SIMULATION 3D PARAMETERS: " << this->name << endl;
    cout << "------------------------------------------------------" << endl;
    cout << "Data path: " << this->dbPath + this->name << endl;
    cout << "Image path: " << this->imagePath.getCompletePath() << endl;
    cout << "Image resolution (um/voxel): " << this->imageVoxelResolution << endl;
    cout << "Diffusion coefficient (um^2/ms): " << this->diffusionCoefficient << endl;
    cout << "Number of images: " << this->numberOfImages << endl;
    cout << "Walkers pore occupancy in simulation: " << this->walkerOccupancy * 100.0 << "%" << endl;
    

    cout << "Initial seed: ";
    if (this->seedFlag)
    {
        cout << this->initialSeed << endl;
    }
    else
    {
        cout << this->initialSeed << "\tdefined by user" << endl;
    }

    cout << "GPU usage: ";
    if ((*this).getGpuUsage())
    {
        cout << "ON" << endl;
    }
    else
    {
        cout << "OFF" << endl;
    }

    cout << "BC: " << (*this).getBoundaryCondition() << endl;

    cout << "------------------------------------------------------" << endl;
    cout << endl;
}

// mapping simulation using bitblock data structure
void Model::mapSimulation_OMP(bool reset)
{
    double begin_time = omp_get_wtime();
    cout << "initializing mapping simulation... ";
    if(reset)
    {
        for (uint id = 0; id < this->walkers.size(); id++)
        {
            this->walkers[id].resetPosition();
            this->walkers[id].resetSeed();
            this->walkers[id].resetCollisions();
            this->walkers[id].resetTCollisions();
        }
    }

    // initialize list of collision histograms
    (*this).initHistogramList(); 

    // loop throughout list
    for(int hst_ID = 0; hst_ID < this->histogramList.size(); hst_ID++)
    {
        int eBegin = this->histogramList[hst_ID].getFirstEcho();
        int eEnd = this->histogramList[hst_ID].getLastEcho();
        for (uint id = 0; id < this->numberOfWalkers; id++)
        {
            for(uint echo = eBegin; echo < eEnd; echo++)
            {
                for (uint step = 0; step < this->stepsPerEcho; step++)
                {
                    this->walkers[id].map((*this).getBitBlock());
                }
            }
        }

        int steps = this->stepsPerEcho * (eEnd - eBegin);
        (*this).createHistogram(hst_ID, steps);

        for (uint id = 0; id < this->numberOfWalkers; id++)
        {
            this->walkers[id].setTCollisions(this->walkers[id].getTCollisions() + this->walkers[id].getCollisions());
            this->walkers[id].resetCollisions();
        }
    }

    // recover walkers collisions from total sum and create a global histogram
    for (uint id = 0; id < this->numberOfWalkers; id++)
    {
        this->walkers[id].setCollisions(this->walkers[id].getTCollisions());   
    }
    (*this).createHistogram();

    cout << "Completed.";
    double finish_time = omp_get_wtime();
    BaseFunctions::printElapsedTime(begin_time, finish_time);
}