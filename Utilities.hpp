#ifndef UTILITIES_D4AD27A1_BEE5_4F83_8B96_64BE80078259
#define UTILITIES_D4AD27A1_BEE5_4F83_8B96_64BE80078259

#include <numeric>
#include <vector>
#include <cmath>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <algorithm>
#include <sstream>

/*

We define some auxiliary functions that will come in handy:
- l2 norm of a vector
- Finite Differences Gradient
- Overload of + - *(with a scalar) for vectors
- Armijo rule

*/

//===============================

double norm2(const std::vector<double> &v)
{
    return sqrt(std::inner_product(v.cbegin(), v.cend(), v.cbegin(), 0.0));
}

//===============================

std::vector<double> gradientFD(const std::function<double(const std::vector<double> &)> &f, const std::vector<double> &x, double h)
{
    std::vector<double> gradient(x.size(), 0.0); // Initialize gradient vector with zeros

    for (size_t i = 0; i < x.size(); ++i)
    {
        //@note the only way to avoid having to create these temporary vectors
        //is to use a view. Yet, it is a bit complicated for just an exercise!
        std::vector<double> x_plus_h = x; // Copy x to avoid modifying the original vector
        std::vector<double> x_minus_h = x;

        x_plus_h[i] += h;  // Increment the i-th element by h
        x_minus_h[i] -= h; // Decrement the i-th element by h

        // Compute the finite difference approximation for the i-th component of the gradient
        gradient[i] = (f(x_plus_h) - f(x_minus_h)) / (2.0 * h);
    }

    return gradient;
}

//===============================

std::vector<double> operator+(const std::vector<double> &v1, const std::vector<double> &v2)
{
    if (v1.size() != v2.size())
    {
        throw std::runtime_error("Vectors must be the same size to add them.");
    }

    std::vector<double> result(v1.size());

    for (size_t i = 0; i < v1.size(); ++i)
    {
        result[i] = v1[i] + v2[i];
    }
    return result;
}

std::vector<double> operator-(const std::vector<double> &v1, const std::vector<double> &v2)
{
    if (v1.size() != v2.size())
    {
        throw std::runtime_error("Vectors must be the same size to subtract them.");
    }

    std::vector<double> result(v1.size());

    for (size_t i = 0; i < v1.size(); ++i)
    {
        result[i] = v1[i] - v2[i];
    }
    return result;
}

std::vector<double> operator*(double scalar, const std::vector<double> &v)
{
    std::vector<double> result(v.size());

    // std::transform(v.begin(), v.end(), result.begin(), [scalar](double value) { return value * scalar; });

    for (size_t i = 0; i < v.size(); ++i)
    {
        result[i] = scalar * v[i];
    }

    return result;
}

//===============================

double armijo(const std::vector<double> &xk, const std::function<double(const std::vector<double> &)> &f,
              const std::function<std::vector<double>(const std::vector<double> &)> &df, double sigma, double a0)
{
    double ak = a0;

    //@note I would add a maximum number of iterations. In principle
    // Armijo sould converge provided f is continuous and df is Lipschitz
    // continuous. Yet, in practice, it may not converge due to numerical
    // errors. In that case, we should stop the loop and return the best
    // value found so far.
    while (f(xk) - f(xk - a0 * df(xk)) >= sigma * a0 * pow(norm2(df(xk)), 2))
    {
        ak /= 2.;
    }

    return ak;
}

//===============================

std::string printVector(const std::vector<double> &vec)
{
    std::stringstream ss;
    //@note Nice the idea of using stringstreams!
    ss << "(";
    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (i != 0)
        {
            ss << ", ";
        }
        ss << vec[i];
    }
    ss << ")";
    return ss.str();
}

//===============================

#endif /* UTILITIES_D4AD27A1_BEE5_4F83_8B96_64BE80078259 */
