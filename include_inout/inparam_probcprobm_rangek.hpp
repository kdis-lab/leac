/*! \file inparam_probcprobm_rangek.hpp
 *
 * \brief Definition of input parameters
 *
 * \version 1.0
 * \date 2015-2017
 * \authors Hermes Robles <hermes@uaz.edu.mx>\n Sebastian Ventura <sventura@uco.es>\n Amelia Zafra <azafra@uco.es>
 * \copyright <a href="https://www.gnu.org/licenses/gpl-3.0.en.html">GPLv3</a> license
 */

#ifndef __IN_PROBC_PROBM_RANGEK_HPP__
#define __IN_PROBC_PROBM_RANGEK_HPP__

#include "inparam_probcprobm.hpp"
#include "inparam_rangek.hpp"
#include "inparam_readinst.hpp"


/*! \namespace inout
  \brief Module for input and output parameters
  \details
  
  \version 1.0
  \date   2015-2017
  \copyright GPLv3 license
*/

namespace  inout {

/*! \class InParamPcPmRk
  \brief Input parameter for GA with probability Pc, Pm and Range K 
*/
template < typename T_CLUSTERIDX, //-1, 0, 1, .., K
           typename T_REAL,
	   typename T_FEATURE,         
	   typename T_FEATURE_SUM,
	   typename T_INSTANCES_CLUSTER_K
	   >
class InParamPcPmRk
  : public InParamPcPm<T_REAL>
  , public InParamRk<T_CLUSTERIDX>
  , public InParamReadInst<T_FEATURE,T_INSTANCES_CLUSTER_K,T_CLUSTERIDX>
{
public:
  InParamPcPmRk
  (const std::string& ais_algorithmoName,
   const std::string& ais_algorithmoAuthor,
   InParam_algTypeOut aiato_algTypeOut,
   int         aii_opNorm
   ) 
    : InParamPcPm<T_REAL>
      (ais_algorithmoName,ais_algorithmoAuthor,aiato_algTypeOut, aii_opNorm) 
    , InParamRk<T_CLUSTERIDX>()
    , InParamReadInst<T_FEATURE,T_INSTANCES_CLUSTER_K,T_CLUSTERIDX>()
  {}

  ~InParamPcPmRk() {}

  virtual void print(std::ostream&  aipf_outFile=std::cout, const char aic_separator=',') const
  {
    InParamPcPm<T_REAL>::print(aipf_outFile,aic_separator);
    InParamRk<T_CLUSTERIDX>::print(aipf_outFile,aic_separator);
    InParamReadInst<T_FEATURE,T_INSTANCES_CLUSTER_K,T_CLUSTERIDX>::print(aipf_outFile,aic_separator);
  }  
};

} /*END namespace inout 
   */

#endif /*__IN_PROBC_PROBM_RANGEK_HPP__*/
