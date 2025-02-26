# GeneratePopulation

This example shows how to use a configuration file to generate a population of cells.
The population is generated between two spheres and can be compacted or spaced to model real situations.

## Usage

The executable has 2 options:
- `-f filename`: path to the configuration file;
- `–vis`: generate a `.off` file to visualize your population with geomview (optional).

Example:
```bash
./generatePopulation -f data/exampleConfig.cfg --vis 
geomview data/exampleConfig.cfg.off &
```

In the data directory, you will find `exampleConfig.xml` which can be used to simulate radiation exposure in Geant4.
