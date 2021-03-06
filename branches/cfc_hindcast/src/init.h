/********+*********+*********+*********+*********+*********+*********+*
 *    This header file determies many of the adjustable parameters    *
 ********+*********+*********+*********+*********+*********+*********+*/

/*  Specify the physical domain.                                      */


#define RE 6.378e6             /*    The radius of the earth in m.    */

#define GFS 9.8                /*    The reduced gravity at the free  */
                               /*  surface, in m s-2.                 */
#define DMAX 6500.0            /*    The maximum depth of the basin,  */
                               /*  in m.  This is required for        */
                               /*  initanal.c to give valid advice.   */
#define MINIMUM_DEPTH 0.5     /*    The minimum ocean depth, in m.   */
                               /*  Anything shallower than this depth */
                               /*  is assumed to be on land, and all  */
                               /*  appropriate fluxes are masked out. */
#define GINT .1177             /*    The reduced gravity of the       */
                               /*  internal interfaces, in m s-2.     */
                               /*  GINT may be superceded in          */
                               /*  initialize.c, but GINT should be   */
                               /*  the average of all interface       */
                               /*  reduced gravities for initanal.c   */
                               /*  to give appropriate advice.        */
#define LENLAT   166.5         /*    The length of the basin in       */
#define LENLON   360.0         /*  degrees of latitude and longitude. */
#define LOWLAT  (-77.5)        /*  The southern latitude of the basin.*/
#define WESTLON (-280)         /*  The western longitude of the basin.*/
#define REENTRANT              /*    If the basin is zonally reentrant*/
                               /*  this should be '#define REENTRANT'.*/
                               /*  Otherwise, use '#undef REENTRANT'. */
#define REENTRANT_Y            /* meridionally reentrant              */

//#define HTEST			/*  use htest[] rather than h[]       */
				/*  for output of mn_h[]              */

/* Specify properties of the passive tracers                          */

#define NUM_ADV_ITER 30         /* number of iterations for advection */

#define MERGED_ML                /* merge the first and second two     */
                               /* layers for all BGC variables       */

#undef  AGE                    /*  Define ideal age tracer in years.  */

#define TRNCINIT	       /* read in initial tracers values from */
			       /* netCDF files -- otherwise initialize*/
			       /* analytically 			      */

#define NTR 3                 /*  The number of tracers to carry.    */
                               /*  Must add up to total of AGE,       */
                               /*    OXYGEN, O18, (CFC11 + CFC12),    */
                               /*    (DOP + PHOSPHATE), (DIC + ALK)   */
                               /*    (NO3 + DON)                      */
                               /*    (15NO3 + DO15N), SF6
				/* CFC11_sat, CFC12_sat, SF6_sar ashao    */

#define NOVARS 16              /*  Number of variables used in        */
                               /*    vardesc structure for output.    */
                               /*    Moved here from offtrac.c        */
                               /*  25OCT07 BX ashao                   */

#define BEGYEAR 1936			/* ashao: Set the start year, for
 	 	 	 	 	 	 	 	 tracers with atmospheric histories */

#define HINDCAST			/* Expect to read in hindcasat fields */
#define BEGHIND 1948        /* The first year of hindcast fields */
#define ENDHIND 2007 		/* Last year of hindcast fields */

#define SEPFILES              /* For use with separate input files for    */
                              /* U,V,W,H                                */
                              /* Files are assumed to be shifted by one */
                              /* month, i.e. uvw leads h                */

#define NMONTHS 12           /* Number of months in forcing field    */
                             /* before repetition. E.g., for 10 year  */
                             /* forcing file use 120, 564 for 47yrs */

#define WRINT 1                /*  Number of months between writes AND */
                               /*   the mean interval for each write   */

#define NTSTEP 1                /*  Number of time steps between       */ 
                               /*  reading of forcing fields          */
                               /*  Default is 1--Updated 29JAN10 BX   */
								/* ashao: This does nothing really now
								 * to decrease the time stamp, need to
								 * figure out what this was supposed to do
								 */

#undef WRTTS                  /* Write output after each sub time step */
                              /* for debugging - 04AUG08 BX           */

#ifdef VARIAB_FORC

# define TS_VARIAB             /* Specify if T and S are read from 
				  variable time-series. If undefined
				  read one year climatology.         */
# define ICE_VARIAB             /* If undefined read one year          */
                               /* climatology--not implemented yet    */
#endif

/* Specify if wd is read in, otherwise we read in ea, eb, eaml        */
#undef ENTRAIN                 /* Define how we get diapycnal Velocity*/
                               /* If ENTRAIN is defined, must read in */
                               /* ea, eb, and eaml. Otherwise we must */
                               /* read in wd.                         */

#ifndef VARIAB_FORC
# define SMOOTH_REST            /* save h to restart file and use it to */
                              /* interpolate at beginning of restart to */
                              /* avoid shock by imperfect wrap arounds  */
#endif

/*  Specify the numerical domain.                                     */

#define XMETRIC_J              /*    Define XMETRIC_J if the x-direc- */
#define XMETRIC_I              /*  tion metrics vary in the y- (or j^)*/
#define YMETRIC_J              /*  direction.  Otherwise undefine     */
#define YMETRIC_I              /*  XMETRIC_J.  XMETRIC_I, YMETRIC_J,  */
                               /*  and YMETRIC_I are used similarly.  */
                               /*  CARTESIAN overrides all of these   */
                               /*  choices.                           */
                               /*    For example, on a regular lat-   */
                               /*  itude longitude grid, define       */
                               /*  XMETRIC_J and undefine the rest.   */
                               /*  For a Mercator grid, define        */
                               /*  only XMETRIC_J and YMETRIC_J.      */


