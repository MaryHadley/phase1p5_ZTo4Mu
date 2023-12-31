//How to run
// root -b -l -q 'phase1p5_ZTo4Mu.C++("<nameOfFile>.root")'

#include <vector>
#include <iostream>
#include <string>
using namespace std;
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TLorentzVector.h"

#include <math.h>
#include <TMath.h>

TFile *root_file;
#include "tree.C"
#include "treeMC.C"

tree *TREE;
treeMC *TREEMC;


void phase1p5_ZTo4Mu (string file){

  root_file = TFile::Open(file.c_str(),"READ");
  TREE   = new tree((TTree*)root_file->Get("tree"));
  TREEMC = new treeMC((TTree*)root_file->Get("treemc"));
   
//   double dummyBranch; //Must declare it here, thanks to Taeun for clearing this up for me :-)
   bool truth_eventHasZTo4Mu;
   
   
   
   
   
   TFile *file2 = TFile::Open("myNewFile_ZTo4Mu.root", "RECREATE");
   TTree* tree2 = TREE->fChain->CloneTree(0); //Clone this with the 0 so you get a skeleton of the tree with all the right branches but no entries
   //later you will call Fill() on this tree and because of the magic of CloneTree, it will restore the information that was in each branch of the original tree
   //this is the tree that you are going to add a branch to, hence the reason for filling it in the loop

//   std::cout << "tree2:  " << std::endl;
//   tree2->Print();

   TTree* treemc2 = TREEMC->fChain->CloneTree(); //Clone this without the zero because you want a clone of the treemc and all its entries since you aren't going
   //to touch it within the loop, you're not adding any branchees to this treemc, you want it back exactly as it was
   
//   tree2->Branch("dummyBranch", &dummyBranch, "dummyBranch/D");
   tree2->Branch("truth_eventHasZTo4Mu", &truth_eventHasZTo4Mu, "truth_eventHasZTo4Mu/O"); //O (the letter, not the number 0) for booleans, see: https://root.cern.ch/doc/master/classTTree.html#addcolumnoffundamentaltypes (thanks to Kevin Pedro for pointing me to this link)
   
   
   
  //Announce what root_file is
  std::cout << "////////////////////////////////////////" << std::endl;
  std::cout << "Processing file:  " << file.c_str() << std::endl;
  std::cout << "////////////////////////////////////////" << std::endl;
  
  int my_counter = 0;
  int entries = (TREE->fChain)->GetEntries();
  int my_entriesMC = (TREEMC->fChain)->GetEntries();
  std::cout << "entries:  " << entries << std::endl;
  std::cout << "entriesMC:  " << my_entriesMC << std::endl;
  
//   
//   for(int iEntry=0; iEntry<entries; iEntry++) { // to be commented out 
//     (TREE->fChain)->GetEntry(iEntry);
//     
//     int entriesMC = (TREEMC->fChain)->GetEntries();
//     for(int jEntry=0; jEntry<entriesMC; jEntry++) {
//         (TREEMC->fChain)->GetEntry(jEntry);
//          // std::cout << "iEntry is: " << iEntry << std::endl;
//          // std::cout << "jEntry is: " << jEntry << std::endl;
//          // std::cout << TREE->event_number_of_event->at(0) << std::endl;
//           if (TREE->run_number_of_event->at(0) == TREEMC->mc_run_number->at(0) && TREE->event_number_of_event->at(0) == TREEMC->mc_event_number->at(0) && TREE->lumi_section_of_event->at(0) == TREEMC->mc_lumi_section->at(0)){
//             //std::cout << "SUCCESS" << std::endl;
//             //std::cout << "########################" << std::endl;
//             my_counter++;
//            // dummyBranch =  -99;
//             denominator_ZplusY = TREEMC->denominator_ZplusY->at(0);
//             tree2->Fill();
//             break;
//             
//           }
//           
//           
//     } // up to here
// 
//     // map<std::string, bool> tree_mc_zplusy;  // create look up table
//     //
//     // Loop on treeMC once first
//     // 
//     // for(int jEntry=0; jEntry<entriesMC; jEntry++) {
//     // (TREEMC->fChain)->GetEntry(jEntry);
//     // std::string my_element; // your element should look like event_run_lumi  tree_mc_zplusy["event_run_lumi"] -> gives you the boolean
//     // my_element = std::to_string(TREEMC->event_number_of_event->at(0)) + string("_") + std::to_string(TREEMC->mc_run_number->at(0)) + string("_") + std::to_string(TREEMC->mc_lumi_section->at(0));
//     //
//     //
//     // 
//     //tree_mc_zpluy.inster({my_element, TREEMC->denominator_ZplusY->at(0) }); // here you fill look up table for every element of tree MC (event, run and lumi section)
//     //} // end of your treeMC loop and you created a look-up table called tree_mc_zplusy
//     //
//     //Loop on tree
//     //for(int iEntry=0; iEntry<entries; iEntry++) {
//     // (TREE->fChain)->GetEntry(iEntry);
//     // std:string look_element = std::to_string(TREE->event_number_of_event->at(0)) + string("_") + std::to_string(TREE->mc_run_number->at(0)) + string("_") + std::to_string(TREE->mc_lumi_section->at(0));
//     // denominator_ZplusY = tree_mc_zplusy[look_element]; // you read look-up table for given event, run, lumi section from the tree 
//     // tree2->Fill();
//     //} // end loop 
//     //
//                 
//   
//   }
       
       std::map<std::string, bool> tree_mc_truth_eventHasZTo4Mu; //Create look-up table here
       
       
       int entriesMC = (TREEMC->fChain)->GetEntries();
       for(int jEntry=0; jEntry<entriesMC; jEntry++) {
         (TREEMC->fChain)->GetEntry(jEntry);
         std::string my_element; //element will look like run_event_lumi, tree_mc_zplusy["run_event_lumi"] --> gives you the boolean
         my_element = std::to_string(TREEMC->mc_run_number->at(0)) + std::string("_") + std::to_string(TREEMC->mc_event_number->at(0)) + std::string("_") + std::to_string(TREEMC->mc_lumi_section->at(0));
         //std::cout << "my_element:  " << my_element << std::endl;
         tree_mc_truth_eventHasZTo4Mu.insert({ my_element, TREEMC->truth_eventHasZTo4Mu->at(0) }); //Fill the look up table for every event of treemc with run_event_lumi and the boolean value
         
         
         //Stuff I tried that didn't work...
         //tree_mc_truth_Zmuon_pt.insert({ my_element, TREEMC->truth_Zmuon_pt->at(0) });
         //tree_mc_truth_Zmuon_pt[my_element].push_back(TREEMC->truth_Zmuon_pt->at(1));
        // tree_mc_truth_Zmuon_pt.insert(std::make_pair(my_element, TREEMC->truth_Zmuon_pt));
        
        
        
         //std::cout << "tree_mc_truth_Zmuon_pt[my_element].size():  " << tree_mc_truth_Zmuon_pt[my_element].size() << std::endl;
       } //End of loop over treemc and we have filled the look-up tables called tree_mc_truth_eventHasZTo4Mu
          
       //Now loop on tree
       for(int iEntry=0; iEntry<entries; iEntry++) {
         (TREE->fChain)->GetEntry(iEntry);
         std::string look_up_element;
         look_up_element = std::to_string(TREE->run_number_of_event->at(0)) + std::string("_") + std::to_string(TREE->event_number_of_event->at(0)) + std::string("_") + std::to_string(TREE->lumi_section_of_event->at(0)); //Pattern is run_event_lumi, just like in our look-up table tree_mc_zplusy
       //  std::cout << "look_up_element:  " << look_up_element << std::endl; 
       truth_eventHasZTo4Mu = tree_mc_truth_eventHasZTo4Mu[look_up_element]; //Here you read the look-up table for a given run_event_lumi that is in tree. You're exploiting the fact here that treemc has an entry for truth_eventHasZTo4Mu
       //for every event and therefore contains all the run_event_lumi's that are in tree, plus of course a bunch of others that are not in tree. Another way to say this is that
       //tree contains a subset of the run_event_lumi's that are in treemc.
       
       
       
       //std::cout << "truth_Zmuon_pt.at(0):  " << truth_Zmuon_pt.at(0) << std::endl; 
       tree2->Fill();
       
       } //End loop over tree
       
       
  
//  TFile *file2 = TFile::Open("myNewFile.root", "RECREATE"); //Do NOT put this here, you need to put it up above, before you declare the tree that is associated with it
//Order is declare file, then declare tree //If you do it wrong, ROOT actually throws a reasonably useful error message that allowed me to deduce that it needed to be
//file, then tree
  file2->cd();
  tree2->Write();
  treemc2->Write();
  file2->Close();
  
//  std::cout << "my_counter:  " << my_counter << std::endl;
}
