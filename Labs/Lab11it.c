#include "main.h"
#include "stm32f4xx_it.h"

//External function calls

//Variables
extern uint8_t arrTrans[16];
extern uint8_t arrRec[16];
uint8_t counterTrans = 1;
uint8_t counterRec = 0;
extern CanTxMsg trans;
extern CanRxMsg rec;


void CAN1_TX_IRQHandler(void) {
  if(CAN_GetITStatus(CAN1, CAN_IT_TME) != RESET) {
    
      //Load new data word to send    
    trans.Data[0] = arrTrans[counterTrans];
    counterTrans = (counterTrans + 1) % 16;
        
    //Send data out through CAN
    CAN_Transmit(CAN1, &trans);
    
    CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
  }
}


void CAN1_RX0_IRQHandler(void) {
  if(CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET) {
  
    //Receive
    CAN_Receive(CAN1, CAN_FIFO0, &rec);
    
    if((rec.StdId == 0x11) && (rec.IDE == CAN_Id_Standard) && (rec.DLC == 1)) {
      arrRec[counterRec] = rec.Data[0];
      counterRec = (counterRec + 1) % 16;
    }
        
    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
  }
}