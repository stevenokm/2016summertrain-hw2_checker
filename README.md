
## Readme for hw2 checker ##

# Syntax #

``` bash
# check answer
hw2_checker <blif file> <output file>
# report golden answer
hw2_checker <blif file>
```

# Example #

``` bash
# check your answer
hw2_checker comb/des.blif des.out
# report golden answer
hw2_checker comb/des.blif
```

# Output #

``` bash
# check answer
ASAP
ASAP Passed
ALAP
Wrong answer <cell name 1> (<golden 1>:<output 1>)
Wrong answer <cell name 2> (<golden 2>:<output 2>)
......
Wrong answer <cell name n> (<golden 3>:<output n>)
ALAP Failed
# repot golden answer
ASAP
<cell name 1> (<golden 1>:0)
<cell name 2> (<golden 2>:0)
......
<cell name n> (<golden n>:0)
ALAP
<cell name 1> (<golden 1>:0)
<cell name 2> (<golden 2>:0)
......
<cell name n> (<golden n>:0)
```

# Compile #

Using ```make``` command to compile the checker

# Issues #

* This checker cannot handle ```.names``` statement with strings in number (ex. 9symml.blif, f51m.blif, z4ml.blif)
* This checker cannot handle ```.names``` statement with output only (ex. k2.blif)

# Notice #

* Your .out file need follow the format of output file

