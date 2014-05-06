/*
 * cfc11.c
 *
 *  Created on: May 2, 2014
 *      Author: ashao
 */
#include "init.h"
#include "alloc.h"
#include "read.h"
#include <math.h>
#include "cfcs_sf6.h"
#include <stdlib.h>
#include <stdio.h>
#include <netcdf.h>
#include <string.h>

double ***mn_cfc11;
double ***cfc11_init;
double **cfc11_sat;
double **cfc11_atmconc;
double **mn_cfc11sat;

int mCFC11;
extern double ****tr;

	struct tracer_boundary atmconc[NUMTRANSIENT];

void allocate_cfc11 ( ) {

	mn_cfc11 = alloc3d(NZ,NXMEM,NYMEM);
	cfc11_init = alloc3d(NZ,NXMEM,NYMEM);
	cfc11_sat = alloc2d(NXMEM,NYMEM);
	mn_cfc11sat = alloc2d(NXMEM,NYMEM);
	cfc11_atmconc = alloc2d(NXMEM,NYMEM);

}

void read_tracer_boundary ( ) {


	int err, i, j, cdfid, timeid;
	int status, varid;
	char infile[25], inpath[200];
	FILE *file;
	long  start[MAX_NC_VARS];
	long  end[MAX_NC_VARS];
	char varname[20];
	extern char directory[75];
	const int numatm = NUMATMVALS;
	// Allocate all the vectors insdie of atmconc
	for (i=0;i<NUMTRANSIENT;i++) {
		atmconc[i].ntime = NUMATMVALS;
		atmconc[i].nval = (double *) malloc(numatm * sizeof(double));
		atmconc[i].sval = (double *) malloc(numatm * sizeof(double));
		atmconc[i].time = (double *) malloc(numatm * sizeof(double));
	}

    sprintf(infile,"cfc_sf6_bc.nc");
    strcpy(inpath, directory);
    strcat(inpath, infile);
    printf("\nLooking for file '%s'.\n",inpath);
    err = open_input_file(inpath,&file,&cdfid,&timeid);

    start[0] = 0;
    end[0] = NUMATMVALS;

    status = nc_inq_varid(cdfid, "Year", &varid);
printf("mCFC11: %d, mCFC12: %d, mSF6: %d\n",mCFC11,mCFC12,mSF6);
    for (i=0;i<NUMTRANSIENT;i++) // Put the year into all the
    	status = nc_get_vara_double(cdfid, varid, start, end,atmconc[i].time);

	strcpy(varname,"CFC11NH");
    status = nc_inq_varid(cdfid, varname, &varid);
    status = nc_get_vara_double(cdfid, varid, start, end,atmconc[mCFC11].nval);
	strcpy(varname,"CFC11SH");
    status = nc_inq_varid(cdfid, varname, &varid);
    status = nc_get_vara_double(cdfid, varid, start, end,atmconc[mCFC11].sval);

	strcpy(varname,"CFC12NH");
    status = nc_inq_varid(cdfid, varname, &varid);
    status = nc_get_vara_double(cdfid, varid, start, end,atmconc[mCFC12].nval);
	strcpy(varname,"CFC12SH");
    status = nc_inq_varid(cdfid, varname, &varid);
    status = nc_get_vara_double(cdfid, varid, start, end,atmconc[mCFC12].sval);

	strcpy(varname,"SF6NH");
    status = nc_inq_varid(cdfid, varname, &varid);
    status = nc_get_vara_double(cdfid, varid, start, end,atmconc[mSF6].nval);
	strcpy(varname,"SF6SH");
    status = nc_inq_varid(cdfid, varname, &varid);
    status = nc_get_vara_double(cdfid, varid, start, end,atmconc[mSF6].sval);
	
	printf("status=%d\n",status);
    close_file(&cdfid,&file);
}

