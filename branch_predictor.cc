/*
 * branch_predictor.cc
 *
 * Branch Predictor Class
 * for Sniper Simulation
 *
 * < Chris Mark >
 *
 */

#include "simulator.h"
#include "branch_predictor.h"
#include "one_bit_branch_predictor.h"
#include "pentium_m_branch_predictor.h"
#include "config.hpp"
#include "stats.h"




/* advcomparch: include file with nbit predictor. */
#include "NotTaken.h"
#include "btfntPredictor.h"
#include "nbit_branch_predictor.h"
#include "local_history_predictor.h"
#include "global_history_predictor.h"
#include "tournament_predictor.h"
#include "tournament_predictor_2.h"

BranchPredictor::BranchPredictor()
   : m_correct_predictions(0)
   , m_incorrect_predictions(0)
{
}

BranchPredictor::BranchPredictor(String name, core_id_t core_id)
   : m_correct_predictions(0)
   , m_incorrect_predictions(0)
{
  registerStatsMetric(name, core_id, "num-correct", &m_correct_predictions);
  registerStatsMetric(name, core_id, "num-incorrect", &m_incorrect_predictions);
}

BranchPredictor::~BranchPredictor()
{ }

UInt64 BranchPredictor::m_mispredict_penalty;

BranchPredictor* BranchPredictor::create(core_id_t core_id)
{
   try
   {
      config::Config *cfg = Sim()->getCfg();  
      assert(cfg);

      m_mispredict_penalty = cfg->getIntArray("perf_model/branch_predictor/mispredict_penalty", core_id);

      String type = cfg->getStringArray("perf_model/branch_predictor/type", core_id);
      if (type == "none")
      {
         return 0;
      }
      else if (type == "one_bit")
      {
         UInt32 size = cfg->getIntArray("perf_model/branch_predictor/size", core_id);
         return new OneBitBranchPredictor("branch_predictor", core_id, size);
      }
      else if (type == "pentium_m")
      {
         return new PentiumMBranchPredictor("branch_predictor", core_id);
      }
      else if (type == "local")
	{
	UInt32 bht_ip_bits = cfg->getIntArray("perf_model/branch_predictor/bht_ip_bits", core_id);
        UInt32 cntr_bits = cfg->getIntArray("perf_model/branch_predictor/cntr_bits", core_id);
	UInt32 pht_ip_bits = cfg->getIntArray("perf_model/branch_predictor/pht_ip_bits", core_id);
        UInt32 history_bits = cfg->getIntArray("perf_model/branch_predictor/history_bits", core_id);
	
	return new LocalHistoryPredictor("branch_predictor", core_id, bht_ip_bits,  pht_ip_bits, cntr_bits, history_bits);
	}
	else if (type == "global")
	{
	UInt32 bhr_length = cfg->getIntArray("perf_model/branch_predictor/bhr_length", core_id);
        UInt32 cntr_bits = cfg->getIntArray("perf_model/branch_predictor/cntr_bits", core_id);
	UInt32 pht_entries = cfg->getIntArray("perf_model/branch_predictor/pht_entries", core_id);
	
	return new GlobalHistoryPredictor ("branch_predictor", core_id, bhr_length,  pht_entries, cntr_bits);
	}
      	else if (type == "static_not_taken")
	{
	return new NotTakenPredictor("branch_predictor", core_id);

	}
	else if (type == "btfnt")
	{
	return new btfntPredictor("branch_predictor", core_id);
	}
	else if (type == "nbit")
      {
         /* advcomparch: create new NbitBranchPredictor */
         UInt32 index_bits = cfg->getIntArray("perf_model/branch_predictor/index_bits", core_id);
         UInt32 cntr_bits = cfg->getIntArray("perf_model/branch_predictor/cntr_bits", core_id);
         return new NbitBranchPredictor("branch_predictor", core_id, index_bits, cntr_bits);
      }	
	else if (type == "Tournament")
	{
	 UInt32 index_bits = cfg->getIntArray("perf_model/branch_predictor/index_bits", core_id);
        UInt32 cntr_bits = cfg->getIntArray("perf_model/branch_predictor/cntr_bits", core_id);
         return new TournamentBranchPredictor("branch_predictor", core_id, index_bits, cntr_bits);
	
	}
	else if (type == "Tournament2")
	{
	 UInt32 index_bits = cfg->getIntArray("perf_model/branch_predictor/index_bits", core_id);
        UInt32 cntr_bits = cfg->getIntArray("perf_model/branch_predictor/cntr_bits", core_id);
         return new TournamentBranchPredictor2("branch_predictor", core_id, index_bits, cntr_bits);
	
	}

      else
      {
         LOG_PRINT_ERROR("Invalid branch predictor type.");
         return 0;
      }
   }
   catch (...)
   {
      LOG_PRINT_ERROR("Config info not available while constructing branch predictor.");
      return 0;
   }

   return 0;
}

UInt64 BranchPredictor::getMispredictPenalty()
{
   return m_mispredict_penalty;
}

void BranchPredictor::resetCounters()
{
  m_correct_predictions = 0;
  m_incorrect_predictions = 0;
}

void BranchPredictor::updateCounters(bool predicted, bool actual)
{
   if (predicted == actual)
      ++m_correct_predictions;
   else
      ++m_incorrect_predictions;
}
