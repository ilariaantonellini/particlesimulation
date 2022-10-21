#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>
#include <vector>

#include "particletype.h"
#include "resonancetype.h"

class Particle {
 public:
  Particle(std::string const name, double px, double py, double pz);
  Particle();

  int GetIndex() const;
  void SetIndex(int index);
  void SetIndex(std::string const name);

  static void AddParticleType(std::string const name, double mass, int charge,
                              double width);

  void PrintParticle() const;
  void PrintIndex() const;

  double GetPx() const;
  double GetPy() const;
  double GetPz() const;

  double GetMass() const;

  double Energy() const;

  double InvMass(Particle& p) const;

  void SetP(double px, double py, double pz);

 private:
  static int const fMaxNumParticleType;  // non obbligatorio usando vector
  static std::vector<ParticleType*> fParticleType;
  static int fNParticleType;  // da controllare e capire
  int fIndex;
  double fPx;
  double fPy;
  double fPz;

  static int FindParticle(
      std::string const nameParticle);  // controllare lo static
};
#endif