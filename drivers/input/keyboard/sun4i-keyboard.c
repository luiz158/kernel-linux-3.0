/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
*
* Copyright (c) 2011
*
* ChangeLog
*
*
*/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/keyboard.h>
#include <linux/ioport.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/timer.h> 
#include <mach/sys_config.h>
#include <mach/gpio.h>
#include <linux/suspend.h>
#include <linux/proc_fs.h>

#if defined(CONFIG_HAS_EARLYSUSPEND) || defined(CONFIG_PM)
    #include <linux/pm.h>
    #include <linux/earlysuspend.h>
	#include <linux/power/aw_pm.h>
#endif
//#define  KEY_DEBUG
//#define  KEY_DEBUG_LEVEL2
//#define  PRINT_SUSPEND_INFO

#define INPUT_DEV_NAME	("sun4i-keyboard")

#define  KEY_MAX_CNT  		(13)
 
#define  KEY_BASSADDRESS	(0xf1c22800)
#define  LRADC_CTRL		(0x00)
#define  LRADC_INTC		(0x04)
#define  LRADC_INT_STA 		(0x08)
#define  LRADC_DATA0		(0x0c)
#define  LRADC_DATA1		(0x10)

#define  FIRST_CONCERT_DLY		(2<<24)
#define  CHAN				(0x3)
#define  ADC_CHAN_SELECT		(CHAN<<22)
#define  LRADC_KEY_MODE		(0)
#define  KEY_MODE_SELECT		(LRADC_KEY_MODE<<12)
#define  KEY_SINGLE_MODE_SELECT		(0x01<<12)
#define  LEVELB_VOL			(0<<4)
#define  LEVELA_B_CNT			(12<<8)

#define  LRADC_HOLD_EN			(1<<6)

#define  LRADC_SAMPLE_32HZ		(3<<2)
#define  LRADC_SAMPLE_62HZ		(2<<2)
#define  LRADC_SAMPLE_125HZ		(1<<2)
#define  LRADC_SAMPLE_250HZ		(0<<2)

#define  LRADC_EN			(1<<0)

#define  LRADC_ADC1_UP_EN		(1<<12)
#define  LRADC_ADC1_DOWN_EN		(1<<9)
#define  LRADC_ADC1_DATA_EN		(1<<8)

#define  LRADC_ADC0_UP_EN			(1<<4)
#define  LRADC_ADC0_ALRDY_HOLD_EN		(1<<3)
#define  LRADC_ADC0_HOLD_EN			(1<<2)
#define  LRADC_ADC0_DOWN_EN			(1<<1)
#define  LRADC_ADC0_DATA_EN			(1<<0)

#define  LRADC_ADC1_UPPEND		(1<<12)
#define  LRADC_ADC1_DOWNPEND		(1<<9)
#define  LRADC_ADC1_DATAPEND		(1<<8)

#define  LRADC_ADC0_UPPEND 		(1<<4)
#define  LRADC_ADC0_DOWNPEND		(1<<1)
#define  LRADC_ADC0_DATAPEND		(1<<0)
//
#define EVB
//#define CUSTUM
#define ONE_CHANNEL
#define MODE_0V2
//#define MODE_0V15
//#define TWO_CHANNEL


#define REPORT_START_NUM			(2)
#define REPORT_REPEAT_KEY_BY_INPUT_CORE

//#define REPORT_REPEAT_KEY_FROM_HW
#define INITIAL_VALUE				(0Xff)

#ifdef CONFIG_KEYBOARD_SUN4I_KEYBOARD_XL

static unsigned char keypad_mapindex[64] =
{
    0,0,0,0,0,0,0,0,               //key 1, 8¸ö£¬ 0-7
    0,0,0,0,0,0,0,0,                 //key 2, 7¸ö£¬ 8-14
    1,1,1,1,1,1,1,                 //key 3, 7¸ö£¬ 15-21
    3,3,3,3,3,3,                    //key 4, 6¸ö£¬ 22-27
    4,4,4,4,4,4,                   //key 5, 6¸ö£¬ 28-33
    2,2,2,2,2,2,2,                   //key 6, 6¸ö£¬ 34-39
    2,2,2,2,2,2,2,           //key 7, 10¸ö£¬40-49
    7,7,7,7,7,7,7,7,7,7,7,7,7,7    //key 8, 17¸ö£¬50-63
};

