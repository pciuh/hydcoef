# hydcoeff

This project, titled **hydceff**, is a C-based program with several functions for hydrodynamic calculations. The Makefile automates the compilation of C files and provides commands to manage the build process. The computations are carried out basing on 2D added mass coefficients estiamted from theoretical computations by readings from graph included in [[1](#1)]. Further basing on slender body theory the 2D coefficients are integrated over the ship length according to Salvesen method [[1](#1)].

## Table of Contents

- [Installation](#installation)
- [Usage](#usage)
- [Makefile Commands](#makefile-commands)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)
- [Reference](#reference)

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

## Usage

The project provides a Makefile to compile the source code into an executable. Once compiled, the program can be run from the command line.

To compile the program:

```bash
make all
```

This will generate an executable named `calhyd`. Further to calculate added mass and damping coefficients you need to create comma separated `hull.csv` including sectional ordinate `x` and section radius `R`. All these must be dimesionless with respect to ship length `L`.

Next the `conf.csv` file including ship length `L` and min/max value of wave frequency `wMin`, `wMax` are demanded. For preparation of `csv` files please look into example `hull/hull.csv` and `conf.csv`. Then run program from command line:

```bash
calhyd hull/hull.csv conf.csv
```

## Makefile Commands

Below is a list of commands available in the Makefile:

| Command         | Description                                                                                       |
|-----------------|---------------------------------------------------------------------------------------------------|
| `make all`      | Compiles the project, producing the `calhyd` executable.                                          |
| `make calhyd`   | Specifically compiles and links all object files to create the `calhyd` executable.               |
| `make clean`    | Removes all object files (`*.o`) to clean the build environment.                                  |

### Compilation Details

- **Compiler**: `gcc`
- **Flags**: `-g3 -Wall -ggdb` - These flags enable debug symbols, all warnings, and gdb debugging.
- **Source Files**:
  - `main.c`
  - `adm_approx.c`
  - `dam_approx.c`
- **Header File**:
  - `func.h`

The object files generated during compilation are:

- `main.o`
- `adm_approx.o`
- `dam_approx.o`

## Project Structure

```
calhyd/
├── main.c            # Main source file
├── adm_approx.c      # Source file for adm_approx calculations
├── dam_approx.c      # Source file for dam_approx calculations
└── func.h            # Header file containing function declarations
```

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature.
3. Submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## References

<a id="1">[1]</a>Faltinsen, O., Sea Loads on Ships and Offshore Structures
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