// RCC 관련 레지스터: 클럭 설정용
#define RCC_APB1ENR    (*(volatile uint32_t*)0x40023840)  // APB1 버스 (I2C 클럭)
#define RCC_AHB1ENR    (*(volatile uint32_t*)0x40023830)  // AHB1 버스 (GPIO 클럭)

// I2C1 베이스 주소 및 레지스터 정의
#define I2C1_BASE      0x40005400
#define I2C1_CR1       (*(volatile uint32_t*)(I2C1_BASE + 0x00))  // 제어 1
#define I2C1_CR2       (*(volatile uint32_t*)(I2C1_BASE + 0x04))  // 제어 2 (클럭 설정)
#define I2C1_OAR1      (*(volatile uint32_t*)(I2C1_BASE + 0x08))  // Own Address 1
#define I2C1_DR        (*(volatile uint32_t*)(I2C1_BASE + 0x10))  // 데이터 레지스터
#define I2C1_SR1       (*(volatile uint32_t*)(I2C1_BASE + 0x14))  // 상태 1
#define I2C1_SR2       (*(volatile uint32_t*)(I2C1_BASE + 0x18))  // 상태 2
#define I2C1_CCR       (*(volatile uint32_t*)(I2C1_BASE + 0x1C))  // 클럭 제어
#define I2C1_TRISE     (*(volatile uint32_t*)(I2C1_BASE + 0x20))  // 최대 상승 시간

// I2C1 초기화
void i2c1_init() {
    RCC_APB1ENR |= (1 << 21);    // I2C1 클럭 활성화
    RCC_AHB1ENR |= (1 << 1);     // GPIOB 클럭 활성화 (PB6=SCL, PB7=SDA)

    I2C1_CR2 = 16;               // APB1 클럭 주파수 설정 (16MHz)
    I2C1_CCR = 80;               // 100kHz 설정 (Standard Mode = 16MHz / (2*CCR))
    I2C1_TRISE = 17;             // 최대 상승 시간 = 1000ns 기준 → TRISE = Freq(MHz) + 1

    I2C1_CR1 |= (1 << 0);        // I2C1 Peripheral Enable
}
//  I2C1 동작 함수들 (START → 주소 전송 → 데이터 전송 → STOP)
void i2c1_start() {
    I2C1_CR1 |= (1 << 8);                    // START 비트 설정
    while (!(I2C1_SR1 & (1 << 0)));          // SB (Start Bit 설정됨) 플래그 대기
}

void i2c1_send_addr(uint8_t addr) {
    I2C1_DR = addr;                          // 슬레이브 주소 (LSB = 0 for Write)
    while (!(I2C1_SR1 & (1 << 1)));          // ADDR 플래그 대기 (주소 전송 완료)
    (void)I2C1_SR2;                          // SR2 읽기로 ADDR 클리어
}

void i2c1_send_data(uint8_t data) {
    while (!(I2C1_SR1 & (1 << 7)));          // TXE (전송 버퍼 Empty) 대기
    I2C1_DR = data;                          // 데이터 전송
    while (!(I2C1_SR1 & (1 << 2)));          // BTF (Byte Transfer Finished) 대기
}

void i2c1_stop() {
    I2C1_CR1 |= (1 << 9);                    // STOP 비트 설정 → 통신 종료
}
//✅ I2C1 마스터 송신 (1 바이트)
void i2c1_write_byte(uint8_t dev_addr, uint8_t reg, uint8_t data) {
    i2c1_start();                            // START 신호 발생
    i2c1_send_addr(dev_addr << 1);          // 슬레이브 주소 전송 (Write 모드)
    i2c1_send_data(reg);                    // 레지스터 주소 전송 (보통 EEPROM은 주소 지정 필요)
    i2c1_send_data(data);                   // 실제 전송할 데이터 바이트
    i2c1_stop();                             // STOP으로 종료
}


void main(){
    i2c1_init();                                 // I2C1 초기화
    i2c1_write_byte(0x50, 0x10, 0xAB);           // EEPROM 0x50, 주소 0x10에 0xAB 쓰기

}

// 주의사항
// 반드시 GPIO (PB6, PB7 등)를 Alternate Function으로 설정해야 동작합니다.

// 오류 핸들링(ACK 실패 등)은 위 코드에 포함되어 있지 않으며, 실전에서는 타임아웃 처리, 에러 플래그 확인이 추가되어야 합니다.

// 빠르게 쓰고 반복적으로 송신할 경우, BTF 확인 없이 Stop을 내리면 문제가 생길 수 있습니다.