#else

//#ifdef MODE_0V2
//standard of key maping
//0.2V mode	 


static unsigned char keypad_mapindex[64] =
{
    0,0,0,0,0,0,0,0,               //key 1, 8¸ö£¬ 0-7
    1,1,1,1,1,1,1,                 //key 2, 7¸ö£¬ 8-14
    2,2,2,2,2,2,2,                 //key 3, 7¸ö£¬ 15-21
    3,3,3,3,3,3,                    //key 4, 6¸ö£¬ 22-27
    4,4,4,4,4,4,                   //key 5, 6¸ö£¬ 28-33
    5,5,5,5,5,5,                   //key 6, 6¸ö£¬ 34-39
    6,6,6,6,6,6,6,6,6,6,           //key 7, 10¸ö£¬40-49
    7,7,7,7,7,7,7,7,7,7,7,7,7,7    //key 8, 17¸ö£¬50-63
};
#endif
                        
#ifdef MODE_0V15
//0.15V mode
static unsigned char keypad_mapindex[64] =
{
	0,0,0,                      //key1
	1,1,1,1,1,                  //key2
	2,2,2,2,2,
	3,3,3,3,
	4,4,4,4,4,
	5,5,5,5,5,
	6,6,6,6,6,
	7,7,7,7,
	8,8,8,8,8,
	9,9,9,9,9,
	10,10,10,10,
	11,11,11,11,
	12,12,12,12,12,12,12,12,12,12 //key13
};
#endif

#ifdef EVB
static unsigned int sun4i_scankeycodes[KEY_MAX_CNT]=
{
#ifdef CONFIG_ANDROID
	[0 ] = KEY_HOME,	/* Middle button */
	[1 ] = KEY_RIGHT,	/* Right button  */
	[2 ] = KEY_LEFT,	/* Left button   */
#else
	[0 ] = KEY_MENU,	/* Middle button */
	[1 ] = KEY_NEXT,	/* Right button  */
	[2 ] = KEY_BACK,	/* Left button   */
#endif
	[3 ] = KEY_RESERVED,
	[4 ] = KEY_RESERVED,
	[5 ] = KEY_RESERVED,
	[6 ] = KEY_RESERVED,
	[7 ] = KEY_RESERVED,
	[8 ] = KEY_RESERVED,
	[9 ] = KEY_RESERVED,
	[10] = KEY_RESERVED,
	[11] = KEY_RESERVED,
	[12] = KEY_RESERVED,
};
#endif
#ifdef CONFIG_PM
struct dev_power_domain keyboard_pm_domain;
#else
#ifdef CONFIG_HAS_EARLYSUSPEND	
struct sun4i_keyboard_data {
    struct early_suspend early_suspend;
};
#endif
#endif

static volatile unsigned int key_val;
static struct input_dev *sun4ikbd_dev;
static unsigned char scancode;

static unsigned int key_reg;
static unsigned int key_int_reg;
static unsigned char suspend_flag = 0;
static unsigned char key_cnt = 0;
static unsigned char cycle_buffer[REPORT_START_NUM] = {0};
static unsigned char transfer_code = INITIAL_VALUE;
static unsigned char judge_flag = 0;
static unsigned char resume_by_btn = 0;
static unsigned char resume_key_dwn = 0;
static unsigned char resume_key_up = 0;
static unsigned int key_val_resume = 0;

#ifdef KEY_DEBUG
static unsigned long j_time1,j_time2;
#endif

#ifdef CONFIG_PM
#else
#ifdef CONFIG_HAS_EARLYSUSPEND
static struct sun4i_keyboard_data *keyboard_data;
#endif
#endif

