/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "stdio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

DAC_HandleTypeDef hdac;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_DAC_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE BEGIN PFP */
uint32_t previousMillis=0,previousMillis1 = 0,previousMillis2 = 0;
uint32_t currentMillis = 0,currentMillis1 = 0,currentMillis2 = 0;
char Sensor_Fail=0;

int i = 0,setcount =0, j = 0, Adresscounter=254,First_Adresscounter=0,Second_Adresscounter=0,Thrid_Adresscounter=0,current_digit,Segment_Position=3;
int First_AdcVal=0,Second_AdcVal=0,Thrid_AdcVal=0,Fourth_AdcVal=0;
int First_I2cVal=0,Second_I2cVal=0,Thrid_I2cVal=0,Fourth_I2cVal=0;
int Running_Application=0,Lockbit=0;
int h_Thrid_Alarmcounter=0,h_Second_Alarmcounter=0,h_First_Alarmcounter=0,High_Alarmcounter = 70,Set_LowAlarm=0;//Alarm Variables
int lw_Thrid_Alarmcounter=0,lw_Second_Alarmcounter=0,lw_First_Alarmcounter=0,Low_Alarmcounter=25;
int Version_Buff[23]={14,13,9,9,1,2,3,4,15,1,0,2,16,13,5,5,11,2,5,5};
int timer_control=0,With_TimerControl=0,Secondary_EntryApp=0;
uint8_t RxData[256];
uint8_t TxData[256];

typedef enum{
	PowerOnStage=0,
	MenuSelectionStage,
	SubMenuSelectionStage,
	Address_Setting,
	Zero_PointCalbiration,
	Gas_Calibration,
	mA_Calibration,
	h_Alarm_Setting,
	lw_Alarm_Setting,
	Life_limitSetting,
	Self_calibration,
	Exit_menu,
	Run_App,
}_stage;
_stage stage;
int Second_stage =1;
void Power_on_dispaly(){
   if(i<=8){
    Gas_IndexMenu();
   }
   else if((i>=8) && (i<=16)){
	Default_Address();

   }
   else if((i>=16) && (i<=24)){
    Version_Info();
   }
   else if((i>=24) && (i<=32)){
	Range_IndexMenu();}
   else if((i>=32) && (i<=40)){
	HighLow_AlarmMenu();}
	}
