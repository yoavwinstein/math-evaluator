# Math-Evaluator
A simple c++ math expression evaluator which uses boost spirit.

# Installation
Simply install boost and use this library.

# Usage
```c++
MathExpressionCompiler comp;
std::vector<unsigned char> code =
comp.compile("pow(2, x)");

MathExpressionVM vm;
vm.setVar("x", 16);
vm.run(code); // returns 65536
vm.setVar("x", 8);
vm.run(code); // returns 256
```