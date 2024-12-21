## How to run?
- ```git clone <RSGP_cpp>```
- Make sure the dataset folder ```testata_v4``` exists. ```./testdata_v4``` should be placed in ```../RSGP_cpp```.
- ```make clean```
- ```make```
- ```./main smallest 1 ../testdata_v4```

Note1: The format is ```./<exe name> <task> <random seed number> <path to the problem dataset>```

Note2: The link of ```../testdata_v4``` is https://drive.google.com/file/d/1GBE5OuOXcygfD-bBK4P-m7GCStVWcHJW/view?usp=sharing

## Tasks (Benchmark problems)
- task = {smallest, count-odds, last-index-of-zero, mnp, mpmn, mptmn, fuel-cost, luhn}
- smallest = MIN in CEC paper
- count-odds = CountOdds in CEC paper
- last-index-of-zero = LastIndexofZero in CEC paper
- mnp = MPI in CEC paper
- mpmn = MPMN in CEC paper
- mptmn = MPTMN in CEC paper
- fuel-cost = FuelCost in CEC paper
- luhn = Luhn in CEC paper

## Parameters
We can adjust those parameters in the function run(...) in ```main.cpp```
- ```population size = 332``` (for each subpopulation)
-  ```generation = 500```

## Others
- ```grammar.cpp```: general grammar table
- ```functions.cpp```: implementation of functions that are used in the grammar
- ```ast.cpp```: loop execution

