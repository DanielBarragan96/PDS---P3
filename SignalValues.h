/*
 * SignalValues.h
 *
 *  Created on: Mar 15, 2018
 *      Author: lei-n
 */

#ifndef SIGNALVALUES_H_
#define SIGNALVALUES_H_

#include "fsl_common.h"

//size of the function
#define FUNCTION_LENGTH 256

//values of the Sinus function
static const uint32_t SinusOutputData[FUNCTION_LENGTH] = {
2047U, 2097U, 2147U, 2198U, 2248U, 2298U, 2347U, 2397U, 2446U, 2496U, 2544U, 2593U, 2641U, 2689U,
2737U, 2784U, 2830U, 2877U, 2922U, 2967U, 3012U, 3056U, 3099U, 3142U, 3184U, 3226U, 3266U, 3306U,
3346U, 3384U, 3422U, 3458U, 3494U, 3530U, 3564U, 3597U, 3629U, 3661U, 3691U, 3721U, 3749U, 3776U,
3803U, 3828U, 3852U, 3875U, 3897U, 3918U, 3938U, 3957U, 3974U, 3991U, 4006U, 4020U, 4033U, 4044U,
4055U, 4064U, 4072U, 4079U, 4084U, 4088U, 4092U, 4093U, 4094U, 4093U, 4092U, 4088U, 4084U, 4079U,
4072U, 4064U, 4055U, 4044U, 4033U, 4020U, 4006U, 3991U, 3974U, 3957U, 3938U, 3918U, 3897U, 3875U,
3852U, 3828U, 3803U, 3776U, 3749U, 3721U, 3691U, 3661U, 3629U, 3597U, 3564U, 3530U, 3494U, 3458U,
3422U, 3384U, 3346U, 3306U, 3266U, 3226U, 3184U, 3142U, 3099U, 3056U, 3012U, 2967U, 2922U, 2877U,
2830U, 2784U, 2737U, 2689U, 2641U, 2593U, 2544U, 2496U, 2446U, 2397U, 2347U, 2298U, 2248U, 2198U,
2147U, 2097U, 2047U, 1997U, 1947U, 1896U, 1846U, 1796U, 1747U, 1697U, 1648U, 1598U, 1550U, 1501U,
1453U, 1405U, 1357U, 1310U, 1264U, 1217U, 1172U, 1127U, 1082U, 1038U, 995U, 952U, 910U, 868U,
828U, 788U, 748U, 710U, 672U, 636U, 600U, 564U, 530U, 497U, 465U, 433U, 403U, 373U,
345U, 318U, 291U, 266U, 242U, 219U, 197U, 176U, 156U, 137U, 120U, 103U, 88U, 74U,
61U, 50U, 39U, 30U, 22U, 15U, 10U, 6U, 2U, 1U, 0U, 1U, 2U, 6U,
10U, 15U, 22U, 30U, 39U, 50U, 61U, 74U, 88U, 103U, 120U, 137U, 156U, 176U,
197U, 219U, 242U, 266U, 291U, 318U, 345U, 373U, 403U, 433U, 465U, 497U, 530U, 564U,
600U, 636U, 672U, 710U, 748U, 788U, 828U, 868U, 910U, 952U, 995U, 1038U, 1082U, 1127U,
1172U, 1217U, 1264U, 1310U, 1357U, 1405U, 1453U, 1501U, 1550U, 1598U, 1648U, 1697U, 1747U, 1796U,
1846U, 1896U, 1947U, 1997U
};

//Lower notes
//period for the pit to get LA
#define A_PERIOD 220.00
//period for the pit to get SOL
#define G_PERIOD 196.00
//period for the pit to get FA
#define F_PERIOD 174.60
//period for the pit to get MI
#define E_PERIOD 164.80
//period for the pit to get RE
#define D_PERIOD 146.8
//period for the pit to get DO
#define C_PERIOD 130.8

/*
//Higher notes
//period for the pit to get LA
#define A_PERIOD 440.00
//period for the pit to get SOL
#define G_PERIOD 392.00
//period for the pit to get FA
#define F_PERIOD 349.20
//period for the pit to get MI
#define E_PERIOD 329.6
//period for the pit to get RE
#define D_PERIOD 293.7
//period for the pit to get DO
#define C_PERIOD 361.6
*/

//the system clock of the Kinetis
#define SYSTEM_CLOCK CLOCK_GetBusClkFreq()
//pit number to count
#define LDVAL_trigger_A  ((SYSTEM_CLOCK)/(A_PERIOD*FUNCTION_LENGTH))
//pit number to count
#define LDVAL_trigger_G  ((SYSTEM_CLOCK)/(G_PERIOD*FUNCTION_LENGTH))
//pit number to count
#define LDVAL_trigger_F  ((SYSTEM_CLOCK)/(F_PERIOD*FUNCTION_LENGTH))
//pit number to count
#define LDVAL_trigger_E  ((SYSTEM_CLOCK)/(E_PERIOD*FUNCTION_LENGTH))
//pit number to count
#define LDVAL_trigger_D  ((SYSTEM_CLOCK)/(D_PERIOD*FUNCTION_LENGTH))
//pit number to count
#define LDVAL_trigger_C  ((SYSTEM_CLOCK)/(C_PERIOD*FUNCTION_LENGTH))

#define LDVAL_trigger_SEC  (SYSTEM_CLOCK/2)

typedef enum{
    END=0,
    SILENCE,
    DO,
    RE,
    MI,
    FA,
    SOL,
    LA,
    SI
}Notes;

uint32_t nextDACValue();

uint32_t nextSongNote();

#endif /* SIGNALVALUES_H_ */
