/*-----------------------------------------------------------------------
 *
 * File Name: MCInjectComputeHough.c
 *
 * Authors: Sintes, A.M., Krishnan, B. 
 *
 * Revision: $Id$
 *  
 * Monte Carlo signal injections for several h_0 values and
 *  compute the Hough transform for only one point in parameter space each time
 *-----------------------------------------------------------------------
 */

/* 
 The idea is that we would like to analize a 300 Hz band on a cluster of
 machines. Each process should analyze 1 Hz band  (or whatever).
 
 	- Read the  band to be analized and the wings needed to read the originals SFTs. 
	-Read the h_0 values to be analyzed in one go
	- Read the file  containing the times and velocities generated by
	DriveHoughColor or compute them 
	-loop over the MC injections:
		+ Generate random parameters (f, f', alpha, delata, i...)
		+ generate h(t), produce its FFT
		+ Add h(f) to SFT for a given h_o value (and all of them)
		+ get number count
		+ wite to file
	(note if one loop fails, should print error , but continue with the next
	value)
	
Input shoud be from
             SFT files 
	     band, wings, nh_0, h_01, h_02....
	     ephemeris info
             (it should also read the times and velocities used in
	     DriveHoughColor)
	     
   This code will output files containing the MC results and info about injected
   signals. 
*/

#include "./MCInjectComputeHough.h" /* proper path*/

#define VALIDATEOUT "./outHM1/skypatch_1/HM1validate"
#define VALIDATEIN  "./outHM1/skypatch_1/HM1templates"

#define TRUE (1==1)
#define FALSE (1==0)



/******************************************************
 *  Assignment of Id string using NRCSID()
 */

