/**************************************************
*
*	Target.h
*	This is a master compilation target file which 
*	controls multi-target files, such as ImgLib
*	and AnalyzeUtil
*
*	c1997-2002 Mark S. Cohen, Ph.D.
*
*	At the moment, valid targets are
*		UNIX
*		MAC
*		SCANSTAT
*
***************************************************/
#pragma once

/* This defines the computer used as target */
#define UNIX	/* SCANSTAT, MAC, UNIX, ALPHA, G4, mactest */
#undef G4

#ifndef G4
#undef __ALTIVEC__
#else
#define MAC
#define __ALTIVEC__
#endif

/* This defines the handling of the log. If neither are defined, the messages are
ignored */
#define NoLog /* LogToDatedFile or LogToScreen */

#ifdef SCANSTAT
#include <files.h>
#define MAC
#define REMOTE
#undef UNIX
#undef ALPHA
#endif

#ifdef MAC
#undef UNIX
#undef ALPHA
#endif

#ifdef ALPHA
#define UNIX
#endif

#ifdef mactest
#include <sioux.h>
#include <console.h>
#define UNIX
#undef SCANSTAT
#undef MAC
#endif

#ifdef UNIX
#undef SCANSTAT
#undef MAC
#endif

