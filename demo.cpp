import siplusplus;

#include <fmt/core.h>

#include <iostream>

#include "si++/format.h"
using namespace SI;

int main(void) {
    DimensionfulFloat<double, BaseUnits::m> wavelength(532e-9);
    DimensionfulFloat<double, BaseUnits::m / BaseUnits::s> c{29979246};
    DimensionfulFloat<double, DerivedUnits::J * BaseUnits::s> hbar(1.0545718e-34
    );

    auto photon_energy = hbar * c / wavelength;

    auto p = photon_energy / 6.28;

    fmt::print("Wavelength: {:.3}\n", wavelength);
    fmt::print("Wavenumber: {:.3}\n", 1.0 / wavelength);
    fmt::print("The speed of light is {}\n", c);
    fmt::print("Corresponding frequency: {:.3}\n", c / wavelength);
    fmt::print("Energy of one photon: {}\n", hbar * c / wavelength);

    auto mass = DimensionfulFloat<double, SI::BaseUnits::kg>{1};
    std::cout << std::endl;

#ifdef ERRORS
    fmt::print(
        "Quantities of different dimensions cannot be added", wavelength + c
    );
    fmt::print(
        "Quantities of different dimensions cannot be compared: {}",
        wavelength <=> c
    );
#endif

    return 0;
}
