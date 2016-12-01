/// \file TrackP
/// \brief Base track model for the Barrel, params only, w/o covariance
/// \author ruben.shahoyan@cern.ch

#ifndef ALICEO2_BASE_TRACK
#define ALICEO2_BASE_TRACK

#include <stdio.h>
#include <string.h>
#include "DetectorsBase/Constants.h"
#include "DetectorsBase/Utils.h"

namespace AliceO2 {
  namespace Base {
    namespace Track {

      using namespace AliceO2::Base::Constants;
      using namespace AliceO2::Base::Utils;

      // aliases for track elements
      enum {kX,kAlpha,
        kY,kZ,kSnp,kTgl,kQ2Pt,
        kSigY2,
        kSigZY,kSigZ2,
        kSigSnpY,kSigSnpZ,kSigSnp2,
        kSigTglY,kSigTglZ,kSigTglSnp,kSigTgl2,
        kSigQ2PtY,kSigQ2PtZ,kSigQ2PtSnp,kSigQ2PtTgl,kSigQ2Pt2};
      enum {kNParams=5,kCovMatSize=15,kTrackPSize=kNParams+2,kTrackPCSize=kTrackPSize+kCovMatSize
        ,kLabCovMatSize=21};

      constexpr float 
        kCY2max=100*100, // SigmaY<=100cm
        kCZ2max=100*100, // SigmaZ<=100cm
        kCSnp2max=1*1,     // SigmaSin<=1
        kCTgl2max=1*1,     // SigmaTan<=1
        kC1Pt2max=100*100; // Sigma1/Pt<=100 1/GeV


      class TrackPar { // track parameterization, kinematics only
        public:
          TrackPar() {memset(mP,0,kTrackPSize*sizeof(float));}
          TrackPar(float x,float alpha, const float par[kNParams]);
          TrackPar(const float xyz[3],const float pxpypz[3],int sign, bool sectorAlpha=true);

          float& operator[](int i)                   { return mP[i]; }
          float  operator[](int i)             const { return mP[i]; }
          operator float*()                    const { return (float*)mP; }

          const float* GetParam()              const { return &mP[kY]; }
          float GetX()                         const { return mP[kX]; }
          float GetAlpha()                     const { return mP[kAlpha]; }
          float GetY()                         const { return mP[kY]; }
          float GetZ()                         const { return mP[kZ]; }
          float GetSnp()                       const { return mP[kSnp]; }
          float GetTgl()                       const { return mP[kTgl]; }
          float GetQ2Pt()                      const { return mP[kQ2Pt]; }

          // derived getters
          float GetCurvature(float b)          const { return mP[kQ2Pt]*b*kB2C;}
          float GetSign()                      const { return mP[kQ2Pt]>0 ? 1.f:-1.f;}
          float GetPhi()                       const { return asinf(GetSnp()) + GetAlpha();}
          float GetPhiPos()                    const;

          float GetP()                         const;
          float GetPt()                        const;
          void  GetXYZ(float xyz[3])           const;
          bool  GetPxPyPz(float pxyz[3])       const;
          bool  GetPosDir(float posdirp[9])    const;

          // parameters manipulation
          bool  RotateParam(float alpha);
          bool  PropagateParamTo(float xk, float b);
          bool  PropagateParamTo(float xk, const float b[3]);
          void  InvertParam();

          void  Print();

          static void g3helx3(float qfield, float step,float vect[7]);

        protected:
          float mP[kTrackPSize];  // x,alpha + 5 parameters
      };

      class TrackParCov { // track+error parameterization
        public:
          TrackParCov() { memset(mPC,0,kTrackPCSize*sizeof(float)); }
          TrackParCov(float x,float alpha, const float par[kNParams], const float cov[kCovMatSize]);
          TrackParCov(const float xyz[3],const float pxpypz[3],const float[kLabCovMatSize],
              int sign, bool sectorAlpha=true);

          operator TrackPar*() { return reinterpret_cast<TrackPar*>(this); }
          operator TrackPar()  { return *reinterpret_cast<TrackPar*>(this); }
          operator TrackPar&() { return *reinterpret_cast<TrackPar*>(this); }

          float& operator[](int i)                   { return mPC[i]; }
          float  operator[](int i)             const { return mPC[i]; }
          operator float*()                    const { return (float*)mPC; }
          const float* GetParam()              const { return &mPC[kY]; }
          const float* GetCov()                const { return &mPC[kSigY2]; }

          float GetX()                         const { return mPC[kX]; }
          float GetAlpha()                     const { return mPC[kAlpha]; }
          float GetY()                         const { return mPC[kY]; }
          float GetZ()                         const { return mPC[kZ]; }
          float GetSnp()                       const { return mPC[kSnp]; }
          float GetTgl()                       const { return mPC[kTgl]; }
          float GetQ2Pt()                      const { return mPC[kQ2Pt]; }


