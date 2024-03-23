#ifndef MUVFUN_DC89F428_38F4_4D7A_AF52_ED84E2E1510F
#define MUVFUN_DC89F428_38F4_4D7A_AF52_ED84E2E1510F

#include "mu_Sfun.hpp"

// Class that implements vector valued function through scalar muparser-interfaced function

class mu_Vfun
{
private:
    unsigned m_dimension;
    std::vector<mu_Sfun> m_gradients;

public:
    mu_Vfun(const std::vector<std::string> &gradient_expr, int dimension) : m_dimension(dimension)
    {
        for (const auto &expr : gradient_expr)
        {
            m_gradients.emplace_back(expr, dimension);
        }
    }

    std::vector<double> operator()(const std::vector<double> &x)
    {
        if (x.size() != m_dimension)
        {
            std::cerr << "Input dimension mismatch" << std::endl;
            return std::vector<double>(m_dimension, std::numeric_limits<double>::quiet_NaN());
        }

        std::vector<double> gradient(m_dimension);

        for (size_t i = 0; i < m_dimension; ++i)
        {
            gradient[i] = m_gradients[i](x);
        }

        return gradient;
    }
};

#endif /* MUXGRAD_DC89F428_38F4_4D7A_AF52_ED84E2E1510F */
