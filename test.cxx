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

  for (int i = 0; i != 2; ++i) {
    (v[i])->Print();
  }

  //Particle p("electron");
  Particle::AddParticleType("proton", 1.67, 5, 5);  // fino a qui ci siamo
  Particle::AddParticleType("electron", 1.67, 5); 
  Particle p("electron");
  p.SetIndex("electron"); // fino a qui ci siamo
  p.PrintIndex();
  p.PrintParticle();

  /* Particle p1("proton", 0.1, 0.2, 0.3);
  p.PrintIndex(); */
  // p.SetIndex("proton");
  // std::cout << p.GetMass() <<'\n'; */
}

//TAGLI:

 /* if (particle.GetIndex() == 6) {
        Particle p1;
        Particle p2;
        double const y = gRandom->Rndm();

        if (y < 0.5) {
          p1.SetIndex("Pion+");
          p2.SetIndex("Kaon-");g+++
          particle.Decay2body(p1, p2);
        } else {
          p1.SetIndex("Pion-");
          p2.SetIndex("Kaon+");
          particle.Decay2body(p1, p2);
        }

        EventParticle.push_back(p1);
        EventParticle.push_back(p2);
      } */
