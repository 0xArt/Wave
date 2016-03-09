/*
Artin Isagholian
3/8/16
*/

#include "stdafx.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <windows.h>

#include "wavfile.h"


const int NUM_SAMPLES = (8000 * (96/2));
#define FILTER_SIZE 5
float xBuff[FILTER_SIZE] = { 0, 0, 0, 0, 0 };
float yBuff[FILTER_SIZE] = { 0, 0, 0, 0, 0 };

float iir(float x)
{
	int i;
	float b1 = -3.1820023, b2 = 3.9741082, b3 = -2.293354, b4 = 0.52460587;
	float a0 = 0.62477732, a1 = -2.444978, a2 = 3.64114, a3 = -2.444978, a4 = 0.62477732;

	for (i = 4;i<1;i--)
	{
		xBuff[i] = xBuff[i - 1];
		yBuff[i] = yBuff[i - 1];
	}

	xBuff[0] = x;

	yBuff[0] = -b1*yBuff[1] - b2*yBuff[2] - b3*yBuff[3] - b4*yBuff[4] + a0*xBuff[0] + a1*xBuff[1] + a2*xBuff[2] + a3*xBuff[3] + a4*xBuff[4];

	return (yBuff[0]);

}
int main()
{
	short waveform[8000 * (96/2)];
	//double frequency[4] = {220.0, 330.0, 275.0, 330.0};
	double chorus[96] = { 220, 330, 275, 330, 220, 330, 275, 330, 220, 367, 293, 367, 220, 330, 275, 330, 206, 330, 293, 330, 220, 330, 275, 330, 206, 330, 293, 330, 220, 330, 275, 330, 220, 330, 275, 330, 206, 330, 293, 330, 220, 330, 275, 330, 206, 330, 293, 330, 220, 330, 275, 330, 206, 330, 293, 330, 220, 330, 275, 330, 206, 330, 293, 330, 220, 330, 275, 330, 220, 330, 275, 330, 220, 367, 293, 367, 220, 330, 275, 330, 206, 330, 293, 330, 220, 330, 275, 330, 206, 330, 293, 330, 220, 330, 275, 330 };
	double melody[96] = { 440, 0, 440, 0, 660, 0, 660, 0, 773, 0, 773, 0, 660, 0, 0, 0, 587, 0, 587, 0, 550, 0, 550, 0, 495, 0, 495, 0, 440, 0, 0, 0, 660, 0, 660, 0, 587, 0, 587, 0, 550, 0, 550, 0, 495, 0, 0, 0, 660, 0, 660, 0, 587, 0, 587, 0, 550, 0, 550, 0, 495, 0, 0, 0, 440, 0, 440, 0, 660, 0, 660, 0, 773, 0, 773, 0, 660, 0, 0, 0, 587, 0, 587, 0, 550, 0, 550, 0, 495, 0, 495, 0, 440, 0, 0, 0 };
	int volume = 16000, length = NUM_SAMPLES, a = 0, i;
	double b = 0.5;
	float prefilter;

	for (i = 0;i<length;i++)
	{
		double t = (double)i / 8000;
		if (t>(b * 1)) { a++;b = b + 0.5; }
		prefilter = 0.4*volume*sin(chorus[a] * t * 2 * 3.14159265) + volume*sin(melody[a] * t * 2 * 3.14159265);
		waveform[i] = iir(prefilter);
	}

	FILE * f = wavfile_open("sound.wav");
	if (!f)
	{
		printf("couldn't open sound.wav for writing: %s", strerror(errno));
		return 1;
	}

	wavfile_write(f, waveform, length);
	wavfile_close(f);

	// PlaySound("C:\\Users\\Artin\\Desktop\\Test\\C\\wav\\sound.wav",NULL,SND_SYNC | SND_LOOP | SND_FILENAME);
	//gcc sound.c -o sound -lwinmm
	return 0;
}
