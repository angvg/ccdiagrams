# Coupled Cluster Amplitude Equation Diagram Generator
Generates Hugenholtz-type diagrams for coupled cluster amplitude equations. Equations with user supplied physical and cluster operators in the form <&Phi;<SUP>A</SUP><SUB>I</SUB>| V<SUB>n</SUB> T<SUB>2</SUB> | &Phi;> are translated.

### Build
To build the program from source, simply use `make`.

### Generating Diagrams
For generation of diagrams from user input operators run `app/ccdg`, for some predefined inputs and a verbose execution run `app/demo`. In addition to command line output, output files in DOT-language are created in `dot/`.

### Graphical Rendering
For graphical rendering the graphiz libraries are required, which can be obtained via

```sudo apt install graphviz```

After running `ccdg` or `demo` the .dot files can be rendered using `make render`. The graphical output is placed in `diagrams/` in postscript format.
