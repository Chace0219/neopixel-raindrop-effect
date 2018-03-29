
#include <Adafruit_NeoPixel.h>

#define PIN 6 // output to Neopixel 
#define NUM_LEDS 230 // 60 leds Per meter, so 60*4 Leds

/* Chase1 Relative Setting */
#define CHASE1COLOR 0x0000FF // R,G,B

#define CHASE1BRIGHTNESS 128 // Brightness(255:MAX, 0:MIN)

#define CHASE1START 0 // the total length from the start point to the end one. 

#define CHASE1END 120 // the total length from the start point to the end one. 

#define CHASE1LENGTH 6 // length of every segment of chase 1

#define CHASE1GAP 2 // the gap between two adjacent segments for chase 1

#define CHASE1RATE 200 // Chase1 Slide Rate time : 500ms


/* Chase2 Relative Setting */
#define CHASE2COLOR 0xFF0000 // R,G,B

#define CHASE2BRIGHTNESS 255 // Brightness(255:MAX, 0:MIN)

#define CHASE2START 125 // Start Pixel Point

#define CHASE2END 229 // End Pixel Point. 

#define CHASE2LENGTH 2 // length of every segment of chase 2

#define CHASE2GAP 2 // the gap between two adjacent segments for chase 2

#define CHASE2RATE 400 // Chase2 Slide  Rate time : 500ms

// Chase Flow rate
#define CHASERATE 500 // Rate time : 500ms

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

unsigned short TotalStrip[NUM_LEDS]; // Pixel Buffer

#define GAP     0
#define CHASE1  1
#define CHASE2  2

/*
 *  It builds Strip buffer as your chase requirements
*/
void InitBuildStrip()
{
    unsigned short nChase1Seg = (CHASE1END - CHASE1START) + 1;
    unsigned short nChase2Seg = (CHASE2END - CHASE2START) + 1;

    unsigned short nBlankCnt = (CHASE2START - CHASE1END) - 1;

    uint8_t Chase1Cnt = nChase1Seg / (CHASE1LENGTH + CHASE1GAP);
    uint8_t Chase1Rem = nChase1Seg % (CHASE1LENGTH + CHASE1GAP);

    uint8_t Chase2Cnt = nChase2Seg / (CHASE2LENGTH + CHASE2GAP);
    uint8_t Chase2Rem = nChase2Seg % (CHASE2LENGTH + CHASE2GAP);

    uint8_t i, j, k, l;
    uint8_t nSeek = 0;
    if(CHASE1START > 0)
    {
        for(i = 0; i < (CHASE1START - 1); i++)
            TotalStrip[nSeek + k] = GAP;    
    }

    nSeek = CHASE1START;
    for(j = 0; j < Chase1Cnt; j++)
    {
        for(k = 0; k < CHASE1GAP; k++)
            TotalStrip[nSeek + k] = GAP;    
        for(k = 0; k < CHASE1LENGTH; k++)
            TotalStrip[(nSeek + CHASE1GAP) + k] = CHASE1;    
        nSeek += (CHASE1LENGTH + CHASE1GAP);
    }

    if(Chase1Rem > 0)
    {
        if(Chase1Rem < CHASE1GAP)
            for(k = 0; k < Chase1Rem; k++)
                TotalStrip[nSeek + k] = GAP;
        else
        {
            for(k = 0; k < CHASE1GAP; k++)
                TotalStrip[nSeek + k] = GAP;
            for( ; k < Chase1Rem; k++)
                TotalStrip[nSeek + k] = CHASE1;
        }
    }

    nSeek = CHASE1END + 1;
    for(i = 0; i < nBlankCnt; i++)
        TotalStrip[nSeek + i] = GAP;    

    nSeek = CHASE2START;
    // Chase2 Segment
    for(j = 0; j < Chase2Cnt; j++)
    {
        for(k = 0; k < CHASE2GAP; k++)
            TotalStrip[nSeek + k] = GAP;    
        for(k = 0; k < CHASE2LENGTH; k++)
            TotalStrip[(nSeek + CHASE2GAP) + k] = CHASE2;    
        nSeek += (CHASE2LENGTH + CHASE2GAP);
    }

    if(Chase2Rem > 0)
    {
        if(Chase2Rem < CHASE2GAP)
            for(k = 0; k < Chase2Rem; k++)
                TotalStrip[nSeek + k] = GAP;
        else
        {
            for(k = 0; k < CHASE2GAP; k++)
                TotalStrip[nSeek + k] = GAP;
            for( ; k < Chase2Rem; k++)
                TotalStrip[nSeek + k] = CHASE2;
        }
    }

    nSeek = CHASE2END + 1;
    if(CHASE2END < NUM_LEDS - 1)
    {
        for(i = (CHASE2END + 1); i < NUM_LEDS; i++)
            TotalStrip[i] = GAP;
        
    }
}

/*
 * 
*/
void SlideChase1(uint8_t nDist)
{
    uint8_t temp = TotalStrip[CHASE1END];
    uint8_t idx;
    for(idx = CHASE1END; idx > CHASE1START; idx--)
        TotalStrip[idx] = TotalStrip[idx - 1];    
    TotalStrip[idx] = temp;
}

void SlideChase2(uint8_t nDist)
{
    uint8_t temp = TotalStrip[CHASE2END];
    uint8_t idx;
    for(idx = CHASE2END; idx > CHASE2START; idx--)
        TotalStrip[idx] = TotalStrip[idx - 1];    
    TotalStrip[idx] = temp;
}

/*
 * 
*/
void DrawStrip()
{
    unsigned short idx = 0;
    uint32_t color;
    for(idx = 0; idx < NUM_LEDS; idx++)
    {
        switch(TotalStrip[idx])
        {
            case GAP:
            {
                color = strip.Color(0x00, 0x00, 0x00);    // BLACK color
            }
            break;

            case CHASE1:
            {
                color = strip.Color(((CHASE1COLOR >> 16) * CHASE1BRIGHTNESS) >> 8, 
                    (((CHASE1COLOR >> 8) & 0xFF) * CHASE1BRIGHTNESS) >> 8, ((CHASE1COLOR & 0xFF) * CHASE1BRIGHTNESS) >> 8);   
            }
            break;

            case CHASE2:
            {
                color = strip.Color(((CHASE2COLOR >> 16) * CHASE2BRIGHTNESS) >> 8, 
                    (((CHASE2COLOR >> 8) & 0xFF) * CHASE2BRIGHTNESS) >> 8, ((CHASE2COLOR & 0xFF) * CHASE2BRIGHTNESS) >> 8);   
            }
            break;
        }
        strip.setPixelColor(idx, color);
    }
}

void setup() 
{
    Serial.begin(19200);
    Serial.println(F("Program Started!"));
    delay(1000);
    InitBuildStrip();
    
    strip.setBrightness(255);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    
    //
    Serial.println(F("Init End!"));
    DrawStrip();
    strip.show(); // Initialize all pixels to 'off'
}

static uint32_t nSlide1Time = millis();
static uint32_t nSlide2Time = millis();
static uint32_t nScanTime = millis();

void loop() 
{

    if(nSlide1Time + CHASE1RATE < millis())
    {
        nSlide1Time = millis();
        SlideChase1(1);                    
    }

    if(nSlide2Time + CHASE2RATE < millis())
    {
        nSlide2Time = millis();
        SlideChase2(1);                    
    }

    if(nScanTime + 20 < millis())
    {
        nScanTime = millis();
        DrawStrip();
        strip.show();
    }
}
