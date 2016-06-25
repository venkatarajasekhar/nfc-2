/****************************************************************************
 * led/led.h
 *
 *   Copyright (C) 2006-2010 Jetway Security Micro. All rights reserved.
 *   Author: Kenichi Cui <kenichi@jetsec.com.cn>
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/********************************************************************************
 * Included Files
 ********************************************************************************/

#ifndef LED_H_
#define LED_H_

/*
 * 
 * led_1 : div2to32led port map (
      CLK     => sCLKBLINK0,
      RSTN    => sPORN,
      BLINKON => sGPOO(78), -- 0x08000C08[14]
      FCEON   => sGPOO(77), -- 0x08000C08[13]
      POLAR   => sGPOO(76), -- 0x08000C08[12]
      DIV0    => sGPOO(72), -- 0x08000C08[8]
      DIV1    => sGPOO(73), -- 0x08000C08[9]
      DIV2    => sGPOO(74), -- 0x08000C08[10]
      DIV3    => sGPOO(75), -- 0x08000C08[11]
      OUT1    => led1       -- LEDG_0, AB1 for DE3
    );
    led_2 : div2to32led port map (
      CLK     => sCLKBLINK0,
      RSTN    => sPORN,
      BLINKON => sGPOO(86), -- 0x08000C08[22]
      FCEON   => sGPOO(85), -- 0x08000C08[21]
      POLAR   => sGPOO(84), -- 0x08000C08[20]
      DIV0    => sGPOO(80), -- 0x08000C08[16]
      DIV1    => sGPOO(81), -- 0x08000C08[17]
      DIV2    => sGPOO(82), -- 0x08000C08[18]
      DIV3    => sGPOO(83), -- 0x08000C08[19]
      OUT1    => led2       -- LEDR_1, AC1 for DE3
    );
 * 
 * 
 */


#define LEDREG_BASE		0x08000C08
#define	LEDREG			((volatile unsigned int *)LEDREG_BASE)

#define LED1_DIV0		0x100
#define LED1_DIV1		0x200
#define LED1_DIV2		0x400
#define LED1_DIV3		0x800
#define LED1_POLAR		0x1000
#define LED1_FCEON		0x2000
#define LED1_BLINKON	0x4000

#define LED2_DIV0		0x10000
#define LED2_DIV1		0x20000
#define LED2_DIV2		0x40000
#define LED2_DIV3		0x80000
#define LED2_POLAR		0x100000
#define LED2_FCEON		0x200000
#define LED2_BLINKON	0x400000

#define POLAR_HIGH		1
#define POLAR_LOW		0

// ret frequence = 6 / (div[0~3] + 1)
#define RATE_HIGH8			0x00
#define RATE_HIGH7			0x01
#define RATE_HIGH6			0x02
#define RATE_HIGH5			0x03
#define RATE_HIGH4			0x04
#define RATE_HIGH3			0x05
#define RATE_HIGH2			0x06
#define RATE_HIGH1			0x07
#define RATE_MID			0x08
#define RATE_LOW1			0x09
#define RATE_LOW2			0x0a
#define RATE_LOW3			0x0b
#define RATE_LOW4			0x0c
#define RATE_LOW5			0x0d
#define RATE_LOW6			0x0e
#define	RATE_LOW7			0x0f


/********************************************************************************
 * Public Variables
 ********************************************************************************/

/********************************************************************************
 * Public Function Prototypes
 ********************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

EXTERN void led1_forceon();
EXTERN void led1_forceoff();
EXTERN void led1_blinkon(int rate);
EXTERN void led2_forceon();
EXTERN void led2_forceoff();
EXTERN void led2_blinkon(int rate);


#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /*LED_H_*/
