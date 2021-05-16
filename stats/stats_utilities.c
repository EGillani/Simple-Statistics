
/*
	/Project Name: stats
	/File: stats_utilities.c
	/Author: Eraj Gillani (0858887)
	/Date: Feb 28th 2020
	/Version 1.0
	/Purpose: to store all the functions needed to execute program in stats.c
*/

#define _CRT_SECURE_NO_WARNINGS
#include "stats_utilities.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>



pairNum_t create_numPair() {
	pairNum_t pair;
	pair.xValue = 0;
	pair.yValue = 0;
	return pair;
}


array_t make_array() {
	array_t arr;
	arr.data = NULL;
	arr.nCapacity = 0;
	arr.nSize = 0;
	return arr;
}


bool array_push_back(array_t* sortedXArray, pairNum_t numPair) {
	// if capacity is reached, double array size 
	if (sortedXArray->nCapacity == sortedXArray->nSize) {
		size_t newCapacity = sortedXArray->nCapacity * 2;

		// account for starting the capacity of zero
		if (0 == newCapacity) ++newCapacity;

		// reallocate memory 
		pairNum_t* pNewBlock = realloc(sortedXArray->data, newCapacity * sizeof(pairNum_t));

		// if memory can't be reallocated, return false
		if (NULL == pNewBlock)
			return false;

		// update values in the array_t object
		sortedXArray->data = pNewBlock;
		sortedXArray->nCapacity = newCapacity;
	}

	// append element to the end of the used space of the array
	sortedXArray->data[sortedXArray->nSize++] = numPair;
	return true;
}


void array_free(array_t* sortedXArray)
{
	free(sortedXArray->data);
	*sortedXArray = make_array(); 
}


array_t makeACopyArray(array_t* sortedXArray)
{
	//make the copied array and transfer all the values from the sortedXArray to ensure correct capacity
	array_t copiedArray = make_array();
	copiedArray.data = calloc(sortedXArray->nCapacity, sizeof(pairNum_t));
	copiedArray.nCapacity = sortedXArray->nCapacity;
	copiedArray.nSize = sortedXArray->nSize;

	//iterate through the array and grab each real-num pair and store it in the copiedArray 
	for (unsigned i = 0; i < sortedXArray->nSize; i++)
	{
		pairNum_t copiedPair;
		copiedPair.xValue = sortedXArray->data[i].xValue;
		copiedPair.yValue = sortedXArray->data[i].yValue;

		copiedArray.data[i] = copiedPair;
	}

	return copiedArray;
}


bool filterStream(FILE* stream, bool eatCommas)
{
	int ch = 0;
	//ignore if EOF, new line, comma and only accept if it is a digit and ignore the (-) before a numerical value 
	while ((ch = fgetc(stream)) != EOF && (isspace(ch) || ch == '\n' || (ch == ',' && eatCommas)));
	if (ch != EOF && (isdigit(ch) || ch == '-'))
	{
		ungetc(ch, stream);
		return true;
	}
	return false;
}


pairNum_t calcMean(array_t sortedXArray)
{
	pairNum_t mean;
	data_set_type sumOfX = 0;
	data_set_type sumOfY = 0;
	//add all the values  in the set
	for (unsigned i = 0; i < sortedXArray.nSize; i++)
	{
		sumOfX += sortedXArray.data[i].xValue;
		sumOfY += sortedXArray.data[i].yValue;
	}
	//divide by the total number of values in the sets 
	mean.xValue = sumOfX / sortedXArray.nSize;
	mean.yValue = sumOfY / sortedXArray.nSize;

	return mean;
}


pairNum_t calcAbsDevStats(array_t sortedXArray, pairNum_t m)
{
	pairNum_t absDevStats;

	data_set_type sumOfX = 0;
	data_set_type sumOfY = 0;
	//add the absolute sum of all the values distance from the mean 
	for (unsigned i = 0; i < sortedXArray.nSize; i++)
	{
		sumOfX += fabs(sortedXArray.data[i].xValue - m.xValue);
		sumOfY += fabs(sortedXArray.data[i].yValue - m.yValue);
	}
	//divde by the total number of values in the set
	absDevStats.xValue = sumOfX / sortedXArray.nSize;
	absDevStats.yValue = sumOfY / sortedXArray.nSize;

	return absDevStats;

}


