/******************************** <lalVerbatim file="SZeroPadAndFFTTestCV">
Author: UTB Relativity Group; contact J. T. Whelan
$Id$
********************************* </lalVerbatim> */

/********************************************************** <lalLaTeX>
\subsection{Program \texttt{SZeroPadAndFFTTest.c}}
\label{stochastic:ss:SZeroPadAndFFTTest.c}

Test suite for \texttt{LALSZeroPadANdFFT()}.

\subsubsection*{Usage}
\begin{verbatim}
./SZeroPadAndFFTTest
Options:
  -h             print this message
  -q             quiet: run silently
  -v             verbose: print extra information
  -d level       set lalDebugLevel to level
  -i filename    read input from filename
  -o filename    print  output to file filename
  -n             set the length 
\end{verbatim}

\subsubsection*{Description}

This program tests the routine \texttt{LALSZeroPadAndFFT()}, which
zero-pads and Fourier transforms a real time series of length $N$ to
produce a complex frequency series of length $N$.

First, it tests that the correct error codes 
(\textit{cf.}\ Sec.~\ref{stochastic:s:StochasticCrossCorrelation.h})
are generated for the following error conditions (tests in
\textit{italics} are not performed if \verb+LAL_NEDEBUG+ is set, as
the corresponding checks in the code are made using the ASSERT macro):
\begin{itemize}
\item \textit{null pointer to output series}
\item \textit{null pointer to input series}
\item \textit{null pointer to plan parameter}
\item \textit{null pointer to data member of output series}
\item \textit{null pointer to data member of input series}
\item \textit{null pointer to data member of optimal filter}
\item \textit{null pointer to data member of data member of input series}
\item \textit{null pointer to data member of data member of output series}
\item \textit{zero length}
\item \textit{negative time spacing}
\item \textit{zero time spacing}
\item negative heterodyning frequency
\item positive heterodyning frequency
\item length mismatch between input series and output series
\item length mismatch between input series and plan parameter
\end{itemize}

It then verifies that the correct frequency series is generated for
the simple test case $\{h[k]=1+k|k=0,\ldots,7\}$ with $\delta
t=0.5\,\textrm{s}$.
For each successful test
(both of these valid data and the invalid ones described above), it
prints ``\texttt{PASS}'' to standard output; if a test fails, it
prints ``\texttt{FAIL}''.

If the \texttt{filename} arguments are present, it also reads a time
series from a file, calls \texttt{LALSZeroPadAndFFT()}, and writes the
results to the specified output file.

\subsubsection*{Exit codes}
\input{SZeroPadAndFFTTestCE}

\subsubsection*{Uses}
\begin{verbatim}
LALSZeroPadAndFFT()
LALCheckMemoryLeaks()
LALSReadTimeSeries()
LALCPrintFrequencySeries()
LALCCreateVector()
LALCDestroyVector()
LALSCreateVector()
LALSDestroyVector()
LALCHARCreateVector()
LALCHARDestroyVector()
LALEstimateFwdRealFFTPlan()
LALMeasureFwdRealFFTPlan()
LALDestroyRealFFTPlan()
LALUnitAsString()
LALUnitCompare()
getopt()
printf()
fprintf()
freopen()
fabs()
\end{verbatim}

\subsubsection*{Notes}

\begin{itemize}
  \item No specific error checking is done on user-specified data.  If
    \texttt{length} is missing, the resulting default will cause a bad
    data error.
\end{itemize}

\vfill{\footnotesize\input{SZeroPadAndFFTTestCV}}

******************************************************* </lalLaTeX> */

#include <lal/LALStdlib.h>

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <config.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif

#include <lal/StochasticCrossCorrelation.h>
#include <lal/AVFactories.h>
#include <lal/ReadFTSeries.h>
#include <lal/PrintFTSeries.h>
#include <lal/Units.h>

#include "CheckStatus.h"

NRCSID(SZEROPADANDFFTTESTC, "$Id$");

#define SZEROPADANDFFTTESTC_LENGTH        8
#define SZEROPADANDFFTTESTC_FULLLENGTH (2 * SZEROPADANDFFTTESTC_LENGTH - 1)
#define SZEROPADANDFFTTESTC_EPOCHSEC      1234
#define SZEROPADANDFFTTESTC_EPOCHNS       56789
#define SZEROPADANDFFTTESTC_DELTAT        0.5
#define SZEROPADANDFFTTESTC_DELTAF 1.0/(SZEROPADANDFFTTESTC_FULLLENGTH * SZEROPADANDFFTTESTC_DELTAT)
#define SZEROPADANDFFTTESTC_TOL           1e-6

