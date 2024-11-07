#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <utility>
#include <algorithm>
#include <numeric>
#include <set>
#include <cmath>

#include "Point.h"

class KDNode 
{
public:
	KDNode(const int& k);
	Point point;
	KDNode* left, * right;
};

