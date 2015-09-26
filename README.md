# Math-Evaluator
A simple c++ math expression evaluator which uses boost's spirit library.

# What can it evaluate?
Simple to complex math expressions, containing variables and functions. Here are a few examples:
```
5.2 + 1 - 13.777
2 * pi * r
pow(sin(x), 2) + pow(cos(x), 2)
3 * (x - 2) + 15 * (y + 8)
```

# Installation
Simply include boost and this library in your project.

# Usage
The usage is simple and intuitive.
```c++
// Create a compiler, compile a piece of code.
MathExpressionCompiler comp;
std::vector<unsigned char> code =
comp.compile("pow(2, x)");

// Create a VM on which we run that piece of code.
MathExpressionVM vm;
vm.setVar("x", 16);
vm.run(code); // returns 65536
vm.setVar("x", 8);
vm.run(code); // returns 256
```