## AlignmentArray  
try using align_val_t in std17 and simd assembly instructions to optimize the performance of array  
to generate this project, you should using visual studio and its assembler  
it has a runtime check for simd instructions
### Usage  
- create a file "Definations.h" and copy the following code:
```c++
#pragma once  

#ifdef ALIGNMENT_EXPORTS  
#define ALIGNMENT_API __declspec(dllexport)  
#else  
#define ALIGNMENT_API __declspec(dllimport)  
#pragma comment(lib, "Alignment.lib")  
#endif  
```
- add additional library directories  
- use it  
```c++
Alignment::Array<T> array(size_t, Alignment::AlignmentSize);  
// size_t is the total size of the array. you could get it by the count of the elements times there single size(sizeof(T)).
// Alignment::AlignmentSize is the alignment(in bytes) of the array. It has a default value which is 16
```
basic types has already been added in  