          float GetSigmaY2()                   const { return mPC[kSigY2]; }
          float GetSigmaZY()                   const { return mPC[kSigZY]; }
          float GetSigmaZ2()                   const { return mPC[kSigZ2]; }
          float GetSigmaSnpY()                 const { return mPC[kSigSnpY]; }
          float GetSigmaSnpZ()                 const { return mPC[kSigSnpZ]; }
          float GetSigmaSnp2()                 const { return mPC[kSigSnp2]; }
          float GetSigmaTglY()                 const { return mPC[kSigTglY]; }
          float GetSigmaTglZ()                 const { return mPC[kSigTglZ]; }
          float GetSigmaTglSnp()               const { return mPC[kSigTglSnp]; }
          float GetSigmaTgl2()                 const { return mPC[kSigTgl2]; }
          float GetSigma1PtY()                 const { return mPC[kSigQ2PtY]; }
          float GetSigma1PtZ()                 const { return mPC[kSigQ2PtZ]; }
          float GetSigma1PtSnp()               const { return mPC[kSigQ2PtSnp]; }
          float GetSigma1PtTgl()               const { return mPC[kSigQ2PtTgl]; }
          float GetSigma1Pt2()                 const { return mPC[kSigQ2Pt2]; }

          // derived getters
          float GetCurvature(float b)          const { return mPC[kQ2Pt]*b*kB2C;}
          float GetSign()                      const { return mPC[kQ2Pt]>0 ? 1.f:-1.f;}
          float GetP()                         const { return Param()->GetP(); }
          float GetPt()                        const { return Param()->GetPt(); }
          float GetPhi()                       const { return Param()->GetPhi(); }
          float GetPhiPos()                    const { return Param()->GetPhiPos(); }
          void  GetXYZ(float xyz[3])           const { Param()->GetXYZ(xyz); }
          bool  GetPxPyPz(float pxyz[3])       const { return Param()->GetPxPyPz(pxyz); }
          bool  GetPosDir(float posdirp[9])    const { return Param()->GetPosDir(posdirp); }

          // parameters manipulation
          bool  RotateParam(float alpha)             { return Param()->RotateParam(alpha); }
          bool  PropagateParamTo(float xk, float b)  { return Param()->PropagateParamTo(xk,b); }
          bool  PropagateParamTo(float xk, const float b[3]) {return Param()->PropagateParamTo(xk,b); }
          void  InvertParam()                        { Param()->InvertParam(); }

          bool  Rotate(float alpha);
          bool  PropagateTo(float xk, float b);
          bool  PropagateTo(float xk, const float b[3]);
          void  Invert();

          float GetPredictedChi2(const float p[2], const float cov[3]) const;
          bool  Update(const float p[2], const float cov[3]);

          bool  CorrectForMaterial(float x2x0,float xrho,float mass,bool anglecorr=false,float dedx=kCalcdEdxAuto);

          void  ResetCovariance(float s2=0);
          void  CheckCovariance();
          void  Print();

        protected:
          // internal cast to TrackPar
          const TrackPar* Param()              const { return reinterpret_cast<const TrackPar*>(this); }
          TrackPar* Param()                          { return reinterpret_cast<TrackPar*>(this); }
          bool TrackPar2Momentum(float p[3], float alpha);

        protected:
          float mPC[kTrackPCSize];  // x, alpha + 5 parameters + 15 errors

          static const float kCalcdEdxAuto; // value indicating request for dedx calculation
      };


      //____________________________________________________________
      inline TrackPar::TrackPar(float x, float alpha, const float par[kNParams]) {
        // explicit constructor
        mP[kX] = x;
        mP[kAlpha] = alpha;
        memcpy(&mP[kY],par,kNParams*sizeof(float));
      }

      //_______________________________________________________
      inline void TrackPar::GetXYZ(float xyz[3]) const {
        // track coordinates in lab frame
        xyz[0] = GetX(); 
        xyz[1] = GetY();
        xyz[2] = GetZ();
        RotateZ(xyz,GetAlpha());
      }

      //_______________________________________________________
      inline float TrackPar::GetPhiPos() const {
        // angle of track position
        float xy[2]={GetX(),GetY()};
        return atan2(xy[1],xy[0]);
      }

      //____________________________________________________________
      inline float TrackPar::GetP() const {
        // return the track momentum
        float ptI = fabs(mP[kQ2Pt]);
        return (ptI>kAlmost0) ? sqrtf(1.f+ mP[kTgl]*mP[kTgl])/ptI : kVeryBig;
      }

      //____________________________________________________________
      inline float TrackPar::GetPt() const {
        // return the track transverse momentum
        float ptI = fabs(mP[kQ2Pt]);
        return (ptI>kAlmost0) ? 1.f/ptI : kVeryBig;
      }

      //============================================================

      //____________________________________________________________
      inline TrackParCov::TrackParCov(float x, float alpha, const float par[kNParams], const float cov[kCovMatSize]) {
        // explicit constructor
        mPC[kX] = x;
        mPC[kAlpha] = alpha;
        memcpy(&mPC[kY],par,kNParams*sizeof(float));
        memcpy(&mPC[kSigY2],cov,kCovMatSize*sizeof(float));
      }
    }
  }
}


#endif