 /*            #                                                   
             #@#@                                                   
          @@@#@@@                                                   
          @ `@@                                                     
          @@                                                        
          @@@                                                       
      .    @@                                                       
     @@@@. @@    @@          @@        @@,@;       @@  :@@@@@@@     
    @@@@@@@'@   @@@          #@`      '@  @@       @@  :@   .@@@.   
    @@@@@@@ @:  @@@           @@      @@  @@      `@:  :@`     @@   
    @@@@@@@@@@@@@@@           @@      @+  .@'     @@   :@`      @@  
    @@@@@@@@@@@@@@@           +@`    +@    @@     @@   :@`      @@  
    @@@@@@@@@@@@@@@            @@    @@    @@    `@.   :@`      @@  
    @@@@@@@@@@@@@@@            @@    @'     @#   @@    :@`      #@  
    @@@@@@@@@@@@@@@,           ;@.  +@      @@   @@    :@`      @@  
   @@@@@@@@@@@@@@@@@            @@  @@      #@  `@`    :@`      @@  
   @@@@@@@@@@@@@@@@@@           @@  @:       @# @@     :@`      @@  
  '@@@@@@@@@@@@@@@@@@           ,@,+@        @@ @@     :@`     ;@+  
  '@@@@+@@@@@@@@@@@@@            @@@@        +@`@      :@`    ,@@   
   @@@  @@@@@@ @@@@@#            @@@.         @@@      :@@@@@@@@    
        @@@@@@  :+:              `@@          @@+      `@@@@@@      
        @@@@@@                                                      
        @@@@@@                                                      
 */

/**
 * Valere Versnip Design 
 *
 * @project MultiSensor_bsp
 * @file rgbled.c
 * @author TimB
 * @date 21-mei-2016
 * @brief	Routines for a RGB led.
 *
 * API for RGB led.
 */

/* ***********************************************************************************************************************************************
 * Include Files
 * ***********************************************************************************************************************************************
 */
#include "rgbled.h"
#include "color.h"

/*
 * ***********************************************************************************************************************************************
 * Defines
 * ***********************************************************************************************************************************************
 */


/*
 * ***********************************************************************************************************************************************
 * Local Members
 * ***********************************************************************************************************************************************
 */


/*
 * ***********************************************************************************************************************************************
 * Private Function Prototypes
 * ***********************************************************************************************************************************************
 */


/*
 * ***********************************************************************************************************************************************
 * Private Functions
 * ***********************************************************************************************************************************************
 */




/*
 * ***********************************************************************************************************************************************
 * Public Functions
 * ***********************************************************************************************************************************************
 */


/**
 * RGBLED Init.
 *
 * @param p_rgbled rgbled device
 * @param p_config configuration data
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t RGBLED_Init(rgbled_t *p_rgbled, rgbled_config_t *p_config)
{
	status_t status = status_ok;
	/* populate the struct */
	p_rgbled->id			= p_config->id;
	p_rgbled->p_pwm			= p_config->p_pwm;
	p_rgbled->red_channel	= p_config->red_channel;
	p_rgbled->green_channel	= p_config->green_channel;
	p_rgbled->blue_channel	= p_config->blue_channel;
	p_rgbled->blinkontime	= p_config->blinkontime;
	p_rgbled->blinkofftime	= p_config->blinkofftime;
	p_rgbled->glow_dim_min	= p_config->glow_dim_min;
	p_rgbled->glow_dim_max	= p_config->glow_dim_max;
	p_rgbled->glowinterval	= p_config->glowinterval;
	p_rgbled->glowstep		= p_config->glowstep;
	p_rgbled->color			= p_config->color;
	p_rgbled->dim			= p_config->dim	;

	p_rgbled->timestamp	= 0;
	p_rgbled->state			= rgbled_state_idle;

	/* depending on ledout call correct function */
	switch(p_config->ledout)
	{
	case rgbled_off:
		status = RGBLED_LedOff(p_rgbled);
		break;
	case rgbled_on:
		status = RGBLED_LedOn(p_rgbled, -1, -1);
		break;
	case rgbled_blink:
		status = RGBLED_LedBlink(p_rgbled, -1, -1, -1, -1);
		break;
	case rgbled_glow:
		status = RGBLED_LedGlow(p_rgbled, -1, -1, -1, -1, -1);
		break;
	}
	return status;
}


