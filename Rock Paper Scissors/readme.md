# Rock-Paper-Scissors Game Documenation

![License](https://github.com/maxcomuk/Cpp-Projects/blob/main/LICENSE)

## Step 1
- we Include all the necessary libraries we will use from visual studio 2022

```
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
```

## Step 2
- We create 3 constant variables that will represent rock, paper and scissors. The reason i do this, is so that we dont have to worry about spelling issues later on and it can end up being a bit ambigous having writing them again

- One thing i learnt is that we cant use constexpr because std::string is not a literal type therefore the compiler wont know the value at compile time wich ruins the purpose of constexpr and it can even lead to some compilers failing to build because of this, so definatly something we will avoid by simply changing constexpr to a const string
```
const std::string rock = "Rock";
const std::string paper = "Paper";
const std::string scissors = "Scissors";
```

## Step 3
- Before starting this project i scanned through the project example wich was created here --> 
```
std::string generate_computers_move()
{
	const std::string objects[] = { rock, paper, scissors };

	int randomNum = (rand() % 3);
	return objects[randomNum];
}
```
