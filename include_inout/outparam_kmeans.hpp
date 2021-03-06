/*! \file outparam_kmeans.hpp
 *
 * \brief outparam kmeans
 *
 * \version 1.0
 * \date 2015-2017
 * \authors Hermes Robles-Berumen <hermes@uaz.edu.mx>\n Sebastian Ventura <sventura@uco.es>\n Amelia Zafra <azafra@uco.es>\n <a href="http://www.uco.es/kdis/">KDIS</a>
 * \copyright <a href="https://www.gnu.org/licenses/gpl-3.0.en.html">GPLv3</a> license
 */

#ifndef __OUT_PARAM_KMEANS_HPP__
#define __OUT_PARAM_KMEANS_HPP__


#include "outparam_clustering.hpp"


/*! \namespace inout
  \brief Module for input and output parameters
  \details
  
  \version 1.0
  \date   2015-2017
  \copyright GPLv3 license
*/

namespace inout {

/*! \class OutParamKmeans
  \brief Output parameter for Kmeans Algorithmo \cite MacQueen:ClusterAnalysis:KMeans:1967
 */
template < typename T_METRIC,
	   typename T_MEMBERCLUSTER_IDX
	   >
class OutParamKmeans: 
  public OutParamClustering<T_METRIC,T_MEMBERCLUSTER_IDX> {
public:
   OutParamKmeans(const OutParamNameObjectiveFunc aienum_usedObjectiveFunc):
  OutParamClustering<T_METRIC,T_MEMBERCLUSTER_IDX>
    ::OutParamClustering(aienum_usedObjectiveFunc)   
  {
    this->initialize(-1);
  }
 
  virtual ~OutParamKmeans() { }

  void initialize(int aii_numRunAlgorithm)
  {
    OutParamClustering<T_METRIC,T_MEMBERCLUSTER_IDX>
    ::initialize(aii_numRunAlgorithm);
    this->_uintidx_numThreshold = OUTPARAMCLUSTERING_INT_NaN;
  }

  inline void setNumThreshold(uintidx aist_numThreshold)	
  {
    this->_uintidx_numThreshold = aist_numThreshold;
  }

  inline uintidx& getNumThreshold()	
  {
    return this->_uintidx_numThreshold;
  }

  virtual void  print(std::ostream& aipf_outFile=std::cout, const char aic_separator=',') const
  {
    OutParamClustering<T_METRIC,T_MEMBERCLUSTER_IDX>::print(aipf_outFile);
    aipf_outFile << aic_separator << "_number threshold" 
		 << aic_separator << this->_uintidx_numThreshold;
  }
protected:

  uintidx  _uintidx_numThreshold;

}; /*END class OutParamKmeans*/

} /*END namespace inout
   */

#endif /*__OUT_PARAM_KMEANS_HPP__*/
