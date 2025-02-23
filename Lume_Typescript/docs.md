# Lume documentation and grammer rules

# Compiler

the program is split into two parts. The compiler, and the intepriter. The compiler is used generate a build folder which the interpiter can read. The reason for doing this, is because it makes it easy to discover syntax error and clean out unreached code, comments, mathematical operations that can be pre calculated, and other stuff thats unnecessary for the program.

# interpiter

The interperiter is whats actually used to execute the code. When compiling is done, and you have a build folder, you can run it via the interpriter

### Comments

comments in lume Are created by encasing text into a less-than and greater-than symbols
<br>
<br>
\<This is an example\>

### variables

variables can be created by assigning a type, then var name, followed by equals and then the value
<br>
<br>
[string] helloWorld -> "helloWorld";
<br>
[int16] pi -> 3.14;

### if statements

if not isTrue = true or isTrue = false;
<br>
     [int] hi -> 1.4
<br>
close

### Integers

Normal ints do not exist. Only int16, int8m, int32, int64


### functions

[bool] func Foo([bool] state, [int16] number);
<br>
close
