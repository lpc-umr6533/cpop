<!--toc:start-->
 - [How to build?](#how-to-build)
 - [Options](#options)
 - [Output](#output)
 - [How to use?](#how-to-use)
<!--toc:end-->

This example shows you how to irradiate

## Options

- 3 geometries are available: a 95 µm radius spheroid with 3 degrees of compaction (25, 50 and 75 %);
- number of particles is initially set at 1 particle per cell, can be changed;
- emission site of particles in cells can be changed (membrane, cytoplasm, homogeneous or nucleus only);
- percentage of cells labeled with particles;
- diffusion of radionuclide's daughter after fixation (only for At-211 for now).

Simulation initialization can be a bit faster if `.gdml` writing is deactivated in `SpheroidalCellMesh.cc`.

## Output

With the `/cpop/population/eventInfo 1` option (the intended one for this example), the output root file contains:

- the initial name of the Particle in the event: here an alpha. If the name is `EndOfRun`, it means that the data are at the run level, not at the event action one (each value will be equal to 0, except the doses in cells, and their id);
- each time the particle enter in a nucleus during the event: its entrances energies;
- each time the particle exit in a nucleus during the event: its exit energies. If the particle stops in the nucleus, Ef = 0;
- ID of the cell that the particle crossed;
- eventID:
- ID of the cell from where the particle originated;
- If particleName is `EndOfRun`, dose deposited in the cell nucleus;
- If particleName is `EndOfRun`, dose deposited in the cell;
- If particleName is `EndOfRun`, dose deposited in the spheroid;
- If the particle is this event has diffused, equal to 1.

## How to use?

Before execution:
- create an `output` directory where you will execute (`.`);
- create `GDML` and `TXT` directories in `../../example` from where you will execute.

```sh
source path/to/geant4/gean4.sh
./targetedAlphaTherapy -m data/run.mac
```

If you are using `G4MULTITHREADED`, merge your root outputs with:

```sh
hadd output.root output_t{0..N}.root
```

Example: if you used 4 threads:

```sh
hadd result.root output_t{0..3}.root
```

```
Help :

Usage: complexRadiation  [-t <int>] -m=<file>

  Required options:
  -m, --macro=file          macro file(default: input_filename.mac)

  Optional options:
  -t, --thread=int          number of threads(only available with G4 multithread option)
```