#define SZEROPADANDFFT_TRUE     1
#define SZEROPADANDFFT_FALSE    0

extern char *optarg;
extern int   optind;

/* int lalDebugLevel = LALMSGLVL3; */
int lalDebugLevel  = LALNDEBUG;
BOOLEAN optVerbose = SZEROPADANDFFT_FALSE;
BOOLEAN optMeasurePlan = SZEROPADANDFFT_FALSE;
UINT4 optLength    = 0;


CHAR optInputFile[LALNameLength] = "";
CHAR optOutputFile[LALNameLength] = "";
INT4 code;

static void
Usage (const char *program, int exitflag);

static void
ParseOptions (int argc, char *argv[]);

/***************************** <lalErrTable file="SZeroPadAndFFTTestCE"> */
#define SZEROPADANDFFTTESTC_ENOM 0
#define SZEROPADANDFFTTESTC_EARG 1
#define SZEROPADANDFFTTESTC_ECHK 2
#define SZEROPADANDFFTTESTC_EFLS 3
#define SZEROPADANDFFTTESTC_EUSE 4

#define SZEROPADANDFFTTESTC_MSGENOM "Nominal exit"
#define SZEROPADANDFFTTESTC_MSGEARG "Error parsing command-line arguments"
#define SZEROPADANDFFTTESTC_MSGECHK "Error checking failed to catch bad data"
#define SZEROPADANDFFTTESTC_MSGEFLS "Incorrect answer for valid data"
#define SZEROPADANDFFTTESTC_MSGEUSE "Bad user-entered data"
/***************************** </lalErrTable> */

int 
main( int argc, char *argv[] )
{
   
   static LALStatus         status;

   UINT4      i;
   REAL8      f;
   
   REAL4                   *sPtr;
   COMPLEX8                *cPtr;

   const REAL4    testInputDataData[SZEROPADANDFFTTESTC_LENGTH] 
                     = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};

   COMPLEX8 expectedOutputDataData[SZEROPADANDFFTTESTC_LENGTH] 
                     = {{+3.600000000000000e+01, 0.0},
                        {-1.094039137097177e+01, +2.279368601990178e+01},
                        {+3.693524635113721e-01, -9.326003289238411e+00},
                        {-8.090169943749448e-01, +7.918722831227928e+00},
                        {+3.502214272222959e-01, -5.268737078678177e+00},
                        {+5.329070518200751e-15, +5.196152422706625e+00},
                        {+3.090169943749475e-01, -4.306254604896173e+00},
                        {+2.208174802380956e-01, +4.325962305777781e+00}};

   RealFFTPlan                *plan = NULL;
   REAL4TimeSeries             goodInput, badInput;
   COMPLEX8FrequencySeries     goodOutput, badOutput;

   BOOLEAN                result;
   LALUnitPair            unitPair;
   LALUnit                dimensionless = {};
   CHARVector             *unitString;

   for (i=0; i<SZEROPADANDFFTTESTC_LENGTH; ++i)
   {
     expectedOutputDataData[i].re *= SZEROPADANDFFTTESTC_DELTAT;
     expectedOutputDataData[i].im *= SZEROPADANDFFTTESTC_DELTAT;
   }

   ParseOptions( argc, argv );

   /* TEST INVALID DATA HERE ------------------------------------------- */

   /* define valid parameters */
   goodInput.f0                   = 0.0;
   goodInput.deltaT               = SZEROPADANDFFTTESTC_DELTAT;
   goodInput.epoch.gpsSeconds     = SZEROPADANDFFTTESTC_EPOCHSEC;
   goodInput.epoch.gpsNanoSeconds = SZEROPADANDFFTTESTC_EPOCHNS;
   goodInput.data                 = NULL;
   goodOutput.data                = NULL;

   badInput = goodInput;
   badOutput = goodOutput;

   /* construct plan */ 
   LALEstimateFwdRealFFTPlan(&status, &plan, 
                             SZEROPADANDFFTTESTC_FULLLENGTH);
   if ( code = CheckStatus( &status, 0 , "", SZEROPADANDFFTTESTC_EFLS,
                            SZEROPADANDFFTTESTC_MSGEFLS ) ) 
   {
     return code;
   }
   /* allocate input and output vectors */
   LALSCreateVector(&status, &(goodInput.data), SZEROPADANDFFTTESTC_LENGTH);
   if ( code = CheckStatus(&status, 0 , "",
                           SZEROPADANDFFTTESTC_EFLS,
                           SZEROPADANDFFTTESTC_MSGEFLS) ) 
   {
     return code;
   }
   LALCCreateVector(&status, &(goodOutput.data), SZEROPADANDFFTTESTC_LENGTH);
   if ( code = CheckStatus(&status, 0 , "",
                           SZEROPADANDFFTTESTC_EFLS,
                           SZEROPADANDFFTTESTC_MSGEFLS) ) 
   {
     return code;
   }

