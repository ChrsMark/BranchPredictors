/*
 * tournament_predictor_2.h
 *
 * Branch Predictor Class : tournament Predictor
 * for Sniper Simulation
 *
 * < Chris Mark >
 *
 */

#ifndef TOURNAMENT_PREDICTOR_2_H
#define TOURNAMENT_PREDICTOR_2_H

#include "branch_predictor.h"
#include <cstring>
#include <iostream>
#include "local_history_predictor.h"
#include "global_history_predictor.h"
#include "nbit_branch_predictor.h"



class TournamentBranchPredictor2 : public BranchPredictor
{
public:
   TournamentBranchPredictor2(String name, core_id_t core_id, 
                       unsigned int index_bits_, unsigned int cntr_bits_)
      : BranchPredictor(name, core_id)
      , index_bits(index_bits_)
      , cntr_bits(cntr_bits_)
   {
      table_entries = 1 << index_bits;
      TABLE = new unsigned long long[table_entries];
      memset(TABLE, 0, table_entries * sizeof(*TABLE));
	  bht_ip_bits=11;
	  pht_ip_bits=12;
	  cntr_bits2=2;
	  history_bits=4;		
	  myLocal = new LocalHistoryPredictor(name,  core_id, bht_ip_bits,  pht_ip_bits,  cntr_bits2,   history_bits);
	 pht_entries = 8192;
	 cntr_bits2 = 2;
		bhr_length = 4;
          myGlobal = new GlobalHistoryPredictor ( name,  core_id, bhr_length,  pht_entries,cntr_bits2);

      COUNTER_MAX = (1 << cntr_bits) - 1;
   };
   ~TournamentBranchPredictor2() 
   {
      delete TABLE;
   };

   /**
    * Called from the Simulator to predict the result of branch instruction 
    * in address 'ip' and target 'target'. 
    **/
   bool predict(IntPtr ip, IntPtr target)
   {
      unsigned int ip_table_index = ip % table_entries;
      unsigned long long ip_table_value = TABLE[ip_table_index];
       prediction = ip_table_value >> (cntr_bits - 1);
	
	if (prediction==1) {
	
       
           final_prediction =myLocal->predict(ip,target);
		temp = myGlobal->predict(ip,target);


      } else {
	   final_prediction =myGlobal->predict(ip,target);
		temp =myLocal->predict(ip,target);
 

      }
	
      return (final_prediction != 0);
   };

   /**
    * Called from the Simulator to update the internal state of the predictor.
    **/
   void update(bool predicted, bool actual, IntPtr ip, IntPtr target)
   {
      
       unsigned int ip_table_index = ip % table_entries;

	if( final_prediction!=temp){
		if ((final_prediction == actual)&&(prediction)){
				if (TABLE[ip_table_index] < COUNTER_MAX)
            				TABLE[ip_table_index]++;

				}
		else if	((final_prediction == actual)&&(prediction==0)){
						if (TABLE[ip_table_index] > 0)
            					    TABLE[ip_table_index]--;
				}
	}
            
	myLocal->update(predicted, actual, ip, target);
	 myGlobal->update(predicted, actual, ip, target);
      
      updateCounters(predicted, actual);
   };

private:
   unsigned int index_bits, cntr_bits;
   unsigned int COUNTER_MAX;
   unsigned int bht_ip_bits ,pht_ip_bits,  cntr_bits2,  history_bits;
   unsigned int pht_entries,bhr_length;
	unsigned long long final_prediction;
	unsigned long long prediction;
	unsigned int temp;
   /* Make this unsigned long long so as to support big numbers of cntr_bits. */
   unsigned long long *TABLE;
   unsigned int table_entries;
	LocalHistoryPredictor*   myLocal;
	GlobalHistoryPredictor * myGlobal;

};

#endif