pairNum_t calcMedian(array_t sortedXArray, array_t sortedYArray)
{
	pairNum_t median;
	//if the array has an even set of values then we need to take the average of the two middle values 
	if (sortedXArray.nSize % 2 == 0)
	{
		median.xValue = (sortedXArray.data[(int)((sortedXArray.nSize - 1) / 2)].xValue + sortedXArray.data[(int)((sortedXArray.nSize) / 2)].xValue) / 2.0;
		median.yValue = (sortedYArray.data[(int)((sortedYArray.nSize - 1) / 2)].yValue + sortedYArray.data[(int)((sortedYArray.nSize) / 2)].yValue) / 2.0;
	}
	else
	{	//find middle value if odd set 
		median.xValue = sortedXArray.data[(int)((sortedXArray.nSize) / 2)].xValue;
		median.yValue = sortedYArray.data[(int)((sortedYArray.nSize) / 2)].yValue;
	}

	return median;
}

array_t calcMode(array_t sortedArray, bool findingXModes)
{
	unsigned count = 0, maxCount = 0;
	array_t modeArray = make_array();

	if (findingXModes)
	{
		for (unsigned i = 0; i < sortedArray.nSize - 1; i++)
		{
			//compare if the two values next to one another are the same
			if (sortedArray.data[i].xValue == sortedArray.data[i + 1].xValue)
			{
				count++;
			}//only execute if the values are not the same and it is not last comparison in the array! 
			if ((sortedArray.data[i].xValue != sortedArray.data[i + 1].xValue) ||
				(sortedArray.data[i].xValue == sortedArray.data[i + 1].xValue && (i == sortedArray.nSize - 2)))
			{
				//if value of greater frequency is found and replace the value and the maxCount

				if (count > maxCount)
				{
					maxCount = count;
					//obtain the new mode value and add it to the array

					pairNum_t modeValues = create_numPair();
					modeValues.xValue = sortedArray.data[i].xValue;
					//clear out the value that previously held mode 
					modeArray.nSize = 0;
					array_push_back(&modeArray, modeValues);
				}
				else if (count == maxCount && maxCount > 0)
				{//if its multi-model and add the mode value to the array 
					pairNum_t modeValues = create_numPair();
					modeValues.xValue = sortedArray.data[i].xValue;

					array_push_back(&modeArray, modeValues);
				}

				count = 0;
			}
		}
		//y-values are empty so we take advantage of that and store our mode frequency count in it 
		if (maxCount > 0)
		{
			modeArray.data[0].yValue = maxCount + 1.0;
		}
	}
	else
	{	//same idea as for sortedXarray, but we are finding y mode values this time 
		for (unsigned i = 0; i < sortedArray.nSize - 1; i++)
		{
			if (sortedArray.data[i].yValue == sortedArray.data[i + 1].yValue)
			{
				count++;
			}
			if ((sortedArray.data[i].yValue != sortedArray.data[i + 1].yValue) ||
				(sortedArray.data[i].yValue == sortedArray.data[i + 1].yValue && (i == sortedArray.nSize - 2)))
			{
				if (count > maxCount)
				{
					maxCount = count;

					pairNum_t modeValues = create_numPair();
					modeValues.yValue = sortedArray.data[i].yValue;
					modeArray.nSize = 0;
					array_push_back(&modeArray, modeValues);
				}
				else if (count == maxCount && maxCount > 0)
				{
					pairNum_t modeValues = create_numPair();
					modeValues.yValue = sortedArray.data[i].yValue;

					array_push_back(&modeArray, modeValues);
				}

				count = 0;
			}
		}
		if (maxCount > 0)
		{
			modeArray.data[0].xValue = maxCount + 1.0;
		}
	}
	//if all the values occur more than once and frequency of all is equal, than no mode exists
	//set the array size to 0 so we can output no mode value in print statements 
	if (modeArray.nSize * (maxCount + 1.0) == sortedArray.nSize)
	{
		modeArray.nSize = 0;
	}

	return modeArray;

}

