#include "main.h"
#include "stm32u585xx.h"
#include "stm32u5xx_ll_rcc.h"
#include "stm32u5xx_ll_bus.h"

void simplified_MSIS_100k() {
    // MSISRANGE can be modified when MSIS is off (MSISON = 0) or when MSIS is ready
    // (MSISRDY = 1). MSISRANGE must NOT be modified when MSIS is on and NOT ready
    // (MSISON = 1 and MSISRDY = 0)
    // MSIS and MSIK frequency ranges can be adjusted by software, by using respectively
    // MSISRANGE[3:0] and MSIKRANGE[3:0] in RCC_ICSCR1, with MSIRGSEL = 1.

    // Disable MSIS. Note: To stop MSIS, a different clock must be
    // running (ie HSI16)
    RCC->CR &= ~(RCC_CR_MSISON);
    while ((RCC->CR & RCC_CR_MSISRDY)) {
    };

    // This bit is set by software to select the MSIS clock range with MSISRANGE[3:0]
    RCC->ICSCR1 |= (RCC_ICSCR1_MSIRGSEL);
    // 1111: range 15 around 100 kHz
    RCC->ICSCR1 &= ~(RCC_ICSCR1_MSISRANGE);
    // 0001: range 1 around 24 MHz
    // RCC->ICSCR1 |= (0x1U << RCC_ICSCR1_MSISRANGE_Pos);
    RCC->ICSCR1 |= (0xFU << RCC_ICSCR1_MSISRANGE_Pos);

    // Enable MSIS
    RCC->CR |= (RCC_CR_MSISON);
    while (!(RCC->CR & RCC_CR_MSISRDY)) {
    };

    // Set MSIS as SYSCLK
    RCC->CFGR1 &= ~(RCC_CFGR1_SW);
    RCC->CFGR1 |= (0x0UL << RCC_CFGR1_SW_Pos);
    while ((RCC->CFGR1 & (0x0UL << RCC_CFGR1_SWS_Pos))) {
    };

    // Disable HSI16
    RCC->CR &= ~(RCC_CR_HSION);
    while ((RCC->CR & RCC_CR_HSIRDY)) {
    };
}

