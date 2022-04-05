# The Soil Language
Soil is an interpreted language for the MarsOS.    

***Global Variables***    
DIR,
ARGS

***Syntax***    
include "PATH"    
struct login {    
  &emsp;  str username,    
  &emsp;  str password,    
}    
str name = 'soil'    
if (name != 'soil) {    
    &emsp;    printf("name is: $name")    
}    
null func_name(str p) {    
 &emsp;   return null    
}    
    
***File Extension***    
*.se    
    
***Commands***    
soil -s MAIN_FILE_NAME    
--source    

***these programs will be written in Soil for MarsOS***
1. proc
2. mem
3. fs
4. net
5. init
6. usr
6. soil
