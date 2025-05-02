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
