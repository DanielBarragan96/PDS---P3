
#include "SignalValues.h"
#include "DataTypeDefinitions.h"

static uint16_t g_counter_sin = 0;
static uint16_t g_counter_song = 0;
static uint16_t g_counter_evolve = 1.00f;
static volatile bool change_note = true;

static uint32_t Song[] =
{
    DO,DO,SOL,SOL,LA,LA,SOL,SILENCE,FA,FA,MI,MI,RE,RE,DO,
    SOL,SOL,FA,FA,MI,MI,RE,SILENCE,SOL,SOL,FA,FA,MI,MI,DO,
    DO,DO,SOL,SOL,LA,LA,SOL,SILENCE,FA,FA,MI,MI,RE,RE,DO,END
};

uint32_t nextDACValue()
{
    if(change_note)
    {
        change_note = false;
        return 4094U/2;
    }
    else
    {
        g_counter_sin++;
        if(FUNCTION_LENGTH <= g_counter_sin)
        {
            g_counter_sin=0;
        }

        return (uint32_t) (SinusOutputData[g_counter_sin]*envolvente[g_counter_evolve]/12288);
    }
}

uint32_t nextSongNote()
{
    g_counter_song++;
        if(FUNCTION_LENGTH <= g_counter_song)
        {
            return END;
        }
        uint32_t ldval;

        switch(Song[g_counter_song])
        {
            case DO:
            {
                ldval = LDVAL_trigger_C;
                break;
            }
            case RE:
            {
                ldval = LDVAL_trigger_D;
                break;
            }
            case MI:
            {
                ldval = LDVAL_trigger_E;
                break;
            }
            case FA:
            {
                ldval = LDVAL_trigger_F;
                break;
            }
            case SOL:
            {
                ldval = LDVAL_trigger_G;
                break;
            }
            case LA:
            {
                ldval = LDVAL_trigger_A;
                break;
            }
            case SI:
            {
                ldval = SI;
                break;
            }
            case END:
            {
                ldval = END;
                break;
            }
            case SILENCE:
            {
                ldval = SILENCE;
                break;
            }
        }
        change_note = true;
        g_counter_sin=0;
        g_counter_evolve=0;
        return ldval;
}

void increaseEnvolv()
{
    g_counter_evolve++;
    if(FUNCTION_LENGTH <= g_counter_evolve)
        g_counter_evolve=0;
}
