#ifndef __KERNEL_CUH__
#define __KERNEL_CUH__

void saxpy(  int nblocks, int nthreads,float* posx, float * posy, float * posz,float* velx, float * vely, float * velz, float * accelx, float * accely, float * accelz,  float * masse );

#endif
