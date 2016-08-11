//****************************************************************************//
//* Copyright (c) 2016, NVIDIA CORPORATION. All rights reserved.             *//
//*                                                                          *//
//* Redistribution and use in source and binary forms, with or without       *//
//* modification, are permitted provided that the following conditions       *//
//* are met:                                                                 *//
//*  * Redistributions of source code must retain the above copyright        *//
//*    notice, this list of conditions and the following disclaimer.         *//
//*  * Redistributions in binary form must reproduce the above copyright     *//
//*    notice, this list of conditions and the following disclaimer in the   *//
//*    documentation and/or other materials provided with the distribution.  *//
//*  * Neither the name of NVIDIA CORPORATION nor the names of its           *//
//*    contributors may be used to endorse or promote products derived       *//
//*    from this software without specific prior written permission.         *//
//*                                                                          *//
//* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY     *//
//* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE        *//
//* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR       *//
//* PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR        *//
//* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,    *//
//* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,      *//
//* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR       *//
//* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY      *//
//* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT             *//
//* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE    *//
//* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.     *//
//****************************************************************************//
/*
 *  Copyright 2014 NVIDIA Corporation.  All rights reserved.
 *
 *  NOTICE TO USER: The source code, and related code and software
 *  ("Code"), is copyrighted under U.S. and international laws.
 *
 *  NVIDIA Corporation owns the copyright and any patents issued or
 *  pending for the Code.
 *
 *  NVIDIA CORPORATION MAKES NO REPRESENTATION ABOUT THE SUITABILITY
 *  OF THIS CODE FOR ANY PURPOSE.  IT IS PROVIDED "AS-IS" WITHOUT EXPRESS
 *  OR IMPLIED WARRANTY OF ANY KIND.  NVIDIA CORPORATION DISCLAIMS ALL
 *  WARRANTIES WITH REGARD TO THE CODE, INCLUDING NON-INFRINGEMENT, AND
 *  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE.  IN NO EVENT SHALL NVIDIA CORPORATION BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 *  WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATED TO THE USE OR
 *  PERFORMANCE OF THE CODE, INCLUDING, BUT NOT LIMITED TO, INFRINGEMENT,
 *  LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 *  NEGLIGENCE OR OTHER TORTIOUS ACTION, AND WHETHER OR NOT THE
 *  POSSIBILITY OF SUCH DAMAGES WERE KNOWN OR MADE KNOWN TO NVIDIA
 *  CORPORATION.
 */
#include <cassert>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>

#define N 5
#define M 5

extern "C"
void offset(float*, int, int, float*);

// extern "C"
// void *__formart_alloc(size_t size) {
//   return new char[size];
// }

void ref_output(float (*input)[N], float (*output)[M+3][N+5])
{
  for( int i = 0 ; i < M ; i++ )
    for( int j = 0 ; j < N ; j++ )
      output[1][i+3][j+5] = input[i][j];
}


int main() {
  float (*inp1)[N] = (float (*)[N] )new float[M*N];
  float (*outp)[M+3][N+5] = (float(*)[M+3][N+5]) new float[2*(M+3)*(N+5)];
  float (*outp_ref)[M+3][N+5] = (float (*)[M+3][N+5]) new float[2*(M+3)*(N+5)];

  for( int i = 0 ; i < M ; i++ )
    for( int j = 0 ; j < N ; j++ )
      inp1[i][j] = (float)rand() / (float)RAND_MAX;
  
  for( int k = 0 ; k < 2 ; k++ )
    for( int i = 0 ; i < (M+3) ; i++ )
      for( int j = 0 ; j < (N+5) ; j++ ){
	outp[k][i][j] = 0.0;
	outp_ref[k][i][j] = 0.0;
      }

  offset((float*)inp1,M,N,(float*)outp);
  ref_output(inp1,outp_ref);
  
  printf("Input[%d,%d] :\n",M,N);
  for( int i = 0 ; i < M ; i++ ){
    for( int j = 0 ; j < N ; j++ )
      printf(" %lf",inp1[i][j]);
    printf("\n");
  }
  printf("Output[2,%d,%d] :\n",M+3,N+5);
  for( int i = 0 ; i < (M+3) ; i++ ){
    for( int j = 0 ; j < (N+5) ; j++ )
      printf(" %lf",outp[1][i][j]);
    printf("\n");
  }
  printf("OutputRef[2,%d,%d] :\n",M+3,N+5);
  for( int i = 0 ; i < (M+3) ; i++ ){
    for( int j = 0 ; j < (N+5) ; j++ )
      printf(" %lf",outp_ref[1][i][j]);
    printf("\n");
  }

  double diff = 0.0;
  for( int k = 0 ; k < 2 ; k++ )
    for( int i = 0 ; i < M+3 ; i++) 
      for( int j = 0 ; j < N+5 ; j++ )
	diff += fabs(outp_ref[k][i][j] - outp[k][i][j]);
  printf("Diff : %lf\n",diff);
  if( diff != 0.0) {
    printf("Incorrect Result\n");
    exit(1);
  }

  delete[] inp1;
  delete[] outp;
  delete[] outp_ref;
}
