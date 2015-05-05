/*
 * local_history_predictor.h
 *
 * Branch Predictor Class: local-history Predictor
 * for Sniper Simulation
 *
 * < Chris Mark >
 *
 */

#ifndef LOCAL_HISTORY_PREDICTOR_H
#define LOCAL_HISTORY_PREDICTOR_H

#include "branch_predictor.h"
#include <cstring>
#include <iostream>

class LocalHistoryPredictor : public BranchPredictor
{
public:
   LocalHistoryPredictor(String name, core_id_t core_id, 
                       unsigned int bht_ip_bits_, unsigned int pht_ip_bits_, unsigned int cntr_bits_, unsigned int history_bits_)
      : BranchPredictor(name, core_id)
      , bht_ip_bits(bht_ip_bits_)
      , pht_ip_bits(pht_ip_bits_)
      , cntr_bits(cntr_bits_)
      , history_bits(history_bits_)
   {
      pht_entries = 1 << (pht_ip_bits + history_bits);
      bht_size =  history_bits;
      bht_entries = 1 << bht_ip_bits;
      bht_ip_size = bht_ip_bits;
      pht_ip_size =  pht_ip_bits + history_bits;
      PHT = new unsigned long long [pht_entries];
      BHT = new unsigned long long [bht_entries];
      memset(PHT, 0, pht_entries * sizeof(*PHT));
      memset(BHT, 0, bht_entries * sizeof(*BHT));

      COUNTER_MAX = (1 << cntr_bits) - 1;
   };
   ~LocalHistoryPredictor() 
   {
      delete PHT;
      delete BHT;
   };

   /**
    * Called from the Simulator to predict the result of branch instruction 
    * in address 'ip' and target 'target'. 
    **/
   bool predict(IntPtr ip, IntPtr target)
   {
      unsigned int bht_ip = ip % bht_ip_size;
      unsigned int pht_ip = ip % pht_ip_bits;
      unsigned long long pht_value = PHT[BHT[bht_ip] + (pht_ip << history_bits)];
      unsigned long long prediction = pht_value >> (cntr_bits - 1);
      return (prediction != 0);
   };

   /**
    * Called from the Simulator to update the internal state of the predictor.
    **/
   void update(bool predicted, bool actual, IntPtr ip, IntPtr target)
   {
      unsigned int bht_ip = ip % bht_ip_size;
      unsigned int pht_ip = ip % pht_ip_bits;
      unsigned int history = BHT[bht_ip];
      unsigned long long pht_address = (history + (pht_ip << history_bits));
      if (actual) {
         if (PHT[pht_address] < COUNTER_MAX)
            PHT[pht_address]++;
	 history = history << 1;
	 history = history % bht_size;
	 history++;
      } else {
         if (PHT[pht_address] > 0)
            PHT[pht_address]--;
	 history = history << 1;
	 history = history % bht_size;
      }

      BHT[bht_ip] = history;
      updateCounters(predicted, actual);
   };

private:
   unsigned int bht_ip_bits, pht_ip_bits, cntr_bits, history_bits;
   unsigned int COUNTER_MAX;

   /* Make this unsigned long long so as to support big numbers of cntr_bits. */
   unsigned long long *PHT, *BHT;
   unsigned int bht_size, bht_entries, pht_entries, pht_ip_size,bht_ip_size;
};

#endif