/**
 * Run0 function for RGBLED.
 *
 * @note This function should be called periodically from higher level routines.
 * @param p_rgbled rgbled device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t RGBLED_Run0(rgbled_t *p_rgbled)
{
	status_t status = status_ok;
	uint32_t currentTime;
	uint8_t dim;

	/* get current time */
	status = SYSTICK_GetTicks(&currentTime);

	/* blinking */
	if(p_rgbled->ledout == rgbled_blink)
	{
		if(p_rgbled->state == rgbled_state_blinkoff)
		{
			if(currentTime - p_rgbled->timestamp >= p_rgbled->blinkofftime)
			{
				status = RGBLED_Update(p_rgbled, p_rgbled->color, p_rgbled->dim);
				p_rgbled->state = rgbled_state_blinkon;
				p_rgbled->timestamp = currentTime;
			}
		}
		else if(p_rgbled->state == rgbled_state_blinkon)
		{
			if(currentTime - p_rgbled->timestamp >= p_rgbled->blinkontime)
			{
				status = RGBLED_Update(p_rgbled, p_rgbled->color, 0);
				p_rgbled->state = rgbled_state_blinkoff;
				p_rgbled->timestamp = currentTime;
			}
		}
	}
	/* glowing */
	else if(p_rgbled->ledout == rgbled_glow)
	{
		/* check if interval has passed */
		if(currentTime - p_rgbled->timestamp >= p_rgbled->glowinterval)
		{
			dim = p_rgbled->currentdim;
			if(p_rgbled->state == rgbled_state_glowup)
			{
				/* check if we are at the maximum */
				if(p_rgbled->glow_dim_max - p_rgbled->currentdim > p_rgbled->glowstep)
				{
					dim += p_rgbled->glowstep;;
				}
				else
				{
					dim = p_rgbled->glow_dim_max;
					p_rgbled->state = rgbled_state_glowdown;
				}
			}
			else if(p_rgbled->state == rgbled_state_glowdown)
			{
				/* check if we are at the minimum */
				if(p_rgbled->currentdim - p_rgbled->glow_dim_min > p_rgbled->glowstep)
				{
					dim -= p_rgbled->glowstep;;
				}
				else
				{
					dim = p_rgbled->glow_dim_min;
					p_rgbled->state = rgbled_state_glowup;
				}
			}

			/* update led */
			status = RGBLED_Update(p_rgbled, p_rgbled->color, dim);
			/* update timestamp */
			p_rgbled->timestamp = currentTime;
		}
	}
	return status;
}

/**
 * Update the rgbled color and dim value.
 *
 * This function sets the rgbled color by setting the pwm.
 * @param p_rgbled rgbled device
 * @param color wanted color (24Bits RGB)
 * @param dimming dimming value 0 - 100
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t RGBLED_Update(rgbled_t *p_rgbled, uint32_t color, uint8_t dimming)
{
	status_t status = status_ok;
	uint8_t r,g,b;

	/* check if dimming is between 0-100 */
	if(dimming > 100)
	{
		status = rgbled_dimvalue;
	}
	if(status == status_ok)
	{
		p_rgbled->color = color;
		p_rgbled->currentdim = dimming;

		/* scale with dimming, and from 255 value to percent */
		r = (uint8_t)((((color >> 16) & 0x00FF)*dimming)/255);
		g = (uint8_t)((((color >> 8) & 0x00FF)*dimming)/255);
		b = (uint8_t)((((color >> 0) & 0x00FF)*dimming)/255);

		status = PWM_setdutycycle(p_rgbled->p_pwm, p_rgbled->red_channel, r);
	}

	if(status == status_ok)
	{
		PWM_setdutycycle(p_rgbled->p_pwm, p_rgbled->green_channel, g);
	}
	if(status == status_ok)
	{
		PWM_setdutycycle(p_rgbled->p_pwm, p_rgbled->blue_channel, b);
	}
	return status;
}


