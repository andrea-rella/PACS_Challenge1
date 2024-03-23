#ifndef DATA_F683239F_AA72_4C18_942E_7205ECA1A71A
#define DATA_F683239F_AA72_4C18_942E_7205ECA1A71A

#include <vector>
#include <iostream>
#include <functional>
#include <string>
#include "json.hpp"
#include <fstream>
#include "mu_Vfun.hpp"
#include "Utilities.hpp"

using json = nlohmann::json;

enum Step
{
    CONST,       // Const
    EXPONENTIAL, // Exponential Decay
    INVERSE,     // Inverse Decay
    ARMIJO       // Armijo Rule
};

enum Optimizer
{
    GD,       // Gradient Descent
    HB,       // Heavy Ball / Momentum Gradient Descent
    NESTEROV, // Nesterob
    ADAM      // ADAM
};

struct Data
{

    // GENERAL PARAMETERS
    std::vector<double> x0;
    double e_s;
    double e_r;
    unsigned int max_it;
    double sigma;
    double a0;
    double mu;

    // ADAM PARAMETERS
    double gamma1; // Decay factor for first momentum
    double gamma2; // Decay factor for second momentum
    double eps;    // Small constant to prevent division by zero

    // HEAVY BALL / NESTEROV PARAMETERS
    double eta;

    // OPTIMIZER'S OPTIONS
    std::function<double(const unsigned int, const std::vector<double> &)> Alpha; // Function for computing the step
    int algorithm;
    int FiniteDifferences;
    double hFD;

    // STEP'S OPTIONS
    int step; // Algorithm chosen

    // FUNCTIONS
    std::function<double(const std::vector<double> &)> Fun;               // Objective function
    std::function<std::vector<double>(const std::vector<double> &)> Grad; // Gradient function

    // CONSTRUCTOR
    Data(const std::string &filename)
    {
        // Parameters will be read from a .json file
        std::ifstream f(filename);
        json data = json::parse(f);

        // ---------------------------------------------------
        // ---------   INITIALISING THE PARAMETERS   ---------
        // ---------------------------------------------------

        std::vector<double> init = data["initial"];
        x0 = init;
        e_s = data.value("tol_s", 1e-6);
        e_r = data.value("tol_r", 1e-6);
        max_it = data.value("max_it", 100);
        a0 = data.value("alpha0", 0.1);
        mu = data.value("decayParameter", 0.1);

        // ---------------------------------------------------
        // ---------    INITIALISING THE FUNCTION    ---------
        // ---------------------------------------------------

        std::string funString = data.value("fun", "");
        std::vector<std::string> gradString = data["grad"];
        mu_Sfun muFun(funString, x0.size());
        mu_Vfun muGrad(gradString, x0.size());
        Fun = muFun;

        // ---------------------------------------------------
        // ---------   INITIALISING THE ALGORITHM    ---------
        // ---------------------------------------------------

        algorithm = data.value("algorithm", 0);
        FiniteDifferences = data.value("FiniteDifferences", 1);
        hFD = data.value("hFD", 1e-4);
        gamma1 = data.value("gamma1", 0.9);
        gamma2 = data.value("gamma2", 0.99);
        eps = data.value("epsilon", 1e-8);
        eta = data.value("eta", 0.9);

        // ---------------------------------------------------
        // --------- CHOOSING THE TYPE OF STEP DECAY ---------
        // ---------------------------------------------------

        step = data.value("alphaMethod", 0);

        std::cout << "\n**** Setting the learning rate **** " << std::endl;

        bool useDefault = false;

        switch (step)
        {
        case 0:
            Alpha = [this](const unsigned int k, std::vector<double> x)
            { return a0; };
            std::cout << "--> Constant value"
                      << std::endl;
            break;

        case 1:
            Alpha = [this](const unsigned int k, std::vector<double> x)
            { return a0 * std::exp(-mu * k); };
            std::cout << "--> Exponential decay"
                      << std::endl;
            break;

        case 2:

            Alpha = [this](const unsigned int k, std::vector<double> x)
            { return a0 / (1 + mu * k); };
            std::cout << "--> Inverse decay"
                      << std::endl;
            break;

        case 3:
            if (algorithm == HB or algorithm == ADAM or algorithm == NESTEROV)
            {
                std::cerr << "--> Cannot use Armijo rule with Heavy ball, Nesterov or Adam."
                          << std::endl;
                useDefault = true;
            }
            else
            {
                Alpha = [this](const unsigned int k, const std::vector<double> &x)
                { return armijo(x, Fun, Grad, sigma, a0); };
                std::cout << "--> Armijo rule"
                          << std::endl;
            }
            break;

        default:
            useDefault = true;
            break;
        }

        if (useDefault)
        {
            Alpha = [this](const unsigned int k, std::vector<double> x)
            { return a0; };
            std::cout << "--> Alpha Method inserted not valid, constant decay applied. Check README.md for details"
                      << std::endl;
        }

        // ---------------------------------------------------------------------------------
        // --------- CHOOSING THE TYPE OF GRADIENT: EXTACT OR CENTERED DIFFERENCES ---------
        // ---------------------------------------------------------------------------------

        std::cout << "\n**** Setting the gradient computation method **** " << std::endl;

        if (FiniteDifferences == 1)
        {
            Grad = [this](const std::vector<double> &x)
            { return gradientFD(Fun, x, hFD); };
            std::cout << "--> Gradient will be computed through centered finite differences\n\n"
                      << std::endl;
        }
        else
        {
            Grad = muGrad;
            std::cout << "--> Gradient will be computed with the closed expression\n\n"
                      << std::endl;
        }
    }
};

#endif /* PARAMETERS_F683239F_AA72_4C18_942E_7205ECA1A71A */
