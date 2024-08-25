# progBuild is a build system I made to make my life easier.
It was originally written in python and then I moved it to C. It's a self hosted system for building C programs.

# Compilation Setps.

# First you will need to install the dependencies
    - cJSON
    - C build tools from your package manager
    - make for cJSON and  only if you dont have it installed already
    - cmake to isntall for cJSON

# Compilation commands and installation.

- if you have a older version you'll just have to run 

    ` progBuild -r C && sudo progBuild -I C `

- if you do not have if installed you will have to run 
 
    `make && sudo ./build/progBuild -I C `
