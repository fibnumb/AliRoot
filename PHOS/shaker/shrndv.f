*CMZ :          17/07/98  15.49.05  by  Federico Carminati
*-- Author :
      SUBROUTINE SHRNDV
c	=================

c	Prepare random generation vectors

*KEEP,SHPHYP.
      COMMON /SHPHYP/ JWEI,NDNDY,YLIM,PTLIM,JWEAK,JPI0,JETA,JPIC,JPRO,
     +                  JKAC,JKA0,JRHO,JOME,JPHI,JPSI,JDRY
*KEEP,SHGENE.
      COMMON /SHGENE/ IEVT,NPI0,NETA,NPIC,NPRO,NKAC,NKA0,NRHO,NOME,
     +                  NPHI,NPSI,NDRY
*KEEP,SHRAND.
      COMMON /SHRAND/ PISP1(100),PISP2(100),ETASP1(100),ETASP2(100),
     +                  PROSP(100),KAOSP(100)
*KEEP,SHNORM.
      COMMON /SHNORM/ PINOR,PIRAT,ETANOR,ETARAT,RHONOR,OMENOR,PHINOR,
     +                  PSINOR,DRYNOR
*KEEP,FUNINT.
      COMMON /FUNINT/ FINT
*KEND.

      EXTERNAL SHFPI,SHFETA,SHFPRO,SHFKAO,SHFRHO,SHFOME,SHFPHI
      EXTERNAL SHFPSI,SHFDRY

      IF (JPI0.EQ.1.OR.JPIC.EQ.1) THEN
        CALL FUNPRE(SHFPI,PISP1,0.,2.)
        PINO1 = FINT
        CALL FUNPRE(SHFPI,PISP2,2.,PTLIM)
        PINO2 = FINT
        PINOR=PINO1+PINO2
        PIRAT=PINO1/PINOR
      ENDIF

      IF (JETA.EQ.1) THEN
        CALL FUNPRE(SHFETA,ETASP1,0.,2.)
        ETANO1 = FINT
        CALL FUNPRE(SHFETA,ETASP2,2.,PTLIM)
        ETANO2 = FINT
        ETANOR=ETANO1+ETANO2
        ETARAT=ETANO1/ETANOR
      ENDIF

      IF (JPRO.EQ.1) THEN
        CALL FUNPRE(SHFPRO,PROSP,0.,PTLIM)
      ENDIF

      IF (JKAC.EQ.1.OR.JKA0.EQ.1) THEN
        CALL FUNPRE(SHFKAO,KAOSP,0.,PTLIM)
      ENDIF


      IF (JRHO.EQ.1) RHONOR = GAUSS(SHFRHO,0.,PTLIM,4.E-6)
      IF (JOME.EQ.1) OMENOR = GAUSS(SHFOME,0.,PTLIM,4.E-6)
      IF (JPHI.EQ.1) PHINOR = GAUSS(SHFPHI,0.,PTLIM,2.E-6)
      IF (JPSI.EQ.1) PSINOR = GAUSS(SHFPSI,0.,PTLIM,2.E-8)
      IF (JDRY.EQ.1) DRYNOR = GAUSS(SHFDRY,0.,PTLIM,1.E-7)

      RETURN
      END
