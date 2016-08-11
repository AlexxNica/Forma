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
#include <cstdio>
#include <cstdlib>
#include <chrono>

typedef std::chrono::high_resolution_clock HighResolutionClock;
typedef std::chrono::milliseconds milliseconds;

#ifndef _NTRIALS_
#define _NTRIALS_ 5
#endif

extern "C" void pyramid_mirror(float * input, float * output);

int main()
{
  int width = 512;
  int height = 512;
  
  float * input = new float[width*height];
  float * output = new float[width*height];

  for( int i = 0 ; i < height ; i++) 
    for( int j = 0 ; j < width ; j++ ){
      input[i*width+j] = (rand()%256)/ 5.0;
    }

  milliseconds time(0);

  ///Warmup
  pyramid_mirror(input,output);

  for( int i = 0 ; i < _NTRIALS_ ; i++ ){
    auto start = HighResolutionClock::now();
    pyramid_mirror(input,output);
    auto stop = HighResolutionClock::now();
    time += std::chrono::duration_cast<milliseconds>(stop-start);
  }
  printf("[FORMA] Total Time : %lf\n",(double)time.count()/_NTRIALS_);

  delete[] input;
  delete[] output;
}
