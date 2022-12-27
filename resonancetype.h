/*-------------------resonancetype.h-------------------*/

#ifndef RESONANCETYPE_H
#define RESONANCETYPE_H

#include "particletype.h"

class ResonanceType : public ParticleType {
 public:
  double GetWidth() const;
  void Print() const;
  ResonanceType(std::string const& name, double mass, int charge, double width);

 private:
  double const fWidth;
};

#endif