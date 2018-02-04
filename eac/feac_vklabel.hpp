/*! \file feac_vklabel.hpp
 *
 * \brief EAC, EACI, EACII, EACIII and FEAC \cite Hruschka:etal:GAClusteringLabelKVar:EAC:2006 and \cite Alves:etal:GAclusteringLabelKVar:FEAC:2006
 *
 * \details  This file is part of the LEAC.\n\n
 * Implementation of the EAC, EACI, EACII, EACIII and FEAC algorithm\n 
 * based on the paper:\n
 * E.R. Hruschka, R.J.G.B. Campello, A.A. Freitas, and A.C.P.L.F. de\n 
 * Carvalho. A survey of evolutionary algorithms for clustering.\n
 * IEEE Transactions on Systems, Man and Cybernetics, Part C:\n
 * Applications  and Reviews, 39(2):133–155, March 2009.\n
 * <a href="http://www.cs.kent.ac.uk/pubs/2009/2884">http://www.cs.kent.ac.uk/pubs/2009/2884</a>.\n
 * \n
 * V. S. Alves, R. J. G. B. Campello, and E. R. Hruschka. Towards a\n
 * fast evolutionary algorithm for clustering. In IEEE International\n 
 * Conference on Evolutionary Computation, CEC 2006, part of WCCI 2006,\n 
 * Vancouver, BC, Canada, 16-21 July 2006, pages 1776–1783. IEEE, 2006.\n 
 * <a href="http://dx.doi.org/10.1109/CEC.2006.1688522">doi:http://dx.doi.org/10.1109/CEC.2006.1688522</a>\n
 * \n
 * Library Evolutionary Algorithms for Clustering (LEAC) is a library\n
 * for the implementation of evolutionary and genetic algorithms\n
 * focused on the partition type clustering problem. Based on the\n
 * current standards of the <a href="http://en.cppreference.com">C++</a> language, as well as on Standard\n
 * Template Library <a href="http://en.cppreference.com/w/cpp/container">STL</a> 
 * and also  <a href="http://www.openblas.net/">OpenBLAS</a> to have a better performance.\n
 * \version 1.0
 * \date 2015-2017
 * \authors Hermes Robles <hermes@uaz.edu.mx>\n Sebastian Ventura <sventura@uco.es>\n Amelia Zafra <azafra@uco.es>
 * \copyright <a href="https://www.gnu.org/licenses/gpl-3.0.en.html">GPLv3</a> \endlink license
 */

#ifndef __FEAC_VKLABEL_HPP__
#define __FEAC_VKLABEL_HPP__

#include <vector>
#include <algorithm>
#include <cmath>

#include <leac.hpp>
#include "inparam_gaclustering_feac.hpp"
#include "outparam_gaclustering.hpp"

#include "plot_runtime_function.hpp"

/*! \namespace eac
  \brief Evolutionary Algorithms for Clustering
  \details Implementation of genetic and evolutionary algorithms used to solve the clustering problem 
  
  \version 1.0
  \date   2015-2017
  \copyright GPLv3 license
*/

