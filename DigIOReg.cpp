#define GPIO_ENABLE_REG reinterpret_cast<volatile uint32_t*>(0x60004020) // Master Direction Reg


#define GPIO0_MODE_REG        reinterpret_cast<volatile uint32_t*>(0x60009004) // Individual Config Reg
#define GPIO1_MODE_REG        reinterpret_cast<volatile uint32_t*>(0x60009008) // Individual Config Reg
#define GPIO2_MODE_REG        reinterpret_cast<volatile uint32_t*>(0x6000900C) // Individual Config Reg
#define GPIO3_MODE_REG        reinterpret_cast<volatile uint32_t*>(0x60009010) // Individual Config Reg
#define GPIO4_MODE_REG        reinterpret_cast<volatile uint32_t*>(0x60009014) // Individual Config Reg
#define GPIO5_MODE_REG        reinterpret_cast<volatile uint32_t*>(0x60009018) // Individual Config Reg
#define GPIO6_MODE_REG        reinterpret_cast<volatile uint32_t*>(0x6000901C) // Individual Config Reg
#define GPIO7_MODE_REG        reinterpret_cast<volatile uint32_t*>(0x60009020) // Individual Config Reg
#define GPIO8_MODE_REG        reinterpret_cast<volatile uint32_t*>(0x60009024) // Individual Config Reg
#define GPIO9_MODE_REG        reinterpret_cast<volatile uint32_t*>(0x60009028) // Individual Config Reg
#define GPIO10_MODE_REG       reinterpret_cast<volatile uint32_t*>(0x6000902C) // Individual Config Reg
#define GPIO11_MODE_REG       reinterpret_cast<volatile uint32_t*>(0x60009030) // Individual Config Reg
#define GPIO12_MODE_REG       reinterpret_cast<volatile uint32_t*>(0x60009034) // Individual Config Reg
#define GPIO13_MODE_REG       reinterpret_cast<volatile uint32_t*>(0x60009038) // Individual Config Reg
#define GPIO14_MODE_REG       reinterpret_cast<volatile uint32_t*>(0x6000903C) // Individual Config Reg
#define GPIO15_MODE_REG       reinterpret_cast<volatile uint32_t*>(0x60009040) // Individual Config Reg
#define GPIO16_MODE_REG       reinterpret_cast<volatile uint32_t*>(0x60009044) // Individual Config Reg
#define GPIO17_MODE_REG       reinterpret_cast<volatile uint32_t*>(0x60009048) // Individual Config Reg
#define GPIO18_MODE_REG       reinterpret_cast<volatile uint32_t*>(0x6000904C) // Individual Config Reg
#define GPIO19_MODE_REG       reinterpret_cast<volatile uint32_t*>(0x60009050) // Individual Config Reg
#define GPIO20_MODE_REG       reinterpret_cast<volatile uint32_t*>(0x60009054) // Individual Config Reg
#define GPIO21_MODE_REG       reinterpret_cast<volatile uint32_t*>(0x60009058) // Individual Config Reg



#define GPIO_DATA_REG reinterpret_cast<volatile uint32_t*>(0x60004004) // Global Output Reg



#include <cstdint>
#include <new>


class DigIOReg {

public:

    enum class Mode : uint32_t {
        input = 0,
        output = 1
        
    };

    const Mode ioregmod; // 1 -> input, 0 -> output
    
    volatile uint32_t* const pinconf; //mode register address
    
    volatile uint32_t* const dataregadd; //data register address
    
    const uint32_t pinmask;

    
    static DigIOReg* digioreg(Mode ioregmod, volatile uint32_t* pinconf, volatile uint32_t* dataregadd, uint32_t pinmask, void* buffer) {
        
        
        return ::new (buffer) DigIOReg(ioregmod, pinconf, dataregadd, pinmask);
        
    }
    
    void SetHigh() {
        if (ioregmod == Mode::output) {
            *dataregadd |= pinmask;  // Just turn on the bit in the data register
    }
        
    }
    

    void SetLow() {
        if (ioregmod == Mode::output) {
            *dataregadd &= ~pinmask; // Just turn off the bit in the data register
    }
        
    }
    
    uint32_t ReadReg() const {
        return *dataregadd;
    }

    uint32_t getMode() const {
        return static_cast<uint32_t>(ioregmod);
    }
    

    
private:

    DigIOReg(Mode m, volatile uint32_t* ra, volatile uint32_t* da, uint32_t p) : ioregmod(m), pinconf(ra), dataregadd(da), pinmask(p) {
        
        *pinconf = (1 << 12);
        
        if (ioregmod == Mode::output) {
            *GPIO_ENABLE_REG |= pinmask;  // Turn ON our pin's bit (Output)
        } 
        
        else {
            *GPIO_ENABLE_REG &= ~pinmask; // Turn OFF our pin's bit (Input)
        }
    
 
    }
    
    
};

uint8_t ledBuffer[sizeof(DigIOReg)];

int main() {
    
    
    
    // 1. Create the Pin 8 Output instance
    DigIOReg* led = DigIOReg::digioreg(
        DigIOReg::Mode::output,  
        GPIO8_MODE_REG,          
        GPIO_DATA_REG,           
        (1 << 8),                
        ledBuffer                
    );

    // 2. Loop forever blinking the LED at a human-readable speed
    while(true) {
        led->SetHigh(); 
        for(volatile uint32_t i = 0; i < 500000; i++); // Wait

        led->SetLow();  
        for(volatile uint32_t i = 0; i < 500000; i++); // Wait
    }

  
    
}








