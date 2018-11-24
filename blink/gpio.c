/*
 * Copyright (C) 2018 xiaochengzi, wechat:taksing-leung
 */

#include "hal/soc/soc.h"
#include "wm_gpio.h"

static const u8_t int_edge_map[] =
{
    [IRQ_TRIGGER_RISING_EDGE] = WM_GPIO_IRQ_TRIG_RISING_EDGE,
    [IRQ_TRIGGER_FALLING_EDGE] = WM_GPIO_IRQ_TRIG_FALLING_EDGE,
    [IRQ_TRIGGER_BOTH_EDGES] = WM_GPIO_IRQ_TRIG_DOUBLE_EDGE,
};

int32_t hal_gpio_init(gpio_dev_t *gpio)
{
    if(gpio->port == NULL){
        return -1;
    }

    u8_t gpio_dir = 0;
    u8_t gpio_attr = 0;

    switch(gpio->config){
        case ANALOG_MODE: 
            break;

        case IRQ_MODE:
            gpio_dir = WM_GPIO_DIR_INPUT;
            gpio_attr = WM_GPIO_ATTR_FLOATING;
            break;

        case OUTPUT_PUSH_PULL:
            gpio_dir = WM_GPIO_DIR_OUTPUT;
            gpio_attr = WM_GPIO_ATTR_FLOATING;
            break;

        case OUTPUT_OPEN_DRAIN_PULL_UP:
            gpio_dir = WM_GPIO_DIR_OUTPUT;
            gpio_attr = WM_GPIO_ATTR_PULLHIGH;
        break;

        case OUTPUT_OPEN_DRAIN_NO_PULL:
            gpio_dir = WM_GPIO_DIR_OUTPUT;
            gpio_attr = WM_GPIO_ATTR_PULLLOW;       
            break;

        case INPUT_PULL_DOWN:
            gpio_dir = WM_GPIO_DIR_OUTPUT;
            gpio_attr = WM_GPIO_ATTR_PULLLOW;
            break;

        case INPUT_PULL_UP:
            gpio_dir = WM_GPIO_DIR_INPUT;
            gpio_attr = WM_GPIO_ATTR_PULLHIGH;
            break;

        case INPUT_HIGH_IMPEDANCE:
            break;

        default:break;
    }

    tls_gpio_cfg(gpio->port, gpio_dir, gpio_attr);

    return 0;
}

// int32_t hal_gpio_finalize(gpio_dev_t *gpio)
// {
//     return -1;
// }

int32_t hal_gpio_output_high(gpio_dev_t *gpio)
{
    tls_gpio_write(gpio->port, 1);

    return 0;
}

int32_t hal_gpio_output_low(gpio_dev_t *gpio)
{
    tls_gpio_write(gpio->port, 0);

    return 0;
}

int32_t hal_gpio_output_toggle(gpio_dev_t *gpio)
{
    u8_t ret = 0;
    ret = tls_gpio_read(gpio->port);
    tls_gpio_write(gpio->port, !ret);

    return 0;
}

int32_t hal_gpio_input_get(gpio_dev_t *gpio, uint32_t *value)
{
   *value = tls_gpio_read(gpio->port);
   
    return 0;
}

int32_t hal_gpio_enable_irq(gpio_dev_t *gpio, gpio_irq_trigger_t trigger,
                                     gpio_irq_handler_t handler, void *arg)
{

    tls_gpio_isr_register(gpio->port, handler, arg);
    tls_gpio_irq_enable(gpio->port,int_edge_map[trigger]);

    return 0;
}

int32_t hal_gpio_disable_irq(gpio_dev_t *gpio)
{
    tls_gpio_irq_disable(gpio->port);

    return 0;
}

int32_t hal_gpio_clear_irq(gpio_dev_t *gpio)
{
    tls_clr_gpio_irq_status(gpio->port);
    return 0;
}