void simplified_MSI_PLL_160M() {
    // Set 4 waiting states for FLASH (?)
    FLASH->ACR &= ~(FLASH_ACR_LATENCY);
    FLASH->ACR |= (FLASH_ACR_LATENCY_4WS);
    while (!(FLASH->ACR & FLASH_ACR_LATENCY_4WS)) {
    };

    // Voltage scaling range selection
    // By default the system works in range 3: medium-low power range
    // It provides a typical output voltage at 1.0 V. The system clock frequency can be up to 55 MHz.
    // However, we want to go to 160MHz... and thus to range 1: high performance
    // It provides a typical output voltage at 1.2 V. It is used when the system clock frequency is up to 160 MHz
    PWR->VOSR &= ~(PWR_VOSR_VOS);
    PWR->VOSR |= (PWR_VOSR_VOS);
    while ((((PWR->VOSR & PWR_VOSR_VOSRDY) == (PWR_VOSR_VOSRDY)) ? 1UL : 0UL)
            == 0) {
    };

    // Enable MSIS
    RCC->CR |= (RCC_CR_MSISON);
    while (!(RCC->CR & RCC_CR_MSISRDY)) {
    };

    // Enable MSIS range selection
    RCC->ICSCR1 |= (RCC_ICSCR1_MSIRGSEL);

    // Set MSIS frequency range
    // 0100: range 4 around 4 MHz (reset value)
    RCC->ICSCR1 &= ~(RCC_ICSCR1_MSISRANGE);
    RCC->ICSCR1 |= (0x4U << RCC_ICSCR1_MSISRANGE_Pos);

    LL_RCC_MSI_SetCalibTrimming(16, LL_RCC_MSI_OSCILLATOR_1);

    // // Set M, N and R bits of PLL
    uint32_t PLLM = 1;
    uint32_t PLLN = 80;
    uint32_t PLLR = 2;
    RCC->PLL1CFGR &= ~(RCC_PLL1CFGR_PLL1SRC | RCC_PLL1CFGR_PLL1M);
    RCC->PLL1CFGR |= (RCC_PLL1CFGR_PLL1SRC_0
            | ((PLLM - 1UL) << RCC_PLL1CFGR_PLL1M_Pos));
    RCC->PLL1DIVR &= ~(RCC_PLL1DIVR_PLL1N | RCC_PLL1DIVR_PLL1R);
    RCC->PLL1DIVR |= ((PLLN - 1UL) << RCC_PLL1DIVR_PLL1N_Pos)
            | ((PLLR - 1UL) << RCC_PLL1DIVR_PLL1R_Pos);

    // Enable the pll1_r_ck output of the PLL1
    RCC->PLL1CFGR |= RCC_PLL1CFGR_PLL1REN;

    // This bit must be set in range 1 and range 2 before increasing the system clock frequency above 55 MHz
    PWR->VOSR &= ~(PWR_VOSR_BOOSTEN);
    PWR->VOSR |= (PWR_VOSR_BOOSTEN);

    while ((((PWR->VOSR & PWR_VOSR_BOOSTRDY) == (PWR_VOSR_BOOSTRDY)) ? 1UL : 0UL)
            == 0) {
    };

    // Select the proper reference (input) frequency range used for PLL1
    // 00-01-10: PLL1 input (ref1_ck) clock range frequency between 4 and 8 MHz
    RCC->PLL1CFGR &= ~(RCC_PLL1CFGR_PLL1RGE);
    RCC->PLL1CFGR |= (0x00000000U);

    // Enable PLL1
    RCC->CR |= (RCC_CR_PLL1ON);
    // Wait until PLL1 is ready
    while ((((RCC->CR & RCC_CR_PLL1RDY) == RCC_CR_PLL1RDY) ? 1UL : 0UL) != 1) {
    }

    // Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz
    RCC->CFGR2 &= ~(RCC_CFGR2_HPRE);
    RCC->CFGR2 |= (RCC_CFGR2_HPRE_3);

    // Set system clock source to PLL1
    // 11: PLL pll1_r_ck selected as system clock
    RCC->CFGR1 &= ~(RCC_CFGR1_SW);
    RCC->CFGR1 |= (RCC_CFGR1_SW_1 | RCC_CFGR1_SW_0);
    // Wait until system clock is ready
    while (!(RCC->CFGR1 & (RCC_CFGR1_SW_1 | RCC_CFGR1_SW_0))) {
    };

    // Insure 1us transition state at intermediate medium speed clock
    for (__IO uint32_t i = (160 >> 1); i != 0; i--);

    // Set AHB prescaler (SYSCLK not divided)
    RCC->CFGR2 &= ~(RCC_CFGR2_HPRE);
    RCC->CFGR2 |= (0x00000000U);

    // Set APB1 prescaler (SYSCLK not divided)
    RCC->CFGR2 &= ~(RCC_CFGR2_PPRE1);
    RCC->CFGR2 |= (0x00000000U);

    // Set APB2 prescaler (SYSCLK not divided)
    RCC->CFGR2 &= ~(RCC_CFGR2_PPRE2);
    RCC->CFGR2 |= (0x00000000U);

    // Set APB3 prescaler (SYSCLK not divided)
    RCC->CFGR2 &= ~(RCC_CFGR3_PPRE3);
    RCC->CFGR2 |= (0x00000000U);
}

void simplified_HSI16() {
    // HSI configuration and activation
    // HSION: HSI16 clock enable
    // This bit is set by hardware to indicate that HSI16 oscillator is stable. It is set only when HSI16
    // is enabled by software (by setting HSION)
    RCC->CR |= (RCC_CR_HSION);
    while (!(RCC->CR & RCC_CR_HSIRDY)) {
    };

    // Set HSI as the SYSCLK source
    // Bits 1:0 SW[1:0]: system clock switch

    // This bitfield is set and cleared by software to select system clock source (SYSCLK). It is
    // configured by hardware to force MSIS oscillator selection when exiting Standby or Shutdown
    // mode. This bitfield is configured by hardware to force MSIS or HSI16 oscillator selection
    // when exiting Stop mode or in case of HSE oscillator failure, depending on STOPWUCK.
    // 00: MSIS selected as system clock
    // 01: HSI16 selected as system clock
    // 10: HSE selected as system clock
    // 11: PLL pll1_r_ck selected as system clock

    // Bits 3:2 SWS[1:0]: system clock switch status
    // This bitfield is set and cleared by hardware to indicate which clock source is used as system clock
    // 00: MSIS oscillator used as system clock
    // 01: HSI16 oscillator used as system clock
    // 10: HSE used as system clock
    // 11: PLL pll1_r_ck used as system clock
    RCC->CFGR1 &= ~(RCC_CFGR1_SW);
    RCC->CFGR1 |= (0x1UL << RCC_CFGR1_SW_Pos);
    while (!(RCC->CFGR1 & RCC_CFGR1_SWS_0)) {
    };

    // Disable MSI
    RCC->CR &= ~(RCC_CR_MSISON);
    while (!(RCC->CR & RCC_CR_MSISRDY)) {
    };
}

