/*----------------------------------------------------------------------- 
 * 
 * File Name: ComplexFFT.c
 * 
 * Author: Creighton, J. D. E.
 * 
 * Revision: $Id$
 * 
 *----------------------------------------------------------------------- 
 * 
 * NAME 
 * ComplexFFT
 * 
 * SYNOPSIS 
 *
 *
 * DESCRIPTION 
 * Creates plans for forward and inverse complex FFTs.
 * Performs foward and inverse complex FFTs on vectors.
 * 
 * DIAGNOSTICS 
 *
 * CALLS
 * 
 * NOTES
 * 
 *-----------------------------------------------------------------------
 */

#include <config.h>

#ifdef HAVE_SFFTW_H
#include <sfftw.h>
#elif HAVE_FFTW_H
#include <fftw.h>
#else
#error "don't have either sfftw.h or fftw.h"
#endif

#include <lal/LALStdlib.h>
#include <lal/AVFactories.h>
#include <lal/ComplexFFT.h>

NRCSID( COMPLEXFFTC, "$Id$" );

/* tell FFTW to use LALMalloc and LALFree */
#define FFTWHOOKS \
  do { fftw_malloc_hook = LALMalloc; fftw_free_hook = LALFree; } while(0)


void
LALEstimateFwdComplexFFTPlan (
    LALStatus          *stat,
    ComplexFFTPlan **plan,
    UINT4            size
    )
{
  INITSTATUS( stat, "LALEstimateFwdComplexFFTPlan", COMPLEXFFTC );

  FFTWHOOKS;

  /* make sure that the argument is not NULL */
  ASSERT( plan, stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );

  /* make sure that the plan has not been previously defined */
  ASSERT( *plan == NULL, stat, COMPLEXFFT_ENNUL, COMPLEXFFT_MSGENNUL );

  /* make sure that the requested size is valid */
  ASSERT( size > 0, stat, COMPLEXFFT_ESIZE, COMPLEXFFT_MSGESIZE );

  /* allocate memory */
  *plan = (ComplexFFTPlan *) LALMalloc( sizeof( ComplexFFTPlan ) );
  ASSERT( *plan, stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );

  /* assign plan fields */
  (*plan)->size = size;
  (*plan)->sign = 1;
  (*plan)->plan = (void *)
    fftw_create_plan( size, 1, FFTW_THREADSAFE | FFTW_ESTIMATE );

  /* check that the plan is not NULL */
  if ( !(*plan)->plan )
  {
    LALFree( *plan );
    *plan = NULL;
    ABORT( stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );
  }

  /* normal exit */
  RETURN( stat );
}


void
LALEstimateInvComplexFFTPlan (
    LALStatus          *stat,
    ComplexFFTPlan **plan,
    UINT4            size
    )
{
  INITSTATUS( stat, "LALEstimateInvComplexFFTPlan", COMPLEXFFTC );

  FFTWHOOKS;

  /* make sure that the argument is not NULL */
  ASSERT( plan, stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );

  /* make sure that the plan has not been previously defined */
  ASSERT( *plan == NULL, stat, COMPLEXFFT_ENNUL, COMPLEXFFT_MSGENNUL );

  /* make sure that the requested size is valid */
  ASSERT( size > 0, stat, COMPLEXFFT_ESIZE, COMPLEXFFT_MSGESIZE );

  /* allocate memory */
  *plan = (ComplexFFTPlan *) LALMalloc( sizeof( ComplexFFTPlan ) );
  ASSERT( *plan, stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );

  /* assign plan fields */
  (*plan)->size = size;
  (*plan)->sign = -1;
  (*plan)->plan = (void *)
    fftw_create_plan( size, -1, FFTW_THREADSAFE | FFTW_ESTIMATE );

  /* check that the plan is not NULL */
  if ( !(*plan)->plan )
  {
    LALFree( *plan );
    *plan = NULL;
    ABORT( stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );
  }

  /* normal exit */
  RETURN( stat );
}


