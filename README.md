# bilevel scheduling industry 4.0

## Description
This project aims to offer an heuristic for the bilevel scheduling problem

## Installation
You must clone the repo and compile the code using Visual Studio 2019 in Release mode or use gcc<br>
<INSERT INSTRUCTIONS>

## Usage
This program is used in command line.<br>
You need to call the exe file to execute the program.

### Parameters
There are three paramaters available where two are mandatory :<br>
instancePath - instance's file path containing the instance's data with (Mandatory)<br>
resultDirectory - directory which will contain the result file (Mandatory)<br>
verbose - the level of verbose wanted from 0 to 5, default value is 0<br>
These 3 arguments must be passed between " ".<br>

### Expected behavior
According to the specified level of verbose, some text can appear on the console.

The solution is added at the end of the result file in resultDirectory.<br>
If the result file doesn't exist yet, it will be created with headers (cf Specification Result file).

## Specification

### Instance file

The instance file must respect the following format:
```
 name : <nomInstance>
 N : <nbjobsN>
 n : <nbJobsToSelect>
 M_max : <nbMachinesV_Max> 
 M_0 : <nbMachinesV_0>
 V_max : <vitesseV_Max>
 V_0 : <vitesseV_0>
 Jobs :
 <pj>\t <dj>\t <wj>
```
Where every element between <> must be replaced by a corresponding value.
The \t symbol represent a tabulation.
You can find an exemple in any performance directory in this gitHub.

### Result file

The generated result file will have the following format:
```
<InstanceName>; <InstanceFilePath>; <sum Cj>; <fonction objective (sum wjUj)>; <N>; <n>; <number of machines>; "<heuristic name>"; "<heuristic's description>"; <resolution time>
```
Where every element between <> must be replaced by the corresponding value.

## Authors and Acknowledgement


## License