#define NXTOT   360            /*    NXTOT and NYTOT are the numbers  */
#define NYTOT   210	       /*  thickness grid points in the zonal */
                               /*  and meridional directions of the   */
                               /*  physical domain.                   */
#define NZ  49                 /*    The number of layers.            */

#ifdef SERIAL_IO_CODE          /*    SERIAL_IO_CODE is specified on   */
                               /*  the compile line of postprocessing */
                               /*  code.                              */
# define NETCDF_OUTPUT          /*    Do not change this line.         */
#else
# undef   MPI_PARALLEL          /*    If MPI_PARALLEL is defined, the  */
                               /*  parallelization is done using the  */
                               /*  MPI subroutines.  Otherwise SHMEM  */
                               /*  subroutines are used.  This option */
                               /*  affects subroutines HIM_par.c and  */
                               /*  HIM_par_IO.c.                      */
# undef   PARALLEL_X            /*    Define PARALLEL_X if there is to */
                               /*  be domain decomposition in the X-  */
                               /*  direction.                         */
# undef   PARALLEL_Y            /*    Define PARALLEL_Y if there is to */
                               /*  be domain decomposition in the Y-  */
                               /*  direction.                         */
# undef   PARALLEL_IO           /*  With PARALLEL_IO and NETCDF_OUTPUT */
                               /*  defined, each processor writes out */
                               /*  /ts own NetCDF file.  These files  */
                               /*  can be combined using the utility  */
                               /*  mppnccombine.                      */
# define  NETCDF_OUTPUT         /*    If NETCDF_OUTPUT is defined, all */
                               /*  output files are in NetCDF format  */
                               /*  and all input files are examined   */
                               /*  to determine whether they are      */
                               /*  in NetCDF format or in unformatted */
                               /*  C binary.                          */
#endif

#ifdef PARALLEL_X
#define NXPROC 2               /*    NXPROC is the minimum number of  */
                               /*  processors in the x-direction.     */
#endif
#ifdef PARALLEL_Y
#define NYPROC 4               /*    NYPROC is the minimum number of  */
                               /*  processors in the y-direction.     */
                               /*  The minimum total number of        */
                               /*  processors is NXPROC*NYPROC.       */
#endif
#define MAXPROC 64             /*    MAXPROC is the maximum number of */
                               /*  processors that might be used      */
                               /*  without recompiling.  MAXPROC must */
                               /*  exceed NXPROC*NYPROC or NXPROC or  */
                               /*  NYPROC, or be undefined.           */

#undef  CHECKPARALLEL          /*    If CHECKPARALLEL is defined, it  */
                               /*  causes the model to run simultan-  */
                               /*  eously on one and several process- */
                               /*  ors.  The subroutine check_field   */
                               /*  can then be called to compare the  */
                               /*  fields in the serial and parallel  */
                               /*  simulations, reporting any diff-   */
                               /*  erences.                           */

#define SAVEINT    5.0         /*    The number of days between saves.*/
                               /* I believe currently NOT used: Suzanne */
#define DT 3600.0              /*    Time step in seconds.            */
                             /* Standard case is one month = 3600.0  */
#define EPSILON 1.0e-10        /*   The minimum layer thickness, in m.*/


/*  The following lines should not be modified.                       */
#define X0 1                   /*    X0 is the offset of the physical */
                               /*  domain from the memory domain.     */
#define X1 (X0+1)              /*    X1 is the lowest index of the    */
                               /*  physical domain (as opposed to the */
                               /*  memory halo) on each processor.    */
#define Y0 1
#define Y1 (Y0+1)
#ifdef PARALLEL_X
# ifdef CHECKPARALLEL
# define NXMEM (NXTOT+2*(X0+1))
# else
# define NXMEM (((NXTOT-1)/NXPROC)+1+2*(X0+1)) /* NXMEM is the maximum    */
                               /*  number of grid points in the       */
                               /*  x-direction on each processor.     */
# endif
#else
# define NXMEM (NXTOT+2*(X0+1))
# define nx (NXTOT+X0)         /*   nx is the maximum index of the    */
                               /*  domain on each processor if there  */
                               /*  is no parallelization in x.        */
#endif                         /* PARALLEL_X */
#ifdef PARALLEL_Y
# ifdef CHECKPARALLEL
# define NYMEM (NYTOT+2*(Y0+1))
# else
# define NYMEM (((NYTOT-1)/NYPROC)+1+2*(Y0+1)) /* NYMEM is the maximum    */
                               /*  number of grid points in the       */
                               /*  x-direction on each processor.     */
# endif
#else
# define NYMEM (NYTOT+2*(Y0+1))
# define ny (NYTOT+Y0)         /*   nx is the maximum index of the    */
                               /*  domain on each processor if there  */
                               /*  is no parallelization in x.        */
#endif                         /* PARALLEL_Y */

#undef ISOTROPIC               


/* misc other stuff for tracadv.c */

#define HMIX 5.0		/* was 5.0		*/


/* default values for horizontal and vertical diffusion: */

#define KDML 0.0
#define KD   0.0
//#define KHTR 5.0e2           /* define the coef. of horiz. diffusion */
#define KHTR 1.0e3           /* define the coef. of horiz. diffusion */

/* end of default values */

/* alternate values for horizontal and vertical diffusion: */
//#define KHTR 2.0e3           /* define the coef. of horiz. diffusion */
//#define KHTR 0.0

#ifdef DIFFUSE_TRACER
#undef KDML
#undef KD
#define KDML 1.00e-4		/* was 1.00e-4		*/
#define KD   1.00e-5		/* was 1.00e-5		*/
#endif

#define NML 1

#define MDT 4800
//HF
#ifndef RESTART
# undef SMOOTH_REST
#endif

#define NZWOA 33

#undef RESTART

#define CFCS
#define NMONTHSTTD 12
