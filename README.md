
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


+-------+--------------+----------------+---------+------------------+
| Patch |    Event     |     Result     |  Event1 |     Result1      |
+=======+==============+================+=========+==================+
| Small | Extinction   | Empty          |         |                  |
+-------+--------------+----------------+---------+------------------+
| Big   | Colonization | Send propagule |         |                  |
+-------+--------------+----------------+---------+------------------+
|       |              | If empty       |         | small actual spc |
+-------+--------------+----------------+---------+------------------+
|       |              | If small==spc  | Growth  | Big              |
+-------+--------------+----------------+---------+------------------+
|       |              | If small>spc   | Compite | Small actual spc |
+-------+--------------+----------------+---------+------------------+

Table:


### Source code

The principal process is in the file IpsPatchStage.cpp and the following functions make the principal processes:

    IPSPatchStage::Evaluate()
    IPSPatchStage::EvalCell(int x,int y)

The file with extension inp have the parameters for the species. The structure line by line is:

1: xdim ydim

2: number of species

3: Parameters of species spreading from this to the following lines

The structure of the lines with species parameters is 

speciesNum GrowthRate ColonizationRate ExtinctionRate PerturbationRate CompetitionRate DispersalDistance 

 
### file.set

Specie stage numberofindividuals 0

## Bibliography


1. Durrett R, Levin SA (2000) Lessons on Pattern Formation from Planet WATOR. J Theor Biol 205: 201–214. doi:10.1006/jtbi.2000.2061.

1. Pascual M, Levin SA (1999) Spatial scaling in a benthic population models with density-dependent disturbance. Theor Popul Biol 56: 106–122.


License
=======

	Copyright 2011 Leonardo A. Saravia
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
