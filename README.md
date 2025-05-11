# STM32F20x 주요 기능 정리

이 문서는 STM32F20x 시리즈 마이크로컨트롤러의 핵심 하드웨어 기능을 요약한 README입니다.

---

## 🧠 1. DMA Controller (DMA)

### 주요 특징
- **2개의 범용 DMA 컨트롤러(DMA1, DMA2)**
- 각 DMA는 **8개의 독립된 스트림** 지원
- **메모리 ↔ 메모리 / 메모리 ↔ 주변장치 / 주변장치 ↔ 메모리** 전송 지원
- **중앙 FIFO**, **버스트 전송**, **최대 대역폭 (AHB/APB)** 제공
- **Circular Buffer**, **Double Buffering** 기능 내장
- 각 스트림은 **하드웨어 요청 또는 소프트웨어 트리거**로 작동

### DMA 사용 가능 주변장치
- SPI, I2S
- I2C
- USART, UART
- TIMx (타이머)
- DAC
- SDIO
- DCMI (카메라)
- ADC

---

## 💾 2. Flexible Static Memory Controller (FSMC)

### 지원 모드
- PC Card / Compact Flash
- SRAM / PSRAM / NOR Flash / NAND Flash

### 기능 요약
- **Write FIFO** 지원
- **외부 메모리 코드 실행 가능** (단, NAND 및 PC Card 제외)
- **최대 60MHz** 동작 속도

### LCD 병렬 인터페이스
- Intel 8080 / Motorola 6800 모드 호환
- 그래픽 LCD와의 손쉬운 연결 가능

---

## ⚡ 3. Nested Vectored Interrupt Controller (NVIC)

### 주요 기능
- **최대 81개의 마스커블 인터럽트 + Cortex-M3의 16개 인터럽트** 지원
- 저지연 인터럽트 처리 (core와 밀접 결합)
- **벡터 테이블 주소 직접 전달**
- **Tail Chaining**, **프로세서 상태 자동 저장/복원**
- 명령 오버헤드 없는 인터럽트 복귀

---

## 🕒 4. Real-Time Clock (RTC), Backup SRAM & Registers

### RTC
- BCD 형식의 초, 분, 시, 요일, 날짜, 월, 연도 저장
- **윤년 자동 보정**, **알람 및 주기적 인터럽트 설정**
- **32.768kHz 클럭 기반 동작** (외부 또는 내부 저속/고속 클럭)
- **프리스케일러 / 자동 웨이크업 / 16비트 타이머 지원**

### Backup SRAM
- **4KB**, EEPROM 유사 비휘발성 저장 영역
- **VBAT 또는 Standby 모드**에서 데이터 유지 가능

### Backup Registers
- **20개, 32비트 크기**
- 최대 **80바이트 사용자 데이터 저장 가능**
- 전원 리셋/시스템 리셋 이후에도 데이터 유지

---

> 📘 이 문서는 STM32F20x 사용자 매뉴얼 기반의 요약이며, 기능 세부 설정은 레지스터 매핑 및 HAL/LL 라이브러리를 통해 적용할 수 있습니다.

📦 전체 메모리 맵 구조 (0x0000_0000 ~ 0xFFFF_FFFF)
STM32는 주소 공간을 512MB 단위로 8개의 블록으로 나눕니다:

주소 범위	블록	설명
0x0000_0000 ~ 0x1FFF_FFFF	Block 0	Code 영역 (Flash, Boot Memory)
0x2000_0000 ~ 0x3FFF_FFFF	Block 1	SRAM 영역
0x4000_0000 ~ 0x5FFF_FFFF	Block 2	Peripherals (레지스터)
0x6000_0000 ~ 0x7FFF_FFFF	Block 3	FSMC bank1 & bank2 (외부 메모리 인터페이스)
0x8000_0000 ~ 0x9FFF_FFFF	Block 4	FSMC bank3 & bank4
0xA000_0000 ~ 0xBFFF_FFFF	Block 5	FSMC 레지스터
0xC000_0000 ~ 0xDFFF_FFFF	Block 6	사용되지 않음
0xE000_0000 ~ 0xFFFF_FFFF	Block 7	Cortex-M3 내부 시스템 및 디버그용 레지스터

🔸 Block 0 (Code 영역) 상세
주소	설명
0x0000_0000	Flash, System Memory, or SRAM으로 alias 됨 (BOOT 핀에 따라 달라짐)
0x0800_0000 ~ 0x080F_FFFF	Flash 메모리
0x1FFF_0000 ~ 0x1FFF_7A0F	System Memory, OTP, Option Bytes 등
0x1FFF_C000 ~ 0x1FFF_FFFF	Option Bytes, Reserved 등

