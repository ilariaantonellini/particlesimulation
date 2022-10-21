#include <iostream>
#include <vector>

#include "particle.h"
#include "particletype.h"
#include "resonancetype.h"

int main() {
  ParticleType k{"Kaon", 50., 3};
  ResonanceType kstar{"Kstar", 5.1, 6, 18.33};

  k.Print();
  kstar.Print();
  auto i = k.GetCharge();
  std::cout << i << '\n';
  std::vector<ParticleType*> v{&k, &kstar};

  for (int i = 0; i < 2; ++i) {
    (v[i])->Print();
  }

  Particle p{"proton"};

  p.AddParticleType("proton", 1.67, 1);
  p.SetIndex("proton");
  //std::cout << p.GetMass() <<'\n'; */
}