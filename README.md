# Implementation Minimisation Algorithmsin C++

This project implements various minimization algorithms in C++, including Gradient Descent, [Heavy Ball](https://arxiv.org/pdf/1412.7457.pdf), [Nesterov](https://arxiv.org/pdf/1607.01981.pdf), and [Adam](https://arxiv.org/pdf/1412.6980.pdf). These algorithms are fundamental in optimization and find applications in various fields such as machine learning, physics, and engineering. The implementation allows users to minimize scalar functions of multiple variables efficiently.

The project utilizes two libraries: [nlohmann/json](https://github.com/nlohmann/json) for handling JSON data, providing a convenient and flexible way to specify algorithm parameters, and [muparser](https://github.com/beltoforion/muparser) for parsing mathematical expressions, enabling users to define objective functions and gradients easily.

## Requirements

- C++ compiler supporting C++11 or later
- [nlohmann/json](https://github.com/nlohmann/json) library
- [muparser](https://github.com/beltoforion/muparser) library

## Data

The data required for the algorithms is contained in a .json file with the following structure:

```json
{
  "initial": [0, 0],
  "tol_s": 1e-6,
  "tol_r": 1e-6,
  "max_it": 700,
  "sigma": 0.3,
  "alpha0": 0.1,

  "fun": "x1 * x2 + 4 * x1^4 + x2^2 + 3 * x1",
  "grad": ["x2 + 16 * x1^3 +3", "x1 + 2 * x2"],

  "decayParameter": 0.2,
  "alphaMethod": 0,

  "algorithm": 0,

  "FiniteDifferences": 0,
  "hFD": 1e-4,

  "gamma1": 0.9,
  "gamma2": 0.999,
  "epsilon": 1e-8,

  "eta": 0.9
}
```

- **initial**: Initial guess for the optimization variables.
- **tol_s**: Tolerance for the change in the objective function value between iterations.
- **tol_r**: Tolerance for the relative change in the objective function value between iterations.
- **max_it**: Maximum number of iterations.
- **sigma**: Parameter for the Heavy Ball algorithm.
- **alpha0**: Initial learning rate for algorithms such as Gradient Descent and Adam.
- **fun**: Mathematical expression defining the objective function (e.g. "3*x1*x2+x2^4", the variables are x1,x2,...).
- **grad**: Mathematical expressions defining the gradients of the objective function with respect to each variable.
- **decayParameter**: Parameter for controlling the learning rate decay ($\mu$).
- **alphaMethod**: Method for updating the learning rate: "0" for constant learning rate, "1" for exponential decay $\alpha_k = \alpha^0 e^{-\mu k}$, "2" for inverse decay $\alpha_k = \frac{\alpha^0}{1+\mu k} $, "3" for approximate line search (armijo).
- **algorithm**: Selected minimization algorithm: "0" for Gradient Descent, "1" for Heavy Ball, "2" for Nesterov, "3" for Adam.
- **FiniteDifferences**: Flag indicating whether to use finite differences for gradient computation (1 means use).
- **hFD**: Step size for finite differences.
- **gamma1**: Parameter for Adam algorithm.
- **gamma2**: Parameter for Adam algorithm.
- **epsilon**: Small value for numerical stability in Adam algorithm.
- **eta**: Momentum term for Nesterov algorithm.

These parameters allow users to customize the optimization process according to their specific requirements and problem characteristics.

## Usage

- Clone the repository with ```git clone https://github.com/andrea-rella/PACS_challenge1_Minimization.git```.
- Modify the paths in the make file to match personal /include and /lib directories. If you are a student following the PACS course of Professor Formaggia you should find
- Build the project using the provided Makefile
- Ensure that `LD_LIBRARY_PATH` environment variable is stored the path of the folder where the muparser $\texttt{.so}$ files reside.
- Run the executable
- Clean the created files with the exeption of the executable by running $\texttt{make clean}$ (run $\texttt{make distclean}$ if you want to get rid also of the executable)

## Possible Errors and Solutions

- **Compilation Errors**: Ensure that you have the necessary dependencies installed and configured properly. Check for any typos or syntax errors in the code.
- **Runtime Errors**: If encountering runtime errors, check the input data in the JSON file for correctness. Ensure that all required parameters are provided and correctly formatted.
- **Algorithm Convergence Issues**: Adjust algorithm parameters such as tolerance values, maximum iterations, and learning rates as necessary. Check the mathematical expressions for errors or singularities.
- **Library Compatibility**: Ensure that the versions of the libraries used are compatible with your compiler and system. Update or downgrade the libraries if necessary.

## Possible Improvements

The implementation is proned to improvements:

- hyper-parameter tuning
- implementing $\eta = 1-\alpha_k$
- make it more robust to initial choice of the point by
  multiple (eventually random) points and then taking the argmin among them
- account for domain bounds which, however, can be easily implemented in the case of N-dimesional rectangular domains (e.g $[a_1,b_1]\times[a_2,b_2]\times \dots \times[a_N,b_N]$).
