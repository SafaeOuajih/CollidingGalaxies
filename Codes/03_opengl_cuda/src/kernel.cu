#include "cuda.h"

__global__ void kernel_acc( float* posx, float * posy, float * posz,float *velx, float *vely, float * velz, float * accelx, float * accely, float * accelz,  float * masse ) {
	int h = 0, M = 10;
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int k = i;
	int size2 =1024;
	float3 acc;
	__shared__ float m[1024],px[1024],py[1024],pz[1024] ;

	//for ( j = 0; j < size2; j++) {
if( i < size2){
	//float px = posx[i];
	//float py = posy[i];
	//float pz = posz[i];
	m[i]  = masse[i];
	px[i] = posx[i];
	py[i] = posy[i];
	pz[i] = posz[i];
	acc.x = 0.0;
	acc.y = 0.0;
	acc.z = 0.0;
		//accelx [i]  =0;
		//accely [i]  =0;	
		//accelz [i]  =0;
	//}
			//int ax = 0;
			//int ay = 0;
			//int az = 0;
	//for(k=0; k<size2; k++){
	    for(h=0;h<size2 ; h++){
		//if ( h != i ) {
			float mulPosX = px[h]-px[i];
			float mulPosY = py[h]-py[i];			
			float mulPosZ = pz[h]-pz[i];
			float d = sqrtf(mulPosX*mulPosX + mulPosY*mulPosY + mulPosZ*mulPosZ);
			if ( d < 1.0 ) d = 1.0;
			float l = m[h]* M * (1/(d*d*d));
			acc.x += mulPosX*l;
			acc.y += mulPosY*l;	
			acc.z += mulPosZ*l;

		//}
            }

	//accelx[i]= accx; 
	//accely[i]= accy; 
	//accelz[i]= accz;

	//posx[i]= 0.0;

	velx[i]+=acc.x;
	vely[i]+=acc.y;
	velz[i]+=acc.z;

        posx[i]+=velx[i]*0.1;
        posy[i]+=vely[i]*0.1;
        posz[i]+=velz[i]*0.1;

}


/*
	int k = i;

	if( k< size2){
			velx[k]+=accelx [k];
			vely[k]+=accely [k];
			velz[k]+=accelz [k];
		        posx[k]+=velx[k]*0.1;
		        posy[k]+=vely[k]*0.1;
		        posz[k]+=velz[k]*0.1;
	}
*/

}



/*
__global__ void kernel_pos(float* posx, float * posy, float * posz,float* velx, float * vely, float * velz, float * accelx, float * accely, float * accelz ) {


int size2 =1024;
//		for(k=0; k<size2; k=k+2){
	int k = blockIdx.x * blockDim.x + threadIdx.x;

	if( k< size2){

			velx[k]+=accelx [k];
			vely[k]+=accely [k];
			velz[k]+=accelz [k];
		        posx[k]+=velx[k]*0.1;
		        posy[k]+=vely[k]*0.1;
		        posz[k]+=velz[k]*0.1;

int l = 0;
	}


		//}
}
*/


void saxpy( int nblocks, int nthreads,float* posx, float * posy, float * posz,float* velx, float * vely, float * velz, float * accelx, float * accely, float * accelz,  float * masse ) {
	kernel_acc<<<nblocks, nthreads>>>(  posx, posy, posz,velx, vely,  velz, accelx, accely, accelz,   masse );
	//kernel_pos<<<nblocks, nthreads>>>(  posx,  posy, posz, velx, vely,  velz,  accelx, accely, accelz );
}
