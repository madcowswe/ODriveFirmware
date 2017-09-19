#include "callbacks.h"
#include "constants.h"
#include "motor.h"
#include "error.h"

void vbusSenseADC_cb(ADC_HandleTypeDef *hadc, bool injected) {
    static const float voltage_scale = 3.3f * 11.0f / (float)(1 << 12);
    // Only one conversion in sequence, so only rank1
    uint32_t ADCValue = HAL_ADCEx_InjectedGetValue(hadc, ADC_INJECTED_RANK_1);
    vbus_voltage = ADCValue * voltage_scale;
}

// step/direction interface
void step_cb(uint16_t GPIO_Pin) {
    static Motor* motor0 = Motor::getMotorByID(0);
    static Motor* motor1 = Motor::getMotorByID(1);
    GPIO_PinState dir_pin;
    float dir;
    switch (GPIO_Pin) {
        case GPIO_1_Pin:
            // M0 stepped
            if (motor0->enable_step_dir) {
                dir_pin = HAL_GPIO_ReadPin(GPIO_2_GPIO_Port, GPIO_2_Pin);
                dir = (dir_pin == GPIO_PIN_SET) ? 1.0f : -1.0f;
                motor0->pos_setpoint += dir * motor0->counts_per_step;
            }
            break;
        case GPIO_3_Pin:
            // M1 stepped
            if (motor1->enable_step_dir) {
                dir_pin = HAL_GPIO_ReadPin(GPIO_4_GPIO_Port, GPIO_4_Pin);
                dir = (dir_pin == GPIO_PIN_SET) ? 1.0f : -1.0f;
                motor1->pos_setpoint += dir * motor1->counts_per_step;
            }
            break;
        default:
            Error::globalFault(ERROR_UNEXPECTED_STEP_SRC);
            break;
    }
}