pairNum_t calcVariance(array_t sortedXArray, pairNum_t mean)
{
	pairNum_t variance;
	data_set_type xValuenumerator = 0;
	data_set_type yValuenumerator = 0;

	//find the expected value of the squared deviation from the mean 
	for (unsigned i = 0; i < sortedXArray.nSize; i++)
	{
		xValuenumerator += (sortedXArray.data[i].xValue - mean.xValue) * (sortedXArray.data[i].xValue - mean.xValue);
		yValuenumerator += (sortedXArray.data[i].yValue - mean.yValue) * (sortedXArray.data[i].yValue - mean.yValue);
	}
	//divde by the total number of values in the set
	variance.xValue = (xValuenumerator) / sortedXArray.nSize;
	variance.yValue = (yValuenumerator) / sortedXArray.nSize;

	return variance;
}


pairNum_t calcStdDeviation(array_t sortedXArray, pairNum_t variance)
{
	pairNum_t stdDeviation;
	//square the variance value obtained from the calcVariance function 
	stdDeviation.xValue = sqrt(variance.xValue);
	stdDeviation.yValue = sqrt(variance.yValue);

	return stdDeviation;
}


pairNum_t calcRegressionLine(array_t sortedXArray)
{
	//NOTE: even though pairNum_t is meant to hold x and y values, this function will store b and a respectively 
	pairNum_t regresLineValues;

	double sumOfX = 0;
	double sumOfY = 0;
	double sumOfXSquared = 0;
	double multOfXnY = 0;
	//using the formula of regression line, iterating through the array to commute the values 
	for (unsigned i = 0; i < sortedXArray.nSize; i++)
	{
		sumOfX += sortedXArray.data[i].xValue;
		sumOfY += sortedXArray.data[i].yValue;
		sumOfXSquared += (sortedXArray.data[i].xValue) * (sortedXArray.data[i].xValue);
		multOfXnY += (sortedXArray.data[i].xValue * sortedXArray.data[i].yValue);
	}

	//store the slope of the line 
	regresLineValues.xValue = (sortedXArray.nSize * (multOfXnY)-(sumOfX * sumOfY)) / (sortedXArray.nSize * sumOfXSquared - sumOfX * sumOfX);
	//store the y-intercept
	regresLineValues.yValue = (sumOfY - regresLineValues.xValue * (sumOfX)) / sortedXArray.nSize;

	return regresLineValues;
}

pairNum_t calcYatMidX(pairNum_t regressLineValues, data_set_type minX, data_set_type maxX)
{
	pairNum_t yAtMidX;
	data_set_type m = regressLineValues.xValue;
	data_set_type b = regressLineValues.yValue;
	//find the mid-x value 
	yAtMidX.xValue = (minX + maxX) / 2;
	//use equation of line (y = mx+b) to find y value 
	yAtMidX.yValue = m * yAtMidX.xValue + b;

	return yAtMidX;
}

void swap(data_set_type* pa, data_set_type* pb)
{
	data_set_type hold = *pa;
	*pa = *pb;
	*pb = hold;
}