namespace eac {

/*! \fn gaencode::ChromosomeFEAC <T_CLUSTERIDX,T_REAL,T_FEATURE,T_FEATURE_SUM,T_INSTANCES_CLUSTER_K> feca_vklabel (inout::OutParamGAClustering<T_REAL, T_CLUSTERIDX>  &aoopcga_outParamClusteringGA, inout::InParamGAClusteringFEAC<T_FEATURE,T_REAL,T_CLUSTERIDX,T_FEATURE_SUM, T_INSTANCES_CLUSTER_K> &aiinpcga_inParamFEAC, const INPUT_ITERATOR aiiterator_instfirst, const INPUT_ITERATOR aiiterator_instlast, dist::Dist<T_REAL,T_FEATURE> &aifunc2p_dist)
  \brief EAC, EAC I, EAC II, EAC III, EAC IV and FEAC
  \details Implementation of the EAC, EAC I, EAC II, EAC III, EAC IV and FEAC  algorithm based on \cite Hruschka:etal:GAClusteringLabelKVar:EAC:2006 and \cite Alves:etal:GAclusteringLabelKVar:FEAC:2006.  
  \returns A partition of a data set, encoded on a chromosome with the membership labels of each instance and the centroids of the clusters.
  \param aoopcga_outParamClusteringGA a inout::OutParamGAClustering with the output parameters of the algorithm
  \param aiinpcga_inParamFEAC a inout::InParamGAClusteringFEAC parameters required by the algorithm
  \param aiiterator_instfirst an input iterator to the initial positions of the sequence of instances
  \param aiiterator_instlast an input iterator to the final positions of the sequence of instances
  \param aifunc2p_dist an object of type dist::Dist to calculate distances
*/
template < typename T_FEATURE, 
	   typename T_REAL,
	   typename T_FEATURE_SUM,
	   typename T_INSTANCES_CLUSTER_K, 
	   typename T_CLUSTERIDX, //-1, 0, 1, .., K
	   typename INPUT_ITERATOR
	   >
gaencode::ChromosomeFEAC
<T_CLUSTERIDX,
 T_REAL,
 T_FEATURE,
 T_FEATURE_SUM,
 T_INSTANCES_CLUSTER_K
 > 
feca_vklabel
(inout::OutParamGAClustering
 <T_REAL,
 T_CLUSTERIDX>                          &aoopcga_outParamClusteringGA,
 inout::InParamGAClusteringFEAC
 <T_FEATURE,
 T_REAL,
 T_CLUSTERIDX,
 T_FEATURE_SUM,
 T_INSTANCES_CLUSTER_K>                 &aiinpcga_inParamFEAC,
 const INPUT_ITERATOR                   aiiterator_instfirst,
 const INPUT_ITERATOR                   aiiterator_instlast,
 dist::Dist<T_REAL,T_FEATURE>           &aifunc2p_dist
 )
{
  /*ASIGNED CHROMOSOME SIZE
   */
  const uintidx  lui_numInstances = uintidx(std::distance(aiiterator_instfirst,aiiterator_instlast));
  
  if ( aiinpcga_inParamFEAC.getNumClusterKMaximum() == 
       INPARAMCLUSTERING_DEFAULT_CLUSTERK_UNDEFINED )
    aiinpcga_inParamFEAC.setNumClusterKMaximum
      (std::round(std::sqrt((double)lui_numInstances)));
  
#ifdef __VERBOSE_YES
  ++geiinparam_verbose;
#ifdef ALG_EAC_VKLABEL_HRUSCHKA_CAMPELLO_CASTRO_2006
  const char* lpc_labelAlgGA = "EAC:ClusteringLabelVarK:HruschkaCampelloCastro2006";
#endif /*ALG_EAC_VKLABEL_HRUSCHKA_CAMPELLO_CASTRO_2006*/
#ifdef ALG_EACI_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006
  const char* lpc_labelAlgGA = "EAC-I:ClusteringLabelVarK:AlvesCampelloHruschka2006";
#endif /*ALG_EACI_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006*/
#ifdef ALG_EACII_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006
  const char* lpc_labelAlgGA = "EAC-II:ClusteringLabelVarK:AlvesCampelloHruschka2006";
#endif /*ALG_EACII_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006*/  
#ifdef ALG_EACIII_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006
  const char* lpc_labelAlgGA = "EAC-III:ClusteringLabelVarK:AlvesCampelloHruschka2006";
#endif /*ALG_EACIII_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006*/
#ifdef ALG_FEAC_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006
  const char* lpc_labelAlgGA = "FECA:ClusteringLabelVarK:AlvesCampelloHruschka2006";
#endif /*ALG_FEAC_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006*/  
 
  if ( geiinparam_verbose <= geiinparam_verboseMax ) {
    std::cout << lpc_labelAlgGA
	      << ": IN(" << geiinparam_verbose << ")\n"
	      << "\t(output inout::OutParamGAClustering&: aoopcga_outParamClusteringGA[" 
	      << &aoopcga_outParamClusteringGA << "]\n"
	      << "\t input  inout::InParamGAClusteringFEAC&: aiinpcga_inParamFEAC[" 
	      << &aiinpcga_inParamFEAC << "]\n"
	      << "\t input aiiterator_instfirst[" << *aiiterator_instfirst << "]\n"
	      << "\t input aiiterator_instlast[" <<  *aiiterator_instlast << "]\n"
	      << "\t input  dist::Dist<T_REAL,T_FEATURE> &aifunc2p_dist[" 
	      << &aifunc2p_dist << ']'
	      << "\n\t\tPopulation size = " 
	      << aiinpcga_inParamFEAC.getSizePopulation()
	      << "\n\t\tk-minimum  = " 
	      << aiinpcga_inParamFEAC.getNumClusterKMinimum()
	      << "\n\t\tk-maximum  = " 
	      << aiinpcga_inParamFEAC.getNumClusterKMaximum()
	      << "\n\t\tKmeansNumMaxIter = " 
	      << aiinpcga_inParamFEAC.getKmeansNumMaxIter() 
	      << "\n\t\tKmeansMaxDiffCent  = " 
	      << aiinpcga_inParamFEAC.getKmeansMaxDiffCent()
	      << "\n\t\trandom-seed = "
	      << aiinpcga_inParamFEAC.getRandomSeed()
	      << "\n\t)"
	      << std::endl;
  }
#endif /*__VERBOSE_YES*/ 


  gaencode::ChromosomeFEAC
    <T_CLUSTERIDX,T_REAL,T_FEATURE,T_FEATURE_SUM,T_INSTANCES_CLUSTER_K>
    ::setStringSize(lui_numInstances);
  
  gaencode::ChromosomeFEAC
    <T_CLUSTERIDX,T_REAL,T_FEATURE,T_FEATURE_SUM,T_INSTANCES_CLUSTER_K> 
    lochrom_best;

  /*VARIABLE NEED FOR POPULATION AND MATINGPOOL GENETIC
   */
  std::vector <gaencode::ChromosomeFEAC
	       <T_CLUSTERIDX,T_REAL,T_FEATURE,T_FEATURE_SUM,T_INSTANCES_CLUSTER_K> >  
    lvectorchrom_population;

  std::uniform_int_distribution<uintidx>
    uniformdis_uiMinMaxK
    ((uintidx) aiinpcga_inParamFEAC.getNumClusterKMinimum(), 
     (uintidx) aiinpcga_inParamFEAC.getNumClusterKMaximum()
     );
  std::uniform_real_distribution<T_REAL> uniformdis_real01(0, 1);

  /*
   */
  T_REAL lt_PMO = 0.5;
  

  runtime::ListRuntimeFunction<COMMON_IDOMAIN> 
    llfh_listFuntionHist
    (aiinpcga_inParamFEAC.getNumMaxGenerations(), "Iterations", "Clustering metrics");

  /*DECLARATION OF VARIABLES: COMPUTING STATISTICAL AND METRIC OF THE ALGORITHM
   */
#ifndef __WITHOUT_PLOT_STAT
  std::ofstream               lfileout_plotStatObjetiveFunc;
  runtime::RuntimeFunctionValue<T_REAL> *lofh_objectiveFunc = NULL;
  runtime::RuntimeFunctionStat<T_REAL>  *lofhs_statObjectiveFunc[STATISTICAL_ALL_MEASURES];
  std::vector<T_REAL>       lvectorT_statfuncObjetiveFunc;
  
  if ( aiinpcga_inParamFEAC.getWithPlotStatObjetiveFunc() ) {  
    
    lvectorT_statfuncObjetiveFunc.reserve
      ( aiinpcga_inParamFEAC.getSizePopulation());
    //DEFINE FUNCTION
    lofh_objectiveFunc  = new runtime::RuntimeFunctionValue<T_REAL>
      ("SS", 
       aiinpcga_inParamFEAC.getAlgorithmoName(),
       RUNTIMEFUNCTION_NOT_STORAGE
       );

    llfh_listFuntionHist.addFuntion(lofh_objectiveFunc);

    //DEFINE FUNCTION STATISTICAL
    for  (int li_i = 0; li_i < STATISTICAL_ALL_MEASURES; li_i++) {
      lofhs_statObjectiveFunc[li_i] = 
	new runtime::RuntimeFunctionStat
	<T_REAL>
	( (char) li_i,
	  aiinpcga_inParamFEAC.getAlgorithmoName(),
	  RUNTIMEFUNCTION_NOT_STORAGE
	  );
      llfh_listFuntionHist.addFuntion(lofhs_statObjectiveFunc[li_i]);
    }
  
    //OPEN FILE STRORE FUNCTION
    aoopcga_outParamClusteringGA.setFileNameOutPlotStatObjetiveFunc
      (aiinpcga_inParamFEAC.getFileNamePlotStatObjetiveFunc(),
       aiinpcga_inParamFEAC.getTimesRunAlgorithm()
       );

    lfileout_plotStatObjetiveFunc.open
      (aoopcga_outParamClusteringGA.getFileNameOutPlotStatObjetiveFunc().c_str(),
       std::ios::out | std::ios::app
       );

    lfileout_plotStatObjetiveFunc.precision(COMMON_COUT_PRECISION);

    //FUNCTION HEADER

    lfileout_plotStatObjetiveFunc 
      <<  llfh_listFuntionHist.getHeaderFuntions() 
      << "\n";
  }

#endif /*__WITHOUT_PLOT_STAT*/
  
 
  /*WHEN CAN MEASURE STARTS AT ZERO INVALID OFFSPRING
   */
  aoopcga_outParamClusteringGA.setTotalInvalidOffspring(0);

  /*OUT: GENETIC ALGORITHM CHARACTERIZATION*/

  runtime::ExecutionTime let_executionTime = runtime::start();
  
  /*1. Initialize a population of random genotypes;

    C. Initial Population, Selection and Main Steps

    The EAC initial population is randomly generated. Each
    gene of a genotype takes a random value from the set
    {1,2,..., k}. The user can define either an initial value for k
    or its minimum and maximum values from a given range.
    Overestimated and underestimated values for k can be
    adopted since EAC can decrease or increase this initial
    number of clusters towards a better estimate
  */

  {/*BEGIN INITIALIZE POPULATION*/

#ifdef __VERBOSE_YES
    geverbosepc_labelstep = "1. POPULATION INITIALIZATION";
    ++geiinparam_verbose;
    if ( geiinparam_verbose <= geiinparam_verboseMax ) {
      std::cout << geverbosepc_labelstep  
		<< ": IN(" << geiinparam_verbose << ')'
		<< std::endl;
    }
#endif /*__VERBOSE_YES*/
    
    /*CREATE SPACE FOR STORE POPULATION-----------------------------------------
     */
    lvectorchrom_population.reserve
      (aiinpcga_inParamFEAC.getSizePopulation());
    
    std::uniform_int_distribution<uintidx> uniformdis_idxInstances
      (0,lui_numInstances-1);
    
    for (uintidx luintidx_i = 0; 
	 luintidx_i < aiinpcga_inParamFEAC.getSizePopulation(); 
	 luintidx_i++) 
      {
	/*Generate a number Ki in range Kmin to Kmax
	 */
	uintidx luintidx_krand = uniformdis_uiMinMaxK(gmt19937_eng);
	
	gaencode::ChromosomeFEAC
	  <T_CLUSTERIDX,T_REAL,T_FEATURE,T_FEATURE_SUM,T_INSTANCES_CLUSTER_K>
	  liter_iChrom
	  (luintidx_krand,
	   data::Instance<T_FEATURE>::getNumDimensions()
	   );
	
	clusteringop::randomInitialize
	  (liter_iChrom.getCentroids(),
	   liter_iChrom.getString(),
	   liter_iChrom.getNumInstancesClusterK(),
	   aiiterator_instfirst,
	   aiiterator_instlast
	   );
	
	liter_iChrom.setObjetiveFunc(-1.0);
	liter_iChrom.setFitness(0);
	  
	lvectorchrom_population.push_back
	  (std::move(liter_iChrom));
	
      }


#ifdef __VERBOSE_YES
    if ( geiinparam_verbose <= geiinparam_verboseMax ) {
      std::cout << geverbosepc_labelstep
		<< ": OUT(" << geiinparam_verbose << ')'
		<< std::endl;
    }
    --geiinparam_verbose;
#endif /*__VERBOSE_YES*/

  } /*END INITIALIZE POPULATION*/


  /*LOOP EVOLUTIONARY*/
  while ( true ) { 

    /*BEGIN ITERATION
     */
    llfh_listFuntionHist.increaseDomainUpperBound();

    /*2. Apply the k-means algorithm to each genotype;

      The data were normalized within the interval [0,1], and
      the local search (k-means) algorithm was programmed to
      stop when one of the following criteria is satisfied: (i) 5
      iterations have been completed – empirical evidence suggest
      that five or less repetitions ordinarily will suffice [1]; or (ii)
      the maximum absolute difference between centroids in two
      consecutive iterations is less than or equal to 0.001.
    */
    {/*BEGIN K-MEANS ALGORITHM*/

#ifdef __VERBOSE_YES

      /*ID PROC
       */
      ++geverboseui_idproc;
      geverbosepc_labelstep = "2. APPLY THE K-MEANS ALGORITHM";
      ++geiinparam_verbose;
      if ( geiinparam_verbose <= geiinparam_verboseMax ) {
	std::cout << geverbosepc_labelstep
		  << ":  IN(" << geiinparam_verbose << ')'
		  << std::endl;
      }
#endif /*__VERBOSE_YES*/
     
      
      for (auto &&liter_iChrom: lvectorchrom_population) {
	/*Chose Ki point randomly from the data
	 */
      
	gaclusteringop::kmeansfeac
	  (liter_iChrom,
	   aiiterator_instfirst,
	   aiiterator_instlast,
	   aiinpcga_inParamFEAC.getKmeansNumMaxIter(),
	   aiinpcga_inParamFEAC.getKmeansMaxDiffCent(),
	   aifunc2p_dist
	   );
	
      }
	

#ifdef __VERBOSE_YES
      if ( geiinparam_verbose <= geiinparam_verboseMax ) {
	std::cout << geverbosepc_labelstep
		  << ": OUT(" << geiinparam_verbose << ')'
		  << std::endl;
      }
      --geiinparam_verbose;
#endif /*__VERBOSE_YES*/

    } /*BEGIN K-MEANS ALGORITHM*/

    /*3. Evaluate each genotype according to the fitness function
     */

    {/*BEGIN FITNESS COMPUTATION*/

#ifdef __VERBOSE_YES
      geverbosepc_labelstep = "3. EVALUATE EACH GENOTYPE FITNESS FUNCTION";
      ++geiinparam_verbose;
      if ( geiinparam_verbose <= geiinparam_verboseMax ) {
	std::cout << geverbosepc_labelstep  
		  << ": IN(" << geiinparam_verbose << ')'
		  << std::endl;
      }
#endif /*__VERBOSE_YES*/

      
      for (auto &&liter_iChrom: lvectorchrom_population) {


#ifdef _METRIC_SIMPLIFIED_SILHOUETTE_
	
	partition::PartitionLabel
	  <T_CLUSTERIDX>
	  lpartitionlabel_clusters
	  (liter_iChrom.getString(),
	   liter_iChrom.getStringSize(),
	   liter_iChrom.getNumClusterK()
	   );

	std::vector<T_REAL>&&  lvectort_partialSilhouette =
	  um::simplifiedSilhouette
	  (liter_iChrom.getCentroids(),
	   aiiterator_instfirst,
	   aiiterator_instlast,
	   lpartitionlabel_clusters,
	   liter_iChrom.getNumInstancesClusterK(),
	   aifunc2p_dist
	   );
	   

	T_REAL lmetrict_partialSilhouette = 
	  interfacesse::sum
	  (lvectort_partialSilhouette.data(),
	   (uintidx) lvectort_partialSilhouette.size()
	   ) / (T_REAL) lvectort_partialSilhouette.size();

	liter_iChrom.setPartialFcC(lvectort_partialSilhouette);
	liter_iChrom.saveLastObjetiveFunc();
	liter_iChrom.setObjetiveFunc(lmetrict_partialSilhouette);
	
#endif /*_METRIC_SIMPLIFIED_SILHOUETTE_*/
	   
#ifndef __WITHOUT_PLOT_STAT
	lvectorT_statfuncObjetiveFunc.push_back(liter_iChrom.getObjetiveFunc());
#endif /*__WITHOUT_PLOT_STAT*/

      }
	
#ifdef __VERBOSE_YES
      if ( geiinparam_verbose <= geiinparam_verboseMax ) {
	std::cout << geverbosepc_labelstep
		  << ": OUT(" << geiinparam_verbose << ')'
		  << std::endl;
      }
      --geiinparam_verbose;
#endif /*__VERBOSE_YES*/
    

    } /*END FITNESS COMPUTATION*/

    /*4. Apply a linear normalization (ranking)
      The genotypes corresponding to each generation are selected according to
      the roulette wheel strategy [16], which does not admit negative objective
      function values. For this reason, a constant equal to one is summed up to the
      objective function before the selection procedure takes place.
      \cite{Hruschka:etal:GAClusteringLabelKVar:EAC:2006}

      bool lb_noChangeFitness = true; ONLY MATLAB ￼in the paper not

	  
    */
#ifdef ALG_EAC_VKLABEL_HRUSCHKA_CAMPELLO_CASTRO_2006
	

    /*The EAC originally employs the Simplified Silhouette (to
      be detailed in Section IV-B) as its fitness function. Although
      the simplified silhouette has shown to be useful in a number
      of datasets (e.g. see [13][15])
    */

    for (auto &&liter_iChrom: lvectorchrom_population) {
      liter_iChrom.setFitness(liter_iChrom.getObjetiveFunc() + 1.0);
    }
		
#endif /*ALG_EAC_VKLABEL_HRUSCHKA_CAMPELLO_CASTRO_2006*/
  

    /*4. Apply a linear normalization
      EAC-I
      FEAC
      [6] \cite{Alves:etal:GAclusteringLabelKVar:FEAC:2006}
    */
#if defined(ALG_EACI_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006) || \
  defined(ALG_EACII_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006) || \
  defined(ALG_EACIII_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006) || \
  defined(ALG_FEAC_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006)
      
#ifdef __VERBOSE_YES
    geverbosepc_labelstep = "4. APPLY A LINEAR NORMALIZATION[6]";
    ++geiinparam_verbose;
    if ( geiinparam_verbose <= geiinparam_verboseMax ) {
      std::cout << geverbosepc_labelstep  
		<< ": IN(" << geiinparam_verbose << ')'
		<< std::endl;
    }
#endif /*__VERBOSE_YES*/

    prob::linearNormalization
      (lvectorchrom_population.begin(),
       lvectorchrom_population.end(),
       [](const gaencode::ChromosomeFEAC
	  <T_CLUSTERIDX,T_REAL,T_FEATURE,T_FEATURE_SUM,T_INSTANCES_CLUSTER_K>
	  &lchromfeac_iter
	  ) -> T_REAL
       {
	 return lchromfeac_iter.getObjetiveFunc();
       },
       [](gaencode::ChromosomeFEAC
	  <T_CLUSTERIDX,T_REAL,T_FEATURE,T_FEATURE_SUM,T_INSTANCES_CLUSTER_K>
	  &lchromfeac_iter, T_REAL airt_funcFitnessLineal)
       {
	 lchromfeac_iter.setFitness(airt_funcFitnessLineal);
       },
       T_REAL(1.0)
       );
	  
#ifdef __VERBOSE_YES
    if ( geiinparam_verbose <= geiinparam_verboseMax ) {
      std::cout << geverbosepc_labelstep
		<< ": OUT(" << geiinparam_verbose << ')'
		<< std::endl;
    }
    --geiinparam_verbose;
#endif /*__VERBOSE_YES*/
    

#endif /*ALG_EACI_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006
	 ALG_EACII_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006
         ALG_EACIII_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006
	 ALG_FEAC_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006
       */


    /*\cite{Hruschka:etal:GAClusteringLabelKVar:EAC:2006}
      In addition, an elitist strategy is adopted [16]: 
      the best genotype (the one with highest fitness)
      is always copied and maintained into the next generation.
    */

    { /*BEGIN PRESERVING THE BEST STRING*/
#ifdef __VERBOSE_YES
      geverbosepc_labelstep = "ELITISM PRESERVING THE BEST";
      ++geiinparam_verbose;
      if ( geiinparam_verbose <= geiinparam_verboseMax ) {
	std::cout << geverbosepc_labelstep
		  << ": IN(" << geiinparam_verbose << ')'
		  << std::endl;
      }
#endif /*__VERBOSE_YES*/

      auto lit_chromMax = std::max_element
	(lvectorchrom_population.begin(), 
	 lvectorchrom_population.end(), 
	 [](const gaencode::ChromosomeFEAC
	    <T_CLUSTERIDX,T_REAL,T_FEATURE,T_FEATURE_SUM,T_INSTANCES_CLUSTER_K>& x, 
	    const gaencode::ChromosomeFEAC
	    <T_CLUSTERIDX,T_REAL,T_FEATURE,T_FEATURE_SUM,T_INSTANCES_CLUSTER_K>& y
	    ) 
	 {  return x.getObjetiveFunc() < y.getObjetiveFunc(); } //FALTA FALTA FINTNESS
	 );

      if ( lochrom_best.getObjetiveFunc() < (*lit_chromMax).getObjetiveFunc() ) {
	lochrom_best = *lit_chromMax;

	/*CHROMOSOME ONE WAS FOUND IN THIS ITERATION*/
	aoopcga_outParamClusteringGA.setIterationGetsBest
	  (llfh_listFuntionHist.getDomainUpperBound());
	aoopcga_outParamClusteringGA.setRunTimeGetsBest
	  (runtime::elapsedTime(let_executionTime));
      } /*END IF*/
      
#ifdef __VERBOSE_YES
      if ( geiinparam_verbose <= geiinparam_verboseMax ) {
	std::cout << geverbosepc_labelstep
		  << ": OUT(" << geiinparam_verbose << ')'
		  << std::endl;
      }
      --geiinparam_verbose;
#endif /*__VERBOSE_YES*/

    } /*END PRESERVING THE BEST STRING*/


    /*MEASUREMENT BEST: COMPUTING STATISTICAL AND METRIC OF THE ALGORITHM
     */
#ifndef __WITHOUT_PLOT_STAT
    if ( aiinpcga_inParamFEAC.getWithPlotStatObjetiveFunc() ) {  
      lofh_objectiveFunc->setValue(lochrom_best.getObjetiveFunc());
      functionhiststat_evaluateAll
	(lofhs_statObjectiveFunc,
	 lvectorT_statfuncObjetiveFunc
	 );
      lfileout_plotStatObjetiveFunc << llfh_listFuntionHist;
      lvectorT_statfuncObjetiveFunc.clear();
    }
#endif /*__WITHOUT_PLOT_STAT*/

    /*To end the program when, the search is extended for a long time, 
      you can spend the maximum time of execution in seconds, in the 
      papers do not consider this option
    */

    if ( (aiinpcga_inParamFEAC.getDesiableObjetiveFunc()
	  <  (lochrom_best.getObjetiveFunc() )) || 
	 (llfh_listFuntionHist.getDomainUpperBound() >=
	  aiinpcga_inParamFEAC.getNumMaxGenerations()) ||
	 (runtime::elapsedTime(let_executionTime) >
	  aiinpcga_inParamFEAC.getMaxExecutiontime() )
	 )
      break;

    std::vector
      <gaencode::ChromosomeFEAC
       <T_CLUSTERIDX,T_REAL,T_FEATURE,T_FEATURE_SUM,T_INSTANCES_CLUSTER_K> >
      lvectorchrom_stringPool;
    lvectorchrom_stringPool.reserve
      (aiinpcga_inParamFEAC.getSizePopulation());
	
       
    { /*BEGIN 5. SELECT GENOTYPES:*/
		
#ifdef __VERBOSE_YES
      geverbosepc_labelstep = "5. SELECT GENOTYPES";
      ++geiinparam_verbose;
      if ( geiinparam_verbose <= geiinparam_verboseMax ) {
	std::cout << geverbosepc_labelstep
		  << ": IN(" << geiinparam_verbose << ')'
		  << std::endl;
      }  
#endif /*__VERBOSE_YES*/    

      /*COPY POPULATION TO STRING POOL FOR ROULETTE WHEEL--------------------------
       */
      const auto lvectorT_probDistRouletteWheel = 
	prob::makeDistRouletteWheel
	(lvectorchrom_population.begin(),
	 lvectorchrom_population.end(),
	 [](const gaencode::ChromosomeFEAC
	    <T_CLUSTERIDX,T_REAL,T_FEATURE,T_FEATURE_SUM,T_INSTANCES_CLUSTER_K>
	    &liter_iChrom
	    ) -> T_REAL
	 {
	   return liter_iChrom.getFitness();
	 }
	 );
    
      /*ELITISMO
       */
      lvectorchrom_stringPool.push_back
	(gaencode::ChromosomeFEAC
	 <T_CLUSTERIDX,T_REAL,T_FEATURE,T_FEATURE_SUM,T_INSTANCES_CLUSTER_K>
	 (lochrom_best)
	 );

      for (uintidx luintidx_i = 1; 
	   luintidx_i < aiinpcga_inParamFEAC.getSizePopulation(); 
	   luintidx_i++) 
	{
	  uintidx luintidx_chrom = 
	    gaselect::getIdxRouletteWheel
	    (lvectorT_probDistRouletteWheel,
	     uintidx(0)
	     );
	  
	  lvectorchrom_stringPool.push_back
	    (gaencode::ChromosomeFEAC
	     <T_CLUSTERIDX,T_REAL,T_FEATURE,T_FEATURE_SUM,T_INSTANCES_CLUSTER_K>
	     (lvectorchrom_population.at(luintidx_chrom))
	     );
	}

#ifdef __VERBOSE_YES
      if ( geiinparam_verbose <= geiinparam_verboseMax ) {
	std::cout << geverbosepc_labelstep
		  << ": OUT(" << geiinparam_verbose << ')'
		  << std::endl;
      }
      --geiinparam_verbose;
#endif /*__VERBOSE_YES*/

    } /*END 5. SELECT GENOTYPES:*/
    
    /*6. Apply the mutation operators:
      EAC-III
      F-EAC
    */
#if defined(ALG_EACIII_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006) || \
  defined(ALG_FEAC_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006)
    { /*BEGIN COMPUTED PROPORTION APPLIED OPERATOR OF MUTATION*/

#ifdef __VERBOSE_YES
      geverbosepc_labelstep = "COMPUTED PROPORTION APPLIED OPERATOR OF MUTATION";
      ++geiinparam_verbose;
      if ( geiinparam_verbose <= geiinparam_verboseMax ) {
	std::cout << geverbosepc_labelstep
		  << ": IN(" << geiinparam_verbose << ')'
		  << std::endl;
      }
#endif /*__VERBOSE_YES*/

      T_REAL lrt_DAFt1_a = 0.0; 
      T_REAL lrt_DAFt1_b = 0.0;
      T_REAL lrt_DAFt2_a = 0.0; 
      T_REAL lrt_DAFt2_b = 0.0;
      uintidx  luintidx_countmo1 = 0;
      uintidx  luintidx_countmo2 = 0;
      
      for (auto &&liter_iChrom: lvectorchrom_stringPool) {
	if ( liter_iChrom.getAppliedOperator() == gaencode::FEAC_OPERATOR_MO1) {
	  lrt_DAFt1_b += liter_iChrom.getLastObjetiveFunc();
	  lrt_DAFt1_a += liter_iChrom.getObjetiveFunc();
	  ++luintidx_countmo1;
	}
	else if ( liter_iChrom.getAppliedOperator() == gaencode::FEAC_OPERATOR_MO2 ) {
	  lrt_DAFt2_b += liter_iChrom.getLastObjetiveFunc();
	  lrt_DAFt2_a += liter_iChrom.getObjetiveFunc();
	  ++luintidx_countmo2;
	}
      }
	  
      
      T_REAL lrt_DAFt1 = ( luintidx_countmo1 == 0)?
	T_REAL(0):(lrt_DAFt1_a - lrt_DAFt1_b) / (T_REAL) luintidx_countmo1;
      T_REAL lrt_DAFt2 = ( luintidx_countmo2 == 0)?
	T_REAL(0):(lrt_DAFt2_a - lrt_DAFt2_b) / (T_REAL) luintidx_countmo2;
	  
      if ( lrt_DAFt1 > 0.0 && lrt_DAFt2 > 0.0 ) 
	lt_PMO = (lrt_DAFt1 / ( lrt_DAFt1 + lrt_DAFt2)); 
      else if ( lrt_DAFt1 <= 0.0 && lrt_DAFt2 <= 0.0 )
	lt_PMO = 0.5;
      else if ( lrt_DAFt1 <= 0.0 )
	lt_PMO = 0.10;
      /*Improve performance by changing the PMO by 0.10 instead of 0.0*/
      else if ( lrt_DAFt2 <= 0.0 )
	lt_PMO = 0.90;
      /*Improve performance by changing the PMO by 0.90 instead of 1.0*/

#ifdef __VERBOSE_YES
      if ( geiinparam_verbose <= geiinparam_verboseMax ) {
	std::cout << geverbosepc_labelstep
		  << ": OUT(" << geiinparam_verbose << ')'
		  << "Number applied operator MO1 = " << luintidx_countmo1
		  << "\tNumber applied operator MO2 = " << luintidx_countmo2
		  << "\tlrt_DAFt1 = " << lrt_DAFt1 << "\tlrt_DAFt2 = " << lrt_DAFt2 
		  << "\t--> lt_PMO = "   << lt_PMO
		  << std::endl;
      }
      --geiinparam_verbose;
#endif /*__VERBOSE_YES*/

    } /*END COMPUTED PROPORTION APPLIED OPERATOR OF MUTATION*/
    
#endif /*ALG_EACIII_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006
	 ALG_FEAC_VKLABEL_ALVES_CAMPELLO_HRUSCHKA_2006
       */
    

    { /*BEGIN MUTATION*/

#ifdef __VERBOSE_YES
      geverbosepc_labelstep = "6. APPLY THE MUTATION OPERATORS";
      ++geiinparam_verbose;
      if ( geiinparam_verbose <= geiinparam_verboseMax ) {
	std::cout << geverbosepc_labelstep
		  << ": IN(" << geiinparam_verbose << ')'
		  << std::endl;
      }
#endif /*__VERBOSE_YES*/

      for (auto &&liter_iChrom: lvectorchrom_stringPool) {
	
	if ( uniformdis_real01(gmt19937_eng) <  lt_PMO  ) {  //lt_PMO = 0.5
	  liter_iChrom.setAppliedOperator(gaencode::FEAC_OPERATOR_MO1);
	  gaclusteringop::MO1 
	    (liter_iChrom,
	     aiiterator_instfirst,
	     aiiterator_instlast,
	     aifunc2p_dist
	     );
	}
	else {
	  liter_iChrom.setAppliedOperator(gaencode::FEAC_OPERATOR_MO2);
	  gaclusteringop::MO2 
	    (liter_iChrom,
	     aiiterator_instfirst,
	     aiiterator_instlast,
	     aifunc2p_dist
	     );
	}
      }

#ifdef __VERBOSE_YES
      if ( geiinparam_verbose <= geiinparam_verboseMax ) {
	std::cout << geverbosepc_labelstep
		  << ": OUT(" << geiinparam_verbose << ')'
		  << std::endl;
      }
      --geiinparam_verbose;
#endif /*__VERBOSE_YES*/

    } /*END MUTATION*/

    { /*BEGIN SWAP POPULATION*/
#ifdef __VERBOSE_YES
      geverbosepc_labelstep = "SWAP POPULATION";
      ++geiinparam_verbose;
      if ( geiinparam_verbose <= geiinparam_verboseMax ) {
	std::cout << geverbosepc_labelstep
		  << ": IN(" << geiinparam_verbose << ')'
		  << std::endl;
      }
#endif /*__VERBOSE_YES*/
     
      lvectorchrom_population.clear();
      lvectorchrom_population.swap(lvectorchrom_stringPool);
	  
#ifdef __VERBOSE_YES
      if ( geiinparam_verbose <= geiinparam_verboseMax ) {
	std::cout << geverbosepc_labelstep
		  << ": OUT(" << geiinparam_verbose << ')'
		  << std::endl;
      }
      --geiinparam_verbose;
#endif /*__VERBOSE_YES*/

    } /*END SWAP POPULATION*/

  }  /*END LOOP EVOLUTIONARY*/

  runtime::stop(let_executionTime);
  aoopcga_outParamClusteringGA.setNumClusterK
    (lochrom_best.getNumClusterK());
  aoopcga_outParamClusteringGA.setMetricFuncRun
    (lochrom_best.getObjetiveFunc());
  aoopcga_outParamClusteringGA.setFitness
    (lochrom_best.getFitness());
  aoopcga_outParamClusteringGA.setAlgorithmRunTime
    (runtime::getTime(let_executionTime));
  aoopcga_outParamClusteringGA.setNumTotalGenerations
    (llfh_listFuntionHist.getDomainUpperBound());

  /*FREE: COMPUTING STATISTICAL AND METRIC OF THE ALGORITHM
   */ 
#ifndef __WITHOUT_PLOT_STAT

  if ( aiinpcga_inParamFEAC.getWithPlotStatObjetiveFunc() ) {  
    plot_funtionHist
      (llfh_listFuntionHist,
       aiinpcga_inParamFEAC,
       aoopcga_outParamClusteringGA
       );  
  }

#endif /*__WITHOUT_PLOT_STAT*/
  
#ifdef __VERBOSE_YES
  if ( geiinparam_verbose <= geiinparam_verboseMax ) {
    geverbosepc_labelstep = lpc_labelAlgGA;
    std::cout << lpc_labelAlgGA 
	      << " OUT(" << geiinparam_verbose << ")\n";
    std::ostringstream lostrstream_labelChrom;
    lostrstream_labelChrom << lpc_labelAlgGA
			   << ":generation: " <<  llfh_listFuntionHist.getDomainUpperBound();
    lochrom_best.print(std::cout,lostrstream_labelChrom.str().c_str(),',',';');
  }
  --geiinparam_verbose;
#endif /*__VERBOSE_YES*/

  return lochrom_best;
 
} /*END feca_vklabel*/

} /*END eac */

#endif /*__FEAC_VKLABEL_HPP__*/