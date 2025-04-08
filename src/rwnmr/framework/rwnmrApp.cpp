// include class header file
#include "rwnmrApp.h"

rwnmrApp::rwnmrApp(int argc, char **argv, string _root) : projectRoot(_root), args(argc, argv), model(NULL)
{
    (*this).setConfigRoot((*this).getProjectRoot() + CONFIG_ROOT);   
}

rwnmrApp::rwnmrApp(ArgsParser _args, string _root) : projectRoot(_root), args(_args), model(NULL)
{
    (*this).setConfigRoot((*this).getProjectRoot() + CONFIG_ROOT); 
}

void rwnmrApp::buildEssentials(RwnmrConfig rwNMR_Config,UctConfig uCT_Config, std::vector<CustomMat>  image)
{
    cout << "--- Building RWNMR essentials" << endl;

 
    // -- Create NMR_Simulation object
    // this->model = new Model(rwNMR_Config, uCT_Config, (*this).getProjectRoot());
    (*this).setModel(new Model(rwNMR_Config, uCT_Config, image));
    // Read digital rock image
    cout << "-- Loading uCT-image" << endl;
    (*this).getModel().readImage();

    // Create and set up random walkers
    cout << endl << "-- Setting random walkers" << endl;
    (*this).getModel().initWalkers();

    // Create collision map and filtering
    if(rwNMR_Config.getMapTime() > 0.0)
    {
        cout << endl << "-- Creating and filtering collisions map" << endl;
        MapFilter mf((*this).getModel(), rwNMR_Config.getMapTime(), rwNMR_Config.getMapFilter(), rwNMR_Config.getMapTol());
        mf.run();
    } else if(rwNMR_Config.getMapSteps() > 0)
    {
        cout << endl << "-- Creating and filtering collisions map" << endl;
        MapFilter mf((*this).getModel(), rwNMR_Config.getMapSteps(), rwNMR_Config.getMapFilter(), rwNMR_Config.getMapTol(), rwNMR_Config.getMapIterations());   
        mf.run();
    }   


    // Save image info
    cout << endl << "-- Saving uCT-image info" << endl;
    (*this).getModel().save();
    cout << endl; (*this).getModel().info();
    // -----    
}

// void rwnmrApp::exec()
// {
//     (*this).buildEssentials();

//     uint commands = (*this).getArgs().getCommands().size();
//     uint current = 2;
//     while(current < commands)
//     {
//         if((*this).getArgsCommand(current) == "cpmg") (*this).CPMG(current);
//         else if((*this).getArgsCommand(current) == "pfgse") (*this).PFGSE(current);
//         else if((*this).getArgsCommand(current) == "ga") (*this).GA(current);
//         else if((*this).getArgsCommand(current) == "multitau") (*this).MultiTau(current);

//         current++;
//     }
// }

NMR_cpmg rwnmrApp::CPMG(CpmgConfig cpmg_config)
{
    cout << "-- CPMG to be executed:" << endl;



    NMR_cpmg cpmg((*this).getModel(), cpmg_config);
    cpmg.run();
    cout << endl << "- cpmg executed succesfully" << endl << endl;
    return cpmg;
    // -----
}

// void rwnmrApp::PFGSE(uint command_idx)
// {
//     cout << "-- PFGSE to be executed:" << endl;
//     // -- Read PFGSE routine config files
//     string PfgseConfig_path;
//     if((*this).getArgsPath(command_idx) != "default") PfgseConfig_path = (*this).getArgsPath(command_idx);
//     else PfgseConfig_path = PFGSE_CONFIG_DEFAULT;
//     PfgseConfig pfgse_Config((*this).getProjectRoot() + PfgseConfig_path, (*this).getProjectRoot());
//     // --
    
//     NMR_PFGSE pfgse((*this).getModel(), pfgse_Config);
//     pfgse.run();
//     cout << "- pfgse sequence executed succesfully" << endl << endl;
//     // -----
// }

// void rwnmrApp::GA(uint command_idx)
// {
//     cout << "-- GA is under construction." << endl;
// }

// void rwnmrApp::MultiTau(uint command_idx)
// {
//     cout << "-- MultiTau to be executed:" << endl;

//     // -- Read MultiTau routine config files
//     string MultitauConfig_path;
//     if((*this).getArgsPath(command_idx) != "default") MultitauConfig_path = (*this).getArgsPath(command_idx);
//     else MultitauConfig_path = MULTITAU_CONFIG_DEFAULT;
//     MultitauConfig multitau_Config((*this).getProjectRoot() + MultitauConfig_path, (*this).getProjectRoot());
//     // --

//     // -- Read CPMG routine config files
//     string CpmgConfig_path;
//     if((*this).getArgsPath(command_idx + 1) != "default") CpmgConfig_path = (*this).getArgsPath(command_idx + 1);
//     else CpmgConfig_path = CPMG_CONFIG_DEFAULT;
//     CpmgConfig cpmg_Config((*this).getProjectRoot() + CpmgConfig_path, (*this).getProjectRoot());
//     // --
    
//     NMR_multitau multitau((*this).getModel(), multitau_Config, cpmg_Config);
//     multitau.run();
//     cout << "- multitau sequence executed succesfully" << endl << endl;
//     // -----
// }