#ifndef LAL_NDEBUG
   if ( ! lalNoDebug )
   {
     /* test behavior for null pointer to output series */
     LALSZeroPadAndFFT(&status, NULL, &goodInput, plan);
     if ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_ENULLP, 
                             STOCHASTICCROSSCORRELATIONH_MSGENULLP,
                             SZEROPADANDFFTTESTC_ECHK,
                             SZEROPADANDFFTTESTC_MSGECHK )) 
     {
       return code;
     }
     printf("  PASS: null pointer to output series results in error:\n       \"%s\"\n", STOCHASTICCROSSCORRELATIONH_MSGENULLP);

     /* test behavior for null pointer to input series */
     LALSZeroPadAndFFT(&status, &goodOutput, NULL, plan);
     if ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_ENULLP, 
                             STOCHASTICCROSSCORRELATIONH_MSGENULLP,
                             SZEROPADANDFFTTESTC_ECHK,
                             SZEROPADANDFFTTESTC_MSGECHK )) 
     {
       return code;
     }
     printf("  PASS: null pointer to input series results in error:\n       \"%s\"\n", STOCHASTICCROSSCORRELATIONH_MSGENULLP);
   
     /* test behavior for null pointer to plan parameter */
     LALSZeroPadAndFFT(&status, &goodOutput, &goodInput, NULL);
     if ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_ENULLP, 
                             STOCHASTICCROSSCORRELATIONH_MSGENULLP,
                             SZEROPADANDFFTTESTC_ECHK,
                             SZEROPADANDFFTTESTC_MSGECHK )) 
     {
       return code;
     }
     printf("  PASS: null pointer to plan parameter results in error:\n       \"%s\"\n", STOCHASTICCROSSCORRELATIONH_MSGENULLP);
   
     /* test behavior for null pointer to data member of output series */
     LALSZeroPadAndFFT(&status, &badOutput, &goodInput, plan);
     if ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_ENULLP, 
                             STOCHASTICCROSSCORRELATIONH_MSGENULLP,
                             SZEROPADANDFFTTESTC_ECHK,
                             SZEROPADANDFFTTESTC_MSGECHK )) 
     {
       return code;
     }
     printf("  PASS: null pointer to data member of output series results in error:\n       \"%s\"\n", STOCHASTICCROSSCORRELATIONH_MSGENULLP);

     /* test behavior for null pointer to data member of input series */
     LALSZeroPadAndFFT(&status, &goodOutput, &badInput, plan);
     if ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_ENULLP, 
                             STOCHASTICCROSSCORRELATIONH_MSGENULLP,
                             SZEROPADANDFFTTESTC_ECHK,
                             SZEROPADANDFFTTESTC_MSGECHK )) 
     {
       return code;
     }
     printf("  PASS: null pointer to data member of input series results in error:\n       \"%s\"\n", STOCHASTICCROSSCORRELATIONH_MSGENULLP);

     /* test behavior for null pointer to data member of data member of output series */
     LALCCreateVector(&status, &(badOutput.data), SZEROPADANDFFTTESTC_LENGTH);
     if ( code = CheckStatus(&status, 0 , "",
                             SZEROPADANDFFTTESTC_EFLS,
                             SZEROPADANDFFTTESTC_MSGEFLS) ) 
     {
       return code;
     }
     cPtr = badOutput.data->data;
     badOutput.data->data = NULL;
     LALSZeroPadAndFFT(&status, &badOutput, &goodInput, plan);
     if ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_ENULLP, 
                             STOCHASTICCROSSCORRELATIONH_MSGENULLP,
                             SZEROPADANDFFTTESTC_ECHK,
                             SZEROPADANDFFTTESTC_MSGECHK )) 
     {
       return code;
     }
     printf("  PASS: null pointer to data member of data member of output series results in error:\n       \"%s\"\n", STOCHASTICCROSSCORRELATIONH_MSGENULLP);
     badOutput.data->data = cPtr;
     LALCDestroyVector(&status, &(badOutput.data));
     if ( code = CheckStatus(&status, 0 , "",
                             SZEROPADANDFFTTESTC_EFLS,
                             SZEROPADANDFFTTESTC_MSGEFLS) ) 
     {
       return code;
     }
     
     /* test behavior for null pointer to data member of data member of output series */
     LALSCreateVector(&status, &(badInput.data), SZEROPADANDFFTTESTC_LENGTH);
     if ( code = CheckStatus(&status, 0 , "",
                             SZEROPADANDFFTTESTC_EFLS,
                             SZEROPADANDFFTTESTC_MSGEFLS) ) 
     {
       return code;
     }
     sPtr = badInput.data->data;
     badInput.data->data = NULL;
     LALSZeroPadAndFFT(&status, &goodOutput, &badInput, plan);
     if ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_ENULLP, 
                             STOCHASTICCROSSCORRELATIONH_MSGENULLP,
                             SZEROPADANDFFTTESTC_ECHK,
                             SZEROPADANDFFTTESTC_MSGECHK )) 
     {
       return code;
     }
     printf("  PASS: null pointer to data member of data member of input series results in error:\n       \"%s\"\n", STOCHASTICCROSSCORRELATIONH_MSGENULLP);
     badInput.data->data = sPtr;
     LALSDestroyVector(&status, &(badInput.data));
     if ( code = CheckStatus(&status, 0 , "",
                             SZEROPADANDFFTTESTC_EFLS,
                             SZEROPADANDFFTTESTC_MSGEFLS) ) 
     {
       return code;
     }

     /* test behavior for zero length */
     goodInput.data->length = goodOutput.data->length = 0;
     plan->size = -1;
     LALSZeroPadAndFFT(&status, &goodOutput, &goodInput, plan);
     if ( code = CheckStatus(&status, STOCHASTICCROSSCORRELATIONH_EZEROLEN,
                             STOCHASTICCROSSCORRELATIONH_MSGEZEROLEN,
                             SZEROPADANDFFTTESTC_ECHK,
                             SZEROPADANDFFTTESTC_MSGECHK)) 
     {
       return code;
     }
     printf("  PASS: zero length results in error:\n       \"%s\"\n",
            STOCHASTICCROSSCORRELATIONH_MSGEZEROLEN);
     /* reassign valid length */
     goodInput.data->length = goodOutput.data->length 
       = SZEROPADANDFFTTESTC_LENGTH;
     plan->size = SZEROPADANDFFTTESTC_FULLLENGTH;

     /* test behavior for negative time spacing */
     goodInput.deltaT = -SZEROPADANDFFTTESTC_DELTAT;
     LALSZeroPadAndFFT(&status, &goodOutput, &goodInput, plan);
     if ( code = CheckStatus(&status,
                             STOCHASTICCROSSCORRELATIONH_ENONPOSDELTAT,
                             STOCHASTICCROSSCORRELATIONH_MSGENONPOSDELTAT,
                             SZEROPADANDFFTTESTC_ECHK,
                             SZEROPADANDFFTTESTC_MSGECHK)) 
     {
       return code;
     }
     printf("  PASS: negative time spacing results in error:\n       \"%s\"\n",
            STOCHASTICCROSSCORRELATIONH_MSGENONPOSDELTAT);
    
     /* test behavior for zero time spacing */
     goodInput.deltaT = 0;
     LALSZeroPadAndFFT(&status, &goodOutput, &goodInput, plan);
     if ( code = CheckStatus(&status,
                             STOCHASTICCROSSCORRELATIONH_ENONPOSDELTAT,
                             STOCHASTICCROSSCORRELATIONH_MSGENONPOSDELTAT,
                             SZEROPADANDFFTTESTC_ECHK,
                             SZEROPADANDFFTTESTC_MSGECHK)) 
     {
       return code;
     }
     printf("  PASS: zero time spacing results in error:\n       \"%s\"\n",
            STOCHASTICCROSSCORRELATIONH_MSGENONPOSDELTAT);
     /* reassign valid time spacing */
     goodInput.deltaT = SZEROPADANDFFTTESTC_DELTAT;
     
   } /* if ( ! lalNoDebug ) */
