#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <signal.h>
#include <math.h>

// on SIGINT cleanup and exit
void siginth(int num)
{
    SDL_CloseAudio();
    SDL_Quit();
    printf("Exiting\n");
    exit(0);
}

// looping melody
int stuff[] = {
    //554, 555, 556, 2217, 2215, 2216,
    //440, 440, 220, 220, 440, 440, 494, 494, 554, 554
    //440, 0, 220, 0, 440, 0, 440, 494
    220, 220, 0, 220, 220, 0, 220, 220, 0, 220, 220, 0,
    175, 175, 0, 175, 175, 0, 175, 175, 0, 175, 175, 0, 
    196, 196, 0, 196, 196, 0, 196, 196, 0, 196, 196, 0, 
    220, 220, 0, 220, 220, 0, 220, 220, 0, 220, 220, 0,
};
// i => current note, k => current sample
int i = 0, k = 0;

enum { SQUARE, TRIANGLE, SINE } method = SQUARE;

// we don't want our function overflowing
#define LIMIT(f) (signed char)(((f > 127.0f) ? 127.0f : ((f < -127.f) ? -127.f : f)))

void audio_callback(void* data, Uint8* stream, int length)
{
    int j;
    SDL_AudioSpec spec = *((SDL_AudioSpec*)data);
    float factor = 126.f;
    if(stuff[i]) {
        int totl = spec.freq / (stuff[i]/2);
        switch(method) {
        case SQUARE:
            for(j = 0; j < length; ++j) {
                // half the period output MAX
                if(k < totl/2) {
                    stream[j] = factor;
                // half output 0
                } else {
                    stream[j] = 0;
                }
                k = (k + 1) % totl;
            }
            break;
        case TRIANGLE:
            for(j = 0; j < length; ++j) {
                // first quarter-period, increasing linear function [0..MAX]
                if(k < totl/4) {
                    stream[j] = (Uint8)LIMIT(factor * ((float)k / (totl/4.f)));
                // second and third quarters, decreasing linear function [MAX..-MAX]
                } else if(k < 3*totl/4) {
                    stream[j] = (Uint8)LIMIT(factor * (
                                ((float)totl/2.f - (float)k)/(totl/4.f)));
                // fourth quarter, increasing linear function [-MAX..0]
                } else {
                    stream[j] = (Uint8)LIMIT(factor * (
                                (float)(k - totl) / (totl/4.)));
                }
                k = (k + 1) % totl;
            }
            break;
        case SINE:
            for(j = 0; j < length; ++j) {
                // I don't understand what the deal with the frequency is
                //     that I DON'T have to multiply by 2 here...
                // Anyway, compared with a reference sound and indeed this
                //     is the correct frequency (somehow)
                stream[j] = (Uint8)(factor * sin(k * stuff[i] * M_PI / (float)spec.freq));
                k = (k + 1) % totl;
            }
            break;
        }
    } else {
        // if we aren't supposed to output any sound, set the buffer to 0
        memset(stream, 0, length);
    }
}

int main(int argc, char* argv[])
{
    SDL_AudioSpec as, spec;

    SDL_Init(SDL_INIT_AUDIO);
    signal(SIGINT, &siginth);

    if(argc > 1) {
        if(strcmp(argv[1], "square") == 0) {
            method = SQUARE;
        } else if(strcmp(argv[1], "triangle") == 0) {
            method = TRIANGLE;
        } else if(strcmp(argv[1], "sine") == 0) {
            method = SINE;
        }
    } else {
        fprintf(stderr, "usage: %s square|triangle|sine\n", argv[0]);
        return 1;
    }

    // set up AudioSpec
    as.freq = 44100;
    as.format = AUDIO_S16SYS;
    as.channels = 1;
    as.samples = 512;
    as.callback = &audio_callback;
    // pass a pointer with the obtained audio spec back to the callback
    as.userdata = &spec;

    // start playing music
    SDL_OpenAudio(&as, &spec);
    SDL_PauseAudio(0);

    while(1) {
        SDL_Delay(100); // arbitrary delay => note length
        // prevent the callback from firing mid-modification
        SDL_LockAudio();
        // shift to the next note
        i = (i + 1) % (sizeof(stuff) / sizeof(stuff[0]));
        // resume callback
        SDL_UnlockAudio();
    }
    return 0;
}
