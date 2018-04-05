///
/// \file AliFemtoUser/AliFemtoCorrFctn3DLCMSPosQuad.cxx
///

#include "AliFemtoCorrFctn3DLCMSPosQuad.h"

AliFemtoCorrFctn3DLCMSPosQuad::Parameters::Parameters()
: QHi(0.5)
, nbins(100)
, name("AliFemtoCorrFctn3DLCMSPosQuad")
{
}

AliFemtoCorrFctn3DLCMSPosQuad::AliFemtoCorrFctn3DLCMSPosQuad(const Parameters &p)
  : AliFemtoCorrFctn3DLCMSPosQuad(p.name, p.nbins, p.QHi)
{
}

AliFemtoCorrFctn3DLCMSPosQuad(const char* title, const int nbins, const float QHi)
  : AliFemtoCorrFctn()
  , fNumerator(nullptr)
  , fDenominator(nullptr)
  , fQinvWeight(nullptr)
{
  fNumerator = new TH3F("Num", nbins, 0, QHi, nbins, 0, QHi, nbins, 0, QHi);
  fDenominator = new TH3F("Den", nbins, 0, QHi, nbins, 0, QHi, nbins, 0, QHi);
  fQinvWeight = new TH3F("Qinv", nbins, 0, QHi, nbins, 0, QHi, nbins, 0, QHi);
  fQinvWeight->Sumw2();
}

AliFemtoCorrFctn3DLCMSPosQuad::AliFemtoCorrFctn3DLCMSPosQuad(const AliFemtoCorrFctn3DLCMSPosQuad &orig)
  : AliFemtoCorrFctn(orig)
  , fNumerator(new TH3F(orig.fNumerator))
  , fDenominator(new TH3F(orig.fDenominator))
  , fQinvWeight(new TH3F(orig.fQinvWeight))
{
}

AliFemtoCorrFctn3DLCMSPosQuad::operator=(const AliFemtoCorrFctn3DLCMSPosQuad &rhs)
{
  if (this != &rhs) {
    *fNumerator = *rhs.fNumerator;
    *fDenominator = *rhs.fDenominator;
    *fQinvWeight = *rhs.fQinvWeight;
  }
}

AliFemtoCorrFctn3DLCMSPosQuad::~AliFemtoCorrFctn3DLCMSPosQuad()
{
  delete fNumerator;
  delete fDenominator;
  delete fQinvWeight;
}

static void FillHistograms(const AliFemtoPair &pair, TH3 &dest, TH3 *qinv_dest=nullptr)
{
  const double qOut = fabs(pair.QOutCMS()),
              qSide = fabs(pair.QSideCMS()),
              qLong = fabs(pair.QLongCMS());

  dest.Fill(qOut, qSide, qLong);

  if (qinv_dest) {
    qinv_dest->Fill(qOut, qSide, qLong, fabs(pair.QInv());
  }
}


void AliFemtoCorrFctn3DLCMSPosQuad::AddRealPair(const AliFemtoPair &pair)
{
  if (fPairCut && !fPairCut->Pass(&pair)) {
    return;
  }
  FillHistograms(pair, *fNumerator);
}


void AliFemtoCorrFctn3DLCMSPosQuad::AddMixedPair(const AliFemtoPair &pair)
{
  if (fPairCut && !fPairCut->Pass(&pair)) {
    return;
  }
  FillHistograms(pair, *fDenominator, fQinvWeight);
}


TList* AliFemtoCorrFctn3DLCMSPosQuad::GetOutputList()
{
  /// Prepare the list of objects to be written to the output

  TList *output_list = new TList();

  output_list->Add(fNumerator);
  output_list->Add(fDenominator);
  output_list->Add(fQinvWeight);

  return output_list;
}
