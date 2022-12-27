/*-------------------particletype.h-------------------*/

#ifndef PARTICLETYPE_H
#define PARTICLETYPE_H

#include <string> 

class ParticleType {
 public:
  std::string GetName() const;
  double GetMass() const;
  int GetCharge() const;
  virtual double GetWidth() const;

  virtual void Print() const;

  ParticleType(std::string const& name, double mass, int charge);

 private:
  std::string const fName;
  double const fMass;
  int const fCharge;
};

#endif