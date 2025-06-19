---
title: Bitcoin的white paper
tags: ['[]']
id: 569
date: 2021-09-19T12:08:39+08:00
---


**青山一道同云雨，明月何曾是两乡**。——《送柴侍御》\[唐\]王昌龄

解以平，18年时因为对Block chain的思考（一窍不通),进行了所谓勇敢的“探索”

```
#include <math.h>
double AttackerSuccessProbability(double q, int z)
{
    double p = 1.0 - q;double lambda = z * (q / p);
    double sum = 1.0;
    int i, k;
    for (k = 0; k <= z; k++)
    {
        double poisson = exp(-lambda);
        for (i = 1; i <= k; i++)
             poisson *= lambda / i;
        sum -= poisson * (1 - pow(q / p, z - k));
    }
    return sum;
}
.
```