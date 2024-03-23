#include "Data.hpp"
#include "Utilities.hpp"
#include "Minimization.hpp"

using json = nlohmann::json;

int main()
{
    Data data("data.json");
    std::vector<double> argmin(data.x0.size());

    argmin = minimize(data);

    std::cout << "\nThe minimum value of the function is: " << data.Fun(argmin) << "\n\n"
              << std::endl;

    return 0;
}