🔸 Block 1 (SRAM 영역) 상세
주소	설명
0x2000_0000 ~ 0x2001_BFFF	SRAM (112KB)
0x2001_C000 ~ 0x2001_FFFF	SRAM (16KB)
0x2002_0000 ~ 0x3FFF_FFFF	Reserved (bit-band alias 영역 포함)

💡 Bit-Banding: 비트를 개별적으로 다루기 위해 특별히 매핑된 주소 영역입니다.

🔸 Block 2 (Peripherals 영역) 상세
주소	주변장치
0x4000_0000 ~ 0x4000_03FF	TIM2, TIM3, TIM4 등
...	...
0x4001_0000 ~ 0x4001_FFFF	USART1, SPI1, ADC1~3 등
총 0x4000_0000 ~ 0x5006_03FF	모든 AHB/APB 장치들이 여기에 위치

🔸 Block 3 & 4 (FSMC External Memory 영역)
이 영역은 **외부 메모리 (PSRAM, NOR, NAND 등)**를 연결하는 FSMC 컨트롤러의 bank별 주소입니다:

주소 범위	연결 장치
0x6000_0000 ~ 0x63FF_FFFF	FSMC bank1 NOR/PSRAM 1
0x6400_0000 ~ 0x67FF_FFFF	FSMC bank1 NOR/PSRAM 2
0x6800_0000 ~ 0x6BFF_FFFF	FSMC bank1 NOR/PSRAM 3
0x6C00_0000 ~ 0x6FFF_FFFF	FSMC bank1 NOR/PSRAM 4
0x7000_0000 ~ 0x7FFF_FFFF	FSMC bank2 NAND
0x8000_0000 ~ 0x8FFF_FFFF	FSMC bank3 NAND

🔸 Block 7 (Cortex-M3 내부 영역)
주소 범위	설명
0xE000_0000 ~ 0xE00F_FFFF	NVIC, SCB, SysTick 등
0xE004_2000 등	디버깅, ITM, DWT 등 디버그 모듈

Memory-Mapped I/O란?
Memory-Mapped I/O는 주변 장치 (GPIO, USART, TIM 등)의 레지스터를 메모리 주소 공간에 직접 매핑해서 일반 메모리처럼 접근하는 방식입니다.

즉,

c
복사
편집
#define GPIOA_MODER  (*(volatile uint32_t*)0x40020000)
GPIOA_MODER = 0x00000001;
처럼 메모리에 값을 쓰는 것처럼 레지스터를 제어하는 방식입니다.

✅ STM32에서의 MMIO 예시
STM32에서는 Block 2 (0x4000_0000 ~ 0x5FFF_FFFF)가 바로 이 MMIO를 위한 주변장치 레지스터 영역입니다.

장치	시작 주소	예시 설명
GPIOA	0x4002_0000	포트 A 제어
USART2	0x4000_4400	시리얼 통신 제어
TIM2	0x4000_0000	타이머 제어
RCC	0x4002_3800	클럭 설정

✅ 왜 MMIO를 사용할까?
방식	특징
MMIO	CPU가 메모리처럼 장치 레지스터에 접근함. Load/Store 명령 사용.
Port-Mapped I/O (x86에서 사용)	특별한 I/O 명령 사용 (in, out)

STM32는 MMIO 방식만 지원합니다. 장점은:

CPU 입장에서 I/O도 메모리처럼 읽고 씀

주소 공간 통합 관리

고속 처리 (버스에 직접 연결됨)

✅ STM32 MMIO 작동 방식 그림
STM32 내부 구조는 다음과 같은 흐름입니다:

복사
편집
[ CPU ]
   ↓
[ BUS (AHB/APB) ]
   ↓
[ 주소 디코더 ] → 0x4002_0000 = GPIOA
   ↓
[ 장치 레지스터 ]
✅ 예제: GPIOA 핀 출력 제어 (MMIO 직접 접근)
c
복사
편집
#define RCC_AHB1ENR   (*(volatile uint32_t*)0x40023830)
#define GPIOA_MODER   (*(volatile uint32_t*)0x40020000)
#define GPIOA_ODR     (*(volatile uint32_t*)0x40020014)

void gpio_init(void) {
    RCC_AHB1ENR |= (1 << 0);        // GPIOA 클럭 활성화
    GPIOA_MODER |= (1 << 10);       // PA5를 출력 모드로
}

void gpio_write_high(void) {
    GPIOA_ODR |= (1 << 5);          // PA5 출력 High
}
✅ 요약
항목	설명
MMIO란?	주변장치 레지스터를 메모리 주소처럼 직접 접근
STM32 적용 주소	0x4000_0000 ~ 0x5FFF_FFFF (Block 2)
접근 방법	C 포인터를 이용해 직접 레지스터 주소에 접근
장점	빠르고 일관된 접근 방식, 간결한 하드웨어 제어