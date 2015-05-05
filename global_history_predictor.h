/*
 * branch_predictor.h
 *
 * Branch Predictor Class: global-history Predictor
 * for Sniper Simulation
 *
 * < Chris Mark >
 *
 */

#ifndef GLOBAL_HISTORY_PREDICTOR_H
#define GLOBAL_HISTORY_PREDICTOR_H

#include "branch_predictor.h"
#include <cstring>
#include <iostream>
#include <math.h>
#include <stdio.h>      
#include <stdlib.h>

class GlobalHistoryPredictor : public BranchPredictor
{
public:
   GlobalHistoryPredictor (String name, core_id_t core_id, 
                       unsigned int bhr_length_, unsigned int pht_entries_, unsigned int cntr_bits_)
      : BranchPredictor(name, core_id)
      , bhr_length(bhr_length_)
      , pht_entries(pht_entries_)
      , cntr_bits(cntr_bits_)
      
   {
      pht_columns = 1 << (bhr_length);
	div_t help = div(pht_entries,pht_columns);
	printf(" pht_entries are %d and pht_columns are %d and help is %d\n",pht_entries,pht_columns,help.quot);
		pht_lines =  (help.quot);
	
      if (pht_lines==1) printf("ok!\n\n\n\n\n\n\n\n\n\\n\n\n\n\n\n");
      pht_line_ip_bits = log2(pht_lines) ;
      
	 PHT = new unsigned int*[pht_lines];
		for(unsigned int i = 0; i < pht_lines; ++i)
    		PHT [i] = new unsigned int[pht_columns];

             
            

      COUNTER_MAX = (1 << cntr_bits) - 1;
   };
   ~GlobalHistoryPredictor () 
   {
     delete PHT;
      
   };

   /**
    * Called from the Simulator to predict the result of branch instruction 
    * in address 'ip' and target 'target'. 
    **/
   bool predict(IntPtr ip, IntPtr target)
   {
      unsigned int pht_column = BHR ;
      unsigned int pht_line = ip % pht_line_ip_bits;
      unsigned long long pht_value = PHT[pht_line][pht_column];
      unsigned long long prediction = pht_value >> (cntr_bits - 1);
      return (prediction != 0);
   };

   /**
    * Called from the Simulator to update the internal state of the predictor.
    **/
   void update(bool predicted, bool actual, IntPtr ip, IntPtr target)
   {
       unsigned int pht_column = BHR ;
      unsigned int pht_line = ip % pht_line_ip_bits;
      unsigned int history = BHR;
      
      if (actual) {
         if (PHT[pht_line][pht_column] < COUNTER_MAX)
            PHT[pht_line][pht_column]++;
	 history = history << 1;
	 history = history % bhr_length;
	 history++;
      } else {
         if (PHT[pht_line][pht_column] > 0)
            PHT[pht_line][pht_column]--;
	 history = history << 1;
	 history = history % bhr_length;
      }

      BHR = history;
      updateCounters(predicted, actual);
   };

private:
   unsigned int bhr_length, pht_entries, cntr_bits;
   unsigned int COUNTER_MAX;

   /* Make this unsigned long long so as to support big numbers of cntr_bits. */
   
   unsigned int BHR,pht_columns, pht_lines, pht_line_ip_bits ;
   unsigned int **PHT;	
};

#endif
