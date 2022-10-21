#include "particle.h"

#include <cmath>
#include <iostream>

#include "particletype.h"
#include "resonancetype.h"

int const Particle::fMaxNumParticleType = 10;
int Particle::fNParticleType = 0;
std::vector<ParticleType*> Particle::fParticleType{0};

int Particle::FindParticle(std::string const nameParticle) {
  for (int i = 0; i != static_cast<int>(fParticleType.size()); ++i) {
    if (nameParticle == fParticleType[i]->GetName()) {
      return i;
    }
  }
  return -1;
}

Particle::Particle() = default;  // constollare se è corretto

Particle::Particle(std::string const name, double px = 0, double py = 0,
                   double pz = 0)
    : fPx{px}, fPy{py}, fPz{pz} {
  if (FindParticle(name) == -1) {
    std::cout << "No correspondence found." << '\n';
  } else {
    fIndex = FindParticle(name);
  }
}

int Particle::GetIndex() const { return fIndex; }

void Particle::AddParticleType(std::string const name, double mass, int charge,
                               double width = 0) {
  ResonanceType particle(name, mass, charge, width);
  if (FindParticle(name) == -1 && fNParticleType < fMaxNumParticleType) {
    fParticleType.push_back(&particle);
  }
}

void Particle::SetIndex(std::string const name) {
  if (FindParticle(name) != -1) {
    fIndex = FindParticle(name);
  }
}

void Particle::SetIndex(int index) { fIndex = index; }

void Particle::PrintParticle() const {
  for (int i = 0; i != static_cast<int>(fParticleType.size()); ++i) {
    fParticleType[i]->Print();
  }
}

void Particle::PrintIndex() const {
  for (int i = 0; i != static_cast<int>(fParticleType.size()); ++i) {
    std::cout << "Index: " << i << '\n';
    std::cout << "Particle name: " << fParticleType[i]->GetName() << '\n';
    std::cout << "Px: " << fPx << '\n';
    std::cout << "Py: " << fPy << '\n';
    std::cout << "Pz: " << fPz << '\n';
  }
}

double Particle::GetPx() const { return fPx; }
double Particle::GetPy() const { return fPy; }
double Particle::GetPz() const { return fPz; }

double Particle::GetMass() const {  // svincolare questo dalla massa
  return fParticleType[fIndex]->GetMass();
}

double Particle::Energy() const {
  double energy =
      std::sqrt((GetMass() * GetMass()) - (fPx * fPx + fPy * fPy + fPz * fPz));
  return energy;
}

double Particle::InvMass(Particle& p) const {
  double invMass = std::sqrt((Energy() + p.Energy()) * (Energy() + p.Energy()) -
                             ((fPx + p.GetPx()) * (fPx + p.GetPx()) +
                              (fPy + p.GetPy()) * (fPy + p.GetPy()) +
                              (fPz + p.GetPz()) * (fPz + p.GetPz())));
  return invMass;
}

void Particle::SetP(double px, double py, double pz) {
  fPx = px;
  fPy = py;
  fPz = pz;
}