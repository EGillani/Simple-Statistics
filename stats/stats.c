/*
	/Project Name: stats
	/File: stats.c
	/Author: Eraj Gillani (0858887)
	/Date: Feb 28th 2020
	/Version 1.0
	/Purpose: To compile a set of statistics on a list of real number pairs
			Statistics: mininum value, maximum value, median value, arithemetic mean, mean absolute deviation (mean, median, mode),
						variance, standard deviation, mode, least squares regression line, outliers 

*/
#define _CRT_SECURE_NO_WARNINGS
#include "stats_utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>


int main(int argc, char* argv[]) 
{


	//read in a stream from a file 
	FILE* stream = stdin;

	//error check
	if (argc > 2) {
		printf("Error: too many command-line arguments (%d)\n", argc);
		printf(
			"Usage: fcount [filename]\n"
			"\twhere filename is the name of the file to read.\n"
			"\tuses stdin if there is no filename.\n"
		);
		return EXIT_FAILURE;
	}
	else if (argc == 2) {
		if ((stream = fopen(argv[1], "rb")) == NULL) 
		{
			printf("Error <%s> ", argv[1]);
			perror(" ");	
			return EXIT_FAILURE;
		}
	}

	//create the array to hold the realnum pairs
	//NOTE: called sortedXArray because later on, this array will be sorted by x values 
	array_t sortedXArray = make_array();
	data_set_type x, y;

	printf("Enter a list of comma-separated real number pairs terminated by EOF or non numeric input.\n");

	//read in the stream and store the real-num pairs in an array 
	while (filterStream(stream, false) && (fscanf_s(stream, "%lf", &x) == 1) && filterStream(stream, true)
		&& (fscanf_s(stream, "%lf", &y) == 1))
	{
		pairNum_t numPair = create_numPair();
		numPair.xValue = x;
		numPair.yValue = y;

		array_push_back(&sortedXArray, numPair);
	}

	//create a copy of the array to sort the real-num pairs by y values 
	array_t sortedYArray = makeACopyArray(&sortedXArray);

	//calculate the statistics that do not need real-num pairs to be sorted 
	pairNum_t mean = calcMean(sortedXArray);
	pairNum_t regressLineValues = calcRegressionLine(sortedXArray);
	pairNum_t variance = calcVariance(sortedXArray, mean);
	pairNum_t stdDeviation = calcStdDeviation(sortedXArray, variance);
	pairNum_t absDevMean = calcAbsDevStats(sortedXArray, mean); 

	//sort both arrays using quickSort and calculate the remaining statistics that need array to be sorted 
	quickSort(sortedXArray, sortedYArray, 0, sortedXArray.nSize - 1);
	pairNum_t median = calcMedian(sortedXArray, sortedYArray);
	pairNum_t absDevMedian = calcAbsDevStats(sortedXArray, median);
	pairNum_t yAtMidX = calcYatMidX(regressLineValues, sortedXArray.data[0].xValue, sortedXArray.data[sortedXArray.nSize - 1].xValue);

	//create the two arrays that will hold all the mode values 
	bool findingXModes = true;
	array_t modeX = calcMode(sortedXArray, findingXModes);
	array_t modeY = calcMode(sortedYArray, !findingXModes);

	//getting the mean absolute distribution around the mode 
	pairNum_t mode = { .xValue = 0.0 , .yValue = 0.0 };
	pairNum_t absDevMode;

	//print the results 
	printf("\n\nResults:\n--------------------------------------------------------------\n");
	printf("%-18s %20zu %20zu", "# elements", sortedXArray.nSize, sortedXArray.nSize);
	printf("\n%-18s %20.3lf %20.3lf", "minimum", sortedXArray.data[0].xValue, sortedYArray.data[0].yValue);
	printf("\n%-18s %20.3lf %20.3lf", "maximum", sortedXArray.data[sortedXArray.nSize - 1].xValue, sortedYArray.data[sortedYArray.nSize - 1].yValue);
	printf("\n%-18s %20.3lf %20.3lf", "mean", mean.xValue, mean.yValue);
	printf("\n%-18s %20.3lf %20.3lf\n", "median",median.xValue, median.yValue);

	//print the mode value only if it exists (the size of the mode arrays must have greater than 0) 
	if (modeX.nSize == 0 && modeY.nSize == 0)
	{
		printf("%s%35s%21s\n", "mode", "no mode", "no mode");
	}
	else if (modeX.nSize == 0 && modeY.nSize > 0)
	{
		//we stored our frequency value in the pairNum_t values that were not used to store the mode values to avoid creating extra functions to store it 
		printf("%s%35s%20s%.0lf\n", "mode", "no mode", "freq.= ", modeY.data[0].xValue);
		for (unsigned i = 0; i < modeY.nSize; i++)
		{
			printf("%60lf\n", modeY.data[i].yValue);
		}
	}
	else if (modeX.nSize > 0 && modeY.nSize == 0)
	{
		printf("%s%34s%.0lf%21s\n", "mode", "freq.= ", modeX.data[0].yValue, "no mode");
		for (unsigned i = 0; i < modeX.nSize; i++)
		{
			printf("%39lf\n", modeX.data[i].xValue);
		}

	}
	else
	{
		//multi-mode output, and taking account for if there is more modes in the modeXarray than the modeYarray 
		printf("%s%34s%.0lf%20s%.0lf\n", "mode", "freq.= ", modeX.data[0].yValue, "freq.= ", modeY.data[0].xValue);
		if (modeX.nSize > modeY.nSize)
		{
			//print out using the size of the smaller array
			for (unsigned i = 0; i < modeY.nSize; i++)
			{
				printf("%39lf%21lf\n", modeX.data[i].xValue, modeY.data[i].yValue);
			}
			//print out the rest of the values using the size of the smaller array as the starting point 
			for (unsigned i = modeY.nSize; i < modeX.nSize; i++)
			{
				printf("%s%38lf\n", " ", modeX.data[i].xValue);

			}

		}
		else
		{	//if the modeXarray is smaller than the modeYarray
			for (unsigned i = 0; i < modeX.nSize; i++)
			{
				printf("%39lf%21lf\n", modeX.data[i].xValue, modeY.data[i].yValue);
			}
			for (unsigned i = modeX.nSize; i < modeY.nSize; i++)
			{
				printf("%s%40.3lf\n", " ", modeY.data[i].yValue);

			}

		}
	}

	//continue printing out the rest of the statistics 
	printf("%-18s %20.3lf %20.3lf", "variance", variance.xValue, variance.yValue);
	printf("\n%-18s %20.3lf %20.3lf", "std. dev.", stdDeviation.xValue, stdDeviation.yValue);
	printf("\nmean absolute deviations:");
	printf("\n%-18s %20.3lf %20.3lf", "-> about the mean", absDevMean.xValue, absDevMean.yValue);
	printf("\n%-18s%20.3lf %20.3lf", "-> about the median", absDevMedian.xValue, absDevMedian.yValue);

	//only printing out mean absolute distribution about the mode if its unimodel 
	if (modeX.nSize == 1 && modeY.nSize == 1)
	{
		mode.xValue = modeX.data[0].xValue;
		mode.yValue = modeY.data[0].yValue;
		absDevMode = calcAbsDevStats(sortedXArray, mode);
		printf("\n%-18s %20.3lf %20.3lf", "-> about the mode", absDevMode.xValue, absDevMode.yValue);
	}
	else if (modeX.nSize == 0 && modeY.nSize == 1)
	{
		mode.yValue = modeY.data[0].yValue;
		absDevMode = calcAbsDevStats(sortedXArray, mode);
		printf("\n%-18s %20s %20.3lf", "-> about the mode", "no mode", absDevMode.yValue);
	}
	else if (modeX.nSize == 1 && modeY.nSize == 0)
	{
		mode.xValue = modeX.data[0].xValue;
		absDevMode = calcAbsDevStats(sortedXArray, mode);
		printf("\n%-18s %20.3lf %20s", "-> about the mode", absDevMode.xValue, "no mode");
	}
	else
		printf("\n%-18s %20s %20s", "-> about the mode", "no mode", "no mode");

	//print the rest of the statistics 
	printf("\n%-25s%3s %10.3lf %10s %9.3lf", "regression values", "a =", regressLineValues.yValue, "b =", regressLineValues.xValue);
	printf("\n%-28s%3s%8.3lf %9s %10.3lf", "Y at mid(x)", "x =", yAtMidX.xValue, "y =", yAtMidX.yValue);
	findOutliers(sortedXArray, stdDeviation, mean);

	//house keeping (free up memory) 
	fclose(stream);
	array_free(&modeX);
	array_free(&modeY);
	array_free(&sortedXArray);
	array_free(&sortedYArray);

}