#if 1
//hxm add GPIO key

static int touchkey3=3;
static int touchkey4=4;


static u32 hdle2;
static u32 hdle3;

extern int gpioKey_to_Vomdebug;

static int button_status;
static struct proc_dir_entry *kb_proc_entry;

static int button_proc_read(char *page, char **start, off_t off, int count,
			    int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d", button_status);

	return len;
}


static int sun4i_keyboard_sysfs_init(void)
{
	// TODO : put this into /sys/class instead
	kb_proc_entry = create_proc_entry("sun4i_button", 0444, NULL);
	kb_proc_entry->read_proc = button_proc_read;

	return 0;
}

static int button_change_status(int status)
{
	if (button_status == status)
		return 0;

	button_status = status;

	return 0;
}

static irqreturn_t GPIOKEY_EINT_Handler2(int irq, void *dev_id)	
{
	int reg_val = 0;
	//printk("shy  gpio GPIOKEY_EINT_Handler2\n");
   	reg_val=sw_gpio_eint_get_irqpd_sta(hdle2);
	
   	// printk("shy1109  GPIOKEY_EINT_Handler2  reg_val=%d\n",reg_val);
   	 if(1==reg_val)
	{
	      // printk("shy go in GPIOKEY_EINT_Handler2\n");


		sw_gpio_eint_clr_irqpd_sta(hdle2);
		//if(g_pm_state==3)
    	//{
    	 //printk("hxm key GPIO GPIOKEY_EINT_Handler2 g_pm_state==3\n");
    	//}
		//else
		//{
		input_report_key(sun4ikbd_dev, sun4i_scankeycodes[3], 1);
		input_sync(sun4ikbd_dev);
		input_report_key(sun4ikbd_dev, sun4i_scankeycodes[3], 0);
		input_sync(sun4ikbd_dev);
		//}
	}
	else		
	{
		printk("Other Interrupt shy 2\n");
		
		return IRQ_NONE;
	}

	
	return IRQ_HANDLED;	
}


static irqreturn_t GPIOKEY_EINT_Handler3(int irq, void *dev_id)	
{
	int reg_val = 0;
   	reg_val=sw_gpio_eint_get_irqpd_sta(hdle3);
	
  	 // printk("shy 1109  GPIOKEY_EINT_Handler3  reg_val=%d\n",reg_val);
       if(1==reg_val)
	{	
//	       printk("shy go in GPIOKEY_EINT_Handler3\n");

		sw_gpio_eint_clr_irqpd_sta(hdle3);
		
//		printk("shy go in GPIOKEY_EINT_Handler5\n");
		//if(g_pm_state==3)
    	//{
    	// printk("hxm key GPIO GPIOKEY_EINT_Handler3 g_pm_state==3\n");
    	//}
		//else
		//{
		input_report_key(sun4ikbd_dev, sun4i_scankeycodes[8], 1);
		input_sync(sun4ikbd_dev);
		input_report_key(sun4ikbd_dev, sun4i_scankeycodes[8], 0);
		input_sync(sun4ikbd_dev);
		//}
	}	    
	else		
	{
		printk("Other Interrupt shy 3\n");
		
		return IRQ_NONE;
	}

	
	return IRQ_HANDLED;
}

static int __init gpio_eint_test_init(void)
{
    int pending;

    //PG9

    hdle2 = sw_gpio_irq_request("GPIOKEY_para", "GPIO_KEY_THREE", TRIG_EDGE_POSITIVE);
    if (!hdle2) {
        printk("request gpio irq failed\n");
        return -1;
    }

    sw_gpio_eint_set_enable(hdle2, 1);
    pending = sw_gpio_eint_get_irqpd_sta(hdle2);
    if (pending < 0) {
        printk("get irq pending failed\n");
    }
	sw_gpio_eint_clr_irqpd_sta(hdle2);


	 //PG10    
    hdle3 = sw_gpio_irq_request("GPIOKEY_para", "GPIO_KEY_FOUR", TRIG_EDGE_POSITIVE);
    if (!hdle3) {
        printk("request gpio irq failed\n");
        return -1;
    }

    sw_gpio_eint_set_enable(hdle3, 1);
    pending = sw_gpio_eint_get_irqpd_sta(hdle3);
    if (pending < 0) {
        printk("get irq pending failed\n");
    }
	sw_gpio_eint_clr_irqpd_sta(hdle3);

    return 0;
}