/* USER CODE END 0 */
int mapValue(int value) {
	const int minInput = 19680;   // 0 ppm at 1.23V VOUT
	const int maxInput = 27360;   // 100 ppm at 1.71V VOUT
    //const int minInput = 26214;//17650;//8186;
    //const int maxInput = 36160;//32767;//65535;
    const int minOutput = 0;
    const int maxOutput = 100;

    // Ensure the value is within the specified range
    if (value < minInput) {
        value = minInput;
    } else if (value > maxInput) {
        value = maxInput;
    }

    // Map the value to the output range
    int mappedValue = ((value - minInput) * (maxOutput - minOutput)) / (maxInput - minInput) + minOutput;

    return mappedValue;
}
int DAC_mapValue(int value) {
    const int minInput = 0;
    const int maxInput = 100;
    const int minOutput = 819;
    const int maxOutput = 3970;//3220;

    // Ensure the value is within the specified range
    if (value < minInput) {
        value = minInput;
    } else if (value > maxInput) {
        value = maxInput;
    }

    // Map the value to the output range
    int mappedValue = ((value - minInput) * (maxOutput - minOutput)) / (maxInput - minInput) + minOutput;
    return mappedValue;
}
/* USER CODE END PFP */
/* USER CODE END PFP */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  if(htim->Instance == TIM3)
  {
	  i++;
if(i>=41)
	i=0;

if(With_TimerControl==0)
       timer_control++;
if(timer_control>750){
   Secondary_EntryApp=1;
    timer_control=0;
}

  }
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

  currentMillis = HAL_GetTick();
  currentMillis1 = HAL_GetTick();
  currentMillis2 = HAL_GetTick();

  	  if (GPIO_Pin == GPIO_PIN_11 && (currentMillis - previousMillis > 10))
  	  {
  		if(Lockbit==0)
  		Second_stage++;

  		if(stage ==Address_Setting){
  			Adresscounter++;
  		}


  		 if(stage ==h_Alarm_Setting){
  			High_Alarmcounter++;
  		  		}
  		 if(stage ==lw_Alarm_Setting){
  	  		  			Low_Alarmcounter++;
  	  		  		  		  		}
  		if(Adresscounter>=255)
  		  			Adresscounter =255;

  		if(High_Alarmcounter>=100)
  			High_Alarmcounter =100;

  		if(Low_Alarmcounter>=100)
  			Low_Alarmcounter =100;

  		previousMillis = currentMillis;
  	  }
  	  else if (GPIO_Pin == GPIO_PIN_12 && (currentMillis1 - previousMillis1 > 10))
  		{

  		if(Lockbit==0){
  		  /*if((Second_stage >=1)){
  						  Second_stage -=1;
  						  }
  						  else {
  							  Second_stage=0;
  						  }*/
  			Second_stage -=1;
  			  		  			if((Second_stage <=0)){
  			  		  				Second_stage=1;
  			  		  			}

  		}


  		if(stage ==Address_Setting){

  			Adresscounter--;
  		  		}

  		 if(stage ==h_Alarm_Setting){
  			High_Alarmcounter--;
  		  		  		}
  		 if(stage ==lw_Alarm_Setting){
  		  			Low_Alarmcounter--;
  		  		  		  		}
  		if(Adresscounter <=0)
  			Adresscounter=0;

  		if(High_Alarmcounter <=0)
  			High_Alarmcounter=0;

  		if(Low_Alarmcounter <=0)
  		  	Low_Alarmcounter=0;

  		previousMillis1 = currentMillis1;

  		}
  	 else if (GPIO_Pin == GPIO_PIN_10 && (currentMillis2 - previousMillis2 > 10))
  	  		{
  		if((Running_Application ==1)||(Secondary_EntryApp==1)){
  		   			Running_Application=0;
  		   			Secondary_EntryApp=0;
  		   			With_TimerControl=0;
  		   		}
  		switch (setcount){
  		case 0:
  			stage =MenuSelectionStage;
  			Lockbit=0;
  		break;
  		case 1:

 	  		  if(Second_stage==1){
 	  			stage =Address_Setting;

 	  		  }
 	  		  else if(Second_stage==2){
 	  		 	  			stage =Zero_PointCalbiration;

 	  		 	  		  }
			else if(Second_stage==5){
			stage =h_Alarm_Setting;

			}
			else if(Second_stage==6){
			stage =lw_Alarm_Setting;

			}

			else if(Second_stage==9){
			stage =Exit_menu;
			Running_Application=1;

			}
  		    Lockbit=1;
  			break;

  		}
  		setcount++;

  		if(setcount>=2){
  			setcount =0;

  		}

  		   previousMillis2 = currentMillis2;
  	 }
}
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//uint16_t ReadMCP3425(uint8_t channel, uint8_t gain)
uint16_t ReadMCP3425(uint8_t channel, uint8_t gain_setting)  //10/9/25
{
 // uint8_t command = 0x90 | (channel << 5) | (gain << 1);
	 //uint8_t command = 0x98 ;
	uint8_t command = 0x98 | (gain_setting & 0x03);  // Add gain bits (0-3)//10/9/25
     uint8_t data[3] = {0};

  // Send configuration command to MCP3425
  if (HAL_I2C_Master_Transmit(&hi2c1, 0xD0, &command, 1, HAL_MAX_DELAY) != HAL_OK)
  {

    Sensor_Fail=1;
    return 0;
  }
    //HAL_Delay(2); // Delay for 1 second
  // Read ADC data from MCP3425
  if (HAL_I2C_Master_Receive(&hi2c1, 0xD1, data, 3, HAL_MAX_DELAY) != HAL_OK)
  {
    Sensor_Fail=1;
    return 0;
  }

  // Combine received bytes into a 16-bit value
  uint16_t result = (data[0] << 8) | data[1];

  // The last b yte contains configuration and status information; you can extract it if needed

  return result;
}
void Reading_Flash(){
	Adresscounter = Flash_Read_NUM(0x08006010);
	High_Alarmcounter= Flash_Read_NUM(0x08005C10);
	Low_Alarmcounter=Flash_Read_NUM(0x08012000);

}

