#ifndef MUSFUN_A1812399_2ECF_4992_A7ED_983C8B03FD54
#define MUSFUN_A1812399_2ECF_4992_A7ED_983C8B03FD54

#include <vector>
#include <string>
#include <muParser.h>
#include <iostream>
#include <limits>

// Class that implements muparser-interfaced fucntions

class mu_Sfun
{

private:
    std::vector<double> m_vars;
    mu::Parser m_parser;

public:
    mu_Sfun(const mu_Sfun &m)
        : m_parser(m.m_parser)
    {
        m_vars.resize(m.m_vars.size());
        for (size_t i = 0; i < m_vars.size(); ++i)
        {
            m_parser.DefineVar("x" + std::to_string(i + 1), &m_vars[i]);
        }
    };

    mu_Sfun(const std::string &s, size_t size)
    {
        m_vars.resize(size);
        try
        {
            for (size_t i = 0; i < size; ++i)
            {
                m_parser.DefineVar("x" + std::to_string(i + 1), &m_vars[i]);
            }
            m_parser.SetExpr(s);
        }
        catch (mu::Parser::exception_type &e)
        {
            std::cerr << e.GetMsg() << std::endl;
        }
    };

    double
    operator()(const std::vector<double> &x)
    {
        std::copy(x.begin(), x.end(), m_vars.begin());
        double y = m_parser.Eval();
        return y;
    };
};

#endif /* MUXFUNCT_A1812399_2ECF_4992_A7ED_983C8B03FD54 */
