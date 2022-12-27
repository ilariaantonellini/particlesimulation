/*-------------------particle.h-------------------*/

#ifndef PARTICLE_H
#define PARTICLE_H

#include <string>
#include <vector>

#include "particletype.h"
#include "resonancetype.h"

class Particle {
 public:
  Particle(std::string const& name, double px = 0., double py = 0.,
           double pz = 0.);
  Particle();

  int GetIndex() const;
  void SetIndex(int index);
  void SetIndex(std::string const& name);

  static void AddParticleType(std::string const& name, double mass, int charge,
                              double width = 0.);

  static void PrintParticle();
  void PrintIndex() const;

  double GetPx() const;
  double GetPy() const;
  double GetPz() const;

  int GetCharge() const;

  double GetMass() const;

  double Energy() const;

  double InvMass(Particle& p) const;

  void SetP(double px, double py, double pz);

  int Decay2body(Particle& dau1, Particle& dau2) const;

 private:
  static std::vector<ParticleType*> fParticleType;

  int fIndex;
  double fPx;
  double fPy;
  double fPz;

  static int FindParticle(std::string const& nameParticle);

  void Boost(double bx, double by, double bz);
};
#endif