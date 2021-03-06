/*
 * NotTaken.h
 *
 * Branch Predictor Class: not-taken branch Predictor
 * for Sniper Simulation
 *
 * < Chris Mark >
 *
 */

#ifndef NOT_TAKEN_H
#define NOT_TAKEN_H

#include "branch_predictor.h"
#include <cstring>
#include <iostream>

class NotTakenPredictor : public BranchPredictor
{
public:
   NotTakenPredictor(String name, core_id_t core_id)
      : BranchPredictor(name, core_id)
      
   {
         };
   ~NotTakenPredictor() 
   {
      
   };

   /**
    * Called from the Simulator to predict the result of branch instruction 
    * in address 'ip' and target 'target'. 
    **/
   bool predict(IntPtr ip, IntPtr target)
   {
      
      unsigned long long prediction = 0;
      return (prediction != 0);
   };

   /**
    * Called from the Simulator to update the internal state of the predictor.
    **/
   void update(bool predicted, bool actual, IntPtr ip, IntPtr target)
   {
     
      
      updateCounters(predicted, actual);
   };


};

#endif
