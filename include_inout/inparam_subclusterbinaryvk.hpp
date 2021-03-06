/*! \file inparam_subclusterbinaryvk.hpp
 *
 * \brief Definition of CLUSTERING program parameters
 *
 * \version 1.0
 * \date 2015-2017
 * \authors Hermes Robles <hermes@uaz.edu.mx>\n Sebastian Ventura <sventura@uco.es>\n Amelia Zafra <azafra@uco.es>
 * \copyright <a href="https://www.gnu.org/licenses/gpl-3.0.en.html">GPLv3</a> license
 */

#ifndef __IN_PARAM_CLUSTERING_VKSUBCLUSTERBINARYVK_HPP__
#define __IN_PARAM_CLUSTERING_VKSUBCLUSTERBINARYVK_HPP__

#include "inparam_pcpm.hpp"
#include "inparam_readinst.hpp"

#define __INPARAM_SUBCLUSTERBINARYVK__

/*! \namespace inout
  \brief Module for input and output parameters
  \details
  
  \version 1.0
  \date   2015-2017
  \copyright GPLv3 license
*/

namespace  inout {
    
/*! \class InParamSubClusterBinaryVk
  \brief Input parameter for CLUSTERING algorithm A genetic approach to the automatic clustering problem\cite Tseng:Yang:GAclusteringVarK:CLUSTERING:2001
*/
  template < typename T_REAL,
	     typename T_BITSIZE,
	     typename T_CLUSTERIDX,
	     typename T_FEATURE,         
	     typename T_FEATURE_SUM,
	     typename T_INSTANCES_CLUSTER_K
	     >
class InParamSubClusterBinaryVk
    : public InParamPcPm<T_REAL>
    , public InParamReadInst<T_FEATURE,T_INSTANCES_CLUSTER_K,T_CLUSTERIDX>
{
public:
  InParamSubClusterBinaryVk
  (const std::string& ais_algorithmoName,
   const std::string& ais_algorithmoAuthor,
   InParam_algTypeOut aiato_algTypeOut,
   int                aii_opNorm
   )
    : InParamPcPm<T_REAL>
      (ais_algorithmoName,ais_algorithmoAuthor,aiato_algTypeOut, aii_opNorm)
    , InParamReadInst<T_FEATURE,T_INSTANCES_CLUSTER_K,T_CLUSTERIDX>()
  {}

  ~InParamSubClusterBinaryVk() {}

  inline void setU(T_REAL aitr_u) 
  {
    this->tr_u = aitr_u;
  }

  inline T_REAL getU() 
  {
    return this->tr_u;
  }

  inline void setLambda(T_REAL aitr_lambda) 
  {
    this->tr_lambda = aitr_lambda;
  }

  inline T_REAL getLambda() 
  {
    return this->tr_lambda;
  }

  inline void setW1(T_REAL aitr_w1) 
  {
    this->tr_w1 = aitr_w1;
  }

  inline T_REAL getW1() 
  {
    return this->tr_w1;
  }

  inline void setW2(T_REAL aitr_w2) 
  {
    this->tr_w2 = aitr_w2;
  }

  inline T_REAL getW2() 
  {
    return this->tr_w2;
  }


  virtual void print(std::ostream&  aipf_outFile=std::cout, const char aic_separator=',') const
  {
    InParamPcPm<T_REAL>::print(aipf_outFile,aic_separator);
    InParamReadInst<T_FEATURE,T_INSTANCES_CLUSTER_K,T_CLUSTERIDX>::print(aipf_outFile,aic_separator);
    aipf_outFile << aic_separator << "_u"   
		 << aic_separator << this->tr_u;
    aipf_outFile << aic_separator << "_lambda" 
		 << aic_separator << this->tr_lambda;
    aipf_outFile << aic_separator << "_w1" 
		 << aic_separator << this->tr_w1;
    aipf_outFile << aic_separator << "_w2" 
		 << aic_separator << this->tr_w2;
  }

protected:

  T_REAL  tr_u;
  T_REAL  tr_lambda;
  T_REAL  tr_w1;
  T_REAL  tr_w2;
  
}; /*InParamSubClusterBinaryVk*/


} /*END namespace inout 
   */

#endif /*__IN_PARAM_CLUSTERING_VKSUBCLUSTERBINARY_HPP__*/
