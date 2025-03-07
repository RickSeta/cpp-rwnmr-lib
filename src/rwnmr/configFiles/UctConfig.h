#ifndef UCT_CONFIG_H_
#define UCT_CONFIG_H_

#include "configFiles_defs.h"
#include "BaseConfig.h"

using namespace std;

class UctConfig : public BaseConfig
{
private:
    uint FIRST_IDX;
    uint DIGITS;
    string EXTENSION;
    uint SLICES;
    double RESOLUTION;
    uint VOXEL_DIVISION;
    uint8_t PORE_COLOR;

public:
    // default constructors
    UctConfig();

    // default destructor
    virtual ~UctConfig()
    {
        // cout << "OMPLoopEnabler object destroyed succesfully" << endl;
    } 

    vector<string> checkConfig();
    // -- Read methods
    void readFirstIdx(string s);
    void readDigits(string s);
    void readExtension(string s);
    void readSlices(string s); 
    void readResolution(string s);
    void readVoxelDivision(string s);
    void readPoreColor(string s);

    // -- Set methods
    void setFirstIdx(uint s){ this->FIRST_IDX = s; }
    void setDigits(uint s){ this->DIGITS = s; }
    void setExtension(string s){ this->EXTENSION = s; }
    void setSlices(uint s){ this->SLICES = s; }
    void setResolution(double s){ this->RESOLUTION = s; }
    void setVoxelDivision(uint s){ this->VOXEL_DIVISION = s; }
    void setPoreColor(uint s){ this->PORE_COLOR = s; }

    // -- Get methods
    uint getFirstIdx(){ return this->FIRST_IDX;}
    uint getDigits(){ return this->DIGITS;}
    string getExtension(){ return this->EXTENSION;}
    uint getSlices(){ return this->SLICES;} 
    double getResolution(){ return this->RESOLUTION;}
    uint getVoxelDivision(){ return this->VOXEL_DIVISION;}
    uint8_t getPoreColor(){ return this->PORE_COLOR;}
};

#endif
