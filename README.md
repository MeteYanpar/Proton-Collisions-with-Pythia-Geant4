                   ************************************************************

                    =========================================================
                  Geant4 - an Object-Oriented Toolkit for Simulation in HEP
                    =========================================================
                   *************************************************************

       This example demonstrates how to generate proton–proton collision events 
      using Pythia8, store them in HepMC3 format, simulate detector interactions 
      with Geant4, and optionally analyze results with ROOT.

1- GEOMETRY DEFINITION

        I tried to build the cern cms detector at detector.construction.cc. 
     I know it's far from the real thing, but I tried to make it as similar 
     as I could.

 2- PHYSICS LIST

       The particle's type and the physic processes which will be available
     in this example are set in the FTFP_BERT_HP physics list. This physics list
     requires data files for electromagnetic and hadronic processes.

3- ACTION INITALIZATION

       This file initiates and manages user actions for the Geant4 simulation.

4- PRIMARY GENERATOR ACTION

      It determines the initial conditions of the particles by reading the 
     proton_collision13TeV.hepmc3 file generated with Pythia8.
       
