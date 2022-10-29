#include <cmath>
#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TRandom.h"
#include "particle.h"
#include "particletype.h"
#include "resonancetype.h"

void particleMain() {
  gRandom->SetSeed();

  TFile* file = new TFile("particlegeneration.root", "RECREATE");

  Particle::AddParticleType("Pion+", 0.13957, 1);
  Particle::AddParticleType("Pion-", 0.13957, -1);
  Particle::AddParticleType("Kaon+", 0.49367, 1);
  Particle::AddParticleType("Kaon-", 0.49367, -1);
  Particle::AddParticleType("Proton+", 0.93827, 1);
  Particle::AddParticleType("Proton-", 0.93827, -1);
  Particle::AddParticleType("K*", 0.89166, 0, 0.050);

  Particle::PrintParticle();

  TH1F* h1 = new TH1F("h1", "Types of particle", 7, 0, 7);
  TH1F* h2 =
      new TH1F("h2", "Distribution of azimutal angles", 1000, 0, 2 * M_PI);
  TH1F* h3 = new TH1F("h3", "Distribution of polar angles", 1000, 0, M_PI);
  TH1F* h4 = new TH1F("h4", "Distribution of p", 500, 0, 10);
  TH1F* h5 = new TH1F("h5", "Distribution of transverse p", 500, 0, 10);
  TH1F* h6 = new TH1F("h6", "Particles energy", 500, 0, 10);
  TH1F* h7 = new TH1F("h7", "Invariant mass for all particles", 80, 0, 2);
  // massa invariante range tra 0  e 2 ->binnaggio rapportanto a larghezza del
  // segnale
  // sigma è 0.05 mev
  // bin grandezza orine di quella quantità fare dai 40 o 160 come multiplo di
  // 40 energia stesso numero di bin della p
  TH1F* h8 = new TH1F("h8", "Invariant mass with opposite charge", 80, 0, 2);
  TH1F* h9 = new TH1F("h9", "Invariant mass with same charge", 80, 0, 2);
  TH1F* h10 = new TH1F(
      "h10", "Invariant mass between Pion+/Kaon- and Pion-/Kaon+", 80, 0, 2);
  TH1F* h11 = new TH1F(
      "h11", "Invariant mass between Pion+/Kaon+ and Pion-/Kaon-", 80, 0, 2);
  TH1F* h12 =
      new TH1F("h12", "Invariant mass between decay products", 80, 0, 2);
  h8->Sumw2();
  h9->Sumw2();
  h10->Sumw2();
  h11->Sumw2();
  h12->Sumw2();

  std::vector<Particle> EventParticles;

  for (int i = 0; i < 1e5; ++i) {
    for (int j = 0; j < 100; ++j) {
      double const phi = gRandom->Uniform(0, 2 * M_PI);
      double const theta = gRandom->Uniform(0, M_PI);
      double const p = gRandom->Exp(1);
      double const x = gRandom->Rndm();

      Particle particle;

      particle.SetP(p * TMath::Sin(theta) * TMath::Cos(phi),
                    p * TMath::Sin(theta) * TMath::Sin(phi),
                    p * TMath::Cos(theta));

      if (x < 0.4) {
        particle.SetIndex("Pion+");
      } else if (x < 0.8) {
        particle.SetIndex("Pion-");
      } else if (x < 0.85) {
        particle.SetIndex("Kaon+");
      } else if (x < 0.90) {
        particle.SetIndex("Kaon-");
      } else if (x < 0.945) {
        particle.SetIndex("Proton+");
      } else if (x < 0.990) {
        particle.SetIndex("Proton-");
      } else {
        particle.SetIndex("K*");
      }
      EventParticles.push_back(particle);

      h1->Fill(particle.GetIndex());
      h2->Fill(phi);
      h3->Fill(theta);
      h4->Fill(p);
      h5->Fill(TMath::Sqrt(particle.GetPx() * particle.GetPx() +
                           particle.GetPy() * particle.GetPy()));
      h6->Fill(particle.Energy());
    }

    for (Particle const& p : EventParticles) {
      if ((p.GetIndex() == 6)) {
        Particle p1;
        Particle p2;
        double const y = gRandom->Rndm();

        if (y < 0.5) {
          p1.SetIndex("Pion+");
          p2.SetIndex("Kaon-");
          p.Decay2body(p1, p2);
        } else {
          p1.SetIndex("Pion-");
          p2.SetIndex("Kaon+");
          p.Decay2body(p1, p2);
        }

        EventParticles.push_back(p1);
        EventParticles.push_back(p2);
        h12->Fill(p1.InvMass(p2));
      }
    }

    auto it = EventParticles.begin();
    for (; it != EventParticles.end(); ++it) {
      auto it_2 = std::next(it);
      for (; it_2 != EventParticles.end(); ++it_2) {
        auto invmass = it->InvMass(*it_2);
        h7->Fill(invmass);  // non so se metterlo
        if (it->GetCharge() != 0 && it_2->GetCharge() != 0) {
          if (it->GetCharge() != it_2->GetCharge()) {
            h8->Fill(invmass);
            if ((it->GetIndex() == 0 && it_2->GetIndex() == 3) ||
                (it_2->GetIndex() == 0 && it->GetIndex() == 3)) {
              h10->Fill(invmass);
            } else if ((it->GetIndex() == 1 && it_2->GetIndex() == 2) ||
                       (it_2->GetIndex() == 1 && it->GetIndex() == 2)) {
              h10->Fill(invmass);
            }
          } else {
            h9->Fill(invmass);
            if ((it->GetIndex() == 0 && it_2->GetIndex() == 2) ||
                (it_2->GetIndex() == 0 && it->GetIndex() == 2)) {
              h11->Fill(invmass);
            } else if ((it->GetIndex() == 1 && it_2->GetIndex() == 3) ||
                       (it_2->GetIndex() == 1 && it->GetIndex() == 3)) {
              h11->Fill(invmass);
            }
          }
        }
      }
    }

    /* for(unsigned long k=100; k!= EventParticles.size() &&
    (k+1)!=EventParticles.size(); k+=2){ double inv =
    EventParticles[k].InvMass(EventParticles[k+1]); h12->Fill(inv);
    } */

    EventParticles.clear();
  }

  TCanvas* c = new TCanvas("c", "Histograms", 200, 10, 600, 400);
  c->Divide(4, 3);
  c->cd(1);
  h1->DrawCopy("H");
  c->cd(2);
  h2->DrawCopy("H");
  c->cd(3);
  h3->DrawCopy("H");
  c->cd(4);
  h4->DrawCopy("H");
  c->cd(5);
  h5->DrawCopy("H");
  c->cd(6);
  h6->DrawCopy("H");
  c->cd(7);
  h7->DrawCopy("H");
  c->cd(8);
  h8->DrawCopy("H");
  c->cd(9);
  h9->DrawCopy("H");
  c->cd(10);
  h10->DrawCopy("H");
  c->cd(11);
  h11->DrawCopy("H");
  c->cd(12);
  h12->DrawCopy("H");

  // file->Write();
  h1->Write();
  h2->Write();
  h3->Write();
  h4->Write();
  h5->Write();
  h6->Write();
  h7->Write();
  h8->Write();
  h9->Write();
  h10->Write();
  h11->Write();
  h12->Write();

  file->Close();
}