void
LALMeasureFwdComplexFFTPlan (
    LALStatus          *stat,
    ComplexFFTPlan **plan,
    UINT4            size
    )
{
  INITSTATUS( stat, "LALMeasureFwdComplexFFTPlan", COMPLEXFFTC );

  FFTWHOOKS;

  /* make sure that the argument is not NULL */
  ASSERT( plan, stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );

  /* make sure that the plan has not been previously defined */
  ASSERT( *plan == NULL, stat, COMPLEXFFT_ENNUL, COMPLEXFFT_MSGENNUL );

  /* make sure that the requested size is valid */
  ASSERT( size > 0, stat, COMPLEXFFT_ESIZE, COMPLEXFFT_MSGESIZE );

  /* allocate memory */
  *plan = (ComplexFFTPlan *) LALMalloc( sizeof( ComplexFFTPlan ) );
  ASSERT( *plan, stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );

  /* assign plan fields */
  (*plan)->size = size;
  (*plan)->sign = 1;
  (*plan)->plan = (void *)
    fftw_create_plan( size, 1, FFTW_THREADSAFE | FFTW_MEASURE );

  /* check that the plan is not NULL */
  if ( !(*plan)->plan )
  {
    LALFree( *plan );
    *plan = NULL;
    ABORT( stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );
  }

  /* normal exit */
  RETURN( stat );
}


void
LALMeasureInvComplexFFTPlan (
    LALStatus          *stat,
    ComplexFFTPlan **plan,
    UINT4            size
    )
{
  INITSTATUS( stat, "LALMeasureInvComplexFFTPlan", COMPLEXFFTC );

  FFTWHOOKS;

  /* make sure that the argument is not NULL */
  ASSERT( plan, stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );

  /* make sure that the plan has not been previously defined */
  ASSERT( *plan == NULL, stat, COMPLEXFFT_ENNUL, COMPLEXFFT_MSGENNUL );

  /* make sure that the requested size is valid */
  ASSERT( size > 0, stat, COMPLEXFFT_ESIZE, COMPLEXFFT_MSGESIZE );

  /* allocate memory */
  *plan = (ComplexFFTPlan *) LALMalloc( sizeof( ComplexFFTPlan ) );
  ASSERT( *plan, stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );

  /* assign plan fields */
  (*plan)->size = size;
  (*plan)->sign = -1;
  (*plan)->plan = (void *)
    fftw_create_plan( size, -1, FFTW_THREADSAFE | FFTW_MEASURE );

  /* check that the plan is not NULL */
  if ( !(*plan)->plan )
  {
    LALFree( *plan );
    *plan = NULL;
    ABORT( stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );
  }

  /* normal exit */
  RETURN( stat );
}


void
LALDestroyComplexFFTPlan (
    LALStatus          *stat,
    ComplexFFTPlan **plan
    )
{
  INITSTATUS (stat, "LALDestroyComplexFFTPlan", COMPLEXFFTC);

  FFTWHOOKS;

  /* make sure that the argument is not NULL */
  ASSERT( plan, stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );

  /* check that the plan is not NULL */
  ASSERT( *plan, stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );

  /* destroy plan and set to NULL pointer */
  fftw_destroy_plan( (fftw_plan)(*plan)->plan );
  LALFree( *plan );
  *plan = NULL;

  /* normal exit */
  RETURN( stat );
}


void
LALCOMPLEX8VectorFFT (
    LALStatus         *stat,
    COMPLEX8Vector *vout,
    COMPLEX8Vector *vinp,
    ComplexFFTPlan *plan
    )
{
  INITSTATUS( stat, "LALCOMPLEX8VectorFFT", COMPLEXFFTC );

  FFTWHOOKS;

  /* make sure that the arguments are not NULL */
  ASSERT( vout, stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );
  ASSERT( vinp, stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );
  ASSERT( plan, stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );

  /* make sure that the data exists */
  ASSERT( vout->data, stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );
  ASSERT( vinp->data, stat, COMPLEXFFT_ENULL, COMPLEXFFT_MSGENULL );

  /* make sure that it is not the same data! */
  ASSERT( vout->data != vinp->data, stat,
          COMPLEXFFT_ESAME, COMPLEXFFT_MSGESAME );

  /* make sure that the lengths agree */
  ASSERT( plan->size > 0, stat, COMPLEXFFT_ESIZE, COMPLEXFFT_MSGESIZE );
  ASSERT( vout->length == plan->size, stat,
          COMPLEXFFT_ESZMM, COMPLEXFFT_MSGESZMM );
  ASSERT( vinp->length == plan->size, stat,
          COMPLEXFFT_ESZMM, COMPLEXFFT_MSGESZMM );

  fftw_one(
      (fftw_plan) plan->plan,
      (fftw_complex *) vinp->data,
      (fftw_complex *) vout->data
      );

  /* normal exit */
  RETURN( stat );
}

