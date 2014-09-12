
# Spatial stochastic Patch model with 2 stages

## Mean field model

There are 2 kinds of algae (A) patches small (s) and big (b) and 2 species, $A_{1s}$ is a species 1 small patch. Small patches represent a lower algae density than big patches.

First formulation:

$$dE/dt = -k_1 E A_{1b} - k_2 E A_{2b} + u_1 A_{1s} + u_2 A_{2s}$$

$$dA_{1s}/dt = k_1 E A_{1b} - u_1 A_{1s} - g_1 A_{1s} A_{1b} - c_{12} A_{1s} A_{2b} + p_1 A_{1b}$$

$$dA_{2s}/dt = k_2 E A_{2b} - u_2 A_{2s} - g_2 A_{2s} A_{2b} + c_{12} A_{1s} A_{2b} + p_2 A_{2b}$$

$$dA_{1b}/dt = g_1 A_{1s} A_{1b} - p_1 A_{1b}$$

$$dA_{2b}/dt = g_2 A_{2s} A_{2b} - p_2 A_{2b}$$

Second formulation:

$$dA_{1s}/dt = k_1 A_{1b} (E - g_1 A_{1s} A_{1b} - c_{12} A_{1s} A_{2b}) + p_1 A_{1b} - u_1 A_{1s}$$

$$dA_{2s}/dt = k_2 A_{2b} (E - g_2 A_{2s} A_{2b} + c_{12} A_{1s} A_{2b}) + p_2 A_{2b} - u_2 A_{2s}$$


## Stochastic spatial model

The spatial model can have N species. Species *i* replaces species *j* with rate *CompetitionRate* if $i<j$.
    
### Parameters


*ColonizationRate*: Big patches disperse colonizers ($p_{i}$)

*GrowthRate*: From small to big patches ($g_{i}$)

*CompetitionRate*: Replacement of species $i<j$  ($c_{i}$)

*ExtinctionRate*:Small patch extinction ($u_{i}$)

*PerturbationRate*: Perturbations transform big patches into small ones ($p_{i}$)


The events of the model are:

| Patch |    Event     |     Result     |  Event1 |     Result1      |
|-------|--------------|----------------|---------|------------------|
| Small | Extinction   | Empty          |         |                  |
| Big   | Colonization | Send propagule |         |                  |
|       |              | If empty       |         | small actual spc |
|       |              | If small==spc  | Growth  | Big              |
|       |              | If small>spc   | Compite | Small actual spc |


### Source code

The principal process is in the file IpsPatchStage.cpp and the following functions make the principal processes:

    IPSPatchStage::Evaluate()
    IPSPatchStage::EvalCell(int x,int y)

### Parameter files

#### inp files

The files with extension **inp** have the parameters for the species. The structure line by line is:

1: xdim ydim

2: number of species

3: Parameters of species spreading from this to the following lines

The structure of the lines with species parameters is 

    speciesNum GrowthRate ColonizationRate ExtinctionRate PerturbationRate CompetitionRate DispersalDistance 

#### set files

The files with extension **set** have the number and stages of individuals to set the initial conditions with random spatial distribution, the structure of the file have 4 columns, and can have several lines for different stages:

    Specie stage numberofindividuals 0



## Bibliography

1. Quartino ML, Deregibus D, Campana GL, Latorre GEJ, Momo FR (2013) Evidence of Macroalgal Colonization on Newly Ice-Free Areas following Glacial Retreat in Potter Cove (South Shetland Islands), Antarctica. PLoS One 8: e58223. doi:10.1371/journal.pone.0058223.

1. Durrett R, Levin SA (2000) Lessons on Pattern Formation from Planet WATOR. J Theor Biol 205: 201–214. doi:10.1006/jtbi.2000.2061.

1. Pascual M, Levin SA (1999) Spatial scaling in a benthic population models with density-dependent disturbance. Theor Popul Biol 56: 106–122.


# License


    The MIT License (MIT)

    Copyright (c) 2013 Leonardo A. Saravia

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
