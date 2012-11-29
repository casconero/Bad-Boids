#ifndef _CPURULESETUP_H_
#define _CPURULESETUP_H_
#include "RulesSetup.h"
#include <kdtree.h>					// generic kdtree implementation
#include <Boid.h>					// Boid struct and operations

static const char *rulesName[]={"cohesionRule","alignmentRule","separationRule","reachRule","wanderRule","followRule"};



typedef struct kdtree Kdtree;
typedef struct kdres Kdres;

typedef void(* Action)(Kdres *, Boid *, Vector *);

Action *actions;
#endif