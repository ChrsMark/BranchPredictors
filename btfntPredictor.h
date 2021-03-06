/*
 * btnfPredictor.h
 *
 * Branch Predictor backword-taken-forward-not-taken class
 * for Sniper Simulation
 *
 * < Chris Mark >
 *
 */


#ifndef BTFNT_PREDICTOR_H
#define BTFNT_PREDICTOR_H

#include "branch_predictor.h"
#include <cstring>
#include <iostream>

class btfntPredictor : public BranchPredictor
{
public:
   btfntPredictor (String name, core_id_t core_id)
      : BranchPredictor(name, core_id)
      
   {
      
   };
   ~btfntPredictor () 
   {
      
   };

   /**
    * Called from the Simulator to predict the result of branch instruction 
    * in address 'ip' and target 'target'. 
    **/
   bool predict(IntPtr ip, IntPtr target)
   {
      int help;
	if (target<ip) help=1;
	else help =0;
	
      int  prediction = help;

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
