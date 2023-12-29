#pragma once
#include "../Structures/structs.h"

/*  cant be bothered to do w2s and bone rotation and other calculations so 
just replace it with one on github or smt  */

Vector3 GetBoneWithRotation(uintptr_t mesh, int bone_id)
{
	float matrixX, matrixY, matrixZ;
	return Vector3(matrixX, matrixY, matrixZ);
}

Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
{
	float x, y, z;
	return Vector3(x, y, z);
}