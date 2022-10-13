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

void rwnmrApp::buildEssentials()
{
    cout << "--- Building RWNMR essentials" << endl;

    // -- Read NMR essentials config files  
    // -- rwnmr & uct image config
    string rwnmr_config_path;
    if((*this).getArgsPath(0) != "default") rwnmr_config_path = (*this).getArgsPath(0);
    else rwnmr_config_path = RWNMR_CONFIG_DEFAULT;
    
    string uct_config_path;
    if((*this).getArgsPath(1) != "default") uct_config_path = (*this).getArgsPath(1);
    else uct_config_path = UCT_CONFIG_DEFAULT;

    rwnmr_config rwNMR_Config((*this).getProjectRoot() + rwnmr_config_path, (*this).getProjectRoot());     
    uct_config uCT_Config((*this).getProjectRoot() + uct_config_path, (*this).getProjectRoot()); 
    // // -----

    // -- Create NMR_Simulation object
    // this->model = new Model(rwNMR_Config, uCT_Config, (*this).getProjectRoot());
    (*this).setModel(new Model(rwNMR_Config, uCT_Config, (*this).getProjectRoot()));
    // Read digital rock image
    cout << "-- Loading uCT-image" << endl;
    (*this).getModel().readImage();

    // Create and set up random walkers
    cout << endl << "-- Setting random walkers" << endl;
    (*this).getModel().initWalkers();

    // Save image info
    cout << endl << "-- Saving uCT-image info" << endl;
    (*this).getModel().save();
    cout << endl; (*this).getModel().info();
    // -----    
}

void rwnmrApp::exec()
{
    (*this).buildEssentials();

    uint commands = (*this).getArgs().getCommands().size();
    uint current = 2;
    while(current < commands)
    {
        if((*this).getArgsCommand(current) == "cpmg") (*this).CPMG(current);
        else if((*this).getArgsCommand(current) == "pfgse") (*this).PFGSE(current);
        else if((*this).getArgsCommand(current) == "ga") (*this).GA(current);
        else if((*this).getArgsCommand(current) == "multitau") (*this).MultiTau(current);

        current++;
    }
}

void rwnmrApp::CPMG(uint command_idx)
{
    cout << "-- CPMG to be executed:" << endl;
    // -- Read CPMG routine config files
    string cpmg_config_path;
    if((*this).getArgsPath(command_idx) != "default") cpmg_config_path = (*this).getArgsPath(command_idx);
    else cpmg_config_path = CPMG_CONFIG_DEFAULT;
    cpmg_config cpmg_Config((*this).getProjectRoot() + cpmg_config_path, (*this).getProjectRoot());
    // --

    // -- Create cpmg object
    NMR_cpmg cpmg((*this).getModel(), cpmg_Config);
    cpmg.run();
    cout << endl << "- cpmg executed succesfully" << endl << endl;
    // -----
}

void rwnmrApp::PFGSE(uint command_idx)
{
    cout << "-- PFGSE to be executed:" << endl;
    // -- Read PFGSE routine config files
    string pfgse_config_path;
    if((*this).getArgsPath(command_idx) != "default") pfgse_config_path = (*this).getArgsPath(command_idx);
    else pfgse_config_path = PFGSE_CONFIG_DEFAULT;
    pfgse_config pfgse_Config((*this).getProjectRoot() + pfgse_config_path, (*this).getProjectRoot());
    // --
    
    NMR_PFGSE pfgse((*this).getModel(), pfgse_Config);
    pfgse.run();
    cout << "- pfgse sequence executed succesfully" << endl << endl;
    // -----
}

void rwnmrApp::GA(uint command_idx)
{
    cout << "-- GA is under construction." << endl;
}

void rwnmrApp::MultiTau(uint command_idx)
{
    cout << "-- MultiTau to be executed:" << endl;

    // -- Read MultiTau routine config files
    string multitau_config_path;
    if((*this).getArgsPath(command_idx) != "default") multitau_config_path = (*this).getArgsPath(command_idx);
    else multitau_config_path = MULTITAU_CONFIG_DEFAULT;
    multitau_config multitau_Config((*this).getProjectRoot() + multitau_config_path, (*this).getProjectRoot());
    // --

    // -- Read CPMG routine config files
    string cpmg_config_path;
    if((*this).getArgsPath(command_idx + 1) != "default") cpmg_config_path = (*this).getArgsPath(command_idx + 1);
    else cpmg_config_path = CPMG_CONFIG_DEFAULT;
    cpmg_config cpmg_Config((*this).getProjectRoot() + cpmg_config_path, (*this).getProjectRoot());
    // --
    
    NMR_multitau multitau((*this).getModel(), multitau_Config, cpmg_Config);
    multitau.run();
    cout << "- multitau sequence executed succesfully" << endl << endl;
    // -----
}
