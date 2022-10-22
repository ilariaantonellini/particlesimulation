#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "particle.h"
#include "particletype.h"
#include "resonancetype.h"

TEST_CASE("Testing particle type") {
  ParticleType k("proton", 1.67, 1);
  CHECK((k.GetName()) == "proton");
  CHECK((k.GetMass()) == 1.67);
  CHECK((k.GetCharge()) == 1);
  ParticleType k1("electron", 9.8, -1);
  CHECK((k1.GetName()) == "electron");
  CHECK((k1.GetMass()) == 9.8);
  CHECK((k1.GetCharge()) == -1);
}

TEST_CASE("Testing resonance type") {
  ResonanceType r("kaon", 555., 2, 12.);
  CHECK((r.GetName()) == "kaon");
  CHECK((r.GetMass()) == 555.);
  CHECK((r.GetCharge()) == 2);
  CHECK((r.GetWidth()) == 12.0);
}

TEST_CASE("Testing vector"){
  ParticleType k{"Kaon", 50., 3};
  ResonanceType kstar{"Kstar", 5.1, 6, 18.33};

  k.Print();
  std::cout << '\n';
  kstar.Print();
  std::cout << '\n';
  std::vector<ParticleType*> v{&k, &kstar};

  for (int i = 0; i != 2; ++i) {
    (v[i])->Print();
    std::cout << '\n';
  }
}

TEST_CASE("Testing particle") {
  Particle p("p");
  p.AddParticleType("e", 9.8, 5);
  p.AddParticleType("p", 1.6, 2, 3.0);
  p.SetIndex("e");
  CHECK((p.GetIndex()) == 0);
  p.SetIndex(1);
  CHECK((p.GetIndex()) == 1);
  p.SetP(0.1, 0.2, 0.3);
  CHECK((p.GetPx() == 0.1));
  CHECK((p.GetPy() == 0.2));
  CHECK((p.GetPz() == 0.3));
  CHECK((p.Energy()) == doctest::Approx(1.64).epsilon(0.01));
  Particle p1("e");
  p1.SetIndex("e");
  CHECK((p1.GetIndex()) == 0);
  p1.SetP(1., 2., 3.);
  CHECK((p1.GetMass()) == 9.8);
  CHECK((p1.GetPx() == 1.));
  CHECK((p1.GetPy() == 2.));
  CHECK((p1.GetPz() == 3.));
  CHECK((p1.Energy()) == doctest::Approx(10.48).epsilon(0.01));
  CHECK((p.InvMass(p1) == doctest::Approx(11.39).epsilon(0.01)));
}