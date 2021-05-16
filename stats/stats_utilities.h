/*
	/Project Name: stats
	/File: stats_utilities.h
	/Author: Eraj Gillani (0858887)
	/Date: Feb 28th 2020
	/Version 1.0
	/Purpose: to store all the headers so the stats.c can get access to all the functions
			  and set the structs and datatypes used 
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

typedef double data_set_type;

//define the struct to hold the x and y pair 
typedef struct pairNum_tag {
	data_set_type xValue;
	data_set_type yValue;
}pairNum_t;

//define the struct that will hold all the values in an array-type structure 
typedef struct array_tag {
	pairNum_t* data;
	size_t nSize;
	size_t nCapacity;
} array_t;



/*! \fn pairNum_t create_numPair()
	 \returns pairNum_t pair
	 \param NONE
	 \purpose creates an empty pair that will be added to the make_array struct
 */
pairNum_t create_numPair();

/*! \fn array_t make_array()
	 \returns array_t array
	 \param NONE
	 \purpose creates an empty array that will be expanded later
	 \note code structure referenced from Garths Santors arrayADT project (W2020)
 */
array_t make_array();

/*! \fn array_push_back(array_t* sortedXArray, pairNum_t numPair)
	 \returns bool value
	 \param array_t* sortedXArray, pairNum_t numPair
	 \purpose when array is full to its capacity, we dynamically allocate memory and expand the capacity of the array
	 \note code structure referenced from Garths Santors arrayADT project (W2020)
 */
bool array_push_back(array_t* pArray, pairNum_t value);

/*! \fn array_free(array_t* sortedXArray)
	 \returns NOTHING!
	 \param array_t* sortedXArray
	 \purpose frees the memory the arrays are holding once their purpose is completed
	 \note code structure referenced from Garths Santors arrayADT project (W2020)
 */
void array_free(array_t* pArray);

/*! \fn double makeACopyArray(array_t sortedXArray)
	 \returns array_t array
	 \param sortedXArray [in] a array_t array
	 \purpose Make a copy of the existing array to sort by y-values
			  Have two arrays to ensure real-num pairs stay together after sorting and
			  can calculate necessary statistics for each set of x and y values
 */
array_t makeACopyArray(array_t* pArray);


/*! \fn  filterStream(FILE* stream, bool eatCommas)
	 \returns bool
	 \param FILE* stream, bool eatCommas
	 \purpose read in the stream and filter in what the stream takes in and ensure only numerical values are read
			  that are separated between commas
 */
bool filterStream(FILE* stream, bool eatCommas);

/*! \fn  calcMean(array_t sortedXArray)
	 \returns pairNum_t
	 \param array_t sortedXArray
	 \purpose calculate the arithmetic mean and return the value in real-num pair
 */
pairNum_t calcMean(array_t sortedXArray);

/*! \fn  calcAbsDevStats(array_t sortedXArray, pairNum_t m)
	 \returns pairNum_t
	 \param array_t sortedXArray, pairNum_t m
	 \purpose calculate the nean absolute deviations
	 \note m is to represent any statistic needed to be found  (mean, median, mode)
	 \math link: https://en.wikipedia.org/wiki/Deviation_(statistics)#Unsigned_or_absolute_deviation
 */
pairNum_t calcAbsDevStats(array_t sortedXArray, pairNum_t m);

/*! \fn  calcMedian(array_t sortedXArray, array_t sortedYArray)
	 \returns pairNum_t
	 \param array_t sortedXArray, array_t sortedYArray
	 \purpose calculate the median for each of the sorted array
 */
pairNum_t calcMedian(array_t sortedXArray, array_t copiedArray);

/*! \fn  calcVariance(array_t sortedXArray, pairNum_t mean)
	 \returns pairNum_t
	 \param array_t sortedXArray, pairNum_t mean
	 \purpose calculate the variance for the sorted array
	 \math link: https://en.wikipedia.org/wiki/Variance#Normal_distribution
 */
pairNum_t calcVariance(array_t sortedXArray, pairNum_t mean);

/*! \fn  calcStdDeviation(array_t sortedXArray, pairNum_t variance)
	 \returns pairNum_t
	 \param array_t sortedXArray, pairNum_t variance
	 \purpose calculate the standard deviation of a finite population
	 \math link: https://en.wikipedia.org/wiki/Standard_deviation
 */
pairNum_t calcStdDeviation(array_t sortedXArray, pairNum_t variance);

/*! \fn  calcRegressionLine(array_t sortedXArray)
	 \returns pairNum_t
	 \param array_t sortedXArray
	 \purpose calculate the regression line values slope (b) and  y-intercept(a)
	 \math link:https://www.mathsisfun.com/data/least-squares-regression.html
 */
pairNum_t calcRegressionLine(array_t sortedXArray);

/*! \fn  calcYatMidX(pairNum_t regressLineValues, data_set_type minX, data_set_type maxX)
	 \returns pairNum_t
	 \param pairNum_t regressLineValues, data_set_type minX, data_set_type maxX
	 \purpose calculate the y-value at the mid-x value of the line using the values obtained from calcRegressionLine function
	 \math link:https://www.mathsisfun.com/data/least-squares-regression.html
 */
pairNum_t calcYatMidX(pairNum_t regressLineValues, data_set_type minX, data_set_type maxX);

/*! \fn  swap(data_set_type* pa, data_set_type* pb)
	 \returns nothing
	 \param double pa, double pb 
	 \purpose swaps two values 
 */
void swap(data_set_type* pa, data_set_type* pb);

/*! \fn  quickSort(array_t sortedXArray, array_t copyArray, data_set_type low, data_set_type high)
	 \returns nothing
	 \param array_t sortedXArray, array_t copyArray, data_set_type low, data_set_type high
	 \purpose to sort both arrays by lowest to highest values to obtain certain statistics 
	 \note QuickSort algorithm was obtained from page 15 in Garth Santors GATS Companion to: Searching and Sorting 
 */
void quickSort(array_t sortedXArray, array_t copyArray, data_set_type low, data_set_type high);

/*! \fn  findOutliers(array_t sortedYArray, pairNum_t stdDeviation, pairNum_t mean)
	 \returns nothing
	 \param array_t sortedYArray, pairNum_t stdDeviation, pairNum_t mean
	 \purpose find the y-value outliers that are two and three standard deviations away from the mean 
 */
void findOutliers(array_t sortedYArray, pairNum_t stdDeviation, pairNum_t mean);

/*! \fn  calcMode(array_t sortedArray, bool findingXModes)
	 \returns array_t
	 \param array_t sortedArray, bool findingXModes
	 \purpose find the mode value or the multi-mode(s) if any 
 */
array_t calcMode(array_t sortedArray, bool findingXModes); 