NRCSID (MCINJECTVALIDATEC, "$Id$");

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/* vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv------------------------------------ */
int main(int argc, char *argv[]){

  static LALStatus            status;  
  static LALDetector          detector;
  static LIGOTimeGPSVector    timeV;
  static REAL8Cart3CoorVector velV;
  static REAL8Vector          timeDiffV;
  static REAL8Vector          foft;
  static HoughPulsarTemplate  pulsarTemplate;
  SFTVector  *inputSFTs  = NULL;  
  EphemerisData   *edat = NULL;
  CHAR  *uvar_earthEphemeris = NULL; 
  CHAR  *uvar_sunEphemeris = NULL;
  REAL8 *alphaVec=NULL;
  REAL8 *deltaVec=NULL;
  REAL8 *freqVec=NULL;
  REAL8 *spndnVec=NULL;
  static REAL8PeriodoPSD   periPSD;
  /* pgV is vector of peakgrams and pg1 is onepeakgram */
  static UCHARPeakGram     *pg1, **pgV; 
  UINT4  msp; /*number of spin-down parameters */
  INT4   uvar_ifo;
  CHAR   *uvar_SFTdir = NULL; /* the directory where the SFT  could be */
  CHAR   *uvar_fnameOut = NULL;               /* The output prefix filename */
  CHAR   *uvar_fnameIn = NULL;  
  UINT4  numberCount, index;
  UINT8  nTemplates;   
  UINT4   mObsCoh, nfSizeCylinder;
  REAL8  uvar_peakThreshold, normalizeThr;
  REAL8  fmin, fmax, fWings, timeBase;
  INT4  uvar_blocksRngMed;
  REAL8  threshold;
  UINT4  sftlength; 
  INT4   sftFminBin;
  UINT4 loopId, tempLoopId;
  FILE  *fpOut = NULL;
  BOOLEAN uvar_help;
  CHAR *fnameLog=NULL;
  FILE *fpLog = NULL;
  CHAR *logstr=NULL;

  /******************************************************************/
  /*    Set up the default parameters.      */
  /* ****************************************************************/
  
  lalDebugLevel = 0;
  /* LALDebigLevel must be called before anything else */
  SUB( LALGetDebugLevel( &status, argc, argv, 'd'), &status);

  msp = 1; /*only one spin-down */
  nfSizeCylinder = NFSIZE;
 
  detector = lalCachedDetectors[LALDetectorIndexGEO600DIFF]; /* default */
  uvar_ifo = IFO;
  if (uvar_ifo ==1) detector=lalCachedDetectors[LALDetectorIndexGEO600DIFF];
  if (uvar_ifo ==2) detector=lalCachedDetectors[LALDetectorIndexLLODIFF];
  if (uvar_ifo ==3) detector=lalCachedDetectors[LALDetectorIndexLHODIFF];
 
  uvar_help = FALSE;
  uvar_peakThreshold = THRESHOLD;

  uvar_earthEphemeris = (CHAR *)LALMalloc(1024*sizeof(CHAR));
  strcpy(uvar_earthEphemeris,EARTHEPHEMERIS);

  uvar_sunEphemeris = (CHAR *)LALMalloc(1024*sizeof(CHAR));
  strcpy(uvar_sunEphemeris,SUNEPHEMERIS);

  uvar_SFTdir = (CHAR *)LALMalloc(1024*sizeof(CHAR));
  strcpy(uvar_SFTdir,SFTDIRECTORY);

  uvar_fnameOut = (CHAR *)LALMalloc(1024*sizeof(CHAR));
  strcpy(uvar_fnameOut,VALIDATEOUT);

  uvar_fnameIn = (CHAR *)LALMalloc(1024*sizeof(CHAR));
  strcpy(uvar_fnameIn,VALIDATEIN);

  uvar_blocksRngMed = BLOCKSRNGMED;
  SUB( LALRngMedBias( &status, &normalizeThr, uvar_blocksRngMed ), &status ); 

  /* register user input variables */
  SUB( LALRegisterBOOLUserVar( &status, "help", 'h', UVAR_HELP, "Print this message", &uvar_help), &status);  
  SUB( LALRegisterINTUserVar( &status, "ifo", 'i', UVAR_OPTIONAL, "Detector GEO(1) LLO(2) LHO(3)", &uvar_ifo ), &status);
  SUB( LALRegisterSTRINGUserVar( &status, "earthEphemeris", 'E', UVAR_OPTIONAL, "Earth Ephemeris file", &uvar_earthEphemeris), &status);
  SUB( LALRegisterSTRINGUserVar( &status, "sunEphemeris", 'S', UVAR_OPTIONAL, "Sun Ephemeris file", &uvar_sunEphemeris), &status);
  SUB( LALRegisterSTRINGUserVar( &status, "SFTdir", 'D', UVAR_OPTIONAL, "SFT Directory", &uvar_SFTdir), &status);
  SUB( LALRegisterSTRINGUserVar( &status, "fnameIn", 'T', UVAR_OPTIONAL, "Input template file", &uvar_fnameIn), &status);
  SUB( LALRegisterSTRINGUserVar( &status, "fnameOut", 'o', UVAR_OPTIONAL, "Output filename", &uvar_fnameOut), &status);
  SUB( LALRegisterINTUserVar( &status, "blocksRngMed", 'w', UVAR_OPTIONAL, "RngMed block size", &uvar_blocksRngMed), &status);
  SUB( LALRegisterREALUserVar( &status, "peakThreshold", 't', UVAR_OPTIONAL, "Peak selection threshold", &uvar_peakThreshold), &status);

  /* read all command line variables */
  SUB( LALUserVarReadAllInput(&status, argc, argv), &status);

  /* set value of bias which might have been changed from default */  
  SUB( LALRngMedBias( &status, &normalizeThr, uvar_blocksRngMed ), &status ); 
  
  /* exit if help was required */
  if (uvar_help)
    exit(0); 
 
  /* open log file for writing */
  fnameLog = LALCalloc( (strlen(uvar_fnameOut) + strlen(".log") + 10),1);
  strcpy(fnameLog,uvar_fnameOut);
  strcat(fnameLog,".log");
  if ((fpLog = fopen(fnameLog, "w")) == NULL) {
    fprintf(stderr, "Unable to open file %s for writing\n", fnameLog);
    LALFree(fnameLog);
    exit(1);
  }
  
  /* get the log string */
  SUB( LALUserVarGetLog(&status, &logstr, UVAR_LOGFMT_CFGFILE), &status);  

  fprintf( fpLog, "## LOG FILE FOR MCInjectValidate\n\n");
  fprintf( fpLog, "# User Input:\n");
  fprintf( fpLog, "#-------------------------------------------\n");
  fprintf( fpLog, logstr);
  LALFree(logstr);

  /* append an ident-string defining the exact CVS-version of the code used */
  {
    CHAR command[1024] = "";
    fprintf (fpLog, "\n\n# CVS-versions of executable:\n");
    fprintf (fpLog, "# -----------------------------------------\n");
    fclose (fpLog);
    
    sprintf (command, "ident %s | sort -u >> %s", argv[0], fnameLog);
    system (command);	/* we don't check this. If it fails, we assume that */
    			/* one of the system-commands was not available, and */
    			/* therefore the CVS-versions will not be logged */

    LALFree(fnameLog);
  }

  /* open output file for writing */
  fpOut= fopen(uvar_fnameOut, "w");
  setlinebuf(fpOut);  /* line buffered on */  


  /*****************************************************************/
  /* read template file */
  /*****************************************************************/
  {
    FILE  *fpIn = NULL;
    INT4   r;
    REAL8  temp1, temp2, temp3, temp4, temp5;
    UINT8  templateCounter; 
    
    fpIn = fopen(uvar_fnameIn, "r");
    if ( !fpIn )
      {
	fprintf(stderr, "Unable to fine file %s\n", uvar_fnameIn);
	return DRIVEHOUGHCOLOR_EFILE;
      }


    nTemplates = 0;
    do 
      {
	r=fscanf(fpIn,"%lf%lf%lf%lf%lf\n", &temp1, &temp2, &temp3, &temp4, &temp5);
	/* make sure the line has the right number of entries or is EOF */
	if (r==5) nTemplates++;
      } while ( r != EOF);
    rewind(fpIn);
    
    alphaVec = (REAL8 *)LALMalloc(nTemplates*sizeof(REAL8));
    deltaVec = (REAL8 *)LALMalloc(nTemplates*sizeof(REAL8));     
    freqVec = (REAL8 *)LALMalloc(nTemplates*sizeof(REAL8));
    spndnVec = (REAL8 *)LALMalloc(nTemplates*sizeof(REAL8));     
    
    
    for (templateCounter = 0; templateCounter < nTemplates; templateCounter++)
      {
	r=fscanf(fpIn,"%lf%lf%lf%lf%lf\n", &temp1, alphaVec + templateCounter, deltaVec + templateCounter, 
		 freqVec + templateCounter,  spndnVec + templateCounter);
      }     
    fclose(fpIn);      
  }


  /**************************************************/
  /* read sfts */     
  /*************************************************/
  fmin = freqVec[0];     /* initial frequency to be analyzed */
  /* assume that the last frequency in the templates file is also the highest frequency */
  fmax = freqVec[nTemplates-1] ; 
  
  /* we need to add wings to fmin and fmax to account for 
     the Doppler shift, the size of the rngmed block size
     and also nfsizecylinder.  The block size and nfsizecylinder are
     specified in terms of frequency bins...this goes as one of the arguments of 
     LALReadSFTfiles */
  /* first correct for Doppler shift */
  fWings =  fmax * VTOT; 
  fmin -= fWings;    
  fmax += fWings; 
  
  /* create pattern to look for in SFT directory */   
  strcat(uvar_SFTdir, "/*SFT*.*");
   
  /* now read all the sfts */
  SUB (LALReadSFTfiles (&status, &inputSFTs, fmin, fmax, nfSizeCylinder + uvar_blocksRngMed, uvar_SFTdir), &status);
  mObsCoh = inputSFTs->length; 
  sftlength = inputSFTs->data->data->length;
  timeBase = 1.0/inputSFTs->data->deltaF;
  sftFminBin = floor( timeBase * inputSFTs->data->f0 + 0.5);


  /* loop over sfts and select peaks */

  for (tempLoopId=0; tempLoopId < mObsCoh; tempLoopId++){

    /* first the memory allocation for the peakgrams */
    pgV = (UCHARPeakGram **)LALMalloc(mObsCoh * sizeof(UCHARPeakGram *));
    pg1 = pgV[tempLoopId];  
    pg1->length = sftlength;
    pg1->data = NULL;
    pg1->data = (UCHAR *)LALMalloc(sftlength* sizeof(UCHAR));
      
    /* calculate the periodogram */
    SUB( SFT2Periodogram(&status, &periPSD.periodogram, inputSFTs->data + tempLoopId ), &status );	
    
    /* calculate psd using running median */
    SUB( LALPeriodo2PSDrng( &status, &periPSD.psd, &periPSD.periodogram, &uvar_blocksRngMed), &status );	
    
    /* select sft bins to get peakgrams using threshold */
    SUB( LALSelectPeakColorNoise(&status, pg1, &threshold,&periPSD), &status); 	
    
  } /* end of loop over sfts */

  /* having calculated the peakgrams we don't need the sfts anymore */
  SUB(LALDestroySFTVector(&status, &inputSFTs),&status );

  
  /* ****************************************************************/
  /* setting timestamps vector */
  /* ****************************************************************/
  timeV.length = mObsCoh;
  timeV.data = NULL;  
  timeV.data = (LIGOTimeGPS *)LALMalloc(mObsCoh*sizeof(LIGOTimeGPS));
  
  { 
    UINT4    j; 
    for (j=0; j < mObsCoh; j++){
      timeV.data[j].gpsSeconds = inputSFTs->data->epoch.gpsSeconds;
      timeV.data[j].gpsNanoSeconds = inputSFTs->data->epoch.gpsNanoSeconds;
    }    
  }
  
  /******************************************************************/
  /* compute the time difference relative to startTime for all SFT */
  /******************************************************************/
  timeDiffV.length = mObsCoh;
  timeDiffV.data = NULL; 
  timeDiffV.data = (REAL8 *)LALMalloc(mObsCoh*sizeof(REAL8));
  
  {   
    REAL8   t0, ts, tn;
    UINT4   j; 

    ts = timeV.data[0].gpsSeconds;
    tn = timeV.data[0].gpsNanoSeconds * 1.00E-9;
    t0=ts+tn;
    timeDiffV.data[0] = 0.0;

    for (j=1; j< mObsCoh; ++j){
      ts = timeV.data[j].gpsSeconds;
      tn = timeV.data[j].gpsNanoSeconds * 1.00E-9;  
      timeDiffV.data[j] = ts+tn -t0; 
    }  
  }

  /******************************************************************/ 
  /*   setting of ephemeris info */ 
  /******************************************************************/ 
  edat = (EphemerisData *)LALMalloc(sizeof(EphemerisData));
  (*edat).ephiles.earthEphemeris = uvar_earthEphemeris;
  (*edat).ephiles.sunEphemeris = uvar_sunEphemeris;
  
  /******************************************************************/
  /* compute detector velocity for those time stamps  
     if it is too slow , we can read it from the file genrated from the driver*/
  /******************************************************************/
  velV.length = mObsCoh;
  velV.data = NULL;
  velV.data = (REAL8Cart3Coor *)LALMalloc(mObsCoh*sizeof(REAL8Cart3Coor));
  
  {  
    VelocityPar   velPar;
    REAL8     vel[3]; 
    UINT4     j; 
    
    LALLeapSecFormatAndAcc lsfas = {LALLEAPSEC_GPSUTC, LALLEAPSEC_STRICT};
    INT4 tmpLeap; /* need this because Date pkg defines leap seconds as
		     INT4, while EphemerisData defines it to be INT2. This won't
                   cause problems before, oh, I don't know, the Earth has been 
                   destroyed in nuclear holocaust. -- dwchin 2004-02-29 */    
    
    velPar.detector = detector;
    velPar.tBase = timeBase;
    velPar.vTol = ACCURACY;
    velPar.edat = NULL;
    
    /* Leap seconds for the start time of the run */   
    SUB( LALLeapSecs(&status, &tmpLeap, &(timeV.data[0]), &lsfas), &status);
    (*edat).leap = (INT2)tmpLeap;
    
    /* read in ephemeris data */
    SUB( LALInitBarycenter( &status, edat), &status);
    velPar.edat = edat;
    
    for(j=0; j< velV.length; ++j){
      velPar.startTime.gpsSeconds     = timeV.data[j].gpsSeconds;
      velPar.startTime.gpsNanoSeconds = timeV.data[j].gpsNanoSeconds;
      
      SUB( LALAvgDetectorVel ( &status, vel, &velPar), &status );
      velV.data[j].x= vel[0];
      velV.data[j].y= vel[1];
      velV.data[j].z= vel[2];   
    }  
  }

  /******************************************************************/ 
  /*   setting of parameters */ 
  /******************************************************************/ 

  
  pulsarTemplate.spindown.length = msp;
  pulsarTemplate.spindown.data = NULL;
  pulsarTemplate.spindown.data = (REAL8 *)LALMalloc(msp*sizeof(REAL8));
 
  
  periPSD.periodogram.length = sftlength;
  periPSD.periodogram.data = NULL;
  periPSD.periodogram.data = (REAL8 *)LALMalloc(sftlength* sizeof(REAL8));
  periPSD.psd.length = sftlength;
  periPSD.psd.data = NULL;
  periPSD.psd.data = (REAL8 *)LALMalloc(sftlength* sizeof(REAL8));
  
  threshold = uvar_peakThreshold/normalizeThr; 
  

  
  foft.length = mObsCoh;
  foft.data = NULL;
  foft.data = (REAL8 *)LALMalloc(mObsCoh*sizeof(REAL8));
  /* ****************************************************************/
   
  for(loopId=0; loopId < nTemplates; ++loopId){
    
    /* set template parameters */    
    pulsarTemplate.f0 = freqVec[loopId];
    pulsarTemplate.longitude = alphaVec[loopId];
    pulsarTemplate.latitude = deltaVec[loopId];
    pulsarTemplate.spindown.data[0] = spndnVec[loopId];
	   
 
    /* ****************************************************************/
    /* Computing the frequency path f(t) = f0(t)* (1+v/c.n)  fot the Hough map */
    /* ****************************************************************/   
    {
      REAL8   f0new, vcProdn, timeDiffN;
      REAL8   sourceDelta, sourceAlpha, cosDelta;
      UINT4   j;
      INT4    i, nspin, factorialN; 
      REAL8Cart3Coor       sourceLocation;
      
      sourceDelta = pulsarTemplate.latitude;
      sourceAlpha = pulsarTemplate.longitude;
      cosDelta = cos(sourceDelta);
      
      sourceLocation.x = cosDelta* cos(sourceAlpha);
      sourceLocation.y = cosDelta* sin(sourceAlpha);
      sourceLocation.z = sin(sourceDelta);
      
      nspin = pulsarTemplate.spindown.length;
      for (j=0; j<mObsCoh; ++j){  /* loop for all different time stamps */
	vcProdn = velV.data[j].x * sourceLocation.x
	  + velV.data[j].y * sourceLocation.y
	  + velV.data[j].z * sourceLocation.z;
	f0new = pulsarTemplate.f0;
	factorialN = 1;
	timeDiffN = timeDiffV.data[j];
	
	for (i=0; i<nspin;++i){ /* loop for spin-down values */
	  factorialN *=(i+1);
	  f0new += pulsarTemplate.spindown.data[i]* timeDiffN / factorialN;
	  timeDiffN *= timeDiffN;
	}
	foft.data[j] = f0new * (1.0 +vcProdn);
      }    
    } /* end of block calculating frequency path */
    
    /* initialize number count */
    numberCount=0;
    
    /* ****************************************************************/
    /* loop over peakgrams and produce the number-count*/
    /* ****************************************************************/      
    for (tempLoopId=0; tempLoopId < mObsCoh; tempLoopId++){

      /* get the right peakgram */      
      pg1 = pgV[tempLoopId];

      /* calculate frequency bin for template */
      index = floor( foft.data[tempLoopId]*timeBase -sftFminBin+0.5); 
      
      /* update the number count */
      numberCount+=pg1->data[index]; 
    } /* end of loop over sfts */
      
    /******************************************************************/
    /* printing result in the output file */
    /******************************************************************/
    
    fprintf(fpOut,"%d %f %f %f %g \n", 
	    numberCount, pulsarTemplate.longitude, pulsarTemplate.latitude, pulsarTemplate.f0,
	    pulsarTemplate.spindown.data[0] );
    
  } /* end of loop over templates */

  /******************************************************************/
  /* Closing files */
  /******************************************************************/  
  fclose(fpOut); 

  
  /******************************************************************/
  /* Free memory and exit */
  /******************************************************************/

  LALFree(alphaVec);
  LALFree(deltaVec);
  LALFree(freqVec);
  LALFree(spndnVec);
  

  LALFree(periPSD.periodogram.data);
  LALFree(periPSD.psd.data);

  for (tempLoopId = 0; tempLoopId < mObsCoh; tempLoopId++){
    pg1 = pgV[tempLoopId];  
    LALFree(pg1->data);
  }
  LALFree(pgV);
  
  LALFree(timeV.data);
  LALFree(timeDiffV.data);
  LALFree(velV.data);
  LALFree(foft.data);

  
  LALFree(pulsarTemplate.spindown.data);
   
  LALFree(edat->ephemE);
  LALFree(edat->ephemS);
  LALFree(edat);
  
  SUB (LALDestroyUserVars(&status), &status);

  LALCheckMemoryLeaks();
  
  
  INFO( DRIVEHOUGHCOLOR_MSGENORM );
  return DRIVEHOUGHCOLOR_ENORM;
}


 

