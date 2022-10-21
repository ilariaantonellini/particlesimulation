#include "resonancetype.h"

#include <iostream>

double ResonanceType::GetWidth() const { return fWidth; }
void ResonanceType::Print() const {
  ParticleType::Print();
  std::cout << "Particle width: " << fWidth << '\n';
}
ResonanceType::ResonanceType(std::string const name, double mass, int charge, double width)
    : ParticleType(name, mass, charge), fWidth{width} {}