static int __exit gpio_eint_test_exit(void)
{
	//printk("shy  exit [%s] called\n", __func__);

	sw_gpio_eint_set_enable(hdle2, 0);
	sw_gpio_irq_free(hdle2);

	sw_gpio_eint_set_enable(hdle3, 0);
	sw_gpio_irq_free(hdle3);

	return 0;
}

static int  GpioKey_probe()
{
	int err = -1;
	int device_used = -1;
	if (SCRIPT_PARSER_OK != (err = script_parser_fetch("GPIOKEY_para", "GPIOKEY_used", &device_used, 1))) {
		pr_err("%s: hxm@@@ script_parser_fetch err.ret = %d. \n", __func__, err);
		return device_used;
	}

	if (device_used){
		pr_err("%s: hxm GPIOKEY__used. \n",  __func__);
	} else{
		pr_err("%s: hxm GPIOKEY__unused. \n",  __func__);
		return device_used ;
	}

//	elan_wq = create_singlethread_workqueue("elan_wq");
//	INIT_WORK(&Gpio_work, gpio_work_func);

	err = request_irq(SW_INT_IRQNO_PIO, GPIOKEY_EINT_Handler2, IRQF_TRIGGER_RISING|IRQF_SHARED  ,"gpiokey3", &touchkey3);
	if (err < 0) {
		printk( "GPIO KEY: request irq failed\n");
		return err;
	}

	err = request_irq(SW_INT_IRQNO_PIO, GPIOKEY_EINT_Handler3, IRQF_TRIGGER_RISING|IRQF_SHARED  ,"gpiokey4", &touchkey4);
	if (err < 0) {
		printk( "GPIO KEY: request irq failed\n");
		return err;
	}

	gpio_eint_test_init();

	return 0;
}
#endif

//end

#ifdef CONFIG_PM
static int sun4i_keyboard_suspend(struct device *dev)
{
#ifdef PRINT_SUSPEND_INFO
	printk("sun4i_keyboard_suspend enter\n");
#endif
	suspend_flag = 1;

	if (g_suspend_state == PM_SUSPEND_MEM)
		key_int_reg = readl(KEY_BASSADDRESS + LRADC_INTC);

	key_reg = readl(KEY_BASSADDRESS + LRADC_CTRL);
	writel(key_reg|KEY_SINGLE_MODE_SELECT,KEY_BASSADDRESS + LRADC_CTRL);

	if (g_suspend_state == PM_SUSPEND_PARTIAL || g_suspend_state == PM_SUSPEND_STANDBY)
		standby_wakeup_event |= SUSPEND_WAKEUP_SRC_KEY;

	resume_by_btn = 0;
	resume_key_up = 0;
	resume_key_dwn = 0;

	return 0;
}

