# hydcoeff

This project, titled **hydcoef**, is a C-based program with several functions for hydrodynamic calculations. The Makefile automates the compilation of C files and provides commands to manage the build process. The computations are based on 2D added mass coefficients estimated from theoretical calculations, with values read from the graph included in [[1](#1)]. Using slender body theory, these 2D coefficients are then integrated over the ship's length according to the Salvesen method [[1](#1)].

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Makefile Commands](#makefile-commands)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)
- [References](#reference)

---

## Installation <a id="installation"></a>

To get started, ensure you have the following prerequisites installed:

- **GCC** - The GNU Compiler Collection
- **GNU Make** - For executing the commands in the Makefile

### Clone the Repository

Clone the project from GitHub to your local machine:

```bash
git clone https://github.com/pciuh/hydcoef.git
cd hydcoef
```

## Usage <a id="usage"></a>

The project provides a Makefile to compile the source code into an executable. Once compiled, the program can be run from the command line.

To compile the program:

```bash
make all
```

This will generate an executable named `calhyd`. Further to calculate added mass and damping coefficients you need to create comma separated `hull.csv` including sectional ordinate `x` and section radius `R`. All these must be dimesionless with respect to ship length `L`.

Next the `conf.csv` file including ship length `L` and min/max value of wave frequency `wMin`, `wMax` are demanded. For preparation of `csv` files look into example `hull/hull.csv` and `conf.csv`. Then run program from command line:

```bash
./calhyd hull/hull.csv conf.csv
```

Added mass and damping coefficients are normalized by calculated ship displacement and pitch moment of inertia. Rsults are stored into the `hull/` folder. Name of results file are e.g.: `hull-hc.csv`.

## Makefile Commands <a id="makefile-commands"></a>

Below is a list of commands available in the Makefile:

| Command         | Description                                                                                       |
|-----------------|---------------------------------------------------------------------------------------------------|
| `make all`      | Compiles the project, producing the `calhyd` executable.                                          |
| `make calhyd`   | Specifically compiles and links all object files to create the `calhyd` executable.               |
| `make clean`    | Removes all object files (`*.o`) to clean the build environment.                                  |

### Compilation Details <a id="project-structure"></a>

- **Compiler**: `gcc`
- **Flags**: `-g3 -Wall -ggdb` - These flags enable debug symbols, all warnings, and gdb debugging.
- **Source Files**:
  - `main.c`
  - `adm_approx.c`
  - `dam_approx.c`
  - `hydro.c`
  - `in_out_files.c`
  - `print_coef.c`
- **Header File**:
  - `func.h`

The object files generated during compilation are:

- `main.o`
- `adm_approx.o`
- `dam_approx.o`
- `hydro.o`
- `in_out_files.o`
- `print_coef.o`

In case of troubles with compilation it is recomemended to use `Dockerfile` then build and run image by command:
```bash
docker build -t calhyd-gcc-app .
docker run -it --rm --name calhyd-running calhyd-gcc-app
```
Above commands should be executed in folder `hydcoef`.

## Project Structure

```
calhyd/
├── hull              # Directory containing hull files and results
├── main.c            # Main source file
├── adm_approx.c      # Source file for added mass calculations
├── dam_approx.c      # Source file for damping calculations
├── hydro.c           # Source file for hydrostatic calculations
├── in_out_file.c     # Source file for input/output flies operations
├── print_coef.c      # Source file for printing some results to screen
└── func.h            # Header file containing function declarations
```

## Contributing <a id="contributing"></a>

Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature.
3. Submit a pull request.

## License <a id="license"></a>

This project is licensed under the MIT License. See the [LICENSE](https://github.com/pciuh/hydcoef/blob/main/LICENSE) file for more details.

## References <a id="reference"></a>

<a id="1">[1]</a> Faltinsen, O.M. (1990) Sea Loads on Ships and Offshore Structures. Cambridge University Press, 1-5.
```bibtex
@book{faltinsen1993sea,
  title={Sea Loads on Ships and Offshore Structures},
  author={Faltinsen, O.},
  isbn={9780521458702},
  lccn={90043346},
  series={Cambridge Ocean Technology Series},
  url={https://books.google.pl/books?id=qZq4Rs2DZXoC},
  year={1993},
  publisher={Cambridge University Press}
}
```