void initialize_cfc11 ( ) {
	int i, j, k;
	mCFC11 = mCFC11;
	extern char restart_filename[200];

#ifdef RESTART
	printf("Initializing CFC-11 from restart %s\n",restart_filename);
	read_var3d( restart_filename, "mn_cfc11", 0, cfc11_init);
#else
	set_darray3d_zero(cfc11_init,NZ,NXMEM,NYMEM);
#endif

	for(i=0;i<NXMEM;i++)
		for(j=0;j<NYMEM;j++)
			for(k=0;k<NZ;k++)
				tr[mCFC11][k][i][j] = cfc11_init[k][i][j];




	free3d(cfc11_init,NZ);
}

void cfc11_saturation( double **sat ) {

	const double solcoeffs[7] = {-229.9261,319.6552,119.4471,-1.39165,-0.142382,0.091459,-0.0157274};
	int i, j;
	double work;
	double TempK;
	extern double ***Temptm, ***Salttm;

	for (i=0;i<NXMEM;i++)
		for (j=0;j<NYMEM;j++)	{
					TempK = Temptm[0][i][j]+273.15;
					work = solcoeffs[0] + solcoeffs[1]*(100/TempK) +
						solcoeffs[2]*log( TempK/100 ) + solcoeffs[3]*pow(TempK/100,2) +
						Salttm[0][i][j]*(solcoeffs[4]+solcoeffs[5]*(TempK/100)+solcoeffs[6]*pow(TempK/100,2));
					sat[i][j] = exp(work)*cfc11_atmconc[i][j];
			}



}

void cfc11_find_atmconc(  ) {

	int i,j;
	extern double **geolat;
	extern double **atmpres;
	const double equatorbound[2] = {10,-10}; // Set the latitudes where to start interpolating atmospheric concentrations
	extern double currtime;
	double hemisphere_concentrations[2];
/*
	*nval = atmconc[mCFC11].nval;
	*sval = atmconc[mCFC11].sval;
	ntime = atmconc[mCFC11].ntime;
	*time = atmconc[mCFC11].time;
	hemisphere_concentrations[0] = lin_interp(currtime,nval,time,0,ntime);
	hemisphere_concentrations[1] = lin_interp(currtime,sval,time,0,ntime);
*/
	// Interpolate in time to find the atmospheric concentration

	printf("atmconc[mCFC11].time=%f\n",atmconc[mCFC11].time[0]);
	hemisphere_concentrations[0] = lin_interpp(currtime,
			atmconc[mCFC11].nval,atmconc[mCFC11].time,0,atmconc[mCFC11].ntime);
	hemisphere_concentrations[1] = lin_interpp(currtime,
			atmconc[mCFC11].sval,atmconc[mCFC11].time,0,atmconc[mCFC11].ntime);

	for (i=0;i<NXMEM;i++)
		for (j=0;j<NYMEM;j++) {
			if (geolat[i][j] < equatorbound[0] && geolat[i][j] > equatorbound[1]) {
				cfc11_atmconc[i][j] = lin_interpp(geolat[i][j],
						equatorbound,hemisphere_concentrations,0,2);
			}
			if (geolat[i][j]>equatorbound[0] ) {
				cfc11_atmconc[i][j] = hemisphere_concentrations[0];
			}
			if (geolat[i][j]<-equatorbound[1] ) {
				cfc11_atmconc[i][j] = hemisphere_concentrations[1];
			}
		}

}

void surface_cfc11( ) {

	int i,j,k;
	printf("Setting CFC-11 surface condition\n");
	// Set oxygen values to saturation at the mixed layer to mimic equilibrium with the atmosphere
	extern double ***Temptm;
	extern double ***Salttm;
	cfc11_find_atmconc( );
	cfc11_saturation( cfc11_sat );
	for (k=0;k<NML;k++)
		for (i=0;i<NXMEM;i++)
			for (j=0;j<NYMEM;j++)
					tr[mCFC11][k][i][j]=cfc11_sat[i][j];

}