static int  sun4i_keyboard_resume(struct device *dev)
{

	unsigned int  reg_val;
#ifdef PRINT_SUSPEND_INFO
	printk("sun4i_keyboard_resume enter\n");
#endif
#ifdef KEY_DEBUG
	printk("sun4ibtn values : resume_by_btn = %d, resume_key_up = %d, resume_key_dwn = %d, key_val = %d  \n",resume_by_btn,resume_key_up,resume_key_dwn,key_val_resume);
#endif
	if (g_suspend_state == PM_SUSPEND_MEM)
		writel(key_int_reg, KEY_BASSADDRESS + LRADC_INTC);

	if (resume_by_btn == 1) {
		scancode = keypad_mapindex[key_val_resume&0x3f];
		if (resume_key_up == 1 && resume_key_dwn == 1) {
#ifdef KEY_DEBUG
			printk("sun4ikbd Key resume debug : enter simulation press and release\n");
#endif
			if (key_val_resume == 18 || key_val_resume == 11 || key_val_resume == 50) {
				input_report_key(sun4ikbd_dev, sun4i_scankeycodes[scancode], 1);
				input_sync(sun4ikbd_dev);
				input_report_key(sun4ikbd_dev, sun4i_scankeycodes[scancode], 0);
				input_sync(sun4ikbd_dev);
			} else {
				input_report_key(sun4ikbd_dev, sun4i_scankeycodes[0], 1);
				input_sync(sun4ikbd_dev);
				input_report_key(sun4ikbd_dev, sun4i_scankeycodes[0], 0);
				input_sync(sun4ikbd_dev);
			}
		} else if (resume_key_up == 0 && resume_key_dwn == 1) {
#ifdef KEY_DEBUG
			printk("sun4ikbd Key resume debug partial key press\n");
#endif
			writel(key_reg, KEY_BASSADDRESS + LRADC_CTRL);
		}
	}

	writel(key_reg,KEY_BASSADDRESS + LRADC_CTRL);

	resume_by_btn = 0;
	resume_key_up = 0;
	resume_key_dwn = 0;
	suspend_flag = 0;

	return 0;
}

#else

#ifdef CONFIG_HAS_EARLYSUSPEND
static void sun4i_keyboard_suspend(struct early_suspend *h)
{
#ifdef PRINT_SUSPEND_INFO
	printk("sun4i_keyboard_suspend enter\n");
#endif
	suspend_flag = 1;
	key_reg = readl(KEY_BASSADDRESS + LRADC_CTRL);
	writel(key_reg|KEY_SINGLE_MODE_SELECT,KEY_BASSADDRESS + LRADC_CTRL);

	if (g_suspend_state == PM_SUSPEND_PARTIAL || g_suspend_state == PM_SUSPEND_STANDBY)
		standby_wakeup_event |= SUSPEND_WAKEUP_SRC_KEY;

	return ;
}

static void sun4i_keyboard_resume(struct early_suspend *h)
{
#ifdef PRINT_SUSPEND_INFO
	printk("sun4i_keyboard_resume enter\n");
#endif
	writel(key_reg,KEY_BASSADDRESS + LRADC_CTRL);

	return ; 
}
#endif
#endif

static irqreturn_t sun4i_isr_key(int irq, void *dummy)
{
	unsigned int  reg_val;

	int loop = 0;

	reg_val  = readl(KEY_BASSADDRESS + LRADC_INT_STA);

#ifdef KEY_DEBUG_LEVEL2
	printk("sun4ikbd Key Interrupt\n");
#endif

	if (reg_val & LRADC_ADC0_DOWNPEND) {
#ifdef KEY_DEBUG
		//printk("sun4ikbd key down\n");
		j_time1 = jiffies;
#endif
		resume_key_dwn = 1;
		judge_flag = 0;
	}
	if (g_resume_keyval != 0x3f) {

		if (g_resume_keyval < 0x16) {
			
				judge_flag = 1;
				scancode = keypad_mapindex[g_resume_keyval&0x3f];
				input_report_key(sun4ikbd_dev, sun4i_scankeycodes[scancode], 1);
				input_sync(sun4ikbd_dev);
		}
		g_resume_keyval = 0x3f;
	}
		

	if ( reg_val & LRADC_ADC0_DATAPEND ){

		key_val = readl(KEY_BASSADDRESS + LRADC_DATA0);


		if ( !judge_flag ) {

#ifdef CONFIG_KEYBOARD_SUN4I_KEYBOARD_SARAIVA
			if (key_val == 0x05)
					button_change_status(1);
#else
			if (key_val == 0x07)
					button_change_status(1);
#endif

			if (key_val < 0x30) {
				judge_flag = 1;
				scancode = keypad_mapindex[key_val&0x3f];
				input_report_key(sun4ikbd_dev, sun4i_scankeycodes[scancode], 1);
				input_sync(sun4ikbd_dev);
			}			
		}
		else {
				input_report_key(sun4ikbd_dev, sun4i_scankeycodes[scancode], 1);
				input_sync(sun4ikbd_dev);
			
		}
	}

	if (reg_val & LRADC_ADC0_UPPEND) {

		if ( judge_flag ) {

#ifdef KEY_DEBUG_LEVEL2
			printk("report data: key_val :%8d transfer_code: %8d \n",key_val, transfer_code);
#endif
			input_report_key(sun4ikbd_dev, sun4i_scankeycodes[scancode], 0);
			input_sync(sun4ikbd_dev);
		}

		button_change_status(0);
#ifdef KEY_DEBUG
		printk("sun4ikbd key up \n");
		j_time2 = jiffies;
		printk("sun4ikbd time pressed %ldms \n",((long)j_time2-(long)j_time1)*1000/HZ);
#endif

		resume_key_up = 1;
		judge_flag = 0;
		transfer_code = INITIAL_VALUE;
		
	}

	writel(reg_val,KEY_BASSADDRESS + LRADC_INT_STA);
	return IRQ_HANDLED;
}

