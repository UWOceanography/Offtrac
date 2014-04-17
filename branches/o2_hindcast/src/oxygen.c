#include "oxygen.h"
extern int mOXYGEN;
#include "init.h"
#include "phosphate.h"
#include "netcdf.h"

/* OXYGEN VARIABLE DECLARATIONS */
// Auxiliary variables
extern int mOXYGEN;
// Output arrays
double ***mn_oxygen;
double ***mn_o2sat;
double ***mn_jo2;
double mn_oxyflux[NXMEM][NYMEM];
// Working arrays
double ***oxy_init;
double o2_sat[NZ][NXMEM][NYMEM];
double oxyflux[NXMEM][NYMEM];
double jo2[NZ][NXMEM][NYMEM];

void initialize_oxygen ( int imon ) {
	extern double ****tr;
	extern double D[NXMEM[NYMEM];
	extern double hstart[NZ][NXMEM][NYMEM];
	extern char restart_filename[200];

	int i, j, k;

	// Set index in tracer array
	mOXYGEN = 0;

	// Allocate working and output arrays
	mn_oxygen = alloc3d(NZ,NXMEM,NYMEM);
	mn_jo2 = alloc3d(NZ,NXMEM,NYMEM);
	mn_o2sat = alloc3d(NZ,NXMEM,NYMEM);
	oxy_init = alloc3d(NZ,NXMEM,NYMEM);

	// Determine how to set the initial distribution of oxygen
#ifdef WOA_OXY
	printf("Initializing oxygen from WOA09\n");
	read_woa_file(imon, hstart, oxy_init, "woa09.o2.nc", "o_an");
#endif

#ifdef RESTART
	printf("Initializing oxygen from restart %s\n",restart_filename);
	read_var3d( restart_filename, "mn_oxygen", imon, double oxy_init)
#endif

	// Copy the initialized tracer value over to main trace array
	for (i=0;i<=NXMEM-1;i++) {
		for (j=0;j<=NYMEM-1;j++) {
			if (D[i][j]>MINIMUM_DEPTH) {
				for (k=0;k<=NZ-1;k++) {
					tr[mOXYGEN][k][i][j] = oxy_init[k][i][j];
				}
			} else {
				for (k=0;k<=NZ-1;k++)
					tr[mOXYGEN][k][i][j] = misval;
			}
		}
	}

	free3d(oxy_init);

}

void oxygen_saturation(double T[NZ][NXMEM][NYMEM], double S[NZ][NXMEM][NYMEM],
		double o2_sat[NZ][NXMEM][NYMEM]) {
	/* compute oxygen saturation concentrations in the mixed layer in ml/l
	 * using Garcia and Gordon 1992 (L&O,1307) and convert to mol/m^3.
	 * Based on Matlab code by Allison Shaw and Steve Emerson.
	 * Ivan Lima (Nov 2002) */

	int i, j, k;
	double TS[NZ][NXMEM][NYMEM], o2s_ml[NZ][NXMEM][NYMEM];
	double work[NZ][NXMEM][NYMEM]; /* temporary work array */
	double molvolo2 = 22.3916; /* molar volumes (liters/mole) */
	double A0, A1, A2, A3, A4, A5, B0, B1, B2, B3, C0; /* constants */

	A0 = 2.00907;
	A1 = 3.22014;
	A2 = 4.05010;
	A3 = 4.94457;
	A4 = -.256847;
	A5 = 3.88767;
	B0 = -.00624523;
	B1 = -.00737614;
	B2 = -.0103410;
	B3 = -.00817083;
	C0 = -4.88682E-07;

	for (k = 0; k < NZ; k++) {
		for (i = X1; i <= nx; i++) {
			for (j = Y1; j <= ny; j++) {
				// HF this was set to 298.15, but T is in C, not K here!
				// HF the Garcia and Gordon paper uses t=40C as upper bound
				if (T[k][i][j] > 40.0) {
					printf(
							"WARNING: TEMPERATURE OUT OF RANGE in oxygen saturation: %g \n",
							T[k][i][j]);
					printf(
							"  setting Temp to 40 deg C at array position %i,%i,%i \n",
							i, j, k);
					T[k][i][j] = 40.0;
				}
				TS[k][i][j]
						= log((298.15 - T[k][i][j]) / (273.15 + T[k][i][j]));
				work[k][i][j] = A0 + A1 * TS[k][i][j] + A2 * pow(TS[k][i][j],
						2.) + A3 * pow(TS[k][i][j], 3.) + A4 * pow(TS[k][i][j],
						4.) + A5 * pow(TS[k][i][j], 5.) + S[k][i][j] * (B0 + B1
						* TS[k][i][j] + B2 * pow(TS[k][i][j], 2.) + B3 * pow(
						TS[k][i][j], 3.)) + C0 * pow(S[k][i][j], 2.);
				o2s_ml[k][i][j] = exp(work[k][i][j]);
				o2_sat[k][i][j] = o2s_ml[k][i][j] / molvolo2;
			}
		}
	}
}

void apply_oxygen_jterms( ) {
	int i,j,k;
	extern double dt;

	// j terms here are calculated from biotic_sms routine in biotic.c
	for (i = 0; i <= NXMEM - 1; i++) {
		for (j = 0; j <= NYMEM - 1; j++) {
			//BX - reinstated by HF
			if (D[i][j]>MINIMUM_DEPTH) {
				for (k = 0; k < NZ; k++) {
					tr[mOXYGEN][k][i][j] += dt * jo2[k][i][j];
				}
			} else {
				tr[mOXYGEN][k][i][j] = misval;
			}
		}
	}


}

void surface_oxygen( ) {
	int i,j,k;

	// Set oxygen values to saturation at the mixed layer to mimic equilibrium with the atmosphere
	extern double Temptm[NZ][NXMEM][NYMEM];
	extern double Salttm[NZ][NXMEM][NYMEM];
	oxygen_saturation(Temptm, Salttm, o2_sat);
	for (k=0;k<NML;k++)
		for (i=0;i<NXMEM;i++)
			for (j=0;j<NYMEM;j++)
				tr[mOXYGEN][k][i][j]=o2_sat[k][i][j];
}