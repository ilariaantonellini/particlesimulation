#include <cmath>
#include <vector>

#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMath.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TStyle.h"
#include "particle.h"
#include "particletype.h"
#include "resonancetype.h"

void setStyle() {
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(1122);
  gStyle->SetOptFit(111);
  gStyle->SetPalette(57);
  gStyle->SetOptTitle(0);
}

void particleMain() {
  // to avoid reload manually
  R__LOAD_LIBRARY(particletype_cxx.so);
  R__LOAD_LIBRARY(resonancetype_cxx.so);
  R__LOAD_LIBRARY(particle_cxx.so);

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
  TH1F* h8 = new TH1F("h8", "Invariant mass with opposite charge", 80, 0, 2);
  TH1F* h9 = new TH1F("h9", "Invariant mass with same charge", 80, 0, 2);
  TH1F* h10 = new TH1F(
      "h10", "Invariant mass between Pion+/Kaon- and Pion-/Kaon+", 80, 0, 2);
  TH1F* h11 = new TH1F(
      "h11", "Invariant mass between Pion+/Kaon+ and Pion-/Kaon-", 80, 0, 2);
  TH1F* h12 =
      new TH1F("h12", "Invariant mass between decay products", 80, 0, 2);
  h7->Sumw2();
  h8->Sumw2();
  h9->Sumw2();
  h10->Sumw2();
  h11->Sumw2();
  h12->Sumw2();

  // std::vector<TH1F*> histograms;
  // histograms.push_back(h1);
  // histograms.push_back(h2);
  // histograms.push_back(h3);
  // histograms.push_back(h4);
  // histograms.push_back(h5);
  // histograms.push_back(h6);
  // histograms.push_back(h7);
  // histograms.push_back(h8);
  // histograms.push_back(h9);
  // histograms.push_back(h10);
  // histograms.push_back(h11);
  // histograms.push_back(h12);

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

    EventParticles.clear();
  }

  // TCanvas* c = new TCanvas("c", "Histograms", 200, 10, 600, 400);
  // c->Divide(4, 3);

  // for (int i = 0; i < 12; ++i) {
  //   c->cd(i + 1);
  //   histograms[i]->DrawCopy("H");
  //   histograms[i]->DrawCopy("E, SAME");
  // }

  file->Write();
  file->Close();
}

int main() { particleMain(); }