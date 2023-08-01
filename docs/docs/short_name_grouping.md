# Flag/Option short name grouping
## Flags
Flag short names can be grouped like so:  
`program -fbz`  
is equivalent to  
`program -f -b -z`

## Options
A *single* option can also be included in a group. 
Given a program `program` that accepts flags `-f`, `-b`, `-z`, and an option `-s`, the inputs    
`program -fbzsstring`  
`program -fbzs=string`  
`program -fbzs string`  
are all equivalent to  
`program -f -b -z -s=string`  
  
### Optional arguments 
Remember that the space separated syntax for [optional argument options](options.md#optional-argument-options) doesn't work like it does for required argument options.  
So the inputs  
`program -fbzsstring`  
`program -fbzs=string`  
are still equivalent to  
`program -f -b -z -s=string`  
but the input  
`program -fbzs string`  
is interpreted as providing `-s` without an argument.  


This behavior is necessary to allow optional argument options to be specified in option groups both with and without arguments.  
Just remember that the space separated syntax won't do what you want