#define LED_RED_SET     (1U << 6)
#define LED_RED_RESET   (LED_RED_SET << 16)
#define LED_GREEN_SET   (1U << 7)
#define LED_GREEN_RESET (LED_GREEN_SET << 16)

// Template for the lab exercises
uint8_t BSP_Button_GetState()
{
    if ((GPIOC->IDR & GPIO_IDR_ID13) == GPIO_IDR_ID13) {
        return 1;
    }
    else {
        return 0;
    }
}

void BSP_LED_Init()
{
    enum {MY_GPIO_MODE_INPUT, MY_GPIO_MODE_OUTPUT, MY_GPIO_MODE_AF, MY_GPIO_MODE_ANALOG};

    RCC->AHB2ENR1 |= RCC_AHB2ENR1_GPIOHEN;
    GPIOH->MODER &= ~(GPIO_MODER_MODE6_Msk); // Reset mode for red LED
    GPIOH->MODER &= ~(GPIO_MODER_MODE7_Msk); // Reset mode for green LED
    GPIOH->MODER |= (MY_GPIO_MODE_OUTPUT << GPIO_MODER_MODE6_Pos);
    GPIOH->MODER |= (MY_GPIO_MODE_OUTPUT << GPIO_MODER_MODE7_Pos);

    // Enable AHB2 clock
    RCC->AHB2ENR1 |= RCC_AHB2ENR1_GPIOCEN;

    // Enable PC13
    GPIOC->MODER &= ~(GPIO_MODER_MODE13_Msk);
    GPIOC->MODER |= (MY_GPIO_MODE_INPUT << GPIO_MODER_MODE13_Pos);

    // Disable PC13 Pull-up/Pull-down
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_Msk;
}

void BSP_LED_Init_raw()
{
    // 0x80 is equivalent to 10000000
    *((uint16_t *)0x46020c8c) = 0x80U;

    // GPIOH_MODER for red LED
    // *((unsigned int *)0x42021c00) = 0xFFFFDFFFU;

    //GPIOH_MODER for red + green LED
    *((unsigned int *)0x42021c00) = 0xFFFF5FFF;
}

#define RCC_BASE_ADDRESS 0x46020C00U
#define RCC_AHB2ENR1 (*((uint16_t *)(RCC_BASE_ADDRESS + 0x08CU)))

#define GPIOH_BASE_ADDRESS 0x42021C00
#define GPIOH_MODER (*((unsigned int *)(GPIOH_BASE_ADDRESS + 0x00U)))
#define GPIOH_BSRR (*((unsigned int *)(GPIOH_BASE_ADDRESS + 0x18U)))

void BSP_LED_Init_raw_defines()
{
    // 0x80 is equivalent to 10000000
    RCC_AHB2ENR1 = 0x80U;

    //GPIOH_MODER for red + green LED
    GPIOH_MODER = 0xFFFF5FFF;
}
/*
int main(void)
{
    BSP_LED_Init();

    while(1) {
        if (BSP_Button_GetState()){
            GPIOH->BSRR = LED_RED_SET;
            GPIOH->BSRR = LED_GREEN_SET;
        } else {
            GPIOH->BSRR = LED_RED_RESET;
            GPIOH->BSRR = LED_GREEN_RESET;
        }
    }
}
*/

int main(void)
{
//    simplified_HSI16();
//    simplified_MSIS_100k();
    LL_AHB3_GRP1_EnableClock(LL_AHB3_GRP1_PERIPH_PWR);
    simplified_MSI_PLL_160M();

    BSP_LED_Init_raw_defines();

    while(1) {
        // GPIOH_BSRR for red LED
         *((unsigned int *)0x42021c18) = 0x40;

         for (int i = 0; i < 1000000; i++);

         *((unsigned int *)0x42021c18) = 0x400000;

         for (int i = 0; i < 1000000; i++);
        // GPIOH_BSRR for green LED
         *((unsigned int *)0x42021c18) = 0x80;

         for (int i = 0; i < 1000000; i++);

         *((unsigned int *)0x42021c18) = 0x800000;
    }
}
