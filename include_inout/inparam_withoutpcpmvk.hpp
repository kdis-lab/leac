/*! \file inparam_withoutpcpmvk.hpp
 *
 * \brief Definition of GA program parameters
 *
 * \version 1.0
 * \date 2015-2017
 * \authors Hermes Robles <hermes@uaz.edu.mx>\n Sebastian Ventura <sventura@uco.es>\n Amelia Zafra <azafra@uco.es>
 * \copyright <a href="https://www.gnu.org/licenses/gpl-3.0.en.html">GPLv3</a> license
 */

#ifndef __IN_PARAM_WITHOUT_PCPMVK_HPP__
#define __IN_PARAM_WITHOUT_PCPMVK_HPP__

#include "inparam_variablek.hpp"
#include "inparam_gaclustering.hpp"
#include "inparam_readinst.hpp"

#define __INPARAM_WITHOUTPCPMVK__

/*! \namespace inout
  \brief Module for input and output parameters
  \details
  
  \version 1.0
  \date   2015-2017
  \copyright GPLv3 license
*/

namespace  inout {
  
/*! \class InParamWithoutPcPmVk
  \brief Input parameter for GA without probability crossover (Pc) and mutation (Pm)\cite Hruschka:Ebecken:GAClusteringLabelKVar:CGA:2003
*/
template < typename T_CLUSTERIDX, //-1, 0, 1, .., K
	   typename T_REAL,
	   typename T_FEATURE,
	   typename T_FEATURE_SUM,
	   typename T_INSTANCES_CLUSTER_K
	  > 
class InParamWithoutPcPmVk
  : public InParamGAClustering
  , public InParamVk<T_CLUSTERIDX>
  , public InParamReadInst<T_FEATURE,T_INSTANCES_CLUSTER_K,T_CLUSTERIDX>
{
public:
  InParamWithoutPcPmVk
  (const std::string&   ais_algorithmoName,
   const std::string&   ais_algorithmoAuthor,
   InParam_algTypeOut   aiato_algTypeOut,
   int                  aii_opNorm)
    :  InParamGAClustering
         (ais_algorithmoName,ais_algorithmoAuthor,aiato_algTypeOut, aii_opNorm) 
    ,  InParamVk<T_CLUSTERIDX>()
    ,  InParamReadInst<T_FEATURE,T_INSTANCES_CLUSTER_K,T_CLUSTERIDX>()
  {}
  
  virtual void print(std::ostream&  aipf_outFile=std::cout, const char aic_separator=',') const
  {
    InParamGAClustering
      ::print(aipf_outFile,aic_separator);
    InParamVk<T_CLUSTERIDX>::print(aipf_outFile,aic_separator);
    InParamReadInst<T_FEATURE,T_INSTANCES_CLUSTER_K,T_CLUSTERIDX>::print(aipf_outFile,aic_separator);
  }
protected:
   
};

} /* END namespace inout
   */

#endif /*__IN_PARAM_WITHOUT_PCPMVK_HPP__*/
