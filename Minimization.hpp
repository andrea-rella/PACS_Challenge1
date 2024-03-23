#ifndef MINIMIZATION_FB161DFE_D7BD_463C_8C11_150E82C21337
#define MINIMIZATION_FB161DFE_D7BD_463C_8C11_150E82C21337

#include "Data.hpp"
#include "Utilities.hpp"
#include "cmath"

/*

Implementatation of the minimization algorithms, in order:
* Gradient Descent
* Momentum Gradient Descent / Heavy Ball
* Nesterov
* ADAM

Moreover a wrapper is implemented to call one of these methods given a *Data* struct

*/

// ---------------------------------------------------------
// ---------           GRADIENT DESCENT            ---------
// ---------------------------------------------------------

std::vector<double> GradientDescent(const Data &data)
{

    size_t k = 0;
    double a = data.a0;
    std::vector<double> x(data.x0), x_prev(data.x0.size());
    std::vector<double> gradient(data.Grad(data.x0));

    bool converged = (norm2(gradient) < data.e_r); // if the condition on the gradient is met form
                                                   // the beginning i don't enter the loop

    while (!converged)
    {
        ++k; // enter the new iteration

        x_prev = x; // save the previous point

        x = x - a * gradient; // compute the update

        a = data.Alpha(k, x);    // update alpha
        gradient = data.Grad(x); // update gradient

        converged = norm2(data.Grad(x)) < data.e_r or norm2(x - x_prev) < data.e_s or k > data.max_it; // check stop condition
    }

    if (k == data.max_it)
    {
        std::cout << "\nMaximum number of iterations reached, the obtained point is: " << printVector(x) << std::endl;
    }
    else
    {
        std::cout << "\nGradient Descent converged to " << printVector(x) << " in  " << k << " iterations" << std::endl;
    }

    return x;
}

// ---------------------------------------------------------
// ---------       HEAVY BALL / MOMENTUM GD        ---------
// ---------------------------------------------------------

std::vector<double> HeavyBall(const Data &data)
{

    size_t k = 1;
    double a = data.a0;
    std::vector<double> x(data.x0 - a * data.Grad(data.x0));
    std::vector<double> x_prev(data.x0);
    std::vector<double> gradient(data.Grad(x));
    bool converged = (norm2(gradient) < data.e_r); // we check for x_1 (given the structure of the algorithm which
                                                   // morally starts having already done one iteration)
                                                   // meaning at least we move by one iteration

    while (!converged)
    {
        ++k;

        x_prev = x;

        x = x + data.eta * (x - x_prev) - a * gradient;

        a = data.Alpha(k, x);
        gradient = data.Grad(x);

        converged = norm2(data.Grad(x)) < data.e_r or norm2(x - x_prev) < data.e_s or k > data.max_it;
    }

    if (k == data.max_it)
    {
        std::cout << "\nMaximum number of iterations reached, the obtained point is: " << printVector(x) << std::endl;
    }
    else
    {
        std::cout << "\nMomentum Gradient Descent converged to " << printVector(x) << " in " << k << " iterations " << std::endl;
    }

    return x;
}

// ---------------------------------------------------------
// ---------               NESTEROV                ---------
// ---------------------------------------------------------

std::vector<double> Nesterov(const Data &data)
{

    size_t k = 1;
    double a = data.a0;

    std::vector<double> x(data.x0 - a * data.Grad(data.x0));
    std::vector<double> x_prev(data.x0);
    std::vector<double> y(data.x0.size());
    std::vector<double> gradient(data.Grad(x));
    bool converged = (norm2(gradient) < data.e_r); // we check for x_1 (given the structure of the algorithm which
                                                   // morally starts having already done one iteration)
                                                   // meaning at least we move by one iteration

    while (!converged)
    {
        ++k;

        x_prev = x;

        y = x + data.eta * (x - x_prev);

        x = y - a * data.Grad(y);

        a = data.Alpha(k, x);
        gradient = data.Grad(x);

        converged = norm2(data.Grad(x)) < data.e_r || norm2(x - x_prev) < data.e_s || k > data.max_it;
    }

    if (k == data.max_it)
    {
        std::cout << "\nMaximum number of iterations reached, the obtained point is: " << printVector(x) << std::endl;
    }
    else
    {
        std::cout << "\nNesterov Algorithm converged to " << printVector(x) << " in " << k << " iterations " << std::endl;
    }

    return x;
}

// ---------------------------------------------------------
// ---------                 ADAM                  ---------
// ---------------------------------------------------------

std::vector<double> Adam(const Data &data)
{

    size_t k = 0;
    double a = data.a0;

    std::vector<double> x(data.x0), x_prev(data.x0.size());
    std::vector<double> m(data.x0.size(), 0.0);
    std::vector<double> v(data.x0.size(), 0.0);
    std::vector<double> gradient(data.Grad(data.x0));
    bool converged = (norm2(gradient) < data.e_r);

    while (!converged)
    {
        ++k;
        x_prev = x;

        for (size_t i = 0; i < x.size(); ++i)
        {
            m[i] = data.gamma1 * m[i] + (1 - data.gamma1) * gradient[i];
            v[i] = data.gamma2 * v[i] + (1 - data.gamma2) * std::pow(gradient[i], 2);
            double m_hat = m[i] / (1 - std::pow(data.gamma1, k));
            double v_hat = v[i] / (1 - std::pow(data.gamma2, k));
            x[i] -= a * m_hat / (std::sqrt(v_hat) + data.eps);
        }

        a = data.Alpha(k, x);
        gradient = data.Grad(x);

        converged = k > data.max_it or norm2(data.Grad(x)) < data.e_r or norm2(x - x_prev) < data.e_s or k > data.max_it;
    }

    if (k == data.max_it)
    {
        std::cout << "\nMaximum number of iterations reached, the obtained point is: " << printVector(x) << std::endl;
    }
    else
    {
        std::cout << "\nADAM converged to " << printVector(x) << " in " << k << " iterations" << std::endl;
    }

    return x;
}

//------------------------------------------------------------

std::vector<double> minimize(const Data &d)
{
    switch (d.algorithm)
    {
    case 0:
        return GradientDescent(d);
        break;
    case 1:
        return HeavyBall(d);
        break;
    case 2:
        return Nesterov(d);
        break;
    case 3:
        return Adam(d);

    default:
        std::cerr << "\nThe method you've chosen is incorrect or not implemented. Please check the syntax in the data.json file.\n\n"
                  << std::endl;
        return std::vector<double>(d.x0.size(), std::numeric_limits<double>::quiet_NaN());
        break;
    }
}

#endif /* METHODS_FB161DFE_D7BD_463C_8C11_150E82C21337 */
