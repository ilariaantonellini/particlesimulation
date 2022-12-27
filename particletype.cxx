/*-------------------particletype.cxx-------------------*/

#include "particletype.h"

#include <iostream>

std::string ParticleType::GetName() const { return fName; }
double ParticleType::GetMass() const { return fMass; }
int ParticleType::GetCharge() const { return fCharge; }
double ParticleType::GetWidth() const { return 0.; }
void ParticleType::Print() const {
  std::cout << "Particle name: " << fName << '\n';
  std::cout << "Particle mass: " << fMass << '\n';
  std::cout << "Particle charge: " << fCharge << '\n';
}
ParticleType::ParticleType(std::string const& name, double mass, int charge)
    : fName{name}, fMass{mass}, fCharge{charge} {}
