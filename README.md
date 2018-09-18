# Claw Machine

## Requirement

### Input format  
[the guarantee number G]  
[total number of customers]  
[customer1 arrive time][continuously play round][rest time][total play round number N]  
[customer2 arrive time][continuously play round][rest time][total play round number N]  

ex:  
```
15  
3  
0 3 3 3  
2 5 3 10  
4 2 3 9  
```


### Output format
ex:
```bash
# Output format
# [time] [customer ID] [movement] ([get prize or not])
    0       1       start playing
    2       2       wait in line
    3       1       finish playing   YES
```

[time]: all integer, we will not use unusual number as test data  
[movement]: ​ 
you only need to output these three kinds of movement  
- start playing
- finish playing
- wait in line​   
  
[get prize or not]: ​ YES/NO​ , you have to output the result after movement finish playingAdvanced   
[machine ID]: ​ #1/#2   
Output format:  
[time] [customer ID] [movement] ([get prize or not])[machine ID]  
 
ex:
```
0 1 start palying #1
2 2 start playing #2
```

Hint:
- For both input and output file, all information on the same line are separated by one
space digit.
- The test data may not have unique result. Every correct result is acceptable.
- The time unit is ‘​ round​ ’.