void quickSort(array_t sortedXArray, array_t sortedYArray, data_set_type low, data_set_type high)
{
	int xValuelo = (int)low;
	int xValuehi = (int)high;

	data_set_type xValuePivotValue = sortedXArray.data[(int)((low + high) / 2)].xValue;

	//sort xValue (x) values
	while (xValuelo <= xValuehi)
	{
		while (sortedXArray.data[xValuelo].xValue < xValuePivotValue)
		{
			xValuelo++;
		}
		while (sortedXArray.data[xValuehi].xValue > xValuePivotValue)
		{
			xValuehi--;
		}
		if (xValuelo <= xValuehi)
		{
			swap(&sortedXArray.data[xValuelo].xValue, &sortedXArray.data[xValuehi].xValue);
			swap(&sortedXArray.data[xValuelo].yValue, &sortedXArray.data[xValuehi].yValue);
			xValuelo++;
			xValuehi--;
		}
	}
	if (low < xValuehi)
	{
		quickSort(sortedXArray, sortedYArray, low, xValuehi);
	}
	if (xValuelo < high)
	{
		quickSort(sortedXArray, sortedYArray, xValuelo, high);
	}

	//sort yValue (y) values
	int yValuelo = (int)low;
	int yValuehi = (int)high;
	data_set_type yValuePivotValue = sortedYArray.data[(int)((low + high) / 2)].yValue;
	while (yValuelo <= yValuehi)
	{
		while (sortedYArray.data[yValuelo].yValue < yValuePivotValue)
		{
			yValuelo++;
		}
		while (sortedYArray.data[yValuehi].yValue > yValuePivotValue)
		{
			yValuehi--;
		}
		if (yValuelo <= yValuehi)
		{
			swap(&sortedYArray.data[yValuelo].yValue, &sortedYArray.data[yValuehi].yValue);
			swap(&sortedYArray.data[yValuelo].xValue, &sortedYArray.data[yValuehi].xValue);
			yValuelo++;
			yValuehi--;
		}
	}
	if (low < yValuehi)
	{
		quickSort(sortedXArray, sortedYArray, low, yValuehi);
	}
	if (yValuelo < high)
	{
		quickSort(sortedXArray, sortedYArray, yValuelo, high);
	}
}

void findOutliers(array_t sortedYArray, pairNum_t stdDeviation, pairNum_t mean)
{
	data_set_type stdDeviationY = stdDeviation.yValue;
	data_set_type meanY = mean.yValue;
	data_set_type diffFromMean = 0;

	//have to arrays that will store the all the outlier pairs 
	array_t outliers2xArray = make_array();
	array_t outliers3xArray = make_array();

	//determine using the sortedYArray, which values are outliers by 2*stdDev and 3*stdDev 
	//store the num pair that meets the requirements in the array 
	for (unsigned i = 0; i < sortedYArray.nSize; i++)
	{	

		diffFromMean = fabs(sortedYArray.data[i].yValue - meanY);
		if (diffFromMean > stdDeviationY * 3)
		{
			pairNum_t numPair = create_numPair();
			numPair.xValue = sortedYArray.data[i].xValue;
			numPair.yValue = sortedYArray.data[i].yValue;
			array_push_back(&outliers3xArray, numPair);
		}
		if (diffFromMean > stdDeviationY * 2)
		{
			pairNum_t numPair = create_numPair();
			numPair.xValue = sortedYArray.data[i].xValue;
			numPair.yValue = sortedYArray.data[i].yValue;
			array_push_back(&outliers2xArray, numPair);
		}
	}
	//print out all the outliers and print "no outliers" if none found in the dataset 
	if (outliers2xArray.nSize == 0)
		printf("\n%-28s%3s", "Outliers(2x)", "no outliers");
	else
		printf("\n%-23s%s%4d", "Outliers(2x)", "# outliers =", outliers2xArray.nSize);

	for (unsigned i = 0; i < outliers2xArray.nSize; i++)
		printf("\n%39.3lf %20.3lf", outliers2xArray.data[i].xValue, outliers2xArray.data[i].yValue);
	
	if (outliers3xArray.nSize == 0)
		printf("\n%-28s%3s", "Outliers(3x)", "no outliers");
	else
		printf("\n%-23s%s%4d", "Outliers(3x)", "# outliers =", outliers3xArray.nSize);


	for (unsigned i = 0; i < outliers3xArray.nSize; i++)
		printf("\n%39.3lf %20.3lf", outliers3xArray.data[i].xValue, outliers3xArray.data[i].yValue);
	
	//free up the memory 
	array_free(&outliers2xArray);
	array_free(&outliers3xArray);
}

