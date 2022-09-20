#ifndef UNITTEST_H_
#define UNITTEST_H_

#include <iostream>
#include <string>
#include <vector>

#include "../Utils/ArgsParser.h"
#include "../ConfigFiles/configFiles_defs.h"
#include "../NMR_Simulation/NMR_Simulation.h"

// #include "UnitTest_includes.h"

using namespace std;

class UnitTest
{
public: 

	UnitTest(string proot);
	UnitTest(const UnitTest& otherTest)
	{
		this->projectRoot = otherTest.projectRoot;
		this->sucessResult = otherTest.sucessResult;
	}
	virtual ~UnitTest(){} 
	void runTest(string tag);
	void runAll();

	void setProjectRoot(string proot){ this->projectRoot = proot; }
	string getProjectRoot(){ return this->projectRoot; }
	void setSuccessResult(bool result){ this->sucessResult = result; }
	bool getSuccessResult(){ return this->sucessResult; }

private:
	string projectRoot;
	bool sucessResult;

};

#endif