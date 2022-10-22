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

TEST_CASE("Testing particle") {
  Particle p("p");
  p.AddParticleType("e", 9.8, 5);
  p.AddParticleType("p", 1.6, 2, 3.0);
  p.SetIndex("p");
  CHECK((p.GetIndex()) == 1);
}