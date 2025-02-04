#include "UctConfig.h"

using namespace std;

// default constructors
UctConfig::UctConfig() : BaseConfig(), PORE_COLOR(0)
{
}

//copy constructors
UctConfig::UctConfig(const UctConfig &otherConfig)
{
	this->DIR_PATH = otherConfig.DIR_PATH;
    this->FILENAME = otherConfig.FILENAME;
    this->FIRST_IDX = otherConfig.FIRST_IDX;
    this->DIGITS = otherConfig.DIGITS;
    this->EXTENSION = otherConfig.EXTENSION;
    this->SLICES = otherConfig.SLICES;
    this->RESOLUTION = otherConfig.RESOLUTION;
    this->VOXEL_DIVISION = otherConfig.VOXEL_DIVISION;
    this->PORE_COLOR = otherConfig.PORE_COLOR;
	this->IMG_FILES_LIST = otherConfig.IMG_FILES_LIST;
	this->IMG_FILES = otherConfig.IMG_FILES;
}

// read config file
void UctConfig::setConfig(const string dirPath, const string filename, const string firstIdx, const string digits, const string extension, const string slices, const string resolution, const string voxelDivision, const string poreColor)
{
    readDirPath(dirPath);
    readFilename(filename);
    readFirstIdx(firstIdx);
    readDigits(digits);
    readExtension(extension);
    readSlices(slices);
    readResolution(resolution);
    readVoxelDivision(voxelDivision);
    readPoreColor(poreColor);
}

void UctConfig::readDirPath(string s)
{
    if(s.length() > 2 and s.substr(0,2) == "./") (*this).setDirPath((*this).getProjectRoot() + s);
	else (*this).setDirPath(s);
}

void UctConfig::readFilename(string s)
{
	this->FILENAME = s;
}

void UctConfig::readFirstIdx(string s)
{
	this->FIRST_IDX = std::stoi(s);
}

void UctConfig::readDigits(string s)
{
	this->DIGITS = std::stoi(s);
}

void UctConfig::readExtension(string s)
{
	this->EXTENSION = s;
}

void UctConfig::readSlices(string s)
{
	this->SLICES = std::stoi(s);
}

void UctConfig::readResolution(string s)
{
	this->RESOLUTION = std::stod(s);
}

void UctConfig::readVoxelDivision(string s)
{
	this->VOXEL_DIVISION = std::stoi(s);
}

void UctConfig::readPoreColor(string s)
{
    uint8_t color = (uint8_t) std::stoi(s);
	if(color > 255) this->PORE_COLOR = 255;
    else this->PORE_COLOR = color;
}

void UctConfig::readImgFilesList(string s)
{
	this->IMG_FILES_LIST = s;
}

void UctConfig::readImgFiles()
{
	const string filepath = (*this).getImgFilesList();
	
    ifstream fileObject;
    fileObject.open(filepath, ios::in);
    if (fileObject.fail())
    {
        cout << "Could not open file from disc." << endl;
        exit(1);
    }

	// reserve memory for image file list
	if(this->IMG_FILES.size() > 0) this->IMG_FILES.clear();
	this->IMG_FILES.reserve((*this).getSlices());

    string line;
	uint slice = 0;
    while(fileObject)
    {
    	getline(fileObject, line);
		// cout << line << endl;
    	if(slice < (*this).getSlices()) this->IMG_FILES.push_back(line);
		slice++;
    } 

    fileObject.close();
}

void UctConfig::createImgFileList()
{
    string dirpath = (*this).getProjectRoot() + CONFIG_ROOT;
	string filepath = dirpath + ".ImagesList.txt";

    ofstream fileObject;
    fileObject.open(filepath, ios::out);
    if (fileObject.fail())
    {
        cout << "Could not open images list file from disc." << endl;
        exit(1);
    }

	// constant strings
    string dirname = (*this).getDirPath();
    string fname = (*this).getFilename();
    uint firstId = (*this).getFirstIdx();
    uint digits = (*this).getDigits();
	uint slices = (*this).getSlices();
    string extension = (*this).getExtension();
	ImagePath paths(dirname, fname, firstId, digits, slices, extension);

    // variable strings
    string currentId;
    string currentImagePath;    

    for (uint slice = 0; slice < slices; slice++)
    {
        fileObject << paths.getImagePathFromList(slice) << endl;
    }	  

	(*this).readImgFilesList(filepath);    
}