/**
 * Turn off the led.
 *
 * This will in fact only set dim value to 0
 * @param p_rgbled rgbled device
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t RGBLED_LedOff(rgbled_t *p_rgbled)
{
	status_t status = status_ok;
	p_rgbled->ledout = rgbled_off;
	p_rgbled->state = rgbled_state_idle;
	status = RGBLED_Update(p_rgbled, p_rgbled->color, 0);
	return status;
}


/**
 * Turn on the led.
 *
 * The color and dim value (if not -1) will also be updated.
 * If these values are -1, the current color and dim value will be used
 * @param p_rgbled rgbled device
 * @param color -1: don't update, else the wanted color
 * @param dim -1: don't update, else wanted dim value (0-100)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t RGBLED_LedOn(rgbled_t *p_rgbled, int32_t color, int16_t dim)
{
	status_t status = status_ok;
	p_rgbled->ledout = rgbled_on;
	p_rgbled->state = rgbled_state_idle;
	if(color >= 0)
	{
		p_rgbled->color = (uint32_t)color;
	}
	if(dim >= 0)
	{
		p_rgbled->dim = (uint8_t)dim;
	}
	status = RGBLED_Update(p_rgbled, p_rgbled->color, p_rgbled->dim);
	return status;
}


/**
 * Set the led to blinking.
 *
 * The color,dim, blinkon, blinkoff value (if not -1) will also be updated.
 * If these values are -1, the current color and dim and blink timings value will be used
 * @param p_rgbled rgbled device
 * @param color -1: don't update, else the wanted color
 * @param dim -1: don't update, else wanted dim value (0-100)
 * @param blinkon -1: don't update, else wanted blinkon time (in ms)
 * @param blinkoff -1: don't update, else wanted blinkoff time (in ms)
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t RGBLED_LedBlink(rgbled_t *p_rgbled, int32_t color, int16_t dim, int32_t blinkon, int32_t blinkoff)
{
	status_t status = status_ok;
	p_rgbled->ledout = rgbled_blink;
	p_rgbled->state = rgbled_state_blinkoff;
	if(color >= 0)
	{
		p_rgbled->color = (uint32_t)color;
	}
	if(dim >= 0)
	{
		p_rgbled->dim = (uint8_t)dim;
	}
	if(blinkon >= 0)
	{
		p_rgbled->blinkontime = (uint16_t)blinkon;
	}
	if(blinkoff >= 0)
	{
		p_rgbled->blinkofftime = (uint16_t)blinkoff;
	}
	status = RGBLED_Update(p_rgbled, p_rgbled->color, p_rgbled->dim);
	/* get timestamp */
	SYSTICK_GetTicks(&p_rgbled->timestamp);
	return status;
}

/**
 * Set the led to glowing.
 *
 * The color,dim, glow_dim_min, glow_dim_max, glowtime value (if not -1) will also be updated.
 * If these values are -1, the current values will be used
 * @param p_rgbled rgbled device
 * @param color -1: don't update, else the wanted color
 * @param glow_dim_min -1: don't update, else the wanted dim min value
 * @param glow_dim_max -1: don't update, else the wanted dim max value
 * @param glowtime -1: don't update, else the wanted glowtime
 * @return	status_ok if succeeded (otherwise check status.h for details).
 */
status_t RGBLED_LedGlow(rgbled_t *p_rgbled, int32_t color, int16_t glow_dim_min, int16_t glow_dim_max, int32_t glowinterval, int16_t glowstep)
{
	status_t status = status_ok;
	p_rgbled->ledout = rgbled_glow;
	p_rgbled->state = rgbled_state_glowup;
	if(color >= 0)
	{
		p_rgbled->color = (uint32_t)color;
	}
	if(glow_dim_min >= 0)
	{
		p_rgbled->glow_dim_min = (uint8_t)glow_dim_min;
	}
	if(glow_dim_max >= 0)
	{
		p_rgbled->glow_dim_max = (uint8_t)glow_dim_max;
	}
	if(glowinterval >= 0)
	{
		p_rgbled->glowinterval = (uint16_t)glowinterval;
	}
	if(glowstep >= 0)
	{
		p_rgbled->glowstep = (uint8_t)glowstep;
	}
	status = RGBLED_Update(p_rgbled, p_rgbled->color, p_rgbled->glow_dim_min);
	/* get timestamp */
	SYSTICK_GetTicks(&p_rgbled->timestamp);
	return status;
}



/* End of file rgbled.c */
