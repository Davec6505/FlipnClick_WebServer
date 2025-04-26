/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/*
 * Array to communicate the sd card mount error over uart.
 */
char const *SYS_FS_ERROR_S[24] = 
{
    /* Success */
    "SYS_FS_ERROR_OK",
    /* (1) A hard error occurred in the low level disk I/O layer */
    "SYS_FS_ERROR_DISK_ERR",
    /* (2) Assertion failed */
    "SYS_FS_ERROR_INT_ERR",
    /* (3) The physical drive cannot work */
    "SYS_FS_ERROR_NOT_READY",
    /* (4) Could not find the file */
    "SYS_FS_ERROR_NO_FILE",
    /* (5) Could not find the path */
    "SYS_FS_ERROR_NO_PATH",
    /* (6) The path name format is invalid */
    "SYS_FS_ERROR_INVALID_NAME",
    /* (7) Access denied due to prohibited access or directory full */
    "SYS_FS_ERROR_DENIED",
    /* (8) Access denied due to prohibited access */
    "SYS_FS_ERROR_EXIST",
    /* (9) The file/directory object is invalid */
    "SYS_FS_ERROR_INVALID_OBJECT",
    /* (10) The physical drive is write protected */
    "SYS_FS_ERROR_WRITE_PROTECTED",
    /* (11) The logical drive number is invalid */
    "SYS_FS_ERROR_INVALID_DRIVE",
    /* (12) The volume has no work area */
    "SYS_FS_ERROR_NOT_ENABLED",
    /* (13) There is no valid volume */
    "SYS_FS_ERROR_NO_FILESYSTEM",
    /* (14) The Format() aborted due to any parameter error */
    "SYS_FS_ERROR_FORMAT_ABORTED",
    /* (15) Could not get a grant to access the volume within defined period */
    "SYS_FS_ERROR_TIMEOUT",
    /* (16) The operation is rejected according to the file sharing policy */
    "SYS_FS_ERROR_LOCKED",
    /* (17) LFN working buffer could not be allocated */
    "SYS_FS_ERROR_NOT_ENOUGH_CORE",
    /* (18) Number of open files */
    "SYS_FS_ERROR_TOO_MANY_OPEN_FILES",
    /* (19) Given parameter is invalid */
    "SYS_FS_ERROR_INVALID_PARAMETER",
    /* (20) Too many mounts requested. Not enough free volume available */
    "SYS_FS_ERROR_NOT_ENOUGH_FREE_VOLUME",
    /* (21) Requested native file system is not supported */
    "SYS_FS_ERROR_FS_NOT_SUPPORTED",
    /* (22) Requested native file system does not match the format of volume */
    "SYS_FS_ERROR_FS_NOT_MATCH_WITH_VOLUME",
    /* (23) Function not supported in native file system layer */ 
    "SYS_FS_ERROR_NOT_SUPPORTED_IN_NATIVE_FS"
};


 const char* _fp_ = "protect/CONFIG.txt";
 const char* __fp__ = "protect/NETWORK.dat"; 
 const char* bmpfiles_ = "protect/BMP_FILES.txt"; 
// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;
uint8_t mountSdmmcFlag  = false;
uint8_t mountNVMFlag    = false;
APP_LED_STATE LEDstate = APP_LED_STATE_OFF;
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/
/*static void str_ip(char* str_,const char*str2){
    
    while(*str2 != '\r'){
        *(str_++) = *(str2++);
    }
    *str_ = '\0';
     SYS_CONSOLE_PRINT("%s \r\n",str_);
    
}
*/
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_MOUNT_DISK;
    
    /* Place the application state machine in its initial state. */
    //appData.state = APP_MOUNT_DISK;
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Tasks ( void )
{
    SYS_STATUS          tcpipStat;
    TCPIP_NET_HANDLE    netH;
    int                 nNets;
    static IPV4_ADDR    dwLastIP[2] = { {-1}, {-1} };
    IPV4_ADDR           ipAddr;
    int                 i;
    const char          *netName, *netBiosName;
    static uint32_t     startTick = 0;
    static uint8_t      mount_error_count = 0;
    SYS_FS_ERROR        err_;
    
   // static int8_t       init_ip_display;
   // TCPIP_MAC_ADDR newMACAddr;
    IPV4_ADDR ip_address,subnet_mask,gw_address;
    SYS_FS_HANDLE _handle;
    char str_val[50]={0};
    char buffer[7][50];
  //  char bmp_file_buffer[20][50];
    
    if((mountNVMFlag == false ) || (mountSdmmcFlag == false))
    {
        if(mountSdmmcFlag == false)
        {
            if(SYS_FS_Mount(APP_SYS_FS_SD_VOL, APP_SYS_FS_SDCARD_MOUNT_POINT, APP_SYS_FS_SDCARD_TYPE, 0, NULL) == 0)
            {
                SYS_CONSOLE_PRINT("SYS_Initialize: The %s File System is mounted\r\n", APP_SYS_FS_SDCARD_TYPE_STRING);
                mountSdmmcFlag = true;
                appData.state = APP_READ_IP;
            }
            else{
                mount_error_count++;
                if(mount_error_count > 999)
                {
                    err_ = SYS_FS_Error();
                    SYS_CONSOLE_PRINT("%s\r\n",SYS_FS_ERROR_S[err_]);
                    mount_error_count = 0;
                }
            }
        }
        if(mountNVMFlag == false)
        {
            if(SYS_FS_Mount(APP_SYS_FS_NVM_VOL, APP_SYS_FS_NVM_MOUNT_POINT, APP_SYS_FS_NVM_TYPE, 0, NULL) == 0)
            {
                SYS_CONSOLE_PRINT("SYS_Initialize: The %s File System is mounted\r\n", APP_SYS_FS_TYPE_STRING);
                mountNVMFlag = true;
            }
        }
    }

    switch(appData.state)
    {
       case APP_MOUNT_DISK:
          /*  if(SYS_FS_Mount(APP_SYS_FS_NVM_VOL, APP_SYS_FS_MOUNT_POINT, APP_SYS_FS_TYPE, 0, NULL) == 0)
            {
                SYS_CONSOLE_PRINT("SYS_Initialize: The %s File System is mounted\r\n", APP_SYS_FS_TYPE_STRING);
                appData.state = APP_TCPIP_WAIT_INIT;//APP_MOUNT_SDISK;
            }
*/
            CORETIMER_DelayMs(100);
            break;
/*       case APP_MOUNT_SDISK:
            if(SYS_FS_Mount(APP_SYS_FS_SD_VOL, APP_SYS_FS_SDCARD_MOUNT_POINT, APP_SYS_FS_SDCARD_TYPE, 0, NULL) == 0)
            {
              SYS_CONSOLE_PRINT("SYS_Initialize: The %s File System is mounted\r\n",APP_SYS_FS_SDCARD_TYPE_STRING);              
              appData.state = APP_TCPIP_WAIT_INIT;
            }
            CORETIMER_DelayMs(200);
            break;*/
        case APP_READ_IP:
   
            _handle = SYS_FS_FileOpen(_fp_,SYS_FS_FILE_OPEN_READ);
            if(_handle != SYS_FS_HANDLE_INVALID){
                int ii = 0;
                while(SYS_FS_FileStringGet(_handle,buffer[ii],50)== SYS_FS_RES_SUCCESS){
                    SYS_CONSOLE_PRINT("%s",buffer[ii++]);
                }
                SYS_FS_FileClose(_handle);

                memset(str_val,'\0',strlen(buffer[2]));
                strncpy(str_val,buffer[2],strlen(buffer[2])-2);
                bool okA = TCPIP_Helper_StringToIPAddress(str_val,&ip_address);
                
                memset(str_val,'\0',strlen(buffer[4]));
                strncpy(str_val,buffer[4],strlen(buffer[4])-2);
                bool okB = TCPIP_Helper_StringToIPAddress(str_val,&subnet_mask);
                
                netH = TCPIP_STACK_IndexToNet(0);
                if(okA & okB){
                  TCPIP_STACK_NetAddressSet (netH, &ip_address,&subnet_mask, true);
                }
                else{
                    SYS_CONSOLE_MESSAGE("Failed to convert IP or SUB");
                }
                
                memset(str_val,'\0',strlen(buffer[3]));
                strncpy(str_val,buffer[3],strlen(buffer[3])-2);
                if(TCPIP_Helper_StringToIPAddress(str_val,&gw_address)){
                   TCPIP_STACK_NetAddressGatewaySet(netH,&gw_address);
                }
                else{
                   SYS_CONSOLE_MESSAGE("Failed to convert GW");
                }
                      
            }else{
                SYS_CONSOLE_PRINT("SD handle error:= %lu \r\n",_handle);
            }
                           
            appData.state = APP_TCPIP_WAIT_INIT;
            break;
        case APP_TCPIP_WAIT_INIT:
            tcpipStat = TCPIP_STACK_Status(sysObj.tcpip);
            if(tcpipStat < 0)
            {   // some error occurred
                SYS_CONSOLE_MESSAGE("APP: TCP/IP stack initialization failed!\r\n");
                appData.state = APP_TCPIP_ERROR;
            }
            else if(tcpipStat == SYS_STATUS_READY)
            {
                // now that the stack is ready we can check the
                // available interfaces and register
                // a Bonjour serviceCo

                nNets = TCPIP_STACK_NumberOfNetworksGet();

                for(i = 0; i < nNets; i++)
                {
                    netH = TCPIP_STACK_IndexToNet(i);
                    netName = TCPIP_STACK_NetNameGet(netH);
                    netBiosName = TCPIP_STACK_NetBIOSName(netH);

#if defined(TCPIP_STACK_USE_NBNS)
                    SYS_CONSOLE_PRINT("    Interface %s on host %s - NBNS enabled\r\n", netName, netBiosName);
#else
                    SYS_CONSOLE_PRINT("    Interface %s on host %s - NBNS disabled\r\n", netName, netBiosName);
#endif // defined(TCPIP_STACK_USE_NBNS)
                    (void)netName;          // avoid compiler warning 
                    (void)netBiosName;      // if SYS_CONSOLE_PRINT is null macro

#if defined(TCPIP_STACK_USE_ZEROCONF_MDNS_SD)
                    // base name of the service Must not exceed 16 bytes long
                    // the last digit will be incremented by interface
                    char mDNSServiceName[] = "MyWebServiceNameX ";

                    mDNSServiceName[sizeof(mDNSServiceName) - 2] = '1' + i;
                    TCPIP_MDNS_ServiceRegister(netH
                            , mDNSServiceName                     // name of the service
                            ,"_http._tcp.local"                   // type of the service
                            ,80                                   // TCP or UDP port, at which this service is available
                            ,((const uint8_t *)"path=/index.htm") // TXT info
                            ,1                                    // auto rename the service when if needed
                            ,NULL                                 // no callback function
                            ,NULL);                               // no application context
#endif // defined(TCPIP_STACK_USE_ZEROCONF_MDNS_SD)
                }

#if defined(TCPIP_STACK_USE_HTTP_NET_SERVER)
                // register the application HTTP processing
                HTTP_APP_Initialize();
#endif // defined(TCPIP_STACK_USE_HTTP_NET_SERVER)

                appData.state = APP_TCPIP_TRANSACT;
            }

            break;

        case APP_TCPIP_TRANSACT:
            if(SYS_TMR_TickCountGet() - startTick >= SYS_TMR_TickCounterFrequencyGet()/2ul)
            {
                startTick = SYS_TMR_TickCountGet();
               // LEDstate ^= APP_LED_STATE_ON;
               /* if(LEDstate == 1)
                {
                    APP_LED_1StateSet();
                }
                else if(LEDstate == 0)
                {
                    APP_LED_1StateClear();
                }*/
            }

            // if the IP address of an interface has changed
            // display the new value on the system console
            nNets = TCPIP_STACK_NumberOfNetworksGet();

            for(i = 0; i < nNets; i++)
            {
                netH = TCPIP_STACK_IndexToNet(i);
                ipAddr.Val = TCPIP_STACK_NetAddress(netH);
                if(dwLastIP[i].Val != ipAddr.Val)
                {
                    dwLastIP[i].Val = ipAddr.Val;
                    SYS_CONSOLE_PRINT("%s IP Address: %d.%d.%d.%d \r\n",
                            TCPIP_STACK_NetNameGet(netH),
                            ipAddr.v[0], ipAddr.v[1], ipAddr.v[2], ipAddr.v[3]);
                }
            }

            break;

        default:
            break;
    }
}

/*******************************************************************************
 End of File
 */