void Write_Flash(){
	Flash_Write_NUM(0x08006010, Adresscounter);
	Flash_Write_NUM(0x08005C10, High_Alarmcounter);
	Flash_Write_NUM(0x08012000, Low_Alarmcounter);

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint16_t I2c_adcValue=0;
	int Ppm_Val =0;
	uint32_t dac_value=0;
    uint8_t Mode_data_to_send[] = {0x12,0x03}; // Data to be sent
    uint8_t TIACN_data_to_send[] = {0x10,0x1B};//{0x10,0x19}; // Data to be sent
    uint8_t REFCN_data_to_send[] = {0x11,0xA0}; // Data to be sent
    uint8_t LOCK_data_to_send[] = {0x01,0x00}; // Data to be sent
    uint16_t device_address = 0x90; // 7-bit address, left-shifted by 1 for write
    uint8_t Mode_data_received[3]={0}; // Buffer to store received data
    uint8_t TIACN_data_received[3]={0}; // Buffer to store received data
    uint8_t REFCN_data_received[3]={0}; // Buffer to store received data
    uint16_t mode_data_size = sizeof(Mode_data_received);
    uint16_t TIACN_data_size = sizeof(TIACN_data_received);
    uint16_t REFCN_data_size = sizeof(REFCN_data_received);
    static char Memory_lock=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  Seven_SegmentInit();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_DAC_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */
  // Start DAC output
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
  /* USER CODE END SysInit */
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);//Controlling the Txand Rx for Communication

     uint8_t txData[] = "Hello, Decteor/transmiter Starting the application !\r\n";
     HAL_UART_Transmit(&huart1, txData, sizeof(txData) - 1, HAL_MAX_DELAY);

     //i2c
     if (HAL_I2C_Master_Transmit(&hi2c1, device_address, Mode_data_to_send, sizeof(Mode_data_to_send), HAL_MAX_DELAY) != HAL_OK)
     {
    	 Sensor_Fail=1;
       Error_Handler();

     }

     if (HAL_I2C_Master_Receive(&hi2c1, device_address, Mode_data_received, mode_data_size, HAL_MAX_DELAY) != HAL_OK)
      {
    	 Sensor_Fail=1;
        Error_Handler();

      }

     /////

     // Transmit data
          if (HAL_I2C_Master_Transmit(&hi2c1, device_address,LOCK_data_to_send , sizeof(LOCK_data_to_send), HAL_MAX_DELAY) != HAL_OK)
          {
        	  Sensor_Fail=1;
            Error_Handler();

          }

          ///////
     // Transmit data
     if (HAL_I2C_Master_Transmit(&hi2c1, device_address, TIACN_data_to_send, sizeof(TIACN_data_to_send), HAL_MAX_DELAY) != HAL_OK)
     {
    	 Sensor_Fail=1;
       Error_Handler();

     }

     if (HAL_I2C_Master_Receive(&hi2c1, device_address, TIACN_data_received, TIACN_data_size, HAL_MAX_DELAY) != HAL_OK)
      {
    	 Sensor_Fail=1;
        Error_Handler();

      }
     ///

     if (HAL_I2C_Master_Transmit(&hi2c1, device_address,REFCN_data_to_send , sizeof(REFCN_data_to_send), HAL_MAX_DELAY) != HAL_OK)
     {
    	 Sensor_Fail=1;
       Error_Handler();

     }


     if (HAL_I2C_Master_Receive(&hi2c1, device_address, REFCN_data_received, REFCN_data_size, HAL_MAX_DELAY) != HAL_OK)
      {
    	 Sensor_Fail=1;
        Error_Handler();

      }
    __HAL_I2C_DISABLE(&hi2c1);

    Reading_Flash();
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 3220);
  while (1)
  {
#if 1
    /* USER CODE END WHILE */
	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

	  if(Second_stage>=10)

	   		   		  Second_stage=1;
	  /*serial display code added on 11/9/25//
	  static uint32_t uart_test = 0;
	      if(HAL_GetTick() - uart_test > 2000) { // Every 2 seconds
	          char test[] = "MAIN_LOOP_WORKING\r\n";

	          // RS485 direction control (like startup message)
	          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // TX mode
	          HAL_Delay(2);
	          HAL_UART_Transmit(&huart1, (uint8_t*)test, strlen(test), 100);
	          HAL_Delay(2);

	          uart_test = HAL_GetTick();
	      }
	      // 📊 SENSOR SERIAL OUTPUT - Add this new section
	          static uint32_t sensor_timer = 0;
	          if(HAL_GetTick() - sensor_timer > 3000) {  // Every 3 seconds
	              // Read sensor regardless of mode
	              uint16_t I2c_adcValue = ReadMCP3425(0, 0);
	              int Ppm_Val = mapValue(I2c_adcValue);
	              uint32_t dac_value = DAC_mapValue(Ppm_Val);

	              char sensor_msg[60];
	              sprintf(sensor_msg, "ADC:%d PPM:%d DAC:%lu Stage:%d\r\n",
	                      I2c_adcValue, Ppm_Val, dac_value, (int)stage);

	              HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	              HAL_Delay(2);
	              HAL_UART_Transmit(&huart1, (uint8_t*)sensor_msg, strlen(sensor_msg), 100);
	              HAL_Delay(2);

	              sensor_timer = HAL_GetTick();
	          }*/


	  if((Running_Application ==1 )||(Secondary_EntryApp==1))
	  	  			 {
	  		  stage=Run_App;
			   // Read analog value from MCP3425 channel 0 with 16x gain
			 	    //I2c_adcValue = ReadMCP3425(0, 2); // Change the channel and gain as needed
			 	   I2c_adcValue = ReadMCP3425(0, 0); // Change the channel and gain as needed 10/9/25
			 	   if(Memory_lock==0){
			 	   			   Write_Flash();
			 	   			   Memory_lock=1;
			 	   		   }

					Ppm_Val=mapValue(I2c_adcValue);
					//Ppm_Val =25;
					Thrid_I2cVal =( (Ppm_Val / 100) % 10 );   // prepare to display digit 2
					Second_I2cVal =( (Ppm_Val / 10) % 10 );   // prepare to display digit 3
					First_I2cVal =(Ppm_Val % 10);   // prepare to display digit 4 (most right)


					dac_value=DAC_mapValue(Ppm_Val);
					char buffer[200];

					//sprintf(buffer, "i2c Value: %d ppm Value: %d\r\n", I2c_adcValue,Ppm_Val);
					//HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
					//HAL_Delay(2000);*/

				    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_value);
					displayNumber(First_I2cVal, 3,0 );
					HAL_Delay(2);
					displayNumber(Second_I2cVal, 2,0 );
					HAL_Delay(2);
					displayNumber(Thrid_I2cVal, 1,0 );
					HAL_Delay(2);



					 if ((Ppm_Val>= High_Alarmcounter)){
										displayNumber(0, 6,0 );//led2 alarm
										HAL_Delay(2);
								    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
									HAL_GPIO_WritePin(GPIOB , GPIO_PIN_0, GPIO_PIN_SET);
									}
					 else{
					 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
					 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
					 }

					 if((Sensor_Fail==1)||(I2c_adcValue<=16000)||(I2c_adcValue >= 32000)){

					displayNumber(0, 5,0 );//led1 fail
					HAL_Delay(2);
					HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);
					 }
					 else{
						 displayNumber(0, 7,0 );//led3 Noarmal
						 HAL_Delay(2);
					 }

		   }
	  switch (stage)
	  	   	  {
	  	   		  case PowerOnStage:
	  	   			  Power_on_dispaly();
	  	   			  break;
	  	   		  case MenuSelectionStage:
	  	   			  switch(Second_stage)
	  	   			  {
	  	   			With_TimerControl =1;
	  	   				  case 1:
	  	   					  Address_Menu();
	  	   				  break;
	  	   				  case 2:
	  	   					  zero_calibartionMenu();
	  	   				  break;
	  	   				  case 3:
	  	   					  gas_calibartionMenu();
	  	   				  break;
	  	   				  case 4:
	  	   					  Dac_calibartionMenu();
	  	   				  break;
	  	   				  case 5:
	  	   					  h_AlarmLimitMenu();
	  	   				  break;
	 					case 6:
	 					      AlarmLimitMenu();
	 					break;
	  	   				  case 7:
	  	   					  LifeLimitMenu();
	  	   				  break;
	  	   				  case 8:
	  	   					  Self_calibartionMenu();
	  	   				  break;
	  	   				  case 9:
	  	   					  Exit_Menu();
	  	   				  break;
	  	   				  default:
	  	   				   break;
	  	   			  }
	  	   	  		  break;
	  	   			 case SubMenuSelectionStage:

	  	   				break;
	  	   			 case Address_Setting:
	  	  					Thrid_Adresscounter =( (Adresscounter / 100) % 10 );   // prepare to display digit 2
	  	  					Second_Adresscounter =( (Adresscounter / 10) % 10 );   // prepare to display digit 3
	  	  					First_Adresscounter =(Adresscounter % 10);   // prepare to display digit 4 (most right)
	  	  					displayNumber(First_Adresscounter, 3,0 );
	  	  					HAL_Delay(2);
	  	  					displayNumber(Second_Adresscounter, 2,0 );
	  	  					HAL_Delay(2);
	  	  					displayNumber(Thrid_Adresscounter, 1,0 );
	  	  					HAL_Delay(2);
	  	  				With_TimerControl =1;

	  	   				break;

	  	   			case Zero_PointCalbiration:
	  	   				HAL_ADC_Start(&hadc1);
	  	   					  	  /* Poll for ADC conversion completion */
	  	   					  	      if (HAL_ADC_PollForConversion(&hadc1,HAL_MAX_DELAY) == HAL_OK)
	  	   					  	      {
	  	   					  	        /* Read ADC value */
	  	  								uint16_t adcValue = HAL_ADC_GetValue(&hadc1);
	  	   					  	    	//uint16_t adcValue = 2096;
	  	  								Fourth_AdcVal=(adcValue / 1000);   // prepare to display digit 1 (most left)
	  	  								Thrid_AdcVal =( (adcValue / 100) % 10 );   // prepare to display digit 2
	  	  								Second_AdcVal =( (adcValue / 10) % 10 );   // prepare to display digit 3
	  	  								First_AdcVal =(adcValue % 10);   // prepare to display digit 4 (most right)

	  	  								displayNumber(First_AdcVal, 3,0 );
	  	  								HAL_Delay(2);
	  	  								displayNumber(Second_AdcVal, 2,0 );
	  	  								HAL_Delay(2);
	  	  								displayNumber(Thrid_AdcVal, 1,0 );
	  	  								HAL_Delay(2);
	  	  								displayNumber(Fourth_AdcVal, 0,0 );
	  	  								HAL_Delay(2);
	  	  								char buffer[20];
	  	  								//sprintf(buffer, "ADC Value: %d\r\n", adcValue);
	  	  								//HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
	  	   					  	        /* Process the ADC value as needed */
	  	   					  	      }
	  	   					  	      HAL_ADC_Stop(&hadc1);
	  	   					  	With_TimerControl =1;

	  	   				break;
	  	   			case Gas_Calibration:
	  	   				break;
	  	   			case mA_Calibration:
	  	   				break;
	  	   			case h_Alarm_Setting:
	 					h_Thrid_Alarmcounter =( (High_Alarmcounter / 100) % 10 );   // prepare to display digit 2
	 					h_Second_Alarmcounter =( (High_Alarmcounter / 10) % 10 );   // prepare to display digit 3
	 					h_First_Alarmcounter =(High_Alarmcounter % 10);   // prepare to display digit 4 (most right)
	 					displayNumber(h_First_Alarmcounter, 3,0 );
	 					HAL_Delay(2);
	 					displayNumber(h_Second_Alarmcounter, 2,0 );
	 					HAL_Delay(2);
	 					displayNumber(h_Thrid_Alarmcounter, 1,0 );
	 					HAL_Delay(2);
	 					With_TimerControl =1;

	  	   				break;
	  	   			case lw_Alarm_Setting:
	  	   				lw_Thrid_Alarmcounter =( (Low_Alarmcounter / 100) % 10 );   // prepare to display digit 2
	  	   				lw_Second_Alarmcounter =( (Low_Alarmcounter / 10) % 10 );   // prepare to display digit 3
	  	   				lw_First_Alarmcounter =(Low_Alarmcounter % 10);   // prepare to display digit 4 (most right)
	 					displayNumber(lw_First_Alarmcounter, 3,0 );
	 					HAL_Delay(2);
	 					displayNumber(lw_Second_Alarmcounter, 2,0 );
	 					HAL_Delay(2);
	 					displayNumber(lw_Thrid_Alarmcounter, 1,0 );
	 					HAL_Delay(2);
	 					With_TimerControl =1;
	  	   				break;
	  	   			case Life_limitSetting:
	  	   				break;
	  	   			case Self_calibration:
	  	   				break;
	  	   			case Exit_menu:

	  	   				break;
	  	   		case Run_App:

	  	   			  break;
	  	   		  default:
	  	   			  break;
	  	   	  }
    /* USER CODE BEGIN 3 */
#endif
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 4800-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 250;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA1 PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB13 PB14 PB15
                           PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB11 PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
 // __disable_irq();
  /*while (1)
  {
	  if(Sensor_Fail==1){
		  displayNumber(0, 5,0 );
		  HAL_Delay(2);
	  }
  }*/
  /* USER CODE END Error_Handler_Debug */
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
