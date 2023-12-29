#include <iostream>
#include <windows.h>
#include "../Kernal/Communications/driver.h";
#include "../Cheat/Defs/Globals/gloabals.h"
#include "../Cheat/Defs/Offsets/offsets.h"
#include "Defs/Structures/structs.h"
#include "Defs/CalculationFuncs/calcs.h"

// ToDo: finish stuff in calcs.h, add overlay (dx9), and defined draw funcs like DrawBox, DrawLines or wtv u want. this is not 
// finshed yet i just wanted to upload it and yh i will updaze and finish it asap. make sure to start my github!!!

int main()
{
    mem::find_driver(); // finds the driver that we created

    // Getting base addy
    mem::process_id = mem::find_process(L"FortniteClient-Win64-Shipping.exe");
    uintptr_t baseAddress = mem::find_image();
    printf("\n [ base_address ] -> %p", (void*)mem::find_image());
    
    // Reading
    mem::read_physical(reinterpret_cast<PVOID>(baseAddress + offsets::UWorld), &uWorldValue, sizeof(DWORD));
    std::cout << " [ uworld_value ] -> " << uWorldValue << std::endl;
    mem::read_physical(reinterpret_cast<PVOID>(baseAddress + offsets::Gameinstance), &Gameinstance, sizeof(DWORD_PTR));
    mem::read_physical(reinterpret_cast<PVOID>(Gameinstance + offsets::LocalPlayers), &LocalPlayers, sizeof(DWORD_PTR));
    mem::read_physical(reinterpret_cast<PVOID>(LocalPlayers), &Localplayer, sizeof(DWORD_PTR));
    mem::read_physical(reinterpret_cast<PVOID>(Localplayer + offsets::PlayerController), &PlayerController, sizeof(DWORD_PTR));
    mem::read_physical(reinterpret_cast<PVOID>(PlayerController + offsets::LocalPawn), &LocalPawn, sizeof(DWORD_PTR));
    mem::read_physical(reinterpret_cast<PVOID>(PlayerController + 0x338), &pcmc, sizeof(uintptr_t));
    mem::read_physical(reinterpret_cast<PVOID>(LocalPawn + offsets::PlayerState), &PlayerState, sizeof(DWORD_PTR));
    mem::read_physical(reinterpret_cast<PVOID>(PlayerController + offsets::PlayerCameraManager), &PlayerCameraManager, sizeof(DWORD_PTR));
    mem::read_physical(reinterpret_cast<PVOID>(LocalPawn + PlayerCameraManager), &PlayerCameraManager, sizeof(DWORD_PTR));
    mem::read_physical(reinterpret_cast<PVOID>(LocalPawn + offsets::RootComponet), &Rootcomp, sizeof(DWORD_PTR));
    mem::read_physical(reinterpret_cast<PVOID>(baseAddress + offsets::PersistentLevel), &Persistentlevel, sizeof(DWORD_PTR));
    mem::read_physical(reinterpret_cast<PVOID>(LocalPawn + 0x948), &Crrneytwep, sizeof(uintptr_t));
    mem::read_physical(reinterpret_cast<PVOID>(Persistentlevel + offsets::ActorCount), &ActorCount, sizeof(DWORD));
    mem::read_physical(reinterpret_cast<PVOID>(Persistentlevel + offsets::AActor), &AOFFSETS, sizeof(DWORD_PTR));
    mem::read_physical(reinterpret_cast<PVOID>(baseAddress + offsets::GameState), &GameState, sizeof(DWORD_PTR));
    mem::read_physical(reinterpret_cast<PVOID>(GameState + offsets::PlayerArray), &PlayerArray, sizeof(DWORD_PTR));
    mem::read_physical(reinterpret_cast<PVOID>(GameState + (offsets::PlayerArray + sizeof(uintptr_t))), &num, sizeof(int));
 
    // Loop threw number of current actors in match (actor loop)
    for (uint32_t i = 0; i < num; i++)
    {
        mem::read_physical(reinterpret_cast<PVOID>(PlayerArray + i * 0x8), &player, sizeof(uintptr_t));
        mem::read_physical(reinterpret_cast<PVOID>(player + 0x308), &CurrentActor, sizeof(uintptr_t));

        if (!CurrentActor) continue;
        float CornerWidth;
        mem::read_physical(reinterpret_cast<PVOID>(CurrentActor + offsets::Mesh), &CurrentActorMesh, sizeof(uint64_t));
        Vector3 HeadBone3d = GetBoneWithRotation(CurrentActorMesh, 67);
        Vector3 HeadOnScreen = ProjectWorldToScreen(Vector3(HeadBone3d.x, HeadBone3d.y, HeadBone3d.z + 15));
        DrawBox(HeadOnScreen.x - (CornerWidth / 2), HeadOnScreen.y, CornerWidth, HeadOnScreen, 225.0f, 225.0f, 225.0f, 2.0f, 0);

    }
    
    return 0;
}

/* once you setup / initialized / found the driver, you can get the base address
      the base addy is the starting point in the memory of a process
      to find out stuff like uworld, we take the uworld offset (distance where uworld
      is from base addy) and we do baseAddress + uworld and then we can read uworld.
      the same applys to other offsets
   */