#endif /* NDEBUG */

   /* test behavior for negative heterodyning frequency */
   goodInput.f0 = -100.0;
   LALSZeroPadAndFFT(&status, &goodOutput, &goodInput, plan);
   if ( code = CheckStatus(&status,
                           STOCHASTICCROSSCORRELATIONH_ENONZEROHETERO,
                           STOCHASTICCROSSCORRELATIONH_MSGENONZEROHETERO,
                           SZEROPADANDFFTTESTC_ECHK,
                           SZEROPADANDFFTTESTC_MSGECHK)) 
   {
     return code;
   }
   printf("  PASS: negative heterodyning frequency results in error:\n       \"%s\"\n",
            STOCHASTICCROSSCORRELATIONH_MSGENONZEROHETERO);
   /* test behavior for positive heterodyning frequency */
   goodInput.f0 = 100.0;
   LALSZeroPadAndFFT(&status, &goodOutput, &goodInput, plan);
   if ( code = CheckStatus(&status,
                           STOCHASTICCROSSCORRELATIONH_ENONZEROHETERO,
                           STOCHASTICCROSSCORRELATIONH_MSGENONZEROHETERO,
                           SZEROPADANDFFTTESTC_ECHK,
                           SZEROPADANDFFTTESTC_MSGECHK)) 
   {
     return code;
   }
   printf("  PASS: positive heterodyning frequency results in error:\n       \"%s\"\n",
          STOCHASTICCROSSCORRELATIONH_MSGENONZEROHETERO);
   goodInput.f0 = 0.0;
   
   /* test behavior for length mismatch between input series and output series */
   goodOutput.data->length = SZEROPADANDFFTTESTC_LENGTH + 1;
   LALSZeroPadAndFFT(&status, &goodOutput, &goodInput, plan);
   if ( code = CheckStatus(&status,
                           STOCHASTICCROSSCORRELATIONH_EMMLEN,
                           STOCHASTICCROSSCORRELATIONH_MSGEMMLEN,
                           SZEROPADANDFFTTESTC_ECHK,
                           SZEROPADANDFFTTESTC_MSGECHK)) 
   {
     return code;
   }
   printf("  PASS: length mismatch between input series and output series results in error:\n       \"%s\"\n",
          STOCHASTICCROSSCORRELATIONH_MSGEMMLEN);
   goodOutput.data->length = SZEROPADANDFFTTESTC_LENGTH;

   /* test behavior for length mismatch between input series and plan parameter */
   plan->size = 2 * SZEROPADANDFFTTESTC_LENGTH + 1;
   LALSZeroPadAndFFT(&status, &goodOutput, &goodInput, plan);
   if ( code = CheckStatus(&status,
                           STOCHASTICCROSSCORRELATIONH_EMMLEN,
                           STOCHASTICCROSSCORRELATIONH_MSGEMMLEN,
                           SZEROPADANDFFTTESTC_ECHK,
                           SZEROPADANDFFTTESTC_MSGECHK)) 
   {
     return code;
   }
   printf("  PASS: length mismatch between input series and plan parameter results in error:\n       \"%s\"\n",
          STOCHASTICCROSSCORRELATIONH_MSGEMMLEN);
   plan->size = SZEROPADANDFFTTESTC_FULLLENGTH;

   /* TEST VALID DATA HERE --------------------------------------------- */

   /* fill input time-series parameters */
   strncpy(goodInput.name,"Dummy test data",LALNameLength);
   goodInput.sampleUnits  = dimensionless;
   goodInput.sampleUnits.unitNumerator[LALUnitIndexADCCount] = 1;

     /* fill input time-series data */
   for (i=0; i<SZEROPADANDFFTTESTC_LENGTH; ++i)
   {
     goodInput.data->data[i] = testInputDataData[i];
   }

   /* zero-pad and FFT */
   LALSZeroPadAndFFT(&status, &goodOutput, &goodInput, plan);
   if ( code = CheckStatus( &status, 0 , "", SZEROPADANDFFTTESTC_EFLS,
                            SZEROPADANDFFTTESTC_MSGEFLS) )
   {
     return code;
   }

   /* check output f0 */
   if (optVerbose)
   {
     printf("f0=%g, should be 0\n", goodOutput.f0);
   }
   if (goodOutput.f0)
   {
     printf("  FAIL: Valid data test\n");
     if (optVerbose)
     {
       printf("Exiting with error: %s\n", SZEROPADANDFFTTESTC_MSGEFLS);
     }
     return SZEROPADANDFFTTESTC_EFLS;
   }

   /* check output deltaF */
   if (optVerbose)
   {
     printf("deltaF=%g, should be %g\n", goodOutput.deltaF,
            SZEROPADANDFFTTESTC_DELTAF);
   }
   if ( fabs(goodOutput.deltaF-SZEROPADANDFFTTESTC_DELTAF)
        / SZEROPADANDFFTTESTC_DELTAF > SZEROPADANDFFTTESTC_TOL )
   {
     printf("  FAIL: Valid data test\n");
     if (optVerbose)
     {
       printf("Exiting with error: %s\n", SZEROPADANDFFTTESTC_MSGEFLS);
     }
     return SZEROPADANDFFTTESTC_EFLS;
   }

   /* check output epoch */
   if (optVerbose)
   {
     printf("epoch=%d seconds, %d nanoseconds; should be %d seconds, %d nanoseconds\n",
            goodOutput.epoch.gpsSeconds, goodOutput.epoch.gpsNanoSeconds,
            SZEROPADANDFFTTESTC_EPOCHSEC, SZEROPADANDFFTTESTC_EPOCHNS);
   }
   if ( goodOutput.epoch.gpsSeconds != SZEROPADANDFFTTESTC_EPOCHSEC
        || goodOutput.epoch.gpsNanoSeconds != SZEROPADANDFFTTESTC_EPOCHNS )
   {
     printf("  FAIL: Valid data test\n");
     if (optVerbose)
     {
       printf("Exiting with error: %s\n", SZEROPADANDFFTTESTC_MSGEFLS);
     }
     return SZEROPADANDFFTTESTC_EFLS;
   }

   /* check output units */
   unitPair.unitOne = dimensionless;
   unitPair.unitOne.unitNumerator[LALUnitIndexADCCount] = 1;
   unitPair.unitOne.unitNumerator[LALUnitIndexSecond] = 1;
   unitPair.unitTwo = goodOutput.sampleUnits;
   LALUnitCompare(&status, &result, &unitPair);
   if ( code = CheckStatus(&status, 0 , "",
			   SZEROPADANDFFTTESTC_EFLS,
			   SZEROPADANDFFTTESTC_MSGEFLS) ) 
   {
     return code;
   }

   if (optVerbose) 
   {
     LALCHARCreateVector(&status, &unitString, LALUnitTextSize);
     if ( code = CheckStatus(&status, 0 , "",
			     SZEROPADANDFFTTESTC_EFLS,
			     SZEROPADANDFFTTESTC_MSGEFLS) ) 
     {
       return code;
     }
    
     LALUnitAsString( &status, unitString, &(unitPair.unitTwo) );
     if ( code = CheckStatus(&status, 0 , "",
                            SZEROPADANDFFTTESTC_EFLS,
                            SZEROPADANDFFTTESTC_MSGEFLS) ) 
     {
       return code;
     }
     printf( "Units are \"%s\", ", unitString->data );
     
     LALUnitAsString( &status, unitString, &(unitPair.unitOne) );
     if ( code = CheckStatus(&status, 0 , "",
			     SZEROPADANDFFTTESTC_EFLS,
			     SZEROPADANDFFTTESTC_MSGEFLS) ) 
     {
       return code;
     }
     printf( "should be \"%s\"\n", unitString->data );
     
     LALCHARDestroyVector(&status, &unitString);
     if ( code = CheckStatus(&status, 0 , "",
			     SZEROPADANDFFTTESTC_EFLS,
			     SZEROPADANDFFTTESTC_MSGEFLS) ) 
     {
       return code;
     }
   }

   if (!result)
   {
     printf("  FAIL: Valid data test #1\n");
     if (optVerbose)
     {
       printf("Exiting with error: %s\n", 
	      SZEROPADANDFFTTESTC_MSGEFLS);
     }
     return SZEROPADANDFFTTESTC_EFLS;
   }

   /* check output values */
   if (optVerbose) 
   {
     printf("hBarTilde(0)=%g + %g i, should be %g\n",
            goodOutput.data->data[0].re, goodOutput.data->data[0].im,
            expectedOutputDataData[0].re);
   }
   if ( fabs(goodOutput.data->data[0].re - expectedOutputDataData[0].re)
        /* / expectedOutputDataData[0].re */> SZEROPADANDFFTTESTC_TOL
        || fabs(goodOutput.data->data[0].im) > SZEROPADANDFFTTESTC_TOL )
   {
     printf("  FAIL: Valid data test\n");
     if (optVerbose)
       {
         printf("Exiting with error: %s\n", SZEROPADANDFFTTESTC_MSGEFLS);
       }
     return SZEROPADANDFFTTESTC_EFLS;
   }
   
   for (i=1; i<SZEROPADANDFFTTESTC_LENGTH; ++i)
   {
     f = i * SZEROPADANDFFTTESTC_DELTAF;
     if (optVerbose) 
     {
       printf("hBarTilde(%f Hz)=%g + %g i, should be %g + %g i\n",
              f, goodOutput.data->data[i].re, goodOutput.data->data[i].im,
              expectedOutputDataData[i].re, expectedOutputDataData[i].im);
     }
     if (fabs(goodOutput.data->data[i].re - expectedOutputDataData[i].re)
         /* / expectedOutputDataData[0].re */> SZEROPADANDFFTTESTC_TOL
         || fabs(goodOutput.data->data[i].im - expectedOutputDataData[i].im)
         /* / expectedOutputDataData[0].re */> SZEROPADANDFFTTESTC_TOL)
     {
       printf("  FAIL: Valid data test\n");
       if (optVerbose)
       {
         printf("Exiting with error: %s\n", SZEROPADANDFFTTESTC_MSGEFLS);
       }
       return SZEROPADANDFFTTESTC_EFLS;
     }
   }

    /* write results to output file 
   LALSPrintTimeSeries(&input, "zeropadgoodInput.dat");
   LALCPrintFrequencySeries(&output, "zeropadgoodOutput.dat");*/
  
   /* clean up valid data */
   LALSDestroyVector(&status, &goodInput.data);
   if ( code = CheckStatus(&status, 0 , "", SZEROPADANDFFTTESTC_EFLS,
                            SZEROPADANDFFTTESTC_MSGEFLS) ) 
   {
     return code;
   }
   LALCDestroyVector(&status, &goodOutput.data);
   if ( code = CheckStatus(&status, 0 , "", SZEROPADANDFFTTESTC_EFLS,
                            SZEROPADANDFFTTESTC_MSGEFLS) ) 
   {
     return code;
   }
   LALDestroyRealFFTPlan(&status, &plan);
   if ( code = CheckStatus(&status, 0 , "", SZEROPADANDFFTTESTC_EFLS,
                            SZEROPADANDFFTTESTC_MSGEFLS) ) 
   {
     return code;
   }

   LALCheckMemoryLeaks();

   printf("PASS: all tests\n");

   /**************** Process User-Entered Data, If Any **************/

   if (optInputFile[0] && optOutputFile[0]){ 
     /* construct plan*/ 
     if (optMeasurePlan) 
     {
       LALMeasureFwdRealFFTPlan(&status, &plan, 2*optLength - 1);
     }
     else 
     {
       LALEstimateFwdRealFFTPlan(&status, &plan, 2*optLength - 1);
     }

     goodInput.data  = NULL;
     goodOutput.data = NULL;

     LALSCreateVector(&status, &goodInput.data, optLength);
     if ( code = CheckStatus( &status, 0 , "", SZEROPADANDFFTTESTC_EUSE,
                              SZEROPADANDFFTTESTC_MSGEUSE) ) 
     {
       return code;
     }
     LALCCreateVector(&status, &goodOutput.data, optLength);
     if ( code = CheckStatus( &status, 0 , "", SZEROPADANDFFTTESTC_EUSE,
                              SZEROPADANDFFTTESTC_MSGEUSE) ) 
     {
       return code;
     }

     /* Read input file */
     LALSReadTimeSeries(&status, &goodInput, optInputFile);
     if ( code = CheckStatus( &status, 0 , "", SZEROPADANDFFTTESTC_EUSE,
                              SZEROPADANDFFTTESTC_MSGEUSE) ) 
     {
       return code;
     }
     
     /* calculate zero-pad and FFT */
     LALSZeroPadAndFFT(&status, &goodOutput, &goodInput, plan);
     if ( code = CheckStatus( &status, 0 , "", SZEROPADANDFFTTESTC_EUSE,
                              SZEROPADANDFFTTESTC_MSGEUSE) ) 
     {
       return code;
     }

     LALCPrintFrequencySeries(&goodOutput, optOutputFile);
     
     printf("===== FFT of Zero-Padded User-Specified Data Written to File %s =====\n", optOutputFile);
     
     /* clean up valid data */
     LALSDestroyVector(&status, &goodInput.data);
     if ( code = CheckStatus( &status, 0 , "", SZEROPADANDFFTTESTC_EUSE,
                              SZEROPADANDFFTTESTC_MSGEUSE) ) 
     {
       return code;
     }
     LALCDestroyVector(&status, &goodOutput.data);
     if ( code = CheckStatus( &status, 0 , "", SZEROPADANDFFTTESTC_EUSE,
                              SZEROPADANDFFTTESTC_MSGEUSE) ) 
     {
       return code;
     }
     LALDestroyRealFFTPlan(&status, &plan);
     if ( code = CheckStatus(&status, 0 , "", SZEROPADANDFFTTESTC_EFLS,
                             SZEROPADANDFFTTESTC_MSGEFLS) ) 
     {
       return code;
     }
     LALCheckMemoryLeaks();
   }
   return SZEROPADANDFFTTESTC_ENOM;
}

