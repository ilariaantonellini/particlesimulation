#include "particle.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "particletype.h"
#include "resonancetype.h"

std::vector<ParticleType*> Particle::fParticleType{};

int Particle::FindParticle(std::string const& nameParticle) {
  for (int i = 0; i != static_cast<int>(fParticleType.size()); ++i) {
    if (nameParticle == fParticleType[i]->GetName()) {
      return i;
    }
  }
  return -1;
}

Particle::Particle() { fIndex = -1; };  // controllare se è corretto

Particle::Particle(std::string const& name, double px, double py, double pz)
    : fPx{px}, fPy{py}, fPz{pz} {
  if (FindParticle(name) == -1) {
    std::cout << "No correspondence found." << '\n';
  } else {
    fIndex = FindParticle(name);
  }
}

int Particle::GetIndex() const { return fIndex; }

void Particle::AddParticleType(
    std::string const& name, double mass,
    int charge,  // non salva le cose in fParticleType
    double width) {
  // ResonanceType particle(name, mass, charge, width);
  // molto riscioso
  if (FindParticle(name) == -1) {
    if (width != 0) {
      ResonanceType* particle = new ResonanceType(name, mass, charge, width);
      fParticleType.push_back(particle);
    } else {
      ParticleType* particle = new ParticleType(name, mass, charge);
      fParticleType.push_back(particle);
    }
  }
}

void Particle::SetIndex(std::string const& name) {  // è sbagliato forse
  if (FindParticle(name) != -1 &&
      FindParticle(name) < static_cast<int>(fParticleType.size())) {
    fIndex = FindParticle(name);
  } else {
    std::cout << "Particle not found." << '\n';
  }
}

void Particle::SetIndex(int index) {
  if (index != -1 && index < static_cast<int>(fParticleType.size())) {
    fIndex = index;
  }
}

void Particle::PrintParticle() {
  // std::cout << fParticleType.size() << '\n';
  std::cout << '\n';
  for (int i = 0; i != static_cast<int>(fParticleType.size()); ++i) {
    fParticleType[i]->Print();
    std::cout << '\n';
  }
  std::cout << '\n';
}

void Particle::PrintIndex() const {
  std::cout << '\n';
  std::cout << "Index: " << fIndex << '\n';
  std::cout << "Particle name: " << fParticleType[fIndex]->GetName() << '\n';
  std::cout << "Px: " << fPx << '\n';
  std::cout << "Py: " << fPy << '\n';
  std::cout << "Pz: " << fPz << '\n';
  std::cout << '\n';
}

double Particle::GetPx() const { return fPx; }
double Particle::GetPy() const { return fPy; }
double Particle::GetPz() const { return fPz; }

int Particle::GetCharge() const { return fParticleType[fIndex]->GetCharge(); }

double Particle::GetMass() const { return fParticleType[fIndex]->GetMass(); }

double Particle::Energy() const {
  double energy =
      std::sqrt((GetMass() * GetMass()) + (fPx * fPx + fPy * fPy + fPz * fPz));
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

int Particle::Decay2body(Particle& dau1, Particle& dau2) const {
  if (GetMass() == 0.0) {
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }

  double massMot = GetMass();
  double massDau1 = dau1.GetMass();
  double massDau2 = dau2.GetMass();

  if (fIndex > -1) {  // add width effect

    // gaussian random numbers

    float x1, x2, w, y1, y2;

    double invnum = 1. / RAND_MAX;
    do {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;
    y2 = x2 * w;

    massMot += fParticleType[fIndex]->GetWidth() * y1;  // come lo risolvo?
  }

  if (massMot < massDau1 + massDau2) {
    printf(
        "Decayment cannot be preformed because mass is too low in this "
        "channel\n");
    return 2;
  }

  double pout =
      sqrt(
          (massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2)) *
          (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2))) /
      massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.SetP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi),
            pout * cos(theta));
  dau2.SetP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi),
            -pout * cos(theta));

  double energy = sqrt(fPx * fPx + fPy * fPy + fPz * fPz + massMot * massMot);

  double bx = fPx / energy;
  double by = fPy / energy;
  double bz = fPz / energy;

  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);

  return 0;
}

void Particle::Boost(double bx, double by, double bz) {
  double energy = Energy();

  // Boost this Lorentz vector
  double b2 = bx * bx + by * by + bz * bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx * fPx + by * fPy + bz * fPz;
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  fPx += gamma2 * bp * bx + gamma * bx * energy;
  fPy += gamma2 * bp * by + gamma * by * energy;
  fPz += gamma2 * bp * bz + gamma * bz * energy;
}