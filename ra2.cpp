/*
    Copyright (C) 2023 Guokai Chen (Original Author)

    Copyright (C) 2023 wudi7mi

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, version 3 of the
    License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <string>
#include <memory>
#include <Windows.h>
#include <TlHelp32.h>   //for PROCESSENTRY32, needs to be included after windows.h
#include <stdint.h>
#include "ra2.h"

#define INVALIDCLASS 0xffffffffu
#define INFOFFSET 0x557c

#define ALLIDOGOFFSET 0x1c
#define SOVDOGOFFSET 0x9

#define TANKOFFSET 0x5568
#define ALLITANKOFFSET 0x9
#define SOVTANKOFFSET 0x3
#define ALLIMINEROFFSET (0x84 / 4)
#define SOVMINEROFFSET (0x4 / 4)    // or 0x4 / 4

#define BUILDINGOFFSET 0x5554
#define ALLIWARFACTORYOFFSET (0x1c / 4)
#define SOVWARFACTORYOFFSET (0x38 / 4)


#define CREDITSPENT_OFFSET 0x2dc
#define BALANCEOFFSET 0x30c
#define USERNAMEOFFSET 0x1602a
#define ISWINNEROFFSET 0x1f7
#define ISLOSEROFFSET 0x1f8

#define POWEROUTPUTOFFSET 0x53a4
#define POWERDRAINOFFSET 0x53a8

#define PRODUCINGBUILDINGINDEXOFFSET 0x564c
#define PRODUCINGUNITINDEXOFFSET 0x5650

#define HOUSETYPECLASSBASEOFFSET 0x34
#define COUNTRYSTRINGOFFSET 0x24

#define COLOROFFSET 0x56fC
#define COLORSCHEMEOFFSET 0x16054

#define ROCKETEEROFFSET 0x04
#define DRONEOFFSET 0x10

#define IFVOFFSET 0x26
#define FLAKTRACKOFFSET 0x11

#define CONSCRIPTOFFSET 0x01
#define GIOFFSET 0x0

#define SUBMARINEOFFSET		0x13
#define DESTROYEROFFSET		0x12

#define DOPHINOFFSET		0x19
#define SQUIDOFFSET			0x18

#define CVOFFSET			0x0d	// Aircraft carrier
#define DREADNOUGHTOFFSET	0x16    // SOV

unsigned ALLIRocketeerPtr[MAXPLAYERS];
unsigned SOVDronePtr[MAXPLAYERS];
unsigned ALLIIFVPtr[MAXPLAYERS];
unsigned SOVFlakTruckPtr[MAXPLAYERS];
unsigned SOVConscriptPtr[MAXPLAYERS];

unsigned ALLIRocketeer[MAXPLAYERS];
unsigned SOVDrone[MAXPLAYERS];
unsigned ALLIIFV[MAXPLAYERS];
unsigned SOVFlakTruck[MAXPLAYERS];
unsigned SOVConscript[MAXPLAYERS];

unsigned ALLIBoatPtr[MAXPLAYERS];
unsigned SOVBoatPtr[MAXPLAYERS];

unsigned ALLIBoat[MAXPLAYERS];
unsigned SOVBoat[MAXPLAYERS];

unsigned ALLIFishPtr[MAXPLAYERS];
unsigned SOVFishPtr[MAXPLAYERS];

unsigned ALLIFish[MAXPLAYERS];
unsigned SOVFish[MAXPLAYERS];

unsigned ALLICarrierPtr[MAXPLAYERS];
unsigned SOVCarrierPtr[MAXPLAYERS];

unsigned ALLICarrier[MAXPLAYERS];
unsigned SOVCarrier[MAXPLAYERS];

unsigned counts[0x2000];

unsigned color[MAXPLAYERS];

bool inited = false;
int validHouses = 0;
unsigned *HouseBase[MAXPLAYERS];

unsigned fixedPoint = (0xa8b230);
unsigned fixedPointValue;
unsigned playerBasePtr[MAXPLAYERS];

unsigned classBaseArrayPtr = 0xa8022c;
unsigned classBaseArray;
unsigned realPlayerBasePtr[MAXPLAYERS];
unsigned realPlayerBase[MAXPLAYERS];
unsigned itemPtr[MAXPLAYERS];
unsigned itemArrayBase[MAXPLAYERS];

unsigned balancePtr[MAXPLAYERS];
unsigned spentCreditPtr[MAXPLAYERS];
unsigned balance[MAXPLAYERS];
unsigned spentCredit[MAXPLAYERS];

unsigned userNamePtr[MAXPLAYERS];
wchar_t userName[MAXPLAYERS][0x20];

unsigned isWinnerPtr[MAXPLAYERS];
unsigned isLoserPtr[MAXPLAYERS];
bool isWinner[MAXPLAYERS];
bool isLoser[MAXPLAYERS];

unsigned powerOutputPtr[MAXPLAYERS];
unsigned powerDrainPtr[MAXPLAYERS];
unsigned powerOutput[MAXPLAYERS];
unsigned powerDrain[MAXPLAYERS];

unsigned producingBuildingIndexPtr[MAXPLAYERS];
unsigned producingUnitIndexPtr[MAXPLAYERS];
unsigned producingBuildingIndex[MAXPLAYERS];
unsigned producingUnitIndex[MAXPLAYERS];

unsigned houseTypeClassBasePtr[MAXPLAYERS];
unsigned houseTypeClassBase[MAXPLAYERS];

unsigned countryNamePtr[MAXPLAYERS];
char countryName[MAXPLAYERS][0x40];

unsigned GIPtr[MAXPLAYERS], ALLIDogPtr[MAXPLAYERS], SOVDogPtr[MAXPLAYERS];
unsigned GI[MAXPLAYERS], ALLIDog[MAXPLAYERS], SOVDog[MAXPLAYERS];
unsigned ALLITankPtr[MAXPLAYERS], SOVTankPtr[MAXPLAYERS];
unsigned ALLITank[MAXPLAYERS], SOVTank[MAXPLAYERS];

unsigned ALLIMinerPtr[MAXPLAYERS], SOVMinerPtr[MAXPLAYERS];
unsigned ALLIMiner[MAXPLAYERS], SOVMiner[MAXPLAYERS];

unsigned ALLIWarFactoryPtr[MAXPLAYERS], SOVWarFactoryPtr[MAXPLAYERS];
unsigned ALLIWarFactory[MAXPLAYERS], SOVWarFactory[MAXPLAYERS];

unsigned playerColorPtr[MAXPLAYERS];
struct ColorStruct playerColor[MAXPLAYERS];

bool validPlayer[MAXPLAYERS];

bool currentGameRunning;

const char *allieCountries[] = {
    "Americans",
    "Alliance",         // Korea
    "French",
    "Germans",
    "British"
};
const char *sovietCountries[] = {
    "Africans",         // Libya
    "Arabs",            // Iraq
    "Confederation",    // Cuba
    "Russians"
};

DWORD findPidByName(const std::string& name)
{
    std::unique_ptr<void, decltype(&CloseHandle)> h(
        CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0),
        &CloseHandle
        );

    if (h.get() == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create snapshot" << std::endl;
        return 0;
    }

    PROCESSENTRY32 processInfo {};
    processInfo.dwSize = sizeof(PROCESSENTRY32);

    for (BOOL success = Process32First(h.get(), &processInfo); success; success = Process32Next(h.get(), &processInfo)) {
        if (name == processInfo.szExeFile) {
            DWORD pid = processInfo.th32ProcessID;
            std::cout << "PID Found: " << pid << std::endl;
            return pid;
        }
    }

    return 0;
}

void readData(HANDLE pHandle, unsigned addr, void *buf, SIZE_T size) {
    ReadProcessMemory(
        pHandle,
        reinterpret_cast<const void*>(addr),
        buf,
        size,
        NULL
        );
    return;
}

void ReadClassBase() {
    HANDLE pHandle = OpenProcess(
        PROCESS_QUERY_INFORMATION |     // Needed to get a process' token
            PROCESS_CREATE_THREAD |     // For obvious reasons
            PROCESS_VM_OPERATION |      // Required to perform operations on address space of process (like WriteProcessMemory)
            PROCESS_VM_READ,            // Required for read data
        FALSE,                          // Don't inherit pHandle
        findPidByName("gamemd-spawn.exe"));

    if (pHandle == NULL)
    {
        fprintf(stderr, "Could not open process\n");
        return;
    }

    readData(pHandle, fixedPoint, &fixedPointValue, 4);

    readData(pHandle, classBaseArrayPtr, &classBaseArray, 4);

    unsigned playerBaseArrayPtr = (fixedPointValue + 1120 * 4);
    for (int i = 0; i < MAXPLAYERS; i++, playerBaseArrayPtr += 4) {
        readData(pHandle, playerBaseArrayPtr, &playerBasePtr[i], 4);

        if (playerBasePtr[i] != INVALIDCLASS) {
            validPlayer[i] = true;

            realPlayerBasePtr[i] = playerBasePtr[i] * 4 + classBaseArray;
            // printf("real playerBaseArrayPtr %x\n", realPlayerBasePtr[i]);
            readData(pHandle, realPlayerBasePtr[i], &realPlayerBase[i], 4);

            // Balance
            balancePtr[i] = realPlayerBase[i] + BALANCEOFFSET;
            readData(pHandle, balancePtr[i], &balance[i], 4);
            printf("Player %d balance %u\n", i, balance[i]);

            // Spent money
            spentCreditPtr[i] = realPlayerBase[i] + CREDITSPENT_OFFSET;
            readData(pHandle, spentCreditPtr[i], &spentCredit[i], 4);
            printf("Player %d spent %u\n", i, spentCredit[i]);

            // User name
            userNamePtr[i] = realPlayerBase[i] + USERNAMEOFFSET;
            readData(pHandle, userNamePtr[i], &userName[i], 0x20);
            printf("Player %d name %ls\n", i, userName[i]);

            // Win or lose
            isWinnerPtr[i] = realPlayerBase[i] + ISWINNEROFFSET;
            readData(pHandle, isWinnerPtr[i], &isWinner[i], 1);
            isLoserPtr[i] = realPlayerBase[i] + ISLOSEROFFSET;
            readData(pHandle, isLoserPtr[i], &isLoser[i], 1);
            // printf("Player %d wins: %u\n", i, isWinner[i]);

            // Power output / drain
            powerOutputPtr[i] = realPlayerBase[i] + POWEROUTPUTOFFSET;
            readData(pHandle, powerOutputPtr[i], &powerOutput[i], 4);
            powerDrainPtr[i] = realPlayerBase[i] + POWERDRAINOFFSET;
            readData(pHandle, powerDrainPtr[i], &powerDrain[i], 4);
            printf("Player %d power output/drain %u/%u \n", i, powerOutput[i], powerDrain[i]);


            unsigned colorPtr = realPlayerBase[i] + COLORSCHEMEOFFSET;
            readData(pHandle, colorPtr, &color[i], 4);
            // printf("Player %d colorScheme %u\n", i, color);

            /*
            // currently unusable
            // Producing building
            producingBuildingIndexPtr = realPlayerBase + ODUCINGBUILDINGINDEXOFFSET;
            ReadProcessMemory(handle,
                              reinterpret_cast<const void*>(producingBuildingIndexPtr),
                              &producingBuildingIndex,
                              4,
                              NULL);
            printf("Player %d producingBuildingIndex %u\n", i, producingBuildingIndex);
            */

            // HouseTypeClassBase
            houseTypeClassBasePtr[i] = realPlayerBase[i] + HOUSETYPECLASSBASEOFFSET;
            readData(pHandle, houseTypeClassBasePtr[i], &houseTypeClassBase[i], 4);
            // printf("Player %d houseTypeClassBase %x\n", i, houseTypeClassBase[i]);

            // Country name
            countryNamePtr[i] = houseTypeClassBase[i] + COUNTRYSTRINGOFFSET;
            readData(pHandle, countryNamePtr[i], &countryName[i], 0x19);
            printf("Player %d countryName %s\n", i, countryName[i]);

            playerColorPtr[i] = houseTypeClassBase[i] + COLOROFFSET;
            readData(pHandle, playerColorPtr[i], &playerColor[i], 3);
            // printf("Player %d color r %u g %u b %u\n", i, playerColor[i].rgb[0], playerColor[i].rgb[1], playerColor[i].rgb[2]);

            // Building part
            itemPtr[i] = realPlayerBase[i] + BUILDINGOFFSET;
            readData(pHandle, itemPtr[i], &itemArrayBase[i], 4);
            // printf("Building itemPtr %x\n", itemPtr[i]);
            // printf("Building itemArrayBase %x\n", itemArrayBase[i]);

            // War Factory
            ALLIWarFactoryPtr[i] = itemArrayBase[i] + 4 * ALLIWARFACTORYOFFSET;
            readData(pHandle, ALLIWarFactoryPtr[i], &ALLIWarFactory[i], 4);
            printf("Player %d ALLI War Factory: %u\n", i, ALLIWarFactory[i]);

            SOVWarFactoryPtr[i] = itemArrayBase[i] + 4 * SOVWARFACTORYOFFSET;
            readData(pHandle, SOVWarFactoryPtr[i], &SOVWarFactory[i], 4);
            printf("Player %d SOV War Factory: %u\n", i, SOVWarFactory[i]);

            // Infantry part
            itemPtr[i] = realPlayerBase[i] + INFOFFSET;
            readData(pHandle, itemPtr[i], &itemArrayBase[i], 4);
            // printf("Infantry itemPtr %x\n", itemPtr[i]);
            // printf("Infantry itemArrayBase %x\n", itemArrayBase[i]);

            // GI
            GIPtr[i] = itemArrayBase[i] + 4 * GIOFFSET;
            readData(pHandle, GIPtr[i], &GI[i], 4);

            // Conscript
            Constcript[i] = itemArrayBase[i] + 4 * GIOFFSET;
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * CONSCRIPTOFFSET),
                              &SOVConscript[i],
                              4,
                              NULL);
            // Rocket fly
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * ROCKETEEROFFSET),
                              &ALLIRocketeer[i],
                              4,
                              NULL);

            // ALLI Dog
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * ALLIDOGOFFSET),
                              &ALLIDog[i],
                              4,
                              NULL);
            // SOV Dog
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * SOVDOGOFFSET),
                              &SOVDog[i],
                              4,
                              NULL);

            printf("Player %d Dog: %u\n", i, SOVDog[i] + ALLIDog[i]);

            // (unit)Tank Part
            itemPtr[i] = realPlayerBase[i] + TANKOFFSET;
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemPtr[i]),
                              &itemArrayBase[i],
                              4,
                              NULL);
            // printf("Tank itemPtr %x\n", itemPtr[i]);
            // printf("Tank itemArrayBase %x\n", itemArrayBase[i]);

            // IFV
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * IFVOFFSET),
                              &ALLIIFV[i],
                              4,
                              NULL);
            // Flak Track
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * FLAKTRACKOFFSET),
                              &SOVFlakTruck[i],
                              4,
                              NULL);
            // ALLI Tank
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * ALLITANKOFFSET),
                              &ALLITank[i],
                              4,
                              NULL);
            // SOV Tank
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * SOVTANKOFFSET),
                              &SOVTank[i],
                              4,
                              NULL);
            printf("Player %d Tank: %u\n", i, SOVTank[i] + ALLITank[i]);

            // Drone
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * DRONEOFFSET),
                              &SOVDrone[i],
                              4,
                              NULL);
            printf("Player %d Drone: %u\n", i, SOVDrone[i]);

            // ALLI Miner
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * ALLIMINEROFFSET),
                              &ALLIMiner[i],
                              4,
                              NULL);

            // SOV Miner
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * SOVMINEROFFSET),
                              &SOVMiner[i],
                              4,
                              NULL);
            printf("Player %d Miner: %u\n", i, SOVMiner[i] + ALLIMiner[i]);

            // ALLI Ship
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * DESTROYEROFFSET),
                              &ALLIBoat[i],
                              4,
                              NULL);
            // printf("Player %d Alli Boat count %u\n", i, ALLIBoat[i]);

            // SOV Ship
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * SUBMARINEOFFSET),
                              &SOVBoat[i],
                              4,
                              NULL);
            // printf("Player %d Sov Boat count %u\n", i, SOVBoat[i]);

            // ALLI Fish
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * DOPHINOFFSET),
                              &ALLIFish[i],
                              4,
                              NULL);
            // printf("Player %d Alli Fish count %u\n", i, ALLIFish[i]);

            // SOV Fish
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * SQUIDOFFSET),
                              &SOVFish[i],
                              4,
                              NULL);
            // printf("Player %d Sov Fish count %u\n", i, SOVFish[i]);

            // ALLI Carrier
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * CVOFFSET),
                              &ALLICarrier[i],
                              4,
                              NULL);
            // printf("Player %d Alli CV count %u\n", i, ALLICarrier[i]);

            // SOV Carrier
            ReadProcessMemory(pHandle,
                              reinterpret_cast<const void*>(itemArrayBase[i] + 4 * DREADNOUGHTOFFSET),
                              &SOVCarrier[i],
                              4,
                              NULL);
            // printf("Player %d Sov CV count %u\n", i, SOVCarrier[i]);

        } else {
            // Empty player, skip
            validPlayer[i] = false;
        }
    }
}


int main() {
    while (1) {
        if (!findPidByName("gamemd-spawn.exe")) {
            currentGameRunning = false;
            Sleep(500);
            continue;
        }
        currentGameRunning = true;

        ReadClassBase();
        Sleep(500);
    }
    return 0;
}
