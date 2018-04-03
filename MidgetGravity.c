#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//________________________________________Declarations

struct Point{
	double *r;	//position in m
	double *v;	//velocity in m/s
	double *a;	//acceleration in m/s/s

	double *r0;
	double *v0;	

	double m;	//mass in kg
};

double dt = 0.00001;		//Time change
int T = 400000;		//Number of dt periods
double G = 0.1;	//Gravity constant

//________________________________________Functions

double newton( struct Point P1, struct Point P2 ){

	double Fg;
	double R;

		R = sqrt(pow(P1.r[0]-P2.r[0],2)+pow(P1.r[1]-P2.r[1],2));
		Fg = G*P1.m*P2.m/R;
		
	return Fg;
}

void move_points( struct Point P1, struct Point P2, double dt ){

	double *Fg;	//Gravity force	
	double ex,ey;	//Versors 

	Fg = malloc(sizeof(double)*2);

	ex = (0-P1.r[0])/fabsf(P1.r[0]);
	ey = (0-P1.r[1])/fabsf(P1.r[1]);

	Fg[0] = newton( P1, P2 )*cos(atan((P2.r[1]-P1.r[1])/(P2.r[0]-P1.r[0])))*ex;
	Fg[1] = newton( P1, P2 )*sin(atan((P2.r[1]-P1.r[1])/(P2.r[0]-P1.r[0])))*ey;

	P1.a[0] = P1.a[0] + Fg[0]/P1.m;
	P1.a[1] = P1.a[1] + Fg[1]/P1.m;

	ex = (0-P2.r[0])/fabsf(P2.r[0]);
	ey = (0-P2.r[1])/fabsf(P2.r[1]);

	Fg[0] = newton( P2, P1 )*cos(atan((P1.r[1]-P2.r[1])/(P1.r[0]-P2.r[0])))*ex;
	Fg[1] = newton( P2, P1 )*sin(atan((P1.r[1]-P2.r[1])/(P1.r[0]-P2.r[0])))*ey;

	P2.a[0] = P2.a[0] + Fg[0]/P2.m;
	P2.a[1] = P2.a[1] + Fg[1]/P2.m;

	P1.v[0] = P1.v[0] + P1.a[0]*dt;
	P1.v[1] = P1.v[1] + P1.a[1]*dt;
	P2.v[0] = P2.v[0] + P2.a[0]*dt;
	P2.v[1] = P2.v[1] + P2.a[1]*dt;

	P1.r[0] = P1.r[0] + P1.v[0]*dt;
	P1.r[1] = P1.r[1] + P1.v[1]*dt;
	P2.r[0] = P2.r[0] + P2.v[0]*dt;
	P2.r[1] = P2.r[1] + P2.v[1]*dt;

}



int main(){

	struct Point P1;	//First body
	struct Point P2;	//Second body

	int i;

	FILE * body1;
	FILE * body2;

	body1 = fopen("body1.txt","w");
	body2 = fopen("body2.txt","w");

//_____________________________________________________Bodies:

		P1.r = malloc( sizeof(double)*2 );
		P1.v = malloc( sizeof(double)*2 );
		P1.a = malloc( sizeof(double)*2 );
		P2.r = malloc( sizeof(double)*2 );
		P2.v = malloc( sizeof(double)*2 );
		P2.a = malloc( sizeof(double)*2 );

		P1.r0 = malloc( sizeof(double)*2 );
		P2.r0 = malloc( sizeof(double)*2 );
		P1.v0 = malloc( sizeof(double)*2 );
		P2.v0 = malloc( sizeof(double)*2 );

		P1.r0[0] = P1.r[0] = -100;
		P1.r0[1] = P1.r[1] = -100;
		P2.r0[0] = P2.r[0] = 100;
		P2.r0[1] = P2.r[1] = 100;

		P1.v0[0] = P1.v[0] = 50;
		P1.v0[1] = P1.v[1] = -50;
		P2.v0[0] = P2.v[0] = -10;
		P2.v0[1] = P2.v[1] = 10;

//		P1.v[0] = 0;
//		P1.v[1] = 0;
//		P2.v[0] = 0;
//		P2.v[1] = 0;

		P1.a[0] = 0;
		P1.a[1] = 0;
		P2.a[0] = 0;
		P2.a[1] = 0;

		P1.m = 5;
		P2.m = 1;

//_____________________________________________________Moving:

		fprintf(body1,"%lf %lf\n",P1.r[0],P1.r[1]);
		fprintf(body2,"%lf %lf\n",P2.r[0],P2.r[1]);

		for(i=0;i!=T;i++){
			move_points( P1, P2, dt );

			fprintf(body1,"%lf %lf\n",P1.r[0],P1.r[1]);
			fprintf(body2,"%lf %lf\n",P2.r[0],P2.r[1]);
		}

	fclose(body1);
	fclose(body2);

//___________________________________________________GNUPLOT

	FILE * plot;

	plot = fopen("plot.gpi","w");

		fprintf(plot,"set terminal png\n");
		fprintf(plot,"set out 'MidgetGravity.png'\n");
		fprintf(plot,"set xrange [-200:200]\n");
		fprintf(plot,"set yrange [-200:200]\n");

		fprintf(plot,"set label at %lf,%lf 'm=%.2lf' point pt 7\n",P1.r0[0],P1.r0[1],P1.m);
		fprintf(plot,"set label at %lf,%lf 'm=%.2lf' point pt 7\n",P2.r0[0],P2.r0[1],P2.m);

		fprintf(plot,"set arrow from %lf,%lf to %lf,%lf filled\n",P1.r0[0],P1.r0[1],P1.r0[0]+P1.v0[0],P1.r0[1]+1.5*P1.v0[1]);
		fprintf(plot,"set arrow from %lf,%lf to %lf,%lf filled\n",P2.r0[0],P2.r0[1],P2.r0[0]+P2.v0[0],P2.r0[1]+1.5*P2.v0[1]);

		fprintf(plot,"plot 'body1.txt' w l t '', 'body2.txt' w l t ''\n");

	fclose(plot);

	free(P1.r);
	free(P1.v);
	free(P1.a);
	free(P2.r);
	free(P2.v);
	free(P2.a);
	free(P1.r0);
	free(P2.r0);
	free(P1.v0);
	free(P2.v0);

return 0;	
}