/*------------------------------------------------------------------------*/

/*
 * Usage ()
 *
 * Prints a usage message for program program and exits with code exitcode.
 *
 */
static void
Usage (const char *program, int exitcode)
{
  fprintf (stderr, "Usage: %s [options]\n", program);
  fprintf (stderr, "Options:\n");
  fprintf (stderr, "  -h             print this message\n");
  fprintf (stderr, "  -q             quiet: run silently\n");
  fprintf (stderr, "  -v             verbose: print extra information\n");
  fprintf (stderr, "  -d level       set lalDebugLevel to level\n");
  fprintf (stderr, "  -i filename    read input from filename\n");
  fprintf (stderr, "  -o filename    print output to file filename\n");
  fprintf (stderr, "  -n length      length of corresponding frequency series is 2*length-1\n"); 
  fprintf (stderr, "  -m             measure plan\n");
  exit (exitcode);
}


/*
 * ParseOptions ()
 *
 * Parses the argc - 1 option strings in argv[].
 *
 */
static void
ParseOptions (int argc, char *argv[])
{
  while (1)
  {
    int c = -1;

    c = getopt (argc, argv, "hqvd:i:o:n:m");
    if (c == -1)
    {
      break;
    }

    switch (c)
    {
      case 'i': /* specify input file */
        strncpy (optInputFile, optarg, LALNameLength);
        break;

      case 'o': /* specify output file */
        strncpy (optOutputFile, optarg, LALNameLength);
        break;
        
      case 'n': /* specify number of points in series */
        optLength = atoi (optarg);
        break;
      
      case 'm': /* specify whether or not to measure plan */
        optMeasurePlan = SZEROPADANDFFT_TRUE;
        break;

      case 'd': /* set debug level */
        lalDebugLevel = atoi (optarg);
        break;

      case 'v': /* optVerbose */
        optVerbose = SZEROPADANDFFT_TRUE;
        break;

      case 'q': /* quiet: run silently (ignore error messages) */
        freopen ("/dev/null", "w", stderr);
        freopen ("/dev/null", "w", stdout);
        break;

      case 'h':
        Usage (argv[0], 0);
        break;

           
      default:
        Usage (argv[0], 1);
    }

  }

  if (optind < argc)
  {
    Usage (argv[0], 1);
  }

  return;
}