/*************************add led test**************************/
#include "../../video/sun5i/disp/OSAL/OSAL_Pin.h"
static int  Led_Init_Para()
{	
	 int err = -1;
     int device_used = -1;
	//printk("%s %s %d \n",__FILE__,__func__,__LINE__);
	if(SCRIPT_PARSER_OK != (err = script_parser_fetch("LED_para", "LED_used", &device_used, 1)))
	{
		pr_err("%s: hxm@@@ script_parser_fetch err.ret = %d. \n", __func__, err);
		return device_used;
	}	
	if(1 == device_used){
		pr_err("%s: hxm GPIOKEY__used. \n",  __func__);	
	}	
	else{
		pr_err("%s: hxm GPIOKEY__unused. \n",  __func__); 
		return device_used ;
	}	

	user_gpio_set_t gpio_info;
	__hdle gpio_hd;
    if(OSAL_Script_FetchParser_Data("LED_para", "GPIO_LED_PIN", (int *)&gpio_info,sizeof(user_gpio_set_t)/sizeof(int)) < 0)
    {
        __wrn("LED_para GPIO_LED_PIN fail\n");
        return -1;
    }
	gpio_hd = OSAL_GPIO_Request(&gpio_info,1);
	gpio_info.mul_sel = 1;
	gpio_info.pull = 1;
	gpio_info.drv_level = 2;
	gpio_info.data = 0;
	gpio_set_one_pin_status(gpio_hd,&gpio_info,"GPIO_LED_PIN",1);


	return gpio_hd;
}

static int __init sun4ikbd_init(void)
{
	int i;
	int err =0;
	unsigned int IntPrioReg;

	// Led_Init_Para();

	sun4ikbd_dev = input_allocate_device();
	if (!sun4ikbd_dev) {
		printk(KERN_ERR "sun4ikbd: not enough memory for input device\n");
		err = -ENOMEM;
		goto fail1;
	}

	sun4ikbd_dev->name = INPUT_DEV_NAME;  
	sun4ikbd_dev->phys = "sun4ikbd/input0"; 
	sun4ikbd_dev->id.bustype = BUS_HOST;      
	sun4ikbd_dev->id.vendor = 0x0001;
	sun4ikbd_dev->id.product = 0x0001;
	sun4ikbd_dev->id.version = 0x0100;

#ifdef REPORT_REPEAT_KEY_BY_INPUT_CORE
	sun4ikbd_dev->evbit[0] = BIT_MASK(EV_KEY)|BIT_MASK(EV_REP);
	printk("REPORT_REPEAT_KEY_BY_INPUT_CORE is defined, support report repeat key value. \n");
#else
	sun4ikbd_dev->evbit[0] = BIT_MASK(EV_KEY);
#endif

	for (i = 0; i < KEY_MAX_CNT; i++)
		set_bit(sun4i_scankeycodes[i], sun4ikbd_dev->keybit);
	
#ifdef ONE_CHANNEL
	IntPrioReg = readl(SW_INT_SRCPRIO_REG1);
	writel(IntPrioReg|(0x3<<30), SW_INT_SRCPRIO_REG1);

	writel(LRADC_ADC0_DOWN_EN|LRADC_ADC0_UP_EN|LRADC_ADC0_DATA_EN,KEY_BASSADDRESS + LRADC_INTC);
	writel(FIRST_CONCERT_DLY|LEVELB_VOL|KEY_MODE_SELECT|LRADC_HOLD_EN|ADC_CHAN_SELECT|LRADC_SAMPLE_250HZ|LRADC_EN,KEY_BASSADDRESS + LRADC_CTRL);
	//writel(FIRST_CONCERT_DLY|LEVELB_VOL|KEY_MODE_SELECT|ADC_CHAN_SELECT|LRADC_SAMPLE_62HZ|LRADC_EN,KEY_BASSADDRESS + LRADC_CTRL);

#else
#endif

	if (request_irq(SW_INT_IRQNO_LRADC, sun4i_isr_key, IRQF_TRIGGER_FALLING|IRQF_TIMER|IRQF_NO_SUSPEND|IRQF_FORCE_RESUME, "sun4ikbd", &sun4ikbd_dev)){
		err = -EBUSY;
		printk("request irq failure. \n");
		goto fail2;
	}

	err = input_register_device(sun4ikbd_dev);
	if (err)
		goto fail3;

#ifdef CONFIG_PM
	keyboard_pm_domain.ops.suspend = sun4i_keyboard_suspend;
	keyboard_pm_domain.ops.resume = sun4i_keyboard_resume;
	sun4ikbd_dev->dev.pwr_domain = &keyboard_pm_domain;
#else
#ifdef CONFIG_HAS_EARLYSUSPEND	
	printk("==register_early_suspend =\n");
	keyboard_data = kzalloc(sizeof(*keyboard_data), GFP_KERNEL);
	if (keyboard_data == NULL) {
		err = -ENOMEM;
		goto err_alloc_data_failed;
	}

	keyboard_data->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 3;
	keyboard_data->early_suspend.suspend = sun4i_keyboard_suspend;
	keyboard_data->early_suspend.resume = sun4i_keyboard_resume;
	register_early_suspend(&keyboard_data->early_suspend);
#endif
#endif
	// GpioKey_probe();

	sun4i_keyboard_sysfs_init();

	return 0;

#ifdef CONFIG_PM
#else
#ifdef CONFIG_HAS_EARLYSUSPEND
 err_alloc_data_failed:
#endif
#endif
 fail3:	
	free_irq(SW_INT_IRQNO_LRADC, sun4i_isr_key);
 fail2:	
	input_free_device(sun4ikbd_dev);
 fail1:
     ;
#ifdef KEY_DEBUG
	printk("sun4ikbd_init failed. \n");
#endif

 return err;
}

static void __exit sun4ikbd_exit(void)
{
#ifdef CONFIG_PM
#else
#ifdef CONFIG_HAS_EARLYSUSPEND	
	 unregister_early_suspend(&keyboard_data->early_suspend);
#endif
#endif
	free_irq(SW_INT_IRQNO_LRADC, sun4i_isr_key);
	input_unregister_device(sun4ikbd_dev);
//	gpio_eint_test_exit();
}

module_init(sun4ikbd_init);
module_exit(sun4ikbd_exit);

MODULE_AUTHOR(" <@>");
MODULE_DESCRIPTION("sun4i-keyboard driver");
MODULE_LICENSE("GPL");

